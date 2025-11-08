# Release 폴더 자동 생성 문제 해결

## 문제

프로젝트를 빌드하면 루트 디렉토리에 `Release\` 폴더가 자동으로 생성되는 문제가 있었습니다.

## 원인

1. **Post-Build Event**: DLL 파일 복사 시 `$(SolutionDir)Release\` 경로를 참조
2. **컴파일러 설정**: 일부 출력 파일 경로가 `.\Release/`로 하드코딩됨
   - `TypeLibraryName`
   - `PrecompiledHeaderOutputFile`
   - `AssemblerListingLocation`
   - `ObjectFileName`
   - `ProgramDataBaseFileName`
   - `ProgramDatabaseFile`

## 해결 방법

### 1. Post-Build Event 수정

**변경 전:**
```xml
if exist "$(SolutionDir)Release\xingAPI.dll" copy /Y "$(SolutionDir)Release\xingAPI.dll" "$(OutDir)"
```

**변경 후:**
```xml
if exist "$(SolutionDir)MMMaster\Release\xingAPI.dll" copy /Y "$(SolutionDir)MMMaster\Release\xingAPI.dll" "$(OutDir)"
```

### 2. 컴파일러 설정 수정

**변경 전:**
```xml
<TypeLibraryName>.\Release/ETrade_Order_Sample.tlb</TypeLibraryName>
<PrecompiledHeaderOutputFile>.\Release/ETrade_Order_Sample.pch</PrecompiledHeaderOutputFile>
<AssemblerListingLocation>.\Release/</AssemblerListingLocation>
<ObjectFileName>.\Release/</ObjectFileName>
<ProgramDataBaseFileName>.\Release/</ProgramDataBaseFileName>
<ProgramDatabaseFile>.\Release/ETrade_Order_Sample.pdb</ProgramDatabaseFile>
```

**변경 후:**
```xml
<TypeLibraryName>$(IntDir)ETrade_Order_Sample.tlb</TypeLibraryName>
<PrecompiledHeaderOutputFile>$(IntDir)ETrade_Order_Sample.pch</PrecompiledHeaderOutputFile>
<AssemblerListingLocation>$(IntDir)</AssemblerListingLocation>
<ObjectFileName>$(IntDir)</ObjectFileName>
<ProgramDataBaseFileName>$(IntDir)</ProgramDataBaseFileName>
<ProgramDatabaseFile>$(IntDir)ETrade_Order_Sample.pdb</ProgramDatabaseFile>
```

## 결과

이제 빌드 시:
- ✅ 루트에 `Release\` 폴더가 생성되지 않음
- ✅ 모든 출력이 `MMMaster\Release\` 또는 `MMMaster\Debug\`에 생성됨
- ✅ 프로젝트별로 출력 디렉토리가 분리됨

## 참고

- `$(IntDir)`: 중간 파일 디렉토리 (프로젝트 설정에 따라 자동 설정)
- `$(OutDir)`: 출력 디렉토리 (프로젝트 설정에 따라 자동 설정)
- `$(SolutionDir)`: 솔루션 디렉토리

