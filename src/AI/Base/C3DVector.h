#ifndef LEMBALL_AI_BASE_C3DVECTOR_H
#define LEMBALL_AI_BASE_C3DVECTOR_H

#include "../../Common.h"

// SIZE 0x0c
class C3DVector {
public:
	C3DVector() {}
	C3DVector(const Fixed& p_x, const Fixed& p_y, const Fixed& p_z);
	C3DVector& operator=(const C3DVector& p_other);

	int m_xFixed; // 0x00
	int m_yFixed; // 0x04
	int m_zFixed; // 0x08
};

#endif
