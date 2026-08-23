#ifndef LEMBALL_AI_NAVIGATION_AIDESTINATIONENTRY_H
#define LEMBALL_AI_NAVIGATION_AIDESTINATIONENTRY_H

#include "../../Common.h"
#include "../Base/AiCoord.h" // complete type

// SIZE 0x14
class AiDestinationEntry {
private:
	eDestinationType m_type;   // 0x00
	AiCoord m_coordinate;      // 0x04
	unsigned short m_metadata; // 0x10
};

#endif
