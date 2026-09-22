#include "CDrawingMark.h"

#include "../Foundation/CChangeList.h"
#include "CGDI.h"
#include "CSurface.h"

// FUNCTION: LEMBALL 0x00432380
void CDrawingMark::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00432390
void CDrawingMark::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->GetChangeList()->SetDrawMark();
}
