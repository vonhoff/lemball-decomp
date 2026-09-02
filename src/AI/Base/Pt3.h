#ifndef LEMBALL_AI_BASE_PT3_H
#define LEMBALL_AI_BASE_PT3_H

#include "../../Common.h"

// SIZE 0x0c
struct Pt3 {
	int m_x; // 0x00
	int m_y; // 0x04
	int m_z; // 0x08
};

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
