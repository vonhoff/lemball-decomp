#include "CZBuffClear.h"

#include "CGDI.h"
#include "Platform/Windows/Graphics/CSurface.h"

// FUNCTION: LEMBALL 0x00439900
void CZBuffClear::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00439910
void CZBuffClear::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
