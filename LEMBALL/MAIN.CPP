#include "GAME.H"
#include "MAIN.H"

HINSTANCE g_hApplicationInstance = 0;

static int StoreHInstanceAndRunGameStartup(HINSTANCE hInstance, LPSTR lpCmdLine) {
    g_hApplicationInstance = hInstance;
    return RunGameStartupSequence(lpCmdLine);
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)nCmdShow;

    return StoreHInstanceAndRunGameStartup(hInstance, lpCmdLine);
}
