#include "CBaseFrontendProcess.h"

#include "../../Control/Game/CGameStatus.h"
#include "../../Frontend/Support/CUserActionMessage.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Messaging/BasePacketHeader.h"
#include "../../Visos/Messaging/CReadPacket.h"
#include "../../Visos/Network/CBaseNetwork.h"
#include "../../Visos/Network/CConnect.h"
#include "CBaseFrontendDrawer.h"
#include "Visos/Foundation/Message.h"

#include <new.h>

// FUNCTION: LEMBALL 0x00407f20
void CBaseFrontendProcess::Processing()
{
}

// FUNCTION: LEMBALL 0x00407f30
bool CBaseFrontendProcess::ProcessMessages(Message* p_message)
{
	return 0;
}

// FUNCTION: LEMBALL 0x00446720
CBaseFrontendProcess::CBaseFrontendProcess(CGame* p_game)
{
	m_game = p_game;
	m_userActionMessage = new CUserActionMessage();
	if (g_pGameStatus->m_skill == 4 && g_pActiveConnection != 0) {
		m_networkWasActive = 1;
	}
	else {
		m_networkWasActive = 0;
	}
	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->AttachMessageQueue(this);
	}
	g_pCurrentFrontendProcess = this;
}

// FUNCTION: LEMBALL 0x004467d0
CBaseFrontendProcess::~CBaseFrontendProcess()
{
	g_pCurrentFrontendProcess = 0;
	if (g_pBaseNetwork != 0) {
		g_pBaseNetwork->DetachMessageQueue();
	}
	if (m_userActionMessage != 0) {
		delete (CUserActionMessage*) m_userActionMessage;
	}
}

// FUNCTION: LEMBALL 0x00446830
void CBaseFrontendProcess::Process()
{
	if (m_networkWasActive != 0 && g_pActiveConnection == 0) {
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
			if (now - started >= 2000) {
				break;
			}
			g_pBaseNetwork->WaitProcess();
		}
	}
	((CUserActionMessage*) m_userActionMessage)->m_action = p_action;
	((CUserActionMessage*) m_userActionMessage)->m_stage = p_stage;
	((CUserActionMessage*) m_userActionMessage)->Send(g_pActiveConnection);
}

// FUNCTION: LEMBALL 0x00446990
bool CBaseFrontendProcess::ReceiveCritical(unsigned long p_id, CReadPacket* p_packet, CConnect* p_connection)
{
	return 0;
}

// GLOBAL: LEMBALL 0x0049f140
CBaseFrontendProcess* g_pCurrentFrontendProcess = 0;

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
