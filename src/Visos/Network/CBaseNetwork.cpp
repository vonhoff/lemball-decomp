#include "CBaseNetwork.h"

#include "../../Network/Game/CNetworkManager.h"
#include "../Foundation/CBaseQueue.h"
#include "../Foundation/CBaseQueueHandler.h"
#include "../Messaging/CAckMessage.h"
#include "../Messaging/CMessFAILEDConnect.h"
#include "../Messaging/CMessGOConnect.h"
#include "../Messaging/CMessOKConnect.h"
#include "../Messaging/CMessReqConnect.h"
#include "../Messaging/CMessReqNewPort.h"
#include "../Messaging/CNetworkMessage.h"
#include "../Messaging/CPulseMessage.h"
#include "CBroadcast.h"
#include "CConnect.h"
#include "CNetworkAddress.h"
#include "Visos/Foundation/Message.h"

struct BasePacketHeader;

extern int g_lastNetworkError;
extern unsigned int g_networkPacketSize;
extern BasePacketHeader* g_pNetworkPacketScratch;
extern char* g_szBroadcastPeerName;
extern unsigned short g_broadcastPort;
extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x004619f0
CBaseNetwork::CBaseNetwork()
{
	g_lastNetworkError = 0;
	m_lastConnect = 0;
	m_firstConnect = 0;
	m_broadcastMode = 0;
	m_suspendBroadcastOnConnect = 0;
	m_initialized = 0;
	m_initialisePending = 0;
	m_pendingDetachQueue = 0;
	m_activeStatusItem = 0;
	m_pendingAttachQueue = 0;
	m_messageQueue = 0;
	m_queueTransitionPending = 0;
	m_shutdownRequested = 0;
	m_serverMode = 0;
	m_criticalRetryLimit = 0x50;
	m_broadcast = 0;

	g_pNetworkStatusQueue = new CBaseQueue(0x1e);
	g_pNetworkStatusQueue->Attach(this, 0x19);
	g_pNetworkPacketQueue = new CBaseQueue(0x1e);
}

// FUNCTION: LEMBALL 0x00461aa0
bool CBaseNetwork::Initialise(const char* p_networkName, int p_packetSize)
{
	unsigned long start;
	unsigned long waitStart;

	m_initialisePending = 1;
	m_networkName = (char*) p_networkName;
	g_networkPacketSize = p_packetSize;
	ForceProcess();
	start = timeGetTime();
	if (m_initialized == 0) {
		do {
			if (g_lastNetworkError != 0 || timeGetTime() - start >= 10000) {
				break;
			}
			WaitProcess();
		} while (m_initialized == 0);
		if (m_initialized == 0) {
			return 0;
		}
	}

	start = timeGetTime();
	while (m_serverMode != 0 && !(m_serverMode != 0 && m_broadcast != 0 && m_broadcast->m_readReady != 0) &&
		   g_lastNetworkError == 0 && timeGetTime() - start < 10000) {
		waitStart = timeGetTime();
		while (timeGetTime() - waitStart < 100) {
		}
		ForceProcess();
	}

	if (m_serverMode != 0) {
		if (m_serverMode != 0) {
			if (m_broadcast != 0) {
				if (m_broadcast->m_readReady != 0) {
					if (g_lastNetworkError == 0) {
						return 1;
					}
				}
			}
		}
	}

	m_shutdownRequested = 1;
	ForceProcess();
	start = timeGetTime();
	while (m_initialized != 0 && timeGetTime() - start < 10000) {
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00461bd0
bool CBaseNetwork::DoInitialise()
{
	m_initialisePending = 0;
	Initialise();
	if (g_lastNetworkError != 0) {
		return 0;
	}

	g_pMessReqConnect = new CMessReqConnect("Request Connect");
	g_pMessReqNewPort = new CMessReqNewPort("Request New Port");
	g_pMessOKConnect = new CMessOKConnect("Authorise Connect");
	g_pMessGOConnect = new CMessGOConnect("Go Ahead Connect");
	g_pMessFAILEDConnect = new CMessFAILEDConnect("Failed Connect");
	g_pPulseMessage = new CPulseMessage;
	g_pAckMessage = new CAckMessage;
	m_broadcast = (CBroadcast*) GetNewBroadcast();
	m_initialized = 1;
	m_serverMode = 1;
	ForceProcess();
	if (m_serverMode != 0 && g_lastNetworkError == 0) {
		if (m_broadcast->Start(m_networkName) == 0) {
			m_serverMode = 0;
			return 0;
		}
		return g_lastNetworkError == 0;
	}
	m_serverMode = 0;
	return 0;
}

// FUNCTION: LEMBALL 0x00461db0
CBaseNetwork::~CBaseNetwork()
{
	CBaseQueue* queue;

	DetachMessageQueue();
	queue = *(CBaseQueue* volatile*) &g_pNetworkPacketQueue;
	if (queue != 0) {
		delete queue;
	}
	g_pNetworkPacketQueue = 0;
	g_pNetworkStatusQueue->Detach(this, 0x19);
	queue = *(CBaseQueue* volatile*) &g_pNetworkStatusQueue;
	if (queue != 0) {
		delete queue;
	}
	g_pNetworkStatusQueue = 0;
}

// FUNCTION: LEMBALL 0x00461e10
void CBaseNetwork::ShutDown()
{
	CConnect* peer;
	CConnect* next;
	CNetworkMessage* message;
	short port;

	if (m_initialized != 0) {
		m_initialized = 0;
		m_serverMode = 0;
		peer = m_firstConnect;
		if (peer != 0) {
			BeforeDestroyConnections();
			while (1) {
				if (peer == 0) {
					break;
				}
				next = peer->m_nextConnect;
				port = peer->m_port;
				peer->Stop();
				delete peer;
				if (port != -1) {
					m_broadcast->ResetPort(port);
				}
				peer = next;
			}
			AfterDestroyConnections();
		}

		if (m_broadcast != 0) {
			m_broadcast->Stop();
			delete m_broadcast;
		}
		if (g_pNetworkPacketScratch != 0) {
			operator delete(g_pNetworkPacketScratch);
		}
		g_pNetworkPacketScratch = 0;
		if (g_pBroadcastReceiveAddress != 0) {
			delete g_pBroadcastReceiveAddress;
		}
		g_pBroadcastReceiveAddress = 0;
		message = *(CNetworkMessage* volatile*) &g_pAckMessage;
		if (message != 0) {
			delete message;
		}
		g_pAckMessage = 0;
		message = *(CNetworkMessage* volatile*) &g_pPulseMessage;
		if (message != 0) {
			delete message;
		}
		g_pPulseMessage = 0;
		message = *(CNetworkMessage* volatile*) &g_pMessReqConnect;
		if (message != 0) {
			delete message;
		}
		g_pMessReqConnect = 0;
		message = *(CNetworkMessage* volatile*) &g_pMessReqNewPort;
		if (message != 0) {
			delete message;
		}
		g_pMessReqNewPort = 0;
		message = *(CNetworkMessage* volatile*) &g_pMessOKConnect;
		if (message != 0) {
			delete message;
		}
		g_pMessOKConnect = 0;
		message = *(CNetworkMessage* volatile*) &g_pMessGOConnect;
		if (message != 0) {
			delete message;
		}
		g_pMessGOConnect = 0;
		message = *(CNetworkMessage* volatile*) &g_pMessFAILEDConnect;
		if (message != 0) {
			delete message;
		}
		g_pMessFAILEDConnect = 0;
		UnInitialise();
	}
}

// FUNCTION: LEMBALL 0x00461fc0
void CBaseNetwork::Delete(CConnect* p_connection)
{
	CConnect* peer = m_firstConnect;
	CConnect* next;
	CConnect* previous;
	if (peer != 0) {
		while (peer != p_connection) {
			peer = peer->m_nextConnect;
			if (peer == 0) {
				return;
			}
		}
		next = peer->m_nextConnect;
		previous = peer->m_previousConnect;
		if (m_lastConnect == peer) {
			m_lastConnect = previous;
		}
		if (peer == m_firstConnect) {
			m_firstConnect = next;
		}
		m_broadcast->ResetPort(peer->m_port);
		peer->Stop();
		delete peer;
		if (previous != 0) {
			previous->m_nextConnect = next;
		}
		if (next != 0) {
			next->m_previousConnect = previous;
		}
	}
}

// FUNCTION: LEMBALL 0x00462040
CConnect* CBaseNetwork::NewConnect()
{
	bool removed;
	CConnect* peer;
	CConnect* next;

	peer = m_firstConnect;
	removed = false;
	while (1) {
		if (peer == 0) {
			break;
		}
		next = peer->m_nextConnect;
		if (peer->m_killRequested != 0) {
			if (!removed) {
				removed = true;
				BeforeDestroyConnections();
			}
			Delete(peer);
		}
		peer = next;
	}
	if (removed) {
		AfterDestroyConnections();
	}

	peer = (CConnect*) GetNewConnect();
	if (m_firstConnect == 0) {
		m_firstConnect = peer;
	}
	else {
		m_lastConnect->m_nextConnect = peer;
	}
	peer->m_previousConnect = m_lastConnect;
	m_lastConnect = peer;
	peer->SetNCBuffers(m_lastSinglePacketMessageId, m_lastNonCriticalMessageId, m_nonCriticalMessageCapacity);
	peer->SetCBuffers(m_criticalPacketCount, m_nonCriticalMessageCapacity);
	return peer;
}

// FUNCTION: LEMBALL 0x00462130
bool CBaseNetwork::Exists(CConnect* p_connection)
{
	CConnect* peer;

	peer = m_firstConnect;
	while (peer != 0) {
		if (p_connection == peer) {
			if (peer->CheckConnectTime() == 0) {
				return 0;
			}
			if (peer->m_killRequested == 0) {
				peer->SetConnectTime();
				return 1;
			}
			return 0;
		}
		peer = peer->m_nextConnect;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00462180
CConnect* CBaseNetwork::FindConnection(CNetworkAddress* p_address)
{
	CConnect* peer = m_firstConnect;
	while (peer != 0) {
		if (peer->m_killRequested == 0 && *peer->m_destinationAddress == *p_address) {
			break;
		}
		peer = peer->m_nextConnect;
	}
	return peer;
}

// FUNCTION: LEMBALL 0x004621c0
void CBaseNetwork::KillUnBornConnection(CNetworkAddress* p_address)
{
	CConnect* peer = FindConnection(p_address);
	if (peer != 0) {
		peer->Kill();
	}
}

// FUNCTION: LEMBALL 0x004621e0
void CBaseNetwork::CtoSRequestConnect(CNetworkAddress* p_address)
{
	CConnect* peer;
	short port;

	peer = FindConnection(p_address);
	if (peer != 0 && peer->m_killRequested == 0 && peer->CheckConnectTime() != 0) {
		return;
	}

	peer = NewConnect();
	port = m_broadcast->FindPort(g_pMessReqConnect->m_connectionData);
	if (port == -1) {
		peer->Kill();
		return;
	}

	peer->InitConnect(g_pMessReqConnect->m_peerName, p_address, port);
	g_pMessOKConnect->m_assignedPort = peer->m_port;
	g_pMessOKConnect->m_connectionId = (unsigned int) peer;
	m_broadcast->Send(p_address, *g_pMessOKConnect);
}

// FUNCTION: LEMBALL 0x00462280
void CBaseNetwork::CtoSRequestNewPort(CNetworkAddress* p_address)
{
	CConnect* peer;
	short port;

	peer = (CConnect*) g_pMessReqNewPort->m_connectionId;
	if (Exists(peer) != 0) {
		m_broadcast->ResetPort(peer->m_port);
		peer->m_newPortRequestCount++;
		if (peer->m_newPortRequestCount > 5) {
			g_pMessFAILEDConnect->m_failureReason = "To many new-port requests";
			m_broadcast->Send(p_address, *g_pMessReqNewPort);
			return;
		}

		port = m_broadcast->FindPort(g_pMessReqNewPort->m_connectionData);
		if (port != -1) {
			peer->SetPort(port);
			g_pMessOKConnect->m_assignedPort = peer->m_port;
			g_pMessOKConnect->m_connectionId = (unsigned int) peer;
			m_broadcast->Send(p_address, *g_pMessOKConnect);
		}
	}
}

// FUNCTION: LEMBALL 0x00462340
void CBaseNetwork::StoCOKConnect(CNetworkAddress* p_address)
{
	CConnect* peer;
	short port;

	peer = FindConnection(p_address);
	if (peer != 0 && peer->m_killRequested == 0 && peer->CheckConnectTime() != 0) {
		if (*g_pBroadcastAddress > *p_address) {
			return;
		}
		peer->Kill();
	}

	if (m_suspendBroadcastOnConnect != 0) {
		m_broadcast->Suspend();
	}

	port = g_pMessOKConnect->m_assignedPort;
	if (m_broadcast->m_connectionData[port] == 0) {
		m_broadcast->m_connectionData[port] = 1;
		peer = NewConnect();
		peer->SetPort(port);
		peer->Listen(p_address);
		g_pMessGOConnect->m_assignedPort = port;
		g_pMessGOConnect->m_connectionId = g_pMessOKConnect->m_connectionId;
		m_broadcast->Send(p_address, *g_pMessGOConnect);
		return;
	}

	g_pMessReqNewPort->m_connectionId = g_pMessOKConnect->m_connectionId;
	g_pMessReqNewPort->m_requestedPort = g_broadcastPort;
	g_pMessReqNewPort->m_connectionData = m_broadcast->m_connectionData;
	g_pMessReqNewPort->m_peerName = g_szBroadcastPeerName;
	m_broadcast->Send(p_address, *g_pMessReqNewPort);
}

// FUNCTION: LEMBALL 0x00462460
void CBaseNetwork::StoCFAILEDConnect(CNetworkAddress* p_address)
{
	m_broadcast->Send(p_address, *g_pMessFAILEDConnect);
}

// FUNCTION: LEMBALL 0x00462480
void CBaseNetwork::CtoSGOConnect(CNetworkAddress* p_address)
{
	CConnect* peer = (CConnect*) g_pMessGOConnect->m_connectionId;
	if (Exists(peer) != 0) {
		peer->Connect();
	}
}

// FUNCTION: LEMBALL 0x004624a0
void CBaseNetwork::Establish(CNetworkAddress* p_address, unsigned char* p_data)
{
	p_address->GetStr();
	if (g_pMessReqConnect->Set(p_data) != 0) {
		CtoSRequestConnect(p_address);
		return;
	}
	if (g_pMessReqNewPort->Set(p_data) != 0) {
		CtoSRequestNewPort(p_address);
		return;
	}
	if (g_pMessOKConnect->Set(p_data) != 0) {
		StoCOKConnect(p_address);
		return;
	}
	if (g_pMessGOConnect->Set(p_data) != 0) {
		CtoSGOConnect(p_address);
		return;
	}
	if (g_pMessFAILEDConnect->Set(p_data) != 0) {
		StoCFAILEDConnect(p_address);
	}
}

// FUNCTION: LEMBALL 0x00462550
void CBaseNetwork::SetNCBuffers(unsigned long p_lastSinglePacketMessageId,
								unsigned long p_lastMessageId,
								int p_messageCapacity)
{
	m_lastSinglePacketMessageId = p_lastSinglePacketMessageId;
	m_lastNonCriticalMessageId = p_lastMessageId;
	m_nonCriticalMessageCapacity = p_messageCapacity;
}

// FUNCTION: LEMBALL 0x00462570
void CBaseNetwork::SetCBuffers(int p_packetCount, int p_messageCapacity)
{
	m_criticalPacketCount = p_packetCount;
	m_criticalMessageCapacity = p_messageCapacity;
}

// FUNCTION: LEMBALL 0x00462590
void CBaseNetwork::AttachMessageQueue(CBaseQueueHandler* p_queueHandler)
{
	m_messageQueue = p_queueHandler;
	g_pNetworkPacketQueue->Attach(p_queueHandler, 0);
}

// FUNCTION: LEMBALL 0x004625b0
void CBaseNetwork::DetachMessageQueue()
{
	if (m_messageQueue != 0) {
		g_pNetworkPacketQueue->Detach(m_messageQueue, 0);
		m_messageQueue = 0;
	}
}

// FUNCTION: LEMBALL 0x004625e0
void CBaseNetwork::Process()
{
	if (m_activeStatusItem != 0) {
		g_pNetworkStatusQueue->Attach((CBaseQueueHandler*) m_activeStatusItem, 0);
		m_queueTransitionPending = 1;
		m_pendingAttachQueue = (CBaseQueueHandler*) m_activeStatusItem;
		m_activeStatusItem = 0;
	}

	if (m_pendingDetachQueue != 0) {
		g_pNetworkStatusQueue->Detach(m_pendingDetachQueue, 0);
		m_pendingDetachQueue = 0;
		m_pendingAttachQueue = 0;
		m_queueTransitionPending = 0;
	}

	if (g_pBaseNetwork->m_initialisePending != 0) {
		g_pBaseNetwork->DoInitialise();
		return;
	}

	if (m_shutdownRequested != 0) {
		ShutDown();
		return;
	}

	if (m_broadcast != 0) {
		m_broadcast->Process();
	}

	CConnect* peer = m_firstConnect;
	while (peer != 0) {
		peer->Process();
		peer = peer->m_nextConnect;
	}

	if (m_pendingAttachQueue != 0) {
		((CNetworkManager*) m_pendingAttachQueue)->Process();
	}
}

// FUNCTION: LEMBALL 0x00462690
void CBaseNetwork::HandleNewConnectionEvent(const char* p_localName, const char* p_remoteName)
{
	NewConnect()->Start(p_localName, p_remoteName);
}

// FUNCTION: LEMBALL 0x004626b0
CConnect* CBaseNetwork::FindEventConnection(CNetworkAddress* p_address)
{
	CConnect* peer = m_firstConnect;
	while (peer != 0) {
		if ((*p_address == *peer->m_destinationAddress) == 0) {
			break;
		}
		peer = peer->m_nextConnect;
	}
	return peer;
}

// FUNCTION: LEMBALL 0x004626f0
void CBaseNetwork::HandleConnectionMessage(CNetworkAddress* p_address)
{
	CConnect* peer = FindEventConnection(p_address);
	if (peer != 0) {
		BeforeDestroyConnections();
		Delete(peer);
		AfterDestroyConnections();
	}
}

// FUNCTION: LEMBALL 0x00462720
bool CBaseNetwork::SendAll(CNetworkMessage& p_message)
{
	CConnect* peer;
	int activeCount;
	bool sendBlocked;

	sendBlocked = 0;
	activeCount = 0;
	peer = m_firstConnect;

	while (1) {
		if (peer == 0) {
			if (sendBlocked == 0 && activeCount > 0) {
				return 1;
			}
			return 0;
		}

		if (peer->m_killRequested == 0) {
			activeCount = activeCount + 1;
			if (sendBlocked == 0) {
				if (peer->Send(p_message) != 0) {
					sendBlocked = 0;
					peer = peer->m_nextConnect;
					continue;
				}
			}
			sendBlocked = 1;
		}

		peer = peer->m_nextConnect;
	}
}

// FUNCTION: LEMBALL 0x004627b0
int CBaseNetwork::ProcessMsg(Message* p_message)
{
	unsigned int type;
	Message* message;
	CNetworkMessage* stream;
	CConnect* peer;

	type = 0;
	message = p_message;
	type = message->type;
	switch (type) {
	case 0xb:
		if (message->code == 1) {
			stream = (CNetworkMessage*) message->payload;
			peer = (CConnect*) message->source;
			peer->Send(*stream);
			stream->CloseDataStream();
		}
		return 1;
	case 0xc:
		if (message->code == 1) {
			stream = (CNetworkMessage*) message->payload;
			SendAll(*stream);
			stream->CloseDataStream();
		}
		return 1;
	default:
		return 0;
	}
}

// FUNCTION: LEMBALL 0x00462aa0
void CBaseNetwork::BeforeDestroyConnections()
{
}

// FUNCTION: LEMBALL 0x00462ab0
void CBaseNetwork::AfterDestroyConnections()
{
}

// FUNCTION: LEMBALL 0x00462ac0
void CBaseNetwork::WaitProcess()
{
}

void CBaseNetwork::Initialise()
{
}

void CBaseNetwork::UnInitialise()
{
}

void* CBaseNetwork::GetNewConnect()
{
	return 0;
}

void* CBaseNetwork::GetNewBroadcast()
{
	return 0;
}

void* CBaseNetwork::GetNewNetworkAddress()
{
	return 0;
}

// GLOBAL: LEMBALL 0x004a1e18
CBaseNetwork* g_pBaseNetwork = 0;
