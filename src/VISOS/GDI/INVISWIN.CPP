#include "VISOS/VSINVIS.H"

#include "LEMBALL/SHELL/ENTRY.H"
#include "VISOS/VSINIT.H"
#include "VISOS/SAFEVT.H"

DECOMP_SIZE_ASSERT(VS_InvisibleWindow, 0x10);

static const char g_PLATFORM_ErrorTitle[] = "ERROR";
static const char g_PLATFORM_BaseWindowRegisterError[] = "Unable to register base window class";
static const char g_PLATFORM_InvisibleWindowTitle[] = "Invisible Window";
static const char g_PLATFORM_InvisibleWindowCreateError[] = "Failed to create invisible window - Error code: ";
static const char g_PLATFORM_ErrorSuffix[] = "\n";

static void *g_VS_InvisibleWindowVTable[1] = { (void *)NetworkSafeVtableNoop };
static int *g_pfWindowMessagePassthroughMode;

typedef LRESULT (*VS_WindowMessageHandler)(VS_InvisibleWindow *pWindow,
                                           UINT uMessage,
                                           WPARAM wParam,
                                           LPARAM lParam);

// FUNCTION: LEMBALL 0x0047C160
LRESULT CALLBACK InvisibleMessageWindowWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    VS_InvisibleWindow *pWindow;
    LRESULT lResult;
    CREATESTRUCTA *pCreateStruct;

    if (*g_pfWindowMessagePassthroughMode != 0) {
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);
    }

    pWindow = (VS_InvisibleWindow *)(unsigned long)GetWindowLongA(hWnd, GWL_USERDATA);
    if (uMessage == WM_CREATE) {
        pCreateStruct = (CREATESTRUCTA *)(unsigned long)lParam;
        SetWindowLongA(hWnd, GWL_USERDATA, (LONG)(unsigned long)pCreateStruct->lpCreateParams);
        return 0;
    }

    if (pWindow != 0) {
        if (pWindow->m_dwReserved1 == 0 || uMessage < pWindow->m_dwReserved0 || pWindow->m_dwReserved1 < uMessage) {
            lResult = ((VS_WindowMessageHandler)(*(void **)pWindow->m_pVTable))(pWindow, uMessage, wParam, lParam);
            if (lResult != -1) {
                return lResult;
            }
        }
    }

    return DefWindowProcA(hWnd, uMessage, wParam, lParam);
}

// FUNCTION: LEMBALL 0x0047C050
int VS_InvisibleWindow::Construct(LPCSTR pszClassName, int *pfClassRegistered) {
    WNDCLASSA WndClass;

    m_pVTable = g_VS_InvisibleWindowVTable;
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
void __fastcall DestroyInvisibleMessageWindow(VS_InvisibleWindow *pWindow) {
    *(void ***)pWindow = g_VS_InvisibleWindowVTable;
    DestroyWindow(pWindow->m_hWnd);
}
