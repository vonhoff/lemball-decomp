#include "Multiplayer/Transport/FileTransport/CFileRWSocket.h"

#include "Multiplayer/Transport/CRWSocket.h"
#include "Multiplayer/Transport/FileTransport/CFileCommonSocket.h"
#include "Multiplayer/Transport/FileTransport/CFileReadSocket.h"
#include "Multiplayer/Transport/FileTransport/CFileWriteSocket.h"

// FUNCTION: LEMBALL 0x0047a220
CFileRWSocket::CFileRWSocket() : CRWSocket(), CFileReadSocket(), CFileWriteSocket()
{
}

// FUNCTION: LEMBALL 0x0047a420
bool CFileRWSocket::SendPacket(const unsigned char* p_data, int p_size)
{
	bool sent = CFileWriteSocket::SendPacket(p_data, p_size);
	if (sent) {
		m_nextWriteSlot %= m_headerSlotCount;
	}
	return sent;
}

// FUNCTION: LEMBALL 0x0047ba60
void CFileRWSocket::Closed(int p_notifyPeer)
{
	CRWSocket::Closed(p_notifyPeer);
}

// FUNCTION: LEMBALL 0x0047baa0
void CFileRWSocket::SendAcknowledgement()
{
	CRWSocket::SendAcknowledgement();
}

// FUNCTION: LEMBALL 0x0047bad0
CNetworkMessage* CFileRWSocket::ReceiveAcknowledgement()
{
	return CRWSocket::ReceiveAcknowledgement();
}
