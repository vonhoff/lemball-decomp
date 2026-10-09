#include "CBaseQueueHandler.h"

#include <stddef.h>
struct tagMESSAGE;

namespace
{
enum {
	QUEUE_HANDLER_SIGNATURE = 0x51484452
};
}

// FUNCTION: LEMBALL 0x00462ea0
CBaseQueueHandler::CBaseQueueHandler()
{
	m_dispatchState = 0;
	m_processedCount = 0;
	m_signature = QUEUE_HANDLER_SIGNATURE;
}

// FUNCTION: LEMBALL 0x00462ec0
int CBaseQueueHandler::ProcessMsg(tagMESSAGE* p_message)
{
	m_processedCount++;
	return 0;
}

// FUNCTION: LEMBALL 0x00462ed0
CVSOStream& CBaseQueueHandler::StreamOut(CVSOStream& p_stream)
{
	return p_stream;
}

// GLOBAL: LEMBALL 0x004a1e1c
CBaseQueue* g_pNetworkStatusQueue = NULL;

// GLOBAL: LEMBALL 0x004a1e20
CBaseQueue* g_pNetworkPacketQueue = NULL;
