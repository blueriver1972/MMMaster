# 사용하지 않는 코드 분석 스크립트
# 주석 처리된 코드, 사용하지 않는 함수 등을 찾아보는 도구

$ErrorActionPreference = "Stop"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "MMMaster 코드 분석" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $ScriptDir

$Report = @()

# 1. 주석 처리된 코드 찾기
Write-Host "1. 주석 처리된 코드 분석 중..." -ForegroundColor Yellow

$CommentedCode = Get-ChildItem -Path $ScriptDir -Include *.cpp,*.h -Recurse | 
    Select-String -Pattern "^\s*//.*(if|for|while|switch|return|void|int|CString)" |
    Select-Object -First 20

if ($CommentedCode) {
    Write-Host "주석 처리된 코드 발견:" -ForegroundColor Yellow
    $CommentedCode | ForEach-Object {
        Write-Host "  $($_.Filename):$($_.LineNumber) - $($_.Line.Trim())" -ForegroundColor Gray
    }
    $Report += "주석 처리된 코드: $($CommentedCode.Count)개"
}

# 2. 사용하지 않는 파일 찾기
Write-Host ""
Write-Host "2. 사용하지 않는 파일 분석 중..." -ForegroundColor Yellow

$SourceFiles = Get-ChildItem -Path $ScriptDir -Include *.cpp -File | 
    Where-Object { $_.Name -notmatch "StdAfx" }

$ProjectFiles = @(
    "ETrade_Order_Sample.cpp",
    "ETrade_Order_SampleDlg.cpp",
    "CSPAQ03700.cpp",
    "CSPAT00700.cpp",
    "CSPAT00800.cpp",
    "CSPBQ00200.cpp",
    "S_Login.cpp",
    "S_Order_Debt.cpp",
    "MySQLCMD.cpp",
    "GetList.cpp",
    "ListofLog.cpp",
    "Searching_List.cpp",
    "Setup.cpp",
    "DegreeList.cpp",
    "T0424.cpp",
    "T1101.cpp",
    "T1102.cpp",
    "XLAutomation.cpp",
    "XLEzAutomation.cpp"
)

$UnusedFiles = $SourceFiles | Where-Object { $ProjectFiles -notcontains $_.Name }

if ($UnusedFiles) {
    Write-Host "프로젝트에 포함되지 않은 파일:" -ForegroundColor Yellow
    $UnusedFiles | ForEach-Object {
        Write-Host "  $($_.Name)" -ForegroundColor Gray
    }
    $Report += "사용하지 않는 파일: $($UnusedFiles.Count)개"
}

# 3. TODO/FIXME 주석 찾기
Write-Host ""
Write-Host "3. TODO/FIXME 주석 찾기..." -ForegroundColor Yellow

$Todos = Get-ChildItem -Path $ScriptDir -Include *.cpp,*.h -Recurse | 
    Select-String -Pattern "TODO|FIXME|XXX|HACK" -CaseSensitive:$false |
    Select-Object -First 20

if ($Todos) {
    Write-Host "TODO/FIXME 발견:" -ForegroundColor Yellow
    $Todos | ForEach-Object {
        Write-Host "  $($_.Filename):$($_.LineNumber) - $($_.Line.Trim())" -ForegroundColor Gray
    }
    $Report += "TODO/FIXME: $($Todos.Count)개"
}

# 4. 중복된 구조체 정의 찾기
Write-Host ""
Write-Host "4. 구조체 정의 분석 중..." -ForegroundColor Yellow

$StructDefs = Get-ChildItem -Path $ScriptDir -Include *.h,*.cpp -Recurse | 
    Select-String -Pattern "^\s*(struct|typedef struct)" |
    Group-Object -Property { $_.Line -replace '\s*struct\s+(\w+).*', '$1' }

$Duplicates = $StructDefs | Where-Object { $_.Count -gt 1 }

if ($Duplicates) {
    Write-Host "중복 정의 가능한 구조체:" -ForegroundColor Yellow
    $Duplicates | ForEach-Object {
        Write-Host "  $($_.Name) - $($_.Count)회 정의" -ForegroundColor Gray
    }
}

# 리포트 출력
Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "분석 완료" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
$Report | ForEach-Object {
    Write-Host $_ -ForegroundColor Green
}

Write-Host ""
Write-Host "참고: 실제 사용 여부는 코드 분석 도구로 확인이 필요합니다." -ForegroundColor Yellow

