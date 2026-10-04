#include "VsSound.h"

#include "CSoundManager.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045b770
bool InitSound(unsigned int p_musicEnabled,
			   unsigned int p_effectsEnabled,
			   int p_channelCount,
			   CWnd* p_window,
			   unsigned int p_platformFlag)
{
	g_pSoundManager = new CSoundManager(p_musicEnabled, p_effectsEnabled, 1, p_channelCount, p_window);
	return true;
}

// FUNCTION: LEMBALL 0x0045b7c0
void EndSound()
{
	CSoundManager* manager = g_pSoundManager;
	if (manager != NULL) {
		manager->~CSoundManager();
		operator delete(manager);
	}
	g_pSoundManager = NULL;
}
