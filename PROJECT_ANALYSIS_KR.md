# MMMaster 프로젝트 상세 분석

## 📋 프로젝트 개요

**MMMaster**는 한국 주식 시장을 위한 MFC 기반 자동 매매 시스템입니다. 이베스트(eBEST)의 XingAPI를 활용하여 실시간 주가 데이터를 수신하고, 매수/매도 주문을 처리하며, 포트폴리오를 관리하는 Windows 데스크톱 애플리케이션입니다.

### 주요 특징
- **실시간 주가 모니터링**: 실시간 호가, 체결가, 거래량 등 시장 데이터 수신
- **자동 매매 시스템**: 조건에 따른 자동 매수/매도 주문 처리
- **포트폴리오 관리**: 보유 종목, 손익, 잔고 관리
- **데이터베이스 연동**: MySQL을 통한 거래 내역 및 시장 데이터 저장
- **패턴 분석**: 주가 패턴 분석 및 매매 신호 생성

---

## 🏗️ 프로젝트 구조

### 기술 스택
- **언어**: C++ (Visual C++ 2008)
- **프레임워크**: Microsoft Foundation Classes (MFC)
- **API**: 이베스트 XingAPI (xingAPI.dll)
- **데이터베이스**: MySQL
- **개발 환경**: Visual Studio 2008 (.vcproj 포맷)

### 디렉토리 구조
```
MMMaster/
├── Source Files/
│   ├── ETrade_Order_Sample.cpp    # 메인 애플리케이션 진입점
│   ├── ETrade_Order_SampleDlg.cpp # 메인 다이얼로그
│   ├── CSPAQ03700.cpp              # 잔고 조회 TR
│   ├── CSPAT00700.cpp              # 매도 주문 TR
│   ├── CSPAT00800.cpp              # 주문 취소 TR
│   ├── CSPBQ00200.cpp              # 매수 가능 조회 TR
│   ├── S_Login.cpp                 # 로그인 처리
│   ├── S_Order_Debt.cpp            # 주문 처리
│   ├── MySQLCMD.cpp                # MySQL 데이터베이스 연동
│   ├── GetList.cpp                 # 종목 리스트 관리
│   ├── T0424.cpp                   # 잔고 조회
│   ├── T1101.cpp                   # 현재가 조회
│   ├── T1102.cpp                   # 현재가(상세) 조회
│   └── ...
├── Header Files/
│   ├── ETrade_Order_Sample.h       # 메인 애플리케이션 클래스
│   ├── ETrade_Order_SampleDlg.h    # 메인 다이얼로그 클래스
│   ├── IXingAPI.h                  # XingAPI 인터페이스
│   ├── MySQLCMD.h                  # MySQL 래퍼 클래스
│   └── packet/*.h                   # TR 패킷 정의 (241개)
├── packet/                          # 이베스트 TR 코드 패킷 정의
│   └── [241개의 헤더 파일]
├── Debug/                           # 디버그 빌드 출력
├── Release/                         # 릴리스 빌드 출력
│   ├── MMMaster.exe                # 실행 파일
│   └── Sys/                        # 시스템 설정 파일
└── res/                             # 리소스 파일 (아이콘 등)
```

---

## 🔧 주요 컴포넌트

### 1. **XingAPI 연동 (IXingAPI.h)**
이베스트 증권사의 XingAPI를 사용하여 실시간 시장 데이터와 주문을 처리합니다.

**주요 기능:**
- 서버 연결 및 로그인
- TR 코드 요청 (조회)
- 실시간 데이터 구독 (Real-time data)
- 주문 전송

**주요 TR 코드:**
- `CSPAT00700`: 매도 주문
- `CSPAT00800`: 주문 취소
- `CSPAQ03700`: 주문 체결 내역 조회
- `CSPBQ00200`: 매수 가능 조회
- `t0424`: 잔고 조회
- `t1101`: 현재가 조회
- `t1102`: 현재가(상세) 조회
- `t1301`: 시간대별 체결가 조회
- `t1636`: 프로그램 매매 현황
- `t1637`: 프로그램 매매 종목 리스트

**참조 문서:**
- XingAPI 사용자 매뉴얼: `docs/3.pdf`, `docs/5.pdf`
- 자세한 내용은 [참조 문서 인덱스](REFERENCE_DOCS.md)를 확인하세요

### 2. **데이터 구조 (ETrade_Order_Sample.h)**

프로젝트에서 사용하는 주요 데이터 구조:

#### **JMINFO** - 종목 정보
```cpp
struct JMINFO {
    CString shcode;      // 종목코드
    CString hname;       // 종목명
    CString price;       // 현재가
    CString msprice;     // 매수가
    CString mdprice;     // 매도가
    CString volume;      // 거래량
    // ... 기타 필드
};
```

#### **COUNTINFO** - 잔고 정보
```cpp
struct COUNTINFO {
    CString expcode;     // 종목코드
    CString janqty;     // 잔고수량
    CString pamt;        // 매입금액
    CString mamt;        // 평가금액
    CString sinamt;      // 손익금액
    // ... 기타 필드
};
```

#### **EVINFO** - 주문 이벤트 정보
```cpp
struct EVINFO {
    CString ordno;       // 주문번호
    CString expcode;     // 종목코드
    CString qty;         // 주문수량
    CString price;       // 주문가격
    CString cheqty;      // 체결수량
    CString status;      // 상태
    // ... 기타 필드
};
```

### 3. **MySQL 데이터베이스 연동 (MySQLCMD.cpp)**

MySQL 데이터베이스와 연동하여 다음 데이터를 저장/조회합니다:
- 거래 내역
- 주가 데이터 (일봉, 분봉)
- 평균가 정보 (5일, 20일, 60일 이동평균)
- 패턴 분석 결과
- 로그 데이터

**주요 메서드:**
- `db_init()`: 데이터베이스 연결 초기화
- `db_query_cmd()`: SQL 쿼리 실행
- `db_update_1302()`: 일봉 데이터 업데이트
- `db_update_1637()`: 프로그램 매매 데이터 업데이트
- `mgAVGjminfo()`: 이동평균 정보 관리

### 4. **주문 처리 (S_Order_Debt.cpp)**

매수/매도 주문을 처리하는 모듈입니다.

**주요 기능:**
- 자동 매수 주문
- 자동 매도 주문
- 주문 취소
- 주문 상태 모니터링

### 5. **로그인 처리 (S_Login.cpp)**

XingAPI 서버에 로그인하는 모듈입니다.

**필요 정보:**
- 사용자 ID
- 비밀번호
- 공인인증서 비밀번호
- 계좌번호

---

## 📊 주요 기능 흐름

### 1. **애플리케이션 시작**
```
InitInstance()
  ├─ AfxOleInit()              # OLE 초기화
  ├─ AfxSocketInit()           # 소켓 초기화
  └─ CETrade_Order_SampleDlg   # 메인 다이얼로그 표시
```

### 2. **서버 연결 및 로그인**
```
OnBnClickedButtonConnect()
  ├─ g_iXingAPI.Connect()      # 서버 연결
  ├─ g_iXingAPI.Login()       # 로그인
  └─ 계좌 정보 조회
```

### 3. **실시간 데이터 수신**
```
OnXMReceiveRealData()
  ├─ 주가 데이터 파싱
  ├─ UI 업데이트
  └─ 패턴 분석 및 매매 신호 생성
```

### 4. **주문 처리**
```
CallTRBuy() / CallSell()
  ├─ CSPAT00700 (매도) 또는 CSPAT00600 (매수) TR 전송
  ├─ 주문번호 수신
  └─ 체결 확인 (CSPAQ03700)
```

### 5. **데이터베이스 저장**
```
주문 체결 후 / 실시간 데이터 수신 시
  └─ MySQLCMD를 통해 데이터베이스에 저장
```

---

## 🔐 보안 및 설정

### 로그인 정보 (StdAfx.h)
```cpp
#define STR_ID              "k8096"
#define STR_PASSWORD_ID     "kim1972"
#define STR_PASSWORD_CERTI  "kim-1923717"
#define ACCOUNT_NUM         "20080800101"
```

⚠️ **주의**: 실제 사용 시 하드코딩된 인증 정보를 제거하고 안전한 방식으로 관리해야 합니다.

### 설정 파일
- `Login.ini`: 로그인 관련 설정 (MAC 주소 등)
- `Release/Sys/`: 시스템 설정 파일들
  - `Guide.ini`: API 가이드
  - `webinfo.ini`: 웹 정보 설정
  - `RD.ini`: 실시간 데이터 설정

---

## 📈 패턴 분석 기능

프로젝트에는 다양한 패턴 분석 기능이 포함되어 있습니다:

- **이동평균 분석**: 5일, 20일, 60일 이동평균
- **거래량 분석**: 거래량 급증 감지
- **프로그램 매매 분석**: 기관 매매 동향 분석
- **패턴 인식**: 특정 주가 패턴 감지 (`PATTENInfo` 구조체)

---

## 🗄️ 데이터베이스 스키마

MySQL 데이터베이스에 저장되는 주요 테이블들:
- 거래 내역 테이블
- 일봉/분봉 데이터 테이블
- 이동평균 데이터 테이블
- 프로그램 매매 데이터 테이블
- 로그 테이블

---

## ⚠️ 알려진 제약사항

1. **Visual Studio 2008**: 오래된 버전의 Visual Studio로 빌드됨
2. **MFC 의존성**: Windows 전용, 이식성 낮음
3. **한국어 주석**: 소스 코드의 주석이 한국어로 작성됨
4. **하드코딩된 인증 정보**: 보안상 위험
5. **XingAPI 의존성**: 이베스트 증권 API 필수
6. **MySQL 연동**: MySQL 클라이언트 라이브러리 필요

---

## 🔄 프로젝트 업그레이드 권장사항

1. **현대적 빌드 시스템**: Visual Studio 2019/2022 또는 CMake로 마이그레이션
2. **보안 강화**: 인증 정보 암호화 및 설정 파일화
3. **코드 리팩토링**: C++11/14/17 표준 사용
4. **의존성 관리**: vcpkg 또는 Conan 사용
5. **테스트 코드**: 단위 테스트 및 통합 테스트 추가
6. **문서화**: Doxygen 등을 통한 API 문서 생성

---

## 📚 참조 문서

### XingAPI 사용자 매뉴얼
프로젝트 개발 시 참조할 수 있는 XingAPI 공식 문서가 `docs/` 폴더에 있습니다:
- `docs/3.pdf` - XingAPI 사용자 매뉴얼 (일반)
- `docs/5.pdf` - XingAPI 사용자 매뉴얼 (고급)

이 문서들은 다음 작업 시 참조하세요:
- TR 코드 추가 및 수정
- 실시간 데이터 구독
- 주문 처리 로직 구현
- 에러 처리 및 디버깅

자세한 내용은 [참조 문서 인덱스](REFERENCE_DOCS.md)를 확인하세요.

---

## 📝 요약

**MMMaster**는 한국 주식 시장을 위한 기능이 풍부한 자동 매매 시스템입니다. XingAPI를 통해 실시간 데이터를 수신하고, MySQL에 데이터를 저장하며, 다양한 패턴 분석 기능을 제공합니다. 다만, 오래된 개발 도구와 프레임워크를 사용하고 있어 현대화가 필요한 상태입니다.

**주요 사용 사례:**
- 실시간 주가 모니터링
- 자동 매매 시스템 운영
- 프로그램 매매 분석
- 포트폴리오 관리
- 거래 내역 분석 및 리포트 생성

