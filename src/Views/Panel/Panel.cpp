#include "Panel.h"

#include "../../AI/Navigation/Ai.h"
#include "../../Visos/Foundation/BaseQueue.h"
#include "../../Visos/Graphics/PvGWnd.h"
#include "../../Visos/Resources/ResAnim.h"
#include "../Display/C2D.h"
#include "../Sound/SoundView.h"
#include "PanelLemming.h"
#include "PanelPauseButton.h"

#include <new.h>

// 68K 0x10b0d942 GetPausePos__6CPanelFv
// FUNCTION: LEMBALL 0x00442f00
VsPoint* Panel::GetPausePos(VsPoint* p_result)
{
	short width = m_window->m_innerRect.m_width;
	short height = m_window->m_innerRect.m_height;
	if (width * height == 0) {
		width = m_window->m_rect.m_width;
		height = m_window->m_rect.m_height;
	}
	p_result->m_x = (short) (((int) width / (int) m_window->m_zoom - (int) m_panelSize.m_x) / 2);
	p_result->m_y = (short) ((int) height / (int) m_window->m_zoom - (int) m_pauseSize.m_y);
	return p_result;
}

// 68K 0x10b0da4a __ct__6CPanelFP3C2D
// FUNCTION: LEMBALL 0x00442f80
Panel::Panel(C2D* p_arg0) : BaseQueueHandler()
{
	m_buttonSize.m_x = 0;
	m_buttonSize.m_y = 0;
	m_balloonSize.m_x = 0;
	m_balloonSize.m_y = 0;
	m_pauseSize.m_x = 0;
	m_pauseSize.m_y = 0;
	m_panelSize.m_x = 0;
	m_panelSize.m_y = 0;
	m_panelPosition.m_x = 0;
	m_panelPosition.m_y = 0;
	m_game = p_arg0;
	m_window = (PvGWnd*) p_arg0->m_display;
	m_ai = p_arg0->m_ai;
	m_resources[0] = ResAnim::Load(0x2d);
	m_resources[1] = ResAnim::Load(0x2a);
	m_resources[2] = ResAnim::Load(0x2c);
	m_resources[3] = ResAnim::Load(0x2b);

	m_buttonSize.m_x = m_resources[1]->m_animationEntries[0].m_width;
	m_buttonSize.m_y = m_resources[1]->m_animationEntries[0].m_height;
	m_balloonSize.m_x = m_resources[2]->m_animationEntries[0].m_width;
	m_balloonSize.m_y = m_resources[2]->m_animationEntries[0].m_height;
	m_pauseSize.m_x = m_resources[0]->m_animationEntries[0].m_width;
	m_pauseSize.m_y = m_resources[0]->m_animationEntries[0].m_height;
	m_panelSize.m_x = m_pauseSize.m_x;
	m_panelSize.m_y = m_pauseSize.m_y;
	m_panelSize.m_x = (short) (m_panelSize.m_x + (m_balloonSize.m_x + m_buttonSize.m_x) * 4);
	VsPoint position;
	GetPausePos(&position);
	m_panelPosition.m_x = position.m_x;
	m_panelPosition.m_y = position.m_y;
	void* storage = operator new(0x13c);
	if (storage != 0) {
		m_pauseButton = new (storage) PanelPauseButton(this, position, m_window, 0x2d, 3);
	}
	else {
		m_pauseButton = 0;
	}

	position.m_x = position.m_x + m_pauseSize.m_x;
	PanelLemming** lemming = m_lemmings;
	for (int i = 0; i < 4; i++) {
		storage = operator new(0x2c);
		if (storage != 0) {
			*lemming = new (storage) PanelLemming(m_ai->m_networkLemmings[i], position, this);
		}
		else {
			*lemming = 0;
		}
		lemming++;
	}
	g_pMasterInputQueue->Attach(this, 0);
}

// 68K 0x10b0dc46 __dt__6CPanelFv
// FUNCTION: LEMBALL 0x00443140
Panel::~Panel()
{
	PanelLemming** lemming;
	int count;
	lemming = m_lemmings;
	count = 4;
	g_pMasterInputQueue->Detach(this, 0);
	do {
		delete *lemming;
		lemming++;
	} while (--count != 0);
	m_resources[3]->UnLoad();
	m_resources[2]->UnLoad();
	m_resources[1]->UnLoad();
	m_resources[0]->UnLoad();
	if (m_pauseButton != 0) {
		delete m_pauseButton;
	}
}

// 68K 0x10b0dd10 OnSize__6CPanelFv
// FUNCTION: LEMBALL 0x004431f0
void Panel::OnSize()
{
	VsPoint calculatedPosition;
	VsPoint position;
	VsPoint* calculated = GetPausePos(&calculatedPosition);
	position.m_x = m_panelPosition.m_x = calculated->m_x;
	position.m_y = m_panelPosition.m_y = calculated->m_y;
	m_pauseButton->Move(position);
	position.m_x += m_pauseSize.m_x;
	PanelLemming** lemming = m_lemmings;
	int count = 4;
	do {
		(*lemming)->Move(position);
		lemming++;
		count--;
	} while (count != 0);
}

// 68K 0x10b0dd98 Process__6CPanelFv
// FUNCTION: LEMBALL 0x00443250
void Panel::Process()
{
	PanelLemming** lemming = m_lemmings;
	int count = 4;

	do {
		(*lemming)->UpdateStatus();
		lemming++;
		count--;
	} while (count != 0);
}

// 68K 0x10b0ddd8 SetPause__6CPanelFUc
// FUNCTION: LEMBALL 0x00443270
void Panel::SetPause(unsigned char p_paused)
{
	m_game->SetPause(p_paused);
	PanelPauseButton* pauseButton = m_pauseButton;
	unsigned int paused = m_game->m_paused;
	pauseButton->m_toggled = paused;
	pauseButton->m_enabled = paused;
}

// 68K 0x10b0de2c TranslateKey__6CPanelFUl
// FUNCTION: LEMBALL 0x004432a0
unsigned long Panel::TranslateKey(unsigned long p_key)
{
	switch (p_key) {
	case 0x14:
		return 8;
	default:
		return 0;
	}
}

// 68K 0x10b0de64 ProcessMsg__6CPanelFP10tagMESSAGE
// FUNCTION: LEMBALL 0x004432c0
int Panel::ProcessMsg(Message* p_message)
{
	if (m_game->m_paused == 0 && m_game->m_ai->m_gameStatus != 1) {
		unsigned int type = p_message->type;
		switch (type) {
		case 4:
			if (TranslateKey(p_message->code) == 8) {
				g_pSoundView->m_pendingEffect = (eSoundEffect) 3;
				unsigned int pause = m_game->m_paused == 0;
				m_game->TriggerPause((unsigned char) pause);
				PanelPauseButton* pauseButton = m_pauseButton;
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

// 68K 0x10b0df46 MouseInPanel__6CPanelFRC8CVSPoint
// FUNCTION: LEMBALL 0x00443360
bool Panel::MouseInPanel(const VsPoint& p_point)
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
