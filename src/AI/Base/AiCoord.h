#ifndef LEMBALL_AI_BASE_AICOORD_H
#define LEMBALL_AI_BASE_AICOORD_H

#include "../../Common.h"

// SIZE 0x0c
class AiCoord {
public:
	AiCoord();

private:
	int m_xFixed; // 0x00
	int m_yFixed; // 0x04
	int m_zFixed; // 0x08
};

#endif
