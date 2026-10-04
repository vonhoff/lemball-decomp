#include "CCopyToBackBuff.h"

#include "CGDI.h"
#include "Platform/Windows/Graphics/CSurface.h"

// FUNCTION: LEMBALL 0x004398a0
void CCopyToBackBuff::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x004398b0
void CCopyToBackBuff::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
