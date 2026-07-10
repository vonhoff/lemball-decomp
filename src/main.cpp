#include "main.h"
#include "engine/runtime_init.h"

HINSTANCE g_hApplicationInstance = 0;

// FUNCTION: LEMBALL 0x004564E0
extern "C" int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)nCmdShow;
    g_hApplicationInstance = hInstance;
    return RunGameStartupSequence(lpCmdLine);
}
