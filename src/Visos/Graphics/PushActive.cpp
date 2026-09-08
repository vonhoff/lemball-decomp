#include "PushActive.h"

#include "../Foundation/ChangeList.h"
#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x101013e4 Draw__11CPushActiveFP4CGDI
// FUNCTION: LEMBALL 0x00439760
void PushActive::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x10101418 Render__11CPushActiveFP4CGDI
// FUNCTION: LEMBALL 0x00439770
void PushActive::Render(Gdi* p_gdi)
{
	ChangeList* changeList = p_gdi->m_renderTarget->GetChangeList();
	changeList->PushActive((unsigned char) m_activeMarker);
}
