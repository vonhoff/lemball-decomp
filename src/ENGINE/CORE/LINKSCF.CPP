#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/GDI/VSGEOM.H"
#include "ENGINE/MEDIA/EFFSTRM.H"
#include "GAME.H"
#include "LEVEL/LEVELRUN.H"

// Recovered constructor and loader boundaries that are still being split into
// their original level modules.  Keep unverified addresses free of markers.

extern void *g_pActiveNetworkRuntimeWindow;
extern int g_cbEffTransportMaxPacketBytes;
extern void *g_pActiveLevelGameMode;
extern int *LoadBinResource(int nResourceId);
extern void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(
    void *pResourceObject);

static void NoopChunkStreamVirtual(void) {
}

static int ReturnFalseChunkStreamVirtual(void) {
    return 0;
}

static void *LEMBALL_FASTCALL GetFirstCachedChunkObject(void *pObject) {
    *(int *)((char *)pObject + 0xa8) = 0;
    if (*(int *)((char *)pObject + 0xa4) == 0) {
        return 0;
    }
    return *(void **)((char *)pObject + 4);
}

static void *LEMBALL_FASTCALL GetNextCachedChunkObject(void *pObject) {
    int nIndex;

    nIndex = *(int *)((char *)pObject + 0xa8) + 1;
    *(int *)((char *)pObject + 0xa8) = nIndex;
    if (*(int *)((char *)pObject + 0xa4) <= nIndex) {
        return 0;
    }
    return *(void **)((char *)pObject + 4 + nIndex * 4);
}

/* These tables retain the recovered common EFF stream interface while the
 * type-specific load and reset methods are reconstructed.  Keeping distinct
 * tables is important: constructors install concrete stream types, and later
 * recovery can replace individual slots without changing object layout. */
static void *g_LINKSCF_LiftChunkManagerVtable[10];
static void *g_LINKSCF_MoveChunkManagerVtable[10];
static void *g_LINKSCF_GmobChunkManagerVtable[10];
static void *g_LINKSCF_PlasChunkManagerVtable[10];
static void *g_LINKSCF_PlasChunkStreamVtable[10];
static void *g_LINKSCF_ProjectilePoolVtable[10];
static void *g_LINKSCF_ManagedEntityQueueCursorVtable[4];
static void *g_LINKSCF_ShpgChunkManagerVtable[8];
static void *g_LINKSCF_EnmyChunkManagerVtable[8];
static void *g_LINKSCF_CollChunkManagerVtable[10];
static void *g_LINKSCF_MineChunkManagerVtable[10];
static void *g_LINKSCF_DoorChunkManagerVtable[10];
static void *g_LINKSCF_RockChunkManagerVtable[10];
static void *g_LINKSCF_CaptureChunkManagerVtable[10];
static void *g_LINKSCF_LasrChunkManagerVtable[10];
static void *g_LINKSCF_TramChunkManagerVtable[10];
static void *g_LINKSCF_PgunChunkManagerVtable[10];
static void *g_LINKSCF_IceChunkManagerVtable[10];
static void *g_LINKSCF_InvsChunkManagerVtable[10];
static void *g_LINKSCF_BoonChunkObjectVtable[66];
static void *g_LINKSCF_LevelChunkStreamDispatcherVtable[10];
static int g_fLinkScfChunkVtablesInitialized;

static void InitializeLinkScfChunkVtables(void) {
    int i;

    if (g_fLinkScfChunkVtablesInitialized != 0) {
        return;
    }
    for (i = 0; i < 10; ++i) {
        g_LINKSCF_LiftChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_MoveChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_GmobChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_PlasChunkStreamVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_ProjectilePoolVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_CollChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_MineChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_DoorChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_RockChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_CaptureChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_LasrChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_TramChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_PgunChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_IceChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_InvsChunkManagerVtable[i] = g_LEVEL_EffChunkStreamBaseVtable[i];
        g_LINKSCF_LevelChunkStreamDispatcherVtable[i] = 0;
    }
    for (i = 0; i < 8; ++i) {
        g_LINKSCF_ShpgChunkManagerVtable[i] = 0;
        g_LINKSCF_EnmyChunkManagerVtable[i] = 0;
    }
    g_LINKSCF_ShpgChunkManagerVtable[1] =
        (void *)GetFirstCachedChunkObject;
    g_LINKSCF_ShpgChunkManagerVtable[2] =
        (void *)GetNextCachedChunkObject;
    g_LINKSCF_EnmyChunkManagerVtable[1] =
        (void *)GetFirstCachedChunkObject;
    g_LINKSCF_EnmyChunkManagerVtable[2] =
        (void *)GetNextCachedChunkObject;
    g_LINKSCF_LiftChunkManagerVtable[6] = (void *)NoopChunkStreamVirtual;
    g_LINKSCF_LiftChunkManagerVtable[7] = (void *)NoopChunkStreamVirtual;
    g_LINKSCF_LiftChunkManagerVtable[9] = (void *)ReturnFalseChunkStreamVirtual;
    g_LINKSCF_MoveChunkManagerVtable[6] = (void *)NoopChunkStreamVirtual;
    g_LINKSCF_MoveChunkManagerVtable[7] = (void *)NoopChunkStreamVirtual;
    g_LINKSCF_MoveChunkManagerVtable[9] = (void *)ReturnFalseChunkStreamVirtual;
    g_LINKSCF_GmobChunkManagerVtable[6] = (void *)NoopChunkStreamVirtual;
    g_LINKSCF_GmobChunkManagerVtable[7] = (void *)NoopChunkStreamVirtual;
    g_LINKSCF_GmobChunkManagerVtable[9] = (void *)ReturnFalseChunkStreamVirtual;
    g_LINKSCF_PlasChunkStreamVtable[6] = (void *)NoopChunkStreamVirtual;
    g_LINKSCF_PlasChunkStreamVtable[7] = (void *)NoopChunkStreamVirtual;
    g_LINKSCF_PlasChunkStreamVtable[9] = (void *)ReturnFalseChunkStreamVirtual;
    g_LINKSCF_ProjectilePoolVtable[6] = (void *)NoopChunkStreamVirtual;
    g_LINKSCF_ProjectilePoolVtable[7] = (void *)NoopChunkStreamVirtual;
    g_LINKSCF_ProjectilePoolVtable[9] = (void *)ReturnFalseChunkStreamVirtual;
    for (i = 0; i < 10; ++i) {
        void **pTable;
        int j;
        void **apTables[10];
        apTables[0] = g_LINKSCF_CollChunkManagerVtable;
        apTables[1] = g_LINKSCF_MineChunkManagerVtable;
        apTables[2] = g_LINKSCF_DoorChunkManagerVtable;
        apTables[3] = g_LINKSCF_RockChunkManagerVtable;
        apTables[4] = g_LINKSCF_CaptureChunkManagerVtable;
        apTables[5] = g_LINKSCF_LasrChunkManagerVtable;
        apTables[6] = g_LINKSCF_TramChunkManagerVtable;
        apTables[7] = g_LINKSCF_PgunChunkManagerVtable;
        apTables[8] = g_LINKSCF_IceChunkManagerVtable;
        apTables[9] = g_LINKSCF_InvsChunkManagerVtable;
        pTable = apTables[i];
        for (j = 6; j < 8; ++j) {
            pTable[j] = (void *)NoopChunkStreamVirtual;
        }
        pTable[9] = (void *)ReturnFalseChunkStreamVirtual;
    }
    g_fLinkScfChunkVtablesInitialized = 1;
}

static void InitializeEffChunkStreamBase(void *pObject,
                                         int nEventCode,
                                         int nPayloadType) {
    *(void **)pObject = g_GAME_EffStreamConstructionVtable;
    *(int *)((char *)pObject + 4) = nEventCode;
    ((GameEffStream *)pObject)->ResetStateFields();
    *(int *)((char *)pObject + 0x2c) = nPayloadType;
    *(void **)pObject = g_LEVEL_EffChunkStreamBaseVtable;
    if (g_pActiveNetworkRuntimeWindow != 0) {
        *(int *)((char *)pObject + 0x24) = 1;
        *(int *)((char *)pObject + 0x18) += g_cbEffTransportMaxPacketBytes;
    }
}

static void InitializeCachedChunkObjectManagerBase(void *pObject) {
    int i;

    for (i = 0; i < 40; ++i) {
        ((int *)pObject)[i] = 0;
    }
    *(int *)((char *)pObject + 0xa4) = 0;
    *(int *)((char *)pObject + 0xa8) = 0;
    *(int *)((char *)pObject + 0xac) = 1;
}

static void *InitializeSimpleChunkManager(void *pObject,
                                         void **pVtable,
                                         int nEventCode,
                                         int nPayloadType) {
    InitializeLinkScfChunkVtables();
    InitializeEffChunkStreamBase(pObject, nEventCode, nPayloadType);
    *(void **)pObject = pVtable;
    return pObject;
}

void InitializeNetworkPlayerPlasChunkObjectPoolThunk(void *pPlasManager) {
    (void)pPlasManager;
}

void LoadLevelChunksForSkillAndLevelThunk(void *pLoaderContext, unsigned int nSkill, unsigned int nLevel, int fReuseState) {
    static const int anSkillResourceBases[5] = {
        0x244, 0x25d, 0x277, 0x294, 0x2aa
    };
    int *pResourceObject;
    unsigned char *pChunk;
    unsigned char *pEnd;
    char *pLevelMode;
    int nResourceId;

    if (nSkill > 4) {
        nSkill = 4;
    }
    if (nLevel == 9999) {
        nLevel = *(unsigned int *)((char *)pLoaderContext + 4);
    }
    nResourceId = anSkillResourceBases[nSkill] + (int)nLevel;
    pResourceObject = LoadBinResource(nResourceId);
    if (pResourceObject == 0) {
        return;
    }
    if (pResourceObject[4] == 0) {
        ((void (*)(void))*(void **)(*pResourceObject + 0x1c))();
    } else {
        pResourceObject[9] = 0;
    }
    ++pResourceObject[2];
    g_pLoadedLevelFileBuffer =
        ((void *(*)(void))*(void **)(*pResourceObject + 0x28))();
    pChunk = (unsigned char *)g_pLoadedLevelFileBuffer;
    pEnd = pChunk + pResourceObject[10];
    pLevelMode = *(char **)pLoaderContext;
    while (pChunk + 8 <= pEnd) {
        unsigned int nTag = *(unsigned int *)pChunk;
        unsigned int cbChunk = *(unsigned int *)(pChunk + 4);
        if (cbChunk < 8 || pChunk + cbChunk > pEnd) {
            break;
        }
        if (nTag == 0x47445346) {
            LoadLevelTileGridChunk(
                *(void **)(pLevelMode + 0x110),
                (const unsigned short *)(pChunk + 8));
        } else if (nTag == 0x41492020) {
            LoadLevelAiChunk(pLevelMode,
                (const unsigned short *)(pChunk + 8), cbChunk - 8);
        } else if (nTag == 0x464c4147) {
            LoadLevelFlagChunk(pLevelMode,
                (const unsigned short *)(pChunk + 8));
        } else if (nTag == 0x44454654) {
            SetLevelTileGridDefaultVariantIds(
                *(void **)(pLevelMode + 0x110),
                (const unsigned short *)(pChunk + 8));
        } else if (nTag == 0x4e414d45) {
            SetLevelTileGridName(*(void **)(pLevelMode + 0x110),
                                 (const char *)(pChunk + 8));
        } else if (nTag == 0x414e494d) {
            LoadAnimChunkRecords(*(void **)(pLevelMode + 0x194),
                                 (const unsigned short *)(pChunk + 8));
        } else if (nTag == 0x4e4f4445) {
            LoadNodeChunkRecords(*(void **)(pLevelMode + 0x17c),
                (const unsigned short *)(pChunk + 8), cbChunk - 8,
                fReuseState);
        }
        if (nTag == 0x454e443f) {
            break;
        }
        pChunk += (cbChunk + 3) & ~3U;
    }
    --pResourceObject[2];
    ReleaseTypedResourceObjectReference(pResourceObject);
    (void)fReuseState;
}

void *ConstructInvsChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeSimpleChunkManager(pObject, g_LINKSCF_InvsChunkManagerVtable, 0x20, 0x15);
    *(void **)((char *)pObject + 0x38) = pLevelMode;
    *(int *)((char *)pObject + 0x30) = nCapacity;
    *(int *)((char *)pObject + 0x3c) = 0;
    return pObject;
}

void *ConstructIceChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeSimpleChunkManager(pObject, g_LINKSCF_IceChunkManagerVtable, 0x19, 0x0e);
    *(void **)((char *)pObject + 0x3c) = pLevelMode;
    *(int *)((char *)pObject + 0x30) = nCapacity;
    *(int *)((char *)pObject + 0x38) = 0;
    return pObject;
}

void *ConstructPgunChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeSimpleChunkManager(pObject, g_LINKSCF_PgunChunkManagerVtable, 0x1f, 0x14);
    *(void **)((char *)pObject + 0x3c) = pLevelMode;
    *(int *)((char *)pObject + 0x30) = nCapacity;
    *(int *)((char *)pObject + 0x38) = 0;
    return pObject;
}

void *ConstructTramChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeSimpleChunkManager(pObject, g_LINKSCF_TramChunkManagerVtable, 0x18, 0x0d);
    *(void **)((char *)pObject + 0x3c) = pLevelMode;
    *(int *)((char *)pObject + 0x30) = nCapacity;
    *(int *)((char *)pObject + 0x38) = 0;
    return pObject;
}

void *ConstructBoonChunkManagerThunk(void *pObject, void *pLevelMode, void *pTileGrid) {
    int i;
    int j;

    for (i = 0; i < 4; ++i) {
        int *pPoint = (int *)((char *)pObject + 4 + i * 0x0c);
        pPoint[0] = 0xaa55aa55;
        pPoint[1] = 0xaa55aa55;
        pPoint[2] = 0xaa55aa55;
    }
    *(void **)((char *)pObject + 0x44) = pLevelMode;
    *(void **)((char *)pObject + 0x48) = pTileGrid;
    g_LINKSCF_BoonChunkObjectVtable[65] = (void *)NoopChunkStreamVirtual;
    for (i = 0; i < 4; ++i) {
        void *pChild = AllocateVSMemBlock(0x128);
        if (pChild != 0) {
            for (j = 0; j < 0x128 / 4; ++j) {
                ((int *)pChild)[j] = 0;
            }
            *(void **)pChild = g_LINKSCF_BoonChunkObjectVtable;
            *(int *)((char *)pChild + 4) = 0x28 + i * 2;
        }
        *(void **)((char *)pObject + 0x34 + i * 4) = pChild;
    }
    return pObject;
}

void *ConstructLasrChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeSimpleChunkManager(pObject, g_LINKSCF_LasrChunkManagerVtable, 0x15, 10);
    *(void **)((char *)pObject + 0x3c) = pLevelMode;
    *(int *)((char *)pObject + 0x30) = nCapacity;
    *(int *)((char *)pObject + 0x38) = 0;
    return pObject;
}

void *ConstructCaptureTriggerChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeSimpleChunkManager(pObject, g_LINKSCF_CaptureChunkManagerVtable, 0x16, 0x0b);
    *(void **)((char *)pObject + 0x3c) = pLevelMode;
    *(int *)((char *)pObject + 0x30) = nCapacity;
    *(int *)((char *)pObject + 0x38) = 0;
    return pObject;
}

void *ConstructRockChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeSimpleChunkManager(pObject, g_LINKSCF_RockChunkManagerVtable, 0x14, 9);
    *(void **)((char *)pObject + 0x3c) = pLevelMode;
    *(int *)((char *)pObject + 0x30) = nCapacity;
    *(int *)((char *)pObject + 0x38) = 0;
    return pObject;
}

void *ConstructDoorChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeSimpleChunkManager(pObject, g_LINKSCF_DoorChunkManagerVtable, 0x13, 8);
    *(void **)((char *)pObject + 0x30) = pLevelMode;
    *(int *)((char *)pObject + 0x38) = nCapacity;
    *(int *)((char *)pObject + 0x3c) = 0;
    return pObject;
}

void *ConstructMineChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeSimpleChunkManager(pObject, g_LINKSCF_MineChunkManagerVtable, 0x0d, 2);
    *(void **)((char *)pObject + 0x30) = pLevelMode;
    *(int *)((char *)pObject + 0x40) = nCapacity;
    *(int *)((char *)pObject + 0x34) = 0;
    *(int *)((char *)pObject + 0x38) = 0;
    return pObject;
}

void *ConstructCollChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeSimpleChunkManager(pObject, g_LINKSCF_CollChunkManagerVtable, 0x11, 6);
    *(void **)((char *)pObject + 0x30) = pLevelMode;
    *(int *)((char *)pObject + 0x34) = 0;
    *(int *)((char *)pObject + 0x38) = nCapacity;
    *(int *)((char *)pObject + 0x3c) = 0;
    return pObject;
}

void *ConstructEnmyChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable) {
    InitializeLinkScfChunkVtables();
    InitializeCachedChunkObjectManagerBase(pObject);
    *(void **)pObject = g_LINKSCF_EnmyChunkManagerVtable;
    (void)pLevelMode;
    (void)pGmobManager;
    (void)pProjectileGeometryTable;
    return pObject;
}

void *ConstructShpgChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable) {
    InitializeLinkScfChunkVtables();
    InitializeCachedChunkObjectManagerBase(pObject);
    *(void **)pObject = g_LINKSCF_ShpgChunkManagerVtable;
    (void)pLevelMode;
    (void)pGmobManager;
    (void)pProjectileGeometryTable;
    return pObject;
}

void *ConstructManagedEntityQueueCursorThunk(void *pObject, int nLevelMode, int nPrimaryCapacity, int nSecondaryCapacity) {
    void *pDispatchQueue;

    InitializeRenderQueueNodeBase(pObject);
    *(void **)pObject = g_LINKSCF_ManagedEntityQueueCursorVtable;
    *(int *)((char *)pObject + 0x20) = nLevelMode;
    *(int *)((char *)pObject + 0x18) = nPrimaryCapacity;
    *(int *)((char *)pObject + 0x1c) = nSecondaryCapacity;
    pDispatchQueue = *(void **)(nLevelMode + 0x164);
    *(void **)((char *)pObject + 0x24) = pDispatchQueue;
    ((GameRenderDispatchQueue *)pDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(pObject, 0);
    return pObject;
}

void *ConstructProjectilePoolThunk(void *pObject) {
    int i;

    InitializeLinkScfChunkVtables();
    InitializeEffChunkStreamBase(pObject, 0x21, 0x16);
    *(void **)pObject = g_LINKSCF_ProjectilePoolVtable;
    *(void **)((char *)pObject + 0x30) = 0;
    for (i = 0; i < 40; ++i) {
        *(void **)((char *)pObject + 0x34 + i * 4) = 0;
    }
    *(int *)((char *)pObject + 0xdc) = 0;
    return pObject;
}

void *ConstructPlasChunkManagerThunk(void *pObject, void *pLevelMode, void *pGmobManager, void *pProjectileGeometryTable) {
    int i;
    char *pStream;

    InitializeLinkScfChunkVtables();
    for (i = 0; i < 40; ++i) {
        ((int *)pObject)[i] = 0;
    }
    *(void **)pObject = g_LINKSCF_PlasChunkManagerVtable;
    *(void **)((char *)pObject + 0xa4) = 0;
    *(void **)((char *)pObject + 0xa8) = 0;
    *(int *)((char *)pObject + 0xac) = 0;
    pStream = (char *)pObject + 0xb0;
    InitializeEffChunkStreamBase(pStream, 0x22, 0x17);
    *(void **)pStream = g_LINKSCF_PlasChunkStreamVtable;
    *(int *)((char *)pObject + 0x128) = 0;
    *(int *)((char *)pObject + 0xe0) = 0x112;
    *(int *)((char *)pObject + 0xf0) = 0x34a;
    *(int *)((char *)pObject + 0x100) = 0;
    *(int *)((char *)pObject + 0x110) = 4;
    *(int *)((char *)pObject + 0x114) = 0;
    *(int *)((char *)pObject + 0x118) = 0;
    *(int *)((char *)pObject + 0x11c) = 0;
    *(int *)((char *)pObject + 0x124) = 1;
    *(int *)((char *)pObject + 0x14c) = 0;
    (void)pLevelMode;
    (void)pGmobManager;
    (void)pProjectileGeometryTable;
    return pObject;
}

void *ConstructGmobChunkManagerThunk(void *pObject, void *pLevelMode) {
    InitializeLinkScfChunkVtables();
    InitializeEffChunkStreamBase(pObject, 0x0c, 1);
    *(void **)pObject = g_LINKSCF_GmobChunkManagerVtable;
    *(void **)((char *)pObject + 0x30) = pLevelMode;
    *(short *)((char *)pObject + 0x34) = 0;
    *(short *)((char *)pObject + 0x36) = 0;
    *(short *)((char *)pObject + 0x38) = 0;
    *(int *)((char *)pObject + 0x3c) = 0;
    return pObject;
}

void *ConstructMoveChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeLinkScfChunkVtables();
    InitializeEffChunkStreamBase(pObject, 0x1a, 0x0f);
    *(void **)pObject = g_LINKSCF_MoveChunkManagerVtable;
    *(void **)((char *)pObject + 0x3c) = pLevelMode;
    *(int *)((char *)pObject + 0x30) = nCapacity;
    *(int *)((char *)pObject + 0x38) = 0;
    return pObject;
}

void *ConstructLiftChunkManagerThunk(void *pObject, void *pLevelMode, int nCapacity) {
    InitializeLinkScfChunkVtables();
    InitializeEffChunkStreamBase(pObject, 0x12, 7);
    *(void **)pObject = g_LINKSCF_LiftChunkManagerVtable;
    *(void **)((char *)pObject + 0x30) = pLevelMode;
    *(int *)((char *)pObject + 0x38) = nCapacity;
    *(int *)((char *)pObject + 0x3c) = 0;
    return pObject;
}

void *ConstructLevelChunkStreamDispatcherThunk(void *pObject, int nChunkTypeCount) {
    int i;

    InitializeLinkScfChunkVtables();
    InitializeRenderQueueNodeBase(pObject);
    *(void **)pObject = g_LINKSCF_LevelChunkStreamDispatcherVtable;
    *(int *)((char *)pObject + 0x14) = nChunkTypeCount;
    *(int *)((char *)pObject + 0x18) = 0;
    *(void **)((char *)pObject + 0x10) =
        AllocateVSMemBlock((unsigned int)(nChunkTypeCount * 4));
    *(int **)((char *)pObject + 0x1c) =
        (int *)AllocateVSMemBlock(0x60);
    for (i = 0; i < 24; ++i) {
        (*(int **)((char *)pObject + 0x1c))[i] = -1;
    }
    *(void **)((char *)pObject + 0x20) = 0;
    return pObject;
}

void SetGeometryChildEnabled(void *pGeometryChild, int fEnabled) {
    ((GameGeometryOwnerRectState *)pGeometryChild)->SetEnabled(fEnabled);
}
