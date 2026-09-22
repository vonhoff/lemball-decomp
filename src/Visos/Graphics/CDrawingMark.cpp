#include "CDrawingMark.h"

#include "../Foundation/CChangeList.h"
#include "CGdi.h"
#include "CSurface.h"

// FUNCTION: LEMBALL 0x00432380
void CDrawingMark::Draw(CGdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00432390
void CDrawingMark::Render(CGdi* p_gdi)
{
	p_gdi->m_renderTarget->GetChangeList()->SetDrawMark();
}
