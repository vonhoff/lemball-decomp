#include "CClipRect.h"

#include "CGdi.h"
#include "CSurface.h"
class CLine;

// FUNCTION: LEMBALL 0x00432a30
CClipRect::CClipRect() : m_top(0)
{
	m_left = 0;
	m_bottom = 0;
	m_right = 0;
}

// FUNCTION: LEMBALL 0x00432ad0
void CClipRect::Draw(CGdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00432ae0
void CClipRect::Render(CGdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CLine*) this);
}
