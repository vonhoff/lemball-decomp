$exe = 'C:\Dev\lemball-decomp\build-msvc400\LEMBALL.EXE'
Get-Process LEMBALL -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
$p = Start-Process -FilePath $exe -ArgumentList '/NOANIM' -WorkingDirectory (Split-Path $exe) -PassThru
Start-Sleep -Seconds 12

Add-Type @"
using System;
using System.Text;
using System.Runtime.InteropServices;
public class NoAnimWin32 {
  public delegate bool EnumProc(IntPtr hWnd, IntPtr lParam);
  [DllImport("user32.dll")] public static extern bool EnumWindows(EnumProc lpEnumFunc, IntPtr lParam);
  [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint pid);
  [DllImport("user32.dll", CharSet=CharSet.Auto)] public static extern int GetWindowText(IntPtr hWnd, StringBuilder lpString, int nMaxCount);
  [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr hWnd);
}
"@

$targetPid = [uint32]$p.Id
$titles = New-Object System.Collections.Generic.List[string]
[NoAnimWin32]::EnumWindows([NoAnimWin32+EnumProc]{
    param($h, $l)
    $pidOut = [uint32]0
    [void][NoAnimWin32]::GetWindowThreadProcessId($h, [ref]$pidOut)
    if ($pidOut -eq $targetPid -and [NoAnimWin32]::IsWindowVisible($h)) {
        $sb = New-Object System.Text.StringBuilder 512
        [void][NoAnimWin32]::GetWindowText($h, $sb, 512)
        $titles.Add($sb.ToString())
    }
    return $true
}, [IntPtr]::Zero) | Out-Null

Write-Output "pid=$($p.Id) exited=$($p.HasExited) exit=$($p.ExitCode)"
$titles | ForEach-Object { Write-Output "window: $_" }
if ($titles | Where-Object { $_ -match 'Runtime|Visual C\+\+' }) { Write-Output 'RESULT=RUNTIME_ERROR' }
elseif (-not $p.HasExited) { Write-Output 'RESULT=ALIVE' }
else { Write-Output "RESULT=EXIT $($p.ExitCode)" }
if (-not $p.HasExited) { Stop-Process -Id $p.Id -Force -ErrorAction SilentlyContinue }
