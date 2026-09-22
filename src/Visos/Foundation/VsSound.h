#ifndef LEMBALL_VISOS_FOUNDATION_VSSOUND_H
#define LEMBALL_VISOS_FOUNDATION_VSSOUND_H

class CBaseSoundDevice;
class CPvMusicDevice;
class CWnd;

bool InitSound(unsigned int p_musicEnabled,
			   unsigned int p_effectsEnabled,
			   int p_channelCount,
			   CWnd* p_window,
			   unsigned int p_platformFlag);
void EndSound();
int MachineSoundDetect(CBaseSoundDevice** p_devices,
					   unsigned char p_musicEnabled,
					   unsigned int p_effectsEnabled,
					   unsigned int p_useMusicCD,
					   unsigned int* p_musicAvailable,
					   CPvMusicDevice** p_musicDevice,
					   int p_deviceParameter);
#endif
