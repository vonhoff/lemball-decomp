#include "CPVMusicDevice.h"

// FUNCTION: LEMBALL 0x0047f510
void CPVMusicDevice::Initialise(unsigned long p_resourceId, unsigned long p_flags)
{
}

// FUNCTION: LEMBALL 0x0047f520
void CPVMusicDevice::SetVolume(unsigned char p_volume)
{
	m_volume = p_volume;
}

// FUNCTION: LEMBALL 0x0047f530
unsigned char CPVMusicDevice::GetVolume()
{
	return m_volume;
}

// FUNCTION: LEMBALL 0x0047f540
void CPVMusicDevice::Process()
{
}

// FUNCTION: LEMBALL 0x0047f550
char* CPVMusicDevice::GetInfo()
{
	return 0;
}
