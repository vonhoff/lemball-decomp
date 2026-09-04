#include "TargetWaveSoundDevice.h"

#include "../Foundation/VsOStream.h"
#include "TargetWaveEffect.h"

#include <new.h>

#define WIN32_LEAN_AND_MEAN
// clang-format off: mmsystem.h requires the Win32 types declared by windows.h.
#include <windows.h>
#include <mmsystem.h>
// clang-format on

// FUNCTION: LEMBALL 0x0047c880
TargetWaveSoundDevice::TargetWaveSoundDevice(int p_channelCount)
{
	WAVEOUTCAPSA* caps;
	unsigned int i;
	unsigned int deviceCount;
	unsigned int deviceId;
	int found;
	WAVEFORMATEX* format;

	m_channelCount = (unsigned int) p_channelCount;
	m_effects = 0;
	m_effectHandles = 0;
	m_effectUsed = 0;
	if (p_channelCount != 0) {
		m_effects = (TargetWaveEffect**) operator new((unsigned int) p_channelCount * 4);
		m_effectHandles = (unsigned int*) operator new((unsigned int) p_channelCount * 4);
		m_effectUsed = (unsigned int*) operator new((unsigned int) p_channelCount * 4);
	}
	m_musicDevice = 0;
	m_available = 0;
	m_stereo = 0;
	m_use16Bit = 0;
	m_unk0x18 = 0;
	m_deviceId = 0xffffffff;
	m_sampleRate = 0;
	i = 0;
	while (i < 8) {
		m_channelState[i] = 0xffffffff;
		m_pad0x40[i] = 0;
		m_pad0x48[i] = 0;
		i = i + 1;
	}
	m_nextHandle = 1;
	i = 0;
	while (i < m_channelCount) {
		m_effects[i] = 0;
		m_effectUsed[i] = 0;
		m_effectHandles[i] = 0;
		i = i + 1;
	}
	m_waveOut = 0;
	found = 0;
	deviceCount = waveOutGetNumDevs();
	deviceId = 0;
	caps = (WAVEOUTCAPSA*) m_caps;
	while (found == 0 && deviceId < deviceCount) {
		if (waveOutGetDevCapsA(deviceId, caps, sizeof(WAVEOUTCAPSA)) == 0) {
			if ((caps->dwFormats & 1) != 0) {
				m_available = 1;
				m_deviceId = deviceId;
				m_use16Bit = 0;
				m_sampleRate = 0x2b11;
			}
			if ((caps->dwFormats & 4) != 0) {
				m_use16Bit = 1;
				m_available = 1;
				m_sampleRate = 0x2b11;
				m_deviceId = deviceId;
			}
			if ((caps->dwFormats & 0x10) != 0) {
				m_available = 1;
				m_deviceId = deviceId;
				m_use16Bit = 0;
				m_sampleRate = 0x5622;
			}
			if ((caps->dwFormats & 0x40) != 0) {
				m_use16Bit = 1;
				m_available = 1;
				m_sampleRate = 0x5622;
				m_deviceId = deviceId;
			}
		}
		if (m_sampleRate != 0) {
			format = (WAVEFORMATEX*) m_waveFormat;
			format->wFormatTag = 1;
			format->nChannels = 1;
			format->nAvgBytesPerSec = 0;
			format->nBlockAlign = 0;
			format->cbSize = 0;
			if (m_stereo == 1) {
				format->nChannels = 2;
			}
			else {
				format->nChannels = 1;
			}
			if (m_use16Bit == 1) {
				format->wBitsPerSample = 0x10;
			}
			else {
				format->wBitsPerSample = 8;
			}
			format->nSamplesPerSec = m_sampleRate;
			format->nAvgBytesPerSec = 1;
			format->nAvgBytesPerSec = (unsigned int) format->nChannels * m_sampleRate;
			format->nBlockAlign =
				(unsigned short) (((unsigned int) format->wBitsPerSample * (unsigned int) format->nChannels) >> 3);
			if (m_use16Bit == 1) {
				format->nAvgBytesPerSec = format->nAvgBytesPerSec * 2;
			}
			found = 1;
		}
		deviceId = deviceId + 1;
	}
	m_waveOut = 0;
}

// FUNCTION: LEMBALL 0x0047caa0
TargetWaveSoundDevice::~TargetWaveSoundDevice()
{
	Dummy14();
	operator delete(m_effects);
	operator delete(m_effectHandles);
	operator delete(m_effectUsed);
}

// FUNCTION: LEMBALL 0x0047caf0
char* TargetWaveSoundDevice::Dummy04()
{
	WAVEOUTCAPSA* caps;

	if (m_available == 1) {
		caps = (WAVEOUTCAPSA*) m_caps;
		return caps->szPname;
	}
	return "ERROR! No Effects Device for WinEff!\n";
}

// FUNCTION: LEMBALL 0x0047cb00
int TargetWaveSoundDevice::Dummy0c(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId)
{
	MMRESULT result;
	char errorText[0x100];

	if (p_music == 1) {
		return (int) m_musicDevice;
	}
	if (p_effects == 1) {
		result = waveOutOpen((HWAVEOUT*) &m_waveOut, m_deviceId, (WAVEFORMATEX*) m_waveFormat, 0, 0, 0);
		if (result != 0) {
			*g_pErrorOutput << "Error! Windows Effect device cannot be opened!\n";
			waveOutGetErrorTextA(result, errorText, 0x100);
			*g_pErrorOutput << errorText << "\n";
		}
		if ((m_caps[0x30] & 4) != 0) {
			waveOutGetVolume((HWAVEOUT) m_waveOut, (DWORD*) &m_savedVolume);
			waveOutSetVolume((HWAVEOUT) m_waveOut, 0xffffffff);
		}
		if (result == 0) {
			m_available = 1;
			return 1;
		}
		m_available = 0;
		return 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0047cbe0
int TargetWaveSoundDevice::Dummy10(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0047cbf0
int TargetWaveSoundDevice::Dummy2c()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047cc00
int TargetWaveSoundDevice::Dummy30()
{
	return 1;
}

// FUNCTION: LEMBALL 0x0047cc10
int TargetWaveSoundDevice::Dummy08()
{
	return (int) m_available;
}

// FUNCTION: LEMBALL 0x0047cc20
int TargetWaveSoundDevice::Dummy14()
{
	MMRESULT result;
	unsigned int tries;
	char errorText[0x100];

	if (m_waveOut != 0) {
		result = 0xffff;
		tries = 0;
		do {
			if (tries >= 500) {
				break;
			}
			tries = tries + 1;
			result = waveOutReset((HWAVEOUT) m_waveOut);
		} while (result != 0);
		if (tries == 500) {
			*g_pErrorOutput << "Error Shutting Down Wave Device : ";
			*g_pErrorOutput << Dummy04() << ".\n";
			*g_pErrorOutput << "System may be unstable!\n";
			waveOutGetErrorTextA(result, errorText, 0x100);
			*g_pErrorOutput << errorText << "\n";
			return 0;
		}
		if ((m_caps[0x30] & 4) != 0) {
			waveOutSetVolume((HWAVEOUT) m_waveOut, m_savedVolume);
		}
		if (m_waveOut != 0) {
			result = 0xffff;
			tries = 0;
			do {
				if (tries >= 500) {
					break;
				}
				tries = tries + 1;
				result = waveOutClose((HWAVEOUT) m_waveOut);
			} while (result != 0);
			if (tries == 500) {
				*g_pErrorOutput << "Error Closing Down Wave Device : ";
				*g_pErrorOutput << Dummy04() << ".\n";
				*g_pErrorOutput << "System may be unstable!\n";
				waveOutGetErrorTextA(result, errorText, 0x100);
				*g_pErrorOutput << errorText << "\n";
				return 0;
			}
		}
	}
	m_waveOut = 0;
	return 1;
}

// FUNCTION: LEMBALL 0x0047cdd0
int TargetWaveSoundDevice::Dummy18()
{
	unsigned int i;
	int playing;

	playing = 0;
	i = 0;
	while (i < 8) {
		if (m_pad0x48[i] == 1) {
			playing = 1;
		}
		i = i + 1;
	}
	return playing;
}

// FUNCTION: LEMBALL 0x0047cdf0
int TargetWaveSoundDevice::Dummy1c()
{
	return 1;
}

// FUNCTION: LEMBALL 0x0047ce00
int TargetWaveSoundDevice::Dummy20()
{
	MMRESULT result;
	unsigned int tries;
	char errorText[0x100];

	if (m_waveOut != 0) {
		result = 0xffff;
		tries = 0;
		do {
			if (tries >= 500) {
				break;
			}
			tries = tries + 1;
			result = waveOutReset((HWAVEOUT) m_waveOut);
		} while (result != 0);
		if (tries == 500) {
			*g_pErrorOutput << "Error stopping playback in device : ";
			*g_pErrorOutput << Dummy04() << ".\n";
			*g_pErrorOutput << "System may be unstable!\n";
			waveOutGetErrorTextA(result, errorText, 0x100);
			*g_pErrorOutput << errorText << "\n";
			return 0;
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0047ced0
int TargetWaveSoundDevice::Dummy24()
{
	return (int) m_musicDevice;
}

// FUNCTION: LEMBALL 0x0047cee0
int TargetWaveSoundDevice::Dummy28()
{
	return (int) m_available;
}

// FUNCTION: LEMBALL 0x0047cef0
int TargetWaveSoundDevice::Dummy34(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2, undefined4 p_arg3)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047cf00
int TargetWaveSoundDevice::Dummy38(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2, undefined4 p_arg3)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047cf10
int TargetWaveSoundDevice::Dummy3c(unsigned char* p_data, unsigned long* p_handle)
{
	void* storage;

	for (unsigned int i = 0; i < m_channelCount; i++) {
		if (m_effectUsed[i] == 0) {
			Dummy14();
			storage = operator new(0x18);
			if (storage == 0) {
				m_effects[i] = 0;
			}
			else {
				m_effects[i] = new (storage)
					TargetWaveEffect(p_data, (unsigned int) m_waveOut, m_sampleRate, (int) m_use16Bit, (int) m_stereo);
			}
			Dummy0c(0, 1, 0);
			m_effectUsed[i] = 1;
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
int TargetWaveSoundDevice::Dummy40(undefined4 p_arg0)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047cff0
int TargetWaveSoundDevice::Dummy44(undefined4 p_arg0)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047d000
int TargetWaveSoundDevice::Dummy4c()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047d010
int TargetWaveSoundDevice::Dummy48(unsigned long p_effectId)
{
	TargetWaveEffect* effect;
	unsigned int i;
	TargetWaveSoundDevice* device;

	i = 0;
	while (i < m_channelCount) {
		device = this;
		if (device->m_effectHandles[i] == p_effectId) {
			effect = device->m_effects[i];
			if (effect != 0) {
				effect->~TargetWaveEffect();
				operator delete(effect);
			}
			device->m_effectUsed[i] = 0;
			device->m_effectHandles[i] = 0;
		}
		i = i + 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0047d080
int TargetWaveSoundDevice::Dummy50()
{
	unsigned int i;
	TargetWaveEffect* effect;

	i = 0;
	while (i < m_channelCount) {
		if (m_effectUsed[i] == 1) {
			effect = m_effects[i];
			if (effect != 0) {
				effect->~TargetWaveEffect();
				operator delete(effect);
			}
			m_effectUsed[i] = 0;
			m_effectHandles[i] = 0;
		}
		i = i + 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0047d160
unsigned char TargetWaveSoundDevice::EffectPlay(unsigned long p_effectId,
												unsigned short p_pitch,
												unsigned char p_volume)
{
	unsigned int i;
	TargetWaveEffect* effect;
	MMRESULT result;

	i = 0;
	while (i < m_channelCount) {
		if (m_effectHandles[i] == p_effectId) {
			result = waveOutReset((HWAVEOUT) m_waveOut);
			if (result != 0) {
				*g_pErrorOutput << "waveOutReset errored: " << (unsigned long) result << "\n";
			}
			effect = m_effects[i];
			result = waveOutWrite((HWAVEOUT) m_waveOut, (WAVEHDR*) effect->m_waveHeader, 0x20);
			if (result != 0) {
				*g_pErrorOutput << "waveOutWrite (play effect) errored: " << (unsigned long) result << "\n";
			}
		}
		i = i + 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0047d220
unsigned char TargetWaveSoundDevice::EffectPlay(unsigned long p_effectId,
												unsigned char p_channel,
												unsigned char p_volume)
{
	BaseSoundDevice* device;

	device = this;
	return device->EffectPlay(p_effectId, (unsigned short) 0xff00, p_volume);
}

// FUNCTION: LEMBALL 0x0047d240
bool TargetWaveSoundDevice::EffectStop(unsigned char p_channel, unsigned char p_effect)
{
	waveOutReset((HWAVEOUT) m_waveOut);
	return 0;
}
