#include "BaseQueueHandler.h"

// 68K 0x10100004 __dt__17CBaseQueueHandlerFv
// SYNTHETIC: LEMBALL 0x004098e0
// BaseQueueHandler::`scalar deleting destructor'
BaseQueueHandler::~BaseQueueHandler()
{
}

// 68K 0x102048e0 __ct__17CBaseQueueHandlerFv
// STUB: LEMBALL 0x00462ea0
BaseQueueHandler::BaseQueueHandler()
{
}

// 68K 0x1020492a ProcessMsg__17CBaseQueueHandlerFP10tagMESSAGE
// STUB: LEMBALL 0x00462ec0
int BaseQueueHandler::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x1020497e StreamOut__17CBaseQueueHandlerFR10CVSOStream
// STUB: LEMBALL 0x00462ed0
VsOStream& BaseQueueHandler::StreamOut(VsOStream& p_stream)
{
	return *(VsOStream*) 0;
}

// GLOBAL: LEMBALL 0x004a1e1c
BaseQueueHandler* g_pNetworkStatusQueue = 0;

// GLOBAL: LEMBALL 0x004a1e20
BaseQueueHandler* g_pNetworkPacketQueue = 0;
