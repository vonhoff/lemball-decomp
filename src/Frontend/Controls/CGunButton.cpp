#include "CGunButton.h"

#include "Application/SoundEffects.h"
#include "Engine/Queues/tagMESSAGE.h"
#include "GameView/Sound/CSoundView.h"

// FUNCTION: LEMBALL 0x0044c200
void CGunButton::OnReleased(BUTTON_FLAGS p_flags)
{
	if (m_pressed != 0 && (p_flags == MOUSE_BUTTON_INDEX_LEFT || p_flags == MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK)) {
		m_enabled = true;
		return;
	}
	m_enabled = false;
}

// FUNCTION: LEMBALL 0x0044c230
void CGunButton::OnPressed(BUTTON_FLAGS p_flags)
{
	if (m_pressed == 0 || (p_flags != MOUSE_BUTTON_INDEX_LEFT && p_flags != MOUSE_BUTTON_INDEX_LEFT_DOUBLE_CLICK)) {
		m_enabled = false;
	}
	else {
		m_enabled = true;
	}
	if (p_flags == MOUSE_BUTTON_INDEX_LEFT) {
		g_pSoundView->PlayEffect(SFX_DRUM1);
	}
}
