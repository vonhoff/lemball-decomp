#include "TargetDirectSoundDevice.h"

#include "../Foundation/VsOStream.h"
#include "../Graphics/Wnd.h"
#include "IDirectSound.h"
#include "TargetDirectSoundEffect.h"

#include <memory.h>
#include <new.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winver.h>

#pragma intrinsic(memset, memcpy, strlen)

// GLOBAL: LEMBALL 0x004a3318
static IDirectSoundBuffer* g_primarySoundBuffer = 0;

// GLOBAL: LEMBALL 0x004a331c
static IDirectSound* g_directSound = 0;

// FUNCTION: LEMBALL 0x0047dd80
TargetDirectSoundDevice::TargetDirectSoundDevice(int p_effectCapacity, int p_buffersPerEffect)
{
	char path[256];
	DWORD versionHandle;
	unsigned int versionLength;
	void* versionInfo;
	unsigned long versionLow;
	int valid;

	m_platform.m_effectCapacity = p_effectCapacity;
	m_platform.m_buffersPerEffect = p_buffersPerEffect;
	m_platform.m_nativeWindow = 0;
	m_platform.m_effects = (TargetDirectSoundEffect**) operator new(p_effectCapacity * 4 + 4);
	m_platform.m_open = 0;
	m_platform.m_musicAvailable = 0;
	m_platform.m_available = 0;
	m_platform.m_unk0x24 = 0;
	m_platform.m_unk0x28 = 0;
	m_platform.m_unk0x2c = 0;
	m_platform.m_sampleRate = 0;
	m_platform.m_unk0x38 = 0xffffffff;
	for (int i = 1; i <= m_platform.m_effectCapacity; i++) {
		m_platform.m_effects[i] = 0;
	}
	m_platform.m_sampleRate = 0x5622;
	m_platform.m_unk0x38 = 0;
	m_platform.m_samplesPerSecond = 0x5622;
	m_platform.m_extraFormatBytes = 0;
	m_platform.m_bitsPerSample = 16;
	m_platform.m_blockAlign = 2;
	m_platform.m_formatTag = 1;
	m_platform.m_unk0x28 = 1;
	m_platform.m_channels = 1;
	m_platform.m_averageBytesPerSecond = 1;
	m_platform.m_averageBytesPerSecond *= m_platform.m_samplesPerSecond * m_platform.m_blockAlign;
	m_platform.m_library = LoadLibraryA("DSOUND.DLL");
	if (m_platform.m_library != 0) {
		if (GetSystemDirectoryA(path, sizeof(path)) != 0) {
			memcpy(path + strlen(path), "\\DSOUND.DLL", 12);
			DWORD size = GetFileVersionInfoSizeA(path, &versionHandle);
			void* data = operator new(size);
			valid = GetFileVersionInfoA(path, 0, size, data);
			if (valid != 0) {
				VerQueryValueA(data, "\\", &versionInfo, &versionLength);
				unsigned long versionHigh = ((VS_FIXEDFILEINFO*) versionInfo)->dwFileVersionMS;
				versionLow = ((VS_FIXEDFILEINFO*) versionInfo)->dwFileVersionLS;
				unsigned short major = (unsigned short) (versionHigh >> 16);
				unsigned short minor = (unsigned short) versionHigh;
				*g_pDebugOutput << "DSOUND version " << (unsigned int) major << "." << (unsigned int) minor << "."
								<< versionLow << "\n";
				if (major >= 4 && (major != 4 || minor >= 2)) {
					valid = 1;
				}
				else {
					valid = 0;
					*g_pErrorOutput << "DSOUND version too old\n";
					*g_pErrorOutput << "Defaulting to windows sound device\n";
				}
			}
			operator delete(data);
			if (valid) {
				m_platform.m_createDirectSound = (long(__stdcall*)(const void*, IDirectSound**, void*)) GetProcAddress(
					(HMODULE) m_platform.m_library,
					"DirectSoundCreate");
				if (m_platform.m_createDirectSound != 0) {
					m_platform.m_available = 1;
					m_platform.m_open = 0;
				}
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0047e000
char* TargetDirectSoundDevice::GetInfo()
{
	return m_platform.m_available == 1 ? "Direct Sound Device\n" : "ERROR! No Effects Device for WinEff!\n";
}

// FUNCTION: LEMBALL 0x0047e020
int TargetDirectSoundDevice::Open(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId)
{
	unsigned int description[5];
	unsigned int result;

	result = m_platform.m_createDirectSound(0, &g_directSound, 0);
	if (result != 0) {
		*g_pErrorOutput << "Direct Sound Create failed: " << TargetDescribeDirectSoundError(result & 0xfff) << "\n";
		m_platform.m_available = 0;
		return 0;
	}
	memset(description, 0, sizeof(description));
	description[0] = sizeof(description);
	description[1] = 1;
	description[2] = 0;
	description[4] = 0;
	result = g_directSound->SetCooperativeLevel(m_platform.m_nativeWindow, 2);
	if (result != 0) {
		*g_pErrorOutput << "Effect Buffer Set Cooperative Level failed: "
						<< TargetDescribeDirectSoundError(result & 0xfff) << "\n";
		m_platform.m_available = 0;
		return 0;
	}
	result = g_directSound->CreateSoundBuffer(description, &g_primarySoundBuffer, 0);
	if (result != 0) {
		*g_pErrorOutput << "Primary Sound Buffer failed: " << TargetDescribeDirectSoundError(result & 0xfff) << "\n";
		m_platform.m_available = 0;
		return 0;
	}
	void* format = &m_platform.m_formatTag;
	result = g_primarySoundBuffer->SetFormat(format);
	if (result != 0) {
		*g_pSysOutput << "Primary Buffer Set Format failed: " << TargetDescribeDirectSoundError(result & 0xfff) << "\n";
		*g_pSysOutput << "Trying 22Khz/8-bit...\n";
		m_platform.m_bitsPerSample = 8;
		result = g_primarySoundBuffer->SetFormat(format);
		if (result != 0) {
			*g_pSysOutput << "Primary Buffer Set Format failed: " << TargetDescribeDirectSoundError(result & 0xfff)
						  << "\n";
			*g_pSysOutput << "Trying 11khz/16-bit...\n";
			m_platform.m_samplesPerSecond = 0x2b11;
			m_platform.m_bitsPerSample = 16;
			result = g_primarySoundBuffer->SetFormat(format);
			if (result != 0) {
				*g_pSysOutput << "Primary Buffer Set Format failed: " << TargetDescribeDirectSoundError(result & 0xfff)
							  << "\n";
				*g_pSysOutput << "Trying 11khz/8-bit...\n";
				m_platform.m_bitsPerSample = 8;
				result = g_primarySoundBuffer->SetFormat(format);
				if (result != 0) {
					*g_pErrorOutput << "Primary Buffer Set Format failed: "
									<< TargetDescribeDirectSoundError(result & 0xfff) << "\n";
					*g_pErrorOutput << "Exausted iterations - cannot play sample\n";
					return 0;
				}
			}
		}
	}
	result = g_primarySoundBuffer->Play(0, 0, 1);
	if (result != 0) {
		*g_pErrorOutput << "Primary Sound Buffer play: " << TargetDescribeDirectSoundError(result & 0xfff) << "\n";
		m_platform.m_available = 0;
		return 0;
	}
	m_platform.m_available = 1;
	m_platform.m_open = 1;
	return 1;
}

// FUNCTION: LEMBALL 0x0047e350
int TargetDirectSoundDevice::Dummy10(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0047e360
int TargetDirectSoundDevice::Dummy2c()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047e370
int TargetDirectSoundDevice::GetBuffersPerEffect()
{
	return m_platform.m_buffersPerEffect;
}

// FUNCTION: LEMBALL 0x0047e380
int TargetDirectSoundDevice::IsAvailable()
{
	return m_platform.m_available;
}

// FUNCTION: LEMBALL 0x0047e390
int TargetDirectSoundDevice::Close()
{
	if (g_primarySoundBuffer != 0) {
		unsigned int result = g_primarySoundBuffer->Release();
		if (result != 0) {
			*g_pErrorOutput << "Primary Sound Buffer Release: " << TargetDescribeDirectSoundError(result & 0xfff)
							<< "\n";
			return 0;
		}
		g_primarySoundBuffer = 0;
	}
	if (g_directSound != 0) {
		unsigned int result = g_directSound->Release();
		if (result != 0) {
			*g_pErrorOutput << "Direct Sound Release failed: " << TargetDescribeDirectSoundError(result & 0xfff)
							<< "\n";
			return 0;
		}
		g_directSound = 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0047e450
int TargetDirectSoundDevice::IsAnyEffectPlaying()
{
	for (int i = 1; i <= m_platform.m_effectCapacity; i++) {
		if (m_platform.m_effects[i] != 0 && m_platform.m_effects[i]->IsPlaying()) {
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0047e490
int TargetDirectSoundDevice::Dummy1c()
{
	return 1;
}

// FUNCTION: LEMBALL 0x0047e4a0
int TargetDirectSoundDevice::StopAllEffects()
{
	for (int i = 1; i <= m_platform.m_effectCapacity; i++) {
		if (m_platform.m_effects[i] != 0) {
			m_platform.m_effects[i]->Stop();
		}
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0047e4e0
int TargetDirectSoundDevice::IsMusicAvailable()
{
	return m_platform.m_musicAvailable;
}

// FUNCTION: LEMBALL 0x0047e4f0
int TargetDirectSoundDevice::IsEffectAvailable()
{
	return m_platform.m_available;
}

// FUNCTION: LEMBALL 0x0047e500
int TargetDirectSoundDevice::Dummy34(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2, undefined4 p_arg3)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047e510
int TargetDirectSoundDevice::Dummy38(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2, undefined4 p_arg3)
{
	return 0;
}

// STUB: LEMBALL 0x0047e520
bool TargetDirectSoundDevice::PrepareEffect(unsigned char* p_data, unsigned long* p_handle, unsigned int p_effectHandle)
{
	return false;
}

// FUNCTION: LEMBALL 0x0047e5b0
int TargetDirectSoundDevice::Dummy40(undefined4 p_arg0)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047e5c0
int TargetDirectSoundDevice::Dummy44(undefined4 p_arg0)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047e5d0
int TargetDirectSoundDevice::Dummy4c()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047e5e0
int TargetDirectSoundDevice::FreeEffect(unsigned long p_effectId)
{
	if (m_platform.m_effects[p_effectId] != 0) {
		delete m_platform.m_effects[p_effectId];
		m_platform.m_effects[p_effectId] = 0;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0047e620
int TargetDirectSoundDevice::FreeAllEffects()
{
	int index = 1;
	TargetDirectSoundDevice* device = this;
	if (index <= device->m_platform.m_effectCapacity) {
		do {
			TargetDirectSoundEffect* effect = device->m_platform.m_effects[index];
			if (effect != 0) {
				delete effect;
				device->m_platform.m_effects[index] = 0;
			}
			index++;
		} while (index <= device->m_platform.m_effectCapacity);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0047e670
unsigned char TargetDirectSoundDevice::GetMasterVolume()
{
	return 0xff;
}

// FUNCTION: LEMBALL 0x0047e680
void TargetDirectSoundDevice::SetMasterVolume(unsigned char p_volume)
{
}

// FUNCTION: LEMBALL 0x0047e690
unsigned char TargetDirectSoundDevice::GetMusicVolume()
{
	return 0xff;
}

// FUNCTION: LEMBALL 0x0047e6a0
void TargetDirectSoundDevice::SetMusicVolume(unsigned char p_volume)
{
}

// FUNCTION: LEMBALL 0x0047e6b0
unsigned char TargetDirectSoundDevice::GetEffectVolume()
{
	return 0xff;
}

// FUNCTION: LEMBALL 0x0047e6c0
void TargetDirectSoundDevice::SetEffectVolume(unsigned char p_volume)
{
}

// FUNCTION: LEMBALL 0x0047e700
bool TargetDirectSoundDevice::SetVolume(unsigned long p_resourceId, int p_index, unsigned char p_volume)
{
	int volume = (p_volume * 10000) / 0xff - 10000;
	return m_platform.m_effects[p_resourceId]->SetBufferVolume(p_index, volume);
}

// FUNCTION: LEMBALL 0x0047e7c0
unsigned char TargetDirectSoundDevice::EffectPlay(unsigned long p_effectId, unsigned short p_pitch, int p_volume)
{
	return m_platform.m_effects[p_effectId]->Play(p_volume);
}

// FUNCTION: LEMBALL 0x0047e7e0
unsigned char TargetDirectSoundDevice::EffectPlay(unsigned long p_effectId, unsigned char p_channel, int p_volume)
{
	int volume = (p_channel * 10000) / 0xff - 10000;
	return m_platform.m_effects[p_effectId]->PlayWithVolume(volume, p_volume);
}

// FUNCTION: LEMBALL 0x0047e860
bool TargetDirectSoundDevice::EffectStop(unsigned char p_channel, unsigned char p_effect)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047e8b0
void TargetDirectSoundDevice::SetWnd(Wnd* p_wnd)
{
	m_platform.m_nativeWindow = p_wnd->m_nativeWindow;
}

// FUNCTION: LEMBALL 0x0047e8c0
int TargetDirectSoundDevice::PrepareEffect(unsigned char* p_data, unsigned long* p_handle)
{
	return PrepareEffect(p_data, p_handle, 1);
}

TargetDirectSoundDevice::~TargetDirectSoundDevice()
{
}
