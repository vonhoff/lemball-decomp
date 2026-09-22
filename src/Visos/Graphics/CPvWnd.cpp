#include "CPvWnd.h"

#include "CHotAreaList.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/CVsSize.h"

struct MenuList;

extern int g_cursorState;
extern int g_nNativeWindowCount;

// FUNCTION: LEMBALL 0x004323b0
unsigned int CPvWnd::GetSizeStatus()
{
	return m_sizeStatus;
}

// FUNCTION: LEMBALL 0x004323c0
void CPvWnd::SetSizeStatus(unsigned int p_status)
{
	m_sizeStatus = p_status;
}

// FUNCTION: LEMBALL 0x004323d0
void CPvWnd::AddChild(class CPvWnd* p_child)
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

// FUNCTION: LEMBALL 0x00432430
void CPvWnd::RemoveChild(class CPvWnd* p_child)
{
	void** node;
	void** nextNode;
	void** prevNode;

	node = (void**) m_childList;
	if (node != 0) {
		do {
			if ((CPvWnd*) node[0] == p_child) {
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

// FUNCTION: LEMBALL 0x0043a4c0
bool CPvWnd::GetMenu(int& p_menuResourceId, MenuList*** p_menuLists)
{
	return 0;
}

// FUNCTION: LEMBALL 0x00465a70
void CPvWnd::OnVisibilityChange()
{
}

// FUNCTION: LEMBALL 0x00465a80
void CPvWnd::SetDontUpdateRect(const CVsRect& p_rect)
{
}

// FUNCTION: LEMBALL 0x00465cc0
CPvWnd::CPvWnd()
{
	int previous;

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
		WindowOwnerList* list = (WindowOwnerList*) operator new(sizeof(WindowOwnerList));
		if (list != 0) {
			list->m_head = 0;
			list->m_tail = 0;
			list->m_count = 0;
			g_pWindowOwnerList = list;
		}
		else {
			g_pWindowOwnerList = 0;
		}
	}
	m_zoom = 1;
	m_lifecycleRefs = 0;
}

// FUNCTION: LEMBALL 0x00465d50
CPvWnd::~CPvWnd()
{
	if (--g_cursorState == 0) {
		WindowOwnerList* list = g_pWindowOwnerList;
		if (list != 0) {
			WindowOwnerNode* node = list->m_head;
			for (;;) {
				if (node == 0) {
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
	while (child != 0) {
		void** next = (void**) child[1];
		operator delete(child);
		child = next;
	}
}

// FUNCTION: LEMBALL 0x00465db0
void CPvWnd::SetInnerWindow(const CVsRect& p_rect)
{
	const short* position;

	m_innerRect.m_width = p_rect.m_width;
	m_innerRect.m_height = p_rect.m_height;
	if (&p_rect != 0) {
		position = &p_rect.m_x;
	}
	else {
		position = 0;
	}
	m_innerRect.m_x = *position;
	m_innerRect.m_y = position[1];
	InternalOnSize();
}

// FUNCTION: LEMBALL 0x00465df0
void CPvWnd::SetRect(const CVsRect& p_rect)
{
	InternalSetRect(p_rect);
}

// FUNCTION: LEMBALL 0x00465e00
void CPvWnd::SetRectInnerZoom(const CVsRect& p_rect, const CVsRect& p_innerRect, int p_zoom)
{
	int oldZoom = m_zoom;
	const CVsPoint* innerXY;
	if (p_zoom != oldZoom) {
		m_zoom = p_zoom;
		InternalOnZoom(oldZoom);
		OnZoom(oldZoom);
	}
	m_innerRect.m_width = p_innerRect.m_width;
	m_innerRect.m_height = p_innerRect.m_height;
	innerXY = (const CVsPoint*) &p_innerRect;
	m_innerRect.m_x = innerXY->m_x;
	m_innerRect.m_y = innerXY->m_y;
	InternalSetRect(p_rect);
}

// FUNCTION: LEMBALL 0x00465e60
void CPvWnd::InitHotAreaList()
{
	unsigned int style;

	style = GetStyle();
	if ((style & 0x800) != 0 && m_hotAreaList == 0) {
		CVsRect listRect;
		if ((int) m_innerRect.m_width * (int) m_innerRect.m_height != 0) {
			listRect.m_width = m_innerRect.m_width;
			listRect.m_height = m_innerRect.m_height;
			CVsPoint* innerPoint = static_cast<CVsPoint*>(&m_innerRect);
			listRect.m_x = innerPoint->m_x;
			listRect.m_y = innerPoint->m_y;
			CVsPoint* rectPoint = static_cast<CVsPoint*>(&m_rect);
			short y = rectPoint->m_y;
			listRect.m_x += rectPoint->m_x;
			listRect.m_y += y;
		}
		else {
			listRect.m_width = m_rect.m_width;
			listRect.m_height = m_rect.m_height;
			CVsPoint* rectPoint = static_cast<CVsPoint*>(&m_rect);
			listRect.m_x = rectPoint->m_x;
			listRect.m_y = rectPoint->m_y;
		}
		CVsPoint offset(m_relativeTopLeft.m_x, m_relativeTopLeft.m_y);
		if (m_parent == 0) {
			offset.m_x = 0;
			offset.m_y = 0;
		}
		m_hotAreaList = new CHotAreaList(listRect, offset, *static_cast<CVsPoint*>(&m_innerRect));
	}
}

// FUNCTION: LEMBALL 0x00465f80
void CPvWnd::InternalOnCreate()
{
	WindowOwnerList* list;
	WindowOwnerNode* node;

	g_nNativeWindowCount = g_nNativeWindowCount + 1;
	if (m_parent == 0) {
		list = g_pWindowOwnerList;
		node = (WindowOwnerNode*) operator new(sizeof(WindowOwnerNode));
		if (node != 0) {
			node->m_window = this;
			node->m_next = 0;
			node->m_prev = 0;
		}
		else {
			node = 0;
		}
		node->m_prev = list->m_tail;
		if (list->m_tail != 0) {
			list->m_tail->m_next = node;
		}
		list->m_tail = node;
		if (list->m_head == 0) {
			list->m_head = node;
		}
		list->m_count = list->m_count + 1;
	}
	m_lifecycleRefs = m_lifecycleRefs + 1;
}

// FUNCTION: LEMBALL 0x00465fe0
void CPvWnd::InternalOnDestroy()
{
	WindowOwnerList* ownerList;
	WindowOwnerNode* node;
	WindowOwnerNode* nextNode;
	WindowOwnerNode* prevNode;

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
	ownerList = g_pWindowOwnerList;
	node = ownerList->m_head;
	if (node != 0) {
		do {
			if (node->m_window == this) {
				break;
			}
			node = node->m_next;
		} while (node != 0);
		if (node != 0) {
			nextNode = node->m_next;
			prevNode = node->m_prev;
			operator delete(node);
			if (nextNode != 0) {
				nextNode->m_prev = prevNode;
			}
			else {
				ownerList->m_tail = prevNode;
			}
			if (prevNode != 0) {
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
void CPvWnd::InternalOnSize()
{
	CHotAreaList* list;
	CVsRect area;
	CVsPoint* innerXY;
	CVsPoint* rectXY;

	list = m_hotAreaList;
	if (list == 0) {
		return;
	}
	area.m_y = 0;
	area.m_x = 0;
	if ((int) m_innerRect.m_width * (int) m_innerRect.m_height != 0) {
		area.m_width = m_innerRect.m_width;
		area.m_height = m_innerRect.m_height;
		if (this != (CPvWnd*) -16) {
			innerXY = (CVsPoint*) &m_innerRect.m_x;
		}
		else {
			innerXY = 0;
		}
		area.m_x = innerXY->m_x;
		area.m_y = innerXY->m_y;
		if (this != (CPvWnd*) -8) {
			rectXY = (CVsPoint*) &m_rect.m_x;
		}
		else {
			rectXY = 0;
		}
		short y = rectXY->m_y;
		area.m_x = (short) (area.m_x + rectXY->m_x);
		area.m_y = (short) (area.m_y + y);
	}
	else {
		area.m_width = m_rect.m_width;
		area.m_height = m_rect.m_height;
		if (this != (CPvWnd*) -8) {
			rectXY = (CVsPoint*) &m_rect.m_x;
		}
		else {
			rectXY = 0;
		}
		area.m_x = rectXY->m_x;
		area.m_y = rectXY->m_y;
	}
	CVsPoint origin(m_relativeTopLeft.m_x, m_relativeTopLeft.m_y);
	if (m_parent == 0) {
		origin.m_x = 0;
		origin.m_y = 0;
	}
	if (this != (CPvWnd*) -16) {
		innerXY = (CVsPoint*) &m_innerRect.m_x;
	}
	else {
		innerXY = 0;
	}
	list->Set(area, origin, *innerXY);
}

// FUNCTION: LEMBALL 0x00466160
void CPvWnd::InternalOnMove()
{
	CHotAreaList* list;
	CVsPoint* innerXY;
	CVsPoint* rectXY;

	list = m_hotAreaList;
	if (list == 0) {
		return;
	}
	CVsRect area;
	if ((int) m_innerRect.m_height * (int) m_innerRect.m_width != 0) {
		area.m_width = m_innerRect.m_width;
		area.m_height = m_innerRect.m_height;
		if (this != (CPvWnd*) -16) {
			innerXY = (CVsPoint*) &m_innerRect.m_x;
		}
		else {
			innerXY = 0;
		}
		area.m_x = innerXY->m_x;
		area.m_y = innerXY->m_y;
		if (this != (CPvWnd*) -8) {
			rectXY = (CVsPoint*) &m_rect.m_x;
		}
		else {
			rectXY = 0;
		}
		short y = rectXY->m_y;
		short x = rectXY->m_x;
		area.m_x = (short) (area.m_x + x);
		area.m_y = (short) (area.m_y + y);
	}
	else {
		area.m_width = m_rect.m_width;
		area.m_height = m_rect.m_height;
		if (this != (CPvWnd*) -8) {
			rectXY = (CVsPoint*) &m_rect.m_x;
		}
		else {
			rectXY = 0;
		}
		area.m_x = rectXY->m_x;
		area.m_y = rectXY->m_y;
	}
	CVsPoint origin(m_relativeTopLeft);
	if (m_parent == 0) {
		origin.m_x = 0;
		origin.m_y = 0;
	}
	if (this != (CPvWnd*) -16) {
		innerXY = (CVsPoint*) &m_innerRect.m_x;
	}
	else {
		innerXY = 0;
	}
	list->Set(area, origin, *innerXY);
}

// FUNCTION: LEMBALL 0x00466260
void CPvWnd::InternalOnMove(CVsPoint p_point)
{
	m_rect.m_x = (short) (m_rect.m_x + p_point.m_x);
	m_rect.m_y = (short) (m_rect.m_y + p_point.m_y);
	InternalOnMove();
}

// FUNCTION: LEMBALL 0x00466280
void CPvWnd::InternalOnZoom(int p_oldZoom)
{
	if (m_hotAreaList != 0) {
		m_hotAreaList->m_scale = m_zoom;
	}
	WindowOwnerNode* child = (WindowOwnerNode*) m_childList;
	while (child != 0) {
		child->m_window->SetZoom(m_zoom);
		child = child->m_next;
	}
}

// FUNCTION: LEMBALL 0x004662b0
void CPvWnd::SetZoom(int p_zoom)
{
	int oldZoom = m_zoom;
	if (p_zoom != oldZoom) {
		m_zoom = p_zoom;
		InternalOnZoom(oldZoom);
		OnZoom(oldZoom);
		InternalOnSize();
		OnSize();
	}
}

// FUNCTION: LEMBALL 0x004662e0
void CPvWnd::ReSetMenu()
{
}

// FUNCTION: LEMBALL 0x004662f0
void CPvWnd::SetMenu(int& p_menuResourceId, MenuList** p_menuLists)
{
}

// FUNCTION: LEMBALL 0x00466300
void CPvWnd::InternalSetRect(const CVsRect& p_rect)
{
	const short* position;

	m_rect.m_width = p_rect.m_width;
	m_rect.m_height = p_rect.m_height;
	if (&p_rect != 0) {
		position = &p_rect.m_x;
	}
	else {
		position = 0;
	}
	m_rect.m_x = *position;
	m_rect.m_y = position[1];
}

// FUNCTION: LEMBALL 0x00466330
void CPvWnd::InternalSetRelTL(const CVsPoint& p_point)
{
}

// FUNCTION: LEMBALL 0x00466340
void CPvWnd::OnDriverChange()
{
}

// FUNCTION: LEMBALL 0x00466350
bool CPvWnd::IsFocusWindow()
{
	return 1;
}

// FUNCTION: LEMBALL 0x00466360
void CPvWnd::Resize(CVsSize p_size)
{
}

void CPvWnd::Create(const CVsRect& p_rect, CPvWnd* p_parent, char* p_title)
{
}
void CPvWnd::Move(const CVsPoint& p_point)
{
}
void CPvWnd::OnCreate()
{
}
void CPvWnd::OnDestroy()
{
}
void CPvWnd::OnSize()
{
}
void CPvWnd::OnMove()
{
}
void CPvWnd::OnMinimise()
{
}
void CPvWnd::OnMaximise()
{
}
void CPvWnd::OnRestore()
{
}
unsigned int CPvWnd::GetStyle()
{
	return 0;
}
void CPvWnd::Refresh(CVsRect* p_rect)
{
}
void CPvWnd::Destroy()
{
}
