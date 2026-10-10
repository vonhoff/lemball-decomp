#include "CDirectSoundDevice.h"
#include "CMciMusicDevice.h"
#include "CWaveSoundDevice.h"
#include "Engine/Sound/CBaseSoundDevice.h"
#include "Engine/Sound/CPVMusicDevice.h"
#include "Engine/Sound/VsSound.h"

#include <stddef.h>

enum {
	DIRECT_SOUND_BUFFERS_PER_EFFECT = 5
};

// FUNCTION: LEMBALL 0x00473390
int MachineSoundDetect(CBaseSoundDevice** p_devices,
					   unsigned char p_musicEnabled,
					   unsigned int p_effectsEnabled,
					   unsigned int p_useMusicCD,
					   unsigned int* p_musicAvailable,
					   CPVMusicDevice** p_musicDevice,
					   int p_effectCapacity)
{
	unsigned int* musicAvailable = p_musicAvailable;
	int count = 0;
	*musicAvailable = 0;
	*p_musicDevice = NULL;
	if (p_useMusicCD == 1) {
		CPVMusicDevice* music = new CMciMusicDevice();
		if (music->IsAvailable() == 1) {
			*musicAvailable = 1;
			*p_musicDevice = music;
		}
		else if (music != NULL) {
			delete music;
		}
		if (p_effectsEnabled == 1) {
			CBaseSoundDevice* device = new CDirectSoundDevice(p_effectCapacity, DIRECT_SOUND_BUFFERS_PER_EFFECT);
			if (device->IsEffectAvailable() == 1) {
				*p_devices = device;
				return 1;
			}
			if (device != NULL) {
				delete device;
			}
			CBaseSoundDevice* wave = new CWaveSoundDevice(p_effectCapacity);
			if (wave->IsEffectAvailable() == 1) {
				*p_devices = wave;
				return 1;
			}
			return 0;
		}
		return 0;
	}
	if (p_effectsEnabled == 1) {
		CBaseSoundDevice* wave = new CWaveSoundDevice(p_effectCapacity);
		if (wave->IsEffectAvailable() == 1) {
			count = 1;
			*p_devices = wave;
		}
		else if (wave != NULL) {
			delete wave;
		}
	}
	return count;
}
