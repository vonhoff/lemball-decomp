#include "CTCPIPWriteSocket.h"

#include "CTCPIPNetwork.h"
#include "CTCPIPNetworkAddress.h"
#include "Visos/Network/CBaseCommonSocket.h"
#include "Visos/Network/CWriteSocket.h"

struct TcpIpDestinationAddress {
	unsigned short m_family;
	unsigned short m_port;
	unsigned int m_address;
	unsigned char m_padding[8];
};

#include "Platform/WinSock/WinSock.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00470030
CTCPIPWriteSocket::CTCPIPWriteSocket()
{
	m_addressFamily = 2;
}

// FUNCTION: LEMBALL 0x004700f0
void CTCPIPWriteSocket::SetDestAddr(CNetworkAddress* p_address)
{
	_SetDestAddr(p_address);
	m_destinationIPv4 = ((CTCPIPNetworkAddress*) p_address)->m_ipv4Address;
}

// FUNCTION: LEMBALL 0x00470120
bool CTCPIPWriteSocket::SendPacket(const unsigned char* p_data, int p_size)
{
	int sent;

	if (m_socketFlags == 0) {
		return false;
	}
	sent = sendto(m_socketHandle,
				  (const char*) p_data,
				  p_size,
				  0,
				  (TcpIpDestinationAddress*) &m_addressFamily,
				  sizeof(TcpIpDestinationAddress));
	m_lastSendTime = timeGetTime();
	if (sent == -1) {
		if (WSAGetLastError() == 0x2733) {
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
	if (p_message == 0x443) {
		if (m_socketHandle == -1) {
			return -1;
		}
		event = (unsigned short) p_lParam;
		error = (unsigned short) ((unsigned long) p_lParam >> 16);
		CBaseCommonSocket::SocketError((NetworkErrors) error);
		switch (event) {
		case 2:
			if (error == 0) {
				m_socketFlags = 1;
			}
			return 0;
		}
	}
	return -1;
}

// FUNCTION: LEMBALL 0x00471c20
void CTCPIPWriteSocket::SetPort(short p_port)
{
	CWriteSocket::SetPort(p_port);
	m_networkPort = htons((unsigned short) (m_port + g_broadcastPort));
}

// FUNCTION: LEMBALL 0x00471ee0
void CTCPIPWriteSocket::Closed(int p_notifyPeer)
{
	CWriteSocket::Closed(p_notifyPeer);
}
