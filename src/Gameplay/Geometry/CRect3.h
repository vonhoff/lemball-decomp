#ifndef LEMBALL_AI_BASE_CRECT3_H
#define LEMBALL_AI_BASE_CRECT3_H

// SIZE 0x18
class CRect3 {
public:
	CRect3() : m_x2(-1), m_y2(-1), m_x1(0), m_y1(0), m_z1(0), m_z2(-1) {}

	int m_x1; // 0x00
	int m_y1; // 0x04
	int m_z1; // 0x08
	int m_x2; // 0x0c
	int m_y2; // 0x10
	int m_z2; // 0x14
};

#endif
