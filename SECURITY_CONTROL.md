# 보안 제어 구조 문서

## 개요

MMMaster는 복사본으로 실행할 수 없도록 보안 제어 구조를 구현했습니다. 계좌 정보, 로그인 정보 등은 암호화되어 `config.ini` 파일에 저장되며, 이 파일은 개발자가 직접 생성하여 배포해야 합니다.

## 보안 제어 방식

### 1. ConfigManager 클래스

**파일**: `ConfigManager.h`, `ConfigManager.cpp`

**기능**:
- `config.ini` 파일에서 암호화된 계좌 정보 읽기/쓰기
- 암호화/복호화 처리
- 설정 파일 검증 및 보안 제어

### 2. 설정 파일 구조

**파일**: `config.ini` (프로그램 실행 디렉토리에 위치)

**형식**:
```ini
[ACCOUNT]
ACCOUNT_INDEX=1
ACCOUNT_ID=<암호화된 계좌번호>
ACCOUNT_PW=<암호화된 계좌 비밀번호>
LOGIN_ID=<암호화된 로그인 ID>
LOGIN_PW=<암호화된 로그인 비밀번호>
VERIFY_PW=<암호화된 공인인증서 비밀번호>

[SECURITY]
VALIDATED=1
CREATED_DATE=2024-01-01 00:00:00
```

### 3. 보안 제어 로직

1. **프로그램 시작 시**:
   - `config.ini` 파일 존재 확인
   - 파일이 없으면 프로그램 종료 (보안 제어)
   - 파일 검증 실패 시 프로그램 종료

2. **계좌 정보 로드**:
   - `config.ini`에서 암호화된 정보 읽기
   - 복호화하여 사용
   - 복호화 실패 시 프로그램 종료

3. **설정 파일 생성**:
   - 개발자 전용 유틸리티로 생성
   - 암호화 키는 개발자가 관리
   - 각 배포본마다 고유한 설정 파일 생성 가능

## 설정 파일 생성 유틸리티

### ConfigGenerator.exe (현재 솔루션에 포함됨)

**기능**:
- 계좌 정보 입력 받기
- 암호화하여 `config.ini` 생성
- 암호화 키 관리

**사용 방법**:
```
ConfigGenerator.exe
> 계좌 인덱스: 1
> 계좌번호: 20080800101
> 계좌 비밀번호: ****
> 로그인 ID: userid
> 로그인 비밀번호: ****
> 공인인증서 비밀번호: ****
> 암호화 키: (개발자 전용)
> [생성] 버튼 클릭
```

## 통합 방법

### 1. ETrade_Order_SampleDlg에 통합

```cpp
// OnInitDialog 또는 OnBnClickedButtonConnect에서
CConfigManager configMgr;
CString strAccountIndex, strAccountID, strAccountPW;
CString strLoginID, strLoginPW, strVerifyPW;

// config.ini에서 계좌 정보 읽기
if(configMgr.ReadAccountInfo(strAccountIndex, strAccountID, strAccountPW, 
   strLoginID, strLoginPW, strVerifyPW) == 0) {
    // 설정 파일이 없거나 검증 실패
    AfxMessageBox(_T("설정 파일이 없거나 손상되었습니다. 프로그램을 종료합니다."));
    exit(1);
}

// 읽은 정보로 로그인
m_csID = strLoginID;
m_csIdPw = strLoginPW;
m_csPubVerify = strVerifyPW;
gscCount = strAccountID;
gscCountpw = strAccountPW;
gcsLogIndex = strAccountIndex;
```

### 2. DB 연동 (선택사항)

현재는 DB의 `cfg_tbl` 테이블에서도 계좌 정보를 읽고 있습니다. 
향후 `config.ini`를 우선적으로 사용하고, DB는 백업으로 사용할 수 있습니다.

## 보안 강화 방안

### 현재 구현 (기본)
- XOR 암호화 (간단한 암호화)
- 설정 파일 존재 확인
- 설정 파일 검증

### 향후 개선 (권장)
1. **Windows CryptoAPI 사용**:
   - AES-256 암호화
   - 더 강력한 보안

2. **하드웨어 바인딩**:
   - MAC 주소, 하드디스크 시리얼 등으로 바인딩
   - 특정 PC에서만 실행 가능

3. **라이선스 키**:
   - 설정 파일에 라이선스 키 포함
   - 서버에서 검증 (선택사항)

4. **디지털 서명**:
   - 설정 파일에 디지털 서명
   - 위조 방지

## 주의사항

1. **암호화 키 관리**:
   - 암호화 키는 개발자가 안전하게 관리해야 함
   - 키가 유출되면 보안 제어가 무력화됨

2. **설정 파일 배포**:
   - 각 사용자/환경마다 별도의 설정 파일 생성
   - 설정 파일은 안전하게 전달

3. **백업**:
   - 설정 파일 손실 시 복구 불가
   - 안전한 곳에 백업 보관

## 설정 파일 생성 스크립트 예시

PowerShell 스크립트로 설정 파일을 생성할 수 있습니다:

```powershell
# CreateConfig.ps1
# 사용법: .\CreateConfig.ps1 -AccountIndex "1" -AccountID "20080800101" ...

param(
    [string]$AccountIndex = "1",
    [string]$AccountID = "",
    [string]$AccountPW = "",
    [string]$LoginID = "",
    [string]$LoginPW = "",
    [string]$VerifyPW = "",
    [string]$EncryptionKey = "MMMaster2024SecureKey"
)

# 암호화 함수 (간단한 XOR)
function Encrypt-String {
    param([string]$PlainText, [string]$Key)
    # 실제 구현은 C++ 코드와 동일하게
    return $PlainText  # 예시
}

$configPath = Join-Path $PSScriptRoot "config.ini"

# INI 파일 생성
@"
[ACCOUNT]
ACCOUNT_INDEX=$AccountIndex
ACCOUNT_ID=$(Encrypt-String $AccountID $EncryptionKey)
ACCOUNT_PW=$(Encrypt-String $AccountPW $EncryptionKey)
LOGIN_ID=$(Encrypt-String $LoginID $EncryptionKey)
LOGIN_PW=$(Encrypt-String $LoginPW $EncryptionKey)
VERIFY_PW=$(Encrypt-String $VerifyPW $EncryptionKey)

[SECURITY]
VALIDATED=1
CREATED_DATE=$(Get-Date -Format "yyyy-MM-dd HH:mm:ss")
"@ | Out-File -FilePath $configPath -Encoding UTF8

Write-Host "Config file created: $configPath"
```

## 문제 해결

### 설정 파일이 없다는 에러
- 해결: 개발자에게 `config.ini` 파일 요청

### 복호화 실패
- 원인: 암호화 키 불일치 또는 파일 손상
- 해결: 올바른 설정 파일로 교체

### 검증 실패
- 원인: 설정 파일이 손상되었거나 위조됨
- 해결: 원본 설정 파일로 교체

