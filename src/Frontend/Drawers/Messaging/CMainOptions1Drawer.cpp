#include "../CMainOptions1Drawer.h"

#include "../../../Control/Game/CGameStatus.h"
#include "../../../Frontend/Base/CBaseFrontendProcess.h"
#include "../../../Frontend/Controls/CGunController.h"
#include "../../../Views/Display/CMain2DDisplay.h"
#include "../../../Visos/Resources/CResBITMAP.h"
#include "../../../Visos/Resources/Manifest.h"
#include "Frontend/Base/CBaseFrontendDrawer.h"
#include "Frontend/Base/FlowProcesses.h"
#include "Visos/Foundation/CVsRect.h"
#include "Visos/Foundation/Message.h"
#include "Visos/Foundation/tagPRIMS.h"
#include "Visos/Graphics/CBitmapRes.h"

class CGWnd;

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00448620
bool CMainOptions1Drawer::ProcessMessages(Message* p_message)
{
	int type;
	int mode;
	CGameStatus* status;

	type = p_message->type;
	switch (type) {
	case 3:
	case 4:
		m_idleDeadline = timeGetTime() + 20000;
		break;
	case 0xc:
		m_idleDeadline = timeGetTime() + 20000;
		switch (p_message->code) {
		case 0xacef0001:
			m_returnState = 3;
			m_quitYet = 1;
			g_nFrontendAutoFlowToggle = 1;
			return true;
		case 0xacef00a4:
			m_returnState = 0x10;
			m_quitYet = 1;
			g_nFrontendAutoFlowToggle = 1;
			return true;
		case 0xacef00a5:
			m_display->ToggleResolution();
			return true;
		case 0xacef00a6:
		case 0xacef00a7: {
			mode = m_selectedDisplayMode;
			status = g_pGameStatus;
			status->m_level = status->m_lastLevels[mode];
			status->m_skill = mode;
			m_quitYet = 1;
			g_nFrontendAutoFlowToggle = 1;
			if (p_message->code == 0xacef00a6) {
				m_returnState = 4;
				return true;
			}
			m_returnState = 0xc;
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
