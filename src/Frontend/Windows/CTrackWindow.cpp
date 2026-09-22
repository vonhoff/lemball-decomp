#include "CTrackWindow.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CGdi.h"
#include "../../Visos/Graphics/CHotAreaList.h"
#include "../../Visos/Graphics/VsGdi.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CClipRect.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CHotAreaHandler.h"
#include "Visos/Graphics/CLine.h"
#include "Visos/Graphics/CPvGWnd.h"

// FUNCTION: LEMBALL 0x0044e790
CTrackWindow::CTrackWindow(const CVsRect& p_rect, int p_value, CPvGWnd* p_parent) : CHotAreaHandler(p_rect)
{
	m_reserved128 = 0;
	m_trackWidth = p_rect.m_width;
	m_value = p_value;
	m_trackRect.m_width = p_rect.m_width;
	m_trackRect.m_height = p_rect.m_height;
	const CVsPoint* position = &p_rect;
	m_trackRect.m_x = position->m_x;
	short y = position->m_y;
	m_gdiFlags = m_gdiFlags + 6;
	m_trackRect.m_y = y;
	m_parent = p_parent;
	m_contextId = -1;
	SetActive(1);
	m_externalEnabled = 1;
	m_reserved = 1;
}

// FUNCTION: LEMBALL 0x0044e8c0
CTrackWindow::~CTrackWindow()
{
	if (m_parent->m_lifecycleRefs == 1) {
		m_parent->m_hotAreaList->RemoveFromList(this);
	}
}

// FUNCTION: LEMBALL 0x0044e940
void CTrackWindow::OnCreate()
{
	m_gdi->m_renderTarget->m_flag74 = 1;
}

// FUNCTION: LEMBALL 0x0044e960
void CTrackWindow::Create(const CVsRect& p_rect, CPvWnd* p_parent, char* p_name)
{
	CHotAreaHandler* handler;
	const CVsPoint* position;

	CGWnd::Create(p_rect, p_parent, p_name);
	CHotAreaHandler::m_bounds.m_width = p_rect.m_width;
	CHotAreaHandler::m_bounds.m_height = p_rect.m_height;
	position = &p_rect;
	CHotAreaHandler::m_bounds.m_x = position->m_x;
	CHotAreaHandler::m_bounds.m_y = position->m_y;
	handler = this;
	m_parent->m_hotAreaList->AddToList(handler);
}

// FUNCTION: LEMBALL 0x0044e9d0
void CTrackWindow::Move(const CVsPoint& p_position)
{
	CGWnd::Move(p_position);
	m_trackRect.m_x = p_position.m_x;
	m_trackRect.m_y = p_position.m_y;
}

// FUNCTION: LEMBALL 0x0044ea00
void CTrackWindow::OnPaint(const CVsRect& p_rect)
{
	int height = m_trackRect.m_height;
	int width = (int) m_trackRect.m_width * m_value / 100;
	if (m_value != 0) {
		m_line.m_color = 0xac;
		m_line.m_x1 = width;
		m_line.m_y1 = height;
		m_line.m_x2 = 0;
		m_line.m_y2 = 0;
		m_line.Draw(m_gdi);
		m_clipRects[0].m_left = 0;
		m_clipRects[0].m_top = 0;
		m_clipRects[0].m_right = width;
		m_clipRects[0].m_bottom = 0;
		m_clipRects[0].m_reserved0c = 0xab;
		m_clipRects[0].Draw(m_gdi);
		m_clipRects[1].m_left = 0;
		m_clipRects[1].m_top = 0;
		m_clipRects[1].m_right = 0;
		m_clipRects[1].m_bottom = height;
		m_clipRects[1].m_reserved0c = 0xab;
		m_clipRects[1].Draw(m_gdi);
		m_clipRects[2].m_left = (short) m_value;
		m_clipRects[2].m_top = height;
		m_clipRects[2].m_right = 0;
		m_clipRects[2].m_bottom = height;
		m_clipRects[2].m_reserved0c = 0xbc;
		m_clipRects[2].Draw(m_gdi);
		m_clipRects[3].m_left = (short) m_value;
		m_clipRects[3].m_top = height;
		m_clipRects[3].m_right = width;
		m_clipRects[3].m_bottom = 0;
		m_clipRects[3].m_reserved0c = 0xbc;
		m_clipRects[3].Draw(m_gdi);
	}
}

// FUNCTION: LEMBALL 0x0044eb60
void CTrackWindow::SetButtonValue(int p_value)
{
	if (m_value != p_value) {
		Message message;
		message.type = 12;
		m_value = p_value;
		message.time = CurrentQueueTimer();
		message.code = m_contextId;
		message.payload = (void*) m_value;
		message.source = (void*) 100;
		g_pMasterInputQueue->Post(message);
	}
}

// FUNCTION: LEMBALL 0x0044ebc0
void CTrackWindow::OnInside(const CVsPoint& p_point)
{
	if (m_buttonState[0] != 0) {
		int distance = (int) p_point.m_x - (int) CHotAreaHandler::m_bounds.m_x;
		if (distance < 0) {
			distance = 0;
		}
		else if (distance > m_trackRect.m_width) {
			distance = m_trackRect.m_width;
		}
		SetButtonValue(distance * 100 / (int) CHotAreaHandler::m_bounds.m_width);
	}
}

// FUNCTION: LEMBALL 0x0044ec10
void CTrackWindow::OnButtonDown(const CVsPoint& p_point, int p_flags)
{
	OnInside(p_point);
}

// FUNCTION: LEMBALL 0x0044ec20
void CTrackWindow::OnDriverChange()
{
}

// FUNCTION: LEMBALL 0x0044efe0
unsigned int CTrackWindow::GetStyle()
{
	return 2147485697;
}
