@echo off
setlocal
set OUT=C:\Dev\lemball-decomp\windbg-out.txt
set EXE=C:\Dev\lemball-decomp\build-msvc400\LEMBALL.EXE
del /q "%OUT%" 2>nul
taskkill /IM LEMBALL.EXE /F >nul 2>&1
taskkill /IM cdb.exe /F >nul 2>&1
taskkill /IM cdbX86.exe /F >nul 2>&1

cd /d C:\Dev\lemball-decomp\build-msvc400
start "cdb-intro" /b cdbX86.exe -cfr C:\Dev\lemball-decomp\tools\debug-intro-crash.wds LEMBALL.EXE >> "%OUT%" 2>&1

powershell -NoProfile -ExecutionPolicy Bypass -File C:\Dev\lemball-decomp\tools\debug-intro-crash-helper.ps1
set ERR=%ERRORLEVEL%
echo helper exit=%ERR%
type "%OUT%" | findstr /I "PURECALL EndPhase SetSpriteWindow WndCreate kb ChildEBP RET ln "
exit /b %ERR%
