# 빌드 시점 키 포함 방식에 대한 설명

## 현재 방식: Pre-Build Event로 헤더 파일 생성

### 왜 이 방식을 사용하는가?

1. **빌드 시점 키 포함의 필요성**
   - 사용자 배포 시나리오: `config.ini` + `MMMaster.exe`만 배포
   - `config.ini`의 계좌 정보를 다른 사람이 변경하지 못하게 막기 위해
   - 암호화 키가 실행 파일에 포함되어야 함

2. **다른 방법들과의 비교**

   #### 방법 1: 환경 변수 사용 (❌ 배포 시 문제)
   - 개발 환경에서는 편리
   - 사용자 PC에 환경 변수 설정 필요 → 복잡함
   - 환경 변수 노출 위험

   #### 방법 2: 별도 키 파일 (❌ 보안 약함)
   - `masterkey.key` 파일을 함께 배포
   - 키 파일이 노출되면 암호화 의미 없음
   - 파일 삭제/변조 가능

   #### 방법 3: 프로젝트 속성에 직접 정의 (❌ 유연성 부족)
   - Visual Studio 프로젝트 속성에서 `BUILD_ENCRYPTION_KEY` 정의
   - 키 변경 시 프로젝트 파일 수정 필요
   - 여러 개발자가 다른 키 사용 시 충돌

   #### 방법 4: Pre-Build Event로 헤더 생성 (✅ 현재 방식)
   - `masterkey.ini` 파일에서 키 읽기
   - 빌드 시점에 `MasterKey.h` 자동 생성
   - 키 변경 시 `masterkey.ini`만 수정하고 재빌드
   - 실행 파일에 키가 포함됨

3. **보안 고려사항**

   ### 장점:
   - ✅ 키가 실행 파일에 포함되어 별도 파일 불필요
   - ✅ `config.ini`만으로는 복호화 불가능
   - ✅ 실행 파일과 `config.ini`가 모두 있어야 작동

   ### 한계:
   - ⚠️ 실행 파일을 역공학하면 키 추출 가능 (XOR 암호화는 약함)
   - ⚠️ 정적 분석 도구로 문자열 검색 시 키 노출 가능
   - ⚠️ 완벽한 보안은 아님 (하지만 일반 사용자 수준에서는 충분)

   ### 개선 방안 (향후):
   - 더 강력한 암호화 알고리즘 사용 (AES 등)
   - 키를 여러 부분으로 분할하여 저장
   - 난독화(Obfuscation) 도구 사용
   - 서버 기반 키 관리 (온라인 환경에서만 가능)

## 빌드 프로세스

```
1. Visual Studio 빌드 시작
   ↓
2. Pre-Build Event 실행
   ↓
3. generate_masterkey_header.ps1 실행
   ↓
4. masterkey.ini 파일 읽기
   ↓
5. MasterKey.h 생성 (#define BUILD_ENCRYPTION_KEY "...")
   ↓
6. 컴파일 시작
   ↓
7. ConfigManager.h에서 MasterKey.h 포함
   ↓
8. BUILD_ENCRYPTION_KEY 매크로 사용
   ↓
9. 실행 파일에 키 포함
```

## 사용 방법

### Step 1: ConfigGenerator로 config.ini 생성
```
ConfigGenerator.exe 실행
> 암호화 키 입력: MMMaster2024SecureKey
> config.ini 생성
> masterkey.ini 자동 생성
```

### Step 2: MMMaster 빌드
```
Visual Studio에서 빌드
> Pre-Build Event 자동 실행
> masterkey.ini 읽기
> MasterKey.h 생성
> 빌드 완료
```

### Step 3: 배포
```
MMMaster.exe + config.ini만 배포
> masterkey.ini는 배포하지 않음
> 실행 파일에 키가 포함되어 있음
```

## 키 변경 방법

1. `masterkey.ini` 파일 수정
2. `ConfigGenerator.exe`로 새로운 키로 `config.ini` 재생성
3. `MMMaster` 프로젝트 재빌드
4. 새로운 실행 파일 배포

## 주의사항

- ⚠️ `masterkey.ini`와 `config.ini`의 키가 일치해야 함
- ⚠️ 키 변경 후 반드시 재빌드 필요
- ⚠️ `MasterKey.h`는 자동 생성 파일이므로 수동 수정 금지
- ⚠️ 빌드 전에 `masterkey.ini`가 존재하는지 확인

