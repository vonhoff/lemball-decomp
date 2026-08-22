#include "FileWriteSocket.h"

// 68K 0x102077ea __ct__16CFileWriteSocketFv
// STUB: LEMBALL 0x00479e20
FileWriteSocket::FileWriteSocket()
{
}

// 68K 0x10207a8c Write__16CFileWriteSocketFR15CNetworkMessageUcUc
// STUB: LEMBALL 0x00479fa0
int FileWriteSocket::Write(NetworkMessage& p_message, unsigned char p_keepLock, unsigned char p_wait)
{
	return 0;
}

// 68K 0x10207bba SetDestAddr__16CFileWriteSocketFP15CNetworkAddress
// STUB: LEMBALL 0x0047a090
void FileWriteSocket::SetDestAddr(NetworkAddress* p_address)
{
}

// 68K 0x10207c0a SendPacket__16CFileWriteSocketFPCUci
// STUB: LEMBALL 0x0047a0b0
bool FileWriteSocket::SendPacket(const unsigned char* p_data, int p_size)
{
	return 0;
}

// 68K 0x1020795a __dt__16CFileWriteSocketFv
// SYNTHETIC: LEMBALL 0x0047bcc0
// FileWriteSocket::`scalar deleting destructor'
FileWriteSocket::~FileWriteSocket()
{
}

// 68K 0x101167b2 Closed__16CFileWriteSocketFUc
// STUB: LEMBALL 0x0047bd30
void FileWriteSocket::Closed(unsigned char p_notifyPeer)
{
}
