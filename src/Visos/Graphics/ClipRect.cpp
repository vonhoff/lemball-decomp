#include "ClipRect.h"

#include "Gdi.h"
#include "VsGdi.h"

// 68K 0x10117384 __ct__9CClipRectFv
// FUNCTION: LEMBALL 0x00432a30
ClipRect::ClipRect() : m_top(0), m_left(0), m_bottom(0), m_right(0)
{
}

// 68K 0x10101a8c Draw__9CClipRectFP4CGDI
// FUNCTION: LEMBALL 0x00432ad0
void ClipRect::Draw(Gdi* p_gdi)
{
	p_gdi->AddToList(this);
}

// 68K 0x10101abc Render__9CClipRectFP4CGDI
// FUNCTION: LEMBALL 0x00432ae0
void ClipRect::Render(Gdi* p_gdi)
{
	p_gdi->m_renderTarget->Blit(this);
}

// 68K 0x10101afa __dt__9CClipRectFv
ClipRect::~ClipRect()
{
}
