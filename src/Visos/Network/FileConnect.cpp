#include "FileConnect.h"

#include "../Foundation/VsTime.h"
#include "../Messaging/Headers.h"

#include <new.h>
#include <string.h>

#pragma intrinsic(strcpy, strlen)

// 68K 0x1020942a __ct__12CFileConnectFv
// FUNCTION: LEMBALL 0x0047af30
FileConnect::FileConnect()
{
	FileCommonSocket::m_unk0x08 = 3;
	FileReadSocket::m_file = new Headers(FileCommonSocket::m_unk0x08);
	FileWriteSocket::m_file = new Headers(FileCommonSocket::m_unk0x08);
	FileReadSocket::m_unk0x04 = m_message.m_payloadCapacity;
	FileWriteSocket::m_unk0x04 = m_message.m_payloadCapacity;
}

// 68K 0x10209eb2 InitialiseFile__12CFileConnectFv
// FUNCTION: LEMBALL 0x0047b370
void FileConnect::InitialiseFile()
{
	Seek(0);
	FileWriteSocket::Write(m_message, 0, 0);
}

// 68K 0x10209f16 Start__12CFileConnectFPCcPCc
// FUNCTION: LEMBALL 0x0047b3b0
bool FileConnect::Start(const char* p_localName, const char* p_remoteName)
{
	m_address = (NetworkAddress*) operator new(strlen(p_remoteName) + 1);
	strcpy((char*) m_address, p_remoteName);
	m_name = (char*) operator new(strlen(p_localName) + 1);
	strcpy(m_name, p_localName);
	return true;
}

// 68K 0x10209fa2 InitSocket__12CFileConnectFv
// STUB: LEMBALL 0x0047b440
void FileConnect::InitSocket()
{
}

// 68K 0x1020a04c Listen__12CFileConnectFP15CNetworkAddress
// STUB: LEMBALL 0x0047b4d0
void FileConnect::Listen(NetworkAddress* p_address)
{
}

// 68K 0x1020a114 Connect__12CFileConnectFv
// FUNCTION: LEMBALL 0x0047b540
void FileConnect::Connect()
{
	InitSocket();
	m_eventPending = 1;
	m_isHost = 1;
	ReadSocket::m_lastReceiveTime = CurrentMilliTimer();
	ConnectSetup();
}

// 68K 0x1020a186 ConnectSetup__12CFileConnectFv
// STUB: LEMBALL 0x0047b580
void FileConnect::ConnectSetup()
{
}

// 68K 0x101168ac Process__12CFileConnectFv
// FUNCTION: LEMBALL 0x0047b990
void FileConnect::Process()
{
	FileReadSocket::Process();
	CConnect::Process();
}

// 68K 0x10209a58 __dt__12CFileConnectFv
FileConnect::~FileConnect()
{
}

// 68K 0x10116932 Closed__12CFileConnectFUc
// FUNCTION: LEMBALL 0x0047bfc0
void FileConnect::Closed(int p_notifyPeer)
{
	CConnect::Closed(p_notifyPeer);
}

// 68K 0x10116968 ReceiveAcknowledgement__12CFileConnectFv
// FUNCTION: LEMBALL 0x0047bff0
NetworkMessage* FileConnect::ReceiveAcknowledgement()
{
	return CConnect::ReceiveAcknowledgement();
}
