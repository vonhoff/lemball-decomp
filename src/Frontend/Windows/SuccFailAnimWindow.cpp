#include "SuccFailAnimWindow.h"

#include "../../Control/Game/Game.h"
#include "../../Frontend/Base/BaseFrontendProcess.h"
#include "../../Views/Sound/SoundView.h"
#include "../../Visos/Resources/Manifest.h"

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
		m_primaryAnimBase = RES_NEWFRONT_STRINGS_AVINAMES_LORES_SUCCESS_SUCCESS1;
		m_secondaryAnimBase = RES_NEWFRONT_STRINGS_AVINAMES_HIRES_SUCCESS_SUCCESS1;
		sample = *g_pSentinel * 0x29 + 0x1f & 0x7fffff;
		*g_pSentinel = sample;
		m_variantIndex = sample % 3;
		return sample / 3;
	}
	m_primaryAnimBase = RES_NEWFRONT_STRINGS_AVINAMES_LORES_FAIL_FAIL1;
	m_secondaryAnimBase = RES_NEWFRONT_STRINGS_AVINAMES_HIRES_FAIL_FAIL1;
	sample = *g_pSentinel * 0x29 + 0x1f & 0x7fffff;
	*g_pSentinel = sample;
	m_variantIndex = sample % 3;
	return sample / 3;
}

// FUNCTION: LEMBALL 0x00450d00
void SuccFailAnimWindow::SetVariant(int p_variant)
{
	unsigned int animBase;

	if (p_variant != 0) {
		animBase = m_primaryAnimBase;
	}
	else {
		animBase = m_secondaryAnimBase;
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
