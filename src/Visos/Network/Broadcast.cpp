#include "Broadcast.h"

#include "../Foundation/VsTime.h"

// 68K 0x1020c60e __ct__10CBroadcastFv
// STUB: LEMBALL 0x00460350
Broadcast::Broadcast()
{
}

// 68K 0x1020c7ac __dt__10CBroadcastFv
// STUB: LEMBALL 0x004604e0
Broadcast::~Broadcast()
{
}

// 68K 0x1020c914 Closed__10CBroadcastFUc
// STUB: LEMBALL 0x004605b0
void Broadcast::Closed(unsigned char p_arg0)
{
}

// 68K 0x1020c956 FindPort__10CBroadcastFPCUc
// STUB: LEMBALL 0x004605d0
short Broadcast::FindPort(const unsigned char* p_arg0)
{
	return 0;
}

// 68K 0x1020c9b8 SetSpecificAddr__10CBroadcastFPCc
// STUB: LEMBALL 0x00460610
void Broadcast::SetSpecificAddr(const char* p_arg0)
{
}

// 68K 0x1020ca24 Initialise__10CBroadcastFPCc
// STUB: LEMBALL 0x00460650
void Broadcast::Initialise(const char* p_arg0)
{
}

// 68K 0x1020cb54 Stop__10CBroadcastFv
// STUB: LEMBALL 0x004607f0
void Broadcast::Stop()
{
}

void Broadcast::Dummy00()
{
}
void Broadcast::Dummy04()
{
}
void Broadcast::Dummy08()
{
}
void Broadcast::Dummy0c()
{
}
void Broadcast::Dummy10()
{
}
void Broadcast::Dummy14()
{
}

// 68K 0x1020cba0 PostRead__10CBroadcastF13NetworkEventsP11CBasePacket
// STUB: LEMBALL 0x00460830
void Broadcast::PostRead(NetworkEvents p_arg0, BasePacket* p_arg1)
{
}

// 68K 0x1020ccae AddToMessage__10CBroadcastFR17CBroadcastMessage
// STUB: LEMBALL 0x004608f0
void Broadcast::AddToMessage(BroadcastMessage& p_arg0)
{
}

// 68K 0x1020cd06 Process__10CBroadcastFv
// STUB: LEMBALL 0x00460910
void Broadcast::Process()
{
}

// 68K 0x1020ce14 SendFailedInit__10CBroadcastF13NetworkErrors
// STUB: LEMBALL 0x004609f0
void Broadcast::SendFailedInit(NetworkErrors p_arg0)
{
}

// 68K 0x1020ce72 Run__10CBroadcastFv
// STUB: LEMBALL 0x00460a20
void Broadcast::Run()
{
	unsigned long time = CurrentMilliTimer();
	m_runEnabled = 1;
	m_lastBroadcastTime = time - 1000;
}

// 68K 0x1020ceaa Suspend__10CBroadcastFv
// FUNCTION: LEMBALL 0x00460a40
void Broadcast::Suspend()
{
	m_runEnabled = 0;
}

// 68K 0x1020ced4 Send__10CBroadcastFP15CNetworkAddressR17CBroadcastMessage
// STUB: LEMBALL 0x00460a50
void Broadcast::Send(NetworkAddress* p_arg0, BroadcastMessage& p_arg1)
{
}

// 68K 0x1010e5ce ResetPort__10CBroadcastFs
// FUNCTION: LEMBALL 0x004629c0
void Broadcast::ResetPort(short p_arg0)
{
	m_connectionData[p_arg0] = 0;
}
