#include "CPt3.h"

#include "AICOORD.h"
#include "Engine/Math/FixedPoint.h"

// FUNCTION: LEMBALL 0x00429e50
void CPt3::InitializeFromAiCoord(const AICOORD& p_coordinate)
{
	m_x = p_coordinate.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	m_y = p_coordinate.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	m_z = p_coordinate.m_zFixed >> FIXED_POINT_FRACTION_BITS;
}
