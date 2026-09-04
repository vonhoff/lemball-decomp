#include "TcpIpReadSocket.h"

#include "../Foundation/VsOStream.h"
#include "TcpIpNetwork.h"
#include "TcpIpNetworkAddress.h"

#include <string.h>

#pragma intrinsic(strcpy)

struct in_addr {
	unsigned long s_addr;
};

struct TcpIpSocketAddress {
	unsigned short m_family;
	unsigned short m_port;
	in_addr m_address;
	unsigned char m_padding[8];
};

struct TcpIpReceiveFromData {
	undefined4 m_reserved;
	int m_addressLength;
	TcpIpSocketAddress m_address;
};

extern "C" int __stdcall recv(int p_socket, char* p_buffer, int p_length, int p_flags);
extern "C" int __stdcall recvfrom(int p_socket,
								  char* p_buffer,
								  int p_length,
								  int p_flags,
								  TcpIpSocketAddress* p_address,
								  int* p_addressLength);
extern "C" int __stdcall WSAGetLastError();
extern "C" char* __stdcall inet_ntoa(in_addr p_address);
extern "C" unsigned long __stdcall timeGetTime(void);

extern unsigned int g_unk0x4a23c4;

// 68K 0x1010c85c ReadBuffFrom__16CTCPIPReadSocketFv
// FUNCTION: LEMBALL 0x0046fe10
bool TcpIpReadSocket::ReadBuffFrom()
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
		TcpIpNetworkAddress* address = (TcpIpNetworkAddress*) g_pBroadcastReceiveAddress;

		strcpy(address->m_text,
			   inet_ntoa(*(in_addr*) &(address->m_ipv4Address = receiveData.m_address.m_address.s_addr)));
	}
	return ProcessPacket();
}

// 68K 0x1010c9c6 ReadBuff__16CTCPIPReadSocketFv
// FUNCTION: LEMBALL 0x0046fee0
bool TcpIpReadSocket::ReadBuff()
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
	g_unk0x4a23c4 += g_receivedPacketSize;
	return ProcessPacket();
}

// 68K 0x1010cb2c Process__16CTCPIPReadSocketFv
// FUNCTION: LEMBALL 0x0046ff90
int TcpIpReadSocket::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
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
		BaseCommonSocket::SocketError((NetworkErrors) error);
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

// 68K 0x101075ba Closed__16CTCPIPReadSocketFUc
// FUNCTION: LEMBALL 0x00471e40
void TcpIpReadSocket::Closed(unsigned char p_notifyPeer)
{
}
