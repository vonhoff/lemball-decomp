#ifndef LEMBALL_VISOS_TARGET_TARGETWAVEEFFECT_H
#define LEMBALL_VISOS_TARGET_TARGETWAVEEFFECT_H

#include "../../Common.h"

// SIZE 0x18
class TargetWaveEffect {
public:
	// No functions mapped yet.

private:
	unsigned int m_prepared;     // 0x00
	void* m_sampleHandle;        // 0x04
	void* m_headerHandle;        // 0x08
	unsigned char* m_sampleData; // 0x0c
	void* m_waveHeader;          // 0x10
	void* m_waveOut;             // 0x14
};

#endif
