#include "CAnim.h"

#include "../Graphics/CGdi.h"
#include "../Graphics/VsGdi.h"
#include "../Resources/CResAnim.h"
class CResZrle;

// FUNCTION: LEMBALL 0x00443990
CAnim::CAnim()
{
}

// FUNCTION: LEMBALL 0x00443a20
void CAnim::Draw(CGdi* p_gdi)
{
	CResAnim* resource;

	resource = m_animResource;
	if (resource->m_loaded != 0) {
		resource->m_age = 0;
	}
	else {
		resource->LoadData();
	}
	resource->m_directUseCount = resource->m_directUseCount + 1;
	p_gdi->AddToList(this);
}

// FUNCTION: LEMBALL 0x00443a60
void CAnim::Render(CGdi* p_gdi)
{
	CResZrle* entry;

	entry = (CResZrle*) ((char*) m_animResource->m_animationEntries + m_animIndex * 0x54);
	p_gdi->m_renderTarget->Blit(this, entry);
	m_animResource->m_directUseCount = m_animResource->m_directUseCount - 1;
}
