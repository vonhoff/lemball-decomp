#include "CVSSize.h"

// FUNCTION: LEMBALL 0x00442150
CVSSize& CVSSize::operator=(const CVSSize& p_source)
{
	m_width = p_source.m_width;
	m_height = p_source.m_height;
	return *this;
}

// FUNCTION: LEMBALL 0x004452c0
CVSSize& CVSSize::operator-=(const CVSSize& p_delta)
{
	m_width = (short) (m_width - p_delta.m_width);
	m_height = (short) (m_height - p_delta.m_height);
	return *this;
}
