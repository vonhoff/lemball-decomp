#include "Platform/Windows/Network/TcpIp/CTCPIPNetwork.h"

#include "Engine/Queues/CBaseQueue.h"
#include "Platform/Windows/Network/TcpIp/CTCPIPBroadcast.h"
#include "Platform/Windows/Network/TcpIp/CTCPIPConnect.h"
#include "Platform/Windows/Network/TcpIp/CTCPIPNetworkAddress.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Platform/Windows/WinSock/WinSock.h"

#include "Engine/Queues/CBaseQueueHandler.h"
#include "Platform/Windows/ThreadConstants.h"
#include "Platform/Windows/Network/CNetworkWnd.h"

#include <new.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// FUNCTION: LEMBALL 0x004713c0
CTCPIPNetwork::CTCPIPNetwork() : CNetworkWnd("TCPIP Network", &g_tcpIpNetworkWindowClassRegistered)
{
}

// FUNCTION: LEMBALL 0x004713f0
void CTCPIPNetwork::Initialise()
{
	WSADATA wsaData;
	int wsaResult;
	int maxDatagram;

	enum {
		WINSOCK_VERSION_1_1 = 0x0101
	};
	wsaResult = WSAStartup(WINSOCK_VERSION_1_1, &wsaData);
	if (wsaResult != 0) {
		g_lastNetworkError = WSAGetLastError();
		return;
	}

	maxDatagram = wsaData.iMaxUdpDg;
	if ((int) g_networkPacketSize > maxDatagram) {
		g_networkPacketSize = (unsigned int) maxDatagram;
	}
	m_timerId = SetTimer((HWND) m_windowHandle, TCPIP_TIMER_ID, TCPIP_TIMER_INTERVAL_MS, NULL);
}

// FUNCTION: LEMBALL 0x00471460
void CTCPIPNetwork::UnInitialise()
{
	KillTimer((HWND) m_windowHandle, m_timerId);
	WSACleanup();
}

// FUNCTION: LEMBALL 0x00471480
int CTCPIPNetwork::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	(void) p_wParam;
	(void) p_lParam;

	if (p_message != WM_TIMER) {
		if (p_message != TCPIP_MESSAGE_FORCE_PROCESS) {
			return NETWORK_WINDOW_MESSAGE_UNHANDLED;
		}
		if (g_pNetworkStatusQueue != NULL && ((CBaseQueue*) g_pNetworkStatusQueue)->GetMessageCount() != 0) {
			((CBaseQueue*) g_pNetworkStatusQueue)
				->ProcessNMsgs(((CBaseQueue*) g_pNetworkStatusQueue)->GetMessageCount());
		}
	}
	Process();
	return 0;
}

// FUNCTION: LEMBALL 0x004714d0
void CTCPIPNetwork::ForceProcess()
{
	PostMessageA((HWND) m_windowHandle, TCPIP_MESSAGE_FORCE_PROCESS, 0, 0);
}

// FUNCTION: LEMBALL 0x004715c0
void* CTCPIPNetwork::GetNewNetworkAddress()
{
	void* storage;
	CTCPIPNetworkAddress* address;

	storage = operator new(sizeof(CTCPIPNetworkAddress));
	if (storage != NULL) {
		address = new (storage) CTCPIPNetworkAddress();
		address->m_text[0] = '\0';
		return address;
	}
	return NULL;
}

// FUNCTION: LEMBALL 0x004715e0
void* CTCPIPNetwork::GetNewConnect()
{
	return new CTCPIPConnect;
}

// FUNCTION: LEMBALL 0x00471810
void* CTCPIPNetwork::GetNewBroadcast()
{
	return new CTCPIPBroadcast;
}

// GLOBAL: LEMBALL 0x004a23b0
unsigned long g_dwTCPIPNetworkThreadId = THREAD_ID_BEFORE_CREATE;

// GLOBAL: LEMBALL 0x004a23b4
void* g_hTCPIPNetworkThread = NULL;

// GLOBAL: LEMBALL 0x004a23b8
int g_socketWindowClassRegistered = 0;

// GLOBAL: LEMBALL 0x004a23bc
int g_tcpIpNetworkWindowClassRegistered = 0;

// GLOBAL: LEMBALL 0x004a23c4
unsigned int g_tcpIpBytesReceived = 0;
