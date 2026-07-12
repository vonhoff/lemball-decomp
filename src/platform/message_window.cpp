#include "message_window.h"
#include "../network/safe_vtable.h"

#include "../main.h"
#include "../engine/memory_arena.h"
#include "../engine/runtime_init.h"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif

static const char g_PLATFORM_ErrorTitle[] = "ERROR";
static const char g_PLATFORM_BaseWindowRegisterError[] = "Unable to register base window class";
static const char g_PLATFORM_InvisibleWindowTitle[] = "Invisible Window";
static const char g_PLATFORM_InvisibleWindowCreateError[] = "Failed to create invisible window - Error code: ";
static const char g_PLATFORM_ErrorSuffix[] = "\n";
static const char g_PLATFORM_DebugWindowTitle[] = "Debug Window";

static PLATFORM_DebugTextWindow *g_pDebugTextWindow = 0;
static HACCEL g_hDebugWindowAccelerators = 0;
static void *g_PLATFORM_InvisibleMessageWindowVTable[1] = { (void *)NetworkSafeVtableNoop };
static void *g_PLATFORM_DebugTextWindowVTable[1] = { (void *)NetworkSafeVtableNoop };
static int *g_pfWindowMessagePassthroughMode;
extern HANDLE g_hDebugSyncEvent;

typedef LRESULT (*PLATFORM_WindowMessageHandler)(PLATFORM_InvisibleMessageWindow *pWindow,
                                                 UINT uMessage,
                                                 WPARAM wParam,
                                                 LPARAM lParam);

// FUNCTION: LEMBALL 0x0047C160
LRESULT CALLBACK InvisibleMessageWindowWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    PLATFORM_InvisibleMessageWindow *pWindow;
    LRESULT lResult;
    CREATESTRUCTA *pCreateStruct;

    if (*g_pfWindowMessagePassthroughMode != 0) {
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);
    }

    pWindow = (PLATFORM_InvisibleMessageWindow *)(unsigned long)GetWindowLongA(hWnd, GWL_USERDATA);
    if (uMessage == WM_CREATE) {
        pCreateStruct = (CREATESTRUCTA *)(unsigned long)lParam;
        SetWindowLongA(hWnd, GWL_USERDATA, (LONG)(unsigned long)pCreateStruct->lpCreateParams);
        return 0;
    }

    if (pWindow != 0) {
        if (pWindow->m_dwReserved1 == 0 || uMessage < pWindow->m_dwReserved0 || pWindow->m_dwReserved1 < uMessage) {
            lResult = ((PLATFORM_WindowMessageHandler)(*(void **)pWindow->m_pVTable))(pWindow, uMessage, wParam, lParam);
            if (lResult != -1) {
                return lResult;
            }
        }
    }

    return DefWindowProcA(hWnd, uMessage, wParam, lParam);
}

// FUNCTION: LEMBALL 0x0047C050
int PLATFORM_InvisibleMessageWindow::Construct(LPCSTR pszClassName, int *pfClassRegistered) {
    WNDCLASSA WndClass;

    m_pVTable = g_PLATFORM_InvisibleMessageWindowVTable;
    m_dwReserved0 = 0;
    m_dwReserved1 = 0;

    if (*pfClassRegistered == 0) {
        WndClass.style = CS_DBLCLKS;
        WndClass.lpfnWndProc = InvisibleMessageWindowWndProc;
        WndClass.cbClsExtra = 0;
        WndClass.cbWndExtra = 4;
        WndClass.hInstance = g_hApplicationInstance;
        WndClass.hIcon = 0;
        WndClass.hCursor = 0;
        WndClass.hbrBackground = 0;
        WndClass.lpszMenuName = 0;
        WndClass.lpszClassName = pszClassName;

        if (RegisterClassA(&WndClass) == 0) {
            MessageBoxA(0, g_PLATFORM_BaseWindowRegisterError, g_PLATFORM_ErrorTitle, MB_OK | MB_ICONERROR);
            ExitProcess(0xbbbb);
            return 0;
        }

        *pfClassRegistered = 1;
    }

    m_hWnd = CreateWindowExA(0,
                             pszClassName,
                             g_PLATFORM_InvisibleWindowTitle,
                             0,
                             (INT)CW_USEDEFAULT,
                             (INT)CW_USEDEFAULT,
                             (INT)CW_USEDEFAULT,
                             (INT)CW_USEDEFAULT,
                             0,
                             0,
                             g_hApplicationInstance,
                             this);

    if (m_hWnd == 0) {
        AppendErrorCString(g_PLATFORM_InvisibleWindowCreateError);
        AppendErrorUInt(GetLastError());
        AppendErrorCString(g_PLATFORM_ErrorSuffix);
    }

    return m_hWnd != 0;
}

// FUNCTION: LEMBALL 0x0047C140
void DestroyInvisibleMessageWindow(PLATFORM_InvisibleMessageWindow *pWindow) {
    if (pWindow == 0) {
        return;
    }

    *(void ***)pWindow = g_PLATFORM_InvisibleMessageWindowVTable;
    DestroyWindow(pWindow->m_hWnd);
}

PLATFORM_DebugTextWindow *PLATFORM_DebugTextWindow::Construct(const char *pszTitle, unsigned int cchBuffer) {
    m_pVTable = g_PLATFORM_DebugTextWindowVTable;
    m_pszTitle = pszTitle;
    m_cchBuffer = cchBuffer;
    return this;
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
            ((PLATFORM_DebugTextWindow *)pvTextWindow)->Construct(g_PLATFORM_DebugWindowTitle, 0x2800);
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

PLATFORM_DebugTextWindow *GetDebugTextWindow(void) {
    return g_pDebugTextWindow;
}
