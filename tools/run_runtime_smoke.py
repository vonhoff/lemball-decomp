#!/usr/bin/env python3
"""Run the reconstructed game against the mounted Paintball CD.

The release binary opens ``pbaimog.vsr`` relative to its process working
directory.  Runtime validation therefore launches the locally built EXE with
the mounted ``D:\\LEMBALL`` directory as its working directory.  Copying the
archive beside the rebuilt executable would hide regressions in that behavior.
"""

from __future__ import annotations

import argparse
import ctypes
import subprocess
import sys
import time
from ctypes import wintypes
from pathlib import Path


EXPECTED_TITLE = "Lemmings Paintball"
FAILURE_TITLES = {
    "Paintball Not Installed",
    "Unable to find CD",
}
WM_NULL = 0x0000
SMTO_ABORTIFHUNG = 0x0002


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("--exe", type=Path, required=True)
    parser.add_argument("--media-directory", type=Path, default=Path(r"D:\LEMBALL"))
    parser.add_argument("--duration", type=float, default=60.0)
    parser.add_argument(
        "--mode",
        choices=("default", "noanim", "both"),
        default="both",
    )
    return parser.parse_args()


def process_windows(pid: int) -> list[tuple[int, str]]:
    user32 = ctypes.WinDLL("user32", use_last_error=True)
    windows: list[tuple[int, str]] = []
    enum_proc_type = ctypes.WINFUNCTYPE(wintypes.BOOL, wintypes.HWND, wintypes.LPARAM)
    user32.EnumWindows.argtypes = (enum_proc_type, wintypes.LPARAM)
    user32.EnumWindows.restype = wintypes.BOOL
    user32.GetWindowThreadProcessId.argtypes = (wintypes.HWND, ctypes.POINTER(wintypes.DWORD))
    user32.GetWindowThreadProcessId.restype = wintypes.DWORD
    user32.IsWindowVisible.argtypes = (wintypes.HWND,)
    user32.IsWindowVisible.restype = wintypes.BOOL
    user32.GetWindowTextLengthW.argtypes = (wintypes.HWND,)
    user32.GetWindowTextLengthW.restype = ctypes.c_int
    user32.GetWindowTextW.argtypes = (wintypes.HWND, wintypes.LPWSTR, ctypes.c_int)
    user32.GetWindowTextW.restype = ctypes.c_int

    def visit(hwnd: int, _lparam: int) -> bool:
        window_pid = wintypes.DWORD()
        user32.GetWindowThreadProcessId(hwnd, ctypes.byref(window_pid))
        if window_pid.value != pid or not user32.IsWindowVisible(hwnd):
            return True
        length = user32.GetWindowTextLengthW(hwnd)
        title = ctypes.create_unicode_buffer(length + 1)
        user32.GetWindowTextW(hwnd, title, len(title))
        windows.append((hwnd, title.value))
        return True

    callback = enum_proc_type(visit)
    if not user32.EnumWindows(callback, 0):
        raise ctypes.WinError(ctypes.get_last_error())
    return windows


def window_is_responsive(hwnd: int) -> bool:
    user32 = ctypes.WinDLL("user32", use_last_error=True)
    result = ctypes.c_size_t()
    user32.SendMessageTimeoutW.argtypes = (
        wintypes.HWND,
        wintypes.UINT,
        wintypes.WPARAM,
        wintypes.LPARAM,
        wintypes.UINT,
        wintypes.UINT,
        ctypes.POINTER(ctypes.c_size_t),
    )
    user32.SendMessageTimeoutW.restype = wintypes.LPARAM
    return bool(
        user32.SendMessageTimeoutW(
            hwnd,
            WM_NULL,
            0,
            0,
            SMTO_ABORTIFHUNG,
            1000,
            ctypes.byref(result),
        )
    )


def stop_test_process(process: subprocess.Popen[bytes]) -> None:
    if process.poll() is not None:
        return
    process.terminate()
    try:
        process.wait(timeout=5)
    except subprocess.TimeoutExpired:
        process.kill()
        process.wait(timeout=5)


def run_mode(exe: Path, media_directory: Path, duration: float, noanim: bool) -> None:
    arguments = [str(exe)]
    label = "noanim" if noanim else "default"
    if noanim:
        arguments.append("/NOANIM")

    process = subprocess.Popen(arguments, cwd=media_directory)
    deadline = time.monotonic() + duration
    saw_main_window = False
    unresponsive_since: float | None = None
    try:
        while time.monotonic() < deadline:
            exit_code = process.poll()
            if exit_code is not None:
                raise RuntimeError(f"{label}: exited early with code 0x{exit_code & 0xFFFFFFFF:08X}")

            windows = process_windows(process.pid)
            titles = {title for _hwnd, title in windows if title}
            failures = FAILURE_TITLES & titles
            if failures:
                raise RuntimeError(f"{label}: displayed failure dialog {sorted(failures)[0]!r}")

            main_windows = [hwnd for hwnd, title in windows if title == EXPECTED_TITLE]
            if main_windows:
                saw_main_window = True
                if any(window_is_responsive(hwnd) for hwnd in main_windows):
                    unresponsive_since = None
                elif unresponsive_since is None:
                    unresponsive_since = time.monotonic()
                elif time.monotonic() - unresponsive_since >= 5:
                    raise RuntimeError(f"{label}: main window stopped responding")
            time.sleep(0.25)

        if not saw_main_window:
            raise RuntimeError(f"{label}: never displayed {EXPECTED_TITLE!r}")
    finally:
        stop_test_process(process)

    print(f"{label}: passed mounted-CD smoke test")


def main() -> int:
    if sys.platform != "win32":
        raise SystemExit("runtime smoke testing requires Windows")
    args = parse_args()
    exe = args.exe.resolve()
    media_directory = args.media_directory.resolve()
    if not exe.is_file():
        raise SystemExit(f"rebuilt executable does not exist: {exe}")
    if (exe.parent / "PBAIMOG.VSR").exists():
        raise SystemExit(f"local archive copy must be removed: {exe.parent / 'PBAIMOG.VSR'}")
    archive = media_directory / "PBAIMOG.VSR"
    cd_probe = Path(media_directory.anchor) / "VSMEM.DLL"
    for required in (archive, cd_probe):
        if not required.is_file():
            raise SystemExit(f"mounted-CD file is missing: {required}")
    if args.duration <= 0:
        raise SystemExit("--duration must be positive")

    modes = (False, True) if args.mode == "both" else (args.mode == "noanim",)
    for noanim in modes:
        run_mode(exe, media_directory, args.duration, noanim)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
