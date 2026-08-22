#include "Connect.h"

// 68K 0x1020cf50 __ct__8CConnectFv
// STUB: LEMBALL 0x00460a90
Connect::Connect()
{
}

// 68K 0x1020d0c6 __dt__8CConnectFv
// STUB: LEMBALL 0x00460c00
Connect::~Connect()
{
}

// 68K 0x1020d1c2 InitConnect__8CConnectFPCcP15CNetworkAddresss
// STUB: LEMBALL 0x00460c60
void Connect::InitConnect(const char* p_arg0, NetworkAddress* p_arg1, short p_arg2)
{
}

// 68K 0x1020d25e CheckConnectTime__8CConnectFv
// STUB: LEMBALL 0x00460ce0
bool Connect::CheckConnectTime()
{
	return 0;
}

// 68K 0x1020d2b6 SetConnectTime__8CConnectFv
// STUB: LEMBALL 0x00460d10
void Connect::SetConnectTime()
{
}

// 68K 0x1020d2ea Stop__8CConnectFv
// STUB: LEMBALL 0x00460d20
void Connect::Stop()
{
}

// 68K 0x1020d348 FirstReceive__8CConnectFv
// STUB: LEMBALL 0x00460d70
void Connect::FirstReceive()
{
}

// 68K 0x1020d414 Send__8CConnectFR15CNetworkMessage
// STUB: LEMBALL 0x00460e40
char Connect::Send(NetworkMessage& p_arg0)
{
	return 0;
}

// 68K 0x1020d4ea Closed__8CConnectFUc
// STUB: LEMBALL 0x00460f00
void Connect::Closed(unsigned char p_arg0)
{
}

// 68K 0x1020d556 ReceiveAcknowledgement__8CConnectFv
// STUB: LEMBALL 0x00460f60
NetworkMessage* Connect::ReceiveAcknowledgement()
{
	return 0;
}

// 68K 0x1020d5cc Kill__8CConnectFv
// STUB: LEMBALL 0x00460fb0
void Connect::Kill()
{
}

// 68K 0x1020d63a PostRead__8CConnectF13NetworkEventsP11CBasePacket
// STUB: LEMBALL 0x00460ff0
void Connect::PostRead(NetworkEvents p_arg0, BasePacket* p_arg1)
{
}

// 68K 0x1020d6a8 Process__8CConnectFv
// STUB: LEMBALL 0x00461030
void Connect::Process()
{
}

// 68K 0x1010e682 ConnectSetup__8CConnectFv
// STUB: LEMBALL 0x004629d0
void Connect::ConnectSetup()
{
}

// GLOBAL: LEMBALL 0x004a011c
Connect* g_pActiveConnection;
