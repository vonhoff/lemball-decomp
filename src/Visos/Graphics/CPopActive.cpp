#include "CPopActive.h"

#include "../Foundation/CChangeList.h"
#include "CGDI.h"
#include "CSurface.h"

// FUNCTION: LEMBALL 0x004397d0
void CPopActive::Draw(CGDI* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x004397e0
void CPopActive::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->GetChangeList()->PopActive();
}
