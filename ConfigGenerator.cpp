#include "ConfigGenerator_stdafx.h"

// ConfigGenerator.cpp : config.ini 생성 유틸리티
// 
// 사용법: ConfigGenerator.exe를 실행하여 계좌 정보를 입력하면
//        프로그램 실행 디렉토리에 config.ini 파일이 생성됩니다.
//

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
#ifdef _WIN32
    // Windows 콘솔에서 한글 출력을 위한 코드 페이지 설정
    // UTF-8 사용 (소스 파일도 UTF-8로 저장되어 있음)
    SetConsoleOutputCP(65001);  // UTF-8 코드 페이지
    SetConsoleCP(65001);
    
    // 콘솔 모드 설정 (한글 입력 지원)
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hInput, &dwMode)) {
        SetConsoleMode(hInput, dwMode | ENABLE_PROCESSED_INPUT);
    }
    
    // 콘솔 폰트를 한글을 지원하는 폰트로 변경
    // Windows 10 이상에서는 자동으로 지원되지만, 명시적으로 설정
    CONSOLE_FONT_INFOEX cfi = { 0 };
    cfi.cbSize = sizeof(cfi);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetCurrentConsoleFontEx(hConsole, FALSE, &cfi)) {
        // 한글을 지원하는 폰트로 변경 (Consolas, Malgun Gothic 등)
        wcscpy_s(cfi.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(hConsole, FALSE, &cfi);
    }
#endif

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

    // 암호화 키 해시 계산 (디버깅용)
    DWORD dwKeyHash = 0;
    for(int i = 0; i < strlen(encryptionKey); i++) {
        dwKeyHash = ((dwKeyHash << 5) + dwKeyHash) + encryptionKey[i];
    }
    CString strKeySample;
    int nKeyLen = strlen(encryptionKey);
    if(nKeyLen > 13) {
        CString strKey = encryptionKey;
        strKeySample = strKey.Left(10) + _T("...") + strKey.Right(3);
    } else {
        strKeySample = encryptionKey;
    }
    std::cout << "[DEBUG] ConfigGenerator - Encryption key info:" << std::endl;
    std::cout << "  Length: " << nKeyLen << std::endl;
    std::cout << "  Sample: " << (LPCTSTR)strKeySample << std::endl;
    std::cout << "  Hash: " << std::hex << std::uppercase << dwKeyHash << std::dec << std::endl;
    std::cout << std::endl;
    
    int result = configMgr.InitializeConfigFile(
        strAccountIndex, strAccountID, strAccountPW,
        strLoginID, strLoginPW, strVerifyPW, strEncryptionKey
    );

    if (result == 1) {
        std::cout << std::endl;
        std::cout << "[성공] config.ini 파일이 생성되었습니다." << std::endl;
        std::cout << "위치: " << configMgr.GetConfigPath() << std::endl;
        std::cout << std::endl;
        
        // masterkey.ini 파일 생성
        CString strMasterKeyPath = configMgr.GetConfigPath();
        int nPos = strMasterKeyPath.ReverseFind('\\');
        if(nPos >= 0) {
            strMasterKeyPath = strMasterKeyPath.Left(nPos + 1);
        }
        strMasterKeyPath += _T("masterkey.ini");
        
        // masterkey.ini 파일에 키 저장
        CString strKeySection = _T("MASTER_KEY");
        CString strKeyName = _T("KEY");
        WritePrivateProfileString(strKeySection, strKeyName, strEncryptionKey, strMasterKeyPath);
        
        std::cout << "[성공] masterkey.ini 파일이 생성되었습니다." << std::endl;
        std::cout << "위치: " << strMasterKeyPath << std::endl;
        std::cout << std::endl;
        std::cout << "주의: config.ini 파일을 안전하게 보관하세요." << std::endl;
        std::cout << "      이 파일이 없으면 프로그램이 실행되지 않을 수 있습니다." << std::endl;
        std::cout << std::endl;
        std::cout << "=== 빌드 안내 ===" << std::endl;
        std::cout << "MMMaster.exe 빌드 시 masterkey.ini 파일을 자동으로 참조합니다." << std::endl;
        std::cout << std::endl;
        std::cout << "다음 단계:" << std::endl;
        std::cout << "1. Visual Studio에서 MMMaster 프로젝트를 빌드하세요." << std::endl;
        std::cout << "2. Pre-Build Event가 masterkey.ini를 읽어 MasterKey.h를 생성합니다." << std::endl;
        std::cout << "3. 빌드된 실행 파일에 암호화 키가 포함됩니다." << std::endl;
        std::cout << std::endl;
        std::cout << "주의: masterkey.ini 파일을 안전하게 보관하세요." << std::endl;
        std::cout << "      이 파일이 없으면 빌드 시 기본값을 사용합니다." << std::endl;
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

