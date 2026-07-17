#include "SHELL/ENTRY.H"
#include "ENGINE/CORE/VSINIT.H"

HINSTANCE g_hApplicationInstance = 0;
HICON g_hApplicationIcon = 0;

// FUNCTION: LEMBALL 0x004564e0
extern "C" int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)nCmdShow;
    g_hApplicationInstance = hInstance;
    return RunGameStartupSequence(lpCmdLine);
}
