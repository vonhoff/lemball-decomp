#include "PvSurface.h"

// 68K 0x10215dd2 __ct__10CPVSurfaceFv
// FUNCTION: LEMBALL 0x004663d0
PvSurface::PvSurface()
{
	m_dontUpdateRect.m_height = 0;
	m_dontUpdateRect.m_width = 0;
	m_dontUpdateRect.m_y = 0;
	m_dontUpdateRect.m_x = 0;
	m_rect0c.m_height = 0;
	m_rect0c.m_width = 0;
	m_rect0c.m_y = 0;
	m_rect0c.m_x = 0;
	m_windowRect.m_height = 0;
	m_windowRect.m_width = 0;
	m_windowRect.m_y = 0;
	m_windowRect.m_x = 0;
	m_relOriginY = 0;
	m_relOriginX = 0;
	m_updateEnabled = 1;
	m_clipRect.m_height = 0;
	m_reserved40 = 0;
	m_clipRect.m_width = 0;
	m_clipRect.m_y = 0;
	m_clipRect.m_x = 0;
}

// 68K 0x10215e76 __dt__10CPVSurfaceFv
// FUNCTION: LEMBALL 0x00466430
PvSurface::~PvSurface()
{
}

// 68K 0x10216542 SetDontUpdateRect__10CPVSurfaceFRC7CVSRect
// FUNCTION: LEMBALL 0x00466880
void PvSurface::SetDontUpdateRect(const VsRect& p_rect)
{
	const short* coords;

	if ((int) m_dontUpdateRect.m_width * (int) m_dontUpdateRect.m_height == 0 ||
		(int) p_rect.m_height * (int) p_rect.m_width == 0) {
		m_dontUpdateRect.m_width = p_rect.m_width;
		m_dontUpdateRect.m_height = p_rect.m_height;
		if (&p_rect != 0) {
			coords = &p_rect.m_x;
		}
		else {
			coords = 0;
		}
		m_dontUpdateRect.m_x = *coords;
		m_dontUpdateRect.m_y = coords[1];
	}
}

// 68K 0x101012e2 Resize__10CPVSurfaceFRC7CVSSize
// FUNCTION: LEMBALL 0x00466920
void PvSurface::Resize(const VsSize& p_size)
{
}

// 68K 0x1010130c HasBackBuff__10CPVSurfaceFv
// FUNCTION: LEMBALL 0x00466930
bool PvSurface::HasBackBuff()
{
	return 0;
}

// 68K 0x101012bc HasZBuff__10CPVSurfaceFv
// FUNCTION: LEMBALL 0x00466940
bool PvSurface::HasZBuff()
{
	return 0;
}

// 68K 0x10101334 SetWorldWidth__10CPVSurfaceFi
// FUNCTION: LEMBALL 0x00466950
void PvSurface::SetWorldWidth(int p_width)
{
}

// 68K 0x1010135c GetWorldWidth__10CPVSurfaceFv
// FUNCTION: LEMBALL 0x00466960
int PvSurface::GetWorldWidth()
{
	return 0;
}
