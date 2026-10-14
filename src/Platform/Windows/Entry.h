#ifndef LEMBALL_SCAFFOLD_PLATFORM_WINDOWS_ENTRY_H
#define LEMBALL_SCAFFOLD_PLATFORM_WINDOWS_ENTRY_H

#include "WindowsTypes.h"

extern HINSTANCE g_pApplicationInstance;
extern void* g_hApplicationIcon;

extern "C" int __stdcall WinMain(HINSTANCE p_hInstance, HINSTANCE p_hPrevInstance, char* p_lpCmdLine, int p_nCmdShow);
bool PumpEvents();
void SyncLoadProgress();

#endif
