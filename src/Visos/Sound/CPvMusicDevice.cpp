#include "CPvMusicDevice.h"

// FUNCTION: LEMBALL 0x0047f510
void CPvMusicDevice::Initialise(unsigned long p_resourceId, unsigned long p_flags)
{
}

// FUNCTION: LEMBALL 0x0047f520
void CPvMusicDevice::SetVolume(unsigned char p_volume)
{
	m_volume = p_volume;
}

// FUNCTION: LEMBALL 0x0047f530
unsigned char CPvMusicDevice::GetVolume()
{
	return m_volume;
}

// FUNCTION: LEMBALL 0x0047f540
void CPvMusicDevice::Process()
{
}

// FUNCTION: LEMBALL 0x0047f550
char* CPvMusicDevice::GetInfo()
{
	return 0;
}
