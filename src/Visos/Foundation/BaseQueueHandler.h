#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_BASEQUEUEHANDLER_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_BASEQUEUEHANDLER_H

#include "../../Common.h"

// SIZE 0x10
// VTABLE: LEMBALL 0x00493110
class BaseQueueHandler {
public:
	BaseQueueHandler();
	virtual VsOStream& StreamOut(VsOStream& p_stream); // vtable+0x00
	virtual int ProcessMsg(Message* p_message);        // vtable+0x08
	virtual ~BaseQueueHandler();                       // vtable+0x04

private:
	unsigned int m_signature;      // 0x04
	unsigned int m_dispatchState;  // 0x08
	unsigned int m_processedCount; // 0x0c
};

// Confirmed class-scoped globals.
extern BaseQueueHandler* g_pNetworkStatusQueue;
extern BaseQueueHandler* g_pNetworkPacketQueue;
#endif
