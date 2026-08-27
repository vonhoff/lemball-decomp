#include "TrackWindow.h"

// 68K 0x1080e1a2 __ct__12CTrackWindowFRC7CVSRectiP7CPVGWnd
// STUB: LEMBALL 0x0044e790
TrackWindow::TrackWindow(const VsRect& p_arg0, int p_arg1, PvGWnd* p_arg2)
{
}

// 68K 0x1080e3d6 OnCreate__12CTrackWindowFv
// FUNCTION: LEMBALL 0x0044e940
void TrackWindow::OnCreate()
{
	char* target = *(char**) (*(int*) ((char*) this + 0x4c) + 0x0c);
	*(unsigned int*) (*(int*) (*(int*) (target + 0x40) + 4) + 0x74 + (int) target) = 1;
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
// STUB: LEMBALL 0x0044ec10
unsigned int TrackWindow::OnButtonDown(const VsPoint& p_point, int p_flags)
{
	return 0;
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

