param([int]$WaitSeconds = 15)
$exe = "C:\Dev\lemball-decomp\build-msvc400\LEMBALL.EXE"
$p = Start-Process -FilePath $exe -WorkingDirectory (Split-Path $exe) -PassThru
Start-Sleep -Seconds $WaitSeconds
Add-Type @"
using System; using System.Text; using System.Runtime.InteropServices;
public class IntroWin32 {
  public delegate bool EnumProc(IntPtr hWnd, IntPtr lParam);
  [DllImport("user32.dll")] public static extern bool EnumWindows(EnumProc lpEnumFunc, IntPtr lParam);
  [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint pid);
  [DllImport("user32.dll", CharSet=CharSet.Auto)] public static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);
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
        [void][IntroWin32]::GetWindowText($h, $sb, 512)
        $titles.Add($sb.ToString())
    }
    return $true
}, [IntPtr]::Zero) | Out-Null
Write-Output "pid=$($p.Id) exited=$($p.HasExited)"
$titles | ForEach-Object { Write-Output "window: $_" }
if ($titles -match 'Runtime|Visual C\+\+') { exit 2 }
if (-not $p.HasExited) { Stop-Process -Id $p.Id -Force -ErrorAction SilentlyContinue }
