// ConfigGenerator.cpp : config.ini 생성 유틸리티
// 
// 사용법: ConfigGenerator.exe를 실행하여 계좌 정보를 입력하면
//        프로그램 실행 디렉토리에 config.ini 파일이 생성됩니다.
//

#include "ConfigGenerator_stdafx.h"
#include "ConfigManager.h"
#include <iostream>
#include <string>
#include <conio.h>

#ifdef _WIN32
#include <windows.h>
#endif

// Windows에서 한글 입력을 위한 함수 (CString 사용)
CString GetPasswordInput(const char* prompt) {
    std::cout << prompt;
    CString password;
    char ch;
    
    // Windows에서 비밀번호 입력 (마스킹)
    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (!password.IsEmpty()) {
                password = password.Left(password.GetLength() - 1);
                std::cout << "\b \b";
            }
        } else if (ch >= 32 && ch < 127) {
            password += ch;
            std::cout << '*';
        }
    }
    std::cout << std::endl;
    
    return password;
}

CString GetStringInput(const char* prompt) {
    std::cout << prompt;
    CString input;
    char buffer[1024];
    std::cin.getline(buffer, sizeof(buffer));
    input = buffer;
    return input;
}

int main(int argc, char* argv[])
{
    std::cout << "========================================" << std::endl;
    std::cout << "  MMMaster Config Generator" << std::endl;
    std::cout << "  config.ini 파일 생성 유틸리티" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    // ConfigManager 인스턴스 생성
    CConfigManager configMgr;

    // 기존 config.ini 파일 확인
    if (configMgr.ConfigFileExists()) {
        std::cout << "[경고] 기존 config.ini 파일이 존재합니다." << std::endl;
        std::cout << "덮어쓰시겠습니까? (y/n): ";
        char ch = _getch();
        std::cout << std::endl;
        if (ch != 'y' && ch != 'Y') {
            std::cout << "취소되었습니다." << std::endl;
            return 0;
        }
    }

    // 계좌 정보 입력
    std::cout << std::endl;
    std::cout << "=== 계좌 정보 입력 ===" << std::endl;
    
    CString accountIndex = GetStringInput("계좌 인덱스 (기본값: 1): ");
    if (accountIndex.IsEmpty()) accountIndex = "1";
    
    CString accountID = GetStringInput("계좌번호: ");
    if (accountID.IsEmpty()) {
        std::cout << "[에러] 계좌번호는 필수입니다." << std::endl;
        return 1;
    }
    
    CString accountPW = GetPasswordInput("계좌 비밀번호: ");
    if (accountPW.IsEmpty()) {
        std::cout << "[에러] 계좌 비밀번호는 필수입니다." << std::endl;
        return 1;
    }
    
    CString loginID = GetStringInput("로그인 ID: ");
    if (loginID.IsEmpty()) {
        std::cout << "[에러] 로그인 ID는 필수입니다." << std::endl;
        return 1;
    }
    
    CString loginPW = GetPasswordInput("로그인 비밀번호: ");
    if (loginPW.IsEmpty()) {
        std::cout << "[에러] 로그인 비밀번호는 필수입니다." << std::endl;
        return 1;
    }
    
    CString verifyPW = GetPasswordInput("공인인증서 비밀번호: ");
    if (verifyPW.IsEmpty()) {
        std::cout << "[에러] 공인인증서 비밀번호는 필수입니다." << std::endl;
        return 1;
    }

    // 암호화 키 입력 (선택사항)
    std::cout << std::endl;
    std::cout << "=== 암호화 설정 ===" << std::endl;
    CString encryptionKey = GetPasswordInput("암호화 키 (기본값 사용하려면 Enter): ");
    if (encryptionKey.IsEmpty()) {
        encryptionKey = "MMMaster2024SecureKey";  // 기본 키
    }

    // config.ini 파일 생성
    std::cout << std::endl;
    std::cout << "config.ini 파일을 생성하는 중..." << std::endl;
    
    CString strAccountIndex = accountIndex;
    CString strAccountID = accountID;
    CString strAccountPW = accountPW;
    CString strLoginID = loginID;
    CString strLoginPW = loginPW;
    CString strVerifyPW = verifyPW;
    CString strEncryptionKey = encryptionKey;

    int result = configMgr.InitializeConfigFile(
        strAccountIndex, strAccountID, strAccountPW,
        strLoginID, strLoginPW, strVerifyPW, strEncryptionKey
    );

    if (result == 1) {
        std::cout << std::endl;
        std::cout << "[성공] config.ini 파일이 생성되었습니다." << std::endl;
        std::cout << "위치: " << configMgr.GetConfigPath() << std::endl;
        std::cout << std::endl;
        std::cout << "주의: config.ini 파일을 안전하게 보관하세요." << std::endl;
        std::cout << "      이 파일이 없으면 프로그램이 실행되지 않을 수 있습니다." << std::endl;
    } else {
        std::cout << std::endl;
        std::cout << "[에러] config.ini 파일 생성에 실패했습니다." << std::endl;
        return 1;
    }

    std::cout << std::endl;
    std::cout << "아무 키나 누르면 종료합니다...";
    _getch();

    return 0;
}

