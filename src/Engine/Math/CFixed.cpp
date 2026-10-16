#include "CFixed.h"

// FUNCTION: LEMBALL 0x0042b9c0
CFixed operator+(const CFixed& p_left, const CFixed& p_right)
{
	return CFixed((int) ((unsigned int) p_left.m_value + (unsigned int) p_right.m_value));
}
