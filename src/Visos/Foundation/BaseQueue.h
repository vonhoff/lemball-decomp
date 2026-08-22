#ifndef LEMBALL_VISOS_FOUNDATION_BASEQUEUE_H
#define LEMBALL_VISOS_FOUNDATION_BASEQUEUE_H

#include "../../Common.h"

// SIZE 0x58
// VTABLE: LEMBALL 0x00499250
class BaseQueue {
public:
	BaseQueue(unsigned int p_arg0);
	BaseQueue(unsigned int p_arg0, char* p_arg1);
	bool Attach(BaseQueueHandler* p_arg0, int p_arg1);
	bool DeleteNth(unsigned int p_arg0);
	bool Detach(BaseQueueHandler* p_arg0, int p_arg1);
	bool GetNth(Message* p_arg0, unsigned int p_arg1);
	bool PeekNth(Message* p_arg0, unsigned int p_arg1);
	bool Process(Message* p_arg0);
	bool ProcessNMsgs(unsigned int p_arg0);
	bool PutNth(Message* p_arg0, unsigned int p_arg1);
	virtual VsOStream& StreamOut(VsOStream& p_stream); // vtable+0x00
	virtual bool Post(Message& p_arg0);                // vtable+0x08
	virtual bool Send(Message& p_arg0);                // vtable+0x0c
	virtual ~BaseQueue();                              // vtable+0x04
	BaseQueue();

private:
	undefined4 m_unk0x04;              // 0x04
	void* m_synchronizationVtable;     // 0x08
	undefined m_criticalSection[0x18]; // 0x0c
	unsigned int m_capacity;           // 0x24
	unsigned int m_messageCount;       // 0x28
	unsigned int m_handlerCount;       // 0x2c
	unsigned int m_nextSequence;       // 0x30
	unsigned int m_overflowCount;      // 0x34
	unsigned int m_postCount;          // 0x38
	unsigned int m_sendCount;          // 0x3c
	unsigned int m_unhandledCount;     // 0x40
	unsigned char* m_messageBuffer;    // 0x44
	unsigned char* m_messageBufferEnd; // 0x48
	unsigned char* m_readCursor;       // 0x4c
	unsigned char* m_writeCursor;      // 0x50
	void* m_handlerList;               // 0x54
};

extern BaseQueue* g_pMasterInputQueue;
#endif
