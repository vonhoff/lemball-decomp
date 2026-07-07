#include "vswin.h"

#include "main.h"
#include "vsmem.h"
#include "vsinit.h"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif

static const char g_VSWIN_ErrorTitle[] = "ERROR";
static const char g_VSWIN_BaseWindowRegisterError[] = "Unable to register base window class";
static const char g_VSWIN_InvisibleWindowTitle[] = "Invisible Window";
static const char g_VSWIN_InvisibleWindowCreateError[] = "Failed to create invisible window - Error code: ";
static const char g_VSWIN_ErrorSuffix[] = "\n";
static const char g_VSWIN_DebugWindowTitle[] = "Debug Window";

static VSWIN_DebugTextWindow *g_pDebugTextWindow = 0;
static HACCEL g_hDebugWindowAccelerators = 0;
static void *g_VSWIN_InvisibleMessageWindowVTable[1] = { 0 };
static void *g_VSWIN_DebugTextWindowVTable[1] = { 0 };

static int HostMessageBoxA(HWND hWnd, LPCSTR pszText, LPCSTR pszCaption, UINT uType) {
    (void)hWnd;
    (void)pszText;
    (void)pszCaption;
    (void)uType;
    return 1;
}

static ATOM HostRegisterClassA(const WNDCLASSA *pWndClass) {
    if (pWndClass == 0 || pWndClass->lpszClassName == 0) {
        return 0;
    }

    return 1;
}

static HWND HostCreateWindowExA(DWORD dwExStyle,
                                LPCSTR pszClassName,
                                LPCSTR pszWindowName,
                                DWORD dwStyle,
                                INT x,
                                INT y,
                                INT nWidth,
                                INT nHeight,
                                HWND hWndParent,
                                HMENU hMenu,
                                HINSTANCE hInstance,
                                void *pvParam) {
    (void)dwExStyle;
    (void)dwStyle;
    (void)x;
    (void)y;
    (void)nWidth;
    (void)nHeight;
    (void)hWndParent;
    (void)hMenu;
    (void)hInstance;
    (void)pvParam;

    if (pszClassName == 0 || pszWindowName == 0) {
        return 0;
    }

    return (HWND)pszWindowName;
}

static DWORD HostGetLastError(void) {
    return 0;
}

static int HostSetEvent(HANDLE hEvent) {
    return hEvent != 0;
}

static int HostGetMessageA(MSG *pMsg, HWND hWnd, UINT uMsgFilterMin, UINT uMsgFilterMax) {
    (void)hWnd;
    (void)uMsgFilterMin;
    (void)uMsgFilterMax;

    if (pMsg == 0) {
        return 0;
    }

    pMsg->hwnd = 0;
    pMsg->message = WM_QUIT;
    pMsg->wParam = 0;
    pMsg->lParam = 0;
    pMsg->time = 0;
    pMsg->pt.x = 0;
    pMsg->pt.y = 0;
    return 0;
}

static int HostTranslateAcceleratorA(HWND hWnd, HACCEL hAccel, MSG *pMsg) {
    (void)hWnd;
    (void)hAccel;
    (void)pMsg;
    return 0;
}

static int HostTranslateMessage(const MSG *pMsg) {
    (void)pMsg;
    return 1;
}

static LRESULT HostDispatchMessageA(const MSG *pMsg) {
    (void)pMsg;
    return 0;
}

int lemball_platform_show_error(const char *title, const char *message) {
    return HostMessageBoxA(0, message, title, MB_OK | MB_ICONERROR);
}

VSWIN_InvisibleMessageWindow::VSWIN_InvisibleMessageWindow(void) {
    m_pVTable = g_VSWIN_InvisibleMessageWindowVTable;
    m_hWnd = 0;
    m_dwReserved0 = 0;
    m_dwReserved1 = 0;
}

// FUNCTION: LEMBALL 0x0047C050
int VSWIN_InvisibleMessageWindow::Construct(LPCSTR pszClassName, int *pfClassRegistered) {
    WNDCLASSA WndClass;

    m_pVTable = g_VSWIN_InvisibleMessageWindowVTable;
    m_dwReserved0 = 0;
    m_dwReserved1 = 0;

    if (pfClassRegistered != 0 && *pfClassRegistered == 0) {
        WndClass.style = CS_DBLCLKS;
        WndClass.lpfnWndProc = 0;
        WndClass.cbClsExtra = 0;
        WndClass.cbWndExtra = 4;
        WndClass.hInstance = g_hApplicationInstance;
        WndClass.hIcon = 0;
        WndClass.hCursor = 0;
        WndClass.hbrBackground = 0;
        WndClass.lpszMenuName = 0;
        WndClass.lpszClassName = pszClassName;

        if (HostRegisterClassA(&WndClass) == 0) {
            lemball_platform_show_error(g_VSWIN_ErrorTitle, g_VSWIN_BaseWindowRegisterError);
            return 0;
        }

        *pfClassRegistered = 1;
    }

    m_hWnd = HostCreateWindowExA(0,
                                 pszClassName,
                                 g_VSWIN_InvisibleWindowTitle,
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
        AppendErrorCString(g_VSWIN_InvisibleWindowCreateError);
        AppendErrorUInt(HostGetLastError());
        AppendErrorCString(g_VSWIN_ErrorSuffix);
    }

    return m_hWnd != 0;
}

HWND VSWIN_InvisibleMessageWindow::GetHwnd(void) const {
    return m_hWnd;
}

VSWIN_DebugTextWindow::VSWIN_DebugTextWindow(void) {
    unsigned int i;

    m_pVTable = g_VSWIN_DebugTextWindowVTable;
    m_pszTitle = 0;
    m_cchBuffer = 0;
    for (i = 0; i < sizeof(m_achReserved); ++i) {
        m_achReserved[i] = 0;
    }
}

VSWIN_DebugTextWindow *VSWIN_DebugTextWindow::Construct(const char *pszTitle, unsigned int cchBuffer) {
    m_pVTable = g_VSWIN_DebugTextWindowVTable;
    m_pszTitle = pszTitle;
    m_cchBuffer = cchBuffer;
    return this;
}

const char *VSWIN_DebugTextWindow::GetTitle(void) const {
    return m_pszTitle;
}

unsigned int VSWIN_DebugTextWindow::GetBufferSize(void) const {
    return m_cchBuffer;
}

int ConstructInvisibleMessageWindow(VSWIN_InvisibleMessageWindow *pWindow,
                                    LPCSTR pszClassName,
                                    int *pfClassRegistered) {
    if (pWindow == 0) {
        return 0;
    }

    return pWindow->Construct(pszClassName, pfClassRegistered);
}

// FUNCTION: LEMBALL 0x00472B10
int DebugMessageThreadMain(void) {
    MSG Msg;
    int nMessageResult;
    void *pvTextWindow;

    pvTextWindow = AllocateVSMemBlock(0x58);
    if (pvTextWindow == 0) {
        g_pDebugTextWindow = 0;
    } else {
        g_pDebugTextWindow = (new (pvTextWindow) VSWIN_DebugTextWindow)->Construct(g_VSWIN_DebugWindowTitle, 0x2800);
    }

    HostSetEvent((HANDLE)1);
    nMessageResult = HostGetMessageA(&Msg, 0, 0, 0);
    while (nMessageResult != 0) {
        if (g_hDebugWindowAccelerators == 0 ||
            HostTranslateAcceleratorA(Msg.hwnd, g_hDebugWindowAccelerators, &Msg) == 0) {
            HostTranslateMessage(&Msg);
            HostDispatchMessageA(&Msg);
        }

        nMessageResult = HostGetMessageA(&Msg, 0, 0, 0);
    }

    HostSetEvent((HANDLE)1);
    return 1;
}

VSWIN_DebugTextWindow *GetDebugTextWindow(void) {
    return g_pDebugTextWindow;
}
