#include "CAnim.h"

#include "Engine/Graphics/Primitives/CGDI.h"
#include "Engine/Resources/Types/CResANIM.h"
#include "Engine/Resources/Types/CResZRLE.h"
#include "Platform/Windows/Graphics/CSurface.h"

// FUNCTION: LEMBALL 0x00443990
CAnim::CAnim()
{
}

// FUNCTION: LEMBALL 0x00443a20
void CAnim::Draw(CGDI* p_gdi)
{
	CResANIM* resource;

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
void CAnim::Render(CGDI* p_gdi)
{
	CResZRLE* entry;

	entry = m_animResource->m_animationEntries + m_animIndex;
	p_gdi->m_renderTarget->Blit(this, entry);
	m_animResource->m_directUseCount = m_animResource->m_directUseCount - 1;
}
