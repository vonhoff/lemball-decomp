#include "PopActive.h"

#include "../Foundation/ChangeList.h"
#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x10100cfc __dt__10CPopActiveFv
PopActive::~PopActive()
{
}

// 68K 0x1010146c Draw__10CPopActiveFP4CGDI
// FUNCTION: LEMBALL 0x004397d0
void PopActive::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x1010149e Render__10CPopActiveFP4CGDI
// FUNCTION: LEMBALL 0x004397e0
void PopActive::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->GetChangeList()->PopActive();
}

