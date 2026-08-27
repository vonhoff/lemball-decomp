#include "PvWnd.h"

#include <new.h>

extern int g_cursorState;
extern void* g_pWindowOwnerList;
extern int g_nNativeWindowCount;

// 68K 0x101049c0 GetSizeStatus__6CPVWndFv
// FUNCTION: LEMBALL 0x004323b0
unsigned int PvWnd::GetSizeStatus()
{
	return m_sizeStatus;
}

// 68K 0x101049ec SetSizeStatus__6CPVWndF13wndSizeStatus
// FUNCTION: LEMBALL 0x004323c0
void PvWnd::SetSizeStatus(unsigned int p_status)
{
	m_sizeStatus = p_status;
}

// 68K 0x10104a28 AddChild__6CPVWndFP6CPVWnd
// STUB: LEMBALL 0x004323d0
void PvWnd::AddChild(class PvWnd* p_child)
{
	void** node;

	node = (void**) operator new(0xc);
	if (node == 0) {
		return;
	}
	node[0] = p_child;
	node[1] = 0;
	node[2] = m_childListTail;
	if (m_childListTail != 0) {
		((void**) m_childListTail)[1] = node;
	}
	m_childListTail = node;
	if (m_childList == 0) {
		m_childList = node;
	}
	m_childCount = m_childCount + 1;
}

// 68K 0x10104aaa RemoveChild__6CPVWndFP6CPVWnd
// STUB: LEMBALL 0x00432430
void PvWnd::RemoveChild(class PvWnd* p_child)
{
}

// 68K 0x1010496a GetMenu__6CPVWndFRiPPP11tagMenuList
// FUNCTION: LEMBALL 0x0043a4c0
bool PvWnd::GetMenu(int& p_menuResourceId, MenuList*** p_menuLists)
{
	return 0;
}

// 68K 0x10104ba0 OnVisibilityChange__6CPVWndFv
// FUNCTION: LEMBALL 0x00465a70
void PvWnd::OnVisibilityChange()
{
}

// 68K 0x10104bc8 SetDontUpdateRect__6CPVWndFRC7CVSRect
// FUNCTION: LEMBALL 0x00465a80
void PvWnd::SetDontUpdateRect(const VsRect& p_rect)
{
}

// 68K 0x10216992 __ct__6CPVWndFv
// STUB: LEMBALL 0x00465cc0
PvWnd::PvWnd()
{
	int previous;

	m_rect.m_height = 0;
	m_rect.m_width = 0;
	m_rect.m_y = 0;
	m_rect.m_x = 0;
	m_innerRect.m_height = 0;
	m_innerRect.m_width = 0;
	m_innerRect.m_y = 0;
	m_innerRect.m_x = 0;
	m_childList = 0;
	m_childListTail = 0;
	m_childCount = 0;
	m_relativeTopLeft.m_y = 0;
	m_relativeTopLeft.m_x = 0;
	m_hotAreaList = 0;
	m_active = 1;
	previous = g_cursorState;
	g_cursorState = g_cursorState + 1;
	if (previous == 0) {
		g_pWindowOwnerList = operator new(0xc);
		if (g_pWindowOwnerList == 0) {
			g_pWindowOwnerList = 0;
		}
		else {
			((void**) g_pWindowOwnerList)[0] = 0;
			((void**) g_pWindowOwnerList)[1] = 0;
			((int*) g_pWindowOwnerList)[2] = 0;
		}
	}
	m_zoom = 1;
	m_lifecycleRefs = 0;
	m_parent = 0;
	m_sizeStatus = 0;
}

// 68K 0x10216a50 __dt__6CPVWndFv
// STUB: LEMBALL 0x00465d50
PvWnd::~PvWnd()
{
}

// 68K 0x10216aee SetInnerWindow__6CPVWndFRC7CVSRect
// STUB: LEMBALL 0x00465db0
void PvWnd::SetInnerWindow(const VsRect& p_rect)
{
}

// 68K 0x10216b5e SetRect__6CPVWndFRC7CVSRect
// STUB: LEMBALL 0x00465df0
void PvWnd::SetRect(const VsRect& p_rect)
{
}

// 68K 0x10216b98 SetRectInnerZoom__6CPVWndFRC7CVSRectRC7CVSRecti
// STUB: LEMBALL 0x00465e00
void PvWnd::SetRectInnerZoom(const VsRect& p_rect, const VsRect& p_innerRect, int p_zoom)
{
}

// 68K 0x10216c50 InitHotAreaList__6CPVWndFv
// STUB: LEMBALL 0x00465e60
unsigned int PvWnd::InitHotAreaList()
{
	return 0;
}

// 68K 0x10216d90 _OnCreate__6CPVWndFv
// STUB: LEMBALL 0x00465f80
void PvWnd::OnCreate()
{
	g_nNativeWindowCount = g_nNativeWindowCount + 1;
	m_lifecycleRefs = m_lifecycleRefs + 1;
}

// 68K 0x10216e16 _OnDestroy__6CPVWndFv
// STUB: LEMBALL 0x00465fe0
void PvWnd::BaseOnDestroy()
{
}

// 68K 0x10216edc _OnSize__6CPVWndFv
// STUB: LEMBALL 0x00466060
void PvWnd::OnSize()
{
}

// 68K 0x10217024 _OnMove__6CPVWndFv
// STUB: LEMBALL 0x00466160
void PvWnd::OnMove()
{
}

// 68K 0x1021716c _OnMove__6CPVWndF8CVSPoint
// STUB: LEMBALL 0x00466260
void PvWnd::BaseOnMove()
{
}

// 68K 0x102171cc _OnZoom__6CPVWndFi
// STUB: LEMBALL 0x00466280
void PvWnd::OnZoom(int p_oldZoom)
{
}

// 68K 0x10217228 SetZoom__6CPVWndFi
// STUB: LEMBALL 0x004662b0
void PvWnd::SetZoom(int p_zoom)
{
	m_zoom = p_zoom;
}

// 68K 0x10117ea6 ReSetMenu__6CPVWndFv
// FUNCTION: LEMBALL 0x004662e0
void PvWnd::ReSetMenu()
{
}

// 68K 0x10117ec6 SetMenu__6CPVWndFRiPP11tagMenuList
// FUNCTION: LEMBALL 0x004662f0
void PvWnd::SetMenu(int& p_menuResourceId, MenuList** p_menuLists)
{
}

// 68K 0x10117ef4 _SetRelTL__6CPVWndFRC8CVSPoint
// FUNCTION: LEMBALL 0x00466330
void PvWnd::SetRelTl(const VsPoint& p_point)
{
}

// 68K 0x1010499c OnDriverChange__6CPVWndFv
// FUNCTION: LEMBALL 0x00466340
void PvWnd::OnDriverChange()
{
}

// 68K 0x10117f1e IsFocusWindow__6CPVWndFv
// FUNCTION: LEMBALL 0x00466350
bool PvWnd::IsFocusWindow()
{
	return 1;
}

// 68K 0x10104bfa Resize__6CPVWndF7CVSSize
// FUNCTION: LEMBALL 0x00466360
void PvWnd::Resize(VsSize p_size)
{
}

void PvWnd::Dummy04() {}
void PvWnd::Dummy38() {}
void PvWnd::Dummy3c() {}
void PvWnd::OnDestroy() {}
void PvWnd::BaseOnSize() {}
void PvWnd::Dummy48() {}
void PvWnd::Dummy4c() {}
void PvWnd::Dummy50() {}
void PvWnd::Dummy54() {}
void PvWnd::Dummy58() {}
void PvWnd::Dummy64() {}
void PvWnd::Dummy70() {}
void PvWnd::Destroy() {}
