#include "CPVSurface.h"

#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"

// FUNCTION: LEMBALL 0x004663d0
CPVSurface::CPVSurface()
{
	m_dontUpdateRect.m_height = 0;
	m_dontUpdateRect.m_width = 0;
	m_dontUpdateRect.m_y = 0;
	m_dontUpdateRect.m_x = 0;
	m_surfaceRect.m_height = 0;
	m_surfaceRect.m_width = 0;
	m_surfaceRect.m_y = 0;
	m_surfaceRect.m_x = 0;
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

// FUNCTION: LEMBALL 0x00466430
CPVSurface::~CPVSurface()
{
}

// FUNCTION: LEMBALL 0x00466880
void CPVSurface::SetDontUpdateRect(const CVSRect& p_rect)
{
	if (m_dontUpdateRect.m_width * m_dontUpdateRect.m_height == 0 || p_rect.m_height * p_rect.m_width == 0) {
		m_dontUpdateRect.m_width = p_rect.m_width;
		m_dontUpdateRect.m_height = p_rect.m_height;
		const CVSPoint* position = &p_rect;
		m_dontUpdateRect.m_x = position->m_x;
		m_dontUpdateRect.m_y = position->m_y;
	}
}

// FUNCTION: LEMBALL 0x00466920
void CPVSurface::Resize(const CVSSize& p_size)
{
}

// FUNCTION: LEMBALL 0x00466930
bool CPVSurface::HasBackBuff()
{
	return false;
}

// FUNCTION: LEMBALL 0x00466940
bool CPVSurface::HasZBuff()
{
	return false;
}

// FUNCTION: LEMBALL 0x00466950
void CPVSurface::SetWorldWidth(int p_width)
{
}

// FUNCTION: LEMBALL 0x00466960
int CPVSurface::GetWorldWidth()
{
	return 0;
}
