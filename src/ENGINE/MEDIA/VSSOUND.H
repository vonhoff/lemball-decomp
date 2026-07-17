#ifndef LEMBALL_AUDIO_MANAGER_H
#define LEMBALL_AUDIO_MANAGER_H

void DestroyAudioManager(void *pAudioManager);
void SetAudioManagerPrimaryContext(void *pAudioManager, void *pPrimaryContext);
void __fastcall StopAllAudioManagerBackends(void *pAudioManager);
void __fastcall RefreshAudioManagerBackendHandles(void *pAudioManager);
int StopWaveOutPlayback(void *pBackend);
int __fastcall RegisterVariantResourceMusicHandle(
    void *pAudioManager, int nUnused, int nMusicResourceId);
void __fastcall StartRegisteredVariantResourceMusic(
    void *pAudioManager, int nUnused, int hMusic);
void __fastcall DispatchChildInterfaceSlot18IfReady(
    void *pAudioManager, int nUnused, int hMusic);
void __fastcall InvokeAudioManagerEmbeddedSlot1cIfMusicActive(
    void *pAudioManager, int nUnused, int hMusic);
void __fastcall StopVariantResourceMusicPlayback(
    void *pAudioManager, int nUnused, int hMusic);
void __fastcall UnregisterVariantResourceMusicHandle(
    void *pAudioManager, int nUnused, int hMusic);
int CreateVariantResourceEffectInstance(void *pAudioManager, int nEffectResourceId);
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
void __fastcall RestoreAudioEffectBackendBaseVtable(void *pBackend);
int SelectAudioManagerBackends(void **ppEffectBackends,
                               int nMusicResourceBase,
                               int cMusicResources,
                               int fMusicBackendEnabled,
                               int *pfMusicBackendReady,
                               void **ppMusicBackend,
                               int cEffectResources);

extern void *g_pAudioManager;

struct VsSoundManager {
    void *ConstructAudioManager(int nMusicResourceBase,
                                int cMusicResources,
                                int fMusicBackendEnabled,
                                int cEffectResources,
                                void *pPrimaryContext);
    void InvokeAudioManagerEmbeddedSlot04(int nValue1, int nValue2);
    void SetAudioManagerStartupMusicName(const char *pszMusicName);
    void SetAudioManagerMusicEnabledFlag(int fEnabled);
    void PlayVariantResourceEffectId(int nEffectResourceId);
    void SetVariantResourceEffectInstanceVolume(int nEffectInstanceId,
                                                int nVolume);
};

#endif
