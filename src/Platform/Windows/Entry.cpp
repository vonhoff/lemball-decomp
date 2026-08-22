#include "Entry.h"

void* g_hApplicationInstance = 0;
void* g_hApplicationIcon = 0;

// STUB: LEMBALL 0x004564e0
extern "C" int __stdcall WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nCmdShow)
{
	g_hApplicationInstance = hInstance;
	return 0;
}
