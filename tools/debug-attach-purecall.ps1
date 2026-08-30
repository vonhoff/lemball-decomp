$ErrorActionPreference = 'Continue'
$out = 'C:\Dev\lemball-decomp\windbg-out.txt'
$exe = 'C:\Dev\lemball-decomp\build-msvc400\LEMBALL.EXE'
Get-Process LEMBALL,cdb,cdbX86 -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
Remove-Item $out -ErrorAction SilentlyContinue

$p = Start-Process -FilePath $exe -WorkingDirectory (Split-Path $exe) -PassThru
Start-Sleep -Seconds 1
& C:\Dev\lemball-decomp\tools\auto-skip-intro.ps1 -WaitSeconds 7 -ProcessId $p.Id
Start-Sleep -Seconds 3

Add-Type @"
using System; using System.Text; using System.Runtime.InteropServices;
public static class CrashDbg4 {
  public delegate bool E(IntPtr h, IntPtr l);
  [DllImport("user32.dll")] public static extern bool EnumWindows(E e, IntPtr l);
  [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr h, out uint p);
  [DllImport("user32.dll",CharSet=CharSet.Auto)] public static extern int GetWindowText(IntPtr h, StringBuilder s, int n);
  [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr h);
}
"@ | Out-Null

$proc = Get-Process -Id $p.Id -ErrorAction SilentlyContinue
$tp = [uint32]$proc.Id
$runtime = $false
[CrashDbg4]::EnumWindows([CrashDbg4+E]{
    param($h,$l)
    $pidOut=[uint32]0; [void][CrashDbg4]::GetWindowThreadProcessId($h,[ref]$pidOut)
    if($pidOut -ne $script:tp -or -not [CrashDbg4]::IsWindowVisible($h)){ return $true }
    $sb=New-Object System.Text.StringBuilder 512
    [void][CrashDbg4]::GetWindowText($h,$sb,512)
    Write-Output ("window: " + $sb.ToString())
    if($sb.ToString() -match 'Runtime|Visual C\+\+'){ $script:runtime=$true }
    return $true
},0) | Out-Null
Write-Output "exited=$($proc.HasExited) runtime=$runtime"
if (-not $runtime) { exit 2 }

$wds = @'
.sympath+ C:\Dev\lemball-decomp\build-msvc400
.reload /f
.echo attached at runtime dialog
~* kb 20
q
'@
Set-Content 'C:\Dev\lemball-decomp\tools\debug-attach-purecall.wds' $wds -Encoding ASCII
& cdbX86.exe -p $proc.Id -cfr C:\Dev\lemball-decomp\tools\debug-attach-purecall.wds *> $out
Select-String -Path $out -Pattern 'attached|ChildEBP|RET|0044|0043|0046|0041|GunController|Wnd|GWnd|Sprite|SetSprite|Create|EndPhase|MainOptions|purecall' | ForEach-Object { $_.Line }
Get-Process LEMBALL,cdb,cdbX86 -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
