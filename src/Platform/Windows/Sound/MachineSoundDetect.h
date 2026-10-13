#ifndef LEMBALL_PLATFORM_WINDOWS_SOUND_MACHINESOUNDDETECT_H
#define LEMBALL_PLATFORM_WINDOWS_SOUND_MACHINESOUNDDETECT_H

class CBaseSoundDevice;
class CPVMusicDevice;

int MachineSoundDetect(CBaseSoundDevice** p_devices,
					   unsigned char p_musicEnabled,
					   unsigned int p_effectsEnabled,
					   unsigned int p_useMusicCD,
					   unsigned int* p_musicAvailable,
					   CPVMusicDevice** p_musicDevice,
					   int p_effectCapacity);

#endif
