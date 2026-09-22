#include "CTcpIpNetwork.h"

#include "../Foundation/CBaseQueue.h"
#include "CTcpIpBroadcast.h"
#include "CTcpIpConnect.h"
#include "CTcpIpNetworkAddress.h"
#include "Platform/WinSock/WinSock.h"
#include "Visos/Foundation/CBaseQueueHandler.h"
#include "Visos/Network/CBaseNetwork.h"
#include "Visos/Target/Network/CNetworkWnd.h"

#include <new.h>

extern "C" __declspec(dllimport) int __stdcall PostMessageA(void* p_window,
															unsigned int p_message,
															unsigned int p_wParam,
															long p_lParam);
extern "C" __declspec(dllimport) unsigned int __stdcall SetTimer(void* p_window,
																 unsigned int p_id,
																 unsigned int p_elapse,
																 void* p_callback);
extern "C" __declspec(dllimport) int __stdcall KillTimer(void* p_window, unsigned int p_id);

// FUNCTION: LEMBALL 0x004713c0
CTcpIpNetwork::CTcpIpNetwork() : CNetworkWnd("TCPIP Network", &g_unk0x4a23bc)
{
}

#pragma pack(1)
struct WinsockStartupData {
	unsigned short m_version;
	unsigned short m_highVersion;
	char m_description[257];
	char m_systemStatus[129];
	unsigned short m_maxSockets;
	unsigned short m_maxDatagram;
	char* m_vendorInfo;
};
#pragma pack()

// FUNCTION: LEMBALL 0x004713f0
void CTcpIpNetwork::Initialise()
{
	WinsockStartupData wsaData;
	int wsaResult;
	int maxDatagram;

	wsaResult = WSAStartup(0x101, &wsaData);
	if (wsaResult != 0) {
		g_lastNetworkError = WSAGetLastError();
		return;
	}

	maxDatagram = wsaData.m_maxDatagram;
	if (maxDatagram < (int) g_networkPacketSize) {
		g_networkPacketSize = (unsigned int) maxDatagram;
	}
	m_timerId = SetTimer(m_windowHandle, 0x12345678, 10, 0);
}

// FUNCTION: LEMBALL 0x00471460
void CTcpIpNetwork::UnInitialise()
{
	KillTimer(m_windowHandle, m_timerId);
	WSACleanup();
}

// FUNCTION: LEMBALL 0x00471480
int CTcpIpNetwork::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	(void) p_wParam;
	(void) p_lParam;

	if (p_message != 0x113) {
		if (p_message != 0x444) {
			return -1;
		}
		if (g_pNetworkStatusQueue != 0 && ((CBaseQueue*) g_pNetworkStatusQueue)->GetMessageCount() != 0) {
			((CBaseQueue*) g_pNetworkStatusQueue)
				->ProcessNMsgs(((CBaseQueue*) g_pNetworkStatusQueue)->GetMessageCount());
		}
	}
	CBaseNetwork& network = *(CBaseNetwork*) ((unsigned char*) this + sizeof(CNetworkWnd));
	network.Process();
	return 0;
}

// FUNCTION: LEMBALL 0x004714d0
void CTcpIpNetwork::ForceProcess()
{
	PostMessageA(m_windowHandle, 0x444, 0, 0);
}

// FUNCTION: LEMBALL 0x004715c0
void* CTcpIpNetwork::GetNewNetworkAddress()
{
	void* storage;
	CTcpIpNetworkAddress* address;

	storage = operator new(sizeof(CTcpIpNetworkAddress));
	if (storage != 0) {
		address = new (storage) CTcpIpNetworkAddress();
		address->m_text[0] = '\0';
		return address;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x004715e0
void* CTcpIpNetwork::GetNewConnect()
{
	return new CTcpIpConnect;
}

// FUNCTION: LEMBALL 0x00471810
void* CTcpIpNetwork::GetNewBroadcast()
{
	return new CTcpIpBroadcast;
}

// GLOBAL: LEMBALL 0x004a23b0
unsigned int g_dwTCPIPNetworkThreadId = 0x12345678;

// GLOBAL: LEMBALL 0x004a23b4
void* g_hTCPIPNetworkThread = 0;

// GLOBAL: LEMBALL 0x004a23b8
int g_unk0x4a23b8 = 0;

// GLOBAL: LEMBALL 0x004a23bc
int g_unk0x4a23bc = 0;

// GLOBAL: LEMBALL 0x004a23c4
unsigned int g_unk0x4a23c4 = 0;

// GLOBAL: LEMBALL 0x004a1e24
unsigned int g_networkPacketSize = 0;

// GLOBAL: LEMBALL 0x004a1e28
BasePacketHeader* g_pNetworkPacketScratch = 0;

// GLOBAL: LEMBALL 0x004a1e2c
unsigned int g_receivedPacketSize = 0;

// GLOBAL: LEMBALL 0x004a1e34
BasePacketHeader* g_pBroadcastPacketTemplate = 0;

// GLOBAL: LEMBALL 0x004a1e38
char* g_szBroadcastPeerName = 0;

// GLOBAL: LEMBALL 0x004a1e3c
unsigned int g_broadcastPayloadLength = 0;

// GLOBAL: LEMBALL 0x004a1e44
unsigned int g_unk0x4a1e44 = 0;

// GLOBAL: LEMBALL 0x004a1e48
unsigned short g_broadcastPort = 0;

// GLOBAL: LEMBALL 0x004a1e4c
int g_lastNetworkError = 0;
