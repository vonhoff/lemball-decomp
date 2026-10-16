#ifndef LEMBALL_AI_BASE_C3DVECTOR_H
#define LEMBALL_AI_BASE_C3DVECTOR_H

#include "Engine/Math/CFixed.h"

// SIZE 0x0c
class C3DVector {
public:
	C3DVector() {}
	C3DVector(const CFixed& p_x, const CFixed& p_y, const CFixed& p_z);
	// FUNCTION: LEMBALL 0x0040c270
	C3DVector& operator=(const C3DVector& p_other)
	{
		m_xFixed = p_other.m_xFixed;
		m_yFixed = p_other.m_yFixed;
		m_zFixed = p_other.m_zFixed;
		return *this;
	}

	CFixed m_xFixed; // 0x00
	CFixed m_yFixed; // 0x04
	CFixed m_zFixed; // 0x08
};

#endif
