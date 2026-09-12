#include "TargetWaveEffect.h"

#include "../Foundation/VsOStream.h"

struct EffPatchHeader {
	unsigned int m_unk0;
	unsigned short m_unk4;
	char m_name[14];
	unsigned short m_waveCount;
	unsigned short m_unk16;
	unsigned int m_unk18;
};

struct EffWaveHeader {
	unsigned int m_unk0;
	unsigned short m_unk4;
	unsigned short m_unk6;
	unsigned int m_length;
	unsigned int m_unk0c;
	unsigned int m_unk10;
	unsigned int m_sampleRate;
	unsigned int m_unk18;
	unsigned int m_unk1c[7];
};

// FUNCTION: LEMBALL 0x0047c210
unsigned short TargetByteSwap16(unsigned short p_value)
{
	unsigned short ax;
	unsigned short cx;

	cx = p_value;
	ax = cx;
	ax = (unsigned short) (ax >> 8);
	cx = (unsigned short) (cx << 8);
	return (unsigned short) (ax + cx);
}

// FUNCTION: LEMBALL 0x0047c230
unsigned int TargetByteSwap32(unsigned int p_value)
{
	unsigned int esi;
	unsigned int edi;
	unsigned int eax;

	esi = p_value;
	eax = esi;
	esi = esi >> 16;
	eax = eax & 0xffff;
	edi = TargetByteSwap16((unsigned short) eax);
	edi = edi << 16;
	eax = TargetByteSwap16((unsigned short) esi);
	return eax + edi;
}

// FUNCTION: LEMBALL 0x0047c260
TargetWaveEffect::TargetWaveEffect(unsigned char* p_patch,
								   HWAVEOUT p_waveOut,
								   DWORD p_sampleRate,
								   int p_use16Bit,
								   int p_stereo)
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
		unsigned int downsample;
		char errorText[0x100];
	} work;

	patchHeader = *(EffPatchHeader*) p_patch;
	patchHeader.m_unk4 = TargetByteSwap16(patchHeader.m_unk4);
	patchHeader.m_waveCount = TargetByteSwap16(patchHeader.m_waveCount);
	m_prepared = 0;
	m_waveOut = p_waveOut;
	if (patchHeader.m_waveCount != 1) {
		*g_pErrorOutput << "Warning! Effect Patch " << ((EffPatchHeader*) p_patch)->m_name << " has more than ";
		*g_pErrorOutput << "one Wave. Only one is supported!\n";
	}
	wave = p_patch + sizeof(EffPatchHeader);
	waveHeader = *(EffWaveHeader*) wave;
	waveHeader.m_unk4 = TargetByteSwap16(waveHeader.m_unk4);
	waveHeader.m_length = TargetByteSwap32(waveHeader.m_length);
	waveHeader.m_sampleRate = TargetByteSwap32(waveHeader.m_sampleRate);
	length = waveHeader.m_length;
	if (p_use16Bit == 0) {
		length >>= 1;
	}
	work.downsample = 0;
	if (waveHeader.m_sampleRate != p_sampleRate) {
		work.downsample = 1;
		length >>= 1;
	}
	m_sampleHandle = GlobalAlloc(0x2002, length);
	if (m_sampleHandle == 0) {
		*g_pErrorOutput << "Error! Sound System unable to allocate memory for Wave data ";
		*g_pErrorOutput << patchHeader.m_name << "\n";
		return;
	}
	m_headerHandle = GlobalAlloc(0x2002, 0x20);
	if (m_headerHandle == 0) {
		*g_pErrorOutput << "Error! Sound System unable to allocate memory for Wave Header ";
		*g_pErrorOutput << patchHeader.m_name << "\n";
		return;
	}
	m_sampleData = (unsigned char*) GlobalLock(m_sampleHandle);
	if (m_sampleData == 0) {
		*g_pErrorOutput << "Error! Sound System unable to lock memory for Wave data ";
		*g_pErrorOutput << patchHeader.m_name << "\n";
		GlobalUnlock(m_sampleHandle);
		GlobalFree(m_sampleHandle);
		return;
	}
	m_waveHeader = (WAVEHDR*) GlobalLock(m_headerHandle);
	if (m_waveHeader == 0) {
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
		if (work.downsample == 0) {
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
		if (work.downsample == 0) {
			if ((length & 0xfffffffe) != 0) {
				length >>= 1;
				dest = m_sampleData;
				do {
					unsigned char high = source[0];
					dest[0] = source[1];
					source += 2;
					dest[1] = (unsigned char) (high ^ 0x80);
					dest += 2;
				} while (--length != 0);
			}
		}
		else if ((length & 0xfffffffc) != 0) {
			length >>= 2;
			dest = m_sampleData;
			do {
				unsigned char high = source[0];
				source++;
				*dest++ = *source++;
				*dest++ = (unsigned char) (high ^ 0x80);
				source += 2;
			} while (--length != 0);
		}
	}
	format.wFormatTag = 1;
	format.nChannels = 2;
	if (p_stereo != 1) {
		format.nChannels = 1;
	}
	format.wBitsPerSample = 0x10;
	if (p_use16Bit != 1) {
		format.wBitsPerSample = 8;
	}
	format.nSamplesPerSec = p_sampleRate;
	format.cbSize = 0;
	format.nAvgBytesPerSec = p_sampleRate * (unsigned int) format.nChannels;
	format.nBlockAlign = (unsigned short) ((format.wBitsPerSample * format.nChannels) / 8);
	if (format.wBitsPerSample == 0x10) {
		format.nAvgBytesPerSec *= 2;
	}
	result = waveOutOpen(&m_waveOut, 0xffffffff, &format, 0, 0, WAVE_FORMAT_QUERY);
	if (result != 0) {
		*g_pErrorOutput << "Error! Sound System cannot support Wave Format!\n";
		waveOutGetErrorTextA(result, work.errorText, 0x100);
		*g_pErrorOutput << work.errorText << "\n";
		*g_pErrorOutput << "Wave Format:\n";
		*g_pErrorOutput << "Samples/Sec: " << format.nSamplesPerSec << "\n";
		*g_pErrorOutput << "Avg Bytes/S: " << format.nAvgBytesPerSec << "\n";
		*g_pErrorOutput << "Align      : " << (unsigned int) format.nBlockAlign << "\n";
		*g_pErrorOutput << "Type       : " << (unsigned int) format.wBitsPerSample << " bit\n";
		return;
	}
	result = waveOutOpen(&m_waveOut, 0xffffffff, &format, 0, 0, 0);
	if (result != 0) {
		*g_pErrorOutput << "Error! Sound System cannot open Wave Device!\n";
		waveOutGetErrorTextA(result, work.errorText, 0x100);
		*g_pErrorOutput << work.errorText << "\n";
		return;
	}
	result = waveOutPrepareHeader(m_waveOut, m_waveHeader, 0x20);
	if (result != 0) {
		*g_pErrorOutput << "Error! Sound System cannot prepare Wave Header!\n";
		waveOutGetErrorTextA(result, work.errorText, 0x100);
		*g_pErrorOutput << work.errorText << "\n";
		return;
	}
	result = waveOutClose(m_waveOut);
	if (result != 0) {
		*g_pErrorOutput << "Error! Sound System cannot close Wave Device!\n";
		waveOutGetErrorTextA(result, work.errorText, 0x100);
		*g_pErrorOutput << work.errorText << "\n";
		return;
	}
	m_prepared = 1;
}

// FUNCTION: LEMBALL 0x0047c820
TargetWaveEffect::~TargetWaveEffect()
{
	MMRESULT result;

	if (m_prepared == 1) {
		result = waveOutUnprepareHeader(m_waveOut, m_waveHeader, 0x20);
		if (result != 0) {
			waveOutUnprepareHeader(m_waveOut, m_waveHeader, 0x20);
		}
		GlobalUnlock(m_sampleHandle);
		GlobalFree(m_sampleHandle);
		GlobalUnlock(m_headerHandle);
		GlobalFree(m_headerHandle);
	}
}
