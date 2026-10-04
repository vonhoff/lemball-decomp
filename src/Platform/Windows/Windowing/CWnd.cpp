#include "CWnd.h"

#define WIN32_LEAN_AND_MEAN
#include "Engine/Startup/PreInit.h"
#include "Platform/Windows/Entry.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Streams/CVSOStream.h"
#include "Engine/Diagnostics/VsDebug.h"
#include "Engine/VsTime.h"
#include "Engine/Queues/PackParam.h"
#include "Platform/Windows/Graphics/CGraphicsDriver.h"
#include "Platform/Windows/Graphics/CGraphicsState.h"
#include "Platform/Windows/CPlatformServices.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Queues/Message.h"
#include "Engine/Windows/CPVWnd.h"
#include "MenuList.h"

#include <conio.h>
#include <string.h>
#include <windows.h>

#define DISPLAYDIB_DEACTIVATE_COMMAND 0x4000

#pragma intrinsic(_outpw)

extern unsigned int g_windowDispatchDisabled;
extern int(__stdcall* g_pDisplayDib)(void*, void*, unsigned short);

enum {
	WINDOW_STYLE_DISTINGUISH_DOUBLE_CLICK = 0x1000
};

enum eDisplayDibFlag {
	DISPLAYDIB_FLAG_NO_PALETTE = 0x0010,
	DISPLAYDIB_FLAG_DONT_LOCK_TASK = 0x0200,
	DISPLAYDIB_FLAG_BEGIN = 0x8000,
	DISPLAYDIB_VGA_RESUME_FLAGS = DISPLAYDIB_FLAG_BEGIN | DISPLAYDIB_FLAG_DONT_LOCK_TASK | DISPLAYDIB_FLAG_NO_PALETTE,
	DISPLAYDIB_MODE_INCREMENT = 1,
	DISPLAYDIB_MODE_320X200_8BIT_INITIAL = 0,
	DISPLAYDIB_MODE_320X240_8BIT_INITIAL = 4
};

// GLOBAL: LEMBALL 0x004a1f64
void* g_hFocusWindow = NULL;

// GLOBAL: LEMBALL 0x004a1f68
CWnd* g_pFocusWindow = NULL;

// GLOBAL: LEMBALL 0x004a1f6c
int g_nMouseCaptureCount = 0;

// GLOBAL: LEMBALL 0x004a1f70
int g_nDisplayDibActive = 0;

// GLOBAL: LEMBALL 0x004a1f74
int g_nLastCursorX = 0;

// GLOBAL: LEMBALL 0x004a1f78
int g_nLastCursorY = 0;

// GLOBAL: LEMBALL 0x004a1fec
int g_cursorState = 0;

// GLOBAL: LEMBALL 0x004a1fe8
int g_nNativeWindowCount = 0;

// GLOBAL: LEMBALL 0x004a1ff0
WindowOwnerList* g_pWindowOwnerList = NULL;

// GLOBAL: LEMBALL 0x004a8188
void* g_pApplicationInstance = NULL;

// GLOBAL: LEMBALL 0x004a1f7c
char g_szVsBaseWindowClass[24] = "VS_Base_Window_Class";

// GLOBAL: LEMBALL 0x004a1f60
char* g_pszVsBaseWindowClass = g_szVsBaseWindowClass;

// GLOBAL: LEMBALL 0x004a1fa8
char g_szUnableToRegisterBaseWindowClass[40] = "Unable to register base window class";

// GLOBAL: LEMBALL 0x004a1fd0
char g_szUnableToCreateWindow[24] = "Unable to create window";

// GLOBAL: LEMBALL 0x004a1f94
char g_szQuitting[12] = "Quitting\n";

// GLOBAL: LEMBALL 0x004a1fa0
char g_szFQuit[8] = "fQuit";

// GLOBAL: LEMBALL 0x004a9bd8
int g_nSavedScreenSaverActive = 0;

// GLOBAL: LEMBALL 0x004a9be0
int g_savedMouseParameters[3] = {0, 0, 0};

static bool RegisterBaseWindowClass();
unsigned int ConvertWindowStyleFlags(unsigned int p_style);

// FUNCTION: LEMBALL 0x004324a0
void CWnd::SetFocusWindow()
{
	g_hFocusWindow = m_nativeWindow;
	g_pFocusWindow = this;
}

// FUNCTION: LEMBALL 0x004324c0
void CWnd::Dummy94()
{
}

// FUNCTION: LEMBALL 0x004324d0
bool CWnd::IsFocusWindow()
{
	return m_nativeWindow == g_hFocusWindow;
}

// FUNCTION: LEMBALL 0x004324f0
void CWnd::OnMinimise()
{
}

// FUNCTION: LEMBALL 0x00432500
void CWnd::OnMaximise()
{
}

// FUNCTION: LEMBALL 0x00432510
void CWnd::OnFocusGained()
{
}

// FUNCTION: LEMBALL 0x00432520
void CWnd::OnFocusLost()
{
}

// FUNCTION: LEMBALL 0x0043a4d0 FOLDED
void CWnd::OnMove()
{
}

// FUNCTION: LEMBALL 0x0043a4e0
void CWnd::OnRestore()
{
}

// FUNCTION: LEMBALL 0x0043a4f0
void CWnd::OnZoom(int p_oldZoom)
{
}

// FUNCTION: LEMBALL 0x0043a500
void CWnd::OnDriverChange()
{
}

// FUNCTION: LEMBALL 0x004644f0 FOLDED
void CWnd::OnCreate()
{
}

// FUNCTION: LEMBALL 0x00464500 FOLDED
void CWnd::OnDestroy()
{
}

// FUNCTION: LEMBALL 0x00464510 FOLDED
void CWnd::OnSize()
{
}

// FUNCTION: LEMBALL 0x00464520
long __stdcall CWnd::ProcessMessage(void* p_hwnd, unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
{
	CWnd* window;
	CREATESTRUCTA* create;
	Message posted;
	short mouseX;
	short mouseY;
	unsigned int style;
	int menuAction;

	if (g_windowDispatchDisabled != 0 && (p_message != WM_ACTIVATEAPP || p_wParam != 0)) {
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}

	posted.m_time = GetMessageTime();
	window = (CWnd*) GetWindowLongA((HWND) p_hwnd, GWL_USERDATA);
	if (g_pTargetGraphicsDriver == NULL) {
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}
	if (g_pTargetGraphicsDriver->m_window != p_hwnd) {
		if (window == NULL && p_message != WM_CREATE) {
			return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
		}
	}
	else {
		window = (CWnd*) g_pTargetGraphicsSystem->m_targetWindow;
		if (window == NULL) {
			return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
		}
	}

	switch (p_message) {
	case WM_CREATE: {
		POINT position;
		create = (CREATESTRUCTA*) p_lParam;
		window = (CWnd*) create->lpCreateParams;
		SetWindowLongA((HWND) p_hwnd, GWL_USERDATA, (LONG) window);
		window->m_nativeWindow = p_hwnd;
		position.x = 0;
		position.y = 0;
		ClientToScreen((HWND) p_hwnd, &position);
		mouseX = (short) position.x;
		mouseY = (short) position.y;
		if ((window->GetStyle() & WS_CHILD) != 0 && g_pTargetGraphicsSystem->IsFullscreenDriver()) {
			mouseX += window->m_createRect->m_relativeTopLeft.m_x;
			mouseY += window->m_createRect->m_relativeTopLeft.m_y;
		}
		window->m_rect.m_x = mouseX;
		window->m_rect.m_y = mouseY;
		CVSPoint* topLeft = &window->m_rect;
		window->m_relativeTopLeft.m_x = topLeft->m_x;
		window->m_relativeTopLeft.m_y = topLeft->m_y;
		window->_OnCreate();
		window->OnCreate();
		return 0;
	}
	case WM_DESTROY: {
		if (p_hwnd == g_hFocusWindow) {
			g_hFocusWindow = NULL;
			g_pFocusWindow = NULL;
		}
		window->m_nativeWindow = NULL;
		window->Destroy();
		if (g_nNativeWindowCount == 0) {
			g_dwWindowQuitRequested = 1;
			*g_pSysOutput << g_szFQuit;
		}
		return 0;
	}
	case WM_SETCURSOR: {
		if ((unsigned short) p_lParam == HTCLIENT) {
			SetCursor(NULL);
			return 1;
		}
	defaultWindowMessage:
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}
	case WM_QUIT: {
		*g_pDebugOutput << g_szQuitting;
		ReleaseCapture();
		return 0;
	}
	case WM_MOVE: {
		if (!g_pTargetGraphicsSystem->IsFullscreenDriver()) {
			POINT position;
			position.x = 0;
			position.y = 0;
			ClientToScreen((HWND) p_hwnd, &position);
			CVSPoint point((short) position.x, (short) position.y);
			window->MoveAbsolute(point);
		}
		return 0;
	}
	case WM_SIZE: {
		window->m_rect.m_width = (short) p_lParam;
		p_lParam >>= 16;
		window->m_rect.m_height = (short) p_lParam;
		switch (p_wParam) {
		case SIZE_RESTORED:
			if (window->GetSizeStatus() != 2) {
				window->SetSizeStatus(2);
				window->OnRestore();
			}
			break;
		case SIZE_MINIMIZED:
			if (window->GetSizeStatus() != 0) {
				window->SetSizeStatus(0);
				window->OnMinimise();
			}
			break;
		case SIZE_MAXIMIZED:
			if (window->GetSizeStatus() != 1) {
				window->SetSizeStatus(1);
				window->OnMaximise();
			}
			break;
		}
		window->_OnSize();
		window->OnSize();
		return 0;
	}
	case WM_SETFOCUS: {
		if (g_hFocusWindow != NULL) {
			g_pFocusWindow->Dummy94();
			g_pFocusWindow->OnFocusLost();
		}
		window->SetFocusWindow();
		window->OnFocusGained();
		goto defaultWindowMessage;
	}
	case WM_KILLFOCUS: {
		if (g_pTargetGraphicsSystem->m_driverMode < GFX_MODE_DD_FS_640X480 ||
			g_pTargetGraphicsSystem->m_driverMode > GFX_MODE_DD_FS_320X200) {
			if (g_hFocusWindow != NULL) {
				g_pFocusWindow->Dummy94();
				g_pFocusWindow->OnFocusLost();
			}
			g_hFocusWindow = NULL;
		}
		goto defaultWindowMessage;
	}
	case WM_ACTIVATEAPP: {
		int wasDisplayDibActive = g_nDisplayDibActive;
		int mode = g_pTargetGraphicsSystem->m_driverMode;
		switch (mode) {
		case GFX_MODE_GDI:
			g_dwFullScreenGdi = 1;
			g_nDisplayDibActive = 0;
			InvalidateRect((HWND) p_hwnd, NULL, 0);
			break;
		case GFX_MODE_VGA_320X200:
		case GFX_MODE_VGA_320X240:
			if (p_wParam != 0) {
				if (window->GetSizeStatus() == 0) {
					SendMessageA((HWND) p_hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				}
				unsigned short flags = g_pTargetGraphicsSystem->m_driverMode == GFX_MODE_VGA_320X240
										   ? DISPLAYDIB_MODE_320X240_8BIT_INITIAL
										   : DISPLAYDIB_MODE_320X200_8BIT_INITIAL;
				flags++;
				flags |= DISPLAYDIB_VGA_RESUME_FLAGS;
				g_pDisplayDib(NULL, NULL, flags);
				g_dwFullScreenGdi = 0;
				g_nDisplayDibActive = 1;
				_outpw(0x3d4, 0xc);
				_outpw(0x3d4, 0xd);
				InvalidateRect((HWND) p_hwnd, NULL, 0);
			}
			else {
				g_pDisplayDib(NULL, NULL, DISPLAYDIB_DEACTIVATE_COMMAND);
				g_nDisplayDibActive = 0;
				g_dwFullScreenGdi = 1;
				SendMessageA((HWND) p_hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}
			break;
		}
		if (wasDisplayDibActive != g_nDisplayDibActive) {
			if (g_nDisplayDibActive != 0) {
				int mouseParameters[3];
				RECT clipRect;
				SystemParametersInfoA(SPI_GETMOUSE, 0, mouseParameters, 0);
				memcpy(g_savedMouseParameters, mouseParameters, sizeof(mouseParameters));
				GetSystemMetrics(SM_CYSCREEN);
				short screenWidth = (short) GetSystemMetrics(SM_CXSCREEN);
				CVSSize* screenSize = &g_pTargetGraphicsDriver->m_screenSize;
				mouseParameters[2] = screenSize->m_width * mouseParameters[2] / screenWidth;
				SystemParametersInfoA(SPI_SETMOUSE, 0, mouseParameters, 0);
				SystemParametersInfoA(SPI_GETSCREENSAVEACTIVE, 0, &g_nSavedScreenSaverActive, 0);
				if (g_nSavedScreenSaverActive != 0) {
					SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, 0, NULL, 0);
				}
				clipRect.top = 0;
				clipRect.left = 0;
				clipRect.right = screenSize->m_width;
				clipRect.bottom = screenSize->m_height;
				ClipCursor(&clipRect);
			}
			else {
				ClipCursor(NULL);
				SystemParametersInfoA(SPI_SETMOUSE, 0, g_savedMouseParameters, 0);
				if (g_nSavedScreenSaverActive != 0) {
					SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, 0, (void*) 1, 0);
				}
			}
		}
		return window->ProcessOtherMessages(p_message, p_wParam, p_lParam);
	}
	case WM_DISPLAYCHANGE: {
		if (g_pTargetGraphicsSystem != NULL) {
			CVSSize size;
			size.m_width = (short) p_lParam;
			p_lParam >>= 16;
			size.m_height = (short) p_lParam;
			g_pTargetGraphicsSystem->UpdateDriverSize(size);
		}
		return 0;
	}
	case WM_KEYDOWN:
	case WM_KEYUP: {
		posted.m_type = (p_message == WM_KEYDOWN) ? MESSAGE_RAW_KEY_DOWN : MESSAGE_RAW_KEY_UP;
		posted.m_code = (int) p_wParam;
		g_pMasterInputQueue->Post(posted);
		return 0;
	}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK: {
		posted.m_type = MESSAGE_MOUSE_BUTTON_DOWN;
		style = window->GetStyle();
		if ((style & WINDOW_STYLE_DISTINGUISH_DOUBLE_CLICK) != 0) {
			switch (p_message) {
			case WM_LBUTTONDOWN:
				posted.m_payload = (void*) INPUT_MOUSE_LEFT;
				break;
			case WM_LBUTTONDBLCLK:
				posted.m_payload = (void*) INPUT_MOUSE_LEFT_DOUBLE_CLICK;
				break;
			case WM_RBUTTONDOWN:
				posted.m_payload = (void*) INPUT_MOUSE_RIGHT;
				break;
			case WM_RBUTTONDBLCLK:
				posted.m_payload = (void*) INPUT_MOUSE_RIGHT_DOUBLE_CLICK;
				break;
			case WM_MBUTTONDOWN:
				posted.m_payload = (void*) INPUT_MOUSE_MIDDLE;
				break;
			case WM_MBUTTONDBLCLK:
				posted.m_payload = (void*) INPUT_MOUSE_MIDDLE_DOUBLE_CLICK;
				break;
			}
		}
		else {
			switch (p_message) {
			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
				posted.m_payload = (void*) INPUT_MOUSE_LEFT;
				break;
			case WM_RBUTTONDOWN:
			case WM_RBUTTONDBLCLK:
				posted.m_payload = (void*) INPUT_MOUSE_RIGHT;
				break;
			case WM_MBUTTONDOWN:
			case WM_MBUTTONDBLCLK:
				posted.m_payload = (void*) INPUT_MOUSE_MIDDLE;
				break;
			}
		}
		mouseX = window->m_rect.m_x + (short) p_lParam;
		p_lParam >>= 16;
		mouseY = window->m_rect.m_y + (short) p_lParam;
		posted.m_code = PackParam(mouseX, mouseY);
		posted.m_source = NULL;
		g_pMasterInputQueue->Post(posted);
		if (g_nMouseCaptureCount++ == 0) {
			SetCapture((HWND) p_hwnd);
		}
		return 0;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP: {
		posted.m_type = MESSAGE_MOUSE_BUTTON_UP;
		switch (p_message) {
		case WM_LBUTTONUP:
			posted.m_payload = (void*) INPUT_MOUSE_LEFT;
			break;
		case WM_RBUTTONUP:
			posted.m_payload = (void*) INPUT_MOUSE_RIGHT;
			break;
		case WM_MBUTTONUP:
			posted.m_payload = (void*) INPUT_MOUSE_MIDDLE;
			break;
		}
		mouseX = window->m_rect.m_x + (short) p_lParam;
		p_lParam >>= 16;
		mouseY = window->m_rect.m_y + (short) p_lParam;
		posted.m_code = PackParam(mouseX, mouseY);
		posted.m_source = NULL;
		g_pMasterInputQueue->Post(posted);
		g_nMouseCaptureCount = g_nMouseCaptureCount - 1;
		if (g_nMouseCaptureCount == 0) {
			ReleaseCapture();
		}
		return 0;
	}
	case WM_COMMAND: {
		menuAction = window->SelectMenu(p_message, p_wParam, p_lParam);
		if (menuAction != 0) {
			Message command;
			command.m_type = MESSAGE_WINDOW_COMMAND;
			command.m_time = CurrentQueueTimer();
			command.m_code = menuAction;
			g_pMasterInputQueue->Post(command);
			return 0;
		}
		return window->ProcessOtherMessages(p_message, p_wParam, p_lParam);
	}

	default:
		return window->ProcessOtherMessages(p_message, p_wParam, p_lParam);
	}
}

// FUNCTION: LEMBALL 0x00464f10
void CWnd::MoveAbsolute(const CVSPoint& p_point)
{
	void** node = (void**) m_childList;
	CVSPoint* position;
	if (this != (CWnd*) -8) {
		position = (CVSPoint*) &m_rect.m_x;
	}
	else {
		position = NULL;
	}
	CVSPoint delta((short) (p_point.m_x - position->m_x), (short) (p_point.m_y - position->m_y));
	for (;;) {
		if (node == NULL) {
			break;
		}
		((CPVWnd*) node[0])->_OnMove(delta);
		node = (void**) node[1];
	}
	m_rect.m_x = p_point.m_x;
	m_rect.m_y = p_point.m_y;
	m_relativeTopLeft.m_x = p_point.m_x;
	m_relativeTopLeft.m_y = p_point.m_y;
	_OnMove();
}

// FUNCTION: LEMBALL 0x00464fa0
void CWnd::Move(const CVSPoint& p_point)
{
	void** node = (void**) m_childList;
	CVSPoint delta((short) (p_point.m_x - m_relativeTopLeft.m_x), (short) (p_point.m_y - m_relativeTopLeft.m_y));
	for (;;) {
		if (node == NULL) {
			break;
		}
		CPVWnd* child = (CPVWnd*) node[0];
		child->_OnMove(delta);
		child->OnMove();
		node = (void**) node[1];
	}
	m_rect.m_x = (short) (m_rect.m_x + delta.m_x);
	m_rect.m_y = (short) (m_rect.m_y + delta.m_y);
	m_relativeTopLeft.m_x = p_point.m_x;
	m_relativeTopLeft.m_y = p_point.m_y;
	_OnMove();
	OnMove();
	if (m_nativeWindow != NULL) {
		SetWindowPos((HWND) m_nativeWindow, NULL, p_point.m_x, p_point.m_y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}
}

// FUNCTION: LEMBALL 0x00465050
void CWnd::ProcessMouseMoves()
{
	POINT position;
	Message posted;

	GetCursorPos(&position);
	if (position.x != g_nLastCursorX || position.y != g_nLastCursorY) {
		g_nLastCursorX = position.x;
		g_nLastCursorY = position.y;
		if (g_pTargetGraphicsSystem->IsFullscreenDriver() != 0 && g_pFocusWindow != NULL) {
			g_nLastCursorX += g_pFocusWindow->m_rect.m_x;
			g_nLastCursorY += g_pFocusWindow->m_rect.m_y;
		}
		posted.m_type = MESSAGE_MOUSE_MOVED;
		posted.m_time = CurrentQueueTimer();
		posted.m_code = PackParam((short) g_nLastCursorX, (short) g_nLastCursorY);
		posted.m_payload = NULL;
		posted.m_source = NULL;
		g_pMasterInputQueue->Post(posted);
	}
}

// FUNCTION: LEMBALL 0x00465110
static bool RegisterBaseWindowClass()
{
	WNDCLASSA windowClass;
	HCURSOR cursor;
	ATOM atom;

	windowClass.style = 0xb;
	windowClass.lpfnWndProc = (WNDPROC) CWnd::ProcessMessage;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 4;
	windowClass.hInstance = (HINSTANCE) g_pApplicationInstance;
	if (g_preInitActive.m_icon == NULL) {
		windowClass.hIcon = LoadIconA(NULL, (LPCSTR) 0x7f00);
	}
	else {
		windowClass.hIcon = (HICON) g_preInitActive.m_icon;
	}
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = (HBRUSH) GetStockObject(4);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = g_pszVsBaseWindowClass;
	cursor = LoadCursorA(NULL, (LPCSTR) 0x7f00);
	atom = RegisterClassA(&windowClass);
	SetCursor(cursor);
	ShowCursor(1);
	if (atom == 0) {
		FatalWin32Error(g_szUnableToRegisterBaseWindowClass);
	}
	return true;
}

// FUNCTION: LEMBALL 0x004651d0
CWnd::CWnd()
{
	if (g_cursorState == 1) {
		RegisterBaseWindowClass();
	}
	m_nativeWindow = NULL;
}

// FUNCTION: LEMBALL 0x00465200
void CWnd::Create(const CVSRect& p_rect, CPVWnd* p_parent, char* p_title)
{
	unsigned int styleFlags;

	m_rect.m_x = 0;
	m_rect.m_y = 0;
	short height = p_rect.m_height;
	m_rect.m_width = p_rect.m_width;
	m_rect.m_height = height;
	SetSizeStatus(2);
	InitHotAreaList();
	m_parent = p_parent;
	m_createRect = p_parent;

	if (p_parent != NULL) {
		styleFlags = GetStyle();
		if ((styleFlags & WS_CHILD) == 0) {
			POINT screenPoint;
			m_parent->AddChild(this);
			const CVSRect& parentRect = m_parent->m_rect;
			screenPoint.x = (LONG) ((int) parentRect.m_x + (int) p_rect.m_x);
			screenPoint.y = (LONG) ((int) parentRect.m_y + (int) p_rect.m_y);
			ClientToScreen((HWND) ((CWnd*) m_parent)->m_nativeWindow, &screenPoint);
			m_rect.m_x = (short) screenPoint.x;
			m_rect.m_y = (short) screenPoint.y;
			const CVSPoint* relativeOrigin = &p_rect;
			short relativeY = relativeOrigin->m_y;
			m_relativeTopLeft.m_x = relativeOrigin->m_x;
			m_relativeTopLeft.m_y = relativeY;
			m_zoom = m_parent->m_zoom;
			_OnCreate();
			OnCreate();
			_OnSize();
			OnSize();
			return;
		}
	}

	switch (g_pTargetGraphicsSystem->m_driverMode) {
	default: {
		unsigned int style = ConvertWindowStyleFlags(GetStyle());
		RECT windowRect;
		HMENU menu;
		HWND parentWindow;
		int hasMenu;
		int menuResourceId;
		MenuList** menuLists;
		styleFlags = GetStyle();
		if ((styleFlags & WS_CHILD) != 0 && p_parent != NULL) {
			m_parent = NULL;
			style &= ~WS_POPUP;
			style |= WS_CHILD;
		}

		windowRect.top = p_rect.m_y;
		windowRect.left = p_rect.m_x;
		windowRect.bottom = p_rect.m_height + windowRect.top;
		windowRect.right = p_rect.m_width + windowRect.left;

		m_menuLists = NULL;
		m_menuResourceId = 0;
		menu = NULL;
		hasMenu = 0;
		if (GetMenu(menuResourceId, &menuLists) != 0) {
			m_menuResourceId = (unsigned int) menuResourceId;
			m_menuLists = menuLists;
			menu = LoadMenuA((HINSTANCE) g_pApplicationInstance, (LPCSTR) (unsigned short) m_menuResourceId);
			hasMenu = 1;
		}

		AdjustWindowRect(&windowRect, style, hasMenu);
		windowRect.bottom -= windowRect.top;
		windowRect.right -= windowRect.left;
		if ((style & WS_CHILD) == 0 || p_parent == NULL) {
			parentWindow = NULL;
		}
		else {
			parentWindow = (HWND) ((CWnd*) p_parent)->m_nativeWindow;
		}
		HWND hwnd = CreateWindowExA(0,
									g_pszVsBaseWindowClass,
									p_title,
									style,
									windowRect.left,
									windowRect.top,
									windowRect.right,
									windowRect.bottom,
									parentWindow,
									menu,
									(HINSTANCE) g_pApplicationInstance,
									this);
		m_nativeWindow = hwnd;
		if (menu != NULL) {
			ReSetMenu();
		}
		if (m_nativeWindow == NULL) {
			FatalWin32Error(g_szUnableToCreateWindow);
		}
		styleFlags = GetStyle();
		if ((styleFlags & WINDOW_STYLE_SHOW_ON_CREATE) != 0) {
			UpdateWindow((HWND) m_nativeWindow);
			ShowWindow((HWND) m_nativeWindow, SW_SHOW);
			SetForegroundWindow((HWND) m_nativeWindow);
			return;
		}
		break;
	}
	case GFX_MODE_DD_FS_640X480:
	case GFX_MODE_DD_FS_320X200: {
		m_parent = NULL;
		m_rect.m_width = p_rect.m_width;
		m_rect.m_height = p_rect.m_height;
		const CVSPoint* rectOrigin = &p_rect;
		m_rect.m_x = rectOrigin->m_x;
		m_rect.m_y = rectOrigin->m_y;
		const CVSPoint* relativeOrigin = &p_rect;
		short relativeY = relativeOrigin->m_y;
		m_relativeTopLeft.m_x = relativeOrigin->m_x;
		m_relativeTopLeft.m_y = relativeY;
		g_pTargetGraphicsSystem->m_targetWindow = (unsigned int) this;
		m_nativeWindow = g_pTargetGraphicsDriver->m_window;
		SetFocusWindow();
		OnFocusGained();
		_OnCreate();
		OnCreate();
		_OnSize();
		OnSize();
	}
	}
}

#pragma warning(disable : 4146)
#define WINDOW_STYLE_INPUT_OVERLAPPED_FRAME 8
#define WINDOW_STYLE_INPUT_CAPTION 2
#define WINDOW_STYLE_INPUT_THICK_FRAME 0x400
#define WINDOW_STYLE_INPUT_TAB_STOP 0x40
#define WINDOW_STYLE_INPUT_HORIZONTAL_SCROLL 0x20
#define WINDOW_STYLE_INPUT_VERTICAL_SCROLL 0x10
#define WINDOW_STYLE_INPUT_GROUP 0x80
#define WINDOW_STYLE_INPUT_SYSTEM_MENU 0x100
#define WINDOW_STYLE_OVERLAPPED_FRAME (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME)

enum {
	WINDOW_STYLE_THICK_FRAME_SHIFT = 8,
	WINDOW_STYLE_TAB_STOP_SHIFT = 10,
	WINDOW_STYLE_HORIZONTAL_SCROLL_SHIFT = 15,
	WINDOW_STYLE_VERTICAL_SCROLL_SHIFT = 17,
	WINDOW_STYLE_GROUP_SHIFT = 10,
	WINDOW_STYLE_SYSTEM_MENU_SHIFT = 11
};

// FUNCTION: LEMBALL 0x004654f0
unsigned int ConvertWindowStyleFlags(unsigned int p_style)
{
	unsigned int style = 0;
	if (p_style & WINDOW_STYLE_INPUT_OVERLAPPED_FRAME) {
		style = WINDOW_STYLE_OVERLAPPED_FRAME;
	}
	if (p_style & WINDOW_STYLE_INPUT_CAPTION) {
		style |= WS_CAPTION;
	}
	else {
		style |= WS_POPUP;
	}
	style |= (p_style & WINDOW_STYLE_INPUT_THICK_FRAME) << WINDOW_STYLE_THICK_FRAME_SHIFT;
	style |= (p_style & WINDOW_STYLE_INPUT_TAB_STOP) << WINDOW_STYLE_TAB_STOP_SHIFT;
	style |= (p_style & WINDOW_STYLE_INPUT_HORIZONTAL_SCROLL) << WINDOW_STYLE_HORIZONTAL_SCROLL_SHIFT;
	style |= (p_style & WINDOW_STYLE_INPUT_VERTICAL_SCROLL) << WINDOW_STYLE_VERTICAL_SCROLL_SHIFT;
	style |= (p_style & WINDOW_STYLE_INPUT_GROUP) << WINDOW_STYLE_GROUP_SHIFT;
	style |= (p_style & WINDOW_STYLE_INPUT_SYSTEM_MENU) << WINDOW_STYLE_SYSTEM_MENU_SHIFT;
	return style;
}
#pragma warning(default : 4146)

// FUNCTION: LEMBALL 0x00465570
CWnd::~CWnd()
{
	if (g_cursorState == 1) {
		ShowCursor(0);
	}
}

// FUNCTION: LEMBALL 0x004655a0
void CWnd::Destroy()
{
	CPVWnd* child;
	void** childNode;

	if (m_lifecycleRefs != 0) {
		childNode = (void**) m_childList;
		for (;;) {
			if (childNode == NULL) {
				break;
			}
			child = (CPVWnd*) childNode[0];
			childNode = (void**) childNode[1];
			child->Destroy();
		}
		OnDestroy();
		_OnDestroy();
		if ((g_pTargetGraphicsSystem->m_driverMode < GFX_MODE_DD_FS_640X480 ||
			 g_pTargetGraphicsSystem->m_driverMode > GFX_MODE_DD_FS_320X200) &&
			m_nativeWindow != NULL) {
			DestroyWindow((HWND) m_nativeWindow);
		}
	}
}

// FUNCTION: LEMBALL 0x004655f0
void CWnd::Refresh(CVSRect* p_rect)
{
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	if (p_rect == NULL) {
		rect.right = 1;
		rect.bottom = 1;
	}
	else {
		rect.right = (LONG) p_rect->m_width;
		rect.bottom = (LONG) p_rect->m_height;
	}
	InvalidateRect((HWND) m_nativeWindow, &rect, 0);
}

// FUNCTION: LEMBALL 0x00465640
int CWnd::ProcessOtherMessages(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
{
	return DefWindowProcA((HWND) m_nativeWindow, p_message, p_wParam, p_lParam);
}

// FUNCTION: LEMBALL 0x00465660
void CWnd::ReSetMenu()
{
	HMENU menu = ::GetMenu((HWND) m_nativeWindow);
	MenuList** lists = (MenuList**) m_menuLists;
	if (*lists != NULL) {
		BOOL(WINAPI * enableMenuItem)(HMENU, UINT, UINT) = EnableMenuItem;
		DWORD(WINAPI * checkMenuItem)(HMENU, UINT, UINT) = CheckMenuItem;
		do {
			MenuList* item = *lists;
			while (item->m_name != NULL) {
				if (item->m_enabled != 0) {
					enableMenuItem(menu, item->m_commandId, MF_ENABLED);
				}
				else {
					enableMenuItem(menu, item->m_commandId, MF_GRAYED);
				}
				if (item->m_checked != 0) {
					checkMenuItem(menu, item->m_commandId, MF_CHECKED);
				}
				else {
					checkMenuItem(menu, item->m_commandId, MF_UNCHECKED);
				}
				item++;
			}
			lists++;
		} while (*lists != NULL);
	}
}

// FUNCTION: LEMBALL 0x004656f0
void CWnd::SetMenu(int& p_menuResourceId, MenuList** p_menuLists)
{
	HMENU currentMenu;
	HMENU newMenu;
	unsigned int resourceId;

	currentMenu = ::GetMenu((HWND) m_nativeWindow);
	resourceId = p_menuResourceId;
	m_menuResourceId = resourceId;
	m_menuLists = p_menuLists;
	if (p_menuLists != NULL) {
		newMenu = LoadMenuA((HINSTANCE) g_pApplicationInstance, (LPCSTR) (unsigned short) resourceId);
		::SetMenu((HWND) m_nativeWindow, newMenu);
		ReSetMenu();
	}
	if (currentMenu != NULL) {
		DestroyMenu(currentMenu);
	}
}

// FUNCTION: LEMBALL 0x00465750
int CWnd::SelectMenu(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
{
	int* menuList;
	int* item;

	if (m_menuLists == NULL) {
		return 0;
	}
	menuList = (int*) m_menuLists;
	while (*menuList != 0) {
		item = (int*) *menuList;
		while (*item != 0) {
			if (item[1] == (int) p_wParam) {
				return item[2];
			}
			item += 6;
		}
		menuList++;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00465790
void CWnd::_OnZoom(int p_oldZoom)
{
	RECT windowRect;
	RECT clientRect;

	CPVWnd::_OnZoom(p_oldZoom);
	if (m_nativeWindow != NULL && g_pTargetGraphicsDriver->m_window != m_nativeWindow) {
		GetWindowRect((HWND) m_nativeWindow, &windowRect);
		GetClientRect((HWND) m_nativeWindow, &clientRect);
		windowRect.right -= windowRect.left;
		windowRect.bottom -= windowRect.top;
		windowRect.right -= clientRect.right;
		windowRect.bottom -= clientRect.bottom;
		windowRect.right += clientRect.right;
		windowRect.bottom += clientRect.bottom;
		SetWindowPos((HWND) m_nativeWindow, NULL, 0, 0, windowRect.right, windowRect.bottom, SWP_NOMOVE | SWP_NOZORDER);
	}
}

// FUNCTION: LEMBALL 0x00465820
void CWnd::_SetRect(const CVSRect& p_rect)
{
	CVSRect rect(p_rect);
	CVSPoint* origin = &rect;
	const CVSSize* size = &rect;
	RECT adjusted;
	RECT window;
	POINT client;
	const CVSPoint* position;
	if (m_nativeWindow != NULL && g_pTargetGraphicsDriver->m_window != m_nativeWindow) {
		if (g_pTargetGraphicsSystem->IsFullscreenDriver() != 0) {
			origin->m_x = 0;
			origin->m_y = 0;
		}
		adjusted.left = origin->m_x;
		adjusted.top = origin->m_y;
		adjusted.right = (short) (origin->m_x + size->m_width);
		adjusted.bottom = (short) (size->m_height + origin->m_y);
		GetWindowRect((HWND) m_nativeWindow, &window);
		client.x = 0;
		client.y = 0;
		ClientToScreen((HWND) m_nativeWindow, &client);
		window.left += origin->m_x - client.x;
		window.top += origin->m_y - client.y;
		unsigned int style = ConvertWindowStyleFlags(GetStyle());
		AdjustWindowRect(&adjusted, style, m_menuLists != NULL);
		adjusted.right -= adjusted.left;
		adjusted.bottom -= adjusted.top;
		SetWindowPos((HWND) m_nativeWindow,
					 NULL,
					 window.left,
					 window.top,
					 adjusted.right,
					 adjusted.bottom,
					 SWP_NOZORDER);
		return;
	}
	const CVSRect* parentRect = &m_parent->m_rect;
	position = parentRect;
	int top = (short) (position->m_y + origin->m_y);
	window.left = (short) (position->m_x + origin->m_x);
	window.top = top;
	ClientToScreen((HWND) ((CWnd*) m_parent)->m_nativeWindow, (POINT*) &window);
	short& windowX = m_rect.m_x;
	windowX = (short) window.left;
	m_rect.m_y = (short) window.top;
	m_rect.m_width = size->m_width;
	m_rect.m_height = size->m_height;
	m_relativeTopLeft.m_x = origin->m_x;
	m_relativeTopLeft.m_y = origin->m_y;
	_OnMove();
	OnMove();
	_OnSize();
	OnSize();
}

// FUNCTION: LEMBALL 0x00465a00
void CWnd::_SetRelTL(const CVSPoint& p_point)
{
	CVSRect rect(p_point.m_x, p_point.m_y, m_rect.m_width, m_rect.m_height);
	_SetRect(rect);
}

// FUNCTION: LEMBALL 0x00465a90
unsigned int CWnd::GetStyle()
{
	return 0;
}
