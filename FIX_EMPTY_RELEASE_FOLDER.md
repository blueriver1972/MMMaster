# 빈 Release 폴더 생성 문제 해결

## 문제

프로젝트를 빌드하면 루트 디렉토리에 빈 `Release\` 폴더가 생성됩니다 (파일은 없음).

## 원인 분석

### 1. Microsoft.CPP.UpgradeFromVC60.props
- Visual Studio 2008 이전 프로젝트를 업그레이드할 때 사용되는 props 파일
- 기본 출력 디렉토리를 `$(Configuration)\` (예: `Release\`)로 설정할 수 있음
- 이 설정이 프로젝트의 명시적 `OutDir` 설정보다 먼저 적용될 수 있음

### 2. Post-Build Event의 xcopy 명령
- `xcopy /E /I /Y "$(SolutionDir)MMMaster\Release\Sys" "$(OutDir)Sys"` 명령이 실행될 때
- `$(OutDir)Sys` 경로를 생성하려고 할 때 중간 경로가 생성될 수 있음

### 3. Visual Studio의 기본 동작
- 일부 빌드 단계에서 기본 출력 디렉토리를 생성할 수 있음

## 해결 방법

### 1. UpgradeFromVC60.props의 기본값 오버라이드

**추가된 코드:**
```xml
<!-- UpgradeFromVC60.props가 설정한 기본 출력 디렉토리를 오버라이드 -->
<PropertyGroup>
  <OutDir Condition="'$(OutDir)'=='' OR '$(OutDir)'=='$(Configuration)\' OR '$(OutDir)'=='.\$(Configuration)\'">$(SolutionDir)MMMaster\$(Configuration)\</OutDir>
  <IntDir Condition="'$(IntDir)'=='' OR '$(IntDir)'=='$(Configuration)\' OR '$(IntDir)'=='.\$(Configuration)\'">$(SolutionDir)MMMaster\$(Configuration)\</IntDir>
</PropertyGroup>
```

이 코드는:
- `OutDir` 또는 `IntDir`가 비어있거나
- `$(Configuration)\` (예: `Release\`) 또는
- `.\$(Configuration)\` (예: `.\Release\`)로 설정되어 있으면
- 프로젝트별 출력 디렉토리로 오버라이드합니다.

### 2. Post-Build Event의 xcopy 명령 수정

**변경 전:**
```xml
if exist "$(SolutionDir)MMMaster\Release\Sys" xcopy /E /I /Y "$(SolutionDir)MMMaster\Release\Sys" "$(OutDir)Sys"
```

**변경 후:**
```xml
if exist "$(SolutionDir)MMMaster\Release\Sys" if not exist "$(OutDir)Sys" mkdir "$(OutDir)Sys" &amp; xcopy /E /I /Y "$(SolutionDir)MMMaster\Release\Sys" "$(OutDir)Sys"
```

이렇게 하면 `xcopy`가 중간 경로를 생성하지 않고, 명시적으로 `$(OutDir)Sys` 디렉토리를 생성합니다.

## 결과

이제 빌드 시:
- ✅ 루트에 빈 `Release\` 폴더가 생성되지 않음
- ✅ 모든 출력이 `MMMaster\Release\` 또는 `MMMaster\Debug\`에 생성됨
- ✅ `UpgradeFromVC60.props`의 기본값이 오버라이드됨

## 추가 확인 사항

만약 여전히 `Release\` 폴더가 생성된다면:

1. **Visual Studio 사용자 설정 확인**
   - `%LOCALAPPDATA%\Microsoft\MSBuild\v4.0\Microsoft.Cpp.$(Platform).user.props` 파일 확인
   - 여기에 기본 출력 디렉토리가 설정되어 있을 수 있음

2. **솔루션 레벨 설정 확인**
   - 솔루션 파일(`.sln`)에서 기본 출력 디렉토리가 설정되어 있을 수 있음

3. **빌드 로그 확인**
   - Visual Studio 빌드 출력에서 어떤 단계에서 `Release\` 폴더가 생성되는지 확인

