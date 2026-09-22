#include "CPopActive.h"

#include "../Foundation/CChangeList.h"
#include "CGdi.h"
#include "VsGdi.h"

// FUNCTION: LEMBALL 0x004397d0
void CPopActive::Draw(CGdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x004397e0
void CPopActive::Render(CGdi* p_gdi)
{
	p_gdi->m_renderTarget->GetChangeList()->PopActive();
}
