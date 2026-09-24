#include "../CHotAreaList.h"

#include "../../Foundation/CBaseQueue.h"
#include "../CHotAreaElement.h"

extern CVsPoint* g_pHotAreaCursor;
extern int g_nHotAreaListCount;

// FUNCTION: LEMBALL 0x0046a650
CHotAreaList::~CHotAreaList()
{
	CHotAreaElement* entry;
	CHotAreaElement* next;

	entry = m_head;
	for (;;) {
		if (entry == 0) {
			break;
		}
		next = entry->m_next;
		DeleteEntry(entry);
		entry = next;
	}
	g_pMasterInputQueue->Detach(static_cast<CBaseQueueHandler*>(this), -0x19);
	g_nHotAreaListCount = g_nHotAreaListCount - 1;
	if (g_nHotAreaListCount == 0) {
		operator delete(g_pHotAreaCursor);
	}
}
