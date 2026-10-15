#include "CWaveSoundDevice.h"

#include <stddef.h>

#include <windows.h>
#include <mmsystem.h>

#include "CWaveEffect.h"
#include "Engine/Sound/CBaseSoundDevice.h"
#include "Engine/Streams/CVSOStream.h"

const int WAVE_CHANNEL_COUNT = 8;
const int WAVE_LOW_SAMPLE_RATE = 11025;
const int WAVE_HIGH_SAMPLE_RATE = 22050;
const int WAVE_DEVICE_RETRY_LIMIT = 500;
const int WAVE_RESULT_UNSET = 0xffff;
const unsigned int WAVE_FULL_VOLUME = 0xffffffff;

// FUNCTION: LEMBALL 0x0047c880
CWaveSoundDevice::CWaveSoundDevice(int p_channelCount)
{
	unsigned int i;
	unsigned int channel;
	UINT deviceId;
	UINT deviceCount;
	int found;

	m_channelCount = (unsigned int) p_channelCount;
	m_effects = new CWaveEffect*[p_channelCount];
	m_effectHandles = new unsigned long[p_channelCount];
	m_effectUsed = new bool[p_channelCount];
	m_musicDevice = 0;
	m_available = false;
	m_stereo = false;
	m_use16Bit = false;
	m_unk0x18 = 0;
	m_deviceId = WAVE_MAPPER;
	m_sampleRate = 0;
	for (i = 0; i < WAVE_CHANNEL_COUNT; i++) {
		m_channelState[i] = 0xffffffff;
		m_pad0x40[i] = 0;
		m_effectPlaying[i] = 0;
	}
	m_nextHandle = 1;
	for (channel = 0; channel < m_channelCount; channel++) {
		m_effects[channel] = NULL;
		m_effectUsed[channel] = false;
		m_effectHandles[channel] = 0;
	}
	found = 0;
	deviceCount = waveOutGetNumDevs();
	if (deviceCount == 0) {
		return;
	}
	deviceId = 0;
	while (deviceId < deviceCount) {
		if (found != 0) {
			return;
		}
		if (waveOutGetDevCapsA(deviceId, &m_caps, sizeof(WAVEOUTCAPSA)) == 0) {
			if ((m_caps.dwFormats & WAVE_FORMAT_1M08) == WAVE_FORMAT_1M08) {
				m_available = true;
				m_deviceId = deviceId;
				m_use16Bit = false;
				m_sampleRate = WAVE_LOW_SAMPLE_RATE;
			}
			if ((m_caps.dwFormats & WAVE_FORMAT_1M16) != 0) {
				m_use16Bit = true;
				m_available = true;
				m_sampleRate = WAVE_LOW_SAMPLE_RATE;
				m_deviceId = deviceId;
			}
			if ((m_caps.dwFormats & WAVE_FORMAT_2M08) != 0) {
				m_available = true;
				m_deviceId = deviceId;
				m_use16Bit = false;
				m_sampleRate = WAVE_HIGH_SAMPLE_RATE;
			}
			if ((m_caps.dwFormats & WAVE_FORMAT_2M16) != 0) {
				m_use16Bit = true;
				m_available = true;
				m_sampleRate = WAVE_HIGH_SAMPLE_RATE;
				m_deviceId = deviceId;
			}
		}
		if (m_sampleRate != 0) {
			m_waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			if (m_stereo == 1) {
				m_waveFormat.nChannels = 2;
			}
			else {
				m_waveFormat.nChannels = 1;
			}
			if (m_use16Bit == 1) {
				m_waveFormat.wBitsPerSample = PCM_SAMPLE_BITS_16;
			}
			else {
				m_waveFormat.wBitsPerSample = PCM_SAMPLE_BITS_8;
			}
			m_waveFormat.nSamplesPerSec = m_sampleRate;
			m_waveFormat.nAvgBytesPerSec = 1;
			m_waveFormat.nBlockAlign = (unsigned short) ((m_waveFormat.wBitsPerSample * m_waveFormat.nChannels) / 8);
			m_waveFormat.nAvgBytesPerSec = (unsigned int) m_waveFormat.nChannels * m_sampleRate;
			if (m_use16Bit == 1) {
				m_waveFormat.nAvgBytesPerSec = m_waveFormat.nAvgBytesPerSec * 2;
			}
			found = 1;
		}
		deviceId = deviceId + 1;
	}
}

// FUNCTION: LEMBALL 0x0047caa0
CWaveSoundDevice::~CWaveSoundDevice()
{
	ShutDown();
	delete[] m_effects;
	delete[] m_effectHandles;
	delete[] m_effectUsed;
}

// FUNCTION: LEMBALL 0x0047caf0
char* CWaveSoundDevice::GetDeviceInfo()
{
	WAVEOUTCAPSA* caps;

	if (m_available == 1) {
		caps = &m_caps;
		return caps->szPname;
	}
	return "ERROR! No Effects Device for WinEff!\n";
}

// FUNCTION: LEMBALL 0x0047cb00
int CWaveSoundDevice::Initialise(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId)
{
	MMRESULT result;
	char errorText[MAXERRORLENGTH];

	if (p_music == 1) {
		return (int) m_musicDevice;
	}
	if (p_effects == 1) {
		result = waveOutOpen(&m_waveOut, m_deviceId, &m_waveFormat, 0, 0, 0);
		if (result != 0) {
			*g_pErrorOutput << "Error! Windows Effect device cannot be opened!\n";
			waveOutGetErrorTextA(result, errorText, sizeof(errorText));
			*g_pErrorOutput << errorText << "\n";
		}
		if ((m_caps.dwSupport & WAVECAPS_VOLUME) != 0) {
			waveOutGetVolume(m_waveOut, &m_savedVolume);
			waveOutSetVolume(m_waveOut, WAVE_FULL_VOLUME);
		}
		if (result == 0) {
			m_available = true;
			return 1;
		}
		m_available = false;
		return 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0047cbe0
int CWaveSoundDevice::Reinitialise(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0047cbf0
int CWaveSoundDevice::GetNumMusicChannels()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047cc00
int CWaveSoundDevice::GetNumEffectsChannels()
{
	return 1;
}

// FUNCTION: LEMBALL 0x0047cc10
int CWaveSoundDevice::Detect()
{
	return (int) m_available;
}

// FUNCTION: LEMBALL 0x0047cc20
int CWaveSoundDevice::ShutDown()
{
	MMRESULT result;
	unsigned int tries;
	char errorText[MAXERRORLENGTH];

	if (m_waveOut != NULL) {
		result = WAVE_RESULT_UNSET;
		tries = 0;
		do {
			if (tries >= WAVE_DEVICE_RETRY_LIMIT) {
				break;
			}
			result = waveOutReset(m_waveOut);
			tries = tries + 1;
		} while (result != 0);
		if (tries == WAVE_DEVICE_RETRY_LIMIT) {
			*g_pErrorOutput << "Error Shutting Down Wave Device : ";
			*g_pErrorOutput << GetDeviceInfo() << ".\n";
			*g_pErrorOutput << "System may be unstable!\n";
			waveOutGetErrorTextA(result, errorText, sizeof(errorText));
			*g_pErrorOutput << errorText << "\n";
			return 0;
		}
		if ((m_caps.dwSupport & WAVECAPS_VOLUME) != 0) {
			waveOutSetVolume(m_waveOut, m_savedVolume);
		}
		if (m_waveOut != NULL) {
			result = WAVE_RESULT_UNSET;
			tries = 0;
			do {
				if (tries >= WAVE_DEVICE_RETRY_LIMIT) {
					break;
				}
				result = waveOutClose(m_waveOut);
				tries = tries + 1;
			} while (result != 0);
			if (tries == WAVE_DEVICE_RETRY_LIMIT) {
				*g_pErrorOutput << "Error Closing Down Wave Device : ";
				*g_pErrorOutput << GetDeviceInfo() << ".\n";
				*g_pErrorOutput << "System may be unstable!\n";
				waveOutGetErrorTextA(result, errorText, sizeof(errorText));
				*g_pErrorOutput << errorText << "\n";
				return 0;
			}
		}
	}
	m_waveOut = NULL;
	return 1;
}

// FUNCTION: LEMBALL 0x0047cdd0
int CWaveSoundDevice::HasActiveVoices()
{
	unsigned int i;
	int playing;

	playing = 0;
	i = 0;
	while (i < WAVE_CHANNEL_COUNT) {
		if (m_effectPlaying[i] == 1) {
			playing = 1;
		}
		i = i + 1;
	}
	return playing;
}

// FUNCTION: LEMBALL 0x0047cdf0
int CWaveSoundDevice::StopAllMusic()
{
	return 1;
}

// FUNCTION: LEMBALL 0x0047ce00
int CWaveSoundDevice::StopAllEffects()
{
	MMRESULT result;
	unsigned int tries;
	char errorText[MAXERRORLENGTH];

	if (m_waveOut != NULL) {
		result = WAVE_RESULT_UNSET;
		tries = 0;
		do {
			if (tries >= WAVE_DEVICE_RETRY_LIMIT) {
				break;
			}
			result = waveOutReset(m_waveOut);
			tries = tries + 1;
		} while (result != 0);
		if (tries == WAVE_DEVICE_RETRY_LIMIT) {
			*g_pErrorOutput << "Error stopping playback in device : ";
			*g_pErrorOutput << GetDeviceInfo() << ".\n";
			*g_pErrorOutput << "System may be unstable!\n";
			waveOutGetErrorTextA(result, errorText, sizeof(errorText));
			*g_pErrorOutput << errorText << "\n";
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0047ced0
int CWaveSoundDevice::IsMusicCapable()
{
	return (int) m_musicDevice;
}

// FUNCTION: LEMBALL 0x0047cee0
int CWaveSoundDevice::IsEffectCapable()
{
	return (int) m_available;
}

// FUNCTION: LEMBALL 0x0047cef0
int CWaveSoundDevice::AllocatePatch(VSPATCH* p_patch, unsigned long* p_handle, unsigned char, unsigned char)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047cf00
int CWaveSoundDevice::AllocateDrumPatch(VSPATCH* p_patch, unsigned long* p_handle, unsigned char, unsigned char)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047cf10
int CWaveSoundDevice::AllocateEffect(unsigned char* p_data, unsigned long* p_handle)
{
	for (unsigned int i = 0; i < m_channelCount; i++) {
		if (m_effectUsed[i] == 0) {
			ShutDown();
			m_effects[i] = new CWaveEffect(p_data, m_waveOut, m_sampleRate, m_use16Bit, m_stereo);
			Initialise(0, 1, 0);
			m_effectUsed[i] = true;
			m_effectHandles[i] = m_nextHandle;
			*p_handle = m_nextHandle;
			m_nextHandle = m_nextHandle + 1;
			if (m_nextHandle == 0) {
				m_nextHandle = m_nextHandle + 1;
			}
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0047cfe0
int CWaveSoundDevice::FreePatch(unsigned long p_patchHandle)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047cff0
int CWaveSoundDevice::FreeDrumPatch(unsigned long p_patchHandle)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047d000
int CWaveSoundDevice::FreeAllPatches()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047d010
int CWaveSoundDevice::FreeEffect(unsigned long p_effectId)
{
	unsigned int channelIndex = 0;
	while (channelIndex < m_channelCount) {
		if (m_effectHandles[channelIndex] == p_effectId) {
			delete m_effects[channelIndex];
			m_effectUsed[channelIndex] = false;
			m_effectHandles[channelIndex] = 0;
		}
		channelIndex = channelIndex + 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0047d080
int CWaveSoundDevice::FreeAllEffects()
{
	unsigned int i = 0;
	if (i < m_channelCount) {
		do {
			if (m_effectUsed[i] == 1) {
				delete m_effects[i];
				m_effectUsed[i] = false;
				m_effectHandles[i] = 0;
			}
			i = i + 1;
		} while (i < m_channelCount);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0047d0f0
unsigned char CWaveSoundDevice::GetMasterVolume()
{
	enum {
		SOUND_VOLUME_MAX = 0xff
	};
	return SOUND_VOLUME_MAX;
}

// FUNCTION: LEMBALL 0x0047d100
void CWaveSoundDevice::SetMasterVolume(unsigned char p_volume)
{
}

// FUNCTION: LEMBALL 0x0047d110
unsigned char CWaveSoundDevice::GetMusicVolume()
{
	enum {
		SOUND_VOLUME_MAX = 0xff
	};
	return SOUND_VOLUME_MAX;
}

// FUNCTION: LEMBALL 0x0047d120
void CWaveSoundDevice::SetMusicVolume(unsigned char p_volume)
{
}

// FUNCTION: LEMBALL 0x0047d130
unsigned char CWaveSoundDevice::GetEffectVolume()
{
	enum {
		SOUND_VOLUME_MAX = 0xff
	};
	return SOUND_VOLUME_MAX;
}

// FUNCTION: LEMBALL 0x0047d140
void CWaveSoundDevice::SetEffectVolume(unsigned char p_volume)
{
}

// FUNCTION: LEMBALL 0x0047d150
bool CWaveSoundDevice::SetVolume(unsigned long p_resourceId, int p_index, unsigned char p_volume)
{
	return false;
}

// FUNCTION: LEMBALL 0x0047d160
unsigned char CWaveSoundDevice::EffectPlay(unsigned long p_effectId, unsigned short p_pitch, int p_loop)
{
	unsigned int i;
	MMRESULT result;

	i = 0;
	if (i < m_channelCount) {
		do {
			if (m_effectHandles[i] == p_effectId) {
				result = waveOutReset(m_waveOut);
				if (result != 0) {
					*g_pErrorOutput << "waveOutReset errored: " << (unsigned int) result << "\n";
				}
				result = waveOutWrite(m_waveOut, m_effects[i]->m_waveHeader, sizeof(*m_effects[i]->m_waveHeader));
				if (result != 0) {
					*g_pErrorOutput << "waveOutWrite (play effect) errored: " << (unsigned int) result << "\n";
				}
			}
			i = i + 1;
		} while (i < m_channelCount);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0047d220
unsigned char CWaveSoundDevice::EffectPlay(unsigned long p_effectId, unsigned char p_volume, int p_loop)
{
	CBaseSoundDevice* device;

	device = this;
	return device->EffectPlay(p_effectId, (unsigned short) 0xff00, p_loop);
}

// FUNCTION: LEMBALL 0x0047d240
bool CWaveSoundDevice::EffectStop(unsigned char p_channel, unsigned char p_effect)
{
	waveOutReset(m_waveOut);
	return false;
}
