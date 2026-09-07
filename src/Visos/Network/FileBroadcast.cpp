#include "FileBroadcast.h"

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

#pragma intrinsic(memcpy, strcpy, strlen)

extern "C" unsigned long __stdcall timeGetTime(void);

// 68K 0x10106e3e Setup__14CFileBroadcastFPCcPCc
// FUNCTION: LEMBALL 0x0046f4f0
void FileBroadcast::Setup(const char* p_peerName, const char* p_path)
{
	char port[2];
	unsigned int length;

	g_szBroadcastPeerName = (char*) operator new(strlen(p_peerName) + 1);
	strcpy(g_szBroadcastPeerName, p_peerName);
	*g_pDebugOutput << "Found Local host OK: " << g_szBroadcastPeerName << "\n";
	g_unk0x4a1e44 = 1;

	g_pFileBroadcastData = (char*) operator new(strlen(p_path) + 0xf);
	strcpy(g_pFileBroadcastData, p_path);
	length = strlen(g_pFileBroadcastData);
	if (g_pFileBroadcastData[length - 1] != '\\' && g_pFileBroadcastData[length - 1] != ':') {
		memcpy(g_pFileBroadcastData + length, "\\", 2);
	}
	memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), "VSNETv", 7);
	VsLtoa(0, port, 10);
	memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), port, strlen(port) + 1);
	VsLtoa(9, port, 10);
	memcpy(g_pFileBroadcastData + strlen(g_pFileBroadcastData), port, strlen(port) + 1);
}

// 68K 0x10208278 __ct__14CFileBroadcastFv
// FUNCTION: LEMBALL 0x0047a570
FileBroadcast::FileBroadcast()
{
	m_message.Initialise();
	m_message.m_payloadCapacity += 2;
	m_message.m_openCount = 0;
	*g_pBroadcastAddress = g_szBroadcastPeerName;

	if (g_pFileBroadcast == 0) {
		g_pFileBroadcast = new PortsMessage();
	}

	FileCommonSocket::m_unk0x08 = 0x14;
	Headers* headers = new Headers(FileCommonSocket::m_unk0x08);
	FileReadSocket::m_file = headers;
	FileWriteSocket::m_file = headers;
	m_portInfoLocked = 0;
}

// 68K 0x10208d44 InitialiseFile__14CFileBroadcastFv
// FUNCTION: LEMBALL 0x0047aa10
void FileBroadcast::InitialiseFile()
{
	NetworkFile::Seek(0);
	FileWriteSocket::Write(m_message, 0, 0);
	FileWriteSocket::Write(*g_pFileBroadcast, 0, 0);
	FileWriteSocket::Write(*FileWriteSocket::m_file, 0, 0);

	unsigned char* data = (unsigned char*) operator new(g_networkPacketSize);
	memset(data, 0, g_networkPacketSize);
	for (int i = 0; i < FileWriteSocket::m_file->m_count; i++) {
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
// STUB: LEMBALL 0x0047ab20
bool FileBroadcast::Start(const char* p_name)
{
	return 0;
}

// 68K 0x10208fb0 ReadPortInfo__14CFileBroadcastFv
// FUNCTION: LEMBALL 0x0047ac50
bool FileBroadcast::ReadPortInfo()
{
	NetworkFile::Seek(m_message.m_payloadCapacity);
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
	NetworkFile::Seek(m_message.m_payloadCapacity);
	m_portInfoLocked = 0;
	bool result = NetworkFile::Write(g_pFileBroadcast->m_buffer + sizeof(BasePacketHeader), length);
	g_pFileBroadcast->CloseDataStream();
	if (result) {
		return NetworkFile::UnLock(m_message.m_payloadCapacity, length);
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
	while (port < 0x200 && g_pFileBroadcast->m_useCounts[port] != 0) {
		port++;
	}
	if (port == 0x200) {
		NetworkFile::UnLock(m_message.m_payloadCapacity, g_pFileBroadcast->m_payloadCapacity);
		return -1;
	}
	g_pFileBroadcast->m_useCounts[port]++;
	return WritePortInfo() ? (short) port : -1;
}

// 68K 0x10209276 ResetPort__14CFileBroadcastFs
// STUB: LEMBALL 0x0047ae00
void FileBroadcast::ResetPort(short p_port)
{
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
// STUB: LEMBALL 0x0047bbf0
void FileBroadcast::Closed(int p_notifyPeer)
{
}

// GLOBAL: LEMBALL 0x004a2de0
PortsMessage* g_pFileBroadcast = 0;
