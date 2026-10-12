#ifndef LEMBALL_ENGINE_MATH_CVSSIZEARITHMETIC_H
#define LEMBALL_ENGINE_MATH_CVSSIZEARITHMETIC_H

#include "CVSSize.h"

inline CVSSize operator-(const CVSSize& p_left, const CVSSize& p_right)
{
	return CVSSize(p_left.m_width - p_right.m_width, p_left.m_height - p_right.m_height);
}

inline CVSSize operator/(const CVSSize& p_size, int p_divisor)
{
	return CVSSize(p_size.m_width / p_divisor, p_size.m_height / p_divisor);
}

#endif
