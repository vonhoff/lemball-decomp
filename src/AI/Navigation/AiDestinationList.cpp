#include "AiDestinationList.h"

#include "AiDestinationEntry.h"

// FUNCTION: LEMBALL 0x00414cd0
void AiDestinationList::RemoveFirst()
{
	if (m_count != 0) {
		for (int i = 0; i < m_count - 1; i++) {
			AiDestinationEntry* entry = &m_entries[i];
			AiDestinationEntry* next = entry + 1;
			entry->m_type = next->m_type;
			entry->m_coordinate.m_xFixed = next->m_coordinate.m_xFixed;
			entry->m_coordinate.m_yFixed = next->m_coordinate.m_yFixed;
			entry->m_coordinate.m_zFixed = next->m_coordinate.m_zFixed;
			entry->m_metadata = next->m_metadata;
		}
		m_count--;
	}
}

// FUNCTION: LEMBALL 0x00414d40
void AiDestinationList::PrependSlot()
{
	for (int i = m_count; i > 0; i--) {
		AiDestinationEntry* entry = &m_entries[i];
		AiDestinationEntry* previous = entry - 1;
		entry->m_type = previous->m_type;
		entry->m_coordinate.m_xFixed = previous->m_coordinate.m_xFixed;
		entry->m_coordinate.m_yFixed = previous->m_coordinate.m_yFixed;
		entry->m_coordinate.m_zFixed = previous->m_coordinate.m_zFixed;
		entry->m_metadata = previous->m_metadata;
	}
	m_count++;
}
