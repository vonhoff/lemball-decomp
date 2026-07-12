#ifndef LEMBALL_AUDIO_MANAGER_H
#define LEMBALL_AUDIO_MANAGER_H

void DestroyAudioManager(void *pAudioManager);
void SetAudioManagerPrimaryContext(void *pAudioManager, void *pPrimaryContext);
void StopAllAudioManagerBackends(void *pAudioManager);
void RefreshAudioManagerBackendHandles(void *pAudioManager);
int RegisterVariantResourceMusicHandle(void *pAudioManager, int nMusicResourceId);
void StartRegisteredVariantResourceMusic(void *pAudioManager, int hMusic);
void DispatchChildInterfaceSlot18IfReady(void *pAudioManager, int hMusic);
void InvokeAudioManagerEmbeddedSlot1cIfMusicActive(void *pAudioManager, int hMusic);
void StopVariantResourceMusicPlayback(void *pAudioManager, int hMusic);
void UnregisterVariantResourceMusicHandle(void *pAudioManager, int hMusic);
int CreateVariantResourceEffectInstance(void *pAudioManager, int nEffectResourceId);
void PlayVariantResourceEffectId(void *pAudioManager, int nEffectResourceId);
void SetVariantResourceEffectInstanceVolume(void *pAudioManager, int nEffectInstanceId, int nVolume);
void InvokeAudioManagerEffectSlot48IfActive(void *pAudioManager, int nEffect);
void SetAudioManagerMusicAndEffectVolume(void *pAudioManager, int nMusicVolume, int nEffectVolume);
unsigned int GetVariantResourceEffectMasterVolume(void *pAudioManager);
void SetAudioManagerEffectResourceContext(void *pAudioManager, void *pPrimaryContext);
void SetAudioManagerMusicResourceContext(void *pAudioManager, void *pPrimaryContext);
char *__fastcall BuildAudioManagerDescriptionString(void *pAudioManager);
int InitializeGlobalAudioManager(int nMusicResourceBase,
                                 int cMusicResources,
                                 int cEffectResources,
                                 void *pPrimaryContext);
void ShutdownGlobalAudioManager(void);
void *ConstructMciMusicBackend(void *pBackend);
void RestoreAudioEffectBackendBaseVtable(void *pBackend);
int SelectAudioManagerBackends(void **ppEffectBackends,
                               int nMusicResourceBase,
                               int cMusicResources,
                               int fMusicBackendEnabled,
                               int *pfMusicBackendReady,
                               void **ppMusicBackend,
                               int cEffectResources);

extern void *g_pAudioManager;

struct AUDIO_Manager {
    void *ConstructAudioManager(int nMusicResourceBase,
                                int cMusicResources,
                                int fMusicBackendEnabled,
                                int cEffectResources,
                                void *pPrimaryContext);
    void InvokeAudioManagerEmbeddedSlot04(int nValue1, int nValue2);
    void SetAudioManagerStartupMusicName(const char *pszMusicName);
    void SetAudioManagerMusicEnabledFlag(int fEnabled);
};

#endif
