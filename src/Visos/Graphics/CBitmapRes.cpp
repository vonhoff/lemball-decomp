#include "CBitmapRes.h"

#include "../Resources/CResBITMAP.h"
#include "CGDI.h"
#include "CSurface.h"
class CBigBitmap;

// FUNCTION: LEMBALL 0x00447380
void CBitmapRes::Draw(CGDI* p_gdi)
{
	CResBITMAP* resource = m_resource;

	if (resource->m_loaded != 0) {
		resource->m_age = 0;
	}
	else {
		resource->LoadData();
	}
	resource->m_directUseCount++;
	m_resource->m_rendered = 1;
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x004473d0
void CBitmapRes::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit((CBigBitmap*) this, m_resource);
	m_resource->m_rendered = 0;
	m_resource->m_directUseCount--;
}
