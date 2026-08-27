#include "BaseQueue.h"

BaseQueue::BaseQueue()
{
}

// 68K 0x102049f4 __ct__10CBaseQueueFUi
// STUB: LEMBALL 0x00463020
BaseQueue::BaseQueue(unsigned int p_arg0)
{
}

// 68K 0x10204aac __ct__10CBaseQueueFUiPc
// STUB: LEMBALL 0x004630a0
BaseQueue::BaseQueue(unsigned int p_arg0, char* p_arg1)
{
}

// 68K 0x10204bfa Post__10CBaseQueueFR10tagMESSAGE
// STUB: LEMBALL 0x004631a0
bool BaseQueue::Post(Message& p_arg0)
{
	return 0;
}

// 68K 0x10204cd8 Send__10CBaseQueueFR10tagMESSAGE
// STUB: LEMBALL 0x00463230
bool BaseQueue::Send(Message& p_arg0)
{
	return 0;
}

// 68K 0x10204d68 StreamOut__10CBaseQueueFR10CVSOStream
// FUNCTION: LEMBALL 0x00463280
VsOStream& BaseQueue::StreamOut(VsOStream& p_stream)
{
	return p_stream;
}

// 68K 0x10204d9e Attach__10CBaseQueueFP17CBaseQueueHandleri
// STUB: LEMBALL 0x004632a0
bool BaseQueue::Attach(BaseQueueHandler* p_arg0, int p_arg1)
{
	return 0;
}

// 68K 0x10204ee0 Detach__10CBaseQueueFP17CBaseQueueHandleri
// STUB: LEMBALL 0x004633b0
bool BaseQueue::Detach(BaseQueueHandler* p_arg0, int p_arg1)
{
	return 0;
}

// 68K 0x10204fbe GetNth__10CBaseQueueFP10tagMESSAGEUi
// STUB: LEMBALL 0x00463570
bool BaseQueue::GetNth(Message* p_arg0, unsigned int p_arg1)
{
	return 0;
}

// 68K 0x1020503c PeekNth__10CBaseQueueFP10tagMESSAGEUi
// STUB: LEMBALL 0x004635b0
bool BaseQueue::PeekNth(Message* p_arg0, unsigned int p_arg1)
{
	return 0;
}

// 68K 0x102050dc PutNth__10CBaseQueueFP10tagMESSAGEUi
// STUB: LEMBALL 0x00463610
bool BaseQueue::PutNth(Message* p_arg0, unsigned int p_arg1)
{
	return 0;
}

// 68K 0x102051fe DeleteNth__10CBaseQueueFUi
// STUB: LEMBALL 0x004636e0
bool BaseQueue::DeleteNth(unsigned int p_arg0)
{
	return 0;
}

// 68K 0x10205344 ProcessNMsgs__10CBaseQueueFUi
// STUB: LEMBALL 0x00463810
bool BaseQueue::ProcessNMsgs(unsigned int p_arg0)
{
	return 0;
}

// 68K 0x10205402 Process__10CBaseQueueFP10tagMESSAGE
// STUB: LEMBALL 0x004638a0
bool BaseQueue::Process(Message* p_arg0)
{
	return 0;
}

// 68K 0x10204b66 __dt__10CBaseQueueFv
BaseQueue::~BaseQueue()
{
}

// GLOBAL: LEMBALL 0x004a9360
BaseQueue* g_pMasterInputQueue;

