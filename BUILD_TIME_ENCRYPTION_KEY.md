# 빌드 시점 마스터 키 설정 가이드

## 개요

사용자 배포 시나리오에 맞춘 보안 구조:
- **개발 PC**: ConfigGenerator.exe로 config.ini 생성 (마스터 키 사용)
- **빌드 시점**: MMMaster.exe에 마스터 키 포함
- **사용자 PC**: config.ini + MMMaster.exe만 배포
- **목적**: config.ini의 계좌 정보를 다른 사람이 변경하지 못하게 막기

## 보안 구조

### 1. 암호화 키 로드 우선순위 (MMMaster.exe)

```
1순위: 빌드 시점 마스터 키 (BUILD_ENCRYPTION_KEY) ← 사용자 배포용
2순위: 환경 변수 MMMASTER_ENCRYPTION_KEY (개발 환경용)
3순위: config.ini의 ENCRYPTION_KEY (하위 호환성, 경고)
4순위: 기본값 "MMMaster2024SecureKey" (하위 호환성)
```

### 2. 파일 변조 감지

- config.ini 생성 시 파일 해시 계산 및 저장
- 실행 시 해시 검증하여 변조 감지
- 해시 불일치 시 프로그램 실행 불가

## 사용 방법

### Step 1: ConfigGenerator로 config.ini 생성

```
ConfigGenerator.exe 실행
> 계좌 인덱스: 1
> 계좌번호: 20131719901
> 계좌 비밀번호: ****
> 로그인 ID: k8096
> 로그인 비밀번호: ****
> 공인인증서 비밀번호: ****
> 암호화 키: MyMasterKey123!  ← 이 키를 기억하세요!
```

**출력 예시:**
```
중요: MMMaster.exe 빌드 시 이 암호화 키를 포함해야 합니다!
Visual Studio 프로젝트 속성에서 다음을 설정하세요:
  C/C++ > Preprocessor > Preprocessor Definitions에 추가:
  BUILD_ENCRYPTION_KEY="MyMasterKey123!"
```

### Step 2: Visual Studio에서 빌드 시점 키 설정

#### 방법 A: 프로젝트 속성에서 설정 (권장)

1. **Visual Studio에서 프로젝트 우클릭** → **속성**
2. **구성 속성** → **C/C++** → **전처리기** → **전처리기 정의**
3. **편집** 버튼 클릭
4. **새 줄 추가** 버튼 클릭
5. 다음 추가:
   ```
   BUILD_ENCRYPTION_KEY="MyMasterKey123!"
   ```
   (ConfigGenerator에서 사용한 키와 동일하게)
6. **확인** → **적용** → **확인**

#### 방법 B: .vcxproj 파일 직접 수정

`ETrade_Order_Sample.vcxproj` 파일에서 각 구성의 `<PreprocessorDefinitions>`에 추가:

```xml
<PreprocessorDefinitions>
  WIN32;
  _DEBUG;
  _WINDOWS;
  BUILD_ENCRYPTION_KEY="MyMasterKey123!";
  %(PreprocessorDefinitions)
</PreprocessorDefinitions>
```

### Step 3: MMMaster.exe 빌드

- Visual Studio에서 **빌드 > 솔루션 빌드**
- 빌드된 실행 파일에 마스터 키가 포함됨

### Step 4: 사용자 배포

배포 파일:
- `MMMaster.exe` (빌드 시점 키 포함)
- `config.ini` (ConfigGenerator로 생성)
- 필요한 DLL 파일들

**중요**: 사용자 PC에는 환경 변수 설정 불필요!

## 보안 효과

### 이전 구조 (문제)
```
config.ini 공유 → 키 + 암호문 모두 공유 → 복호화 가능 ❌
```

### 개선된 구조 (해결)
```
config.ini 공유 → 암호문만 공유 → 키 없음 → 복호화 불가 ✅
MMMaster.exe에 키 포함 → 실행 파일과 config.ini가 쌍으로 동작
파일 해시 검증 → config.ini 변조 감지 → 실행 불가 ✅
```

## 파일 해시 검증

### 동작 방식

1. **ConfigGenerator에서 config.ini 생성 시**:
   - ACCOUNT 섹션의 모든 값으로 해시 계산
   - `[SECURITY]` 섹션에 `FILE_HASH` 저장

2. **MMMaster.exe 실행 시**:
   - ACCOUNT 섹션 값으로 해시 재계산
   - 저장된 해시와 비교
   - 불일치 시 → 파일 변조 감지 → 프로그램 종료

### 해시 예시

```ini
[ACCOUNT]
ACCOUNT_INDEX=1
ACCOUNT_ID=<암호화된 계좌번호>
...

[SECURITY]
VALIDATED=1
CREATED_DATE=2024-01-01 00:00:00
FILE_HASH=A1B2C3D4  ← 변조 감지용 해시
```

## 주의사항

### 1. 키 관리

- **ConfigGenerator에서 사용한 키** = **빌드 시점에 포함할 키** (동일해야 함)
- 키가 다르면 복호화 실패
- 각 사용자마다 다른 키 사용 가능

### 2. 빌드 시점 키 변경

키를 변경하려면:
1. ConfigGenerator로 새 config.ini 생성 (새 키 사용)
2. Visual Studio 프로젝트 속성에서 `BUILD_ENCRYPTION_KEY` 업데이트
3. MMMaster.exe 재빌드
4. 새 config.ini와 새 MMMaster.exe를 쌍으로 배포

### 3. 파일 해시

- config.ini의 ACCOUNT 섹션을 수정하면 해시가 변경됨
- 해시 불일치 시 프로그램 실행 불가
- **의도**: 다른 사람이 계좌 정보를 변경하지 못하게 막기

## 추가 보안 강화 옵션

### 옵션 1: 하드웨어 바인딩 (추가 구현 가능)

특정 PC에서만 실행되도록:
- MAC 주소, 하드디스크 시리얼 등으로 키 생성
- PC 변경 시 재설정 필요

### 옵션 2: 더 강력한 해시 알고리즘

현재: 간단한 체크섬
향후: SHA256 등 사용 가능

## 요약

✅ **빌드 시점 마스터 키**: MMMaster.exe에 키 포함
✅ **파일 해시 검증**: config.ini 변조 감지
✅ **키 분리**: config.ini에 키 저장 안 함
✅ **사용자 배포**: config.ini + MMMaster.exe만 배포

이 구조로 다른 사람이 config.ini를 변경해도 실행할 수 없습니다!

