#include "CVSSize.h"

// FUNCTION: LEMBALL 0x00442150
CVSSize& CVSSize::operator=(const CVSSize& p_source)
{
	m_width = p_source.m_width;
	m_height = p_source.m_height;
	return *this;
}
