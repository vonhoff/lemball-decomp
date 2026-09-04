#include "BaseNetwork.h"

#include "../../Network/Game/NetworkManager.h"
#include "../Foundation/BaseQueue.h"
#include "../Foundation/BaseQueueHandler.h"
#include "../Messaging/AckMessage.h"
#include "../Messaging/MessFailedConnect.h"
#include "../Messaging/MessGoConnect.h"
#include "../Messaging/MessOkConnect.h"
#include "../Messaging/MessReqConnect.h"
#include "../Messaging/MessReqNewPort.h"
#include "../Messaging/NetworkMessage.h"
#include "../Messaging/PulseMessage.h"
#include "Broadcast.h"
#include "Connect.h"
#include "NetworkAddress.h"

extern int g_lastNetworkError;
extern unsigned int g_networkPacketSize;
extern BasePacketHeader* g_pNetworkPacketScratch;
extern char* g_szBroadcastPeerName;
extern unsigned short g_broadcastPort;
extern "C" unsigned long __stdcall timeGetTime(void);

// 68K 0x1020e61c __ct__12CBaseNetworkFv
// FUNCTION: LEMBALL 0x004619f0
BaseNetwork::BaseNetwork()
{
	g_lastNetworkError = 0;
	m_lastConnect = 0;
	m_firstConnect = 0;
	m_unk0x34 = 0;
	m_unk0x30 = 0;
	m_initialized = 0;
	m_initialisePending = 0;
	m_pendingDetachQueue = 0;
	m_activeStatusItem = 0;
	m_pendingAttachQueue = 0;
	m_messageQueue = 0;
	m_queueTransitionPending = 0;
	m_shutdownRequested = 0;
	m_serverMode = 0;
	m_unk0x60 = 0x50;
	m_broadcast = 0;

	g_pNetworkStatusQueue = new BaseQueue(0x1e);
	g_pNetworkStatusQueue->Attach(this, 0x19);
	g_pNetworkPacketQueue = new BaseQueue(0x1e);
}

// 68K 0x1020e6fc Initialise__12CBaseNetworkFPCci
// FUNCTION: LEMBALL 0x00461aa0
bool BaseNetwork::Initialise(const char* p_arg0, int p_arg1)
{
	unsigned long start;
	unsigned long waitStart;

	m_initialisePending = 1;
	m_networkName = (char*) p_arg0;
	g_networkPacketSize = p_arg1;
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
	if (m_serverMode != 0) {
		do {
			if (m_serverMode != 0 && m_broadcast != 0 && m_broadcast->m_readReady != 0) {
				break;
			}
			if (g_lastNetworkError != 0 || timeGetTime() - start >= 10000) {
				break;
			}
			waitStart = timeGetTime();
			while (timeGetTime() - waitStart < 100) {
			}
			ForceProcess();
		} while (m_serverMode != 0);
	}

	if (m_serverMode != 0) {
		if (m_broadcast != 0) {
			if (m_broadcast->m_readReady != 0) {
				if (g_lastNetworkError == 0) {
					return 1;
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

// 68K 0x1020e886 DoInitialise__12CBaseNetworkFv
// FUNCTION: LEMBALL 0x00461bd0
bool BaseNetwork::DoInitialise()
{
	m_initialisePending = 0;
	Initialise();
	if (g_lastNetworkError != 0) {
		return 0;
	}

	g_pMessReqConnect = new MessReqConnect("Request Connect");
	g_pMessReqNewPort = new MessReqNewPort("Request New Port");
	g_pMessOKConnect = new MessOkConnect("Authorise Connect");
	g_pMessGOConnect = new MessGoConnect("Go Ahead Connect");
	g_pMessFAILEDConnect = new MessFailedConnect("Failed Connect");
	g_pPulseMessage = new PulseMessage;
	g_pAckMessage = new AckMessage;
	m_broadcast = (Broadcast*) GetNewBroadcast();
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

// 68K 0x1020ea84 __dt__12CBaseNetworkFv
// FUNCTION: LEMBALL 0x00461db0
BaseNetwork::~BaseNetwork()
{
	BaseQueue* queue;

	DetachMessageQueue();
	queue = *(BaseQueue* volatile*) &g_pNetworkPacketQueue;
	if (queue != 0) {
		delete queue;
	}
	g_pNetworkPacketQueue = 0;
	g_pNetworkStatusQueue->Detach(this, 0x19);
	queue = *(BaseQueue* volatile*) &g_pNetworkStatusQueue;
	if (queue != 0) {
		delete queue;
	}
	g_pNetworkStatusQueue = 0;
}

// 68K 0x1020eb50 ShutDown__12CBaseNetworkFv
// FUNCTION: LEMBALL 0x00461e10
void BaseNetwork::ShutDown()
{
	Connect* peer;
	Connect* next;
	NetworkMessage* message;
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
		message = *(NetworkMessage* volatile*) &g_pAckMessage;
		if (message != 0) {
			delete message;
		}
		g_pAckMessage = 0;
		message = *(NetworkMessage* volatile*) &g_pPulseMessage;
		if (message != 0) {
			delete message;
		}
		g_pPulseMessage = 0;
		message = *(NetworkMessage* volatile*) &g_pMessReqConnect;
		if (message != 0) {
			delete message;
		}
		g_pMessReqConnect = 0;
		message = *(NetworkMessage* volatile*) &g_pMessReqNewPort;
		if (message != 0) {
			delete message;
		}
		g_pMessReqNewPort = 0;
		message = *(NetworkMessage* volatile*) &g_pMessOKConnect;
		if (message != 0) {
			delete message;
		}
		g_pMessOKConnect = 0;
		message = *(NetworkMessage* volatile*) &g_pMessGOConnect;
		if (message != 0) {
			delete message;
		}
		g_pMessGOConnect = 0;
		message = *(NetworkMessage* volatile*) &g_pMessFAILEDConnect;
		if (message != 0) {
			delete message;
		}
		g_pMessFAILEDConnect = 0;
		UnInitialise();
	}
}

// 68K 0x1020eda0 Delete__12CBaseNetworkFP8CConnect
// FUNCTION: LEMBALL 0x00461fc0
void BaseNetwork::Delete(Connect* p_arg0)
{
	Connect* first;
	Connect* peer;
	Connect* next;
	Connect* previous;

	first = m_firstConnect;
	peer = first;
	if (peer != 0) {
		while (peer != p_arg0) {
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
		if (peer == first) {
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

// 68K 0x1020ee72 NewConnect__12CBaseNetworkFv
// FUNCTION: LEMBALL 0x00462040
Connect* BaseNetwork::NewConnect()
{
	Connect* peer;
	Connect* next;
	bool removed;

	removed = false;
	peer = m_firstConnect;
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

	peer = (Connect*) GetNewConnect();
	if (m_firstConnect == 0) {
		m_firstConnect = peer;
	}
	else {
		m_lastConnect->m_nextConnect = peer;
	}
	peer->m_previousConnect = m_lastConnect;
	m_lastConnect = peer;
	peer->SetNcBuffers(m_nonCriticalPacketCount, m_nonCriticalSequenceWindow, m_nonCriticalSubpacketCount);
	peer->SetCBuffers(m_criticalPacketCount, m_nonCriticalSubpacketCount);
	return peer;
}

// 68K 0x1020ef72 Exists__12CBaseNetworkFP8CConnect
// FUNCTION: LEMBALL 0x00462130
bool BaseNetwork::Exists(Connect* p_arg0)
{
	Connect* peer;

	peer = m_firstConnect;
	while (peer != 0) {
		if (p_arg0 == peer) {
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

// 68K 0x1020efec FindConnection__12CBaseNetworkFP15CNetworkAddress
// FUNCTION: LEMBALL 0x00462180
Connect* BaseNetwork::FindConnection(NetworkAddress* p_arg0)
{
	Connect* peer = m_firstConnect;
	while (peer != 0) {
		if (peer->m_killRequested == 0 && *peer->m_destinationAddress == *p_arg0) {
			break;
		}
		peer = peer->m_nextConnect;
	}
	return peer;
}

// 68K 0x1020f06e KillUnBornConnection__12CBaseNetworkFP15CNetworkAddress
// FUNCTION: LEMBALL 0x004621c0
void BaseNetwork::KillUnBornConnection(NetworkAddress* p_arg0)
{
	Connect* peer = FindConnection(p_arg0);
	if (peer != 0) {
		peer->Kill();
	}
}

// 68K 0x1020f0d2 CtoSRequestConnect__12CBaseNetworkFP15CNetworkAddress
// FUNCTION: LEMBALL 0x004621e0
void BaseNetwork::CtoSRequestConnect(NetworkAddress* p_arg0)
{
	Connect* peer;
	short port;

	peer = FindConnection(p_arg0);
	if (peer != 0 && peer->m_killRequested == 0 && peer->CheckConnectTime() != 0) {
		return;
	}

	peer = NewConnect();
	port = m_broadcast->FindPort(g_pMessReqConnect->m_connectionData);
	if (port == -1) {
		peer->Kill();
		return;
	}

	peer->InitConnect(g_pMessReqConnect->m_peerName, p_arg0, port);
	g_pMessOKConnect->m_assignedPort = peer->m_port;
	g_pMessOKConnect->m_connectionId = (unsigned int) peer;
	m_broadcast->Send(p_arg0, *g_pMessOKConnect);
}

// 68K 0x1020f1cc CtoSRequestNewPort__12CBaseNetworkFP15CNetworkAddress
// FUNCTION: LEMBALL 0x00462280
void BaseNetwork::CtoSRequestNewPort(NetworkAddress* p_arg0)
{
	Connect* peer;
	short port;

	peer = (Connect*) g_pMessReqNewPort->m_connectionId;
	if (Exists(peer) != 0) {
		m_broadcast->ResetPort(peer->m_port);
		peer->m_newPortRequestCount++;
		if (peer->m_newPortRequestCount > 5) {
			g_pMessFAILEDConnect->m_failureReason = "To many new-port requests";
			m_broadcast->Send(p_arg0, *g_pMessReqNewPort);
			return;
		}

		port = m_broadcast->FindPort(g_pMessReqNewPort->m_connectionData);
		if (port != -1) {
			peer->SetPort(port);
			g_pMessOKConnect->m_assignedPort = peer->m_port;
			g_pMessOKConnect->m_connectionId = (unsigned int) peer;
			m_broadcast->Send(p_arg0, *g_pMessOKConnect);
		}
	}
}

// 68K 0x1020f2fc StoCOKConnect__12CBaseNetworkFP15CNetworkAddress
// FUNCTION: LEMBALL 0x00462340
void BaseNetwork::StoCokConnect(NetworkAddress* p_arg0)
{
	Connect* peer;
	unsigned short port;

	peer = FindConnection(p_arg0);
	if (peer != 0 && peer->m_killRequested == 0 && peer->CheckConnectTime() != 0) {
		if (*g_pBroadcastAddress > *p_arg0) {
			return;
		}
		peer->Kill();
	}

	if (m_unk0x30 != 0) {
		m_broadcast->Suspend();
	}

	port = g_pMessOKConnect->m_assignedPort;
	if (m_broadcast->m_connectionData[(short) port] == 0) {
		m_broadcast->m_connectionData[(short) port] = 1;
		peer = NewConnect();
		peer->SetPort(port);
		peer->Listen(p_arg0);
		g_pMessGOConnect->m_assignedPort = port;
		g_pMessGOConnect->m_connectionId = g_pMessOKConnect->m_connectionId;
		m_broadcast->Send(p_arg0, *g_pMessGOConnect);
		return;
	}

	g_pMessReqNewPort->m_connectionId = g_pMessOKConnect->m_connectionId;
	g_pMessReqNewPort->m_requestedPort = g_broadcastPort;
	g_pMessReqNewPort->m_connectionData = m_broadcast->m_connectionData;
	g_pMessReqNewPort->m_peerName = g_szBroadcastPeerName;
	m_broadcast->Send(p_arg0, *g_pMessReqNewPort);
}

// 68K 0x1020f47c StoCFAILEDConnect__12CBaseNetworkFP15CNetworkAddress
// FUNCTION: LEMBALL 0x00462460
void BaseNetwork::StoCfailedConnect(NetworkAddress* p_arg0)
{
	m_broadcast->Send(p_arg0, *g_pMessFAILEDConnect);
}

// 68K 0x1020f4d2 CtoSGOConnect__12CBaseNetworkFP15CNetworkAddress
// FUNCTION: LEMBALL 0x00462480
void BaseNetwork::CtoSgoConnect(NetworkAddress* p_arg0)
{
	Connect* peer = (Connect*) g_pMessGOConnect->m_connectionId;
	if (Exists(peer) != 0) {
		peer->Connect();
	}
}

// 68K 0x1020f53c Establish__12CBaseNetworkFP15CNetworkAddressPUc
// FUNCTION: LEMBALL 0x004624a0
void BaseNetwork::Establish(NetworkAddress* p_arg0, unsigned char* p_arg1)
{
	(*(void(__fastcall**)(NetworkAddress*)) * (void***) p_arg0)(p_arg0);
	if (g_pMessReqConnect->Set(p_arg1) != 0) {
		CtoSRequestConnect(p_arg0);
		return;
	}
	if (g_pMessReqNewPort->Set(p_arg1) != 0) {
		CtoSRequestNewPort(p_arg0);
		return;
	}
	if (g_pMessOKConnect->Set(p_arg1) != 0) {
		StoCokConnect(p_arg0);
		return;
	}
	if (g_pMessGOConnect->Set(p_arg1) != 0) {
		CtoSgoConnect(p_arg0);
		return;
	}
	if (g_pMessFAILEDConnect->Set(p_arg1) != 0) {
		StoCfailedConnect(p_arg0);
	}
}

// 68K 0x1020f632 SetNCBuffers__12CBaseNetworkFUlUli
// FUNCTION: LEMBALL 0x00462550
void BaseNetwork::SetNcBuffers(unsigned long p_arg0, unsigned long p_arg1, int p_arg2)
{
	m_nonCriticalPacketCount = p_arg0;
	m_nonCriticalSequenceWindow = p_arg1;
	m_nonCriticalSubpacketCount = p_arg2;
}

// 68K 0x1020f676 SetCBuffers__12CBaseNetworkFii
// FUNCTION: LEMBALL 0x00462570
void BaseNetwork::SetCBuffers(int p_arg0, int p_arg1)
{
	m_criticalPacketCount = p_arg0;
	m_criticalSubpacketCount = p_arg1;
}

// 68K 0x1020f6b0 AttachMessageQueue__12CBaseNetworkFP17CBaseQueueHandler
// FUNCTION: LEMBALL 0x00462590
void BaseNetwork::AttachMessageQueue(BaseQueueHandler* p_arg0)
{
	m_messageQueue = p_arg0;
	g_pNetworkPacketQueue->Attach(p_arg0, 0);
}

// 68K 0x1020f710 DetachMessageQueue__12CBaseNetworkFv
// FUNCTION: LEMBALL 0x004625b0
void BaseNetwork::DetachMessageQueue()
{
	if (m_messageQueue != 0) {
		g_pNetworkPacketQueue->Detach(m_messageQueue, 0);
		m_messageQueue = 0;
	}
}

// 68K 0x1020f768 Process__12CBaseNetworkFv
// FUNCTION: LEMBALL 0x004625e0
void BaseNetwork::Process()
{
	if (m_activeStatusItem != 0) {
		g_pNetworkStatusQueue->Attach((BaseQueueHandler*) m_activeStatusItem, 0);
		m_queueTransitionPending = 1;
		m_pendingAttachQueue = (BaseQueueHandler*) m_activeStatusItem;
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

	Connect* peer = m_firstConnect;
	while (peer != 0) {
		peer->Process();
		peer = peer->m_nextConnect;
	}

	if (m_pendingAttachQueue != 0) {
		((NetworkManager*) m_pendingAttachQueue)->Process();
	}
}

// 68K 0x1020f866 SendAll__12CBaseNetworkFR15CNetworkMessage
// FUNCTION: LEMBALL 0x00462720
bool BaseNetwork::SendAll(NetworkMessage& p_arg0)
{
	Connect* peer;
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
				if (peer->Send(p_arg0) != 0) {
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

// 68K 0x1020f8f0 ProcessMsg__12CBaseNetworkFP10tagMESSAGE
// FUNCTION: LEMBALL 0x004627b0
int BaseNetwork::ProcessMsg(Message* p_arg0)
{
	unsigned int type;
	Message* message;
	NetworkMessage* stream;
	Connect* peer;

	type = 0;
	message = p_arg0;
	type = message->type;
	switch (type) {
	case 0xb:
		if (message->code == 1) {
			stream = (NetworkMessage*) message->payload;
			peer = (Connect*) message->source;
			peer->Send(*stream);
			stream->CloseDataStream();
		}
		return 1;
	case 0xc:
		if (message->code == 1) {
			stream = (NetworkMessage*) message->payload;
			SendAll(*stream);
			stream->CloseDataStream();
		}
		return 1;
	default:
		return 0;
	}
}

// 68K 0x1010f03e BeforeDestroyConnections__12CBaseNetworkFv
// FUNCTION: LEMBALL 0x00462aa0
void BaseNetwork::BeforeDestroyConnections()
{
}

// 68K 0x1010f074 AfterDestroyConnections__12CBaseNetworkFv
// FUNCTION: LEMBALL 0x00462ab0
void BaseNetwork::AfterDestroyConnections()
{
}

// 68K 0x10107a8e WaitProcess__12CBaseNetworkFv
// FUNCTION: LEMBALL 0x00462ac0
void BaseNetwork::WaitProcess()
{
}

void BaseNetwork::Initialise()
{
}

void BaseNetwork::UnInitialise()
{
}

void* BaseNetwork::GetNewConnect()
{
	return 0;
}

void* BaseNetwork::GetNewBroadcast()
{
	return 0;
}

void* BaseNetwork::GetNewNetworkAddress()
{
	return 0;
}

// GLOBAL: LEMBALL 0x004a1e18
BaseNetwork* g_pBaseNetwork = 0;
