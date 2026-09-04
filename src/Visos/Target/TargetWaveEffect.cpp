#include "TargetWaveEffect.h"

#define WIN32_LEAN_AND_MEAN
#include "../Foundation/VsOStream.h"

// clang-format off: mmsystem.h requires the Win32 types declared by windows.h.
#include <windows.h>
#include <mmsystem.h>
// clang-format on

struct EffPatchHeader {
	unsigned int m_unk0;
	unsigned int m_unk4;
	unsigned short m_unk8;
	unsigned short m_unka;
	unsigned int m_unkc;
	unsigned int m_unk10;
	unsigned int m_unk14;
	unsigned short m_waveCount;
	unsigned short m_unk1a;
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
								   unsigned int p_waveOut,
								   unsigned int p_sampleRate,
								   int p_use16Bit,
								   int p_stereo)
{
	WAVEFORMATEX format;
	EffPatchHeader patchHeader;
	EffWaveHeader waveHeader;
	WAVEHDR* header;
	unsigned char* source;
	unsigned char* dest;
	unsigned int length;
	char* name;
	MMRESULT result;
	union {
		unsigned int downsample;
		char errorText[0x100];
	} work;

	patchHeader = *(EffPatchHeader*) p_patch;
	patchHeader.m_unk8 = TargetByteSwap16(patchHeader.m_unk8);
	patchHeader.m_waveCount = TargetByteSwap16(patchHeader.m_waveCount);
	m_prepared = 0;
	m_waveOut = (void*) p_waveOut;
	if (patchHeader.m_waveCount != 1) {
		name = (char*) p_patch;
		name = name + 6;
		*g_pErrorOutput << "Warning! Effect Patch " << name << " has more than "
						<< "one Wave. Only one is supported!\n";
	}
	waveHeader = *(EffWaveHeader*) (p_patch + sizeof(EffPatchHeader));
	waveHeader.m_unk4 = TargetByteSwap16(waveHeader.m_unk4);
	waveHeader.m_length = TargetByteSwap32(waveHeader.m_length);
	waveHeader.m_sampleRate = TargetByteSwap32(waveHeader.m_sampleRate);
	length = waveHeader.m_length;
	if (p_use16Bit == 0) {
		length = length >> 1;
	}
	work.downsample = 0;
	if (waveHeader.m_sampleRate != p_sampleRate) {
		work.downsample = 1;
		length = length >> 1;
	}
	name = (char*) &patchHeader;
	name = name + 6;
	m_sampleHandle = GlobalAlloc(0x2002, length);
	if (m_sampleHandle == 0) {
		*g_pErrorOutput << "Error! Sound System unable to allocate memory for Wave data " << name << "\n";
	}
	else {
		m_headerHandle = GlobalAlloc(0x2002, 0x20);
		if (m_headerHandle == 0) {
			*g_pErrorOutput << "Error! Sound System unable to allocate memory for Wave Header " << name << "\n";
		}
		else {
			m_sampleData = (unsigned char*) GlobalLock((HGLOBAL) m_sampleHandle);
			if (m_sampleData == 0) {
				*g_pErrorOutput << "Error! Sound System unable to lock memory for Wave data " << name << "\n";
				GlobalUnlock((HGLOBAL) m_sampleHandle);
				GlobalFree((HGLOBAL) m_sampleHandle);
			}
			else {
				m_waveHeader = GlobalLock((HGLOBAL) m_headerHandle);
				if (m_waveHeader == 0) {
					*g_pErrorOutput << "Error! Sound System unable to lock memory for Wave Header" << name << "\n";
					GlobalUnlock((HGLOBAL) m_headerHandle);
					GlobalFree((HGLOBAL) m_headerHandle);
				}
				else {
					header = (WAVEHDR*) m_waveHeader;
					header->lpData = (char*) m_sampleData;
					header->dwBufferLength = length;
					header->dwUser = (DWORD) ((((int) (char) p_patch[9] * 0x100 + (int) (char) p_patch[8]) * 0x100 +
											   (int) (char) p_patch[7]) *
												  0x100 +
											  (int) (char) p_patch[6]);
					header->dwFlags = 0;
					header->dwLoops = 0;
					if (p_use16Bit == 0) {
						source = p_patch + sizeof(EffPatchHeader) + sizeof(EffWaveHeader);
						dest = m_sampleData;
						if (work.downsample == 0) {
							while (length != 0) {
								*dest = *source;
								dest = dest + 1;
								source = source + 2;
								length = length - 1;
							}
						}
						else {
							while (length != 0) {
								*dest = *source;
								source = source + 1;
								dest = dest + 1;
								*source = (unsigned char) (*source + 3);
								length = length - 1;
							}
						}
					}
					else {
						source = p_patch + sizeof(EffPatchHeader) + sizeof(EffWaveHeader);
						if (work.downsample == 0) {
							if ((length & 0xfffffffe) != 0) {
								length = length >> 1;
								dest = m_sampleData;
								while (length != 0) {
									dest[0] = source[1];
									dest[1] = (unsigned char) (source[0] ^ 0x80);
									source = source + 2;
									dest = dest + 2;
									length = length - 1;
								}
							}
						}
						else if ((length & 0xfffffffc) != 0) {
							length = length >> 2;
							dest = m_sampleData;
							while (length != 0) {
								dest[0] = source[1];
								dest[1] = (unsigned char) (source[0] ^ 0x80);
								source = source + 4;
								dest = dest + 2;
								length = length - 1;
							}
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
					format.nBlockAlign = (unsigned short) ((int) ((unsigned int) format.wBitsPerSample *
																  (unsigned int) format.nChannels) >>
														   3);
					if (format.wBitsPerSample == 0x10) {
						format.nAvgBytesPerSec = format.nAvgBytesPerSec * 2;
					}
					result = waveOutOpen((HWAVEOUT*) &m_waveOut, 0xffffffff, &format, 0, 0, WAVE_FORMAT_QUERY);
					if (result == 0) {
						result = waveOutOpen((HWAVEOUT*) &m_waveOut, 0xffffffff, &format, 0, 0, 0);
						if (result == 0) {
							result = waveOutPrepareHeader((HWAVEOUT) m_waveOut, header, 0x20);
							if (result == 0) {
								result = waveOutClose((HWAVEOUT) m_waveOut);
								if (result == 0) {
									m_prepared = 1;
								}
								else {
									*g_pErrorOutput << "Error! Sound System cannot close Wave Device!\n";
									waveOutGetErrorTextA(result, work.errorText, 0x100);
									*g_pErrorOutput << work.errorText << "\n";
								}
							}
							else {
								*g_pErrorOutput << "Error! Sound System cannot prepare Wave Header!\n";
								waveOutGetErrorTextA(result, work.errorText, 0x100);
								*g_pErrorOutput << work.errorText << "\n";
							}
						}
						else {
							*g_pErrorOutput << "Error! Sound System cannot open Wave Device!\n";
							waveOutGetErrorTextA(result, work.errorText, 0x100);
							*g_pErrorOutput << work.errorText << "\n";
						}
					}
					else {
						*g_pErrorOutput << "Error! Sound System cannot support Wave Format!\n";
						waveOutGetErrorTextA(result, work.errorText, 0x100);
						*g_pErrorOutput << work.errorText << "\n";
						*g_pErrorOutput << "Wave Format:\n";
						*g_pErrorOutput << "Samples/Sec: " << format.nSamplesPerSec << "\n";
						*g_pErrorOutput << "Avg Bytes/S: " << format.nAvgBytesPerSec << "\n";
						*g_pErrorOutput << "Align      : " << (unsigned long) format.nBlockAlign << "\n";
						*g_pErrorOutput << "Type       : " << (unsigned long) format.wBitsPerSample << " bit\n";
					}
				}
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0047c820
TargetWaveEffect::~TargetWaveEffect()
{
	MMRESULT result;

	if (m_prepared == 1) {
		result = waveOutUnprepareHeader((HWAVEOUT) m_waveOut, (WAVEHDR*) m_waveHeader, 0x20);
		if (result != 0) {
			waveOutUnprepareHeader((HWAVEOUT) m_waveOut, (WAVEHDR*) m_waveHeader, 0x20);
		}
		GlobalUnlock((HGLOBAL) m_sampleHandle);
		GlobalFree((HGLOBAL) m_sampleHandle);
		GlobalUnlock((HGLOBAL) m_headerHandle);
		GlobalFree((HGLOBAL) m_headerHandle);
	}
}
