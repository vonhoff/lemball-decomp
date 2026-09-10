#include "BitmapRes.h"

#include "../Resources/ResBitmap.h"
#include "Bitmap.h"
#include "Gdi.h"
#include "VsGdi.h"

// FUNCTION: LEMBALL 0x00447380
void BitmapRes::Draw(Gdi* p_gdi)
{
	ResBitmap* resource = m_resource;

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
void BitmapRes::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((BigBitmap*) this, m_resource);
	m_resource->m_rendered = 0;
	m_resource->m_directUseCount--;
}
