#include "VsRect.h"

#include "VsPoint.h"

// FUNCTION: LEMBALL 0x00442130
VsPoint& VsPoint::operator=(const VsPoint& p_source)
{
	m_x = p_source.m_x;
	m_y = p_source.m_y;
	return *this;
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

