#include "CTCPIPReadSocket.h"

#include "Visos/Streams/CVSOStream.h"
#include "CTCPIPNetwork.h"
#include "CTCPIPNetworkAddress.h"
#include "Visos/Network/NetworkConstants.h"
#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Network/CNetworkAddress.h"

#include <string.h>

#pragma intrinsic(strcpy)

#include "Platform/WinSock/TcpIpSocketAddress.h"
#include "Platform/WinSock/WinSock.h"
#include "Platform/WinSock/in_addr.h"

extern "C" unsigned long __stdcall timeGetTime(void);

extern unsigned int g_tcpIpBytesReceived;

// FUNCTION: LEMBALL 0x0046fe10
bool CTCPIPReadSocket::ReadBuffFrom()
{
	int addressLength = sizeof(TcpIpSocketAddress);
	TcpIpSocketAddress sourceAddress;

	g_receivedPacketSize = recvfrom(m_socketHandle,
									(char*) g_pNetworkPacketScratch,
									g_networkPacketSize,
									0,
									&sourceAddress,
									&addressLength);
	if (g_receivedPacketSize == (unsigned int) -1) {
		*g_pErrorOutput << "Receive error (after receive from):" << WSAGetLastError() << "\n";
		SocketError();
		return false;
	}

	{
		CTCPIPNetworkAddress* address = (CTCPIPNetworkAddress*) g_pBroadcastReceiveAddress;
		in_addr senderAddress = sourceAddress.m_address;

		address->m_ipv4Address = senderAddress.s_addr;
		strcpy(address->m_text, inet_ntoa(senderAddress));
	}
	return ProcessPacket();
}

// FUNCTION: LEMBALL 0x0046fee0
bool CTCPIPReadSocket::ReadBuff()
{
	g_receivedPacketSize = recv(m_socketHandle, (char*) g_pNetworkPacketScratch, g_networkPacketSize, 0);
	m_lastReceiveTime = timeGetTime();
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
