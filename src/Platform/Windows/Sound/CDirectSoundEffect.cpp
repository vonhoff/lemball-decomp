#include "CDirectSoundEffect.h"

enum {
	SOUND_EFFECT_NO_BUFFER_INDEX = -1
};

#include "DirectSound.h"
#include "Engine/Sound/EffectFormat.h"
#include "Engine/Streams/CVSOStream.h"
#include "Engine/Strings/VsString.h"
#include "Platform/Windows/DirectX/DirectSound.h"

#include <string.h>

enum {
	DIRECT_SOUND_EFFECT_CONTROL_VOLUME = 0x01,
	DIRECT_SOUND_EFFECT_CONTROL_PAN = 0x02,
	DIRECT_SOUND_EFFECT_CONTROL_FREQUENCY = 0x04
};

#define WIN32_LEAN_AND_MEAN
// clang-format off
#include <windows.h>
#include <mmsystem.h>

// clang-format on

struct DirectSoundError {
	const char* m_name;
	unsigned int m_code;
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

// GLOBAL: LEMBALL 0x004aa128
static char g_directSoundErrorText[sizeof("UNKNOWN DIRECT SOUND ERROR: ") + 11];

// FUNCTION: LEMBALL 0x0047d290
const char* DescribeDirectSoundError(unsigned int p_error)
{
	const char* prefix = g_unknownDirectSoundError;
	strcpy(g_directSoundErrorText, prefix);
	int i = 0;
	unsigned int code;
	const DirectSoundError* entry = g_directSoundErrors;
	do {
		code = entry->m_code;
		if (code == p_error) {
			return g_directSoundErrors[i].m_name;
		}
		entry++;
		i++;
	} while (code != 0);
	vsLtoa(p_error, g_directSoundErrorText + strlen(prefix), 10);
	return g_directSoundErrorText;
}

// FUNCTION: LEMBALL 0x0047d310
CDirectSoundEffect::CDirectSoundEffect(int p_bufferCount,
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
	DSBUFFERDESC description;
	WAVEFORMATEX format;
	EffPatchHeader patchHeader;
	EffWaveHeader waveHeader;
	unsigned char* wave;
	unsigned char* source;
	long result;
	int index;

	m_unknown04 = 0;
	m_controlFlags = p_controlFlags;
	m_bufferCount = p_bufferCount;
	m_buffers = new IDirectSoundBuffer*[p_bufferCount];
	for (index = 0; index < m_bufferCount; index++) {
		m_buffers[index] = NULL;
	}
	memcpy(&patchHeader, p_patch, sizeof(patchHeader));
	patchHeader.m_formatVersion = SwapBytes16(patchHeader.m_formatVersion);
	patchHeader.m_waveCount = SwapBytes16(patchHeader.m_waveCount);
	m_prepared = 0;
	m_unknown04 = 0;
	if (patchHeader.m_waveCount != EFFECT_PATCH_SUPPORTED_WAVE_COUNT) {
		// STRING: LEMBALL 0x004a34dc
		*g_pErrorOutput << "Warning! Effect Patch " << ((EffPatchHeader*) p_patch)->m_name << " has more than ";
		// STRING: LEMBALL 0x004a3504
		*g_pErrorOutput << "one Wave. Only one is supported!\n";
	}
	wave = p_patch + sizeof(EffPatchHeader);
	memcpy(&waveHeader, wave, sizeof(waveHeader));
	waveHeader.m_formatVersion = SwapBytes16(waveHeader.m_formatVersion);
	waveHeader.m_length = SwapBytes32(waveHeader.m_length);
	waveHeader.m_sampleRate = SwapBytes32(waveHeader.m_sampleRate);
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
	description.dwBufferBytes = length;
	description.lpwfxFormat = &format;
	description.dwFlags = DSBCAPS_STATIC;
	description.dwSize = sizeof(description);
	if ((m_controlFlags & DIRECT_SOUND_EFFECT_CONTROL_VOLUME) != 0) {
		description.dwFlags |= DSBCAPS_CTRLVOLUME;
	}
	if ((m_controlFlags & DIRECT_SOUND_EFFECT_CONTROL_PAN) != 0) {
		description.dwFlags |= DSBCAPS_CTRLPAN;
	}
	if ((m_controlFlags & DIRECT_SOUND_EFFECT_CONTROL_FREQUENCY) != 0) {
		description.dwFlags |= DSBCAPS_CTRLFREQUENCY;
	}
	result = g_directSound->CreateSoundBuffer(&description, m_buffers, NULL);
	if (result != 0) {
		// STRING: LEMBALL 0x004a3528
		*g_pErrorOutput << "Effect Buffer Create failed: "
						<< DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK) << "\n";
		return;
	}
	result = m_buffers[0]->Lock(0, length, (void**) &audio[0], &audioBytes[0], (void**) &audio[1], &audioBytes[1], 0);
	if (result != 0) {
		// STRING: LEMBALL 0x004a354c
		*g_pErrorOutput << "Effect Buffer Lock failed: "
						<< DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK) << "\n";
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
						*dest++ = (unsigned char) (high ^ PCM_8BIT_SIGN_BIT_MASK);
					} while (--count != 0);
				}
			}
			else if ((length >> 2) > 0) {
				unsigned int count = length >> 2;
				do {
					unsigned char high = *source++;
					*dest++ = *source++;
					*dest++ = (unsigned char) (high ^ PCM_8BIT_SIGN_BIT_MASK);
					source += 2;
				} while (--count != 0);
			}
		}
	}
	result = m_buffers[0]->Unlock(audio[0], audioBytes[0], audio[1], audioBytes[1]);
	if (result != 0) {
		// STRING: LEMBALL 0x004a356c
		*g_pErrorOutput << "Effect Buffer Unlock failed: "
						<< DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK) << "\n";
		return;
	}
	for (index = 1; index < m_bufferCount; index++) {
		result = g_directSound->DuplicateSoundBuffer(m_buffers[0], &m_buffers[index]);
		if (result != 0) {
			// STRING: LEMBALL 0x004a3590
			*g_pErrorOutput << "Duplicate Effect Buffer Create failed: "
							<< DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK) << "\n";
			return;
		}
	}
	m_prepared = 1;
}

// FUNCTION: LEMBALL 0x0047d6e0
CDirectSoundEffect::~CDirectSoundEffect()
{
	for (int i = 0; m_bufferCount > i; i++) {
		if (m_buffers[i] != NULL) {
			m_buffers[i]->Release();
		}
	}
	delete[] m_buffers;
}

// FUNCTION: LEMBALL 0x0047d720
int CDirectSoundEffect::FindIdleBuffer()
{
	int i;
	unsigned long status;
	for (i = 0; i < m_bufferCount; i++) {
		long result = m_buffers[i]->GetStatus(&status);
		if (result != 0) {
			*g_pErrorOutput << "Effect Buffer Status Request failed: "
							<< DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK) << "\n";
			return SOUND_EFFECT_NO_BUFFER_INDEX;
		}
		if ((status & DIRECT_SOUND_BUFFER_STATUS_PLAYING) == 0) {
			return i;
		}
	}
	return SOUND_EFFECT_NO_BUFFER_INDEX;
}

// FUNCTION: LEMBALL 0x0047d830
bool CDirectSoundEffect::IsPlaying()
{
	unsigned long status;
	for (int i = 0; i < m_bufferCount; i++) {
		long result = m_buffers[i]->GetStatus(&status);
		if (result != 0) {
			*g_pErrorOutput << "Effect Buffer Status Request failed: "
							<< DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK) << "\n";
			return false;
		}
		if (((unsigned char) status & DIRECT_SOUND_BUFFER_STATUS_PLAYING) != 0) {
			return true;
		}
	}
	return false;
}

// FUNCTION: LEMBALL 0x0047d8c0
int CDirectSoundEffect::Play(int p_loop)
{
	m_looping = p_loop;
	int index = FindIdleBuffer();
	if (index != SOUND_EFFECT_NO_BUFFER_INDEX) {
		long result = m_buffers[index]->SetCurrentPosition(0);
		if (result != 0) {
			*g_pErrorOutput << "Effect Set Current Position failed: "
							<< DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK) << "\n";
			return SOUND_EFFECT_NO_BUFFER_INDEX;
		}
		PlayBuffer(index);
	}
	return index;
}

// FUNCTION: LEMBALL 0x0047d940
int CDirectSoundEffect::PlayWithVolume(int p_volume, int p_loop)
{
	m_looping = p_loop;
	int index = FindIdleBuffer();
	if (index != SOUND_EFFECT_NO_BUFFER_INDEX) {
		long result = m_buffers[index]->SetCurrentPosition(0);
		if (result != 0) {
			*g_pErrorOutput << "Effect Set Current Position failed: "
							<< DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK) << "\n";
			return SOUND_EFFECT_NO_BUFFER_INDEX;
		}
		SetBufferVolume(index, p_volume);
		PlayBuffer(index);
	}
	return index;
}

// FUNCTION: LEMBALL 0x0047da20
void CDirectSoundEffect::PlayBuffer(int p_index)
{
	long result = m_buffers[p_index]->Play(0, 0, m_looping != 0);
	if (result != 0) {
		*g_pErrorOutput << "Effect Play failed: " << DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK)
						<< "\n";
	}
}

// FUNCTION: LEMBALL 0x0047dad0
void CDirectSoundEffect::Stop()
{
	int i = 0;
	while (i < m_bufferCount) {
		IDirectSoundBuffer** slot = &m_buffers[i];
		long result = (*slot)->Stop();
		if (result != 0) {
			*g_pErrorOutput << "Effect Stop failed: " << DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK)
							<< "\n";
			return;
		}
		++i;
	}
}

// FUNCTION: LEMBALL 0x0047dba0
bool CDirectSoundEffect::SetBufferVolume(int p_index, int p_volume)
{
	IDirectSoundBuffer*& buffer = m_buffers[p_index];
	long result = buffer->SetVolume(p_volume);
	if (result != 0) {
		*g_pErrorOutput << "Effect Buffer Set Volume Request failed: "
						<< DescribeDirectSoundError(result & DIRECT_SOUND_ERROR_CODE_MASK) << "\n";
		return false;
	}
	return true;
}
