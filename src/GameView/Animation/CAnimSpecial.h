#ifndef LEMBALL_AI_OBJECTS_CANIMSPECIAL_H
#define LEMBALL_AI_OBJECTS_CANIMSPECIAL_H

#include <stddef.h>

class CMap;
class CGround;
// SIZE 0x0c
struct AnimSpecialEntry {
	short m_x;                         // 0x00
	short m_y;                         // 0x02
	unsigned short m_sortKey;          // 0x04
	unsigned short m_alignmentPadding; // 0x06
	CGround* m_groundEntry;            // 0x08
};
// SIZE 0x08
class CAnimSpecial {
public:
	CAnimSpecial() : m_entries(NULL), m_entryCount(0) {}
	~CAnimSpecial()
	{
		if (m_entries != NULL) {
			operator delete(m_entries);
		}
	}
	void Initialise(CMap* p_map);
	friend class C2D;

private:
	AnimSpecialEntry* m_entries; // 0x00
	int m_entryCount;            // 0x04
};

#endif
