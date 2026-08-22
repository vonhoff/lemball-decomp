#ifndef LEMBALL_SCAFFOLD_VISOS_VSSOUND_H
#define LEMBALL_SCAFFOLD_VISOS_VSSOUND_H

#include "../Common.h"

bool InitSound(unsigned char p_arg0, unsigned char p_arg1, int p_arg2, Wnd* p_arg3, unsigned char p_arg4);
void EndSound();
int MachineSoundDetect(BaseSoundDevice** p_devices,
					   unsigned char p_musicEnabled,
					   unsigned char p_effectsEnabled,
					   unsigned char p_useMusicCD,
					   unsigned char* p_musicAvailable,
					   PvMusicDevice** p_musicDevice,
					   int p_deviceParameter);
#endif
