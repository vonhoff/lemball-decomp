#ifndef LEMBALL_AI_NAVIGATION_CAIDESTINATIONLIST_H
#define LEMBALL_AI_NAVIGATION_CAIDESTINATIONLIST_H

#include "CAiDestinationEntry.h"

// SIZE 0x08
class CAiDestinationList {
public:
	void RemoveFirst();
	void PrependSlot();
	CAiDestinationEntry PopFirst()
	{
		CAiDestinationEntry entry(m_entries[0]);
		RemoveFirst();
		return entry;
	}
	unsigned short m_count;         // 0x00
	unsigned short m_capacity;      // 0x02
	CAiDestinationEntry* m_entries; // 0x04
};

#endif
