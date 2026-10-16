#include "CVSRect.h"

#include "CVSPoint.h"

// FUNCTION: LEMBALL 0x00442190
CVSRect& CVSRect::operator=(const CVSRect& p_source)
{
	m_width = p_source.m_width;
	m_height = p_source.m_height;
	const CVSPoint& point = p_source;
	m_x = point.m_x;
	m_y = point.m_y;
	return *this;
}

// FUNCTION: LEMBALL 0x0044c100
CVSRect* CVSRect::ExpandToInclude(const CVSRect& p_rect)
{
	if (p_rect.m_width * p_rect.m_height == 0) {
		return this;
	}
	if (p_rect.m_x < m_x) {
		m_width += m_x - p_rect.m_x;
		m_x = p_rect.m_x;
	}
	short rectRight = p_rect.m_width + p_rect.m_x;
	short right = m_width + m_x;
	if (right < rectRight) {
		m_width = p_rect.m_x - m_x + p_rect.m_width;
	}
	if (p_rect.m_y < m_y) {
		m_height += m_y - p_rect.m_y;
		m_y = p_rect.m_y;
	}
	short rectBottom = p_rect.m_height + p_rect.m_y;
	short bottom = m_height + m_y;
	if (bottom < rectBottom) {
		m_height = p_rect.m_height - m_y + p_rect.m_y;
	}
	return this;
}
