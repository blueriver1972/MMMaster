# masterkey.ini 참조 위치 및 흐름

## 빌드 시점 참조 흐름

### 1. Pre-Build Event (빌드 전 실행)

**파일**: `ETrade_Order_Sample.vcxproj`

**위치**: 각 구성(Debug, Release 등)의 `<PreBuildEvent>` 섹션

```xml
<PreBuildEvent>
  <Command>powershell.exe -ExecutionPolicy Bypass -File "$(ProjectDir)generate_masterkey_header.ps1" -ProjectDir "$(ProjectDir)" -Configuration "$(Configuration)"</Command>
  <Message>MasterKey.h 생성 중 (masterkey.ini에서 읽기)...</Message>
</PreBuildEvent>
```

**실행 시점**: Visual Studio 빌드 시작 전
**참조 위치**: `$(ProjectDir)masterkey.ini` (프로젝트 루트 디렉토리)

---

### 2. PowerShell 스크립트 실행

**파일**: `generate_masterkey_header.ps1`

**위치**: 프로젝트 루트 디렉토리

**참조 코드**:
```powershell
$MasterKeyIni = Join-Path $ProjectDir "masterkey.ini"  # ← 여기서 참조
$OutputHeader = Join-Path $ProjectDir "MasterKey.h"

if (Test-Path $MasterKeyIni) {
    # masterkey.ini 파일 읽기
    $iniContent = Get-Content $MasterKeyIni -Raw -Encoding UTF8
    # [MASTER_KEY] 섹션의 KEY 값 추출
    if ($iniContent -match '(?m)^\s*\[MASTER_KEY\]\s*$\s*KEY\s*=\s*(.+?)(?:\s*$|\r?\n)') {
        $MasterKey = $matches[1].Trim()
    }
}
```

**참조 위치**: 
- 절대 경로: `D:\stock\ebest\20180327\MMMaster\masterkey.ini`
- 상대 경로: 프로젝트 루트의 `masterkey.ini`

---

### 3. MasterKey.h 생성

**생성 위치**: 프로젝트 루트 디렉토리 (`MasterKey.h`)

**생성 내용**:
```cpp
#pragma once

// 빌드 시점 마스터 키 (masterkey.ini에서 읽어옴)
#define BUILD_ENCRYPTION_KEY "TestMasterKey456!"
```

**참조**: `masterkey.ini`의 `[MASTER_KEY]` 섹션의 `KEY` 값

---

### 4. ConfigManager.h에서 포함

**파일**: `ConfigManager.h`

**위치**: 7번째 줄

```cpp
#include "MasterKey.h"  // ← 여기서 MasterKey.h 포함
```

**참조**: 프로젝트 루트의 `MasterKey.h` 파일

---

### 5. ConfigManager.cpp에서 사용

**파일**: `ConfigManager.cpp`

**위치**: `LoadEncryptionKey()` 함수 (38-56번째 줄)

```cpp
#ifdef BUILD_ENCRYPTION_KEY
    m_strEncryptionKey = MASTER_ENCRYPTION_KEY;  // ← 여기서 사용
    // MASTER_ENCRYPTION_KEY는 ConfigManager.h에서 정의됨
    // #define MASTER_ENCRYPTION_KEY _T(BUILD_ENCRYPTION_KEY)
    // BUILD_ENCRYPTION_KEY는 MasterKey.h에서 정의됨
    // #define BUILD_ENCRYPTION_KEY "TestMasterKey456!"
#endif
```

**참조 체인**:
1. `ConfigManager.cpp` → `MASTER_ENCRYPTION_KEY` 매크로 사용
2. `ConfigManager.h` → `MASTER_ENCRYPTION_KEY` 정의 (`_T(BUILD_ENCRYPTION_KEY)`)
3. `MasterKey.h` → `BUILD_ENCRYPTION_KEY` 정의 (`"TestMasterKey456!"`)
4. `masterkey.ini` → 실제 키 값 저장

---

## 전체 참조 흐름도

```
빌드 시작
    ↓
Pre-Build Event 실행
    ↓
generate_masterkey_header.ps1 실행
    ↓
masterkey.ini 읽기 (프로젝트 루트)
    ↓
MasterKey.h 생성 (프로젝트 루트)
    ↓
컴파일 시작
    ↓
ConfigManager.h 포함
    ↓
MasterKey.h 포함 (#include "MasterKey.h")
    ↓
BUILD_ENCRYPTION_KEY 매크로 정의 확인
    ↓
ConfigManager.cpp 컴파일
    ↓
LoadEncryptionKey() 함수에서 MASTER_ENCRYPTION_KEY 사용
    ↓
실행 시 암호화 키로 사용
```

---

## 파일 위치 요약

| 파일 | 위치 | 역할 |
|------|------|------|
| `masterkey.ini` | 프로젝트 루트 | 암호화 키 저장 |
| `generate_masterkey_header.ps1` | 프로젝트 루트 | 빌드 전처리 스크립트 |
| `MasterKey.h` | 프로젝트 루트 | 자동 생성된 헤더 (빌드 시 생성) |
| `ConfigManager.h` | 프로젝트 루트 | MasterKey.h 포함 |
| `ConfigManager.cpp` | 프로젝트 루트 | 키 사용 |
| `ETrade_Order_Sample.vcxproj` | 프로젝트 루트 | Pre-Build Event 정의 |

---

## 참조 경로 상세

### 1. Pre-Build Event에서
```
$(ProjectDir) = D:\stock\ebest\20180327\MMMaster\
masterkey.ini = D:\stock\ebest\20180327\MMMaster\masterkey.ini
```

### 2. PowerShell 스크립트에서
```powershell
$ProjectDir = "D:\stock\ebest\20180327\MMMaster\"
$MasterKeyIni = "D:\stock\ebest\20180327\MMMaster\masterkey.ini"
```

### 3. 컴파일 시
```cpp
#include "MasterKey.h"  // 프로젝트 루트에서 찾음
```

---

## 확인 방법

빌드 출력에서 다음 메시지를 확인할 수 있습니다:

```
1>MasterKey.h 생성 중 (masterkey.ini에서 읽기)...
1>=== MasterKey Header Generator ===
1>Project Directory: D:\stock\ebest\20180327\MMMaster\
1>Master Key INI: D:\stock\ebest\20180327\MMMaster\masterkey.ini
1>Output Header: D:\stock\ebest\20180327\MMMaster\MasterKey.h
1>✓ masterkey.ini 파일을 찾았습니다.
1>✓ masterkey.ini에서 키를 읽었습니다 (길이: 17)
1>✓ MasterKey.h 파일이 생성되었습니다.
```

이 메시지가 보이면 Pre-Build Event가 정상적으로 실행되고 `masterkey.ini`를 참조한 것입니다.

