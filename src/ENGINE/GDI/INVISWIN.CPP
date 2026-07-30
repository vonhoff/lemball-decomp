#include "ENGINE/CORE/SAFEVT.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/GDI/VSINVIS.H"
#include "ENGINE/GDI/VSWINDOW.H"
#include "SHELL/ENTRY.H"

DECOMP_SIZE_ASSERT(VS_InvisibleWindow, 0x10);

static const char g_PLATFORM_BaseWindowRegisterError[] = "Unable to register base window class";
static const char g_PLATFORM_InvisibleWindowTitle[] = "Invisible Window";
static const char g_PLATFORM_InvisibleWindowCreateError[] = "Failed to create invisible window - Error code: ";
static const char g_PLATFORM_ErrorSuffix[] = "\n";

// FUNCTION: LEMBALL 0x004714f0
LRESULT LEMBALL_FASTCALL DefaultInvisibleWindowMessageHandler(VS_InvisibleWindow* pWindow,
															  void* pUnused,
															  UINT uMessage,
															  WPARAM wParam,
															  LPARAM lParam)
{
	(void) pWindow;
	(void) pUnused;
	(void) uMessage;
	(void) wParam;
	(void) lParam;
	return -1;
}

// GLOBAL: LEMBALL 0x0049ac30
static void* g_VS_InvisibleWindowVTable[1] = {(void*) DefaultInvisibleWindowMessageHandler};
struct VS_InvisibleWindowMessageInterface {
	virtual LRESULT HandleWindowMessage(UINT uMessage, WPARAM wParam, LPARAM lParam) = 0; // vtable+0x0
};
LRESULT CALLBACK InvisibleMessageWindowWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
extern void __cdecl ShowFatalGetLastErrorMessageAndExit(const char* pszMessage);

// FUNCTION: LEMBALL 0x0047c050
VS_InvisibleWindow* VS_InvisibleWindow::Construct(LPCSTR pszClassName, int* pfClassRegistered)
{
	WNDCLASSA WndClass;

	m_pVTable = g_VS_InvisibleWindowVTable;
	m_dwReserved0 = 0;
	m_dwReserved1 = 0;

	if (*pfClassRegistered == 0) {
		WndClass.style = 0;
		WndClass.cbClsExtra = 0;
		WndClass.hInstance = g_hApplicationInstance;
		WndClass.hIcon = 0;
		WndClass.hCursor = 0;
		WndClass.hbrBackground = 0;
		WndClass.lpszMenuName = 0;
		WndClass.lpszClassName = pszClassName;
		WndClass.lpfnWndProc = InvisibleMessageWindowWndProc;
		WndClass.cbWndExtra = 4;

		if (RegisterClassA(&WndClass) == 0) {
			ShowFatalGetLastErrorMessageAndExit(g_PLATFORM_BaseWindowRegisterError);
		}

		*pfClassRegistered = 1;
	}

	m_hWnd = CreateWindowExA(0,
							 pszClassName,
							 g_PLATFORM_InvisibleWindowTitle,
							 0,
							 (INT) CW_USEDEFAULT,
							 (INT) CW_USEDEFAULT,
							 (INT) CW_USEDEFAULT,
							 (INT) CW_USEDEFAULT,
							 0,
							 0,
							 g_hApplicationInstance,
							 this);

	if (m_hWnd == 0) {
		unsigned int nError = GetLastError();
		g_pErrorOutputStream->AppendCStringToStream(g_PLATFORM_InvisibleWindowCreateError)
			->AppendUIntToStream(nError)
			->AppendCStringToStream(g_PLATFORM_ErrorSuffix);
	}

	return this;
}

// FUNCTION: LEMBALL 0x0047c140
void __fastcall DestroyInvisibleMessageWindow(VS_InvisibleWindow* pWindow)
{
	*(void***) pWindow = g_VS_InvisibleWindowVTable;
	DestroyWindow(pWindow->m_hWnd);
}
// FUNCTION: LEMBALL 0x0047c160
LRESULT CALLBACK InvisibleMessageWindowWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	VS_InvisibleWindow* pWindow;
	LRESULT lResult;
	CREATESTRUCTA* pCreateStruct;

	if (g_fWindowMessagePassthroughMode != 0) {
		return DefWindowProcA(hWnd, uMessage, wParam, lParam);
	}

	pWindow = (VS_InvisibleWindow*) (unsigned long) GetWindowLongA(hWnd, GWL_USERDATA);
	if (uMessage == WM_CREATE) {
		pCreateStruct = (CREATESTRUCTA*) (unsigned long) lParam;
		SetWindowLongA(hWnd, GWL_USERDATA, (LONG) (unsigned long) pCreateStruct->lpCreateParams);
		return 0;
	}

	if (pWindow != 0) {
		if (pWindow->m_dwReserved1 == 0 || uMessage < pWindow->m_dwReserved0 || pWindow->m_dwReserved1 < uMessage) {
			lResult = ((VS_InvisibleWindowMessageInterface*) pWindow)->HandleWindowMessage(uMessage, wParam, lParam);
			if (lResult != -1) {
				return lResult;
			}
		}
	}

	return DefWindowProcA(hWnd, uMessage, wParam, lParam);
}
