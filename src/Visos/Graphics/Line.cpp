#include "Line.h"

#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x10115cd8 __ct__5CLineFv
// FUNCTION: LEMBALL 0x004394c0
Line::Line() : m_y1(0)
{
	m_x1 = 0;
	m_y2 = 0;
	m_x2 = 0;
}

// 68K 0x101018e8 Draw__5CLineFP4CGDI
// FUNCTION: LEMBALL 0x00439800
void Line::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x10101914 Render__5CLineFP4CGDI
// FUNCTION: LEMBALL 0x00439810
void Line::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((SolidRect*) this);
}
