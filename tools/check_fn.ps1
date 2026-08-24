# Fast single-function grind loop for reccmp.
# Incremental build by default ( ~2-3s ). Use -CleanFirst only when PDB desyncs.
# Default stdout is score-only; pass -Diff for assembly diff.
#
# Usage:
#   .\tools\check_fn.ps1 0x0045ca30
#   .\tools\check_fn.ps1 0x0045ca30,0x0045cab0
#   .\tools\check_fn.ps1 0x0045ca30 -Diff
#   .\tools\check_fn.ps1 0x0045ca30 -CleanFirst
#   .\tools\check_fn.ps1 0x0045ca30 -NoBuild

param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string[]]$Address,
    [switch]$Diff,
    [switch]$CleanFirst,
    [switch]$NoBuild
)

$ErrorActionPreference = 'Continue'
$Root = Split-Path -Parent $PSScriptRoot
Set-Location $Root

$BuildDir = Join-Path $Root 'build-msvc400'
$Reccmp = Join-Path $Root '.decomp-venv\Scripts\reccmp-reccmp.exe'
$QuietBuild = Join-Path $Root 'tools\quiet_build.ps1'

function Normalize-Addr([string]$a) {
    # PowerShell parses bare 0x0045ca30 as an Int before binding, so we may
    # receive "4573744". Re-encode decimal-only tokens as hex.
    $a = $a.Trim().ToLower()
    if ($a -match '^0x[0-9a-f]+$') {
        return $a
    }
    if ($a -match '^\d+$') {
        return ('0x{0:x8}' -f [int64]$a)
    }
    if ($a -match '^[0-9a-f]+$') {
        return '0x' + $a
    }
    return $a
}

$addrs = @($Address | ForEach-Object {
    $_ -split '[,;\s]+' | Where-Object { $_ -ne '' } | ForEach-Object { Normalize-Addr $_ }
})

if (-not $NoBuild) {
    $buildArgs = @{}
    if ($CleanFirst) { $buildArgs['CleanFirst'] = $true }
    & $QuietBuild @buildArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Output "BUILD_FAILED exit=$LASTEXITCODE (see build-msvc400/last_build.log)"
        exit $LASTEXITCODE
    }
}

Push-Location $BuildDir
try {
    foreach ($addr in $addrs) {
        $sw = [Diagnostics.Stopwatch]::StartNew()
        $raw = & $Reccmp --target LEMBALL --verbose $addr --print-rec-addr 2>&1 | ForEach-Object { "$_" }
        $sw.Stop()
        $text = $raw -join "`n"

        $failed = $text -match 'Failed to find a match|Debug data out of sync|Failed to find function symbol'
        $scoreLine = ($raw | Where-Object { $_ -match 'is only [\d.]+% similar|100%' } | Select-Object -Last 1)

        if ($failed) {
            Write-Output ("ADDR {0} PDB_DESYNC elapsed_s={1:N1} -> re-run with -CleanFirst" -f $addr, $sw.Elapsed.TotalSeconds)
            $hints = $raw | Where-Object { $_ -match 'Failed to find|Debug data out of sync|Error' }
            foreach ($h in $hints) { Write-Output $h }
            continue
        }

        if ($scoreLine) {
            Write-Output ("ADDR {0} {1} elapsed_s={2:N1}" -f $addr, $scoreLine.Trim(), $sw.Elapsed.TotalSeconds)
        }
        else {
            Write-Output ("ADDR {0} NO_SCORE_LINE elapsed_s={1:N1}" -f $addr, $sw.Elapsed.TotalSeconds)
        }

        if ($Diff) {
            Write-Output $text
        }
    }
}
finally {
    Pop-Location
}
