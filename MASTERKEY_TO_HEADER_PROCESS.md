# masterkey.ini → MasterKey.h 변환 과정

## 전체 과정

### Step 1: masterkey.ini 파일 읽기

**파일**: `masterkey.ini`
```ini
[MASTER_KEY]
KEY=TestMasterKey456!
```

**스크립트 위치**: `generate_masterkey_header.ps1` (22-45번째 줄)

```powershell
# masterkey.ini 파일 읽기
$MasterKey = $DefaultKey  # 기본값: "MyMasterKey123!"

if (Test-Path $MasterKeyIni) {
    # INI 파일 전체 내용 읽기
    $iniContent = Get-Content $MasterKeyIni -Raw -Encoding UTF8
    
    # 정규식으로 [MASTER_KEY] 섹션의 KEY 값 추출
    if ($iniContent -match '(?m)^\s*\[MASTER_KEY\]\s*$\s*KEY\s*=\s*(.+?)(?:\s*$|\r?\n)') {
        $MasterKey = $matches[1].Trim()  # "TestMasterKey456!" 추출
    }
}
```

**결과**: `$MasterKey = "TestMasterKey456!"`

---

### Step 2: 특수문자 이스케이프 처리

**스크립트 위치**: `generate_masterkey_header.ps1` (52-54번째 줄)

```powershell
# 키에 특수문자가 있으면 이스케이프 처리
# 따옴표와 백슬래시를 이스케이프
$EscapedKey = $MasterKey -replace '\\', '\\\\' -replace '"', '\"'
```

**예시**:
- 입력: `TestMasterKey456!`
- 출력: `TestMasterKey456!` (특수문자 없음, 그대로)

**예시 (특수문자 포함)**:
- 입력: `MyKey"with"quotes`
- 출력: `MyKey\"with\"quotes` (따옴표 이스케이프)

---

### Step 3: C++ 헤더 파일 내용 생성

**스크립트 위치**: `generate_masterkey_header.ps1` (56-66번째 줄)

```powershell
# 헤더 파일 내용 생성 (Here-String 사용)
$HeaderContent = @"
// 자동 생성된 파일 - 수정하지 마세요!
// 이 파일은 빌드 전처리 스크립트(generate_masterkey_header.ps1)에 의해 자동 생성됩니다.
// masterkey.ini 파일을 수정한 후 프로젝트를 재빌드하면 자동으로 업데이트됩니다.

#pragma once

// 빌드 시점 마스터 키 (masterkey.ini에서 읽어옴)
#define BUILD_ENCRYPTION_KEY "$EscapedKey"
"@
```

**변수 치환**:
- `$EscapedKey`가 실제 키 값으로 대체됨
- 예: `"TestMasterKey456!"` → `#define BUILD_ENCRYPTION_KEY "TestMasterKey456!"`

**생성될 내용**:
```cpp
// 자동 생성된 파일 - 수정하지 마세요!
// 이 파일은 빌드 전처리 스크립트(generate_masterkey_header.ps1)에 의해 자동 생성됩니다.
// masterkey.ini 파일을 수정한 후 프로젝트를 재빌드하면 자동으로 업데이트됩니다.

#pragma once

// 빌드 시점 마스터 키 (masterkey.ini에서 읽어옴)
#define BUILD_ENCRYPTION_KEY "TestMasterKey456!"
```

---

### Step 4: MasterKey.h 파일 쓰기

**스크립트 위치**: `generate_masterkey_header.ps1` (68-81번째 줄)

```powershell
# UTF-8 BOM 없이 쓰기 (Visual Studio 호환)
$Utf8NoBomEncoding = New-Object System.Text.UTF8Encoding $false
[System.IO.File]::WriteAllText($OutputHeader, $HeaderContent, $Utf8NoBomEncoding)
```

**파일 경로**: `D:\stock\ebest\20180327\MMMaster\MasterKey.h`

**결과**: `MasterKey.h` 파일이 생성되거나 업데이트됨

---

## 전체 흐름 예시

### 입력: masterkey.ini
```ini
[MASTER_KEY]
KEY=TestMasterKey456!
```

### 처리 과정:
1. **파일 읽기**: `Get-Content masterkey.ini`
2. **정규식 매칭**: `[MASTER_KEY]` 섹션의 `KEY=TestMasterKey456!` 추출
3. **이스케이프**: 특수문자 처리 (이 경우 불필요)
4. **헤더 생성**: C++ 헤더 파일 내용 생성
5. **파일 쓰기**: `MasterKey.h` 파일 생성

### 출력: MasterKey.h
```cpp
#pragma once

// 빌드 시점 마스터 키 (masterkey.ini에서 읽어옴)
#define BUILD_ENCRYPTION_KEY "TestMasterKey456!"
```

---

## 정규식 패턴 설명

```powershell
$iniContent -match '(?m)^\s*\[MASTER_KEY\]\s*$\s*KEY\s*=\s*(.+?)(?:\s*$|\r?\n)'
```

**패턴 분석**:
- `(?m)`: 멀티라인 모드
- `^\s*`: 줄 시작, 공백 0개 이상
- `\[MASTER_KEY\]`: `[MASTER_KEY]` 리터럴
- `\s*$`: 공백 0개 이상, 줄 끝
- `\s*KEY\s*=\s*`: `KEY=` 앞뒤 공백 허용
- `(.+?)`: 키 값 캡처 (비탐욕적)
- `(?:\s*$|\r?\n)`: 줄 끝 또는 개행

**매칭 예시**:
```
[MASTER_KEY]
KEY=TestMasterKey456!
```
→ `$matches[1] = "TestMasterKey456!"`

---

## 실행 시점

**Pre-Build Event**에서 실행:
- Visual Studio 빌드 시작 전
- 모든 소스 파일 컴파일 전
- `MasterKey.h`가 먼저 생성되어야 함

**실행 명령**:
```xml
powershell.exe -ExecutionPolicy Bypass -File "$(ProjectDir)generate_masterkey_header.ps1" -ProjectDir "$(ProjectDir)" -Configuration "$(Configuration)"
```

---

## 확인 방법

빌드 출력에서 확인:
```
1>MasterKey.h 생성 중 (masterkey.ini에서 읽기)...
1>=== MasterKey Header Generator ===
1>Project Directory: D:\stock\ebest\20180327\MMMaster\
1>Master Key INI: D:\stock\ebest\20180327\MMMaster\masterkey.ini
1>Output Header: D:\stock\ebest\20180327\MMMaster\MasterKey.h
1>
1>✓ masterkey.ini 파일을 찾았습니다.
1>✓ masterkey.ini에서 키를 읽었습니다 (길이: 17)
1>
1>✓ MasterKey.h 파일이 생성되었습니다.
1>  위치: D:\stock\ebest\20180327\MMMaster\MasterKey.h
```

생성된 `MasterKey.h` 파일을 열어서 확인할 수도 있습니다.

