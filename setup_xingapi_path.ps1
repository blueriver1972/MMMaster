# XingAPI 경로 설정 및 파일 복사 스크립트

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "XingAPI 경로 설정 및 파일 복사" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$XingAPIPath = "C:\LS_SEC\xingAPI"
$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

# 경로 확인
if (-not (Test-Path $XingAPIPath)) {
    Write-Host "오류: $XingAPIPath 경로를 찾을 수 없습니다." -ForegroundColor Red
    exit 1
}

Write-Host "XingAPI 경로: $XingAPIPath" -ForegroundColor Green
Write-Host "프로젝트 경로: $ProjectRoot" -ForegroundColor Green
Write-Host ""

# 복사할 파일 목록
$FilesToCopy = @(
    "xingAPI.dll",
    "xingAPI.ini",
    "XA_Common.dll",
    "XA_DataSet.dll",
    "XA_Session.dll",
    "inicore_v2.4.3.1.dll",
    "inicrypto_v5.2.0.1.dll",
    "inipki_v5.2.7.1.dll",
    "inisafenet_v7.2.dll"
)

# IXingAPI.h 비교
Write-Host "IXingAPI.h 파일 확인 중..." -ForegroundColor Yellow
$SourceHeader = Join-Path $XingAPIPath "IXingAPI.h"
$ProjectHeader = Join-Path $ProjectRoot "IXingAPI.h"

if (Test-Path $SourceHeader) {
    if (Test-Path $ProjectHeader) {
        $SourceHash = (Get-FileHash $SourceHeader).Hash
        $ProjectHash = (Get-FileHash $ProjectHeader).Hash
        
        if ($SourceHash -ne $ProjectHash) {
            Write-Host "✓ IXingAPI.h 업데이트 필요 (버전이 다름)" -ForegroundColor Yellow
            $Update = Read-Host "업데이트하시겠습니까? (Y/N)"
            if ($Update -eq "Y" -or $Update -eq "y") {
                Copy-Item $SourceHeader $ProjectHeader -Force
                Write-Host "✓ IXingAPI.h 업데이트 완료" -ForegroundColor Green
            }
        } else {
            Write-Host "✓ IXingAPI.h 일치" -ForegroundColor Green
        }
    } else {
        Copy-Item $SourceHeader $ProjectHeader -Force
        Write-Host "✓ IXingAPI.h 복사 완료" -ForegroundColor Green
    }
}

Write-Host ""

# DLL 파일 복사
Write-Host "필수 DLL 파일 복사 중..." -ForegroundColor Yellow

$CopyCount = 0
foreach ($File in $FilesToCopy) {
    $SourceFile = Join-Path $XingAPIPath $File
    $DestFile = Join-Path $ProjectRoot $File
    
    if (Test-Path $SourceFile) {
        Copy-Item $SourceFile $DestFile -Force -ErrorAction SilentlyContinue
        Write-Host "  ✓ $File" -ForegroundColor Green
        $CopyCount++
    } else {
        Write-Host "  - $File (없음)" -ForegroundColor Gray
    }
}

# Release 폴더에 복사
if (Test-Path (Join-Path $ProjectRoot "Release")) {
    Write-Host ""
    Write-Host "Release 폴더에 복사 중..." -ForegroundColor Yellow
    foreach ($File in $FilesToCopy) {
        $SourceFile = Join-Path $XingAPIPath $File
        $DestFile = Join-Path $ProjectRoot "Release\$File"
        
        if (Test-Path $SourceFile) {
            Copy-Item $SourceFile $DestFile -Force -ErrorAction SilentlyContinue
        }
    }
    Write-Host "  ✓ Release 폴더 복사 완료" -ForegroundColor Green
}

# Debug 폴더에 복사
if (Test-Path (Join-Path $ProjectRoot "Debug")) {
    Write-Host ""
    Write-Host "Debug 폴더에 복사 중..." -ForegroundColor Yellow
    foreach ($File in $FilesToCopy) {
        $SourceFile = Join-Path $XingAPIPath $File
        $DestFile = Join-Path $ProjectRoot "Debug\$File"
        
        if (Test-Path $SourceFile) {
            Copy-Item $SourceFile $DestFile -Force -ErrorAction SilentlyContinue
        }
    }
    Write-Host "  ✓ Debug 폴더 복사 완료" -ForegroundColor Green
}

# Sys 폴더 복사 (설정 파일)
$SysSource = Join-Path $XingAPIPath "Sys"
$SysDest = Join-Path $ProjectRoot "Release\Sys"

if (Test-Path $SysSource) {
    Write-Host ""
    Write-Host "Sys 폴더 확인 중..." -ForegroundColor Yellow
    if (-not (Test-Path $SysDest)) {
        New-Item -ItemType Directory -Path $SysDest -Force | Out-Null
    }
    
    Get-ChildItem $SysSource -File | ForEach-Object {
        Copy-Item $_.FullName $SysDest -Force
        Write-Host "  ✓ $($_.Name)" -ForegroundColor Green
    }
}

# Login.ini 복사
$LoginSource = Join-Path $XingAPIPath "Login.ini"
if (Test-Path $LoginSource) {
    Copy-Item $LoginSource (Join-Path $ProjectRoot "Login.ini") -Force
    Copy-Item $LoginSource (Join-Path $ProjectRoot "Release\Login.ini") -Force -ErrorAction SilentlyContinue
    Write-Host "  ✓ Login.ini 복사 완료" -ForegroundColor Green
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "복사 완료: $CopyCount 개 파일" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "다음 단계:" -ForegroundColor Yellow
Write-Host "1. Visual Studio에서 프로젝트 다시 빌드" -ForegroundColor White
Write-Host "2. 실행 파일이 DLL을 찾을 수 있는지 확인" -ForegroundColor White
Write-Host ""

