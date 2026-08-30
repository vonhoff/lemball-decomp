param(
    [int]$StartupWaitSeconds = 20,
    [int]$SkipWaitSeconds = 7
)

$ErrorActionPreference = 'Stop'
$out = 'C:\Dev\lemball-decomp\windbg-out.txt'
$deadline = (Get-Date).AddSeconds(180)
$p = $null
while ((Get-Date) -lt $deadline) {
    $p = Get-Process LEMBALL -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($p) { break }
    Start-Sleep -Milliseconds 500
}
if (-not $p) {
    Write-Error 'LEMBALL never started under cdb'
    exit 1
}
Write-Output "LEMBALL pid=$($p.Id)"
Start-Sleep -Seconds $StartupWaitSeconds

& C:\Dev\lemball-decomp\tools\auto-skip-intro.ps1 -WaitSeconds $SkipWaitSeconds -ProcessId $p.Id -RetryCount 10 -RetryDelayMs 400

$trapDeadline = (Get-Date).AddSeconds(45)
while ((Get-Date) -lt $trapDeadline) {
    if (Test-Path $out) {
        $text = Get-Content $out -Raw -ErrorAction SilentlyContinue
        if ($text -match '\*\*\* PURECALL \*\*\*') { break }
    }
    Start-Sleep -Milliseconds 250
}

if (Test-Path $out) {
    Get-Content $out -Tail 60
}
exit 0
