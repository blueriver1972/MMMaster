# 프로젝트 설정 확인 및 수정 스크립트

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "프로젝트 설정 확인" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# .vcxproj 파일 찾기
$VcxProjFile = Get-ChildItem -Path . -Filter "*.vcxproj" | Select-Object -First 1

if (-not $VcxProjFile) {
    Write-Host "경고: .vcxproj 파일을 찾을 수 없습니다." -ForegroundColor Yellow
    Write-Host "프로젝트를 Visual Studio에서 변환해야 합니다." -ForegroundColor Yellow
    Write-Host ""
    Write-Host "해결 방법:" -ForegroundColor Cyan
    Write-Host "1. Visual Studio에서 MMMaster.sln 열기" -ForegroundColor White
    Write-Host "2. 프로젝트 변환 (자동 제안됨)" -ForegroundColor White
    Write-Host "3. 프로젝트 우클릭 > 속성 > MFC 사용: 동적 라이브러리에서 MFC 사용" -ForegroundColor White
    exit 0
}

Write-Host "프로젝트 파일: $($VcxProjFile.Name)" -ForegroundColor Green
Write-Host ""

# MFC 설정 확인
$Content = Get-Content $VcxProjFile.FullName -Raw

if ($Content -match 'UseOfMfc') {
    Write-Host "✓ MFC 설정 발견" -ForegroundColor Green
    
    if ($Content -match 'UseOfMfc>Dynamic') {
        Write-Host "✓ MFC: Dynamic (올바름)" -ForegroundColor Green
    } elseif ($Content -match 'UseOfMfc>Static') {
        Write-Host "⚠ MFC: Static - Dynamic으로 변경 권장" -ForegroundColor Yellow
    } else {
        Write-Host "⚠ MFC 설정을 확인할 수 없습니다" -ForegroundColor Yellow
    }
} else {
    Write-Host "✗ MFC 설정이 없습니다 - 추가 필요" -ForegroundColor Red
}

Write-Host ""

# 추가 포함 디렉토리 확인
if ($Content -match 'AdditionalIncludeDirectories') {
    Write-Host "✓ 추가 포함 디렉토리 설정됨" -ForegroundColor Green
} else {
    Write-Host "⚠ 추가 포함 디렉토리 설정 확인 필요" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "권장 조치" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Visual Studio에서:" -ForegroundColor Yellow
Write-Host "1. 프로젝트 우클릭 > 속성" -ForegroundColor White
Write-Host "2. 구성 속성 > 일반" -ForegroundColor White
Write-Host "3. MFC 사용: '동적 라이브러리에서 MFC 사용' 선택" -ForegroundColor White
Write-Host "4. 적용 > 확인" -ForegroundColor White
Write-Host ""
Write-Host "또는 StdAfx.h에 <new> 헤더가 추가되었습니다." -ForegroundColor Green
Write-Host "이제 빌드를 다시 시도하세요." -ForegroundColor Green

