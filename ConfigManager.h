#pragma once

#include <afx.h>

// MasterKey.h 포함 (빌드 전처리 스크립트에서 생성)
// masterkey.ini 파일이 있으면 그 키를 사용하고, 없으면 기본값 사용
#include "MasterKey.h"

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
//
// 빌드 시점 마스터 키 설정:
// - Visual Studio 프로젝트 속성에서 Preprocessor Definitions에 추가:
//   BUILD_ENCRYPTION_KEY="YourMasterKey123!"
// - 또는 컴파일러 옵션: /D "BUILD_ENCRYPTION_KEY=\"YourMasterKey123!\""
//================================================================================================

// 빌드 시점 마스터 키 (masterkey.ini에서 자동 생성)
// Pre-Build Event에서 generate_masterkey_header.ps1가 masterkey.ini를 읽어 MasterKey.h 생성
// MasterKey.h는 상단에서 이미 포함됨

#ifdef BUILD_ENCRYPTION_KEY
	#define MASTER_ENCRYPTION_KEY _T(BUILD_ENCRYPTION_KEY)
#else
	// 개발용 기본 키 (빌드 시점 키가 없을 때만 사용)
	// masterkey.ini 파일이 없거나 Pre-Build Event가 실행되지 않은 경우
	#define MASTER_ENCRYPTION_KEY _T("MyMasterKey123!")
#endif
class CConfigManager
{
public:
	CConfigManager();
	virtual ~CConfigManager();

	// 설정 파일 경로
	static CString GetConfigPath();
	
	// 마스터 키 파일 경로
	static CString GetMasterKeyPath();

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
	// 암호화 키 (config.ini에서 읽거나 기본값 사용)
	CString m_strEncryptionKey;
	
	// 암호화 키 로드 (config.ini에서 읽기)
	void LoadEncryptionKey();

	// 간단한 XOR 암호화 (향후 AES로 개선 가능)
	CString EncryptString(CString strPlainText, CString strKey);
	CString DecryptString(CString strCipherText, CString strKey);
	
	// 파일 해시 계산 (변조 감지용)
	CString CalculateFileHash();

	// INI 파일 읽기/쓰기 헬퍼
	CString ReadIniString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDefault = _T(""));
	int WriteIniString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszValue);
};

