#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDDEVICE_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDDEVICE_H

#include "../../Common.h"
#include "../Sound/BaseSoundDevice.h" // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x0049acd8
class TargetDirectSoundDevice : public BaseSoundDevice {
public:
	TargetDirectSoundDevice(int p_effectCapacity, int p_buffersPerEffect);
	virtual ~TargetDirectSoundDevice();
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
	virtual unsigned char GetMasterVolume();
	virtual void SetMasterVolume(unsigned char p_volume);
	virtual unsigned char GetMusicVolume();
	virtual void SetMusicVolume(unsigned char p_volume);
	virtual unsigned char GetEffectVolume();
	virtual void SetEffectVolume(unsigned char p_volume);
	virtual bool SetVolume(unsigned long p_resourceId, int p_index, unsigned char p_volume);
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned short p_pitch, unsigned char p_volume);
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned char p_channel, unsigned char p_volume);
	virtual bool EffectStop(unsigned char p_channel, unsigned char p_effect);
	virtual void SetWnd(Wnd* p_wnd);

private:
	union {
		undefined m_platformState[0x50]; // 0x04
		struct {
			undefined m_unk0x04[0x0c];   // 0x04
			void* m_nativeWindow;        // 0x10
			int m_effectCapacity;        // 0x14
			int m_buffersPerEffect;      // 0x18
			undefined4 m_musicAvailable; // 0x1c
			undefined4 m_available;      // 0x20
			undefined m_unk0x24[0x30];   // 0x24
		} m_platform;
	};
};

// SYNTHETIC: LEMBALL 0x0047e8e0
// TargetDirectSoundDevice::`scalar deleting destructor'

#endif
