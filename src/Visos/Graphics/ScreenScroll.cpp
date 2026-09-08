#include "ScreenScroll.h"

#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x10101e34 Draw__13CScreenScrollFP4CGDI
// FUNCTION: LEMBALL 0x00439860
void ScreenScroll::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x10101e6a Render__13CScreenScrollFP4CGDI
// FUNCTION: LEMBALL 0x00439870
void ScreenScroll::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
