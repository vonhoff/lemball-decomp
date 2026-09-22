#include "CFileRwSocket.h"

#include "Visos/Network/CFileCommonSocket.h"
#include "Visos/Network/CFileReadSocket.h"
#include "Visos/Network/CFileWriteSocket.h"
#include "Visos/Network/CRwSocket.h"

// FUNCTION: LEMBALL 0x0047a220
CFileRwSocket::CFileRwSocket() : CRwSocket(), CFileReadSocket(), CFileWriteSocket()
{
}

// FUNCTION: LEMBALL 0x0047a420
bool CFileRwSocket::SendPacket(const unsigned char* p_data, int p_size)
{
	bool sent = CFileWriteSocket::SendPacket(p_data, p_size);
	if (sent) {
		CFileWriteSocket::m_nextWriteSlot %= CFileCommonSocket::m_headerSlotCount;
	}
	return sent;
}

// FUNCTION: LEMBALL 0x0047ba60
void CFileRwSocket::Closed(int p_notifyPeer)
{
	CRwSocket::Closed(p_notifyPeer);
}

// FUNCTION: LEMBALL 0x0047baa0
void CFileRwSocket::SendAcknowledgement()
{
	CRwSocket::SendAcknowledgement();
}

// FUNCTION: LEMBALL 0x0047bad0
CNetworkMessage* CFileRwSocket::ReceiveAcknowledgement()
{
	return CRwSocket::ReceiveAcknowledgement();
}
