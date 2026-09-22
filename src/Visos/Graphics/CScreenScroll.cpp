#include "CScreenScroll.h"

#include "CGdi.h"
#include "CSurface.h"

// FUNCTION: LEMBALL 0x00439860
void CScreenScroll::Draw(CGdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00439870
void CScreenScroll::Render(CGdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
