#include "CCopyColourToBackBuff.h"

#include "CGDI.h"

// FUNCTION: LEMBALL 0x00439720
CCopyColourToBackBuff::~CCopyColourToBackBuff()
{
}

#include "CSurface.h"

// FUNCTION: LEMBALL 0x004398d0
void CCopyColourToBackBuff::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x004398e0
void CCopyColourToBackBuff::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
