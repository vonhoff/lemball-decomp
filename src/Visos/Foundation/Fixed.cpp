#include "Fixed.h"

// 68K 0x10119ede __pl__FRC6CFixedRC6CFixed
// FUNCTION: LEMBALL 0x0042b9c0
Fixed operator+(const Fixed& p_left, const Fixed& p_right)
{
	return Fixed(p_left.m_value + p_right.m_value);
}

// 68K 0x10117fac __ct__6CFixedFv
// FUNCTION: LEMBALL 0x0045a9a0
Fixed::Fixed() : m_value(DEBUG_SENTINEL)
{
}
