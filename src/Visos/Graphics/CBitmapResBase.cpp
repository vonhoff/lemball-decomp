#include "CBitmapResBase.h"

#include "../Resources/CResBitmap.h"
#include "CGdi.h"
#include "VsGdi.h"
class CBitmap;

// FUNCTION: LEMBALL 0x00447300
void CBitmapResBase::Draw(CGdi* p_gdi)
{
	register CResBitmap* resource = m_resource;
	register CBitmapResBase* self = this;

	if (resource->m_loaded != 0) {
		resource->m_age = 0;
	}
	else {
		resource->LoadData();
	}
	resource->m_directUseCount++;
	p_gdi->AddToList(self);
}

// FUNCTION: LEMBALL 0x00447340
void CBitmapResBase::Render(CGdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CBitmap*) this, m_resource);
	m_resource->m_directUseCount--;
}
