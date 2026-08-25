#include "SoundManager.h"

#include "../Resources/ResEffect.h"
#include "BaseSoundDevice.h"
#include "PvMusicDevice.h"

struct SoundDeviceDispatch {
	virtual void Delete(int p_delete) = 0;
	virtual void Slot04() = 0;
	virtual void Slot08() = 0;
	virtual int Open(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId) = 0;
	virtual int OpenPrimary(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId) = 0;
	virtual void Background() = 0;
	virtual void Slot18() = 0;
	virtual void Slot1c() = 0;
	virtual void Slot20() = 0;
	virtual void Slot24() = 0;
	virtual void Slot28() = 0;
	virtual void Slot2c() = 0;
	virtual void Slot30() = 0;
	virtual void Slot34() = 0;
	virtual void Slot38() = 0;
	virtual void Prepare(unsigned char* p_data, unsigned long* p_handle) = 0;
	virtual void Slot40() = 0;
	virtual void Slot44() = 0;
	virtual void FreeEffect(unsigned long p_effectId) = 0;
	virtual void Slot4c() = 0;
	virtual void Slot50() = 0;
	virtual void Slot54() = 0;
	virtual void Slot58() = 0;
	virtual void Slot5c() = 0;
	virtual void Slot60() = 0;
	virtual unsigned long QueryVolume(unsigned int p_unused) = 0;
	virtual void Slot68() = 0;
	virtual void Slot6c() = 0;
	virtual void Slot70() = 0;
	virtual void Slot74() = 0;
	virtual void Slot78() = 0;
	virtual void Slot7c() = 0;
	virtual void Slot80() = 0;
	virtual void Slot84() = 0;
	virtual void Slot88() = 0;
	virtual void PlayEffect(unsigned long p_effectId, unsigned long p_arg1, unsigned long p_arg2) = 0;
};

struct EffectPlay2Dispatch {
	virtual void Delete(int p_delete) = 0;
	virtual void Slot04() = 0;
	virtual void Slot08() = 0;
	virtual int Open(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId) = 0;
	virtual int OpenPrimary(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId) = 0;
	virtual void Background() = 0;
	virtual void Slot18() = 0;
	virtual void Slot1c() = 0;
	virtual void Slot20() = 0;
	virtual void Slot24() = 0;
	virtual void Slot28() = 0;
	virtual void Slot2c() = 0;
	virtual void Slot30() = 0;
	virtual void Slot34() = 0;
	virtual void Slot38() = 0;
	virtual void Slot3c() = 0;
	virtual void Slot40() = 0;
	virtual void Slot44() = 0;
	virtual void Slot48() = 0;
	virtual void Slot4c() = 0;
	virtual void Slot50() = 0;
	virtual void Slot54() = 0;
	virtual void Slot58() = 0;
	virtual void Slot5c() = 0;
	virtual void Slot60() = 0;
	virtual unsigned long QueryVolume(unsigned int p_unused) = 0;
	virtual void Slot68() = 0;
	virtual void Slot6c() = 0;
	virtual void Slot70() = 0;
	virtual void Slot74() = 0;
	virtual void Slot78() = 0;
	virtual void Slot7c() = 0;
	virtual void Slot80() = 0;
	virtual void Slot84() = 0;
	virtual void Slot88() = 0;
	virtual void PlayEffect(unsigned long p_effectId, unsigned long p_volume) = 0;
};

struct MusicDeviceDispatch {
	virtual void Delete(int p_delete) = 0;
	virtual unsigned long Initialise(unsigned long p_resourceId, unsigned long p_flags) = 0;
	virtual void Play(unsigned long p_allocated, unsigned long p_handle) = 0;
	virtual void Slot0c() = 0;
	virtual void ProcessHandle(unsigned long p_handle) = 0;
	virtual void FreeHandle(unsigned long p_handle) = 0;
	virtual void StopHandle(unsigned long p_handle) = 0;
};

// 68K 0x10218192 __ct__13CSoundManagerFUcUcUciP4CWnd
// STUB: LEMBALL 0x0045af80
SoundManager::SoundManager(unsigned char p_arg0, unsigned char p_arg1, unsigned char p_arg2, int p_arg3, Wnd* p_arg4)
{
}

// 68K 0x10218368 __dt__13CSoundManagerFv
// STUB: LEMBALL 0x0045b110
SoundManager::~SoundManager()
{
}

// 68K 0x1021846a SetResId__13CSoundManagerFUl
// FUNCTION: LEMBALL 0x0045b190
void SoundManager::SetResId(unsigned long p_resourceId)
{
	m_resourceId = p_resourceId;
}

// 68K 0x102184d0 Background__13CSoundManagerFv
// FUNCTION: LEMBALL 0x0045b1d0
void SoundManager::Background()
{
	unsigned int i;
	BaseSoundDevice** devices;

	if (m_background == 0) {
		m_background = 1;
		i = 0;
		if (i < m_deviceCount) {
			devices = m_devices;
			do {
				if (*devices != 0) {
					((SoundDeviceDispatch*) *devices)->Background();
				}
				devices = devices + 1;
				i = i + 1;
			} while (i < m_deviceCount);
		}
	}
}

// 68K 0x10218532 Foreground__13CSoundManagerFv
// FUNCTION: LEMBALL 0x0045b210
void SoundManager::Foreground()
{
	unsigned int music;
	unsigned int effects;
	BaseSoundDevice* musicOutput;

	if (m_background != 0) {
		music = m_requestedMusic;
		effects = 0;
		m_background = 0;
		musicOutput = m_musicOutput;
		if (m_effectOutput == musicOutput) {
			m_effectOutput = 0;
			effects = m_requestedEffects;
		}
		if (m_useMusicCD == 0) {
			if (musicOutput != 0) {
				if (((SoundDeviceDispatch*) musicOutput)->Open(music, effects, m_resourceId) == 0) {
					music = 0;
					effects = 0;
				}
				m_musicAvailable = music;
				m_effectsAvailable = effects;
			}
		}
		if (m_effectOutput != 0) {
			if (((SoundDeviceDispatch*) m_effectOutput)->Open(0, m_requestedEffects, m_resourceId) == 0) {
				m_requestedEffects = 0;
			}
			m_effectsAvailable = m_requestedEffects;
			return;
		}
		if (m_effectsAvailable != 0) {
			m_effectOutput = m_musicOutput;
		}
	}
}

// 68K 0x10218602 PrepareMusic__13CSoundManagerFUlUc
// FUNCTION: LEMBALL 0x0045b2c0
unsigned long SoundManager::PrepareMusic(unsigned long p_resourceId, unsigned char p_flags)
{
	return m_musicDevice->Initialise(p_resourceId, *(unsigned long*) &p_flags);
}

// 68K 0x102186b8 PlayMusic__13CSoundManagerFUl
// FUNCTION: LEMBALL 0x0045b2e0
unsigned long SoundManager::PlayMusic(unsigned long p_handle, unsigned long p_unused)
{
	unsigned long allocated;

	if (m_musicAvailable == 1 && m_useMusicCD == 1) {
		allocated = m_nextMusicHandle;
		m_nextMusicHandle = allocated + 1;
		if (m_nextMusicHandle == 0) {
			m_nextMusicHandle = 1;
		}
		((MusicDeviceDispatch*) m_musicDevice)->Play(allocated, p_handle);
		return allocated;
	}
	return 0;
}

// 68K 0x10218680 StreamOut__13CSoundManagerFR10CVSOStream
// FUNCTION: LEMBALL 0x0045b320
VsOStream& SoundManager::StreamOut(VsOStream& p_stream)
{
	return p_stream;
}

// 68K 0x10218714 ProcessMusic__13CSoundManagerFUl
// FUNCTION: LEMBALL 0x0045b330
void SoundManager::ProcessMusic(unsigned long p_handle)
{
	if (m_musicAvailable == 1) {
		if (p_handle != 0) {
			if (m_useMusicCD == 1) {
				((MusicDeviceDispatch*) m_musicDevice)->ProcessHandle(p_handle);
			}
		}
	}
}

// 68K 0x1021875e StopMusic__13CSoundManagerFUl
// FUNCTION: LEMBALL 0x0045b370
void SoundManager::StopMusic(unsigned long p_handle)
{
	if (m_musicAvailable == 1) {
		if (m_useMusicCD == 1) {
			((MusicDeviceDispatch*) m_musicDevice)->StopHandle(p_handle);
		}
	}
}

// 68K 0x102187b4 FreeMusic__13CSoundManagerFUl
// FUNCTION: LEMBALL 0x0045b3b0
void SoundManager::FreeMusic(unsigned long p_handle)
{
	if (m_musicAvailable == 1) {
		if (m_useMusicCD == 1) {
			((MusicDeviceDispatch*) m_musicDevice)->FreeHandle(p_handle);
		}
	}
}

// 68K 0x10218810 PrepareEffect__13CSoundManagerFUl
// FUNCTION: LEMBALL 0x0045b3f0
unsigned long SoundManager::PrepareEffect(unsigned long p_resourceId)
{
	unsigned long handle;
	ResEffect* effect;
	unsigned char* data;

	if (m_effectsAvailable == 1) {
		effect = ResEffect::Load(p_resourceId);
		if (effect->m_loaded != 0) {
			effect->m_age = 0;
		}
		else {
			effect->LoadData();
		}
		effect->m_directUseCount = effect->m_directUseCount + 1;
		data = effect->GetData();
		((SoundDeviceDispatch*) m_effectOutput)->Prepare(data, &handle);
		effect->m_directUseCount = effect->m_directUseCount - 1;
		effect->UnLoad();
		return handle;
	}
	return 0;
}

// 68K 0x102188c0 PlayEffect__13CSoundManagerFUl
// FUNCTION: LEMBALL 0x0045b460
void SoundManager::PlayEffect(unsigned long p_effectId)
{
	BaseSoundDevice* device;

	if (m_effectsAvailable == 1) {
		device = m_effectOutput;
		((EffectPlay2Dispatch*) device)->PlayEffect(p_effectId, ((EffectPlay2Dispatch*) device)->QueryVolume(0));
	}
}

// 68K 0x10218924 PlayEffect__13CSoundManagerFUlUc
// FUNCTION: LEMBALL 0x0045b490
void SoundManager::PlayEffect(unsigned long p_effectId, unsigned int p_channel)
{
	if (m_effectsAvailable == 1) {
		((SoundDeviceDispatch*) m_effectOutput)->PlayEffect(p_effectId, p_channel, 0);
	}
}

// 68K 0x10218978 FreeEffect__13CSoundManagerFUl
// FUNCTION: LEMBALL 0x0045b4f0
void SoundManager::FreeEffect(unsigned long p_effectId)
{
	if (m_effectsAvailable == 1) {
		((SoundDeviceDispatch*) m_effectOutput)->FreeEffect(p_effectId);
	}
}

// 68K 0x102189c2 SetVolumes__13CSoundManagerFii
// FUNCTION: LEMBALL 0x0045b510
void SoundManager::SetVolumes(int p_effectVolume, int p_musicVolume)
{
	if (p_effectVolume != -1) {
		if (m_effectOutput != 0) {
			m_effectOutput->SetMusicVolume((unsigned char) p_effectVolume);
		}
	}
	if (p_musicVolume != -1 && m_useMusicCD != 0) {
		m_musicDevice->SetVolume((unsigned char) p_musicVolume);
		return;
	}
	if (p_musicVolume != -1) {
		if (m_musicOutput != 0) {
			m_musicOutput->SetMasterVolume((unsigned char) p_musicVolume);
		}
	}
}

// 68K 0x10218a5e GetEffectVolume__13CSoundManagerFv
// FUNCTION: LEMBALL 0x0045b560
unsigned char SoundManager::GetEffectVolume()
{
	if (m_effectOutput != 0) {
		return m_effectOutput->GetEffectVolume();
	}
	return 0;
}

// 68K 0x10218aaa SetEffectsWnd__13CSoundManagerFP4CWnd
// FUNCTION: LEMBALL 0x0045b580
void SoundManager::SetEffectsWnd(Wnd* p_window)
{
	m_effectOutput->SetWnd(p_window);
}

// 68K 0x10218af2 SetMusicWnd__13CSoundManagerFP4CWnd
// FUNCTION: LEMBALL 0x0045b5a0
void SoundManager::SetMusicWnd(Wnd* p_window)
{
	if (m_musicOutput != 0) {
		m_musicOutput->SetWnd(p_window);
	}
}

// 68K 0x10218b40 UseMusicCD__13CSoundManagerFUc
// FUNCTION: LEMBALL 0x0045b5f0
void SoundManager::UseMusicCd(unsigned int p_enabled)
{
	m_musicDevice->m_reserved0c = p_enabled;
}

// GLOBAL: LEMBALL 0x004a97c8
char g_szSoundDriverInfo[1024];
