#include "TcpIpBroadcast.h"

#include "../Foundation/BaseQueue.h"
#include "../Foundation/BaseQueueHandler.h"
#include "../Foundation/VsOStream.h"
#include "TcpIpNetwork.h"
#include "TcpIpNetworkAddress.h"

#include <memory.h>
#include <new.h>
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

struct TcpIpAddressResult {
	undefined4 m_reserved;
	unsigned char m_data[20];
};

struct TcpIpHostAddressResult {
	undefined4 m_reserved;
	in_addr m_address;
};

struct TcpIpStatusResult {
	undefined4 m_reserved;
	Message m_message;
};

union TcpIpLookupResult {
	TcpIpAddressResult m_address;
	TcpIpStatusResult m_status;
};

extern "C" unsigned long __stdcall inet_addr(const char* p_text);
extern "C" char* __stdcall inet_ntoa(in_addr p_address);
extern "C" int __stdcall gethostname(char* p_name, int p_nameSize);
extern "C" int __stdcall socket(int p_addressFamily, int p_type, int p_protocol);
extern "C" int __stdcall bind(int p_socket, const TcpIpSocketAddress* p_address, int p_addressLength);
extern "C" unsigned short __stdcall htons(unsigned short p_value);
extern "C" unsigned short __stdcall ntohs(unsigned short p_value);
extern "C" int __stdcall setsockopt(int p_socket, int p_level, int p_option, const char* p_value, int p_valueSize);
extern "C" unsigned long __stdcall timeGetTime(void);
extern "C" int __stdcall WSACancelAsyncRequest(unsigned int p_request);
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

// 68K 0x1010d08a __ct__15CTCPIPBroadcastFv
// FUNCTION: LEMBALL 0x00470270
TcpIpBroadcast::TcpIpBroadcast()
{
	m_specificNameBuffer = 0;
	m_specificNameRequest = 0;
}

// 68K 0x1010d828 GetSpecificAddr__15CTCPIPBroadcastFPCc
// FUNCTION: LEMBALL 0x00470580
void TcpIpBroadcast::GetSpecificAddr(const char* p_name)
{
	NetworkAddress* address;
	void* storage;

	if (inet_addr(p_name) != 0xffffffff) {
		storage = operator new(sizeof(TcpIpNetworkAddress));
		if (storage != 0) {
			address = new (storage) TcpIpNetworkAddress;
			((TcpIpNetworkAddress*) storage)->m_text[0] = '\0';
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

// 68K 0x1010d8c2 GotName__15CTCPIPBroadcastFv
// FUNCTION: LEMBALL 0x00470650
void TcpIpBroadcast::GotName(int p_failed)
{
	TcpIpLookupResult result;

	if (p_failed == 0) {
		NetworkAddress* address;
		TcpIpHostEntry* hostEntry;
		void* storage;

		hostEntry = (TcpIpHostEntry*) m_specificNameBuffer;
		memcpy(result.m_address.m_data, *hostEntry->m_addressList, hostEntry->m_addressLength);
		storage = operator new(sizeof(TcpIpNetworkAddress));
		if (storage != 0) {
			address = new (storage) TcpIpNetworkAddress;
			((TcpIpNetworkAddress*) storage)->m_text[0] = '\0';
		}
		else {
			address = 0;
		}
		m_specificAddress = address;
		strcpy(((TcpIpNetworkAddress*) address)->m_text,
			   inet_ntoa(*(in_addr*) &(((TcpIpNetworkAddress*) address)->m_ipv4Address =
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

// 68K 0x1010d9fa Start__15CTCPIPBroadcastFPCc
// FUNCTION: LEMBALL 0x00470730
bool TcpIpBroadcast::Start(const char* p_name)
{
	char hostName[0x100];

	g_broadcastPort = 0x52f2;
	Broadcast::Initialise(p_name);
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

// 68K 0x1010daf8 GotHost__15CTCPIPBroadcastFv
// FUNCTION: LEMBALL 0x00470840
void TcpIpBroadcast::GotHost(int p_failed)
{
	TcpIpHostAddressResult result;

	if (p_failed != 0) {
		*g_pErrorOutput << "Local host name not found\n";
	}
	else {
		TcpIpHostEntry* hostEntry;
		TcpIpNetworkAddress* address;

		hostEntry = (TcpIpHostEntry*) m_asyncBuffer;
		memcpy(&result.m_address, *hostEntry->m_addressList, hostEntry->m_addressLength);
		address = (TcpIpNetworkAddress*) g_pBroadcastAddress;
		strcpy(address->m_text, inet_ntoa(*(in_addr*) &(address->m_ipv4Address = result.m_address.s_addr)));
		g_pBroadcastAddress->GetStr();
	}
	g_unk0x4a1e44 = 1;
	operator delete(m_asyncBuffer);
	m_asyncBuffer = 0;
	m_socketHandle = socket(2, 2, 0);
	if (m_socketHandle == -1) {
		SocketError();
		Broadcast::SendFailedInit((NetworkErrors) 1);
		return;
	}
	m_isOpen = 1;
	m_asyncBuffer = (char*) operator new(0x400);
	m_writeReady = 1;
	m_asyncRequest = WSAAsyncGetServByName(m_windowHandle, 0x442, "tftp", "udp", m_asyncBuffer, 0x400);
	if (m_asyncRequest == 0) {
		SocketError();
		Broadcast::SendFailedInit((NetworkErrors) 2);
	}
}

// FUNCTION: LEMBALL 0x004709c0
void TcpIpBroadcast::HandleServiceLookupResult(bool p_failed)
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
		Broadcast::SendFailedInit((NetworkErrors) 3);
		return;
	}
	address.m_family = 2;
	address.m_port = htons((unsigned short) (m_port + g_broadcastPort));
	address.m_address.s_addr = ((TcpIpNetworkAddress*) g_pBroadcastAddress)->m_ipv4Address;
	if (bind(m_socketHandle, &address, sizeof(address)) == -1) {
		SocketError();
		Broadcast::SendFailedInit((NetworkErrors) 4);
		return;
	}
	if (Broadcast::m_unk0x14 != 0) {
		selectResult = WSAAsyncSelect(m_socketHandle, m_windowHandle, 0x443, 3);
	}
	else {
		selectResult = WSAAsyncSelect(m_socketHandle, m_windowHandle, 0x443, 2);
	}
	if (selectResult == -1) {
		SocketError();
		Broadcast::SendFailedInit((NetworkErrors) 5);
		return;
	}
	m_readReady = 1;
	m_writeReady = 0;
	Broadcast::m_lastBroadcastTime = timeGetTime() - 1000;
	message.type = 2;
	message.code = 0;
	g_pNetworkStatusQueue->Post(message);
}

// 68K 0x1010dd7e Process__15CTCPIPBroadcastFv
// FUNCTION: LEMBALL 0x00470b90
int TcpIpBroadcast::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	unsigned int event;
	unsigned int error;
	int result;

	switch (p_message) {
	case 0x440:
		result = HandleAsyncNameResolutionResult(p_wParam, p_lParam, &m_asyncBuffer);
		if (result != 0xe) {
			GotHost(result == 2);
		}
		return 0;
	case 0x441:
		m_specificNameRequest = 0;
		result = HandleAsyncNameResolutionResult(p_wParam, p_lParam, &m_specificNameBuffer);
		if (result != 0xe) {
			GotName(result == 2);
		}
		return 0;
	case 0x442:
		result = HandleAsyncNameResolutionResult(p_wParam, p_lParam, &m_asyncBuffer);
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
		BaseCommonSocket::SocketError((NetworkErrors) error);
		if (event == 1) {
			if (error == 0) {
				TcpIpReadSocket::ReadBuffFrom();
			}
			return 0;
		}
		break;
	}
	return TcpIpRwSocket::Process(p_message, p_wParam, p_lParam);
}

// 68K 0x1010dd0a StartListen__15CTCPIPBroadcastFv
// FUNCTION: LEMBALL 0x00470d30
void TcpIpBroadcast::StartListen()
{
	if (Broadcast::m_unk0x14 == 0) {
		if (m_readReady != 0 && WSAAsyncSelect(m_socketHandle, m_windowHandle, 0x443, 3) == -1) {
			SocketError();
			return;
		}
		Broadcast::m_unk0x14 = 1;
	}
}

// 68K 0x1010dd46 StopListen__15CTCPIPBroadcastFv
// FUNCTION: LEMBALL 0x00470d80
void TcpIpBroadcast::StopListen()
{
	if (Broadcast::m_unk0x14 != 0) {
		if (m_readReady != 0 && WSAAsyncSelect(m_socketHandle, m_windowHandle, 0x443, 2) == -1) {
			SocketError();
			return;
		}
		Broadcast::m_unk0x14 = 0;
	}
}

// 68K 0x1010d556 __dt__15CTCPIPBroadcastFv
TcpIpBroadcast::~TcpIpBroadcast()
{
}

// 68K 0x1010e602 Closed__15CTCPIPBroadcastFUc
// FUNCTION: LEMBALL 0x00471fc0
void TcpIpBroadcast::Closed(unsigned char p_notifyPeer)
{
	Broadcast::Closed(*(unsigned int*) &p_notifyPeer);
}
