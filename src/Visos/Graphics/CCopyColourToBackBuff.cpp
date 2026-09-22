#include "CCopyColourToBackBuff.h"

#include "CGDI.h"
#include "CSurface.h"
class CZBuffScroll;

// FUNCTION: LEMBALL 0x00439930
void CCopyColourToBackBuff::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00439940
void CCopyColourToBackBuff::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CZBuffScroll*) this);
}
