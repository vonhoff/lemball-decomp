#ifndef LEMBALL_VISOS_TARGET_SOUND_CWAVEEFFECT_H
#define LEMBALL_VISOS_TARGET_SOUND_CWAVEEFFECT_H

#define WIN32_LEAN_AND_MEAN
// clang-format off
#include <windows.h>
#include <mmsystem.h>
// clang-format on

// SIZE 0x18
class CWaveEffect {
public:
	CWaveEffect(unsigned char* p_patch, HWAVEOUT p_waveOut, DWORD p_sampleRate, int p_use16Bit, int p_stereo);
	~CWaveEffect();

private:
	friend class CWaveSoundDevice;

	unsigned int m_prepared;     // 0x00
	HGLOBAL m_sampleHandle;      // 0x04
	HGLOBAL m_headerHandle;      // 0x08
	unsigned char* m_sampleData; // 0x0c
	WAVEHDR* m_waveHeader;       // 0x10
	HWAVEOUT m_waveOut;          // 0x14
};

#endif
