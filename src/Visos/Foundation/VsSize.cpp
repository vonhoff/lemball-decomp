#include "VsSize.h"

// FUNCTION: LEMBALL 0x00442150
VsSize& VsSize::operator=(const VsSize& p_source)
{
	m_width = p_source.m_width;
	m_height = p_source.m_height;
	return *this;
}
