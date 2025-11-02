# Visual Studio 2022 프로젝트 설정 스크립트

param(
    [string]$Config = "Release",
    [switch]$SkipCMake
)

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "MMMaster VS 2022 프로젝트 설정" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ScriptDir

# CMake 찾기
function Find-CMake {
    # 1. PATH에서 찾기
    $CmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
    if ($CmakeCmd) {
        return $CmakeCmd.Source
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
            return $Path
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
            return $Path
        }
    }
    
    return $null
}

# CMake를 사용하여 VS 2022 프로젝트 생성
if (-not $SkipCMake) {
    Write-Host "CMake 찾는 중..." -ForegroundColor Yellow
    
    $CmakeExe = Find-CMake
    
    if (-not $CmakeExe) {
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Red
        Write-Host "오류: CMake를 찾을 수 없습니다" -ForegroundColor Red
        Write-Host "========================================" -ForegroundColor Red
        Write-Host ""
        Write-Host "해결 방법:" -ForegroundColor Yellow
        Write-Host "1. CMake 설치: https://cmake.org/download/" -ForegroundColor White
        Write-Host "2. Visual Studio에서 CMake 도구 설치" -ForegroundColor White
        Write-Host "3. fix_cmake_issue.ps1 실행하여 자동 수정 시도" -ForegroundColor White
        Write-Host ""
        Write-Host "또는 .\fix_cmake_issue.ps1 를 실행하세요" -ForegroundColor Cyan
        Write-Host ""
        exit 1
    }
    
    Write-Host "✓ CMake 발견: $CmakeExe" -ForegroundColor Green
    Write-Host ""
    Write-Host "CMake로 Visual Studio 2022 프로젝트 생성 중..." -ForegroundColor Yellow
    
    $BuildDir = Join-Path $ScriptDir "build"
    if (-not (Test-Path $BuildDir)) {
        New-Item -ItemType Directory -Path $BuildDir | Out-Null
    }
    
    $CMakeArgs = @(
        "-S", $ScriptDir
        "-B", $BuildDir
        "-G", "Visual Studio 17 2022"
        "-A", "Win32"
    )
    
    try {
        & $CmakeExe @CMakeArgs
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host ""
            Write-Host "프로젝트 생성 완료!" -ForegroundColor Green
            Write-Host "프로젝트 파일: $BuildDir\MMMaster.sln" -ForegroundColor Green
            Write-Host ""
            Write-Host "Visual Studio 2022에서 열기:" -ForegroundColor Yellow
            Write-Host "  $BuildDir\MMMaster.sln" -ForegroundColor White
        } else {
            Write-Host "CMake 실행 실패 (종료 코드: $LASTEXITCODE)" -ForegroundColor Red
            exit $LASTEXITCODE
        }
    } catch {
        Write-Host "오류: $_" -ForegroundColor Red
        Write-Host "CMake가 설치되어 있는지 확인하세요." -ForegroundColor Yellow
        exit 1
    }
}

# 빌드 (선택사항)
$Build = Read-Host "지금 빌드하시겠습니까? (Y/N)"
if ($Build -eq "Y" -or $Build -eq "y") {
    Write-Host ""
    Write-Host "빌드 시작..." -ForegroundColor Yellow
    
    # CMake 실행 파일 사용 (이미 찾아진 것 사용)
    if (-not $CmakeExe) {
        $CmakeExe = Find-CMake
    }
    
    if (-not $CmakeExe) {
        Write-Host "오류: CMake를 찾을 수 없습니다" -ForegroundColor Red
        exit 1
    }
    
    $BuildArgs = @(
        "--build", $BuildDir
        "--config", $Config
    )
    
    & $CmakeExe @BuildArgs
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "빌드 성공!" -ForegroundColor Green
        $ExePath = Join-Path $BuildDir "bin" $Config "MMMaster.exe"
        if (Test-Path $ExePath) {
            Write-Host "실행 파일: $ExePath" -ForegroundColor Green
        }
    } else {
        Write-Host "빌드 실패 (종료 코드: $LASTEXITCODE)" -ForegroundColor Red
    }
}

Write-Host ""
Write-Host "설정 완료!" -ForegroundColor Green

