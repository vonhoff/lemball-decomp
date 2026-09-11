#include "PvScrollableSurface.h"

#include "../Foundation/VsSize.h"

// 68K 0x102165a8 SetWorldWidth__20CPVScrollableSurfaceFi
// FUNCTION: LEMBALL 0x004668d0
void PvScrollableSurface::SetWorldWidth(int p_width)
{
	VsSize size;
	VsRect* rect;

	if (m_reserved40 != p_width) {
		m_reserved40 = p_width;
		rect = &m_rect0c;
		size.m_width = rect->m_width;
		size.m_height = rect->m_height;
		Resize(size);
	}
}

// 68K 0x10105d8e GetWorldWidth__20CPVScrollableSurfaceFv
// FUNCTION: LEMBALL 0x0046db30
int PvScrollableSurface::GetWorldWidth()
{
	return m_reserved40;
}
