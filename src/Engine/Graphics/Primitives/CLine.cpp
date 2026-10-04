#include "CLine.h"

#include "CGDI.h"
#include "Platform/Windows/Graphics/CSurface.h"

// FUNCTION: LEMBALL 0x00432a30
CLine::CLine()
{
}

// FUNCTION: LEMBALL 0x00432ad0
void CLine::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00432ae0
void CLine::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}
