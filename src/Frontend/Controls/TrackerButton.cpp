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
// STUB: LEMBALL 0x0044ed70
void TrackerButton::Move(const VsPoint& p_point)
{
}

// 68K 0x1080ea02 __dt__14CTrackerButtonFv
TrackerButton::~TrackerButton()
{
}
