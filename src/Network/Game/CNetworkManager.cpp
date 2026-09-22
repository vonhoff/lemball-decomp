#include "CNetworkManager.h"

#include "../../Visos/Foundation/CBaseQueue.h"
#include "../../Visos/Foundation/VsInit.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Network/CBroadcast.h"
#include "../../Visos/Network/CConnect.h"
#include "../../Visos/Network/CFileNetwork.h"
#include "../Messages/CGameRejectMessage.h"
#include "../Messages/CNetworkGameMessage.h"
#include "CNetworkGameStage.h"
#include "Visos/Foundation/CBaseQueueHandler.h"
#include "Visos/Network/CReadSocket.h"

// FUNCTION: LEMBALL 0x00452550
CNetworkManager::CNetworkManager(const char* p_arg0) : CBaseQueueHandler()
{
	int networkLoaded = 0;

	m_connectionsChanged = networkLoaded;
	m_externalDriverLoaded = networkLoaded;
	m_localDriverLoaded = networkLoaded;
	m_gameMessages = new CNetworkGameMessage[10];
	m_gameStage = new CNetworkGameStage;
	m_lastGameStateSendTime = CurrentMilliTimer() - 2000;
	m_observedGameState = 0;
	m_desiredGameState = 0;
	m_gameMessage = new CNetworkGameMessage;
	m_rejectMessage = new CGameRejectMessage;
	for (int i = 0; i < 10; i++) {
		m_connections[i] = 0;
	}

	if (p_arg0 != 0) {
		networkLoaded = VsFNetInit();
		if (networkLoaded != 0) {
			m_externalDriverLoaded = 1;
		}
	}
	if (p_arg0 == 0) {
		networkLoaded = VsNetInit();
		m_localDriverLoaded = 1;
	}
	if (networkLoaded != 0) {
		if (p_arg0 != 0) {
			((CFileNetwork*) g_pBaseNetwork)->Setup(p_arg0, "t:\\network");
		}
		if (g_pBaseNetwork->Initialise("Paintball v0.1", 0x400)) {
			g_pBaseNetwork->SetCBuffers(100, 0x10);
			g_pBaseNetwork->SetNcBuffers(4, 4, 0);
			m_networkInitialised = 0;
			g_pActiveConnection = 0;
			m_killRequested = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x004526e0
CNetworkManager::~CNetworkManager()
{
	if (m_externalDriverLoaded != 0) {
		VsFNetQuit();
	}
	if (m_localDriverLoaded != 0) {
		VsNetQuit();
	}
	delete m_rejectMessage;
	delete m_gameStage;
	delete m_gameMessage;
	delete[] m_gameMessages;
}

// FUNCTION: LEMBALL 0x00452740
bool CNetworkManager::Start()
{
	if (g_pBaseNetwork != 0 && g_pBaseNetwork->m_serverMode != 0) {
		CBaseNetwork* network = g_pBaseNetwork;
		network->m_activeStatusItem = this;
		network->ForceProcess();
		g_pNetworkPacketQueue->Attach(this, 0x19);
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00452780
void CNetworkManager::StartBroadcast(const char* p_address)
{
	g_pBaseNetwork->m_broadcast->StopListen();
	Broadcast(p_address);
	CBaseNetwork* network = g_pBaseNetwork;
	network->m_unk0x34 = 1;
	network->m_broadcast->StartListen();
}

// FUNCTION: LEMBALL 0x004527c0
void CNetworkManager::Stop()
{
	if (g_pActiveConnection != 0) {
		m_rejectMessage->m_flag = 1;
		m_rejectMessage->Send(g_pActiveConnection);
		unsigned int startTime = CurrentMilliTimer();
		while (m_rejectMessage->m_pendingSendCount != 0 && CurrentMilliTimer() - startTime < 1000) {
		}
		g_pActiveConnection->Kill();
		g_pActiveConnection = 0;
	}
	if (g_pBaseNetwork != 0 && g_pBaseNetwork->m_serverMode != 0) {
		g_pNetworkPacketQueue->Detach(this, 0x19);
		CBaseNetwork* network = g_pBaseNetwork;
		network->m_pendingDetachQueue = this;
		network->ForceProcess();
	}
}

#include "../../Frontend/Base/CBaseFrontendDrawer.h"
#include "../../Frontend/Processes/CNetworkOptionsProc.h"
#include "../../Visos/Foundation/CVsOStream.h"
#include "../../Visos/Foundation/Message.h"
#include "../../Visos/Messaging/BasePacketHeader.h"
#include "../../Visos/Messaging/CReadPacket.h"

extern char* g_szGameName;
extern char g_szNetworkGameName[16];

// FUNCTION: LEMBALL 0x00452850
int CNetworkManager::ProcessMsg(Message* p_message)
{
	int status = p_message->code;
	int slot;
	CConnect* request;

	switch ((unsigned int) p_message->type) {
	case 1:
		return 1;
	case 3:
		if (status == 0) {
			request = (CConnect*) p_message->payload;
			if (g_pActiveConnection != 0) {
				*g_pDebugOutput << "Game connection request during game\n";
				request->Kill();
				return 1;
			}
			slot = 0;
			CConnect** connections = m_connections;
			do {
				if (*connections == 0) {
					m_connections[slot] = request;
					g_szGameName = g_pBaseFrontendDrawer != 0 ? g_szNetworkGameName : 0;
					m_gameMessages[slot].Send(m_connections[slot]);
					m_connectionsChanged = 1;
					break;
				}
				connections++;
				slot++;
			} while (slot < 10);
			if (slot == 10) {
				request->Kill();
			}
		}
		return 1;
	case 5: {
		CConnect* connection = (CConnect*) p_message->payload;
		CReadPacket* packet = (CReadPacket*) p_message->source;
		if (status != 0) {
			return 1;
		}
		switch ((unsigned int) ((BasePacketHeader*) packet->m_data)->m_messageId) {
		case 6:
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
	case 10: {
		CConnect* connection = (CConnect*) p_message->payload;
		int index = 0;
		CConnect** connections = m_connections;
		do {
			if (*connections == connection) {
				break;
			}
			connections++;
			index++;
		} while (index < 10);
		if (index != 10) {
			m_gameMessages[index].m_valid = 0;
			m_connections[index] = 0;
			if (g_pNetworkOptionsProc != 0) {
				g_pNetworkOptionsProc->NetworkEvent((NetworkEvents) p_message->type);
			}
			if (g_pActiveConnection == connection) {
				Kill();
			}
			m_connectionsChanged = 1;
		}
		return 1;
	}
	case 13:
		if (g_pNetworkOptionsProc != 0) {
			g_pNetworkOptionsProc->NetworkEvent((NetworkEvents) p_message->type);
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
	if (p_address == 0 || *p_address == '\0') {
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
		g_pActiveConnection = 0;
		m_killRequested = 0;
	}
	if (g_pActiveConnection != 0) {
		if (g_pActiveConnection->CReadSocket::IsChanged(*m_gameStage)) {
			g_pActiveConnection->CReadSocket::GetLatest(*m_gameStage);
			if (m_observedGameState != m_gameStage->m_stage) {
				m_observedGameState = m_gameStage->m_stage;
				m_gameStage->m_stage = m_desiredGameState;
				m_gameStage->Send(g_pActiveConnection);
			}
		}
		if (m_observedGameState != m_desiredGameState && CurrentMilliTimer() - m_lastGameStateSendTime > 2000) {
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
	if (index == -1) {
		return 0;
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
		if (*connection != 0 && m_gameMessages[index].m_valid != 0) {
			count++;
		}
		connection++;
		index++;
	} while (index < 10);
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
	} while (index < 10);
	if (index == 10) {
		return -1;
	}
	return index;
}

// GLOBAL: LEMBALL 0x004a0120
CNetworkManager* g_pNetworkManager = 0;

// GLOBAL: LEMBALL 0x004a0124
char* g_szGameName = 0;
