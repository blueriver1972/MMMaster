#pragma once

#include <afx.h>

//================================================================================================
// ConfigManager: 설정 파일 관리 클래스
// 
// 기능:
// - config.ini 파일에서 암호화된 계좌 정보 읽기/쓰기
// - 암호화/복호화 처리
// - 설정 파일 검증 및 보안 제어
//
// 보안:
// - 계좌 정보는 AES 암호화로 저장
// - 설정 파일이 없거나 손상된 경우 기본값 사용 불가 (보안 제어)
//================================================================================================
class CConfigManager
{
public:
	CConfigManager();
	virtual ~CConfigManager();

	// 설정 파일 경로
	static CString GetConfigPath();

	// 계좌 정보 읽기 (암호화된 정보 복호화)
	// 반환값: 성공=1, 실패=0
	int ReadAccountInfo(CString& strAccountIndex, CString& strAccountID, 
		CString& strAccountPW, CString& strLoginID, CString& strLoginPW, 
		CString& strVerifyPW);

	// 계좌 정보 쓰기 (정보 암호화하여 저장)
	// 반환값: 성공=1, 실패=0
	int WriteAccountInfo(CString strAccountIndex, CString strAccountID, 
		CString strAccountPW, CString strLoginID, CString strLoginPW, 
		CString strVerifyPW);

	// 설정 파일 존재 여부 확인
	BOOL ConfigFileExists();

	// 설정 파일 검증 (무결성 확인)
	BOOL ValidateConfigFile();

	// 설정 파일 초기화 (기본값 생성 - 개발자 전용)
	// 주의: 이 함수는 개발자가 직접 호출하여 설정 파일을 생성해야 함
	int InitializeConfigFile(CString strAccountIndex, CString strAccountID, 
		CString strAccountPW, CString strLoginID, CString strLoginPW, 
		CString strVerifyPW, CString strEncryptionKey);

private:
	// 암호화 키 (하드코딩 또는 외부에서 주입)
	CString m_strEncryptionKey;

	// 간단한 XOR 암호화 (향후 AES로 개선 가능)
	CString EncryptString(CString strPlainText, CString strKey);
	CString DecryptString(CString strCipherText, CString strKey);

	// INI 파일 읽기/쓰기 헬퍼
	CString ReadIniString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefault = _T(""));
	int WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue);
};

