#include "CPanelPauseButton.h"

#include "../../AI/Navigation/CAI.h"
#include "Platform/Windows/Graphics/CCursor.h"
#include "Visos/Controls/CDepressedButton.h"
#include "../Display/C2D.h"
#include "../Sound/CSoundView.h"
#include "CPanel.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Queues/Message.h"
#include "Visos/Input/CBaseCursor.h"
#include "Visos/Controls/CGraphicButton.h"
#include "Visos/Controls/CPVButton.h"
#include "Visos/Controls/CToggleButton.h"

// FUNCTION: LEMBALL 0x004421d0
CPanelPauseButton::CPanelPauseButton(CPanel* p_panel,
									 const CVSPoint& p_position,
									 CPVGWnd* p_parent,
									 unsigned long p_animId,
									 unsigned long p_flags)
	: CToggleButton(p_position, p_parent, p_animId, p_flags)
{
	m_panel = p_panel;
	m_pressedInside = 0;
	m_externalEnabled = 1;
}

// FUNCTION: LEMBALL 0x00442240
void CPanelPauseButton::OnInside(const CVSPoint& p_point)
{
	CursorChangeType(CURSOR_DISPLAY_HAND, m_pressedInside);
}

// FUNCTION: LEMBALL 0x00442260
void CPanelPauseButton::DrawButton()
{
	CGraphicButton::DrawButton();
}

// FUNCTION: LEMBALL 0x00442270
void CPanelPauseButton::OnPaint(const CVSRect& p_rect)
{
	CDepressedButton::OnPaint(p_rect);
}

// FUNCTION: LEMBALL 0x00442280
void CPanelPauseButton::OnPressed(int p_flags)
{
	if (p_flags == MOUSE_BUTTON_INDEX_LEFT) {
		m_pressedInside = 1;
		CursorChangeType(CURSOR_DISPLAY_HAND, 1);
		g_pSoundView->m_pendingEffect = SFX_MOUSE_CLICK;
	}
}

// FUNCTION: LEMBALL 0x004422b0
void CPanelPauseButton::OnReleased(int p_flags)
{
	if (p_flags == MOUSE_BUTTON_INDEX_LEFT) {
		unsigned int paused = m_toggled ^ 1;
		m_toggled = paused;
		m_enabled = paused;

		if ((paused == 0 && m_panel->m_game->GetPauser() != 0) ||
			(paused != 0 && m_panel->m_game->m_ai->m_gameStatus != GAME_STATUS_PAUSED)) {
			m_panel->m_game->TriggerPause(paused);
		}

		m_toggled = m_panel->m_game->m_paused;
		m_enabled = m_toggled;
		m_pressedInside = 0;
		CursorChangeType(CURSOR_DISPLAY_HAND, 0);
	}
}

// FUNCTION: LEMBALL 0x00442350
void CPanelPauseButton::OnExternalButtonUp(const CVSPoint& p_point, int p_flags)
{
	CPVButton::OnExternalButtonUp(p_point, p_flags);
	if (p_flags == MOUSE_BUTTON_INDEX_LEFT && m_pressedInside != 0) {
		m_pressedInside = 0;
		CursorChangeType(CURSOR_DISPLAY_HAND, 0);
	}
}
