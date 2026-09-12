#ifndef LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDEFFECT_H
#define LEMBALL_VISOS_TARGET_TARGETDIRECTSOUNDEFFECT_H

#include "../../Common.h"
#include "IDirectSoundBuffer.h"

const char* TargetDescribeDirectSoundError(unsigned int p_error);

// SIZE 0x1c
class TargetDirectSoundEffect {
public:
	TargetDirectSoundEffect(int p_bufferCount,
							unsigned char* p_patch,
							unsigned int p_sampleRate,
							int p_use16Bit,
							int p_stereo,
							unsigned int p_controlFlags);
	~TargetDirectSoundEffect();
	bool IsPlaying();
	void Stop();
	int Play(int p_loop);
	int PlayWithVolume(int p_volume, int p_loop);
	bool SetBufferVolume(int p_index, int p_volume);
	bool IsPrepared() const { return m_prepared != 0; }

private:
	int FindIdleBuffer();
	void PlayBuffer(int p_index);

	unsigned int m_prepared;        // 0x00
	undefined4 m_unknown04;         // 0x04
	IDirectSoundBuffer** m_buffers; // 0x08
	undefined4 m_looping;           // 0x0c
	int m_bufferCount;              // 0x10
	undefined4 m_unknown14;         // 0x14
	unsigned int m_controlFlags;    // 0x18
};

#endif
