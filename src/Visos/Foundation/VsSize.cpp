#include "VsSize.h"

// FUNCTION: LEMBALL 0x00442150
VsSize& VsSize::operator=(const VsSize& p_source)
{
	m_width = p_source.m_width;
	m_height = p_source.m_height;
	return *this;
}

// FUNCTION: LEMBALL 0x0046daf0
VsSize::VsSize(short p_width, short p_height) : m_width(p_width), m_height(p_height)
{
}

// FUNCTION: LEMBALL 0x0046db10
VsSize::VsSize(const VsSize& p_source) : m_width(p_source.m_width), m_height(p_source.m_height)
{
}
