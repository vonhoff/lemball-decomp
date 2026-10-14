#include "CPanel.h"

#include "../Display/C2D.h"
#include "Application/SoundEffects.h"
#include "CPanelLemming.h"
#include "CPanelPauseButton.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Engine/Queues/CBaseQueueHandler.h"
#include "Engine/Queues/tagMESSAGE.h"
#include "Engine/Resources/ResourceCatalog.h"
#include "Engine/Resources/Types/CResANIM.h"
#include "Engine/Resources/Types/CResZRLE.h"
#include "GameView/Display/CMain2DDisplay.h"
#include "GameView/Sound/CSoundView.h"
#include "Gameplay/Simulation/CAI.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x00442f00
CVSPoint CPanel::GetPausePos()
{
	CVSPoint result;
	CVSSize size;
	int zoom = m_window->m_zoom;
	if (m_window->m_innerRect.m_width * m_window->m_innerRect.m_height != 0) {
		size.m_width = m_window->m_innerRect.m_width;
		size.m_height = m_window->m_innerRect.m_height;
	}
	else {
		size.m_width = m_window->m_rect.m_width;
		size.m_height = m_window->m_rect.m_height;
	}
	size.m_width /= zoom;
	size.m_height /= zoom;
	result.m_y = size.m_height - m_pauseSize.m_height;
	result.m_x = (size.m_width - m_panelSize.m_width) / 2;
	return result;
}

// FUNCTION: LEMBALL 0x00442f80
CPanel::CPanel(C2D* p_gameView) : CBaseQueueHandler()
{
	int i;
	m_game = p_gameView;
	m_window = p_gameView->m_display;
	m_ai = p_gameView->m_ai;
	m_resources[0] = CResANIM::Load(RES_GAME_BUTPAWS);
	m_resources[1] = CResANIM::Load(RES_GAME_BUTAMMO);
	m_resources[2] = CResANIM::Load(RES_GAME_BUTLEMMING);
	m_resources[3] = CResANIM::Load(RES_GAME_BUTBALLOON);

	CVSSize* size = &m_resources[1]->m_animationEntries[0].m_size;
	m_ammoButtonSize.m_width = size->m_width;
	m_ammoButtonSize.m_height = size->m_height;
	size = &m_resources[2]->m_animationEntries[0].m_size;
	m_lemmingButtonSize.m_width = size->m_width;
	m_lemmingButtonSize.m_height = size->m_height;
	size = &m_resources[0]->m_animationEntries[0].m_size;
	m_pauseSize.m_width = size->m_width;
	m_pauseSize.m_height = size->m_height;
	m_panelSize.m_width = m_pauseSize.m_width;
	m_panelSize.m_height = m_pauseSize.m_height;
	m_panelSize.m_width = m_panelSize.m_width + (m_lemmingButtonSize.m_width + m_ammoButtonSize.m_width) * 4;
	const CVSPoint& calculated = GetPausePos();
	short x = calculated.m_x;
	m_panelPosition.m_x = x;
	short y = calculated.m_y;
	m_panelPosition.m_y = y;
	CVSPoint position(x, y);
	m_pauseButton = new CPanelPauseButton(this, position, m_window, RES_GAME_BUTPAWS, 3);

	position.m_x = position.m_x + m_pauseSize.m_width;
	i = 0;
	do {
		m_lemmings[i] = new CPanelLemming(m_ai->m_networkLemmings[i], position, this);
		i++;
	} while (i < 4);
	g_pMasterInputQueue->Attach(this, 0);
}

// FUNCTION: LEMBALL 0x00443140
CPanel::~CPanel()
{
	g_pMasterInputQueue->Detach(this, 0);
	for (int i = 0; i < 4; i++) {
		delete m_lemmings[i];
	}
	m_resources[3]->UnLoad();
	m_resources[2]->UnLoad();
	m_resources[1]->UnLoad();
	m_resources[0]->UnLoad();
	if (m_pauseButton != NULL) {
		delete m_pauseButton;
	}
}

// FUNCTION: LEMBALL 0x004431c0
void CPanel::RefreshLemmings()
{
	for (int i = 0; i < 4; i++) {
		m_lemmings[i]->m_lemming = m_ai->m_networkLemmings[i];
	}
}

// FUNCTION: LEMBALL 0x004431f0
void CPanel::OnSize()
{
	const CVSPoint& calculated = GetPausePos();
	short x = calculated.m_x;
	m_panelPosition.m_x = x;
	short y = calculated.m_y;
	m_panelPosition.m_y = y;
	CVSPoint position(x, y);
	m_pauseButton->Move(position);
	position.m_x += m_pauseSize.m_width;
	CPanelLemming** lemming = m_lemmings;
	int count = 4;
	do {
		(*lemming)->Move(position);
		lemming++;
		count--;
	} while (count != 0);
}

// FUNCTION: LEMBALL 0x00443250
void CPanel::Process()
{
	int count;
	CPanelLemming** lemming = m_lemmings;
	count = 4;

	do {
		(*lemming)->UpdateStatus();
		lemming++;
		count--;
	} while (count != 0);
}

// FUNCTION: LEMBALL 0x00443270
void CPanel::SetPause(unsigned int p_paused)
{
	m_game->SetPause(p_paused);
	CPanelPauseButton* pauseButton = m_pauseButton;
	unsigned int paused = m_game->m_paused;
	pauseButton->m_toggled = paused;
	pauseButton->m_enabled = paused;
}

enum ePanelKeyAction {
	PANEL_KEY_ACTION_NONE = 0,
	PANEL_KEY_ACTION_PAUSE = 8
};

// FUNCTION: LEMBALL 0x004432a0
unsigned long CPanel::TranslateKey(unsigned long p_key)
{
	switch (p_key) {
	case INPUT_KEY_P:
		return PANEL_KEY_ACTION_PAUSE;
	default:
		return PANEL_KEY_ACTION_NONE;
	}
}

// FUNCTION: LEMBALL 0x004432c0
int CPanel::ProcessMsg(tagMESSAGE* p_message)
{
	if (m_game->m_paused == 0 && m_game->m_ai->m_gameStatus != GAME_STATUS_PAUSED) {
		unsigned int type = p_message->m_type;
		switch (type) {
		case MESSAGE_KEY_DOWN:
			if (TranslateKey(p_message->m_code) == PANEL_KEY_ACTION_PAUSE) {
				g_pSoundView->m_pendingEffect = SFX_MOUSE_CLICK;
				unsigned int pause = m_game->m_paused == 0;
				m_game->TriggerPause(pause);
				CPanelPauseButton* pauseButton = m_pauseButton;
				unsigned int paused = m_game->m_paused;
				pauseButton->m_toggled = paused;
				pauseButton->m_enabled = paused;
				return 1;
			}
			break;
		}
		return 0;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00443360
bool CPanel::MouseInPanel(const CVSPoint& p_point)
{
	short panelWidth = m_panelSize.m_width;
	short panelX = m_panelPosition.m_x;
	short panelHeight = m_panelSize.m_height;
	short panelY = m_panelPosition.m_y;
	if (panelX <= p_point.m_x && (short) (panelWidth + panelX) > p_point.m_x && panelY <= p_point.m_y &&
		(short) (panelHeight + panelY) > p_point.m_y) {
		return true;
	}
	return false;
}
