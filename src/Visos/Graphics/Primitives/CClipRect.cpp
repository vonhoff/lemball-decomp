#include "CClipRect.h"

#include "CGDI.h"
#include "Visos/Graphics/Surfaces/CSurface.h"

// FUNCTION: LEMBALL 0x00432b10
void CClipRect::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00432b20
void CClipRect::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
