#include "CopyColourToBackBuff.h"

#include "Gdi.h"
#include "VsGdi.h"
class ZBuffClear;

// 68K 0x10101f7c Draw__21CCopyColourToBackBuffFP4CGDI
// FUNCTION: LEMBALL 0x00439930
void CopyColourToBackBuff::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x10101fba Render__21CCopyColourToBackBuffFP4CGDI
// FUNCTION: LEMBALL 0x00439940
void CopyColourToBackBuff::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((ZBuffClear*) this);
}
