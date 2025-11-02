# Visual Studio 2022 & CMake 빌드 가이드

## 🎯 빠른 시작

### 방법 1: CMake 사용 (권장)

#### 자동 설정 스크립트 실행
```powershell
.\setup_vs2022.ps1
```

#### 수동 설정
```powershell
# 1. 빌드 디렉토리 생성
mkdir build
cd build

# 2. CMake로 VS 2022 프로젝트 생성
cmake -S .. -B . -G "Visual Studio 17 2022" -A Win32

# 3. Visual Studio에서 열기
start MMMaster.sln

# 또는 빌드만 실행
cmake --build . --config Release
```

### 방법 2: Visual Studio에서 직접

1. Visual Studio 2022 실행
2. **파일 > 열기 > CMake...**
3. `CMakeLists.txt` 선택
4. **프로젝트 > CMake 설정 구성** (필요시)
5. **빌드 > 솔루션 빌드** (Ctrl+Shift+B)

### 방법 3: Cursor AI에서

1. Cursor AI에서 프로젝트 열기
2. `Ctrl+Shift+P` → "CMake: Configure"
3. `Ctrl+Shift+P` → "CMake: Build"
4. 또는 터미널에서:
   ```powershell
   .\setup_vs2022.ps1
   ```

---

## 📋 필수 요구사항

### 소프트웨어

- ✅ **Visual Studio 2022** (Community 이상)
  - Desktop development with C++
  - MFC and ATL support
  
- ✅ **CMake 3.20 이상**
  - https://cmake.org/download/
  - 또는 Visual Studio 설치 시 포함

- ✅ **Windows SDK 10.0 이상**

### 라이브러리

- ✅ **MySQL Connector/C**
  - https://dev.mysql.com/downloads/connector/c/
  - 또는 환경 변수 `MYSQL_DIR` 설정

- ✅ **XingAPI DLL**
  - 이베스트 개발자 센터에서 다운로드
  - 프로젝트 루트에 배치

---

## 🔧 빌드 설정

### CMake 옵션

```powershell
# Debug 빌드
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug

# Release 빌드 (기본)
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# MySQL 경로 지정
cmake -S . -B build -DMYSQL_DIR="C:/Program Files/MySQL/MySQL Connector C 8.0"
```

### 출력 경로

- **CMake 빌드**: `build/bin/Release/MMMaster.exe`
- **VS 빌드**: `Release/MMMaster.exe` (기존)

---

## 🚀 빌드 명령어

### PowerShell 스크립트 사용

```powershell
# 전체 설정 및 빌드
.\setup_vs2022.ps1

# 빌드만
.\build.ps1 -Configuration Release
```

### CMake 직접 사용

```powershell
# 설정
cmake -S . -B build -G "Visual Studio 17 2022" -A Win32

# 빌드
cmake --build build --config Release

# 클린
cmake --build build --target clean
```

### Visual Studio에서

1. `build/MMMaster.sln` 열기
2. **빌드 > 솔루션 빌드** (Ctrl+Shift+B)
3. 구성: Release 또는 Debug 선택

---

## 🐛 문제 해결

### 오류 1: CMake를 찾을 수 없음

```powershell
# CMake 설치 확인
cmake --version

# PATH에 추가되지 않은 경우
$env:PATH += ";C:\Program Files\CMake\bin"
```

### 오류 2: MFC 라이브러리 오류

Visual Studio 설치 관리자에서:
1. **개별 구성 요소** 탭
2. **MFC and ATL support** 확인

### 오류 3: MySQL 라이브러리를 찾을 수 없음

**방법 1: 환경 변수 설정**
```powershell
$env:MYSQL_DIR = "C:\Program Files\MySQL\MySQL Connector C 8.0"
```

**방법 2: CMake 캐시 수정**
```powershell
cmake -S . -B build -DMYSQL_INCLUDE_DIR="C:/Program Files/MySQL/MySQL Connector C 8.0/include" -DMYSQL_LIBRARY="C:/Program Files/MySQL/MySQL Connector C 8.0/lib/vs14/libmysql.lib"
```

### 오류 4: DLL을 찾을 수 없음

빌드 후 다음 DLL이 출력 디렉토리에 있는지 확인:
- `xingAPI.dll`
- `libmysql.dll`

CMake가 자동으로 복사하도록 설정되어 있습니다.

---

## 📁 프로젝트 구조

```
MMMaster/
├── build/                  # CMake 빌드 출력 (생성됨)
│   ├── MMMaster.sln       # VS 2022 솔루션
│   └── bin/               # 실행 파일
├── .vscode/               # Cursor/VS Code 설정
│   ├── settings.json
│   ├── tasks.json
│   └── launch.json
├── CMakeLists.txt         # CMake 빌드 설정
├── setup_vs2022.ps1      # 자동 설정 스크립트
└── ...
```

---

## ✅ 빌드 확인

빌드 성공 후:

```powershell
# 실행 파일 확인
Test-Path build\bin\Release\MMMaster.exe

# 파일 정보
Get-Item build\bin\Release\MMMaster.exe | Format-List Name, Length, LastWriteTime
```

---

## 🔄 다음 단계

1. **코드 정리**: `.\analyze_unused_code.ps1` 실행하여 불필요한 코드 분석
2. **테스트**: 빌드된 실행 파일 실행
3. **디버깅**: Visual Studio 또는 Cursor에서 디버깅

---

## 📝 참고사항

- CMake는 Visual Studio 2022의 네이티브 CMake 지원을 사용합니다
- `.vcxproj` 파일은 CMake가 자동으로 생성합니다
- 기존 `.vcproj` 파일은 더 이상 사용되지 않습니다 (참고용)

