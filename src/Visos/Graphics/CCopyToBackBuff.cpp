#include "CCopyToBackBuff.h"

#include "CGDI.h"
#include "CSurface.h"
class CZBuffClear;

// FUNCTION: LEMBALL 0x00439900
void CCopyToBackBuff::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00439910
void CCopyToBackBuff::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CZBuffClear*) this);
}
