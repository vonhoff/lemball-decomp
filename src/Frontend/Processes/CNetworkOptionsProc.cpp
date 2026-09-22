#include "CNetworkOptionsProc.h"

#include "../../Control/Game/CGame.h"
#include "../../Network/Game/CNetworkManager.h"
#include "../../Network/Messages/CGameAcceptMessage.h"
#include "../../Network/Messages/CGameRejectMessage.h"
#include "../../Visos/Messaging/BasePacketHeader.h"
#include "../../Visos/Messaging/CReadPacket.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Network/CBroadcast.h"
#include "../../Visos/Network/CConnect.h"
#include "../Base/CBaseFrontendDrawer.h"
#include "../Drawers/CNetworkOptionsDrawer.h"

#define g_pNetworkOptionsDrawer ((CNetworkOptionsDrawer*) g_pBaseFrontendDrawer)

#include "Frontend/Base/CBaseFrontendProcess.h"
#include "Visos/Messaging/CNetworkMessage.h"

#include <new.h>

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00455050
CNetworkOptionsProc::CNetworkOptionsProc(CGame* p_game) : CBaseFrontendProcess(p_game)
{
	void* storage;

	storage = operator new(0x30);
	if (storage == 0) {
		m_rejectMessage = 0;
	}
	else {
		m_rejectMessage = new (storage) CGameRejectMessage();
	}
	storage = operator new(0x30);
	if (storage == 0) {
		m_acceptMessage = 0;
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
	g_pNetworkOptionsProc = 0;
	if (g_pNetworkOptionsDrawer != 0) {
		if (g_pNetworkOptionsDrawer->GetReturnState() == 0) {
			Stop();
		}
		else {
			StopBroadcast();
		}
	}
	if (m_rejectMessage != 0) {
		delete m_rejectMessage;
	}
	if (m_acceptMessage != 0) {
		delete m_acceptMessage;
	}
}

// FUNCTION: LEMBALL 0x00455130
void CNetworkOptionsProc::Start()
{
	m_startFailed = 0;
	if (m_started == 0) {
		g_pNetworkManager = new CNetworkManager(0);
		if (!g_pNetworkManager->Start()) {
			g_pNetworkManager->Stop();
			delete g_pNetworkManager;
			g_pNetworkManager = 0;
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

	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->m_broadcast->Suspend();
		g_pBaseNetwork->m_broadcast->StopListen();
	}
	if (g_pNetworkManager != 0) {
		connections = g_pNetworkManager->m_connections;
		m_rejectMessage->m_flag = 1;
		index = 0;
		do {
			if (index < 10) {
				g_pNetworkOptionsDrawer->GameNotReady(index);
			}
			if (*connections != 0 && *connections != g_pActiveConnection) {
				startTime = timeGetTime();
				while (m_rejectMessage->m_pendingSendCount != 0 && timeGetTime() - startTime < 1000) {
				}
				m_rejectMessage->Send(*connections);
				startTime = timeGetTime();
				while (m_rejectMessage->m_pendingSendCount != 0 && timeGetTime() - startTime < 1000) {
				}
				(*connections)->Kill();
			}
			connections++;
			index++;
		} while (index < 10);
	}
}

// FUNCTION: LEMBALL 0x004552a0
void CNetworkOptionsProc::Stop()
{
	unsigned long startTime;

	StopBroadcast();
	if (g_pNetworkManager != 0) {
		g_pNetworkManager->Stop();
	}
	if (g_pBaseNetwork != 0) {
		startTime = timeGetTime();
		while (timeGetTime() - startTime < 2000 && g_pBaseNetwork->m_queueTransitionPending != 0) {
		}
	}
	if (g_pNetworkManager != 0) {
		delete g_pNetworkManager;
		g_pNetworkManager = 0;
	}
	m_startFailed = 0;
	m_started = 0;
}

// FUNCTION: LEMBALL 0x00455320
void CNetworkOptionsProc::NetworkEvent(NetworkEvents p_event)
{
	if (g_pNetworkOptionsDrawer != 0) {
		switch (p_event) {
		case 10:
			g_pNetworkOptionsDrawer->ResetHandlers();
			break;
		case 13:
			g_pNetworkOptionsDrawer->m_pendingEvent = 7;
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
	case 5: {
		CNetworkMessage* message = (CNetworkMessage*) g_pNetworkManager->GetGameMessage(connection);
		if (message != 0) {
			message->Set(packet->m_data + sizeof(BasePacketHeader));
		}
		packet->m_used = 0;
		drawer->m_networkState = 1;
		return true;
	}
	case 6: {
		m_rejectMessage->Set(packet->m_data + sizeof(BasePacketHeader));
		packet->m_used = 0;
		drawer->GameNotReady(g_pNetworkManager->GetnGame(connection));
		if (m_rejectMessage->m_flag != 0) {
			connection->Kill();
		}
		drawer->m_networkState = 1;
		return true;
	}
	case 7: {
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
CNetworkOptionsProc* g_pNetworkOptionsProc = 0;
