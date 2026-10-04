#include "Platform/Windows/Network/TcpIp/CTCPIPWriteSocket.h"

#include "Platform/Windows/Network/TcpIp/CTCPIPNetwork.h"
#include "Platform/Windows/Network/TcpIp/CTCPIPNetworkAddress.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Platform/Windows/WinSock/WinSock.h"
#include "Multiplayer/Transport/CBaseCommonSocket.h"
#include "Multiplayer/Transport/CWriteSocket.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00470030
CTCPIPWriteSocket::CTCPIPWriteSocket()
{
	m_destination.m_family = AF_INET;
}

// FUNCTION: LEMBALL 0x004700f0
void CTCPIPWriteSocket::SetDestAddr(CNetworkAddress* p_address)
{
	_SetDestAddr(p_address);
	m_destination.m_address.s_addr = ((CTCPIPNetworkAddress*) p_address)->m_ipv4Address;
}

// FUNCTION: LEMBALL 0x00470120
bool CTCPIPWriteSocket::SendPacket(const unsigned char* p_data, int p_size)
{
	int sent;

	if (m_socketFlags == 0) {
		return false;
	}
	sent = sendto(m_socketHandle, (const char*) p_data, p_size, 0, &m_destination, sizeof(m_destination));
	m_lastSendTime = timeGetTime();
	if (sent == NETWORK_SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			return false;
		}
		SocketError();
		return false;
	}
	return true;
}

// FUNCTION: LEMBALL 0x004701a0
int CTCPIPWriteSocket::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	unsigned int event;
	unsigned int error;

	(void) p_wParam;
	if (p_message == TCPIP_MESSAGE_SOCKET_EVENT) {
		if (m_socketHandle == NETWORK_SOCKET_HANDLE_INVALID) {
			return NETWORK_WINDOW_MESSAGE_UNHANDLED;
		}
		event = (unsigned short) p_lParam;
		error = (unsigned short) ((unsigned long) p_lParam >> 16);
		CBaseCommonSocket::SocketError((NetworkErrors) error);
		switch (event) {
		case FD_WRITE:
			if (error == 0) {
				m_socketFlags = 1;
			}
			return 0;
		}
	}
	return NETWORK_WINDOW_MESSAGE_UNHANDLED;
}

// FUNCTION: LEMBALL 0x00471c20
void CTCPIPWriteSocket::SetPort(short p_port)
{
	CWriteSocket::SetPort(p_port);
	m_destination.m_port = htons((unsigned short) (m_port + g_broadcastPort));
}

// FUNCTION: LEMBALL 0x00471ee0
void CTCPIPWriteSocket::Closed(int p_notifyPeer)
{
	CWriteSocket::Closed(p_notifyPeer);
}
