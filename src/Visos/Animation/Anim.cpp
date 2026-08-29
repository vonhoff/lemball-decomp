#include "Anim.h"

#include "../Graphics/Gdi.h"
#include "../Graphics/VsGdi.h"
#include "../Resources/ResAnim.h"

// 68K 0x10115b4e __ct__5CAnimFv
// FUNCTION: LEMBALL 0x00443990
Anim::Anim()
{
}

// 68K 0x101017c0 Draw__5CAnimFP4CGDI
// FUNCTION: LEMBALL 0x00443a20
void Anim::Draw(Gdi* p_gdi)
{
	ResAnim* resource;

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

// 68K 0x10101830 Render__5CAnimFP4CGDI
// FUNCTION: LEMBALL 0x00443a60
void Anim::Render(Gdi* p_gdi)
{
	ResZrle* entry;

	entry = (ResZrle*) ((char*) m_animResource->m_animationEntries + m_animIndex * 0x54);
	p_gdi->m_renderTarget->Blit(this, entry);
	m_animResource->m_directUseCount = m_animResource->m_directUseCount - 1;
}

// 68K 0x10101896 __dt__5CAnimFv
Anim::~Anim()
{
}
