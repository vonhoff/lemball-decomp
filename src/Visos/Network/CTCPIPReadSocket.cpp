#include "CTCPIPReadSocket.h"

#include "../Foundation/CVSOStream.h"
#include "CTCPIPNetwork.h"
#include "CTCPIPNetworkAddress.h"
#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Network/CNetworkAddress.h"

#include <string.h>

#pragma intrinsic(strcpy)

#include "Platform/WinSock/TcpIpSocketAddress.h"
#include "Platform/WinSock/in_addr.h"

struct TcpIpReceiveFromData {
	unsigned int m_reserved;
	int m_addressLength;
	TcpIpSocketAddress m_address;
};

#include "Platform/WinSock/WinSock.h"

extern "C" unsigned long __stdcall timeGetTime(void);

extern unsigned int g_tcpIpBytesReceived;

// FUNCTION: LEMBALL 0x0046fe10
bool CTCPIPReadSocket::ReadBuffFrom()
{
	TcpIpReceiveFromData receiveData;
	receiveData.m_addressLength = sizeof(TcpIpSocketAddress);

	g_receivedPacketSize = recvfrom(m_socketHandle,
									(char*) g_pNetworkPacketScratch,
									g_networkPacketSize,
									0,
									&receiveData.m_address,
									&receiveData.m_addressLength);
	if (g_receivedPacketSize == (unsigned int) -1) {
		*g_pErrorOutput << "Receive error (after receive from):" << WSAGetLastError() << "\n";
		SocketError();
		return false;
	}

	{
		CTCPIPNetworkAddress* address = (CTCPIPNetworkAddress*) g_pBroadcastReceiveAddress;

		strcpy(address->m_text,
			   inet_ntoa(*(in_addr*) &(address->m_ipv4Address = receiveData.m_address.m_address.s_addr)));
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
	if (p_message == 0x443) {
		event = (unsigned short) p_lParam;
		if (m_socketHandle == -1) {
			return 0;
		}
		error = (unsigned short) ((unsigned long) p_lParam >> 16);
		CBaseCommonSocket::SocketError((NetworkErrors) error);
		if (event == 1) {
			if (error == 0) {
				ReadBuff();
				return 0;
			}
			*g_pErrorOutput << "Receive error:" << (int) error << "\n";
			return 0;
		}
	}
	return -1;
}

// FUNCTION: LEMBALL 0x00471e40
void CTCPIPReadSocket::Closed(int p_notifyPeer)
{
}
