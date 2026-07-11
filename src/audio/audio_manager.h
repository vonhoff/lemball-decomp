#ifndef LEMBALL_AUDIO_MANAGER_H
#define LEMBALL_AUDIO_MANAGER_H

void *ConstructAudioManager(void *pAudioManager,
                            int nMusicResourceBase,
                            int cMusicResources,
                            int fMusicBackendEnabled,
                            int cEffectResources,
                            void *pPrimaryContext);
void DestroyAudioManager(void *pAudioManager);
void SetAudioManagerPrimaryContext(void *pAudioManager, void *pPrimaryContext);
void StopAllAudioManagerBackends(void *pAudioManager);
void RefreshAudioManagerBackendHandles(void *pAudioManager);
void InvokeAudioManagerEmbeddedSlot04(void *pAudioManager, int nValue1, int nValue2);
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
void SetAudioManagerStartupMusicName(void *pAudioManager, const char *pszMusicName);
void SetAudioManagerMusicEnabledFlag(void *pAudioManager, int fEnabled);
char *BuildAudioManagerDescriptionString(void *pAudioManager);
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

#endif
