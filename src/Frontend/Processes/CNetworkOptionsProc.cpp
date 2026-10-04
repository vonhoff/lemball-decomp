#include "CNetworkOptionsProc.h"

#include "../../Control/Game/CGame.h"
#include "../../Network/Game/CNetworkManager.h"
#include "../../Network/Messages/CGameAcceptMessage.h"
#include "../../Network/Messages/CGameRejectMessage.h"
#include "Visos/Network/Packets/BasePacketHeader.h"
#include "Visos/Network/Packets/CReadPacket.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Network/CBroadcast.h"
#include "../../Visos/Network/CConnect.h"
#include "../../Visos/Network/NetworkConstants.h"
#include "../Base/CBaseFrontendDrawer.h"
#include "../Drawers/CNetworkOptionsDrawer.h"

#define g_pNetworkOptionsDrawer ((CNetworkOptionsDrawer*) g_pBaseFrontendDrawer)

#include "Frontend/Base/CBaseFrontendProcess.h"
#include "Visos/Network/Protocol/CNetworkMessage.h"

#include <new.h>
#include <stddef.h>

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00455050
CNetworkOptionsProc::CNetworkOptionsProc(CGame* p_game) : CBaseFrontendProcess(p_game)
{
	void* storage;

	storage = operator new(sizeof(CGameRejectMessage));
	if (storage == NULL) {
		m_rejectMessage = NULL;
	}
	else {
		m_rejectMessage = new (storage) CGameRejectMessage();
	}
	storage = operator new(sizeof(CGameAcceptMessage));
	if (storage == NULL) {
		m_acceptMessage = NULL;
	}
	else {
		m_acceptMessage = new (storage) CGameAcceptMessage();
	}
	m_started = 0;
	m_startFailed = 0;
	g_pNetworkOptionsProc = this;
}

// FUNCTION: LEMBALL 0x004550c0
CNetworkOptionsProc::~CNetworkOptionsProc()
{
	g_pNetworkOptionsProc = NULL;
	if (g_pNetworkOptionsDrawer != NULL) {
		if (g_pNetworkOptionsDrawer->GetReturnState() == 0) {
			Stop();
		}
		else {
			StopBroadcast();
		}
	}
	if (m_rejectMessage != NULL) {
		delete m_rejectMessage;
	}
	if (m_acceptMessage != NULL) {
		delete m_acceptMessage;
	}
}

// FUNCTION: LEMBALL 0x00455130
void CNetworkOptionsProc::Start()
{
	m_startFailed = 0;
	if (m_started == 0) {
		g_pNetworkManager = new CNetworkManager(NULL);
		if (!g_pNetworkManager->Start()) {
			g_pNetworkManager->Stop();
			delete g_pNetworkManager;
			g_pNetworkManager = NULL;
			m_startFailed = 1;
			return;
		}
		g_pBaseNetwork->AttachMessageQueue(this);
		m_started = 1;
	}
}

// FUNCTION: LEMBALL 0x004551d0
void CNetworkOptionsProc::StopBroadcast()
{
	int index;
	CConnect** connections;
	unsigned long startTime;

	if (g_pBaseNetwork != NULL) {
		g_pBaseNetwork->m_broadcast->Suspend();
		g_pBaseNetwork->m_broadcast->StopListen();
	}
	if (g_pNetworkManager != NULL) {
		connections = g_pNetworkManager->m_connections;
		m_rejectMessage->m_flag = 1;
		index = 0;
		do {
			if (index < NETWORK_GAME_SLOT_COUNT) {
				g_pNetworkOptionsDrawer->GameNotReady(index);
			}
			if (*connections != NULL && *connections != g_pActiveConnection) {
				startTime = timeGetTime();
				while (m_rejectMessage->m_pendingSendCount != 0 &&
					   timeGetTime() - startTime < NETWORK_MESSAGE_SEND_WAIT_TIMEOUT_MS) {
				}
				m_rejectMessage->Send(*connections);
				startTime = timeGetTime();
				while (m_rejectMessage->m_pendingSendCount != 0 &&
					   timeGetTime() - startTime < NETWORK_MESSAGE_SEND_WAIT_TIMEOUT_MS) {
				}
				(*connections)->Kill();
			}
			connections++;
			index++;
		} while (index < NETWORK_GAME_SLOT_COUNT);
	}
}

// FUNCTION: LEMBALL 0x004552a0
void CNetworkOptionsProc::Stop()
{
	unsigned long startTime;

	StopBroadcast();
	if (g_pNetworkManager != NULL) {
		g_pNetworkManager->Stop();
	}
	if (g_pBaseNetwork != NULL) {
		startTime = timeGetTime();
		while (timeGetTime() - startTime < NETWORK_QUEUE_TRANSITION_TIMEOUT_MS &&
			   g_pBaseNetwork->m_queueTransitionPending != 0) {
		}
	}
	if (g_pNetworkManager != NULL) {
		delete g_pNetworkManager;
		g_pNetworkManager = NULL;
	}
	m_startFailed = 0;
	m_started = 0;
}

// FUNCTION: LEMBALL 0x00455320
void CNetworkOptionsProc::NetworkEvent(NetworkEvents p_event)
{
	if (g_pNetworkOptionsDrawer != NULL) {
		switch (p_event) {
		case NETWORK_EVENT_CONNECTION_CLOSED:
			g_pNetworkOptionsDrawer->ResetHandlers();
			break;
		case NETWORK_EVENT_HOST_LOOKUP_FAILED:
			g_pNetworkOptionsDrawer->m_pendingEvent = NETWORK_OPTIONS_MESSAGE_HOST_LOOKUP_FAILED;
			break;
		}
	}
}

// FUNCTION: LEMBALL 0x00455360
bool CNetworkOptionsProc::ReceiveCritical(unsigned long p_id, CReadPacket* p_packet, CConnect* p_connection)
{
	CNetworkOptionsDrawer* drawer = g_pNetworkOptionsDrawer;
	CConnect* connection = p_connection;
	CReadPacket* packet = p_packet;

	switch (p_id) {
	case GAME_MESSAGE_GAME_INFO: {
		CNetworkMessage* message = (CNetworkMessage*) g_pNetworkManager->GetGameMessage(connection);
		if (message != NULL) {
			message->Set(packet->m_data + sizeof(BasePacketHeader));
		}
		packet->m_used = 0;
		drawer->m_networkState = NETWORK_OPTIONS_HANDLERS_STALE;
		return true;
	}
	case GAME_MESSAGE_REJECT: {
		m_rejectMessage->Set(packet->m_data + sizeof(BasePacketHeader));
		packet->m_used = 0;
		drawer->GameNotReady(g_pNetworkManager->GetnGame(connection));
		if (m_rejectMessage->m_flag != 0) {
			connection->Kill();
		}
		drawer->m_networkState = NETWORK_OPTIONS_HANDLERS_STALE;
		return true;
	}
	case GAME_MESSAGE_ACCEPT: {
		m_acceptMessage->Set(packet->m_data + sizeof(BasePacketHeader));
		packet->m_used = 0;
		drawer->GameReady(g_pNetworkManager->GetnGame(connection));
		if (m_acceptMessage->m_flag != 0) {
			if (drawer->AcceptingLock()) {
				m_acceptMessage->Send(connection);
			}
		}
		return true;
	}
	default:
		return false;
	}
}

// FUNCTION: LEMBALL 0x00455480
void CNetworkOptionsProc::Accept(CConnect* p_connection, unsigned int p_ready)
{
	m_acceptMessage->m_flag = p_ready;
	m_acceptMessage->Send(p_connection);
}

// FUNCTION: LEMBALL 0x004554a0
void CNetworkOptionsProc::Reject(CConnect* p_connection)
{
	m_rejectMessage->m_flag = 0;
	m_rejectMessage->Send(p_connection);
}

// FUNCTION: LEMBALL 0x00455ea0
void CNetworkOptionsProc::Processing()
{
}

// GLOBAL: LEMBALL 0x004a0128
CNetworkOptionsProc* g_pNetworkOptionsProc = NULL;
