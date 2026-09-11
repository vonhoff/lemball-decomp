#include "TargetDirectSoundEffect.h"

#include "../Foundation/VsOStream.h"
#include "../Foundation/VsString.h"

#include <string.h>

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

// STUB: LEMBALL 0x0047d8c0
int TargetDirectSoundEffect::Play(int p_loop)
{
	return -1;
}

// STUB: LEMBALL 0x0047d940
int TargetDirectSoundEffect::PlayWithVolume(int p_volume, int p_loop)
{
	return -1;
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
