#ifndef LEMBALL_VISOS_TARGET_TARGETWAVESOUNDDEVICE_H
#define LEMBALL_VISOS_TARGET_TARGETWAVESOUNDDEVICE_H

#include "../../Common.h"
#include "../Sound/BaseSoundDevice.h" // complete type

#define WIN32_LEAN_AND_MEAN
// clang-format off: mmsystem.h requires the Win32 types declared by windows.h.
#include <windows.h>
#include <mmsystem.h>
// clang-format on

class TargetWaveEffect;

// SIZE 0xb8
// VTABLE: LEMBALL 0x0049ac38
class TargetWaveSoundDevice : public BaseSoundDevice {
public:
	TargetWaveSoundDevice(int p_channelCount);
	virtual ~TargetWaveSoundDevice();
	virtual char* GetInfo();
	virtual int IsAvailable();
	virtual int Open(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId);
	virtual int Dummy10(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId);
	virtual int Close();
	virtual int IsAnyEffectPlaying();
	virtual int Dummy1c();
	virtual int StopAllEffects();
	virtual int IsMusicAvailable();
	virtual int IsEffectAvailable();
	virtual int Dummy2c();
	virtual int GetBuffersPerEffect();
	virtual int Dummy34(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2, undefined4 p_arg3);
	virtual int Dummy38(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2, undefined4 p_arg3);
	virtual int PrepareEffect(unsigned char* p_data, unsigned long* p_handle);
	virtual int Dummy40(undefined4 p_arg0);
	virtual int Dummy44(undefined4 p_arg0);
	virtual int FreeEffect(unsigned long p_effectId);
	virtual int Dummy4c();
	virtual int FreeAllEffects();
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned short p_pitch, int p_volume);  // vtable+0x90
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned char p_channel, int p_volume); // vtable+0x8c
	virtual bool EffectStop(unsigned char p_channel, unsigned char p_effect);                          // vtable+0x94

private:
	unsigned int m_channelCount;    // 0x04
	unsigned int m_musicDevice;     // 0x08
	unsigned int m_available;       // 0x0c
	unsigned int m_stereo;          // 0x10
	unsigned int m_use16Bit;        // 0x14
	undefined4 m_unk0x18;           // 0x18
	undefined4 m_unk0x1c;           // 0x1c
	unsigned int m_channelState[8]; // 0x20
	unsigned char m_pad0x40[8];     // 0x40
	unsigned char m_pad0x48[8];     // 0x48
	WAVEOUTCAPSA m_caps;            // 0x50
	DWORD m_sampleRate;             // 0x84
	UINT m_deviceId;                // 0x88
	HWAVEOUT m_waveOut;             // 0x8c
	WAVEFORMATEX m_waveFormat;      // 0x90; next DWORD aligned at 0xa4
	unsigned int m_nextHandle;      // 0xa4
	TargetWaveEffect** m_effects;   // 0xa8
	unsigned int* m_effectUsed;     // 0xac
	unsigned int* m_effectHandles;  // 0xb0
	DWORD m_savedVolume;            // 0xb4
};

// SYNTHETIC: LEMBALL 0x0047d270
// TargetWaveSoundDevice::`scalar deleting destructor'

#endif
