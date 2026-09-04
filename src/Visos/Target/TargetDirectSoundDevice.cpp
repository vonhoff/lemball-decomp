#include "TargetDirectSoundDevice.h"

#include "../Graphics/Wnd.h"

// STUB: LEMBALL 0x0047dd80
TargetDirectSoundDevice::TargetDirectSoundDevice(int p_channelCount, int p_flags)
{
	int i;

	i = 0;
	while (i < 0x50) {
		m_platformState[i] = 0;
		i = i + 1;
	}
	m_platformState[0] = (unsigned char) p_channelCount;
	m_platformState[1] = (unsigned char) p_flags;
}

// FUNCTION: LEMBALL 0x0047e000
char* TargetDirectSoundDevice::Dummy04()
{
	return m_platform.m_unk0x20 == 1 ? "Direct Sound Device\n" : "ERROR! No Effects Device for WinEff!\n";
}

// STUB: LEMBALL 0x0047e020
int TargetDirectSoundDevice::Dummy0c(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId)
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
int TargetDirectSoundDevice::Dummy30()
{
	return m_platform.m_unk0x18;
}

// FUNCTION: LEMBALL 0x0047e380
int TargetDirectSoundDevice::Dummy08()
{
	return m_platform.m_unk0x20;
}

// STUB: LEMBALL 0x0047e390
int TargetDirectSoundDevice::Dummy14()
{
	return 0;
}

// STUB: LEMBALL 0x0047e450
int TargetDirectSoundDevice::Dummy18()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047e490
int TargetDirectSoundDevice::Dummy1c()
{
	return 1;
}

// STUB: LEMBALL 0x0047e4a0
int TargetDirectSoundDevice::Dummy20()
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047e4e0
int TargetDirectSoundDevice::Dummy24()
{
	return m_platform.m_unk0x1c;
}

// FUNCTION: LEMBALL 0x0047e4f0
int TargetDirectSoundDevice::Dummy28()
{
	return m_platform.m_unk0x20;
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

// STUB: LEMBALL 0x0047e5e0
int TargetDirectSoundDevice::Dummy48(unsigned long p_effectId)
{
	return 0;
}

// STUB: LEMBALL 0x0047e620
int TargetDirectSoundDevice::Dummy50()
{
	return 0;
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

// STUB: LEMBALL 0x0047e700
bool TargetDirectSoundDevice::SetVolume(unsigned long p_resourceId, int p_index, unsigned char p_volume)
{
	return 0;
}

// STUB: LEMBALL 0x0047e7c0
unsigned char TargetDirectSoundDevice::EffectPlay(unsigned long p_effectId,
												  unsigned short p_pitch,
												  unsigned char p_volume)
{
	return 0;
}

// STUB: LEMBALL 0x0047e7e0
unsigned char TargetDirectSoundDevice::EffectPlay(unsigned long p_effectId,
												  unsigned char p_channel,
												  unsigned char p_volume)
{
	return 0;
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

// STUB: LEMBALL 0x0047e8c0
int TargetDirectSoundDevice::Dummy3c(unsigned char* p_data, unsigned long* p_handle)
{
	return 0;
}

TargetDirectSoundDevice::~TargetDirectSoundDevice()
{
}
