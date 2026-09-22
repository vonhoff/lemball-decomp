#include "CWnd.h"

#define WIN32_LEAN_AND_MEAN
#include "../../Control/Support/PreInit.h"
#include "../../Platform/Windows/Entry.h"
#include "../Foundation/CBaseQueue.h"
#include "../Foundation/CVsOStream.h"
#include "../Foundation/VsDebug.h"
#include "../Foundation/VsTime.h"
#include "../Messaging/PackParam.h"
#include "../Target/Graphics/CGraphicsDriver.h"
#include "../Target/Graphics/CGraphicsState.h"
#include "../Target/System/CPlatformServices.h"
#include "CCursor.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CPvWnd.h"
#include "Visos/Graphics/MenuList.h"

#include <conio.h>
#include <string.h>
#include <windows.h>

#pragma intrinsic(_outpw)

extern unsigned int g_windowDispatchDisabled;
extern int(__stdcall* g_pDisplayDib)(void*, void*, unsigned int);

// GLOBAL: LEMBALL 0x004a1f64
void* g_hFocusWindow = 0;

// GLOBAL: LEMBALL 0x004a1f68
CWnd* g_pFocusWindow = 0;

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
WindowOwnerList* g_pWindowOwnerList = 0;

// GLOBAL: LEMBALL 0x004a8188
void* g_pApplicationInstance = 0;

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

	posted.time = GetMessageTime();
	window = (CWnd*) GetWindowLongA((HWND) p_hwnd, GWL_USERDATA);
	if (g_pTargetGraphicsDriver == 0) {
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}
	if (g_pTargetGraphicsDriver->m_window != p_hwnd) {
		if (window == 0 && p_message != WM_CREATE) {
			return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
		}
	}
	else {
		window = (CWnd*) g_pTargetGraphicsSystem->m_reserved04;
		if (window == 0) {
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
		CVsPoint* topLeft = &window->m_rect;
		window->m_relativeTopLeft.m_x = topLeft->m_x;
		window->m_relativeTopLeft.m_y = topLeft->m_y;
		window->InternalOnCreate();
		window->OnCreate();
		return 0;
	}
	case WM_DESTROY: {
		if (p_hwnd == g_hFocusWindow) {
			g_hFocusWindow = 0;
			g_pFocusWindow = 0;
		}
		window->m_nativeWindow = 0;
		window->Destroy();
		if (g_nNativeWindowCount == 0) {
			g_dwWindowQuitRequested = 1;
			*g_pSysOutput << g_szFQuit;
		}
		return 0;
	}
	case WM_SETCURSOR: {
		if ((unsigned short) p_lParam == HTCLIENT) {
			SetCursor(0);
			return 1;
		}
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
			CVsPoint point((short) position.x, (short) position.y);
			window->MoveAbsolute(point);
		}
		return 0;
	}
	case WM_SIZE: {
		window->m_rect.m_width = (short) p_lParam;
		window->m_rect.m_height = (short) (p_lParam >> 16);
		if (p_wParam == SIZE_RESTORED) {
			if (window->GetSizeStatus() != 2) {
				window->SetSizeStatus(2);
				window->OnRestore();
			}
		}
		else if (p_wParam == SIZE_MINIMIZED) {
			if (window->GetSizeStatus() != 0) {
				window->SetSizeStatus(0);
				window->OnMinimise();
			}
		}
		else if (p_wParam == SIZE_MAXIMIZED) {
			if (window->GetSizeStatus() != 1) {
				window->SetSizeStatus(1);
				window->OnMaximise();
			}
		}
		window->InternalOnSize();
		window->OnSize();
		return 0;
	}
	case WM_SETFOCUS: {
		if (g_hFocusWindow != 0) {
			g_pFocusWindow->Dummy94();
			g_pFocusWindow->OnFocusLost();
		}
		window->SetFocusWindow();
		window->OnFocusGained();
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}
	case WM_KILLFOCUS: {
		if (g_pTargetGraphicsSystem->m_driverMode < 4 || 5 < g_pTargetGraphicsSystem->m_driverMode) {
			if (g_hFocusWindow != 0) {
				g_pFocusWindow->Dummy94();
				g_pFocusWindow->OnFocusLost();
			}
			g_hFocusWindow = 0;
		}
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}
	case WM_ACTIVATEAPP: {
		int wasDisplayDibActive = g_nDisplayDibActive;
		int mode = g_pTargetGraphicsSystem->m_driverMode;
		switch (mode) {
		case 1:
			g_dwFullScreenGdi = 1;
			g_nDisplayDibActive = 0;
			InvalidateRect((HWND) p_hwnd, 0, 0);
			break;
		case 2:
		case 3:
			if (p_wParam != 0) {
				if (window->GetSizeStatus() == 0) {
					SendMessageA((HWND) p_hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				}
				unsigned short flags = (g_pTargetGraphicsSystem->m_driverMode == 3 ? 5 : 1) | 0x8210;
				g_pDisplayDib(0, 0, flags);
				g_dwFullScreenGdi = 0;
				g_nDisplayDibActive = 1;
				_outpw(0x3d4, 0xc);
				_outpw(0x3d4, 0xd);
				InvalidateRect((HWND) p_hwnd, 0, 0);
			}
			else {
				g_pDisplayDib(0, 0, 0x4000);
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
				CVsSize* screenSize = &g_pTargetGraphicsDriver->m_screenSize;
				mouseParameters[2] = screenSize->m_width * mouseParameters[2] / screenWidth;
				SystemParametersInfoA(SPI_SETMOUSE, 0, mouseParameters, 0);
				SystemParametersInfoA(SPI_GETSCREENSAVEACTIVE, 0, &g_nSavedScreenSaverActive, 0);
				if (g_nSavedScreenSaverActive != 0) {
					SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, 0, 0, 0);
				}
				clipRect.top = 0;
				clipRect.left = 0;
				clipRect.right = screenSize->m_width;
				clipRect.bottom = screenSize->m_height;
				ClipCursor(&clipRect);
			}
			else {
				ClipCursor(0);
				SystemParametersInfoA(SPI_SETMOUSE, 0, g_savedMouseParameters, 0);
				if (g_nSavedScreenSaverActive != 0) {
					SystemParametersInfoA(SPI_SETSCREENSAVEACTIVE, 0, (void*) 1, 0);
				}
			}
		}
		return window->ProcessOtherMessages(WM_ACTIVATEAPP, p_wParam, p_lParam);
	}
	case WM_DISPLAYCHANGE: {
		if (g_pTargetGraphicsSystem != 0) {
			CVsSize size;
			size.m_width = (short) p_lParam;
			size.m_height = (short) (p_lParam >> 16);
			g_pTargetGraphicsSystem->UpdateDriverSize(size);
		}
		return 0;
	}
	case WM_KEYDOWN:
	case WM_KEYUP: {
		posted.type = (unsigned short) ((p_message == WM_KEYDOWN) + 1);
		posted.code = (int) p_wParam;
		g_pMasterInputQueue->Post(posted);
		return 0;
	}
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK: {
		posted.type = 6;
		style = window->GetStyle();
		if ((style & 0x1000) != 0) {
			switch (p_message) {
			case WM_LBUTTONDOWN:
				posted.payload = (void*) 0x43;
				break;
			case WM_LBUTTONDBLCLK:
				posted.payload = (void*) 0x46;
				break;
			case WM_RBUTTONDOWN:
				posted.payload = (void*) 0x44;
				break;
			case WM_RBUTTONDBLCLK:
				posted.payload = (void*) 0x47;
				break;
			case WM_MBUTTONDOWN:
				posted.payload = (void*) 0x45;
				break;
			case WM_MBUTTONDBLCLK:
				posted.payload = (void*) 0x48;
				break;
			}
		}
		else {
			switch (p_message) {
			case WM_LBUTTONDOWN:
			case WM_LBUTTONDBLCLK:
				posted.payload = (void*) 0x43;
				break;
			case WM_RBUTTONDOWN:
			case WM_RBUTTONDBLCLK:
				posted.payload = (void*) 0x44;
				break;
			case WM_MBUTTONDOWN:
			case WM_MBUTTONDBLCLK:
				posted.payload = (void*) 0x45;
				break;
			}
		}
		mouseX = window->m_rect.m_x + (short) p_lParam;
		mouseY = window->m_rect.m_y + (short) (p_lParam >> 16);
		posted.code = PackParam(mouseX, mouseY);
		posted.source = 0;
		g_pMasterInputQueue->Post(posted);
		if (g_nMouseCaptureCount++ == 0) {
			SetCapture((HWND) p_hwnd);
		}
		return 0;
	}
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP: {
		posted.type = 5;
		if (p_message == WM_LBUTTONUP) {
			posted.payload = (void*) 0x43;
		}
		else if (p_message == WM_RBUTTONUP) {
			posted.payload = (void*) 0x44;
		}
		else {
			posted.payload = (void*) 0x45;
		}
		mouseX = window->m_rect.m_x + (short) p_lParam;
		mouseY = window->m_rect.m_y + (short) (p_lParam >> 16);
		posted.code = PackParam(mouseX, mouseY);
		posted.source = 0;
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
			command.type = 0xf;
			command.time = CurrentQueueTimer();
			command.code = menuAction;
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
void CWnd::MoveAbsolute(const CVsPoint& p_point)
{
	void** node = (void**) m_childList;
	CVsPoint* position;
	if (this != (CWnd*) -8) {
		position = (CVsPoint*) &m_rect.m_x;
	}
	else {
		position = 0;
	}
	CVsPoint delta((short) (p_point.m_x - position->m_x), (short) (p_point.m_y - position->m_y));
	for (;;) {
		if (node == 0) {
			break;
		}
		((CPvWnd*) node[0])->InternalOnMove(delta);
		node = (void**) node[1];
	}
	m_rect.m_x = p_point.m_x;
	m_rect.m_y = p_point.m_y;
	m_relativeTopLeft.m_x = p_point.m_x;
	m_relativeTopLeft.m_y = p_point.m_y;
	InternalOnMove();
}

// FUNCTION: LEMBALL 0x00464fa0
void CWnd::Move(const CVsPoint& p_point)
{
	void** node = (void**) m_childList;
	CVsPoint delta((short) (p_point.m_x - m_relativeTopLeft.m_x), (short) (p_point.m_y - m_relativeTopLeft.m_y));
	for (;;) {
		if (node == 0) {
			break;
		}
		CPvWnd* child = (CPvWnd*) node[0];
		child->InternalOnMove(delta);
		child->OnMove();
		node = (void**) node[1];
	}
	m_rect.m_x = (short) (m_rect.m_x + delta.m_x);
	m_rect.m_y = (short) (m_rect.m_y + delta.m_y);
	m_relativeTopLeft.m_x = p_point.m_x;
	m_relativeTopLeft.m_y = p_point.m_y;
	InternalOnMove();
	OnMove();
	if (m_nativeWindow != 0) {
		SetWindowPos((HWND) m_nativeWindow, 0, p_point.m_x, p_point.m_y, 0, 0, 5);
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
		if (g_pTargetGraphicsSystem->IsFullscreenDriver() != 0 && g_pFocusWindow != 0) {
			g_nLastCursorX += g_pFocusWindow->m_rect.m_x;
			g_nLastCursorY += g_pFocusWindow->m_rect.m_y;
		}
		posted.type = 7;
		posted.time = CurrentQueueTimer();
		posted.code = PackParam((short) g_nLastCursorX, (short) g_nLastCursorY);
		posted.payload = 0;
		posted.source = 0;
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
	if (g_preInitActive.m_icon == 0) {
		windowClass.hIcon = LoadIconA(0, (LPCSTR) 0x7f00);
	}
	else {
		windowClass.hIcon = (HICON) g_preInitActive.m_icon;
	}
	windowClass.hCursor = 0;
	windowClass.hbrBackground = (HBRUSH) GetStockObject(4);
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = g_pszVsBaseWindowClass;
	cursor = LoadCursorA(0, (LPCSTR) 0x7f00);
	atom = RegisterClassA(&windowClass);
	SetCursor(cursor);
	ShowCursor(1);
	if (atom == 0) {
		FatalWin32Error(g_szUnableToRegisterBaseWindowClass);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x004651d0
CWnd::CWnd()
{
	if (g_cursorState == 1) {
		RegisterBaseWindowClass();
	}
	m_nativeWindow = 0;
}

// FUNCTION: LEMBALL 0x00465200
void CWnd::Create(const CVsRect& p_rect, CPvWnd* p_parent, char* p_title)
{
	unsigned int styleFlags;
	unsigned int style;
	POINT screenPoint;
	RECT windowRect;
	HMENU menu;
	HWND parentWindow;
	int hasMenu;
	int menuResourceId;
	MenuList** menuLists;
	HWND hwnd;

	m_rect.m_x = 0;
	m_rect.m_y = 0;
	m_rect.m_width = p_rect.m_width;
	m_rect.m_height = p_rect.m_height;
	SetSizeStatus(2);
	InitHotAreaList();
	m_parent = p_parent;
	m_createRect = p_parent;

	if (p_parent != 0) {
		styleFlags = GetStyle();
		if ((styleFlags & 0x40000000) == 0) {
			p_parent->AddChild(this);
			screenPoint.x = (LONG) ((int) p_parent->m_rect.m_x + (int) p_rect.m_x);
			screenPoint.y = (LONG) ((int) p_parent->m_rect.m_y + (int) p_rect.m_y);
			ClientToScreen((HWND) ((CWnd*) p_parent)->m_nativeWindow, &screenPoint);
			m_rect.m_x = (short) screenPoint.x;
			m_rect.m_y = (short) screenPoint.y;
			m_relativeTopLeft.m_x = p_rect.m_x;
			m_relativeTopLeft.m_y = p_rect.m_y;
			m_zoom = p_parent->m_zoom;
			InternalOnCreate();
			OnCreate();
			InternalOnSize();
			OnSize();
			return;
		}
	}

	if (g_pTargetGraphicsSystem->m_driverMode < 4 || 5 < g_pTargetGraphicsSystem->m_driverMode) {
		style = ConvertWindowStyleFlags(GetStyle());
		parentWindow = 0;
		styleFlags = GetStyle();
		if ((styleFlags & 0x40000000) != 0 && p_parent != 0) {
			m_parent = 0;
			style = (style & 0x7fffffff) | 0x40000000;
		}

		windowRect.left = p_rect.m_x;
		windowRect.top = p_rect.m_y;
		windowRect.right = p_rect.m_x + p_rect.m_width;
		windowRect.bottom = p_rect.m_y + p_rect.m_height;

		m_menuLists = 0;
		m_menuResourceId = 0;
		menuResourceId = 0;
		menuLists = 0;
		hasMenu = GetMenu(menuResourceId, &menuLists);
		menu = 0;
		if (hasMenu != 0) {
			m_menuResourceId = (unsigned int) menuResourceId;
			m_menuLists = menuLists;
			menu = LoadMenuA((HINSTANCE) g_pApplicationInstance, (LPCSTR) (menuResourceId & 0xffff));
		}

		AdjustWindowRect(&windowRect, style, hasMenu != 0);
		if ((style & 0x40000000) == 0 || p_parent == 0) {
			parentWindow = 0;
		}
		else {
			parentWindow = (HWND) ((CWnd*) p_parent)->m_nativeWindow;
		}
		hwnd = CreateWindowExA(0,
							   g_pszVsBaseWindowClass,
							   p_title,
							   style,
							   windowRect.left,
							   windowRect.top,
							   windowRect.right - windowRect.left,
							   windowRect.bottom - windowRect.top,
							   parentWindow,
							   menu,
							   (HINSTANCE) g_pApplicationInstance,
							   this);
		m_nativeWindow = hwnd;
		if (menu != 0) {
			ReSetMenu();
		}
		if (m_nativeWindow == 0) {
			FatalWin32Error(g_szUnableToCreateWindow);
		}
		styleFlags = GetStyle();
		if ((styleFlags & 1) != 0) {
			UpdateWindow((HWND) m_nativeWindow);
			ShowWindow((HWND) m_nativeWindow, 5);
			SetForegroundWindow((HWND) m_nativeWindow);
		}
		return;
	}

	m_parent = 0;
	m_rect.m_width = p_rect.m_width;
	m_rect.m_height = p_rect.m_height;
	m_rect.m_x = p_rect.m_x;
	m_rect.m_y = p_rect.m_y;
	m_relativeTopLeft.m_x = p_rect.m_x;
	m_relativeTopLeft.m_y = p_rect.m_y;
	g_pTargetGraphicsSystem->m_reserved04 = (unsigned int) this;
	m_nativeWindow = g_pTargetGraphicsDriver->m_window;
	SetFocusWindow();
	OnFocusGained();
	InternalOnCreate();
	OnCreate();
	InternalOnSize();
	OnSize();
}

#pragma warning(disable : 4146)
// FUNCTION: LEMBALL 0x004654f0
unsigned int ConvertWindowStyleFlags(unsigned int p_style)
{
	unsigned int style = 0;
	if (p_style & 8) {
		style = 0xcb0000;
	}
	if (p_style & 2) {
		style |= 0xc00000;
	}
	else {
		style |= 0x80000000;
	}
	style |= (p_style & 0x400) << 8;
	style |= (p_style & 0x40) << 10;
	style |= (p_style & 0x20) << 15;
	style |= (p_style & 0x10) << 17;
	style |= (p_style & 0x80) << 10;
	style |= (p_style & 0x100) << 11;
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
	CPvWnd* child;
	void** childNode;

	if (m_lifecycleRefs != 0) {
		childNode = (void**) m_childList;
		for (;;) {
			if (childNode == 0) {
				break;
			}
			child = (CPvWnd*) childNode[0];
			childNode = (void**) childNode[1];
			child->Destroy();
		}
		OnDestroy();
		InternalOnDestroy();
		if ((g_pTargetGraphicsSystem->m_driverMode < 4 || g_pTargetGraphicsSystem->m_driverMode > 5) &&
			m_nativeWindow != 0) {
			DestroyWindow((HWND) m_nativeWindow);
		}
	}
}

// FUNCTION: LEMBALL 0x004655f0
void CWnd::Refresh(CVsRect* p_rect)
{
	RECT rect;

	rect.left = 0;
	rect.top = 0;
	if (p_rect == 0) {
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
	if (*lists != 0) {
		BOOL(WINAPI * enableMenuItem)(HMENU, UINT, UINT) = EnableMenuItem;
		DWORD(WINAPI * checkMenuItem)(HMENU, UINT, UINT) = CheckMenuItem;
		do {
			MenuList* item = *lists;
			while (item->m_name != 0) {
				if (item->m_enabled != 0) {
					enableMenuItem(menu, item->m_commandId, MF_ENABLED);
				}
				else {
					enableMenuItem(menu, item->m_commandId, MF_GRAYED);
				}
				if (item->m_unk10 != 0) {
					checkMenuItem(menu, item->m_commandId, MF_CHECKED);
				}
				else {
					checkMenuItem(menu, item->m_commandId, MF_UNCHECKED);
				}
				item++;
			}
			lists++;
		} while (*lists != 0);
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
	if (p_menuLists != 0) {
		newMenu = LoadMenuA((HINSTANCE) g_pApplicationInstance, (LPCSTR) (unsigned short) resourceId);
		::SetMenu((HWND) m_nativeWindow, newMenu);
		ReSetMenu();
	}
	if (currentMenu != 0) {
		DestroyMenu(currentMenu);
	}
}

// FUNCTION: LEMBALL 0x00465750
int CWnd::SelectMenu(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
{
	int* menuList;
	int* item;

	if (m_menuLists == 0) {
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
void CWnd::InternalOnZoom(int p_oldZoom)
{
	RECT windowRect;
	RECT clientRect;

	CPvWnd::InternalOnZoom(p_oldZoom);
	if (m_nativeWindow != 0 && g_pTargetGraphicsDriver->m_window != m_nativeWindow) {
		GetWindowRect((HWND) m_nativeWindow, &windowRect);
		GetClientRect((HWND) m_nativeWindow, &clientRect);
		windowRect.right -= windowRect.left;
		windowRect.bottom -= windowRect.top;
		windowRect.right -= clientRect.right;
		windowRect.bottom -= clientRect.bottom;
		windowRect.right += clientRect.right;
		windowRect.bottom += clientRect.bottom;
		SetWindowPos((HWND) m_nativeWindow, 0, 0, 0, windowRect.right, windowRect.bottom, 6);
	}
}

// FUNCTION: LEMBALL 0x00465820
void CWnd::InternalSetRect(const CVsRect& p_rect)
{
	CVsRect rect(p_rect);
	CVsPoint* origin = &rect;
	const CVsSize* size = &rect;
	RECT adjusted;
	RECT window;
	POINT client;
	const CVsPoint* position;
	if (m_nativeWindow != 0 && g_pTargetGraphicsDriver->m_window != m_nativeWindow) {
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
		AdjustWindowRect(&adjusted, style, m_menuLists != 0);
		adjusted.right -= adjusted.left;
		adjusted.bottom -= adjusted.top;
		SetWindowPos((HWND) m_nativeWindow, 0, window.left, window.top, adjusted.right, adjusted.bottom, 4);
		return;
	}
	position = (const CVsPoint*) &m_parent->m_rect;
	window.top = (short) (position->m_y + origin->m_y);
	window.left = (short) (position->m_x + origin->m_x);
	ClientToScreen((HWND) ((CWnd*) m_parent)->m_nativeWindow, (POINT*) &window);
	m_rect.m_x = (short) window.left;
	m_rect.m_y = (short) window.top;
	m_rect.m_width = size->m_width;
	m_rect.m_height = size->m_height;
	m_relativeTopLeft.m_x = origin->m_x;
	m_relativeTopLeft.m_y = origin->m_y;
	InternalOnMove();
	OnMove();
	InternalOnSize();
	OnSize();
}

// FUNCTION: LEMBALL 0x00465a00
void CWnd::InternalSetRelTL(const CVsPoint& p_point)
{
	CVsRect rect(p_point.m_x, p_point.m_y, m_rect.m_width, m_rect.m_height);
	InternalSetRect(rect);
}

// FUNCTION: LEMBALL 0x00465a90
unsigned int CWnd::GetStyle()
{
	return 0;
}
