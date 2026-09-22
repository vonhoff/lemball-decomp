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
void CVsRect::ExpandToInclude(const CVsRect& p_rect)
{
	if ((int) p_rect.m_width * (int) p_rect.m_height != 0) {
		if (p_rect.m_x < m_x) {
			m_width = m_width + (m_x - p_rect.m_x);
			m_x = p_rect.m_x;
		}
		if ((short) (m_width + m_x) < (short) (p_rect.m_width + p_rect.m_x)) {
			m_width = (p_rect.m_x - m_x) + p_rect.m_width;
		}
		if (p_rect.m_y < m_y) {
			m_height = m_height + (m_y - p_rect.m_y);
			m_y = p_rect.m_y;
		}
		if ((short) (m_height + m_y) < (short) (p_rect.m_y + p_rect.m_height)) {
			m_height = (p_rect.m_height - m_y) + p_rect.m_y;
		}
	}
}

// FUNCTION: LEMBALL 0x00478b80
CVsRect::CVsRect(short p_x, short p_y, CVsSize* p_size) : CVsSize(*p_size), CVsPoint(p_x, p_y)
{
}
