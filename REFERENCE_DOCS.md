# 참조 문서 인덱스

이 문서는 MMMaster 프로젝트 개발 시 참조할 수 있는 문서들의 목록과 설명을 제공합니다.

## 📚 XingAPI 사용자 매뉴얼

### 문서 위치
- **폴더**: `docs/`
- **파일**: 
  - `3.pdf` - XingAPI 사용자 매뉴얼 (일반)
  - `5.pdf` - XingAPI 사용자 매뉴얼 (고급)

### 문서 설명
이베스트 XingAPI의 공식 사용자 매뉴얼입니다. API 사용 방법, TR 코드 설명, 실시간 데이터 구독 방법 등이 포함되어 있습니다.

### 프로젝트에서의 활용
이 문서들은 다음 작업 시 참조하세요:

#### 1. **XingAPI 초기화 및 연결**
- **관련 파일**: `S_Login.cpp`, `IXingAPI.h`
- **참조 섹션**: API 초기화, 서버 연결, 로그인 처리
- **사용 예시**:
  ```cpp
  g_iXingAPI.Connect()  // 서버 연결
  g_iXingAPI.Login()    // 로그인
  ```

#### 2. **TR 코드 요청**
- **관련 파일**: `CSPAT00700.cpp`, `CSPAT00800.cpp`, `CSPAQ03700.cpp`, `T0424.cpp`, `T1101.cpp`, `T1102.cpp` 등
- **참조 섹션**: TR 코드 요청 방법, 데이터 수신 처리
- **주요 TR 코드**:
  - `CSPAT00700`: 매도 주문
  - `CSPAT00800`: 주문 취소
  - `CSPAQ03700`: 주문 체결 내역 조회
  - `CSPBQ00200`: 매수 가능 조회
  - `T0424`: 잔고 조회
  - `T1101`: 현재가 조회
  - `T1102`: 현재가(상세) 조회
  - `T1301`: 시간대별 체결가 조회
  - `T1636`: 프로그램 매매 현황
  - `T1637`: 프로그램 매매 종목 리스트

#### 3. **실시간 데이터 구독**
- **관련 파일**: `ETrade_Order_SampleDlg.cpp` (OnXMReceiveRealData)
- **참조 섹션**: 실시간 데이터 구독 방법, 데이터 파싱
- **사용 예시**:
  ```cpp
  g_iXingAPI.AdviseRealData()    // 실시간 데이터 구독
  g_iXingAPI.UnadviseRealData()  // 구독 해제
  ```

#### 4. **주문 처리**
- **관련 파일**: `S_Order_Debt.cpp`, `CSPAT00700.cpp`
- **참조 섹션**: 주문 전송, 주문 취소, 체결 확인
- **주요 기능**:
  - 자동 매수/매도 주문
  - 주문 취소
  - 체결 상태 확인

#### 5. **에러 처리**
- **관련 파일**: `S_Login.cpp`, `ETrade_Order_SampleDlg.cpp`
- **참조 섹션**: 에러 코드, 에러 메시지 처리
- **사용 예시**:
  ```cpp
  int nErrorCode = g_iXingAPI.GetLastError();
  CString strMsg = g_iXingAPI.GetErrorMessage(nErrorCode);
  ```

## 🔍 문서 검색 가이드

### TR 코드 찾기
1. 문서에서 TR 코드 섹션 검색 (예: "CSPAT00700", "T0424")
2. 입력 필드 및 출력 필드 확인
3. 연속 조회 방법 확인 (NextKey 사용)

### API 함수 찾기
1. 함수명으로 검색 (예: "Request", "AdviseRealData")
2. 매개변수 및 반환값 확인
3. 사용 예제 코드 확인

### 실시간 데이터 찾기
1. "실시간 데이터" 또는 "Real Data" 섹션 검색
2. TR 코드별 실시간 데이터 필드 확인
3. 구독/해제 방법 확인

## 📋 프로젝트 주요 TR 코드 매핑

| TR 코드 | 용도 | 관련 파일 | 문서 참조 |
|---------|------|-----------|-----------|
| CSPAT00700 | 매도 주문 | `CSPAT00700.cpp` | 주문 처리 섹션 |
| CSPAT00800 | 주문 취소 | `CSPAT00800.cpp` | 주문 처리 섹션 |
| CSPAQ03700 | 체결 내역 조회 | `CSPAQ03700.cpp` | 조회 TR 섹션 |
| CSPBQ00200 | 매수 가능 조회 | `CSPBQ00200.cpp` | 조회 TR 섹션 |
| T0424 | 잔고 조회 | `T0424.cpp` | 조회 TR 섹션 |
| T1101 | 현재가 조회 | `T1101.cpp` | 조회 TR 섹션 |
| T1102 | 현재가(상세) 조회 | `T1102.cpp` | 조회 TR 섹션 |
| T1301 | 시간대별 체결가 | `ETrade_Order_SampleDlg.cpp` | 조회 TR 섹션 |
| T1636 | 프로그램 매매 현황 | `GetList.cpp` | 조회 TR 섹션 |
| T1637 | 프로그램 매매 종목 | `ETrade_Order_SampleDlg.cpp` | 조회 TR 섹션 |

## 🛠️ 개발 시 참조 순서

1. **새로운 TR 코드 추가 시**
   - `docs/3.pdf` 또는 `docs/5.pdf`에서 TR 코드 검색
   - 입력/출력 필드 확인
   - `packet/` 폴더의 패킷 정의 파일 참조
   - 기존 TR 코드 구현 파일 참조 (예: `T0424.cpp`)

2. **실시간 데이터 추가 시**
   - 문서에서 실시간 데이터 섹션 확인
   - TR 코드별 실시간 데이터 필드 확인
   - `ETrade_Order_SampleDlg.cpp`의 `OnXMReceiveRealData` 함수 참조

3. **에러 발생 시**
   - `g_iXingAPI.GetLastError()`로 에러 코드 확인
   - 문서의 에러 코드 섹션에서 원인 확인
   - `g_iXingAPI.GetErrorMessage()`로 에러 메시지 확인

## 📝 참고사항

- 문서는 최신 버전입니다
- PDF 파일은 프로젝트 루트의 `docs/` 폴더에 위치합니다
- 문서 내용이 코드와 다를 경우, 최신 API 문서를 확인하세요
- 이베스트 개발자 센터에서 추가 문서를 확인할 수 있습니다

## 🔗 관련 링크

- 이베스트 개발자 센터: https://developers.ebestsec.co.kr/
- XingAPI 다운로드: 이베스트 개발자 센터에서 제공
- API 문의: 이베스트 고객센터

---

**마지막 업데이트**: 2024년
**문서 버전**: 최신 버전

