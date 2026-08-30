#include "Wnd.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../../Control/Support/PreInit.h"
#include "../../Platform/Windows/Entry.h"
#include "../Foundation/BaseQueue.h"
#include "../Foundation/BaseQueueHandler.h"
#include "../Foundation/VsDebug.h"
#include "../Foundation/VsOStream.h"
#include "../Foundation/VsTime.h"
#include "../Messaging/PackParam.h"
#include "../Network/BaseNetwork.h"
#include "../Resources/MogRes.h"
#include "../Target/TargetGraphicsDriver.h"
#include "../Target/TargetGraphicsSystemState.h"
#include "../Target/TargetPlatformServices.h"
#include "Cursor.h"

// GLOBAL: LEMBALL 0x004a1f64
void* g_hFocusWindow = 0;

// GLOBAL: LEMBALL 0x004a1f68
Wnd* g_pFocusWindow = 0;

// GLOBAL: LEMBALL 0x004a1f6c
int g_nMouseCaptureCount = 0;

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

static bool RegisterBaseWindowClass();
unsigned int ConvertWindowStyleFlags(unsigned int p_style);

// FUNCTION: LEMBALL 0x004324a0
void Wnd::SetFocusWindow()
{
	g_hFocusWindow = m_nativeWindow;
	g_pFocusWindow = this;
}

// 68K 0x1010480a IsFocusWindow__4CWndFv
// FUNCTION: LEMBALL 0x004324d0
bool Wnd::IsFocusWindow()
{
	return m_nativeWindow == g_hFocusWindow;
}

// 68K 0x101048b4 OnMinimise__4CWndFv
// FUNCTION: LEMBALL 0x004324f0
void Wnd::OnMinimise()
{
}

// 68K 0x101048d2 OnMaximise__4CWndFv
// FUNCTION: LEMBALL 0x00432500
void Wnd::OnMaximise()
{
}

// 68K 0x1010489a OnMove__4CWndFv
// FUNCTION: LEMBALL 0x0043a4d0 FOLDED
void Wnd::Dummy48()
{
}

// 68K 0x101048f0 OnRestore__4CWndFv
// FUNCTION: LEMBALL 0x0043a4e0
void Wnd::OnRestore()
{
}

// 68K 0x1010490e OnZoom__4CWndFi
// FUNCTION: LEMBALL 0x0043a4f0
void Wnd::OnZoom(int p_oldZoom)
{
}

// 68K 0x10104928 OnZoomBox__4CWndFUc
// FUNCTION: LEMBALL 0x0043a500
void Wnd::OnZoomBox()
{
}

// 68K 0x10104846 OnCreate__4CWndFv
// FUNCTION: LEMBALL 0x004644f0 FOLDED
void Wnd::Dummy3c()
{
}

// 68K 0x10104862 OnDestroy__4CWndFv
// FUNCTION: LEMBALL 0x00464500 FOLDED
void Wnd::OnDestroy()
{
}

// 68K 0x10104880 OnSize__4CWndFv
// FUNCTION: LEMBALL 0x00464510 FOLDED
void Wnd::BaseOnSize()
{
}

// 68K 0x10110340 ProcessMessage__4CWndFR11EventRecord
// FUNCTION: LEMBALL 0x00464520
long __stdcall Wnd::ProcessMessage(void* p_hwnd, unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
{
	Wnd* window;
	CREATESTRUCTA* create;
	Message posted;
	short mouseX;
	short mouseY;
	unsigned int style;
	int menuAction;
	unsigned long sequence;

	if (g_pTargetGraphicsDriver == 0) {
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}

	sequence = GetMessageTime();
	window = (Wnd*) GetWindowLongA((HWND) p_hwnd, GWL_USERDATA);
	if (g_pTargetGraphicsDriver->m_window == p_hwnd) {
		Wnd* reservedWindow = (Wnd*) g_pTargetGraphicsSystem->m_reserved04;
		if (reservedWindow != 0) {
			window = reservedWindow;
		}
		else if (window == 0) {
			return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
		}
	}
	else if (window == 0 && p_message != WM_CREATE) {
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}

	mouseX = (short) p_lParam;
	mouseY = (short) (p_lParam >> 16);

	if (p_message == WM_CREATE) {
		create = (CREATESTRUCTA*) p_lParam;
		window = (Wnd*) create->lpCreateParams;
		SetWindowLongA((HWND) p_hwnd, GWL_USERDATA, (LONG) window);
		window->m_nativeWindow = p_hwnd;
		window->OnCreate();
		window->Dummy3c();
		return 0;
	}
	if (p_message == WM_DESTROY) {
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
	if (p_message == WM_CLOSE) {
		if ((unsigned short) p_lParam == 1) {
			ReleaseCapture();
			return 1;
		}
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}
	if (p_message == WM_QUIT) {
		if (g_pDebugOutput != 0) {
			*g_pDebugOutput << g_szQuitting;
		}
		ReleaseCapture();
		return 0;
	}
	if (p_message == WM_SIZE) {
		window->m_rect.m_width = (short) p_lParam;
		window->m_rect.m_height = (short) (p_lParam >> 16);
		window->OnSize();
		window->BaseOnSize();
		return 0;
	}
	if (p_message == WM_SETFOCUS) {
		if (g_hFocusWindow != 0 && g_pFocusWindow != 0) {
			g_pFocusWindow->Dummy94();
			g_pFocusWindow->Dummy9c();
		}
		window->SetFocusWindow();
		window->Dummy98();
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}
	if (p_message == WM_KILLFOCUS) {
		if (g_pTargetGraphicsSystem->m_driverMode < 4 || 5 < g_pTargetGraphicsSystem->m_driverMode) {
			if (g_hFocusWindow != 0 && g_pFocusWindow != 0) {
				g_pFocusWindow->Dummy94();
				g_pFocusWindow->Dummy9c();
			}
			g_hFocusWindow = 0;
		}
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}
	if (p_message == WM_KEYDOWN || p_message == WM_KEYUP) {
		posted.type = (unsigned short) ((p_message == WM_KEYDOWN) + 1);
		posted.time = sequence;
		posted.code = (int) p_wParam;
		posted.payload = 0;
		posted.source = 0;
		if (g_pMasterInputQueue != 0) {
			g_pMasterInputQueue->Post(posted);
		}
		return 0;
	}
	if (p_message == WM_LBUTTONDOWN || p_message == WM_LBUTTONDBLCLK || p_message == WM_RBUTTONDOWN ||
		p_message == WM_RBUTTONDBLCLK || p_message == WM_MBUTTONDOWN || p_message == WM_MBUTTONDBLCLK) {
		posted.type = 6;
		posted.time = sequence;
		style = window->GetStyle();
		if ((style & 0x1000) == 0) {
			if (p_message == WM_LBUTTONDOWN || p_message == WM_LBUTTONDBLCLK) {
				posted.payload = (void*) 0x43;
			}
			else if (p_message == WM_RBUTTONDOWN || p_message == WM_RBUTTONDBLCLK) {
				posted.payload = (void*) 0x44;
			}
			else {
				posted.payload = (void*) 0x45;
			}
		}
		else {
			if (p_message == WM_LBUTTONDOWN) {
				posted.payload = (void*) 0x43;
			}
			else if (p_message == WM_LBUTTONDBLCLK) {
				posted.payload = (void*) 0x46;
			}
			else if (p_message == WM_RBUTTONDOWN) {
				posted.payload = (void*) 0x44;
			}
			else if (p_message == WM_RBUTTONDBLCLK) {
				posted.payload = (void*) 0x47;
			}
			else if (p_message == WM_MBUTTONDOWN) {
				posted.payload = (void*) 0x45;
			}
			else {
				posted.payload = (void*) 0x48;
			}
		}
		posted.code = PackParam((short) (window->m_rect.m_x + mouseX), (short) (window->m_rect.m_y + mouseY));
		posted.source = 0;
		if (g_pMasterInputQueue != 0) {
			g_pMasterInputQueue->Post(posted);
		}
		g_nMouseCaptureCount = g_nMouseCaptureCount + 1;
		if (g_nMouseCaptureCount == 1) {
			SetCapture((HWND) p_hwnd);
		}
		return 0;
	}
	if (p_message == WM_LBUTTONUP || p_message == WM_RBUTTONUP || p_message == WM_MBUTTONUP) {
		posted.type = 5;
		posted.time = sequence;
		if (p_message == WM_LBUTTONUP) {
			posted.payload = (void*) 0x43;
		}
		else if (p_message == WM_RBUTTONUP) {
			posted.payload = (void*) 0x44;
		}
		else {
			posted.payload = (void*) 0x45;
		}
		posted.code = PackParam((short) (window->m_rect.m_x + mouseX), (short) (window->m_rect.m_y + mouseY));
		posted.source = 0;
		if (g_pMasterInputQueue != 0) {
			g_pMasterInputQueue->Post(posted);
		}
		g_nMouseCaptureCount = g_nMouseCaptureCount - 1;
		if (g_nMouseCaptureCount == 0) {
			ReleaseCapture();
		}
		return 0;
	}
	if (p_message == WM_COMMAND) {
		menuAction = window->SelectMenu(p_message, p_wParam, p_lParam);
		if (menuAction != 0) {
			posted.type = 0xf;
			posted.time = CurrentQueueTimer();
			posted.code = menuAction;
			posted.payload = 0;
			posted.source = 0;
			if (g_pMasterInputQueue != 0) {
				g_pMasterInputQueue->Post(posted);
			}
			return 0;
		}
		return window->ProcessOtherMessages(p_message, p_wParam, p_lParam);
	}

	return window->ProcessOtherMessages(p_message, p_wParam, p_lParam);
}

// 68K 0x101112b8 MoveAbsolute__4CWndFRC8CVSPoint
// STUB: LEMBALL 0x00464f10
void Wnd::MoveAbsolute(const VsPoint& p_point)
{
}

// 68K 0x10111384 Move__4CWndFRC8CVSPoint
// STUB: LEMBALL 0x00464fa0
void Wnd::Move(const VsPoint& p_point)
{
}

// 68K 0x1011023e ProcessMouseMoves__4CWndFv
// FUNCTION: LEMBALL 0x00465050
void Wnd::ProcessMouseMoves()
{
	POINT position;
	Message posted;
	void* nativeWindow;
	WindowOwnerNode* node;

	GetCursorPos(&position);
	nativeWindow = 0;
	if (g_pFocusWindow != 0) {
		nativeWindow = g_pFocusWindow->m_nativeWindow;
	}
	if (nativeWindow == 0 && g_pTargetGraphicsDriver != 0) {
		nativeWindow = g_pTargetGraphicsDriver->m_window;
	}
	if (nativeWindow == 0 && g_pWindowOwnerList != 0) {
		node = g_pWindowOwnerList->m_head;
		while (node != 0 && nativeWindow == 0) {
			if (node->m_window != 0) {
				nativeWindow = ((Wnd*) node->m_window)->m_nativeWindow;
			}
			node = node->m_next;
		}
	}
	if (nativeWindow != 0) {
		ScreenToClient((HWND) nativeWindow, &position);
	}
	else if (g_pTargetGraphicsSystem->IsFullscreenDriver() != 0 && g_pFocusWindow != 0) {
		position.x = position.x + g_pFocusWindow->m_rect.m_x;
		position.y = position.y + g_pFocusWindow->m_rect.m_y;
	}
	if (position.x != g_nLastCursorX || position.y != g_nLastCursorY) {
		g_nLastCursorX = position.x;
		g_nLastCursorY = position.y;
		posted.type = 7;
		posted.time = CurrentQueueTimer();
		posted.code = PackParam((short) g_nLastCursorX, (short) g_nLastCursorY);
		posted.payload = 0;
		posted.source = 0;
		if (g_pMasterInputQueue != 0) {
			g_pMasterInputQueue->Post(posted);
		}
	}
}

// STUB: LEMBALL 0x00465110
static bool RegisterBaseWindowClass()
{
	WNDCLASSA windowClass;
	HCURSOR cursor;
	ATOM atom;

	windowClass.style = 0xb;
	windowClass.lpfnWndProc = (WNDPROC) Wnd::ProcessMessage;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 4;
	windowClass.hInstance = (HINSTANCE) g_pApplicationInstance;
	windowClass.hIcon = (HICON) g_preInitActive.m_icon;
	if (g_preInitActive.m_icon == 0) {
		windowClass.hIcon = LoadIconA(0, (LPCSTR) 0x7f00);
	}
	windowClass.hCursor = 0;
	windowClass.hbrBackground = (HBRUSH) GetStockObject(4);
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = g_pszVsBaseWindowClass;
	cursor = LoadCursorA(0, (LPCSTR) 0x7f00);
	atom = RegisterClassA(&windowClass);
	SetCursor(cursor);
	ShowCursor(1);
	if (atom != 0) {
		return 1;
	}
	FatalWin32Error(g_szUnableToRegisterBaseWindowClass);
	return 0;
}

// 68K 0x10110d2c __ct__4CWndFv
// STUB: LEMBALL 0x004651d0
Wnd::Wnd()
{
	if (g_cursorState == 1) {
		RegisterBaseWindowClass();
	}
	m_nativeWindow = 0;
}

// 68K 0x10110e74 Create__4CWndFRC7CVSRectP6CPVWndPc
// FUNCTION: LEMBALL 0x00465200
void Wnd::Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title)
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
			ClientToScreen((HWND) ((Wnd*) p_parent)->m_nativeWindow, &screenPoint);
			m_rect.m_x = (short) screenPoint.x;
			m_rect.m_y = (short) screenPoint.y;
			m_relativeTopLeft.m_x = p_rect.m_x;
			m_relativeTopLeft.m_y = p_rect.m_y;
			m_zoom = p_parent->m_zoom;
			OnCreate();
			Dummy3c();
			OnSize();
			BaseOnSize();
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
			parentWindow = (HWND) ((Wnd*) p_parent)->m_nativeWindow;
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
	Dummy98();
	OnCreate();
	Dummy3c();
	OnSize();
	BaseOnSize();
}

// FUNCTION: LEMBALL 0x004654f0
unsigned int ConvertWindowStyleFlags(unsigned int p_style)
{
	return ((p_style & 8) == 0) - 1 & 0xcb0000
		| (-(unsigned int) ((p_style & 2) == 0) & 0x7f400000) + 0xc00000 | (p_style & 0x400) << 8
		| (p_style & 0x40) << 10 | (p_style & 0x20) << 15 | (p_style & 0x10) << 17 | (p_style & 0x80) << 10
		| (p_style & 0x100) << 11;
}

// 68K 0x1011126e __dt__4CWndFv
// FUNCTION: LEMBALL 0x00465570
Wnd::~Wnd()
{
	if (g_cursorState == 1) {
		ShowCursor(0);
	}
}

// 68K 0x10111462 Destroy__4CWndFv
// FUNCTION: LEMBALL 0x004655a0
void Wnd::Destroy()
{
	PvWnd* child;
	void** childNode;

	if (m_lifecycleRefs != 0) {
		childNode = (void**) m_childList;
		while (childNode != 0) {
			child = (PvWnd*) childNode[0];
			childNode = (void**) childNode[1];
			child->Destroy();
		}
		OnDestroy();
		BaseOnDestroy();
		if ((g_pTargetGraphicsSystem->m_driverMode < 4 || g_pTargetGraphicsSystem->m_driverMode > 5)
			&& m_nativeWindow != 0) {
			DestroyWindow((HWND) m_nativeWindow);
		}
	}
}

// 68K 0x10104946 Refresh__4CWndFP7CVSRect
// FUNCTION: LEMBALL 0x004655f0
void Wnd::Refresh(VsRect* p_rect)
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

// 68K 0x10110854 ProcessOtherMessages__4CWndFR11EventRecords
// FUNCTION: LEMBALL 0x00465640
int Wnd::ProcessOtherMessages(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
{
	return DefWindowProcA((HWND) m_nativeWindow, p_message, p_wParam, p_lParam);
}

// 68K 0x10111552 ReSetMenu__4CWndFv
// STUB: LEMBALL 0x00465660
void Wnd::ReSetMenu()
{
}

// 68K 0x10111570 SetMenu__4CWndFRiPP11tagMenuList
// FUNCTION: LEMBALL 0x004656f0
void Wnd::SetMenu(int& p_menuResourceId, MenuList** p_menuLists)
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

// 68K 0x1011162c SelectMenu__4CWndFl
// FUNCTION: LEMBALL 0x00465750
int Wnd::SelectMenu(unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
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

// 68K 0x1011177e _SetRect__4CWndFRC7CVSRect
// STUB: LEMBALL 0x00465820
void Wnd::SetRect(const VsRect& p_rect)
{
}

// 68K 0x10111a0a _SetRelTL__4CWndFRC8CVSPoint
// FUNCTION: LEMBALL 0x00465a00
void Wnd::SetRelTl(const VsPoint& p_point)
{
	VsRect rect(p_point.m_x, p_point.m_y, m_rect.m_width, m_rect.m_height);
	SetRect(rect);
}

// 68K 0x10110cb8 GetStyle__4CWndFv
// FUNCTION: LEMBALL 0x00465a90
unsigned int Wnd::GetStyle()
{
	return 0;
}

void Wnd::Dummy94()
{
}

void Wnd::Dummy98()
{
}

void Wnd::Dummy9c()
{
}
