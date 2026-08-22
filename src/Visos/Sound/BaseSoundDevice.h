#ifndef LEMBALL_VISOS_SOUND_BASESOUNDDEVICE_H
#define LEMBALL_VISOS_SOUND_BASESOUNDDEVICE_H

#include "../../Common.h"

// SIZE 0x04
// VTABLE: LEMBALL 0x0049ade8
class BaseSoundDevice {
public:
	BaseSoundDevice();
	bool EffectPlay(unsigned long p_effectId, unsigned char p_channel, unsigned char p_volume);
	bool EffectPlay(unsigned long p_effectId, unsigned short p_pitch, unsigned char p_volume);
	bool EffectStop(unsigned char p_channel, unsigned char p_effect);
	bool MusicSetChannelVolume(unsigned char p_channel, unsigned char p_volume);
	bool SetVolume(unsigned long p_resourceId, int p_index, unsigned char p_volume);
	unsigned char GetEffectVolume();
	unsigned char GetMasterVolume();
	unsigned char GetMusicVolume();
	virtual void SetWnd(Wnd* p_arg0); // vtable+0x98
	virtual ~BaseSoundDevice();       // vtable+0x00
	void MusicMiscControlMessage(unsigned char p_channel, unsigned char p_control, unsigned char p_value);
	void MusicPlayNote(unsigned char p_channel, unsigned char p_note, unsigned char p_velocity);
	void MusicSetChannelPatch(unsigned char p_channel, unsigned long p_patch);
	void MusicSetNoteVolume(unsigned char p_channel, unsigned char p_note, unsigned char p_volume);
	void MusicSetPitchWheel(unsigned char p_channel, unsigned short p_pitch);
	void MusicStopNote(unsigned char p_channel, unsigned char p_note, unsigned char p_velocity);
	void SetEffectVolume(unsigned char p_volume);
	void SetMasterVolume(unsigned char p_volume);
	void SetMusicVolume(unsigned char p_volume);
};

#endif
