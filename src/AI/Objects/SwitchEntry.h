#ifndef LEMBALL_AI_OBJECTS_SWITCHENTRY_H
#define LEMBALL_AI_OBJECTS_SWITCHENTRY_H

enum swMessage {
	SW_NONE = 0,
	SW_LIFT = 1,
	SW_LIFTS = 2,
	SW_DOOR = 3,
	SW_MOVER = 4,
	SW_ICE = 5
};

// SIZE 0x08
struct SwitchEntry {
	swMessage m_message;       // 0x00
	unsigned short m_objectId; // 0x04
};

#endif
