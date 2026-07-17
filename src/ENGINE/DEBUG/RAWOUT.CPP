#include "ENGINE/DEBUG/VSDBG.H"

#include "ENGINE/DEBUG/CTEXTWIN.H"
#include "ENGINE/CORE/VSINIT.H"

extern int g_fSubsystemsReady;
extern int g_fDebugFileEnabled;
extern CTextWindow *g_pDebugTextWindow;

void PostAllocatedTextControlString(CTextWindow *pWindow,
                                    const char *pszText,
                                    unsigned int dwColour);

static int RawOutputString(const char *pszText,
                           const char *pszPreservedName,
                           unsigned int dwColour) {
    if (g_fSubsystemsReady == 0) {
        MessageBoxA(0, pszText, pszPreservedName, 0);
        return 1;
    }

    if (g_pDebugTextWindow != 0) {
        PostAllocatedTextControlString(g_pDebugTextWindow, pszText, dwColour);
        return g_pDebugTextWindow != 0;
    }

    if (g_fDebugFileEnabled != 0) {
        AppendStringToDebugOutFile(pszText);
    }
    return g_pDebugTextWindow != 0;
}

// FUNCTION: LEMBALL 0x00472910
int _RAWOUT_DebugString(const char *pszText) {
    return RawOutputString(pszText, "_RAWOUT_DebugString", 0x00008000);
}

// FUNCTION: LEMBALL 0x00472980
int _RAWOUT_ErrorString(const char *pszText) {
    return RawOutputString(pszText, "_RAWOUT_ErrorString", 0x000000ff);
}

// FUNCTION: LEMBALL 0x004729f0
int _RAWOUT_SysString(const char *pszText) {
    return RawOutputString(pszText, "_RAWOUT_SysString", 0x00ff0000);
}
