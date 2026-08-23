#ifndef LEMBALL_AI_OBJECTS_SWITCHENTRY_H
#define LEMBALL_AI_OBJECTS_SWITCHENTRY_H

#include "../../Common.h"

// SIZE 0x08
struct SwitchEntry {
	unsigned int m_message;    // 0x00
	unsigned short m_objectId; // 0x04
};

#endif
