#include "CFileConnect.h"

#include "../Foundation/VsTime.h"
#include "../Messaging/CHeaders.h"
#include "CFileNetwork.h"
#include "CTCPIPNetwork.h"
#include "Visos/Network/CConnect.h"
#include "Visos/Network/CFileCommonSocket.h"
#include "Visos/Network/CFileOpenManagement.h"
#include "Visos/Network/CFileReadSocket.h"
#include "Visos/Network/CFileWriteSocket.h"
#include "Visos/Network/CNetworkFile.h"
#include "Visos/Network/COpenCount.h"
#include "Visos/Network/CReadSocket.h"
#include "Visos/Network/CWriteSocket.h"

#include <memory.h>
#include <string.h>

#pragma intrinsic(memcpy, strcpy, strlen)

// FUNCTION: LEMBALL 0x0047af30
CFileConnect::CFileConnect()
{
	CFileCommonSocket::m_headerSlotCount = 3;
	CFileReadSocket::m_file = new CHeaders(CFileCommonSocket::m_headerSlotCount);
	CFileWriteSocket::m_file = new CHeaders(CFileCommonSocket::m_headerSlotCount);
	CFileReadSocket::m_headersOffset = m_message.m_payloadCapacity;
	CFileWriteSocket::m_headersOffset = m_message.m_payloadCapacity;
}

// FUNCTION: LEMBALL 0x0047b2a0
CFileConnect::~CFileConnect()
{
	delete CFileReadSocket::m_file;
	delete CFileWriteSocket::m_file;
}

// FUNCTION: LEMBALL 0x0047b370
void CFileConnect::InitialiseFile()
{
	Seek(0);
	CFileWriteSocket::Write(m_message, 0, 0);
}

// FUNCTION: LEMBALL 0x0047b3b0
bool CFileConnect::Start(const char* p_localName, const char* p_remoteName)
{
	m_address = (CNetworkAddress*) operator new(strlen(p_remoteName) + 1);
	strcpy((char*) m_address, p_remoteName);
	m_name = (char*) operator new(strlen(p_localName) + 1);
	strcpy(m_name, p_localName);
	return true;
}

// FUNCTION: LEMBALL 0x0047b440
void CFileConnect::InitSocket()
{
	char* path = g_pFileBroadcastData;
	char* extension = strchr(path, '.');
	if (extension != 0) {
		strcpy(extension, ".con");
	}
	else {
		memcpy(path + strlen(path), ".con", 5);
	}

	CFileCommonSocket::CreateSocket(path);
	CFileOpenManagement::IncOpenCount();
	CFileReadSocket::m_dataOffset = CFileReadSocket::m_file->m_payloadCapacity + CFileReadSocket::m_headersOffset;
}

// FUNCTION: LEMBALL 0x0047b4d0
void CFileConnect::Listen(CNetworkAddress* p_address)
{
	InitSocket();
	SetDestAddr(p_address);
	m_closePending = 1;
	m_eventPending = 1;
	m_isHost = 0;
	CWriteSocket::m_lastSendTime = CurrentMilliTimer() - 1000;
	CReadSocket::m_lastReceiveTime = CurrentMilliTimer();
	ConnectSetup();
}

// FUNCTION: LEMBALL 0x0047b540
void CFileConnect::Connect()
{
	InitSocket();
	m_eventPending = 1;
	m_isHost = 1;
	CReadSocket::m_lastReceiveTime = CurrentMilliTimer();
	ConnectSetup();
}

// FUNCTION: LEMBALL 0x0047b580
void CFileConnect::ConnectSetup()
{
	int port = m_port;
	int writeIndex = port * 2;
	int readIndex = writeIndex;
	if (m_isHost == 0) {
		readIndex++;
	}
	else {
		writeIndex++;
	}

	CFileReadSocket::m_headersOffset +=
		(CFileCommonSocket::m_headerSlotCount * g_networkPacketSize + CFileReadSocket::m_file->m_payloadCapacity) *
		readIndex;
	CFileReadSocket::m_dataOffset = CFileReadSocket::m_file->m_payloadCapacity + CFileReadSocket::m_headersOffset;
	CFileWriteSocket::m_headersOffset +=
		(CFileCommonSocket::m_headerSlotCount * g_networkPacketSize + CFileWriteSocket::m_file->m_payloadCapacity) *
		writeIndex;
	CFileWriteSocket::m_dataOffset = CFileWriteSocket::m_file->m_payloadCapacity + CFileWriteSocket::m_headersOffset;

	unsigned int packetBytes = CFileCommonSocket::m_headerSlotCount * g_networkPacketSize;
	unsigned int requiredSize = CFileReadSocket::m_dataOffset + packetBytes;
	unsigned int writeSize = CFileWriteSocket::m_dataOffset + packetBytes;
	if (requiredSize < writeSize) {
		requiredSize = writeSize;
	}

	unsigned long started = CurrentMilliTimer();
	bool locked;
	do {
		locked = CNetworkFile::Lock(0, m_message.m_payloadCapacity);
	} while (!locked && CurrentMilliTimer() - started < 100);

	if (!locked) {
		goto setupComplete;
	}

	while (GetFileSize() < requiredSize) {
		unsigned char* data = (unsigned char*) operator new(g_networkPacketSize);
		memset(data, 0, g_networkPacketSize);
		Seek(GetFileSize());

		CFileWriteSocket::Write(*CFileWriteSocket::m_file, 0, 0);
		int i = 0;
		for (; i < CFileCommonSocket::m_headerSlotCount; i++) {
			CNetworkFile::Write(data, g_networkPacketSize);
		}

		CFileWriteSocket::Write(*CFileReadSocket::m_file, 0, 0);
		i = 0;
		for (; i < CFileCommonSocket::m_headerSlotCount; i++) {
			CNetworkFile::Write(data, g_networkPacketSize);
		}

		operator delete(data);
	}

	if (!CNetworkFile::UnLock(0, m_message.m_payloadCapacity)) {
		SocketError();
		return;
	}

setupComplete:
	m_isOpen = 1;
	m_socketFlags = 1;
}

// FUNCTION: LEMBALL 0x0047b990
void CFileConnect::Process()
{
	CFileReadSocket::Process();
	CConnect::Process();
}

// FUNCTION: LEMBALL 0x0047bfc0
void CFileConnect::Closed(int p_notifyPeer)
{
	CConnect::Closed(p_notifyPeer);
}

// FUNCTION: LEMBALL 0x0047bff0
CNetworkMessage* CFileConnect::ReceiveAcknowledgement()
{
	return CConnect::ReceiveAcknowledgement();
}
