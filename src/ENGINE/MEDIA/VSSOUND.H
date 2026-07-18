#ifndef LEMBALL_AUDIO_MANAGER_H
#define LEMBALL_AUDIO_MANAGER_H

#include "ENGINE/CORE/COMMON.H"

void __fastcall DestroyAudioManager(void *pAudioManager);
void __fastcall RefreshAudioManagerBackendHandles(void *pAudioManager);
int StopWaveOutPlayback(void *pBackend);
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
    void SetAudioManagerPrimaryContext(void *pPrimaryContext);
    void StopAllAudioManagerBackends(void);
    int CreateVariantResourceEffectInstance(int nEffectResourceId);
    void InvokeAudioManagerEffectSlot48IfActive(int nEffect);
    unsigned char GetVariantResourceEffectMasterVolume(void);
    void SetAudioManagerEffectResourceContext(void *pPrimaryContext);
    void SetAudioManagerMusicResourceContext(void *pPrimaryContext);
    int RegisterVariantResourceMusicHandle(int nMusicResourceId, int nUnused);
    void SetAudioManagerStartupMusicName(const char *pszMusicName);
    void SetAudioManagerMusicEnabledFlag(int fEnabled);
    void PlayVariantResourceEffectId(int nEffectResourceId);
    void SetVariantResourceEffectInstanceVolume(int nEffectInstanceId,
                                                int nVolume);
    void SetAudioManagerMusicAndEffectVolume(int nMusicVolume,
                                             int nEffectVolume);
};

#endif
