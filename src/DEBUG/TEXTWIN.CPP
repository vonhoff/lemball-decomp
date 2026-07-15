#include "DEBUG/CTEXTWIN.H"

#include "CORE/VSINIT.H"
#include "MEMORY/VSMEM.H"
#include "CORE/SAFEVT.H"

#include <string.h>

DECOMP_SIZE_ASSERT(CTextWindow, 0x58);

static const char g_PLATFORM_DebugWindowTitle[] = "Debug Window";

CTextWindow *g_pDebugTextWindow = 0;
static HACCEL g_hDebugWindowAccelerators = 0;
static void *g_CTextWindowVTable[1] = { (void *)NetworkSafeVtableNoop };
extern HANDLE g_hDebugSyncEvent;

CTextWindow *CTextWindow::Construct(const char *pszTitle, unsigned int cchBuffer) {
    m_pVTable = g_CTextWindowVTable;
    m_pszTitle = pszTitle;
    m_cchBuffer = cchBuffer;
    return this;
}

void PostAllocatedTextControlString(CTextWindow *pWindow,
                                    const char *pszText,
                                    unsigned int dwColour) {
    char *pszCopy;
    unsigned int cchText;

    cchText = (unsigned int)strlen(pszText) + 1;
    pszCopy = (char *)AllocateVSMemBlock(cchText);
    if (pszCopy == 0) {
        return;
    }
    memcpy(pszCopy, pszText, cchText);
    PostMessageA(*(HWND *)((char *)pWindow + 0x1c), 0x420, (WPARAM)pszCopy,
                 (LPARAM)dwColour);
}

// FUNCTION: LEMBALL 0x00472B10
int __stdcall DebugMessageThreadMain(void) {
    MSG Msg;
    int nMessageResult;
    void *pvTextWindow;

    pvTextWindow = AllocateVSMemBlock(0x58);
    if (pvTextWindow == 0) {
        g_pDebugTextWindow = 0;
    } else {
        g_pDebugTextWindow =
            ((CTextWindow *)pvTextWindow)->Construct(g_PLATFORM_DebugWindowTitle, 0x2800);
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

CTextWindow *GetDebugTextWindow(void) {
    return g_pDebugTextWindow;
}
