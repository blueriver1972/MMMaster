# MMMaster 실행 환경 구성 가이드

이 문서는 MMMaster 프로젝트를 실행할 수 있는 환경을 구성하는 단계별 가이드를 제공합니다.

## 🎯 빠른 시작

가장 빠른 방법으로 실행 환경을 구성하려면 다음 순서를 따르세요:

1. [Visual Studio 설치](#1-visual-studio-설치)
2. [XingAPI 다운로드](#2-xingapi-다운로드)
3. [MySQL 설치](#3-mysql-설치)
4. [프로젝트 빌드](#4-프로젝트-빌드)

---

## 1. Visual Studio 설치

### 1.1 Visual Studio 2019/2022 설치 (권장)

1. **Visual Studio 다운로드**
   - https://visualstudio.microsoft.com/downloads/
   - Community 버전 (무료) 사용 가능

2. **필수 구성 요소 선택**
   설치 중 다음 항목을 선택하세요:
   
   ✅ **Desktop development with C++**
   - ✅ MSVC v142 또는 v143 컴파일러 도구 집합
   - ✅ Windows 10/11 SDK
   - ✅ **MFC and ATL support** ⭐ (필수)
   - ✅ C++ CMake tools for Windows (선택)

3. **설치 완료 후 확인**
   ```powershell
   # Developer Command Prompt 열기
   # 또는 PowerShell에서:
   & "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\LaunchVsDevShell.ps1"
   ```

### 1.2 프로젝트 파일 변환

Visual Studio 2019/2022에서 `.sln` 파일을 열면 자동으로 변환 제안이 나타납니다:

1. `MMMaster.sln` 파일을 Visual Studio로 엽니다
2. "프로젝트를 현재 버전으로 변환" 선택
3. 플랫폼 도구 집합을 **v142** (VS2019) 또는 **v143** (VS2022)로 변경

---

## 2. XingAPI 다운로드

### 2.1 이베스트 개발자 센터 가입

1. https://www.ebestsec.co.kr/ 접속
2. 개발자 센터 가입 (이베스트 계좌 필요)
3. API 다운로드 섹션에서 XingAPI 다운로드

### 2.2 DLL 파일 배치

다운로드한 XingAPI에서 다음 파일들을 프로젝트 폴더로 복사:

```
MMMaster/
├── Release/
│   ├── xingAPI.dll
│   ├── xingAPI.ini
│   ├── XA_Common.dll
│   ├── XA_DataSet.dll
│   ├── XA_Session.dll
│   ├── inicore_v2.4.3.1.dll
│   ├── inicrypto_v5.2.0.1.dll
│   ├── inipki_v5.2.7.1.dll
│   └── inisafenet_v7.2.dll
└── Debug/ (동일한 파일들)
```

### 2.3 xingAPI.ini 설정 확인

```ini
[SETTING]
ServerAddress=your_server_address
ServerPort=your_port
```

---

## 3. MySQL 설치

### 3.1 MySQL Server 설치

1. **MySQL Community Server 다운로드**
   - https://dev.mysql.com/downloads/mysql/
   - Windows용 설치 패키지 선택

2. **설치 옵션**
   - 설치 타입: **Developer Default** 권장
   - 포트: 기본 3306
   - root 비밀번호 설정 (기억해두세요!)

3. **설치 확인**
   ```powershell
   mysql --version
   ```

### 3.2 MySQL Connector/C 설치

1. **MySQL Connector/C 다운로드**
   - https://dev.mysql.com/downloads/connector/c/
   - Windows (x86, 64-bit) ZIP Archive 선택

2. **압축 해제 및 경로 확인**
   ```
   C:\Program Files\MySQL\MySQL Connector C 8.0\
   ├── include\
   │   └── mysql.h
   └── lib\
       ├── vs14\
       │   ├── libmysql.lib
       │   └── libmysql.dll
       └── ...
   ```

### 3.3 프로젝트에 MySQL 라이브러리 연결

#### Visual Studio에서 설정:

1. **프로젝트 속성 열기**
   - 솔루션 탐색기에서 프로젝트 우클릭
   - 속성 선택

2. **C/C++ > 일반 > 추가 포함 디렉토리**
   ```
   C:\Program Files\MySQL\MySQL Connector C 8.0\include
   ```

3. **링커 > 일반 > 추가 라이브러리 디렉토리**
   ```
   C:\Program Files\MySQL\MySQL Connector C 8.0\lib\vs14
   ```

4. **링커 > 입력 > 추가 종속성**
   ```
   libmysql.lib
   ```

5. **DLL 복사**
   - `libmysql.dll`을 `Release` 및 `Debug` 폴더에 복사

### 3.4 데이터베이스 생성

```sql
-- MySQL 콘솔에서 실행
CREATE DATABASE mmmaster_db 
CHARACTER SET utf8mb4 
COLLATE utf8mb4_unicode_ci;

-- 사용자 생성
CREATE USER 'mmmaster_user'@'localhost' 
IDENTIFIED BY 'your_secure_password';

-- 권한 부여
GRANT ALL PRIVILEGES ON mmmaster_db.* 
TO 'mmmaster_user'@'localhost';

FLUSH PRIVILEGES;
```

---

## 4. 프로젝트 빌드

### 4.1 Visual Studio에서 빌드

1. `MMMaster.sln` 파일을 Visual Studio로 엽니다
2. 상단 툴바에서:
   - **Configuration**: Release 선택
   - **Platform**: Win32 선택
3. **빌드 > 솔루션 빌드** (Ctrl+Shift+B)

### 4.2 PowerShell 스크립트로 빌드

```powershell
# 프로젝트 루트 디렉토리에서
.\build.ps1 -Configuration Release
```

### 4.3 명령줄에서 빌드

```powershell
# Developer Command Prompt 또는 VS Developer PowerShell
cd D:\stock\ebest\20180327\MMMaster
msbuild MMMaster.sln /p:Configuration=Release /p:Platform=Win32
```

---

## 5. 실행 준비

### 5.1 필수 파일 확인

`Release` 폴더에 다음 파일들이 있는지 확인:

```
Release/
├── MMMaster.exe          ✅ 실행 파일
├── xingAPI.dll           ✅ XingAPI
├── XA_Common.dll         ✅ XingAPI 의존성
├── XA_DataSet.dll        ✅ XingAPI 의존성
├── XA_Session.dll        ✅ XingAPI 의존성
├── libmysql.dll          ✅ MySQL 클라이언트
├── Login.ini             ✅ 로그인 설정
└── Sys/                  ✅ 시스템 설정 폴더
```

### 5.2 설정 파일 수정

#### Login.ini
```ini
[LOGIN]
MACAddr=YOUR_MAC_ADDRESS
```

#### 보안: 인증 정보 처리

**⚠️ 중요**: `StdAfx.h`에 하드코딩된 인증 정보가 있습니다.

실행 전 다음 중 하나를 수행하세요:

1. **임시로 수정** (개발용)
   ```cpp
   // StdAfx.h에서 실제 인증 정보로 변경
   #define STR_ID              "your_id"
   #define STR_PASSWORD_ID     "your_password"
   ```

2. **설정 파일로 이동** (권장)
   - 인증 정보를 별도 설정 파일로 분리
   - 또는 암호화하여 저장

---

## 6. 실행 및 테스트

### 6.1 실행

```powershell
cd Release
.\MMMaster.exe
```

### 6.2 로그인 테스트

1. 애플리케이션 시작
2. **연결** 버튼 클릭
3. 로그인 정보 입력
4. 연결 성공 확인

### 6.3 기능 테스트 체크리스트

- [ ] 서버 연결 성공
- [ ] 로그인 성공
- [ ] 계좌 정보 조회
- [ ] 실시간 주가 수신
- [ ] 데이터베이스 연결 (MySQL)
- [ ] 잔고 조회
- [ ] 주문 테스트 (모의투자 권장)

---

## 7. 문제 해결

### 문제 1: DLL을 찾을 수 없음

**증상**: 
```
오류: xingAPI.dll을 찾을 수 없습니다.
```

**해결**:
1. DLL 파일이 실행 파일과 같은 폴더에 있는지 확인
2. 시스템 PATH에 추가 (권장하지 않음)
3. Dependency Walker로 누락된 DLL 확인

### 문제 2: MySQL 연결 실패

**증상**: 데이터베이스 연결 오류

**해결**:
1. MySQL 서버가 실행 중인지 확인
   ```powershell
   Get-Service MySQL*
   ```
2. `MySQLCMD.cpp`의 `db_init()` 함수에서 연결 정보 확인
3. 방화벽 설정 확인

### 문제 3: 빌드 오류

**증상**: 링커 오류 (unresolved external symbol)

**해결**:
1. MySQL 라이브러리 경로 확인
2. 라이브러리 이름 확인 (libmysql.lib)
3. 프로젝트 속성 > 링커 > 추가 종속성 확인

### 문제 4: 인코딩 문제

**증상**: 한글 주석이 깨짐

**해결**:
1. Visual Studio: 파일 > 고급 저장 옵션
2. 인코딩: 한국어 (EUC-KR) 또는 UTF-8 with BOM

---

## 8. 추가 리소스

### 참고 링크

- [이베스트 XingAPI 문서](https://www.ebestsec.co.kr/)
- [MySQL C API 참조](https://dev.mysql.com/doc/c-api/8.0/en/)
- [MFC 프로그래밍 가이드](https://docs.microsoft.com/en-us/cpp/mfc/)

### 지원

문제가 지속되면:
1. 이베스트 기술 지원팀에 문의
2. MySQL 커뮤니티 포럼
3. Visual Studio 커뮤니티

---

## ✅ 완료 체크리스트

실행 환경 구성이 완료되었는지 확인하세요:

- [ ] Visual Studio 설치 완료 (MFC 포함)
- [ ] XingAPI DLL 파일 배치 완료
- [ ] MySQL 설치 및 설정 완료
- [ ] MySQL Connector/C 설치 완료
- [ ] 프로젝트 빌드 성공
- [ ] 필수 DLL 파일 확인 완료
- [ ] 설정 파일 수정 완료
- [ ] 애플리케이션 실행 성공
- [ ] 로그인 테스트 성공

---

축하합니다! MMMaster 실행 환경 구성이 완료되었습니다! 🎉

