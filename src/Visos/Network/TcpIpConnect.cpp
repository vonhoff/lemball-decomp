#include "TcpIpConnect.h"

#include "../Foundation/VsOStream.h"
#include "TcpIpNetwork.h"
#include "TcpIpNetworkAddress.h"

#include <memory.h>
#include <new.h>
#include <stdlib.h>
#include <string.h>

#pragma intrinsic(memcpy, strcpy, strlen)

struct in_addr {
	unsigned long s_addr;
};

struct TcpIpHostEntry {
	char* m_name;
	char** m_aliases;
	short m_addressType;
	short m_addressLength;
	char** m_addressList;
};

struct TcpIpServiceEntry {
	char* m_name;
	char** m_aliases;
	short m_port;
	char* m_protocol;
};

struct TcpIpSocketAddress {
	unsigned short m_family;
	unsigned short m_port;
	in_addr m_address;
	unsigned char m_padding[8];
};

extern "C" unsigned short __stdcall htons(unsigned short p_value);
extern "C" unsigned short __stdcall ntohs(unsigned short p_value);
extern "C" char* __stdcall inet_ntoa(in_addr p_address);
extern "C" int __stdcall socket(int p_addressFamily, int p_type, int p_protocol);
extern "C" int __stdcall bind(int p_socket, const TcpIpSocketAddress* p_address, int p_addressLength);
extern "C" int __stdcall ioctlsocket(int p_socket, long p_command, unsigned long* p_value);
extern "C" unsigned long __stdcall timeGetTime(void);
extern "C" unsigned int __stdcall WSAAsyncGetHostByName(void* p_window,
														unsigned int p_message,
														const char* p_name,
														char* p_buffer,
														int p_bufferSize);
extern "C" unsigned int __stdcall WSAAsyncGetServByName(void* p_window,
														unsigned int p_message,
														const char* p_service,
														const char* p_protocol,
														char* p_buffer,
														int p_bufferSize);
extern "C" int __stdcall WSAAsyncSelect(int p_socket, void* p_window, unsigned int p_message, long p_events);

// 68K 0x1010ddfa Start__13CTCPIPConnectFPCcPCc
// FUNCTION: LEMBALL 0x00470dd0
bool TcpIpConnect::Start(const char* p_localName, const char* p_remoteName)
{
	m_address = (NetworkAddress*) operator new(strlen(p_remoteName) + 1);
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

// 68K 0x1010def2 GotHost__13CTCPIPConnectFv
// FUNCTION: LEMBALL 0x00470ed0
void TcpIpConnect::GotHost(int p_failed)
{
	if (p_failed == 0) {
		in_addr hostAddress;
		TcpIpHostEntry* hostEntry;

		hostEntry = (TcpIpHostEntry*) m_asyncBuffer;
		memcpy(&hostAddress, *hostEntry->m_addressList, hostEntry->m_addressLength);
		TcpIpNetworkAddress address;
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
void TcpIpConnect::HandleServiceLookupResult(bool p_failed)
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

// 68K 0x1010e02c InitSocket__13CTCPIPConnectFv
// FUNCTION: LEMBALL 0x00471090
void TcpIpConnect::InitSocket()
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

// 68K 0x1010e070 Listen__13CTCPIPConnectFP15CNetworkAddress
// FUNCTION: LEMBALL 0x00471110
void TcpIpConnect::Listen(NetworkAddress* p_address)
{
	TcpIpSocketAddress address;

	InitSocket();
	SetDestAddr(p_address);
	address.m_family = 2;
	address.m_port = htons((unsigned short) (m_port + g_broadcastPort));
	address.m_address.s_addr = ((TcpIpNetworkAddress*) g_pBroadcastAddress)->m_ipv4Address;
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
	WriteSocket::m_lastSendTime = timeGetTime() - 1000;
	ReadSocket::m_lastReceiveTime = timeGetTime();
}

// 68K 0x1010e120 Connect__13CTCPIPConnectFv
// FUNCTION: LEMBALL 0x00471210
void TcpIpConnect::Connect()
{
	TcpIpSocketAddress address;

	InitSocket();
	address.m_family = 2;
	address.m_port = htons((unsigned short) (m_port + g_broadcastPort));
	address.m_address.s_addr = ((TcpIpNetworkAddress*) g_pBroadcastAddress)->m_ipv4Address;
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
	ReadSocket::m_lastReceiveTime = timeGetTime();
}

// 68K 0x1010e182 Process__13CTCPIPConnectFv
// FUNCTION: LEMBALL 0x004712e0
int TcpIpConnect::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	int result;

	if (m_killRequested == 0) {
		switch (p_message) {
		case 0x440:
			result = HandleAsyncNameResolutionResult(p_wParam, p_lParam, &m_asyncBuffer);
			if (result != 0xe) {
				GotHost(result == 2);
			}
			return 0;
		case 0x442:
			result = HandleAsyncNameResolutionResult(p_wParam, p_lParam, &m_asyncBuffer);
			if (result != 0xe) {
				HandleServiceLookupResult(result == 2);
			}
			return 0;
		default:
			return TcpIpRwSocket::Process(p_message, p_wParam, p_lParam);
		}
	}
	return -1;
}

// 68K 0x1010e6a6 __dt__13CTCPIPConnectFv
TcpIpConnect::~TcpIpConnect()
{
}

// 68K 0x1010e964 Closed__13CTCPIPConnectFUc
// FUNCTION: LEMBALL 0x00471b80
void TcpIpConnect::Closed(unsigned char p_notifyPeer)
{
	CConnect::Closed(*(unsigned int*) &p_notifyPeer);
}

// 68K 0x1010e998 ReceiveAcknowledgement__13CTCPIPConnectFv
// FUNCTION: LEMBALL 0x00471be0
NetworkMessage* TcpIpConnect::ReceiveAcknowledgement()
{
	return CConnect::ReceiveAcknowledgement();
}
