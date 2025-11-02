# MMMaster 데이터 포맷 문서

이 문서는 MMMaster 프로젝트에서 사용하는 MySQL 데이터베이스의 테이블 구조와 데이터 포맷을 설명합니다.

## 개요

MMMaster는 별도의 데이터 수집 프로젝트에서 수집한 데이터를 MySQL 데이터베이스에서 읽어와 사용하며, 실시간 정보는 이베스트 XingAPI를 통해 직접 조회합니다.

## 데이터베이스 구조

### 1. 계좌 설정 테이블 (cfg_tbl)

**용도**: 계좌 정보 및 매매 설정 저장

**테이블 구조**:
```sql
CREATE TABLE cfg_tbl(
    idindex char(4) PRIMARY KEY,           -- 계좌 인덱스 (예: "1", "2")
    countid char(30),                      -- 계좌번호
    countpw VARBINARY(64),                 -- 계좌 비밀번호 (암호화)
    loginid char(30),                      -- 로그인 ID
    loginpw VARBINARY(64),                 -- 로그인 비밀번호 (암호화)
    verifypw VARBINARY(64),                -- 공인인증서 비밀번호 (암호화)
    msvalue char(12),                      -- 매수 금액
    msrate char(6),                        -- 매수 비율
    mdlevel char(6),                       -- 매도 레벨
    sunik char(6),                         -- 손익
    maxsunik char(6),                      -- 최대 손익
    sonjel char(6),                        -- 손절
    maxsonjel char(6),                     -- 최대 손절
    allcount char(6)                       -- 전체 종목 수
) ENGINE = MyISAM
```

**암호화 방식**:
- `countpw`: AES_ENCRYPT('password', '12345678')
- `loginpw`: AES_ENCRYPT('password', '23456781')
- `verifypw`: AES_ENCRYPT('password', '34567812')

**사용 예시**:
```cpp
// 계좌 정보 조회
ctrlDB.db_getinfo("1");  // idindex='1'인 계좌 정보 조회
```

### 2. 잔고 정보 테이블 (date_jango_tbl)

**용도**: 일별 잔고 정보 저장

**테이블 구조**:
```sql
CREATE TABLE date_jango_tbl(
    codemode char(4),      -- 코드 모드
    shcode char(6),        -- 종목코드
    hname char(40),        -- 종목명
    cur_date char(30),     -- 현재 날짜
    janqty char(12),       -- 잔고 수량
    pamt char(12),         -- 매입 금액
    price char(12),        -- 가격
    rate char(6),          -- 비율
    PRIMARY KEY(codemode, shcode, cur_date)
) ENGINE = MyISAM
```

### 3. 일별 잔고 요약 테이블 (daily_jango_{index}_tbl)

**용도**: 계좌별 일별 잔고 요약 정보

**테이블 구조**:
```sql
CREATE TABLE daily_jango_1_tbl(
    cur_date char(30) PRIMARY KEY,         -- 현재 날짜
    total_assets char(20),                 -- 총 자산
    purchase_amount char(20),              -- 매입 금액
    valuation_gain char(20),               -- 평가 손익
    realized_profit char(20),              -- 실현 손익
    rate char(6),                          -- 비율
    rev char(10)                           -- 예비 필드
) ENGINE = MyISAM
```

**참고**: 테이블명의 `{index}`는 계좌 인덱스입니다 (예: `daily_jango_1_tbl`).

### 4. 종목 정보 테이블 (envelop_jm_tbl)

**용도**: 종목별 매수/매도 정보 추적

**테이블 구조**:
```sql
CREATE TABLE envelop_jm_tbl(
    shcode char(6),                        -- 종목코드
    hname char(40),                        -- 종목명
    reg_date char(10),                     -- 등록 날짜
    cur_datetime char(20),                 -- 현재 일시
    price char(12),                        -- 가격
    20avgprice char(12),                   -- 20일 평균가
    ms_datetime char(20),                  -- 매수 일시
    msprice char(12),                      -- 매수가
    md_datetime char(20),                  -- 매도 일시
    mdprice char(12),                      -- 매도가
    rate char(6),                          -- 비율
    mode char(6),                          -- 모드
    countmode char(6),                     -- 계좌 모드
    PRIMARY KEY(shcode, cur_datetime, price, mode, countmode)
) ENGINE = MyISAM
```

### 5. 주문 정보 테이블 (Order_JM_tbl)

**용도**: 주문 내역 저장

**테이블 구조**:
```sql
CREATE TABLE Order_JM_tbl(
    shcode char(6),                        -- 종목코드
    hname char(40),                        -- 종목명
    reg_date char(10),                     -- 등록 날짜
    cur_datetime char(20),                 -- 현재 일시
    price char(12),                        -- 가격
    msprice char(12),                      -- 매수가
    mdprice char(12),                      -- 매도가
    info char(20),                         -- 정보
    PRIMARY KEY(shcode, reg_date, info)
) ENGINE = MyISAM
```

### 6. 로그 메시지 테이블 (logmsg_tbl)

**용도**: 시스템 로그 메시지 저장

**테이블 구조**:
```sql
CREATE TABLE logmsg_tbl(
    shcode char(6),                        -- 종목코드
    cur_date char(30),                     -- 현재 날짜
    msg varchar(300),                      -- 메시지
    PRIMARY KEY(shcode, cur_date)
)
```

### 7. 이동평균 정보 테이블 (avg_5_20_60_jm_tbl)

**용도**: 종목별 이동평균 정보 (최신 데이터)

**테이블 구조**:
```sql
CREATE TABLE avg_5_20_60_jm_tbl(
    shcode char(6) PRIMARY KEY,            -- 종목코드
    hname char(40),                        -- 종목명
    cur_date char(20),                     -- 현재 날짜
    avg5price char(12),                    -- 5일 평균가
    avg20price char(12),                   -- 20일 평균가
    avg60price char(12),                   -- 60일 평균가
    avg5vol char(12),                      -- 5일 평균 거래량
    avg20vol char(12),                     -- 20일 평균 거래량
    avg60vol char(12),                     -- 60일 평균 거래량
    revinfo char(20)                       -- 예비 정보
) ENGINE = MyISAM
```

### 8. 이동평균 이력 테이블 (avg_5_20_60_jmlist_tbl)

**용도**: 종목별 이동평균 정보 이력

**테이블 구조**:
```sql
CREATE TABLE avg_5_20_60_jmlist_tbl(
    shcode char(6),                        -- 종목코드
    hname char(40),                        -- 종목명
    cur_date char(20),                     -- 현재 날짜
    avg5price char(12),                    -- 5일 평균가
    avg20price char(12),                   -- 20일 평균가
    avg60price char(12),                   -- 60일 평균가
    avg5vol char(12),                      -- 5일 평균 거래량
    avg20vol char(12),                     -- 20일 평균 거래량
    avg60vol char(12),                     -- 60일 평균 거래량
    revinfo char(20),                      -- 예비 정보
    PRIMARY KEY(shcode, cur_date)
) ENGINE = MyISAM
```

### 9. 프로그램 매매 정보 테이블 (t1637_tbl)

**용도**: 프로그램 매매 정보 저장 (XingAPI T1637 TR 결과)

**테이블 구조**:
```sql
CREATE TABLE t1637_tbl(
    shcode char(6),                        -- 종목코드
    cur_date char(20),                     -- 현재 날짜
    time char(12),                         -- 시간
    price char(12),                        -- 가격
    sign char(4),                          -- 부호
    changevalue char(12),                 -- 변화값
    diff char(12),                         -- 차이
    svalue char(18),                       -- 거래대금
    svolume char(18),                      -- 거래량
    PRIMARY KEY(shcode, cur_date, time)
) ENGINE = MyISAM
```

### 10. 종목 리스트 테이블 (dbList_tbl)

**용도**: 종목 검색 리스트

**테이블 구조**:
```sql
CREATE TABLE dbList_tbl(
    shcode char(6) PRIMARY KEY,             -- 종목코드
    cur_date datetime,                     -- 현재 날짜
    hname char(40),                        -- 종목명
    mode1 char(5),                         -- 모드1
    mode2 char(5),                         -- 모드2
    price char(8)                          -- 가격
)
```

### 11. 분석 리스트 테이블 (analyList_tbl)

**용도**: 종목 분석 정보 (백업 DB)

**테이블 구조**:
```sql
CREATE TABLE analyList_tbl(
    shcode char(6) PRIMARY KEY,            -- 종목코드
    hname char(30),                        -- 종목명
    cur_date char(30),                     -- 현재 날짜
    curprice char(12),                     -- 현재가
    diff char(6),                          -- 차이
    sms char(12),                          -- 단기 이동평균
    smsval char(12),                       -- 단기 이동평균값
    sinc char(9),                          -- 증가
    fsms char(12),                         -- 장기 이동평균
    fsmsval char(12),                      -- 장기 이동평균값
    totvol char(15),                       -- 총 거래량
    totval char(12),                       -- 총 거래대금
    rate char(5),                          -- 비율
    twentyflag char(4),                    -- 20일 플래그
    newprice char(4),                      -- 신규 가격
    psms char(12)                          -- 이전 이동평균
)
```

## 데이터 타입 설명

### 문자열 필드
- `char(n)`: 고정 길이 문자열 (n자)
- `varchar(n)`: 가변 길이 문자열 (최대 n자)

### 암호화 필드
- `VARBINARY(64)`: AES 암호화된 바이너리 데이터

### 날짜/시간 필드
- `char(30)`: 날짜 문자열 (예: "2024-01-01")
- `char(20)`: 날짜시간 문자열 (예: "2024-01-01 09:00:00")
- `datetime`: MySQL datetime 타입

## 데이터 수집 프로젝트 연동

별도의 데이터 수집 프로젝트에서 다음 테이블들을 업데이트해야 합니다:

1. **avg_5_20_60_jm_tbl**: 이동평균 정보
2. **avg_5_20_60_jmlist_tbl**: 이동평균 이력
3. **t1637_tbl**: 프로그램 매매 정보
4. **dbList_tbl**: 종목 리스트

## 데이터 포맷 예시

### 계좌 정보 조회
```cpp
// cfg_tbl에서 idindex='1' 조회
SELECT countid, AES_DECRYPT(countpw, '12345678'), 
       loginid, AES_DECRYPT(loginpw, '23456781'),
       AES_DECRYPT(verifypw, '34567812'),
       msvalue, msrate, mdlevel, sunik, maxsunik, 
       sonjel, maxsonjel, allcount 
FROM cfg_tbl 
WHERE idindex = '1'
```

### 잔고 정보 조회
```cpp
// date_jango_tbl에서 특정 종목의 잔고 조회
SELECT * FROM date_jango_tbl 
WHERE shcode = '005930' AND codemode = '0'
ORDER BY cur_date DESC
```

## 주의사항

1. **암호화 키**: 현재 암호화 키가 하드코딩되어 있어 보안상 위험합니다. 향후 개선 필요.
2. **문자 인코딩**: MySQL 연결 시 `euckr` 문자셋 사용
3. **날짜 형식**: 날짜는 문자열로 저장되며 형식이 일관되어야 함
4. **계좌 인덱스**: 기본 계좌 인덱스는 "1"로 고정

## 향후 개선 사항

1. 데이터 포맷 버전 관리
2. 암호화 키 관리 개선
3. 데이터 검증 로직 추가
4. 데이터 마이그레이션 스크립트

