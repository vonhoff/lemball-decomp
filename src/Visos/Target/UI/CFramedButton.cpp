#include "CFramedButton.h"

#include "../../Foundation/CChangeList.h"
#include "../../Graphics/CClipRect.h"
#include "../../Graphics/CGDI.h"
#include "../../Graphics/CHotAreaList.h"
#include "../../Graphics/CLine.h"
#include "../../Graphics/CSurface.h"
#include "Visos/Foundation/CVsPoint.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Graphics/CDepressedButton.h"
#include "Visos/Graphics/CGWnd.h"
#include "Visos/Graphics/CHotAreaHandler.h"
#include "Visos/Graphics/CPVGWnd.h"

extern char g_szButton[];

// FUNCTION: LEMBALL 0x00468a40
CFramedButton::CFramedButton(const CVsRect& p_rect, CPVGWnd* p_parent, unsigned int p_frameColor)
	: CDepressedButton(p_rect, p_parent)
{
	m_frameColor = p_frameColor;
	InitializeFramePrimitives();
	CVsRect createRect;
	createRect.m_width = m_bounds.m_width;
	createRect.m_height = m_bounds.m_height;
	createRect.m_x = m_buttonX;
	createRect.m_y = m_buttonY;
	CGWnd* window = this;
	window->Create(createRect, m_ownerWindow, g_szButton);
	CHotAreaHandler::m_bounds.m_x += m_relativeTopLeft.m_x;
	CHotAreaHandler::m_bounds.m_y += m_relativeTopLeft.m_y;
	CHotAreaHandler* area = this;
	m_ownerWindow->m_hotAreaList->AddToList(area);
}

// FUNCTION: LEMBALL 0x00468b20
CFramedButton::CFramedButton(CPVGWnd* p_parent, unsigned int p_frameColor) : CDepressedButton(p_parent)
{
	m_frameColor = p_frameColor;
	InitializeFramePrimitives();
}

// FUNCTION: LEMBALL 0x00468b80
void CFramedButton::InitializeFramePrimitives()
{
	m_frameLine = new CLine[1];
	m_gdiFlags++;
	m_frameRects = new CClipRect[4];
	m_gdiFlags += 4;
}

// FUNCTION: LEMBALL 0x00468c10
CFramedButton::~CFramedButton()
{
	delete[] m_frameRects;
	delete[] m_frameLine;
}

// FUNCTION: LEMBALL 0x00468c50
void CFramedButton::DrawButton()
{
	unsigned int light;
	unsigned int dark;
	unsigned int i;
	m_gdi->m_renderTarget->GetCurrDB();
	CVsRect bounds;
	bounds.m_width = m_bounds.m_width;
	bounds.m_height = m_bounds.m_height;
	bounds.m_x = 0;
	bounds.m_y = 0;
	CLine* line = m_frameLine;
	line->m_bounds.m_width = bounds.m_width;
	line->m_bounds.m_height = bounds.m_height;
	line->m_bounds.m_x = bounds.m_x;
	line->m_bounds.m_y = bounds.m_y;
	line->m_color = m_frameColor;
	m_frameLine->Draw(m_gdi);
	bool depressed = m_pressed != 0 && CHotAreaHandler::m_active != 0;
	if (depressed) {
		light = 0xf8;
		dark = 0xff;
	}
	else {
		light = 0xff;
		dark = 0xf8;
	}
	CClipRect* edge = &m_frameRects[0];
	edge->m_left = 0;
	edge->m_top = 0;
	edge->m_right = (short) (m_bounds.m_width - 1);
	edge->m_bottom = 0;
	edge->m_reserved0c = light;
	edge = &m_frameRects[1];
	edge->m_left = 0;
	edge->m_top = 0;
	edge->m_right = 0;
	edge->m_bottom = (short) (m_bounds.m_height - 1);
	edge->m_reserved0c = light;
	edge = &m_frameRects[2];
	edge->m_left = (short) (m_bounds.m_width - 1);
	edge->m_top = 0;
	edge->m_right = (short) (m_bounds.m_width - 1);
	edge->m_bottom = (short) (m_bounds.m_height - 1);
	edge->m_reserved0c = dark;
	edge = &m_frameRects[3];
	edge->m_left = 0;
	edge->m_top = (short) (m_bounds.m_height - 1);
	edge->m_right = (short) (m_bounds.m_width - 1);
	edge->m_bottom = (short) (m_bounds.m_height - 1);
	edge->m_reserved0c = dark;
	i = 0;
	do {
		m_frameRects[i].Draw(m_gdi);
		i++;
	} while (i < 4);
}

// FUNCTION: LEMBALL 0x00468dd0
void CFramedButton::OnPaint(const CVsRect& p_rect)
{
	if (m_gdi->m_primitiveCount == 0 && (m_autoDraw != 0 || m_forceDrawCount != 0 || m_pressed != m_lastDrawnPressed)) {
		if (GetSizeStatus() != 0) {
			_DrawButton();
			DrawButton();
		}
		CChangeList* changeList = m_gdi->m_renderTarget->GetChangeList();
		m_gdi->AddToList(m_primitive);
		changeList->Reset();
		m_drawCompleted = 1;
	}
}
