#include "CVSIOs.h"

// FUNCTION: LEMBALL 0x00458410
CVSIOs::CVSIOs(CVSStreambuf* p_streamBuffer)
	: m_streamBuffer(p_streamBuffer), m_flags(0x14), m_fill(' '), m_width(0), m_radix(VSO_RADIX_DECIMAL)
{
}

// FUNCTION: LEMBALL 0x00458440
CVSIOs::~CVSIOs()
{
}
