# MMMaster 빌드 환경 가이드

## 🎯 목표

Visual Studio 2022와 CMake를 사용하여 빌드 가능한 환경으로 변환 완료.

## ✅ 완료된 작업

### 1. CMake 빌드 시스템
- ✅ `CMakeLists.txt` 생성 및 최적화
- ✅ VS 2022 호환성 확보
- ✅ MySQL 라이브러리 자동 탐지
- ✅ DLL 자동 복사 설정

### 2. Visual Studio 2022 지원
- ✅ CMake로 `.vcxproj` 자동 생성
- ✅ MFC 지원 설정
- ✅ PCH (Precompiled Header) 설정

### 3. Cursor AI 환경
- ✅ `.vscode/settings.json` - CMake 통합
- ✅ `.vscode/tasks.json` - 빌드 작업
- ✅ `.vscode/launch.json` - 디버깅 설정

### 4. 도구 및 스크립트
- ✅ `setup_vs2022.ps1` - 자동 설정 스크립트
- ✅ `analyze_unused_code.ps1` - 코드 분석 도구
- ✅ `BUILD_VS2022.md` - 상세 빌드 가이드
- ✅ `CODE_CLEANUP_GUIDE.md` - 코드 정리 가이드

---

## 🚀 빠른 시작

### 첫 빌드

```powershell
# 1. 자동 설정 (CMake 프로젝트 생성)
.\setup_vs2022.ps1

# 2. Visual Studio에서 열기
start build\MMMaster.sln

# 또는 Cursor AI에서
# Ctrl+Shift+P → "CMake: Configure"
# Ctrl+Shift+P → "CMake: Build"
```

### 수동 빌드

```powershell
# CMake 설정
cmake -S . -B build -G "Visual Studio 17 2022" -A Win32

# 빌드
cmake --build build --config Release
```

---

## 📁 새로 생성된 파일

```
MMMaster/
├── CMakeLists.txt              # CMake 빌드 설정 (개선됨)
├── setup_vs2022.ps1           # VS 2022 자동 설정
├── analyze_unused_code.ps1    # 코드 분석 도구
├── BUILD_VS2022.md           # 빌드 가이드
├── CODE_CLEANUP_GUIDE.md     # 코드 정리 가이드
├── README_BUILD.md           # 이 파일
├── .vscode/                  # Cursor/VS Code 설정
│   ├── settings.json
│   ├── tasks.json
│   └── launch.json
└── build/                    # 빌드 출력 (생성됨)
```

---

## 🔧 빌드 방법

### 방법 1: PowerShell 스크립트 (가장 쉬움)

```powershell
.\setup_vs2022.ps1
```

### 방법 2: Visual Studio 2022

1. Visual Studio 2022 실행
2. 파일 > 열기 > CMake...
3. `CMakeLists.txt` 선택
4. 빌드 > 솔루션 빌드

### 방법 3: Cursor AI

1. 프로젝트 열기
2. `Ctrl+Shift+P` → "CMake: Configure"
3. `Ctrl+Shift+P` → "CMake: Build"

### 방법 4: 명령줄

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
```

---

## 📋 코드 정리 작업

### 다음 단계

코드 정리는 점진적으로 진행하세요:

1. **분석 실행**
   ```powershell
   .\analyze_unused_code.ps1
   ```

2. **가이드 참조**
   - `CODE_CLEANUP_GUIDE.md` 파일 확인

3. **단계별 정리**
   - 명확히 사용하지 않는 파일부터 제거
   - 각 단계마다 빌드 테스트

### 제거하지 말아야 할 것

- ✅ 로그인 정보 (`StdAfx.h`의 정의들)
- ✅ MFC 매크로 코드
- ✅ 실제 사용 중인 TR 패킷 헤더

---

## 🐛 문제 해결

자세한 문제 해결은 `BUILD_VS2022.md`를 참조하세요.

**주요 문제:**

1. **CMake를 찾을 수 없음**
   - CMake 설치 확인
   - PATH 환경 변수 확인

2. **MySQL 라이브러리 오류**
   - MySQL Connector/C 설치
   - `MYSQL_DIR` 환경 변수 설정

3. **MFC 오류**
   - Visual Studio에서 "MFC and ATL support" 설치 확인

---

## 📚 추가 문서

- **BUILD_VS2022.md** - 상세 빌드 가이드
- **CODE_CLEANUP_GUIDE.md** - 코드 정리 가이드
- **setup_environment.md** - 환경 설정 (기존)
- **PROJECT_ANALYSIS_KR.md** - 프로젝트 분석 (기존)

---

## ✅ 체크리스트

빌드 환경 확인:

- [ ] Visual Studio 2022 설치 (MFC 지원 포함)
- [ ] CMake 3.20 이상 설치
- [ ] MySQL Connector/C 설치 또는 경로 설정
- [ ] `setup_vs2022.ps1` 실행 성공
- [ ] `build/MMMaster.sln` 생성 확인
- [ ] 빌드 성공 확인
- [ ] 실행 파일 생성 확인

---

## 🎉 완료!

이제 Visual Studio 2022와 CMake를 사용하여 프로젝트를 빌드할 수 있습니다!

다음 단계: 코드 정리 작업을 진행하세요.

