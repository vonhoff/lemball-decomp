#include "Platform/Windows/Network/TcpIp/CTCPIPConnect.h"

#include "Engine/Streams/CVSOStream.h"
#include "Platform/Windows/Network/TcpIp/CTCPIPNetwork.h"
#include "Platform/Windows/Network/TcpIp/CTCPIPNetworkAddress.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Multiplayer/Transport/CConnect.h"
#include "Multiplayer/Transport/CNetworkAddress.h"
#include "Platform/Windows/Network/TcpIp/CTCPIPRWSocket.h"
#include "Engine/Time/VsTime.h"

#include <stdlib.h>
#include <string.h>

#pragma intrinsic(memcpy, strcpy, strlen)

#include "Platform/Windows/WinSock/WinSock.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00470dd0
bool CTCPIPConnect::Start(const char* p_localName, const char* p_remoteName)
{
	m_address = (char*) operator new(strlen(p_remoteName) + 1);
	strcpy(m_address, p_remoteName);
	m_asyncBuffer = (char*) operator new(MAXGETHOSTSTRUCT);
	m_name = (char*) operator new(strlen(p_localName) + 1);
	strcpy(m_name, p_localName);
	m_writeReady = 1;
	m_asyncRequest = WSAAsyncGetHostByName(m_windowHandle,
										   TCPIP_MESSAGE_LOCAL_HOST_RESOLVED,
										   p_localName,
										   m_asyncBuffer,
										   MAXGETHOSTSTRUCT);
	if (m_asyncRequest == 0) {
		SocketError();
		return false;
	}
	return true;
}

// FUNCTION: LEMBALL 0x00470ed0
void CTCPIPConnect::GotHost(int p_failed)
{
	if (p_failed == 0) {
		in_addr hostAddress;
		TcpIpHostEntry* hostEntry;

		hostEntry = (TcpIpHostEntry*) m_asyncBuffer;
		memcpy(&hostAddress, *hostEntry->m_addressList, hostEntry->m_addressLength);
		CTCPIPNetworkAddress address;
		address.m_ipv4Address = hostAddress.s_addr;
		strcpy(address.m_text, inet_ntoa(hostAddress));
		SetDestAddr(&address);
		m_port = (short) atoi(m_address);
		if (m_port == 0) {
			m_writeReady = 1;
			m_asyncRequest = WSAAsyncGetServByName(m_windowHandle,
												   TCPIP_MESSAGE_SERVICE_RESOLVED,
												   m_address,
												   "TCP",
												   m_asyncBuffer,
												   MAXGETHOSTSTRUCT);
			if (m_asyncRequest == 0) {
				SocketError();
				return;
			}
		}
		else {
			Connect();
		}
	}
	else {
		*g_pErrorOutput << "Computer specified was not found\n";
	}
}

// FUNCTION: LEMBALL 0x00471000
void CTCPIPConnect::HandleServiceLookupResult(bool p_failed)
{
	if (!p_failed) {
		TcpIpServiceEntry* serviceEntry;

		serviceEntry = (TcpIpServiceEntry*) m_asyncBuffer;
		SetPort((short) (ntohs(serviceEntry->m_port) - g_broadcastPort));
	}
	else {
		SetPort(0);
		*g_pErrorOutput << "Service port number specified was not found\n";
	}
	operator delete(m_asyncBuffer);
	m_asyncBuffer = NULL;
	if (!p_failed) {
		Connect();
	}
}

// FUNCTION: LEMBALL 0x00471090
void CTCPIPConnect::InitSocket()
{
	enum {
		SOCKET_IOCTL_SET_NONBLOCKING = 0x8004667e
	};
	unsigned long nonBlocking;

	m_socketHandle = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socketHandle == NETWORK_SOCKET_HANDLE_INVALID) {
		SocketError();
		return;
	}
	m_isOpen = 1;
	nonBlocking = 1;
	if (ioctlsocket(m_socketHandle, SOCKET_IOCTL_SET_NONBLOCKING, &nonBlocking) == NETWORK_SOCKET_ERROR) {
		SocketError();
	}
}

// FUNCTION: LEMBALL 0x00471110
void CTCPIPConnect::Listen(CNetworkAddress* p_address)
{
	TcpIpSocketAddress address;

	InitSocket();
	SetDestAddr(p_address);
	address.m_family = AF_INET;
	address.m_port = htons((unsigned short) (m_port + g_broadcastPort));
	address.m_address.s_addr = ((CTCPIPNetworkAddress*) g_pBroadcastAddress)->m_ipv4Address;
	if (bind(m_socketHandle, &address, sizeof(address)) == NETWORK_SOCKET_ERROR) {
		SocketError();
		return;
	}
	if (WSAAsyncSelect(m_socketHandle, m_windowHandle, TCPIP_MESSAGE_SOCKET_EVENT, FD_READ | FD_WRITE) ==
		NETWORK_SOCKET_ERROR) {
		SocketError();
		return;
	}
	m_closePending = 1;
	m_eventPending = 1;
	m_isHost = 0;
	CWriteSocket::m_lastSendTime = timeGetTime() - NETWORK_CRITICAL_PACKET_RETRY_INTERVAL_MS;
	CReadSocket::m_lastReceiveTime = timeGetTime();
}

// FUNCTION: LEMBALL 0x00471210
void CTCPIPConnect::Connect()
{
	TcpIpSocketAddress address;

	InitSocket();
	address.m_family = AF_INET;
	address.m_port = htons((unsigned short) (m_port + g_broadcastPort));
	address.m_address.s_addr = ((CTCPIPNetworkAddress*) g_pBroadcastAddress)->m_ipv4Address;
	if (bind(m_socketHandle, &address, sizeof(address)) == NETWORK_SOCKET_ERROR) {
		SocketError();
		return;
	}
	if (WSAAsyncSelect(m_socketHandle, m_windowHandle, TCPIP_MESSAGE_SOCKET_EVENT, FD_READ | FD_WRITE) ==
		NETWORK_SOCKET_ERROR) {
		SocketError();
		return;
	}
	m_eventPending = 1;
	m_isHost = 1;
	CReadSocket::m_lastReceiveTime = CurrentMilliTimer();
}

// FUNCTION: LEMBALL 0x004712e0
int CTCPIPConnect::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	NameResult result;

	if (m_killRequested == 0) {
		switch (p_message) {
		case TCPIP_MESSAGE_LOCAL_HOST_RESOLVED:
			result = OnNameResolved(p_wParam, p_lParam, &m_asyncBuffer);
			if (result != NAME_LOOKUP_ERROR_HANDLED) {
				GotHost(result == NAME_LOOKUP_FAILED);
			}
			return 0;
		case TCPIP_MESSAGE_SERVICE_RESOLVED:
			result = OnNameResolved(p_wParam, p_lParam, &m_asyncBuffer);
			if (result != NAME_LOOKUP_ERROR_HANDLED) {
				HandleServiceLookupResult(result == NAME_LOOKUP_FAILED);
			}
			return 0;
		default:
			return CTCPIPRWSocket::Process(p_message, p_wParam, p_lParam);
		}
	}
	return NETWORK_WINDOW_MESSAGE_UNHANDLED;
}

// FUNCTION: LEMBALL 0x00471b80
void CTCPIPConnect::Closed(int p_notifyPeer)
{
	CConnect::Closed(p_notifyPeer);
}

// FUNCTION: LEMBALL 0x00471be0
CNetworkMessage* CTCPIPConnect::ReceiveAcknowledgement()
{
	return CConnect::ReceiveAcknowledgement();
}
