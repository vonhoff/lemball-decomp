#include "PvWnd.h"

#include "HotAreaList.h"

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
// FUNCTION: LEMBALL 0x004323d0
void PvWnd::AddChild(class PvWnd* p_child)
{
	void** node;

	node = (void**) operator new(0xc);
	if (node != 0) {
		node[0] = p_child;
		node[1] = 0;
		node[2] = 0;
	}
	else {
		node = 0;
	}
	node[2] = m_childListTail;
	if (m_childListTail != 0) {
		((void**) m_childListTail)[1] = node;
	}
	m_childListTail = node;
	if (m_childList == 0) {
		m_childList = node;
	}
	m_childCount++;
}

// 68K 0x10104aaa RemoveChild__6CPVWndFP6CPVWnd
// FUNCTION: LEMBALL 0x00432430
void PvWnd::RemoveChild(class PvWnd* p_child)
{
	void** node;
	void** nextNode;
	void** prevNode;

	node = (void**) m_childList;
	if (node != 0) {
		do {
			if ((PvWnd*) node[0] == p_child) {
				break;
			}
			node = (void**) node[1];
		} while (node != 0);
		if (node != 0) {
			nextNode = (void**) node[1];
			prevNode = (void**) node[2];
			operator delete(node);
			if (nextNode != 0) {
				nextNode[2] = prevNode;
			}
			else {
				m_childListTail = prevNode;
			}
			if (prevNode != 0) {
				prevNode[1] = nextNode;
				m_childCount = m_childCount - 1;
				return;
			}
			m_childList = nextNode;
			m_childCount = m_childCount - 1;
		}
	}
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
// FUNCTION: LEMBALL 0x00465db0
void PvWnd::SetInnerWindow(const VsRect& p_rect)
{
	m_innerRect = p_rect;
	OnSize();
}

// 68K 0x10216b5e SetRect__6CPVWndFRC7CVSRect
// STUB: LEMBALL 0x00465df0
void PvWnd::SetRect(const VsRect& p_rect)
{
}

// 68K 0x10216b98 SetRectInnerZoom__6CPVWndFRC7CVSRectRC7CVSRecti
// FUNCTION: LEMBALL 0x00465e00
void PvWnd::SetRectInnerZoom(const VsRect& p_rect, const VsRect& p_innerRect, int p_zoom)
{
	int oldZoom = m_zoom;
	if (p_zoom != oldZoom) {
		m_zoom = p_zoom;
		OnZoom(oldZoom);
		OnZoomBox();
	}
	m_innerRect = p_innerRect;
	SetRect(p_rect);
}

// 68K 0x10216c50 InitHotAreaList__6CPVWndFv
// FUNCTION: LEMBALL 0x00465e60
unsigned int PvWnd::InitHotAreaList()
{
	VsRect listRect;
	VsPoint offset;
	VsPoint innerPoint;
	void* storage;
	unsigned int style;

	style = GetStyle();
	if ((style & 0x800) != 0 && m_hotAreaList == 0) {
		listRect.m_width = m_innerRect.m_width;
		listRect.m_height = m_innerRect.m_height;
		if ((int) listRect.m_width * (int) listRect.m_height == 0) {
			listRect.m_width = m_rect.m_width;
			listRect.m_height = m_rect.m_height;
			listRect.m_x = m_rect.m_x;
			listRect.m_y = m_rect.m_y;
		}
		else {
			listRect.m_x = (short) (m_innerRect.m_x + m_rect.m_x);
			listRect.m_y = (short) (m_innerRect.m_y + m_rect.m_y);
		}
		offset.m_x = m_relativeTopLeft.m_x;
		offset.m_y = m_relativeTopLeft.m_y;
		if (m_parent == 0) {
			offset.m_x = 0;
			offset.m_y = 0;
		}
		innerPoint.m_x = m_innerRect.m_x;
		innerPoint.m_y = m_innerRect.m_y;
		storage = operator new(0x60);
		if (storage == 0) {
			m_hotAreaList = 0;
		}
		else {
			m_hotAreaList = new (storage) HotAreaList(listRect, offset, innerPoint);
		}
	}
	return 0;
}

// 68K 0x10216d90 _OnCreate__6CPVWndFv
// FUNCTION: LEMBALL 0x00465f80
void PvWnd::OnCreate()
{
	void** list;
	void** node;

	g_nNativeWindowCount = g_nNativeWindowCount + 1;
	if (m_parent == 0) {
		list = (void**) g_pWindowOwnerList;
		node = (void**) operator new(0xc);
		if (node != 0) {
			node[0] = this;
			node[1] = 0;
			node[2] = 0;
			if (list != 0) {
				node[2] = list[1];
				if (list[1] != 0) {
					((void**) list[1])[1] = node;
				}
				list[1] = node;
				if (list[0] == 0) {
					list[0] = node;
				}
				*(int*) &list[2] = *(int*) &list[2] + 1;
			}
		}
	}
	m_lifecycleRefs = m_lifecycleRefs + 1;
}

// 68K 0x10216e16 _OnDestroy__6CPVWndFv
// FUNCTION: LEMBALL 0x00465fe0
void PvWnd::BaseOnDestroy()
{
	void** ownerList;
	void** node;
	void** nextNode;
	void** prevNode;

	g_nNativeWindowCount = g_nNativeWindowCount - 1;
	m_lifecycleRefs = m_lifecycleRefs - 1;
	if (m_hotAreaList != 0) {
		delete m_hotAreaList;
		m_hotAreaList = 0;
	}
	if (m_parent != 0) {
		m_parent->RemoveChild(this);
		return;
	}
	ownerList = (void**) g_pWindowOwnerList;
	node = (void**) ownerList[0];
	if (node != 0) {
		do {
			if ((PvWnd*) node[0] == this) {
				break;
			}
			node = (void**) node[1];
		} while (node != 0);
		if (node != 0) {
			nextNode = (void**) node[1];
			prevNode = (void**) node[2];
			operator delete(node);
			if (nextNode != 0) {
				nextNode[2] = prevNode;
			}
			else {
				ownerList[1] = prevNode;
			}
			if (prevNode != 0) {
				prevNode[1] = nextNode;
				*(int*) &ownerList[2] = *(int*) &ownerList[2] - 1;
				return;
			}
			ownerList[0] = nextNode;
			*(int*) &ownerList[2] = *(int*) &ownerList[2] - 1;
		}
	}
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
// FUNCTION: LEMBALL 0x004662b0
void PvWnd::SetZoom(int p_zoom)
{
	int oldZoom = m_zoom;
	if (p_zoom != oldZoom) {
		m_zoom = p_zoom;
		OnZoom(oldZoom);
		OnZoomBox();
		OnSize();
		BaseOnSize();
	}
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

void PvWnd::Create(const VsRect& p_rect, PvWnd* p_parent, char* p_title) {}
void PvWnd::Move(const VsPoint& p_point) {}
void PvWnd::Dummy3c() {}
void PvWnd::OnDestroy() {}
void PvWnd::BaseOnSize() {}
void PvWnd::Dummy48() {}
void PvWnd::OnMinimise() {}
void PvWnd::OnMaximise() {}
void PvWnd::OnRestore() {}
void PvWnd::OnZoomBox() {}
unsigned int PvWnd::GetStyle()
{
	return 0;
}
void PvWnd::Refresh(VsRect* p_rect) {}
void PvWnd::Destroy() {}
