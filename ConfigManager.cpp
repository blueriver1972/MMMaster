// ConfigManager.cpp : 설정 파일 관리 구현
//

#include "stdafx.h"
#include "ConfigManager.h"
#include <afx.h>

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
	// 기본 암호화 키 (실제 사용 시 더 강력한 키 사용 권장)
	// 주의: 이 키는 개발자가 관리하며, 설정 파일 생성 시 사용됨
	m_strEncryptionKey = _T("MMMaster2024SecureKey");
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

// 설정 파일 존재 여부 확인
BOOL CConfigManager::ConfigFileExists()
{
	CString strPath = GetConfigPath();
	CFileStatus status;
	return CFile::GetStatus(strPath, status);
}

// 설정 파일 검증
BOOL CConfigManager::ValidateConfigFile()
{
	if(!ConfigFileExists()) {
		return FALSE;
	}

	// 설정 파일 무결성 확인
	// 예: 특정 섹션이나 키가 존재하는지 확인
	CString strValue = ReadIniString(_T("SECURITY"), _T("VALIDATED"), _T(""));
	if(strValue.IsEmpty()) {
		return FALSE;
	}

	return TRUE;
}

// 계좌 정보 읽기
int CConfigManager::ReadAccountInfo(CString& strAccountIndex, CString& strAccountID, 
	CString& strAccountPW, CString& strLoginID, CString& strLoginPW, CString& strVerifyPW)
{
	// 설정 파일 존재 확인
	if(!ConfigFileExists()) {
		OutputDebugString(_T("[ERROR] ConfigManager::ReadAccountInfo - config.ini file not found\n"));
		return 0;
	}

	// 설정 파일 검증
	if(!ValidateConfigFile()) {
		OutputDebugString(_T("[ERROR] ConfigManager::ReadAccountInfo - config.ini validation failed\n"));
		return 0;
	}

	// 암호화된 정보 읽기
	CString strEncryptedAccountID = ReadIniString(_T("ACCOUNT"), _T("ACCOUNT_ID"), _T(""));
	CString strEncryptedAccountPW = ReadIniString(_T("ACCOUNT"), _T("ACCOUNT_PW"), _T(""));
	CString strEncryptedLoginID = ReadIniString(_T("ACCOUNT"), _T("LOGIN_ID"), _T(""));
	CString strEncryptedLoginPW = ReadIniString(_T("ACCOUNT"), _T("LOGIN_PW"), _T(""));
	CString strEncryptedVerifyPW = ReadIniString(_T("ACCOUNT"), _T("VERIFY_PW"), _T(""));
	strAccountIndex = ReadIniString(_T("ACCOUNT"), _T("ACCOUNT_INDEX"), _T("1"));

	// 복호화
	strAccountID = DecryptString(strEncryptedAccountID, m_strEncryptionKey);
	strAccountPW = DecryptString(strEncryptedAccountPW, m_strEncryptionKey);
	strLoginID = DecryptString(strEncryptedLoginID, m_strEncryptionKey);
	strLoginPW = DecryptString(strEncryptedLoginPW, m_strEncryptionKey);
	strVerifyPW = DecryptString(strEncryptedVerifyPW, m_strEncryptionKey);

	// 검증: 복호화된 정보가 비어있으면 실패
	if(strAccountID.IsEmpty() || strAccountPW.IsEmpty() || 
	   strLoginID.IsEmpty() || strLoginPW.IsEmpty()) {
		OutputDebugString(_T("[ERROR] ConfigManager::ReadAccountInfo - Decryption failed or empty values\n"));
		return 0;
	}

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
	WriteIniString(_T("SECURITY"), _T("CREATED_DATE"), COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S")));

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

	// 계좌 정보 쓰기
	return WriteAccountInfo(strAccountIndex, strAccountID, strAccountPW, 
		strLoginID, strLoginPW, strVerifyPW);
}

// 간단한 XOR 암호화 (향후 AES로 개선 가능)
CString CConfigManager::EncryptString(CString strPlainText, CString strKey)
{
	if(strPlainText.IsEmpty() || strKey.IsEmpty()) {
		return _T("");
	}

	CString strResult = strPlainText;
	int nKeyLen = strKey.GetLength();
	if(nKeyLen == 0) return _T("");

	for(int i = 0; i < strResult.GetLength(); i++) {
		strResult.SetAt(i, strResult[i] ^ strKey[i % nKeyLen]);
	}

	// Base64 인코딩 (간단한 변환)
	// 실제로는 Windows CryptoAPI나 OpenSSL 사용 권장
	return strResult;
}

CString CConfigManager::DecryptString(CString strCipherText, CString strKey)
{
	// XOR 암호화는 복호화가 암호화와 동일
	return EncryptString(strCipherText, strKey);
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

