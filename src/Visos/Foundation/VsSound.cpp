#include "VsSound.h"

#include "../Sound/PvMusicDevice.h"
#include "../Sound/SoundManager.h"
#include "../Target/MciMusicDevice.h"
#include "../Target/TargetDirectSoundDevice.h"
#include "../Target/TargetWaveSoundDevice.h"

#include <new.h>

// 68K 0x101038e8 InitSound__FUcUciP4CWndUc
// FUNCTION: LEMBALL 0x0045b770
bool InitSound(unsigned int p_musicEnabled,
			   unsigned int p_effectsEnabled,
			   int p_channelCount,
			   Wnd* p_window,
			   unsigned int p_platformFlag)
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
// FUNCTION: LEMBALL 0x00473390
int MachineSoundDetect(BaseSoundDevice** p_devices,
					   unsigned char p_musicEnabled,
					   unsigned int p_effectsEnabled,
					   unsigned int p_useMusicCD,
					   unsigned int* p_musicAvailable,
					   PvMusicDevice** p_musicDevice,
					   int p_deviceParameter)
{
	int count = 0;
	*p_musicAvailable = 0;
	*p_musicDevice = 0;
	if (p_useMusicCD == 1) {
		PvMusicDevice* music = new MciMusicDevice();
		if (music->IsAvailable() == 1) {
			*p_musicAvailable = 1;
			*p_musicDevice = music;
		}
		else if (music != 0) {
			delete music;
		}
		if (p_effectsEnabled == 1) {
			BaseSoundDevice* device = new TargetDirectSoundDevice(p_deviceParameter, 5);
			if (device->IsEffectAvailable() == 1) {
				*p_devices = device;
				return 1;
			}
			if (device != 0) {
				delete device;
			}
			BaseSoundDevice* wave = new TargetWaveSoundDevice(p_deviceParameter);
			if (wave->IsEffectAvailable() == 1) {
				*p_devices = wave;
				return 1;
			}
			return 0;
		}
		return 0;
	}
	if (p_effectsEnabled == 1) {
		BaseSoundDevice* wave = new TargetWaveSoundDevice(p_deviceParameter);
		if (wave->IsEffectAvailable() == 1) {
			count = 1;
			*p_devices = wave;
		}
		else if (wave != 0) {
			delete wave;
		}
	}
	return count;
}
