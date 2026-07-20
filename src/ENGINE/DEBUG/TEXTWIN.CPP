#include "ENGINE/DEBUG/CTEXTWIN.H"

#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/SAFEVT.H"

#include <string.h>

DECOMP_SIZE_ASSERT(CTextWindow, 0x58);

static const char g_PLATFORM_DebugWindowTitle[] = "Debug Window";

CTextWindow *g_pDebugTextWindow = 0;
static HACCEL g_hDebugWindowAccelerators = 0;
static void *g_CTextWindowVTable[2] = {
    (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop
};
extern HANDLE g_hDebugSyncEvent;
extern void __cdecl ShowFatalGetLastErrorMessageAndExit(
    const char *pszMessage);

class CTextWindowSyncInterface {
public:
    virtual void BeginUpdate(void);
    virtual void EndUpdate(void);
};

// FUNCTION: LEMBALL 0x00472b10
int __stdcall DebugMessageThreadMain(void) {
    MSG Msg;
    int nMessageResult;
    void *pvTextWindow;

    pvTextWindow = AllocateVSMemBlock(0x58);
    if (pvTextWindow == 0) {
        g_pDebugTextWindow = 0;
    } else {
        g_pDebugTextWindow = (CTextWindow *)pvTextWindow;
        g_pDebugTextWindow->m_pVTable = g_CTextWindowVTable;
        g_pDebugTextWindow->m_pszTitle = g_PLATFORM_DebugWindowTitle;
        g_pDebugTextWindow->m_cchBuffer = 0x2800;
    }

    SetEvent(g_hDebugSyncEvent);
    nMessageResult = GetMessageA(&Msg, 0, 0, 0);
    while (nMessageResult != 0) {
        if (g_hDebugWindowAccelerators == 0 || TranslateAcceleratorA(Msg.hwnd, g_hDebugWindowAccelerators, &Msg) == 0) {
            TranslateMessage(&Msg);
            DispatchMessageA(&Msg);
        }

        nMessageResult = GetMessageA(&Msg, 0, 0, 0);
    }

    SetEvent(g_hDebugSyncEvent);
    return 1;
}

// FUNCTION: LEMBALL 0x00473d90
void CTextWindow::PostAllocatedTextControlString(const char *pszText, unsigned int dwColour) {
    char *pszCopy;
    unsigned int cchText;

    ((CTextWindowSyncInterface *)this)->BeginUpdate();
    if (g_pDebugTextWindow == 0) {
        return;
    }
    cchText = (unsigned int)strlen(pszText) + 1;
    pszCopy = (char *)AllocateVSMemBlock(cchText);
    if (pszCopy == 0) {
        ShowFatalGetLastErrorMessageAndExit(
            "Unable to allocate memory for string copy");
    }
    strcpy(pszCopy, pszText);
    if (*(HWND *)((char *)this + 0x1c) != 0) {
        PostMessageA(*(HWND *)((char *)this + 0x1c), 0x420,
                     (WPARAM)pszCopy, (LPARAM)dwColour);
    }
    ((CTextWindowSyncInterface *)this)->EndUpdate();
}
