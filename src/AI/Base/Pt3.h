#ifndef LEMBALL_AI_BASE_PT3_H
#define LEMBALL_AI_BASE_PT3_H

class AiCoord;
// SIZE 0x0c
struct Pt3 {
	Pt3() {}
	Pt3(int p_x, int p_y, int p_z) : m_x(p_x), m_y(p_y), m_z(p_z) {}
	void InitializeFromAiCoord(const AiCoord& p_coordinate);

	int m_x; // 0x00
	int m_y; // 0x04
	int m_z; // 0x08
};

#include "Rect3.h"

#endif
