#include "CBaseFrontendProcess.h"

#include "CBaseFrontendDrawer.h"
#include "CUserActionMessage.h"
#include "Engine/Queues/tagMESSAGE.h"
#include "Engine/Time/VsTime.h"
#include "Multiplayer/Transport/CBaseCommonSocket.h"
#include "Multiplayer/Transport/CBaseNetwork.h"
#include "Multiplayer/Transport/CConnect.h"
#include "Multiplayer/Transport/NetworkConstants.h"
#include "Multiplayer/Transport/Packets/CReadPacket.h"

#include <stddef.h>

// GLOBAL: LEMBALL 0x0049f140
CBaseFrontendProcess* g_pCurrentFrontendProcess = NULL;

// GLOBAL: LEMBALL 0x0049f4f0
int g_nFrontendAutoFlowToggle = 1;

#include "Application/CGameStatus.h"
#include "Level/CLevelLoader.h"
#include "Multiplayer/CGameFlaggedMessage.h"
#include "Multiplayer/Transport/Packets/BasePacketHeader.h"

// FUNCTION: LEMBALL 0x00407f20
void CBaseFrontendProcess::Processing()
{
}

// FUNCTION: LEMBALL 0x00407f30
bool CBaseFrontendProcess::ProcessMessages(tagMESSAGE* p_message)
{
	return false;
}

// FUNCTION: LEMBALL 0x00446720
CBaseFrontendProcess::CBaseFrontendProcess(CGame* p_game)
{
	m_game = p_game;
	m_userActionMessage = new CUserActionMessage();
	if (g_pGameStatus->m_skill == SKILL_NETWORK && g_pActiveConnection != NULL) {
		m_networkWasActive = true;
	}
	else {
		m_networkWasActive = false;
	}
	if (g_pBaseNetwork != NULL) {
		g_pBaseNetwork->AttachMessageQueue(this);
	}
	g_pCurrentFrontendProcess = this;
}

// FUNCTION: LEMBALL 0x004467d0
CBaseFrontendProcess::~CBaseFrontendProcess()
{
	g_pCurrentFrontendProcess = NULL;
	if (g_pBaseNetwork != NULL) {
		g_pBaseNetwork->DetachMessageQueue();
	}
	if (m_userActionMessage != NULL) {
		delete m_userActionMessage;
	}
}

// FUNCTION: LEMBALL 0x00446830
void CBaseFrontendProcess::Process()
{
	if (m_networkWasActive != 0 && g_pActiveConnection == NULL) {
		g_pBaseFrontendDrawer->LostConnection();
	}
	Processing();
}

// FUNCTION: LEMBALL 0x00446860
void CBaseFrontendProcess::Action(eUserActions p_action, eUserActionStages p_stage)
{
	unsigned long started;
	unsigned long now;

	if (m_userActionMessage->m_pendingSendCount != 0) {
		started = CurrentMilliTimer();
		while (m_userActionMessage->m_pendingSendCount != 0) {
			now = CurrentMilliTimer();
			if (now - started >= NETWORK_PENDING_SEND_TIMEOUT_MS) {
				break;
			}
			g_pBaseNetwork->WaitProcess();
		}
	}
	m_userActionMessage->m_action = p_action;
	m_userActionMessage->m_stage = p_stage;
	m_userActionMessage->Send(g_pActiveConnection);
}

// FUNCTION: LEMBALL 0x004468d0
int CBaseFrontendProcess::ProcessMsg(tagMESSAGE* p_message)
{
	int code = p_message->m_code;
	tagMESSAGE* message = p_message;
	CReadPacket* packet;
	CConnect* connection;
	unsigned int id;

	if (g_pBaseFrontendDrawer == NULL) {
		return 0;
	}
	if (ProcessMessages(message) == 0) {
		switch (message->m_type) {
		case NETWORK_EVENT_CRITICAL_PACKET_READY:
			connection = (CConnect*) message->m_payload;
			packet = (CReadPacket*) message->m_source;
			if (code != 0) {
				return 1;
			}
			id = ((BasePacketHeader*) packet->m_data)->m_messageId;
			if (id != GAME_MESSAGE_USER_ACTION) {
				return ReceiveCritical(id, packet, connection);
			}
			m_userActionMessage->Set(packet->m_data + sizeof(BasePacketHeader));
			packet->m_used = 0;
			g_pBaseFrontendDrawer->RemoteAction(m_userActionMessage->m_action, m_userActionMessage->m_stage);
			return 1;
		default:
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00446990
bool CBaseFrontendProcess::ReceiveCritical(unsigned long p_id, CReadPacket* p_packet, CConnect* p_connection)
{
	return false;
}
