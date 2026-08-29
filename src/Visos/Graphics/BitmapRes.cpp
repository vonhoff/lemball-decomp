#include "BitmapRes.h"

#include "../Resources/ResBitmap.h"
#include "Bitmap.h"
#include "Gdi.h"
#include "VsGdi.h"

// STUB: LEMBALL 0x00447120
BitmapRes::BitmapRes()
{
	m_x = 0;
	m_y = 0;
	m_width = 0;
	m_height = 0;
	m_sourceX = 0;
	m_sourceY = 0;
	m_resource = 0;
	m_flags = 0;
	m_remap = 0;
	m_unknown1c = 0;
	m_unknown20 = 0;
}

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

// FUNCTION: LEMBALL 0x0044b5f0
BitmapResBase::BitmapResBase()
	: m_y(0), m_x(0), m_height(0), m_width(0), m_sourceY(0), m_sourceX(0)
{
}
