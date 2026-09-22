#include "CBigBitmap.h"

#include "CGDI.h"

// FUNCTION: LEMBALL 0x00439720
CBigBitmap::~CBigBitmap()
{
}

#include "CSurface.h"
class CCopyColourToBackBuff;

// FUNCTION: LEMBALL 0x004398d0
void CBigBitmap::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x004398e0
void CBigBitmap::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CCopyColourToBackBuff*) this);
}
