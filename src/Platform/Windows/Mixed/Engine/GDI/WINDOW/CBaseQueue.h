#ifndef LEMBALL_CBASEQUEUE_H
#define LEMBALL_CBASEQUEUE_H

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"

class CBaseQueue {
public:
	void* m_pVtable00;
	int m_nReserved04;
	void* m_pLockVtable08;
	char m_abCriticalSection0C[0x18];
	int m_nCapacity24;
	int m_nQueued28;
	int m_nClients2C;
	int m_nSerial30;
	int m_nFull34;
	int m_nPosted38;
	int m_nSent3C;
	int m_nUnhandled40;
	RenderDispatchQueueEntry* m_pBuffer44;
	RenderDispatchQueueEntry* m_pEnd48;
	RenderDispatchQueueEntry* m_pHead4C;
	RenderDispatchQueueEntry* m_pTail50;
	RenderDispatchClientNode* m_pHandlers54;

	int PutNth(RenderDispatchQueueEntry* pMessage, unsigned int nIndex);
};

typedef char CBaseQueueSizeCheck[sizeof(CBaseQueue) == 0x58 ? 1 : -1];

#endif
