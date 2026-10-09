#include "CTrackerButton.h"

#include "CTrackWindow.h"
#include "Engine/Input/CHotAreaHandler.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Platform/Windows/Windowing/CGWnd.h"
#include "Platform/Windows/Windowing/CGraphicButton.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"

#include <stddef.h>

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

// FUNCTION: LEMBALL 0x0044ed70
void CTrackerButton::Move(const CVSPoint& p_point)
{
	m_forceDrawCount = 1;
	CHotAreaHandler::m_bounds.m_x -= m_relativeTopLeft.m_x;
	CHotAreaHandler::m_bounds.m_y -= m_relativeTopLeft.m_y;
	CGWnd::Move(p_point);
	CHotAreaHandler::m_bounds.m_x += m_relativeTopLeft.m_x;
	CHotAreaHandler::m_bounds.m_y += m_relativeTopLeft.m_y;
	m_trackWindow->Move(CVSPoint(m_buttonPosition.m_x + m_trackOffset.m_x, m_buttonPosition.m_y + m_trackOffset.m_y));
	m_trackWindow->Move(CVSPoint(p_point.m_x + m_trackOffset.m_x, p_point.m_y + m_trackOffset.m_y));
}
