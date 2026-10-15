#ifndef LEMBALL_VISOS_FOUNDATION_VSSOUND_H
#define LEMBALL_VISOS_FOUNDATION_VSSOUND_H

class CWnd;

bool InitSound(unsigned int p_musicEnabled,
			   unsigned int p_effectsEnabled,
			   int p_channelCount,
			   CWnd* p_window,
			   unsigned int p_platformFlag);
void EndSound();
#endif
