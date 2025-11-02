# MMMaster 프로젝트 빌드 및 실행 가이드

## 📋 목차
1. [필수 요구사항](#필수-요구사항)
2. [환경 설정](#환경-설정)
3. [프로젝트 빌드](#프로젝트-빌드)
4. [실행 방법](#실행-방법)
5. [문제 해결](#문제-해결)

---

## 필수 요구사항

### 소프트웨어 요구사항

#### 1. **Visual Studio**
- **Visual Studio 2008** (원본 프로젝트용)
- 또는 **Visual Studio 2019/2022** (업그레이드 필요 시)
- Windows SDK
- MFC (Microsoft Foundation Classes)

#### 2. **이베스트 XingAPI**
- **xingAPI.dll**: 이베스트 증권사 API DLL
- **설치 경로**: 프로젝트 루트 또는 Release/Debug 폴더
- 다운로드: [이베스트 개발자 센터](https://www.ebestsec.co.kr/)

#### 3. **MySQL**
- **MySQL Server**: 데이터베이스 서버
- **MySQL Client Library**: 
  - `libmysql.lib` (정적 라이브러리)
  - `libmysql.dll` (동적 라이브러리)
- 또는 MySQL Connector/C

#### 4. **Windows 환경**
- Windows 7 이상 (권장: Windows 10/11)
- Visual C++ Redistributable 패키지

---

## 환경 설정

### 1. Visual Studio 설치

#### 옵션 A: Visual Studio 2008 사용 (원본 환경)
```powershell
# Visual Studio 2008 설치 (이미 구버전이므로 새로 설치 불가능)
# 대신 Visual Studio 2019/2022에서 vcproj 변환 필요
```

#### 옵션 B: Visual Studio 2019/2022 사용 (권장)
1. Visual Studio 2019/2022 Community 이상 버전 다운로드
2. 설치 시 다음 구성 요소 선택:
   - **Desktop development with C++**
   - **MFC and ATL support**
   - **Windows 10/11 SDK**

### 2. XingAPI 다운로드 및 설정

```powershell
# 1. 이베스트 개발자 센터에서 XingAPI 다운로드
# 2. 다음 파일들을 프로젝트 폴더에 복사:
xingAPI.dll          -> MMMaster\Release\
xingAPI.ini          -> MMMaster\Release\
XA_Common.dll        -> MMMaster\Release\
XA_DataSet.dll       -> MMMaster\Release\
XA_Session.dll       -> MMMaster\Release\
inicore_v2.4.3.1.dll -> MMMaster\Release\
inicrypto_v5.2.0.1.dll -> MMMaster\Release\
inipki_v5.2.7.1.dll  -> MMMaster\Release\
inisafenet_v7.2.dll  -> MMMaster\Release\
```

### 3. MySQL 설치 및 설정

#### MySQL Server 설치
```powershell
# MySQL Community Server 다운로드 및 설치
# https://dev.mysql.com/downloads/mysql/
```

#### MySQL Client Library 다운로드
```powershell
# MySQL Connector/C 다운로드
# https://dev.mysql.com/downloads/connector/c/
```

#### 프로젝트에 MySQL 라이브러리 연결
1. MySQL 라이브러리 파일 위치 확인:
   ```
   MySQL Connector/C/lib/vs14/libmysql.lib
   MySQL Connector/C/lib/vs14/libmysql.dll
   ```

2. 프로젝트 설정에서 라이브러리 경로 추가:
   - **Additional Library Directories**: MySQL lib 경로 추가
   - **Additional Dependencies**: `libmysql.lib` 추가
   - DLL 파일은 실행 파일과 같은 폴더에 복사

### 4. 프로젝트 파일 업그레이드 (선택사항)

Visual Studio 2019/2022를 사용하는 경우:

```powershell
# 1. Visual Studio에서 MMMaster.sln 열기
# 2. 자동으로 vcproj -> vcxproj 변환 제안됨
# 3. 변환 수락
# 4. 플랫폼 도구 집합을 v142 또는 v143으로 변경
```

---

## 프로젝트 빌드

### 방법 1: Visual Studio IDE에서 빌드

1. **프로젝트 열기**
   ```
   MMMaster.sln 파일을 Visual Studio로 엽니다.
   ```

2. **플랫폼 설정**
   - **Configuration**: Release 또는 Debug
   - **Platform**: Win32

3. **빌드 실행**
   ```
   빌드(B) > 솔루션 빌드(Ctrl+Shift+B)
   ```

4. **빌드 출력 확인**
   ```
   Release/MMMaster.exe 생성 확인
   ```

### 방법 2: 명령줄에서 빌드

#### Visual Studio 2019/2022
```powershell
# Developer Command Prompt 열기
cd D:\stock\ebest\20180327\MMMaster

# MSBuild로 빌드
msbuild MMMaster.sln /p:Configuration=Release /p:Platform=Win32
```

#### Visual Studio 2008 (구버전)
```cmd
# Visual Studio 2008 Command Prompt
cd /d D:\stock\ebest\20180327\MMMaster
devenv MMMaster.sln /build Release
```

### 방법 3: CMake 빌드 (새로 생성)

프로젝트를 현대화하려면 CMake 빌드 시스템을 추가할 수 있습니다:

```cmake
# CMakeLists.txt (새로 생성)
cmake_minimum_required(VERSION 3.15)
project(MMMaster)

set(CMAKE_CXX_STANDARD 11)

# MFC 사용
set(CMAKE_MFC_FLAG 2)

# 소스 파일
file(GLOB_RECURSE SOURCES "*.cpp" "*.h")
add_executable(MMMaster ${SOURCES})

# 라이브러리 링크
target_link_libraries(MMMaster
    xingapi
    mysqlclient
)

# 포함 디렉토리
target_include_directories(MMMaster PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/packet
)
```

---

## 실행 방법

### 1. 필수 파일 확인

실행 전 다음 파일들이 `Release` 폴더에 있는지 확인:

```
Release/
├── MMMaster.exe
├── xingAPI.dll
├── xingAPI.ini
├── XA_Common.dll
├── XA_DataSet.dll
├── XA_Session.dll
├── inicore_v2.4.3.1.dll
├── inicrypto_v5.2.0.1.dll
├── inipki_v5.2.7.1.dll
├── inisafenet_v7.2.dll
├── libmysql.dll (MySQL Client)
├── Login.ini
└── Sys/
    ├── Guide.ini
    ├── webinfo.ini
    └── ...
```

### 2. 데이터베이스 설정

MySQL 데이터베이스를 먼저 설정해야 합니다:

```sql
-- MySQL에서 데이터베이스 생성
CREATE DATABASE mmmaster_db CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

-- 사용자 생성 및 권한 부여
CREATE USER 'mmmaster_user'@'localhost' IDENTIFIED BY 'your_password';
GRANT ALL PRIVILEGES ON mmmaster_db.* TO 'mmmaster_user'@'localhost';
FLUSH PRIVILEGES;
```

### 3. 설정 파일 수정

#### `Login.ini` 확인
```ini
[LOGIN]
MACAddr=YOUR_MAC_ADDRESS
```

#### `StdAfx.h` 수정 (보안상 권장)
하드코딩된 인증 정보를 제거하고 설정 파일로 이동:

```cpp
// 기존 (위험)
#define STR_ID              "k8096"
#define STR_PASSWORD_ID     "kim1972"

// 개선안: 설정 파일에서 읽기
// 또는 환경 변수 사용
```

### 4. 애플리케이션 실행

```powershell
cd Release
.\MMMaster.exe
```

### 5. 로그인

1. 애플리케이션 실행
2. **연결** 버튼 클릭
3. 로그인 정보 입력:
   - 사용자 ID
   - 비밀번호
   - 공인인증서 비밀번호
4. 로그인 성공 시 계좌 정보 표시

---

## 문제 해결

### 빌드 오류

#### 오류 1: MFC 라이브러리를 찾을 수 없음
```
해결: Visual Studio 설치 관리자에서 "MFC and ATL support" 설치
```

#### 오류 2: xingAPI.dll을 찾을 수 없음
```
해결: 
1. xingAPI.dll을 Release 폴더에 복사
2. 또는 시스템 PATH에 추가
```

#### 오류 3: MySQL 라이브러리 오류
```
오류: unresolved external symbol _mysql_init@4
해결:
1. libmysql.lib가 Additional Dependencies에 포함되었는지 확인
2. libmysql.dll이 실행 파일과 같은 폴더에 있는지 확인
3. MySQL Connector/C 버전 확인
```

#### 오류 4: 문자 인코딩 오류
```
오류: 한글 주석이 깨짐
해결:
1. Visual Studio: 파일 > 고급 저장 옵션 > 인코딩: 한국어(EUC-KR)
2. 또는 프로젝트 속성 > C/C++ > 명령줄에 /source-charset:utf-8 추가
```

### 실행 오류

#### 오류 1: 연결 실패
```
증상: XingAPI 서버 연결 실패
해결:
1. 인터넷 연결 확인
2. 방화벽 설정 확인
3. xingAPI.ini 설정 확인
4. 이베스트 HTS가 정상 작동하는지 확인
```

#### 오류 2: 로그인 실패
```
증상: 로그인 오류
해결:
1. ID/비밀번호 확인
2. 공인인증서 설치 확인
3. 공인인증서 비밀번호 확인
4. 계좌번호 확인
```

#### 오류 3: 데이터베이스 연결 실패
```
증상: MySQL 연결 오류
해결:
1. MySQL 서버 실행 확인
2. 연결 정보 확인 (주소, 포트, 사용자, 비밀번호)
3. MySQLCMD.cpp의 db_init() 함수에서 연결 정보 확인
```

### 디버깅 팁

1. **로그 확인**
   ```
   애플리케이션의 로그 창에서 오류 메시지 확인
   ```

2. **디버그 빌드**
   ```
   Debug 모드로 빌드하여 상세한 오류 정보 확인
   ```

3. **의존성 확인**
   ```
   Dependency Walker로 누락된 DLL 확인
   https://www.dependencywalker.com/
   ```

---

## 추가 리소스

### 참고 문서
- [이베스트 XingAPI 개발자 가이드](https://www.ebestsec.co.kr/)
- [MySQL C API 참조](https://dev.mysql.com/doc/c-api/8.0/en/)
- [MFC 클래스 라이브러리](https://docs.microsoft.com/en-us/cpp/mfc/)

### 지원
- 이베스트 기술 지원
- MySQL 커뮤니티 포럼

---

## 빠른 시작 체크리스트

- [ ] Visual Studio 설치 (MFC 지원 포함)
- [ ] XingAPI 다운로드 및 DLL 파일 복사
- [ ] MySQL 설치 및 데이터베이스 생성
- [ ] MySQL Client Library 다운로드 및 설정
- [ ] 프로젝트 빌드 성공
- [ ] 필수 DLL 파일 확인
- [ ] 설정 파일 수정 (로그인 정보 등)
- [ ] 애플리케이션 실행 및 로그인 테스트

---

## 다음 단계

프로젝트가 성공적으로 빌드되고 실행되면:

1. **기능 테스트**: 각 모듈별 기능 확인
2. **데이터베이스 연동 테스트**: MySQL 저장/조회 테스트
3. **실시간 데이터 테스트**: XingAPI 실시간 데이터 수신 확인
4. **매매 시뮬레이션**: 모의투자 계좌로 테스트

주의: 실제 계좌로 테스트하기 전에 충분한 시뮬레이션을 권장합니다.

