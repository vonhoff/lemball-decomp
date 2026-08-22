#ifndef LEMBALL_VISOS_SOUND_SOUNDMANAGER_H
#define LEMBALL_VISOS_SOUND_SOUNDMANAGER_H

#include "../../Common.h"

// SIZE 0xc90
// VTABLE: LEMBALL 0x00498978
class SoundManager {
public:
	SoundManager(unsigned char p_arg0, unsigned char p_arg1, unsigned char p_arg2, int p_arg3, Wnd* p_arg4);
	unsigned char GetEffectVolume();
	unsigned long PrepareEffect(unsigned long p_resourceId);
	unsigned long PrepareMusic(unsigned long p_resourceId, unsigned char p_flags);
	virtual VsOStream& StreamOut(VsOStream& p_stream); // vtable+0x00
	void Background();
	void Foreground();
	void FreeEffect(unsigned long p_effectId);
	void FreeMusic(unsigned long p_handle);
	void PlayEffect(unsigned long p_effectId);
	void PlayEffect(unsigned long p_effectId, unsigned char p_channel);
	void PlayMusic(unsigned long p_handle);
	void ProcessMusic(unsigned long p_handle);
	void SetEffectsWnd(Wnd* p_window);
	void SetMusicWnd(Wnd* p_window);
	void SetResId(unsigned long p_resourceId);
	void SetVolumes(int p_effectVolume, int p_musicVolume);
	void StopMusic(unsigned long p_handle);
	void UseMusicCd(unsigned char p_enabled);
	~SoundManager();

private:
	unsigned int m_reserved04;         // 0x04
	unsigned int m_background;         // 0x08
	unsigned int m_musicAvailable;     // 0x0c
	unsigned int m_effectsAvailable;   // 0x10
	unsigned int m_requestedMusic;     // 0x14
	unsigned int m_requestedEffects;   // 0x18
	unsigned int m_useMusicCD;         // 0x1c
	unsigned int m_deviceCount;        // 0x20
	BaseSoundDevice* m_devices[4];     // 0x24
	BaseSoundDevice* m_musicDevice;    // 0x34
	undefined m_reserved38[0x34];      // 0x38
	unsigned int m_nextMusicHandle;    // 0x6c
	unsigned int m_resourceId;         // 0x70
	BaseSoundDevice* m_musicOutput;    // 0x74
	BaseSoundDevice* m_effectOutput;   // 0x78
	undefined m_runtimeState[0xc00];   // 0x7c
	unsigned char m_musicRequested;    // 0xc7c
	unsigned char m_effectsRequested;  // 0xc7d
	unsigned char m_musicCapability;   // 0xc7e
	unsigned char m_effectsCapability; // 0xc7f
	unsigned int m_musicState;         // 0xc80
	unsigned int m_effectsState;       // 0xc84
	unsigned int m_advancedEffects;    // 0xc88
	unsigned int m_musicStateCopy;     // 0xc8c
};

// Confirmed class-scoped globals.
extern char g_szSoundDriverInfo[1024];
#endif
