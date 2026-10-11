#ifndef LEMBALL_VISOS_FOUNDATION_FIXEDPOINTCONVERSION_H
#define LEMBALL_VISOS_FOUNDATION_FIXEDPOINTCONVERSION_H

#include "FixedPoint.h"

inline int IntegerToFixed(const int& p_value)
{
	return (int) ((unsigned int) p_value << FIXED_POINT_FRACTION_BITS);
}

#endif
