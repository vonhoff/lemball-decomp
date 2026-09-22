#include "CGunButton.h"

#include "../../Views/Sound/CSoundView.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x0044c1e0
int Sgn(int p_value)
{
	int res = p_value;
	if (res == 0) {
		return res;
	}
	if (res < 0) {
		return -1;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0044c200
void CGunButton::OnReleased(int p_flags)
{
	if (m_pressed != 0 && (p_flags == 0 || p_flags == 3)) {
		m_enabled = 1;
		return;
	}
	m_enabled = 0;
}

// FUNCTION: LEMBALL 0x0044c230
void CGunButton::OnPressed(int p_flags)
{
	if (m_pressed == 0 || (p_flags != 0 && p_flags != 3)) {
		m_enabled = 0;
	}
	else {
		m_enabled = 1;
	}
	if (p_flags == 0) {
		g_pSoundView->PlayEffect(SFX_DRUM1);
	}
}
