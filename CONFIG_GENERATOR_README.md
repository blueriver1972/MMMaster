# ConfigGenerator 사용 가이드

## 개요

`ConfigGenerator.exe`는 MMMaster 프로그램의 `config.ini` 설정 파일을 생성하는 유틸리티입니다.

## 빌드 방법

### Visual Studio에서 빌드

1. `MMMaster.sln` 솔루션 파일 열기
2. 솔루션 탐색기에서 `ConfigGenerator` 프로젝트 선택
3. 빌드 → 프로젝트 빌드 (또는 F7)
4. 빌드된 실행 파일: `Release/ConfigGenerator.exe` 또는 `Debug/ConfigGenerator.exe`

### 명령줄에서 빌드

```powershell
# Release 빌드
msbuild ConfigGenerator.vcxproj /p:Configuration=Release /p:Platform=x64

# Debug 빌드
msbuild ConfigGenerator.vcxproj /p:Configuration=Debug /p:Platform=x64
```

## 사용 방법

1. **ConfigGenerator.exe 실행**
   ```
   ConfigGenerator.exe
   ```

2. **계좌 정보 입력**
   - 계좌 인덱스 (기본값: 1)
   - 계좌번호
   - 계좌 비밀번호 (입력 시 마스킹됨)
   - 로그인 ID
   - 로그인 비밀번호 (입력 시 마스킹됨)
   - 공인인증서 비밀번호 (입력 시 마스킹됨)

3. **암호화 키 입력** (선택사항)
   - 기본값 사용: Enter 키
   - 커스텀 키 사용: 원하는 암호화 키 입력

4. **config.ini 파일 생성**
   - 프로그램 실행 디렉토리에 `config.ini` 파일이 생성됩니다.
   - 파일 위치는 화면에 표시됩니다.

## 주의사항

1. **보안**
   - 생성된 `config.ini` 파일은 암호화된 정보를 포함합니다.
   - 파일을 안전하게 보관하세요.
   - 파일이 없거나 손상되면 MMMaster 프로그램이 실행되지 않을 수 있습니다.

2. **기존 파일 덮어쓰기**
   - 기존 `config.ini` 파일이 있으면 덮어쓰기 확인을 받습니다.
   - 'y'를 입력하면 덮어쓰고, 'n'을 입력하면 취소됩니다.

3. **암호화 키**
   - 암호화 키는 개발자가 관리합니다.
   - 같은 키를 사용해야 암호화/복호화가 정상적으로 작동합니다.
   - 키를 변경하면 기존 설정 파일을 읽을 수 없게 됩니다.

## 문제 해결

### 빌드 오류: MFC 라이브러리 없음
- Visual Studio 설치 시 "MFC and ATL support" 선택 확인
- 프로젝트 속성 → 일반 → MFC 사용: "정적 라이브러리에서 MFC 사용" 선택

### 실행 오류: DLL 없음
- Visual C++ 재배포 가능 패키지 설치 필요
- 또는 Debug 빌드 사용 (정적 링크)

### config.ini 파일이 생성되지 않음
- 프로그램 실행 디렉토리에 쓰기 권한 확인
- 관리자 권한으로 실행 시도

## 예시

```
========================================
  MMMaster Config Generator
  config.ini 파일 생성 유틸리티
========================================

=== 계좌 정보 입력 ===
계좌 인덱스 (기본값: 1): 1
계좌번호: 20080800101
계좌 비밀번호: ********
로그인 ID: userid
로그인 비밀번호: ********
공인인증서 비밀번호: ********

=== 암호화 설정 ===
암호화 키 (기본값 사용하려면 Enter): 

config.ini 파일을 생성하는 중...

[성공] config.ini 파일이 생성되었습니다.
위치: D:\stock\ebest\20180327\MMMaster\config.ini

주의: config.ini 파일을 안전하게 보관하세요.
      이 파일이 없으면 프로그램이 실행되지 않을 수 있습니다.

아무 키나 누르면 종료합니다...
```

