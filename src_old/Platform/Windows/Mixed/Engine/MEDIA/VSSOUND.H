#ifndef LEMBALL_AUDIO_MANAGER_H
#define LEMBALL_AUDIO_MANAGER_H

#include "Platform/Windows/Mixed/Engine/CORE/COMMON.H"

void __fastcall DestroyAudioManager(void* pAudioManager);
void __fastcall RefreshAudioManagerBackendHandles(void* pAudioManager);
int StopWaveOutPlayback(void* pBackend);
char* __fastcall BuildAudioManagerDescriptionString(void* pAudioManager);
int InitializeGlobalAudioManager(int nMusicResourceBase,
								 int cMusicResources,
								 int cEffectResources,
								 void* pPrimaryContext);
void ShutdownGlobalAudioManager(void);
void* ConstructMciMusicBackend(void* pBackend);
void __fastcall RestoreAudioEffectBackendBaseVtable(void* pBackend);
int SelectAudioManagerBackends(void** ppEffectBackends,
							   int nMusicResourceBase,
							   int cMusicResources,
							   int fMusicBackendEnabled,
							   int* pfMusicBackendReady,
							   void** ppMusicBackend,
							   int cEffectResources);

extern void* g_pAudioManager;

struct CSoundManager {
	void* ConstructAudioManager(int nMusicResourceBase,
								int cMusicResources,
								int fMusicBackendEnabled,
								int cEffectResources,
								void* pPrimaryContext);
	void InvokeAudioManagerEmbeddedSlot04(int nValue1, int nValue2);
	void SetResId(void* pPrimaryContext);
	void Background(void);
	int PrepareEffect(int nEffectResourceId);
	void FreeEffect(int nEffect);
	unsigned char GetEffectVolume(void);
	void SetEffectsWnd(void* pPrimaryContext);
	void SetMusicWnd(void* pPrimaryContext);
	int PrepareMusic(int nMusicResourceId, int nUnused);
	void PlayMusic(int hMusic);
	void ProcessMusic(unsigned long nDuration);
	void DispatchChildInterfaceSlot18IfReady(int hMusic);
	void InvokeAudioManagerEmbeddedSlot1cIfMusicActive(int hMusic);
	void StopMusic(int hMusic);
	void FreeMusic(int hMusic);
	void SetAudioManagerStartupMusicName(const char* pszMusicName);
	void UseMusicCD(int fEnabled);
	void PlayEffect(int nEffectResourceId);
	void PlayEffect(int nEffectInstanceId, int nVolume);
	void SetVolumes(int nMusicVolume, int nEffectVolume);
};

#endif
