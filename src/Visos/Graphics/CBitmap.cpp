#include "CBitmap.h"

#include "CGDI.h"
#include "CSurface.h"
class CCopyToBackBuff;

// FUNCTION: LEMBALL 0x004398a0
void CBitmap::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x004398b0
void CBitmap::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CCopyToBackBuff*) this);
}
