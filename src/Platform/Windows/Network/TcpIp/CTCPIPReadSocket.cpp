#include "Platform/Windows/Network/TcpIp/CTCPIPReadSocket.h"

#include "Engine/Streams/CVSOStream.h"
#include "Engine/Time/VsTime.h"
#include "Multiplayer/Transport/CBaseCommonSocket.h"
#include "Multiplayer/Transport/CBaseNetwork.h"
#include "Multiplayer/Transport/CNetworkAddress.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Platform/Windows/Network/TcpIp/CTCPIPNetworkAddress.h"
#include "Platform/Windows/WindowsTypes.h"

#include <string.h>
#include <winsock.h>

extern unsigned int g_tcpIpBytesReceived;

// FUNCTION: LEMBALL 0x0046fe10
bool CTCPIPReadSocket::ReadBuffFrom()
{
	int addressLength = sizeof(sockaddr_in);
	sockaddr_in sourceAddress;

	g_receivedPacketSize = recvfrom(m_socketHandle,
									(char*) g_pNetworkPacketScratch,
									g_networkPacketSize,
									0,
									(sockaddr*) &sourceAddress,
									&addressLength);
	if (g_receivedPacketSize == (unsigned int) -1) {
		*g_pErrorOutput << "Receive error (after receive from):" << WSAGetLastError() << "\n";
		SocketError();
		return false;
	}

	{
		CTCPIPNetworkAddress* address = (CTCPIPNetworkAddress*) g_pBroadcastReceiveAddress;
		address->m_ipv4Address = sourceAddress.sin_addr.s_addr;
		strcpy(address->m_text, inet_ntoa(address->Get()));
	}
	return ProcessPacket();
}

// FUNCTION: LEMBALL 0x0046fee0
bool CTCPIPReadSocket::ReadBuff()
{
	g_receivedPacketSize = recv(m_socketHandle, (char*) g_pNetworkPacketScratch, g_networkPacketSize, 0);
	m_lastReceiveTime = CurrentMilliTimer();
	if (g_receivedPacketSize == (unsigned int) -1) {
		*g_pErrorOutput << "Receive error (after receive):" << WSAGetLastError() << "\n";
		SocketError();
		return false;
	}
	if (m_readReady == 0) {
		FirstReceive();
	}
	g_tcpIpBytesReceived += g_receivedPacketSize;
	return ProcessPacket();
}

// FUNCTION: LEMBALL 0x0046ff90
int CTCPIPReadSocket::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	unsigned int event;
	unsigned int error;

	(void) p_wParam;
	if (p_message == TCPIP_MESSAGE_SOCKET_EVENT) {
		event = (unsigned short) p_lParam;
		if (m_socketHandle == NETWORK_SOCKET_HANDLE_INVALID) {
			return 0;
		}
		error = (unsigned short) ((unsigned long) p_lParam >> 16);
		CBaseCommonSocket::SocketError((NetworkErrors) error);
		if (event == FD_READ) {
			if (error == 0) {
				ReadBuff();
				return 0;
			}
			*g_pErrorOutput << "Receive error:" << (int) error << "\n";
			return 0;
		}
	}
	return NETWORK_WINDOW_MESSAGE_UNHANDLED;
}

// FUNCTION: LEMBALL 0x00471e40
void CTCPIPReadSocket::Closed(int p_notifyPeer)
{
}
