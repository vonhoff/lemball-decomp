#include "TrackerButton.h"

#include "../Windows/TrackWindow.h"

#include <new.h>

// 68K 0x1080e8c4 __ct__14CTrackerButtonFRC8CVSPointP7CPVGWndUlR7CVSRecti
// FUNCTION: LEMBALL 0x0044ec30
TrackerButton::TrackerButton(const VsPoint& p_arg0, PvGWnd* p_arg1, unsigned long p_arg2, VsRect& p_arg3, int p_arg4)
	: GraphicButton(p_arg0, p_arg1, p_arg2, 3)
{
	VsRect createRect;
	void* storage;

	m_trackOffsetY = 0;
	m_trackOffsetX = 0;
	createRect.m_width = p_arg3.m_width;
	createRect.m_height = p_arg3.m_height;
	createRect.m_x = (short) (p_arg3.m_x + p_arg0.m_x);
	createRect.m_y = (short) (p_arg3.m_y + p_arg0.m_y);
	m_trackOffsetX = p_arg3.m_x;
	m_trackOffsetY = p_arg3.m_y;
	storage = operator new(0x134);
	if (storage == 0) {
		m_trackWindow = 0;
	}
	else {
		m_trackWindow = new (storage) TrackWindow(p_arg3, p_arg4, p_arg1);
	}
	m_trackWindow->Create(createRect, p_arg1, 0);
}

// 68K 0x1080eaac Move__14CTrackerButtonFRC8CVSPoint
// FUNCTION: LEMBALL 0x0044ed70
void TrackerButton::Move(const VsPoint& p_point)
{
	m_forceDrawCount = 1;
	HotAreaHandler::m_x -= m_relativeTopLeft.m_x;
	HotAreaHandler::m_y -= m_relativeTopLeft.m_y;
	GWnd::Move(p_point);
	HotAreaHandler::m_x += m_relativeTopLeft.m_x;
	HotAreaHandler::m_y += m_relativeTopLeft.m_y;
	m_trackWindow->Move(VsPoint(m_buttonX + m_trackOffsetX, m_buttonY + m_trackOffsetY));
	m_trackWindow->Move(VsPoint(p_point.m_x + m_trackOffsetX, p_point.m_y + m_trackOffsetY));
}

// 68K 0x1080ea02 __dt__14CTrackerButtonFv
TrackerButton::~TrackerButton()
{
}
