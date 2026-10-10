#include "CFixed.h"

#include "Engine/Diagnostics/VsDebug.h"

// FUNCTION: LEMBALL 0x0042b9c0
CFixed operator+(const CFixed& p_left, const CFixed& p_right)
{
	return CFixed((int) ((unsigned int) p_left.m_value + (unsigned int) p_right.m_value));
}

// FUNCTION: LEMBALL 0x0045a9a0
CFixed::CFixed() : m_value(DEBUG_SENTINEL)
{
}
