#include "vsaudio.h"

#include "../game.h"
#include "vsmem.h"
#include "vsinit.h"

#include <string.h>

typedef int (*VSAUDIO_TestProc)(void);
typedef int (*VSAUDIO_OpenProc)(int nResourceBase, int cResources, void *pPrimaryContext);
typedef void (*VSAUDIO_VoidProc)(void);
typedef void (*VSAUDIO_IntProc)(int nValue);
typedef int (*VSAUDIO_CreateEffectProc)(int nValue, void *pOut);
typedef void (*VSAUDIO_EffectPlayProc)(int nEffectResourceId, int nVolume);
typedef void (*VSAUDIO_EffectVolumeProc)(int nEffectInstanceId, int nVolume, int nReserved);
typedef int (*VSAUDIO_ReturnIntProc)(void);
typedef const char *(*VSAUDIO_NameProc)(void);
typedef void (*VSAUDIO_ContextProc)(void *pPrimaryContext);
typedef void *(*VSAUDIO_DeleteProc)(void *pObject, int fDelete);

struct VSAUDIO_BackendControl {
    void **m_pVtable;
    int m_nNextHandle;
    int m_fStartupMusicNameSet;
    int m_fMusicEnabled;
    GAME_DynamicCString m_StartupMusicName;
};

void *ConstructAudioFallbackControl(VSAUDIO_BackendControl *pControl);
void DestroyAudioFallbackControl(VSAUDIO_BackendControl *pControl);
void *DeleteAudioFallbackControl(VSAUDIO_BackendControl *pControl, int fDelete);
const char *ReturnEmptyAudioBackendName(void);
int ReturnAudioBackendReady(void);
int ReturnAudioBackendUnavailable(void);
int AcceptAudioBackendResources(int nResourceBase, int cResources, void *pPrimaryContext);
int CreateFallbackEffectInstance(int nValue, void *pOut);
void NoOpAudioInt(int nValue);
void NoOpAudioPair(int nValue1, int nValue2);
void NoOpAudioTriple(int nValue1, int nValue2, int nValue3);
int RegisterFallbackMusicHandle(VSAUDIO_BackendControl *pControl, int nMusicResourceId);

void *g_pAudioManager = 0;

static void *g_AudioManagerVtable[1] = { 0 };
static void *g_MciMusicBackendVtable[13] = {
    (void *)DeleteAudioFallbackControl,
    (void *)ReturnEmptyAudioBackendName,
    (void *)RegisterFallbackMusicHandle,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioPair,
    (void *)NoOpAudioInt,
    (void *)ReturnAudioBackendReady,
    (void *)ReturnEmptyAudioBackendName,
};
static void *g_AudioEffectBackendVtable[39] = {
    (void *)DeleteAudioFallbackControl,
    (void *)ReturnEmptyAudioBackendName,
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
    (void *)CreateFallbackEffectInstance,
    0,
    0,
    (void *)NoOpAudioInt,
    (void *)ReturnAudioBackendReady,
    (void *)NoOpAudioTriple,
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
static char g_szAudioManagerDescription[0x400];

void ConstructAudioDynamicString(GAME_DynamicCString *pString) {
    pString->m_pszText = 0;
    pString->m_cchCapacity = 0;
}

void DestroyAudioDynamicString(GAME_DynamicCString *pString) {
    if (pString->m_pszText != 0) {
        FreeVSMemBlock(pString->m_pszText);
        pString->m_pszText = 0;
    }
    pString->m_cchCapacity = 0;
}

GAME_DynamicCString *AssignAudioDynamicString(GAME_DynamicCString *pString, const char *pszText) {
    unsigned int cchText;
    char *pszBuffer;

    cchText = (unsigned int)strlen(pszText) + 1;
    if (pString->m_cchCapacity < (int)cchText) {
        if (pString->m_pszText != 0) {
            FreeVSMemBlock(pString->m_pszText);
        }
        pszBuffer = (char *)AllocateVSMemBlock(cchText);
        pString->m_pszText = pszBuffer;
        pString->m_cchCapacity = (int)cchText;
    }
    if (pString->m_pszText != 0) {
        memcpy(pString->m_pszText, pszText, cchText);
    }
    return pString;
}

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
            fBackendReady = ((VSAUDIO_TestProc)(*(void ***)ppBackend[i])[9])();
            if (fBackendReady == 1) {
                *(void **)((char *)pAudioManager + 0x74) = ppBackend[i];
            }
            fBackendReady = ((VSAUDIO_TestProc)(*(void ***)ppBackend[i])[10])();
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
    if (*(int *)((char *)pAudioManager + 0x18) != 0 && *(void **)((char *)pAudioManager + 0x78) != 0) {
        SetAudioManagerEffectResourceContext(pAudioManager, pPrimaryContext);
    }
    if (*(int *)((char *)pAudioManager + 0x14) != 0) {
        SetAudioManagerMusicResourceContext(pAudioManager, pPrimaryContext);
    }
    RefreshAudioManagerBackendHandles(pAudioManager);
    if (*(void **)((char *)pAudioManager + 0x34) == 0) {
        *(void **)((char *)pAudioManager + 0x34) =
            ConstructAudioFallbackControl((VSAUDIO_BackendControl *)((char *)pAudioManager + 0x7c));
    }
    *(unsigned char *)((char *)pAudioManager + 0xc7c) = *(int *)((char *)pAudioManager + 0xc) == 1;
    *(unsigned char *)((char *)pAudioManager + 0xc7e) = 0;
    *(unsigned char *)((char *)pAudioManager + 0xc7d) = *(int *)((char *)pAudioManager + 0x10) == 1;
    *(unsigned char *)((char *)pAudioManager + 0xc7f) = 0;
    *(int *)((char *)pAudioManager + 0xc80) = *(int *)((char *)pAudioManager + 0xc);
    *(int *)((char *)pAudioManager + 0xc84) = *(int *)((char *)pAudioManager + 0x10);
    *(int *)((char *)pAudioManager + 0xc88) = 0;
    *(int *)((char *)pAudioManager + 0xc8c) = *(int *)((char *)pAudioManager + 0xc);
    return pAudioManager;
}

// FUNCTION: LEMBALL 0x0045B110
void DestroyAudioManager(void *pAudioManager) {
    void **ppBackend;
    unsigned int i;
    void *pBackend;
    VSAUDIO_BackendControl *pControl;

    *(void ***)pAudioManager = g_AudioManagerVtable;
    if (*(int *)((char *)pAudioManager + 0xc) != 0 && *(void **)((char *)pAudioManager + 0x74) != 0) {
        pBackend = *(void **)((char *)pAudioManager + 0x74);
        ((VSAUDIO_VoidProc)(*(void ***)pBackend)[7])();
    }
    if (*(int *)((char *)pAudioManager + 0x10) != 0 && *(void **)((char *)pAudioManager + 0x78) != 0) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        ((VSAUDIO_VoidProc)(*(void ***)pBackend)[8])();
    }
    *(void **)((char *)pAudioManager + 0x74) = 0;
    *(void **)((char *)pAudioManager + 0x78) = 0;
    ppBackend = (void **)((char *)pAudioManager + 0x24);
    for (i = 0; i < *(unsigned int *)((char *)pAudioManager + 0x20); ++i) {
        if (ppBackend[i] != 0) {
            ((VSAUDIO_IntProc)(*(void ***)ppBackend[i])[22])(0);
            ((VSAUDIO_VoidProc)(*(void ***)ppBackend[i])[5])();
            ((VSAUDIO_DeleteProc)(*(void ***)ppBackend[i])[0])(ppBackend[i], 1);
        }
    }
    pControl = *(VSAUDIO_BackendControl **)((char *)pAudioManager + 0x34);
    if (pControl != 0) {
        if ((char *)pControl >= (char *)pAudioManager && (char *)pControl < (char *)pAudioManager + 0xc90) {
            DestroyAudioFallbackControl(pControl);
        } else {
            ((VSAUDIO_DeleteProc)pControl->m_pVtable[0])(pControl, 1);
        }
    }
}

// FUNCTION: LEMBALL 0x0045B190
void SetAudioManagerPrimaryContext(void *pAudioManager, void *pPrimaryContext) {
    int fAccepted;
    void *pBackend;

    *(void **)((char *)pAudioManager + 0x70) = pPrimaryContext;
    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (*(int *)((char *)pAudioManager + 8) == 0 && pBackend != 0) {
        fAccepted = ((VSAUDIO_OpenProc)(*(void ***)pBackend)[4])(0,
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
                ((VSAUDIO_VoidProc)(*(void ***)ppBackend[i])[5])();
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
            fAccepted = ((VSAUDIO_OpenProc)(*(void ***)pMusicBackend)[3])(nMusicResourceBase,
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
            fAccepted = ((VSAUDIO_OpenProc)(*(void ***)((char *)pAudioManager + 0x78))[3])(
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
        ((VSAUDIO_IntProc)(*(void ***)pControl)[4])(hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B370
void DispatchChildInterfaceSlot18IfReady(void *pAudioManager, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((VSAUDIO_IntProc)(*(void ***)pControl)[6])(hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B390
void InvokeAudioManagerEmbeddedSlot1cIfMusicActive(void *pAudioManager, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((VSAUDIO_IntProc)(*(void ***)pControl)[7])(hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B3B0
void StopVariantResourceMusicPlayback(void *pAudioManager, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((VSAUDIO_IntProc)(*(void ***)pControl)[5])(hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B3D0
void UnregisterVariantResourceMusicHandle(void *pAudioManager, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && hMusic != 0 &&
        *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((VSAUDIO_IntProc)(*(void ***)pControl)[3])(hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B3F0
int CreateVariantResourceEffectInstance(void *pAudioManager, int nEffectResourceId) {
    void *pBackend;
    int nEffectInstanceId;

    nEffectInstanceId = 0;
    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        ((VSAUDIO_CreateEffectProc)(*(void ***)pBackend)[15])(nEffectResourceId, &nEffectInstanceId);
        return nEffectInstanceId;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045B460
void PlayVariantResourceEffectId(void *pAudioManager, int nEffectResourceId) {
    void *pBackend;
    int nVolume;

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        nVolume = ((VSAUDIO_ReturnIntProc)(*(void ***)pBackend)[25])();
        ((VSAUDIO_EffectPlayProc)(*(void ***)pBackend)[35])(nEffectResourceId, nVolume);
    }
}

// FUNCTION: LEMBALL 0x0045B490
void SetVariantResourceEffectInstanceVolume(void *pAudioManager, int nEffectInstanceId, int nVolume) {
    void *pBackend;

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        ((VSAUDIO_EffectVolumeProc)(*(void ***)pBackend)[35])(nEffectInstanceId, nVolume, 0);
    }
}

// FUNCTION: LEMBALL 0x0045B4F0
void InvokeAudioManagerEffectSlot48IfActive(void *pAudioManager, int nEffect) {
    void *pBackend;

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        ((VSAUDIO_IntProc)(*(void ***)pBackend)[18])(nEffect);
    }
}

// FUNCTION: LEMBALL 0x0045B510
void SetAudioManagerMusicAndEffectVolume(void *pAudioManager, int nMusicVolume, int nEffectVolume) {
    void *pBackend;
    void *pControl;

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (nMusicVolume != -1 && pBackend != 0) {
        ((VSAUDIO_IntProc)(*(void ***)pBackend)[26])(nMusicVolume);
    }
    if (nEffectVolume != -1) {
        if (*(int *)((char *)pAudioManager + 0x1c) != 0) {
            pControl = *(void **)((char *)pAudioManager + 0x34);
            ((VSAUDIO_IntProc)(*(void ***)pControl)[8])(nEffectVolume);
        } else {
            pBackend = *(void **)((char *)pAudioManager + 0x74);
            if (pBackend != 0) {
                ((VSAUDIO_IntProc)(*(void ***)pBackend)[24])(nEffectVolume);
            }
        }
    }
}

// FUNCTION: LEMBALL 0x0045B560
unsigned int GetVariantResourceEffectMasterVolume(void *pAudioManager) {
    void *pBackend;

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (pBackend != 0) {
        return ((VSAUDIO_ReturnIntProc)(*(void ***)pBackend)[25])();
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045B580
void SetAudioManagerEffectResourceContext(void *pAudioManager, void *pPrimaryContext) {
    void *pBackend;

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    ((VSAUDIO_ContextProc)(*(void ***)pBackend)[38])(pPrimaryContext);
}

// FUNCTION: LEMBALL 0x0045B5A0
void SetAudioManagerMusicResourceContext(void *pAudioManager, void *pPrimaryContext) {
    void *pBackend;

    pBackend = *(void **)((char *)pAudioManager + 0x74);
    if (pBackend != 0) {
        ((VSAUDIO_ContextProc)(*(void ***)pBackend)[38])(pPrimaryContext);
    }
}

// FUNCTION: LEMBALL 0x0045B5C0
void SetAudioManagerStartupMusicName(void *pAudioManager, const char *pszMusicName) {
    VSAUDIO_BackendControl *pControl;

    pControl = *(VSAUDIO_BackendControl **)((char *)pAudioManager + 0x34);
    AssignAudioDynamicString(&pControl->m_StartupMusicName, pszMusicName);
    pControl->m_fStartupMusicNameSet = 1;
}

// FUNCTION: LEMBALL 0x0045B5F0
void SetAudioManagerMusicEnabledFlag(void *pAudioManager, int fEnabled) {
    VSAUDIO_BackendControl *pControl;

    pControl = *(VSAUDIO_BackendControl **)((char *)pAudioManager + 0x34);
    pControl->m_fMusicEnabled = fEnabled;
}

// FUNCTION: LEMBALL 0x0045B600
char *BuildAudioManagerDescriptionString(void *pAudioManager) {
    g_szAudioManagerDescription[0] = '\0';
    if (*(void **)((char *)pAudioManager + 0x78) != 0 && *(int *)((char *)pAudioManager + 0x18) != 0) {
        strcat(g_szAudioManagerDescription, "Effects : ");
    }
    if (*(int *)((char *)pAudioManager + 0x1c) != 0 && *(int *)((char *)pAudioManager + 0x14) != 0) {
        strcat(g_szAudioManagerDescription, "\n");
    }
    if (*(void **)((char *)pAudioManager + 0x74) != 0 && *(int *)((char *)pAudioManager + 0x14) != 0) {
        strcat(g_szAudioManagerDescription, "Music : ");
    }
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
    *(void ***)pBackend = g_AudioEffectBackendVtable;
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

// FUNCTION: LEMBALL 0x0047DD80
void *ConstructDirectSoundEffectBackend(void *pBackend, int cEffectResources, int nCooperativeLevel) {
    ConstructAudioEffectBackendBase(pBackend);
    *(void ***)pBackend = g_AudioEffectBackendVtable;
    *(int *)((char *)pBackend + 0x14) = cEffectResources;
    *(int *)((char *)pBackend + 0x18) = nCooperativeLevel;
    *(int *)((char *)pBackend + 0x20) = 1;
    *(int *)((char *)pBackend + 0x28) = 1;
    *(int *)((char *)pBackend + 0x34) = 0x5622;
    *(int *)((char *)pBackend + 0x38) = 0;
    *(int *)((char *)pBackend + 0x40) = 0x5622;
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047E940
void *ConstructMciMusicBackend(void *pBackend) {
    VSAUDIO_BackendControl *pControl;

    pControl = (VSAUDIO_BackendControl *)pBackend;
    pControl->m_pVtable = g_MciMusicBackendVtable;
    pControl->m_nNextHandle = 1;
    pControl->m_fStartupMusicNameSet = 0;
    pControl->m_fMusicEnabled = 0;
    ConstructAudioDynamicString(&pControl->m_StartupMusicName);
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047F940
void *ConstructAudioEffectBackendBase(void *pBackend) {
    *(void ***)pBackend = g_AudioEffectBackendVtable;
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047F950
void RestoreAudioEffectBackendBaseVtable(void *pBackend) {
    *(void ***)pBackend = g_AudioEffectBackendVtable;
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
                fAvailable = ((VSAUDIO_TestProc)pBackendVtable[10])();
                if (fAvailable == 1) {
                    *ppEffectBackends = pBackend;
                    return 1;
                }
                ((VSAUDIO_DeleteProc)pBackendVtable[0])(pBackend, 1);
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
        fAvailable = ((VSAUDIO_TestProc)pBackendVtable[11])();
        if (fAvailable == 1) {
            *pfMusicBackendReady = 1;
            *ppMusicBackend = pBackend;
        } else {
            ((VSAUDIO_DeleteProc)pBackendVtable[0])(pBackend, 1);
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
        fAvailable = ((VSAUDIO_TestProc)pBackendVtable[10])();
        if (fAvailable == 1) {
            *ppEffectBackends = pBackend;
            return 1;
        }
        ((VSAUDIO_DeleteProc)pBackendVtable[0])(pBackend, 1);
    }

    pBackend = 0;
    pBackendStorage = AllocateVSMemBlock(0xb8);
    if (pBackendStorage != 0) {
        pBackend = ConstructWaveOutEffectBackend(pBackendStorage, cEffectResources);
    }
    if (pBackend != 0) {
        pBackendVtable = *(void ***)pBackend;
        fAvailable = ((VSAUDIO_TestProc)pBackendVtable[10])();
        if (fAvailable == 1) {
            *ppEffectBackends = pBackend;
            return 1;
        }
    }
    return 0;
}

void *ConstructAudioFallbackControl(VSAUDIO_BackendControl *pControl) {
    pControl->m_pVtable = g_MciMusicBackendVtable;
    pControl->m_nNextHandle = 1;
    pControl->m_fStartupMusicNameSet = 0;
    pControl->m_fMusicEnabled = 0;
    ConstructAudioDynamicString(&pControl->m_StartupMusicName);
    return pControl;
}

void DestroyAudioFallbackControl(VSAUDIO_BackendControl *pControl) {
    pControl->m_pVtable = g_MciMusicBackendVtable;
    DestroyAudioDynamicString(&pControl->m_StartupMusicName);
}

void *DeleteAudioFallbackControl(VSAUDIO_BackendControl *pControl, int fDelete) {
    DestroyAudioFallbackControl(pControl);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pControl);
    }
    return pControl;
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

int RegisterFallbackMusicHandle(VSAUDIO_BackendControl *pControl, int nMusicResourceId) {
    int hMusic;

    (void)nMusicResourceId;
    hMusic = pControl->m_nNextHandle;
    ++pControl->m_nNextHandle;
    return hMusic;
}
