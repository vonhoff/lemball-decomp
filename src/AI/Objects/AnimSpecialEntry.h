#ifndef LEMBALL_AI_OBJECTS_ANIMSPECIALENTRY_H
#define LEMBALL_AI_OBJECTS_ANIMSPECIALENTRY_H

class Ground;
// SIZE 0x0c
struct AnimSpecialEntry {
	short m_x;                         // 0x00
	short m_y;                         // 0x02
	unsigned short m_sortKey;          // 0x04
	unsigned short m_alignmentPadding; // 0x06
	Ground* m_groundEntry;             // 0x08
};

#endif
