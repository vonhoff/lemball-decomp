#include "VsSound.h"

#include "../Sound/SoundManager.h"
#include <new.h>

// 68K 0x101038e8 InitSound__FUcUciP4CWndUc
// FUNCTION: LEMBALL 0x0045b770
bool InitSound(unsigned int p_musicEnabled, unsigned int p_effectsEnabled, int p_channelCount, Wnd* p_window, unsigned int p_platformFlag)
{
	g_pSoundManager = new SoundManager(p_musicEnabled, p_effectsEnabled, 1, p_channelCount, p_window);
	return 1;
}

// 68K 0x10103a48 EndSound__Fv
// FUNCTION: LEMBALL 0x0045b7c0
void EndSound()
{
	SoundManager* manager = g_pSoundManager;
	if (manager != 0) {
		manager->~SoundManager();
		operator delete(manager);
	}
	g_pSoundManager = 0;
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
