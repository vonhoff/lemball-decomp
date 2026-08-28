#include "BaseFrontendProcess.h"

#include "../../Control/Game/GameStatus.h"
#include "../../Frontend/Drawers/NetworkOptionsDrawer.h"
#include "../../Frontend/Processes/NetworkOptionsProc.h"
#include "../../Frontend/Support/UserActionMessage.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Messaging/BasePacketHeader.h"
#include "../../Visos/Messaging/ReadPacket.h"
#include "../../Visos/Network/BaseNetwork.h"
#include "../../Visos/Network/Connect.h"

#include <new.h>

BaseFrontendProcess::BaseFrontendProcess()
{
}

// 68K 0x1011bac4 Processing__20CBaseFrontendProcessFv
// FUNCTION: LEMBALL 0x00407f20
void BaseFrontendProcess::Processing()
{
}

// 68K 0x1011baf4 ProcessMessages__20CBaseFrontendProcessFP10tagMESSAGE
// FUNCTION: LEMBALL 0x00407f30
bool BaseFrontendProcess::ProcessMessages(Message* p_message)
{
	return 0;
}

// 68K 0x10801930 __ct__20CBaseFrontendProcessFP5CGame
// FUNCTION: LEMBALL 0x00446720
BaseFrontendProcess::BaseFrontendProcess(Game* p_arg0)
{
	void* storage;

	m_processState = 0;
	m_returnState = 0;
	m_game = p_arg0;
	storage = operator new(0x34);
	if (storage == 0) {
		m_userActionMessage = 0;
	}
	else {
		m_userActionMessage = new (storage) UserActionMessage();
	}
	if (g_pGameStatus != 0 && g_pGameStatus->m_skill == 4 && g_pActiveConnection != 0) {
		m_networkWasActive = 1;
	}
	else {
		m_networkWasActive = 0;
	}
	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->AttachMessageQueue(this);
	}
}

// 68K 0x10801abe Process__20CBaseFrontendProcessFv
// FUNCTION: LEMBALL 0x00446830
void BaseFrontendProcess::Process()
{
	if (m_networkWasActive != 0 && g_pActiveConnection == 0) {
		g_pNetworkOptionsDrawer->LostConnection();
	}
	Processing();
}

// 68K 0x10801b1a Action__20CBaseFrontendProcessF12eUserActions17eUserActionStages
// FUNCTION: LEMBALL 0x00446860
void BaseFrontendProcess::Action(int p_action, int p_stage)
{
	UserActionMessage* message;
	unsigned long started;
	unsigned long now;

	message = (UserActionMessage*) m_userActionMessage;
	if (message->m_pendingSendCount != 0) {
		started = CurrentMilliTimer();
		while (message->m_pendingSendCount != 0) {
			now = CurrentMilliTimer();
			if (1999 < now - started) {
				break;
			}
			g_pBaseNetwork->WaitProcess();
		}
	}
	message->m_action = (eUserActions) p_action;
	message->m_stage = (eUserActionStages) p_stage;
	message->Send(g_pActiveConnection);
}

// 68K 0x10801bde ProcessMsg__20CBaseFrontendProcessFP10tagMESSAGE
// FUNCTION: LEMBALL 0x004468d0
int BaseFrontendProcess::ProcessMsg(Message* p_message)
{
	ReadPacket* packet;
	unsigned int id;
	unsigned int type;

	if (g_pNetworkOptionsDrawer == 0) {
		return 0;
	}
	if (ProcessMessages(p_message) != 0) {
		return 1;
	}
	type = p_message->type;
	if (type != 5) {
		return 0;
	}
	packet = (ReadPacket*) p_message->source;
	if (p_message->code != 0) {
		return 1;
	}
	id = ((BasePacketHeader*) packet->m_data)->m_messageId;
	if (id != 8) {
		return ReceiveCritical(id, packet, (Connect*) p_message->payload);
	}
	((UserActionMessage*) m_userActionMessage)->Set(packet->m_data + sizeof(BasePacketHeader));
	packet->m_used = 0;
	g_pNetworkOptionsDrawer->RemoteAction(
		((UserActionMessage*) m_userActionMessage)->m_action,
		((UserActionMessage*) m_userActionMessage)->m_stage);
	return 1;
}

// 68K 0x10801cdc ReceiveCritical__20CBaseFrontendProcessFUlP11CReadPacketP8CConnect
// FUNCTION: LEMBALL 0x00446990
bool BaseFrontendProcess::ReceiveCritical(unsigned long p_id, ReadPacket* p_packet, Connect* p_connection)
{
	return 0;
}

// 68K 0x10801a0a __dt__20CBaseFrontendProcessFv
BaseFrontendProcess::~BaseFrontendProcess()
{
}

// GLOBAL: LEMBALL 0x0049ca30
int g_nTestAllLevels = 0;

// GLOBAL: LEMBALL 0x0049f4f0
int g_nFrontendAutoFlowToggle = 1;

// GLOBAL: LEMBALL 0x004a6284
int g_nAnimationsDisabled = 0;

// GLOBAL: LEMBALL 0x004a6288
int g_nZoomEnabled = 0;

// GLOBAL: LEMBALL 0x004a628c
int g_nMusicAvailable = 0;

// GLOBAL: LEMBALL 0x004a6290
int g_nEffectsAvailable = 0;

// GLOBAL: LEMBALL 0x004a6294
int g_nAnimationsAvailable = 0;

// GLOBAL: LEMBALL 0x004a6298
int g_nZoomAvailable = 0;

// GLOBAL: LEMBALL 0x004a6300
int g_nDisplayMode = 0;

