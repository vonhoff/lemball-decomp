#include "../CMainOptions1Drawer.h"

#include "../../../Control/Game/CGameStatus.h"
#include "../../../Frontend/Base/CBaseFrontendProcess.h"
#include "../../../Views/Display/CMain2DDisplay.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Visos/Foundation/CVSRect.h"
#include "Visos/Foundation/Message.h"

class CGWnd;

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00448620
bool CMainOptions1Drawer::ProcessMessages(Message* p_message)
{
	int type;
	int mode;
	CGameStatus* status;

	type = p_message->m_type;
	switch (type) {
	case 3:
	case 4:
		m_idleDeadline = timeGetTime() + 20000;
		break;
	case MESSAGE_BUTTON_RELEASED:
		m_idleDeadline = timeGetTime() + 20000;
		switch ((unsigned int) p_message->m_code) {
		case MAIN_OPTIONS1_BUTTON_MESSAGE_OPTIONS:
			m_returnState = FLOW_MAIN_OPTIONS_2;
			m_quitYet = 1;
			g_nFrontendAutoFlowToggle = 1;
			return true;
		case MAIN_OPTIONS1_BUTTON_MESSAGE_PASSWORD:
			m_returnState = FLOW_PASSWORD;
			m_quitYet = 1;
			g_nFrontendAutoFlowToggle = 1;
			return true;
		case MAIN_OPTIONS1_BUTTON_MESSAGE_RESOLUTION:
			m_display->ToggleResolution();
			return true;
		case MAIN_OPTIONS1_BUTTON_MESSAGE_PREVIEW:
		case MAIN_OPTIONS1_BUTTON_MESSAGE_NETWORK: {
			mode = m_selectedDisplayMode;
			status = g_pGameStatus;
			status->m_level = status->m_lastLevels[mode];
			status->m_skill = mode;
			m_quitYet = 1;
			g_nFrontendAutoFlowToggle = 1;
			if (p_message->m_code == MAIN_OPTIONS1_BUTTON_MESSAGE_PREVIEW) {
				m_returnState = FLOW_PREVIEW;
			}
			else {
				m_returnState = FLOW_NETWORK_OPTIONS;
			}
			return true;
		}
		}
		break;
	default:
		m_processedCount = m_processedCount + 1;
		return false;
	}
	return false;
}
