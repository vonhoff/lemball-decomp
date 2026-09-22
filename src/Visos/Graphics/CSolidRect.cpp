#include "CSolidRect.h"

#include "CGDI.h"
#include "CSurface.h"
class CClipRect;

// FUNCTION: LEMBALL 0x00432b10
void CSolidRect::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00432b20
void CSolidRect::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CClipRect*) this);
}

// FUNCTION: LEMBALL 0x004756d0
CVsRect* CSolidRect::GetBounds()
{
	return &m_bounds;
}
