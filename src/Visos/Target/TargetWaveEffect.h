#ifndef LEMBALL_VISOS_TARGET_TARGETWAVEEFFECT_H
#define LEMBALL_VISOS_TARGET_TARGETWAVEEFFECT_H

#include "../../Common.h"

// SIZE 0x18
class TargetWaveEffect {
public:
	TargetWaveEffect(unsigned char* p_patch,
					 unsigned int p_waveOut,
					 unsigned int p_sampleRate,
					 int p_use16Bit,
					 int p_stereo);
	~TargetWaveEffect();

private:
	friend class TargetWaveSoundDevice;

	unsigned int m_prepared;     // 0x00
	void* m_sampleHandle;        // 0x04
	void* m_headerHandle;        // 0x08
	unsigned char* m_sampleData; // 0x0c
	void* m_waveHeader;          // 0x10
	void* m_waveOut;             // 0x14
};

#endif
