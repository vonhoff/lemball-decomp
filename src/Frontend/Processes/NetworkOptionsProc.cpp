#include "NetworkOptionsProc.h"

#include "../../Control/Game/Game.h"
#include "../../Network/Game/NetworkManager.h"
#include "../../Network/Messages/GameAcceptMessage.h"
#include "../../Network/Messages/GameRejectMessage.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Messaging/BasePacketHeader.h"
#include "../../Visos/Messaging/ReadPacket.h"
#include "../../Visos/Network/BaseNetwork.h"
#include "../../Visos/Network/Broadcast.h"
#include "../../Visos/Network/Connect.h"
#include "../Drawers/NetworkOptionsDrawer.h"
#include "../Base/BaseFrontendDrawer.h"

#define g_pNetworkOptionsDrawer ((NetworkOptionsDrawer*) g_pBaseFrontendDrawer)

#include <new.h>

// 68K 0x10808988 __ct__19CNetworkOptionsProcFP5CGame
// FUNCTION: LEMBALL 0x00455050
NetworkOptionsProc::NetworkOptionsProc(Game* p_arg0) : BaseFrontendProcess(p_arg0)
{
	void* storage;

	storage = operator new(0x30);
	if (storage == 0) {
		m_rejectMessage = 0;
	}
	else {
		m_rejectMessage = new (storage) GameRejectMessage();
	}
	storage = operator new(0x30);
	if (storage == 0) {
		m_acceptMessage = 0;
	}
	else {
		m_acceptMessage = new (storage) GameAcceptMessage();
	}
	m_started = 0;
	m_startFailed = 0;
	g_pNetworkOptionsProc = this;
}

// 68K 0x10808a36 __dt__19CNetworkOptionsProcFv
// FUNCTION: LEMBALL 0x004550e0
NetworkOptionsProc::~NetworkOptionsProc()
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

// 68K 0x10808b14 Start__19CNetworkOptionsProcFv
// FUNCTION: LEMBALL 0x00455130
void NetworkOptionsProc::Start()
{
	m_startFailed = 0;
	if (m_started == 0) {
		g_pNetworkManager = new NetworkManager(0);
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

// 68K 0x10808bee StopBroadcast__19CNetworkOptionsProcFv
// FUNCTION: LEMBALL 0x004551d0
void NetworkOptionsProc::StopBroadcast()
{
	int index;
	Connect** connections;
	unsigned long startTime;

	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->m_broadcast->Suspend();
		g_pBaseNetwork->m_broadcast->Stop();
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
				startTime = CurrentMilliTimer();
				while (m_rejectMessage->m_pendingSendCount != 0 && CurrentMilliTimer() - startTime < 1000) {
				}
				m_rejectMessage->Send(*connections);
				startTime = CurrentMilliTimer();
				while (m_rejectMessage->m_pendingSendCount != 0 && CurrentMilliTimer() - startTime < 1000) {
				}
				(*connections)->Kill();
			}
			connections++;
			index++;
		} while (index < 10);
	}
}

// 68K 0x10808d12 Stop__19CNetworkOptionsProcFv
// FUNCTION: LEMBALL 0x004552a0
void NetworkOptionsProc::Stop()
{
	unsigned long startTime;

	StopBroadcast();
	if (g_pNetworkManager != 0) {
		g_pNetworkManager->Stop();
	}
	if (g_pBaseNetwork != 0) {
		startTime = CurrentMilliTimer();
		while (CurrentMilliTimer() - startTime < 2000 && g_pBaseNetwork->m_queueTransitionPending != 0) {
		}
	}
	if (g_pNetworkManager != 0) {
		delete g_pNetworkManager;
		g_pNetworkManager = 0;
	}
	m_startFailed = 0;
	m_started = 0;
}

// 68K 0x10808dc8 NetworkEvent__19CNetworkOptionsProcF13NetworkEvents
// FUNCTION: LEMBALL 0x00455320
void NetworkOptionsProc::NetworkEvent(int p_event)
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

// 68K 0x10808e40 ReceiveCritical__19CNetworkOptionsProcFUlP11CReadPacketP8CConnect
// FUNCTION: LEMBALL 0x00455360
bool NetworkOptionsProc::ReceiveCritical(unsigned long p_id, ReadPacket* p_packet, Connect* p_connection)
{
	NetworkOptionsDrawer* drawer = g_pNetworkOptionsDrawer;
	Connect* connection = p_connection;
	ReadPacket* packet = p_packet;

	switch (p_id) {
	case 5: {
		NetworkMessage* message = (NetworkMessage*) g_pNetworkManager->GetGameMessage(connection);
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

// 68K 0x10808fa6 Accept__19CNetworkOptionsProcFP8CConnectUc
// FUNCTION: LEMBALL 0x00455480
void NetworkOptionsProc::Accept(Connect* p_connection, unsigned int p_ready)
{
	m_acceptMessage->m_flag = p_ready;
	m_acceptMessage->Send(p_connection);
}

// 68K 0x10809004 Reject__19CNetworkOptionsProcFP8CConnect
// FUNCTION: LEMBALL 0x004554a0
void NetworkOptionsProc::Reject(Connect* p_connection)
{
	m_rejectMessage->m_flag = 0;
	m_rejectMessage->Send(p_connection);
}

// 68K 0x1011c4b2 Processing__19CNetworkOptionsProcFv
// FUNCTION: LEMBALL 0x00455ea0
void NetworkOptionsProc::Processing()
{
}


// GLOBAL: LEMBALL 0x004a0128
NetworkOptionsProc* g_pNetworkOptionsProc = 0;

