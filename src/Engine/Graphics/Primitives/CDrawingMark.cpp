#include "CDrawingMark.h"

#include "CGDI.h"
#include "Engine/Graphics/CChangeList.h"
#include "Platform/Windows/Graphics/CSurface.h"

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
