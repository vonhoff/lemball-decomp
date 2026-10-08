#include "CNetworkManager.h"

#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Queues/CBaseQueueHandler.h"
#include "Engine/Startup/VsInit.h"
#include "Engine/Time/VsTime.h"
#include "Multiplayer/CGameFlaggedMessage.h"
#include "Multiplayer/CGameRejectMessage.h"
#include "Multiplayer/CNetworkGameMessage.h"
#include "Multiplayer/CNetworkGameStage.h"
#include "Multiplayer/Transport/CBaseCommonSocket.h"
#include "Multiplayer/Transport/CBaseNetwork.h"
#include "Multiplayer/Transport/CBroadcast.h"
#include "Multiplayer/Transport/CConnect.h"
#include "Multiplayer/Transport/CReadSocket.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Multiplayer/Transport/Protocol/CNetworkMessage.h"
#include "Platform/Windows/Network/FileTransport/CFileNetwork.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00452550
CNetworkManager::CNetworkManager(const char* p_filePeerName) : CBaseQueueHandler()
{
	int networkLoaded = 0;

	m_connectionsChanged = networkLoaded;
	m_externalDriverLoaded = networkLoaded;
	m_localDriverLoaded = networkLoaded;
	m_gameMessages = new CNetworkGameMessage[NETWORK_GAME_SLOT_COUNT];
	m_gameStage = new CNetworkGameStage;
	m_lastGameStateSendTime = CurrentMilliTimer() - NETWORK_GAME_STATE_RESEND_INTERVAL_MS;
	m_observedGameState = 0;
	m_desiredGameState = 0;
	m_gameMessage = new CNetworkGameMessage;
	m_rejectMessage = new CGameRejectMessage;
	for (int i = 0; i < NETWORK_GAME_SLOT_COUNT; i++) {
		m_connections[i] = NULL;
	}

	if (p_filePeerName != NULL) {
		networkLoaded = VSFNET_Init();
		if (networkLoaded != 0) {
			m_externalDriverLoaded = 1;
		}
	}
	if (p_filePeerName == NULL) {
		networkLoaded = VSNET_Init();
		m_localDriverLoaded = 1;
	}
	if (networkLoaded != 0) {
		if (p_filePeerName != NULL) {
			((CFileNetwork*) g_pBaseNetwork)->Setup(p_filePeerName, "t:\\network");
		}
		if (g_pBaseNetwork->Initialise("Paintball v0.1", NETWORK_PACKET_SIZE_BYTES)) {
			g_pBaseNetwork->SetCBuffers(NETWORK_CRITICAL_PACKET_COUNT, NETWORK_CRITICAL_MESSAGE_CAPACITY_BYTES);
			g_pBaseNetwork->SetNCBuffers(NETWORK_MESSAGE_GAME_STAGE,
										 NETWORK_MESSAGE_GAME_STAGE,
										 NETWORK_NONCRITICAL_MESSAGE_CAPACITY_BYTES);
			m_networkInitialised = 0;
			g_pActiveConnection = NULL;
			m_killRequested = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x004526e0
CNetworkManager::~CNetworkManager()
{
	if (m_externalDriverLoaded != 0) {
		VSFNET_Quit();
	}
	if (m_localDriverLoaded != 0) {
		VSNET_Quit();
	}
	delete m_rejectMessage;
	delete m_gameStage;
	delete m_gameMessage;
	delete[] m_gameMessages;
}

// FUNCTION: LEMBALL 0x00452740
bool CNetworkManager::Start()
{
	if (g_pBaseNetwork != NULL && g_pBaseNetwork->m_serverMode != 0) {
		CBaseNetwork* network = g_pBaseNetwork;
		network->m_activeStatusItem = this;
		network->ForceProcess();
		g_pNetworkPacketQueue->Attach(this, NETWORK_QUEUE_PRIORITY);
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00452780
void CNetworkManager::StartBroadcast(const char* p_address)
{
	g_pBaseNetwork->m_broadcast->StopListen();
	Broadcast(p_address);
	CBaseNetwork* network = g_pBaseNetwork;
	network->m_broadcastMode = 1;
	network->m_broadcast->StartListen();
}

// FUNCTION: LEMBALL 0x004527c0
void CNetworkManager::Stop()
{
	if (g_pActiveConnection != NULL) {
		m_rejectMessage->m_flag = 1;
		m_rejectMessage->Send(g_pActiveConnection);
		unsigned int startTime = CurrentMilliTimer();
		while (m_rejectMessage->m_pendingSendCount != 0 &&
			   CurrentMilliTimer() - startTime < NETWORK_MESSAGE_SEND_WAIT_TIMEOUT_MS) {
		}
		g_pActiveConnection->Kill();
		g_pActiveConnection = NULL;
	}
	if (g_pBaseNetwork != NULL && g_pBaseNetwork->m_serverMode != 0) {
		g_pNetworkPacketQueue->Detach(this, NETWORK_QUEUE_PRIORITY);
		CBaseNetwork* network = g_pBaseNetwork;
		network->m_pendingDetachQueue = this;
		network->ForceProcess();
	}
}

#include "Engine/Queues/Message.h"
#include "Engine/Streams/CVSOStream.h"
#include "Frontend/CBaseFrontendDrawer.h"
#include "Frontend/Network/CNetworkOptionsProc.h"
#include "Multiplayer/Transport/Packets/BasePacketHeader.h"
#include "Multiplayer/Transport/Packets/CReadPacket.h"

extern char* g_szGameName;
extern char g_szNetworkGameName[16];

// FUNCTION: LEMBALL 0x00452850
int CNetworkManager::ProcessMsg(Message* p_message)
{
	unsigned int messageType;
	int status = p_message->m_code;
	int slot;
	CConnect* request;
	messageType = p_message->m_type;

	switch (messageType) {
	case CONNECT_QUEUE_SEND_FAILED:
		return 1;
	case CONNECT_QUEUE_FIRST_RECEIVE:
		if (status == NETWORK_ERROR_NONE) {
			request = (CConnect*) p_message->m_payload;
			if (g_pActiveConnection != NULL) {
				*g_pDebugOutput << "Game connection request during game\n";
				request->Kill();
				return 1;
			}
			slot = 0;
			CConnect** connections = m_connections;
			do {
				if (*connections == NULL) {
					m_connections[slot] = request;
					g_szGameName = g_pBaseFrontendDrawer != NULL ? g_szNetworkGameName : NULL;
					m_gameMessages[slot].Send(m_connections[slot]);
					m_connectionsChanged = 1;
					break;
				}
				connections++;
				slot++;
			} while (slot < NETWORK_GAME_SLOT_COUNT);
			if (slot == NETWORK_GAME_SLOT_COUNT) {
				request->Kill();
			}
		}
		return 1;
	case NETWORK_EVENT_CRITICAL_PACKET_READY: {
		CConnect* connection = (CConnect*) p_message->m_payload;
		CReadPacket* packet = (CReadPacket*) p_message->m_source;
		if (status != NETWORK_ERROR_NONE) {
			return 1;
		}
		switch ((unsigned int) ((BasePacketHeader*) packet->m_data)->m_messageId) {
		case GAME_MESSAGE_REJECT:
			m_rejectMessage->Set(packet->m_data + sizeof(BasePacketHeader));
			packet->m_used = 0;
			if (m_rejectMessage->m_flag != 0) {
				connection->Kill();
			}
			break;
		default:
			packet->m_used = 0;
			break;
		}
		return 1;
	}
	case CONNECT_QUEUE_CLOSED: {
		CConnect* connection = (CConnect*) p_message->m_payload;
		int index = 0;
		CConnect** connections = m_connections;
		do {
			if (*connections == connection) {
				break;
			}
			connections++;
			index++;
		} while (index < NETWORK_GAME_SLOT_COUNT);
		if (index != NETWORK_GAME_SLOT_COUNT) {
			m_gameMessages[index].m_valid = 0;
			m_connections[index] = NULL;
			if (g_pNetworkOptionsProc != NULL) {
				g_pNetworkOptionsProc->NetworkEvent((NetworkEvents) p_message->m_type);
			}
			if (g_pActiveConnection == connection) {
				Kill();
			}
			m_connectionsChanged = 1;
		}
		return 1;
	}
	case NETWORK_EVENT_HOST_LOOKUP_FAILED:
		if (g_pNetworkOptionsProc != NULL) {
			g_pNetworkOptionsProc->NetworkEvent((NetworkEvents) messageType);
		}
		return 1;
	default:
		return 0;
	}
}

// FUNCTION: LEMBALL 0x00452a40
void CNetworkManager::Broadcast(const char* p_address)
{
	m_broadcastStartTime = CurrentMilliTimer();
	class CBroadcast* broadcast = g_pBaseNetwork->m_broadcast;
	if (broadcast->m_runEnabled != 0) {
		broadcast->Suspend();
	}
	if (p_address == NULL || *p_address == '\0') {
		broadcast->m_addressMode = 0;
	}
	else {
		broadcast->SetSpecificAddr(p_address);
	}
	class CBroadcast** broadcastPtr = &g_pBaseNetwork->m_broadcast;
	if ((*broadcastPtr)->m_runEnabled == 0) {
		g_pBaseNetwork->m_suspendBroadcastOnConnect = 0;
		(*broadcastPtr)->Run();
	}
}

// FUNCTION: LEMBALL 0x00452ab0
void CNetworkManager::Kill()
{
	m_killRequested = 1;
}

// FUNCTION: LEMBALL 0x00452ac0
void CNetworkManager::GameProcess()
{
	if (m_killRequested != 0) {
		g_pActiveConnection = NULL;
		m_killRequested = 0;
	}
	if (g_pActiveConnection != NULL) {
		if (g_pActiveConnection->CReadSocket::IsChanged(*m_gameStage)) {
			g_pActiveConnection->CReadSocket::GetLatest(*m_gameStage);
			if (m_observedGameState != m_gameStage->m_stage) {
				m_observedGameState = m_gameStage->m_stage;
				m_gameStage->m_stage = m_desiredGameState;
				m_gameStage->Send(g_pActiveConnection);
			}
		}
		if (m_observedGameState != m_desiredGameState &&
			CurrentMilliTimer() - m_lastGameStateSendTime > NETWORK_GAME_STATE_RESEND_INTERVAL_MS) {
			m_gameStage->m_stage = m_desiredGameState;
			m_gameStage->Send(g_pActiveConnection);
			m_lastGameStateSendTime = CurrentMilliTimer();
		}
	}
}

// FUNCTION: LEMBALL 0x00452b80
void CNetworkManager::Process()
{
}

// FUNCTION: LEMBALL 0x00452b90
CNetworkGameMessage* CNetworkManager::GetGameMessage(CConnect* p_connection)
{
	int index = GetnGame(p_connection);
	if (index == NETWORK_GAME_INDEX_NOT_FOUND) {
		return NULL;
	}
	return m_gameMessages + index;
}

// FUNCTION: LEMBALL 0x00452bc0
int CNetworkManager::CountActiveGames()
{
	int count = 0;
	int index = 0;
	CConnect** connection = m_connections;
	do {
		if (*connection != NULL && m_gameMessages[index].m_valid != 0) {
			count++;
		}
		connection++;
		index++;
	} while (index < NETWORK_GAME_SLOT_COUNT);
	return count;
}

// FUNCTION: LEMBALL 0x00452bf0
int CNetworkManager::GetnGame(CConnect* p_connection)
{
	int index;
	CConnect** connection;

	index = 0;
	connection = m_connections;
	do {
		if (*connection == p_connection) {
			break;
		}
		connection++;
		index++;
	} while (index < NETWORK_GAME_SLOT_COUNT);
	if (index == NETWORK_GAME_SLOT_COUNT) {
		return NETWORK_GAME_INDEX_NOT_FOUND;
	}
	return index;
}

// GLOBAL: LEMBALL 0x004a0120
CNetworkManager* g_pNetworkManager = NULL;

// GLOBAL: LEMBALL 0x004a0124
char* g_szGameName = NULL;
