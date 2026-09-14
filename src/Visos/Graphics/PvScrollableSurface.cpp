#include "PvScrollableSurface.h"

#include "../Foundation/VsSize.h"

// 68K 0x102165a8 SetWorldWidth__20CPVScrollableSurfaceFi
// FUNCTION: LEMBALL 0x004668d0
void PvScrollableSurface::SetWorldWidth(int p_width)
{
	short size[2];
	VsRect* rect;

	if (m_reserved40 != p_width) {
		m_reserved40 = p_width;
		rect = &m_rect0c;
		size[0] = rect->m_width;
		size[1] = rect->m_height;
		Resize(*(VsSize*) size);
	}
}

// 68K 0x10105d8e GetWorldWidth__20CPVScrollableSurfaceFv
// FUNCTION: LEMBALL 0x0046db30
int PvScrollableSurface::GetWorldWidth()
{
	return m_reserved40;
}
