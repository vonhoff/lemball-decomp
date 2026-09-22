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
CTrackerButton::CTrackerButton(const CVsPoint& p_arg0,
							   CPvGWnd* p_arg1,
							   unsigned long p_arg2,
							   CVsRect& p_arg3,
							   int p_arg4)
	: CGraphicButton(p_arg0, p_arg1, p_arg2, 3)
{
	CVsRect createRect;
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
		m_trackWindow = new (storage) CTrackWindow(p_arg3, p_arg4, p_arg1);
	}
	m_trackWindow->Create(createRect, p_arg1, 0);
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
