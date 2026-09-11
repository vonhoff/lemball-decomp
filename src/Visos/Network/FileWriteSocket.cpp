#include "FileWriteSocket.h"

#include "../Messaging/HeaderMessage.h"
#include "../Messaging/Headers.h"
#include "NetworkAddress.h"
#include "TcpIpNetwork.h"

#include <string.h>

#pragma intrinsic(strcpy)

extern "C" unsigned long __stdcall timeGetTime(void);

// 68K 0x102077ea __ct__16CFileWriteSocketFv
// FUNCTION: LEMBALL 0x00479e20
FileWriteSocket::FileWriteSocket() : FileBaseSocket(), WriteSocket(), FileCommonSocket()
{
	m_unk0x10 = 0;
}

// 68K 0x10207a8c Write__16CFileWriteSocketFR15CNetworkMessageUcUc
// FUNCTION: LEMBALL 0x00479fa0
int FileWriteSocket::Write(NetworkMessage& p_message, undefined4 p_keepLock, undefined4 p_wait)
{
	p_message.OpenDataStream();
	unsigned int offset = Tell();
	int length = p_message.m_writeCursor - p_message.m_buffer - sizeof(BasePacketHeader);
	GetFileSize();

	if (p_wait == 0) {
		unsigned long started = timeGetTime();
		bool locked;
		do {
			locked = NetworkFile::Lock(offset, length);
		} while (!locked && timeGetTime() - started < 100);
		if (!locked) {
			length = 0;
		}
	}

	if (length != 0) {
		if (!NetworkFile::Write(p_message.m_buffer + sizeof(BasePacketHeader), length)) {
			NetworkFile::UnLock(offset, length);
			length = 0;
		}
		else if (p_keepLock == 0 && !NetworkFile::UnLock(offset, length)) {
			length = 0;
		}
	}
	p_message.CloseDataStream();
	return length;
}

// 68K 0x10207bba SetDestAddr__16CFileWriteSocketFP15CNetworkAddress
// FUNCTION: LEMBALL 0x0047a090
void FileWriteSocket::SetDestAddr(NetworkAddress* p_address)
{
	InternalSetDestAddr(p_address);
}

// 68K 0x10207c0a SendPacket__16CFileWriteSocketFPCUci
// FUNCTION: LEMBALL 0x0047a0b0
bool FileWriteSocket::SendPacket(const unsigned char* p_data, int p_size)
{
	int error = 0;
	HeaderMessage* header;
	unsigned int headerOffset;
	int lockLength;

	if (m_socketFlags == 0) {
		return false;
	}

	headerOffset = m_file->m_payloadCapacity * m_unk0x10 + m_unk0x04;
	Seek(headerOffset);
	header = &m_file->m_headers[m_unk0x10];
	strcpy(header->m_text0, g_pBroadcastAddress->GetStr());
	strcpy(header->m_text1, m_destinationAddress->GetStr());
	header->m_headerValue = (unsigned long) p_size;

	lockLength = Write(*header, 1, 0);
	if (lockLength != 0) {
		Seek(m_dataOffset + m_unk0x10 * g_networkPacketSize);
		if (!NetworkFile::Write(p_data, p_size)) {
			error = 1;
		}
	}
	else {
		error = 1;
	}

	if (error == 0) {
		m_unk0x10++;
		WriteSocket::m_lastSendTime = timeGetTime();
	}
	if (lockLength != 0) {
		NetworkFile::UnLock(headerOffset, lockLength);
	}
	return error == 0;
}

// 68K 0x1020795a __dt__16CFileWriteSocketFv
FileWriteSocket::~FileWriteSocket()
{
}

// 68K 0x101167b2 Closed__16CFileWriteSocketFUc
// FUNCTION: LEMBALL 0x0047bd30
void FileWriteSocket::Closed(int p_notifyPeer)
{
	WriteSocket::Closed(p_notifyPeer);
}
