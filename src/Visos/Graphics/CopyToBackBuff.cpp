#include "CopyToBackBuff.h"

#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x10101efe Draw__15CCopyToBackBuffFP4CGDI
// FUNCTION: LEMBALL 0x00439900
void CopyToBackBuff::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x10101f36 Render__15CCopyToBackBuffFP4CGDI
// FUNCTION: LEMBALL 0x00439910
void CopyToBackBuff::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((ZBuffScroll*) this);
}
