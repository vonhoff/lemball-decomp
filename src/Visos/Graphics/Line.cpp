#include "Line.h"

#include "Gdi.h"
#include "VsGdi.h"

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
