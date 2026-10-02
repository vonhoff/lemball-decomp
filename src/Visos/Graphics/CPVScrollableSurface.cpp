#include "CPVScrollableSurface.h"

#include "Visos/Foundation/CVSRect.h"

struct CVSSize;

// FUNCTION: LEMBALL 0x004668d0
void CPVScrollableSurface::SetWorldWidth(int p_width)
{
	short size[2];
	CVSRect* rect;

	if (m_reserved40 != p_width) {
		m_reserved40 = p_width;
		rect = &m_surfaceRect;
		size[0] = rect->m_width;
		size[1] = rect->m_height;
		Resize(*(CVSSize*) size);
	}
}

// FUNCTION: LEMBALL 0x0046db30
int CPVScrollableSurface::GetWorldWidth()
{
	return m_reserved40;
}
