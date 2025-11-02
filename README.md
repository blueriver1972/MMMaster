# MMMaster - 한국 주식 자동 매매 시스템

이베스트 XingAPI를 활용한 MFC 기반 주식 자동 매매 및 모니터링 시스템입니다.

## 📋 프로젝트 개요

**MMMaster**는 실시간 주가 데이터를 수신하고, 자동 매매 주문을 처리하며, 포트폴리오를 관리하는 Windows 데스크톱 애플리케이션입니다.

### 주요 기능

- ✅ **실시간 주가 모니터링**: 실시간 호가, 체결가, 거래량 수신
- ✅ **자동 매매 시스템**: 조건 기반 자동 매수/매도
- ✅ **포트폴리오 관리**: 보유 종목, 손익, 잔고 관리
- ✅ **패턴 분석**: 주가 패턴 분석 및 매매 신호 생성
- ✅ **데이터베이스 연동**: MySQL을 통한 거래 내역 저장
- ✅ **보안 제어**: config.ini 기반 암호화된 계좌 정보 관리
- ✅ **에러 로깅**: 모든 API/DB 호출에 상세한 에러 로깅
- ✅ **문서화**: 상세한 주석 및 데이터 포맷 문서

## 🚀 빠른 시작

### 필수 요구사항

- Windows 7 이상
- Visual Studio 2019/2022 (MFC 지원)
- 이베스트 XingAPI
- MySQL Server 8.0 이상
- MySQL Connector/C

### 설치 방법

1. **Visual Studio 설치**
   ```
   Visual Studio 2019/2022 Community 이상
   → Desktop development with C++
   → MFC and ATL support 선택
   ```

2. **XingAPI 다운로드**
   - 이베스트 개발자 센터에서 다운로드
   - DLL 파일을 Release/Debug 폴더에 복사

3. **MySQL 설치**
   - MySQL Community Server 설치
   - MySQL Connector/C 다운로드 및 설정

4. **프로젝트 빌드**
   ```powershell
   # PowerShell 스크립트 사용
   .\build.ps1 -Configuration Release
   
   # 또는 Visual Studio에서
   # 솔루션 빌드 (Ctrl+Shift+B)
   ```

자세한 내용은 [실행 환경 구성 가이드](setup_environment.md)를 참조하세요.

## 📁 프로젝트 구조

```
MMMaster/
├── Source Files/          # C++ 소스 파일
├── Header Files/          # 헤더 파일
├── packet/                # 이베스트 TR 패킷 정의 (241개)
├── docs/                  # 참조 문서 (XingAPI 사용자 매뉴얼)
├── Release/               # 릴리스 빌드 출력
├── Debug/                 # 디버그 빌드 출력
├── res/                   # 리소스 파일
├── ConfigManager.h/cpp    # 설정 파일 관리 (보안 제어)
├── PROJECT_ANALYSIS_KR.md # 프로젝트 상세 분석
├── BUILD_GUIDE_KR.md     # 빌드 가이드
├── setup_environment.md  # 환경 구성 가이드
├── REFERENCE_DOCS.md     # 참조 문서 인덱스
├── DATA_FORMAT.md        # 데이터 포맷 문서
├── SECURITY_CONTROL.md   # 보안 제어 구조 문서
└── CMakeLists.txt        # CMake 빌드 파일 (선택)
```

## 📚 문서

### 프로젝트 문서
- [프로젝트 상세 분석](PROJECT_ANALYSIS_KR.md) - 프로젝트 구조 및 기능 설명
- [빌드 가이드](BUILD_GUIDE_KR.md) - 빌드 방법 및 문제 해결
- [실행 환경 구성](setup_environment.md) - 단계별 환경 설정 가이드
- [참조 문서 인덱스](REFERENCE_DOCS.md) - XingAPI 사용자 매뉴얼 및 참조 문서
- [데이터 포맷 문서](DATA_FORMAT.md) - MySQL 테이블 구조 및 데이터 포맷
- [보안 제어 구조](SECURITY_CONTROL.md) - config.ini 기반 보안 제어 시스템

### 참조 문서
- **XingAPI 사용자 매뉴얼**: `docs/` 폴더에 위치
  - `3.pdf` - XingAPI 사용자 매뉴얼 (일반)
  - `5.pdf` - XingAPI 사용자 매뉴얼 (고급)
  
자세한 내용은 [참조 문서 인덱스](REFERENCE_DOCS.md)를 확인하세요.

## 🔧 주요 컴포넌트

### 1. XingAPI 연동
- 서버 연결 및 로그인
- TR 코드 요청 (조회)
- 실시간 데이터 구독

### 2. 주문 처리
- 자동 매수/매도 주문
- 주문 취소
- 체결 확인

### 3. 데이터베이스 연동
- MySQL 연동
- 거래 내역 저장
- 주가 데이터 저장

### 4. 패턴 분석
- 이동평균 분석
- 거래량 분석
- 프로그램 매매 분석

## ⚙️ 설정

### 계좌 정보 설정 (보안 제어)

**config.ini 파일 사용 (권장)**:
- 프로그램 실행 디렉토리에 `config.ini` 파일 생성
- 계좌 정보는 암호화되어 저장됨
- 설정 파일이 없으면 프로그램 실행 불가 (보안 제어)

**config.ini 생성 방법**:
1. 개발자 전용 유틸리티 사용 (ConfigGenerator.exe)
2. 또는 `ConfigManager::InitializeConfigFile()` 함수 사용

자세한 내용은 [보안 제어 구조 문서](SECURITY_CONTROL.md)를 참조하세요.

**기존 방식 (하위 호환성)**:
- `config.ini`가 없으면 DB의 `cfg_tbl` 테이블에서 계좌 정보 읽기
- 기본 계좌 인덱스: "1" (DB index 1)

### 데이터베이스 설정

`MySQLCMD.cpp`에서 MySQL 연결 정보를 설정합니다.

**데이터 포맷**:
- MySQL 테이블 구조 및 데이터 포맷은 [데이터 포맷 문서](DATA_FORMAT.md) 참조

## 🐛 문제 해결

### 빌드 오류

- **MFC 라이브러리 오류**: Visual Studio 설치 시 "MFC and ATL support" 선택 확인
- **MySQL 라이브러리 오류**: MySQL Connector/C 경로 및 라이브러리 링크 확인
- **DLL 오류**: 필수 DLL 파일이 실행 파일과 같은 폴더에 있는지 확인

자세한 내용은 [빌드 가이드](BUILD_GUIDE_KR.md)를 참조하세요.

## 📝 라이선스

이 프로젝트는 개인 사용 목적으로 작성되었습니다.

## 🤝 기여

버그 리포트 및 기능 제안은 이슈로 등록해주세요.

## 📞 지원

- 이베스트 XingAPI 관련: 이베스트 개발자 센터
- MySQL 관련: MySQL 커뮤니티 포럼

## ⚠️ 면책 조항

이 소프트웨어는 교육 및 연구 목적으로 제공됩니다. 실제 매매에 사용할 경우 모든 책임은 사용자에게 있습니다.

---

**Made with ❤️ for Korean Stock Market**

