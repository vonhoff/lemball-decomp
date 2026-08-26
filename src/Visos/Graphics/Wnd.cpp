#include "Wnd.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "../Target/TargetGraphicsDriver.h"
#include "../Target/TargetGraphicsSystemState.h"

// GLOBAL: LEMBALL 0x004a1f64
void* g_hFocusWindow = 0;

// GLOBAL: LEMBALL 0x004a1fec
int g_cursorState = 0;

// GLOBAL: LEMBALL 0x004a8188
void* g_pApplicationInstance = 0;

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
void Wnd::OnCreate()
{
}

// 68K 0x10104862 OnDestroy__4CWndFv
// FUNCTION: LEMBALL 0x00464500 FOLDED
void Wnd::OnDestroy()
{
}

// 68K 0x10104880 OnSize__4CWndFv
// FUNCTION: LEMBALL 0x00464510 FOLDED
void Wnd::OnSize()
{
}

// 68K 0x10110340 ProcessMessage__4CWndFR11EventRecord
// STUB: LEMBALL 0x00464520
int Wnd::ProcessMessage(EventRecord& p_event)
{
	return 0;
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

// 68K 0x10110d2c __ct__4CWndFv
// STUB: LEMBALL 0x004651d0
Wnd::Wnd()
{
}

// 68K 0x10110e74 Create__4CWndFRC7CVSRectP6CPVWndPc
// STUB: LEMBALL 0x00465200
void Wnd::Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title)
{
}

// STUB: LEMBALL 0x004654f0
unsigned int ConvertWindowStyleFlags(unsigned int p_style)
{
	unsigned int style = 0;
	if (p_style & 8) {
		style = 0xcb0000;
	}
	if (p_style & 2) {
		style |= 0x80000000;
	}
	else {
		style |= 0xc00000;
	}
	style |= (p_style & 0x400) << 8;
	style |= (p_style & 0x40) << 10;
	style |= (p_style & 0x20) << 15;
	style |= (p_style & 0x10) << 17;
	style |= (p_style & 0x80) << 10;
	style |= (p_style & 0x100) << 11;
	return style;
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
