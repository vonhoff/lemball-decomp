#include "CZRLE.h"

#include "../Resources/CResBase.h"
#include "../Resources/CResZRLE.h"
#include "CGDI.h"
#include "CSurface.h"

// FUNCTION: LEMBALL 0x00467ae0
void CZRLE::Draw(CGDI* p_gdi)
{
	CResBase* resource = m_resource;
	if (resource->m_loaded != 0) {
		resource->m_age = 0;
	}
	else {
		resource->LoadData();
	}
	resource->m_directUseCount++;
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00467b10
void CZRLE::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this, (CResZRLE*) m_resource);
	m_resource->m_directUseCount--;
}
