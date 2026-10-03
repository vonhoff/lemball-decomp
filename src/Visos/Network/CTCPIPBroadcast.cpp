#include "CTCPIPBroadcast.h"

#include "../Foundation/CBaseQueue.h"
#include "../Foundation/CBaseQueueHandler.h"
#include "../Foundation/CVSOStream.h"
#include "CTCPIPNetwork.h"
#include "CTCPIPNetworkAddress.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Network/CBroadcast.h"
#include "Visos/Network/CNetworkAddress.h"
#include "Visos/Network/CTCPIPRWSocket.h"
#include "Visos/Network/CTCPIPReadSocket.h"

#include <new.h>
#include <string.h>

#pragma intrinsic(memcpy, strcpy, strlen)

#include "Platform/WinSock/TcpIpHostEntry.h"
#include "Platform/WinSock/TcpIpServiceEntry.h"
#include "Platform/WinSock/TcpIpSocketAddress.h"
#include "Platform/WinSock/WinSock.h"
#include "Platform/WinSock/in_addr.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00470270
CTCPIPBroadcast::CTCPIPBroadcast()
{
	m_specificNameBuffer = NULL;
	m_specificNameRequest = 0;
}

// FUNCTION: LEMBALL 0x004704e0
CTCPIPBroadcast::~CTCPIPBroadcast()
{
	if (m_specificNameBuffer != NULL) {
		operator delete(m_specificNameBuffer);
	}
}

// FUNCTION: LEMBALL 0x00470580
void CTCPIPBroadcast::GetSpecificAddr(const char* p_name)
{
	CTCPIPNetworkAddress* address;
	void* storage;

	if (inet_addr(p_name) != INADDR_NONE) {
		storage = operator new(sizeof(CTCPIPNetworkAddress));
		if (storage != NULL) {
			address = new (storage) CTCPIPNetworkAddress;
			address->m_text[0] = '\0';
		}
		else {
			address = NULL;
		}
		m_specificAddress = address;
		address->operator=(p_name);
		address->GetStr();
		return;
	}
	if (m_specificNameRequest != 0) {
		if (WSACancelAsyncRequest(m_specificNameRequest) != 0) {
			SocketError();
			return;
		}
		operator delete(m_specificNameBuffer);
		m_specificNameBuffer = NULL;
	}
	m_specificNameBuffer = (char*) operator new(MAXGETHOSTSTRUCT);
	m_specificNameRequest = WSAAsyncGetHostByName(m_windowHandle,
												  TCPIP_MESSAGE_SPECIFIC_HOST_RESOLVED,
												  p_name,
												  m_specificNameBuffer,
												  MAXGETHOSTSTRUCT);
	if (m_specificNameRequest == 0) {
		SocketError();
	}
}

// FUNCTION: LEMBALL 0x00470650
void CTCPIPBroadcast::GotName(int p_failed)
{
	enum {
		STATUS_HOST_LOOKUP_FAILED = 15
	};

	if (p_failed == 0) {
		CTCPIPNetworkAddress* address;
		TcpIpHostEntry* hostEntry;
		void* storage;
		in_addr lookupAddress;

		hostEntry = (TcpIpHostEntry*) m_specificNameBuffer;
		memcpy(&lookupAddress, *hostEntry->m_addressList, hostEntry->m_addressLength);
		storage = operator new(sizeof(CTCPIPNetworkAddress));
		if (storage != NULL) {
			address = new (storage) CTCPIPNetworkAddress;
			address->m_text[0] = '\0';
		}
		else {
			address = NULL;
		}
		m_specificAddress = address;
		in_addr hostAddress = lookupAddress;
		address->m_ipv4Address = hostAddress.s_addr;
		strcpy(address->m_text, inet_ntoa(hostAddress));
		address->GetStr();
	}
	else {
		*g_pErrorOutput << "Specified computer name not found\n";
		if (m_addressMode == BROADCAST_ADDRESS_SPECIFIC) {
			Message message;
			message.m_type = NETWORK_EVENT_HOST_LOOKUP_FAILED;
			message.m_code = STATUS_HOST_LOOKUP_FAILED;
			g_pNetworkStatusQueue->Post(message);
		}
	}
	operator delete(m_specificNameBuffer);
	m_specificNameBuffer = NULL;
}

// FUNCTION: LEMBALL 0x00470730
bool CTCPIPBroadcast::Start(const char* p_name)
{
	char hostName[0x100];

	g_broadcastPort = 0x52f2;
	CBroadcast::Initialise(p_name);
	if (gethostname(hostName, sizeof(hostName)) == -1) {
		SocketError();
		return false;
	}
	g_szBroadcastPeerName = (char*) operator new(strlen(hostName) + 1);
	strcpy(g_szBroadcastPeerName, hostName);
	m_asyncBuffer = (char*) operator new(MAXGETHOSTSTRUCT);
	m_writeReady = 1;
	m_asyncRequest = WSAAsyncGetHostByName(m_windowHandle,
										   TCPIP_MESSAGE_LOCAL_HOST_RESOLVED,
										   g_szBroadcastPeerName,
										   m_asyncBuffer,
										   MAXGETHOSTSTRUCT);
	if (m_asyncRequest == 0) {
		SocketError();
		return false;
	}
	return true;
}

// FUNCTION: LEMBALL 0x00470840
void CTCPIPBroadcast::GotHost(int p_failed)
{
	in_addr lookupAddress;

	if (p_failed != 0) {
		*g_pErrorOutput << "Local host name not found\n";
	}
	else {
		TcpIpHostEntry* hostEntry;
		CTCPIPNetworkAddress* address;

		hostEntry = (TcpIpHostEntry*) m_asyncBuffer;
		memcpy(&lookupAddress, *hostEntry->m_addressList, hostEntry->m_addressLength);
		address = (CTCPIPNetworkAddress*) g_pBroadcastAddress;
		in_addr hostAddress = lookupAddress;
		address->m_ipv4Address = hostAddress.s_addr;
		strcpy(address->m_text, inet_ntoa(hostAddress));
		g_pBroadcastAddress->GetStr();
	}
	g_localHostLookupComplete = 1;
	operator delete(m_asyncBuffer);
	m_asyncBuffer = NULL;
	m_socketHandle = socket(AF_INET, SOCK_DGRAM, 0);
	if (m_socketHandle == -1) {
		SocketError();
		CBroadcast::SendFailedInit((NetworkErrors) 1);
		return;
	}
	m_isOpen = 1;
	m_asyncBuffer = (char*) operator new(MAXGETHOSTSTRUCT);
	m_writeReady = 1;
	m_asyncRequest = WSAAsyncGetServByName(m_windowHandle,
										   TCPIP_MESSAGE_SERVICE_RESOLVED,
										   "tftp",
										   "udp",
										   m_asyncBuffer,
										   MAXGETHOSTSTRUCT);
	if (m_asyncRequest == 0) {
		SocketError();
		CBroadcast::SendFailedInit((NetworkErrors) 2);
	}
}

// FUNCTION: LEMBALL 0x004709c0
void CTCPIPBroadcast::HandleServiceLookupResult(bool p_failed)
{
	int option;
	int selectResult;
	TcpIpSocketAddress address;
	Message message;

	if (p_failed) {
		*g_pErrorOutput << "Failed to determine service port number\n";
		SetPort(0);
	}
	else {
		TcpIpServiceEntry* serviceEntry;

		serviceEntry = (TcpIpServiceEntry*) m_asyncBuffer;
		SetPort((short) (ntohs(serviceEntry->m_port) - g_broadcastPort));
	}
	operator delete(m_asyncBuffer);
	m_asyncBuffer = NULL;
	option = 1;
	if (setsockopt(m_socketHandle, SOL_SOCKET, SO_BROADCAST, (const char*) &option, sizeof(option)) == -1) {
		SocketError();
		CBroadcast::SendFailedInit((NetworkErrors) 3);
		return;
	}
	address.m_family = AF_INET;
	address.m_port = htons((unsigned short) (m_port + g_broadcastPort));
	address.m_address.s_addr = ((CTCPIPNetworkAddress*) g_pBroadcastAddress)->m_ipv4Address;
	if (bind(m_socketHandle, &address, sizeof(address)) == -1) {
		SocketError();
		CBroadcast::SendFailedInit((NetworkErrors) 4);
		return;
	}
	if (CBroadcast::m_listenEnabled != 0) {
		selectResult = WSAAsyncSelect(m_socketHandle, m_windowHandle, TCPIP_MESSAGE_SOCKET_EVENT, FD_READ | FD_WRITE);
	}
	else {
		selectResult = WSAAsyncSelect(m_socketHandle, m_windowHandle, TCPIP_MESSAGE_SOCKET_EVENT, FD_WRITE);
	}
	if (selectResult == -1) {
		SocketError();
		CBroadcast::SendFailedInit((NetworkErrors) 5);
		return;
	}
	m_readReady = 1;
	m_writeReady = 0;
	CBroadcast::m_lastBroadcastTime = timeGetTime() - 1000;
	message.m_type = 2;
	message.m_code = 0;
	g_pNetworkStatusQueue->Post(message);
}

// FUNCTION: LEMBALL 0x00470b90
int CTCPIPBroadcast::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	unsigned int event;
	unsigned int error;
	int result;

	switch (p_message) {
	case TCPIP_MESSAGE_LOCAL_HOST_RESOLVED:
		result = OnNameResolved(p_wParam, p_lParam, &m_asyncBuffer);
		if (result != NAME_LOOKUP_ERROR_HANDLED) {
			GotHost(result == NAME_LOOKUP_FAILED);
		}
		return 0;
	case TCPIP_MESSAGE_SPECIFIC_HOST_RESOLVED:
		m_specificNameRequest = 0;
		result = OnNameResolved(p_wParam, p_lParam, &m_specificNameBuffer);
		if (result != NAME_LOOKUP_ERROR_HANDLED) {
			GotName(result == NAME_LOOKUP_FAILED);
		}
		return 0;
	case TCPIP_MESSAGE_SERVICE_RESOLVED:
		result = OnNameResolved(p_wParam, p_lParam, &m_asyncBuffer);
		if (result != NAME_LOOKUP_ERROR_HANDLED) {
			HandleServiceLookupResult(result == NAME_LOOKUP_FAILED);
		}
		return 0;
	case TCPIP_MESSAGE_SOCKET_EVENT:
		if (m_socketHandle == -1) {
			return 0;
		}
		event = (unsigned short) p_lParam;
		error = (unsigned short) ((unsigned long) p_lParam >> 16);
		CBaseCommonSocket::SocketError((NetworkErrors) error);
		switch (event) {
		case FD_READ:
			if (error == 0) {
				CTCPIPReadSocket::ReadBuffFrom();
			}
			return 0;
		}
		break;
	}
	return CTCPIPRWSocket::Process(p_message, p_wParam, p_lParam);
}

// FUNCTION: LEMBALL 0x00470d30
void CTCPIPBroadcast::StartListen()
{
	if (CBroadcast::m_listenEnabled == 0) {
		if (m_readReady != 0 &&
			WSAAsyncSelect(m_socketHandle, m_windowHandle, TCPIP_MESSAGE_SOCKET_EVENT, FD_READ | FD_WRITE) == -1) {
			SocketError();
			return;
		}
		CBroadcast::m_listenEnabled = 1;
	}
}

// FUNCTION: LEMBALL 0x00470d80
void CTCPIPBroadcast::StopListen()
{
	if (CBroadcast::m_listenEnabled != 0) {
		if (m_readReady != 0 &&
			WSAAsyncSelect(m_socketHandle, m_windowHandle, TCPIP_MESSAGE_SOCKET_EVENT, FD_WRITE) == -1) {
			SocketError();
			return;
		}
		CBroadcast::m_listenEnabled = 0;
	}
}

// FUNCTION: LEMBALL 0x00471fc0
void CTCPIPBroadcast::Closed(int p_notifyPeer)
{
	CBroadcast::Closed(p_notifyPeer);
}
