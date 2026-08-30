param(
    [int]$WaitSeconds = 7
)

$exe = "C:\Dev\lemball-decomp\build-msvc400\LEMBALL.EXE"
$p = Start-Process -FilePath $exe -WorkingDirectory (Split-Path $exe) -PassThru
Start-Sleep -Seconds 1
& "$PSScriptRoot\auto-skip-intro.ps1" -WaitSeconds $WaitSeconds -ProcessId $p.Id
Start-Sleep -Seconds 3

Add-Type @"
using System;
using System.Text;
using System.Runtime.InteropServices;
public class IntroWin32 {
  public delegate bool EnumProc(IntPtr hWnd, IntPtr lParam);
  [DllImport("user32.dll")] public static extern bool EnumWindows(EnumProc lpEnumFunc, IntPtr lParam);
  [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint pid);
  [DllImport("user32.dll", CharSet=CharSet.Auto)] public static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);
  [DllImport("user32.dll", CharSet=CharSet.Auto)] public static extern int GetClassName(IntPtr hWnd, StringBuilder lpString, int nMaxCount);
  [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr hWnd);
}
"@

$targetPid = [uint32]$p.Id
$titles = New-Object System.Collections.Generic.List[string]
[IntroWin32]::EnumWindows([IntroWin32+EnumProc]{
    param($h, $l)
    $pidOut = [uint32]0
    [void][IntroWin32]::GetWindowThreadProcessId($h, [ref]$pidOut)
    if ($pidOut -eq $targetPid -and [IntroWin32]::IsWindowVisible($h)) {
        $sb = New-Object System.Text.StringBuilder 512
        $cb = New-Object System.Text.StringBuilder 256
        [void][IntroWin32]::GetWindowText($h, $sb, 512)
        [void][IntroWin32]::GetClassName($h, $cb, 256)
        $titles.Add(("title='{0}' class='{1}'" -f $sb.ToString(), $cb.ToString()))
    }
    return $true
}, [IntPtr]::Zero) | Out-Null

Write-Output "pid=$($p.Id) exited=$($p.HasExited) exit=$($p.ExitCode)"
Write-Output "windows:"
$titles | ForEach-Object { Write-Output $_ }

$runtimeDialog = $titles | Where-Object { $_ -match 'Runtime|Debug|Error|pure|R6025|Microsoft Visual C\+\+' }
if ($runtimeDialog) {
    Write-Output "RESULT=RUNTIME_ERROR_DIALOG"
    exit 2
}
if ($titles -match "Lemmings|Paintball|LEMBALL") {
    Write-Output "RESULT=GAME_WINDOW"
    exit 0
}
if (-not $p.HasExited) {
    Write-Output "RESULT=ALIVE_UNKNOWN"
    exit 1
}
Write-Output "RESULT=EXITED"
exit 3
