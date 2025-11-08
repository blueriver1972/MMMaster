# masterkey.ini 기반 빌드 시점 키 관리 가이드

## 개요

이제 **Visual Studio 프로젝트 속성을 수정할 필요 없이**, `masterkey.ini` 파일만 관리하면 됩니다!

### 개선된 워크플로우

1. **ConfigGenerator.exe 실행** → `config.ini` + `masterkey.ini` 생성
2. **MMMaster.exe 빌드** → Pre-Build Event가 `masterkey.ini`를 읽어 `MasterKey.h` 자동 생성
3. **빌드된 실행 파일** → 마스터 키가 포함됨

## 사용 방법

### Step 1: ConfigGenerator로 설정 파일 생성

```
ConfigGenerator.exe 실행
> 계좌 인덱스: 1
> 계좌번호: 20131719901
> 계좌 비밀번호: ****
> 로그인 ID: k8096
> 로그인 비밀번호: ****
> 공인인증서 비밀번호: ****
> 암호화 키: MyMasterKey123!
```

**생성되는 파일:**
- `config.ini` - 암호화된 계좌 정보
- `masterkey.ini` - 마스터 키 (빌드 시점에 사용)

**masterkey.ini 형식:**
```ini
[MASTER_KEY]
KEY=MyMasterKey123!
```

### Step 2: MMMaster.exe 빌드

Visual Studio에서 **빌드 > 솔루션 빌드**를 실행하면:

1. **Pre-Build Event 실행**:
   - `generate_masterkey_header.ps1` 스크립트 실행
   - `masterkey.ini` 파일 읽기
   - `MasterKey.h` 헤더 파일 생성

2. **컴파일**:
   - `MasterKey.h`에 정의된 `BUILD_ENCRYPTION_KEY` 사용
   - 실행 파일에 마스터 키 포함

**출력 예시:**
```
=== MasterKey Header Generator ===
Project Directory: D:\stock\ebest\20180327\MMMaster\
Master Key INI: D:\stock\ebest\20180327\MMMaster\masterkey.ini
Output Header: D:\stock\ebest\20180327\MMMaster\MasterKey.h

✓ masterkey.ini 파일을 찾았습니다.
✓ masterkey.ini에서 키를 읽었습니다 (길이: 16)
✓ MasterKey.h 파일이 생성되었습니다.
```

### Step 3: 키 변경 및 재빌드

키를 변경하려면:

1. **ConfigGenerator.exe 재실행**:
   - 새 키 입력
   - `masterkey.ini` 자동 업데이트

2. **MMMaster.exe 재빌드**:
   - Pre-Build Event가 새 키를 읽어 `MasterKey.h` 재생성
   - 새 키가 포함된 실행 파일 생성

**또는 직접 masterkey.ini 수정:**
```ini
[MASTER_KEY]
KEY=NewMasterKey456!
```
→ 재빌드하면 새 키가 자동으로 반영됨

## 파일 구조

```
프로젝트 루트/
├── ConfigGenerator.exe          # 설정 파일 생성 유틸리티
├── generate_masterkey_header.ps1 # 빌드 전처리 스크립트
├── masterkey.ini                 # 마스터 키 (ConfigGenerator가 생성)
├── MasterKey.h                   # 자동 생성된 헤더 (빌드 시 생성)
├── config.ini                    # 암호화된 계좌 정보
└── ETrade_Order_Sample.vcxproj  # Pre-Build Event 포함
```

## 암호화 키 로드 우선순위

MMMaster.exe 실행 시:

1. **빌드 시점 마스터 키** (MasterKey.h에서) ← **사용자 배포용**
2. **masterkey.ini 파일** (빌드 시점 키가 없을 때)
3. **환경 변수** `MMMASTER_ENCRYPTION_KEY` (개발 환경용)
4. **config.ini의 ENCRYPTION_KEY** (하위 호환성, 경고)
5. **기본값** `"MyMasterKey123!"` (하위 호환성)

## 장점

### 이전 방식 (프로젝트 속성 수정)
```
❌ Visual Studio 프로젝트 속성 수정 필요
❌ 키 변경 시 프로젝트 파일 수정 필요
❌ 여러 사용자/환경 관리 어려움
```

### 개선된 방식 (masterkey.ini)
```
✅ 프로젝트 속성 수정 불필요
✅ masterkey.ini만 수정하면 됨
✅ ConfigGenerator로 자동 생성
✅ 키 변경 후 재빌드만 하면 됨
✅ 여러 사용자/환경별로 다른 masterkey.ini 관리 가능
```

## 주의사항

### 1. masterkey.ini 파일 관리

- **개발 PC**: `masterkey.ini` 파일 보관
- **사용자 배포**: `masterkey.ini`는 배포하지 않음 (빌드 시점에만 필요)
- **보안**: `masterkey.ini`는 `.gitignore`에 추가 권장

### 2. MasterKey.h 파일

- **자동 생성 파일**: 수정하지 마세요!
- **빌드 시점에만 필요**: 소스 제어에 포함하지 않아도 됨
- **기본값**: `masterkey.ini`가 없으면 기본값 `"MyMasterKey123!"` 사용

### 3. 빌드 전처리 스크립트

- **PowerShell 실행 정책**: 스크립트 실행을 위해 `-ExecutionPolicy Bypass` 사용
- **오류 처리**: `masterkey.ini`가 없어도 빌드 계속 (기본값 사용)

## 문제 해결

### Q: MasterKey.h가 생성되지 않아요

**A:** Pre-Build Event가 실행되지 않았을 수 있습니다.
- Visual Studio에서 **빌드 > 솔루션 정리** 후 **빌드 > 솔루션 빌드**
- `generate_masterkey_header.ps1` 파일이 프로젝트 루트에 있는지 확인

### Q: masterkey.ini를 찾을 수 없다는 경고가 나와요

**A:** 정상입니다. 기본값을 사용합니다.
- ConfigGenerator.exe를 실행하여 `masterkey.ini` 생성
- 또는 프로젝트 루트에 직접 `masterkey.ini` 파일 생성

### Q: 키를 변경했는데 반영이 안 돼요

**A:** 재빌드가 필요합니다.
- **빌드 > 솔루션 정리**
- **빌드 > 솔루션 빌드**
- Pre-Build Event가 새 키를 읽어 `MasterKey.h` 재생성

## 요약

✅ **ConfigGenerator.exe 실행** → `masterkey.ini` 자동 생성
✅ **MMMaster.exe 빌드** → Pre-Build Event가 `masterkey.ini` 읽어 `MasterKey.h` 생성
✅ **키 변경** → `masterkey.ini` 수정 후 재빌드만 하면 됨
✅ **프로젝트 속성 수정 불필요** → 더 간단하고 직관적!

