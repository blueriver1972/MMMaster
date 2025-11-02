# 빠른 설정 스크립트 - Visual Studio 환경 설정 포함

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "MMMaster 빠른 설정" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ScriptDir

# Visual Studio 환경 설정
Write-Host "Visual Studio 환경 설정 중..." -ForegroundColor Yellow

$VsDevCmd = "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

if (Test-Path $VsDevCmd) {
    # VsDevCmd 실행하여 환경 변수 설정
    $envTemp = cmd /c "`"$VsDevCmd`" && set" | ForEach-Object {
        if ($_ -match '^([^=]+)=(.*)$') {
            [System.Environment]::SetEnvironmentVariable($matches[1], $matches[2], 'Process')
        }
    }
    Write-Host "✓ Visual Studio 환경 설정 완료" -ForegroundColor Green
} else {
    Write-Host "경고: VsDevCmd.bat를 찾을 수 없습니다" -ForegroundColor Yellow
    Write-Host "Visual Studio Developer Command Prompt를 사용하세요" -ForegroundColor Yellow
}

Write-Host ""

# CMake 경로 추가
$CmakePath = "${env:ProgramFiles}\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin"
if (Test-Path "$CmakePath\cmake.exe") {
    $env:PATH = "$CmakePath;$env:PATH"
    Write-Host "✓ CMake 경로 추가됨" -ForegroundColor Green
}

# CMake 확인
$CmakeCmd = Get-Command cmake -ErrorAction SilentlyContinue
if ($CmakeCmd) {
    Write-Host "✓ CMake 발견: $($CmakeCmd.Source)" -ForegroundColor Green
    $Version = & $CmakeCmd.Source --version | Select-Object -First 1
    Write-Host "  $Version" -ForegroundColor Gray
} else {
    Write-Host "✗ CMake를 찾을 수 없습니다" -ForegroundColor Red
    Write-Host ""
    Write-Host "Visual Studio Developer Command Prompt를 사용하세요:" -ForegroundColor Yellow
    Write-Host "  1. 시작 메뉴에서 'Developer Command Prompt for VS 2022' 검색" -ForegroundColor White
    Write-Host "  2. 실행 후 다음 명령 실행:" -ForegroundColor White
    Write-Host "     cmake -S . -B build -G `"Visual Studio 17 2022`" -A Win32" -ForegroundColor Cyan
    exit 1
}

Write-Host ""

# 프로젝트 생성
Write-Host "CMake 프로젝트 생성 중..." -ForegroundColor Yellow

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
    & cmake @CMakeArgs
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Green
        Write-Host "프로젝트 생성 완료!" -ForegroundColor Green
        Write-Host "========================================" -ForegroundColor Green
        Write-Host ""
        Write-Host "다음 단계:" -ForegroundColor Yellow
        Write-Host "1. Visual Studio에서 열기:" -ForegroundColor White
        Write-Host "   start $BuildDir\MMMaster.sln" -ForegroundColor Cyan
        Write-Host ""
        Write-Host "2. 또는 빌드만 실행:" -ForegroundColor White
        Write-Host "   cmake --build $BuildDir --config Release" -ForegroundColor Cyan
        Write-Host ""
    } else {
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Red
        Write-Host "프로젝트 생성 실패 (종료 코드: $LASTEXITCODE)" -ForegroundColor Red
        Write-Host "========================================" -ForegroundColor Red
        Write-Host ""
        Write-Host "Visual Studio Developer Command Prompt를 사용하세요:" -ForegroundColor Yellow
        Write-Host "FIX_CMAKE_ISSUE.md 파일을 참조하세요" -ForegroundColor Yellow
    }
} catch {
    Write-Host ""
    Write-Host "오류: $_" -ForegroundColor Red
    Write-Host ""
    Write-Host "Visual Studio Developer Command Prompt를 사용하세요" -ForegroundColor Yellow
}

