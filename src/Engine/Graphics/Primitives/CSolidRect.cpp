#include "CSolidRect.h"

#include "CGDI.h"
#include "Engine/Graphics/Surfaces/CSurface.h"

// FUNCTION: LEMBALL 0x00439800
void CSolidRect::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00439810
void CSolidRect::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}

// FUNCTION: LEMBALL 0x004756d0
CVSRect* CSolidRect::GetBounds()
{
	return &m_bounds;
}
