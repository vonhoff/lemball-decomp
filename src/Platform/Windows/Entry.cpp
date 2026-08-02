#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"
#include "Platform/Windows/Entry.h"

// GLOBAL: LEMBALL 0x004a8188
HINSTANCE g_hApplicationInstance;
HICON g_hApplicationIcon = 0;

// FUNCTION: LEMBALL 0x004564e0
extern "C" int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	(void) hPrevInstance;
	(void) nCmdShow;
	g_hApplicationInstance = hInstance;
	return RunGameStartupSequence(lpCmdLine);
}
