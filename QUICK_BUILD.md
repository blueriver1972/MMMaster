# 빠른 빌드 가이드

## 현재 상황

✅ **Release 폴더에 이미 빌드된 MMMaster.exe가 있습니다.**

## 옵션 1: 기존 빌드 파일 사용 (빠른 방법)

이미 빌드된 실행 파일이 있으므로 바로 실행할 수 있습니다:

```powershell
cd D:\stock\ebest\20180327\MMMaster\Release
.\MMMaster.exe
```

**주의**: 이 파일은 Visual Studio 2008에서 빌드되었을 가능성이 있습니다.

---

## 옵션 2: Visual Studio에서 프로젝트 변환 후 빌드 (권장)

Visual Studio 2019/2022를 사용하려면 프로젝트를 변환해야 합니다.

### 단계별 가이드

#### 1단계: Visual Studio에서 프로젝트 열기

1. Visual Studio 2019 또는 2022를 실행합니다
2. **파일 > 열기 > 프로젝트/솔루션** 선택
3. `MMMaster.sln` 파일 선택

#### 2단계: 프로젝트 변환

Visual Studio가 자동으로 변환을 제안합니다:

```
이 프로젝트를 현재 버전의 Visual Studio로 변환하시겠습니까?
```

- **"예"** 클릭

#### 3단계: 플랫폼 도구 집합 변경

1. **솔루션 탐색기**에서 프로젝트 우클릭
2. **속성** 선택
3. **구성 속성 > 일반**에서:
   - **플랫폼 도구 집합**: **v143** (VS2022) 또는 **v142** (VS2019)로 변경
4. **확인** 클릭

#### 4단계: 빌드

- **빌드 > 솔루션 빌드** (Ctrl+Shift+B)
- 또는 상단 도구 모음에서:
  - **Configuration**: Release 선택
  - **Platform**: Win32 선택
  - **빌드** 버튼 클릭

#### 5단계: 빌드 확인

```
Release/MMMaster.exe 파일이 생성되거나 업데이트됨
```

---

## 옵션 3: 명령줄에서 빌드 (고급)

프로젝트를 변환한 후 명령줄에서 빌드:

```powershell
# Visual Studio Developer PowerShell에서
cd D:\stock\ebest\20180327\MMMaster

# MSBuild로 빌드
msbuild MMMaster.sln /t:Build /p:Configuration=Release /p:Platform=Win32
```

**참고**: 프로젝트가 `.vcxproj`로 변환되어 있어야 합니다.

---

## 빌드 오류 해결

### 오류 1: MFC 라이브러리를 찾을 수 없음

**해결**: Visual Studio 설치 관리자에서 "MFC and ATL support" 설치 확인

### 오류 2: MySQL 라이브러리 오류

**해결**: 
1. 프로젝트 속성 > **C/C++ > 일반 > 추가 포함 디렉토리**
   - MySQL include 경로 추가 (예: `C:\Program Files\MySQL\MySQL Connector C 8.0\include`)

2. 프로젝트 속성 > **링커 > 일반 > 추가 라이브러리 디렉토리**
   - MySQL lib 경로 추가 (예: `C:\Program Files\MySQL\MySQL Connector C 8.0\lib\vs14`)

3. 프로젝트 속성 > **링커 > 입력 > 추가 종속성**
   - `libmysql.lib` 추가

### 오류 3: DLL 파일을 찾을 수 없음

**해결**: 다음 DLL이 Release 폴더에 있는지 확인:
- xingAPI.dll
- XA_Common.dll
- XA_DataSet.dll
- XA_Session.dll
- libmysql.dll (MySQL 클라이언트)

---

## 빌드 성공 확인

빌드가 성공하면:

```
Release/MMMaster.exe 파일이 생성/업데이트됨
마지막 수정 시간이 현재 시간으로 변경됨
```

---

## 다음 단계

빌드가 완료되면:

1. [실행 환경 구성 가이드](setup_environment.md) 확인
2. 필수 DLL 파일 확인
3. 설정 파일 수정 (Login.ini 등)
4. 애플리케이션 실행 및 테스트

---

## 빠른 체크리스트

빌드 전 확인사항:

- [ ] Visual Studio 2019/2022 설치 (MFC 지원 포함)
- [ ] 프로젝트 변환 완료 (.vcxproj)
- [ ] 플랫폼 도구 집합 설정 (v142/v143)
- [ ] MySQL 라이브러리 경로 설정
- [ ] 필수 DLL 파일 확인

빌드 후 확인사항:

- [ ] MMMaster.exe 생성 확인
- [ ] 빌드 오류 없음
- [ ] DLL 파일 모두 존재

