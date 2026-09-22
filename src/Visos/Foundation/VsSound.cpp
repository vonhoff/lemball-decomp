#include "VsSound.h"

#include "../Sound/CPvMusicDevice.h"
#include "../Sound/CSoundManager.h"
#include "../Target/MciMusicDevice.h"
#include "../Target/TargetDirectSoundDevice.h"
#include "../Target/TargetWaveSoundDevice.h"
#include "Visos/Sound/CBaseSoundDevice.h"

// FUNCTION: LEMBALL 0x0045b770
bool InitSound(unsigned int p_musicEnabled,
			   unsigned int p_effectsEnabled,
			   int p_channelCount,
			   CWnd* p_window,
			   unsigned int p_platformFlag)
{
	g_pSoundManager = new CSoundManager(p_musicEnabled, p_effectsEnabled, 1, p_channelCount, p_window);
	return 1;
}

// FUNCTION: LEMBALL 0x0045b7c0
void EndSound()
{
	CSoundManager* manager = g_pSoundManager;
	if (manager != 0) {
		manager->~CSoundManager();
		operator delete(manager);
	}
	g_pSoundManager = 0;
}

// FUNCTION: LEMBALL 0x00473390
int MachineSoundDetect(CBaseSoundDevice** p_devices,
					   unsigned char p_musicEnabled,
					   unsigned int p_effectsEnabled,
					   unsigned int p_useMusicCD,
					   unsigned int* p_musicAvailable,
					   CPvMusicDevice** p_musicDevice,
					   int p_deviceParameter)
{
	int count = 0;
	*p_musicAvailable = 0;
	*p_musicDevice = 0;
	if (p_useMusicCD == 1) {
		CPvMusicDevice* music = new MciMusicDevice();
		if (music->IsAvailable() == 1) {
			*p_musicAvailable = 1;
			*p_musicDevice = music;
		}
		else if (music != 0) {
			delete music;
		}
		if (p_effectsEnabled == 1) {
			CBaseSoundDevice* device = new TargetDirectSoundDevice(p_deviceParameter, 5);
			if (device->IsEffectAvailable() == 1) {
				*p_devices = device;
				return 1;
			}
			if (device != 0) {
				delete device;
			}
			CBaseSoundDevice* wave = new TargetWaveSoundDevice(p_deviceParameter);
			if (wave->IsEffectAvailable() == 1) {
				*p_devices = wave;
				return 1;
			}
			return 0;
		}
		return 0;
	}
	if (p_effectsEnabled == 1) {
		CBaseSoundDevice* wave = new TargetWaveSoundDevice(p_deviceParameter);
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
