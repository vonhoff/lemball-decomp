#include "PvMusicDevice.h"

// 68K 0x1010241c Initialise__14CPVMusicDeviceFUlUl
// FUNCTION: LEMBALL 0x0047f510
void PvMusicDevice::Initialise(unsigned long p_resourceId, unsigned long p_flags)
{
}

// 68K 0x1010244a SetVolume__14CPVMusicDeviceFUc
// FUNCTION: LEMBALL 0x0047f520
void PvMusicDevice::SetVolume(unsigned char p_volume)
{
	m_volume = p_volume;
}

// 68K 0x1010247c GetVolume__14CPVMusicDeviceFv
// FUNCTION: LEMBALL 0x0047f530
unsigned char PvMusicDevice::GetVolume()
{
	return m_volume;
}

// 68K 0x101024aa Process__14CPVMusicDeviceFv
// FUNCTION: LEMBALL 0x0047f540
void PvMusicDevice::Process()
{
}

// 68K 0x101024d0 GetInfo__14CPVMusicDeviceFv
// FUNCTION: LEMBALL 0x0047f550
char* PvMusicDevice::GetInfo()
{
	return 0;
}
