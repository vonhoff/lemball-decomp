#include "CBigBitmap.h"

#include "CGdi.h"

// FUNCTION: LEMBALL 0x00439720
CBigBitmap::~CBigBitmap()
{
}

#include "VsGdi.h"
class CCopyColourToBackBuff;

// FUNCTION: LEMBALL 0x004398d0
void CBigBitmap::Draw(CGdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x004398e0
void CBigBitmap::Render(CGdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CCopyColourToBackBuff*) this);
}
