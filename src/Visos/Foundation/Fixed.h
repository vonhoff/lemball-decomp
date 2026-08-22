#ifndef LEMBALL_VISOS_FOUNDATION_FIXED_H
#define LEMBALL_VISOS_FOUNDATION_FIXED_H

#include "../../Common.h"

// SIZE 0x04
class Fixed {
public:
	Fixed();

private:
	int m_value; // 0x00
};

Fixed operator+(const Fixed& p_left, const Fixed& p_right);
#endif
