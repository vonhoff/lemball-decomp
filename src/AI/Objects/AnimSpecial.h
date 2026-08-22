#ifndef LEMBALL_SCAFFOLD_AI_OBJECTS_ANIMSPECIAL_H
#define LEMBALL_SCAFFOLD_AI_OBJECTS_ANIMSPECIAL_H

#include "../../Common.h"

// SIZE 0x08
class AnimSpecial {
public:
	unsigned int Initialise(Map* p_map);

private:
	AnimSpecialEntry* m_entries; // 0x00
	int m_entryCount;            // 0x04
};

#endif
