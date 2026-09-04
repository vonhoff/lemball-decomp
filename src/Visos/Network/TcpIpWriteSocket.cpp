#include "TcpIpWriteSocket.h"

#include "TcpIpNetwork.h"
#include "TcpIpNetworkAddress.h"

struct TcpIpDestinationAddress {
	unsigned short m_family;
	unsigned short m_port;
	unsigned int m_address;
	unsigned char m_padding[8];
};

extern "C" int __stdcall sendto(int p_socket,
								const char* p_buffer,
								int p_length,
								int p_flags,
								const TcpIpDestinationAddress* p_address,
								int p_addressLength);
extern "C" int __stdcall WSAGetLastError();
extern "C" unsigned short __stdcall htons(unsigned short p_value);
extern "C" unsigned long __stdcall timeGetTime(void);

// 68K 0x1010cbac __ct__17CTCPIPWriteSocketFv
// FUNCTION: LEMBALL 0x00470030
TcpIpWriteSocket::TcpIpWriteSocket()
{
	m_addressFamily = 2;
}

// 68K 0x1010cc62 SetDestAddr__17CTCPIPWriteSocketFP15CNetworkAddress
// FUNCTION: LEMBALL 0x004700f0
void TcpIpWriteSocket::SetDestAddr(NetworkAddress* p_address)
{
	_SetDestAddr(p_address);
	m_destinationIPv4 = ((TcpIpNetworkAddress*) p_address)->m_ipv4Address;
}

// 68K 0x1010ccfe SendPacket__17CTCPIPWriteSocketFPCUci
// FUNCTION: LEMBALL 0x00470120
bool TcpIpWriteSocket::SendPacket(const unsigned char* p_data, int p_size)
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
int TcpIpWriteSocket::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
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
		BaseCommonSocket::SocketError((NetworkErrors) error);
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

// 68K 0x1010dcd0 SetPort__17CTCPIPWriteSocketFs
// FUNCTION: LEMBALL 0x00471c20
void TcpIpWriteSocket::SetPort(short p_port)
{
	WriteSocket::SetPort(p_port);
	m_networkPort = htons((unsigned short) (m_port + g_broadcastPort));
}

// 68K 0x10106290 __dt__17CTCPIPWriteSocketFv
TcpIpWriteSocket::~TcpIpWriteSocket()
{
}

// 68K 0x1010e556 Closed__17CTCPIPWriteSocketFUc
// FUNCTION: LEMBALL 0x00471ee0
void TcpIpWriteSocket::Closed(unsigned char p_notifyPeer)
{
	WriteSocket::Closed(p_notifyPeer);
}
