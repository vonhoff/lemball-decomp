#include "CGunButton.h"

#include "GameView/Sound/CSoundView.h"
#include "Application/SoundEffects.h"
#include "Engine/Queues/Message.h"

// FUNCTION: LEMBALL 0x0044c200
void CGunButton::OnReleased(eMouseButtonIndex p_flags)
{
	if (m_pressed != 0 && (p_flags == MOUSE_BUTTON_INDEX_LEFT || p_flags == MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK)) {
		m_enabled = 1;
		return;
	}
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x0044c230
void CGunButton::OnPressed(eMouseButtonIndex p_flags)
{
	if (m_pressed == 0 || (p_flags != MOUSE_BUTTON_INDEX_LEFT && p_flags != MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK)) {
		m_enabled = 0;
	}
	else {
		m_enabled = 1;
	}
	if (p_flags == MOUSE_BUTTON_INDEX_LEFT) {
		g_pSoundView->PlayEffect(SFX_DRUM1);
	}
}
