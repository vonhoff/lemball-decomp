#include "CScreenScroll.h"

#include "CGDI.h"
#include "CSurface.h"

// FUNCTION: LEMBALL 0x00439860
void CScreenScroll::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00439870
void CScreenScroll::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
