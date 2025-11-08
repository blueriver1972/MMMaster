// ConfigManager.cpp : 설정 파일 관리 구현
//

#ifdef _CONFIGGENERATOR_PROJECT
// ConfigGenerator 프로젝트에서는 precompiled header를 사용하지 않으므로 직접 포함
#define VC_EXTRALEAN
#define _AFXDLL
#include <afx.h>
#include <afxwin.h>
#include <afxdisp.h>  // COleDateTime을 위해 필요
#else
// MMMaster 프로젝트에서는 precompiled header 사용 (첫 번째 줄에 와야 함)
#include "stdafx.h"
#endif
#include "ConfigManager.h"

//================================================================================================
// ConfigManager 구현
// 
// 보안 제어 구조:
// 1. config.ini 파일이 없으면 프로그램 실행 불가 (보안 제어)
// 2. 설정 파일은 개발자가 직접 생성하여 배포
// 3. 암호화된 계좌 정보만 저장
// 4. 복사본으로는 실행 불가하도록 설정 파일 검증
//================================================================================================

CConfigManager::CConfigManager()
{
	// 암호화 키 로드: config.ini에서 읽거나 기본값 사용
	LoadEncryptionKey();
}

// 암호화 키 로드 (빌드 시점 키 우선, masterkey.ini, 환경 변수, 기본값 순)
void CConfigManager::LoadEncryptionKey()
{
	// 1순위: 빌드 시점 마스터 키 (masterkey.ini에서 자동 생성된 헤더 파일)
	// Pre-Build Event에서 masterkey.ini를 읽어 MasterKey.h 생성
#ifdef BUILD_ENCRYPTION_KEY
	m_strEncryptionKey = MASTER_ENCRYPTION_KEY;
	CString strDebugMsg1;
	// 키의 처음 10자와 마지막 3자만 출력 (보안)
	CString strKeySample;
	if(m_strEncryptionKey.GetLength() > 13) {
		strKeySample = m_strEncryptionKey.Left(10) + _T("...") + m_strEncryptionKey.Right(3);
	} else {
		strKeySample = m_strEncryptionKey;  // 짧으면 전체 출력
	}
	// 키의 해시값 계산 (키 확인용)
	DWORD dwKeyHash = 0;
	for(int i = 0; i < m_strEncryptionKey.GetLength(); i++) {
		dwKeyHash = ((dwKeyHash << 5) + dwKeyHash) + m_strEncryptionKey[i];
	}
	strDebugMsg1.Format(_T("[DEBUG] ConfigManager::LoadEncryptionKey - Using build-time master key (length: %d, sample: %s, hash: %08X)\n"), 
		m_strEncryptionKey.GetLength(), strKeySample, dwKeyHash);
	OutputDebugString(strDebugMsg1);
	return;
#endif
	
	// 2순위: masterkey.ini 파일에서 읽기 (빌드 시점 키가 없을 때)
	CString strMasterKeyPath = GetMasterKeyPath();
	TCHAR szKey[256] = {0};
	DWORD dwResult = GetPrivateProfileString(_T("MASTER_KEY"), _T("KEY"), _T(""), 
		szKey, sizeof(szKey) / sizeof(TCHAR), strMasterKeyPath);
	if(dwResult > 0) {
		m_strEncryptionKey = szKey;
		CString strDebugMsg;
		// 키의 처음 10자만 출력 (보안)
		CString strKeySample = m_strEncryptionKey.Left(min(10, m_strEncryptionKey.GetLength()));
		strDebugMsg.Format(_T("[DEBUG] ConfigManager::LoadEncryptionKey - Loaded encryption key from masterkey.ini (length: %d, sample: %s...)\n"), 
			m_strEncryptionKey.GetLength(), strKeySample);
		OutputDebugString(strDebugMsg);
		return;
	}
	
	// 3순위: 환경 변수에서 읽기 (개발 환경용)
	TCHAR szEnvKey[256] = {0};
	dwResult = GetEnvironmentVariable(_T("MMMASTER_ENCRYPTION_KEY"), szEnvKey, 256);
	if(dwResult > 0 && dwResult < 256) {
		m_strEncryptionKey = szEnvKey;
		CString strDebugMsg;
		strDebugMsg.Format(_T("[DEBUG] ConfigManager::LoadEncryptionKey - Loaded encryption key from environment variable (length: %d)\n"), 
			m_strEncryptionKey.GetLength());
		OutputDebugString(strDebugMsg);
		return;
	}
	
	// 4순위: config.ini에서 읽기 (하위 호환성, 보안 약함)
	// 주의: config.ini에 키가 있으면 파일 공유 시 복호화 가능하므로 권장하지 않음
	if(ConfigFileExists()) {
		CString strKey = ReadIniString(_T("SECURITY"), _T("ENCRYPTION_KEY"), _T(""));
		if(!strKey.IsEmpty()) {
			m_strEncryptionKey = strKey;
			CString strDebugMsg;
			strDebugMsg.Format(_T("[WARNING] ConfigManager::LoadEncryptionKey - Loaded encryption key from config.ini (INSECURE! Use masterkey.ini instead)\n"));
			OutputDebugString(strDebugMsg);
			return;
		}
	}
	
	// 5순위: 기본 암호화 키 (하위 호환성)
	// 주의: 이 키는 개발자가 관리하며, 설정 파일 생성 시 사용됨
	m_strEncryptionKey = _T("MyMasterKey123!");
	CString strDebugMsg;
	strDebugMsg.Format(_T("[DEBUG] ConfigManager::LoadEncryptionKey - Using default encryption key: %s\n"), m_strEncryptionKey);
	OutputDebugString(strDebugMsg);
}

CConfigManager::~CConfigManager()
{
}

// 설정 파일 경로 반환
CString CConfigManager::GetConfigPath()
{
	CString strPath;
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strModulePath = szPath;
	int nPos = strModulePath.ReverseFind('\\');
	if(nPos >= 0) {
		strPath = strModulePath.Left(nPos + 1);
	}
	strPath += _T("config.ini");
	return strPath;
}

// 마스터 키 파일 경로 반환
CString CConfigManager::GetMasterKeyPath()
{
	CString strPath;
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString strModulePath = szPath;
	int nPos = strModulePath.ReverseFind('\\');
	if(nPos >= 0) {
		strPath = strModulePath.Left(nPos + 1);
	}
	strPath += _T("masterkey.ini");
	return strPath;
}

// 설정 파일 존재 여부 확인
BOOL CConfigManager::ConfigFileExists()
{
	CString strPath = GetConfigPath();
	CFileStatus status;
	return CFile::GetStatus(strPath, status);
}

// 설정 파일 검증 (무결성 및 변조 감지)
BOOL CConfigManager::ValidateConfigFile()
{
	if(!ConfigFileExists()) {
		return FALSE;
	}

	// 기본 검증: VALIDATED 플래그 확인
	CString strValue = ReadIniString(_T("SECURITY"), _T("VALIDATED"), _T(""));
	if(strValue.IsEmpty()) {
		return FALSE;
	}

	// 파일 해시 검증 (변조 감지)
	CString strStoredHash = ReadIniString(_T("SECURITY"), _T("FILE_HASH"), _T(""));
	if(!strStoredHash.IsEmpty()) {
		CString strCalculatedHash = CalculateFileHash();
		if(strStoredHash != strCalculatedHash) {
			CString strDebugMsg;
			strDebugMsg.Format(_T("[ERROR] ConfigManager::ValidateConfigFile - File hash mismatch! File may have been tampered.\n"));
			strDebugMsg += _T("  Stored hash: ") + strStoredHash + _T("\n");
			strDebugMsg += _T("  Calculated hash: ") + strCalculatedHash + _T("\n");
			OutputDebugString(strDebugMsg);
			return FALSE;  // 파일이 변조됨
		}
		OutputDebugString(_T("[DEBUG] ConfigManager::ValidateConfigFile - File hash verified\n"));
	}

	return TRUE;
}

// 계좌 정보 읽기
int CConfigManager::ReadAccountInfo(CString& strAccountIndex, CString& strAccountID, 
	CString& strAccountPW, CString& strLoginID, CString& strLoginPW, CString& strVerifyPW)
{
	CString strDebugMsg;
	CString strConfigPath = GetConfigPath();
	
	strDebugMsg.Format(_T("[DEBUG] ConfigManager::ReadAccountInfo - Looking for config.ini at: %s\n"), strConfigPath);
	OutputDebugString(strDebugMsg);
	
	// 설정 파일 존재 확인
	if(!ConfigFileExists()) {
		strDebugMsg.Format(_T("[ERROR] ConfigManager::ReadAccountInfo - config.ini file not found at: %s\n"), strConfigPath);
		OutputDebugString(strDebugMsg);
		return 0;
	}
	
	strDebugMsg.Format(_T("[DEBUG] ConfigManager::ReadAccountInfo - config.ini file found at: %s\n"), strConfigPath);
	OutputDebugString(strDebugMsg);

	// 설정 파일 검증
	if(!ValidateConfigFile()) {
		OutputDebugString(_T("[ERROR] ConfigManager::ReadAccountInfo - config.ini validation failed\n"));
		return 0;
	}
	
	OutputDebugString(_T("[DEBUG] ConfigManager::ReadAccountInfo - config.ini validation passed\n"));

	// 암호화 키 확인 (해시값으로 확인)
	DWORD dwKeyHash = 0;
	for(int i = 0; i < m_strEncryptionKey.GetLength(); i++) {
		dwKeyHash = ((dwKeyHash << 5) + dwKeyHash) + m_strEncryptionKey[i];
	}
	CString strKeySample;
	if(m_strEncryptionKey.GetLength() > 13) {
		strKeySample = m_strEncryptionKey.Left(10) + _T("...") + m_strEncryptionKey.Right(3);
	} else {
		strKeySample = m_strEncryptionKey;
	}
	strDebugMsg.Format(_T("[DEBUG] ConfigManager::ReadAccountInfo - Using encryption key (length: %d, sample: %s, hash: %08X)\n"), 
		m_strEncryptionKey.GetLength(), strKeySample, dwKeyHash);
	OutputDebugString(strDebugMsg);
	
	// 암호화된 정보 읽기
	CString strEncryptedAccountID = ReadIniString(_T("ACCOUNT"), _T("ACCOUNT_ID"), _T(""));
	CString strEncryptedAccountPW = ReadIniString(_T("ACCOUNT"), _T("ACCOUNT_PW"), _T(""));
	CString strEncryptedLoginID = ReadIniString(_T("ACCOUNT"), _T("LOGIN_ID"), _T(""));
	CString strEncryptedLoginPW = ReadIniString(_T("ACCOUNT"), _T("LOGIN_PW"), _T(""));
	CString strEncryptedVerifyPW = ReadIniString(_T("ACCOUNT"), _T("VERIFY_PW"), _T(""));
	strAccountIndex = ReadIniString(_T("ACCOUNT"), _T("ACCOUNT_INDEX"), _T("1"));
	
	strDebugMsg.Format(_T("[DEBUG] ConfigManager::ReadAccountInfo - Encrypted data read - AccountIndex: %s, AccountID length: %d, LoginID length: %d\n"), 
		strAccountIndex, strEncryptedAccountID.GetLength(), strEncryptedLoginID.GetLength());
	OutputDebugString(strDebugMsg);
	
	// 암호화된 데이터 샘플 출력 (처음 20자만)
	if(strEncryptedAccountID.GetLength() > 0) {
		CString strSample = strEncryptedAccountID.Left(min(20, strEncryptedAccountID.GetLength()));
		strDebugMsg.Format(_T("[DEBUG] ConfigManager::ReadAccountInfo - Encrypted AccountID sample (first 20 chars): %s\n"), strSample);
		OutputDebugString(strDebugMsg);
	}

	// 복호화
	strAccountID = DecryptString(strEncryptedAccountID, m_strEncryptionKey);
	strAccountPW = DecryptString(strEncryptedAccountPW, m_strEncryptionKey);
	strLoginID = DecryptString(strEncryptedLoginID, m_strEncryptionKey);
	strLoginPW = DecryptString(strEncryptedLoginPW, m_strEncryptionKey);
	strVerifyPW = DecryptString(strEncryptedVerifyPW, m_strEncryptionKey);
	
	// 복호화된 결과를 안전하게 출력 (인쇄 가능한 문자만)
	CString strSafeAccountID, strSafeLoginID;
	for(int i = 0; i < strAccountID.GetLength(); i++) {
		TCHAR c = strAccountID[i];
		if(c >= 32 && c < 127) {
			strSafeAccountID += c;
		} else {
			strSafeAccountID += _T('?');
		}
	}
	for(int i = 0; i < strLoginID.GetLength(); i++) {
		TCHAR c = strLoginID[i];
		if(c >= 32 && c < 127) {
			strSafeLoginID += c;
		} else {
			strSafeLoginID += _T('?');
		}
	}
	
	strDebugMsg.Format(_T("[DEBUG] ConfigManager::ReadAccountInfo - Decrypted - AccountIndex: %s, AccountID: %s (length: %d), AccountPW length: %d, LoginID: %s, LoginPW length: %d, VerifyPW length: %d\n"), 
		strAccountIndex, strSafeAccountID, strAccountID.GetLength(), strAccountPW.GetLength(), strSafeLoginID, strLoginPW.GetLength(), strVerifyPW.GetLength());
	OutputDebugString(strDebugMsg);

	// 검증: 복호화된 정보가 비어있으면 실패
	if(strAccountID.IsEmpty() || strAccountPW.IsEmpty() || 
	   strLoginID.IsEmpty() || strLoginPW.IsEmpty()) {
		strDebugMsg.Format(_T("[ERROR] ConfigManager::ReadAccountInfo - Decryption failed or empty values - AccountID empty: %d, AccountPW empty: %d, LoginID empty: %d, LoginPW empty: %d\n"),
			strAccountID.IsEmpty(), strAccountPW.IsEmpty(), strLoginID.IsEmpty(), strLoginPW.IsEmpty());
		OutputDebugString(strDebugMsg);
		return 0;
	}
	
	OutputDebugString(_T("[DEBUG] ConfigManager::ReadAccountInfo - Successfully read and decrypted account info\n"));
	return 1;
}

// 계좌 정보 쓰기
int CConfigManager::WriteAccountInfo(CString strAccountIndex, CString strAccountID, 
	CString strAccountPW, CString strLoginID, CString strLoginPW, CString strVerifyPW)
{
	// 암호화
	CString strEncryptedAccountID = EncryptString(strAccountID, m_strEncryptionKey);
	CString strEncryptedAccountPW = EncryptString(strAccountPW, m_strEncryptionKey);
	CString strEncryptedLoginID = EncryptString(strLoginID, m_strEncryptionKey);
	CString strEncryptedLoginPW = EncryptString(strLoginPW, m_strEncryptionKey);
	CString strEncryptedVerifyPW = EncryptString(strVerifyPW, m_strEncryptionKey);

	// INI 파일에 쓰기
	WriteIniString(_T("ACCOUNT"), _T("ACCOUNT_INDEX"), strAccountIndex);
	WriteIniString(_T("ACCOUNT"), _T("ACCOUNT_ID"), strEncryptedAccountID);
	WriteIniString(_T("ACCOUNT"), _T("ACCOUNT_PW"), strEncryptedAccountPW);
	WriteIniString(_T("ACCOUNT"), _T("LOGIN_ID"), strEncryptedLoginID);
	WriteIniString(_T("ACCOUNT"), _T("LOGIN_PW"), strEncryptedLoginPW);
	WriteIniString(_T("ACCOUNT"), _T("VERIFY_PW"), strEncryptedVerifyPW);
	
	// 검증 플래그 설정
	WriteIniString(_T("SECURITY"), _T("VALIDATED"), _T("1"));
	CString strCreatedDate = COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"));
	WriteIniString(_T("SECURITY"), _T("CREATED_DATE"), (LPCTSTR)strCreatedDate);
	
	// 주의: 암호화 키는 config.ini에 저장하지 않음 (보안 강화)
	// 키는 빌드 시점에 포함되거나 환경 변수로 관리
	
	// 파일 해시 계산 및 저장 (변조 감지용)
	// 주의: 해시는 ACCOUNT 섹션의 내용을 기반으로 계산
	CString strFileHash = CalculateFileHash();
	WriteIniString(_T("SECURITY"), _T("FILE_HASH"), strFileHash);

	return 1;
}

// 설정 파일 초기화 (개발자 전용)
int CConfigManager::InitializeConfigFile(CString strAccountIndex, CString strAccountID, 
	CString strAccountPW, CString strLoginID, CString strLoginPW, 
	CString strVerifyPW, CString strEncryptionKey)
{
	// 암호화 키 설정
	if(!strEncryptionKey.IsEmpty()) {
		m_strEncryptionKey = strEncryptionKey;
	}
	
	// 암호화 키 해시 계산 (디버깅용)
	DWORD dwKeyHash = 0;
	for(int i = 0; i < m_strEncryptionKey.GetLength(); i++) {
		dwKeyHash = ((dwKeyHash << 5) + dwKeyHash) + m_strEncryptionKey[i];
	}
	CString strKeySample;
	if(m_strEncryptionKey.GetLength() > 13) {
		strKeySample = m_strEncryptionKey.Left(10) + _T("...") + m_strEncryptionKey.Right(3);
	} else {
		strKeySample = m_strEncryptionKey;
	}
	CString strDebugMsg;
	strDebugMsg.Format(_T("[DEBUG] ConfigManager::InitializeConfigFile - Encryption key (length: %d, sample: %s, hash: %08X)\n"), 
		m_strEncryptionKey.GetLength(), strKeySample, dwKeyHash);
	OutputDebugString(strDebugMsg);

	// 계좌 정보 쓰기
	int nResult = WriteAccountInfo(strAccountIndex, strAccountID, strAccountPW, 
		strLoginID, strLoginPW, strVerifyPW);
	
	if(nResult == 1) {
		OutputDebugString(_T("[DEBUG] ConfigManager::InitializeConfigFile - Config file created/updated with Base64 encoding\n"));
	}
	
	return nResult;
}

// Base64 문자 확인
static inline bool IsBase64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

// Base64 인코딩 (간단한 구현)
static CString Base64Encode(const CString& strInput)
{
	const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	CStringA strInputA(strInput);
	const unsigned char* bytes_to_encode = (const unsigned char*)strInputA.GetString();
	int in_len = strInputA.GetLength();
	CStringA strRet;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for(i = 0; (i <4) ; i++)
				strRet += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i) {
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			strRet += base64_chars[char_array_4[j]];

		while((i++ < 3))
			strRet += '=';
	}

	return CString(strRet);
}

// Base64 디코딩
static CString Base64Decode(const CString& strInput)
{
	const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	CStringA strInputA(strInput);
	int in_len = strInputA.GetLength();
	int i = 0;
	int j = 0;
	int in = 0;
	unsigned char char_array_4[4], char_array_3[3];
	CStringA strRet;

	while (in_len-- && (strInputA[in] != '=') && IsBase64(strInputA[in])) {
		char_array_4[i++] = strInputA[in]; in++;
		if (i ==4) {
			for (i = 0; i <4; i++) {
				const char* pFound = strchr(base64_chars, char_array_4[i]);
				if(pFound != NULL) {
					char_array_4[i] = (unsigned char)(pFound - base64_chars);
				} else {
					char_array_4[i] = 0;
				}
			}

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				strRet += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++) {
			const char* pFound = strchr(base64_chars, char_array_4[j]);
			if(pFound != NULL) {
				char_array_4[j] = (unsigned char)(pFound - base64_chars);
			} else {
				char_array_4[j] = 0;
			}
		}

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) strRet += char_array_3[j];
	}

	return CString(strRet);
}

// 간단한 XOR 암호화 + Base64 인코딩 (NULL 문자 문제 해결)
CString CConfigManager::EncryptString(CString strPlainText, CString strKey)
{
	if(strPlainText.IsEmpty() || strKey.IsEmpty()) {
		return _T("");
	}

	CString strResult = strPlainText;
	int nKeyLen = strKey.GetLength();
	if(nKeyLen == 0) return _T("");

	// XOR 암호화
	for(int i = 0; i < strResult.GetLength(); i++) {
		strResult.SetAt(i, strResult[i] ^ strKey[i % nKeyLen]);
	}

	// Base64 인코딩 (NULL 문자 문제 해결)
	// INI 파일에 저장할 때 NULL 문자가 포함되면 GetPrivateProfileString이 문자열을 잘라버림
	return Base64Encode(strResult);
}

CString CConfigManager::DecryptString(CString strCipherText, CString strKey)
{
	if(strCipherText.IsEmpty() || strKey.IsEmpty()) {
		CString strDebugMsg;
		strDebugMsg.Format(_T("[DEBUG] ConfigManager::DecryptString - Empty input (cipher length: %d, key length: %d)\n"), 
			strCipherText.GetLength(), strKey.GetLength());
		OutputDebugString(strDebugMsg);
		return _T("");
	}

	// 하위 호환성: Base64 디코딩 시도
	// Base64 문자열은 특정 패턴을 가지므로, Base64로 보이면 디코딩 시도
	CString strDecoded;
	bool bIsBase64 = false;
	
	// Base64 문자열 체크 (길이가 4의 배수이고, Base64 문자만 포함)
	if(strCipherText.GetLength() % 4 == 0 && strCipherText.GetLength() > 0) {
		bool bAllBase64 = true;
		for(int i = 0; i < strCipherText.GetLength(); i++) {
			unsigned char c = (unsigned char)strCipherText[i];
			if(!IsBase64(c) && c != '=') {
				bAllBase64 = false;
				break;
			}
		}
		
		if(bAllBase64) {
			// Base64 디코딩 시도
			strDecoded = Base64Decode(strCipherText);
			bIsBase64 = true;
			CString strDebugMsg;
			strDebugMsg.Format(_T("[DEBUG] ConfigManager::DecryptString - Base64 decoded (input length: %d, decoded length: %d)\n"), 
				strCipherText.GetLength(), strDecoded.GetLength());
			OutputDebugString(strDebugMsg);
		} else {
			CString strDebugMsg;
			strDebugMsg.Format(_T("[DEBUG] ConfigManager::DecryptString - Not Base64 format (length: %d, not multiple of 4 or contains non-Base64 chars)\n"), 
				strCipherText.GetLength());
			OutputDebugString(strDebugMsg);
		}
	} else {
		CString strDebugMsg;
		strDebugMsg.Format(_T("[DEBUG] ConfigManager::DecryptString - Not Base64 format (length: %d, not multiple of 4)\n"), 
			strCipherText.GetLength());
		OutputDebugString(strDebugMsg);
	}
	
	// Base64가 아니거나 디코딩 실패 시 기존 방식 (XOR만) 사용
	if(!bIsBase64 || strDecoded.IsEmpty()) {
		strDecoded = strCipherText;  // 원본 그대로 사용
		CString strDebugMsg;
		strDebugMsg.Format(_T("[DEBUG] ConfigManager::DecryptString - Using raw ciphertext (length: %d)\n"), strDecoded.GetLength());
		OutputDebugString(strDebugMsg);
	}
	
	// XOR 복호화 (암호화와 동일)
	CString strResult = strDecoded;
	int nKeyLen = strKey.GetLength();
	if(nKeyLen == 0) {
		OutputDebugString(_T("[ERROR] ConfigManager::DecryptString - Empty encryption key!\n"));
		return _T("");
	}

	for(int i = 0; i < strResult.GetLength(); i++) {
		strResult.SetAt(i, strResult[i] ^ strKey[i % nKeyLen]);
	}
	
	CString strDebugMsg;
	strDebugMsg.Format(_T("[DEBUG] ConfigManager::DecryptString - XOR decryption completed (result length: %d, key length: %d)\n"), 
		strResult.GetLength(), nKeyLen);
	OutputDebugString(strDebugMsg);

	return strResult;
}

// 파일 해시 계산 (변조 감지용 - 간단한 체크섬)
CString CConfigManager::CalculateFileHash()
{
	// ACCOUNT 섹션의 모든 값을 읽어서 해시 계산
	// 실제로는 더 강력한 해시 알고리즘(SHA256 등) 사용 권장
	CString strHashSource;
	
	strHashSource += ReadIniString(_T("ACCOUNT"), _T("ACCOUNT_INDEX"), _T(""));
	strHashSource += ReadIniString(_T("ACCOUNT"), _T("ACCOUNT_ID"), _T(""));
	strHashSource += ReadIniString(_T("ACCOUNT"), _T("ACCOUNT_PW"), _T(""));
	strHashSource += ReadIniString(_T("ACCOUNT"), _T("LOGIN_ID"), _T(""));
	strHashSource += ReadIniString(_T("ACCOUNT"), _T("LOGIN_PW"), _T(""));
	strHashSource += ReadIniString(_T("ACCOUNT"), _T("VERIFY_PW"), _T(""));
	
	// 간단한 해시 계산 (실제로는 SHA256 등 사용 권장)
	DWORD dwHash = 0;
	for(int i = 0; i < strHashSource.GetLength(); i++) {
		dwHash = ((dwHash << 5) + dwHash) + strHashSource[i];
	}
	
	CString strHash;
	strHash.Format(_T("%08X"), dwHash);
	return strHash;
}

// INI 파일 읽기
CString CConfigManager::ReadIniString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefault)
{
	CString strPath = GetConfigPath();
	TCHAR szValue[1024];
	DWORD dwResult = GetPrivateProfileString(lpszSection, lpszKey, lpszDefault, 
		szValue, sizeof(szValue) / sizeof(TCHAR), strPath);
	return CString(szValue);
}

// INI 파일 쓰기
int CConfigManager::WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue)
{
	CString strPath = GetConfigPath();
	return WritePrivateProfileString(lpszSection, lpszKey, lpszValue, strPath);
}

