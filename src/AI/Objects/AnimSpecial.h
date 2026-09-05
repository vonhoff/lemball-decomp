#ifndef LEMBALL_AI_OBJECTS_ANIMSPECIAL_H
#define LEMBALL_AI_OBJECTS_ANIMSPECIAL_H

#include "../../Common.h"

// SIZE 0x08
class AnimSpecial {
public:
	// Inlined in Ai::Restart at 0x0041170d.
	AnimSpecial() : m_entries(0), m_entryCount(0) {}
	unsigned int Initialise(Map* p_map);

private:
	AnimSpecialEntry* m_entries; // 0x00
	int m_entryCount;            // 0x04
};

#endif
