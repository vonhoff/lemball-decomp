#include "CTcpIpBroadcast.h"

#include "../Foundation/CBaseQueue.h"
#include "../Foundation/CBaseQueueHandler.h"
#include "../Foundation/CVsOStream.h"
#include "CTcpIpNetwork.h"
#include "CTcpIpNetworkAddress.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Network/CBroadcast.h"
#include "Visos/Network/CNetworkAddress.h"
#include "Visos/Network/CTcpIpReadSocket.h"
#include "Visos/Network/CTcpIpRwSocket.h"

#include <memory.h>
#include <new.h>
#include <string.h>

#pragma intrinsic(memcpy, strcpy, strlen)

#include "Platform/WinSock/TcpIpHostEntry.h"
#include "Platform/WinSock/TcpIpServiceEntry.h"
#include "Platform/WinSock/TcpIpSocketAddress.h"
#include "Platform/WinSock/in_addr.h"

struct TcpIpAddressResult {
	unsigned int m_reserved;
	unsigned char m_data[20];
};

struct TcpIpHostAddressResult {
	unsigned int m_reserved;
	in_addr m_address;
};

struct TcpIpStatusResult {
	unsigned int m_reserved;
	Message m_message;
};

union TcpIpLookupResult {
	TcpIpAddressResult m_address;
	TcpIpStatusResult m_status;
};

#include "Platform/WinSock/WinSock.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00470270
CTcpIpBroadcast::CTcpIpBroadcast()
{
	m_specificNameBuffer = 0;
	m_specificNameRequest = 0;
}

// FUNCTION: LEMBALL 0x004704e0
CTcpIpBroadcast::~CTcpIpBroadcast()
{
	if (m_specificNameBuffer != 0) {
		operator delete(m_specificNameBuffer);
	}
}

// FUNCTION: LEMBALL 0x00470580
void CTcpIpBroadcast::GetSpecificAddr(const char* p_name)
{
	CNetworkAddress* address;
	void* storage;

	if (inet_addr(p_name) != 0xffffffff) {
		storage = operator new(sizeof(CTcpIpNetworkAddress));
		if (storage != 0) {
			address = new (storage) CTcpIpNetworkAddress;
			((CTcpIpNetworkAddress*) storage)->m_text[0] = '\0';
		}
		else {
			address = 0;
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
		m_specificNameBuffer = 0;
	}
	m_specificNameBuffer = (char*) operator new(0x400);
	m_specificNameRequest = WSAAsyncGetHostByName(m_windowHandle, 0x441, p_name, m_specificNameBuffer, 0x400);
	if (m_specificNameRequest == 0) {
		SocketError();
	}
}

// FUNCTION: LEMBALL 0x00470650
void CTcpIpBroadcast::GotName(int p_failed)
{
	TcpIpLookupResult result;

	if (p_failed == 0) {
		CNetworkAddress* address;
		TcpIpHostEntry* hostEntry;
		void* storage;

		hostEntry = (TcpIpHostEntry*) m_specificNameBuffer;
		memcpy(result.m_address.m_data, *hostEntry->m_addressList, hostEntry->m_addressLength);
		storage = operator new(sizeof(CTcpIpNetworkAddress));
		if (storage != 0) {
			address = new (storage) CTcpIpNetworkAddress;
			((CTcpIpNetworkAddress*) storage)->m_text[0] = '\0';
		}
		else {
			address = 0;
		}
		m_specificAddress = address;
		strcpy(((CTcpIpNetworkAddress*) address)->m_text,
			   inet_ntoa(*(in_addr*) &(((CTcpIpNetworkAddress*) address)->m_ipv4Address =
										   *(unsigned int*) result.m_address.m_data)));
		address->GetStr();
	}
	else {
		*g_pErrorOutput << "Specified computer name not found\n";
		if (m_addressMode == 2) {
			*(volatile unsigned short*) &result.m_status.m_message.type = 0xd;
			*(volatile int*) &result.m_status.m_message.code = 0xf;
			g_pNetworkStatusQueue->Post(result.m_status.m_message);
		}
	}
	operator delete(m_specificNameBuffer);
	m_specificNameBuffer = 0;
}

// FUNCTION: LEMBALL 0x00470730
bool CTcpIpBroadcast::Start(const char* p_name)
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
	m_asyncBuffer = (char*) operator new(0x400);
	m_writeReady = 1;
	m_asyncRequest = WSAAsyncGetHostByName(m_windowHandle, 0x440, g_szBroadcastPeerName, m_asyncBuffer, 0x400);
	if (m_asyncRequest == 0) {
		SocketError();
		return false;
	}
	return true;
}

// FUNCTION: LEMBALL 0x00470840
void CTcpIpBroadcast::GotHost(int p_failed)
{
	TcpIpHostAddressResult result;

	if (p_failed != 0) {
		*g_pErrorOutput << "Local host name not found\n";
	}
	else {
		TcpIpHostEntry* hostEntry;
		CTcpIpNetworkAddress* address;

		hostEntry = (TcpIpHostEntry*) m_asyncBuffer;
		memcpy(&result.m_address, *hostEntry->m_addressList, hostEntry->m_addressLength);
		address = (CTcpIpNetworkAddress*) g_pBroadcastAddress;
		strcpy(address->m_text, inet_ntoa(*(in_addr*) &(address->m_ipv4Address = result.m_address.s_addr)));
		g_pBroadcastAddress->GetStr();
	}
	g_localHostLookupComplete = 1;
	operator delete(m_asyncBuffer);
	m_asyncBuffer = 0;
	m_socketHandle = socket(2, 2, 0);
	if (m_socketHandle == -1) {
		SocketError();
		CBroadcast::SendFailedInit((NetworkErrors) 1);
		return;
	}
	m_isOpen = 1;
	m_asyncBuffer = (char*) operator new(0x400);
	m_writeReady = 1;
	m_asyncRequest = WSAAsyncGetServByName(m_windowHandle, 0x442, "tftp", "udp", m_asyncBuffer, 0x400);
	if (m_asyncRequest == 0) {
		SocketError();
		CBroadcast::SendFailedInit((NetworkErrors) 2);
	}
}

// FUNCTION: LEMBALL 0x004709c0
void CTcpIpBroadcast::HandleServiceLookupResult(bool p_failed)
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
	m_asyncBuffer = 0;
	option = 1;
	if (setsockopt(m_socketHandle, 0xffff, 0x20, (const char*) &option, sizeof(option)) == -1) {
		SocketError();
		CBroadcast::SendFailedInit((NetworkErrors) 3);
		return;
	}
	address.m_family = 2;
	address.m_port = htons((unsigned short) (m_port + g_broadcastPort));
	address.m_address.s_addr = ((CTcpIpNetworkAddress*) g_pBroadcastAddress)->m_ipv4Address;
	if (bind(m_socketHandle, &address, sizeof(address)) == -1) {
		SocketError();
		CBroadcast::SendFailedInit((NetworkErrors) 4);
		return;
	}
	if (CBroadcast::m_listenEnabled != 0) {
		selectResult = WSAAsyncSelect(m_socketHandle, m_windowHandle, 0x443, 3);
	}
	else {
		selectResult = WSAAsyncSelect(m_socketHandle, m_windowHandle, 0x443, 2);
	}
	if (selectResult == -1) {
		SocketError();
		CBroadcast::SendFailedInit((NetworkErrors) 5);
		return;
	}
	m_readReady = 1;
	m_writeReady = 0;
	CBroadcast::m_lastBroadcastTime = timeGetTime() - 1000;
	message.type = 2;
	message.code = 0;
	g_pNetworkStatusQueue->Post(message);
}

// FUNCTION: LEMBALL 0x00470b90
int CTcpIpBroadcast::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	unsigned int event;
	unsigned int error;
	int result;

	switch (p_message) {
	case 0x440:
		result = OnNameResolved(p_wParam, p_lParam, &m_asyncBuffer);
		if (result != 0xe) {
			GotHost(result == 2);
		}
		return 0;
	case 0x441:
		m_specificNameRequest = 0;
		result = OnNameResolved(p_wParam, p_lParam, &m_specificNameBuffer);
		if (result != 0xe) {
			GotName(result == 2);
		}
		return 0;
	case 0x442:
		result = OnNameResolved(p_wParam, p_lParam, &m_asyncBuffer);
		if (result != 0xe) {
			HandleServiceLookupResult(result == 2);
		}
		return 0;
	case 0x443:
		if (m_socketHandle == -1) {
			return 0;
		}
		event = (unsigned short) p_lParam;
		error = (unsigned short) ((unsigned long) p_lParam >> 16);
		CBaseCommonSocket::SocketError((NetworkErrors) error);
		switch (event) {
		case 1:
			if (error == 0) {
				CTcpIpReadSocket::ReadBuffFrom();
			}
			return 0;
		}
		break;
	}
	return CTcpIpRwSocket::Process(p_message, p_wParam, p_lParam);
}

// FUNCTION: LEMBALL 0x00470d30
void CTcpIpBroadcast::StartListen()
{
	if (CBroadcast::m_listenEnabled == 0) {
		if (m_readReady != 0 && WSAAsyncSelect(m_socketHandle, m_windowHandle, 0x443, 3) == -1) {
			SocketError();
			return;
		}
		CBroadcast::m_listenEnabled = 1;
	}
}

// FUNCTION: LEMBALL 0x00470d80
void CTcpIpBroadcast::StopListen()
{
	if (CBroadcast::m_listenEnabled != 0) {
		if (m_readReady != 0 && WSAAsyncSelect(m_socketHandle, m_windowHandle, 0x443, 2) == -1) {
			SocketError();
			return;
		}
		CBroadcast::m_listenEnabled = 0;
	}
}

// FUNCTION: LEMBALL 0x00471fc0
void CTcpIpBroadcast::Closed(int p_notifyPeer)
{
	CBroadcast::Closed(p_notifyPeer);
}
