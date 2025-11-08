# 빌드 출력 디렉토리 구조 설명

## 현재 구조

### ✅ 사용 중인 디렉토리 (프로젝트 설정)

프로젝트 파일(`.vcxproj`)에서 명시적으로 설정된 출력 디렉토리:

#### MMMaster 프로젝트
- `MMMaster\Debug\` - Win32 Debug 빌드 출력
- `MMMaster\Release\` - Win32 Release 빌드 출력
- `MMMaster\x64\Debug\` - x64 Debug 빌드 출력
- `MMMaster\x64\Release\` - x64 Release 빌드 출력

#### ConfigGenerator 프로젝트
- `ConfigGenerator\Debug\` - Win32 Debug 빌드 출력
- `ConfigGenerator\Release\` - Win32 Release 빌드 출력
- `ConfigGenerator\x64\Debug\` - x64 Debug 빌드 출력
- `ConfigGenerator\x64\Release\` - x64 Release 빌드 출력

### ❌ 중복/사용 안 하는 디렉토리 (루트)

루트 디렉토리에 있는 이전 빌드 결과물:
- `Debug\` - 이전 빌드 결과물 (삭제 가능)
- `Release\` - 이전 빌드 결과물 (삭제 가능)
- `x64\` - 이전 빌드 결과물 (삭제 가능)

## 왜 중복이 발생했나?

1. **이전 프로젝트 설정**: Visual Studio의 기본 출력 디렉토리 설정 사용
2. **프로젝트 수정 후**: 출력 디렉토리를 프로젝트별로 분리하도록 변경
3. **이전 빌드 결과물**: 루트의 `Debug\`, `Release\`, `x64\`는 이전 빌드 결과물

## 해결 방법

루트에 있는 중복 디렉토리는 삭제해도 됩니다:
- `Debug\` 삭제 가능
- `Release\` 삭제 가능
- `x64\` 삭제 가능

이들은 빌드 결과물이므로 언제든지 재생성 가능합니다.

## 프로젝트 설정 확인

### ETrade_Order_Sample.vcxproj
```xml
<OutDir>$(SolutionDir)MMMaster\Debug\</OutDir>
<OutDir>$(SolutionDir)MMMaster\Release\</OutDir>
<OutDir>$(SolutionDir)MMMaster\x64\Debug\</OutDir>
<OutDir>$(SolutionDir)MMMaster\x64\Release\</OutDir>
```

### ConfigGenerator.vcxproj
```xml
<OutDir>$(SolutionDir)ConfigGenerator\Debug\</OutDir>
<OutDir>$(SolutionDir)ConfigGenerator\Release\</OutDir>
<OutDir>$(SolutionDir)ConfigGenerator\x64\Debug\</OutDir>
<OutDir>$(SolutionDir)ConfigGenerator\x64\Release\</OutDir>
```

## 권장 사항

1. 루트의 `Debug\`, `Release\`, `x64\` 폴더 삭제
2. 프로젝트별 출력 디렉토리 구조 유지:
   ```
   MMMaster/
   ├── Debug/
   ├── Release/
   └── x64/
       ├── Debug/
       └── Release/
   ConfigGenerator/
   ├── Debug/
   ├── Release/
   └── x64/
       ├── Debug/
       └── Release/
   ```

