#ifndef LEMBALL_VISOS_SOUND_BASESOUNDDEVICE_H
#define LEMBALL_VISOS_SOUND_BASESOUNDDEVICE_H

#include "../../Common.h"

// SIZE 0x04
// VTABLE: LEMBALL 0x0049ade8
class BaseSoundDevice {
public:
	virtual ~BaseSoundDevice();                                                                          // vtable+0x00
	virtual char* GetInfo() = 0;                                                                         // vtable+0x04
	virtual int IsAvailable() = 0;                                                                       // vtable+0x08
	virtual int Open(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId) = 0;      // vtable+0x0c
	virtual int Dummy10(unsigned int p_music, unsigned int p_effects, unsigned long p_resourceId) = 0;   // vtable+0x10
	virtual int Close() = 0;                                                                             // vtable+0x14
	virtual int IsAnyEffectPlaying() = 0;                                                                // vtable+0x18
	virtual int Dummy1c() = 0;                                                                           // vtable+0x1c
	virtual int StopAllEffects() = 0;                                                                    // vtable+0x20
	virtual int IsMusicAvailable() = 0;                                                                  // vtable+0x24
	virtual int IsEffectAvailable() = 0;                                                                 // vtable+0x28
	virtual int Dummy2c() = 0;                                                                           // vtable+0x2c
	virtual int GetBuffersPerEffect() = 0;                                                               // vtable+0x30
	virtual int Dummy34(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2, undefined4 p_arg3) = 0; // vtable+0x34
	virtual int Dummy38(undefined4 p_arg0, undefined4 p_arg1, undefined4 p_arg2, undefined4 p_arg3) = 0; // vtable+0x38
	virtual int PrepareEffect(unsigned char* p_data, unsigned long* p_handle) = 0;                       // vtable+0x3c
	virtual int Dummy40(undefined4 p_arg0) = 0;                                                          // vtable+0x40
	virtual int Dummy44(undefined4 p_arg0) = 0;                                                          // vtable+0x44
	virtual int FreeEffect(unsigned long p_effectId) = 0;                                                // vtable+0x48
	virtual int Dummy4c() = 0;                                                                           // vtable+0x4c
	virtual int FreeAllEffects() = 0;                                                                    // vtable+0x50
	virtual unsigned char GetMasterVolume();                                                             // vtable+0x54
	virtual void SetMasterVolume(unsigned char p_volume);                                                // vtable+0x58
	virtual unsigned char GetMusicVolume();                                                              // vtable+0x5c
	virtual void SetMusicVolume(unsigned char p_volume);                                                 // vtable+0x60
	virtual unsigned char GetEffectVolume();                                                             // vtable+0x64
	virtual void SetEffectVolume(unsigned char p_volume);                                                // vtable+0x68
	virtual bool SetVolume(unsigned long p_resourceId, int p_index, unsigned char p_volume);             // vtable+0x6c
	virtual void MusicPlayNote(unsigned char p_channel, unsigned char p_note, unsigned char p_velocity); // vtable+0x70
	virtual void MusicSetNoteVolume(unsigned char p_channel,
									unsigned char p_note,
									unsigned char p_volume);                                             // vtable+0x74
	virtual void MusicStopNote(unsigned char p_channel, unsigned char p_note, unsigned char p_velocity); // vtable+0x78
	virtual unsigned char MusicSetChannelVolume(unsigned char p_channel, unsigned char p_volume);        // vtable+0x7c
	virtual void MusicSetChannelPatch(unsigned char p_channel, unsigned long p_patch);                   // vtable+0x80
	virtual void MusicSetPitchWheel(unsigned char p_channel, unsigned short p_pitch);                    // vtable+0x84
	virtual void MusicMiscControlMessage(unsigned char p_channel,
										 unsigned char p_control,
										 unsigned char p_value);                                       // vtable+0x88
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned short p_pitch, int p_volume);  // vtable+0x90
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned char p_channel, int p_volume); // vtable+0x8c
	virtual bool EffectStop(unsigned char p_channel, unsigned char p_effect);                          // vtable+0x94
	virtual void SetWnd(Wnd* p_window);                                                                // vtable+0x98

	BaseSoundDevice();
};

// SYNTHETIC: LEMBALL 0x0047fa90
// BaseSoundDevice::`scalar deleting destructor'

#endif
