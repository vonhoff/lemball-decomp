param(
    [int]$WaitSeconds = 5
)

$exe = "C:\Dev\lemball-decomp\build-msvc400\LEMBALL.EXE"
$p = Start-Process -FilePath $exe -WorkingDirectory (Split-Path $exe) -PassThru
Start-Sleep -Seconds 1
& C:\Dev\lemball-decomp\tools\auto-skip-intro.ps1 -WaitSeconds ($WaitSeconds - 1) -ProcessId $p.Id
$deadline = (Get-Date).AddSeconds(20)
while (-not $p.HasExited -and (Get-Date) -lt $deadline) {
    Start-Sleep -Milliseconds 100
}
if (-not $p.HasExited) {
    Stop-Process -Id $p.Id -Force
    "alive after skip - OK (reached menu or still running)"
} else {
    "exit=$($p.ExitCode) after skip"
}
