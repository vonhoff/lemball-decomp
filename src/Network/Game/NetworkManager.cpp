#include "NetworkManager.h"

#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Foundation/VsInit.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Network/BaseNetwork.h"
#include "../../Visos/Network/Broadcast.h"
#include "../../Visos/Network/Connect.h"
#include "../../Visos/Network/FileNetwork.h"
#include "../Messages/GameRejectMessage.h"
#include "../Messages/NetworkGameMessage.h"
#include "NetworkGameStage.h"

#include <new.h>

// 68K 0x10a00346 __ct__15CNetworkManagerFPCc
// FUNCTION: LEMBALL 0x00452550
NetworkManager::NetworkManager(const char* p_arg0) : BaseQueueHandler()
{
	int networkLoaded = 0;

	m_connectionsChanged = networkLoaded;
	m_externalDriverLoaded = networkLoaded;
	m_localDriverLoaded = networkLoaded;
	m_gameMessages = new NetworkGameMessage[10];
	m_gameStage = new NetworkGameStage;
	m_lastGameStateSendTime = CurrentMilliTimer() - 2000;
	m_observedGameState = 0;
	m_desiredGameState = 0;
	m_gameMessage = new NetworkGameMessage;
	m_rejectMessage = new GameRejectMessage;
	for (int i = 0; i < 10; i++) {
		m_connections[i] = 0;
	}

	if (p_arg0 != 0) {
		networkLoaded = VsFNetInit();
		if (networkLoaded != 0) {
			m_externalDriverLoaded = 1;
		}
	}
	else {
		networkLoaded = VsNetInit();
		m_localDriverLoaded = 1;
	}
	if (networkLoaded != 0) {
		if (p_arg0 != 0) {
			((FileNetwork*) g_pBaseNetwork)->Setup(p_arg0, "t:\\network");
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

// 68K 0x10a005e8 Start__15CNetworkManagerFv
// FUNCTION: LEMBALL 0x00452740
bool NetworkManager::Start()
{
	if (g_pBaseNetwork != 0 && g_pBaseNetwork->m_serverMode != 0) {
		g_pBaseNetwork->m_activeStatusItem = this;
		g_pBaseNetwork->ForceProcess();
		g_pNetworkPacketQueue->Attach(this, 0x19);
		return 1;
	}
	return 0;
}

// 68K 0x10a0065e StartBroadcast__15CNetworkManagerFPCc
// FUNCTION: LEMBALL 0x00452780
void NetworkManager::StartBroadcast(const char* p_address)
{
	g_pBaseNetwork->m_broadcast->StopListen();
	Broadcast(p_address);
	BaseNetwork* network = g_pBaseNetwork;
	network->m_unk0x34 = 1;
	network->m_broadcast->StartListen();
}

// 68K 0x10a006d8 Stop__15CNetworkManagerFv
// FUNCTION: LEMBALL 0x004527c0
void NetworkManager::Stop()
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
		BaseNetwork* network = g_pBaseNetwork;
		network->m_pendingDetachQueue = this;
		network->ForceProcess();
	}
}

// 68K 0x10a007a6 ProcessMsg__15CNetworkManagerFP10tagMESSAGE
// STUB: LEMBALL 0x00452850
int NetworkManager::ProcessMsg(Message* p_message)
{
	return 0;
}

// 68K 0x10a00996 Broadcast__15CNetworkManagerFPCc
// FUNCTION: LEMBALL 0x00452a40
void NetworkManager::Broadcast(const char* p_address)
{
	m_broadcastStartTime = CurrentMilliTimer();
	class Broadcast* broadcast = g_pBaseNetwork->m_broadcast;
	if (broadcast->m_runEnabled != 0) {
		broadcast->Suspend();
	}
	if (p_address == 0 || *p_address == '\0') {
		broadcast->m_addressMode = 0;
	}
	else {
		broadcast->SetSpecificAddr(p_address);
	}
	class Broadcast** broadcastPtr = &g_pBaseNetwork->m_broadcast;
	if ((*broadcastPtr)->m_runEnabled == 0) {
		g_pBaseNetwork->m_suspendBroadcastOnConnect = 0;
		(*broadcastPtr)->Run();
	}
}

// 68K 0x10a00a42 Kill__15CNetworkManagerFv
// FUNCTION: LEMBALL 0x00452ab0
void NetworkManager::Kill()
{
	m_killRequested = 1;
}

// 68K 0x10a00a70 GameProcess__15CNetworkManagerFv
// FUNCTION: LEMBALL 0x00452ac0
void NetworkManager::GameProcess()
{
	if (m_killRequested != 0) {
		g_pActiveConnection = 0;
		m_killRequested = 0;
	}
	if (g_pActiveConnection != 0) {
		if (g_pActiveConnection->ReadSocket::IsChanged(*m_gameStage)) {
			g_pActiveConnection->ReadSocket::GetLatest(*m_gameStage);
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

// 68K 0x10a00b6a Process__15CNetworkManagerFv
// FUNCTION: LEMBALL 0x00452b80
void NetworkManager::Process()
{
}

// 68K 0x10a00b92 GetGameMessage__15CNetworkManagerFP8CConnect
// FUNCTION: LEMBALL 0x00452b90
NetworkGameMessage* NetworkManager::GetGameMessage(Connect* p_connection)
{
	int index = GetnGame(p_connection);
	if (index == -1) {
		return 0;
	}
	return m_gameMessages + index;
}

// 68K 0x10a00bfa GetnGame__15CNetworkManagerFP8CConnect
// FUNCTION: LEMBALL 0x00452bf0
int NetworkManager::GetnGame(Connect* p_connection)
{
	int index;
	Connect** connection;

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

// 68K 0x10a00500 __dt__15CNetworkManagerFv
NetworkManager::~NetworkManager()
{
}

// GLOBAL: LEMBALL 0x004a0120
NetworkManager* g_pNetworkManager = 0;

// GLOBAL: LEMBALL 0x004a0124
char* g_szGameName = 0;
