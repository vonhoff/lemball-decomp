#include "GameView/CSoundView.h"

#include "Gameplay/Simulation/GameTime.h"

#include "Application/GameMain.h"

#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CViewData.h"
#include "Application/CDemo.h"
#include "Frontend/CBaseFrontendDrawer.h"
#include "Frontend/CBaseFrontendProcess.h"
#include "Gameplay/Geometry/Facing.h"
#include "Engine/Resources/Manifest.h"
#include "Engine/Sound/CSoundManager.h"
#include "GameView/CLoadUpdate.h"
#include "Application/SoundEffects.h"
#include "Engine/Math/FixedPoint.h"
#include "Engine/Math/RandomConstants.h"

#include <stddef.h>

enum {
	SOUND_GAMEPLAY_MUSIC_TRACK_COUNT = RES_MUSIC_FRONTEND_MUSIC3 - RES_MUSIC_GAME_MUSIC,
	SOUND_EFFECT_ATTENUATION_ARITHMETIC_SCALE = 40,
	SOUND_EFFECT_FADE_RANGE_PIXELS = 3120,
	SOUND_EFFECT_ATTENUATION_DENOMINATOR = SOUND_EFFECT_ATTENUATION_ARITHMETIC_SCALE * SOUND_EFFECT_FADE_RANGE_PIXELS
};

extern "C" unsigned long __stdcall timeGetTime(void);

// GLOBAL: LEMBALL 0x0049eb80
CSoundView* g_pSoundView = NULL;

// GLOBAL: LEMBALL 0x0049eb88
EffectSpec g_pEffectSpecs[SOUND_EFFECT_SPEC_COUNT] = {
	{SFX_LETSGO, RES_SFX_LETSGO, 9, SOUND_STATE_GAMEPLAY},
	{SFX_YIPPEE, RES_SFX_YIPPEE, 9, SOUND_STATE_GAMEPLAY},
	{SFX_MOUSE_CLICK, RES_SFX_MOUSE_CLICK, 0x63, SOUND_STATE_GAMEPLAY},
	{SFX_SHEEP, RES_SFX_SHEEP, 9, SOUND_STATE_GAMEPLAY},
	{SFX_AIRLOCK, RES_SFX_AIRLOCK, 9, SOUND_STATE_MASK_NONE},
	{SFX_AIRPIPE, RES_SFX_AIRPIPE, 9, SOUND_STATE_MASK_NONE},
	{SFX_BIGGUN, RES_SFX_BIGGUN, 9, SOUND_STATE_MASK_FRONTEND_AND_GAMEPLAY},
	{SFX_BIRDS, RES_SFX_BIRDS, 9, SOUND_STATE_MASK_NONE},
	{SFX_CATAPULT, RES_SFX_CATAPULT, 9, SOUND_STATE_GAMEPLAY},
	{SFX_CRATEEXP, RES_SFX_CRATEEXP, 9, SOUND_STATE_GAMEPLAY},
	{SFX_DOOROPEN, RES_SFX_DOOROPEN, 9, SOUND_STATE_GAMEPLAY},
	{SFX_DUPLICTR, RES_SFX_DUPLICTR, 9, SOUND_STATE_GAMEPLAY},
	{SFX_GUN, RES_SFX_GUN, 9, SOUND_STATE_GAMEPLAY},
	{SFX_GUNHIT, RES_SFX_GUNHIT, 0xf, SOUND_STATE_MASK_FRONTEND_AND_GAMEPLAY},
	{SFX_LASER, RES_SFX_LASER, 7, SOUND_STATE_GAMEPLAY},
	{SFX_MINEEXP, RES_SFX_MINEEXP, 9, SOUND_STATE_GAMEPLAY},
	{SFX_RELOAD, RES_SFX_RELOAD, 9, SOUND_STATE_MASK_FRONTEND_AND_GAMEPLAY},
	{SFX_ROCKET, RES_SFX_ROCKET, 9, SOUND_STATE_GAMEPLAY},
	{SFX_ROPESLID, RES_SFX_ROPESLID, 9, SOUND_STATE_MASK_FRONTEND_AND_GAMEPLAY},
	{SFX_SNATCH, RES_SFX_SNATCH, 9, SOUND_STATE_GAMEPLAY},
	{SFX_SWITCH, RES_SFX_SWITCH, 9, SOUND_STATE_GAMEPLAY},
	{SFX_TIMBONUS, RES_SFX_TIMBONUS, 9, SOUND_STATE_GAMEPLAY},
	{SFX_TRMPLINE, RES_SFX_TRMPLINE, 9, SOUND_STATE_GAMEPLAY},
	{SFX_TRAPDOOR, RES_SFX_TRAPDOOR, 9, SOUND_STATE_GAMEPLAY},
	{SFX_FIRE, RES_SFX_FIRE, 9, SOUND_STATE_GAMEPLAY},
	{SFX_CHANGEOP, RES_SFX_CHANGEOP, 9, SOUND_STATE_MASK_FRONTEND_AND_GAMEPLAY},
	{SFX_CHINK, RES_SFX_CHINK, 9, SOUND_STATE_MASK_FRONTEND_AND_GAMEPLAY},
	{SFX_AAAAH1, RES_SFX_AAAAH1, 0xf, SOUND_STATE_GAMEPLAY},
	{SFX_AAAAH2, RES_SFX_AAAAH2, 0xf, SOUND_STATE_GAMEPLAY},
	{SFX_EEEEH, RES_SFX_EEEEH, 0xf, SOUND_STATE_GAMEPLAY},
	{SFX_BALLOON, RES_SFX_BALLOON, 9, SOUND_STATE_GAMEPLAY},
	{SFX_DOORAPPR, RES_SFX_DOORAPPR, 9, SOUND_STATE_GAMEPLAY},
	{SFX_DOORGO, RES_SFX_DOORGO, 9, SOUND_STATE_GAMEPLAY},
	{SFX_ELECCY, RES_SFX_ELECCY, 9, SOUND_STATE_MASK_FRONTEND_AND_GAMEPLAY},
	{SFX_LEMSPLAT, RES_SFX_LEMSPLAT, 9, SOUND_STATE_GAMEPLAY},
	{SFX_DRUM1, RES_SFX_DRUM1, 9, SOUND_STATE_MASK_FRONTEND_AND_GAMEPLAY},
	{SFX_DRUM2, RES_SFX_DRUM2, 9, SOUND_STATE_GAMEPLAY},
	{SFX_SUCCESS, RES_SFX_SUCCESS, 9, SOUND_STATE_FRONTEND},
	{SFX_FAILURE, RES_SFX_FAILURE, 9, SOUND_STATE_FRONTEND},
	{SFX_KEYS, RES_SFX_KEYS, 9, SOUND_STATE_GAMEPLAY},
	{SFX_COLLECT_BALLOON, RES_SFX_COLLECT_BALLOON, 9, SOUND_STATE_GAMEPLAY},
	{SFX_BALLOON_EXPLODE, RES_SFX_BALLOON_EXPLODE, 9, SOUND_STATE_GAMEPLAY},
	{SFX_LIFT, RES_SFX_LIFT, 9, SOUND_STATE_GAMEPLAY},
	{SFX_WHEEE, RES_SFX_WHEEE, 9, SOUND_STATE_GAMEPLAY},
};

// GLOBAL: LEMBALL 0x0049ed98
unsigned int g_dwEffectsOn = 0;

// GLOBAL: LEMBALL 0x0049ed9c
unsigned int g_dwMusicOn = 0;

// FUNCTION: LEMBALL 0x00439a70
CSoundView::CSoundView()
{
	int i;
	EffectSpec* spec;

	m_unk0x64 = 0;
	m_initialGameTick = g_dwGameTick;
	m_flags = 0;
	m_currentState = SOUND_STATE_SILENT;
	m_musicHandle = 0;
	m_musicResourceId = 0;
	m_randomMusicIndex = 0;
	m_loadUpdate = NULL;
	if (g_nMusicVolume != 0) {
		g_pSoundManager->PrepareMusic(0x2220, 0xb482);
		g_dwMusicOn = 0;
	}
	SetEffectsOn(1);
	{
		i = 0;
		while (i < SOUND_EFFECT_SLOT_COUNT) {
			m_effectSlots[i].m_handle = SOUND_EFFECT_HANDLE_UNPREPARED;
			m_effectSlots[i].m_spec = NULL;
			i = i + 1;
		}
	}
	if (g_nEffectsAvailable != 0) {
		spec = g_pEffectSpecs;
		do {
			if (g_nEffectsAvailable != 0) {
				m_effectSlots[spec->m_soundId].m_spec = spec;
				m_effectSlots[spec->m_soundId].m_handle = SOUND_EFFECT_HANDLE_UNPREPARED;
			}
			spec++;
		} while (spec < g_pEffectSpecs + SOUND_EFFECT_SPEC_COUNT);
	}
	m_pendingEffect = SFX_NONE;
}

// FUNCTION: LEMBALL 0x00439b30
CSoundView::~CSoundView()
{
	if (g_nMusicVolume != 0) {
		SetMusicOn(0);
	}
	UnprepareEffects();
}

// FUNCTION: LEMBALL 0x00439b50
void CSoundView::SetEffectsOn(unsigned int p_enabled)
{
	if (g_nEffectsAvailable != 0) {
		g_dwEffectsOn = p_enabled;
	}
}

// FUNCTION: LEMBALL 0x00439b70
void CSoundView::PlayEffect(eSoundEffect p_soundId)
{
	if (g_dwEffectsOn != 0) {
		g_pSoundManager->PlayEffect(m_effectSlots[p_soundId].m_handle);
	}
}

// FUNCTION: LEMBALL 0x00439ba0
void CSoundView::SetMusicOn(unsigned int p_enabled)
{
	unsigned long handle;

	if ((p_enabled != 0 && g_dwMusicOn != 0) || (p_enabled == 0 && g_dwMusicOn == 0)) {
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
		g_pSoundManager->StopMusicCd(m_musicHandle);
		g_dwMusicOn = p_enabled;
	}
}

// FUNCTION: LEMBALL 0x00439c40
void CSoundView::SoundEffect(CViewData* p_viewData, int p_count, AICOORD& p_listener)
{
	int attenuatedVol;
	int volume;
	int dist;
	unsigned long now;
	int x;
	int y;
	eSoundEffect effectId;
	int i;

	if (g_nEffectsAvailable != 0 && g_dwEffectsOn != 0) {
		if (m_pendingEffect != 0) {
			eSoundEffect pendingEffect = m_pendingEffect;
			m_pendingEffect = SFX_NONE;
			g_pSoundManager->PlayEffect(m_effectSlots[pendingEffect].m_handle);
		}
		now = timeGetTime();
		x = p_listener.m_xFixed >> FIXED_POINT_FRACTION_BITS;
		y = p_listener.m_yFixed >> FIXED_POINT_FRACTION_BITS;
		volume = g_pSoundManager->GetEffectVolume();
		if (p_count > 0) {
			for (i = 0; i < p_count; i++) {
				effectId = p_viewData[i].m_soundEffect;
				if (effectId != 0) {
					const int& effectX = (unsigned short) p_viewData[i].m_gameX;
					const int& effectY = (unsigned short) p_viewData[i].m_gameY;
					dist = Distance(x, y, effectX, effectY);
					attenuatedVol = volume;
					dist -= SOUND_EFFECT_FULL_VOLUME_RADIUS_PIXELS;
					if (dist > 0) {
						dist *= volume;
						attenuatedVol = volume + (dist * -SOUND_EFFECT_ATTENUATION_ARITHMETIC_SCALE) /
													 SOUND_EFFECT_ATTENUATION_DENOMINATOR;
						if (attenuatedVol > volume) {
							attenuatedVol = volume;
						}
					}
					if (now - m_effectSlots[effectId].m_lastPlayed > SOUND_EFFECT_COOLDOWN_MS) {
						g_pSoundManager->PlayEffect(m_effectSlots[effectId].m_handle, attenuatedVol);
						m_effectSlots[effectId].m_lastPlayed = now;
					}
				}
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00439d60
void CSoundView::UnprepareEffects()
{
	for (int i = 0; i < SOUND_EFFECT_SLOT_COUNT; i++) {
		if (m_effectSlots[i].m_handle != SOUND_EFFECT_HANDLE_UNPREPARED) {
			g_pSoundManager->FreeEffect(m_effectSlots[i].m_handle);
			m_effectSlots[i].m_handle = SOUND_EFFECT_HANDLE_UNPREPARED;
		}
	}
}

// FUNCTION: LEMBALL 0x00439d90
void CSoundView::PrepareEffects(unsigned short p_stateMask)
{
	EffectSpec* spec;
	int i;
	unsigned long timestamp;
	EffectSlot* slot;

	if (g_nEffectsAvailable != 0) {
		timestamp = timeGetTime() - SOUND_EFFECT_COOLDOWN_MS;
		for (i = 0; i < SOUND_EFFECT_SLOT_COUNT; i++) {
			slot = &m_effectSlots[i];
			spec = slot->m_spec;
			if (spec != NULL && (spec->m_groupMask & p_stateMask) != 0) {
				slot->m_handle = g_pSoundManager->PrepareEffect(spec->m_resourceId);
			}
			slot->m_lastPlayed = timestamp;
			if (m_loadUpdate != NULL) {
				m_loadUpdate->UpdateNonCacheLoad();
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00439df0
int CSoundView::GetnEffects(unsigned short p_stateMask)
{
	int count;
	EffectSlot* slot;
	int i;

	count = 0;
	if (g_nEffectsAvailable != 0) {
		slot = m_effectSlots;
		i = SOUND_EFFECT_SLOT_COUNT;
		do {
			if (slot->m_spec != NULL && (slot->m_spec->m_groupMask & p_stateMask) != 0) {
				count++;
			}
			slot++;
			i--;
		} while (i != 0);
	}
	return count;
}

// FUNCTION: LEMBALL 0x00439e30
void CSoundView::ChangeState(unsigned short p_state, CLoadUpdate* p_loadUpdate)
{
	int restartMusic;
	int musicId;
	int seed;

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
		case SOUND_STATE_SILENT:
		case SOUND_STATE_INTRO:
			return;
		case SOUND_STATE_RESULTS:
			p_state = SOUND_STATE_FRONTEND;
			restartMusic = 0;
		case SOUND_STATE_FRONTEND:
			musicId = RES_MUSIC_FRONTEND_MUSIC3;
			g_pSoundManager->SetResId(RES_MUSIC_EFFECTS_BASEEFFECTS);
			break;
		case SOUND_STATE_GAMEPLAY:
			g_pSoundManager->SetResId(RES_MUSIC_EFFECTS_BASEEFFECTS);
			musicId = m_randomMusicIndex + RES_MUSIC_GAME_MUSIC;
			seed = (*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
			*g_pRandomSeed = seed;
			m_randomMusicIndex = seed % SOUND_GAMEPLAY_MUSIC_TRACK_COUNT;
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
		m_loadUpdate = NULL;
	}
}

// FUNCTION: LEMBALL 0x00439f50
void CSoundView::SetEffectsVolume(unsigned char p_volume)
{
	g_pSoundManager->SetVolumes(p_volume, SOUND_VOLUME_UNCHANGED);
}

// FUNCTION: LEMBALL 0x00439f70
void CSoundView::SetMusicVolume(unsigned char p_volume)
{
	g_pSoundManager->SetVolumes(SOUND_VOLUME_UNCHANGED, p_volume);
}

// FUNCTION: LEMBALL 0x00439f90
void CSoundView::StopMusicIfEnabled()
{
	if (g_dwMusicOn != 0) {
		g_pSoundManager->StopMusic(m_musicHandle);
	}
}

// FUNCTION: LEMBALL 0x00439fb0
void CSoundView::ResumeMusicIfEnabled()
{
	if (g_dwMusicOn != 0) {
		g_pSoundManager->ResumeMusicCd(m_musicHandle);
	}
}
