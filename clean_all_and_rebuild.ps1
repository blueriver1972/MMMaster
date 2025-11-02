# 완전 정리 및 재빌드 스크립트

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "프로젝트 완전 정리" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ScriptDir

Write-Host "정리 중..." -ForegroundColor Yellow

# 중간 파일 삭제
$ItemsToRemove = @(
    "Debug",
    "Release",
    "*.pch",
    "*.obj",
    "*.idb",
    "*.pdb",
    "*.ilk",
    "*.exp",
    "*.lib",
    "*.log",
    "*.tlb",
    "*.res",
    "*.aps",
    "*.ncb",
    "*.suo"
)

$RemovedCount = 0
foreach ($Pattern in $ItemsToRemove) {
    Get-ChildItem -Path . -Filter $Pattern -Recurse -ErrorAction SilentlyContinue | 
        Where-Object { $_.FullName -notlike "*\packet\*" } |
        Remove-Item -Force -ErrorAction SilentlyContinue | Out-Null
    $RemovedCount++
}

# out, build 폴더 삭제 (CMake)
if (Test-Path "out") {
    Remove-Item -Recurse -Force "out" -ErrorAction SilentlyContinue
}
if (Test-Path "build") {
    Remove-Item -Recurse -Force "build" -ErrorAction SilentlyContinue
}

Write-Host "✓ 정리 완료" -ForegroundColor Green
Write-Host ""

# XingAPI 파일 확인
Write-Host "XingAPI 파일 확인 중..." -ForegroundColor Yellow
$XingAPIPath = "C:\LS_SEC\xingAPI"
if (Test-Path $XingAPIPath) {
    Write-Host "✓ XingAPI 경로 확인됨" -ForegroundColor Green
} else {
    Write-Host "⚠ XingAPI 경로를 찾을 수 없습니다: $XingAPIPath" -ForegroundColor Yellow
}

# 필수 DLL 확인
Write-Host ""
Write-Host "필수 DLL 확인:" -ForegroundColor Yellow
$RequiredDLLs = @("xingAPI.dll", "XA_Common.dll", "XA_DataSet.dll", "XA_Session.dll")
$AllPresent = $true
foreach ($DLL in $RequiredDLLs) {
    if (Test-Path $DLL) {
        Write-Host "  ✓ $DLL" -ForegroundColor Green
    } else {
        Write-Host "  ✗ $DLL (없음)" -ForegroundColor Red
        $AllPresent = $false
    }
}

if (-not $AllPresent) {
    Write-Host ""
    Write-Host "XingAPI 파일을 복사하세요:" -ForegroundColor Yellow
    Write-Host "  .\setup_xingapi_path.ps1" -ForegroundColor Cyan
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "다음 단계" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "1. Visual Studio에서 프로젝트 열기" -ForegroundColor White
Write-Host "2. 빌드 > 솔루션 정리 (Clean Solution)" -ForegroundColor White
Write-Host "3. 빌드 > 솔루션 다시 빌드 (Rebuild Solution)" -ForegroundColor White
Write-Host ""
Write-Host "또는 Visual Studio를 재시작한 후 빌드하세요." -ForegroundColor Yellow
Write-Host ""

