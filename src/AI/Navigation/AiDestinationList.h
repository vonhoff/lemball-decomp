#ifndef LEMBALL_AI_NAVIGATION_AIDESTINATIONLIST_H
#define LEMBALL_AI_NAVIGATION_AIDESTINATIONLIST_H

#include "AiDestinationEntry.h"

// SIZE 0x08
class AiDestinationList {
public:
	void RemoveFirst();
	void PrependSlot();
	AiDestinationEntry PopFirst()
	{
		AiDestinationEntry entry(m_entries[0]);
		RemoveFirst();
		return entry;
	}
	unsigned short m_count;        // 0x00
	unsigned short m_capacity;     // 0x02
	AiDestinationEntry* m_entries; // 0x04
};

#endif
