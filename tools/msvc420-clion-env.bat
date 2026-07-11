@echo off
rem Environment used by CLion for the faithful MSVC 4.20 build.
rem Set this explicitly because the original VCVARS32.BAT assumes a
rem Visual C++ 4.x shell and is not reliable when sourced by modern CLion.
if not defined MSVC420_ROOT for %%I in ("%~dp0..\msvc420") do set "MSVC420_ROOT=%%~fI"
set "MSDevDir=%MSVC420_ROOT%"
set "PATH=%MSDevDir%\bin;%MSDevDir%\bin\WINNT;%PATH%"
set "INCLUDE=%MSDevDir%\INCLUDE;%MSDevDir%\MFC\INCLUDE;%INCLUDE%"
set "LIB=%MSDevDir%\LIB;%MSDevDir%\MFC\LIB;%LIB%"
set "CC=%MSDevDir%\bin\CL.EXE"
set "CXX=%MSDevDir%\bin\CL.EXE"
