# new.h 오류 최종 해결 가이드

## 🔍 문제 분석

**현재 상황:**
- `StdAfx.h`에 `_INC_NEW` 매크로와 `#include <new>` 추가 완료
- 여전히 빌드 시 `new.h` 오류 발생
- PCH 파일이 재생성되지 않아서 오래된 내용 사용 중

**원인:**
Visual Studio가 변경된 `StdAfx.h`를 인식하지 못하고 오래된 PCH를 사용하고 있습니다.

---

## ✅ 최종 해결 방법

### 방법 1: Visual Studio에서 완전 정리 (권장)

**Visual Studio에서:**

1. **빌드 > 솔루션 정리** (Clean Solution)
   - 모든 중간 파일 삭제

2. **솔루션 탐색기**에서:
   - `StdAfx.cpp` 우클릭
   - **속성**
   - **C/C++ > 미리 컴파일된 헤더**
   - 확인: **미리 컴파일된 헤더**: "생성(/Yc)"
   - 확인: **미리 컴파일된 헤더 파일**: "StdAfx.h"

3. **빌드 > 솔루션 다시 빌드** (Ctrl+Alt+F7)
   - 또는 **프로젝트 우클릭 > 다시 빌드**

### 방법 2: 수동으로 PCH 파일 완전 삭제

**PowerShell에서:**

```powershell
# 모든 PCH 및 중간 파일 삭제
Remove-Item -Recurse -Force Debug -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force Release -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "*.pch" -Recurse -ErrorAction SilentlyContinue
Remove-Item -Recurse -Force "*.obj" -Recurse -ErrorAction SilentlyContinue

Write-Host "정리 완료. Visual Studio에서 다시 빌드하세요."
```

### 방법 3: 프로젝트 속성 확인

**Visual Studio에서:**

1. **프로젝트 우클릭 > 속성**
2. **구성 속성 > C/C++ > 미리 컴파일된 헤더**
   - **미리 컴파일된 헤더**: "사용(/Yu)" 확인
   - **미리 컴파일된 헤더 파일**: "StdAfx.h" 확인
   - **미리 컴파일된 헤더 출력 파일**: ".\Release\ETrade_Order_Sample.pch" (또는 .\Debug\...)

3. **구성 속성 > 일반**
   - **MFC 사용**: "동적 라이브러리에서 MFC 사용"

---

## 🔧 추가 확인: StdAfx.h 내용

현재 `StdAfx.h` 파일에는 다음이 포함되어 있습니다:

```cpp
// Visual Studio 2022 호환성을 위한 헤더
#define _INC_NEW            // new.h 포함 방지
#include <new>              // 표준 C++ new 헤더
#include <afxwin.h>         // MFC
```

**이 설정은 올바릅니다!** 단지 PCH가 재생성되지 않았을 뿐입니다.

---

## 🚀 지금 바로 시도

### 가장 확실한 방법:

**Visual Studio에서:**

1. **파일 > 닫기** (모든 파일 닫기)
2. **빌드 > 솔루션 정리**
3. **Visual Studio 종료**
4. **Visual Studio 다시 실행**
5. **프로젝트 열기**
6. **빌드 > 솔루션 다시 빌드**

이렇게 하면 PCH가 완전히 재생성됩니다.

---

## 📝 확인 사항

빌드 전 최종 확인:

- [x] `StdAfx.h`에 `#define _INC_NEW` 있음
- [x] `StdAfx.h`에 `#include <new>` 있음
- [ ] PCH 파일 삭제됨
- [ ] 프로젝트 정리 완료
- [ ] Visual Studio 재시작

---

## 💡 원리

`afx.h` 파일 내부:
```cpp
#ifndef _INC_NEW
	#include <new.h>
#endif
```

따라서:
- `_INC_NEW`가 정의되면 → `new.h` 포함 안 함 ✓
- `_INC_NEW`가 정의 안 되면 → `new.h` 포함 시도 → 오류 ✗

현재 `StdAfx.h`에는 올바르게 정의되어 있으므로, PCH만 재생성하면 해결됩니다!

---

## ✅ 예상 결과

PCH 재생성 후:
- ✅ `new.h` 오류 사라짐
- ✅ 모든 파일 컴파일 성공
- ✅ 빌드 성공

