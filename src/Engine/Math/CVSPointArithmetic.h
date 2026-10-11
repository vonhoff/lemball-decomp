#ifndef LEMBALL_ENGINE_MATH_CVSPOINTARITHMETIC_H
#define LEMBALL_ENGINE_MATH_CVSPOINTARITHMETIC_H

#include "CVSPoint.h"

inline CVSPoint operator-(const CVSPoint& p_left, const CVSPoint& p_right)
{
	return CVSPoint((short) (p_left.m_x - p_right.m_x), (short) (p_left.m_y - p_right.m_y));
}

inline CVSPoint operator/(const CVSPoint& p_point, int p_divisor)
{
	return CVSPoint((short) (p_point.m_x / p_divisor), (short) (p_point.m_y / p_divisor));
}

#endif
