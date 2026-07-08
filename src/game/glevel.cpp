#include "../game.h"

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
unsigned char g_abManagedEntitySlotClaimBitset[0x100] = {0};
unsigned char DAT_0049d108 = 1;

void *g_pLiftTileGrid = 0;
void *g_pAnimChunkTileGrid = 0;

static void *g_pReturnTrueVtableCallback = (void *)0x004932c8;
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
    pbBits = g_abManagedEntitySlotClaimBitset;
    for (i = 0x40; i != 0; --i) {
        pbBits[0] = 0;
        pbBits[1] = 0;
        pbBits[2] = 0;
        pbBits[3] = 0;
        pbBits += 4;
    }
    g_abManagedEntitySlotClaimBitset[0] |= DAT_0049d108;
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
