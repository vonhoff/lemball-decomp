#include "CPVWnd.h"

#include <stddef.h>

#include "Engine/Input/CHotAreaList.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Math/CVSSize.h"
#include "Engine/Containers/CList.h"

struct tagMenuList;

extern int g_windowInstanceCount;
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
void CPVWnd::AddChild(CPVWnd* p_child)
{
	m_childList.Add(p_child);
}

// FUNCTION: LEMBALL 0x00432430
void CPVWnd::RemoveChild(CPVWnd* p_child)
{
	m_childList.Remove(p_child);
}

// FUNCTION: LEMBALL 0x0043a4c0
bool CPVWnd::GetMenu(int& p_menuResourceId, tagMenuList*** p_menuLists)
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
	m_hotAreaList = NULL;
	m_active = true;
	if (g_windowInstanceCount++ == 0) {
		g_pWindowOwnerList = new CList<CPVWnd, CListElement<CPVWnd> >;
	}
	m_zoom = 1;
	m_lifecycleRefs = 0;
}

// FUNCTION: LEMBALL 0x00465d50
CPVWnd::~CPVWnd()
{
	if (--g_windowInstanceCount == 0) {
		delete g_pWindowOwnerList;
	}
}

// FUNCTION: LEMBALL 0x00465db0
void CPVWnd::SetInnerWindow(const CVSRect& p_rect)
{
	m_innerRect.m_width = p_rect.m_width;
	m_innerRect.m_height = p_rect.m_height;
	const CVSPoint& position = p_rect;
	m_innerRect.m_x = position.m_x;
	m_innerRect.m_y = position.m_y;
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
	innerXY = &p_innerRect;
	m_innerRect.m_x = innerXY->m_x;
	m_innerRect.m_y = innerXY->m_y;
	_SetRect(p_rect);
}

// FUNCTION: LEMBALL 0x00465e60
void CPVWnd::InitHotAreaList()
{
	unsigned int style;

	style = GetStyle();
	if ((style & WINDOW_STYLE_HOT_AREA_LIST) != 0 && m_hotAreaList == NULL) {
		CVSRect listRect;
		if (m_innerRect.m_width * m_innerRect.m_height != 0) {
			listRect.m_width = m_innerRect.m_width;
			listRect.m_height = m_innerRect.m_height;
			CVSPoint* innerPoint = &m_innerRect;
			listRect.m_x = innerPoint->m_x;
			listRect.m_y = innerPoint->m_y;
			CVSPoint* rectPoint = &m_rect;
			short y = rectPoint->m_y;
			listRect.m_x += rectPoint->m_x;
			listRect.m_y += y;
		}
		else {
			listRect.m_width = m_rect.m_width;
			listRect.m_height = m_rect.m_height;
			CVSPoint* rectPoint = &m_rect;
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

// FUNCTION: LEMBALL 0x00465f80
void CPVWnd::_OnCreate()
{
	++g_nNativeWindowCount;
	if (m_parent == NULL) {
		g_pWindowOwnerList->Add(this);
	}
	++m_lifecycleRefs;
}

// FUNCTION: LEMBALL 0x00465fe0
void CPVWnd::_OnDestroy()
{
	--g_nNativeWindowCount;
	--m_lifecycleRefs;
	if (m_hotAreaList != NULL) {
		delete m_hotAreaList;
		m_hotAreaList = NULL;
	}
	if (m_parent != NULL) {
		m_parent->RemoveChild(this);
	}
	else {
		g_pWindowOwnerList->Remove(this);
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
	if (m_innerRect.m_width * m_innerRect.m_height != 0) {
		area.m_width = m_innerRect.m_width;
		area.m_height = m_innerRect.m_height;
		innerXY = &m_innerRect;
		area.m_x = innerXY->m_x;
		area.m_y = innerXY->m_y;
		rectXY = &m_rect;
		short y = rectXY->m_y;
		area.m_x = area.m_x + rectXY->m_x;
		area.m_y = area.m_y + y;
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
	if (m_innerRect.m_height * m_innerRect.m_width != 0) {
		area.m_width = m_innerRect.m_width;
		area.m_height = m_innerRect.m_height;
		innerXY = &m_innerRect;
		area.m_x = innerXY->m_x;
		area.m_y = innerXY->m_y;
		rectXY = &m_rect;
		short y = rectXY->m_y;
		short x = rectXY->m_x;
		area.m_x = area.m_x + x;
		area.m_y = area.m_y + y;
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
	m_rect.m_x = m_rect.m_x + p_point.m_x;
	m_rect.m_y = m_rect.m_y + p_point.m_y;
	_OnMove();
}

// FUNCTION: LEMBALL 0x00466280
void CPVWnd::_OnZoom(int p_oldZoom)
{
	if (m_hotAreaList != NULL) {
		m_hotAreaList->m_scale = m_zoom;
	}
	CListElement<CPVWnd>* child = m_childList.m_head;
	while (child != NULL) {
		child->m_object->SetZoom(m_zoom);
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
void CPVWnd::SetMenu(int& p_menuResourceId, tagMenuList** p_menuLists)
{
}

// FUNCTION: LEMBALL 0x00466300
void CPVWnd::_SetRect(const CVSRect& p_rect)
{
	m_rect.m_width = p_rect.m_width;
	m_rect.m_height = p_rect.m_height;
	const CVSPoint& position = p_rect;
	m_rect.m_x = position.m_x;
	m_rect.m_y = position.m_y;
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
