#include "BaseSoundDevice.h"

// 68K 0x101024f8 SetWnd__16CBaseSoundDeviceFP4CWnd
// FUNCTION: LEMBALL 0x0047d260
void BaseSoundDevice::SetWnd(Wnd* p_arg0)
{
}

// 68K 0x10217d84 __ct__16CBaseSoundDeviceFv
// FUNCTION: LEMBALL 0x0047f940
BaseSoundDevice::BaseSoundDevice()
{
}

char* BaseSoundDevice::Dummy04()
{
	return 0;
}

int BaseSoundDevice::Dummy08()
{
	return 0;
}

int BaseSoundDevice::Dummy0c(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId)
{
	return 0;
}

int BaseSoundDevice::Dummy10(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId)
{
	return 0;
}

int BaseSoundDevice::Dummy14()
{
	return 0;
}

int BaseSoundDevice::Dummy18()
{
	return 0;
}

int BaseSoundDevice::Dummy1c()
{
	return 0;
}

int BaseSoundDevice::Dummy20()
{
	return 0;
}

int BaseSoundDevice::Dummy24()
{
	return 0;
}

int BaseSoundDevice::Dummy28()
{
	return 0;
}

int BaseSoundDevice::Dummy2c()
{
	return 0;
}

int BaseSoundDevice::Dummy30()
{
	return 0;
}

void BaseSoundDevice::Dummy34()
{
}

void BaseSoundDevice::Dummy38()
{
}

int BaseSoundDevice::Dummy3c(unsigned char* p_data, unsigned long* p_handle)
{
	return 0;
}

void BaseSoundDevice::Dummy40()
{
}

void BaseSoundDevice::Dummy44()
{
}

int BaseSoundDevice::Dummy48(unsigned long p_effectId)
{
	return 0;
}

void BaseSoundDevice::Dummy4c()
{
}

void BaseSoundDevice::Dummy50()
{
}

// 68K 0x10217e02 GetMasterVolume__16CBaseSoundDeviceFv
// FUNCTION: LEMBALL 0x0047f980
unsigned char BaseSoundDevice::GetMasterVolume()
{
	return 0xff;
}

// 68K 0x10217e38 SetMasterVolume__16CBaseSoundDeviceFUc
// FUNCTION: LEMBALL 0x0047f990
void BaseSoundDevice::SetMasterVolume(unsigned char p_volume)
{
}

// 68K 0x10217e6a GetMusicVolume__16CBaseSoundDeviceFv
// FUNCTION: LEMBALL 0x0047f9a0
unsigned char BaseSoundDevice::GetMusicVolume()
{
	return 0xff;
}

// 68K 0x10217e9e SetMusicVolume__16CBaseSoundDeviceFUc
// FUNCTION: LEMBALL 0x0047f9b0
void BaseSoundDevice::SetMusicVolume(unsigned char p_volume)
{
}

// 68K 0x10217ed0 GetEffectVolume__16CBaseSoundDeviceFv
// FUNCTION: LEMBALL 0x0047f9c0
unsigned char BaseSoundDevice::GetEffectVolume()
{
	return 0xff;
}

// 68K 0x10217f06 SetEffectVolume__16CBaseSoundDeviceFUc
// FUNCTION: LEMBALL 0x0047f9d0
void BaseSoundDevice::SetEffectVolume(unsigned char p_volume)
{
}

// 68K 0x10217f38 SetVolume__16CBaseSoundDeviceFUliUc
// FUNCTION: LEMBALL 0x0047f9e0
bool BaseSoundDevice::SetVolume(unsigned long p_resourceId, int p_index, unsigned char p_volume)
{
	return 0;
}

// 68K 0x10217f6a MusicPlayNote__16CBaseSoundDeviceFUcUcUc
// FUNCTION: LEMBALL 0x0047f9f0
void BaseSoundDevice::MusicPlayNote(unsigned char p_channel, unsigned char p_note, unsigned char p_velocity)
{
}

// 68K 0x10217f9e MusicSetNoteVolume__16CBaseSoundDeviceFUcUcUc
// FUNCTION: LEMBALL 0x0047fa00
void BaseSoundDevice::MusicSetNoteVolume(unsigned char p_channel, unsigned char p_note, unsigned char p_volume)
{
}

// 68K 0x10217fd8 MusicStopNote__16CBaseSoundDeviceFUcUcUc
// FUNCTION: LEMBALL 0x0047fa10
void BaseSoundDevice::MusicStopNote(unsigned char p_channel, unsigned char p_note, unsigned char p_velocity)
{
}

// 68K 0x1021800c MusicSetChannelVolume__16CBaseSoundDeviceFUcUc
// FUNCTION: LEMBALL 0x0047fa20
unsigned char BaseSoundDevice::MusicSetChannelVolume(unsigned char p_channel, unsigned char p_volume)
{
	return 0;
}

// 68K 0x10218048 MusicSetChannelPatch__16CBaseSoundDeviceFUcUl
// FUNCTION: LEMBALL 0x0047fa30
void BaseSoundDevice::MusicSetChannelPatch(unsigned char p_channel, unsigned long p_patch)
{
}

// 68K 0x10218082 MusicSetPitchWheel__16CBaseSoundDeviceFUcUs
// FUNCTION: LEMBALL 0x0047fa40
void BaseSoundDevice::MusicSetPitchWheel(unsigned char p_channel, unsigned short p_pitch)
{
}

// 68K 0x102180ba MusicMiscControlMessage__16CBaseSoundDeviceFUcUcUc
// FUNCTION: LEMBALL 0x0047fa50
void BaseSoundDevice::MusicMiscControlMessage(unsigned char p_channel, unsigned char p_control, unsigned char p_value)
{
}

// 68K 0x102180f8 EffectPlay__16CBaseSoundDeviceFUlUsUc
// FUNCTION: LEMBALL 0x0047fa60
unsigned char BaseSoundDevice::EffectPlay(unsigned long p_effectId, unsigned short p_pitch, unsigned char p_volume)
{
	return 0;
}

// 68K 0x1021812c EffectPlay__16CBaseSoundDeviceFUlUcUc
// FUNCTION: LEMBALL 0x0047fa70
unsigned char BaseSoundDevice::EffectPlay(unsigned long p_effectId, unsigned char p_channel, unsigned char p_volume)
{
	return 0;
}

// 68K 0x10218160 EffectStop__16CBaseSoundDeviceFUcUc
// FUNCTION: LEMBALL 0x0047fa80
bool BaseSoundDevice::EffectStop(unsigned char p_channel, unsigned char p_effect)
{
	return 1;
}

// 68K 0x10217db8 __dt__16CBaseSoundDeviceFv
BaseSoundDevice::~BaseSoundDevice()
{
}

