#ifndef LEMBALL_VISOS_FOUNDATION_FIXED_H
#define LEMBALL_VISOS_FOUNDATION_FIXED_H

#include "../../Common.h"

// SIZE 0x04
class Fixed {
public:
	Fixed();
	inline Fixed(int p_value) : m_value(p_value) {}
	inline Fixed(const Fixed& p_other) : m_value(p_other.m_value) {}

	int m_value; // 0x00
};

Fixed operator+(const Fixed& p_left, const Fixed& p_right);
#endif
