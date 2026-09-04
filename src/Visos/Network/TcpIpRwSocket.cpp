#include "TcpIpRwSocket.h"

// FUNCTION: LEMBALL 0x00470220
int TcpIpRwSocket::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	int result;

	result = TcpIpWriteSocket::Process(p_message, p_wParam, p_lParam);
	if (result == -1) {
		result = TcpIpReadSocket::Process(p_message, p_wParam, p_lParam);
	}
	return result;
}

// FUNCTION: LEMBALL 0x00471830
TcpIpRwSocket::TcpIpRwSocket()
{
}

// 68K 0x1010e9d8 SendAcknowledgement__14CTCPIPRWSocketFv
// FUNCTION: LEMBALL 0x00471bb0
void TcpIpRwSocket::SendAcknowledgement()
{
	RwSocket::SendAcknowledgement();
}

// 68K 0x1010e590 Closed__14CTCPIPRWSocketFUc
// FUNCTION: LEMBALL 0x00471d30
void TcpIpRwSocket::Closed(unsigned char p_notifyPeer)
{
	RwSocket::Closed(*(unsigned int*) &p_notifyPeer);
}

// 68K 0x1010e638 ReceiveAcknowledgement__14CTCPIPRWSocketFv
// FUNCTION: LEMBALL 0x00471d70
NetworkMessage* TcpIpRwSocket::ReceiveAcknowledgement()
{
	return RwSocket::ReceiveAcknowledgement();
}
