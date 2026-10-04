#include "../CHotAreaList.h"

#include "../../Foundation/CBaseQueue.h"
#include "../CHotAreaElement.h"

#include <stddef.h>

extern CVSPoint* g_pHotAreaCursor;
extern int g_nHotAreaListCount;

// FUNCTION: LEMBALL 0x0046a650
CHotAreaList::~CHotAreaList()
{
	CHotAreaElement* entry;
	CHotAreaElement* next;

	entry = m_head;
	for (;;) {
		if (entry == NULL) {
			break;
		}
		next = entry->m_next;
		DeleteEntry(entry);
		entry = next;
	}
	g_pMasterInputQueue->Detach(static_cast<CBaseQueueHandler*>(this), MASTER_INPUT_QUEUE_PRIORITY);
	g_nHotAreaListCount = g_nHotAreaListCount - 1;
	if (g_nHotAreaListCount == 0) {
		operator delete(g_pHotAreaCursor);
	}
}
