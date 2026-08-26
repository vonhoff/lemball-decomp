#ifndef LEMBALL_VISOS_SOUND_BASESOUNDDEVICE_H
#define LEMBALL_VISOS_SOUND_BASESOUNDDEVICE_H

#include "../../Common.h"

// SIZE 0x04
// VTABLE: LEMBALL 0x0049ade8
class BaseSoundDevice {
public:
	virtual ~BaseSoundDevice();                                                                             // vtable+0x00
	virtual void Dummy04();                                                                                 // vtable+0x04
	virtual void Dummy08();                                                                                 // vtable+0x08
	virtual void Dummy0c();                                                                                 // vtable+0x0c
	virtual void Dummy10();                                                                                 // vtable+0x10
	virtual void Dummy14();                                                                                 // vtable+0x14
	virtual void Dummy18();                                                                                 // vtable+0x18
	virtual void Dummy1c();                                                                                 // vtable+0x1c
	virtual void Dummy20();                                                                                 // vtable+0x20
	virtual void Dummy24();                                                                                 // vtable+0x24
	virtual void Dummy28();                                                                                 // vtable+0x28
	virtual void Dummy2c();                                                                                 // vtable+0x2c
	virtual void Dummy30();                                                                                 // vtable+0x30
	virtual void Dummy34();                                                                                 // vtable+0x34
	virtual void Dummy38();                                                                                 // vtable+0x38
	virtual void Dummy3c();                                                                                 // vtable+0x3c
	virtual void Dummy40();                                                                                 // vtable+0x40
	virtual void Dummy44();                                                                                 // vtable+0x44
	virtual void Dummy48();                                                                                 // vtable+0x48
	virtual void Dummy4c();                                                                                 // vtable+0x4c
	virtual void Dummy50();                                                                                 // vtable+0x50
	virtual unsigned char GetMasterVolume();                                                                // vtable+0x54
	virtual void SetMasterVolume(unsigned char p_volume);                                                   // vtable+0x58
	virtual unsigned char GetMusicVolume();                                                                 // vtable+0x5c
	virtual void SetMusicVolume(unsigned char p_volume);                                                    // vtable+0x60
	virtual unsigned char GetEffectVolume();                                                                // vtable+0x64
	virtual void SetEffectVolume(unsigned char p_volume);                                                   // vtable+0x68
	virtual bool SetVolume(unsigned long p_resourceId, int p_index, unsigned char p_volume);                // vtable+0x6c
	virtual void MusicPlayNote(unsigned char p_channel, unsigned char p_note, unsigned char p_velocity);     // vtable+0x70
	virtual void MusicSetNoteVolume(unsigned char p_channel, unsigned char p_note, unsigned char p_volume); // vtable+0x74
	virtual void MusicStopNote(unsigned char p_channel, unsigned char p_note, unsigned char p_velocity);     // vtable+0x78
	virtual unsigned char MusicSetChannelVolume(unsigned char p_channel, unsigned char p_volume);                    // vtable+0x7c
	virtual void MusicSetChannelPatch(unsigned char p_channel, unsigned long p_patch);                      // vtable+0x80
	virtual void MusicSetPitchWheel(unsigned char p_channel, unsigned short p_pitch);                       // vtable+0x84
	virtual void MusicMiscControlMessage(unsigned char p_channel, unsigned char p_control, unsigned char p_value); // vtable+0x88
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned char p_channel, unsigned char p_volume);    // vtable+0x8c
	virtual unsigned char EffectPlay(unsigned long p_effectId, unsigned short p_pitch, unsigned char p_volume);     // vtable+0x90
	virtual bool EffectStop(unsigned char p_channel, unsigned char p_effect);                               // vtable+0x94
	virtual void SetWnd(Wnd* p_arg0);                                                                       // vtable+0x98

	BaseSoundDevice();
};

#endif
