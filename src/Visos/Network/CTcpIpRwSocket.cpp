#include "CTcpIpRwSocket.h"

#include "Visos/Network/CRwSocket.h"
#include "Visos/Network/CTcpIpReadSocket.h"
#include "Visos/Network/CTcpIpWriteSocket.h"

// FUNCTION: LEMBALL 0x00470220
int CTcpIpRwSocket::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	int result;

	result = CTcpIpWriteSocket::Process(p_message, p_wParam, p_lParam);
	if (result == -1) {
		result = CTcpIpReadSocket::Process(p_message, p_wParam, p_lParam);
	}
	return result;
}

// FUNCTION: LEMBALL 0x00471bb0
void CTcpIpRwSocket::SendAcknowledgement()
{
	CRwSocket::SendAcknowledgement();
}

// FUNCTION: LEMBALL 0x00471d30
void CTcpIpRwSocket::Closed(int p_notifyPeer)
{
	CRwSocket::Closed(p_notifyPeer);
}

// FUNCTION: LEMBALL 0x00471d70
CNetworkMessage* CTcpIpRwSocket::ReceiveAcknowledgement()
{
	return CRwSocket::ReceiveAcknowledgement();
}
