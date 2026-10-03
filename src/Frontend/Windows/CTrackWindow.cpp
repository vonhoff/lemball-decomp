#include "CTrackWindow.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/CGDI.h"
#include "../../Visos/Graphics/CHotAreaList.h"
#include "../../Visos/Graphics/CSurface.h"
#include "Visos/Foundation/CVSPoint.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CHotAreaHandler.h"
#include "Visos/Graphics/CLine.h"
#include "Visos/Graphics/CPVGWnd.h"
#include "Visos/Graphics/CSolidRect.h"

// FUNCTION: LEMBALL 0x0044e790
CTrackWindow::CTrackWindow(const CVSRect& p_rect, int p_value, CPVGWnd* p_parent) : CHotAreaHandler(p_rect)
{
	m_reserved128 = 0;
	m_trackWidth = p_rect.m_width;
	m_value = p_value;
	m_trackRect.m_width = p_rect.m_width;
	m_trackRect.m_height = p_rect.m_height;
	const CVSPoint* position = &p_rect;
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

// FUNCTION: LEMBALL 0x0044e940
void CTrackWindow::OnCreate()
{
	m_gdi->m_renderTarget->m_flag74 = 1;
}

// FUNCTION: LEMBALL 0x0044e960
void CTrackWindow::Create(const CVSRect& p_rect, CPVWnd* p_parent, char* p_name)
{
	CHotAreaHandler* handler;
	const CVSPoint* position;

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
void CTrackWindow::Move(const CVSPoint& p_position)
{
	CGWnd::Move(p_position);
	m_trackRect.m_x = p_position.m_x;
	m_trackRect.m_y = p_position.m_y;
}

// FUNCTION: LEMBALL 0x0044ea00
void CTrackWindow::OnPaint(const CVSRect& p_rect)
{
	int height = m_trackRect.m_height;
	int width = (int) m_trackRect.m_width * m_value / 100;
	if (m_value != 0) {
		m_line.m_colour = 0xac;
		m_line.m_bounds.m_width = width;
		m_line.m_bounds.m_height = height;
		m_line.m_bounds.m_x = 0;
		m_line.m_bounds.m_y = 0;
		m_line.Draw(m_gdi);
		m_edges[0].m_start.m_x = 0;
		m_edges[0].m_start.m_y = 0;
		m_edges[0].m_end.m_x = width;
		m_edges[0].m_end.m_y = 0;
		m_edges[0].m_colour = 0xab;
		m_edges[0].Draw(m_gdi);
		m_edges[1].m_start.m_x = 0;
		m_edges[1].m_start.m_y = 0;
		m_edges[1].m_end.m_x = 0;
		m_edges[1].m_end.m_y = height;
		m_edges[1].m_colour = 0xab;
		m_edges[1].Draw(m_gdi);
		m_edges[2].m_start.m_x = (short) m_value;
		m_edges[2].m_start.m_y = height;
		m_edges[2].m_end.m_x = 0;
		m_edges[2].m_end.m_y = height;
		m_edges[2].m_colour = 0xbc;
		m_edges[2].Draw(m_gdi);
		m_edges[3].m_start.m_x = (short) m_value;
		m_edges[3].m_start.m_y = height;
		m_edges[3].m_end.m_x = width;
		m_edges[3].m_end.m_y = 0;
		m_edges[3].m_colour = 0xbc;
		m_edges[3].Draw(m_gdi);
	}
}

// FUNCTION: LEMBALL 0x0044eb60
void CTrackWindow::SetButtonValue(int p_value)
{
	if (m_value != p_value) {
		Message message;
		message.m_type = MESSAGE_BUTTON_RELEASED;
		m_value = p_value;
		message.m_time = CurrentQueueTimer();
		message.m_code = m_contextId;
		message.m_payload = (void*) m_value;
		message.m_source = (void*) 100;
		g_pMasterInputQueue->Post(message);
	}
}

// FUNCTION: LEMBALL 0x0044ebc0
void CTrackWindow::OnInside(const CVSPoint& p_point)
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
void CTrackWindow::OnButtonDown(const CVSPoint& p_point, int p_flags)
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
