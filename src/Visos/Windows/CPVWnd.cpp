#include "CPVWnd.h"

#include "Visos/Controls/CHotAreaList.h"
#include "Visos/Math/CVSPoint.h"
#include "Visos/Math/CVSRect.h"
#include "Visos/Math/CVSSize.h"

#include <stddef.h>

struct MenuList;

extern int g_cursorState;
extern int g_nNativeWindowCount;

// FUNCTION: LEMBALL 0x004323b0
unsigned int CPVWnd::GetSizeStatus()
{
	return m_sizeStatus;
}

// FUNCTION: LEMBALL 0x004323c0
void CPVWnd::SetSizeStatus(unsigned int p_status)
{
	m_sizeStatus = p_status;
}

// FUNCTION: LEMBALL 0x004323d0
void CPVWnd::AddChild(class CPVWnd* p_child)
{
	void** node;

	node = (void**) operator new(0xc);
	if (node != NULL) {
		node[0] = p_child;
		node[1] = NULL;
		node[2] = NULL;
	}
	else {
		node = NULL;
	}
	node[2] = m_childListTail;
	if (m_childListTail != NULL) {
		((void**) m_childListTail)[1] = node;
	}
	m_childListTail = node;
	if (m_childList == NULL) {
		m_childList = node;
	}
	m_childCount++;
}

// FUNCTION: LEMBALL 0x00432430
void CPVWnd::RemoveChild(class CPVWnd* p_child)
{
	void** node;
	void** nextNode;
	void** prevNode;

	node = (void**) m_childList;
	if (node != NULL) {
		do {
			if ((CPVWnd*) node[0] == p_child) {
				break;
			}
			node = (void**) node[1];
		} while (node != NULL);
		if (node != NULL) {
			nextNode = (void**) node[1];
			prevNode = (void**) node[2];
			operator delete(node);
			if (nextNode != NULL) {
				nextNode[2] = prevNode;
			}
			else {
				m_childListTail = prevNode;
			}
			if (prevNode != NULL) {
				prevNode[1] = nextNode;
				m_childCount = m_childCount - 1;
				return;
			}
			m_childList = nextNode;
			m_childCount = m_childCount - 1;
		}
	}
}

// FUNCTION: LEMBALL 0x0043a4c0
bool CPVWnd::GetMenu(int& p_menuResourceId, MenuList*** p_menuLists)
{
	return false;
}

// FUNCTION: LEMBALL 0x00465a70
void CPVWnd::OnVisibilityChange()
{
}

// FUNCTION: LEMBALL 0x00465a80
void CPVWnd::SetDontUpdateRect(const CVSRect& p_rect)
{
}

// FUNCTION: LEMBALL 0x00465cc0
CPVWnd::CPVWnd()
{
	int previous;

	m_childList = NULL;
	m_childListTail = NULL;
	m_childCount = 0;
	m_relativeTopLeft.m_y = 0;
	m_relativeTopLeft.m_x = 0;
	m_hotAreaList = NULL;
	m_active = 1;
	previous = g_cursorState;
	g_cursorState = g_cursorState + 1;
	if (previous == 0) {
		WindowOwnerList* list = (WindowOwnerList*) operator new(sizeof(WindowOwnerList));
		if (list != NULL) {
			list->m_head = NULL;
			list->m_tail = NULL;
			list->m_count = 0;
			g_pWindowOwnerList = list;
		}
		else {
			g_pWindowOwnerList = NULL;
		}
	}
	m_zoom = 1;
	m_lifecycleRefs = 0;
}

// FUNCTION: LEMBALL 0x00465d50
CPVWnd::~CPVWnd()
{
	if (--g_cursorState == 0) {
		WindowOwnerList* list = g_pWindowOwnerList;
		if (list != NULL) {
			WindowOwnerNode* node = list->m_head;
			for (;;) {
				if (node == NULL) {
					break;
				}
				WindowOwnerNode* next = node->m_next;
				operator delete(node);
				node = next;
			}
			operator delete(list);
		}
	}
	void** child = (void**) m_childList;
	while (child != NULL) {
		void** next = (void**) child[1];
		operator delete(child);
		child = next;
	}
}

// FUNCTION: LEMBALL 0x00465db0
void CPVWnd::SetInnerWindow(const CVSRect& p_rect)
{
	const short* position;

	m_innerRect.m_width = p_rect.m_width;
	m_innerRect.m_height = p_rect.m_height;
	if (&p_rect != NULL) {
		position = &p_rect.m_x;
	}
	else {
		position = NULL;
	}
	m_innerRect.m_x = *position;
	m_innerRect.m_y = position[1];
	_OnSize();
}

// FUNCTION: LEMBALL 0x00465df0
void CPVWnd::SetRect(const CVSRect& p_rect)
{
	_SetRect(p_rect);
}

// FUNCTION: LEMBALL 0x00465e00
void CPVWnd::SetRectInnerZoom(const CVSRect& p_rect, const CVSRect& p_innerRect, int p_zoom)
{
	int oldZoom = m_zoom;
	const CVSPoint* innerXY;
	if (p_zoom != oldZoom) {
		m_zoom = p_zoom;
		_OnZoom(oldZoom);
		OnZoom(oldZoom);
	}
	m_innerRect.m_width = p_innerRect.m_width;
	m_innerRect.m_height = p_innerRect.m_height;
	innerXY = (const CVSPoint*) &p_innerRect;
	m_innerRect.m_x = innerXY->m_x;
	m_innerRect.m_y = innerXY->m_y;
	_SetRect(p_rect);
}

#define CPVWND_INIT_HOT_AREA_LIST_STYLE 0x800
// FUNCTION: LEMBALL 0x00465e60
void CPVWnd::InitHotAreaList()
{
	unsigned int style;

	style = GetStyle();
	if ((style & CPVWND_INIT_HOT_AREA_LIST_STYLE) != 0 && m_hotAreaList == NULL) {
		CVSRect listRect;
		if ((int) m_innerRect.m_width * (int) m_innerRect.m_height != 0) {
			listRect.m_width = m_innerRect.m_width;
			listRect.m_height = m_innerRect.m_height;
			CVSPoint* innerPoint = static_cast<CVSPoint*>(&m_innerRect);
			listRect.m_x = innerPoint->m_x;
			listRect.m_y = innerPoint->m_y;
			CVSPoint* rectPoint = static_cast<CVSPoint*>(&m_rect);
			short y = rectPoint->m_y;
			listRect.m_x += rectPoint->m_x;
			listRect.m_y += y;
		}
		else {
			listRect.m_width = m_rect.m_width;
			listRect.m_height = m_rect.m_height;
			CVSPoint* rectPoint = static_cast<CVSPoint*>(&m_rect);
			listRect.m_x = rectPoint->m_x;
			listRect.m_y = rectPoint->m_y;
		}
		CVSPoint offset(m_relativeTopLeft.m_x, m_relativeTopLeft.m_y);
		if (m_parent == NULL) {
			offset.m_x = 0;
			offset.m_y = 0;
		}
		m_hotAreaList = new CHotAreaList(listRect, offset, *static_cast<CVSPoint*>(&m_innerRect));
	}
}
#undef CPVWND_INIT_HOT_AREA_LIST_STYLE

// FUNCTION: LEMBALL 0x00465f80
void CPVWnd::_OnCreate()
{
	g_nNativeWindowCount++;
	if (m_parent == NULL) {
		WindowOwnerList* list = g_pWindowOwnerList;
		WindowOwnerNode* node = (WindowOwnerNode*) operator new(sizeof(WindowOwnerNode));
		if (node != NULL) {
			node->m_window = this;
			node->m_next = NULL;
			node->m_prev = NULL;
		}
		else {
			node = NULL;
		}
		node->m_prev = list->m_tail;
		if (list->m_tail != NULL) {
			list->m_tail->m_next = node;
		}
		list->m_tail = node;
		if (list->m_head == NULL) {
			list->m_head = node;
		}
		list->m_count++;
	}
	m_lifecycleRefs++;
}

// FUNCTION: LEMBALL 0x00465fe0
void CPVWnd::_OnDestroy()
{
	WindowOwnerList* ownerList;
	WindowOwnerNode* node;
	WindowOwnerNode* nextNode;
	WindowOwnerNode* prevNode;

	g_nNativeWindowCount = g_nNativeWindowCount - 1;
	m_lifecycleRefs = m_lifecycleRefs - 1;
	if (m_hotAreaList != NULL) {
		delete m_hotAreaList;
		m_hotAreaList = NULL;
	}
	if (m_parent != NULL) {
		m_parent->RemoveChild(this);
		return;
	}
	ownerList = g_pWindowOwnerList;
	node = ownerList->m_head;
	if (node != NULL) {
		do {
			if (node->m_window == this) {
				break;
			}
			node = node->m_next;
		} while (node != NULL);
		if (node != NULL) {
			nextNode = node->m_next;
			prevNode = node->m_prev;
			operator delete(node);
			if (nextNode != NULL) {
				nextNode->m_prev = prevNode;
			}
			else {
				ownerList->m_tail = prevNode;
			}
			if (prevNode != NULL) {
				prevNode->m_next = nextNode;
				ownerList->m_count = ownerList->m_count - 1;
				return;
			}
			ownerList->m_head = nextNode;
			ownerList->m_count = ownerList->m_count - 1;
		}
	}
}

// FUNCTION: LEMBALL 0x00466060
void CPVWnd::_OnSize()
{
	CHotAreaList* list;
	CVSRect area;
	CVSPoint* innerXY;
	CVSPoint* rectXY;

	list = m_hotAreaList;
	if (list == NULL) {
		return;
	}
	area.m_y = 0;
	area.m_x = 0;
	if ((int) m_innerRect.m_width * (int) m_innerRect.m_height != 0) {
		area.m_width = m_innerRect.m_width;
		area.m_height = m_innerRect.m_height;
		innerXY = &m_innerRect;
		area.m_x = innerXY->m_x;
		area.m_y = innerXY->m_y;
		rectXY = &m_rect;
		short y = rectXY->m_y;
		area.m_x = (short) (area.m_x + rectXY->m_x);
		area.m_y = (short) (area.m_y + y);
	}
	else {
		area.m_width = m_rect.m_width;
		area.m_height = m_rect.m_height;
		rectXY = &m_rect;
		area.m_x = rectXY->m_x;
		area.m_y = rectXY->m_y;
	}
	CVSPoint origin(m_relativeTopLeft.m_x, m_relativeTopLeft.m_y);
	if (m_parent == NULL) {
		origin.m_x = 0;
		origin.m_y = 0;
	}
	innerXY = &m_innerRect;
	list->Set(area, origin, *innerXY);
}

// FUNCTION: LEMBALL 0x00466160
void CPVWnd::_OnMove()
{
	CHotAreaList* list;
	CVSPoint* innerXY;
	CVSPoint* rectXY;

	list = m_hotAreaList;
	if (list == NULL) {
		return;
	}
	CVSRect area;
	if ((int) m_innerRect.m_height * (int) m_innerRect.m_width != 0) {
		area.m_width = m_innerRect.m_width;
		area.m_height = m_innerRect.m_height;
		innerXY = &m_innerRect;
		area.m_x = innerXY->m_x;
		area.m_y = innerXY->m_y;
		rectXY = &m_rect;
		short y = rectXY->m_y;
		short x = rectXY->m_x;
		area.m_x = (short) (area.m_x + x);
		area.m_y = (short) (area.m_y + y);
	}
	else {
		area.m_width = m_rect.m_width;
		area.m_height = m_rect.m_height;
		rectXY = &m_rect;
		area.m_x = rectXY->m_x;
		area.m_y = rectXY->m_y;
	}
	CVSPoint origin(m_relativeTopLeft);
	if (m_parent == NULL) {
		origin.m_x = 0;
		origin.m_y = 0;
	}
	innerXY = &m_innerRect;
	list->Set(area, origin, *innerXY);
}

// FUNCTION: LEMBALL 0x00466260
void CPVWnd::_OnMove(CVSPoint p_point)
{
	m_rect.m_x = (short) (m_rect.m_x + p_point.m_x);
	m_rect.m_y = (short) (m_rect.m_y + p_point.m_y);
	_OnMove();
}

// FUNCTION: LEMBALL 0x00466280
void CPVWnd::_OnZoom(int p_oldZoom)
{
	if (m_hotAreaList != NULL) {
		m_hotAreaList->m_scale = m_zoom;
	}
	WindowOwnerNode* child = (WindowOwnerNode*) m_childList;
	while (child != NULL) {
		child->m_window->SetZoom(m_zoom);
		child = child->m_next;
	}
}

// FUNCTION: LEMBALL 0x004662b0
void CPVWnd::SetZoom(int p_zoom)
{
	int oldZoom = m_zoom;
	if (p_zoom != oldZoom) {
		m_zoom = p_zoom;
		_OnZoom(oldZoom);
		OnZoom(oldZoom);
		_OnSize();
		OnSize();
	}
}

// FUNCTION: LEMBALL 0x004662e0
void CPVWnd::ReSetMenu()
{
}

// FUNCTION: LEMBALL 0x004662f0
void CPVWnd::SetMenu(int& p_menuResourceId, MenuList** p_menuLists)
{
}

// FUNCTION: LEMBALL 0x00466300
void CPVWnd::_SetRect(const CVSRect& p_rect)
{
	const short* position;

	m_rect.m_width = p_rect.m_width;
	m_rect.m_height = p_rect.m_height;
	if (&p_rect != NULL) {
		position = &p_rect.m_x;
	}
	else {
		position = NULL;
	}
	m_rect.m_x = *position;
	m_rect.m_y = position[1];
}

// FUNCTION: LEMBALL 0x00466330
void CPVWnd::_SetRelTL(const CVSPoint& p_point)
{
}

// FUNCTION: LEMBALL 0x00466340
void CPVWnd::OnDriverChange()
{
}

// FUNCTION: LEMBALL 0x00466350
bool CPVWnd::IsFocusWindow()
{
	return true;
}

// FUNCTION: LEMBALL 0x00466360
void CPVWnd::Resize(CVSSize p_size)
{
}

void CPVWnd::Create(const CVSRect& p_rect, CPVWnd* p_parent, char* p_title)
{
}
void CPVWnd::Move(const CVSPoint& p_point)
{
}
void CPVWnd::OnCreate()
{
}
void CPVWnd::OnDestroy()
{
}
void CPVWnd::OnSize()
{
}
void CPVWnd::OnMove()
{
}
void CPVWnd::OnMinimise()
{
}
void CPVWnd::OnMaximise()
{
}
void CPVWnd::OnRestore()
{
}
unsigned int CPVWnd::GetStyle()
{
	return 0;
}
void CPVWnd::Refresh(CVSRect* p_rect)
{
}
void CPVWnd::Destroy()
{
}
