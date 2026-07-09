#include "../game/game_app.h"
#include "../engine/memory_arena.h"

extern "C" DWORD timeGetTime(void);

extern int g_nLevelFrameClockTick;
int g_nNetworkFrameClockLatestTimeMs = 0;
int g_nLevelFrameClockLastSampleMs = 0;
int g_nLevelFrameClockTimeMs = 0;
int g_nLevelFrameClockDemoTimeMs = 0;
int g_nLevelFrameClockTimeBaseMs = 0;
int g_nLevelFrameClockTickBase = 0;

int g_fLevelEndStateRequestLatched = 0;
void *g_pActiveManagedEntityOwner = 0;
static unsigned char *g_GAME_ManagedEntitySlotBitMasks = (unsigned char *)0x0049d108;
static unsigned char *g_GAME_ManagedEntitySlotClaimBitset = (unsigned char *)0x004a6410;
static unsigned short *g_GAME_ManagedEntityRegistryCount = (unsigned short *)0x004a74bc;
static int *g_GAME_ManagedEntityRegistryTable = (int *)0x004a6510;
static int *g_GAME_SelectedNetworkLobbyPeerId = (int *)0x004a011c;

void *g_pLiftTileGrid = 0;
void *g_pAnimChunkTileGrid = 0;

extern int ReturnTrueVtableCallback(void);

static void *g_pReturnTrueVtableCallback[1] = {
    (void *)ReturnTrueVtableCallback,
};
static void *g_pReturnTrueVtableCallbackThunk = (void *)0x00498610;
static void *g_pLevelGameStateStreamVtable = (void *)0x00493a00;

extern void ResetEffStreamStateFields(void *pEffStreamSubobject);

struct GAME_LevelChunkStreamDispatcher {
    int m_nRenderQueueNode00;
    int m_nRenderQueueTag04;
    int m_nRenderQueueReserved08;
    int m_nRenderQueueReserved0C;
    void **m_ppRegisteredStreams10;
    int m_cChunkTypes14;
    int m_cRegisteredStreams18;
    int *m_pChunkTypeToIndex1C;
};

struct GAME_LevelTileReachabilityHelper {
    int m_pTileGrid00;
    int m_ppRows04;
    int m_nReserved08;
    char m_abReserved0C[0x1000];
    int m_cColumns100C;
    int m_cRows1010;
    int m_nReserved1014;
};

struct GAME_LevelTileGrid {
    int m_nReserved00;
    int m_pReserved04;
    int m_cColumns08;
    int m_cRows0C;
    int m_pCells10;
};

void ClaimManagedEntitySlotId(int nManagedEntityObject);
unsigned short GetManagedEntitySlotId(int nManagedEntityObject);
void SetManagedEntitySlotId(void *pManagedEntityObject, unsigned short nSlotId);
unsigned int FindFirstFreeManagedEntitySlotIdForwardThunk(void);
unsigned int FindLastFreeManagedEntitySlotIdThunk(void);
void SetLevelChunkTypeEnabledFlagThunk(void *pLevelMode, int nChunkType, int fEnabled);
void DispatchType14CountedChildSlotC4Thunk(int nChunkManager);
void FillReachabilityGridFromTileFlagsThunk(void *pReachabilityHelper);

// FUNCTION: LEMBALL 0x0040CF00
void *ReturnArgumentVtableCallback(void *pObject) {
    return pObject;
}

// FUNCTION: LEMBALL 0x00408090
void ResetLevelFrameClockThunk(void) {
    g_nNetworkFrameClockLatestTimeMs = 0;
    g_nLevelFrameClockLastSampleMs = 0;
    g_nLevelFrameClockTimeMs = 0;
    g_nLevelFrameClockTick = 0;
    g_nLevelFrameClockDemoTimeMs = 0;
    g_nLevelFrameClockTimeBaseMs = (int)timeGetTime();
    g_nLevelFrameClockTickBase = g_nLevelFrameClockTimeBaseMs / 0x32;
}

// FUNCTION: LEMBALL 0x004165E0
void InitializeManagedEntitySlotTablesThunk(void *pLevelGameMode) {
    int i;
    unsigned char *pbBits;

    g_fLevelEndStateRequestLatched = 0;
    g_pActiveManagedEntityOwner = pLevelGameMode;
    pbBits = g_GAME_ManagedEntitySlotClaimBitset;
    for (i = 0x40; i != 0; --i) {
        pbBits[0] = 0;
        pbBits[1] = 0;
        pbBits[2] = 0;
        pbBits[3] = 0;
        pbBits += 4;
    }
    g_GAME_ManagedEntitySlotClaimBitset[0] |= g_GAME_ManagedEntitySlotBitMasks[0];
}

// FUNCTION: LEMBALL 0x00461310
void ClearLockedRecordSlotPayloadFlags(int *pTable) {
    int i;
    int nSlots;
    int nOffset;
    int *pRecordSlot;

    nOffset = 0;
    nSlots = 0;
    if (0 < pTable[0]) {
        do {
            nOffset += 4;
            ++nSlots;
            pRecordSlot = *(int **)((unsigned long)pTable[2] - 4 + nOffset);
            *(int *)((char *)pRecordSlot + 0x24) = 0;
        } while (nSlots < pTable[0]);
    }
}

// FUNCTION: LEMBALL 0x0045FCC0
void ClearPrimaryLockedRecordTablePayloadFlags(void *pPayload) {
    ClearLockedRecordSlotPayloadFlags(*(int **)((char *)pPayload + 0x48));
}

// FUNCTION: LEMBALL 0x0045FCD0
void ClearSecondaryLockedRecordTablePayloadFlags(void *pPayload) {
    ClearLockedRecordSlotPayloadFlags(*(int **)((char *)pPayload + 0x50));
}

// FUNCTION: LEMBALL 0x00452FE0
void *ConstructNetworkLevelChunkDeltaStreamThunk(void *pObject, int nOwner) {
    int nDispatcherPayload;

    *(void **)pObject = g_pReturnTrueVtableCallback;
    *(int *)((char *)pObject + 4) = 3;
    ResetEffStreamStateFields(pObject);
    *(void **)pObject = g_pReturnTrueVtableCallbackThunk;
    *(int *)((char *)pObject + 0x2c) = nOwner;
    *(int *)((char *)pObject + 0x30) = nOwner + 0x1d0;
    nDispatcherPayload = *(int *)((unsigned long)nOwner + 0x34);
    *(int *)((char *)pObject + 0x24) = 0;
    *(int *)((char *)pObject + 0x18) += nDispatcherPayload + 8;
    return pObject;
}

// FUNCTION: LEMBALL 0x00410B80
void *ConstructLevelGameStateStreamThunk(void *pObject) {
    int *pStream;

    pStream = (int *)pObject;
    *(void **)pStream = g_pReturnTrueVtableCallback;
    pStream[1] = 10;
    ResetEffStreamStateFields(pStream);
    *(void **)pStream = g_pLevelGameStateStreamVtable;
    pStream[6] += 0x10;
    pStream[9] = 1;
    return pObject;
}

// FUNCTION: LEMBALL 0x00430370
void ClearLevelTileGridCells(int nTileGridCells) {
    int y;
    int x;
    int *pCell;
    int *pTileGridFields;

    pTileGridFields = (int *)(unsigned long)nTileGridCells;
    y = 0;
    if (0 < pTileGridFields[3]) {
        do {
            for (x = 0; x < pTileGridFields[2]; ++x) {
                pCell = (int *)(unsigned long)((y * pTileGridFields[2] + x) * 0xc + pTileGridFields[1]);
                pCell[0] = 0x209;
                *(short *)(pCell + 1) = 0;
                *(short *)((char *)pCell + 6) = 0;
                *(short *)(pCell + 2) = 0;
                *(short *)((char *)pCell + 10) = 0;
            }
            ++y;
        } while (y < pTileGridFields[3]);
    }
}

// FUNCTION: LEMBALL 0x004303C0
void *ConstructLevelTileGridThunk(void *pObject) {
    *(int *)((char *)pObject + 0xc) = 0;
    *(int *)((char *)pObject + 0x10) = 0;
    *(int *)((char *)pObject + 0x14) = 0;
    g_pLiftTileGrid = pObject;
    g_pAnimChunkTileGrid = pObject;
    *(int *)((char *)pObject + 0x20) = 0;
    *(int *)((char *)pObject + 0x1c) = 0;
    *(int *)((char *)pObject + 0x48) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x004303F0
void InitializeLevelTileGridThunk(void *pTileGrid) {
    *(int *)((char *)pTileGrid + 4) = 0;
    *(unsigned char *)((char *)pTileGrid + 0x24) = 0;
    *(int *)((char *)pTileGrid + 0x4c) = 0x209;
    *(int *)((char *)pTileGrid + 0x50) = 0;
    ClearLevelTileGridCells((int)(unsigned long)pTileGrid + 8);
}

// FUNCTION: LEMBALL 0x00423090
void *ConstructLevelTileReachabilityHelperThunk(void *pObject, void *pTileGrid) {
    *(void **)pObject = pTileGrid;
    *(int *)((char *)pObject + 4) = 0;
    *(unsigned char *)((char *)pObject + 0x100c) = 0;
    *(int *)((char *)pObject + 8) = 0;
    *(int *)((char *)pObject + 0x1010) = 0;
    *(int *)((char *)pObject + 0x1014) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x004231a0
void ResizeLevelTileReachabilityGridThunk(void *pReachabilityHelper) {
    GAME_LevelTileReachabilityHelper *pHelper;
    GAME_LevelTileGrid *pTileGrid;
    int i;
    int cRows;
    int cAllocatedRows;
    int **ppRows;

    pHelper = (GAME_LevelTileReachabilityHelper *)pReachabilityHelper;
    pTileGrid = (GAME_LevelTileGrid *)(unsigned long)pHelper->m_pTileGrid00;
    i = 0;
    if (pHelper->m_ppRows04 != 0) {
        ppRows = (int **)(unsigned long)(pHelper->m_ppRows04 - 4);
        if (0 < pHelper->m_cRows1010) {
            cAllocatedRows = 0;
            do {
                ++ppRows;
                ++cAllocatedRows;
                FreeVSMemBlock(*ppRows);
            } while (cAllocatedRows < pHelper->m_cRows1010);
        }
        FreeVSMemBlock((void *)(unsigned long)pHelper->m_ppRows04);
    }

    pHelper->m_cColumns100C = pTileGrid->m_cColumns08;
    cRows = pTileGrid->m_cRows0C;
    i = 0;
    cAllocatedRows = 0;
    pHelper->m_cRows1010 = cRows;
    pHelper->m_ppRows04 = (int)(unsigned long)AllocateVSMemBlock((unsigned int)cRows << 2);
    ppRows = (int **)(unsigned long)(pHelper->m_ppRows04 - 4);
    if (0 < pHelper->m_cRows1010) {
        do {
            ++ppRows;
            ++cAllocatedRows;
            *ppRows = (int *)AllocateVSMemBlock((unsigned int)pHelper->m_cColumns100C * 2);
        } while (cAllocatedRows < pHelper->m_cRows1010);
    }
    FillReachabilityGridFromTileFlagsThunk(pReachabilityHelper);
}

// FUNCTION: LEMBALL 0x004230f0
void FillReachabilityGridFromTileFlagsThunk(void *pReachabilityHelper) {
    GAME_LevelTileReachabilityHelper *pHelper;
    GAME_LevelTileGrid *pTileGrid;
    int nTileFlags;
    unsigned short nCellValue;
    int y;
    int x;
    unsigned short *pReachabilityRow;

    pHelper = (GAME_LevelTileReachabilityHelper *)pReachabilityHelper;
    pTileGrid = (GAME_LevelTileGrid *)(unsigned long)pHelper->m_pTileGrid00;
    y = 0;
    if (0 < pHelper->m_cRows1010) {
        do {
            x = 0;
            pReachabilityRow = (unsigned short *)(unsigned long)(*(int *)(pHelper->m_ppRows04 + y * 4));
            if (0 < pHelper->m_cColumns100C) {
                do {
                    if (x < 0 || y < 0) {
                        nCellValue = 3;
                    } else {
                        nTileFlags = pHelper->m_pTileGrid00;
                        if (pTileGrid->m_cColumns08 <= x || pTileGrid->m_cRows0C <= y) {
                            nCellValue = 3;
                        } else {
                            nCellValue = *(unsigned short *)(pTileGrid->m_pCells10 + 6 +
                                                             (y * pTileGrid->m_cColumns08 + x) * 0xc);
                        }
                    }

                    if ((nCellValue & 1) == 0) {
                        pReachabilityRow[x] = 0xff00;
                    } else {
                        pReachabilityRow[x] = 0xffff;
                    }
                    ++x;
                } while (x < pHelper->m_cColumns100C);
            }
            ++y;
        } while (y < pHelper->m_cRows1010);
    }
}

// FUNCTION: LEMBALL 0x0040B120
void ResetRegisteredLevelChunkStreamsThunk(void *pDispatcher) {
    GAME_LevelChunkStreamDispatcher *pStreamDispatcher;
    int *pStream;
    int i;
    int nOffset;

    pStreamDispatcher = (GAME_LevelChunkStreamDispatcher *)pDispatcher;
    nOffset = 0;
    i = 0;
    if (0 < pStreamDispatcher->m_cRegisteredStreams18) {
        do {
            pStream = *(int **)((unsigned long)pStreamDispatcher->m_ppRegisteredStreams10 + nOffset);
            if (pStream != 0) {
                ((void (*)())*(void **)((unsigned long)*pStream + 0x18))();
            }
            nOffset += 4;
            ++i;
        } while (i < pStreamDispatcher->m_cRegisteredStreams18);
    }
}

// FUNCTION: LEMBALL 0x0040B150
void RegisterLevelChunkStreamThunk(void *pDispatcher, void *pStreamObject) {
    GAME_LevelChunkStreamDispatcher *pStreamDispatcher;
    int nVtable;
    int *pStream;

    pStreamDispatcher = (GAME_LevelChunkStreamDispatcher *)pDispatcher;
    pStream = (int *)pStreamObject;
    pStreamDispatcher->m_pChunkTypeToIndex1C[pStream[0xb]] = pStreamDispatcher->m_cRegisteredStreams18;
    nVtable = *pStream;
    pStreamDispatcher->m_ppRegisteredStreams10[pStreamDispatcher->m_cRegisteredStreams18] = pStreamObject;
    ++pStreamDispatcher->m_cRegisteredStreams18;
    ((void (*)())*(void **)((unsigned long)nVtable + 0x18))();
}

// FUNCTION: LEMBALL 0x00441B90
int GetSelectedLevelNumberThunk(void *pLevelProgressState) {
    return *(int *)((char *)pLevelProgressState + 0x3c + *(int *)((char *)pLevelProgressState + 8) * 4);
}

// FUNCTION: LEMBALL 0x00416610
unsigned short GetManagedEntitySlotId(int nManagedEntityObject) {
    return *(unsigned short *)(nManagedEntityObject + 0x6c);
}

// FUNCTION: LEMBALL 0x00416740
void ClaimManagedEntitySlotId(int nManagedEntityObject) {
    unsigned char bClaimByte;
    unsigned short nSlotId;
    unsigned int i;
    unsigned int cManagedEntities;

    nSlotId = *(unsigned short *)(nManagedEntityObject + 0x6c);
    if (nSlotId != 0xffff) {
        bClaimByte = g_GAME_ManagedEntitySlotClaimBitset[nSlotId >> 3];
        if ((g_GAME_ManagedEntitySlotBitMasks[nSlotId & 7] & bClaimByte) != 0) {
            cManagedEntities = (unsigned int)*g_GAME_ManagedEntityRegistryCount;
            for (i = 0; (int)i < (int)cManagedEntities; ++i) {
                if (g_GAME_ManagedEntityRegistryTable[i & 0xffff] != 0) {
                    GetManagedEntitySlotId(g_GAME_ManagedEntityRegistryTable[i & 0xffff]);
                }
            }
            *(unsigned short *)(nManagedEntityObject + 0x6c) = 0xffff;
            return;
        }
        g_GAME_ManagedEntitySlotClaimBitset[nSlotId >> 3] = g_GAME_ManagedEntitySlotBitMasks[nSlotId & 7] | bClaimByte;
    }
}

// FUNCTION: LEMBALL 0x00416620
void SetManagedEntitySlotId(void *pManagedEntityObject, unsigned short nSlotId) {
    *(unsigned short *)((char *)pManagedEntityObject + 0x6c) = nSlotId;
    ClaimManagedEntitySlotId((int)(unsigned long)pManagedEntityObject);
}

// FUNCTION: LEMBALL 0x00416670
unsigned int FindFirstFreeManagedEntitySlotIdForwardThunk(void) {
    unsigned int iByte;
    int iBit;

    iByte = 0;
    do {
        if (g_GAME_ManagedEntitySlotClaimBitset[iByte] != 0xff) {
            iBit = 0;
            do {
                if ((g_GAME_ManagedEntitySlotBitMasks[iBit] & g_GAME_ManagedEntitySlotClaimBitset[iByte]) == 0) {
                    return (unsigned int)(iBit + iByte * 8);
                }
                ++iBit;
            } while (iBit < 8);
        }
        ++iByte;
    } while ((int)iByte < 0x100);
    return iByte & 0xffff0000;
}

// FUNCTION: LEMBALL 0x004166A0
unsigned int FindLastFreeManagedEntitySlotIdThunk(void) {
    unsigned int iByte;
    int iBit;

    iByte = 0xff;
    do {
        if (g_GAME_ManagedEntitySlotClaimBitset[iByte] != 0xff) {
            iBit = 7;
            do {
                if ((g_GAME_ManagedEntitySlotBitMasks[iBit] & g_GAME_ManagedEntitySlotClaimBitset[iByte]) == 0) {
                    return (unsigned int)(iBit + iByte * 8);
                }
                --iBit;
            } while (0 < iBit);
        }
        --iByte;
    } while (0 < (int)iByte);
    return iByte & 0xffff0000;
}

// FUNCTION: LEMBALL 0x004130A0
void SetLevelChunkTypeEnabledFlagThunk(void *pLevelMode, int nChunkType, int fEnabled) {
    if (nChunkType < 0x211 || 0x212 < nChunkType) {
        *(int *)(*(int *)((char *)pLevelMode + 0x50) + nChunkType * 4) = fEnabled;
    }
}

// FUNCTION: LEMBALL 0x004031BB
void DispatchType14CountedChildSlotC4Thunk(int nChunkManager) {
    int *pChildObject;
    int i;
    int nOffset;

    nOffset = 0;
    i = 0;
    if (*(short *)(nChunkManager + 0x36) != 0) {
        do {
            pChildObject = *(int **)(*(int *)(nChunkManager + 0x3c) + nOffset);
            if (pChildObject[0x19] == 0x14) {
                ((void (*)())*(void **)(*pChildObject + 0xc4))();
            }
            nOffset += 4;
            ++i;
        } while (i < (int)(unsigned int)*(unsigned short *)(nChunkManager + 0x36));
    }
}

// FUNCTION: LEMBALL 0x00412FB0
void AssignMissingManagedEntitySlotIdsThunk(int nLevelMode) {
    void *pManagedEntityObject;
    unsigned short nSlotId;
    unsigned int nFreeSlotId;
    unsigned int i;
    unsigned int cManagedEntities;

    cManagedEntities = (unsigned int)*g_GAME_ManagedEntityRegistryCount;
    i = 0;
    if (cManagedEntities != 0) {
        do {
            pManagedEntityObject = (void *)(unsigned long)g_GAME_ManagedEntityRegistryTable[i & 0xffff];
            nSlotId = GetManagedEntitySlotId((int)(unsigned long)pManagedEntityObject);
            if (nSlotId == 0xffff) {
                nFreeSlotId = FindFirstFreeManagedEntitySlotIdForwardThunk();
                SetManagedEntitySlotId(pManagedEntityObject, (unsigned short)nFreeSlotId);
            }
            ++i;
        } while ((int)i < (int)cManagedEntities);
    }
    DispatchType14CountedChildSlotC4Thunk(*(int *)(nLevelMode + 0x158));
}

// FUNCTION: LEMBALL 0x00412100
void InitializeLevelChunkTypeEnabledFlagsThunk(void *pLevelMode) {
    int nChunkType;
    unsigned int i;
    unsigned int cManagedEntities;

    cManagedEntities = (unsigned int)*g_GAME_ManagedEntityRegistryCount;
    for (i = 0; (int)i < (int)cManagedEntities; ++i) {
        if (g_GAME_ManagedEntityRegistryTable[i & 0xffff] != 0) {
            nChunkType = *(int *)(g_GAME_ManagedEntityRegistryTable[i & 0xffff] + 100);
            if (nChunkType != 0xffff) {
                SetLevelChunkTypeEnabledFlagThunk(pLevelMode, nChunkType, 1);
            }
        }
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 0x180) + 8) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 9, 0);
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 400) + 0x34) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x1a, 0);
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x19, 0);
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 0x1a0) + 0x34) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x2f, 0);
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x1e, 0);
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 0x188) + 0x3c) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x13, 0);
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 0x198) + 0x34) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x20, 0);
    }
    if (*(int *)(*(int *)((char *)pLevelMode + 0x1c8) + 0xc) == 0) {
        SetLevelChunkTypeEnabledFlagThunk(pLevelMode, 0x35, 0);
    }
}

// FUNCTION: LEMBALL 0x00412080
void AssignNetworkPlayerManagedEntitySlotIdsThunk(int nLevelMode) {
    unsigned int nSlotId;
    int i;
    int *pOffsets;
    void **ppManagedEntities;
    int anOffsets[2];

    if (*g_GAME_SelectedNetworkLobbyPeerId != 0) {
        anOffsets[0] = 0;
        anOffsets[1] = 0;
        if (*(int *)(*g_GAME_SelectedNetworkLobbyPeerId + 0x1c) == 0) {
            anOffsets[1] = 4;
        } else {
            anOffsets[0] = 4;
        }

        pOffsets = anOffsets;
        do {
            i = 4;
            ppManagedEntities = (void **)(nLevelMode + 0x1d0 + *pOffsets * 4);
            do {
                nSlotId = FindLastFreeManagedEntitySlotIdThunk();
                SetManagedEntitySlotId(*ppManagedEntities, (unsigned short)nSlotId);
                --i;
                ++ppManagedEntities;
            } while (i != 0);
            ++pOffsets;
        } while (pOffsets < &anOffsets[2]);
    }
}

// FUNCTION: LEMBALL 0x0043D820
void ResetBallChunkEntriesThunk(void *pBallManager) {
    int i;
    int cEntries;

    cEntries = 0;
    if (*(int *)((char *)pBallManager + 4) != 0 && 0 < *(int *)((char *)pBallManager + 0xc)) {
        i = 0;
        do {
            cEntries += 4;
            ++i;
            ((void (*)())*(void **)(**(int **)(*(int *)((char *)pBallManager + 4) - 4 + cEntries) + 0x104))();
        } while (i < *(int *)((char *)pBallManager + 0xc));
    }
}

// FUNCTION: LEMBALL 0x0043E4A0
void ResetBoonChunkManagerObjectsThunk(void *pObject) {
    unsigned short *pState;

    pState = (unsigned short *)pObject;
    *pState = 0;
    ((void (*)())*(void **)(**(int **)(pState + 0x1a) + 0x104))();
    ((void (*)())*(void **)(**(int **)(pState + 0x1c) + 0x104))();
    ((void (*)())*(void **)(**(int **)(pState + 0x1e) + 0x104))();
    ((void (*)())*(void **)(**(int **)(pState + 0x20) + 0x104))();
}
