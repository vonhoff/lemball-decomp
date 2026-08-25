#include "NetworkOptionsProc.h"

// 68K 0x10808988 __ct__19CNetworkOptionsProcFP5CGame
// STUB: LEMBALL 0x00455050
NetworkOptionsProc::NetworkOptionsProc(Game* p_arg0)
{
}

// 68K 0x10808b14 Start__19CNetworkOptionsProcFv
// STUB: LEMBALL 0x00455130
void NetworkOptionsProc::Start()
{
}

// 68K 0x10808bee StopBroadcast__19CNetworkOptionsProcFv
// STUB: LEMBALL 0x004551d0
void NetworkOptionsProc::StopBroadcast()
{
}

// 68K 0x10808d12 Stop__19CNetworkOptionsProcFv
// STUB: LEMBALL 0x004552a0
void NetworkOptionsProc::Stop()
{
}

// 68K 0x10808dc8 NetworkEvent__19CNetworkOptionsProcF13NetworkEvents
// STUB: LEMBALL 0x00455320
void NetworkOptionsProc::NetworkEvent(int p_event)
{
}

// 68K 0x10808e40 ReceiveCritical__19CNetworkOptionsProcFUlP11CReadPacketP8CConnect
// STUB: LEMBALL 0x00455360
bool NetworkOptionsProc::ReceiveCritical(unsigned long p_id, ReadPacket* p_packet, Connect* p_connection)
{
	return 0;
}

// 68K 0x10808fa6 Accept__19CNetworkOptionsProcFP8CConnectUc
// STUB: LEMBALL 0x00455480
void NetworkOptionsProc::Accept(Connect* p_connection, unsigned char p_ready)
{
}

// 68K 0x10809004 Reject__19CNetworkOptionsProcFP8CConnect
// STUB: LEMBALL 0x004554a0
void NetworkOptionsProc::Reject(Connect* p_connection)
{
}

// 68K 0x1011c4b2 Processing__19CNetworkOptionsProcFv
// STUB: LEMBALL 0x00455ea0
void NetworkOptionsProc::Processing()
{
}

// 68K 0x10808a36 __dt__19CNetworkOptionsProcFv
// SYNTHETIC: LEMBALL 0x00455eb0
// NetworkOptionsProc::`scalar deleting destructor'
NetworkOptionsProc::~NetworkOptionsProc()
{
}

// GLOBAL: LEMBALL 0x0049f140
NetworkOptionsProc* g_pNetworkOptionsProc = 0;
