#ifndef LEMBALL_AI_BASE_LOADSHEEPDATA_H
#define LEMBALL_AI_BASE_LOADSHEEPDATA_H

#include "../../Common.h"

// SIZE 0x06
struct LoadSheepData {
public:
	// No functions mapped yet.

	unsigned char m_sheepCount;     // 0x00
	unsigned char m_formationIndex; // 0x01
	unsigned short m_x;             // 0x02
	unsigned short m_y;             // 0x04
};

#endif
