#include "TrackWindow.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/HotAreaList.h"
#include "../../Visos/Graphics/VsGdi.h"

// 68K 0x1080e1a2 __ct__12CTrackWindowFRC7CVSRectiP7CPVGWnd
// FUNCTION: LEMBALL 0x0044e790
TrackWindow::TrackWindow(const VsRect& p_rect, int p_value, PvGWnd* p_parent) : HotAreaHandler(p_rect)
{
	m_reserved128 = 0;
	m_trackSize.m_x = p_rect.m_width;
	m_trackSize.m_y = 0;
	m_value = p_value;
	m_trackRect.m_width = p_rect.m_width;
	m_trackRect.m_height = p_rect.m_height;
	m_trackRect.m_x = p_rect.m_x;
	m_gdiFlags = m_gdiFlags + 6;
	m_trackRect.m_y = p_rect.m_y;
	m_parent = p_parent;
	m_contextId = -1;
	SetActive(1);
	m_externalEnabled = 1;
	m_reserved = 1;
}

// 68K 0x1080e3d6 OnCreate__12CTrackWindowFv
// FUNCTION: LEMBALL 0x0044e940
void TrackWindow::OnCreate()
{
	m_gdi->m_renderTarget->m_flag74 = 1;
}

// 68K 0x1080e418 Create__12CTrackWindowFRC7CVSRectP6CPVWndPc
// FUNCTION: LEMBALL 0x0044e960
void TrackWindow::Create(const VsRect& p_rect, PvWnd* p_parent, char* p_name)
{
	HotAreaHandler* handler;
	const VsPoint* position;

	GWnd::Create(p_rect, p_parent, p_name);
	HotAreaHandler::m_width = p_rect.m_width;
	HotAreaHandler::m_height = p_rect.m_height;
	position = &p_rect;
	HotAreaHandler::m_x = position->m_x;
	HotAreaHandler::m_y = position->m_y;
	handler = this;
	m_parent->m_hotAreaList->AddToList(handler);
}

// 68K 0x1080e4cc Move__12CTrackWindowFRC8CVSPoint
// FUNCTION: LEMBALL 0x0044e9d0
void TrackWindow::Move(const VsPoint& p_position)
{
	GWnd::Move(p_position);
	m_trackRect.m_x = p_position.m_x;
	m_trackRect.m_y = p_position.m_y;
}

// 68K 0x1080e52c OnPaint__12CTrackWindowFRC7CVSRect
// STUB: LEMBALL 0x0044ea00
void TrackWindow::OnPaint(const VsRect& p_rect)
{
}

// 68K 0x1080e732 SetButtonValue__12CTrackWindowFi
// FUNCTION: LEMBALL 0x0044eb60
void TrackWindow::SetButtonValue(int p_value)
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

// 68K 0x1080e7be OnInside__12CTrackWindowFRC8CVSPoint
// FUNCTION: LEMBALL 0x0044ebc0
void TrackWindow::OnInside(const VsPoint& p_point)
{
	if (m_buttonState[0] != 0) {
		int distance = (int) p_point.m_x - (int) HotAreaHandler::m_x;
		if (distance < 0) {
			distance = 0;
		}
		else if (distance > m_trackRect.m_width) {
			distance = m_trackRect.m_width;
		}
		SetButtonValue(distance * 100 / (int) HotAreaHandler::m_width);
	}
}

// 68K 0x1080e842 OnButtonDown__12CTrackWindowFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x0044ec10
void TrackWindow::OnButtonDown(const VsPoint& p_point, int p_flags)
{
	OnInside(p_point);
}

// 68K 0x1080e898 OnDriverChange__12CTrackWindowFv
// FUNCTION: LEMBALL 0x0044ec20
void TrackWindow::OnDriverChange()
{
}

// 68K 0x1011c650 GetStyle__12CTrackWindowFv
// FUNCTION: LEMBALL 0x0044efe0
unsigned int TrackWindow::GetStyle()
{
	return 2147485697;
}

// 68K 0x1080e2e2 __dt__12CTrackWindowFv
TrackWindow::~TrackWindow()
{
}
