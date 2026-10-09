#ifndef LEMBALL_AI_BASE_AICOORD_H
#define LEMBALL_AI_BASE_AICOORD_H

#include "Engine/Diagnostics/VsDebug.h"

// SIZE 0x0c
class AICOORD {
public:
	AICOORD()
	{
		m_xFixed = DEBUG_SENTINEL;
		m_yFixed = DEBUG_SENTINEL;
		m_zFixed = DEBUG_SENTINEL;
	}

	AICOORD(int p_x, int p_y, int p_z)
	{
		m_xFixed = p_x;
		m_yFixed = p_y;
		m_zFixed = p_z;
	}
	// FUNCTION: LEMBALL 0x0040c2b0
	AICOORD& operator=(const AICOORD& p_other)
	{
		m_xFixed = p_other.m_xFixed;
		m_yFixed = p_other.m_yFixed;
		m_zFixed = p_other.m_zFixed;
		return *this;
	}
	AICOORD(const AICOORD& p_other);

	int m_xFixed; // 0x00
	int m_yFixed; // 0x04
	int m_zFixed; // 0x08
};

// SYNTHETIC: LEMBALL 0x0041c360
// AICOORD::AICOORD

#endif
