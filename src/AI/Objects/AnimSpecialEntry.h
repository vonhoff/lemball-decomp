#ifndef LEMBALL_AI_OBJECTS_ANIMSPECIALENTRY_H
#define LEMBALL_AI_OBJECTS_ANIMSPECIALENTRY_H

#include "../../Common.h"

// SIZE 0x0c
struct AnimSpecialEntry {
public:
	// No functions mapped yet.

	short m_x;                     // 0x00
	short m_y;                     // 0x02
	short m_sortKey;               // 0x04
	undefined2 m_alignmentPadding; // 0x06
	void* m_groundEntry;           // 0x08
};

#endif
