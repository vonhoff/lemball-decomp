# Quiet CMake build wrapper for agent/CI loops.
# Full log -> build-msvc400/last_build.log
# Stdout -> warnings/errors + final status + elapsed only.
#
# Usage:
#   .\tools\quiet_build.ps1
#   .\tools\quiet_build.ps1 -CleanFirst
#   .\tools\quiet_build.ps1 -ExtraArgs @('--target','LEMBALL')

param(
    [switch]$CleanFirst,
    [string[]]$ExtraArgs = @()
)

$ErrorActionPreference = 'Continue'
$Root = Split-Path -Parent $PSScriptRoot
if (-not $Root) { $Root = (Get-Location).Path }
Set-Location $Root

$BuildDir = Join-Path $Root 'build-msvc400'
$LogPath = Join-Path $BuildDir 'last_build.log'
$CMake = Join-Path $Root '.decomp-venv\Scripts\cmake.exe'

if (-not (Test-Path $CMake)) {
    Write-Output "ERROR: missing $CMake"
    exit 2
}

$cmakeArgs = @('--build', '--preset', 'msvc400')
if ($CleanFirst) {
    $cmakeArgs += '--clean-first'
}
if ($ExtraArgs.Count -gt 0) {
    $cmakeArgs += $ExtraArgs
}

$sw = [Diagnostics.Stopwatch]::StartNew()
$raw = & $CMake @cmakeArgs 2>&1 | ForEach-Object { "$_" }
$sw.Stop()
$exit = $LASTEXITCODE
$text = ($raw -join "`n") + "`n"

New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
Set-Content -Path $LogPath -Value $text -Encoding utf8

$keep = $raw | Where-Object {
    $_ -match '(?i)warning|error|fatal|failed|built target|linking|\[\s*100%\s*\]'
}
foreach ($line in $keep) {
    Write-Output $line
}

$exe = Join-Path $BuildDir 'LEMBALL.EXE'
$pdb = Join-Path $BuildDir 'LEMBALL.pdb'
$hasExe = Test-Path $exe
$hasPdb = Test-Path $pdb

Write-Output ("RESULT exit={0} elapsed_s={1:N1} exe={2} pdb={3} log={4}" -f `
    $exit, $sw.Elapsed.TotalSeconds, $hasExe, $hasPdb, $LogPath)

exit $exit
