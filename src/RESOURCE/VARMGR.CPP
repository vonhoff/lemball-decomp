#include "GAME.H"
#include "SHELL/OPTIONS.H"
#include "ENGINE/MEDIA/VSSOUND.H"
#include "ENGINE/CORE/VSMEM.H"
#include "RESOURCE/VARLOAD.H"
#include "RESOURCE/VARMGR.H"
#include "RESOURCE/MOGLOAD.H"

extern "C" DWORD WINAPI timeGetTime(void);

struct GameMainGameVariantResourceBundle {
    void **m_pVtable;
    void *m_pStatusIndicatorManager;
    int m_nLoadedResourceCount;
    int m_nTotalResourceCount;
    void **m_ppZrleListResources;
    int m_cLoadedZrleListResources;
    int m_cZrleListResources;
    void **m_ppListResources;
    int m_cLoadedListResources;
    int m_cListResources;
    void **m_ppBitmapResources;
    int m_cLoadedBitmapResources;
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

extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);

static void LEMBALL_FASTCALL UpdateMainGameVariantResourceLoadProgressThunk(
    void *pObject);
void VariantResourceBundleNoopThunk(void);

static void *g_GAME_MainGameVariantResourceBundleVtableStorage[4] = {
    (void *)UpdateMainGameVariantResourceLoadProgressThunk,
    (void *)VariantResourceBundleNoopThunk,
    0,
    (void *)InitializeNonZrleVariantRenderEntry,
};
static void *g_GAME_MainGameVariantResourceBundleVtable =
    g_GAME_MainGameVariantResourceBundleVtableStorage;

static void LEMBALL_FASTCALL UpdateMainGameVariantResourceLoadProgressThunk(
    void *pObject);
void VariantResourceBundleNoopThunk(void);
void *LEMBALL_FASTCALL ConstructVariantResourceEntryManagerThunk(void *pManager);
void SetVariantResourceEffectsEnabledThunk(int fEnabled);
static void VariantResourceBundleNoop(void);
void *LEMBALL_FASTCALL ConstructVariantResourceEntryManagerBody(void *pManager);
void LEMBALL_FASTCALL ShutdownVariantResourceEntryManager(void *pManager);
void SetVariantResourceEffectsEnabled(int fEnabled);
void *ConstructMainGameVariantResourceBundle(void *pBundle, void *pPrimaryContext, unsigned short nVariantMode);
void LEMBALL_FASTCALL DestroyMainGameVariantResourceMode(void *pBundle);
static void LEMBALL_FASTCALL UpdateMainGameVariantResourceLoadProgress(
    void *pObject);
void ReleaseMainGameVariantZrleListResource(void *pBundle, int nResourceId);
void ReleaseMainGameVariantListResource(void *pBundle, int nResourceId);
void ReleaseMainGameVariantBitmapResource(void *pBundle, int nResourceId);
void ReleaseMainGameVariantPaletteResource(void *pBundle, int nResourceId);
void ReleaseMainGameVariantStringResource(void *pBundle, int nResourceId);

// GLOBAL: LEMBALL 0x0049f1c0
static int g_GAME_MainGameVariantCompactZrleListResourceIds[0x44] = {
    0x173, 0x174, 0x175, 0x176, 0x177, 0x178, 0x179, 0x17a,
    0x17b, 0x1db, 0x1da, 0x1e4, 0x1dd, 0x1e0, 0x1e1, 0x1e2,
    0x1e3, 0x1ea, 0x1f8, 0x1f7, 0x1d8, 0x1d9, 0x1d6, 0x1d7,
    0x183, 0x1e8, 0x1e9, 0x1ec, 0x208, 0x205, 0x206, 0x207,
    0x1ed, 0x1e7, 0x1eb, 0x1de, 0x1df, 0x1ef, 0x1f0, 0x1f3,
    0x1f4, 0x1e5, 0x1e6, 0x1f1, 0x1f2, 0x180, 0x181, 0x17c,
    0x182, 0x17d, 0x17e, 0x17f, 0x185, 0x184, 0x186, 0x187,
    0x188, 0x189, 0x18a, 0x18b, 0x18c, 0x18d, 0x18e, 0x18f,
    0x191, 0x190, 0x192, 0x1dc,
};

// GLOBAL: LEMBALL 0x0049f2d0
static int g_GAME_MainGameVariantStandardZrleListResourceIds[0x43] = {
    0x146, 0x147, 0x148, 0x149, 0x14a, 0x14b, 0x14c, 0x14d,
    0x14e, 0x1a7, 0x1a6, 0x1b0, 0x1a9, 0x1ac, 0x1ad, 0x1ae,
    0x1af, 0x1b6, 0x1c4, 0x1c3, 0x1a4, 0x1a5, 0x1a2, 0x1a3,
    0x156, 0x1b4, 0x1b5, 0x1b8, 0x1b9, 0x1d5, 0x1d2, 0x1d3,
    0x1d4, 0x1aa, 0x1ab, 0x1b3, 0x1b7, 0x1bb, 0x1bc, 0x1bf,
    0x1c0, 0x1b1, 0x1b2, 0x1bd, 0x1be, 0x153, 0x154, 0x14f,
    0x155, 0x150, 0x151, 0x152, 0x158, 0x157, 0x159, 0x15a,
    0x15b, 0x15c, 0x15d, 0x15e, 0x15f, 0x160, 0x161, 0x162,
    0x164, 0x163, 0x165,
};

// GLOBAL: LEMBALL 0x0049f3dc
static int g_GAME_MainGameVariantCompactListResourceIds[1] = {0x114};

// GLOBAL: LEMBALL 0x0049f3e0
static int g_GAME_MainGameVariantStandardListResourceIds[1] = {0x113};

// GLOBAL: LEMBALL 0x0049f3e8
static int g_GAME_MainGameVariantCompactBitmapResourceIds[3] = {0x13c, 0x13d, 0x13e};

// GLOBAL: LEMBALL 0x0049f3f8
static int g_GAME_MainGameVariantStandardBitmapResourceIds[3] = {0x131, 0x132, 0x133};

// GLOBAL: LEMBALL 0x0049f408
static int g_GAME_MainGameVariantPaletteResourceIds[2] = {0x10a, 0x102};

// GLOBAL: LEMBALL 0x0049f410
static int g_GAME_MainGameVariantStringResourceIds[1] = {0x100};
static int g_GAME_VariantResourceEntryEffectTable[44][3] = {
    {1, 532, 262153}, {2, 544, 262153}, {3, 543, 262243}, {4, 537, 262153},
    {5, 521, 9}, {6, 522, 9}, {7, 523, 458761}, {8, 524, 9},
    {9, 525, 262153}, {10, 526, 262153}, {11, 527, 262153}, {12, 528, 262153},
    {13, 529, 262153}, {14, 530, 458767}, {15, 531, 262151}, {16, 533, 262153},
    {17, 534, 458761}, {18, 535, 262153}, {19, 536, 458761}, {20, 538, 262153},
    {21, 539, 262153}, {22, 540, 262153}, {23, 542, 262153}, {24, 541, 262153},
    {26, 545, 262153}, {27, 546, 458761}, {25, 547, 458761}, {28, 548, 262159},
    {29, 549, 262159}, {30, 555, 262159}, {31, 550, 262153}, {32, 551, 262153},
    {33, 552, 262153}, {34, 556, 458761}, {35, 557, 262153}, {37, 553, 458761},
    {38, 554, 262153}, {39, 559, 196617}, {40, 560, 196617}, {41, 564, 262153},
    {42, 562, 262153}, {43, 563, 262153}, {44, 561, 262153}, {45, 565, 262153},
};
// GLOBAL: LEMBALL 0x0049ed98
int g_fVariantResourceEffectsEnabled = 1;
int g_fVariantResourceMusicEnabled = 0;

extern void *g_pVariantResourceEntryManager;
extern void *g_pSessionRandomState;
extern int g_nLevelFrameClockTick;
extern int g_fLevelDemoModeEnabled;
extern void *g_pMainResourceArchive;
extern void *ConstructLevelScreenStatusIndicatorManager(void *pObject, int nStatusMode, void *pPrimaryContext);
extern void LEMBALL_FASTCALL ReleaseLevelSelectionModeRenderStateResource(void *pStatusIndicatorManager);
extern void LEMBALL_FASTCALL ReleaseStatusIndicatorPrimaryBitmapThunk(
    void *pStatusIndicatorManager);
extern void DestroyLevelScreenStatusIndicatorManager(void *pStatusIndicatorManager);
extern void LoadMainGameVariantZrleListResource(void *pBundle, int nResourceId);
extern void LoadMainGameVariantListResource(void *pBundle, int nResourceId);
extern void LoadMainGameVariantBitmapResource(void *pBundle, int nResourceId);
extern void LoadMainGameVariantPaletteResource(void *pBundle, int nResourceId);
extern void LoadMainGameVariantTwoArrayListResource(void *pBundle, int nResourceId);
void SetVariantResourceEffectsEnabledThunk(int fEnabled);


// FUNCTION: LEMBALL 0x00401104
static void LEMBALL_FASTCALL UpdateMainGameVariantResourceLoadProgressThunk(
    void *pObject)
{
    UpdateMainGameVariantResourceLoadProgress(pObject);
}

// FUNCTION: LEMBALL 0x00401479
void GameVariantResourceEntryManager::PlayVariantResourceEffectThunk(int nEffectSlot) {
    PlayVariantResourceEffect(nEffectSlot);
}

// FUNCTION: LEMBALL 0x004015b9
void VariantResourceBundleNoopThunk(void) {
    VariantResourceBundleNoop();
}

// FUNCTION: LEMBALL 0x0040295a
void *LEMBALL_FASTCALL ConstructVariantResourceEntryManagerThunk(void *pManager) {
    return ConstructVariantResourceEntryManagerBody(pManager);
}

// FUNCTION: LEMBALL 0x00403873
void SetVariantResourceEffectsEnabledThunk(int fEnabled) {
    SetVariantResourceEffectsEnabled(fEnabled);
}

// FUNCTION: LEMBALL 0x004358c0
static void VariantResourceBundleNoop(void) {
}

// FUNCTION: LEMBALL 0x00439a70
void *LEMBALL_FASTCALL ConstructVariantResourceEntryManagerBody(void *pManager) {
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
        ((VsSoundManager *)g_pAudioManager)
            ->InvokeAudioManagerEmbeddedSlot04(0x2220, 0xb482);
        g_fVariantResourceMusicEnabled = 0;
    }
    SetVariantResourceEffectsEnabledThunk(1);
    i = 0x32;
    pEntry = pVariantManager + 0x1b;
    do {
        *pEntry = -1;
        pEntry[-1] = 0;
        pEntry += 3;
        --i;
    } while (i != 0);

    if (g_fEffectsOptionAvailable != 0) {
        pEntry = &g_GAME_VariantResourceEntryEffectTable[0][0];
        do {
            if (g_fEffectsOptionAvailable != 0) {
                pVariantManager[pEntry[0] * 3 + 0x1a] = (int)(unsigned long)pEntry;
                pVariantManager[pEntry[0] * 3 + 0x1b] = -1;
            }
            pEntry += 3;
        } while (pEntry < &g_GAME_VariantResourceEntryEffectTable[44][0]);
    }
    pVariantManager[4] = 0;
    return pManager;
}

// FUNCTION: LEMBALL 0x00439b30
void LEMBALL_FASTCALL ShutdownVariantResourceEntryManager(void *pManager) {
    GameVariantResourceEntryManager *pVariantManager;

    pVariantManager = (GameVariantResourceEntryManager *)pManager;
    if (g_fMusicEnabled != 0) {
        pVariantManager->SetVariantResourceMusicEnabled(0);
    }
    pVariantManager->ClearActiveVariantResourceEntries();
}

// FUNCTION: LEMBALL 0x00439b50
void SetVariantResourceEffectsEnabled(int fEnabled) {
    if (g_fEffectsOptionAvailable != 0) {
        g_fVariantResourceEffectsEnabled = fEnabled;
    }
}

// FUNCTION: LEMBALL 0x00439b70
void GameVariantResourceEntryManager::PlayVariantResourceEffect(int nEffectSlot) {
    if (g_fVariantResourceEffectsEnabled != 0) {
        ((VsSoundManager *)g_pAudioManager)
            ->PlayVariantResourceEffectId(
                *(int *)((char *)this + nEffectSlot * 0x0c + 0x6c));
    }
}

// FUNCTION: LEMBALL 0x00439ba0
void GameVariantResourceEntryManager::SetVariantResourceMusicEnabled(int fEnabled) {
    int hMusic;
    void *pManager;

    pManager = this;

    if (fEnabled != 0) {
        if (g_fVariantResourceMusicEnabled != 0) {
            return;
        }
    } else if (g_fVariantResourceMusicEnabled == 0) {
        return;
    }

    if (g_fMusicEnabled != 0 && *(int *)((char *)pManager + 0x2c0) != 0) {
        if (fEnabled != 0) {
            hMusic = RegisterVariantResourceMusicHandle(
                g_pAudioManager, 0, *(int *)((char *)pManager + 0x2c0));
            *(int *)((char *)pManager + 0x2c4) = hMusic;
            StartRegisteredVariantResourceMusic(
                g_pAudioManager, 0, hMusic);
            g_fVariantResourceMusicEnabled = fEnabled;
            return;
        }
        StopVariantResourceMusicPlayback(
            g_pAudioManager, 0, *(int *)((char *)pManager + 0x2c4));
        UnregisterVariantResourceMusicHandle(
            g_pAudioManager, 0, *(int *)((char *)pManager + 0x2c4));
        g_fVariantResourceMusicEnabled = fEnabled;
    }
}

// FUNCTION: LEMBALL 0x00439d60
void GameVariantResourceEntryManager::ClearActiveVariantResourceEntries(void) {
    int *pEntry;
    int i;
    int pManager;

    pManager = (int)(unsigned long)this;
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

// FUNCTION: LEMBALL 0x00439d90
void GameVariantResourceEntryManager::PopulateVariantResourceEntriesForFlagMask(unsigned short nMask) {
    int *pEntry;
    int i;
    DWORD dwNow;
    void *pManager;

    pManager = this;
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
                void *pBundle = *(void **)((char *)pManager + 4);
                ((void (LEMBALL_FASTCALL *)(void *, int))(*(void ***)pBundle)[0])(
                    pBundle, 0);
            }
            pEntry += 3;
            --i;
        } while (i != 0);
    }
}

// FUNCTION: LEMBALL 0x00439df0
int GameVariantResourceEntryManager::CountVariantResourceEntriesWithFlagMask(unsigned short nMask) {
    int cMatches;
    int *pEntry;
    int i;
    void *pManager;

    pManager = this;
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

// FUNCTION: LEMBALL 0x00439e30
void GameVariantResourceEntryManager::SwitchVariantResourceEntryMode(unsigned short nMask, void *pBundle) {
    int nMusicResourceId;
    int nNextRandom;
    int fEnableMusic;
    void *pManager;

    pManager = this;
    if (*(unsigned short *)((char *)pManager + 0x0c) != nMask) {
        fEnableMusic = g_fLevelDemoModeEnabled == 0;
        if (fEnableMusic) {
            SetVariantResourceMusicEnabled(0);
            ClearActiveVariantResourceEntries();
            StopAllAudioManagerBackends(g_pAudioManager);
        } else {
            ClearActiveVariantResourceEntries();
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
            nNextRandom = (*(int *)g_pSessionRandomState * 0x29 + 0x1f) & 0x7fffff;
            *(int *)g_pSessionRandomState = nNextRandom;
            *(int *)((char *)pManager + 0x2c8) = nNextRandom % 9;
            break;
        }

        *(void **)((char *)pManager + 4) = pBundle;
        if (g_fLevelDemoModeEnabled == 0) {
            RefreshAudioManagerBackendHandles(g_pAudioManager);
            *(int *)((char *)pManager + 0x2c0) = nMusicResourceId;
        }
        PopulateVariantResourceEntriesForFlagMask(nMask);
        if (fEnableMusic) {
            SetVariantResourceMusicEnabled(1);
        }
        *(void **)((char *)pManager + 4) = 0;
    }

done_switch:
    return;
}

// FUNCTION: LEMBALL 0x004479e0
void *ConstructMainGameVariantResourceBundle(void *pBundle, void *pPrimaryContext, unsigned short nVariantMode) {
    GameMainGameVariantResourceBundle *pVariantBundle;
    int *pPrimaryContextWords;
    int *panResourceIds;
    int cResources;
    int i;
    int *panEnd;

    pVariantBundle = (GameMainGameVariantResourceBundle *)pBundle;
    pPrimaryContextWords = (int *)pPrimaryContext;

    pVariantBundle->m_pVtable = (void **)g_GAME_MainGameVariantResourceBundleVtable;
    pVariantBundle->m_pReserved48 = 0;
    pVariantBundle->m_pReserved40 = 0;
    pVariantBundle->m_pReserved38 = 0;
    pVariantBundle->m_cLoadedBitmapResources = 0;
    pVariantBundle->m_cLoadedListResources = 0;
    pVariantBundle->m_cLoadedZrleListResources = 0;

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
        ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
            ->CountVariantResourceEntriesWithFlagMask(nVariantMode);
    ReleaseStatusIndicatorPrimaryBitmapThunk(
        pVariantBundle->m_pStatusIndicatorManager);
    pVariantBundle->m_nLoadedResourceCount = 0;
    ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
        ->SwitchVariantResourceEntryMode(nVariantMode, pVariantBundle);

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

    for (panResourceIds = g_GAME_MainGameVariantPaletteResourceIds;
         panResourceIds < g_GAME_MainGameVariantPaletteResourceIds + 2;
         ++panResourceIds) {
        LoadMainGameVariantPaletteResource(pVariantBundle, *panResourceIds);
    }

    for (panResourceIds = g_GAME_MainGameVariantStringResourceIds;
         panResourceIds < g_GAME_MainGameVariantStringResourceIds + 1;
         ++panResourceIds) {
        ((GameMainGameVariantResourceBundleLoader *)pVariantBundle)
            ->LoadMainGameVariantStringResource(*panResourceIds);
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

// FUNCTION: LEMBALL 0x00447c50
void LEMBALL_FASTCALL DestroyMainGameVariantResourceMode(void *pBundle) {
    GameMainGameVariantResourceBundle *pVariantBundle;
    int *pnResourceId;
    unsigned int i;

    pVariantBundle = (GameMainGameVariantResourceBundle *)pBundle;
    pVariantBundle->m_pVtable = (void **)g_GAME_MainGameVariantResourceBundleVtable;
    ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
        ->SwitchVariantResourceEntryMode(0, 0);

    for (i = 0; i < (unsigned int)pVariantBundle->m_cZrleListResources; ++i) {
        ReleaseMainGameVariantZrleListResource(pVariantBundle, pVariantBundle->m_panZrleListResourceIds[i]);
    }
    for (i = 0; i < (unsigned int)pVariantBundle->m_cListResources; ++i) {
        ReleaseMainGameVariantListResource(pVariantBundle, pVariantBundle->m_panListResourceIds[i]);
    }
    for (i = 0; i < (unsigned int)pVariantBundle->m_cBitmapResources; ++i) {
        ReleaseMainGameVariantBitmapResource(pVariantBundle, pVariantBundle->m_panBitmapResourceIds[i]);
    }

    for (pnResourceId = g_GAME_MainGameVariantPaletteResourceIds;
         pnResourceId < g_GAME_MainGameVariantPaletteResourceIds + 2;
         ++pnResourceId) {
        ReleaseMainGameVariantPaletteResource(pVariantBundle, *pnResourceId);
    }
    for (pnResourceId = g_GAME_MainGameVariantStringResourceIds;
         pnResourceId < g_GAME_MainGameVariantStringResourceIds + 1;
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
    ((MogLoadResourceArchive *)g_pMainResourceArchive)
        ->PruneUnreferencedCachedResourceObjects();
}

// FUNCTION: LEMBALL 0x00447d80
static void LEMBALL_FASTCALL UpdateMainGameVariantResourceLoadProgress(
    void *pObject)
{
    int nLoaded;
    GameMainGameVariantResourceBundle *pBundle;

    pBundle = (GameMainGameVariantResourceBundle *)pObject;
    nLoaded = ++pBundle->m_nLoadedResourceCount;
    {
        void *pProgressClient =
            (char *)pBundle->m_pStatusIndicatorManager + 0x70;
        ((void (LEMBALL_FASTCALL *)(void *, int, int))
             (*(void ***)pProgressClient)[0])(
            pProgressClient, 0,
            (nLoaded * 100) / pBundle->m_nTotalResourceCount);
    }
}

// FUNCTION: LEMBALL 0x00447de0
void ReleaseMainGameVariantZrleListResource(void *pBundle, int nResourceId) {
    GameMainGameVariantResourceBundle *pVariantBundle;
    unsigned int i;

    pVariantBundle = (GameMainGameVariantResourceBundle *)pBundle;
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

// FUNCTION: LEMBALL 0x00447e60
void ReleaseMainGameVariantListResource(void *pBundle, int nResourceId) {
    GameMainGameVariantResourceBundle *pVariantBundle;
    unsigned int i;

    pVariantBundle = (GameMainGameVariantResourceBundle *)pBundle;
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

// FUNCTION: LEMBALL 0x00447ee0
void ReleaseMainGameVariantBitmapResource(void *pBundle, int nResourceId) {
    GameMainGameVariantResourceBundle *pVariantBundle;
    unsigned int i;

    pVariantBundle = (GameMainGameVariantResourceBundle *)pBundle;
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

// FUNCTION: LEMBALL 0x00447f60
void ReleaseMainGameVariantPaletteResource(void *pBundle, int nResourceId) {
    GameMainGameVariantResourceBundle *pVariantBundle;
    unsigned int i;

    pVariantBundle = (GameMainGameVariantResourceBundle *)pBundle;
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

// FUNCTION: LEMBALL 0x00447fe0
void ReleaseMainGameVariantStringResource(void *pBundle, int nResourceId) {
    GameMainGameVariantResourceBundle *pVariantBundle;
    unsigned int i;

    pVariantBundle = (GameMainGameVariantResourceBundle *)pBundle;
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
