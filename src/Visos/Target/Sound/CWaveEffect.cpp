#include "CWaveEffect.h"

#include "../../Foundation/CVSOStream.h"
#include "EffPatchHeader.h"
#include "EffWaveHeader.h"

#include <string.h>

inline CVSOStream& operator<<(CVSOStream& p_stream, unsigned short p_value)
{
	return p_stream << (unsigned int) p_value;
}

// FUNCTION: LEMBALL 0x0047c210
unsigned short SwapBytes16(unsigned short p_value)
{
	unsigned short high = (unsigned short) (p_value >> 8);
	p_value = (unsigned short) (p_value << 8);
	return (unsigned short) (high + p_value);
}

// FUNCTION: LEMBALL 0x0047c230
unsigned int SwapBytes32(unsigned int p_value)
{
	unsigned int low = p_value & 0xffff;
	p_value >>= 16;
	low = SwapBytes16((unsigned short) low);
	low <<= 16;
	return SwapBytes16((unsigned short) p_value) + low;
}

// FUNCTION: LEMBALL 0x0047c260
CWaveEffect::CWaveEffect(unsigned char* p_patch, HWAVEOUT p_waveOut, DWORD p_sampleRate, int p_use16Bit, int p_stereo)
{
	WAVEFORMATEX format;
	EffPatchHeader patchHeader;
	EffWaveHeader waveHeader;
	unsigned char* wave;
	unsigned char* source;
	unsigned char* dest;
	unsigned int length;
	MMRESULT result;
	union {
		unsigned int m_downsample;
		char m_errorText[MAXERRORLENGTH];
	} work;

	memcpy(&patchHeader, p_patch, sizeof(patchHeader));
	patchHeader.m_formatVersion = SwapBytes16(patchHeader.m_formatVersion);
	patchHeader.m_waveCount = SwapBytes16(patchHeader.m_waveCount);
	m_prepared = 0;
	m_waveOut = p_waveOut;
	if (patchHeader.m_waveCount != 1) {
		*g_pErrorOutput << "Warning! Effect Patch " << ((EffPatchHeader*) p_patch)->m_name << " has more than ";
		*g_pErrorOutput << "one Wave. Only one is supported!\n";
	}
	wave = p_patch + sizeof(EffPatchHeader);
	memcpy(&waveHeader, wave, sizeof(waveHeader));
	waveHeader.m_formatVersion = SwapBytes16(waveHeader.m_formatVersion);
	waveHeader.m_length = SwapBytes32(waveHeader.m_length);
	waveHeader.m_sampleRate = SwapBytes32(waveHeader.m_sampleRate);
	length = waveHeader.m_length;
	if (p_use16Bit == 0) {
		length >>= 1;
	}
	work.m_downsample = 0;
	if (waveHeader.m_sampleRate != p_sampleRate) {
		work.m_downsample = 1;
		length >>= 1;
	}
	m_sampleHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, length);
	if (m_sampleHandle == NULL) {
		*g_pErrorOutput << "Error! Sound System unable to allocate memory for Wave data ";
		*g_pErrorOutput << patchHeader.m_name << "\n";
		return;
	}
	m_headerHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(WAVEHDR));
	if (m_headerHandle == NULL) {
		*g_pErrorOutput << "Error! Sound System unable to allocate memory for Wave Header ";
		*g_pErrorOutput << patchHeader.m_name << "\n";
		return;
	}
	m_sampleData = (unsigned char*) GlobalLock(m_sampleHandle);
	if (m_sampleData == NULL) {
		*g_pErrorOutput << "Error! Sound System unable to lock memory for Wave data ";
		*g_pErrorOutput << patchHeader.m_name << "\n";
		GlobalUnlock(m_sampleHandle);
		GlobalFree(m_sampleHandle);
		return;
	}
	m_waveHeader = (WAVEHDR*) GlobalLock(m_headerHandle);
	if (m_waveHeader == NULL) {
		*g_pErrorOutput << "Error! Sound System unable to lock memory for Wave Header";
		*g_pErrorOutput << patchHeader.m_name << "\n";
		GlobalUnlock(m_headerHandle);
		GlobalFree(m_headerHandle);
		return;
	}
	m_waveHeader->lpData = (char*) m_sampleData;
	m_waveHeader->dwBufferLength = length;
	m_waveHeader->dwUser =
		(DWORD) ((((int) (char) p_patch[9] * 0x100 + (int) (char) p_patch[8]) * 0x100 + (int) (char) p_patch[7]) *
					 0x100 +
				 (int) (char) p_patch[6]);
	m_waveHeader->dwFlags = 0;
	m_waveHeader->dwLoops = 0;
	if (p_use16Bit == 0) {
		source = wave + sizeof(EffWaveHeader);
		dest = m_sampleData;
		if (work.m_downsample == 0) {
			while (length != 0) {
				*dest++ = *source++;
				source++;
				length--;
			}
		}
		else {
			while (length != 0) {
				*dest++ = *source++;
				*source = (unsigned char) (*source + 3);
				length--;
			}
		}
	}
	else {
		source = wave + sizeof(EffWaveHeader);
		dest = m_sampleData;
		if (work.m_downsample == 0) {
			if ((length >> 1) > 0) {
				length >>= 1;
				do {
					unsigned char high = *source++;
					*dest++ = *source++;
					*dest++ = (unsigned char) (high ^ 0x80);
				} while (--length != 0);
			}
		}
		else if ((length >> 2) > 0) {
			length >>= 2;
			do {
				unsigned char high = source[0];
				source++;
				*dest++ = *source++;
				*dest++ = (unsigned char) (high ^ 0x80);
				source += 2;
			} while (--length != 0);
		}
	}
	format.wFormatTag = WAVE_FORMAT_PCM;
	if (p_stereo == 1) {
		format.nChannels = 2;
	}
	else {
		format.nChannels = 1;
	}
	format.wBitsPerSample = PCM_SAMPLE_BITS_16;
	if (p_use16Bit != 1) {
		format.wBitsPerSample = PCM_SAMPLE_BITS_8;
	}
	format.nSamplesPerSec = p_sampleRate;
	format.cbSize = 0;
	format.nBlockAlign = (unsigned short) ((format.wBitsPerSample * format.nChannels) / 8);
	format.nAvgBytesPerSec = p_sampleRate * (unsigned int) format.nChannels;
	if (format.wBitsPerSample == PCM_SAMPLE_BITS_16) {
		format.nAvgBytesPerSec *= 2;
	}
	result = waveOutOpen(&m_waveOut, WAVE_MAPPER, &format, 0, 0, WAVE_FORMAT_QUERY);
	if (result != 0) {
		*g_pErrorOutput << "Error! Sound System cannot support Wave Format!\n";
		waveOutGetErrorTextA(result, work.m_errorText, sizeof(work.m_errorText));
		*g_pErrorOutput << work.m_errorText << "\n";
		*g_pErrorOutput << "Wave Format:\n";
		*g_pErrorOutput << "Samples/Sec: " << format.nSamplesPerSec << "\n";
		*g_pErrorOutput << "Avg Bytes/S: " << format.nAvgBytesPerSec << "\n";
		*g_pErrorOutput << "Align      : " << format.nBlockAlign << "\n";
		*g_pErrorOutput << "Type       : " << format.wBitsPerSample << " bit\n";
		return;
	}
	result = waveOutOpen(&m_waveOut, WAVE_MAPPER, &format, 0, 0, 0);
	if (result != 0) {
		*g_pErrorOutput << "Error! Sound System cannot open Wave Device!\n";
		waveOutGetErrorTextA(result, work.m_errorText, sizeof(work.m_errorText));
		*g_pErrorOutput << work.m_errorText << "\n";
		return;
	}
	result = waveOutPrepareHeader(m_waveOut, m_waveHeader, sizeof(*m_waveHeader));
	if (result != 0) {
		*g_pErrorOutput << "Error! Sound System cannot prepare Wave Header!\n";
		waveOutGetErrorTextA(result, work.m_errorText, sizeof(work.m_errorText));
		*g_pErrorOutput << work.m_errorText << "\n";
		return;
	}
	result = waveOutClose(m_waveOut);
	if (result != 0) {
		*g_pErrorOutput << "Error! Sound System cannot close Wave Device!\n";
		waveOutGetErrorTextA(result, work.m_errorText, sizeof(work.m_errorText));
		*g_pErrorOutput << work.m_errorText << "\n";
		return;
	}
	m_prepared = 1;
}

// FUNCTION: LEMBALL 0x0047c820
CWaveEffect::~CWaveEffect()
{
	MMRESULT result;

	if (m_prepared == 1) {
		result = waveOutUnprepareHeader(m_waveOut, m_waveHeader, sizeof(*m_waveHeader));
		if (result != 0) {
			waveOutUnprepareHeader(m_waveOut, m_waveHeader, sizeof(*m_waveHeader));
		}
		GlobalUnlock(m_sampleHandle);
		GlobalFree(m_sampleHandle);
		GlobalUnlock(m_headerHandle);
		GlobalFree(m_headerHandle);
	}
}
