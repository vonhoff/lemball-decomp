#include "SuccFailAnimWindow.h"

#include "../../Control/Game/Game.h"
#include "../../Frontend/Base/BaseFrontendProcess.h"
#include "../../Views/Sound/SoundView.h"

// FUNCTION: LEMBALL 0x00450c40
unsigned int SuccFailAnimWindow::Initialise(SuccFailDrawer* p_owner, Main2DDisplay* p_display, int p_variant)
{
	unsigned int sample;

	m_owner = p_owner;
	m_display = p_display;
	m_variant = p_variant;
	m_state = 0;
	m_variantIndex = 0;
	if (p_variant != 0) {
		m_primaryAnimBase = 0x11e;
		m_secondaryAnimBase = 0x126;
		sample = *g_pSentinel * 0x29 + 0x1f & 0x7fffff;
		*g_pSentinel = sample;
		m_variantIndex = sample % 3;
		return sample / 3;
	}
	m_primaryAnimBase = 0x121;
	m_secondaryAnimBase = 0x129;
	sample = *g_pSentinel * 0x29 + 0x1f & 0x7fffff;
	*g_pSentinel = sample;
	m_variantIndex = sample % 3;
	return sample / 3;
}

// FUNCTION: LEMBALL 0x00450d00
void SuccFailAnimWindow::SetVariant(int p_variant)
{
	unsigned int animBase;

	if (p_variant == 0) {
		animBase = m_secondaryAnimBase;
	}
	else {
		animBase = m_primaryAnimBase;
	}
	AnimWnd::SetAnim(m_variantIndex + animBase);
}

// FUNCTION: LEMBALL 0x00450d30
void SuccFailAnimWindow::OnStop()
{
	if (g_nAnimationsDisabled == 0) {
		Play();
	}
	if (m_state == 0) {
		g_pSoundView->SetMusicOn(1);
		m_state = 1;
	}
}

// FUNCTION: LEMBALL 0x004510d0
unsigned int SuccFailAnimWindow::GetStyle()
{
	return 0x40000001;
}
