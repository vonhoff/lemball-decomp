#include "BitmapResBase.h"

#include "../Resources/ResBitmap.h"
#include "Bitmap.h"
#include "Gdi.h"
#include "VsGdi.h"

// FUNCTION: LEMBALL 0x00447300
void BitmapResBase::Draw(Gdi* p_gdi)
{
	register ResBitmap* resource = m_resource;
	register BitmapResBase* self = this;

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
void BitmapResBase::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit((Bitmap*) this, m_resource);
	m_resource->m_directUseCount--;
}
