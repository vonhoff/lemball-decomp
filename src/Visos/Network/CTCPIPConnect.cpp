#include "CTCPIPConnect.h"

#include "../Foundation/CVSOStream.h"
#include "CTCPIPNetwork.h"
#include "CTCPIPNetworkAddress.h"
#include "Visos/Network/CConnect.h"
#include "Visos/Network/CNetworkAddress.h"
#include "Visos/Network/CTCPIPRWSocket.h"

#include <memory.h>
#include <stdlib.h>
#include <string.h>

#pragma intrinsic(memcpy, strcpy, strlen)

#include "Platform/WinSock/TcpIpHostEntry.h"
#include "Platform/WinSock/TcpIpServiceEntry.h"
#include "Platform/WinSock/TcpIpSocketAddress.h"
#include "Platform/WinSock/WinSock.h"
#include "Platform/WinSock/in_addr.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00470dd0
bool CTCPIPConnect::Start(const char* p_localName, const char* p_remoteName)
{
	m_address = (CNetworkAddress*) operator new(strlen(p_remoteName) + 1);
	strcpy((char*) m_address, p_remoteName);
	m_asyncBuffer = (char*) operator new(0x400);
	m_name = (char*) operator new(strlen(p_localName) + 1);
	strcpy(m_name, p_localName);
	m_writeReady = 1;
	m_asyncRequest = WSAAsyncGetHostByName(m_windowHandle, 0x440, p_localName, m_asyncBuffer, 0x400);
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
		address.m_text[0] = '\0';
		strcpy(address.m_text, inet_ntoa(*(in_addr*) &(address.m_ipv4Address = hostAddress.s_addr)));
		SetDestAddr(&address);
		m_port = (short) atoi((char*) m_address);
		if (m_port == 0) {
			m_writeReady = 1;
			m_asyncRequest =
				WSAAsyncGetServByName(m_windowHandle, 0x442, (const char*) m_address, "TCP", m_asyncBuffer, 0x400);
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
	m_asyncBuffer = 0;
	if (!p_failed) {
		Connect();
	}
}

// FUNCTION: LEMBALL 0x00471090
void CTCPIPConnect::InitSocket()
{
	unsigned long nonBlocking;

	m_socketHandle = socket(2, 2, 0);
	if (m_socketHandle == -1) {
		SocketError();
		return;
	}
	m_isOpen = 1;
	nonBlocking = 1;
	if (ioctlsocket(m_socketHandle, 0x8004667e, &nonBlocking) == -1) {
		SocketError();
	}
}

// FUNCTION: LEMBALL 0x00471110
void CTCPIPConnect::Listen(CNetworkAddress* p_address)
{
	TcpIpSocketAddress address;

	InitSocket();
	SetDestAddr(p_address);
	address.m_family = 2;
	address.m_port = htons((unsigned short) (m_port + g_broadcastPort));
	address.m_address.s_addr = ((CTCPIPNetworkAddress*) g_pBroadcastAddress)->m_ipv4Address;
	if (bind(m_socketHandle, &address, sizeof(address)) == -1) {
		SocketError();
		return;
	}
	if (WSAAsyncSelect(m_socketHandle, m_windowHandle, 0x443, 3) == -1) {
		SocketError();
		return;
	}
	m_closePending = 1;
	m_eventPending = 1;
	m_isHost = 0;
	CWriteSocket::m_lastSendTime = timeGetTime() - 1000;
	CReadSocket::m_lastReceiveTime = timeGetTime();
}

// FUNCTION: LEMBALL 0x00471210
void CTCPIPConnect::Connect()
{
	TcpIpSocketAddress address;

	InitSocket();
	address.m_family = 2;
	address.m_port = htons((unsigned short) (m_port + g_broadcastPort));
	address.m_address.s_addr = ((CTCPIPNetworkAddress*) g_pBroadcastAddress)->m_ipv4Address;
	if (bind(m_socketHandle, &address, sizeof(address)) == -1) {
		SocketError();
		return;
	}
	if (WSAAsyncSelect(m_socketHandle, m_windowHandle, 0x443, 3) == -1) {
		SocketError();
		return;
	}
	m_eventPending = 1;
	m_isHost = 1;
	CReadSocket::m_lastReceiveTime = timeGetTime();
}

// FUNCTION: LEMBALL 0x004712e0
int CTCPIPConnect::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	int result;

	if (m_killRequested == 0) {
		switch (p_message) {
		case 0x440:
			result = OnNameResolved(p_wParam, p_lParam, &m_asyncBuffer);
			if (result != 0xe) {
				GotHost(result == 2);
			}
			return 0;
		case 0x442:
			result = OnNameResolved(p_wParam, p_lParam, &m_asyncBuffer);
			if (result != 0xe) {
				HandleServiceLookupResult(result == 2);
			}
			return 0;
		default:
			return CTCPIPRWSocket::Process(p_message, p_wParam, p_lParam);
		}
	}
	return -1;
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
