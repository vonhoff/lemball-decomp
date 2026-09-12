#include "FileBroadcast.h"

#include "../Foundation/BaseQueue.h"
#include "../Foundation/VsOStream.h"
#include "../Foundation/VsString.h"
#include "../Messaging/BasePacketHeader.h"
#include "../Messaging/Headers.h"
#include "../Messaging/PortsMessage.h"
#include "FileNetwork.h"
#include "NetworkAddress.h"
#include "TcpIpNetwork.h"

#include <memory.h>
#include <new.h>
#include <string.h>

#pragma intrinsic(memcpy, strcpy, strlen, strcat)

extern "C" unsigned long __stdcall timeGetTime(void);

// 68K 0x10106e3e Setup__14CFileBroadcastFPCcPCc
// FUNCTION: LEMBALL 0x0046f4f0
void FileBroadcast::Setup(const char* p_peerName, const char* p_path)
{
	unsigned int length;

	g_szBroadcastPeerName = (char*) operator new(strlen(p_peerName) + 1);
	strcpy(g_szBroadcastPeerName, p_peerName);
	*g_pDebugOutput << "Found Local host OK: " << g_szBroadcastPeerName << "\n";
	g_unk0x4a1e44 = 1;

	g_pFileBroadcastData = (char*) operator new(strlen(p_path) + 0xf);
	length = strlen(p_path);
	strcpy(g_pFileBroadcastData, p_path);
	if (g_pFileBroadcastData[length] != '\\' && g_pFileBroadcastData[length] != ':') {
		memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), "\\", 2);
	}
	char port[2] = "0";
	memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), "VSNETv", 7);
	VsLtoa(0, port, 10);
	strcat(g_pFileBroadcastData, port);
	VsLtoa(9, port, 10);
	strcat(g_pFileBroadcastData, port);
}

// 68K 0x10208278 __ct__14CFileBroadcastFv
// FUNCTION: LEMBALL 0x0047a570
FileBroadcast::FileBroadcast()
{
	*g_pBroadcastAddress = g_szBroadcastPeerName;

	if (g_pFileBroadcast == 0) {
		g_pFileBroadcast = new PortsMessage();
	}

	FileCommonSocket::m_unk0x08 = 0x14;
	FileWriteSocket::m_file = new Headers(FileCommonSocket::m_unk0x08);
	FileReadSocket::m_file = FileWriteSocket::m_file;
	FileWriteSocket::m_unk0x04 = g_pFileBroadcast->m_payloadCapacity + m_message.m_payloadCapacity;
	FileReadSocket::m_unk0x04 = FileWriteSocket::m_unk0x04;
	m_portInfoLocked = 0;
}

// 68K 0x10208d44 InitialiseFile__14CFileBroadcastFv
// FUNCTION: LEMBALL 0x0047aa10
void FileBroadcast::InitialiseFile()
{
	Seek(0);
	FileWriteSocket::Write(m_message, 0, 0);
	FileWriteSocket::Write(*g_pFileBroadcast, 0, 0);
	FileWriteSocket::Write(*FileWriteSocket::m_file, 0, 0);

	unsigned char* data = (unsigned char*) operator new(g_networkPacketSize);
	memset(data, 0, g_networkPacketSize);
	for (int i = 0; i < FileCommonSocket::m_unk0x08; i++) {
		NetworkFile::Write(data, g_networkPacketSize);
	}
	operator delete(data);
}

// 68K 0x10208e36 GetSpecificAddr__14CFileBroadcastFPCc
// FUNCTION: LEMBALL 0x0047ab10
void FileBroadcast::GetSpecificAddr(const char* p_name)
{
}

// 68K 0x10208e68 Start__14CFileBroadcastFPCc
// FUNCTION: LEMBALL 0x0047ab20
bool FileBroadcast::Start(const char* p_name)
{
	Broadcast::Initialise(p_name);

	char* extension = strchr(g_pFileBroadcastData, '.');
	if (extension != 0) {
		strcpy(extension, ".bct");
	}
	else {
		memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), ".bct", 5);
	}

	bool created = FileCommonSocket::CreateSocket(g_pFileBroadcastData);
	FileOpenManagement::IncOpenCount();
	FileWriteSocket::m_dataOffset = FileReadSocket::m_file->m_payloadCapacity + FileReadSocket::m_unk0x04;
	FileReadSocket::m_dataOffset = FileWriteSocket::m_dataOffset;

	if (created) {
		m_isOpen = 1;
		m_readReady = 1;
		m_writeReady = 0;
		m_socketFlags = 1;
		m_lastBroadcastTime = timeGetTime() - 1000;

		Message message;
		message.type = 2;
		message.code = 0;
		g_pNetworkStatusQueue->Post(message);
		m_lastProcessTime = timeGetTime();
	}
	return created;
}

// 68K 0x10208fb0 ReadPortInfo__14CFileBroadcastFv
// FUNCTION: LEMBALL 0x0047ac50
bool FileBroadcast::ReadPortInfo()
{
	Seek(m_message.m_payloadCapacity);
	unsigned int length = g_pFileBroadcast->m_payloadCapacity;
	if (!NetworkFile::Lock(m_message.m_payloadCapacity, length)) {
		return false;
	}
	if (NetworkFile::Read((unsigned char*) g_pNetworkPacketScratch, length)) {
		g_pFileBroadcast->Set((unsigned char*) g_pNetworkPacketScratch);
		m_portInfoLocked = 1;
		return true;
	}
	NetworkFile::UnLock(m_message.m_payloadCapacity, length);
	return false;
}

// 68K 0x1020909c WritePortInfo__14CFileBroadcastFv
// FUNCTION: LEMBALL 0x0047ace0
bool FileBroadcast::WritePortInfo()
{
	g_pFileBroadcast->OpenDataStream();
	unsigned int length = g_pFileBroadcast->m_payloadCapacity;
	Seek(m_message.m_payloadCapacity);
	m_portInfoLocked = 0;
	bool result = NetworkFile::Write(g_pFileBroadcast->m_buffer + sizeof(BasePacketHeader), length);
	g_pFileBroadcast->CloseDataStream();
	if (result) {
		return NetworkFile::UnLock(m_message.m_payloadCapacity, length) != 0;
	}
	NetworkFile::UnLock(m_message.m_payloadCapacity, length);
	return false;
}

// 68K 0x10209196 FindPort__14CFileBroadcastFPCUc
// FUNCTION: LEMBALL 0x0047ad70
short FileBroadcast::FindPort(const unsigned char* p_data)
{
	(void) p_data;
	if (!ReadPortInfo()) {
		return -1;
	}
	g_pFileBroadcast->Set((unsigned char*) g_pNetworkPacketScratch);
	int port = 0;
	unsigned int length = g_pFileBroadcast->m_payloadCapacity;
	unsigned char* counts = g_pFileBroadcast->m_useCounts;
	while (port < 0x200 && counts[(unsigned short) port] != 0) {
		port++;
	}
	if (port != 0x200) {
		counts[(unsigned short) port]++;
		int written = WritePortInfo();
		int result = -1;
		if (written) {
			result = port;
		}
		return result;
	}
	NetworkFile::UnLock(m_message.m_payloadCapacity, length);
	return -1;
}

// 68K 0x10209276 ResetPort__14CFileBroadcastFs
// FUNCTION: LEMBALL 0x0047ae00
void FileBroadcast::ResetPort(short p_port)
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
		if (extension != 0) {
			strcpy(extension, ".con");
		}
		else {
			memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), ".con", 5);
		}
		Delete(g_pFileBroadcastData);
	}
}

// 68K 0x1020935e StartListen__14CFileBroadcastFv
// FUNCTION: LEMBALL 0x0047aed0
void FileBroadcast::StartListen()
{
	m_listenEnabled = 1;
}

// 68K 0x10209392 StopListen__14CFileBroadcastFv
// FUNCTION: LEMBALL 0x0047aee0
void FileBroadcast::StopListen()
{
	m_listenEnabled = 0;
}

// 68K 0x102093c4 Process__14CFileBroadcastFv
// FUNCTION: LEMBALL 0x0047aef0
void FileBroadcast::Process()
{
	unsigned long currentTime = timeGetTime();
	if (100 < currentTime - m_lastProcessTime) {
		if (m_listenEnabled != 0) {
			FileReadSocket::Process();
		}
		m_lastProcessTime = timeGetTime();
	}
	Broadcast::Process();
}

// 68K 0x102088cc __dt__14CFileBroadcastFv
FileBroadcast::~FileBroadcast()
{
}

// 68K 0x10116874 Closed__14CFileBroadcastFUc
// FUNCTION: LEMBALL 0x0047bbf0
void FileBroadcast::Closed(int p_notifyPeer)
{
	Broadcast::Closed(p_notifyPeer);
}

// GLOBAL: LEMBALL 0x004a2de0
PortsMessage* g_pFileBroadcast = 0;
