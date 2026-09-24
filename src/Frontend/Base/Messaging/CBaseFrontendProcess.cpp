#include "../CBaseFrontendProcess.h"

#include "../../../Control/Game/CGameStatus.h"
#include "../../../Frontend/Support/CUserActionMessage.h"
#include "../../../Visos/Foundation/VsTime.h"
#include "../../../Visos/Messaging/BasePacketHeader.h"
#include "../../../Visos/Messaging/CReadPacket.h"
#include "../../../Visos/Network/CBaseNetwork.h"
#include "../../../Visos/Network/CConnect.h"
#include "../CBaseFrontendDrawer.h"
#include "Visos/Foundation/Message.h"

#include <new.h>

// FUNCTION: LEMBALL 0x004468d0
int CBaseFrontendProcess::ProcessMsg(Message* p_message)
{
	int code = p_message->code;
	Message* message = p_message;
	CReadPacket* packet;
	CConnect* connection;
	unsigned int id;

	if (g_pBaseFrontendDrawer == 0) {
		return 0;
	}
	if (ProcessMessages(message) == 0) {
		switch ((unsigned int) message->type) {
		case 5:
			connection = (CConnect*) message->payload;
			packet = (CReadPacket*) message->source;
			if (code != 0) {
				return 1;
			}
			id = ((BasePacketHeader*) packet->m_data)->m_messageId;
			if (id != 8) {
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
