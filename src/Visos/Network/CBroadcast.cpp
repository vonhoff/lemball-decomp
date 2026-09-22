#include "CBroadcast.h"

#include "../Foundation/CBaseQueue.h"
#include "../Foundation/VsString.h"
#include "../Messaging/BasePacketHeader.h"
#include "../Messaging/CBasePacket.h"
#include "../Messaging/CBroadcastMessage.h"
#include "../Messaging/CMessReqConnect.h"
#include "../Messaging/CReadPacket.h"
#include "CBaseNetwork.h"
#include "CNetworkAddress.h"
#include "CTcpIpNetwork.h"
#include "Visos/Foundation/CBaseQueueHandler.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Network/CRwSocket.h"
#include "Visos/Network/CWriteSocket.h"

#include <memory.h>
#include <string.h>

#pragma intrinsic(memcpy, strcat, strcpy, strlen)

extern "C" unsigned long __stdcall timeGetTime(void);

// GLOBAL: LEMBALL 0x004a1e08
int g_unk0x4a1e08 = 0;

// GLOBAL: LEMBALL 0x004a1e0c
int g_unk0x4a1e0c = 9;

// FUNCTION: LEMBALL 0x00460350
CBroadcast::CBroadcast()
{
	int index;

	m_listenEnabled = 0;
	m_runEnabled = 0;
	m_addressMode = 0;
	m_specificAddress = 0;
	g_pBroadcastAddress = (CNetworkAddress*) g_pBaseNetwork->GetNewNetworkAddress();
	m_connectionData = new unsigned char[0x200];
	for (index = 0; index < 0x200; index++) {
		m_connectionData[index] = 0;
	}
}

// FUNCTION: LEMBALL 0x004604e0
CBroadcast::~CBroadcast()
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

// FUNCTION: LEMBALL 0x004605b0
void CBroadcast::Closed(int p_notifyPeer)
{
	(void) p_notifyPeer;
	g_pBaseNetwork->KillUnBornConnection(CWriteSocket::m_destinationAddress);
}

// FUNCTION: LEMBALL 0x004605d0
short CBroadcast::FindPort(const unsigned char* p_arg0)
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

// FUNCTION: LEMBALL 0x00460610
void CBroadcast::SetSpecificAddr(const char* p_arg0)
{
	if (m_specificAddress != 0) {
		CNetworkAddress* address;

		address = m_specificAddress;
		m_specificAddress = 0;
		operator delete(address);
	}
	m_addressMode = 2;
	GetSpecificAddr(p_arg0);
}

// FUNCTION: LEMBALL 0x00460650
void CBroadcast::Initialise(const char* p_arg0)
{
	CRwSocket::SetNcBuffers(3, 3, 0);
	CRwSocket::SetCBuffers(1, 0);
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

// FUNCTION: LEMBALL 0x004607f0
void CBroadcast::Stop()
{
	if (g_szBroadcastPeerName != 0) {
		operator delete(g_szBroadcastPeerName);
		g_szBroadcastPeerName = 0;
	}
	CBaseCommonSocket::CloseSocket();
}

// FUNCTION: LEMBALL 0x00460830
void CBroadcast::PostRead(NetworkEvents p_arg0, CBasePacket* p_arg1)
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
		((CReadPacket*) p_arg1)->m_used = 0;
	}
}

// FUNCTION: LEMBALL 0x004608f0
void CBroadcast::AddToMessage(CBroadcastMessage& p_arg0)
{
	p_arg0.CopyDataStream((unsigned char*) g_pBroadcastPacketTemplate, g_broadcastPayloadLength + 0x11);
}

// FUNCTION: LEMBALL 0x00460910
void CBroadcast::Process()
{
	if (m_readReady != 0) {
		CRwSocket::Process();
		if (m_runEnabled != 0) {
			unsigned long time;

			time = timeGetTime();
			if (1000 < time - m_lastBroadcastTime) {
				bool send;
				CNetworkAddress* address;

				send = true;
				g_pMessReqConnect->m_requestedPort = g_broadcastPort;
				g_pMessReqConnect->m_peerName = g_szBroadcastPeerName;
				g_pMessReqConnect->m_connectionData = m_connectionData;
				address = (CNetworkAddress*) g_pBaseNetwork->GetNewNetworkAddress();
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

// FUNCTION: LEMBALL 0x004609f0
void CBroadcast::SendFailedInit(NetworkErrors p_arg0)
{
	Message message;

	message.type = 2;
	message.code = p_arg0;
	g_pNetworkStatusQueue->Post(message);
}

// FUNCTION: LEMBALL 0x00460a20
void CBroadcast::Run()
{
	unsigned long time = timeGetTime();
	m_runEnabled = 1;
	m_lastBroadcastTime = time - 1000;
}

// FUNCTION: LEMBALL 0x00460a40
void CBroadcast::Suspend()
{
	m_runEnabled = 0;
}

// FUNCTION: LEMBALL 0x00460a50
void CBroadcast::Send(CNetworkAddress* p_arg0, CBroadcastMessage& p_arg1)
{
	AddToMessage(p_arg1);
	SetDestAddr(p_arg0);
	CWriteSocket::Send(p_arg1);
}

// FUNCTION: LEMBALL 0x004629c0
void CBroadcast::ResetPort(short p_arg0)
{
	m_connectionData[p_arg0] = 0;
}
