#include "Broadcast.h"

#include "../Foundation/BaseQueue.h"
#include "../Foundation/VsMem.h"
#include "../Foundation/VsString.h"
#include "../Foundation/VsTime.h"
#include "../Messaging/BasePacket.h"
#include "../Messaging/BasePacketHeader.h"
#include "../Messaging/BroadcastMessage.h"
#include "../Messaging/MessReqConnect.h"
#include "../Messaging/ReadPacket.h"
#include "BaseNetwork.h"
#include "NetworkAddress.h"
#include "TcpIpNetwork.h"

#include <memory.h>
#include <new.h>
#include <string.h>

#pragma intrinsic(memcpy, strcat, strcpy, strlen)

extern "C" unsigned long __stdcall timeGetTime(void);

// GLOBAL: LEMBALL 0x004a1e08
undefined4 g_unk0x4a1e08 = 0;

// GLOBAL: LEMBALL 0x004a1e0c
undefined4 g_unk0x4a1e0c = 9;

// 68K 0x1020c60e __ct__10CBroadcastFv
// FUNCTION: LEMBALL 0x00460350
Broadcast::Broadcast()
{
	int index;

	m_unk0x14 = 0;
	m_runEnabled = 0;
	m_addressMode = 0;
	m_specificAddress = 0;
	g_pBroadcastAddress = (NetworkAddress*) g_pBaseNetwork->GetNewNetworkAddress();
	m_connectionData = new unsigned char[0x200];
	for (index = 0; index < 0x200; index++) {
		m_connectionData[index] = 0;
	}
}

// 68K 0x1020c7ac __dt__10CBroadcastFv
// FUNCTION: LEMBALL 0x004604e0
Broadcast::~Broadcast()
{
	if (m_specificAddress != 0) {
		operator delete(m_specificAddress);
	}
	operator delete(m_connectionData);
	operator delete(g_pBroadcastAddress);
	g_pBroadcastAddress = 0;
	if (g_szBroadcastPeerName != 0) {
		operator delete(g_szBroadcastPeerName);
		g_szBroadcastPeerName = 0;
	}
	if (g_pBroadcastPacketTemplate != 0) {
		operator delete(g_pBroadcastPacketTemplate);
		g_pBroadcastPacketTemplate = 0;
	}
}

// 68K 0x1020c914 Closed__10CBroadcastFUc
// FUNCTION: LEMBALL 0x004605b0
void Broadcast::Closed(unsigned char p_arg0)
{
	(void) p_arg0;
	g_pBaseNetwork->KillUnBornConnection(WriteSocket::m_destinationAddress);
}

// 68K 0x1020c956 FindPort__10CBroadcastFPCUc
// FUNCTION: LEMBALL 0x004605d0
short Broadcast::FindPort(const unsigned char* p_arg0)
{
	short result;
	int index;

	result = -1;
	index = 0;
	while (index < 0x200) {
		if (p_arg0[index] == 0 && m_connectionData[index] == 0) {
			result = (short) index;
			m_connectionData[result] = 1;
			break;
		}
		index++;
	}
	return result;
}

// 68K 0x1020c9b8 SetSpecificAddr__10CBroadcastFPCc
// FUNCTION: LEMBALL 0x00460610
void Broadcast::SetSpecificAddr(const char* p_arg0)
{
	if (m_specificAddress != 0) {
		NetworkAddress* address;

		address = m_specificAddress;
		m_specificAddress = 0;
		operator delete(address);
	}
	m_addressMode = 2;
	GetSpecificAddr(p_arg0);
}

// 68K 0x1020ca24 Initialise__10CBroadcastFPCc
// FUNCTION: LEMBALL 0x00460650
void Broadcast::Initialise(const char* p_arg0)
{
	RwSocket::SetNcBuffers(3, 3, 0);
	RwSocket::SetCBuffers(1, 0);
	if (g_pBroadcastPacketTemplate == 0) {
		char* payload;

		g_pBroadcastPacketTemplate = (BasePacketHeader*) new unsigned char[0x410];
		payload = (char*) (g_pBroadcastPacketTemplate + 1);
		strcpy(payload, "ViSOS (VSNET v");
		VsLtoa((long) g_unk0x4a1e08, payload + strlen(payload), 10);
		memcpy(payload + strlen(payload), ".", 2);
		VsLtoa((long) g_unk0x4a1e0c, payload + strlen(payload), 10);
		memcpy(payload + strlen(payload), " ", 2);
		strcat(payload, p_arg0);
		memcpy(payload + strlen(payload), ") is Broadcasting:", 19);
		g_broadcastPayloadLength = strlen(payload);
	}
}

// 68K 0x1020cb54 Stop__10CBroadcastFv
// FUNCTION: LEMBALL 0x004607f0
void Broadcast::Stop()
{
	if (g_szBroadcastPeerName != 0) {
		operator delete(g_szBroadcastPeerName);
		g_szBroadcastPeerName = 0;
	}
	BaseCommonSocket::CloseSocket();
}

// 68K 0x1020cba0 PostRead__10CBroadcastF13NetworkEventsP11CBasePacket
// FUNCTION: LEMBALL 0x00460830
void Broadcast::PostRead(NetworkEvents p_arg0, BasePacket* p_arg1)
{
	BasePacketHeader* packetHeader;
	unsigned char* payload;

	g_pBroadcastReceiveAddress->GetStr();
	if (!(*g_pBroadcastReceiveAddress == *g_pBroadcastAddress) &&
		(m_addressMode != 2 ||
		 (m_specificAddress != 0 && m_addressMode == 2 && *g_pBroadcastReceiveAddress == *m_specificAddress))) {
		unsigned int length;

		length = g_broadcastPayloadLength;
		packetHeader = (BasePacketHeader*) p_arg1->m_data;
		payload = (unsigned char*) (packetHeader + 1);
		if (strncmp((char*) (packetHeader + 1), (char*) (g_pBroadcastPacketTemplate + 1), length + 1) == 0) {
			g_pBaseNetwork->Establish(g_pBroadcastReceiveAddress, payload + length + 1);
		}
	}
	if (p_arg0 == 7) {
		((ReadPacket*) p_arg1)->m_used = 0;
	}
}

// 68K 0x1020ccae AddToMessage__10CBroadcastFR17CBroadcastMessage
// FUNCTION: LEMBALL 0x004608f0
void Broadcast::AddToMessage(BroadcastMessage& p_arg0)
{
	p_arg0.CopyDataStream((unsigned char*) g_pBroadcastPacketTemplate, g_broadcastPayloadLength + 0x11);
}

// 68K 0x1020cd06 Process__10CBroadcastFv
// FUNCTION: LEMBALL 0x00460910
void Broadcast::Process()
{
	if (m_readReady != 0) {
		RwSocket::Process();
		if (m_runEnabled != 0) {
			unsigned long time;

			time = timeGetTime();
			if (1000 < time - m_lastBroadcastTime) {
				bool send;
				NetworkAddress* address;

				send = true;
				g_pMessReqConnect->m_requestedPort = g_broadcastPort;
				g_pMessReqConnect->m_peerName = g_szBroadcastPeerName;
				g_pMessReqConnect->m_connectionData = m_connectionData;
				address = (NetworkAddress*) g_pBaseNetwork->GetNewNetworkAddress();
				if (m_addressMode == 2) {
					if (m_specificAddress != 0) {
						*address = *m_specificAddress;
					}
					else {
						send = false;
					}
				}
				else {
					*address = (eBroadcastTypes) m_addressMode;
				}
				if (send) {
					Send(address, *g_pMessReqConnect);
				}
				operator delete(address);
				m_lastBroadcastTime = time;
			}
		}
	}
}

// 68K 0x1020ce14 SendFailedInit__10CBroadcastF13NetworkErrors
// FUNCTION: LEMBALL 0x004609f0
void Broadcast::SendFailedInit(NetworkErrors p_arg0)
{
	Message message;

	message.type = 2;
	message.code = p_arg0;
	g_pNetworkStatusQueue->Post(message);
}

// 68K 0x1020ce72 Run__10CBroadcastFv
// FUNCTION: LEMBALL 0x00460a20
void Broadcast::Run()
{
	unsigned long time = timeGetTime();
	m_runEnabled = 1;
	m_lastBroadcastTime = time - 1000;
}

// 68K 0x1020ceaa Suspend__10CBroadcastFv
// FUNCTION: LEMBALL 0x00460a40
void Broadcast::Suspend()
{
	m_runEnabled = 0;
}

// 68K 0x1020ced4 Send__10CBroadcastFP15CNetworkAddressR17CBroadcastMessage
// FUNCTION: LEMBALL 0x00460a50
void Broadcast::Send(NetworkAddress* p_arg0, BroadcastMessage& p_arg1)
{
	AddToMessage(p_arg1);
	SetDestAddr(p_arg0);
	WriteSocket::Send(p_arg1);
}

// 68K 0x1010e5ce ResetPort__10CBroadcastFs
// FUNCTION: LEMBALL 0x004629c0
void Broadcast::ResetPort(short p_arg0)
{
	m_connectionData[p_arg0] = 0;
}
