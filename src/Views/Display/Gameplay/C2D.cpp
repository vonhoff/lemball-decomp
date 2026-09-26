#include "../C2D.h"

#include "../../../AI/Navigation/CAI.h"
#include "../../../Control/Game/CDemo.h"
#include "../../../Control/Game/CGameStatus.h"
#include "../../../Frontend/Base/CBaseFrontendProcess.h"
#include "../../../Visos/Foundation/CVSOStream.h"
#include "../../../Visos/Foundation/VsTime.h"
#include "../../../Visos/Network/CConnect.h"
#include "../../Animation/CLemmingAnimsManager.h"
#include "../../Panel/CPanel.h"
#include "../CMain2DDisplay.h"

// FUNCTION: LEMBALL 0x00436a10
void C2D::Process()
{
	CheckValidFormGroup();
	if (g_pDemo != 0 && g_pDemo->m_demoMode != 0 && g_pDemo->m_gameOver != 0) {
		m_ai->GameState(GAME_STATUS_SUCCESS);
	}
	if (m_connectionTimeoutActive != 0) {
		if (CurrentMilliTimer() - m_connectionTimeoutStart >= 2000) {
			m_quitRequested = 1;
			m_returnState = FLOW_MAIN_OPTIONS_1;
		}
		return;
	}
	if (m_lemmingAnims->m_loaded == 0) {
		m_lemmingAnims->Load(m_ai->m_mapType);
		OnLoaded();
	}
	SetMouseShape();
	m_panel->Process();
	if (m_ai->m_started == 0 && m_ai->m_networkMode != 0) {
		if (m_pauseMessage != PAUSE_MSG_PLEASE_WAIT) {
			NewPauseWindow(PAUSE_MSG_PLEASE_WAIT);
		}
	}
	else if (m_pauseMessage == PAUSE_MSG_PLEASE_WAIT) {
		NewPauseWindow(PAUSE_MSG_NONE);
	}

	if (m_optionSelection != 0) {
		switch (m_pauseMessage) {
		case PAUSE_MSG_PAUSED:
			switch (m_optionSelection) {
			case 2:
				if (m_pauser != 0) {
					m_panel->SetPause(0);
				}
				break;
			case 3:
				NewPauseWindow(PAUSE_MSG_ARE_YOU_SURE);
				break;
			case 4:
				NewPauseWindow(PAUSE_MSG_ARE_YOU_SURE);
				break;
			default:
				*g_pErrorOutput << "Unknown Pause window option selected\n";
				break;
			}
			break;
		case PAUSE_MSG_LOADING:
			m_optionSelection = 0;
			goto optionHandled;
		case PAUSE_MSG_ARE_YOU_SURE:
			*g_pErrorOutput << "Confirmed\n";
			if (m_optionSelection == 2) {
				*g_pErrorOutput << "Confirmed Yes\n";
				if (m_previousPauseMessage == PAUSE_MSG_PAUSED) {
					*g_pErrorOutput << "Confirmed Yes Pause\n";
					switch (m_pauseSelection) {
					case 3:
						*g_pErrorOutput << "Confirmed Yes Pause Restart\n";
						m_ai->GameState(GAME_STATUS_RESTART);
						break;
					case 4:
						*g_pErrorOutput << "Confirmed Yes Pause Quit\n";
						m_ai->QuitGame();
						break;
					default:
						*g_pErrorOutput << "Unknown Confirmation option selected\n";
						break;
					}
				}
			}
			else {
				NewPauseWindow(PAUSE_MSG_PAUSED);
			}
			break;
		}
		m_optionSelection = 0;
	}

optionHandled:
	switch (m_ai->m_gameStatus) {
	case GAME_STATUS_PAUSED:
		if (m_paused == 0 && m_ai->m_gameStatePending == 0) {
			m_panel->SetPause(1);
		}
		break;
	case GAME_STATUS_RUNNING:
		if (m_paused != 0 && m_ai->m_gameStatePending == 0) {
			m_panel->SetPause(0);
		}
		break;
	case GAME_STATUS_SUCCESS:
		if (g_nDemoMode != 0) {
			m_returnState = FLOW_MAIN_OPTIONS_1;
		}
		else {
			if (m_score != m_ai->m_score) {
				m_score = m_ai->m_score;
			}
			m_returnState = FLOW_SUCCESS;
			if (m_ai->m_networkMode == 0) {
				int levelCount = g_pGameStatus->NoOfLevelsInSkill(g_pGameStatus->m_skill);
				if (levelCount == g_pGameStatus->Level()) {
					m_returnState = FLOW_LEVEL_INTRO;
					g_pGameStatus->IncSkill(1);
				}
				else {
					g_pGameStatus->IncLevel();
				}
			}
		}
		m_quitRequested = 1;
		NewPauseWindow(PAUSE_MSG_LOADING);
		m_display->RefreshView();
		break;
	case GAME_STATUS_FAILURE:
		m_quitRequested = 1;
		if (g_nDemoMode != 0) {
			m_returnState = FLOW_MAIN_OPTIONS_1;
		}
		else {
			m_returnState = FLOW_FAILURE;
		}
		m_ai->m_paused = 0;
		NewPauseWindow(PAUSE_MSG_LOADING);
		m_display->RefreshView();
		g_pGameStatus->m_levelState = m_levelScore;
		m_ai->m_score = m_levelScore;
		m_score = m_levelScore;
		break;
	case GAME_STATUS_RESTART:
		Restart();
		break;
	}

	if (m_ai->m_networkMode != 0 && g_pActiveConnection == 0) {
		NewPauseWindow(PAUSE_MSG_CONNECTION_LOST);
		m_connectionTimeoutActive = 1;
		m_connectionTimeoutStart = CurrentMilliTimer();
	}
	if (g_nTestAllLevels != 0) {
		switch ((int) m_levelTestFrame) {
		case 50:
		case 51:
		case 52:
			m_display->ToggleResolution();
			break;
		case 53:
			m_ai->GameState(GAME_STATUS_SUCCESS);
			break;
		}
		m_levelTestFrame++;
	}
}
