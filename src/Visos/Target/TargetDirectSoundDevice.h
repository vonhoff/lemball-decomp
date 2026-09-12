#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDDEVICE_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDDEVICE_H

#include "../../Common.h"
#include "../Sound/BaseSoundDevice.h" // complete type

class IDirectSound;

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
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned short p_pitch, int p_volume);
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned char p_channel, int p_volume);
	virtual bool EffectStop(unsigned char p_channel, unsigned char p_effect);
	virtual void SetWnd(Wnd* p_wnd);

private:
	bool PrepareEffect(unsigned char* p_data, unsigned long* p_handle, unsigned int p_effectHandle);

	union {
		undefined m_platformState[0x50]; // 0x04
		struct {
			void* m_library;                                                          // 0x04
			long(__stdcall* m_createDirectSound)(const void*, IDirectSound**, void*); // 0x08
			undefined4 m_open;                                                        // 0x0c
			void* m_nativeWindow;                                                     // 0x10
			int m_effectCapacity;                                                     // 0x14
			int m_buffersPerEffect;                                                   // 0x18
			undefined4 m_musicAvailable;                                              // 0x1c
			undefined4 m_available;                                                   // 0x20
			undefined4 m_unk0x24;                                                     // 0x24
			undefined4 m_unk0x28;                                                     // 0x28
			undefined4 m_unk0x2c;                                                     // 0x2c
			undefined4 m_unk0x30;                                                     // 0x30
			unsigned int m_sampleRate;                                                // 0x34
			undefined4 m_unk0x38;                                                     // 0x38
			unsigned short m_formatTag;                                               // 0x3c
			unsigned short m_channels;                                                // 0x3e
			unsigned int m_samplesPerSecond;                                          // 0x40
			unsigned int m_averageBytesPerSecond;                                     // 0x44
			unsigned short m_blockAlign;                                              // 0x48
			unsigned short m_bitsPerSample;                                           // 0x4a
			unsigned short m_extraFormatBytes;                                        // 0x4c
			undefined2 m_pad0x4e;                                                     // 0x4e
			TargetDirectSoundEffect** m_effects;                                      // 0x50
		} m_platform;
	};
};

// SYNTHETIC: LEMBALL 0x0047e8e0
// TargetDirectSoundDevice::`scalar deleting destructor'

#endif
