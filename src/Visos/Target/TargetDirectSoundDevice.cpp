#include "TargetDirectSoundDevice.h"

#include "../Graphics/Wnd.h"
#include "TargetDirectSoundEffect.h"

// STUB: LEMBALL 0x0047dd80
TargetDirectSoundDevice::TargetDirectSoundDevice(int p_effectCapacity, int p_buffersPerEffect)
{
	int i;

	i = 0;
	while (i < 0x50) {
		m_platformState[i] = 0;
		i = i + 1;
	}
	m_platform.m_effectCapacity = p_effectCapacity;
	m_platform.m_buffersPerEffect = p_buffersPerEffect;
}

// FUNCTION: LEMBALL 0x0047e000
char* TargetDirectSoundDevice::GetInfo()
{
	return m_platform.m_available == 1 ? "Direct Sound Device\n" : "ERROR! No Effects Device for WinEff!\n";
}

// STUB: LEMBALL 0x0047e020
int TargetDirectSoundDevice::Open(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId)
{
	return 0;
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

// STUB: LEMBALL 0x0047e390
int TargetDirectSoundDevice::Close()
{
	return 0;
}

// STUB: LEMBALL 0x0047e450
int TargetDirectSoundDevice::IsAnyEffectPlaying()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047e490
int TargetDirectSoundDevice::Dummy1c()
{
	return 1;
}

// STUB: LEMBALL 0x0047e4a0
int TargetDirectSoundDevice::StopAllEffects()
{
	return 0;
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
	for (int i = 1; i <= m_platform.m_effectCapacity; i++) {
		if (m_platform.m_effects[i] != 0) {
			delete m_platform.m_effects[i];
			m_platform.m_effects[i] = 0;
		}
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
