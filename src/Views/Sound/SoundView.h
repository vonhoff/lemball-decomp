#ifndef LEMBALL_VIEWS_SOUND_SOUNDVIEW_H
#define LEMBALL_VIEWS_SOUND_SOUNDVIEW_H

#include "../../Common.h"

// SIZE 0x2cc
class SoundView {
public:
	SoundView();
	int GetnEffects(unsigned short p_stateMask);
	void ChangeState(unsigned short p_state, LoadUpdate* p_loadUpdate);
	void PlayEffect(eSoundEffect p_soundId);
	void PrepareEffects(unsigned short p_stateMask);
	void SetEffectsOn(unsigned char p_enabled);
	void SetEffectsVolume(unsigned char p_volume);
	void SetMusicOn(unsigned char p_enabled);
	void SetMusicVolume(unsigned char p_volume);
	void SoundEffect(ViewData* p_viewData, int p_count, AiCoord& p_listener);
	void UnprepareEffects();
	~SoundView();

private:
	void* m_effectSpecTable;         // 0x00
	LoadUpdate* m_loadUpdate;        // 0x04
	unsigned int m_flags;            // 0x08
	unsigned short m_currentState;   // 0x0c
	eSoundEffect m_pendingEffect;    // 0x10
	undefined m_headerState[0x54];   // 0x14
	undefined m_effectSlots[0x258];  // 0x68
	unsigned int m_musicResourceId;  // 0x2c0
	unsigned int m_musicHandle;      // 0x2c4
	unsigned int m_randomMusicIndex; // 0x2c8
};

#endif
