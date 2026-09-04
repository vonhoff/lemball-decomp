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
	p_gdi->m_renderTarget->Blit((ClipRect*) this);
}

// FUNCTION: LEMBALL 0x004756d0
VsRect* SolidRect::GetBounds()
{
	return (VsRect*) &m_left;
}
