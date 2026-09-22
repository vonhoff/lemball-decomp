#include "CLine.h"

#include "CGdi.h"
#include "CSurface.h"
class CSolidRect;

// FUNCTION: LEMBALL 0x00439800
void CLine::Draw(CGdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00439810
void CLine::Render(CGdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CSolidRect*) this);
}
