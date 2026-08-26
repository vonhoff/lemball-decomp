#include "SoundView.h"

#include "../../AI/Base/AiCoord.h"
#include "../../AI/Objects/ViewData.h"
#include "../../Control/Game/Demo.h"
#include "../../Control/Game/Game.h"
#include "../../Frontend/Base/BaseFrontendDrawer.h"
#include "../../Frontend/Base/BaseFrontendProcess.h"
#include "../../Visos/Foundation/VsMath.h"
#include "../../Visos/Foundation/VsTime.h"
#include "../../Visos/Sound/SoundManager.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// GLOBAL: LEMBALL 0x0049eb80
SoundView* g_pSoundView = 0;

// GLOBAL: LEMBALL 0x0049eb88
EffectSpec g_pEffectSpecs[44] = {
	{1, 0x214, 9, 4},
	{2, 0x220, 9, 4},
	{3, 0x21f, 0x63, 4},
	{4, 0x219, 9, 4},
	{5, 0x209, 9, 0},
	{6, 0x20a, 9, 0},
	{7, 0x20b, 9, 7},
	{8, 0x20c, 9, 0},
	{9, 0x20d, 9, 4},
	{10, 0x20e, 9, 4},
	{11, 0x20f, 9, 4},
	{12, 0x210, 9, 4},
	{13, 0x211, 9, 4},
	{14, 0x212, 0xf, 7},
	{15, 0x213, 7, 4},
	{16, 0x215, 9, 4},
	{17, 0x216, 9, 7},
	{18, 0x217, 9, 4},
	{19, 0x218, 9, 7},
	{20, 0x21a, 9, 4},
	{21, 0x21b, 9, 4},
	{22, 0x21c, 9, 4},
	{23, 0x21e, 9, 4},
	{24, 0x21d, 9, 4},
	{26, 0x221, 9, 4},
	{27, 0x222, 9, 7},
	{25, 0x223, 9, 7},
	{28, 0x224, 0xf, 4},
	{29, 0x225, 0xf, 4},
	{30, 0x22b, 0xf, 4},
	{31, 0x226, 9, 4},
	{32, 0x227, 9, 4},
	{33, 0x228, 9, 4},
	{34, 0x22c, 9, 7},
	{35, 0x22d, 9, 4},
	{37, 0x229, 9, 7},
	{38, 0x22a, 9, 4},
	{39, 0x22f, 9, 3},
	{40, 0x230, 9, 3},
	{41, 0x234, 9, 4},
	{42, 0x232, 9, 4},
	{43, 0x233, 9, 4},
	{44, 0x231, 9, 4},
	{45, 0x235, 9, 4},
};

// GLOBAL: LEMBALL 0x0049ed98
unsigned int g_dwEffectsOn = 0;

// GLOBAL: LEMBALL 0x0049ed9c
unsigned int g_dwMusicOn = 0;

// 68K 0x10b0fb42 __ct__10CSoundViewFv
// FUNCTION: LEMBALL 0x00439a70
SoundView::SoundView()
{
	int i;
	EffectSpec* spec;

	m_unk0x64 = 0;
	m_effectSpecTable = (void*) g_dwGameTick;
	m_flags = 0;
	m_currentState = 0;
	m_musicHandle = 0;
	m_musicResourceId = 0;
	m_randomMusicIndex = 0;
	m_loadUpdate = 0;
	if (g_nMusicVolume != 0) {
		g_pSoundManager->PrepareMusic(0x2220, 0xb482);
		g_dwMusicOn = 0;
	}
	SetEffectsOn(1);
	i = 50;
	do {
		m_effectSlots[50 - i].m_handle = 0xffffffff;
		m_effectSlots[50 - i].m_spec = 0;
		i--;
	} while (i != 0);
	if (g_nEffectsAvailable != 0) {
		spec = g_pEffectSpecs;
		do {
			if (g_nEffectsAvailable != 0) {
				m_effectSlots[spec->m_soundId].m_spec = spec;
				m_effectSlots[spec->m_soundId].m_handle = 0xffffffff;
			}
			spec++;
		} while (spec < g_pEffectSpecs + 44);
	}
	m_pendingEffect = (eSoundEffect) 0;
}

// 68K 0x10b0fbd4 __dt__10CSoundViewFv
// FUNCTION: LEMBALL 0x00439b30
SoundView::~SoundView()
{
	if (g_nMusicVolume != 0) {
		SetMusicOn(0);
	}
	UnprepareEffects();
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
// FUNCTION: LEMBALL 0x00439ba0
void SoundView::SetMusicOn(unsigned int p_enabled)
{
	unsigned long handle;

	if (p_enabled == g_dwMusicOn) {
		return;
	}

	if (g_nMusicVolume != 0 && m_musicResourceId != 0) {
		if (p_enabled != 0) {
			handle = g_pSoundManager->PlayMusic(m_musicResourceId, 1);
			m_musicHandle = handle;
			g_pSoundManager->ProcessMusic(handle);
			g_dwMusicOn = p_enabled;
			return;
		}
		g_pSoundManager->FreeMusic(m_musicHandle);
		g_pSoundManager->StopMusic(m_musicHandle);
		g_dwMusicOn = p_enabled;
	}
}

// 68K 0x10b0fd8a SoundEffect__10CSoundViewFP9CViewDataiR7AICOORD
// FUNCTION: LEMBALL 0x00439c40
void SoundView::SoundEffect(ViewData* p_viewData, int p_count, AiCoord& p_listener)
{
	int dist;
	int volume;
	int attenuatedVol;
	unsigned long now;
	int x;
	int y;
	eSoundEffect effectId;
	int i;

	if (g_nEffectsAvailable != 0 && g_dwEffectsOn != 0) {
		if (m_pendingEffect != 0) {
			g_pSoundManager->PlayEffect(m_effectSlots[m_pendingEffect].m_handle);
			m_pendingEffect = (eSoundEffect) 0;
		}
		now = timeGetTime();
		x = p_listener.m_xFixed >> 12;
		y = p_listener.m_yFixed >> 12;
		volume = g_pSoundManager->GetEffectVolume();
		if (p_count > 0) {
			for (i = 0; i < p_count; i++, p_viewData++) {
				effectId = p_viewData->m_soundEffect;
				if (effectId != 0) {
					dist = Distance(x, y, (unsigned short) p_viewData->m_viewX, (unsigned short) p_viewData->m_viewY);
					attenuatedVol = volume;
					dist -= 200;
					if (dist > 0) {
						attenuatedVol = volume + (int) (dist * volume * -40) / 124800;
						if (volume < attenuatedVol) {
							attenuatedVol = volume;
						}
					}
					if (now - m_effectSlots[effectId].m_lastPlayed > 100) {
						g_pSoundManager->PlayEffect(m_effectSlots[effectId].m_handle, (unsigned char) attenuatedVol);
						m_effectSlots[effectId].m_lastPlayed = now;
					}
				}
			}
		}
	}
}

// 68K 0x10b0ff0a UnprepareEffects__10CSoundViewFv
// FUNCTION: LEMBALL 0x00439d60
void SoundView::UnprepareEffects()
{
	int i;
	unsigned int* handle;

	handle = &m_effectSlots[0].m_handle;
	i = 50;
	do {
		if (*handle != 0xffffffff) {
			g_pSoundManager->FreeEffect(*handle);
			*handle = 0xffffffff;
		}
		handle = (unsigned int*) ((char*) handle + sizeof(EffectSlot));
		i--;
	} while (i != 0);
}

// 68K 0x10b0ff80 PrepareEffects__10CSoundViewFUs
// FUNCTION: LEMBALL 0x00439d90
void SoundView::PrepareEffects(unsigned short p_stateMask)
{
	EffectSlot* slot;
	unsigned long timestamp;
	int i;
	EffectSpec* spec;

	if (g_nEffectsAvailable != 0) {
		slot = m_effectSlots;
		timestamp = timeGetTime() - 100;
		i = 50;
		do {
			spec = slot->m_spec;
			if (spec != 0 && (spec->m_groupMask & p_stateMask) != 0) {
				slot->m_handle = g_pSoundManager->PrepareEffect(spec->m_resourceId);
			}
			slot->m_lastPlayed = timestamp;
			if (m_loadUpdate != 0) {
				m_loadUpdate->Update();
			}
			slot++;
			i--;
		} while (i != 0);
	}
}

// 68K 0x10b10068 GetnEffects__10CSoundViewFUs
// FUNCTION: LEMBALL 0x00439df0
int SoundView::GetnEffects(unsigned short p_stateMask)
{
	int count;
	EffectSlot* slot;
	int i;

	count = 0;
	if (g_nEffectsAvailable != 0) {
		slot = m_effectSlots;
		i = 50;
		do {
			if (slot->m_spec != 0 && (slot->m_spec->m_groupMask & p_stateMask) != 0) {
				count++;
			}
			slot++;
			i--;
		} while (i != 0);
	}
	return count;
}

// 68K 0x10b100d0 ChangeState__10CSoundViewFUsP11CLoadUpdate
// FUNCTION: LEMBALL 0x00439e30
void SoundView::ChangeState(unsigned short p_state, LoadUpdate* p_loadUpdate)
{
	int restartMusic;
	int musicId;
	int sentinel;

	if (m_currentState != p_state) {
		restartMusic = 1;
		if (g_nDemoMode != 0) {
			UnprepareEffects();
			restartMusic = 0;
		}
		else {
			SetMusicOn(0);
			UnprepareEffects();
			g_pSoundManager->Background();
		}
		m_currentState = p_state;
		musicId = 0;
		switch (p_state) {
		case 0:
		case 1:
			return;
		case 2:
			p_state = 3;
			restartMusic = 0;
		case 3:
			musicId = 0x242;
			g_pSoundManager->SetResId(0x243);
			break;
		case 4:
			g_pSoundManager->SetResId(0x243);
			musicId = m_randomMusicIndex + 0x239;
			sentinel = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
			*g_pSentinel = sentinel;
			m_randomMusicIndex = sentinel % 9;
			break;
		}
		m_loadUpdate = p_loadUpdate;
		if (g_nDemoMode == 0) {
			g_pSoundManager->Foreground();
			m_musicResourceId = musicId;
		}
		PrepareEffects(p_state);
		if (restartMusic != 0) {
			SetMusicOn(1);
		}
		m_loadUpdate = 0;
	}
}

// 68K 0x10b10236 SetEffectsVolume__10CSoundViewFUc
// FUNCTION: LEMBALL 0x00439f50
void SoundView::SetEffectsVolume(unsigned char p_volume)
{
	g_pSoundManager->SetVolumes((int) p_volume, -1);
}

// 68K 0x10b1027c SetMusicVolume__10CSoundViewFUc
// FUNCTION: LEMBALL 0x00439f70
void SoundView::SetMusicVolume(unsigned char p_volume)
{
	g_pSoundManager->SetVolumes(-1, (int) p_volume);
}
