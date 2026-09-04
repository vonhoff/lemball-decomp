#include "TargetDirectSoundDevice.h"

// STUB: LEMBALL 0x0047dd80
TargetDirectSoundDevice::TargetDirectSoundDevice(int p_channelCount, int p_flags)
{
	int i;

	i = 0;
	while (i < 0x50) {
		m_platformState[i] = 0;
		i = i + 1;
	}
	m_platformState[0] = (unsigned char) p_channelCount;
	m_platformState[1] = (unsigned char) p_flags;
}

int TargetDirectSoundDevice::Dummy28()
{
	return 0;
}

TargetDirectSoundDevice::~TargetDirectSoundDevice()
{
}
