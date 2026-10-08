#include "CFramedButton.h"

#include "CDepressedButton.h"
#include "CGWnd.h"
#include "CPVGWnd.h"
#include "Engine/Graphics/CChangeList.h"
#include "Engine/Graphics/Primitives/CGDI.h"
#include "Engine/Graphics/Primitives/CLine.h"
#include "Engine/Graphics/Primitives/CSolidRect.h"
#include "Engine/Input/CHotAreaHandler.h"
#include "Engine/Input/CHotAreaList.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Platform/Windows/Graphics/CSurface.h"

enum {
	BUTTON_FRAME_LIGHT_SHADE_PALETTE_INDEX = 0xf8,
	BUTTON_FRAME_DARK_SHADE_PALETTE_INDEX = 0xff
};

extern char g_szButton[];

// FUNCTION: LEMBALL 0x00468a40
CFramedButton::CFramedButton(const CVSRect& p_rect, CPVGWnd* p_parent, unsigned int p_frameColour)
	: CDepressedButton(p_rect, p_parent)
{
	m_frameColour = p_frameColour;
	InitializeFramePrimitives();
	CVSRect createRect;
	createRect.m_width = m_bounds.m_width;
	createRect.m_height = m_bounds.m_height;
	createRect.m_x = m_buttonPosition.m_x;
	createRect.m_y = m_buttonPosition.m_y;
	CGWnd* window = this;
	window->Create(createRect, m_ownerWindow, g_szButton);
	CHotAreaHandler::m_bounds.m_x += m_relativeTopLeft.m_x;
	CHotAreaHandler::m_bounds.m_y += m_relativeTopLeft.m_y;
	CHotAreaHandler* area = this;
	m_ownerWindow->m_hotAreaList->AddToList(area);
}

// FUNCTION: LEMBALL 0x00468b20
CFramedButton::CFramedButton(CPVGWnd* p_parent, unsigned int p_frameColour) : CDepressedButton(p_parent)
{
	m_frameColour = p_frameColour;
	InitializeFramePrimitives();
}

// FUNCTION: LEMBALL 0x00468b80
void CFramedButton::InitializeFramePrimitives()
{
	m_frameLine = new CSolidRect[1];
	m_gdiFlags++;
	m_frameEdges = new CLine[FRAMED_BUTTON_EDGE_COUNT];
	unsigned int& flags = m_gdiFlags;
	flags += FRAMED_BUTTON_EDGE_COUNT;
}

// FUNCTION: LEMBALL 0x00468c10
CFramedButton::~CFramedButton()
{
	delete[] m_frameEdges;
	delete[] m_frameLine;
}

// FUNCTION: LEMBALL 0x00468c50
void CFramedButton::DrawButton()
{
	unsigned int light;
	unsigned int dark;
	unsigned int i;
	m_gdi->m_renderTarget->GetCurrDB();
	CVSRect bounds;
	bounds.m_width = m_bounds.m_width;
	bounds.m_height = m_bounds.m_height;
	bounds.m_y = 0;
	bounds.m_x = 0;
	const CVSRect* rectangle = &bounds;
	unsigned int colour;
	CSolidRect* line = m_frameLine;
	colour = m_frameColour;
	line->m_bounds.m_width = rectangle->m_width;
	line->m_bounds.m_height = rectangle->m_height;
	line->m_bounds.m_x = rectangle->m_x;
	line->m_bounds.m_y = rectangle->m_y;
	line->m_colour = colour;
	m_frameLine->Draw(m_gdi);
	bool depressed = m_pressed != 0 && CHotAreaHandler::m_active != 0;
	if (depressed) {
		light = BUTTON_FRAME_LIGHT_SHADE_PALETTE_INDEX;
		dark = BUTTON_FRAME_DARK_SHADE_PALETTE_INDEX;
	}
	else {
		light = BUTTON_FRAME_DARK_SHADE_PALETTE_INDEX;
		dark = BUTTON_FRAME_LIGHT_SHADE_PALETTE_INDEX;
	}
	{
		int right;
		CLine* edge = &m_frameEdges[FRAMED_BUTTON_EDGE_TOP];
		right = m_bounds.m_width - 1;
		edge->m_start.m_x = 0;
		edge->m_start.m_y = 0;
		edge->m_end.m_x = (short) right;
		edge->m_end.m_y = 0;
		edge->m_colour = light;
	}
	{
		int bottom;
		CLine* edge = m_frameEdges;
		bottom = m_bounds.m_height - 1;
		edge[FRAMED_BUTTON_EDGE_LEFT].m_start.m_x = 0;
		edge[FRAMED_BUTTON_EDGE_LEFT].m_start.m_y = 0;
		edge++;
		edge->m_end.m_x = 0;
		edge->m_end.m_y = (short) bottom;
		edge->m_colour = light;
	}
	{
		CLine* edge = m_frameEdges;
		short left = (short) (m_bounds.m_width - 1);
		edge += FRAMED_BUTTON_EDGE_RIGHT;
		int right = m_bounds.m_width - 1;
		int bottom = m_bounds.m_height - 1;
		edge->m_start.m_x = left;
		edge->m_start.m_y = 0;
		edge->m_end.m_x = (short) right;
		edge->m_end.m_y = (short) bottom;
		edge->m_colour = dark;
	}
	{
		int bottom;
		int right;
		CLine* edge = m_frameEdges;
		bottom = m_bounds.m_height - 1;
		right = m_bounds.m_width - 1;
		edge[FRAMED_BUTTON_EDGE_BOTTOM].m_start.m_x = 0;
		edge[FRAMED_BUTTON_EDGE_BOTTOM].m_start.m_y = (short) bottom;
		edge[FRAMED_BUTTON_EDGE_BOTTOM].m_end.m_x = (short) right;
		edge += FRAMED_BUTTON_EDGE_BOTTOM;
		edge->m_end.m_y = (short) bottom;
		edge->m_colour = dark;
	}
	i = 0;
	do {
		m_frameEdges[i].Draw(m_gdi);
		i++;
	} while (i < FRAMED_BUTTON_EDGE_COUNT);
}

// FUNCTION: LEMBALL 0x00468dd0
void CFramedButton::OnPaint(const CVSRect& p_rect)
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
