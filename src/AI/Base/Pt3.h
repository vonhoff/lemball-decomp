#ifndef LEMBALL_AI_BASE_PT3_H
#define LEMBALL_AI_BASE_PT3_H

#include "../../Common.h"

// SIZE 0x0c
struct Pt3 {
#ifdef LEMBALL_INLINE_PT3_VALUE
	Pt3(int p_x, int p_y, int p_z) : m_x(p_x), m_y(p_y), m_z(p_z) {}
#endif
#ifdef LEMBALL_OUTLINE_VIEW_HELPERS
	void InitializeFromAiCoord(const AiCoord& p_coordinate);
#endif

	int m_x; // 0x00
	int m_y; // 0x04
	int m_z; // 0x08
};

#include "Rect3.h"

#endif
