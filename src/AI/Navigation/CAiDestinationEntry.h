#ifndef LEMBALL_AI_NAVIGATION_CAIDESTINATIONENTRY_H
#define LEMBALL_AI_NAVIGATION_CAIDESTINATIONENTRY_H

#include "../Base/AiCoord.h"

enum eDestinationType {
	DESTINATION_NONE = 0,
	DESTINATION_COORD = 1,
	DESTINATION_OBJECT = 2
};

// SIZE 0x14
class CAiDestinationEntry {
public:
	CAiDestinationEntry();
	// FUNCTION: LEMBALL 0x00414cb0
	AiCoord GetCoordinate() { return m_coordinate; }
	eDestinationType m_type;   // 0x00
	AiCoord m_coordinate;      // 0x04
	unsigned short m_metadata; // 0x10

	friend class CGameObject;
};

#endif
