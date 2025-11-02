# CMake 문제 해결 가이드

## 🔍 문제 분석

터미널에서 다음 오류가 발생했습니다:

```
'cmake' 용어가 cmdlet, 함수, 스크립트 파일 또는 실행할 수 있는 프로그램 이름으로 인식되지 않습니다.
```

**원인**: CMake가 PATH 환경 변수에 등록되어 있지 않습니다.

**확인 결과**: CMake는 Visual Studio에 설치되어 있습니다!
- 경로: `C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`

---

## ✅ 해결 방법

### 방법 1: Visual Studio Developer Command Prompt 사용 (권장)

1. **시작 메뉴**에서 **"Developer Command Prompt for VS 2022"** 검색
2. 실행
3. 프로젝트 디렉토리로 이동:
   ```cmd
   cd D:\stock\ebest\20180327\MMMaster
   ```
4. CMake 실행:
   ```cmd
   cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
   ```

### 방법 2: PATH에 CMake 추가 (현재 세션만)

PowerShell에서:
```powershell
$env:PATH += ";C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
cmake --version  # 확인
```

### 방법 3: 직접 경로 지정

```powershell
$CmakeExe = "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
& $CmakeExe -S . -B build -G "Visual Studio 17 2022" -A Win32
```

### 방법 4: Visual Studio에서 직접 (가장 쉬움)

1. Visual Studio 2022 실행
2. **파일 > 열기 > CMake...**
3. `CMakeLists.txt` 선택
4. 자동으로 프로젝트 생성 및 빌드 가능

---

## 🚀 빠른 해결 (지금 바로)

### PowerShell에서:

```powershell
# 1. CMake 경로 추가
$env:PATH += ";C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"

# 2. 확인
cmake --version

# 3. 프로젝트 생성
cmake -S . -B build -G "Visual Studio 17 2022" -A Win32

# 4. 빌드
cmake --build build --config Release
```

### 또는 Visual Studio Developer PowerShell:

```powershell
# Developer PowerShell은 이미 PATH가 설정되어 있음
cd D:\stock\ebest\20180327\MMMaster
cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
cmake --build build --config Release
```

---

## 💡 영구적 해결 (선택사항)

시스템 환경 변수에 CMake 경로를 추가하면 매번 PATH를 추가할 필요가 없습니다:

1. **시스템 속성 > 고급 > 환경 변수**
2. **시스템 변수**의 **Path** 선택
3. **편집** 클릭
4. **새로 만들기** 클릭
5. 다음 경로 추가:
   ```
   C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin
   ```
6. **확인** 클릭
7. PowerShell 재시작

---

## 📝 참고

- Visual Studio 2022에는 CMake가 포함되어 있습니다
- 별도 설치가 필요하지 않습니다
- Developer Command Prompt를 사용하면 자동으로 PATH가 설정됩니다

---

## ✅ 다음 단계

CMake가 작동하면:

1. 프로젝트 생성: `cmake -S . -B build -G "Visual Studio 17 2022" -A Win32`
2. Visual Studio에서 열기: `build\MMMaster.sln`
3. 빌드: Visual Studio에서 Ctrl+Shift+B

