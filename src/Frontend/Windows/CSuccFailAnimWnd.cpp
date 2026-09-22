#include "CSuccFailAnimWnd.h"

#include "../../Control/Game/CGame.h"
#include "../../Frontend/Base/CBaseFrontendProcess.h"
#include "../../Views/Sound/CSoundView.h"
#include "../../Visos/Resources/Manifest.h"
#include "Visos/Graphics/CAnimWnd.h"

// FUNCTION: LEMBALL 0x00450c40
void CSuccFailAnimWnd::Initialise(CSuccFailDrawer* p_owner, CMain2DDisplay* p_display, int p_success)
{
	int sample;

	m_owner = p_owner;
	m_display = p_display;
	m_success = p_success;
	m_musicStarted = 0;
	m_variantIndex = 0;
	if (p_success != 0) {
		m_lowResAnimBase = RES_NEWFRONT_STRINGS_AVINAMES_LORES_SUCCESS_SUCCESS1;
		m_highResAnimBase = RES_NEWFRONT_STRINGS_AVINAMES_HIRES_SUCCESS_SUCCESS1;
		sample = *g_pSentinel * 0x29 + 0x1f & 0x7fffff;
		*g_pSentinel = sample;
		m_variantIndex = sample % 3;
		return;
	}
	m_lowResAnimBase = RES_NEWFRONT_STRINGS_AVINAMES_LORES_FAIL_FAIL1;
	m_highResAnimBase = RES_NEWFRONT_STRINGS_AVINAMES_HIRES_FAIL_FAIL1;
	sample = *g_pSentinel * 0x29 + 0x1f & 0x7fffff;
	*g_pSentinel = sample;
	m_variantIndex = sample % 3;
}

// FUNCTION: LEMBALL 0x00450d00
void CSuccFailAnimWnd::SetVariant(int p_lowResolution)
{
	unsigned int animBase;

	if (p_lowResolution != 0) {
		animBase = m_lowResAnimBase;
	}
	else {
		animBase = m_highResAnimBase;
	}
	CAnimWnd::SetAnim(m_variantIndex + animBase);
}

// FUNCTION: LEMBALL 0x00450d30
void CSuccFailAnimWnd::OnStop()
{
	if (g_nAnimationsDisabled == 0) {
		Play();
	}
	if (m_musicStarted == 0) {
		g_pSoundView->SetMusicOn(1);
		m_musicStarted = 1;
	}
}

// FUNCTION: LEMBALL 0x004510d0
unsigned int CSuccFailAnimWnd::GetStyle()
{
	return 0x40000001;
}
