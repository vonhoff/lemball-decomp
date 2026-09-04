#include "Connect.h"

#include "../Foundation/BaseQueue.h"
#include "../Foundation/VsMem.h"
#include "../Foundation/VsTime.h"
#include "BaseNetwork.h"
#include "Broadcast.h"

#include <new.h>
#include <string.h>

extern "C" unsigned long __stdcall timeGetTime(void);

// 68K 0x1020cf50 __ct__8CConnectFv
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
	m_connectTime = timeGetTime();
}

// 68K 0x1020d0c6 __dt__8CConnectFv
// FUNCTION: LEMBALL 0x00460c00
CConnect::~CConnect()
{
}

// 68K 0x1020d1c2 InitConnect__8CConnectFPCcP15CNetworkAddresss
// FUNCTION: LEMBALL 0x00460c60
void Connect::InitConnect(const char* p_arg0, NetworkAddress* p_arg1, short p_arg2)
{
	m_name = (char*) operator new(strlen(p_arg0) + 1);
	strcpy(m_name, p_arg0);
	SetPort(p_arg2);
	SetDestAddr(p_arg1);
}

// 68K 0x1020d25e CheckConnectTime__8CConnectFv
// FUNCTION: LEMBALL 0x00460ce0
bool Connect::CheckConnectTime()
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

// 68K 0x1020d2b6 SetConnectTime__8CConnectFv
// FUNCTION: LEMBALL 0x00460d10
void Connect::SetConnectTime()
{
	m_connectTime = timeGetTime();
}

// 68K 0x1020d2ea Stop__8CConnectFv
// FUNCTION: LEMBALL 0x00460d20
void Connect::Stop()
{
	if (m_name != 0) {
		operator delete(m_name);
		m_name = 0;
	}
	if (m_address != 0) {
		operator delete(m_address);
		m_address = 0;
	}
	BaseCommonSocket::CloseSocket();
}

// 68K 0x1020d348 FirstReceive__8CConnectFv
// FUNCTION: LEMBALL 0x00460d70
void Connect::FirstReceive()
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
		WriteSocket::m_lastSendTime = timeGetTime() - 1000;
		ReadSocket::m_lastReceiveTime = timeGetTime();
		if (g_pBaseNetwork->m_unk0x30 != 0) {
			g_pBaseNetwork->m_broadcast->Suspend();
		}
	}
	g_pNetworkStatusQueue->Post(message);
}

// 68K 0x1020d414 Send__8CConnectFR15CNetworkMessage
// FUNCTION: LEMBALL 0x00460e40
bool Connect::Send(NetworkMessage& p_arg0)
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
		sent = WriteSocket::Send(p_arg0);
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

// 68K 0x1020d4ea Closed__8CConnectFUc
// FUNCTION: LEMBALL 0x00460f00
void Connect::Closed(unsigned char p_arg0)
{
	register unsigned int notifyPeer;
	Message message;

	notifyPeer = *(volatile unsigned int*) &p_arg0;
	m_killRequested = 1;
	RwSocket::Closed((unsigned char) notifyPeer);
	if (notifyPeer != 0) {
		message.type = 10;
		message.code = 0;
		message.payload = this;
		g_pNetworkStatusQueue->Post(message);
	}
}

// 68K 0x1020d556 ReceiveAcknowledgement__8CConnectFv
// FUNCTION: LEMBALL 0x00460f60
NetworkMessage* Connect::ReceiveAcknowledgement()
{
	NetworkMessage* acknowledgement;
	Message message;

	acknowledgement = WriteSocket::ReceiveAcknowledgement();
	if (acknowledgement != 0) {
		message.type = 6;
		message.code = 0;
		message.payload = this;
		message.source = acknowledgement;
		g_pNetworkPacketQueue->Post(message);
	}
	return acknowledgement;
}

// 68K 0x1020d5cc Kill__8CConnectFv
// FUNCTION: LEMBALL 0x00460fb0
void Connect::Kill()
{
	if (m_isOpen != 0 && m_readReady != 0) {
		BaseCommonSocket::CloseSocket();
		if (m_established != 0) {
			Closed(1);
		}
	}
	m_killRequested = 1;
}

// 68K 0x1020d63a PostRead__8CConnectF13NetworkEventsP11CBasePacket
// FUNCTION: LEMBALL 0x00460ff0
void Connect::PostRead(NetworkEvents p_arg0, BasePacket* p_arg1)
{
	Message message;

	message.type = p_arg0;
	message.code = 0;
	message.payload = this;
	message.source = p_arg1;
	g_pNetworkPacketQueue->Post(message);
}

// 68K 0x1020d6a8 Process__8CConnectFv
// FUNCTION: LEMBALL 0x00461030
void Connect::Process()
{
	if (m_killRequested == 0) {
		if (m_established == 0 && m_eventPending == 0) {
			if (4000 < timeGetTime() - m_connectTime) {
				Kill();
				return;
			}
		}
		else {
			ReadSocket::Process();
			WriteSocket::Process();
		}
	}
}

// 68K 0x1010e682 ConnectSetup__8CConnectFv
// FUNCTION: LEMBALL 0x004629d0
void Connect::ConnectSetup()
{
}

// GLOBAL: LEMBALL 0x004a011c
Connect* g_pActiveConnection = 0;
