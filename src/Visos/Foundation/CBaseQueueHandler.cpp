#include "CBaseQueueHandler.h"
struct Message;

// FUNCTION: LEMBALL 0x00462ea0
CBaseQueueHandler::CBaseQueueHandler()
{
	m_dispatchState = 0;
	m_processedCount = 0;
	m_signature = 0x51484452;
}

// FUNCTION: LEMBALL 0x00462ec0
int CBaseQueueHandler::ProcessMsg(Message* p_message)
{
	m_processedCount++;
	return 0;
}

// FUNCTION: LEMBALL 0x00462ed0
CVsOStream& CBaseQueueHandler::StreamOut(CVsOStream& p_stream)
{
	return p_stream;
}

// GLOBAL: LEMBALL 0x004a1e1c
CBaseQueue* g_pNetworkStatusQueue = 0;

// GLOBAL: LEMBALL 0x004a1e20
CBaseQueue* g_pNetworkPacketQueue = 0;
