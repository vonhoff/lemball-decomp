param(
    [int]$StartupWaitSeconds = 25,
    [int]$SkipWaitSeconds = 7
)

$ErrorActionPreference = 'Stop'
$out = 'C:\Dev\lemball-decomp\windbg-out.txt'
Remove-Item $out -ErrorAction SilentlyContinue

Get-Process LEMBALL,cdb,cdbX86 -ErrorAction SilentlyContinue | Stop-Process -Force

$proc = Start-Process -FilePath 'cdbX86.exe' `
    -ArgumentList @('-cfr', 'C:\Dev\lemball-decomp\tools\debug-intro-crash.wds', 'C:\Dev\lemball-decomp\build-msvc400\LEMBALL.EXE') `
    -WorkingDirectory 'C:\Dev\lemball-decomp\build-msvc400' `
    -PassThru -WindowStyle Hidden

Start-Sleep -Seconds 5
$deadline = (Get-Date).AddSeconds(120)
$game = $null
while ((Get-Date) -lt $deadline) {
    $game = Get-Process LEMBALL -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($game) { break }
    Start-Sleep -Milliseconds 500
}
if (-not $game) {
    Write-Error 'LEMBALL never started'
    Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue
    exit 1
}
Write-Output "LEMBALL pid=$($game.Id) cdb pid=$($proc.Id)"
Start-Sleep -Seconds $StartupWaitSeconds

& C:\Dev\lemball-decomp\tools\auto-skip-intro.ps1 -WaitSeconds $SkipWaitSeconds -ProcessId $game.Id -RetryCount 10 -RetryDelayMs 400

$trapDeadline = (Get-Date).AddSeconds(60)
while ((Get-Date) -lt $trapDeadline) {
    if (Test-Path $out) {
        $text = Get-Content $out -Raw -ErrorAction SilentlyContinue
        if ($text -match '\*\*\* PURECALL \*\*\*') { break }
    }
    if ($proc.HasExited) { break }
    Start-Sleep -Milliseconds 250
}

if (-not $proc.HasExited) {
    Stop-Process -Id $proc.Id -Force -ErrorAction SilentlyContinue
}
Get-Process LEMBALL -ErrorAction SilentlyContinue | Stop-Process -Force

if (Test-Path $out) {
    Select-String -Path $out -Pattern 'PURECALL|ChildEBP|LEMBALL!|debug session' | ForEach-Object { $_.Line }
}
