#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDDEVICE_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDDEVICE_H

#include "../../Common.h"
#include "../Sound/BaseSoundDevice.h" // complete type

// SIZE 0x54
// VTABLE: LEMBALL 0x0049acd8
class TargetDirectSoundDevice : public BaseSoundDevice {
public:
	TargetDirectSoundDevice(int p_channelCount, int p_flags);
	virtual ~TargetDirectSoundDevice();
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
	virtual int Dummy34(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2, undefined4 p_arg3);
	virtual int Dummy38(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2, undefined4 p_arg3);
	virtual int Dummy3c(unsigned char* p_data, unsigned long* p_handle);
	virtual int Dummy40(undefined4 p_arg0);
	virtual int Dummy44(undefined4 p_arg0);
	virtual int Dummy48(unsigned long p_effectId);
	virtual int Dummy4c();
	virtual int Dummy50();
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
	undefined m_platformState[0x50]; // 0x04
};

// SYNTHETIC: LEMBALL 0x0047e8e0
// TargetDirectSoundDevice::`scalar deleting destructor'

#endif
