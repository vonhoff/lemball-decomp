#include "../CBaseFrontendProcess.h"

#include "../../../Control/Game/CGameStatus.h"
#include "../../../Frontend/Support/CUserActionMessage.h"
#include "../../../Visos/Network/CBaseNetwork.h"
#include "../../../Visos/Network/CConnect.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00446720
CBaseFrontendProcess::CBaseFrontendProcess(CGame* p_game)
{
	m_game = p_game;
	m_userActionMessage = new CUserActionMessage();
	if (g_pGameStatus->m_skill == 4 && g_pActiveConnection != NULL) {
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
