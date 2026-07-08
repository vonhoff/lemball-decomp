#include "audio_manager.h"

#include "../game/game_app.h"
#include "../main.h"
#include "../resource/resource_archive.h"
#include "../engine/memory_arena.h"
#include "../engine/runtime_init.h"

#include <string.h>

typedef int (*AUDIO_TestProc)(void);
typedef int (*AUDIO_OpenProc)(int nResourceBase, int cResources, void *pPrimaryContext);
typedef void (*AUDIO_VoidProc)(void);
typedef void (*AUDIO_IntProc)(int nValue);
typedef int (*AUDIO_CreateEffectProc)(int nValue, void *pOut);
typedef void (*AUDIO_EffectPlayProc)(int nEffectResourceId, int nVolume);
typedef void (*AUDIO_EffectVolumeProc)(int nEffectInstanceId, int nVolume, int nReserved);
typedef int (*AUDIO_ReturnIntProc)(void);
typedef int (*AUDIO_ReturnIntArgProc)(int nValue);
typedef const char *(*AUDIO_NameProc)(void);
typedef void (*AUDIO_ContextProc)(void *pPrimaryContext);
typedef void *(*AUDIO_DeleteProc)(void *pObject, int fDelete);

struct AUDIO_ResourceObject {
    void **m_pVtable;
    int m_nReserved04;
    int m_nLockCount08;
    int m_cReferences;
    int m_nLoadState10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1C;
    int m_nReserved20;
    int m_nReserved24;
    int m_cbResourceData28;
    int m_lResourceOffset2C;
    int m_nResourceId30;
    unsigned int *m_padwResourceDescriptor34;
    void *m_pResourceData38;
    int m_nReserved3C;
    unsigned int m_uTypeTag;
    int m_nResultCode44;
};

struct AUDIO_BackendControl {
    void **m_pVtable;
    int m_nReserved04;
    int m_fStartupMusicNameSet;
    int m_fMusicEnabled;
    GAME_DynamicCString m_StartupMusicName;
};

struct AUDIO_WaveOutEffectBackend {
    void **m_pVtable;
    int m_cEffectResources;
    int m_nReserved08;
    int m_fDeviceAvailable;
    char m_abReserved10[0x48];
    char m_szDeviceDescription[0x34];
};

struct AUDIO_DirectSoundEffectBackend {
    void **m_pVtable;
    HMODULE m_hDsoundModule;
    FARPROC m_pfnDirectSoundCreate;
    int m_nReserved0c;
    int m_nReserved10;
    int m_cEffectResources;
    int m_nCooperativeLevel;
    int m_nReserved1c;
    int m_fDeviceAvailable;
};

struct AUDIO_MciMusicBackend {
    void **m_pVtable;
    int m_nReserved04;
    int m_fBasePathSet;
    int m_fSearchCdromPath;
    GAME_DynamicCString m_BasePath;
    int m_nPreparedTrackHandle;
    int m_fDeviceAvailable;
    MCIDEVICEID m_uDeviceId;
    int m_fTrackPlaying;
    int m_fTrackPaused;
    int m_nPausedTrackPosition;
    HWND m_hNotificationWindow;
};

struct AUDIO_MciOpenParms {
    DWORD dwCallback;
    MCIDEVICEID wDeviceID;
    LPCSTR lpstrDeviceType;
    LPCSTR lpstrElementName;
    LPCSTR lpstrAlias;
};

struct AUDIO_MciGenericParms {
    DWORD dwCallback;
};

struct AUDIO_MciSetParms {
    DWORD dwCallback;
    DWORD dwTimeFormat;
};

struct AUDIO_MciPlayParms {
    DWORD dwCallback;
    DWORD dwFrom;
    DWORD dwTo;
};

struct AUDIO_MciStatusParms {
    DWORD dwCallback;
    DWORD dwReturn;
    DWORD dwItem;
    DWORD dwTrack;
};

struct AUDIO_DynamicStringEntry {
    void **m_pVtable;
    int m_nReserved04;
    int m_fReserved08;
    int m_fReserved0c;
    GAME_DynamicCString m_Text;
};

const char *ReturnEmptyAudioBackendName(void);
int ReturnAudioBackendReady(void);
int ReturnAudioBackendUnavailable(void);
int AcceptAudioBackendResources(int nResourceBase, int cResources, void *pPrimaryContext);
int CreateFallbackEffectInstance(int nValue, void *pOut);
void NoOpAudioInt(int nValue);
void NoOpAudioPair(int nValue1, int nValue2);
void NoOpAudioTriple(int nValue1, int nValue2, int nValue3);
void DestroyAudioDynamicStringEntry(AUDIO_DynamicStringEntry *pEntry);
void *DeleteAudioDynamicStringEntry(AUDIO_DynamicStringEntry *pEntry, unsigned char fDelete);
void DestroyWaveOutEffectBackend(void *pBackend);
char *GetWaveOutEffectBackendDescription(AUDIO_WaveOutEffectBackend *pBackend);
int IsWaveOutEffectBackendAvailable(void);
int CreateWaveOutEffectInstance(void *pBackend, void *pPatchResource, int *pnEffectInstanceId);
int FreeWaveOutEffectInstance(void *pBackend, int nEffectInstanceId);
unsigned int PlayWaveOutEffectInstance(void *pBackend, int nEffectInstanceId);
void *DeleteWaveOutEffectBackend(AUDIO_WaveOutEffectBackend *pBackend, unsigned char fDelete);
void DestroyDirectSoundEffectBackend(void *pBackend);
char *GetDirectSoundEffectBackendDescription(AUDIO_DirectSoundEffectBackend *pBackend);
int CreateDirectSoundEffectInstance(void *pBackend, void *pPatchResource, int *pnEffectInstanceId, int nFlags);
int FreeDirectSoundEffectInstance(void *pBackend, int nEffectInstanceId);
int ReleaseAllDirectSoundEffectInstances(void *pBackend);
void PlayDirectSoundEffectInstanceWithMappedVolume(void *pBackend, int nEffectInstanceId, unsigned char nVolume,
                                                   int fLoopEnabled);
void PauseDirectSoundEffectInstance(void *pBackend, int nEffectInstanceId, int nBufferSlot);
void PlayDirectSoundEffectInstance(void *pBackend, int nEffectInstanceId, int nBufferSlot);
void *DeleteDirectSoundEffectBackend(AUDIO_DirectSoundEffectBackend *pBackend, unsigned char fDelete);
void PrepareMciMusicTrack(AUDIO_MciMusicBackend *pBackend, int nTrackHandle, int nMusicResourceId);
void FreePreparedMciMusicTrack(AUDIO_MciMusicBackend *pBackend, int nTrackHandle);
void PlayPreparedMciMusicTrack(AUDIO_MciMusicBackend *pBackend, int nTrackHandle);
void StopMciMusicTrack(AUDIO_MciMusicBackend *pBackend, int nTrackHandle);
void PauseMciMusicTrack(AUDIO_MciMusicBackend *pBackend, int nTrackHandle);

void *g_pAudioManager = 0;

static void *g_AudioManagerVtable[1] = { 0 };
static void *g_WaveOutEffectBackendVtable[39] = {
    (void *)DeleteWaveOutEffectBackend,
    (void *)GetWaveOutEffectBackendDescription,
    0,
    (void *)AcceptAudioBackendResources,
    (void *)AcceptAudioBackendResources,
    (void *)NoOpAudioInt,
    0,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioInt,
    (void *)ReturnAudioBackendUnavailable,
    (void *)IsWaveOutEffectBackendAvailable,
    (void *)ReturnAudioBackendReady,
    0,
    0,
    0,
    (void *)CreateWaveOutEffectInstance,
    0,
    0,
    (void *)FreeWaveOutEffectInstance,
    (void *)ReturnAudioBackendReady,
    (void *)PlayWaveOutEffectInstance,
    0,
    (void *)NoOpAudioInt,
    0,
    (void *)NoOpAudioInt,
    (void *)ReturnAudioBackendReady,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioTriple,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (void *)NoOpAudioTriple,
    0,
    0,
    (void *)NoOpAudioInt,
};
static void *g_DirectSoundEffectBackendVtable[39] = {
    (void *)DeleteDirectSoundEffectBackend,
    (void *)GetDirectSoundEffectBackendDescription,
    0,
    (void *)AcceptAudioBackendResources,
    (void *)AcceptAudioBackendResources,
    (void *)NoOpAudioInt,
    0,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioInt,
    (void *)ReturnAudioBackendUnavailable,
    (void *)ReturnAudioBackendReady,
    (void *)ReturnAudioBackendReady,
    0,
    0,
    0,
    (void *)CreateDirectSoundEffectInstance,
    0,
    0,
    (void *)FreeDirectSoundEffectInstance,
    (void *)ReturnAudioBackendReady,
    (void *)PlayDirectSoundEffectInstanceWithMappedVolume,
    0,
    (void *)NoOpAudioInt,
    0,
    (void *)NoOpAudioInt,
    (void *)ReturnAudioBackendReady,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioTriple,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (void *)NoOpAudioTriple,
    0,
    0,
    (void *)NoOpAudioInt,
};
static void *g_MciMusicBackendVtable[13] = {
    (void *)DeleteAudioDynamicStringEntry,
    (void *)ReturnEmptyAudioBackendName,
    (void *)PrepareMciMusicTrack,
    (void *)FreePreparedMciMusicTrack,
    (void *)PlayPreparedMciMusicTrack,
    (void *)StopMciMusicTrack,
    (void *)PauseMciMusicTrack,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioPair,
    (void *)NoOpAudioInt,
    (void *)ReturnAudioBackendReady,
    (void *)ReturnEmptyAudioBackendName,
};
static char g_szAudioManagerDescription[0x400];
static void *g_AudioDynamicStringEntryVtable = (void *)0x0049ADB0;
static AUDIO_MciMusicBackend *g_pActiveMciMusicBackend = 0;
static int g_nPreparedMciMusicTrackHandle = 0;
static const char g_AUDIO_SequencerDeviceType[] = "sequencer";
static const char g_AUDIO_MciWindowClassName[] = "HLMusicWindow";
static const char g_AUDIO_NoEffectsBackendName[] = "ERROR: No Effects Device for Windows";
static const char g_AUDIO_NoDirectSoundBackendName[] = "ERROR: No Effects Device for Windows";
static const char g_AUDIO_HlMidiNotFoundPrefix[] = "Error: HL Midi Device Not Found. ";
static const char g_AUDIO_MciErrorPrefix[] = "MCI Error: ";
static const char g_AUDIO_ErrorSuffix[] = "\n";
static const char g_AUDIO_MusicExtension[] = ".mid";
static const char g_AUDIO_PathSeparator[] = "\\";

// FUNCTION: LEMBALL 0x0045AF80
void *ConstructAudioManager(void *pAudioManager,
                            int nMusicResourceBase,
                            int cMusicResources,
                            int fMusicBackendEnabled,
                            int cEffectResources,
                            void *pPrimaryContext) {
    void **ppBackend;
    unsigned int i;
    int fMusicBackendReady;
    int nMusicBase;
    int cBackends;
    int fBackendReady;

    *(void ***)pAudioManager = g_AudioManagerVtable;
    nMusicBase = 0;
    if (fMusicBackendEnabled == 0) {
        nMusicBase = nMusicResourceBase;
    }
    *(int *)((char *)pAudioManager + 8) = 1;
    *(int *)((char *)pAudioManager + 0x14) = nMusicBase;
    *(int *)((char *)pAudioManager + 0x18) = cMusicResources;
    *(void **)((char *)pAudioManager + 0x70) = 0;
    *(int *)((char *)pAudioManager + 0x6c) = 1;
    ppBackend = (void **)((char *)pAudioManager + 0x24);
    fMusicBackendReady = 0;
    *(int *)((char *)pAudioManager + 0x1c) = fMusicBackendEnabled;
    *(void **)((char *)pAudioManager + 0x34) = 0;
    cBackends = SelectAudioManagerBackends(ppBackend,
                                           nMusicBase,
                                           cMusicResources,
                                           fMusicBackendEnabled,
                                           &fMusicBackendReady,
                                           (void **)((char *)pAudioManager + 0x34),
                                           cEffectResources);
    *(int *)((char *)pAudioManager + 0x20) = cBackends;
    *(void **)((char *)pAudioManager + 0x74) = 0;
    *(void **)((char *)pAudioManager + 0x78) = 0;
    for (i = 0; i < (unsigned int)cBackends; ++i) {
        if (ppBackend[i] != 0) {
            fBackendReady = ((AUDIO_TestProc)(*(void ***)ppBackend[i])[9])();
            if (fBackendReady == 1) {
                *(void **)((char *)pAudioManager + 0x74) = ppBackend[i];
            }
            fBackendReady = ((AUDIO_TestProc)(*(void ***)ppBackend[i])[10])();
            if (fBackendReady == 1) {
                *(void **)((char *)pAudioManager + 0x78) = ppBackend[i];
            }
        }
    }
    *(int *)((char *)pAudioManager + 0xc) = 0;
    *(int *)((char *)pAudioManager + 0x10) = 0;
    if (*(int *)((char *)pAudioManager + 0x1c) == 1) {
        if (fMusicBackendReady == 0) {
            *(int *)((char *)pAudioManager + 0xc) = 0;
            *(void **)((char *)pAudioManager + 0x74) = 0;
        } else {
            *(int *)((char *)pAudioManager + 0xc) = 1;
        }
    }
    if (*(int *)((char *)pAudioManager + 0x18) != 0) {
        SetAudioManagerEffectResourceContext(pAudioManager, pPrimaryContext);
    }
    if (*(int *)((char *)pAudioManager + 0x14) != 0) {
        SetAudioManagerMusicResourceContext(pAudioManager, pPrimaryContext);
    }
    RefreshAudioManagerBackendHandles(pAudioManager);

    if (*(int *)((char *)pAudioManager + 0xc) == 1) {
        *(unsigned char *)((char *)pAudioManager + 0xc7c) = 1;
        *(unsigned char *)((char *)pAudioManager + 0xc7e) = 1;
        if (*(void **)((char *)pAudioManager + 0x74) != 0) {
            *(unsigned char *)((char *)pAudioManager + 0xc7e) =
                ((AUDIO_TestProc)(*(void ***)*(void **)((char *)pAudioManager + 0x74))[11])();
        }
    } else {
        *(unsigned char *)((char *)pAudioManager + 0xc7c) = 0;
        *(unsigned char *)((char *)pAudioManager + 0xc7e) = 0;
    }

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        *(unsigned char *)((char *)pAudioManager + 0xc7d) = 1;
        *(unsigned char *)((char *)pAudioManager + 0xc7f) =
            ((AUDIO_TestProc)(*(void ***)*(void **)((char *)pAudioManager + 0x78))[12])();
    } else {
        *(unsigned char *)((char *)pAudioManager + 0xc7d) = 0;
        *(unsigned char *)((char *)pAudioManager + 0xc7f) = 0;
    }
    *(int *)((char *)pAudioManager + 0xc80) = *(int *)((char *)pAudioManager + 0xc);
    *(int *)((char *)pAudioManager + 0xc84) = *(int *)((char *)pAudioManager + 0x10);
    *(int *)((char *)pAudioManager + 0xc88) = 1;
    if (*(unsigned char *)((char *)pAudioManager + 0xc7f) < 2) {
        *(int *)((char *)pAudioManager + 0xc88) = 0;
    }
    *(int *)((char *)pAudioManager + 0xc8c) = *(int *)((char *)pAudioManager + 0xc);
    return pAudioManager;
}

// FUNCTION: LEMBALL 0x0045B110
void DestroyAudioManager(void *pAudioManager) {
    void **ppBackend;
    unsigned int i;
    void *pBackend;

    *(void ***)pAudioManager = g_AudioManagerVtable;
    if (*(int *)((char *)pAudioManager + 0xc) != 0 && *(void **)((char *)pAudioManager + 0x74) != 0) {
        pBackend = *(void **)((char *)pAudioManager + 0x74);
        ((AUDIO_VoidProc)(*(void ***)pBackend)[7])();
    }
    if (*(int *)((char *)pAudioManager + 0x10) != 0 && *(void **)((char *)pAudioManager + 0x78) != 0) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        ((AUDIO_VoidProc)(*(void ***)pBackend)[8])();
    }
    *(void **)((char *)pAudioManager + 0x74) = 0;
    *(void **)((char *)pAudioManager + 0x78) = 0;
    ppBackend = (void **)((char *)pAudioManager + 0x24);
    for (i = 0; i < *(unsigned int *)((char *)pAudioManager + 0x20); ++i) {
        if (ppBackend[i] != 0) {
            ((AUDIO_IntProc)(*(void ***)ppBackend[i])[22])(0);
            ((AUDIO_VoidProc)(*(void ***)ppBackend[i])[5])();
            ((AUDIO_DeleteProc)(*(void ***)ppBackend[i])[0])(ppBackend[i], 1);
        }
    }
    if (*(void **)((char *)pAudioManager + 0x34) != 0) {
        ((AUDIO_DeleteProc)(*(void ***)*(void **)((char *)pAudioManager + 0x34))[0])(
            *(void **)((char *)pAudioManager + 0x34), 1);
    }
}

// FUNCTION: LEMBALL 0x0045B190
void SetAudioManagerPrimaryContext(void *pAudioManager, void *pPrimaryContext) {
    int fAccepted;
    void *pBackend;

    *(void **)((char *)pAudioManager + 0x70) = pPrimaryContext;
    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (*(int *)((char *)pAudioManager + 8) == 0 && pBackend != 0) {
        fAccepted = ((AUDIO_OpenProc)(*(void ***)pBackend)[4])(0,
                                                                 *(int *)((char *)pAudioManager + 0x18),
                                                                 pPrimaryContext);
        if (fAccepted == 0) {
            *(int *)((char *)pAudioManager + 0x18) = 0;
        }
    }
}

// FUNCTION: LEMBALL 0x0045B1D0
void StopAllAudioManagerBackends(void *pAudioManager) {
    void **ppBackend;
    unsigned int i;

    if (*(int *)((char *)pAudioManager + 8) == 0) {
        *(int *)((char *)pAudioManager + 8) = 1;
        ppBackend = (void **)((char *)pAudioManager + 0x24);
        for (i = 0; i < *(unsigned int *)((char *)pAudioManager + 0x20); ++i) {
            if (ppBackend[i] != 0) {
                ((AUDIO_VoidProc)(*(void ***)ppBackend[i])[5])();
            }
        }
    }
}

// FUNCTION: LEMBALL 0x0045B210
void RefreshAudioManagerBackendHandles(void *pAudioManager) {
    void *pMusicBackend;
    int fAccepted;
    int nMusicResourceBase;
    int cEffectResources;

    if (*(int *)((char *)pAudioManager + 8) != 0) {
        nMusicResourceBase = *(int *)((char *)pAudioManager + 0x14);
        cEffectResources = 0;
        *(int *)((char *)pAudioManager + 8) = 0;
        pMusicBackend = *(void **)((char *)pAudioManager + 0x74);
        if (*(void **)((char *)pAudioManager + 0x78) == pMusicBackend) {
            *(void **)((char *)pAudioManager + 0x78) = 0;
            cEffectResources = *(int *)((char *)pAudioManager + 0x18);
        }
        if (*(int *)((char *)pAudioManager + 0x1c) == 0 && pMusicBackend != 0) {
            fAccepted = ((AUDIO_OpenProc)(*(void ***)pMusicBackend)[3])(nMusicResourceBase,
                                                                          cEffectResources,
                                                                          *(void **)((char *)pAudioManager + 0x70));
            if (fAccepted == 0) {
                nMusicResourceBase = 0;
                cEffectResources = 0;
            }
            *(int *)((char *)pAudioManager + 0xc) = nMusicResourceBase;
            *(int *)((char *)pAudioManager + 0x10) = cEffectResources;
        }
        if (*(void **)((char *)pAudioManager + 0x78) != 0) {
            fAccepted = ((AUDIO_OpenProc)(*(void ***)((char *)pAudioManager + 0x78))[3])(
                0, *(int *)((char *)pAudioManager + 0x18), *(void **)((char *)pAudioManager + 0x70));
            if (fAccepted == 0) {
                *(int *)((char *)pAudioManager + 0x18) = 0;
            }
            *(int *)((char *)pAudioManager + 0x10) = *(int *)((char *)pAudioManager + 0x18);
        } else if (*(int *)((char *)pAudioManager + 0x10) != 0) {
            *(void **)((char *)pAudioManager + 0x78) = *(void **)((char *)pAudioManager + 0x74);
        }
    }
}

// FUNCTION: LEMBALL 0x0045B2C0
void InvokeAudioManagerEmbeddedSlot04(void *pAudioManager, int nValue1, int nValue2) {
    void *pControl;

    pControl = *(void **)((char *)pAudioManager + 0x34);
    ((void (*)(int, int))(*(void ***)pControl)[1])(nValue1, nValue2);
}

// FUNCTION: LEMBALL 0x0045B2E0
int RegisterVariantResourceMusicHandle(void *pAudioManager, int nMusicResourceId) {
    int hMusic;
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        hMusic = *(int *)((char *)pAudioManager + 0x6c);
        *(int *)((char *)pAudioManager + 0x6c) = hMusic + 1;
        if (hMusic + 1 == 0) {
            *(int *)((char *)pAudioManager + 0x6c) = 1;
        }
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((void (*)(int, int))(*(void ***)pControl)[2])(hMusic, nMusicResourceId);
        return hMusic;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045B330
void StartRegisteredVariantResourceMusic(void *pAudioManager, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && hMusic != 0 &&
        *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((AUDIO_IntProc)(*(void ***)pControl)[4])(hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B370
void DispatchChildInterfaceSlot18IfReady(void *pAudioManager, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((AUDIO_IntProc)(*(void ***)pControl)[6])(hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B390
void InvokeAudioManagerEmbeddedSlot1cIfMusicActive(void *pAudioManager, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((AUDIO_IntProc)(*(void ***)pControl)[7])(hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B3B0
void StopVariantResourceMusicPlayback(void *pAudioManager, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((AUDIO_IntProc)(*(void ***)pControl)[5])(hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B3D0
void UnregisterVariantResourceMusicHandle(void *pAudioManager, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && hMusic != 0 &&
        *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((AUDIO_IntProc)(*(void ***)pControl)[3])(hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B3F0
int CreateVariantResourceEffectInstance(void *pAudioManager, int nEffectResourceId) {
    AUDIO_ResourceObject *pEffectResource;
    int nEffectInstanceId;
    int nEffectData;

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        pEffectResource = (AUDIO_ResourceObject *)LoadEffResource(nEffectResourceId);
        if (pEffectResource->m_nLoadState10 == 0) {
            ((AUDIO_VoidProc)pEffectResource->m_pVtable[7])();
        } else {
            pEffectResource->m_nReserved24 = 0;
        }

        ++pEffectResource->m_nLockCount08;
        nEffectData = ((AUDIO_ReturnIntProc)pEffectResource->m_pVtable[10])();
        ((AUDIO_CreateEffectProc)(*(void ***)*(void **)((char *)pAudioManager + 0x78))[15])(
            nEffectData, &nEffectInstanceId);
        --pEffectResource->m_nLockCount08;
        ReleaseTypedResourceObjectReference(pEffectResource);
        return nEffectInstanceId;
    }

    nEffectInstanceId = 0;
    return 0;
}

// FUNCTION: LEMBALL 0x0045B460
void PlayVariantResourceEffectId(void *pAudioManager, int nEffectResourceId) {
    void *pBackend;
    int nVolume;

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        nVolume = ((AUDIO_ReturnIntArgProc)(*(void ***)pBackend)[25])(0);
        ((AUDIO_EffectPlayProc)(*(void ***)pBackend)[35])(nEffectResourceId, nVolume);
    }
}

// FUNCTION: LEMBALL 0x0045B490
void SetVariantResourceEffectInstanceVolume(void *pAudioManager, int nEffectInstanceId, int nVolume) {
    void *pBackend;

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        ((AUDIO_EffectVolumeProc)(*(void ***)pBackend)[35])(nEffectInstanceId, nVolume, 0);
    }
}

// FUNCTION: LEMBALL 0x0045B4F0
void InvokeAudioManagerEffectSlot48IfActive(void *pAudioManager, int nEffect) {
    void *pBackend;

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        ((AUDIO_IntProc)(*(void ***)pBackend)[18])(nEffect);
    }
}

// FUNCTION: LEMBALL 0x0045B510
void SetAudioManagerMusicAndEffectVolume(void *pAudioManager, int nMusicVolume, int nEffectVolume) {
    void *pBackend;
    void *pControl;

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (nMusicVolume != -1 && pBackend != 0) {
        ((AUDIO_IntProc)(*(void ***)pBackend)[26])(nMusicVolume);
    }
    if (nEffectVolume != -1) {
        if (*(int *)((char *)pAudioManager + 0x1c) != 0) {
            pControl = *(void **)((char *)pAudioManager + 0x34);
            ((AUDIO_IntProc)(*(void ***)pControl)[8])(nEffectVolume);
        } else {
            pBackend = *(void **)((char *)pAudioManager + 0x74);
            if (pBackend != 0) {
                ((AUDIO_IntProc)(*(void ***)pBackend)[24])(nEffectVolume);
            }
        }
    }
}

// FUNCTION: LEMBALL 0x0045B560
unsigned int GetVariantResourceEffectMasterVolume(void *pAudioManager) {
    void *pBackend;

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (pBackend != 0) {
        return ((AUDIO_ReturnIntProc)(*(void ***)pBackend)[25])();
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045B580
void SetAudioManagerEffectResourceContext(void *pAudioManager, void *pPrimaryContext) {
    void *pBackend;

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    ((AUDIO_ContextProc)(*(void ***)pBackend)[38])(pPrimaryContext);
}

// FUNCTION: LEMBALL 0x0045B5A0
void SetAudioManagerMusicResourceContext(void *pAudioManager, void *pPrimaryContext) {
    void *pBackend;

    pBackend = *(void **)((char *)pAudioManager + 0x74);
    if (pBackend != 0) {
        ((AUDIO_ContextProc)(*(void ***)pBackend)[38])(pPrimaryContext);
    }
}

// FUNCTION: LEMBALL 0x0045B5C0
void SetAudioManagerStartupMusicName(void *pAudioManager, const char *pszMusicName) {
    AUDIO_BackendControl *pControl;

    pControl = *(AUDIO_BackendControl **)((char *)pAudioManager + 0x34);
    AssignDynamicCString(&pControl->m_StartupMusicName, pszMusicName);
    pControl->m_fStartupMusicNameSet = 1;
}

// FUNCTION: LEMBALL 0x0045B5F0
void SetAudioManagerMusicEnabledFlag(void *pAudioManager, int fEnabled) {
    AUDIO_BackendControl *pControl;

    pControl = *(AUDIO_BackendControl **)((char *)pAudioManager + 0x34);
    pControl->m_fMusicEnabled = fEnabled;
}

// FUNCTION: LEMBALL 0x0045B600
char *BuildAudioManagerDescriptionString(void *pAudioManager) {
    VSINIT_FixedBufferStream DescriptionBufferStream;
    VSINIT_FormattedOutputStream DescriptionStream;
    void *pBackend;

    DescriptionStream.m_pVtable = g_FormattedOutputStreamVtable;
    ConstructStreamFormatTargetState(&DescriptionStream.m_TargetState, &DescriptionBufferStream);
    DescriptionStream.m_TargetState.m_pVtable = g_StreamFormatSubobjectVtable;
    ConstructFixedBufferStream(&DescriptionBufferStream, g_szAudioManagerDescription, sizeof(g_szAudioManagerDescription),
                               0);
    DescriptionStream.m_TargetState.m_pDownstream = &DescriptionBufferStream;
    g_szAudioManagerDescription[0] = '\0';

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (pBackend != 0 && *(int *)((char *)pAudioManager + 0x18) != 0) {
        AppendCStringToStream(&DescriptionStream, "Effects : ");
        AppendCStringToStream(&DescriptionStream, ((AUDIO_NameProc)(*(void ***)pBackend)[1])());
    }

    if (*(int *)((char *)pAudioManager + 0x1c) != 0 && *(int *)((char *)pAudioManager + 0x14) != 0) {
        AppendCStringToStream(&DescriptionStream,
                              ((AUDIO_NameProc)(*(void ***)*(void **)((char *)pAudioManager + 0x34))[12])());
        AppendCStringToStream(&DescriptionStream, "\n");
    }

    pBackend = *(void **)((char *)pAudioManager + 0x74);
    if (pBackend != 0 && *(int *)((char *)pAudioManager + 0x14) != 0) {
        AppendCStringToStream(&DescriptionStream, "Music : ");
        AppendCStringToStream(&DescriptionStream, ((AUDIO_NameProc)(*(void ***)pBackend)[1])());
    }

    DestroyFixedBufferStream(&DescriptionBufferStream);
    RestoreStreamFormatSubobjectVtable(&DescriptionStream.m_TargetState);
    ConstructStreamFormatState(&DescriptionStream.m_TargetState);
    return g_szAudioManagerDescription;
}

// FUNCTION: LEMBALL 0x0045B770
int InitializeGlobalAudioManager(int nMusicResourceBase,
                                 int cMusicResources,
                                 int cEffectResources,
                                 void *pPrimaryContext) {
    void *pAudioManager;

    pAudioManager = AllocateVSMemBlock(0xc90);
    if (pAudioManager != 0) {
        g_pAudioManager =
            ConstructAudioManager(pAudioManager, nMusicResourceBase, cMusicResources, 1, cEffectResources,
                                  pPrimaryContext);
        return 1;
    }
    g_pAudioManager = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x0045B7C0
void ShutdownGlobalAudioManager(void) {
    void *pAudioManager;

    pAudioManager = g_pAudioManager;
    if (g_pAudioManager != 0) {
        DestroyAudioManager(g_pAudioManager);
        FreeVSMemBlock(pAudioManager);
    }
    g_pAudioManager = 0;
}

// FUNCTION: LEMBALL 0x0047C880
void *ConstructWaveOutEffectBackend(void *pBackend, int cEffectResources) {
    ConstructAudioEffectBackendBase(pBackend);
    *(void ***)pBackend = g_WaveOutEffectBackendVtable;
    *(int *)((char *)pBackend + 4) = cEffectResources;
    *(int *)((char *)pBackend + 8) = 0;
    *(int *)((char *)pBackend + 0xc) = 1;
    *(int *)((char *)pBackend + 0x10) = 0;
    *(int *)((char *)pBackend + 0x14) = 0;
    *(int *)((char *)pBackend + 0x18) = 0;
    *(int *)((char *)pBackend + 0x84) = 0x5622;
    *(int *)((char *)pBackend + 0x88) = -1;
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047CAF0
char *GetWaveOutEffectBackendDescription(AUDIO_WaveOutEffectBackend *pBackend) {
    if (pBackend->m_fDeviceAvailable == 1) {
        return pBackend->m_szDeviceDescription;
    }
    return (char *)g_AUDIO_NoEffectsBackendName;
}

// FUNCTION: LEMBALL 0x0047CBE0
int IsWaveOutEffectBackendAvailable(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x0047CF10
int CreateWaveOutEffectInstance(void *pBackend, void *pPatchResource, int *pnEffectInstanceId) {
    unsigned int i;
    int *pnSlotInUse;
    int nBackendVtable;
    void *pPatchBuffer;
    int nEffectInstanceId;
    int nNextEffectInstanceId;

    if (*(unsigned int *)((char *)pBackend + 4) != 0) {
        pnSlotInUse = *(int **)((char *)pBackend + 0xac);
        for (i = 0; i < *(unsigned int *)((char *)pBackend + 4); ++i) {
            if (*pnSlotInUse == 0) {
                nBackendVtable = *(int *)pBackend;
                ((AUDIO_VoidProc)*(void **)(nBackendVtable + 0x14))();
                pPatchBuffer = AllocateVSMemBlock(0x18);
                if (pPatchBuffer == 0) {
                    *(void **)(*(int *)((char *)pBackend + 0xa8) + i * 4) = 0;
                } else {
                    pPatchBuffer = ((void *(*)(void *, void *, int, int, int, int))0x0047BF00)(
                        pPatchBuffer, pPatchResource, *(int *)((char *)pBackend + 0x8c), *(int *)((char *)pBackend + 0x84),
                        *(int *)((char *)pBackend + 0x14), *(int *)((char *)pBackend + 0x10));
                    *(void **)(*(int *)((char *)pBackend + 0xa8) + i * 4) = pPatchBuffer;
                }
                ((void (*)(int, int, int))*(void **)(nBackendVtable + 0xc))(0, 1, 0);
                *(int *)(*(int *)((char *)pBackend + 0xac) + i * 4) = 1;
                *(int *)(*(int *)((char *)pBackend + 0xb0) + i * 4) = *(int *)((char *)pBackend + 0xa4);
                *pnEffectInstanceId = *(int *)((char *)pBackend + 0xa4);
                nEffectInstanceId = *(int *)((char *)pBackend + 0xa4);
                nNextEffectInstanceId = nEffectInstanceId + 1;
                *(int *)((char *)pBackend + 0xa4) = nNextEffectInstanceId;
                if (nNextEffectInstanceId == 0) {
                    *(int *)((char *)pBackend + 0xa4) = nEffectInstanceId + 2;
                }
                return 1;
            }
            ++pnSlotInUse;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047D010
int FreeWaveOutEffectInstance(void *pBackend, int nEffectInstanceId) {
    unsigned int i;
    int nOffset;
    void *pPatchBuffer;

    if (*(int *)((char *)pBackend + 4) != 0) {
        nOffset = 0;
        for (i = 0; i < *(unsigned int *)((char *)pBackend + 4); ++i) {
            if (*(int *)(*(int *)((char *)pBackend + 0xb0) + nOffset) == nEffectInstanceId) {
                pPatchBuffer = *(void **)(*(int *)((char *)pBackend + 0xa8) + nOffset);
                if (pPatchBuffer != 0) {
                    ((void (*)(void *))0x0047BCC0)(pPatchBuffer);
                    FreeVSMemBlock(pPatchBuffer);
                }
                *(int *)(*(int *)((char *)pBackend + 0xac) + nOffset) = 0;
                *(int *)(*(int *)((char *)pBackend + 0xb0) + nOffset) = 0;
            }
            nOffset += 4;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047D160
unsigned int PlayWaveOutEffectInstance(void *pBackend, int nEffectInstanceId) {
    unsigned int i;
    int nOffset;
    MMRESULT nWaveResult;

    if (*(int *)((char *)pBackend + 4) != 0) {
        nOffset = 0;
        for (i = 0; i < *(unsigned int *)((char *)pBackend + 4); ++i) {
            if (*(int *)(*(int *)((char *)pBackend + 0xb0) + nOffset) == nEffectInstanceId) {
                nWaveResult = waveOutReset(*(HWAVEOUT *)((char *)pBackend + 0x8c));
                if (nWaveResult != 0) {
                    AppendCStringToStream(g_pErrorOutputStream, "waveOutReset errored: ");
                    AppendUnsignedIntToStreamVariant(g_pErrorOutputStream, nWaveResult);
                    AppendCStringToStream(g_pErrorOutputStream, "\n");
                }
                nWaveResult = waveOutWrite(*(HWAVEOUT *)((char *)pBackend + 0x8c),
                                           *(LPWAVEHDR *)(*(int *)(*(int *)((char *)pBackend + 0xa8) + nOffset) + 0x10),
                                           0x20);
                if (nWaveResult != 0) {
                    AppendCStringToStream(g_pErrorOutputStream, "waveOutWrite (play effect) errored: ");
                    AppendUnsignedIntToStreamVariant(g_pErrorOutputStream, nWaveResult);
                    return (unsigned int)(unsigned char)'\n';
                }
            }
            nOffset += 4;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047CAA0
void DestroyWaveOutEffectBackend(void *pBackend) {
    *(void ***)pBackend = g_WaveOutEffectBackendVtable;
    FreeVSMemBlock(*(void **)((char *)pBackend + 0xa8));
    FreeVSMemBlock(*(void **)((char *)pBackend + 0xb0));
    FreeVSMemBlock(*(void **)((char *)pBackend + 0xac));
    RestoreAudioEffectBackendBaseVtable(pBackend);
}

// FUNCTION: LEMBALL 0x0047D270
void *DeleteWaveOutEffectBackend(AUDIO_WaveOutEffectBackend *pBackend, unsigned char fDelete) {
    DestroyWaveOutEffectBackend(pBackend);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pBackend);
    }
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047DD80
void *ConstructDirectSoundEffectBackend(void *pBackend, int cEffectResources, int nCooperativeLevel) {
    ConstructAudioEffectBackendBase(pBackend);
    *(void ***)pBackend = g_DirectSoundEffectBackendVtable;
    *(int *)((char *)pBackend + 0x14) = cEffectResources;
    *(int *)((char *)pBackend + 0x18) = nCooperativeLevel;
    *(int *)((char *)pBackend + 0x20) = 1;
    *(int *)((char *)pBackend + 0x28) = 1;
    *(int *)((char *)pBackend + 0x34) = 0x5622;
    *(int *)((char *)pBackend + 0x38) = 0;
    *(int *)((char *)pBackend + 0x40) = 0x5622;
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047E000
char *GetDirectSoundEffectBackendDescription(AUDIO_DirectSoundEffectBackend *pBackend) {
    if (pBackend->m_fDeviceAvailable == 1) {
        return (char *)"Direct Sound Device";
    }
    return (char *)g_AUDIO_NoDirectSoundBackendName;
}

// FUNCTION: LEMBALL 0x0047DFC0
void DestroyDirectSoundEffectBackend(void *pBackend) {
    *(void ***)pBackend = g_DirectSoundEffectBackendVtable;
    FreeVSMemBlock(*(void **)((char *)pBackend + 0x50));
    if (*(HMODULE *)((char *)pBackend + 4) != 0) {
        FreeLibrary(*(HMODULE *)((char *)pBackend + 4));
        *(HMODULE *)((char *)pBackend + 4) = 0;
    }
    RestoreAudioEffectBackendBaseVtable(pBackend);
}

// FUNCTION: LEMBALL 0x0047E8E0
void *DeleteDirectSoundEffectBackend(AUDIO_DirectSoundEffectBackend *pBackend, unsigned char fDelete) {
    DestroyDirectSoundEffectBackend(pBackend);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pBackend);
    }
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047E520
int CreateDirectSoundEffectInstance(void *pBackend, void *pPatchResource, int *pnEffectInstanceId, int nFlags) {
    int nEffectInstanceId;
    void *pBufferPool;

    nEffectInstanceId = 1;
    if (0 < *(int *)((char *)pBackend + 0x14)) {
        do {
            if (*(int *)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4) == 0) {
                pBufferPool = AllocateVSMemBlock(0x1c);
                if (pBufferPool == 0) {
                    *(void **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4) = 0;
                } else {
                    pBufferPool = ((void *(*)(void *, int, void *, int, int, int, int))0x0047D310)(
                        pBufferPool, *(int *)((char *)pBackend + 0x18), pPatchResource, *(int *)((char *)pBackend + 0x34),
                        *(int *)((char *)pBackend + 0x28), *(int *)((char *)pBackend + 0x24), nFlags);
                    *(void **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4) = pBufferPool;
                }
                if (**(int **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4) != 0) {
                    *pnEffectInstanceId = nEffectInstanceId;
                }
                return 1;
            }
            ++nEffectInstanceId;
        } while (nEffectInstanceId <= *(int *)((char *)pBackend + 0x14));
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047E5E0
int FreeDirectSoundEffectInstance(void *pBackend, int nEffectInstanceId) {
    unsigned int pBufferPool;

    pBufferPool = *(unsigned int *)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4);
    if (pBufferPool != 0) {
        ((void (*)(int))0x0047D700)(pBufferPool);
        FreeVSMemBlock((void *)(unsigned long)pBufferPool);
        *(int *)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4) = 0;
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047E620
int ReleaseAllDirectSoundEffectInstances(void *pBackend) {
    int nEffectInstanceId;
    int nOffset;
    unsigned int pBufferPool;

    nEffectInstanceId = 1;
    if (0 < *(int *)((char *)pBackend + 0x14)) {
        nOffset = 4;
        do {
            pBufferPool = *(unsigned int *)(*(int *)((char *)pBackend + 0x50) + nOffset);
            if (pBufferPool != 0) {
                ((void (*)(int))0x0047D700)(pBufferPool);
                FreeVSMemBlock((void *)(unsigned long)pBufferPool);
                *(int *)(*(int *)((char *)pBackend + 0x50) + nOffset) = 0;
            }
            nOffset += 4;
            ++nEffectInstanceId;
        } while (nEffectInstanceId <= *(int *)((char *)pBackend + 0x14));
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0047E7E0
void PlayDirectSoundEffectInstanceWithMappedVolume(void *pBackend, int nEffectInstanceId, unsigned char nVolume,
                                                   int fLoopEnabled) {
    ((void (*)(void *, int, int))0x0047C8A0)(
        *(void **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4),
        ((unsigned int)nVolume * 10000) / 0xff - 10000, fLoopEnabled);
}

// FUNCTION: LEMBALL 0x0047E820
void PauseDirectSoundEffectInstance(void *pBackend, int nEffectInstanceId, int nBufferSlot) {
    ((void (*)(void *, int))0x0047C940)(*(void **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4),
                                        nBufferSlot);
}

// FUNCTION: LEMBALL 0x0047E840
void PlayDirectSoundEffectInstance(void *pBackend, int nEffectInstanceId, int nBufferSlot) {
    ((void (*)(void *, int))0x0047C990)(*(void **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4),
                                        nBufferSlot);
}

// FUNCTION: LEMBALL 0x0047E940
void *ConstructMciMusicBackend(void *pBackend) {
    AUDIO_MciMusicBackend *pMusicBackend;
    AUDIO_MciOpenParms OpenParms;
    WNDCLASSA WindowClass;
    MCIERROR nMciError;
    char szMciErrorText[0x80];

    pMusicBackend = (AUDIO_MciMusicBackend *)pBackend;
    ConstructDynamicCString(&pMusicBackend->m_BasePath);
    pMusicBackend->m_pVtable = (void **)g_AudioDynamicStringEntryVtable;
    pMusicBackend->m_fSearchCdromPath = 0;
    pMusicBackend->m_fBasePathSet = 0;
    pMusicBackend->m_pVtable = g_MciMusicBackendVtable;
    pMusicBackend->m_nPreparedTrackHandle = 0;
    pMusicBackend->m_fDeviceAvailable = 0;
    pMusicBackend->m_uDeviceId = 0;
    pMusicBackend->m_fTrackPlaying = 0;
    pMusicBackend->m_fTrackPaused = 0;
    pMusicBackend->m_nPausedTrackPosition = 0;
    pMusicBackend->m_hNotificationWindow = 0;
    g_nPreparedMciMusicTrackHandle = 0;
    g_pActiveMciMusicBackend = pMusicBackend;

    OpenParms.dwCallback = 0;
    OpenParms.wDeviceID = 0;
    OpenParms.lpstrDeviceType = g_AUDIO_SequencerDeviceType;
    OpenParms.lpstrElementName = 0;
    OpenParms.lpstrAlias = 0;
    nMciError = mciSendCommandA(0, 0x803, 0x2000, (DWORD_PTR)&OpenParms);
    if (nMciError == 0) {
        pMusicBackend->m_uDeviceId = OpenParms.wDeviceID;
        pMusicBackend->m_fDeviceAvailable = 1;
        mciSendCommandA(OpenParms.wDeviceID, 0x804, 0, 0);

        memset(&WindowClass, 0, sizeof(WindowClass));
        WindowClass.style = 3;
        WindowClass.hInstance = g_hApplicationInstance;
        WindowClass.lpszClassName = g_AUDIO_MciWindowClassName;
        RegisterClassA(&WindowClass);
        pMusicBackend->m_hNotificationWindow =
            CreateWindowExA(0,
                            g_AUDIO_MciWindowClassName,
                            g_AUDIO_MciWindowClassName,
                            0,
                            (INT)CW_USEDEFAULT,
                            (INT)CW_USEDEFAULT,
                            (INT)CW_USEDEFAULT,
                            (INT)CW_USEDEFAULT,
                            0,
                            0,
                            g_hApplicationInstance,
                            0);
        if (pMusicBackend->m_hNotificationWindow == 0) {
            AppendCStringToStream(g_pErrorOutputStream, "Error: Unable to Create Window for HL Music\n");
        }
    } else {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_HlMidiNotFoundPrefix);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_MciErrorPrefix);
        AppendCStringToStream(g_pErrorOutputStream, szMciErrorText);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_ErrorSuffix);
        pMusicBackend->m_fDeviceAvailable = 0;
    }
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047EAD0
void PrepareMciMusicTrack(AUDIO_MciMusicBackend *pBackend, int nTrackHandle, int nMusicResourceId) {
    MOGLOAD_StringResourceObject *pStringResource;
    AUDIO_MciOpenParms OpenParms;
    AUDIO_MciGenericParms SeekParms;
    AUDIO_MciSetParms SetParms;
    MCIERROR nMciError;
    char szMciErrorText[0x80];
    char szMusicPath[0x200];
    const char *pszBasePath;
    const char *pszMusicName;

    pBackend->m_nPreparedTrackHandle = nTrackHandle;
    g_nPreparedMciMusicTrackHandle = nTrackHandle;

    pStringResource = LoadStringResource(nMusicResourceId);
    if (pStringResource == 0) {
        pBackend->m_nPreparedTrackHandle = 0;
        g_nPreparedMciMusicTrackHandle = 0;
        return;
    }

    pszBasePath = g_abProcessCurrentDirectoryBuffer;
    if (pBackend->m_fSearchCdromPath != 0) {
        pszBasePath = FindCdromFilePathBySuffix(pStringResource->m_pszText38);
        if (pszBasePath == 0) {
            pszBasePath = g_abProcessCurrentDirectoryBuffer;
        }
    }

    szMusicPath[0] = '\0';
    if (pBackend->m_fBasePathSet != 0) {
        strcpy(szMusicPath, pBackend->m_BasePath.m_pszText);
        if (szMusicPath[0] != '\0' && szMusicPath[strlen(szMusicPath) - 1] != '\\') {
            strcat(szMusicPath, g_AUDIO_PathSeparator);
        }
    }

    strcat(szMusicPath, pszBasePath);
    if (szMusicPath[0] != '\0' && szMusicPath[strlen(szMusicPath) - 1] != '\\') {
        strcat(szMusicPath, g_AUDIO_PathSeparator);
    }
    pszMusicName = pStringResource->m_pszText38;
    strcat(szMusicPath, pszMusicName);
    strcat(szMusicPath, g_AUDIO_MusicExtension);

    OpenParms.dwCallback = 0;
    OpenParms.wDeviceID = 0;
    OpenParms.lpstrDeviceType = (LPCSTR)0x20b;
    OpenParms.lpstrElementName = szMusicPath;
    OpenParms.lpstrAlias = 0;
    nMciError = mciSendCommandA(0, 0x803, 0x3200, (DWORD_PTR)&OpenParms);
    ReleaseTypedResourceObjectReference(pStringResource);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        AppendCStringToStream(g_pErrorOutputStream, "Error: Unable to Prepare Music: ");
        AppendCStringToStream(g_pErrorOutputStream, szMusicPath);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_ErrorSuffix);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_MciErrorPrefix);
        AppendCStringToStream(g_pErrorOutputStream, szMciErrorText);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_ErrorSuffix);
        pBackend->m_nPreparedTrackHandle = 0;
        g_nPreparedMciMusicTrackHandle = 0;
        return;
    }

    pBackend->m_uDeviceId = OpenParms.wDeviceID;
    SeekParms.dwCallback = 0;
    nMciError = mciSendCommandA(OpenParms.wDeviceID, 0x807, 0x100, (DWORD_PTR)&SeekParms);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        AppendCStringToStream(g_pErrorOutputStream, "Error: Unable to Prepare Music\n");
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_MciErrorPrefix);
        AppendCStringToStream(g_pErrorOutputStream, szMciErrorText);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_ErrorSuffix);
        pBackend->m_nPreparedTrackHandle = 0;
        g_nPreparedMciMusicTrackHandle = 0;
        return;
    }

    SetParms.dwCallback = 0;
    SetParms.dwTimeFormat = 0;
    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x80d, 0x400, (DWORD_PTR)&SetParms);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        AppendCStringToStream(g_pErrorOutputStream, "Error: Unable to Prepare Music\n");
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_MciErrorPrefix);
        AppendCStringToStream(g_pErrorOutputStream, szMciErrorText);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_ErrorSuffix);
        pBackend->m_nPreparedTrackHandle = 0;
        g_nPreparedMciMusicTrackHandle = 0;
        return;
    }

    pBackend->m_fTrackPlaying = 0;
    pBackend->m_fTrackPaused = 0;
}

// FUNCTION: LEMBALL 0x0047EE70
void FreePreparedMciMusicTrack(AUDIO_MciMusicBackend *pBackend, int nTrackHandle) {
    pBackend->m_nPreparedTrackHandle = 0;
    g_nPreparedMciMusicTrackHandle = 0;
    mciSendCommandA(pBackend->m_uDeviceId, 0x804, 0, 0);
    (void)nTrackHandle;
}

// FUNCTION: LEMBALL 0x0047EEE0
void PlayPreparedMciMusicTrack(AUDIO_MciMusicBackend *pBackend, int nTrackHandle) {
    AUDIO_MciGenericParms SeekParms;
    AUDIO_MciPlayParms PlayParms;
    MCIERROR nMciError;
    char szMciErrorText[0x80];

    SeekParms.dwCallback = 0;
    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x807, 0x100, (DWORD_PTR)&SeekParms);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        AppendCStringToStream(g_pErrorOutputStream, "Error: Unable to Play Music\n");
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_MciErrorPrefix);
        AppendCStringToStream(g_pErrorOutputStream, szMciErrorText);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_ErrorSuffix);
        (void)nTrackHandle;
        return;
    }

    memset(&PlayParms, 0, sizeof(PlayParms));
    PlayParms.dwCallback = (DWORD)(DWORD_PTR)pBackend->m_hNotificationWindow;
    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x806, 1, (DWORD_PTR)&PlayParms);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        AppendCStringToStream(g_pErrorOutputStream, "Error: Unable to Play Music\n");
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_MciErrorPrefix);
        AppendCStringToStream(g_pErrorOutputStream, szMciErrorText);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_ErrorSuffix);
        (void)nTrackHandle;
        return;
    }

    pBackend->m_fTrackPlaying = 1;
    pBackend->m_fTrackPaused = 0;
    pBackend->m_nPausedTrackPosition = 0;
    (void)nTrackHandle;
}

// FUNCTION: LEMBALL 0x0047F040
void StopMciMusicTrack(AUDIO_MciMusicBackend *pBackend, int nTrackHandle) {
    MCIERROR nMciError;
    char szMciErrorText[0x80];

    if (pBackend->m_fTrackPlaying == 0) {
        (void)nTrackHandle;
        return;
    }

    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x808, 0, 0);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        AppendCStringToStream(g_pErrorOutputStream, "Error: Unable to Stop Music\n");
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_MciErrorPrefix);
        AppendCStringToStream(g_pErrorOutputStream, szMciErrorText);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_ErrorSuffix);
        (void)nTrackHandle;
        return;
    }

    pBackend->m_fTrackPlaying = 0;
    (void)nTrackHandle;
}

// FUNCTION: LEMBALL 0x0047F120
void PauseMciMusicTrack(AUDIO_MciMusicBackend *pBackend, int nTrackHandle) {
    AUDIO_MciStatusParms StatusParms;
    MCIERROR nMciError;
    char szMciErrorText[0x80];

    memset(&StatusParms, 0, sizeof(StatusParms));
    StatusParms.dwItem = 2;
    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x814, 0x100, (DWORD_PTR)&StatusParms);
    if (nMciError == 0) {
        pBackend->m_nPausedTrackPosition = (int)StatusParms.dwReturn;
    }

    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x808, 0, 0);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        AppendCStringToStream(g_pErrorOutputStream, "Error: Unable to Stop Music\n");
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_MciErrorPrefix);
        AppendCStringToStream(g_pErrorOutputStream, szMciErrorText);
        AppendCStringToStream(g_pErrorOutputStream, g_AUDIO_ErrorSuffix);
        (void)nTrackHandle;
        return;
    }

    pBackend->m_fTrackPlaying = 0;
    pBackend->m_fTrackPaused = 1;
    (void)nTrackHandle;
}

// FUNCTION: LEMBALL 0x0047F940
void *ConstructAudioEffectBackendBase(void *pBackend) {
    *(void ***)pBackend = g_WaveOutEffectBackendVtable;
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047F950
void RestoreAudioEffectBackendBaseVtable(void *pBackend) {
    *(void ***)pBackend = g_WaveOutEffectBackendVtable;
}

// FUNCTION: LEMBALL 0x00473390
int SelectAudioManagerBackends(void **ppEffectBackends,
                               int nMusicResourceBase,
                               int cMusicResources,
                               int fMusicBackendEnabled,
                               int *pfMusicBackendReady,
                               void **ppMusicBackend,
                               int cEffectResources) {
    void *pBackendStorage;
    void *pBackend;
    void **pBackendVtable;
    int fAvailable;

    (void)nMusicResourceBase;
    *pfMusicBackendReady = 0;
    *ppMusicBackend = 0;
    if (fMusicBackendEnabled != 1) {
        if (cMusicResources == 1) {
            pBackend = 0;
            pBackendStorage = AllocateVSMemBlock(0xb8);
            if (pBackendStorage != 0) {
                pBackend = ConstructWaveOutEffectBackend(pBackendStorage, cEffectResources);
            }
            if (pBackend != 0) {
                pBackendVtable = *(void ***)pBackend;
                fAvailable = ((AUDIO_TestProc)pBackendVtable[10])();
                if (fAvailable == 1) {
                    *ppEffectBackends = pBackend;
                    return 1;
                }
                ((AUDIO_DeleteProc)pBackendVtable[0])(pBackend, 1);
            }
        }
        return 0;
    }

    pBackend = 0;
    pBackendStorage = AllocateVSMemBlock(0x34);
    if (pBackendStorage != 0) {
        pBackend = ConstructMciMusicBackend(pBackendStorage);
    }
    if (pBackend != 0) {
        pBackendVtable = *(void ***)pBackend;
        fAvailable = ((AUDIO_TestProc)pBackendVtable[11])();
        if (fAvailable == 1) {
            *pfMusicBackendReady = 1;
            *ppMusicBackend = pBackend;
        } else {
            ((AUDIO_DeleteProc)pBackendVtable[0])(pBackend, 1);
        }
    }

    if (cMusicResources != 1) {
        return 0;
    }

    pBackend = 0;
    pBackendStorage = AllocateVSMemBlock(0x54);
    if (pBackendStorage != 0) {
        pBackend = ConstructDirectSoundEffectBackend(pBackendStorage, cEffectResources, 5);
    }
    if (pBackend != 0) {
        pBackendVtable = *(void ***)pBackend;
        fAvailable = ((AUDIO_TestProc)pBackendVtable[10])();
        if (fAvailable == 1) {
            *ppEffectBackends = pBackend;
            return 1;
        }
        ((AUDIO_DeleteProc)pBackendVtable[0])(pBackend, 1);
    }

    pBackend = 0;
    pBackendStorage = AllocateVSMemBlock(0xb8);
    if (pBackendStorage != 0) {
        pBackend = ConstructWaveOutEffectBackend(pBackendStorage, cEffectResources);
    }
    if (pBackend != 0) {
        pBackendVtable = *(void ***)pBackend;
        fAvailable = ((AUDIO_TestProc)pBackendVtable[10])();
        if (fAvailable == 1) {
            *ppEffectBackends = pBackend;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047EAC0
void DestroyAudioDynamicStringEntry(AUDIO_DynamicStringEntry *pEntry) {
    pEntry->m_pVtable = (void **)g_AudioDynamicStringEntryVtable;
    DestroyDynamicCString(&pEntry->m_Text);
}

// FUNCTION: LEMBALL 0x0047F590
void *DeleteAudioDynamicStringEntry(AUDIO_DynamicStringEntry *pEntry, unsigned char fDelete) {
    DestroyAudioDynamicStringEntry(pEntry);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pEntry);
    }
    return pEntry;
}

const char *ReturnEmptyAudioBackendName(void) {
    return "";
}

int ReturnAudioBackendReady(void) {
    return 1;
}

int ReturnAudioBackendUnavailable(void) {
    return 0;
}

int AcceptAudioBackendResources(int nResourceBase, int cResources, void *pPrimaryContext) {
    (void)nResourceBase;
    (void)cResources;
    (void)pPrimaryContext;
    return 1;
}

int CreateFallbackEffectInstance(int nValue, void *pOut) {
    if (pOut != 0) {
        *(int *)pOut = nValue;
    }
    return nValue;
}

void NoOpAudioInt(int nValue) {
    (void)nValue;
}

void NoOpAudioPair(int nValue1, int nValue2) {
    (void)nValue1;
    (void)nValue2;
}

void NoOpAudioTriple(int nValue1, int nValue2, int nValue3) {
    (void)nValue1;
    (void)nValue2;
    (void)nValue3;
}
