#include "CVector.h"

#include "FixedPoint.h"
#include "Engine/Diagnostics/VsDebug.h"

// FUNCTION: LEMBALL 0x0040c290
CVector& CVector::operator=(const CVector& p_other)
{
	m_xFixed = p_other.m_xFixed;
	m_yFixed = p_other.m_yFixed;
	return *this;
}

// FUNCTION: LEMBALL 0x0041a3c0
CVector::CVector() : m_xFixed(DEBUG_SENTINEL), m_yFixed(DEBUG_SENTINEL)
{
}

// FUNCTION: LEMBALL 0x00422380
CVector operator*(const CVector& p_vector, int p_scale)
{
	int y = p_vector.m_yFixed * p_scale;
	int x = p_vector.m_xFixed * p_scale;
	return CVector(x, y);
}

// FUNCTION: LEMBALL 0x0044b640
CVector::CVector(long p_x, long p_y)
{
	m_xFixed = p_x << FIXED_POINT_FRACTION_BITS;
	m_yFixed = p_y << FIXED_POINT_FRACTION_BITS;
}

// FUNCTION: LEMBALL 0x0044b660
CVector operator+(const CVector& p_left, const CVector& p_right)
{
	int y = p_right.m_yFixed + p_left.m_yFixed;
	int x = p_right.m_xFixed + p_left.m_xFixed;
	return CVector(x, y);
}
