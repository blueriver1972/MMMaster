# 폴더 및 파일 분석 결과

## 1. 빌드 관련 폴더 분석

### ✅ 사용 중인 폴더 (Visual Studio 2022)

#### `Debug\`, `Release\`
- **용도**: Visual Studio 빌드 출력 디렉토리
- **상태**: 현재 사용 중
- **설정 위치**: `ETrade_Order_Sample.vcxproj`의 `<OutDir>`, `<IntDir>` 속성
- **내용**: 
  - `Debug\`: 디버그 빌드 결과물 (MMMaster.exe, .obj, .pdb 등)
  - `Release\`: 릴리스 빌드 결과물
- **삭제 가능**: ✅ 빌드 결과물이므로 언제든지 재생성 가능

#### `x64\`
- **용도**: x64 플랫폼 빌드 출력 디렉토리
- **상태**: 현재 사용 중 (프로젝트에 x64 구성 포함)
- **내용**: x64 플랫폼용 빌드 결과물
- **삭제 가능**: ✅ 빌드 결과물이므로 언제든지 재생성 가능

#### `MMMaster\Debug\`, `MMMaster\Release\`
- **용도**: 프로젝트 설정에서 분리된 출력 디렉토리
- **상태**: 현재 사용 중 (최근 수정으로 분리된 출력 디렉토리)
- **설정**: `$(SolutionDir)MMMaster\Debug\`, `$(SolutionDir)MMMaster\Release\`
- **삭제 가능**: ✅ 빌드 결과물이므로 언제든지 재생성 가능

### ❓ CMake 관련 폴더

#### `build\`
- **용도**: CMake 빌드 디렉토리 (추정)
- **상태**: CMakeLists.txt가 있지만 현재 Visual Studio 프로젝트 사용 중
- **삭제 가능**: ✅ CMake를 사용하지 않으면 삭제 가능

#### `out\`
- **용도**: 불명확 (CMake 또는 다른 빌드 시스템)
- **상태**: 현재 사용 여부 불명확
- **삭제 가능**: ⚠️ 내용 확인 후 결정

### 📦 백업 폴더

#### `Backup\`
- **용도**: 백업 파일 저장
- **상태**: 백업용
- **삭제 가능**: ⚠️ 중요한 백업이 있을 수 있으므로 확인 후 결정

---

## 2. 불필요한 파일 분석

### 🗑️ 삭제 가능한 파일들

#### Visual Studio 캐시 파일 (.ncb)
- `APIGetting.ncb`
- `ctrlMoni.ncb`
- `EFTMaster.ncb`
- `ETrade_doit.ncb`
- `ETrade_fd.ncb`
- `ETrade_Order_Sample.ncb`
- `ETrade_order.ncb`
- `ETrade_Sh.ncb`
- `ETrade_TR.ncb`
- `FinderMaster.ncb`
- `JMMaster.ncb`
- `Master.ncb`
- `MMMaster.ncb`
- `MkDB.ncb`
- `Moui.ncb`
- `S_ETN.ncb`
- `Siljun.ncb`
- `T1301_che.ncb`
- `TR_Real.ncb`
- **설명**: IntelliSense 캐시 파일, 자동 재생성됨
- **삭제 가능**: ✅

#### 이전 Visual Studio 프로젝트 파일 (.vcproj)
- `ETrade_Order_Sample.vcproj`
- `ETrade_Order_Sample.vcproj.DESKTOP-6HADA34.User.user`
- `ETrade_Order_Sample.vcproj.DESKTOP-CUISER4.user.user`
- `ETrade_Order_Sample.vcproj.DESKTOP-SO7VFB4.user.user`
- **설명**: Visual Studio 2008 이전 버전 프로젝트 파일, 현재 사용 안 함
- **삭제 가능**: ✅ (현재는 .vcxproj 사용)

#### Visual Studio 사용자 설정 파일 (.vcxproj.user)
- `ETrade_Order_Sample.vcxproj.user`
- `ConfigGenerator.vcxproj.user`
- **설명**: 사용자별 설정 (로컬 경로 등), 버전 관리에 포함하지 않음
- **삭제 가능**: ⚠️ 개인 설정이 있을 수 있음 (재생성 가능)

#### Visual Studio 솔루션 사용자 옵션 (.suo)
- `MMMaster.suo`
- **설명**: 사용자별 솔루션 설정
- **삭제 가능**: ✅ 재생성 가능

#### Visual Studio 리소스 파일 (.aps)
- `ETrade_Order_Sample.aps`
- **설명**: 리소스 편집기 캐시 파일
- **삭제 가능**: ✅ 재생성 가능

#### CMake 관련 파일 (CMake 미사용 시)
- `CMakeLists.txt`
- `CMakeSettings.json`
- **설명**: CMake 빌드 시스템 설정 파일
- **삭제 가능**: ⚠️ CMake를 사용하지 않으면 삭제 가능

#### 기타 파일
- `nul`: 빈 파일 (삭제 가능)
- `UpgradeLog.htm`: Visual Studio 업그레이드 로그 (삭제 가능)
- `tsvnsync.tsex`: TortoiseSVN 관련 (삭제 가능)

### 📚 문서 파일 (.md) - 정리 필요

현재 31개의 .md 파일이 있습니다. 일부는 중복되거나 불필요할 수 있습니다:

**필요한 문서:**
- `README.md`: 메인 문서
- `BUILD_GUIDE_KR.md`: 빌드 가이드
- `MASTERKEY_INI_GUIDE.md`: 마스터 키 가이드
- `CONFIG_GENERATOR_README.md`: ConfigGenerator 가이드

**정리 가능한 문서:**
- `FIX_BUILD_ERRORS.md`: 빌드 오류 수정 기록 (과거 이슈)
- `FIX_COMPILER_ERROR.md`: 컴파일러 오류 수정 기록 (과거 이슈)
- `FIX_NEW_H_ERROR.md`: 헤더 오류 수정 기록 (과거 이슈)
- `FIX_CMAKE_ISSUE.md`: CMake 이슈 수정 기록 (과거 이슈)
- `TERMINAL_ERROR_ANALYSIS.md`: 터미널 오류 분석 (과거 이슈)
- `FINAL_FIX_NEW_H.md`: 최종 수정 기록 (과거 이슈)
- `VERIFY_MASTERKEY.md`: 마스터 키 검증 (일시적)
- `CHECK_BUILD_KEY.md`: 빌드 키 확인 (일시적)
- `MASTERKEY_TO_HEADER_PROCESS.md`: 마스터 키 프로세스 (중복 가능)
- `MASTERKEY_REFERENCE_FLOW.md`: 마스터 키 참조 흐름 (중복 가능)

### 🔧 PowerShell 스크립트 (.ps1) - 정리 필요

현재 11개의 .ps1 파일이 있습니다:

**필요한 스크립트:**
- `generate_masterkey_header.ps1`: Pre-Build Event에서 사용 (필수)

**정리 가능한 스크립트:**
- `fix_cmake_issue.ps1`: CMake 이슈 수정 (CMake 미사용 시)
- `cleanup_unused_files.ps1`: 불필요한 파일 정리
- `analyze_unused_code.ps1`: 사용하지 않는 코드 분석
- `convert_project.ps1`: 프로젝트 변환 (일회성)
- `fix_project_settings.ps1`: 프로젝트 설정 수정 (일회성)

---

## 3. 권장 정리 작업

### 즉시 삭제 가능:
1. 모든 `.ncb` 파일 (IntelliSense 캐시)
2. `.vcproj` 파일 (이전 프로젝트 파일)
3. `.aps` 파일 (리소스 캐시)
4. `.suo` 파일 (솔루션 사용자 옵션)
5. `nul`, `UpgradeLog.htm`, `tsvnsync.tsex`

### 확인 후 삭제:
1. `build\` 폴더 (CMake 미사용 시)
2. `out\` 폴더 (내용 확인 후)
3. `Backup\` 폴더 (중요한 백업 확인 후)
4. 과거 이슈 해결 문서들 (`.md` 파일)
5. 일회성 스크립트들 (`.ps1` 파일)

### 유지해야 할 파일:
1. 모든 소스 코드 파일 (`.cpp`, `.h`)
2. 프로젝트 파일 (`.vcxproj`, `.sln`)
3. 리소스 파일 (`.rc`, `res\`)
4. 필수 DLL 파일들
5. 설정 파일 (`masterkey.ini`, `xingAPI.ini`, `Login.ini`)
6. 핵심 문서 (`README.md`, 빌드 가이드 등)

---

## 4. 정리 스크립트

다음 PowerShell 스크립트로 자동 정리 가능:

```powershell
# Visual Studio 캐시 파일 삭제
Get-ChildItem -Path . -Filter "*.ncb" -Recurse | Remove-Item -Force
Get-ChildItem -Path . -Filter "*.aps" -Recurse | Remove-Item -Force
Get-ChildItem -Path . -Filter "*.suo" -Recurse | Remove-Item -Force

# 이전 프로젝트 파일 삭제
Get-ChildItem -Path . -Filter "*.vcproj*" -Recurse | Remove-Item -Force

# 빌드 결과물 삭제 (재생성 가능)
Remove-Item -Path "Debug" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "Release" -Recurse -Force -ErrorAction SilentlyContinue
Remove-Item -Path "x64" -Recurse -Force -ErrorAction SilentlyContinue
```

