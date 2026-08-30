param(
    [int]$WaitSeconds = 5,
    [int]$ProcessId = 0,
    [int]$RetryCount = 5,
    [int]$RetryDelayMs = 400
)

Add-Type @"
using System;
using System.Runtime.InteropServices;
public static class SkipIntroWin32 {
    public delegate bool EnumWindowsProc(IntPtr hWnd, IntPtr lParam);
    [DllImport("user32.dll")] public static extern bool EnumWindows(EnumWindowsProc lpEnumFunc, IntPtr lParam);
    [DllImport("user32.dll")] public static extern uint GetWindowThreadProcessId(IntPtr hWnd, out uint lpdwProcessId);
    [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern IntPtr SendMessage(IntPtr hWnd, uint Msg, IntPtr wParam, IntPtr lParam);
    [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern bool AttachThreadInput(uint idAttach, uint idAttachTo, bool fAttach);
    [DllImport("user32.dll")] public static extern bool AllowSetForegroundWindow(uint dwProcessId);
    [DllImport("kernel32.dll")] public static extern uint GetCurrentThreadId();
    public const uint WM_KEYDOWN = 0x0100;
    public const uint WM_KEYUP = 0x0101;
}
"@

function Get-GameWindow {
    param([int]$TargetPid)
    $proc = Get-Process -Id $TargetPid -ErrorAction Stop
    if ($proc.MainWindowHandle -ne [IntPtr]::Zero) {
        return $proc.MainWindowHandle
    }
    $found = [IntPtr]::Zero
    $target = [uint32]$TargetPid
    $cb = [SkipIntroWin32+EnumWindowsProc]{
        param($hWnd, $lParam)
        if (-not [SkipIntroWin32]::IsWindowVisible($hWnd)) { return $true }
        $winPid = [uint32]0
        [void][SkipIntroWin32]::GetWindowThreadProcessId($hWnd, [ref]$winPid)
        if ($winPid -eq $script:target) {
            $script:found = $hWnd
            return $false
        }
        return $true
    }
    [void][SkipIntroWin32]::EnumWindows($cb, [IntPtr]::Zero)
    return $found
}

function Send-SkipKey {
    param([IntPtr]$Hwnd, [UInt32]$GamePid)
    # WM_KEYDOWN/UP -> Wnd::ProcessMessage -> Translator -> IntroAnimDrawer::EndPhase
    $nullPid = [uint32]0
    $gameTid = [SkipIntroWin32]::GetWindowThreadProcessId($Hwnd, [ref]$nullPid)
    $ourTid = [SkipIntroWin32]::GetCurrentThreadId()
    [void][SkipIntroWin32]::AllowSetForegroundWindow($GamePid)
    [void][SkipIntroWin32]::AttachThreadInput($ourTid, $gameTid, $true)
    [void][SkipIntroWin32]::SetForegroundWindow($Hwnd)
    [void][SkipIntroWin32]::SendMessage($Hwnd, [SkipIntroWin32]::WM_KEYDOWN, [IntPtr]0x20, [IntPtr]0x00390001)
    Start-Sleep -Milliseconds 30
    [void][SkipIntroWin32]::SendMessage($Hwnd, [SkipIntroWin32]::WM_KEYUP, [IntPtr]0x20, [IntPtr]0xC0390001)
    [void][SkipIntroWin32]::AttachThreadInput($ourTid, $gameTid, $false)
    return $true
}

if ($ProcessId -eq 0) {
    Write-Error "ProcessId required (LEMBALL pid)"
    exit 1
}

$deadline = (Get-Date).AddSeconds(60)
$hwnd = [IntPtr]::Zero
while ((Get-Date) -lt $deadline) {
    Start-Sleep -Milliseconds 200
    try {
        $proc = Get-Process -Id $ProcessId -ErrorAction Stop
    } catch {
        continue
    }
    if ($proc.HasExited) {
        Write-Error "Process $ProcessId exited before window appeared"
        exit 1
    }
    $hwnd = Get-GameWindow -TargetPid $ProcessId
    if ($hwnd -ne [IntPtr]::Zero) { break }
}

if ($hwnd -eq [IntPtr]::Zero) {
    Write-Error "Game window not found for pid=$ProcessId"
    exit 1
}

Start-Sleep -Seconds $WaitSeconds

for ($i = 0; $i -lt $RetryCount; $i++) {
    Send-SkipKey -Hwnd $hwnd -GamePid ([UInt32]$ProcessId) | Out-Null
    Start-Sleep -Milliseconds $RetryDelayMs
    try {
        $proc = Get-Process -Id $ProcessId -ErrorAction Stop
        if ($proc.HasExited) { break }
    } catch {
        break
    }
}

Write-Output "Sent skip key via SendMessage to hwnd=$hwnd pid=$ProcessId after ${WaitSeconds}s wait"
