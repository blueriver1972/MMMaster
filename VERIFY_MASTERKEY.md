# masterkey.ini → MasterKey.h → 빌드 적용 확인 가이드

## 현재 상태 확인

### 1. masterkey.ini 확인
```ini
[MASTER_KEY]
KEY=MMMaster2024SecureKey
```
**키 길이**: 21자

### 2. MasterKey.h 확인
```cpp
#define BUILD_ENCRYPTION_KEY "MMMaster2024SecureKey"
```
**키 길이**: 21자

### 3. 문제점
디버그 로그에서 여전히 `TestMasterKey456!` (17자)가 나오고 있습니다.

## 원인 분석

### 가능한 원인들:

1. **빌드 시 Pre-Build Event가 실행되지 않음**
   - Visual Studio에서 빌드 출력 확인 필요
   - "MasterKey.h 생성 중..." 메시지 확인

2. **이전 빌드된 실행 파일 사용**
   - 재빌드 필요
   - 솔루션 정리 후 재빌드

3. **MasterKey.h 파일이 빌드 시점에 업데이트되지 않음**
   - Pre-Build Event 실행 확인
   - MasterKey.h 파일의 수정 시간 확인

## 확인 방법

### Step 1: Pre-Build Event 실행 확인

Visual Studio 빌드 출력에서 다음 메시지 확인:
```
1>MasterKey.h 생성 중 (masterkey.ini에서 읽기)...
1>=== MasterKey Header Generator ===
1>✓ masterkey.ini 파일을 찾았습니다.
1>✓ masterkey.ini에서 키를 읽었습니다 (길이: 21)
1>✓ MasterKey.h 파일이 생성되었습니다.
```

**이 메시지가 없으면**: Pre-Build Event가 실행되지 않은 것입니다.

### Step 2: MasterKey.h 파일 확인

빌드 후 `MasterKey.h` 파일을 열어서 확인:
```cpp
#define BUILD_ENCRYPTION_KEY "MMMaster2024SecureKey"  // ← 21자여야 함
```

### Step 3: 빌드 출력 디렉토리 확인

빌드된 실행 파일이 올바른 키를 사용하는지 확인:
- 디버그 로그에서 키 해시값 확인
- `MMMaster2024SecureKey`의 해시값 계산 및 비교

## 해결 방법

### 방법 1: 솔루션 정리 및 재빌드

1. Visual Studio에서 **빌드 > 솔루션 정리**
2. **빌드 > 솔루션 빌드**
3. 빌드 출력에서 Pre-Build Event 메시지 확인
4. 실행 후 디버그 로그에서 키 해시값 확인

### 방법 2: Pre-Build Event 수동 실행

PowerShell에서 직접 실행:
```powershell
powershell.exe -ExecutionPolicy Bypass -File "D:\stock\ebest\20180327\MMMaster\generate_masterkey_header.ps1" -ProjectDir "D:\stock\ebest\20180327\MMMaster\" -Configuration "Debug"
```

그 후 재빌드

### 방법 3: MasterKey.h 수동 확인

빌드 전에 `MasterKey.h` 파일을 열어서 확인:
- 키가 `MMMaster2024SecureKey`인지 확인
- 키 길이가 21자인지 확인

## 키 해시값 계산

`MMMaster2024SecureKey`의 해시값을 계산하여 디버그 로그와 비교:

```cpp
// C++ 코드에서 계산
DWORD dwKeyHash = 0;
CString strKey = _T("MMMaster2024SecureKey");
for(int i = 0; i < strKey.GetLength(); i++) {
    dwKeyHash = ((dwKeyHash << 5) + dwKeyHash) + strKey[i];
}
// 해시값 출력하여 디버그 로그와 비교
```

## 예상 결과

정상적으로 작동하면:
```
[DEBUG] ConfigManager::LoadEncryptionKey - Using build-time master key (length: 21, sample: MMMaster2...Key, hash: XXXXXXXX)
```

키 길이가 21자이고, 해시값이 일치해야 합니다.

