# CMake 설치/경로 문제 해결 스크립트

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "CMake 설치 및 경로 확인" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# CMake 찾기
$CmakePath = 'C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe'


# 1. PATH에서 찾기
$CmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
if ($CmakeCmd) {
    $CmakePath = $CmakeCmd.Source
    Write-Host "✓ PATH에서 CMake 발견: $CmakePath" -ForegroundColor Green
} else {
    Write-Host "✗ PATH에서 CMake를 찾을 수 없습니다" -ForegroundColor Yellow
}

# 2. 일반적인 설치 경로 확인
$CommonPaths = @(
    "C:\Program Files\CMake\bin\cmake.exe",
    "C:\Program Files (x86)\CMake\bin\cmake.exe",
    "$env:ProgramFiles\CMake\bin\cmake.exe",
    "${env:ProgramFiles(x86)}\CMake\bin\cmake.exe"
)

foreach ($Path in $CommonPaths) {
    if (Test-Path $Path) {
        $CmakePath = $Path
        Write-Host "✓ CMake 발견: $Path" -ForegroundColor Green
        break
    }
}

# 3. Visual Studio에 포함된 CMake 확인
$VSCmakePaths = @(
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe",
    "${env:ProgramFiles}\Microsoft Visual Studio\2022\Enterprise\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
)

foreach ($Path in $VSCmakePaths) {
    if (Test-Path $Path) {
        $CmakePath = $Path
        Write-Host "✓ Visual Studio의 CMake 발견: $Path" -ForegroundColor Green
        break
    }
}

Write-Host ""

# 해결 방법 제시
if ($CmakePath) {
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "해결 방법 1: PATH에 추가" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Green
    Write-Host ""
    
    $CmakeDir = Split-Path $CmakePath -Parent
    Write-Host "다음 명령을 실행하여 현재 세션에 PATH 추가:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "  `$env:PATH += `";$CmakeDir`"" -ForegroundColor White
    Write-Host ""
    Write-Host "또는 영구적으로 추가하려면 시스템 환경 변수에 추가하세요." -ForegroundColor Yellow
    Write-Host ""
    
    # 현재 세션에 추가할지 물어보기
    $AddToPath = Read-Host "지금 현재 세션에 PATH에 추가하시겠습니까? (Y/N)"
    if ($AddToPath -eq "Y" -or $AddToPath -eq "y") {
        $env:PATH += ";$CmakeDir"
        Write-Host "✓ PATH에 추가되었습니다" -ForegroundColor Green
        
        # 확인
        $CmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
        if ($CmakeCmd) {
            Write-Host "✓ CMake 확인: $($CmakeCmd.Source)" -ForegroundColor Green
            Write-Host "  버전: $(& $CmakeCmd.Source --version | Select-Object -First 1)" -ForegroundColor Gray
        }
    }
    
    Write-Host ""
    Write-Host "========================================" -ForegroundColor Green
    Write-Host "해결 방법 2: setup_vs2022.ps1 수정" -ForegroundColor Cyan
    Write-Host "========================================" -ForegroundColor Green
    Write-Host ""
    Write-Host "setup_vs2022.ps1 파일을 수정하여 CMake 경로를 직접 지정할 수 있습니다." -ForegroundColor Yellow
    
} else {
    Write-Host "========================================" -ForegroundColor Red
    Write-Host "CMake를 찾을 수 없습니다" -ForegroundColor Red
    Write-Host "========================================" -ForegroundColor Red
    Write-Host ""
    Write-Host "다음 방법 중 하나를 선택하세요:" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "방법 1: CMake 설치" -ForegroundColor Cyan
    Write-Host "  1. https://cmake.org/download/ 방문" -ForegroundColor White
    Write-Host "  2. Windows x64 Installer 다운로드" -ForegroundColor White
    Write-Host "  3. 설치 시 'Add CMake to system PATH' 선택" -ForegroundColor White
    Write-Host ""
    Write-Host "방법 2: Visual Studio에서 CMake 설치" -ForegroundColor Cyan
    Write-Host "  1. Visual Studio 설치 관리자 실행" -ForegroundColor White
    Write-Host "  2. 수정 선택" -ForegroundColor White
    Write-Host "  3. 'Desktop development with C++' → 'CMake tools for Visual Studio' 체크" -ForegroundColor White
    Write-Host "  4. 수정 클릭" -ForegroundColor White
    Write-Host ""
    Write-Host "방법 3: 기존 .vcproj 사용 (CMake 없이)" -ForegroundColor Cyan
    Write-Host "  Visual Studio에서 직접 MMMaster.sln 열기" -ForegroundColor White
    Write-Host ""
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "다음 단계" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "CMake가 설치되면 다시 실행하세요:" -ForegroundColor Yellow
Write-Host "  .\setup_vs2022.ps1" -ForegroundColor White
Write-Host ""

