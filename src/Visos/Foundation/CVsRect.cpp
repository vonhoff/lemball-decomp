#include "CVsRect.h"

#include "CVsPoint.h"
#include "CVsSize.h"

// FUNCTION: LEMBALL 0x00442190
CVsRect& CVsRect::operator=(const CVsRect& p_source)
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

// FUNCTION: LEMBALL 0x0044c100
CVsRect* CVsRect::ExpandToInclude(const CVsRect& p_rect)
{
	if ((int) p_rect.m_width * (int) p_rect.m_height == 0) {
		return this;
	}
	{
		short left;
		short rectX = p_rect.m_x;
		left = m_x;
		if (rectX < left) {
			m_width = m_width + (left - rectX);
			m_x = p_rect.m_x;
		}
		short rectWidth;
		short sourceX = p_rect.m_x;
		rectWidth = p_rect.m_width;
		left = m_x;
		short rectRight = rectWidth + sourceX;
		short right = m_width + left;
		if (right < rectRight) {
			m_width = (sourceX - left) + rectWidth;
		}
	}
	{
		short top;
		short rectY = p_rect.m_y;
		top = m_y;
		if (top > rectY) {
			m_height = m_height + (top - rectY);
			m_y = p_rect.m_y;
		}
		short rectHeight = p_rect.m_height;
		short sourceY = p_rect.m_y;
		top = m_y;
		short rectBottom = rectHeight + sourceY;
		short bottom = m_height + top;
		if (bottom < rectBottom) {
			m_height = (rectHeight - top) + sourceY;
		}
	}
	return this;
}

// FUNCTION: LEMBALL 0x00478b80
CVsRect::CVsRect(short p_x, short p_y, CVsSize* p_size) : CVsSize(*p_size), CVsPoint(p_x, p_y)
{
}
