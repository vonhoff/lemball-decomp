#ifndef LEMBALL_VISOS_FOUNDATION_VSSOUND_H
#define LEMBALL_VISOS_FOUNDATION_VSSOUND_H

#include "../../Common.h"

bool InitSound(unsigned int p_musicEnabled,
			   unsigned int p_effectsEnabled,
			   int p_channelCount,
			   Wnd* p_window,
			   unsigned int p_platformFlag);
void EndSound();
int MachineSoundDetect(BaseSoundDevice** p_devices,
					   unsigned char p_musicEnabled,
					   unsigned char p_effectsEnabled,
					   unsigned char p_useMusicCD,
					   unsigned char* p_musicAvailable,
					   PvMusicDevice** p_musicDevice,
					   int p_deviceParameter);
#endif
