# 문제 해결 요약

## 🔍 발견된 문제

터미널 오류:
```
'cmake' 용어가 cmd치트, 함수, 스크립트 파일 또는 실행할 수 있는 프로그램 이름으로 인식되지 않습니다.
```

## ✅ 원인

CMake는 Visual Studio에 설치되어 있지만, **PATH 환경 변수에 등록되어 있지 않습니다**.

**CMake 위치**: 
- `C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe`

## 🚀 해결 방법 (우선순위 순)

### 방법 1: Visual Studio Developer Command Prompt 사용 ⭐ (가장 권장)

**가장 간단하고 확실한 방법입니다!**

1. **시작 메뉴**에서 **"Developer Command Prompt for VS 2022"** 검색 및 실행
2. 프로젝트 디렉토리로 이동:
   ```cmd
   cd D:\stock\ebest\20180327\MMMaster
   ```
3. CMake 실행:
   ```cmd
   cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
   cmake --build build --config Release
   ```

**이 방법이 가장 확실합니다!**

---

### 방법 2: Visual Studio에서 직접 열기 ⭐⭐

**가장 쉬운 방법입니다!**

1. Visual Studio 2022 실행
2. **파일 > 열기 > CMake...**
3. `CMakeLists.txt` 파일 선택
4. 자동으로 프로젝트가 생성되고 빌드 가능

**추천: 이 방법을 사용하세요!**

---

### 방법 3: PowerShell에서 PATH 추가

현재 세션에만 적용:

```powershell
$env:PATH += ";C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
cmake -S . -B build -G "Visual Studio 17 2022" -A Win32
```

---

### 방법 4: quick_setup.ps1 사용

새로 만든 스크립트:

```powershell
.\quick_setup.ps1
```

**참고**: PowerShell 인코딩 문제로 작동하지 않을 수 있습니다.

---

## 📋 단계별 가이드

### 가장 확실한 방법:

1. **Visual Studio 2022 실행**
2. **파일 > 열기 > CMake...**
3. `CMakeLists.txt` 선택
4. 완료! 프로젝트가 자동으로 생성됩니다.

또는

1. **Developer Command Prompt for VS 2022** 실행
2. `cd D:\stock\ebest\20180327\MMMaster`
3. `cmake -S . -B build -G "Visual Studio 17 2022" -A Win32`

---

## 💡 추가 참고

- **FIX_CMAKE_ISSUE.md** - 상세한 문제 해결 가이드
- **BUILD_VS2022.md** - 빌드 가이드

---

## ✅ 결론

**지금 바로 사용할 방법:**

👉 **Visual Studio 2022에서 파일 > 열기 > CMake...로 CMakeLists.txt 열기**

이것이 가장 쉬우고 확실한 방법입니다!

