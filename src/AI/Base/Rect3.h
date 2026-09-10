#ifndef LEMBALL_AI_BASE_RECT3_H
#define LEMBALL_AI_BASE_RECT3_H

#include "../../Common.h"

// SIZE 0x18
class Rect3 {
public:
	int m_x1; // 0x00
	int m_y1; // 0x04
	int m_z1; // 0x08
	int m_x2; // 0x0c
	int m_y2; // 0x10
	int m_z2; // 0x14
};

#endif
