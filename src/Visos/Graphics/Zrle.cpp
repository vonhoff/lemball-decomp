#include "Zrle.h"

#include "../Resources/ResBase.h"
#include "../Resources/ResZrle.h"
#include "Gdi.h"
#include "VsGdi.h"


// 68K 0x101014e8 Draw__5CZRLEFP4CGDI
// FUNCTION: LEMBALL 0x00467ae0
void Zrle::Draw(Gdi* p_gdi)
{
	ResBase* resource = m_resource;
	if (resource->m_loaded != 0) {
		resource->m_age = 0;
	}
	else {
		resource->LoadData();
	}
	resource->m_directUseCount++;
	p_gdi->AddToList(this);
}

// 68K 0x10101544 Render__5CZRLEFP4CGDI
// FUNCTION: LEMBALL 0x00467b10
void Zrle::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this, (ResZrle*) m_resource);
	m_resource->m_directUseCount--;
}

// 68K 0x10100a7a __dt__5CZRLEFv
Zrle::~Zrle()
{
}

