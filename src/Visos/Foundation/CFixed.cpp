#include "CFixed.h"

#include "VsDebug.h"

// FUNCTION: LEMBALL 0x0042b9c0
CFixed operator+(const CFixed& p_left, const CFixed& p_right)
{
	return CFixed(p_left.m_value + p_right.m_value);
}

// FUNCTION: LEMBALL 0x0045a9a0
CFixed::CFixed() : m_value(DEBUG_SENTINEL)
{
}
