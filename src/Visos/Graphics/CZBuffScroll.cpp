#include "CZBuffScroll.h"

#include "CGDI.h"
#include "CSurface.h"

// FUNCTION: LEMBALL 0x00439930
void CZBuffScroll::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00439940
void CZBuffScroll::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
