# Visual Studio 프로젝트 변환 및 빌드 스크립트
# .vcproj 파일을 .vcxproj로 변환 후 빌드

param(
    [string]$Configuration = "Release",
    [string]$Platform = "Win32"
)

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "MMMaster 프로젝트 변환 및 빌드" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ScriptDir

# Visual Studio Developer Command Prompt 사용
$VSWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"

if (-not (Test-Path $VSWhere)) {
    Write-Host "오류: Visual Studio를 찾을 수 없습니다." -ForegroundColor Red
    exit 1
}

# Visual Studio 설치 경로 찾기
$VSInstallPath = & $VSWhere -latest -property installationPath

if (-not $VSInstallPath) {
    Write-Host "오류: Visual Studio 설치 경로를 찾을 수 없습니다." -ForegroundColor Red
    exit 1
}

Write-Host "Visual Studio 경로: $VSInstallPath" -ForegroundColor Green
Write-Host ""

# 방법 1: Visual Studio에서 자동 변환 (수동 작업 필요)
Write-Host "========================================" -ForegroundColor Yellow
Write-Host "프로젝트 변환이 필요합니다!" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Yellow
Write-Host ""
Write-Host "다음 단계를 따라주세요:" -ForegroundColor Cyan
Write-Host ""
Write-Host "1. Visual Studio를 엽니다" -ForegroundColor White
Write-Host "2. 파일 > 열기 > 프로젝트/솔루션" -ForegroundColor White
Write-Host "3. MMMaster.sln 파일을 선택" -ForegroundColor White
Write-Host "4. '프로젝트를 현재 버전으로 변환' 메시지가 나타나면 '예' 클릭" -ForegroundColor White
Write-Host "5. 변환 완료 후:" -ForegroundColor White
Write-Host "   - 프로젝트 우클릭 > 속성" -ForegroundColor White
Write-Host "   - 플랫폼 도구 집합을 'v143' (VS2022) 또는 'v142' (VS2019)로 변경" -ForegroundColor White
Write-Host "6. 솔루션 빌드 (Ctrl+Shift+B)" -ForegroundColor White
Write-Host ""

# 방법 2: devenv.com 사용 시도
$DevEnv = Join-Path $VSInstallPath "Common7\IDE\devenv.com"

if (Test-Path $DevEnv) {
    Write-Host "devenv.com을 사용하여 빌드를 시도합니다..." -ForegroundColor Yellow
    Write-Host ""
    
    $SolutionPath = Join-Path $ScriptDir "MMMaster.sln"
    
    try {
        & $DevEnv $SolutionPath /build "${Configuration}|${Platform}"
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host ""
            Write-Host "빌드 성공!" -ForegroundColor Green
        } else {
            Write-Host ""
            Write-Host "빌드 실패 (종료 코드: $LASTEXITCODE)" -ForegroundColor Red
            Write-Host "Visual Studio에서 수동으로 변환 및 빌드가 필요합니다." -ForegroundColor Yellow
        }
    } catch {
        Write-Host "오류: $_" -ForegroundColor Red
        Write-Host "Visual Studio에서 수동으로 변환 및 빌드가 필요합니다." -ForegroundColor Yellow
    }
} else {
    Write-Host "devenv.com을 찾을 수 없습니다." -ForegroundColor Yellow
    Write-Host "Visual Studio에서 수동으로 변환 및 빌드가 필요합니다." -ForegroundColor Yellow
}

Write-Host ""

