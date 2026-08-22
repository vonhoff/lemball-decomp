#include "VsSound.h"

// 68K 0x101038e8 InitSound__FUcUciP4CWndUc
// STUB: LEMBALL 0x0045b770
bool InitSound(unsigned char p_arg0, unsigned char p_arg1, int p_arg2, Wnd* p_arg3, unsigned char p_arg4)
{
	return 0;
}

// 68K 0x10103a48 EndSound__Fv
// STUB: LEMBALL 0x0045b7c0
void EndSound()
{
}

// 68K 0x101037a0 MachineSoundDetect__FPP16CBaseSoundDeviceUcUcUcPUcPP14CPVMusicDevicei
// STUB: LEMBALL 0x00473390
int MachineSoundDetect(BaseSoundDevice** p_devices,
					   unsigned char p_musicEnabled,
					   unsigned char p_effectsEnabled,
					   unsigned char p_useMusicCD,
					   unsigned char* p_musicAvailable,
					   PvMusicDevice** p_musicDevice,
					   int p_deviceParameter)
{
	return 0;
}
