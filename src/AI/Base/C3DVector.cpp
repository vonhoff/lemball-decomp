#define LEMBALL_OUTLINE_VIEW_HELPERS
#include "C3DVector.h"

// FUNCTION: LEMBALL 0x0040c270
C3DVector& C3DVector::operator=(const C3DVector& p_other)
{
	m_xFixed = p_other.m_xFixed;
	m_yFixed = p_other.m_yFixed;
	m_zFixed = p_other.m_zFixed;
	return *this;
}
