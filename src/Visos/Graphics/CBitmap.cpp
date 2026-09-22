#include "CBitmap.h"

#include "CGdi.h"
#include "VsGdi.h"
class CCopyToBackBuff;

// FUNCTION: LEMBALL 0x004398a0
void CBitmap::Draw(CGdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x004398b0
void CBitmap::Render(CGdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CCopyToBackBuff*) this);
}
