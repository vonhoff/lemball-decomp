#include "CConnect.h"

#include "../Foundation/CBaseQueue.h"
#include "../Foundation/VsTime.h"
#include "CBaseNetwork.h"
#include "CBroadcast.h"
#include "Visos/Foundation/CBaseQueueHandler.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Messaging/CNetworkMessage.h"
#include "Visos/Network/CReadSocket.h"
#include "Visos/Network/CRwSocket.h"
#include "Visos/Network/CWriteSocket.h"

#include <string.h>

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00460a90
CConnect::CConnect()
{
	m_address = 0;
	m_name = 0;
	m_previousConnect = 0;
	m_nextConnect = 0;
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
void CConnect::InitConnect(const char* p_arg0, CNetworkAddress* p_arg1, short p_arg2)
{
	m_name = (char*) operator new(strlen(p_arg0) + 1);
	strcpy(m_name, p_arg0);
	SetPort(p_arg2);
	SetDestAddr(p_arg1);
}

// FUNCTION: LEMBALL 0x00460ce0
bool CConnect::CheckConnectTime()
{
	unsigned long now;

	if (m_established == 0) {
		now = timeGetTime();
		if (4000 < now - m_connectTime) {
			Kill();
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00460d10
void CConnect::SetConnectTime()
{
	m_connectTime = timeGetTime();
}

// FUNCTION: LEMBALL 0x00460d20
void CConnect::Stop()
{
	if (m_name != 0) {
		operator delete(m_name);
		m_name = 0;
	}
	if (m_address != 0) {
		operator delete(m_address);
		m_address = 0;
	}
	CBaseCommonSocket::CloseSocket();
}

// FUNCTION: LEMBALL 0x00460d70
void CConnect::FirstReceive()
{
	Message message;

	message.type = 3;
	message.code = 0;
	message.payload = this;
	m_established = 1;
	m_writeReady = 0;
	m_eventPending = m_writeReady;
	m_readReady = 1;
	if (m_isHost != 0) {
		m_closePending = 1;
		CWriteSocket::m_lastSendTime = timeGetTime() - 1000;
		CReadSocket::m_lastReceiveTime = timeGetTime();
		if (g_pBaseNetwork->m_suspendBroadcastOnConnect != 0) {
			g_pBaseNetwork->m_broadcast->Suspend();
		}
	}
	g_pNetworkStatusQueue->Post(message);
}

// FUNCTION: LEMBALL 0x00460e40
bool CConnect::Send(CNetworkMessage& p_arg0)
{
	bool opened;
	bool isOpen;
	bool sent;
	Message message;

	if (m_readReady != 0 && m_killRequested == 0) {
		isOpen = (int) p_arg0.m_openDepth > 0;
		opened = !isOpen;
		if (opened) {
			p_arg0.OpenDataStream();
		}
		sent = CWriteSocket::Send(p_arg0);
		if (!sent) {
			message.type = 1;
			message.code = 0xc;
			if (p_arg0.m_headerEnabled == 0) {
				message.code = 0xb;
			}
			message.payload = &p_arg0;
			message.source = this;
			g_pNetworkStatusQueue->Post(message);
		}
		if (opened) {
			p_arg0.CloseDataStream();
		}
		return sent;
	}
	p_arg0.m_pendingSendCount = 0;
	return 0;
}

// FUNCTION: LEMBALL 0x00460f00
void CConnect::Closed(int p_notifyPeer)
{
	Message message;

	m_killRequested = 1;
	CRwSocket::Closed(p_notifyPeer);
	if (p_notifyPeer != 0) {
		message.type = 10;
		message.code = 0;
		message.payload = this;
		g_pNetworkStatusQueue->Post(message);
	}
}

// FUNCTION: LEMBALL 0x00460f60
CNetworkMessage* CConnect::ReceiveAcknowledgement()
{
	CNetworkMessage* acknowledgement;
	Message message;

	acknowledgement = CWriteSocket::ReceiveAcknowledgement();
	if (acknowledgement != 0) {
		message.type = 6;
		message.code = 0;
		message.payload = this;
		message.source = acknowledgement;
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
void CConnect::PostRead(NetworkEvents p_arg0, CBasePacket* p_arg1)
{
	Message message;

	message.type = p_arg0;
	message.code = 0;
	message.payload = this;
	message.source = p_arg1;
	g_pNetworkPacketQueue->Post(message);
}

// FUNCTION: LEMBALL 0x00461030
void CConnect::Process()
{
	if (m_killRequested == 0) {
		if (m_established == 0 && m_eventPending == 0) {
			if (4000 < timeGetTime() - m_connectTime) {
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
CConnect* g_pActiveConnection = 0;
