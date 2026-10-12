#ifndef LEMBALL_VISOS_TARGET_SOUND_CDIRECTSOUNDDEVICE_H
#define LEMBALL_VISOS_TARGET_SOUND_CDIRECTSOUNDDEVICE_H

#include "Engine/Sound/CBaseSoundDevice.h"

// clang-format off
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <mmsystem.h>
// clang-format on

class IDirectSound;
class CDirectSoundEffect;

// SIZE 0x54
// VTABLE: LEMBALL 0x0049acd8
class CDirectSoundDevice : public CBaseSoundDevice {
public:
	CDirectSoundDevice(int p_effectCapacity, int p_buffersPerEffect);
	virtual ~CDirectSoundDevice();
	virtual char* GetDeviceInfo();
	virtual int Detect();
	virtual int Initialise(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId);
	virtual int Reinitialise(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId);
	virtual int ShutDown();
	virtual int HasActiveVoices();
	virtual int StopAllMusic();
	virtual int StopAllEffects();
	virtual int IsMusicCapable();
	virtual int IsEffectCapable();
	virtual int GetNumMusicChannels();
	virtual int GetNumEffectsChannels();
	virtual int AllocatePatch(VSPATCH* p_patch, unsigned long* p_handle, unsigned char, unsigned char);
	virtual int AllocateDrumPatch(VSPATCH* p_patch, unsigned long* p_handle, unsigned char, unsigned char);
	virtual int AllocateEffect(unsigned char* p_data, unsigned long* p_handle);
	virtual int FreePatch(unsigned long p_patchHandle);
	virtual int FreeDrumPatch(unsigned long p_patchHandle);
	virtual int FreeEffect(unsigned long p_effectId);
	virtual int FreeAllPatches();
	virtual int FreeAllEffects();
	virtual unsigned char GetMasterVolume();
	virtual void SetMasterVolume(unsigned char p_volume);
	virtual unsigned char GetMusicVolume();
	virtual void SetMusicVolume(unsigned char p_volume);
	virtual unsigned char GetEffectVolume();
	virtual void SetEffectVolume(unsigned char p_volume);
	virtual bool SetVolume(unsigned long p_resourceId, int p_index, unsigned char p_volume);
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned short p_pitch, int p_loop);
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned char p_volume, int p_loop);
	virtual bool EffectStop(unsigned char p_channel, unsigned char p_effect);
	virtual void SetWnd(CWnd* p_wnd);

private:
	bool PrepareEffect(unsigned char* p_data, unsigned long* p_handle, unsigned int p_effectHandle);

	struct {
		HMODULE m_library;                                                        // 0x04
		long(__stdcall* m_createDirectSound)(const void*, IDirectSound**, void*); // 0x08
		bool m_open;                                                              // 0x0c
		void* m_nativeWindow;                                                     // 0x10
		int m_effectCapacity;                                                     // 0x14
		int m_buffersPerEffect;                                                   // 0x18
		bool m_musicAvailable;                                                    // 0x1c
		bool m_available;                                                         // 0x20
		bool m_stereo;                                                            // 0x24
		bool m_use16Bit;                                                          // 0x28
		unsigned int m_unk0x2c;                                                   // 0x2c
		unsigned int m_unk0x30;                                                   // 0x30
		unsigned int m_sampleRate;                                                // 0x34
		unsigned int m_deviceId;                                                  // 0x38
		WAVEFORMATEX m_format;                                                    // 0x3c
		unsigned short m_pad0x4e;                                                 // 0x4e
		CDirectSoundEffect** m_effects;                                           // 0x50
	} m_platform;
};

// SYNTHETIC: LEMBALL 0x0047e8e0
// CDirectSoundDevice::`scalar deleting destructor'

#endif
