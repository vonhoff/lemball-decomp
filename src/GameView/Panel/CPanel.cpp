#include "CPanel.h"

#include "Gameplay/Simulation/CAI.h"
#include "Engine/Queues/CBaseQueue.h"
#include "Platform/Windows/Windowing/CPVGWnd.h"
#include "Engine/Resources/Types/CResANIM.h"
#include "../Display/C2D.h"
#include "../Sound/CSoundView.h"
#include "CPanelLemming.h"
#include "CPanelPauseButton.h"
#include "Application/SoundEffects.h"
#include "Engine/Queues/CBaseQueueHandler.h"
#include "Engine/Math/CVSPoint.h"
#include "Engine/Math/CVSRect.h"
#include "Engine/Queues/Message.h"
#include "Engine/Resources/Types/CResZRLE.h"
#include "Engine/Resources/Manifest.h"

#include <new.h>

// FUNCTION: LEMBALL 0x00442f00
CVSPoint CPanel::GetPausePos()
{
	CVSPoint result;
	short width = m_window->m_innerRect.m_width;
	short height = m_window->m_innerRect.m_height;
	int zoom = (int) m_window->m_zoom;
	if (width * height == 0) {
		width = m_window->m_rect.m_width;
		height = m_window->m_rect.m_height;
	}
	width = (short) ((int) width / zoom);
	result.m_y = (short) ((int) height / zoom - (int) m_pauseSize.m_y);
	result.m_x = (short) (((int) width - (int) m_panelSize.m_x) / 2);
	return result;
}

// FUNCTION: LEMBALL 0x00442f80
CPanel::CPanel(C2D* p_gameView) : CBaseQueueHandler()
{
	int i;
	CPanelLemming** lemming;
	m_game = p_gameView;
	m_window = (CPVGWnd*) p_gameView->m_display;
	m_ai = p_gameView->m_ai;
	m_resources[0] = CResANIM::Load(RES_GAME_BUTPAWS);
	m_resources[1] = CResANIM::Load(RES_GAME_BUTAMMO);
	m_resources[2] = CResANIM::Load(RES_GAME_BUTLEMMING);
	m_resources[3] = CResANIM::Load(RES_GAME_BUTBALLOON);

	CVSSize* size = (CVSSize*) &m_resources[1]->m_animationEntries[0].m_width;
	m_ammoButtonSize.m_x = size->m_width;
	m_ammoButtonSize.m_y = size->m_height;
	size = (CVSSize*) &m_resources[2]->m_animationEntries[0].m_width;
	m_lemmingButtonSize.m_x = size->m_width;
	m_lemmingButtonSize.m_y = size->m_height;
	size = (CVSSize*) &m_resources[0]->m_animationEntries[0].m_width;
	m_pauseSize.m_x = size->m_width;
	m_pauseSize.m_y = size->m_height;
	m_panelSize.m_x = m_pauseSize.m_x;
	m_panelSize.m_y = m_pauseSize.m_y;
	m_panelSize.m_x = (short) (m_panelSize.m_x + (m_lemmingButtonSize.m_x + m_ammoButtonSize.m_x) * 4);
	CVSPoint calculated = GetPausePos();
	short x = calculated.m_x;
	m_panelPosition.m_x = x;
	short y = calculated.m_y;
	m_panelPosition.m_y = y;
	CVSPoint position(x, y);
	void* storage = operator new(sizeof(CPanelPauseButton));
	if (storage != NULL) {
		m_pauseButton = new (storage) CPanelPauseButton(this, position, m_window, RES_GAME_BUTPAWS, 3);
	}
	else {
		m_pauseButton = NULL;
	}

	position.m_x = position.m_x + m_pauseSize.m_x;
	i = 0;
	lemming = m_lemmings;
	do {
		storage = operator new(sizeof(CPanelLemming));
		if (storage != NULL) {
			*lemming = new (storage) CPanelLemming(m_ai->m_networkLemmings[i], position, this);
		}
		else {
			*lemming = NULL;
		}
		i++;
		lemming++;
	} while (i < 4);
	g_pMasterInputQueue->Attach(this, 0);
}

// FUNCTION: LEMBALL 0x00443140
CPanel::~CPanel()
{
	CPanelLemming** lemming;
	int count;
	lemming = m_lemmings;
	g_pMasterInputQueue->Detach(this, 0);
	count = 4;
	do {
		delete *lemming;
		lemming++;
	} while (--count != 0);
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
	CVSPoint calculated = GetPausePos();
	short x = calculated.m_x;
	m_panelPosition.m_x = x;
	short y = calculated.m_y;
	m_panelPosition.m_y = y;
	CVSPoint position(x, y);
	m_pauseButton->Move(position);
	position.m_x += m_pauseSize.m_x;
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
	CPanelLemming** lemming = m_lemmings;
	int count = 4;

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
int CPanel::ProcessMsg(Message* p_message)
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
	short panelWidth = m_panelSize.m_x;
	short panelX = m_panelPosition.m_x;
	short panelHeight = m_panelSize.m_y;
	short panelY = m_panelPosition.m_y;
	if (panelX <= p_point.m_x && (short) (panelWidth + panelX) > p_point.m_x && panelY <= p_point.m_y &&
		(short) (panelHeight + panelY) > p_point.m_y) {
		return true;
	}
	return false;
}
