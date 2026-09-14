#include "TargetFramedButton.h"

#include "../Foundation/ChangeList.h"
#include "../Graphics/ClipRect.h"
#include "../Graphics/Gdi.h"
#include "../Graphics/HotAreaList.h"
#include "../Graphics/Line.h"
#include "../Graphics/VsGdi.h"

extern char g_szButton[];

// FUNCTION: LEMBALL 0x00468a40
TargetFramedButton::TargetFramedButton(const VsRect& p_rect, PvGWnd* p_parent, unsigned int p_frameColor)
	: DepressedButton(p_rect, p_parent)
{
	m_frameColor = p_frameColor;
	InitializeFramePrimitives();
	VsRect createRect;
	createRect.m_width = m_width;
	createRect.m_height = m_height;
	createRect.m_x = m_buttonX;
	createRect.m_y = m_buttonY;
	GWnd* window = this;
	window->Create(createRect, m_ownerWindow, g_szButton);
	HotAreaHandler::m_x += m_relativeTopLeft.m_x;
	HotAreaHandler::m_y += m_relativeTopLeft.m_y;
	HotAreaHandler* area = this;
	m_ownerWindow->m_hotAreaList->AddToList(area);
}

// FUNCTION: LEMBALL 0x00468b20
TargetFramedButton::TargetFramedButton(PvGWnd* p_parent, unsigned int p_frameColor) : DepressedButton(p_parent)
{
	m_frameColor = p_frameColor;
	InitializeFramePrimitives();
}

// FUNCTION: LEMBALL 0x00468b80
void TargetFramedButton::InitializeFramePrimitives()
{
	m_frameLine = new Line[1];
	m_gdiFlags++;
	m_frameRects = new ClipRect[4];
	m_gdiFlags += 4;
}

// FUNCTION: LEMBALL 0x00468c10
TargetFramedButton::~TargetFramedButton()
{
	delete[] m_frameRects;
	delete[] m_frameLine;
}

// FUNCTION: LEMBALL 0x00468c50
void TargetFramedButton::DrawButton()
{
	unsigned int light;
	unsigned int dark;
	unsigned int i;
	m_gdi->m_renderTarget->GetCurrDb();
	VsRect bounds;
	bounds.m_width = m_width;
	bounds.m_height = m_height;
	bounds.m_x = 0;
	bounds.m_y = 0;
	Line* line = m_frameLine;
	line->m_x1 = bounds.m_width;
	line->m_y1 = bounds.m_height;
	line->m_x2 = bounds.m_x;
	line->m_y2 = bounds.m_y;
	line->m_color = m_frameColor;
	m_frameLine->Draw(m_gdi);
	bool depressed = m_pressed != 0 && HotAreaHandler::m_active != 0;
	if (depressed) {
		light = 0xf8;
		dark = 0xff;
	}
	else {
		light = 0xff;
		dark = 0xf8;
	}
	ClipRect* edge = &m_frameRects[0];
	edge->m_left = 0;
	edge->m_top = 0;
	edge->m_right = (short) (m_width - 1);
	edge->m_bottom = 0;
	edge->m_reserved0c = light;
	edge = &m_frameRects[1];
	edge->m_left = 0;
	edge->m_top = 0;
	edge->m_right = 0;
	edge->m_bottom = (short) (m_height - 1);
	edge->m_reserved0c = light;
	edge = &m_frameRects[2];
	edge->m_left = (short) (m_width - 1);
	edge->m_top = 0;
	edge->m_right = (short) (m_width - 1);
	edge->m_bottom = (short) (m_height - 1);
	edge->m_reserved0c = dark;
	edge = &m_frameRects[3];
	edge->m_left = 0;
	edge->m_top = (short) (m_height - 1);
	edge->m_right = (short) (m_width - 1);
	edge->m_bottom = (short) (m_height - 1);
	edge->m_reserved0c = dark;
	i = 0;
	do {
		m_frameRects[i].Draw(m_gdi);
		i++;
	} while (i < 4);
}

// FUNCTION: LEMBALL 0x00468dd0
void TargetFramedButton::OnPaint(const VsRect& p_rect)
{
	if (m_gdi->m_primitiveCount == 0 && (m_autoDraw != 0 || m_forceDrawCount != 0 || m_pressed != m_lastDrawnPressed)) {
		if (GetSizeStatus() != 0) {
			InternalDrawButton();
			DrawButton();
		}
		ChangeList* changeList = m_gdi->m_renderTarget->GetChangeList();
		m_gdi->AddToList(m_primitive);
		changeList->Reset();
		m_drawCompleted = 1;
	}
}
