# MMMaster 빌드 전처리 스크립트
# masterkey.ini 파일을 읽어서 MasterKey.h 헤더 파일 생성

param(
    [string]$ProjectDir = $PSScriptRoot
)

# 경로 정리 (끝의 백슬래시 제거 및 따옴표 제거)
$ProjectDir = $ProjectDir.TrimEnd('\', '/').Trim('"').Trim("'")

# 절대 경로로 변환
if (-not [System.IO.Path]::IsPathRooted($ProjectDir)) {
    $resolved = Resolve-Path $ProjectDir -ErrorAction SilentlyContinue
    if ($null -ne $resolved) {
        $ProjectDir = $resolved.Path
    } else {
        $ProjectDir = $PSScriptRoot
    }
}

$MasterKeyIni = Join-Path $ProjectDir "masterkey.ini"
$OutputHeader = Join-Path $ProjectDir "MasterKey.h"
$DefaultKey = "MyMasterKey123!"

Write-Host "=== MasterKey Header Generator ===" -ForegroundColor Cyan
Write-Host "Project Directory: $ProjectDir" -ForegroundColor Gray
Write-Host "Master Key INI: $MasterKeyIni" -ForegroundColor Gray
Write-Host "Output Header: $OutputHeader" -ForegroundColor Gray
Write-Host ""

# masterkey.ini 파일 읽기
$MasterKey = $DefaultKey

if (Test-Path $MasterKeyIni) {
    Write-Host "✓ masterkey.ini 파일을 찾았습니다." -ForegroundColor Green
    
    try {
        # INI 파일에서 키 읽기 (UTF-8 또는 ANSI 인코딩 자동 감지)
        $iniContent = Get-Content $MasterKeyIni -Raw -Encoding UTF8 -ErrorAction SilentlyContinue
        if ($null -eq $iniContent) {
            $iniContent = Get-Content $MasterKeyIni -Raw -Encoding Default -ErrorAction SilentlyContinue
        }
        
        # [MASTER_KEY] 섹션의 KEY 값 추출
        if ($null -ne $iniContent) {
            # 줄 단위로 분리하여 처리
            $lines = $iniContent -split "`r?`n"
            $inSection = $false
            foreach ($line in $lines) {
                $trimmed = $line.Trim()
                if ($trimmed -eq "[MASTER_KEY]") {
                    $inSection = $true
                } elseif ($inSection -and $trimmed.StartsWith("KEY=")) {
                    $MasterKey = $trimmed.Substring(4).Trim()
                    break
                } elseif ($trimmed.StartsWith("[") -and $trimmed.EndsWith("]")) {
                    $inSection = $false
                }
            }
        }
        
        if ([string]::IsNullOrWhiteSpace($MasterKey) -or $MasterKey -eq $DefaultKey) {
            Write-Host "⚠ masterkey.ini의 KEY 값을 읽지 못했습니다. 기본값을 사용합니다." -ForegroundColor Yellow
            $MasterKey = $DefaultKey
        } else {
            Write-Host "✓ masterkey.ini에서 키를 읽었습니다 (길이: $($MasterKey.Length))" -ForegroundColor Green
        }
    } catch {
        Write-Host "⚠ masterkey.ini 읽기 오류: $_" -ForegroundColor Yellow
        Write-Host "  기본값을 사용합니다." -ForegroundColor Gray
        $MasterKey = $DefaultKey
    }
} else {
    Write-Host "⚠ masterkey.ini 파일을 찾을 수 없습니다. 기본값을 사용합니다." -ForegroundColor Yellow
    Write-Host "  기본값: $DefaultKey" -ForegroundColor Gray
    Write-Host "  ConfigGenerator.exe를 실행하여 masterkey.ini를 생성하세요." -ForegroundColor Gray
}

# 키에 특수문자가 있으면 이스케이프 처리
# C++ 문자열 리터럴에서 필요한 이스케이프: 백슬래시와 따옴표
$EscapedKey = $MasterKey
# 백슬래시를 이스케이프: \ -> \\
$EscapedKey = $EscapedKey.Replace('\', '\\')
# 따옴표를 이스케이프: " -> \"
$EscapedKey = $EscapedKey.Replace('"', '\"')

# 헤더 파일 생성
$HeaderContent = "// 자동 생성된 파일 - 수정하지 마세요!`r`n"
$HeaderContent += "// 이 파일은 빌드 전처리 스크립트(generate_masterkey_header.ps1)에 의해 자동 생성됩니다.`r`n"
$HeaderContent += "// masterkey.ini 파일을 수정한 후 프로젝트를 재빌드하면 자동으로 업데이트됩니다.`r`n"
$HeaderContent += "`r`n"
$HeaderContent += "#pragma once`r`n"
$HeaderContent += "`r`n"
$HeaderContent += "// 빌드 시점 마스터 키 (masterkey.ini에서 읽어옴)`r`n"
$HeaderContent += "#define BUILD_ENCRYPTION_KEY `"$EscapedKey`"`r`n"

# 헤더 파일 쓰기
try {
    # UTF-8 BOM 없이 쓰기 (Visual Studio 호환)
    $Utf8NoBomEncoding = New-Object System.Text.UTF8Encoding $false
    [System.IO.File]::WriteAllText($OutputHeader, $HeaderContent, $Utf8NoBomEncoding)
    Write-Host ""
    Write-Host "✓ MasterKey.h 파일이 생성되었습니다." -ForegroundColor Green
    Write-Host "  위치: $OutputHeader" -ForegroundColor Gray
} catch {
    Write-Host ""
    Write-Host "⚠ MasterKey.h 파일 생성 실패: $_" -ForegroundColor Yellow
    Write-Host "  기본값을 사용하여 계속 진행합니다." -ForegroundColor Yellow
    # 빌드가 중단되지 않도록 성공 코드 반환
}

Write-Host ""

# 항상 성공 코드 반환 (빌드 중단 방지)
exit 0
