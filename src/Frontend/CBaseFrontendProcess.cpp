#include "CBaseFrontendProcess.h"

#include "CBaseFrontendDrawer.h"
#include "CUserActionMessage.h"
#include "Engine/Network/CBaseNetwork.h"
#include "Engine/Network/CConnect.h"
#include "Engine/Network/NetworkConstants.h"
#include "Engine/Network/Packets/CReadPacket.h"
#include "Engine/Queues/Message.h"
#include "Engine/VsTime.h"

#include <stddef.h>

// GLOBAL: LEMBALL 0x0049f140
CBaseFrontendProcess* g_pCurrentFrontendProcess = NULL;

// GLOBAL: LEMBALL 0x0049f4f0
int g_nFrontendAutoFlowToggle = 1;

#include "Application/CGameStatus.h"
#include "Level/CLevelLoader.h"
#include "Multiplayer/CGameFlaggedMessage.h"
#include "Engine/Network/Packets/BasePacketHeader.h"

// FUNCTION: LEMBALL 0x00407f20
void CBaseFrontendProcess::Processing()
{
}

// FUNCTION: LEMBALL 0x00407f30
bool CBaseFrontendProcess::ProcessMessages(Message* p_message)
{
	return false;
}

// FUNCTION: LEMBALL 0x00446720
CBaseFrontendProcess::CBaseFrontendProcess(CGame* p_game)
{
	m_game = p_game;
	m_userActionMessage = new CUserActionMessage();
	if (g_pGameStatus->m_skill == SKILL_NETWORK && g_pActiveConnection != NULL) {
		m_networkWasActive = 1;
	}
	else {
		m_networkWasActive = 0;
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
		delete (CUserActionMessage*) m_userActionMessage;
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

	if (((CUserActionMessage*) m_userActionMessage)->m_pendingSendCount != 0) {
		started = CurrentMilliTimer();
		while (((CUserActionMessage*) m_userActionMessage)->m_pendingSendCount != 0) {
			now = CurrentMilliTimer();
			if (now - started >= NETWORK_PENDING_SEND_TIMEOUT_MS) {
				break;
			}
			g_pBaseNetwork->WaitProcess();
		}
	}
	((CUserActionMessage*) m_userActionMessage)->m_action = p_action;
	((CUserActionMessage*) m_userActionMessage)->m_stage = p_stage;
	((CUserActionMessage*) m_userActionMessage)->Send(g_pActiveConnection);
}

// FUNCTION: LEMBALL 0x004468d0
int CBaseFrontendProcess::ProcessMsg(Message* p_message)
{
	int code = p_message->m_code;
	Message* message = p_message;
	CReadPacket* packet;
	CConnect* connection;
	unsigned int id;

	if (g_pBaseFrontendDrawer == NULL) {
		return 0;
	}
	if (ProcessMessages(message) == 0) {
		switch ((unsigned int) message->m_type) {
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
			((CUserActionMessage*) m_userActionMessage)->Set(packet->m_data + sizeof(BasePacketHeader));
			packet->m_used = 0;
			g_pBaseFrontendDrawer->RemoteAction(((CUserActionMessage*) m_userActionMessage)->m_action,
												((CUserActionMessage*) m_userActionMessage)->m_stage);
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
