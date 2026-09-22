#include "CTCPIPRWSocket.h"

#include "Visos/Network/CRwSocket.h"
#include "Visos/Network/CTCPIPReadSocket.h"
#include "Visos/Network/CTCPIPWriteSocket.h"

// FUNCTION: LEMBALL 0x00470220
int CTCPIPRWSocket::Process(unsigned int p_message, unsigned int p_wParam, long p_lParam)
{
	int result;

	result = CTCPIPWriteSocket::Process(p_message, p_wParam, p_lParam);
	if (result == -1) {
		result = CTCPIPReadSocket::Process(p_message, p_wParam, p_lParam);
	}
	return result;
}

// FUNCTION: LEMBALL 0x00471bb0
void CTCPIPRWSocket::SendAcknowledgement()
{
	CRwSocket::SendAcknowledgement();
}

// FUNCTION: LEMBALL 0x00471d30
void CTCPIPRWSocket::Closed(int p_notifyPeer)
{
	CRwSocket::Closed(p_notifyPeer);
}

// FUNCTION: LEMBALL 0x00471d70
CNetworkMessage* CTCPIPRWSocket::ReceiveAcknowledgement()
{
	return CRwSocket::ReceiveAcknowledgement();
}
