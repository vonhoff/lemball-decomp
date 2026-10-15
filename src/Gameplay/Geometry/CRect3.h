#ifndef LEMBALL_AI_BASE_CRECT3_H
#define LEMBALL_AI_BASE_CRECT3_H

#include "CPt3.h"

// SIZE 0x18
class CRect3 {
public:
	CRect3() : m_min(0, 0, 0), m_max(-1, -1, -1) {}

	CPt3 m_min; // 0x00
	CPt3 m_max; // 0x0c
};

#endif
