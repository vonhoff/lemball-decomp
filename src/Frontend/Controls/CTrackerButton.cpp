#include "CTrackerButton.h"

#include "../Windows/CTrackWindow.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CGraphicButton.h"
#include "Visos/Graphics/CHotAreaHandler.h"
#include "Visos/Graphics/CPvGWnd.h"

#include <new.h>

// FUNCTION: LEMBALL 0x0044ec30
CTrackerButton::CTrackerButton(const CVsPoint& p_position,
							   CPvGWnd* p_parent,
							   unsigned long p_animId,
							   CVsRect& p_trackRect,
							   int p_value)
	: CGraphicButton(p_position, p_parent, p_animId, 3)
{
	m_trackOffsetY = 0;
	m_trackOffsetX = 0;
	CVsRect createRect(p_trackRect);
	createRect.m_x += p_position.m_x;
	createRect.m_y += p_position.m_y;
	const CVsPoint* trackPosition = &p_trackRect;
	m_trackOffsetX = trackPosition->m_x;
	m_trackOffsetY = trackPosition->m_y;
	m_trackWindow = new CTrackWindow(p_trackRect, p_value, p_parent);
	m_trackWindow->Create(createRect, p_parent, 0);
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
void CTrackerButton::Move(const CVsPoint& p_point)
{
	m_forceDrawCount = 1;
	CHotAreaHandler::m_bounds.m_x -= m_relativeTopLeft.m_x;
	CHotAreaHandler::m_bounds.m_y -= m_relativeTopLeft.m_y;
	CGWnd::Move(p_point);
	CHotAreaHandler::m_bounds.m_x += m_relativeTopLeft.m_x;
	CHotAreaHandler::m_bounds.m_y += m_relativeTopLeft.m_y;
	m_trackWindow->Move(CVsPoint(m_buttonX + m_trackOffsetX, m_buttonY + m_trackOffsetY));
	m_trackWindow->Move(CVsPoint(p_point.m_x + m_trackOffsetX, p_point.m_y + m_trackOffsetY));
}
