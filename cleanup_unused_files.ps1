# 사용하지 않는 파일 제거 스크립트
# 실행 전 반드시 Git에 커밋하거나 백업을 생성하세요!

param(
    [switch]$DryRun,
    [switch]$Force
)

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "사용하지 않는 파일 제거 스크립트" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

if (-not $Force) {
    Write-Host "⚠️  경고: 이 스크립트는 파일을 영구적으로 삭제합니다!" -ForegroundColor Red
    Write-Host ""
    $Confirm = Read-Host "계속하시겠습니까? (yes 입력)"
    if ($Confirm -ne "yes") {
        Write-Host "취소되었습니다." -ForegroundColor Yellow
        exit 0
    }
}

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ScriptDir

# 제거할 파일 목록
$FilesToRemove = @(
    "Serach_LiST.cpp",
    "Serach_LiST.h",
    "analyPatten.cpp",
    "analyPatten.h"
)

Write-Host ""
if ($DryRun) {
    Write-Host "🔍 DRY RUN 모드 - 실제로 제거하지 않습니다" -ForegroundColor Yellow
    Write-Host ""
}

$RemovedCount = 0
$NotFoundCount = 0

foreach ($File in $FilesToRemove) {
    $FilePath = Join-Path $ScriptDir $File
    
    if (Test-Path $FilePath) {
        if ($DryRun) {
            Write-Host "  [제거 예정] $File" -ForegroundColor Yellow
        } else {
            try {
                Remove-Item $FilePath -Force
                Write-Host "  ✓ 제거됨: $File" -ForegroundColor Green
                $RemovedCount++
            } catch {
                Write-Host "  ✗ 오류: $File - $_" -ForegroundColor Red
            }
        }
    } else {
        Write-Host "  - 없음: $File" -ForegroundColor Gray
        $NotFoundCount++
    }
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
if ($DryRun) {
    Write-Host "DRY RUN 완료" -ForegroundColor Yellow
    Write-Host "$($FilesToRemove.Count)개 파일이 제거될 예정입니다." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "실제로 제거하려면 -Force 스위치를 사용하세요:" -ForegroundColor White
    Write-Host "  .\cleanup_unused_files.ps1 -Force" -ForegroundColor Cyan
} else {
    Write-Host "제거 완료" -ForegroundColor Green
    Write-Host "  제거된 파일: $RemovedCount개" -ForegroundColor Green
    Write-Host "  없었던 파일: $NotFoundCount개" -ForegroundColor Gray
    Write-Host ""
    Write-Host "빌드 테스트를 수행하세요:" -ForegroundColor Yellow
    Write-Host "  cmake --build build --config Release" -ForegroundColor Cyan
}
Write-Host "========================================" -ForegroundColor Cyan

