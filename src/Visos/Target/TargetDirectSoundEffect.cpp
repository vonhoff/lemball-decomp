#include "TargetDirectSoundEffect.h"

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

// STUB: LEMBALL 0x0047dba0
bool TargetDirectSoundEffect::SetBufferVolume(int p_index, int p_volume)
{
	IDirectSoundBuffer* buffer = m_buffers[p_index];
	return buffer->SetVolume(p_volume) == 0;
}
