# MMMaster 프로젝트 정리 스크립트
# Visual Studio 캐시 및 불필요한 파일 정리

Write-Host "=== MMMaster 프로젝트 정리 시작 ===" -ForegroundColor Cyan
Write-Host ""

# 1. Visual Studio 캐시 파일 삭제
Write-Host "1. Visual Studio 캐시 파일 삭제 중..." -ForegroundColor Yellow
$ncbFiles = Get-ChildItem -Path . -Filter "*.ncb" -Recurse -ErrorAction SilentlyContinue
$apsFiles = Get-ChildItem -Path . -Filter "*.aps" -Recurse -ErrorAction SilentlyContinue
$suoFiles = Get-ChildItem -Path . -Filter "*.suo" -Recurse -ErrorAction SilentlyContinue

if ($ncbFiles) {
    $ncbFiles | Remove-Item -Force
    Write-Host "  ✓ .ncb 파일 삭제 완료 ($($ncbFiles.Count)개)" -ForegroundColor Green
}
if ($apsFiles) {
    $apsFiles | Remove-Item -Force
    Write-Host "  ✓ .aps 파일 삭제 완료 ($($apsFiles.Count)개)" -ForegroundColor Green
}
if ($suoFiles) {
    $suoFiles | Remove-Item -Force
    Write-Host "  ✓ .suo 파일 삭제 완료 ($($suoFiles.Count)개)" -ForegroundColor Green
}

# 2. 이전 프로젝트 파일 삭제
Write-Host ""
Write-Host "2. 이전 프로젝트 파일 삭제 중..." -ForegroundColor Yellow
$vcprojFiles = Get-ChildItem -Path . -Filter "*.vcproj*" -Recurse -ErrorAction SilentlyContinue
if ($vcprojFiles) {
    $vcprojFiles | Remove-Item -Force
    Write-Host "  ✓ .vcproj 파일 삭제 완료 ($($vcprojFiles.Count)개)" -ForegroundColor Green
}

# 3. 기타 불필요한 파일 삭제
Write-Host ""
Write-Host "3. 기타 불필요한 파일 삭제 중..." -ForegroundColor Yellow
$otherFiles = @("nul", "UpgradeLog.htm", "tsvnsync.tsex")
foreach ($file in $otherFiles) {
    if (Test-Path $file) {
        Remove-Item -Path $file -Force -ErrorAction SilentlyContinue
        Write-Host "  ✓ $file 삭제 완료" -ForegroundColor Green
    }
}

# 4. 빌드 결과물 삭제 (선택적)
Write-Host ""
Write-Host "4. 빌드 결과물 삭제 (선택적)..." -ForegroundColor Yellow
Write-Host "  빌드 결과물을 삭제하시겠습니까? (Y/N): " -NoNewline -ForegroundColor Cyan
$response = Read-Host
if ($response -eq "Y" -or $response -eq "y") {
    $buildFolders = @("Debug", "Release", "x64", "MMMaster\Debug", "MMMaster\Release", "MMMaster\x64\Debug", "MMMaster\x64\Release")
    foreach ($folder in $buildFolders) {
        if (Test-Path $folder) {
            Remove-Item -Path $folder -Recurse -Force -ErrorAction SilentlyContinue
            Write-Host "  ✓ $folder 삭제 완료" -ForegroundColor Green
        }
    }
} else {
    Write-Host "  빌드 결과물 삭제를 건너뜁니다." -ForegroundColor Gray
}

# 5. CMake 빌드 폴더 삭제 (선택적)
Write-Host ""
Write-Host "5. CMake 빌드 폴더 삭제 (선택적)..." -ForegroundColor Yellow
Write-Host "  CMake 빌드 폴더를 삭제하시겠습니까? (Y/N): " -NoNewline -ForegroundColor Cyan
$response = Read-Host
if ($response -eq "Y" -or $response -eq "y") {
    if (Test-Path "build") {
        Remove-Item -Path "build" -Recurse -Force -ErrorAction SilentlyContinue
        Write-Host "  ✓ build 폴더 삭제 완료" -ForegroundColor Green
    }
    if (Test-Path "out") {
        Remove-Item -Path "out" -Recurse -Force -ErrorAction SilentlyContinue
        Write-Host "  ✓ out 폴더 삭제 완료" -ForegroundColor Green
    }
} else {
    Write-Host "  CMake 빌드 폴더 삭제를 건너뜁니다." -ForegroundColor Gray
}

Write-Host ""
Write-Host "=== 정리 완료 ===" -ForegroundColor Green
Write-Host ""
Write-Host "참고: Visual Studio를 다시 열면 캐시 파일들이 자동으로 재생성됩니다." -ForegroundColor Gray

