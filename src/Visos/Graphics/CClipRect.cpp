#include "CClipRect.h"

#include "CGDI.h"
#include "CSurface.h"
class CLine;

// FUNCTION: LEMBALL 0x00432a30
CClipRect::CClipRect()
{
}

// FUNCTION: LEMBALL 0x00432ad0
void CClipRect::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00432ae0
void CClipRect::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CLine*) this);
}
