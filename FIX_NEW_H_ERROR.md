# new.h 오류 해결 가이드

## 🔍 문제 분석

Visual Studio에서 프로젝트를 빌드할 때 다음 오류 발생:

```
fatal error C1083: 포함 파일을 열 수 없습니다. 'new.h': No such file or directory
파일: afx.h(62,11)
```

### 원인

Visual Studio 2008에서 2022로 변환하면서 발생하는 MFC 관련 문제입니다:
- Visual Studio 2022의 MFC에서는 `new.h` 대신 `<new>` 헤더를 사용
- 프로젝트 설정이 올바르게 변환되지 않았을 수 있음

---

## ✅ 해결 방법

### 방법 1: Visual Studio에서 프로젝트 설정 수정 (권장)

1. **솔루션 탐색기**에서 프로젝트 우클릭
2. **속성** 선택
3. **구성 속성 > 일반**에서:
   - **MFC 사용**: **동적 라이브러리에서 MFC 사용** 선택
   - **플랫폼 도구 집합**: **v143** (VS2022) 확인
4. **구성 속성 > C/C++ > 일반**에서:
   - **추가 포함 디렉토리** 확인
   - MFC 경로가 올바른지 확인
5. **확인** 클릭

### 방법 2: 프로젝트 파일 직접 수정

`.vcxproj` 파일이 있다면 다음을 확인:

```xml
<PropertyGroup>
  <UseOfMfc>Dynamic</UseOfMfc>
</PropertyGroup>
```

### 방법 3: Visual Studio 재설치 또는 구성 요소 추가

1. Visual Studio 설치 관리자 실행
2. **수정** 클릭
3. **Desktop development with C++** 확인:
   - ✅ **MFC and ATL support (최신 버전)**
   - ✅ **Windows 10/11 SDK**
4. **수정** 클릭

### 방법 4: StdAfx.h에 명시적으로 추가 (임시 해결)

`StdAfx.h` 파일의 상단에 추가:

```cpp
#include <new>  // Visual Studio 2022용
```

**위치**: `#include <afxwin.h>` 전에 추가

---

## 🚀 빠른 해결 (지금 바로)

### Visual Studio에서:

1. **프로젝트 > 속성** (Alt+F7)
2. **구성 속성 > 일반**
3. **MFC 사용**: **동적 라이브러리에서 MFC 사용** 확인
4. **적용 > 확인**
5. **빌드 > 솔루션 다시 빌드** (Ctrl+Alt+F7)

---

## 📝 확인 사항

빌드 전 확인:

- [ ] MFC and ATL support가 설치되어 있는지
- [ ] 프로젝트 속성에서 "동적 라이브러리에서 MFC 사용" 선택
- [ ] 플랫폼 도구 집합이 v143 (VS2022)인지
- [ ] Windows SDK가 올바르게 설정되어 있는지

---

## 🔧 추가 문제 해결

### 여전히 오류가 발생하면:

1. **프로젝트 정리**:
   - 빌드 > 솔루션 정리
   - `Debug`, `Release` 폴더의 `.obj`, `.pch` 파일 삭제

2. **재빌드**:
   - 빌드 > 솔루션 다시 빌드

3. **Visual Studio 재시작**

---

## 💡 참고

- Visual Studio 2022에서는 `new.h` 대신 `<new>` 헤더 사용
- MFC 설정이 올바르면 자동으로 처리됨
- 프로젝트를 변환할 때 MFC 설정이 누락될 수 있음

---

## ✅ 완료 후

빌드가 성공하면:

```
Release\MMMaster.exe
```

또는

```
Debug\MMMaster.exe
```

생성됩니다.

