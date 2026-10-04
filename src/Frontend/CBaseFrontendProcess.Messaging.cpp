#include "CBaseFrontendProcess.h"

#include "CUserActionMessage.h"
#include "Network/Messages/CGameFlaggedMessage.h"
#include "Visos/Network/Packets/BasePacketHeader.h"
#include "Visos/Network/Packets/CReadPacket.h"
#include "Visos/Network/CConnect.h"
#include "CBaseFrontendDrawer.h"
#include "Visos/Queues/Message.h"

#include <stddef.h>

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
