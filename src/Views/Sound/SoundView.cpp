#include "SoundView.h"
#include "../../Frontend/Base/BaseFrontendProcess.h"
#include "../../Visos/Sound/SoundManager.h"

// GLOBAL: LEMBALL 0x0049ed98
unsigned int g_dwEffectsOn = 0;

// GLOBAL: LEMBALL 0x0049eb80
SoundView* g_pSoundView = 0;

// 68K 0x10b0fb42 __ct__10CSoundViewFv
// STUB: LEMBALL 0x00439a70
SoundView::SoundView()
{
}

// 68K 0x10b0fbd4 __dt__10CSoundViewFv
// STUB: LEMBALL 0x00439b30
SoundView::~SoundView()
{
}

// 68K 0x10b0fc30 SetEffectsOn__10CSoundViewFUc
// FUNCTION: LEMBALL 0x00439b50
void SoundView::SetEffectsOn(unsigned int p_enabled)
{
	if (g_nEffectsAvailable != 0) {
		g_dwEffectsOn = p_enabled;
	}
}

// 68K 0x10b0fc68 PlayEffect__10CSoundViewF12eSoundEffect
// FUNCTION: LEMBALL 0x00439b70
void SoundView::PlayEffect(eSoundEffect p_soundId)
{
	if (g_dwEffectsOn != 0) {
		g_pSoundManager->PlayEffect(m_effectSlots[p_soundId].m_handle);
	}
}

// 68K 0x10b0fcd2 SetMusicOn__10CSoundViewFUc
// STUB: LEMBALL 0x00439ba0
void SoundView::SetMusicOn(unsigned char p_enabled)
{
}

// 68K 0x10b0fd8a SoundEffect__10CSoundViewFP9CViewDataiR7AICOORD
// STUB: LEMBALL 0x00439c40
void SoundView::SoundEffect(ViewData* p_viewData, int p_count, AiCoord& p_listener)
{
}

// 68K 0x10b0ff0a UnprepareEffects__10CSoundViewFv
// STUB: LEMBALL 0x00439d60
void SoundView::UnprepareEffects()
{
}

// 68K 0x10b0ff80 PrepareEffects__10CSoundViewFUs
// STUB: LEMBALL 0x00439d90
void SoundView::PrepareEffects(unsigned short p_stateMask)
{
}

// 68K 0x10b10068 GetnEffects__10CSoundViewFUs
// STUB: LEMBALL 0x00439df0
int SoundView::GetnEffects(unsigned short p_stateMask)
{
	return 0;
}

// 68K 0x10b100d0 ChangeState__10CSoundViewFUsP11CLoadUpdate
// STUB: LEMBALL 0x00439e30
void SoundView::ChangeState(unsigned short p_state, LoadUpdate* p_loadUpdate)
{
}

// 68K 0x10b10236 SetEffectsVolume__10CSoundViewFUc
// STUB: LEMBALL 0x00439f50
void SoundView::SetEffectsVolume(unsigned char p_volume)
{
}

// 68K 0x10b1027c SetMusicVolume__10CSoundViewFUc
// STUB: LEMBALL 0x00439f70
void SoundView::SetMusicVolume(unsigned char p_volume)
{
}
