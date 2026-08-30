#include "GunButton.h"

#include "../../Views/Sound/SoundView.h"
#include "GunButtons.h"

GunButton::GunButton(const VsPoint& p_position, PvGWnd* p_parent, unsigned long p_animId, unsigned long p_flags)
	: GraphicButton(p_position, p_parent, p_animId, p_flags)
{
}

// 68K 0x10802938 sgn__Fi
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

// 68K 0x1080295e OnReleased__10CGunButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x0044c200
void GunButton::OnReleased(int p_flags)
{
	if (m_pressed != 0 && (p_flags == 0 || p_flags == 3)) {
		m_enabled = 1;
		return;
	}
	m_enabled = 0;
}

// 68K 0x108029bc OnPressed__10CGunButtonF12BUTTON_FLAGS
// FUNCTION: LEMBALL 0x0044c230
void GunButton::OnPressed(int p_flags)
{
	if (m_pressed == 0 || (p_flags != 0 && p_flags != 3)) {
		m_enabled = 0;
	}
	else {
		m_enabled = 1;
	}
	if (p_flags == 0) {
		g_pSoundView->PlayEffect((eSoundEffect) 0x25);
	}
}

// 68K 0x1011c1a2 __dt__10CGunButtonFv
GunButton::~GunButton()
{
}
