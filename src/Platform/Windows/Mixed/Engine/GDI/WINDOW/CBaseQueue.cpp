#include "Platform/Windows/Mixed/Engine/GDI/WINDOW/CBaseQueue.h"

// FUNCTION: LEMBALL 0x00463610
int CBaseQueue::PutNth(RenderDispatchQueueEntry* pMessage, unsigned int nIndex)
{
	RenderDispatchQueueEntry* pInsert;
	RenderDispatchQueueEntry* pDestination;
	RenderDispatchQueueEntry* pSource;
	unsigned int nMoved;

	((void(__fastcall*)(void*)) (*(void***) &m_pLockVtable08)[0])(&m_pLockVtable08);

	pInsert = m_pHead4C + nIndex;
	if (m_pEnd48 <= pInsert) {
		pInsert = (RenderDispatchQueueEntry*)
			((((int) pInsert - (int) m_pEnd48) / (int) sizeof(RenderDispatchQueueEntry)) *
				(int) sizeof(RenderDispatchQueueEntry) + (int) m_pBuffer44);
	}

	if ((unsigned int) m_nQueued28 > nIndex) {
		pDestination = m_pTail50;
		pSource = pDestination - 1;
		nMoved = 0;
		if ((unsigned int) m_nQueued28 - nIndex != 0) {
			do {
				if (pSource < m_pBuffer44) {
					pSource = m_pEnd48 - 1;
				}
				if (pDestination < m_pBuffer44) {
					pDestination = m_pEnd48 - 1;
				}
				*pDestination = *pSource;
				--pSource;
				--pDestination;
				++nMoved;
			} while ((unsigned int) m_nQueued28 - nIndex > nMoved);
		}
	}

	*pInsert = *pMessage;
	++m_pTail50;
	if (m_pEnd48 <= m_pTail50) {
		m_pTail50 = m_pBuffer44;
	}
	++m_nQueued28;

	((void(__fastcall*)(void*)) (*(void***) &m_pLockVtable08)[1])(&m_pLockVtable08);
	return 1;
}
