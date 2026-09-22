#include "CBaseSoundDevice.h"

// FUNCTION: LEMBALL 0x0047d260
void CBaseSoundDevice::SetWnd(CWnd* p_window)
{
}

// FUNCTION: LEMBALL 0x0047f940
CBaseSoundDevice::CBaseSoundDevice()
{
}

// FUNCTION: LEMBALL 0x0047f950
CBaseSoundDevice::~CBaseSoundDevice()
{
}

// FUNCTION: LEMBALL 0x0047f980
unsigned char CBaseSoundDevice::GetMasterVolume()
{
	return 0xff;
}

// FUNCTION: LEMBALL 0x0047f990
void CBaseSoundDevice::SetMasterVolume(unsigned char p_volume)
{
}

// FUNCTION: LEMBALL 0x0047f9a0
unsigned char CBaseSoundDevice::GetMusicVolume()
{
	return 0xff;
}

// FUNCTION: LEMBALL 0x0047f9b0
void CBaseSoundDevice::SetMusicVolume(unsigned char p_volume)
{
}

// FUNCTION: LEMBALL 0x0047f9c0
unsigned char CBaseSoundDevice::GetEffectVolume()
{
	return 0xff;
}

// FUNCTION: LEMBALL 0x0047f9d0
void CBaseSoundDevice::SetEffectVolume(unsigned char p_volume)
{
}

// FUNCTION: LEMBALL 0x0047f9e0
bool CBaseSoundDevice::SetVolume(unsigned long p_resourceId, int p_index, unsigned char p_volume)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047f9f0
void CBaseSoundDevice::MusicPlayNote(unsigned char p_channel, unsigned char p_note, unsigned char p_velocity)
{
}

// FUNCTION: LEMBALL 0x0047fa00
void CBaseSoundDevice::MusicSetNoteVolume(unsigned char p_channel, unsigned char p_note, unsigned char p_volume)
{
}

// FUNCTION: LEMBALL 0x0047fa10
void CBaseSoundDevice::MusicStopNote(unsigned char p_channel, unsigned char p_note, unsigned char p_velocity)
{
}

// FUNCTION: LEMBALL 0x0047fa20
unsigned char CBaseSoundDevice::MusicSetChannelVolume(unsigned char p_channel, unsigned char p_volume)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047fa30
void CBaseSoundDevice::MusicSetChannelPatch(unsigned char p_channel, unsigned long p_patch)
{
}

// FUNCTION: LEMBALL 0x0047fa40
void CBaseSoundDevice::MusicSetPitchWheel(unsigned char p_channel, unsigned short p_pitch)
{
}

// FUNCTION: LEMBALL 0x0047fa50
void CBaseSoundDevice::MusicMiscControlMessage(unsigned char p_channel, unsigned char p_control, unsigned char p_value)
{
}

// FUNCTION: LEMBALL 0x0047fa60
unsigned char CBaseSoundDevice::EffectPlay(unsigned long p_effectId, unsigned short p_pitch, int p_volume)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047fa70
unsigned char CBaseSoundDevice::EffectPlay(unsigned long p_effectId, unsigned char p_channel, int p_volume)
{
	return 0;
}

// FUNCTION: LEMBALL 0x0047fa80
bool CBaseSoundDevice::EffectStop(unsigned char p_channel, unsigned char p_effect)
{
	return 1;
}
