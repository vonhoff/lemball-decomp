#include "FileRwSocket.h"

// 68K 0x10207d98 __ct__13CFileRWSocketFv
// FUNCTION: LEMBALL 0x0047a220
FileRwSocket::FileRwSocket() : RwSocket(), FileReadSocket(), FileWriteSocket()
{
}

// 68K 0x10208024 SendPacket__13CFileRWSocketFPCUci
// FUNCTION: LEMBALL 0x0047a420
bool FileRwSocket::SendPacket(const unsigned char* p_data, int p_size)
{
	bool sent = FileWriteSocket::SendPacket(p_data, p_size);
	if (sent) {
		FileWriteSocket::m_unk0x10 %= FileCommonSocket::m_unk0x08;
	}
	return sent;
}

// 68K 0x101167ec Closed__13CFileRWSocketFUc
// FUNCTION: LEMBALL 0x0047ba60
void FileRwSocket::Closed(int p_notifyPeer)
{
	RwSocket::Closed(p_notifyPeer);
}

// 68K 0x101168ec SendAcknowledgement__13CFileRWSocketFv
// FUNCTION: LEMBALL 0x0047baa0
void FileRwSocket::SendAcknowledgement()
{
	RwSocket::SendAcknowledgement();
}

// 68K 0x1011682a ReceiveAcknowledgement__13CFileRWSocketFv
// FUNCTION: LEMBALL 0x0047bad0
NetworkMessage* FileRwSocket::ReceiveAcknowledgement()
{
	return RwSocket::ReceiveAcknowledgement();
}

// 68K 0x1010681e __dt__13CFileRWSocketFv
FileRwSocket::~FileRwSocket()
{
}
