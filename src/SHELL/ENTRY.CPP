#include "SHELL/ENTRY.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/WIN32.H"
#include <string.h>

extern "C" BOOL WINAPI SetCurrentDirectoryA(LPCSTR lpPathName);

// GLOBAL: LEMBALL 0x004a8188
HINSTANCE g_hApplicationInstance;
HICON g_hApplicationIcon = 0;

#if defined(LEMBALL_SKIP_CD_TEST)
static void UseExecutableDirectoryForDevelopmentBuild(void) {
    char szExecutablePath[260];
    char *pszSeparator;

    if (GetModuleFileNameA(0, szExecutablePath, sizeof(szExecutablePath)) == 0) {
        return;
    }
    pszSeparator = strrchr(szExecutablePath, '\\');
    if (pszSeparator == 0) {
        return;
    }
    *pszSeparator = '\0';
    SetCurrentDirectoryA(szExecutablePath);
}
#endif

// FUNCTION: LEMBALL 0x004564e0
extern "C" int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    (void)hPrevInstance;
    (void)nCmdShow;
#if defined(LEMBALL_SKIP_CD_TEST)
    UseExecutableDirectoryForDevelopmentBuild();
#endif
    g_hApplicationInstance = hInstance;
    return RunGameStartupSequence(lpCmdLine);
}
