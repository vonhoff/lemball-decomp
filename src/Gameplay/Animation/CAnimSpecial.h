#ifndef LEMBALL_AI_OBJECTS_CANIMSPECIAL_H
#define LEMBALL_AI_OBJECTS_CANIMSPECIAL_H

#include <stddef.h>

class CMap;
struct AnimSpecialEntry;
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
