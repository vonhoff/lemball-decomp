#include "CConnect.h"

#include "CBaseNetwork.h"
#include "CBroadcast.h"
#include "CReadSocket.h"
#include "CRwSocket.h"
#include "CWriteSocket.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Queues/CBaseQueueHandler.h"
#include "Engine/Queues/Message.h"
#include "Engine/Time/VsTime.h"
#include "Multiplayer/Transport/CBaseCommonSocket.h"
#include "Multiplayer/Transport/Protocol/CNetworkMessage.h"
#include "NetworkConstants.h"

#include <string.h>

enum {
	CONNECT_TIMEOUT_MS = 4000
};

// FUNCTION: LEMBALL 0x00460a90
CConnect::CConnect()
{
	m_address = NULL;
	m_name = NULL;
	m_previousConnect = NULL;
	m_nextConnect = NULL;
	m_isHost = 0;
	m_killRequested = 0;
	m_established = 0;
	m_newPortRequestCount = 0;
	m_connectTime = CurrentMilliTimer();
}

// FUNCTION: LEMBALL 0x00460c00
CConnect::~CConnect()
{
}

// FUNCTION: LEMBALL 0x00460c60
void CConnect::InitConnect(const char* p_peerName, CNetworkAddress* p_address, short p_port)
{
	m_name = (char*) operator new(strlen(p_peerName) + 1);
	strcpy(m_name, p_peerName);
	SetPort(p_port);
	SetDestAddr(p_address);
}

// FUNCTION: LEMBALL 0x00460ce0
bool CConnect::CheckConnectTime()
{
	unsigned long now;

	if (m_established == 0) {
		now = CurrentMilliTimer();
		if (NETWORK_CONNECT_TIMEOUT_MS < now - m_connectTime) {
			Kill();
			return false;
		}
	}
	return true;
}

// FUNCTION: LEMBALL 0x00460d10
void CConnect::SetConnectTime()
{
	m_connectTime = CurrentMilliTimer();
}

// FUNCTION: LEMBALL 0x00460d20
void CConnect::Stop()
{
	if (m_name != NULL) {
		operator delete(m_name);
		m_name = NULL;
	}
	if (m_address != NULL) {
		operator delete(m_address);
		m_address = NULL;
	}
	CBaseCommonSocket::CloseSocket();
}

// FUNCTION: LEMBALL 0x00460d70
void CConnect::FirstReceive()
{
	Message message;

	message.m_type = CONNECT_QUEUE_FIRST_RECEIVE;
	m_established = 1;
	message.m_code = 0;
	message.m_payload = this;
	m_writeReady = 0;
	m_eventPending = m_writeReady;
	m_readReady = 1;
	if (m_isHost != 0) {
		m_closePending = 1;
		CWriteSocket::m_lastSendTime = CurrentMilliTimer() - NETWORK_CLOSE_PENDING_PULSE_INTERVAL_MS;
		CReadSocket::m_lastReceiveTime = CurrentMilliTimer();
		if (g_pBaseNetwork->m_suspendBroadcastOnConnect != 0) {
			g_pBaseNetwork->m_broadcast->Suspend();
		}
	}
	g_pNetworkStatusQueue->Post(message);
}

// FUNCTION: LEMBALL 0x00460e40
bool CConnect::Send(CNetworkMessage& p_message)
{
	bool opened;
	bool isOpen;
	bool sent;
	Message message;

	if (m_readReady != 0 && m_killRequested == 0) {
		isOpen = p_message.m_openDepth > 0;
		opened = !isOpen;
		if (opened) {
			p_message.OpenDataStream();
		}
		sent = CWriteSocket::Send(p_message);
		if (!sent) {
			message.m_type = CONNECT_QUEUE_SEND_FAILED;
			message.m_code = 0xc;
			if (p_message.m_headerEnabled == 0) {
				message.m_code = 0xb;
			}
			message.m_payload = &p_message;
			message.m_source = this;
			g_pNetworkStatusQueue->Post(message);
		}
		if (opened) {
			p_message.CloseDataStream();
		}
		return sent;
	}
	p_message.m_pendingSendCount = 0;
	return false;
}

// FUNCTION: LEMBALL 0x00460f00
void CConnect::Closed(int p_notifyPeer)
{
	Message message;

	m_killRequested = 1;
	CRwSocket::Closed(p_notifyPeer);
	if (p_notifyPeer != 0) {
		message.m_type = CONNECT_QUEUE_CLOSED;
		message.m_code = 0;
		message.m_payload = this;
		g_pNetworkStatusQueue->Post(message);
	}
}

// FUNCTION: LEMBALL 0x00460f60
CNetworkMessage* CConnect::ReceiveAcknowledgement()
{
	CNetworkMessage* acknowledgement;
	Message message;

	acknowledgement = CWriteSocket::ReceiveAcknowledgement();
	if (acknowledgement != NULL) {
		message.m_type = CONNECT_QUEUE_ACKNOWLEDGEMENT;
		message.m_code = 0;
		message.m_payload = this;
		message.m_source = acknowledgement;
		g_pNetworkPacketQueue->Post(message);
	}
	return acknowledgement;
}

// FUNCTION: LEMBALL 0x00460fb0
void CConnect::Kill()
{
	if (m_isOpen != 0 && m_readReady != 0) {
		CBaseCommonSocket::CloseSocket();
		if (m_established != 0) {
			Closed(1);
		}
	}
	m_killRequested = 1;
}

// FUNCTION: LEMBALL 0x00460ff0
void CConnect::PostRead(NetworkEvents p_event, CBasePacket* p_packet)
{
	Message message;

	message.m_type = p_event;
	message.m_code = 0;
	message.m_payload = this;
	message.m_source = p_packet;
	g_pNetworkPacketQueue->Post(message);
}

// FUNCTION: LEMBALL 0x00461030
void CConnect::Process()
{
	if (m_killRequested == 0) {
		if (m_established == 0 && m_eventPending == 0) {
			if (CONNECT_TIMEOUT_MS < CurrentMilliTimer() - m_connectTime) {
				Kill();
				return;
			}
		}
		else {
			CReadSocket::Process();
			CWriteSocket::Process();
		}
	}
}

// FUNCTION: LEMBALL 0x004629d0
void CConnect::ConnectSetup()
{
}

// GLOBAL: LEMBALL 0x004a011c
CConnect* g_pActiveConnection = NULL;
