#include "VsRect.h"

#include "VsPoint.h"
#include "VsSize.h"

// FUNCTION: LEMBALL 0x00442130
VsPoint& VsPoint::operator=(const VsPoint& p_source)
{
	m_x = p_source.m_x;
	m_y = p_source.m_y;
	return *this;
}

// FUNCTION: LEMBALL 0x004452a0
VsPoint* VsPoint::AddInPlace(VsPoint* p_delta)
{
	m_x = (short) (m_x + p_delta->m_x);
	m_y = (short) (m_y + p_delta->m_y);
	return this;
}

// FUNCTION: LEMBALL 0x00442190
VsRect& VsRect::operator=(const VsRect& p_source)
{
	const short* coords;

	m_width = p_source.m_width;
	m_height = p_source.m_height;
	if (&p_source != 0) {
		coords = &p_source.m_x;
	}
	else {
		coords = 0;
	}
	m_x = *coords;
	m_y = coords[1];
	return *this;
}

// 68K 0x10108342 __as__7CVSRectFRC7CVSRect
// FUNCTION: LEMBALL 0x0044e6c0
VsRect::VsRect(const VsRect& p_source)
{
	const short* coords;

	m_width = p_source.m_width;
	m_height = p_source.m_height;
	if (&p_source != 0) {
		coords = &p_source.m_x;
	}
	else {
		coords = 0;
	}
	m_x = *coords;
	m_y = coords[1];
}

// FUNCTION: LEMBALL 0x00478b80
VsRect* VsRect::InitFromSizeAndPosition(short p_x, short p_y, VsSize* p_size)
{
	m_width = p_size->m_width;
	m_height = p_size->m_height;
	m_x = p_x;
	m_y = p_y;
	return this;
}
