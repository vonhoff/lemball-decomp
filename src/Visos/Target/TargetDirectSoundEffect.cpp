#include "TargetDirectSoundEffect.h"

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
