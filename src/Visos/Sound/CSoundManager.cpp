#include "CSoundManager.h"

#include "../Foundation/VsSound.h"
#include "../Resources/CResEFFECT.h"
#include "CBaseSoundDevice.h"
#include "CPVMusicDevice.h"

#include <string.h>

#pragma intrinsic(strcat)

struct SoundDeviceDispatch {
	virtual void Delete(int p_delete) = 0;
	virtual void Slot04() = 0;
	virtual void Slot08() = 0;
	virtual int Open(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId) = 0;
	virtual int OpenPrimary(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId) = 0;
	virtual void Slot14() = 0;
	virtual void Slot18() = 0;
	virtual void Slot1c() = 0;
	virtual void Slot20() = 0;
	virtual int IsMusicAvailable() = 0;
	virtual int IsEffectAvailable() = 0;
	virtual unsigned char Slot2c() = 0;
	virtual unsigned char GetBuffersPerEffect() = 0;
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
	virtual unsigned long QueryVolume() = 0;
	virtual void Slot68() = 0;
	virtual void Slot6c() = 0;
	virtual void Slot70() = 0;
	virtual void Slot74() = 0;
	virtual void Slot78() = 0;
	virtual void Slot7c() = 0;
	virtual void Slot80() = 0;
	virtual void Slot84() = 0;
	virtual void Slot88() = 0;
	virtual void PlayEffect(unsigned long p_effectId, unsigned long p_volume, unsigned long p_flags) = 0;
};

struct MusicDeviceDispatch {
	virtual void Delete(int p_delete) = 0;
	virtual unsigned long Initialise(unsigned long p_resourceId, unsigned long p_flags) = 0;
	virtual void Prepare(unsigned long p_handle, unsigned long p_resourceId) = 0;
	virtual void Slot0c() = 0;
	virtual void Play(unsigned long p_handle) = 0;
	virtual void Stop(unsigned long p_handle) = 0;
	virtual void Pause(unsigned long p_handle) = 0;
};

// FUNCTION: LEMBALL 0x0045af80
CSoundManager::CSoundManager(unsigned int p_musicEnabled,
							 unsigned int p_effectsEnabled,
							 unsigned int p_useMusicCD,
							 int p_deviceParameter,
							 CWnd* p_window)
{
	unsigned int detectedCd;
	unsigned int i;

	if (p_useMusicCD != 0) {
		p_musicEnabled = 0;
	}
	m_background = 1;
	m_requestedMusic = p_musicEnabled;
	m_requestedEffects = p_effectsEnabled;
	m_resourceId = 0;
	m_nextMusicHandle = 1;
	i = 0;
	detectedCd = 0;
	m_useMusicCD = p_useMusicCD;
	m_musicDevice = 0;
	m_deviceCount = MachineSoundDetect(m_devices,
									   p_musicEnabled,
									   p_effectsEnabled,
									   p_useMusicCD,
									   &detectedCd,
									   &m_musicDevice,
									   p_deviceParameter);
	m_musicOutput = 0;
	m_effectOutput = 0;
	if (i < m_deviceCount) {
		do {
			if (((SoundDeviceDispatch*) m_devices[i])->IsMusicAvailable() == 1) {
				m_musicOutput = m_devices[i];
			}
			if (((SoundDeviceDispatch*) m_devices[i])->IsEffectAvailable() == 1) {
				m_effectOutput = m_devices[i];
			}
			++i;
		} while (i < m_deviceCount);
	}
	m_musicAvailable = 0;
	m_effectsAvailable = 0;
	if (m_useMusicCD == 1) {
		if (detectedCd == 0) {
			m_musicAvailable = 0;
			m_musicOutput = 0;
		}
		else {
			m_musicAvailable = 1;
		}
	}
	if (m_requestedEffects != 0) {
		SetEffectsWnd(p_window);
	}
	if (m_requestedMusic != 0) {
		SetMusicWnd(p_window);
	}
	Foreground();
	if (m_musicAvailable == 1) {
		m_musicRequested = 1;
		m_musicCapability = 1;
		if (m_musicOutput != 0) {
			m_musicCapability = ((SoundDeviceDispatch*) m_musicOutput)->Slot2c();
		}
	}
	else {
		m_musicRequested = 0;
		m_musicCapability = 0;
	}
	if (m_effectsAvailable == 1) {
		m_effectsRequested = 1;
		m_effectsCapability = ((SoundDeviceDispatch*) m_effectOutput)->GetBuffersPerEffect();
	}
	else {
		m_effectsRequested = 0;
		m_effectsCapability = 0;
	}
	m_musicState = m_musicAvailable;
	m_effectsState = m_effectsAvailable;
	m_advancedEffects = 1;
	if (m_effectsCapability <= 1) {
		m_advancedEffects = 0;
	}
	m_musicStateCopy = m_musicAvailable;
}

// FUNCTION: LEMBALL 0x0045b110
CSoundManager::~CSoundManager()
{
	unsigned int i;
	CBaseSoundDevice** devices;

	if (m_musicAvailable != 0 && m_musicOutput != 0) {
		((SoundDeviceDispatch*) m_musicOutput)->Slot1c();
	}
	if (m_effectsAvailable != 0 && m_effectOutput != 0) {
		((SoundDeviceDispatch*) m_effectOutput)->Slot20();
	}
	i = 0;
	m_musicOutput = 0;
	m_effectOutput = 0;
	if (i < m_deviceCount) {
		devices = m_devices;
		do {
			if (*devices != 0) {
				(*devices)->SetMasterVolume(0);
				((SoundDeviceDispatch*) *devices)->Slot14();
				delete *devices;
			}
			devices = devices + 1;
			i = i + 1;
		} while (i < m_deviceCount);
	}
	delete m_musicDevice;
}

// FUNCTION: LEMBALL 0x0045b190
void CSoundManager::SetResId(unsigned long p_resourceId)
{
	m_resourceId = p_resourceId;
	if (m_background == 0) {
		if (m_effectOutput != 0) {
			if (((SoundDeviceDispatch*) m_effectOutput)->OpenPrimary(0, m_requestedEffects, p_resourceId) == 0) {
				m_requestedEffects = 0;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0045b1d0
void CSoundManager::Background()
{
	unsigned int i;
	CBaseSoundDevice** devices;

	if (m_background == 0) {
		m_background = 1;
		i = 0;
		if (i < m_deviceCount) {
			devices = m_devices;
			do {
				if (*devices != 0) {
					((SoundDeviceDispatch*) *devices)->Slot14();
				}
				devices = devices + 1;
				i = i + 1;
			} while (i < m_deviceCount);
		}
	}
}

// FUNCTION: LEMBALL 0x0045b210
void CSoundManager::Foreground()
{
	unsigned int music;
	unsigned int effects;
	CBaseSoundDevice* musicOutput;

	if (m_background != 0) {
		music = m_requestedMusic;
		m_background = 0;
		effects = 0;
		musicOutput = m_musicOutput;
		if (m_effectOutput == musicOutput) {
			effects = m_requestedEffects;
			m_effectOutput = 0;
		}
		if (m_useMusicCD == 0 && musicOutput != 0) {
			if (((SoundDeviceDispatch*) musicOutput)->Open(music, effects, m_resourceId) == 0) {
				music = 0;
				effects = 0;
			}
			m_musicAvailable = music;
			m_effectsAvailable = effects;
		}
		if (m_effectOutput != 0) {
			if (((SoundDeviceDispatch*) m_effectOutput)->Open(0, m_requestedEffects, m_resourceId) == 0) {
				m_requestedEffects = 0;
			}
			m_effectsAvailable = m_requestedEffects;
			return;
		}
		else if (m_effectsAvailable != 0) {
			m_effectOutput = m_musicOutput;
		}
	}
}

// FUNCTION: LEMBALL 0x0045b2c0
unsigned long CSoundManager::PrepareMusic(unsigned long p_resourceId, unsigned int p_flags)
{
	return ((MusicDeviceDispatch*) m_musicDevice)->Initialise(p_resourceId, p_flags);
}

// FUNCTION: LEMBALL 0x0045b2e0
unsigned long CSoundManager::PlayMusic(unsigned long p_handle, unsigned long p_unused)
{
	unsigned long allocated;

	if (m_musicAvailable == 1 && m_useMusicCD == 1) {
		allocated = m_nextMusicHandle;
		m_nextMusicHandle = allocated + 1;
		if (m_nextMusicHandle == 0) {
			m_nextMusicHandle = 1;
		}
		((MusicDeviceDispatch*) m_musicDevice)->Prepare(allocated, p_handle);
		return allocated;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045b320
CVSOStream& CSoundManager::StreamOut(CVSOStream& p_stream)
{
	return p_stream;
}

// FUNCTION: LEMBALL 0x0045b330
void CSoundManager::ProcessMusic(unsigned long p_handle)
{
	if (m_musicAvailable == 1) {
		if (p_handle != 0) {
			if (m_useMusicCD == 1) {
				((MusicDeviceDispatch*) m_musicDevice)->Play(p_handle);
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0045b370
void CSoundManager::StopMusic(unsigned long p_handle)
{
	if (m_musicAvailable == 1) {
		if (m_useMusicCD == 1) {
			((MusicDeviceDispatch*) m_musicDevice)->Pause(p_handle);
		}
	}
}

// FUNCTION: LEMBALL 0x0045b390
void CSoundManager::ResumeMusicCd(unsigned long p_handle)
{
	if (m_musicAvailable == 1 && m_useMusicCD == 1) {
		m_musicDevice->Resume(p_handle);
	}
}

// FUNCTION: LEMBALL 0x0045b3b0
void CSoundManager::FreeMusic(unsigned long p_handle)
{
	if (m_musicAvailable == 1) {
		if (m_useMusicCD == 1) {
			((MusicDeviceDispatch*) m_musicDevice)->Stop(p_handle);
		}
	}
}

// FUNCTION: LEMBALL 0x0045b3d0
void CSoundManager::StopMusicCd(unsigned long p_handle)
{
	if (m_musicAvailable == 1 && p_handle != 0 && m_useMusicCD == 1) {
		m_musicDevice->Free(p_handle);
	}
}

// FUNCTION: LEMBALL 0x0045b3f0
unsigned long CSoundManager::PrepareEffect(unsigned long p_resourceId)
{
	unsigned long handle;
	CResEFFECT* effect;
	unsigned char* data;

	if (m_effectsAvailable == 1) {
		effect = CResEFFECT::Load(p_resourceId);
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

// FUNCTION: LEMBALL 0x0045b460
void CSoundManager::PlayEffect(unsigned long p_effectId)
{
	if (m_effectsAvailable == 1) {
		((SoundDeviceDispatch*) m_effectOutput)
			->PlayEffect(p_effectId, ((SoundDeviceDispatch*) m_effectOutput)->QueryVolume(), 0);
	}
}

// FUNCTION: LEMBALL 0x0045b490
void CSoundManager::PlayEffect(unsigned long p_effectId, unsigned int p_channel)
{
	if (m_effectsAvailable == 1) {
		((SoundDeviceDispatch*) m_effectOutput)->PlayEffect(p_effectId, p_channel, 0);
	}
}

// FUNCTION: LEMBALL 0x0045b4f0
void CSoundManager::FreeEffect(unsigned long p_effectId)
{
	if (m_effectsAvailable == 1) {
		((SoundDeviceDispatch*) m_effectOutput)->FreeEffect(p_effectId);
	}
}

// FUNCTION: LEMBALL 0x0045b510
void CSoundManager::SetVolumes(int p_effectVolume, int p_musicVolume)
{
	if (p_effectVolume != -1) {
		if (m_effectOutput != 0) {
			m_effectOutput->SetEffectVolume((unsigned char) p_effectVolume);
		}
	}
	if (p_musicVolume != -1 && m_useMusicCD != 0) {
		m_musicDevice->SetVolume((unsigned char) p_musicVolume);
		return;
	}
	if (p_musicVolume != -1) {
		if (m_musicOutput != 0) {
			m_musicOutput->SetMusicVolume((unsigned char) p_musicVolume);
		}
	}
}

// FUNCTION: LEMBALL 0x0045b560
unsigned char CSoundManager::GetEffectVolume()
{
	if (m_effectOutput != 0) {
		return m_effectOutput->GetEffectVolume();
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045b580
void CSoundManager::SetEffectsWnd(CWnd* p_window)
{
	m_effectOutput->SetWnd(p_window);
}

// FUNCTION: LEMBALL 0x0045b5a0
void CSoundManager::SetMusicWnd(CWnd* p_window)
{
	if (m_musicOutput != 0) {
		m_musicOutput->SetWnd(p_window);
	}
}

// FUNCTION: LEMBALL 0x0045b5c0
void CSoundManager::SetMusicCdPath(char* p_path)
{
	CPVMusicDevice* musicDevice = m_musicDevice;
	musicDevice->m_path = p_path;
	musicDevice->m_usePathPrefix = 1;
}

// FUNCTION: LEMBALL 0x0045b5f0
void CSoundManager::UseMusicCD(unsigned int p_enabled)
{
	m_musicDevice->m_useCdDirectory = p_enabled;
}

#include "../Foundation/CDebugOStream.h"
#include "Visos/Foundation/CVSOStream.h"

// GLOBAL: LEMBALL 0x004a1ca8
char g_szEffectsDriverPrefix[12] = "Effects : ";

// GLOBAL: LEMBALL 0x004a1cb4
char g_szSoundDriverNewline[4] = "\n";

// GLOBAL: LEMBALL 0x004a1cb8
char g_szMusicDriverPrefix[12] = "Music : ";

// FUNCTION: LEMBALL 0x0045b600
char* CSoundManager::BuildDriverInfo()
{
	CDebugOStream stream(g_szSoundDriverInfo, sizeof(g_szSoundDriverInfo));
	g_szSoundDriverInfo[0] = 0;
	if (m_effectOutput != 0 && m_requestedEffects != 0) {
		stream << g_szEffectsDriverPrefix << m_effectOutput->GetInfo();
	}
	if (m_useMusicCD != 0 && m_requestedMusic != 0) {
		stream << m_musicDevice->GetInfo() << g_szSoundDriverNewline;
	}
	if (m_musicOutput != 0 && m_requestedMusic != 0) {
		stream << g_szMusicDriverPrefix << m_musicOutput->GetInfo();
	}
	return g_szSoundDriverInfo;
}

// GLOBAL: LEMBALL 0x004a97c8
char g_szSoundDriverInfo[1024];

// GLOBAL: LEMBALL 0x004a9bc8
CSoundManager* g_pSoundManager = 0;
