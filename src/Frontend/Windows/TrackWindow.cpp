#include "TrackWindow.h"

#include "../../Visos/Graphics/Gdi.h"
#include "../../Visos/Graphics/VsGdi.h"

// 68K 0x1080e1a2 __ct__12CTrackWindowFRC7CVSRectiP7CPVGWnd
// FUNCTION: LEMBALL 0x0044e790
TrackWindow::TrackWindow(const VsRect& p_arg0, int p_arg1, PvGWnd* p_arg2) : HotAreaHandler(p_arg0)
{
	m_reserved128 = 0;
	m_trackSize.m_x = p_arg0.m_width;
	m_trackSize.m_y = 0;
	m_value = p_arg1;
	m_trackRect.m_width = p_arg0.m_width;
	m_trackRect.m_height = p_arg0.m_height;
	m_trackRect.m_x = p_arg0.m_x;
	m_gdiFlags = m_gdiFlags + 6;
	m_trackRect.m_y = p_arg0.m_y;
	m_parent = p_arg2;
	m_contextId = -1;
	SetActive(1);
	m_externalEnabled = 1;
	m_reserved = 1;
}

// 68K 0x1080e3d6 OnCreate__12CTrackWindowFv
// FUNCTION: LEMBALL 0x0044e940
void TrackWindow::Dummy3c()
{
	m_gdi->m_renderTarget->m_flag74 = 1;
}

// 68K 0x1080e418 Create__12CTrackWindowFRC7CVSRectP6CPVWndPc
// STUB: LEMBALL 0x0044e960
void TrackWindow::Create(const VsRect& p_rect, PvWnd* p_parent, char* p_name)
{
}

// 68K 0x1080e4cc Move__12CTrackWindowFRC8CVSPoint
// STUB: LEMBALL 0x0044e9d0
void TrackWindow::Move(const VsPoint& p_position)
{
}

// 68K 0x1080e52c OnPaint__12CTrackWindowFRC7CVSRect
// STUB: LEMBALL 0x0044ea00
void TrackWindow::OnPaint(const VsRect& p_rect)
{
}

// 68K 0x1080e732 SetButtonValue__12CTrackWindowFi
// STUB: LEMBALL 0x0044eb60
void TrackWindow::SetButtonValue(int p_value)
{
}

// 68K 0x1080e7be OnInside__12CTrackWindowFRC8CVSPoint
// STUB: LEMBALL 0x0044ebc0
void TrackWindow::OnInside(const VsPoint& p_point)
{
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
