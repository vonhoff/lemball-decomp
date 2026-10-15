#include "CTrackerButton.h"

#include <stddef.h>

#include "CTrackWindow.h"
#include "Engine/Input/CHotAreaHandler.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Platform/Windows/Windowing/CGWnd.h"
#include "Platform/Windows/Windowing/CGraphicButton.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"

// FUNCTION: LEMBALL 0x0044ec30
CTrackerButton::CTrackerButton(const CVSPoint& p_position,
							   CPVGWnd* p_parent,
							   unsigned long p_animId,
							   CVSRect& p_trackRect,
							   int p_value)
	: CGraphicButton(p_position, p_parent, p_animId, 3)
{
	CVSRect createRect(p_trackRect);
	createRect.m_x += p_position.m_x;
	createRect.m_y += p_position.m_y;
	const CVSPoint* trackPosition = &p_trackRect;
	m_trackOffset.m_x = trackPosition->m_x;
	m_trackOffset.m_y = trackPosition->m_y;
	m_trackWindow = new CTrackWindow(p_trackRect, p_value, p_parent);
	m_trackWindow->Create(createRect, p_parent, NULL);
}

// FUNCTION: LEMBALL 0x0044ed20
CTrackerButton::~CTrackerButton()
{
	if (m_trackWindow->m_lifecycleRefs == 1) {
		m_trackWindow->Destroy();
	}
	delete m_trackWindow;
}

inline CVSPoint operator+(const CVSPoint& p_left, const CVSPoint& p_right)
{
	return CVSPoint(p_left.m_x + p_right.m_x, p_left.m_y + p_right.m_y);
}

// FUNCTION: LEMBALL 0x0044ed70
void CTrackerButton::Move(const CVSPoint& p_point)
{
	m_forceDrawCount = 1;
	m_bounds.m_x -= m_relativeTopLeft.m_x;
	m_bounds.m_y -= m_relativeTopLeft.m_y;
	CGWnd::Move(p_point);
	m_bounds.m_x += m_relativeTopLeft.m_x;
	m_bounds.m_y += m_relativeTopLeft.m_y;
	m_trackWindow->Move(m_buttonPosition + m_trackOffset);
	m_trackWindow->Move(p_point + m_trackOffset);
}
