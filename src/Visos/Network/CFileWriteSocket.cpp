#include "CFileWriteSocket.h"

#include "../Messaging/CHeaderMessage.h"
#include "../Messaging/CHeaders.h"
#include "CNetworkAddress.h"
#include "CTCPIPNetwork.h"
#include "Visos/Messaging/BasePacketHeader.h"
#include "Visos/Messaging/CNetworkMessage.h"
#include "Visos/Network/CFileBaseSocket.h"
#include "Visos/Network/CFileCommonSocket.h"
#include "Visos/Network/CNetworkFile.h"
#include "Visos/Network/CWriteSocket.h"

#include <string.h>

#pragma intrinsic(strcpy)

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00479e20
CFileWriteSocket::CFileWriteSocket() : CFileBaseSocket(), CWriteSocket(), CFileCommonSocket()
{
	m_nextWriteSlot = 0;
}

// FUNCTION: LEMBALL 0x00479f40
CFileWriteSocket::~CFileWriteSocket()
{
}

// FUNCTION: LEMBALL 0x00479fa0
int CFileWriteSocket::Write(CNetworkMessage& p_message, int p_keepLock, int p_wait)
{
	p_message.OpenDataStream();
	unsigned int offset = Tell();
	int length = p_message.m_writeCursor - p_message.m_buffer - sizeof(BasePacketHeader);
	GetFileSize();

	if (p_wait == 0) {
		unsigned long started = timeGetTime();
		bool locked;
		do {
			locked = CNetworkFile::Lock(offset, length);
		} while (!locked && timeGetTime() - started < 100);
		if (!locked) {
			length = 0;
		}
	}

	if (length != 0) {
		if (!CNetworkFile::Write(p_message.m_buffer + sizeof(BasePacketHeader), length)) {
			CNetworkFile::UnLock(offset, length);
			length = 0;
		}
		else if (p_keepLock == 0 && !CNetworkFile::UnLock(offset, length)) {
			length = 0;
		}
	}
	p_message.CloseDataStream();
	return length;
}

// FUNCTION: LEMBALL 0x0047a090
void CFileWriteSocket::SetDestAddr(CNetworkAddress* p_address)
{
	_SetDestAddr(p_address);
}

// FUNCTION: LEMBALL 0x0047a0b0
bool CFileWriteSocket::SendPacket(const unsigned char* p_data, int p_size)
{
	int error = 0;
	CHeaderMessage* header;
	unsigned int headerOffset;
	int lockLength;

	if (m_socketFlags == 0) {
		return false;
	}

	headerOffset = m_file->m_headers->m_payloadCapacity * m_nextWriteSlot + m_headersOffset;
	Seek(headerOffset);
	header = &m_file->m_headers[m_nextWriteSlot];
	strcpy(header->m_text0, g_pBroadcastAddress->GetStr());
	strcpy(header->m_text1, m_destinationAddress->GetStr());
	header->m_headerValue = (unsigned long) p_size;

	lockLength = Write(m_file->m_headers[m_nextWriteSlot], 1, 0);
	if (lockLength != 0) {
		Seek(m_dataOffset + m_nextWriteSlot * g_networkPacketSize);
		if (!CNetworkFile::Write(p_data, p_size)) {
			error = 1;
		}
	}
	else {
		error = 1;
	}

	if (error == 0) {
		m_nextWriteSlot++;
		CWriteSocket::m_lastSendTime = timeGetTime();
	}
	if (lockLength != 0) {
		CNetworkFile::UnLock(headerOffset, lockLength);
	}
	return error == 0;
}

// FUNCTION: LEMBALL 0x0047bd30
void CFileWriteSocket::Closed(int p_notifyPeer)
{
	CWriteSocket::Closed(p_notifyPeer);
}
