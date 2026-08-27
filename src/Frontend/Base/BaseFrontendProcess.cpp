#include "BaseFrontendProcess.h"

#include "../../Control/Game/GameStatus.h"
#include "../../Frontend/Support/UserActionMessage.h"
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
// STUB: LEMBALL 0x00446720
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
// STUB: LEMBALL 0x00446830
void BaseFrontendProcess::Process()
{
}

// 68K 0x10801b1a Action__20CBaseFrontendProcessF12eUserActions17eUserActionStages
// STUB: LEMBALL 0x00446860
void BaseFrontendProcess::Action(int p_action, int p_stage)
{
}

// 68K 0x10801bde ProcessMsg__20CBaseFrontendProcessFP10tagMESSAGE
// STUB: LEMBALL 0x004468d0
bool BaseFrontendProcess::ProcessMsg(Message* p_message)
{
	return 0;
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

