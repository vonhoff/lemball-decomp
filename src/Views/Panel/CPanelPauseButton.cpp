#include "CPanelPauseButton.h"

#include "../../AI/Navigation/CAi.h"
#include "../../Visos/Graphics/CCursor.h"
#include "../../Visos/Graphics/CDepressedButton.h"
#include "../Display/C2D.h"
#include "../Sound/CSoundView.h"
#include "CPanel.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Graphics/CBaseCursor.h"
#include "Visos/Graphics/CGraphicButton.h"
#include "Visos/Graphics/CPvButton.h"
#include "Visos/Graphics/CToggleButton.h"

// FUNCTION: LEMBALL 0x004421d0
CPanelPauseButton::CPanelPauseButton(CPanel* p_arg0,
									 const CVsPoint& p_arg1,
									 CPvGWnd* p_arg2,
									 unsigned long p_arg3,
									 unsigned long p_arg4)
	: CToggleButton(p_arg1, p_arg2, p_arg3, p_arg4)
{
	m_panel = p_arg0;
	m_pressedInside = 0;
	m_externalEnabled = 1;
}

// FUNCTION: LEMBALL 0x00442240
void CPanelPauseButton::OnInside(const CVsPoint& p_point)
{
	CursorChangeType(CURSOR_DISPLAY_HAND, m_pressedInside);
}

// FUNCTION: LEMBALL 0x00442260
void CPanelPauseButton::DrawButton()
{
	CGraphicButton::DrawButton();
}

// FUNCTION: LEMBALL 0x00442270
void CPanelPauseButton::OnPaint(const CVsRect& p_rect)
{
	reinterpret_cast<CDepressedButton*>(this)->CDepressedButton::OnPaint(p_rect);
}

// FUNCTION: LEMBALL 0x00442280
void CPanelPauseButton::OnPressed(int p_flags)
{
	if (p_flags == 0) {
		m_pressedInside = 1;
		CursorChangeType(CURSOR_DISPLAY_HAND, 1);
		g_pSoundView->m_pendingEffect = SFX_MOUSE_CLICK;
	}
}

// FUNCTION: LEMBALL 0x004422b0
void CPanelPauseButton::OnReleased(int p_flags)
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
		CursorChangeType(CURSOR_DISPLAY_HAND, 0);
	}
}

// FUNCTION: LEMBALL 0x00442350
void CPanelPauseButton::OnExternalButtonUp(const CVsPoint& p_point, int p_flags)
{
	CPvButton::OnExternalButtonUp(p_point, p_flags);
	if (p_flags == 0 && m_pressedInside != 0) {
		m_pressedInside = 0;
		CursorChangeType(CURSOR_DISPLAY_HAND, 0);
	}
}
