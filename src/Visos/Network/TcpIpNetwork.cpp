#include "TcpIpNetwork.h"

// 68K 0x1010e1d0 __ct__13CTCPIPNetworkFv
// STUB: LEMBALL 0x004713c0
TcpIpNetwork::TcpIpNetwork()
{
	// STRING: LEMBALL 0x004a2754 "TCPIP Network"
}

// 68K 0x1010e20e Initialise__13CTCPIPNetworkFv
// STUB: LEMBALL 0x004713f0
void TcpIpNetwork::Initialise()
{
}

// 68K 0x1010e2d2 UnInitialise__13CTCPIPNetworkFv
// STUB: LEMBALL 0x00471460
void TcpIpNetwork::UnInitialise()
{
}

// 68K 0x1010e324 Process__13CTCPIPNetworkFv
// STUB: LEMBALL 0x00471480
int TcpIpNetwork::Process()
{
	return 0;
}

// 68K 0x1010e3fa ForceProcess__13CTCPIPNetworkFv
// STUB: LEMBALL 0x004714d0
void TcpIpNetwork::ForceProcess()
{
}

// 68K 0x1010ea20 GetNewNetworkAddress__13CTCPIPNetworkFv
// STUB: LEMBALL 0x004715c0
TcpIpNetworkAddress* TcpIpNetwork::GetNewNetworkAddress()
{
	return 0;
}

// 68K 0x1010ea7c GetNewConnect__13CTCPIPNetworkFv
// STUB: LEMBALL 0x004715e0
TcpIpConnect* TcpIpNetwork::GetNewConnect()
{
	return 0;
}

// 68K 0x1010ef5e GetNewBroadcast__13CTCPIPNetworkFv
// STUB: LEMBALL 0x00471810
TcpIpBroadcast* TcpIpNetwork::GetNewBroadcast()
{
	return 0;
}

// 68K 0x1010efea __dt__13CTCPIPNetworkFv
// SYNTHETIC: LEMBALL 0x00471a10
// TcpIpNetwork::`scalar deleting destructor'
TcpIpNetwork::~TcpIpNetwork()
{
}

// Confirmed class-scoped globals.
// GLOBAL: LEMBALL 0x004a23b0
unsigned int g_dwTCPIPNetworkThreadId;

// GLOBAL: LEMBALL 0x004a23b4
void* g_hTCPIPNetworkThread;

// GLOBAL: LEMBALL 0x004a1e24
unsigned int g_networkPacketSize;

// GLOBAL: LEMBALL 0x004a1e28
BasePacketHeader* g_pNetworkPacketScratch;

// GLOBAL: LEMBALL 0x004a1e2c
unsigned int g_receivedPacketSize;

// GLOBAL: LEMBALL 0x004a1e34
unsigned char* g_pBroadcastPacketTemplate;

// GLOBAL: LEMBALL 0x004a1e38
char* g_szBroadcastPeerName;

// GLOBAL: LEMBALL 0x004a1e3c
unsigned int g_broadcastPayloadLength;

// GLOBAL: LEMBALL 0x004a1e48
unsigned short g_broadcastPort;

// GLOBAL: LEMBALL 0x004a1e4c
int g_lastNetworkError;
