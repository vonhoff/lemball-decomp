#include "CBaseNetwork.h"

#include "Multiplayer/CNetworkManager.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Queues/CBaseQueueHandler.h"
#include "Engine/Network/Protocol/CAckMessage.h"
#include "Engine/Network/Protocol/CMessFAILEDConnect.h"
#include "Engine/Network/Protocol/CMessGOConnect.h"
#include "Engine/Network/Protocol/CMessOKConnect.h"
#include "Engine/Network/Protocol/CMessReqConnect.h"
#include "Engine/Network/Protocol/CMessReqNewPort.h"
#include "Engine/Network/Protocol/CNetworkMessage.h"
#include "Engine/Network/Protocol/CPulseMessage.h"
#include "CBroadcast.h"
#include "CConnect.h"
#include "CNetworkAddress.h"
#include "NetworkConstants.h"
#include "Engine/Queues/Message.h"

struct BasePacketHeader;

extern int g_lastNetworkError;
extern unsigned int g_networkPacketSize;
extern BasePacketHeader* g_pNetworkPacketScratch;
extern char* g_szBroadcastPeerName;
extern unsigned short g_broadcastPort;
extern "C" unsigned long __stdcall timeGetTime(void);

enum {
	NETWORK_QUEUE_MESSAGE_CAPACITY = 0x1e,
	CRITICAL_PACKET_RETRY_LIMIT = 0x50,
	NETWORK_NEW_PORT_REQUEST_LIMIT = 5
};

// FUNCTION: LEMBALL 0x004619f0
CBaseNetwork::CBaseNetwork()
{
	g_lastNetworkError = 0;
	m_lastConnect = NULL;
	m_firstConnect = NULL;
	m_broadcastMode = 0;
	m_suspendBroadcastOnConnect = 0;
	m_initialised = 0;
	m_initialisePending = 0;
	m_pendingDetachQueue = NULL;
	m_activeStatusItem = NULL;
	m_pendingAttachQueue = NULL;
	m_messageQueue = NULL;
	m_queueTransitionPending = 0;
	m_shutdownRequested = 0;
	m_serverMode = 0;
	m_criticalRetryLimit = CRITICAL_PACKET_RETRY_LIMIT;
	m_broadcast = NULL;

	g_pNetworkStatusQueue = new CBaseQueue(NETWORK_QUEUE_MESSAGE_CAPACITY);
	g_pNetworkStatusQueue->Attach(this, NETWORK_QUEUE_PRIORITY);
	g_pNetworkPacketQueue = new CBaseQueue(NETWORK_QUEUE_MESSAGE_CAPACITY);
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
	if (m_initialised == 0) {
		do {
			if (g_lastNetworkError != 0 || timeGetTime() - start >= NETWORK_LIFECYCLE_TIMEOUT_MS) {
				break;
			}
			WaitProcess();
		} while (m_initialised == 0);
		if (m_initialised == 0) {
			return false;
		}
	}

	start = timeGetTime();
	while (m_serverMode != 0 && !(m_serverMode != 0 && m_broadcast != NULL && m_broadcast->m_readReady != 0) &&
		   g_lastNetworkError == 0 && timeGetTime() - start < NETWORK_LIFECYCLE_TIMEOUT_MS) {
		waitStart = timeGetTime();
		while (timeGetTime() - waitStart < NETWORK_STARTUP_POLL_INTERVAL_MS) {
		}
		ForceProcess();
	}

	if (m_serverMode != 0) {
		if (m_serverMode != 0) {
			if (m_broadcast != NULL) {
				if (m_broadcast->m_readReady != 0) {
					if (g_lastNetworkError == 0) {
						return true;
					}
				}
			}
		}
	}

	m_shutdownRequested = 1;
	ForceProcess();
	start = timeGetTime();
	while (m_initialised != 0 && timeGetTime() - start < NETWORK_LIFECYCLE_TIMEOUT_MS) {
	}
	return false;
}

// FUNCTION: LEMBALL 0x00461bd0
bool CBaseNetwork::DoInitialise()
{
	m_initialisePending = 0;
	Initialise();
	if (g_lastNetworkError != 0) {
		return false;
	}

	g_pMessReqConnect = new CMessReqConnect("Request Connect");
	g_pMessReqNewPort = new CMessReqNewPort("Request New Port");
	g_pMessOKConnect = new CMessOKConnect("Authorise Connect");
	g_pMessGOConnect = new CMessGOConnect("Go Ahead Connect");
	g_pMessFAILEDConnect = new CMessFAILEDConnect("Failed Connect");
	g_pPulseMessage = new CPulseMessage;
	g_pAckMessage = new CAckMessage;
	m_broadcast = (CBroadcast*) GetNewBroadcast();
	m_initialised = 1;
	m_serverMode = 1;
	ForceProcess();
	if (m_serverMode != 0 && g_lastNetworkError == 0) {
		if (m_broadcast->Start(m_networkName) == 0) {
			m_serverMode = 0;
			return false;
		}
		return g_lastNetworkError == 0;
	}
	m_serverMode = 0;
	return false;
}

// FUNCTION: LEMBALL 0x00461db0
CBaseNetwork::~CBaseNetwork()
{
	CBaseQueue* queue;

	DetachMessageQueue();
	queue = *(CBaseQueue* volatile*) &g_pNetworkPacketQueue;
	if (queue != NULL) {
		delete queue;
	}
	g_pNetworkPacketQueue = NULL;
	g_pNetworkStatusQueue->Detach(this, NETWORK_QUEUE_PRIORITY);
	queue = *(CBaseQueue* volatile*) &g_pNetworkStatusQueue;
	if (queue != NULL) {
		delete queue;
	}
	g_pNetworkStatusQueue = NULL;
}

// FUNCTION: LEMBALL 0x00461e10
void CBaseNetwork::ShutDown()
{
	CConnect* peer;
	CConnect* next;
	CNetworkMessage* message;
	short port;

	if (m_initialised != 0) {
		m_initialised = 0;
		m_serverMode = 0;
		peer = m_firstConnect;
		if (peer != NULL) {
			BeforeDestroyConnections();
			while (1) {
				if (peer == NULL) {
					break;
				}
				next = peer->m_nextConnect;
				port = peer->m_port;
				peer->Stop();
				delete peer;
				if (port != NETWORK_PORT_UNASSIGNED) {
					m_broadcast->ResetPort(port);
				}
				peer = next;
			}
			AfterDestroyConnections();
		}

		if (m_broadcast != NULL) {
			m_broadcast->Stop();
			delete m_broadcast;
		}
		if (g_pNetworkPacketScratch != NULL) {
			operator delete(g_pNetworkPacketScratch);
		}
		g_pNetworkPacketScratch = NULL;
		if (g_pBroadcastReceiveAddress != NULL) {
			delete g_pBroadcastReceiveAddress;
		}
		g_pBroadcastReceiveAddress = NULL;
		message = *(CNetworkMessage* volatile*) &g_pAckMessage;
		if (message != NULL) {
			delete message;
		}
		g_pAckMessage = NULL;
		message = *(CNetworkMessage* volatile*) &g_pPulseMessage;
		if (message != NULL) {
			delete message;
		}
		g_pPulseMessage = NULL;
		message = *(CNetworkMessage* volatile*) &g_pMessReqConnect;
		if (message != NULL) {
			delete message;
		}
		g_pMessReqConnect = NULL;
		message = *(CNetworkMessage* volatile*) &g_pMessReqNewPort;
		if (message != NULL) {
			delete message;
		}
		g_pMessReqNewPort = NULL;
		message = *(CNetworkMessage* volatile*) &g_pMessOKConnect;
		if (message != NULL) {
			delete message;
		}
		g_pMessOKConnect = NULL;
		message = *(CNetworkMessage* volatile*) &g_pMessGOConnect;
		if (message != NULL) {
			delete message;
		}
		g_pMessGOConnect = NULL;
		message = *(CNetworkMessage* volatile*) &g_pMessFAILEDConnect;
		if (message != NULL) {
			delete message;
		}
		g_pMessFAILEDConnect = NULL;
		UnInitialise();
	}
}

// FUNCTION: LEMBALL 0x00461fc0
void CBaseNetwork::Delete(CConnect* p_connection)
{
	CConnect* peer = m_firstConnect;
	CConnect* next;
	CConnect* previous;
	if (peer != NULL) {
		while (peer != p_connection) {
			peer = peer->m_nextConnect;
			if (peer == NULL) {
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
		if (previous != NULL) {
			previous->m_nextConnect = next;
		}
		if (next != NULL) {
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
		if (peer == NULL) {
			break;
		}
		next = peer->m_nextConnect;
		if (peer->m_killRequested != 0) {
			if (!removed) {
				BeforeDestroyConnections();
				removed = true;
			}
			Delete(peer);
		}
		peer = next;
	}
	if (removed) {
		AfterDestroyConnections();
	}

	peer = (CConnect*) GetNewConnect();
	if (m_firstConnect == NULL) {
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
	while (peer != NULL) {
		if (p_connection == peer) {
			if (peer->CheckConnectTime() == 0) {
				return false;
			}
			if (peer->m_killRequested == 0) {
				peer->SetConnectTime();
				return true;
			}
			return false;
		}
		peer = peer->m_nextConnect;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00462180
CConnect* CBaseNetwork::FindConnection(CNetworkAddress* p_address)
{
	CConnect* peer = m_firstConnect;
	while (peer != NULL) {
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
	if (peer != NULL) {
		peer->Kill();
	}
}

// FUNCTION: LEMBALL 0x004621e0
void CBaseNetwork::CtoSRequestConnect(CNetworkAddress* p_address)
{
	CConnect* peer;
	short port;

	peer = FindConnection(p_address);
	if (peer != NULL && peer->m_killRequested == 0 && peer->CheckConnectTime() != 0) {
		return;
	}

	peer = NewConnect();
	port = m_broadcast->FindPort(g_pMessReqConnect->m_connectionData);
	if (port == NETWORK_PORT_NOT_FOUND) {
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
		if (peer->m_newPortRequestCount > NETWORK_NEW_PORT_REQUEST_LIMIT) {
			g_pMessFAILEDConnect->m_failureReason = "To many new-port requests";
			m_broadcast->Send(p_address, *g_pMessReqNewPort);
			return;
		}

		port = m_broadcast->FindPort(g_pMessReqNewPort->m_connectionData);
		if (port != NETWORK_PORT_NOT_FOUND) {
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
	if (peer != NULL && peer->m_killRequested == 0 && peer->CheckConnectTime() != 0) {
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
	if (m_messageQueue != NULL) {
		g_pNetworkPacketQueue->Detach(m_messageQueue, 0);
		m_messageQueue = NULL;
	}
}

// FUNCTION: LEMBALL 0x004625e0
void CBaseNetwork::Process()
{
	if (m_activeStatusItem != NULL) {
		g_pNetworkStatusQueue->Attach((CBaseQueueHandler*) m_activeStatusItem, 0);
		m_queueTransitionPending = 1;
		m_pendingAttachQueue = (CBaseQueueHandler*) m_activeStatusItem;
		m_activeStatusItem = NULL;
	}

	if (m_pendingDetachQueue != NULL) {
		g_pNetworkStatusQueue->Detach(m_pendingDetachQueue, 0);
		m_pendingDetachQueue = NULL;
		m_pendingAttachQueue = NULL;
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

	if (m_broadcast != NULL) {
		m_broadcast->Process();
	}

	CConnect* peer = m_firstConnect;
	while (peer != NULL) {
		peer->Process();
		peer = peer->m_nextConnect;
	}

	if (m_pendingAttachQueue != NULL) {
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
	while (peer != NULL) {
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
	if (peer != NULL) {
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
		if (peer == NULL) {
			if (sendBlocked == 0 && activeCount > 0) {
				return true;
			}
			return false;
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
	type = message->m_type;
	switch (type) {
	case NETWORK_QUEUE_SEND_ONE:
		if (message->m_code == NETWORK_QUEUE_SEND_REQUESTED) {
			stream = (CNetworkMessage*) message->m_payload;
			peer = (CConnect*) message->m_source;
			peer->Send(*stream);
			stream->CloseDataStream();
		}
		return 1;
	case NETWORK_QUEUE_SEND_ALL:
		if (message->m_code == NETWORK_QUEUE_SEND_REQUESTED) {
			stream = (CNetworkMessage*) message->m_payload;
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
	return NULL;
}

void* CBaseNetwork::GetNewBroadcast()
{
	return NULL;
}

void* CBaseNetwork::GetNewNetworkAddress()
{
	return NULL;
}

// GLOBAL: LEMBALL 0x004a1e18
CBaseNetwork* g_pBaseNetwork = NULL;
