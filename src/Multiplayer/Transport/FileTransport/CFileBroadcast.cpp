#include "Multiplayer/Transport/FileTransport/CFileBroadcast.h"

#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Streams/CVSOStream.h"
#include "Engine/Strings/VsString.h"
#include "Multiplayer/Transport/Packets/BasePacketHeader.h"
#include "Multiplayer/Transport/Protocol/CHeaders.h"
#include "Multiplayer/Transport/Protocol/CPortsMessage.h"

#include "Multiplayer/Transport/CNetworkAddress.h"
#include "Multiplayer/Transport/CBaseNetwork.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Engine/Queues/CBaseQueueHandler.h"
#include "Engine/Queues/Message.h"
#include "Multiplayer/Transport/CBroadcast.h"
#include "Multiplayer/Transport/FileTransport/CFileCommonSocket.h"
#include "Multiplayer/Transport/FileTransport/CFileOpenManagement.h"
#include "Multiplayer/Transport/FileTransport/CFileReadSocket.h"
#include "Multiplayer/Transport/FileTransport/CFileWriteSocket.h"
#include "Multiplayer/Transport/FileTransport/CNetworkFile.h"
#include "Multiplayer/Transport/FileTransport/COpenCount.h"

#include <string.h>
#include "Engine/Time/VsTime.h"

#pragma intrinsic(memcpy, strcpy, strlen, strcat)

// FUNCTION: LEMBALL 0x0046f4f0
void CFileBroadcast::Setup(const char* p_peerName, const char* p_path)
{
	unsigned int length;

	g_szBroadcastPeerName = (char*) operator new(strlen(p_peerName) + 1);
	strcpy(g_szBroadcastPeerName, p_peerName);
	*g_pDebugOutput << "Found Local host OK: " << g_szBroadcastPeerName << "\n";
	g_localHostLookupComplete = 1;

	g_pFileBroadcastData = (char*) operator new(strlen(p_path) + 0xf);
	length = strlen(p_path);
	strcpy(g_pFileBroadcastData, p_path);
	if (g_pFileBroadcastData[length] != '\\' && g_pFileBroadcastData[length] != ':') {
		memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), "\\", 2);
	}
	char port[2] = "0";
	memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), "VSNETv", 7);
	vsLtoa(0, port, 10);
	strcat(g_pFileBroadcastData, port);
	vsLtoa(9, port, 10);
	strcat(g_pFileBroadcastData, port);
}

// FUNCTION: LEMBALL 0x0047a570
CFileBroadcast::CFileBroadcast()
{
	*g_pBroadcastAddress = g_szBroadcastPeerName;

	if (g_pFileBroadcast == NULL) {
		g_pFileBroadcast = new CPortsMessage();
	}

	CFileCommonSocket::m_headerSlotCount = 0x14;
	CFileWriteSocket::m_file = new CHeaders(CFileCommonSocket::m_headerSlotCount);
	CFileReadSocket::m_file = CFileWriteSocket::m_file;
	CFileWriteSocket::m_headersOffset = g_pFileBroadcast->m_payloadCapacity + m_message.m_payloadCapacity;
	CFileReadSocket::m_headersOffset = CFileWriteSocket::m_headersOffset;
	m_portInfoLocked = 0;
}

// FUNCTION: LEMBALL 0x0047a910
CFileBroadcast::~CFileBroadcast()
{
	delete CFileReadSocket::m_file;
	if (g_pFileBroadcastData != NULL) {
		operator delete(g_pFileBroadcastData);
		g_pFileBroadcastData = NULL;
	}
	if (g_pFileBroadcast != NULL) {
		delete g_pFileBroadcast;
		g_pFileBroadcast = NULL;
	}
}

// FUNCTION: LEMBALL 0x0047aa10
void CFileBroadcast::InitialiseFile()
{
	Seek(0);
	CFileWriteSocket::Write(m_message, 0, 0);
	CFileWriteSocket::Write(*g_pFileBroadcast, 0, 0);
	CFileWriteSocket::Write(*CFileWriteSocket::m_file, 0, 0);

	unsigned char* data = (unsigned char*) operator new(g_networkPacketSize);
	memset(data, 0, g_networkPacketSize);
	for (int i = 0; i < CFileCommonSocket::m_headerSlotCount; i++) {
		CNetworkFile::Write(data, g_networkPacketSize);
	}
	operator delete(data);
}

// FUNCTION: LEMBALL 0x0047ab10
void CFileBroadcast::GetSpecificAddr(const char* p_name)
{
}

// FUNCTION: LEMBALL 0x0047ab20
bool CFileBroadcast::Start(const char* p_name)
{
	CBroadcast::Initialise(p_name);

	char* extension = strchr(g_pFileBroadcastData, '.');
	if (extension != NULL) {
		strcpy(extension, ".bct");
	}
	else {
		memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), ".bct", 5);
	}

	bool created = CFileCommonSocket::CreateSocket(g_pFileBroadcastData);
	CFileOpenManagement::IncOpenCount();
	CFileWriteSocket::m_dataOffset = CFileReadSocket::m_file->m_payloadCapacity + CFileReadSocket::m_headersOffset;
	CFileReadSocket::m_dataOffset = CFileWriteSocket::m_dataOffset;

	if (created) {
		m_isOpen = 1;
		m_readReady = 1;
		m_writeReady = 0;
		m_socketFlags = 1;
		m_lastBroadcastTime = CurrentMilliTimer() - NETWORK_BROADCAST_INTERVAL_MS;

		Message message;
		message.m_type = 2;
		message.m_code = 0;
		g_pNetworkStatusQueue->Post(message);
		m_lastProcessTime = CurrentMilliTimer();
	}
	return created;
}

// FUNCTION: LEMBALL 0x0047ac50
bool CFileBroadcast::ReadPortInfo()
{
	Seek(m_message.m_payloadCapacity);
	unsigned int length = g_pFileBroadcast->m_payloadCapacity;
	if (!CNetworkFile::Lock(m_message.m_payloadCapacity, length)) {
		return false;
	}
	if (CNetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, length)) {
		g_pFileBroadcast->Set((unsigned char*) g_pNetworkPacketScratch);
		m_portInfoLocked = 1;
		return true;
	}
	CNetworkFile::UnLock(m_message.m_payloadCapacity, length);
	return false;
}

// FUNCTION: LEMBALL 0x0047ace0
bool CFileBroadcast::WritePortInfo()
{
	g_pFileBroadcast->OpenDataStream();
	unsigned int length = g_pFileBroadcast->m_payloadCapacity;
	Seek(m_message.m_payloadCapacity);
	m_portInfoLocked = 0;
	bool result = CNetworkFile::Write(g_pFileBroadcast->m_buffer + sizeof(BasePacketHeader), length);
	g_pFileBroadcast->CloseDataStream();
	if (result) {
		return CNetworkFile::UnLock(m_message.m_payloadCapacity, length) != 0;
	}
	CNetworkFile::UnLock(m_message.m_payloadCapacity, length);
	return false;
}

// FUNCTION: LEMBALL 0x0047ad70
short CFileBroadcast::FindPort(const unsigned char* p_data)
{
	(void) p_data;
	if (!ReadPortInfo()) {
		return NETWORK_PORT_NOT_FOUND;
	}
	g_pFileBroadcast->Set((unsigned char*) g_pNetworkPacketScratch);
	int port = 0;
	unsigned int length = g_pFileBroadcast->m_payloadCapacity;
	unsigned char* counts = g_pFileBroadcast->m_useCounts;
	while (port < NETWORK_PORT_COUNT && counts[(unsigned short) port] != 0) {
		port++;
	}
	if (port != NETWORK_PORT_COUNT) {
		counts[(unsigned short) port]++;
		int written = WritePortInfo();
		int result = NETWORK_PORT_NOT_FOUND;
		if (written) {
			result = port;
		}
		return result;
	}
	CNetworkFile::UnLock(m_message.m_payloadCapacity, length);
	return NETWORK_PORT_NOT_FOUND;
}

// FUNCTION: LEMBALL 0x0047ae00
void CFileBroadcast::ResetPort(short p_port)
{
	m_connectionData[p_port] = 0;
	if (m_portInfoLocked == 0) {
		ReadPortInfo();
		g_pFileBroadcast->m_useCounts[(unsigned short) p_port]--;
		WritePortInfo();
	}
	else {
		g_pFileBroadcast->m_useCounts[(unsigned short) p_port]--;
	}

	if (!g_pFileBroadcast->AnyUsed()) {
		char* extension = strchr(g_pFileBroadcastData, '.');
		if (extension != NULL) {
			strcpy(extension, ".con");
		}
		else {
			memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), ".con", 5);
		}
		Delete(g_pFileBroadcastData);
	}
}

// FUNCTION: LEMBALL 0x0047aed0
void CFileBroadcast::StartListen()
{
	m_listenEnabled = 1;
}

// FUNCTION: LEMBALL 0x0047aee0
void CFileBroadcast::StopListen()
{
	m_listenEnabled = 0;
}

// FUNCTION: LEMBALL 0x0047aef0
void CFileBroadcast::Process()
{
	unsigned long currentTime = CurrentMilliTimer();
	if (100 < currentTime - m_lastProcessTime) {
		if (m_listenEnabled != 0) {
			CFileReadSocket::Process();
		}
		m_lastProcessTime = CurrentMilliTimer();
	}
	CBroadcast::Process();
}

// FUNCTION: LEMBALL 0x0047bbf0
void CFileBroadcast::Closed(int p_notifyPeer)
{
	CBroadcast::Closed(p_notifyPeer);
}

// GLOBAL: LEMBALL 0x004a2de0
CPortsMessage* g_pFileBroadcast = NULL;

// GLOBAL: LEMBALL 0x004a2de4
char* g_pFileBroadcastData = NULL;
