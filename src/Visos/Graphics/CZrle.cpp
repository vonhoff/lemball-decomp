#include "CZrle.h"

#include "../Resources/CResBase.h"
#include "../Resources/CResZrle.h"
#include "CGdi.h"
#include "VsGdi.h"

// FUNCTION: LEMBALL 0x00467ae0
void CZrle::Draw(CGdi* p_gdi)
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
void CZrle::Render(CGdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this, (CResZrle*) m_resource);
	m_resource->m_directUseCount--;
}
