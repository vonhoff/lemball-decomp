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
	p_gdi->m_renderTarget->GetChangeList()->PushActive((unsigned char) m_activeMarker);
}

// 68K 0x10100cae __dt__11CPushActiveFv
PushActive::~PushActive()
{
}

// 68K 0x101174dc __ct__11CPushActiveFv
// FUNCTION: LEMBALL 0x004698b0
PushActive::PushActive() : m_activeMarker(0)
{
}
