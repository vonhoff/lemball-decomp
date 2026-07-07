#include "game.h"
#include "main.h"

HINSTANCE g_hApplicationInstance = 0;

// FUNCTION: LEMBALL 0x004564E0
static void PASCAL StoreHInstanceAndRunGameStartup(HINSTANCE hInstance,
                                                   HINSTANCE hPrevInstance,
                                                   LPSTR lpCmdLine,
                                                   int nCmdShow) {
    (void)hPrevInstance;
    (void)nCmdShow;
    g_hApplicationInstance = hInstance;
    RunGameStartupSequence(lpCmdLine);
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    StoreHInstanceAndRunGameStartup(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return 0;
}
