#ifndef LEMBALL_SCAFFOLD_PLATFORM_WINDOWS_ENTRY_H
#define LEMBALL_SCAFFOLD_PLATFORM_WINDOWS_ENTRY_H

extern void* g_pApplicationInstance;
extern void* g_hApplicationIcon;

extern "C" int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nCmdShow);
bool PumpEvents();
void SyncLoadProgress();

#endif
