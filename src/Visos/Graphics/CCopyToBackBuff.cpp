#include "CCopyToBackBuff.h"

#include "CGdi.h"
#include "CSurface.h"
class CZBuffClear;

// FUNCTION: LEMBALL 0x00439900
void CCopyToBackBuff::Draw(CGdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00439910
void CCopyToBackBuff::Render(CGdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CZBuffClear*) this);
}
