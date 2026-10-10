#include "CSoundManager.h"

#include "CBaseSoundDevice.h"
#include "CPVMusicDevice.h"
#include "Engine/Resources/Types/CResEFFECT.h"
#include "Engine/Strings/CString.h"
#include "VsSound.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045af80
CSoundManager::CSoundManager(unsigned int p_musicEnabled,
							 unsigned int p_effectsEnabled,
							 unsigned int p_useMusicCD,
							 int p_effectCapacity,
							 CWnd* p_window)
{
	unsigned int detectedCd;
	unsigned int i;

	if (p_useMusicCD != 0) {
		p_musicEnabled = 0;
	}
	m_background = true;
	m_requestedMusic = p_musicEnabled;
	m_requestedEffects = p_effectsEnabled;
	m_resourceId = 0;
	m_nextMusicHandle = 1;
	i = 0;
	detectedCd = 0;
	m_useMusicCD = p_useMusicCD;
	m_musicDevice = NULL;
	m_deviceCount = MachineSoundDetect(m_devices,
									   p_musicEnabled,
									   p_effectsEnabled,
									   p_useMusicCD,
									   &detectedCd,
									   &m_musicDevice,
									   p_effectCapacity);
	m_musicOutput = NULL;
	m_effectOutput = NULL;
	if (i < m_deviceCount) {
		do {
			if (m_devices[i]->IsMusicAvailable() == 1) {
				m_musicOutput = m_devices[i];
			}
			if (m_devices[i]->IsEffectAvailable() == 1) {
				m_effectOutput = m_devices[i];
			}
			++i;
		} while (i < m_deviceCount);
	}
	m_musicAvailable = false;
	m_effectsAvailable = false;
	if (m_useMusicCD == true) {
		if (detectedCd == 0) {
			m_musicAvailable = false;
			m_musicOutput = NULL;
		}
		else {
			m_musicAvailable = true;
		}
	}
	if (m_requestedEffects != false) {
		SetEffectsWnd(p_window);
	}
	if (m_requestedMusic != false) {
		SetMusicWnd(p_window);
	}
	Foreground();
	if (m_musicAvailable == true) {
		m_musicRequested = 1;
		m_musicCapability = 1;
		if (m_musicOutput != NULL) {
			m_musicCapability = m_musicOutput->Dummy2c();
		}
	}
	else {
		m_musicRequested = 0;
		m_musicCapability = 0;
	}
	if (m_effectsAvailable == true) {
		m_effectsRequested = 1;
		m_effectsCapability = m_effectOutput->GetBuffersPerEffect();
	}
	else {
		m_effectsRequested = 0;
		m_effectsCapability = 0;
	}
	m_musicState = m_musicAvailable;
	m_effectsState = m_effectsAvailable;
	m_advancedEffects = true;
	if (m_effectsCapability <= 1) {
		m_advancedEffects = false;
	}
	m_musicStateCopy = m_musicAvailable;
}

// FUNCTION: LEMBALL 0x0045b110
CSoundManager::~CSoundManager()
{
	unsigned int i;

	if (m_musicAvailable != false && m_musicOutput != NULL) {
		m_musicOutput->Dummy1c();
	}
	if (m_effectsAvailable != false && m_effectOutput != NULL) {
		m_effectOutput->StopAllEffects();
	}
	i = 0;
	m_musicOutput = NULL;
	m_effectOutput = NULL;
	if (i < m_deviceCount) {
		do {
			if (m_devices[i] != NULL) {
				m_devices[i]->SetMasterVolume(0);
				m_devices[i]->Close();
				delete m_devices[i];
			}
			i = i + 1;
		} while (i < m_deviceCount);
	}
	delete m_musicDevice;
}

// FUNCTION: LEMBALL 0x0045b190
void CSoundManager::SetResId(unsigned long p_resourceId)
{
	m_resourceId = p_resourceId;
	if (m_background == false) {
		if (m_effectOutput != NULL) {
			if (m_effectOutput->Dummy10(0, m_requestedEffects, p_resourceId) == 0) {
				m_requestedEffects = false;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0045b1d0
void CSoundManager::Background()
{
	unsigned int i;
	CBaseSoundDevice** devices;

	if (m_background == false) {
		m_background = true;
		i = 0;
		if (i < m_deviceCount) {
			devices = m_devices;
			do {
				if (*devices != NULL) {
					(*devices)->Close();
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
	bool music;
	bool effects;
	CBaseSoundDevice* musicOutput;

	if (m_background != false) {
		music = m_requestedMusic;
		m_background = false;
		effects = false;
		musicOutput = m_musicOutput;
		if (m_effectOutput == musicOutput) {
			effects = m_requestedEffects;
			m_effectOutput = NULL;
		}
		if (m_useMusicCD == false && musicOutput != NULL) {
			if (musicOutput->Open(music, effects, m_resourceId) == 0) {
				music = false;
				effects = false;
			}
			m_musicAvailable = music;
			m_effectsAvailable = effects;
		}
		if (m_effectOutput != NULL) {
			if (m_effectOutput->Open(0, m_requestedEffects, m_resourceId) == 0) {
				m_requestedEffects = false;
			}
			m_effectsAvailable = m_requestedEffects;
			return;
		}
		else if (m_effectsAvailable != false) {
			m_effectOutput = m_musicOutput;
		}
	}
}

// FUNCTION: LEMBALL 0x0045b2c0
void CSoundManager::PrepareMusic(unsigned long p_unused1, unsigned int p_unused2)
{
	m_musicDevice->Initialise(p_unused1, p_unused2);
}

// FUNCTION: LEMBALL 0x0045b2e0
unsigned long CSoundManager::PlayMusic(unsigned long p_resourceId, unsigned long p_unused)
{
	unsigned long allocated;

	if (m_musicAvailable == true && m_useMusicCD == true) {
		allocated = m_nextMusicHandle;
		m_nextMusicHandle = allocated + 1;
		if (m_nextMusicHandle == 0) {
			m_nextMusicHandle = 1;
		}
		m_musicDevice->Prepare(allocated, p_resourceId);
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
	if (m_musicAvailable == true) {
		if (p_handle != 0) {
			if (m_useMusicCD == true) {
				m_musicDevice->Play(p_handle);
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0045b370
void CSoundManager::StopMusic(unsigned long p_handle)
{
	if (m_musicAvailable == true) {
		if (m_useMusicCD == true) {
			m_musicDevice->Pause(p_handle);
		}
	}
}

// FUNCTION: LEMBALL 0x0045b390
void CSoundManager::ResumeMusicCd(unsigned long p_handle)
{
	if (m_musicAvailable == true && m_useMusicCD == true) {
		m_musicDevice->Resume(p_handle);
	}
}

// FUNCTION: LEMBALL 0x0045b3b0
void CSoundManager::FreeMusic(unsigned long p_handle)
{
	if (m_musicAvailable == true) {
		if (m_useMusicCD == true) {
			m_musicDevice->Stop(p_handle);
		}
	}
}

// FUNCTION: LEMBALL 0x0045b3d0
void CSoundManager::StopMusicCd(unsigned long p_handle)
{
	if (m_musicAvailable == true && p_handle != 0 && m_useMusicCD == true) {
		m_musicDevice->Free(p_handle);
	}
}

// FUNCTION: LEMBALL 0x0045b3f0
unsigned long CSoundManager::PrepareEffect(unsigned long p_resourceId)
{
	unsigned long handle;
	CResEFFECT* effect;
	unsigned char* data;

	if (m_effectsAvailable == true) {
		effect = CResEFFECT::Load(p_resourceId);
		if (effect->m_loaded != 0) {
			effect->m_age = 0;
		}
		else {
			effect->LoadData();
		}
		effect->m_directUseCount = effect->m_directUseCount + 1;
		data = effect->GetData();
		m_effectOutput->PrepareEffect(data, &handle);
		effect->m_directUseCount = effect->m_directUseCount - 1;
		effect->UnLoad();
		return handle;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045b460
void CSoundManager::PlayEffect(unsigned long p_effectId)
{
	if (m_effectsAvailable == true) {
		m_effectOutput->EffectPlay(p_effectId, m_effectOutput->GetEffectVolume(), 0);
	}
}

// FUNCTION: LEMBALL 0x0045b490
void CSoundManager::PlayEffect(unsigned long p_effectId, unsigned int p_channel)
{
	if (m_effectsAvailable == true) {
		m_effectOutput->EffectPlay(p_effectId, (unsigned char) p_channel, 0);
	}
}

// FUNCTION: LEMBALL 0x0045b4f0
void CSoundManager::FreeEffect(unsigned long p_effectId)
{
	if (m_effectsAvailable == true) {
		m_effectOutput->FreeEffect(p_effectId);
	}
}

// FUNCTION: LEMBALL 0x0045b510
void CSoundManager::SetVolumes(int p_effectVolume, int p_musicVolume)
{
	if (p_effectVolume != SOUND_VOLUME_UNCHANGED) {
		if (m_effectOutput != NULL) {
			m_effectOutput->SetEffectVolume((unsigned char) p_effectVolume);
		}
	}
	if (p_musicVolume != SOUND_VOLUME_UNCHANGED && m_useMusicCD != false) {
		m_musicDevice->SetVolume((unsigned char) p_musicVolume);
		return;
	}
	if (p_musicVolume != SOUND_VOLUME_UNCHANGED) {
		if (m_musicOutput != NULL) {
			m_musicOutput->SetMusicVolume((unsigned char) p_musicVolume);
		}
	}
}

// FUNCTION: LEMBALL 0x0045b560
unsigned char CSoundManager::GetEffectVolume()
{
	if (m_effectOutput != NULL) {
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
	if (m_musicOutput != NULL) {
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

#include "Engine/Diagnostics/CDebugOStream.h"
#include "Engine/Streams/CVSOStream.h"

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
	if (m_effectOutput != NULL && m_requestedEffects != false) {
		stream << g_szEffectsDriverPrefix << m_effectOutput->GetInfo();
	}
	if (m_useMusicCD != false && m_requestedMusic != false) {
		stream << m_musicDevice->GetInfo() << g_szSoundDriverNewline;
	}
	if (m_musicOutput != NULL && m_requestedMusic != false) {
		stream << g_szMusicDriverPrefix << m_musicOutput->GetInfo();
	}
	return g_szSoundDriverInfo;
}

// GLOBAL: LEMBALL 0x004a97c8
char g_szSoundDriverInfo[1024];

// GLOBAL: LEMBALL 0x004a9bc8
CSoundManager* g_pSoundManager;
