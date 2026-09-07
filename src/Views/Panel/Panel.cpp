#include "Panel.h"

#include "../../AI/Navigation/Ai.h"
#include "../../Visos/Graphics/PvGWnd.h"
#include "../Display/C2D.h"
#include "../Sound/SoundView.h"
#include "PanelLemming.h"
#include "PanelPauseButton.h"

// 68K 0x10b0d942 GetPausePos__6CPanelFv
// FUNCTION: LEMBALL 0x00442f00
VsPoint Panel::GetPausePos()
{
	short width = m_window->m_innerRect.m_width;
	short height = m_window->m_innerRect.m_height;
	if (width * height == 0) {
		width = m_window->m_rect.m_width;
		height = m_window->m_rect.m_height;
	}
	VsPoint result;
	result.m_x = (short) (((int) width / (int) m_window->m_zoom - (int) m_panelSize.m_x) / 2);
	result.m_y = (short) ((int) height / (int) m_window->m_zoom - (int) m_pauseSize.m_y);
	return result;
}

// 68K 0x10b0da4a __ct__6CPanelFP3C2D
// STUB: LEMBALL 0x00442f80
Panel::Panel(C2D* p_arg0)
{
}

// 68K 0x10b0dc46 __dt__6CPanelFv
// STUB: LEMBALL 0x00443140
Panel::~Panel()
{
}

// 68K 0x10b0dd10 OnSize__6CPanelFv
// STUB: LEMBALL 0x004431f0
void Panel::OnSize()
{
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
		if (type == 4 && TranslateKey(p_message->code) == 8) {
			g_pSoundView->m_pendingEffect = (eSoundEffect) 3;
			unsigned int pause = m_game->m_paused == 0;
			m_game->TriggerPause((unsigned char) pause);
			PanelPauseButton* pauseButton = m_pauseButton;
			unsigned int paused = m_game->m_paused;
			pauseButton->m_toggled = paused;
			pauseButton->m_enabled = paused;
			return 1;
		}
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
	if (panelX <= p_point.m_x && p_point.m_x < (short) (panelX + panelWidth) && panelY <= p_point.m_y &&
		p_point.m_y < (short) (panelY + panelHeight)) {
		return true;
	}
	return false;
}
