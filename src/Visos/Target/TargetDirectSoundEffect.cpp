#include "TargetDirectSoundEffect.h"

#include "../Foundation/VsOStream.h"
#include "../Foundation/VsString.h"
#include "EffPatchHeader.h"
#include "EffWaveHeader.h"
#include "IDirectSound.h"

#include <string.h>

#define WIN32_LEAN_AND_MEAN
// clang-format off: mmsystem.h requires Win32 types.
#include <windows.h>
#include <mmsystem.h>
// clang-format on

unsigned short TargetByteSwap16(unsigned short p_value);
unsigned int TargetByteSwap32(unsigned int p_value);

struct DirectSoundBufferDescription {
	unsigned int m_size;
	unsigned int m_flags;
	unsigned int m_bytes;
	unsigned int m_reserved;
	WAVEFORMATEX* m_format;
};

struct DirectSoundError {
	const char* name;
	unsigned int code;
};

// GLOBAL: LEMBALL 0x004a3320
static DirectSoundError g_directSoundErrors[] = {
	{"DSERR_ALLOCATED", 10},
	{"DSERR_CONTROLUNAVAIL", 30},
	{"DSERR_INVALIDPARAM", 0x80070057},
	{"DSERR_INVALIDCALL", 50},
	{"DSERR_GENERIC", 0x80004005},
	{"DSERR_PRIOLEVELNEEDED", 70},
	{"DSERR_OUTOFMEMORY", 0x8007000e},
	{"DSERR_BADFORMAT", 100},
	{"DSERR_UNSUPPORTED", 0x80004001},
	{"DSERR_NODRIVER", 120},
	{"DSERR_ALREADYINITIALIZED", 130},
	{"DSERR_NOAGGREGATION", 0x80040110},
	{"DSERR_BUFFERLOST", 150},
	{"DSERR_OTHERAPPHASPRIO", 160},
	{"", 0},
};

// GLOBAL: LEMBALL 0x004a34b8
static const char* g_unknownDirectSoundError = "UNKNOWN DIRECT SOUND ERROR: ";

// Storage covers the observed prefix plus a signed 32-bit decimal error code.
// GLOBAL: LEMBALL 0x004aa128
static char g_directSoundErrorText[sizeof("UNKNOWN DIRECT SOUND ERROR: ") + 11];

// FUNCTION: LEMBALL 0x0047d290
const char* TargetDescribeDirectSoundError(unsigned int p_error)
{
	strcpy(g_directSoundErrorText, g_unknownDirectSoundError);
	int i = 0;
	unsigned int code;
	do {
		code = g_directSoundErrors[i].code;
		if (code == p_error) {
			return g_directSoundErrors[i].name;
		}
		i++;
	} while (code != 0);
	VsLtoa(p_error, g_directSoundErrorText + strlen(g_unknownDirectSoundError), 10);
	return g_directSoundErrorText;
}

// FUNCTION: LEMBALL 0x0047d310
TargetDirectSoundEffect::TargetDirectSoundEffect(int p_bufferCount,
												 unsigned char* p_patch,
												 unsigned int p_sampleRate,
												 int p_use16Bit,
												 int p_stereo,
												 unsigned int p_controlFlags)
{
	unsigned int length;
	unsigned int downsample;
	unsigned long audioBytes[2];
	unsigned char* audio[2];
	DirectSoundBufferDescription description;
	WAVEFORMATEX format;
	EffPatchHeader patchHeader;
	EffWaveHeader waveHeader;
	unsigned char* wave;
	unsigned char* source;
	unsigned int result;
	int index;

	m_unknown04 = 0;
	m_controlFlags = p_controlFlags;
	m_bufferCount = p_bufferCount;
	m_buffers = new IDirectSoundBuffer*[p_bufferCount];
	for (index = 0; index < m_bufferCount; index++) {
		m_buffers[index] = 0;
	}
	memcpy(&patchHeader, p_patch, sizeof(patchHeader));
	patchHeader.m_unk4 = TargetByteSwap16(patchHeader.m_unk4);
	patchHeader.m_waveCount = TargetByteSwap16(patchHeader.m_waveCount);
	m_prepared = 0;
	m_unknown04 = 0;
	if (patchHeader.m_waveCount != 1) {
		// STRING: LEMBALL 0x004a34dc
		*g_pErrorOutput << "Warning! Effect Patch " << ((EffPatchHeader*) p_patch)->m_name << " has more than ";
		// STRING: LEMBALL 0x004a3504
		*g_pErrorOutput << "one Wave. Only one is supported!\n";
	}
	wave = p_patch + sizeof(EffPatchHeader);
	memcpy(&waveHeader, wave, sizeof(waveHeader));
	waveHeader.m_unk4 = TargetByteSwap16(waveHeader.m_unk4);
	waveHeader.m_length = TargetByteSwap32(waveHeader.m_length);
	waveHeader.m_sampleRate = TargetByteSwap32(waveHeader.m_sampleRate);
	length = p_use16Bit != 0 ? waveHeader.m_length : waveHeader.m_length >> 1;
	downsample = 0;
	if (waveHeader.m_sampleRate != p_sampleRate) {
		length >>= 1;
		downsample = 1;
	}
	format.wFormatTag = 1;
	format.nSamplesPerSec = p_sampleRate;
	format.nChannels = 1;
	format.cbSize = 0;
	format.nBlockAlign = 2;
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;
	format.wBitsPerSample = 16;
	memset(&description, 0, sizeof(description));
	description.m_bytes = length;
	description.m_format = &format;
	description.m_flags = 2;
	description.m_size = sizeof(description);
	if ((m_controlFlags & 1) != 0) {
		description.m_flags |= 0x80;
	}
	if ((m_controlFlags & 2) != 0) {
		description.m_flags |= 0x40;
	}
	if ((m_controlFlags & 4) != 0) {
		description.m_flags |= 0x20;
	}
	result = g_directSound->CreateSoundBuffer(&description, m_buffers, 0);
	if (result != 0) {
		// STRING: LEMBALL 0x004a3528
		*g_pErrorOutput << "Effect Buffer Create failed: " << TargetDescribeDirectSoundError(result & 0xfff) << "\n";
		return;
	}
	result = m_buffers[0]->Lock(0, length, (void**) &audio[0], &audioBytes[0], (void**) &audio[1], &audioBytes[1], 0);
	if (result != 0) {
		// STRING: LEMBALL 0x004a354c
		*g_pErrorOutput << "Effect Buffer Lock failed: " << TargetDescribeDirectSoundError(result & 0xfff) << "\n";
		return;
	}
	source = wave + sizeof(EffWaveHeader);
	for (index = 0; index < 2; index++) {
		unsigned char* dest = audio[index];
		if (audioBytes[index] != 0) {
			if (downsample == 0) {
				if ((length >> 1) > 0) {
					unsigned int count = length >> 1;
					do {
						unsigned char high = *source++;
						*dest++ = *source++;
						*dest++ = (unsigned char) (high ^ 0x80);
					} while (--count != 0);
				}
			}
			else if ((length >> 2) > 0) {
				unsigned int count = length >> 2;
				do {
					unsigned char high = *source++;
					*dest++ = *source++;
					*dest++ = (unsigned char) (high ^ 0x80);
					source += 2;
				} while (--count != 0);
			}
		}
	}
	result = m_buffers[0]->Unlock(audio[0], audioBytes[0], audio[1], audioBytes[1]);
	if (result != 0) {
		// STRING: LEMBALL 0x004a356c
		*g_pErrorOutput << "Effect Buffer Unlock failed: " << TargetDescribeDirectSoundError(result & 0xfff) << "\n";
		return;
	}
	for (index = 1; index < m_bufferCount; index++) {
		result = g_directSound->DuplicateSoundBuffer(m_buffers[0], &m_buffers[index]);
		if (result != 0) {
			// STRING: LEMBALL 0x004a3590
			*g_pErrorOutput << "Duplicate Effect Buffer Create failed: "
							<< TargetDescribeDirectSoundError(result & 0xfff) << "\n";
			return;
		}
	}
	m_prepared = 1;
}

// FUNCTION: LEMBALL 0x0047d6e0
TargetDirectSoundEffect::~TargetDirectSoundEffect()
{
	for (int i = 0; m_bufferCount > i; i++) {
		if (m_buffers[i] != 0) {
			m_buffers[i]->Release();
		}
	}
	delete[] m_buffers;
}

// FUNCTION: LEMBALL 0x0047d720
int TargetDirectSoundEffect::FindIdleBuffer()
{
	int i;
	unsigned long status;
	for (i = 0; i < m_bufferCount; i++) {
		unsigned int result = m_buffers[i]->GetStatus(&status);
		if (result != 0) {
			*g_pErrorOutput << "Effect Buffer Status Request failed: " << TargetDescribeDirectSoundError(result & 0xfff)
							<< "\n";
			return -1;
		}
		if ((status & 1) == 0) {
			return i;
		}
	}
	return -1;
}

// FUNCTION: LEMBALL 0x0047d830
bool TargetDirectSoundEffect::IsPlaying()
{
	unsigned long status;
	for (int i = 0; i < m_bufferCount; i++) {
		unsigned int result = m_buffers[i]->GetStatus(&status);
		if (result != 0) {
			*g_pErrorOutput << "Effect Buffer Status Request failed: " << TargetDescribeDirectSoundError(result & 0xfff)
							<< "\n";
			return 0;
		}
		if (((unsigned char) status & 1) != 0) {
			return 1;
		}
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0047d8c0
int TargetDirectSoundEffect::Play(int p_loop)
{
	m_looping = p_loop;
	int index = FindIdleBuffer();
	if (index != -1) {
		unsigned int result = m_buffers[index]->SetCurrentPosition(0);
		if (result != 0) {
			*g_pErrorOutput << "Effect Set Current Position failed: " << TargetDescribeDirectSoundError(result & 0xfff)
							<< "\n";
			return -1;
		}
		PlayBuffer(index);
	}
	return index;
}

// FUNCTION: LEMBALL 0x0047d940
int TargetDirectSoundEffect::PlayWithVolume(int p_volume, int p_loop)
{
	m_looping = p_loop;
	int index = FindIdleBuffer();
	if (index != -1) {
		unsigned int result = m_buffers[index]->SetCurrentPosition(0);
		if (result != 0) {
			*g_pErrorOutput << "Effect Set Current Position failed: " << TargetDescribeDirectSoundError(result & 0xfff)
							<< "\n";
			return -1;
		}
		SetBufferVolume(index, p_volume);
		PlayBuffer(index);
	}
	return index;
}

// FUNCTION: LEMBALL 0x0047da20
void TargetDirectSoundEffect::PlayBuffer(int p_index)
{
	unsigned int result = m_buffers[p_index]->Play(0, 0, m_looping != 0);
	if (result != 0) {
		*g_pErrorOutput << "Effect Play failed: " << TargetDescribeDirectSoundError(result & 0xfff) << "\n";
	}
}

// FUNCTION: LEMBALL 0x0047dad0
void TargetDirectSoundEffect::Stop()
{
	for (int i = 0; i < m_bufferCount; i++) {
		unsigned int result = m_buffers[i]->Stop();
		if (result != 0) {
			*g_pErrorOutput << "Effect Stop failed: " << TargetDescribeDirectSoundError(result & 0xfff) << "\n";
			return;
		}
	}
}

// FUNCTION: LEMBALL 0x0047dba0
bool TargetDirectSoundEffect::SetBufferVolume(int p_index, int p_volume)
{
	unsigned int result = m_buffers[p_index]->SetVolume(p_volume);
	if (result != 0) {
		*g_pErrorOutput << "Effect Buffer Set Volume Request failed: " << TargetDescribeDirectSoundError(result & 0xfff)
						<< "\n";
		return 0;
	}
	return 1;
}
