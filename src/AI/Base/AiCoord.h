#ifndef LEMBALL_AI_BASE_AICOORD_H
#define LEMBALL_AI_BASE_AICOORD_H

#include "../../Common.h"

// SIZE 0x0c
class AiCoord {
public:
	inline AiCoord()
	{
		m_xFixed = DEBUG_SENTINEL;
		m_yFixed = DEBUG_SENTINEL;
		m_zFixed = DEBUG_SENTINEL;
	}

	int m_xFixed; // 0x00
	int m_yFixed; // 0x04
	int m_zFixed; // 0x08
};

// 68K 0x10119314 __ct__7AICOORDFv
// SYNTHETIC: LEMBALL 0x0041c360
// AiCoord::AiCoord


#endif

