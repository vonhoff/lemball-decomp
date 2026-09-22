#include "CVsSize.h"

// FUNCTION: LEMBALL 0x00442150
CVsSize& CVsSize::operator=(const CVsSize& p_source)
{
	m_width = p_source.m_width;
	m_height = p_source.m_height;
	return *this;
}
