#ifndef LEMBALL_VISOS_TARGET_TARGETWAVESOUNDDEVICE_H
#define LEMBALL_VISOS_TARGET_TARGETWAVESOUNDDEVICE_H

#include "../../Common.h"
#include "../Sound/BaseSoundDevice.h" // complete type

class TargetWaveEffect;

// SIZE 0xb8
// VTABLE: LEMBALL 0x0049ac38
class TargetWaveSoundDevice : public BaseSoundDevice {
public:
	TargetWaveSoundDevice(int p_channelCount);
	virtual ~TargetWaveSoundDevice();
	virtual char* Dummy04();
	virtual int Dummy08();
	virtual int Dummy0c(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId);
	virtual int Dummy10(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId);
	virtual int Dummy14();
	virtual int Dummy18();
	virtual int Dummy1c();
	virtual int Dummy20();
	virtual int Dummy24();
	virtual int Dummy28();
	virtual int Dummy2c();
	virtual int Dummy30();
	virtual int Dummy3c(unsigned char* p_data, unsigned long* p_handle);
	virtual int Dummy48(unsigned long p_effectId);
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned char p_channel, unsigned char p_volume);
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned short p_pitch, unsigned char p_volume);

private:
	unsigned int m_channelCount;     // 0x04
	unsigned int m_musicDevice;      // 0x08
	unsigned int m_available;        // 0x0c
	unsigned int m_stereo;           // 0x10
	unsigned int m_use16Bit;         // 0x14
	undefined4 m_unk0x18;            // 0x18
	undefined4 m_unk0x1c;            // 0x1c
	unsigned int m_channelState[8];  // 0x20
	unsigned char m_pad0x40[8];      // 0x40
	unsigned char m_pad0x48[8];      // 0x48
	undefined m_caps[0x34];          // 0x50
	unsigned int m_sampleRate;       // 0x84
	unsigned int m_deviceId;         // 0x88
	void* m_waveOut;                 // 0x8c
	undefined m_waveFormat[0x14];    // 0x90
	unsigned int m_nextHandle;       // 0xa4
	TargetWaveEffect** m_effects;    // 0xa8
	unsigned int* m_effectUsed;      // 0xac
	unsigned int* m_effectHandles;   // 0xb0
	unsigned int m_savedVolume;      // 0xb4
};

// SYNTHETIC: LEMBALL 0x0047d270
// TargetWaveSoundDevice::`scalar deleting destructor'

#endif
