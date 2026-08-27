#include "Wnd.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../../Control/Support/PreInit.h"
#include "../../Platform/Windows/Entry.h"
#include "../Foundation/BaseQueue.h"
#include "../Foundation/BaseQueueHandler.h"
#include "../Foundation/VsDebug.h"
#include "../Network/BaseNetwork.h"
#include "../Resources/MogRes.h"
#include "../Target/TargetGraphicsDriver.h"
#include "../Target/TargetGraphicsSystemState.h"
#include "../Target/TargetPlatformServices.h"
#include "Cursor.h"

// GLOBAL: LEMBALL 0x004a1f64
void* g_hFocusWindow = 0;

// GLOBAL: LEMBALL 0x004a1fec
int g_cursorState = 0;

// GLOBAL: LEMBALL 0x004a1fe8
int g_nNativeWindowCount = 0;

// GLOBAL: LEMBALL 0x004a1ff0
void* g_pWindowOwnerList = 0;

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

static bool RegisterBaseWindowClass();
unsigned int ConvertWindowStyleFlags(unsigned int p_style);

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
void Wnd::OnMove()
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
// STUB: LEMBALL 0x00464520
long __stdcall Wnd::ProcessMessage(void* p_hwnd, unsigned int p_message, unsigned int p_wParam, unsigned int p_lParam)
{
	Wnd* window;
	CREATESTRUCTA* create;

	if (g_pTargetGraphicsDriver == 0) {
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}

	window = (Wnd*) GetWindowLongA((HWND) p_hwnd, GWL_USERDATA);
	if (g_pTargetGraphicsDriver->m_window == p_hwnd) {
		window = (Wnd*) g_pTargetGraphicsSystem->m_reserved04;
		if (window == 0) {
			return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
		}
	}
	else if (window == 0 && p_message != WM_CREATE) {
		return DefWindowProcA((HWND) p_hwnd, p_message, p_wParam, p_lParam);
	}

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
		window->m_nativeWindow = 0;
		g_dwWindowQuitRequested = 1;
		return 0;
	}
	if (p_message == WM_SIZE) {
		window->m_rect.m_width = (short) p_lParam;
		window->m_rect.m_height = (short) (p_lParam >> 16);
		window->OnSize();
		window->BaseOnSize();
		return 0;
	}
	if (p_message == WM_QUIT) {
		return 0;
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
// STUB: LEMBALL 0x00465050
void Wnd::ProcessMouseMoves()
{
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
// STUB: LEMBALL 0x00465200
void Wnd::Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title)
{
	unsigned int style;
	unsigned int flags;
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

	flags = GetStyle();
	if (p_parent != 0 && (flags & 0x40000000) == 0) {
		p_parent->AddChild(this);
		m_rect.m_x = (short) (p_parent->m_rect.m_x + p_rect.m_x);
		m_rect.m_y = (short) (p_parent->m_rect.m_y + p_rect.m_y);
		m_relativeTopLeft.m_x = p_rect.m_x;
		m_relativeTopLeft.m_y = p_rect.m_y;
		m_zoom = p_parent->m_zoom;
		OnCreate();
		Dummy3c();
		OnSize();
		BaseOnSize();
		return;
	}

	if (g_pTargetGraphicsSystem->m_driverMode < 4 || 5 < g_pTargetGraphicsSystem->m_driverMode) {
		style = ConvertWindowStyleFlags(GetStyle());
		parentWindow = 0;
		if ((GetStyle() & 0x40000000) != 0 && p_parent != 0) {
			m_parent = 0;
			style = (style & 0x7fffffff) | 0x40000000;
			parentWindow = (HWND) ((Wnd*) p_parent)->m_nativeWindow;
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
		if ((GetStyle() & 1) != 0) {
			UpdateWindow((HWND) m_nativeWindow);
			ShowWindow((HWND) m_nativeWindow, 5);
			SetForegroundWindow((HWND) m_nativeWindow);
		}
	}
	else {
		m_parent = 0;
		m_rect.m_width = p_rect.m_width;
		m_rect.m_height = p_rect.m_height;
		m_rect.m_x = p_rect.m_x;
		m_rect.m_y = p_rect.m_y;
		m_relativeTopLeft.m_x = p_rect.m_x;
		m_relativeTopLeft.m_y = p_rect.m_y;
		g_pTargetGraphicsSystem->m_reserved04 = (unsigned int) this;
		m_nativeWindow = g_pTargetGraphicsDriver->m_window;
		OnCreate();
		Dummy3c();
		OnSize();
		BaseOnSize();
	}
}

// STUB: LEMBALL 0x004654f0
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
// STUB: LEMBALL 0x004655a0
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
