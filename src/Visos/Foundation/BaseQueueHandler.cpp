#include "BaseQueueHandler.h"

// 68K 0x102048e0 __ct__17CBaseQueueHandlerFv
// FUNCTION: LEMBALL 0x00462ea0
BaseQueueHandler::BaseQueueHandler()
{
	m_dispatchState = 0;
	m_processedCount = 0;
	m_signature = 0x51484452;
}

// 68K 0x1020492a ProcessMsg__17CBaseQueueHandlerFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00462ec0
int BaseQueueHandler::ProcessMsg(Message* p_message)
{
	m_processedCount++;
	return 0;
}

// 68K 0x1020497e StreamOut__17CBaseQueueHandlerFR10CVSOStream
// FUNCTION: LEMBALL 0x00462ed0
VsOStream& BaseQueueHandler::StreamOut(VsOStream& p_stream)
{
	return p_stream;
}

// GLOBAL: LEMBALL 0x004a1e1c
BaseQueue* g_pNetworkStatusQueue = 0;

// GLOBAL: LEMBALL 0x004a1e20
BaseQueue* g_pNetworkPacketQueue = 0;
