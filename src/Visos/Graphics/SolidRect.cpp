#include "SolidRect.h"

#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x1010194e Draw__10CSolidRectFP4CGDI
// FUNCTION: LEMBALL 0x00432b10
void SolidRect::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x10101980 Render__10CSolidRectFP4CGDI
// FUNCTION: LEMBALL 0x00432b20
void SolidRect::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}

// 68K 0x1010a6c2 __ct__10CSolidRectFv
// FUNCTION: LEMBALL 0x00439520
SolidRect::SolidRect() : m_top(0), m_left(0), m_bottom(0), m_right(0)
{
}

// FUNCTION: LEMBALL 0x004756d0
VsRect* SolidRect::GetBounds()
{
	return (VsRect*) &m_left;
}
