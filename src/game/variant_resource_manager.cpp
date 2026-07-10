#include "../game/game_app.h"
#include "../platform/startup_options.h"
#include "../audio/audio_manager.h"
#include "../engine/memory_arena.h"
#include "../network/safe_vtable.h"

extern "C" DWORD timeGetTime(void);

struct GAME_MainGameVariantResourceBundle {
    void **m_pVtable;
    void *m_pStatusIndicatorManager;
    int m_nLoadedResourceCount;
    int m_nTotalResourceCount;
    void **m_ppZrleListResources;
    void *m_pReserved14;
    int m_cZrleListResources;
    void **m_ppListResources;
    void *m_pReserved20;
    int m_cListResources;
    void **m_ppBitmapResources;
    void *m_pReserved2C;
    int m_cBitmapResources;
    void **m_ppPaletteResources;
    void *m_pReserved38;
    void **m_ppStringResources;
    void *m_pReserved40;
    void *m_pTwoArrayListResources;
    void *m_pReserved48;
    int *m_panZrleListResourceIds;
    int *m_panListResourceIds;
    int *m_panBitmapResourceIds;
};

static void *g_GAME_MainGameVariantResourceBundleVtable = NetworkGetSafeVtable();
static int *g_GAME_MainGameVariantCompactZrleListResourceIds;
static int *g_GAME_MainGameVariantStandardZrleListResourceIds;
static int *g_GAME_MainGameVariantCompactListResourceIds;
static int *g_GAME_MainGameVariantStandardListResourceIds;
static int *g_GAME_MainGameVariantCompactBitmapResourceIds;
static int *g_GAME_MainGameVariantStandardBitmapResourceIds;
static int *g_GAME_MainGameVariantPaletteResourceIds;
static int *g_GAME_MainGameVariantStringResourceIds;
static int *g_GAME_MainGameVariantStringResourceIdsEnd;
static int *g_GAME_VariantResourceEntryEffectTable;
int g_fVariantResourceEffectsEnabled = 0;
int g_fVariantResourceMusicEnabled = 0;

extern void *g_pVariantResourceEntryManager;
extern void *g_pSessionRandomState;
extern int g_nLevelFrameClockTick;
extern int g_fLevelDemoModeEnabled;
extern void *g_pMainResourceArchive;
extern void *ConstructLevelScreenStatusIndicatorManager(void *pObject, int nStatusMode, void *pPrimaryContext);
extern void ReleaseLevelSelectionModeRenderStateResource(void *pStatusIndicatorManager);
extern void DestroyLevelScreenStatusIndicatorManager(void *pStatusIndicatorManager);
extern int CountVariantResourceEntriesWithFlagMask(void *pManager, unsigned short nMask);
extern void SwitchVariantResourceEntryMode(void *pManager, unsigned short nMask, void *pBundle);
extern void LoadMainGameVariantZrleListResource(void *pBundle, int nResourceId);
extern void LoadMainGameVariantListResource(void *pBundle, int nResourceId);
extern void LoadMainGameVariantBitmapResource(void *pBundle, int nResourceId);
extern void LoadMainGameVariantPaletteResource(void *pBundle, int nResourceId);
extern void LoadMainGameVariantStringResource(void *pBundle, int nResourceId);
extern void LoadMainGameVariantTwoArrayListResource(void *pBundle, int nResourceId);
extern void PopulateVariantResourceEntriesForFlagMask(void *pManager, unsigned short nMask);
extern void PruneUnreferencedCachedResourceObjects(void *pArchive);

// FUNCTION: LEMBALL 0x00438B50
void SetVariantResourceEffectsEnabled(int fEnabled) {
    if (g_fEffectsOptionAvailable != 0) {
        g_fVariantResourceEffectsEnabled = fEnabled;
    }
}

// FUNCTION: LEMBALL 0x00438BA0
void SetVariantResourceMusicEnabled(void *pManager, int fEnabled) {
    int hMusic;

    if (fEnabled != 0) {
        if (g_fVariantResourceMusicEnabled != 0) {
            return;
        }
    } else if (g_fVariantResourceMusicEnabled == 0) {
        return;
    }

    if (g_fMusicEnabled != 0 && *(int *)((char *)pManager + 0x2c0) != 0) {
        if (fEnabled != 0) {
            hMusic = RegisterVariantResourceMusicHandle(g_pAudioManager, *(int *)((char *)pManager + 0x2c0));
            *(int *)((char *)pManager + 0x2c4) = hMusic;
            StartRegisteredVariantResourceMusic(g_pAudioManager, hMusic);
            g_fVariantResourceMusicEnabled = fEnabled;
            return;
        }
        StopVariantResourceMusicPlayback(g_pAudioManager, *(int *)((char *)pManager + 0x2c4));
        UnregisterVariantResourceMusicHandle(g_pAudioManager, *(int *)((char *)pManager + 0x2c4));
        g_fVariantResourceMusicEnabled = fEnabled;
    }
}

// FUNCTION: LEMBALL 0x00438D60
void ClearActiveVariantResourceEntries(int pManager) {
    int *pEntry;
    int i;

    pEntry = (int *)(unsigned long)(pManager + 0x6c);
    i = 0x32;
    do {
        if (*pEntry != -1) {
            InvokeAudioManagerEffectSlot48IfActive(g_pAudioManager, *pEntry);
            *pEntry = -1;
        }
        pEntry += 3;
        --i;
    } while (i != 0);
}

// FUNCTION: LEMBALL 0x00438D90
void PopulateVariantResourceEntriesForFlagMask(void *pManager, unsigned short nMask) {
    int *pEntry;
    int i;
    DWORD dwNow;

    if (g_fEffectsOptionAvailable != 0) {
        pEntry = (int *)((char *)pManager + 0x68);
        dwNow = timeGetTime();
        i = 0x32;
        do {
            if (*pEntry != 0 && ((*(unsigned short *)(unsigned long)(*pEntry + 10) & nMask) != 0)) {
                pEntry[1] = CreateVariantResourceEffectInstance(g_pAudioManager, *(int *)(unsigned long)(*pEntry + 4));
            }
            pEntry[2] = (int)(dwNow - 100);
            if (*(void **)((char *)pManager + 4) != 0) {
                ((void (*)())**(void ***)((char *)pManager + 4))();
            }
            pEntry += 3;
            --i;
        } while (i != 0);
    }
}

// FUNCTION: LEMBALL 0x00438DF0
int CountVariantResourceEntriesWithFlagMask(void *pManager, unsigned short nMask) {
    int cMatches;
    int *pEntry;
    int i;

    cMatches = 0;
    if (g_fEffectsOptionAvailable != 0) {
        pEntry = (int *)((char *)pManager + 0x68);
        i = 0x32;
        do {
            if (*pEntry != 0 && ((*(unsigned short *)(unsigned long)(*pEntry + 10) & nMask) != 0)) {
                ++cMatches;
            }
            pEntry += 3;
            --i;
        } while (i != 0);
    }
    return cMatches;
}

// FUNCTION: LEMBALL 0x00438E30
void SwitchVariantResourceEntryMode(void *pManager, unsigned short nMask, void *pBundle) {
    int nMusicResourceId;
    unsigned int uNextRandom;
    int fEnableMusic;

    if (*(unsigned short *)((char *)pManager + 0x0c) != nMask) {
        fEnableMusic = g_fLevelDemoModeEnabled == 0;
        if (fEnableMusic) {
            SetVariantResourceMusicEnabled(pManager, 0);
            ClearActiveVariantResourceEntries((int)(unsigned long)pManager);
            StopAllAudioManagerBackends(g_pAudioManager);
        } else {
            ClearActiveVariantResourceEntries((int)(unsigned long)pManager);
        }

        *(unsigned short *)((char *)pManager + 0x0c) = nMask;
        nMusicResourceId = 0;
        switch (nMask) {
        case 0:
        case 1:
            goto done_switch;
        case 2:
            nMask = 3;
            fEnableMusic = 0;
        case 3:
            nMusicResourceId = 0x242;
            SetAudioManagerPrimaryContext(g_pAudioManager, (void *)0x243);
            break;
        case 4:
            SetAudioManagerPrimaryContext(g_pAudioManager, (void *)0x243);
            nMusicResourceId = *(int *)((char *)pManager + 0x2c8) + 0x239;
            uNextRandom = (*(unsigned int *)g_pSessionRandomState * 0x29 + 0x1fU) & 0x7fffff;
            *(unsigned int *)g_pSessionRandomState = uNextRandom;
            *(unsigned int *)((char *)pManager + 0x2c8) = uNextRandom % 9;
            break;
        }

        *(void **)((char *)pManager + 4) = pBundle;
        if (g_fLevelDemoModeEnabled == 0) {
            RefreshAudioManagerBackendHandles(g_pAudioManager);
            *(int *)((char *)pManager + 0x2c0) = nMusicResourceId;
        }
        PopulateVariantResourceEntriesForFlagMask(pManager, nMask);
        if (fEnableMusic) {
            SetVariantResourceMusicEnabled(pManager, 1);
        }
        *(void **)((char *)pManager + 4) = 0;
    }

done_switch:
    return;
}

// FUNCTION: LEMBALL 0x00439A70
void *ConstructVariantResourceEntryManager(void *pManager) {
    int *pVariantManager;
    int *pEntry;
    int i;

    pVariantManager = (int *)pManager;
    pVariantManager[0x19] = 0;
    pVariantManager[0] = g_nLevelFrameClockTick;
    pVariantManager[2] = 0;
    *(unsigned short *)(pVariantManager + 3) = 0;
    pVariantManager[0xb1] = 0;
    pVariantManager[0xb0] = 0;
    pVariantManager[0xb2] = 0;
    pVariantManager[1] = 0;
    if (g_fMusicEnabled != 0) {
        InvokeAudioManagerEmbeddedSlot04(g_pAudioManager, 0x2220, 0xb482);
        g_fVariantResourceMusicEnabled = 0;
    }
    SetVariantResourceEffectsEnabled(1);
    i = 0x32;
    pEntry = pVariantManager + 0x1b;
    do {
        *pEntry = -1;
        pEntry[-1] = 0;
        pEntry += 3;
        --i;
    } while (i != 0);

    if (g_fEffectsOptionAvailable != 0) {
        pEntry = g_GAME_VariantResourceEntryEffectTable;
        do {
            if (g_fEffectsOptionAvailable != 0) {
                pVariantManager[pEntry[0] * 3 + 0x1a] = (int)(unsigned long)pEntry;
                pVariantManager[pEntry[0] * 3 + 0x1b] = -1;
            }
            pEntry += 3;
        } while (pEntry < (g_GAME_VariantResourceEntryEffectTable + 0x96 / sizeof(int)));
    }
    pVariantManager[4] = 0;
    return pManager;
}

// FUNCTION: LEMBALL 0x00439B30
void ShutdownVariantResourceEntryManager(void *pManager) {
    if (g_fMusicEnabled != 0) {
        SetVariantResourceMusicEnabled(pManager, 0);
    }
    ClearActiveVariantResourceEntries((int)(unsigned long)pManager);
}

// FUNCTION: LEMBALL 0x00447DE0
void ReleaseMainGameVariantZrleListResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundle *pVariantBundle;
    unsigned int i;

    pVariantBundle = (GAME_MainGameVariantResourceBundle *)pBundle;
    i = 0;
    if ((unsigned int)pVariantBundle->m_cZrleListResources != 0) {
        while (pVariantBundle->m_ppZrleListResources[i] == 0 ||
               (*(int *)((char *)pVariantBundle->m_ppZrleListResources[i] + 0x30) != nResourceId)) {
            ++i;
            if ((unsigned int)pVariantBundle->m_cZrleListResources <= i) {
                return;
            }
        }
        ReleaseTypedResourceObjectReference(pVariantBundle->m_ppZrleListResources[i]);
        pVariantBundle->m_ppZrleListResources[i] = 0;
    }
}

// FUNCTION: LEMBALL 0x00447E60
void ReleaseMainGameVariantListResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundle *pVariantBundle;
    unsigned int i;

    pVariantBundle = (GAME_MainGameVariantResourceBundle *)pBundle;
    i = 0;
    if ((unsigned int)pVariantBundle->m_cListResources != 0) {
        while (pVariantBundle->m_ppListResources[i] == 0 ||
               (*(int *)((char *)pVariantBundle->m_ppListResources[i] + 0x30) != nResourceId)) {
            ++i;
            if ((unsigned int)pVariantBundle->m_cListResources <= i) {
                return;
            }
        }
        ReleaseTypedResourceObjectReference(pVariantBundle->m_ppListResources[i]);
        pVariantBundle->m_ppListResources[i] = 0;
    }
}

// FUNCTION: LEMBALL 0x00447EE0
void ReleaseMainGameVariantBitmapResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundle *pVariantBundle;
    unsigned int i;

    pVariantBundle = (GAME_MainGameVariantResourceBundle *)pBundle;
    i = 0;
    if ((unsigned int)pVariantBundle->m_cBitmapResources != 0) {
        while (pVariantBundle->m_ppBitmapResources[i] == 0 ||
               (*(int *)((char *)pVariantBundle->m_ppBitmapResources[i] + 0x30) != nResourceId)) {
            ++i;
            if ((unsigned int)pVariantBundle->m_cBitmapResources <= i) {
                return;
            }
        }
        ReleaseTypedResourceObjectReference(pVariantBundle->m_ppBitmapResources[i]);
        pVariantBundle->m_ppBitmapResources[i] = 0;
    }
}

// FUNCTION: LEMBALL 0x00447F60
void ReleaseMainGameVariantPaletteResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundle *pVariantBundle;
    unsigned int i;

    pVariantBundle = (GAME_MainGameVariantResourceBundle *)pBundle;
    i = 0;
    if (*(int *)&pVariantBundle->m_pReserved38 != 0) {
        while (pVariantBundle->m_ppPaletteResources[i] == 0 ||
               (*(int *)((char *)pVariantBundle->m_ppPaletteResources[i] + 0x30) != nResourceId)) {
            ++i;
            if (*(unsigned int *)&pVariantBundle->m_pReserved38 <= i) {
                return;
            }
        }
        ReleaseTypedResourceObjectReference(pVariantBundle->m_ppPaletteResources[i]);
        pVariantBundle->m_ppPaletteResources[i] = 0;
    }
}

// FUNCTION: LEMBALL 0x00447FE0
void ReleaseMainGameVariantStringResource(void *pBundle, int nResourceId) {
    GAME_MainGameVariantResourceBundle *pVariantBundle;
    unsigned int i;

    pVariantBundle = (GAME_MainGameVariantResourceBundle *)pBundle;
    i = 0;
    if (*(int *)&pVariantBundle->m_pReserved40 != 0) {
        while (pVariantBundle->m_ppStringResources[i] == 0 ||
               (*(int *)((char *)pVariantBundle->m_ppStringResources[i] + 0x30) != nResourceId)) {
            ++i;
            if (*(unsigned int *)&pVariantBundle->m_pReserved40 <= i) {
                return;
            }
        }
        ReleaseTypedResourceObjectReference(pVariantBundle->m_ppStringResources[i]);
        pVariantBundle->m_ppStringResources[i] = 0;
    }
}

// FUNCTION: LEMBALL 0x00447C50
void DestroyMainGameVariantResourceMode(void *pBundle) {
    GAME_MainGameVariantResourceBundle *pVariantBundle;
    int *pnResourceId;
    unsigned int i;

    pVariantBundle = (GAME_MainGameVariantResourceBundle *)pBundle;
    pVariantBundle->m_pVtable = (void **)g_GAME_MainGameVariantResourceBundleVtable;
    SwitchVariantResourceEntryMode(g_pVariantResourceEntryManager, 0, 0);

    for (i = 0; i < (unsigned int)pVariantBundle->m_cZrleListResources; ++i) {
        ReleaseMainGameVariantZrleListResource(pVariantBundle, pVariantBundle->m_panZrleListResourceIds[i]);
    }
    for (i = 0; i < (unsigned int)pVariantBundle->m_cListResources; ++i) {
        ReleaseMainGameVariantListResource(pVariantBundle, pVariantBundle->m_panListResourceIds[i]);
    }
    for (i = 0; i < (unsigned int)pVariantBundle->m_cBitmapResources; ++i) {
        ReleaseMainGameVariantBitmapResource(pVariantBundle, pVariantBundle->m_panBitmapResourceIds[i]);
    }

    for (pnResourceId = g_GAME_MainGameVariantPaletteResourceIds; pnResourceId < g_GAME_MainGameVariantStringResourceIds;
         ++pnResourceId) {
        ReleaseMainGameVariantPaletteResource(pVariantBundle, *pnResourceId);
    }
    for (pnResourceId = g_GAME_MainGameVariantStringResourceIds; pnResourceId < g_GAME_MainGameVariantStringResourceIdsEnd;
         ++pnResourceId) {
        ReleaseMainGameVariantStringResource(pVariantBundle, *pnResourceId);
    }

    for (i = 0; i < 6; ++i) {
        ReleaseTypedResourceObjectReference(*(void **)((char *)pVariantBundle->m_pTwoArrayListResources + i * 4));
        *(int *)((char *)pVariantBundle->m_pTwoArrayListResources + i * 4) = 0;
    }

    FreeVSMemBlock(pVariantBundle->m_pTwoArrayListResources);
    FreeVSMemBlock(pVariantBundle->m_ppStringResources);
    FreeVSMemBlock(pVariantBundle->m_ppPaletteResources);
    FreeVSMemBlock(pVariantBundle->m_ppBitmapResources);
    FreeVSMemBlock(pVariantBundle->m_ppListResources);
    FreeVSMemBlock(pVariantBundle->m_ppZrleListResources);
    PruneUnreferencedCachedResourceObjects(g_pMainResourceArchive);
}

// FUNCTION: LEMBALL 0x004479E0
void *ConstructMainGameVariantResourceBundle(void *pBundle, void *pPrimaryContext, unsigned short nVariantMode) {
    GAME_MainGameVariantResourceBundle *pVariantBundle;
    int *pPrimaryContextWords;
    int *panResourceIds;
    int cResources;
    int i;
    int *panEnd;

    pVariantBundle = (GAME_MainGameVariantResourceBundle *)pBundle;
    pPrimaryContextWords = (int *)pPrimaryContext;

    pVariantBundle->m_pVtable = (void **)g_GAME_MainGameVariantResourceBundleVtable;
    pVariantBundle->m_pReserved48 = 0;
    pVariantBundle->m_pReserved40 = 0;
    pVariantBundle->m_pReserved38 = 0;
    pVariantBundle->m_pReserved2C = 0;
    pVariantBundle->m_pReserved20 = 0;
    pVariantBundle->m_pReserved14 = 0;

    if (g_fCompactPrimaryContextLayout != 0) {
        pVariantBundle->m_panZrleListResourceIds = g_GAME_MainGameVariantCompactZrleListResourceIds;
        pVariantBundle->m_panListResourceIds = g_GAME_MainGameVariantCompactListResourceIds;
        pVariantBundle->m_panBitmapResourceIds = g_GAME_MainGameVariantCompactBitmapResourceIds;
        pVariantBundle->m_cZrleListResources = 0x44;
        pVariantBundle->m_cListResources = 1;
        pVariantBundle->m_cBitmapResources = 3;
        pVariantBundle->m_nTotalResourceCount = 0x48;
    } else {
        pVariantBundle->m_panZrleListResourceIds = g_GAME_MainGameVariantStandardZrleListResourceIds;
        pVariantBundle->m_panListResourceIds = g_GAME_MainGameVariantStandardListResourceIds;
        pVariantBundle->m_panBitmapResourceIds = g_GAME_MainGameVariantStandardBitmapResourceIds;
        pVariantBundle->m_cZrleListResources = 0x43;
        pVariantBundle->m_cListResources = 1;
        pVariantBundle->m_cBitmapResources = 3;
        pVariantBundle->m_nTotalResourceCount = 0x47;
    }

    pVariantBundle->m_nTotalResourceCount += 3;
    pVariantBundle->m_nTotalResourceCount += 3;
    pVariantBundle->m_nTotalResourceCount += 3;

    pVariantBundle->m_ppZrleListResources =
        (void **)AllocateVSMemBlock((unsigned int)pVariantBundle->m_cZrleListResources * 4U);
    pVariantBundle->m_ppListResources =
        (void **)AllocateVSMemBlock((unsigned int)pVariantBundle->m_cListResources * 4U);
    pVariantBundle->m_ppBitmapResources =
        (void **)AllocateVSMemBlock((unsigned int)pVariantBundle->m_cBitmapResources * 4U);
    pVariantBundle->m_ppPaletteResources = (void **)AllocateVSMemBlock(8);
    pVariantBundle->m_ppStringResources = (void **)AllocateVSMemBlock(4);
    pVariantBundle->m_pTwoArrayListResources = AllocateVSMemBlock(0x18);

    pVariantBundle->m_pStatusIndicatorManager = AllocateVSMemBlock(0x134);
    if (pVariantBundle->m_pStatusIndicatorManager != 0) {
        pVariantBundle->m_pStatusIndicatorManager =
            ConstructLevelScreenStatusIndicatorManager(pVariantBundle->m_pStatusIndicatorManager,
                                                       pPrimaryContextWords[0x13],
                                                       pPrimaryContextWords);
    }

    if (pVariantBundle->m_pStatusIndicatorManager != 0) {
        pPrimaryContextWords[0x29] = (int)(unsigned long)((char *)pVariantBundle->m_pStatusIndicatorManager + 0x74);
    } else {
        pPrimaryContextWords[0x29] = 0;
    }

    pVariantBundle->m_nTotalResourceCount +=
        CountVariantResourceEntriesWithFlagMask(g_pVariantResourceEntryManager, nVariantMode);
    ReleaseLevelSelectionModeRenderStateResource(pVariantBundle->m_pStatusIndicatorManager);
    pVariantBundle->m_nLoadedResourceCount = 0;
    SwitchVariantResourceEntryMode(g_pVariantResourceEntryManager, nVariantMode, pVariantBundle);

    for (i = 0; i < pVariantBundle->m_cZrleListResources; ++i) {
        LoadMainGameVariantZrleListResource(pVariantBundle, pVariantBundle->m_panZrleListResourceIds[i]);
    }

    cResources = pVariantBundle->m_cListResources;
    for (i = 0; i < cResources; ++i) {
        LoadMainGameVariantListResource(pVariantBundle, pVariantBundle->m_panListResourceIds[i]);
    }

    for (i = 0; i < pVariantBundle->m_cBitmapResources; ++i) {
        LoadMainGameVariantBitmapResource(pVariantBundle, pVariantBundle->m_panBitmapResourceIds[i]);
    }

    for (panResourceIds = g_GAME_MainGameVariantPaletteResourceIds; panResourceIds < g_GAME_MainGameVariantStringResourceIds;
         ++panResourceIds) {
        LoadMainGameVariantPaletteResource(pVariantBundle, *panResourceIds);
    }

    for (panResourceIds = g_GAME_MainGameVariantStringResourceIds; panResourceIds < g_GAME_MainGameVariantStringResourceIdsEnd;
         ++panResourceIds) {
        LoadMainGameVariantStringResource(pVariantBundle, *panResourceIds);
    }

    for (i = 0x11e; i < 0x121; ++i) {
        LoadMainGameVariantTwoArrayListResource(pVariantBundle, i);
    }

    for (i = 0x121; i < 0x124; ++i) {
        LoadMainGameVariantTwoArrayListResource(pVariantBundle, i);
    }

    pPrimaryContextWords[0x29] = 0;
    if (pVariantBundle->m_pStatusIndicatorManager != 0) {
        DestroyLevelScreenStatusIndicatorManager(pVariantBundle->m_pStatusIndicatorManager);
        FreeVSMemBlock(pVariantBundle->m_pStatusIndicatorManager);
    }
    return pVariantBundle;
}
