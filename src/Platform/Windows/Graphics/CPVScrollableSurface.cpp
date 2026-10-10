#include "CPVScrollableSurface.h"

#include "Engine/Math/CVSSize.h"

// FUNCTION: LEMBALL 0x004668d0
void CPVScrollableSurface::SetWorldWidth(int p_width)
{
	if (m_worldWidth != p_width) {
		m_worldWidth = p_width;
		CVSSize size(m_surfaceRect);
		Resize(size);
	}
}

// FUNCTION: LEMBALL 0x0046db30
int CPVScrollableSurface::GetWorldWidth()
{
	return m_worldWidth;
}
