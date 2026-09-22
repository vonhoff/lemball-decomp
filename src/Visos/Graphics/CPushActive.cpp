#include "CPushActive.h"

#include "../Foundation/CChangeList.h"
#include "CGDI.h"
#include "CSurface.h"

// FUNCTION: LEMBALL 0x00439760
void CPushActive::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00439770
void CPushActive::Render(CGDI* p_gdi)
{
	CChangeList* changeList = p_gdi->m_renderTarget->GetChangeList();
	changeList->PushActive((unsigned char) m_activeMarker);
}
