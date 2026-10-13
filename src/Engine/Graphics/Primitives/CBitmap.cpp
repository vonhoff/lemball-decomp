#include "CBitmap.h"

#include "CGDI.h"
#include "Engine/Resources/Types/CResBITMAP.h"
#include "Platform/Windows/Graphics/CSurface.h"

// FUNCTION: LEMBALL 0x00447300
void CBitmap::Draw(CGDI* p_gdi)
{
	CResBITMAP* resource = m_resource;
	CBitmap* self = this;

	resource->Lock();
	p_gdi->AddToList(self);
}

// FUNCTION: LEMBALL 0x00447340
void CBitmap::Render(CGDI* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this, m_resource);
	m_resource->m_directUseCount--;
}
