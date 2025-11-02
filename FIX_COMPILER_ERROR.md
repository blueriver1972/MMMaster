# C++ 컴파일러 오류 해결 가이드

## 🔍 문제 분석

Visual Studio에서 CMake 프로젝트를 열었을 때 다음 오류 발생:

```
The C++ compiler is not able to compile a simple test program.
RC Pass 1: command "rc /fo ..." failed (exit code 0)
지정된 파일을 찾을 수 없습니다
CMAKE_MT-NOTFOUND
```

### 원인

1. **플랫폼 불일치**: CMake가 x64로 설정되어 있지만 프로젝트는 Win32여야 함
2. **Windows SDK 누락**: 리소스 컴파일러(RC) 또는 Windows SDK 설정 문제
3. **CMake 캐시 문제**: 잘못된 캐시로 인한 설정 충돌

---

## ✅ 해결 방법

### 방법 1: Visual Studio에서 Win32 구성 선택 (권장)

1. **솔루션 탐색기**에서 프로젝트 우클릭
2. **CMake 설정** 선택
3. **구성** 드롭다운에서 **Win32-Release** 또는 **Win32-Debug** 선택
4. 적용

또는 상단 도구 모음에서:
- **구성** 드롭다운을 **Win32-Release**로 변경

### 방법 2: CMake 캐시 삭제 후 재생성

**PowerShell에서:**

```powershell
# 캐시 및 빌드 디렉토리 삭제
Remove-Item -Recurse -Force out -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue

# Visual Studio에서 CMake 프로젝트 다시 열기
# 또는 Developer Command Prompt에서:
cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
```

### 방법 3: CMakeSettings.json 사용

프로젝트 루트에 `CMakeSettings.json` 파일이 생성되었습니다.
Visual Studio에서 이 파일을 인식하고 Win32 구성을 사용합니다.

**Visual Studio에서:**
1. 프로젝트 우클릭 → **CMake 설정 보기**
2. **Win32-Release** 또는 **Win32-Debug** 선택

### 방법 4: Visual Studio 워크로드 확인

1. Visual Studio 설치 관리자 실행
2. **수정** 클릭
3. **Desktop development with C++** 확인:
   - ✅ **MFC and ATL support**
   - ✅ **Windows 10/11 SDK** (최신 버전)
   - ✅ **C++ CMake tools for Windows**
4. **수정** 클릭하여 설치

---

## 🚀 빠른 해결 (지금 바로)

### Visual Studio에서:

1. **솔루션 탐색기** 열기 (Ctrl+Alt+L)
2. 프로젝트 우클릭
3. **CMake 설정 보기**
4. **구성** 드롭다운에서:
   - **Win32-Release** 선택 (권장)
   - 또는 **Win32-Debug** 선택
5. **저장** (자동 저장됨)

**이제 빌드하세요!**

---

### Developer Command Prompt에서:

```cmd
cd D:\stock\ebest\20180327\MMMaster

# 캐시 삭제
if exist out rmdir /s /q out
if exist build rmdir /s /q build

# Win32로 재생성
cmake -S . -B build -G "Visual Studio 17 2022" -A Win32

# 빌드
cmake --build build --config Release
```

---

## 🔧 추가 문제 해결

### Windows SDK 문제인 경우

**확인:**
```powershell
Get-ChildItem "C:\Program Files (x86)\Windows Kits\10\Include" | Select-Object Name
```

**없으면 설치:**
Visual Studio 설치 관리자에서:
- **개별 구성 요소** 탭
- **Windows 10 SDK** 또는 **Windows 11 SDK** 설치

### 리소스 컴파일러(RC) 문제

**확인:**
```powershell
Test-Path "C:\Program Files (x86)\Windows Kits\10\bin\10.0.*\x86\rc.exe"
```

없으면 Windows SDK 재설치

---

## 📋 확인 사항

빌드 전 확인:

- [ ] Visual Studio에서 **Win32-Release** 또는 **Win32-Debug** 구성 선택
- [ ] CMake 캐시 삭제됨
- [ ] Windows SDK 설치 확인
- [ ] MFC and ATL support 설치 확인

---

## 💡 참고

- **원래 프로젝트는 Win32 플랫폼**입니다
- CMakeSettings.json에 Win32 구성이 추가되었습니다
- Visual Studio는 기본적으로 x64를 선택하므로 수동으로 Win32 선택 필요

---

## ✅ 완료 후

빌드가 성공하면:

```
out\build\Win32-Release\bin\MMMaster.exe
```

또는 Visual Studio에서 빌드하면 자동으로 생성됩니다.

