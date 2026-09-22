#ifndef LEMBALL_VIEWS_SOUND_CSOUNDVIEW_H
#define LEMBALL_VIEWS_SOUND_CSOUNDVIEW_H

#include "SoundEffects.h"
class CLoadUpdate;

class AiCoord;
class CViewData;
struct EffectSpec {
	int m_soundId;              // 0x00
	unsigned long m_resourceId; // 0x04
	unsigned short m_reserved;  // 0x08
	unsigned short m_groupMask; // 0x0a
};

struct EffectSlot {
	EffectSpec* m_spec;         // 0x00
	unsigned int m_handle;      // 0x04
	unsigned long m_lastPlayed; // 0x08
};

// SIZE 0x2cc
class CSoundView {
public:
	CSoundView();
	int GetnEffects(unsigned short p_stateMask);
	void ChangeState(unsigned short p_state, CLoadUpdate* p_loadUpdate);
	void PlayEffect(eSoundEffect p_soundId);
	void PrepareEffects(unsigned short p_stateMask);
	void SetEffectsOn(unsigned int p_enabled);
	void SetEffectsVolume(unsigned char p_volume);
	void SetMusicOn(unsigned int p_enabled);
	void SetMusicVolume(unsigned char p_volume);
	void SoundEffect(CViewData* p_viewData, int p_count, AiCoord& p_listener);
	void UnprepareEffects();
	~CSoundView();

	friend class C2D;
	friend class CPanel;
	friend class CPanelPauseButton;
	friend class CPanelButton;

private:
	void* m_effectSpecTable;         // 0x00
	CLoadUpdate* m_loadUpdate;       // 0x04
	unsigned int m_flags;            // 0x08
	unsigned short m_currentState;   // 0x0c
	unsigned short m_reserved0e;     // 0x0e
	eSoundEffect m_pendingEffect;    // 0x10
	char m_headerState[0x50];        // 0x14
	unsigned int m_unk0x64;          // 0x64
	EffectSlot m_effectSlots[50];    // 0x68
	unsigned int m_musicResourceId;  // 0x2c0
	unsigned int m_musicHandle;      // 0x2c4
	unsigned int m_randomMusicIndex; // 0x2c8
};

extern CSoundView* g_pSoundView;
extern unsigned int g_dwEffectsOn;
extern unsigned int g_dwMusicOn;
extern EffectSpec g_pEffectSpecs[44];

#endif
