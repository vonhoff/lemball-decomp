#include "PanelPauseButton.h"

#include "../../AI/Navigation/Ai.h"
#include "../../Visos/Graphics/Cursor.h"
#include "../Display/C2D.h"
#include "../Sound/SoundView.h"
#include "Panel.h"

// 68K 0x10b0c502 __ct__17CPanelPauseButtonFP6CPanelRC8CVSPointP7CPVGWndUlUl
// STUB: LEMBALL 0x004421d0
PanelPauseButton::PanelPauseButton(Panel* p_arg0,
								   const VsPoint& p_arg1,
								   PvGWnd* p_arg2,
								   unsigned long p_arg3,
								   unsigned long p_arg4)
	: ToggleButton(p_arg1, p_arg2, p_arg3, p_arg4)
{
	m_panel = p_arg0;
	m_pressedInside = 0;
	m_externalEnabled = 1;
}

// 68K 0x10b0c5b8 OnInside__17CPanelPauseButtonFRC8CVSPoint
// FUNCTION: LEMBALL 0x00442240
void PanelPauseButton::OnInside(const VsPoint& p_point)
{
	CursorChangeType((eCursorDisplayType) 1, m_pressedInside);
}

// 68K 0x10b0c604 DrawButton__17CPanelPauseButtonFv
// FUNCTION: LEMBALL 0x00442260
void PanelPauseButton::DrawButton()
{
	GraphicButton::DrawButton();
}

// 68K 0x10b0c63c OnPaint__17CPanelPauseButtonFRC7CVSRect
// FUNCTION: LEMBALL 0x00442270
void PanelPauseButton::OnPaint(const VsRect& p_rect)
{
	GraphicButton::OnPaint(p_rect);
}

// 68K 0x10b0c67e OnPressed__17CPanelPauseButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00442280
void PanelPauseButton::OnPressed(int p_flags)
{
	if (p_flags == 0) {
		m_pressedInside = 1;
		CursorChangeType((eCursorDisplayType) 1, 1);
		g_pSoundView->m_pendingEffect = (eSoundEffect) 3;
	}
}

// 68K 0x10b0c6ec OnReleased__17CPanelPauseButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x004422b0
void PanelPauseButton::OnReleased(int p_flags)
{
	if (p_flags == 0) {
		unsigned int paused = m_toggled ^ 1;
		m_toggled = paused;
		m_enabled = paused;

		if ((paused == 0 && m_panel->m_game->GetPauser() != 0) ||
			(paused != 0 && m_panel->m_game->m_ai->m_gameStatus != 1)) {
			m_panel->m_game->TriggerPause((unsigned char) paused);
		}

		m_toggled = m_panel->m_game->m_paused;
		m_enabled = m_toggled;
		m_pressedInside = 0;
		CursorChangeType((eCursorDisplayType) 1, 0);
	}
}

// 68K 0x10b0c7c6 OnExternalButtonUp__17CPanelPauseButtonFRC8CVSPoint12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x00442350
void PanelPauseButton::OnExternalButtonUp(const VsPoint& p_point, int p_flags)
{
	PvButton::OnExternalButtonUp(p_point, p_flags);
	if (p_flags == 0 && m_pressedInside != 0) {
		m_pressedInside = 0;
		CursorChangeType((eCursorDisplayType) 1, 0);
	}
}

// 68K 0x1011cd5e __dt__17CPanelPauseButtonFv
PanelPauseButton::~PanelPauseButton()
{
}
