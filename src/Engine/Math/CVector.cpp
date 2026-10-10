#include "CVector.h"

#include "Engine/Diagnostics/VsDebug.h"
#include "FixedPoint.h"

// FUNCTION: LEMBALL 0x0041a3c0
CVector::CVector() : m_xFixed(DEBUG_SENTINEL), m_yFixed(DEBUG_SENTINEL)
{
}

// FUNCTION: LEMBALL 0x00422380
CVector operator*(const CVector& p_vector, int p_scale)
{
	int y = (unsigned int) p_vector.m_yFixed * p_scale;
	int x = (unsigned int) p_vector.m_xFixed * p_scale;
	return CVector(x, y);
}

// FUNCTION: LEMBALL 0x0044b640
CVector::CVector(long p_x, long p_y)
{
	m_xFixed = (unsigned long) p_x << FIXED_POINT_FRACTION_BITS;
	m_yFixed = (unsigned long) p_y << FIXED_POINT_FRACTION_BITS;
}

// FUNCTION: LEMBALL 0x0044b660
CVector operator+(const CVector& p_left, const CVector& p_right)
{
	int y = (unsigned int) p_right.m_yFixed + p_left.m_yFixed;
	int x = (unsigned int) p_right.m_xFixed + p_left.m_xFixed;
	return CVector(x, y);
}
