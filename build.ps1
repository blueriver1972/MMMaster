# MMMaster 프로젝트 빌드 스크립트
# PowerShell 스크립트

param(
    [string]$Configuration = "Release",
    [string]$Platform = "Win32"
)

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "MMMaster 프로젝트 빌드 스크립트" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# 현재 디렉토리 확인
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ScriptDir

Write-Host "작업 디렉토리: $ScriptDir" -ForegroundColor Yellow
Write-Host ""

# Visual Studio 설치 확인
$VSWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $VSWhere)) {
    Write-Host "오류: Visual Studio를 찾을 수 없습니다." -ForegroundColor Red
    Write-Host "Visual Studio 2019 또는 2022를 설치해주세요." -ForegroundColor Red
    exit 1
}

# MSBuild 경로 찾기
$MSBuild = & $VSWhere -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe | Select-Object -First 1

if (-not $MSBuild -or -not (Test-Path $MSBuild)) {
    Write-Host "오류: MSBuild를 찾을 수 없습니다." -ForegroundColor Red
    exit 1
}

Write-Host "MSBuild 경로: $MSBuild" -ForegroundColor Green
Write-Host ""

# 솔루션 파일 확인
$SolutionFile = Join-Path $ScriptDir "MMMaster.sln"
if (-not (Test-Path $SolutionFile)) {
    Write-Host "오류: MMMaster.sln 파일을 찾을 수 없습니다." -ForegroundColor Red
    exit 1
}

Write-Host "솔루션 파일: $SolutionFile" -ForegroundColor Green
Write-Host "빌드 설정: $Configuration" -ForegroundColor Green
Write-Host "플랫폼: $Platform" -ForegroundColor Green
Write-Host ""

# 필수 DLL 확인
Write-Host "필수 DLL 파일 확인 중..." -ForegroundColor Yellow
$RequiredDLLs = @(
    "xingAPI.dll",
    "XA_Common.dll",
    "XA_DataSet.dll",
    "XA_Session.dll"
)

$DLLMissing = $false
foreach ($DLL in $RequiredDLLs) {
    $DLLPath = Join-Path $ScriptDir $DLL
    if (-not (Test-Path $DLLPath)) {
        Write-Host "경고: $DLL 을(를) 찾을 수 없습니다." -ForegroundColor Yellow
        $DLLMissing = $true
    } else {
        Write-Host "확인: $DLL" -ForegroundColor Green
    }
}

if ($DLLMissing) {
    Write-Host ""
    Write-Host "경고: 일부 DLL 파일이 누락되었습니다." -ForegroundColor Yellow
    Write-Host "빌드는 계속되지만 실행 시 오류가 발생할 수 있습니다." -ForegroundColor Yellow
    Write-Host ""
}

# 빌드 실행
Write-Host "빌드 시작..." -ForegroundColor Cyan
Write-Host ""

$BuildArgs = @(
    $SolutionFile
    "/t:Build"
    "/p:Configuration=$Configuration"
    "/p:Platform=$Platform"
    "/v:minimal"
    "/m"
)

try {
    & $MSBuild $BuildArgs
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Green
        Write-Host "빌드 성공!" -ForegroundColor Green
        Write-Host "========================================" -ForegroundColor Green
        
        # 출력 파일 확인
        $OutputPath = Join-Path $ScriptDir "$Configuration\MMMaster.exe"
        if (Test-Path $OutputPath) {
            $FileInfo = Get-Item $OutputPath
            Write-Host ""
            Write-Host "실행 파일 위치: $OutputPath" -ForegroundColor Green
            Write-Host "파일 크기: $([math]::Round($FileInfo.Length / 1KB, 2)) KB" -ForegroundColor Green
            Write-Host "생성 시간: $($FileInfo.LastWriteTime)" -ForegroundColor Green
        }
    } else {
        Write-Host ""
        Write-Host "========================================" -ForegroundColor Red
        Write-Host "빌드 실패 (종료 코드: $LASTEXITCODE)" -ForegroundColor Red
        Write-Host "========================================" -ForegroundColor Red
        exit $LASTEXITCODE
    }
} catch {
    Write-Host ""
    Write-Host "빌드 중 오류 발생: $_" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "빌드 완료!" -ForegroundColor Green

