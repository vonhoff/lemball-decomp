#include "../game/game_app.h"
#include "../engine/memory_arena.h"

static void *g_NETWORK_FileBackedDualChannelStateVtable = (void *)0x0049A558;
static void *g_NETWORK_FileBackedDualStreamVtable = (void *)0x0049A550;
static void *g_NETWORK_FileBackedDualThunkVtable = (void *)0x0049A540;
static void *g_NETWORK_FileBackedDualPrimaryThunkVtable = (void *)0x0049A5D8;
static void *g_NETWORK_FileBackedDualSecondaryThunkVtable = (void *)0x0049A5B0;
static void *g_NETWORK_FileBackedDualFileThunkVtable = (void *)0x0049A570;
static void *g_NETWORK_FileBackedDualFatalThunkVtable = (void *)0x0049A56C;

static void *g_NETWORK_FileBackedTimedChannelStateVtable = (void *)0x0049A680;
static void *g_NETWORK_FileBackedTimedStreamVtable = (void *)0x0049A678;
static void *g_NETWORK_FileBackedTimedThunkVtable = (void *)0x0049A668;
static void *g_NETWORK_FileBackedTimedPrimaryThunkVtable = (void *)0x0049A708;
static void *g_NETWORK_FileBackedTimedSecondaryThunkVtable = (void *)0x0049A6D8;
static void *g_NETWORK_FileBackedTimedFileThunkVtable = (void *)0x0049A698;
static void *g_NETWORK_FileBackedTimedFatalThunkVtable = (void *)0x0049A694;

static void *g_NETWORK_EmbeddedFileBackedTimedStreamVtable = (void *)0x0049A770;
static void *g_NETWORK_EmbeddedFileBackedDualStreamVtable = (void *)0x0049A768;
static void *g_NETWORK_EmbeddedFileBackedDualThunkVtable = (void *)0x0049A758;
static void *g_NETWORK_EmbeddedFileBackedFileWrapperThunkVtable = (void *)0x0049A748;
static void *g_NETWORK_EmbeddedFileBackedBaseVtable = (void *)0x0049A778;
static void *g_NETWORK_EmbeddedFileBackedPrimaryThunkVtable = (void *)0x0049A730;
static void *g_NETWORK_EmbeddedFileBackedTimedFileViewVtable = (void *)0x0049A718;
static void *g_NETWORK_EmbeddedFileBackedFinalPrimaryThunkVtable = (void *)0x0049A838;
static void *g_NETWORK_EmbeddedFileBackedFinalTimedThunkVtable = (void *)0x0049A808;
static void *g_NETWORK_EmbeddedFileBackedFinalDualThunkVtable = (void *)0x0049A7E0;
static void *g_NETWORK_EmbeddedFileBackedFinalFileThunkVtable = (void *)0x0049A7A0;
static void *g_NETWORK_EmbeddedFileBackedFinalFatalThunkVtable = (void *)0x0049A79C;

static void *g_NETWORK_GlobalStateCompositeTimedStreamVtable = (void *)0x0049A900;
static void *g_NETWORK_GlobalStateCompositeTransportVtable = (void *)0x0049A8E8;
static void *g_NETWORK_GlobalStateCompositeDualStreamVtable = (void *)0x0049A8E0;
static void *g_NETWORK_GlobalStateCompositeDualThunkVtable = (void *)0x0049A8D8;
static void *g_NETWORK_GlobalStateCompositeFileWrapperVtable = (void *)0x0049A8C8;
static void *g_NETWORK_GlobalStateCompositeTimedFileThunkVtable = (void *)0x0049A8B8;
static void *g_NETWORK_GlobalStateCompositeEmbeddedStackVtable = (void *)0x0049A8A0;
static void *g_NETWORK_GlobalStateCompositeEmbeddedStackDataVtable = (void *)0x0049A888;
static void *g_NETWORK_GlobalStateCompositeEmbeddedStackBaseVtable = (void *)0x0049A860;
static void *g_NETWORK_GlobalStateCompositeInitialVtable = (void *)0x0049A848;
static void *g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable = (void *)0x0049AA90;
static void *g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable = (void *)0x0049AA60;
static void *g_NETWORK_GlobalStateCompositeInitialDualThunkVtable = (void *)0x0049AA38;
static void *g_NETWORK_GlobalStateCompositeInitialFileThunkVtable = (void *)0x0049A9F8;
static void *g_NETWORK_GlobalStateCompositeInitialFatalThunkVtable = (void *)0x0049A9F4;
static void *g_NETWORK_GlobalStateCompositeFinalVtable = (void *)0x0049A9D8;
static void *g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable = (void *)0x0049A9C8;
static void *g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable = (void *)0x0049A998;
static void *g_NETWORK_GlobalStateCompositeFinalDualThunkVtable = (void *)0x0049A970;
static void *g_NETWORK_GlobalStateCompositeFinalFileThunkVtable = (void *)0x0049A930;
static void *g_NETWORK_GlobalStateCompositeFinalFatalThunkVtable = (void *)0x0049A928;

static void *g_NETWORK_DualSlotCompositeTimedStreamVtable = (void *)0x0049AB40;
static void *g_NETWORK_DualSlotCompositeTransportVtable = (void *)0x0049AB28;
static void *g_NETWORK_DualSlotCompositeDualStreamVtable = (void *)0x0049AB20;
static void *g_NETWORK_DualSlotCompositeDualThunkVtable = (void *)0x0049AB18;
static void *g_NETWORK_DualSlotCompositeFileWrapperVtable = (void *)0x0049AB08;
static void *g_NETWORK_DualSlotCompositeTimedFileThunkVtable = (void *)0x0049AAF8;
static void *g_NETWORK_DualSlotCompositeEmbeddedStackVtable = (void *)0x0049AAE0;
static void *g_NETWORK_DualSlotCompositeEmbeddedStackDataVtable = (void *)0x0049AAC8;
static void *g_NETWORK_DualSlotCompositeEmbeddedStackBaseVtable = (void *)0x0049AAA0;
static void *g_NETWORK_DualSlotCompositeFinalVtable = (void *)0x0049AC18;
static void *g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable = (void *)0x0049AC08;
static void *g_NETWORK_DualSlotCompositeFinalTimedThunkVtable = (void *)0x0049ABD8;
static void *g_NETWORK_DualSlotCompositeFinalDualThunkVtable = (void *)0x0049ABB0;
static void *g_NETWORK_DualSlotCompositeFinalFileThunkVtable = (void *)0x0049AB70;
static void *g_NETWORK_DualSlotCompositeFinalFatalThunkVtable = (void *)0x0049AB68;

static void *g_NETWORK_EffStateStreamVtable = (void *)0x0049A4D8;
static void *g_NETWORK_EffRecordSlotTableVtable = (void *)0x0049A4C0;
static void *g_NETWORK_EffRecordSlotVtable = (void *)0x0049A4A8;

struct NETWORK_EffStreamRecordSlot {
    void *m_pVtable;
    int m_nReserved04;
    char m_abStreamState08[0x24];
    unsigned short m_wCommittedMarker;
    unsigned short m_wObservedMarker;
    DWORD m_cbPayload;
    char m_szSourceName[0x15];
    char m_szTargetName[0x17];
};

struct NETWORK_EffStreamRecordSlotTable {
    char m_abBase00[0x2c];
    NETWORK_EffStreamRecordSlot *m_pSlots;
    unsigned short *m_pwSlotFlags;
    int m_cSlots;
};

void *g_pGlobalStateEff512ByteStream = 0;
void *g_pEffTransportPacketBuffer = 0;
int g_cbEffTransportCurrentPacketBytes = 0;

extern void *g_pEffTransportRuntimeService;
extern void *g_pEffTransportPeerAddressState;
extern void *g_pActiveNetworkRuntimeWindow;
extern char *g_pszFileBasedNetworkLocalHostName;
extern char *g_pszFileBasedNetworkConfiguredPath;
extern int g_cbEffTransportMaxPacketBytes;

extern void ResetEffStreamStateFields(void *pEffStreamSubobject);
extern void *ConstructEffStreamChannelState(void *pChannelState);
extern void DestroyEffStreamChannelState(void *pChannelState);
extern void *ConstructTimedEffStream(void *pStream, int fConstructChannelState);
extern void DestroyTimedEffStream(void *pStream);
extern void *ConstructDualHandleEffStream(void *pStream, int fConstructChannelState);
extern void DestroyDualHandleEffStream(void *pStream);
extern void *ConstructEffTransportRuntimeChannelStack(void *pChannelStack, int fConstructEmbeddedObjects);
extern void ReleaseEffTransportRuntimeBuffers(int nChannelStateBase);
extern void *ConstructCompositeEffTransportStack(void *pObject, int fConstructEmbeddedObjects);
extern void RestoreCompositeEffTransportVtables(int nObjectBasePlus0x30);
extern void DestroyEffStreamBase(void *pStream);
extern DWORD WriteEffStreamToLockedFile(void *pFileWrapper, int *pStream, int fWritePayload, int fFlush);
extern int WriteWin32FileWrapper(void *pFileWrapper, const void *pvSource, DWORD cbWrite);
extern int ReadWin32FileWrapper(void *pFileWrapper, LPVOID pvTarget, DWORD cbRead);
extern int LockWin32FileRange(void *pFileWrapper, DWORD dwOffset, DWORD cbRange);
extern int UnlockWin32FileRange(void *pFileWrapper, DWORD dwOffset, DWORD cbRange);
extern void ScheduleNetworkRuntimeTimerEvent(void *pRuntimeWindow, int nTicks);
extern int LoadEffStreamFromMemory(void *pStream, int nSourceBuffer);
extern void ProcessEffTransportPacketHeader(int *pStream);

extern "C" DWORD timeGetTime(void);

// FUNCTION: LEMBALL 0x00479920
void *ClearFileBackedEffChannelStateWord(void *pObject) {
    *(int *)pObject = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479540
void *InitializeEffStreamRecordSlot(void *pSlot) {
    NETWORK_EffStreamRecordSlot *pRecordSlot;
    int *pWords;

    pRecordSlot = (NETWORK_EffStreamRecordSlot *)pSlot;
    pWords = (int *)pSlot;
    pWords[0] = (int)(unsigned long)0x004932C8;
    pWords[1] = 0;
    ResetEffStreamStateFields(pSlot);
    pRecordSlot->m_wObservedMarker = 1;
    pWords[6] = pWords[6] + 0x30;
    pRecordSlot->m_wCommittedMarker = 0;
    pRecordSlot->m_pVtable = g_NETWORK_EffRecordSlotVtable;
    pRecordSlot->m_cbPayload = 0;
    pRecordSlot->m_szTargetName[0] = '\0';
    pRecordSlot->m_szSourceName[0] = '\0';
    return pSlot;
}

// FUNCTION: LEMBALL 0x00479620
void *ConstructEffStreamRecordSlotTable(void *pObject, int cSlots) {
    NETWORK_EffStreamRecordSlotTable *pTable;
    int *pSlotStorage;
    NETWORK_EffStreamRecordSlot *pSlot;
    int i;
    int nFlagOffset;

    pTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    *(int *)pObject = 0x004932C8;
    *(int *)((char *)pObject + 4) = 0;
    ResetEffStreamStateFields(pObject);
    pTable->m_cSlots = cSlots;
    *(void **)pObject = g_NETWORK_EffRecordSlotTableVtable;

    pSlotStorage = (int *)AllocateVSMemBlock(cSlots * 0x60 + 4);
    if (pSlotStorage == 0) {
        pTable->m_pSlots = 0;
    } else {
        *pSlotStorage = cSlots;
        pSlot = (NETWORK_EffStreamRecordSlot *)(pSlotStorage + 1);
        i = cSlots;
        while (--i >= 0) {
            InitializeEffStreamRecordSlot(pSlot);
            ++pSlot;
        }
        pTable->m_pSlots = (NETWORK_EffStreamRecordSlot *)(pSlotStorage + 1);
    }

    pTable->m_pwSlotFlags = (unsigned short *)AllocateVSMemBlock((unsigned int)(pTable->m_cSlots * 2));
    nFlagOffset = 0;
    i = 0;
    if (0 < pTable->m_cSlots) {
        do {
            nFlagOffset += 2;
            *(unsigned short *)((char *)pTable->m_pwSlotFlags + nFlagOffset - 2) = 0;
            ++i;
        } while (i < pTable->m_cSlots);
    }

    *(int *)((char *)pObject + 0x18) =
        *(int *)((char *)pObject + 0x18) + *(int *)((char *)&pTable->m_pSlots[0] + 0x18) * cSlots;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479810
void *ConstructEff512ByteStateStream(void *pObject) {
    int i;
    int nPayload;
    int *pWords;

    pWords = (int *)pObject;
    pWords[0] = 0x004932C8;
    i = 0;
    pWords[1] = 0;
    ResetEffStreamStateFields(pObject);
    pWords[0] = (int)(unsigned long)g_NETWORK_EffStateStreamVtable;
    nPayload = (int)(unsigned long)AllocateVSMemBlock(0x200);
    pWords[0xb] = nPayload;
    pWords[6] = pWords[6] + 0x200;
    do {
        ++i;
        *(unsigned char *)(nPayload - 1 + i) = 0;
    } while (i < 0x200);
    return pObject;
}

// FUNCTION: LEMBALL 0x00479930
void *ConstructDualFileBackedEffChannel(void *pObject, int fConstructEmbeddedObjects) {
    int nThunkOffset;

    if (fConstructEmbeddedObjects != 0) {
        *(void **)((char *)pObject + 0xc) = g_NETWORK_FileBackedDualChannelStateVtable;
        *(void **)((char *)pObject + 0x88) = g_NETWORK_FileBackedDualStreamVtable;
        *(void **)((char *)pObject + 0xb4) = g_NETWORK_FileBackedDualThunkVtable;
        ConstructEffStreamChannelState((char *)pObject + 0x18);
        ConstructDualHandleEffStream((char *)pObject + 0x44, 0);
        *(void **)((char *)pObject + 0x9c) = (void *)0x00498070;
        *(int *)((char *)pObject + 0xa0) = 0;
        *(int *)((char *)pObject + 0xa4) = 1;
        *(void **)((char *)pObject + 0x9c) = (void *)0x0049A628;
        *(void **)((char *)pObject + 0xb0) = (void *)0x0049A53C;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0xb4) + 4) + 0xb4 + (int)(unsigned long)pObject) =
            (void *)0x0049A500;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0xb4) + 8) + 0xb4 + (int)(unsigned long)pObject) =
            (void *)0x0049A4F0;
    }

    ClearFileBackedEffChannelStateWord(pObject);
    *(void **)(*(int *)(*(int *)((char *)pObject + 0xc) + 4) + 0xc + (int)(unsigned long)pObject) =
        g_NETWORK_FileBackedDualPrimaryThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0xc) + 8) + 0xc + (int)(unsigned long)pObject) =
        g_NETWORK_FileBackedDualSecondaryThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0xc) + 0xc) + 0xc + (int)(unsigned long)pObject) =
        g_NETWORK_FileBackedDualFileThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0xc) + 0x10) + 0xc + (int)(unsigned long)pObject) =
        g_NETWORK_FileBackedDualFatalThunkVtable;

    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0xc) + 4);
    *(int *)(nThunkOffset + 8 + (int)(unsigned long)pObject) = nThunkOffset - 0xc;
    *(int *)((char *)pObject + 0x10) = -1;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479E20
void *ConstructTimedFileBackedEffChannel(void *pObject, int fConstructEmbeddedObjects) {
    int nThunkOffset;

    if (fConstructEmbeddedObjects != 0) {
        *(void **)((char *)pObject + 0xc) = g_NETWORK_FileBackedTimedChannelStateVtable;
        *(void **)((char *)pObject + 0x8c) = g_NETWORK_FileBackedTimedStreamVtable;
        *(void **)((char *)pObject + 0xd4) = g_NETWORK_FileBackedTimedThunkVtable;
        ConstructEffStreamChannelState((char *)pObject + 0x18);
        ConstructTimedEffStream((char *)pObject + 0x48, 0);
        *(void **)((char *)pObject + 0xbc) = (void *)0x00498070;
        *(int *)((char *)pObject + 0xc0) = 0;
        *(int *)((char *)pObject + 0xc4) = 1;
        *(void **)((char *)pObject + 0xbc) = (void *)0x0049A628;
        *(void **)((char *)pObject + 0xd0) = (void *)0x0049A53C;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0xd4) + 4) + 0xd4 + (int)(unsigned long)pObject) =
            (void *)0x0049A500;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0xd4) + 8) + 0xd4 + (int)(unsigned long)pObject) =
            (void *)0x0049A4F0;
    }

    ClearFileBackedEffChannelStateWord(pObject);
    *(void **)(*(int *)(*(int *)((char *)pObject + 0xc) + 4) + 0xc + (int)(unsigned long)pObject) =
        g_NETWORK_FileBackedTimedPrimaryThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0xc) + 8) + 0xc + (int)(unsigned long)pObject) =
        g_NETWORK_FileBackedTimedSecondaryThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0xc) + 0xc) + 0xc + (int)(unsigned long)pObject) =
        g_NETWORK_FileBackedTimedFileThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0xc) + 0x10) + 0xc + (int)(unsigned long)pObject) =
        g_NETWORK_FileBackedTimedFatalThunkVtable;

    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0xc) + 4);
    *(int *)(nThunkOffset + 8 + (int)(unsigned long)pObject) = nThunkOffset - 0xc;
    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0xc) + 8);
    *(int *)(nThunkOffset + 8 + (int)(unsigned long)pObject) = nThunkOffset - 0x3c;
    *(int *)((char *)pObject + 0x10) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479F40
void RestoreTimedFileBackedEffChannelVtables(int nTimedFileBasePlus0x0c) {
    int nThunkOffset;

    *(void **)(*(int *)(*(int *)(nTimedFileBasePlus0x0c - 0xc) + 4) + nTimedFileBasePlus0x0c - 0xc) =
        g_NETWORK_FileBackedTimedPrimaryThunkVtable;
    *(void **)(*(int *)(*(int *)(nTimedFileBasePlus0x0c - 0xc) + 8) + nTimedFileBasePlus0x0c - 0xc) =
        g_NETWORK_FileBackedTimedSecondaryThunkVtable;
    *(void **)(*(int *)(*(int *)(nTimedFileBasePlus0x0c - 0xc) + 0xc) + nTimedFileBasePlus0x0c - 0xc) =
        g_NETWORK_FileBackedTimedFileThunkVtable;
    *(void **)(*(int *)(*(int *)(nTimedFileBasePlus0x0c - 0xc) + 0x10) + nTimedFileBasePlus0x0c - 0xc) =
        g_NETWORK_FileBackedTimedFatalThunkVtable;

    nThunkOffset = *(int *)(*(int *)(nTimedFileBasePlus0x0c - 0xc) + 4);
    *(int *)(nThunkOffset + nTimedFileBasePlus0x0c - 0x10) = nThunkOffset - 0xc;
    nThunkOffset = *(int *)(*(int *)(nTimedFileBasePlus0x0c - 0xc) + 8);
    *(int *)(nThunkOffset + nTimedFileBasePlus0x0c - 0x10) = nThunkOffset - 0x3c;
}

// FUNCTION: LEMBALL 0x0047A220
void *ConstructEmbeddedFileBackedEffChannelStack(void *pObject, int fConstructEmbeddedObjects) {
    int nThunkOffset;

    if (fConstructEmbeddedObjects != 0) {
        *(void **)((char *)pObject + 0x7c) = g_NETWORK_EmbeddedFileBackedTimedStreamVtable;
        *(void **)((char *)pObject + 0xf4) = g_NETWORK_EmbeddedFileBackedDualStreamVtable;
        *(void **)((char *)pObject + 0x108) = g_NETWORK_EmbeddedFileBackedDualThunkVtable;
        *(void **)((char *)pObject + 0x124) = g_NETWORK_EmbeddedFileBackedFileWrapperThunkVtable;
        *(void **)pObject = g_NETWORK_EmbeddedFileBackedBaseVtable;
        *(void **)((char *)pObject + 0x138) = g_NETWORK_EmbeddedFileBackedPrimaryThunkVtable;
        *(void **)((char *)pObject + 0x14c) = g_NETWORK_EmbeddedFileBackedTimedFileViewVtable;

        ConstructEffStreamChannelState((char *)pObject + 8);
        ConstructTimedEffStream((char *)pObject + 0x38, 0);
        ConstructDualHandleEffStream((char *)pObject + 0xb0, 0);

        *(void **)(*(int *)(*(int *)((char *)pObject + 0x108) + 4) + 0x108 + (int)(unsigned long)pObject) =
            (void *)0x004990D8;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x108) + 8) + 0x108 + (int)(unsigned long)pObject) =
            (void *)0x004932C8;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x108) + 0xc) + 0x108 + (int)(unsigned long)pObject) =
            (void *)0x004932C8;

        nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x108) + 4);
        *(int *)(nThunkOffset + 0x104 + (int)(unsigned long)pObject) = nThunkOffset - 8;
        nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x108) + 8);
        *(int *)(nThunkOffset + 0x104 + (int)(unsigned long)pObject) = nThunkOffset - 0x38;
        nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x108) + 0xc);
        *(int *)(nThunkOffset + 0x104 + (int)(unsigned long)pObject) = nThunkOffset - 0xb0;

        *(void **)((char *)pObject + 0x10c) = (void *)0x00498070;
        *(int *)((char *)pObject + 0x110) = 0;
        *(int *)((char *)pObject + 0x114) = 1;
        *(void **)((char *)pObject + 0x10c) = (void *)0x0049A628;
        *(void **)((char *)pObject + 0x120) = (void *)0x0049A53C;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x124) + 4) + 0x124 + (int)(unsigned long)pObject) =
            (void *)0x0049A500;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x124) + 8) + 0x124 + (int)(unsigned long)pObject) =
            (void *)0x0049A4F0;

        ConstructDualFileBackedEffChannel((char *)pObject + 300, 0);
        ConstructTimedFileBackedEffChannel((char *)pObject + 0x140, 0);
    }

    *(void **)(*(int *)(*(int *)pObject + 4) + (int)(unsigned long)pObject) =
        g_NETWORK_EmbeddedFileBackedFinalPrimaryThunkVtable;
    *(void **)(*(int *)(*(int *)pObject + 8) + (int)(unsigned long)pObject) =
        g_NETWORK_EmbeddedFileBackedFinalTimedThunkVtable;
    *(void **)(*(int *)(*(int *)pObject + 0xc) + (int)(unsigned long)pObject) =
        g_NETWORK_EmbeddedFileBackedFinalDualThunkVtable;
    *(void **)(*(int *)(*(int *)pObject + 0x14) + (int)(unsigned long)pObject) =
        g_NETWORK_EmbeddedFileBackedFinalFileThunkVtable;
    *(void **)(*(int *)(*(int *)pObject + 0x18) + (int)(unsigned long)pObject) =
        g_NETWORK_EmbeddedFileBackedFinalFatalThunkVtable;
    *(int *)(*(int *)(*(int *)pObject + 4) - 4 + (int)(unsigned long)pObject) = *(int *)(*(int *)pObject + 4) - 8;
    *(int *)(*(int *)(*(int *)pObject + 8) - 4 + (int)(unsigned long)pObject) = *(int *)(*(int *)pObject + 8) - 0x38;
    *(int *)(*(int *)(*(int *)pObject + 0xc) - 4 + (int)(unsigned long)pObject) =
        *(int *)(*(int *)pObject + 0xc) - 0xb0;
    return pObject;
}

// FUNCTION: LEMBALL 0x0047A570
void *ConstructGlobalStateFileBackedEffComposite(void *pObject, int fConstructEmbeddedObjects) {
    int nThunkOffset;
    void *pStorage;
    int nVtable;

    if (fConstructEmbeddedObjects != 0) {
        *(void **)((char *)pObject + 0x20) = g_NETWORK_GlobalStateCompositeTimedStreamVtable;
        *(void **)((char *)pObject + 4) = g_NETWORK_GlobalStateCompositeTransportVtable;
        *(void **)((char *)pObject + 0xd4) = g_NETWORK_GlobalStateCompositeDualStreamVtable;
        *(void **)((char *)pObject + 0x14c) = g_NETWORK_GlobalStateCompositeDualThunkVtable;
        *(void **)((char *)pObject + 0x160) = g_NETWORK_GlobalStateCompositeFileWrapperVtable;
        *(void **)((char *)pObject + 0x180) = g_NETWORK_GlobalStateCompositeTimedFileThunkVtable;
        *(void **)((char *)pObject + 0x194) = g_NETWORK_GlobalStateCompositeEmbeddedStackVtable;
        *(void **)((char *)pObject + 0x1a8) = g_NETWORK_GlobalStateCompositeEmbeddedStackDataVtable;
        *(void **)((char *)pObject + 0x1b0) = g_NETWORK_GlobalStateCompositeEmbeddedStackBaseVtable;

        ConstructEffStreamChannelState((char *)pObject + 0x60);
        ConstructTimedEffStream((char *)pObject + 0x90, 0);
        ConstructDualHandleEffStream((char *)pObject + 0x108, 0);

        *(void **)(*(int *)(*(int *)((char *)pObject + 0x160) + 4) + 0x160 + (int)(unsigned long)pObject) =
            (void *)0x004990D8;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x160) + 8) + 0x160 + (int)(unsigned long)pObject) =
            (void *)0x004932C8;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x160) + 0xc) + 0x160 + (int)(unsigned long)pObject) =
            (void *)0x004932C8;

        nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x160) + 4);
        *(int *)(nThunkOffset + 0x15c + (int)(unsigned long)pObject) = nThunkOffset - 8;
        nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x160) + 8);
        *(int *)(nThunkOffset + 0x15c + (int)(unsigned long)pObject) = nThunkOffset - 0x38;
        nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x160) + 0xc);
        *(int *)(nThunkOffset + 0x15c + (int)(unsigned long)pObject) = nThunkOffset - 0xb0;

        *(int *)((char *)pObject + 0x168) = 0;
        *(void **)((char *)pObject + 0x164) = (void *)0x00498070;
        *(int *)((char *)pObject + 0x16c) = 1;
        *(void **)((char *)pObject + 0x164) = (void *)0x0049A628;
        *(void **)((char *)pObject + 0x17c) = (void *)0x0049A53C;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x180) + 4) + 0x180 + (int)(unsigned long)pObject) =
            (void *)0x0049A500;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x180) + 8) + 0x180 + (int)(unsigned long)pObject) =
            (void *)0x0049A4F0;

        ConstructDualFileBackedEffChannel((char *)pObject + 0x188, 0);
        ConstructTimedFileBackedEffChannel((char *)pObject + 0x19c, 0);
        ConstructEmbeddedFileBackedEffChannelStack((char *)pObject + 0x1b0, 0);
    }

    *(int *)((char *)pObject + 0x28) = 0;
    *(int *)((char *)pObject + 0x24) = 0x004932C8;
    ResetEffStreamStateFields((char *)pObject + 0x24);
    *(int *)((char *)pObject + 0x3c) = *(int *)((char *)pObject + 0x3c) + 2;
    *(unsigned short *)((char *)pObject + 0x50) = 0;
    *(void **)((char *)pObject + 0x24) = g_NETWORK_GlobalStateCompositeInitialVtable;

    *(void **)(*(int *)(*(int *)((char *)pObject + 0x20) + 4) + 0x20 + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x20) + 8) + 0x20 + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x20) + 0xc) + 0x20 + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeInitialDualThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x14) + 0x20 + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeInitialFileThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x18) + 0x20 + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeInitialFatalThunkVtable;
    *(int *)(*(int *)(*(int *)((char *)pObject + 0x20) + 4) + 0x1c + (int)(unsigned long)pObject) = 0;
    *(int *)(*(int *)(*(int *)((char *)pObject + 0x20) + 8) + 0x1c + (int)(unsigned long)pObject) = 0;
    *(int *)(*(int *)(*(int *)((char *)pObject + 0x20) + 0xc) + 0x1c + (int)(unsigned long)pObject) = 0;

    ConstructEffTransportRuntimeChannelStack(pObject, 0);

    *(void **)pObject = g_NETWORK_GlobalStateCompositeFinalVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x20) + 4) + 0x20 + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x20) + 8) + 0x20 + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x20) + 0xc) + 0x20 + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeFinalDualThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x14) + 0x20 + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeFinalFileThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x18) + 0x20 + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeFinalFatalThunkVtable;

    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x20) + 4);
    *(int *)(nThunkOffset + 0x1c + (int)(unsigned long)pObject) = nThunkOffset - 0x40;
    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x20) + 8);
    *(int *)(nThunkOffset + 0x1c + (int)(unsigned long)pObject) = nThunkOffset - 0x70;
    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x20) + 0xc);
    *(int *)(nThunkOffset + 0x1c + (int)(unsigned long)pObject) = nThunkOffset - 0xe8;
    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x20) + 0x18);
    *(int *)(nThunkOffset + 0x1c + (int)(unsigned long)pObject) = nThunkOffset - 0x15c;

    (*(void (**)(char *))(*(int *)g_pEffTransportRuntimeService + 8))(g_pszFileBasedNetworkLocalHostName);

    if (g_pGlobalStateEff512ByteStream == 0) {
        pStorage = AllocateVSMemBlock(0x30);
        if (pStorage == 0) {
            g_pGlobalStateEff512ByteStream = 0;
        } else {
            g_pGlobalStateEff512ByteStream = ConstructEff512ByteStateStream(pStorage);
        }
    }

    *(int *)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x18) + 0x28 + (int)(unsigned long)pObject) = 0x14;
    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x20) + 0x20 + (int)(unsigned long)pObject) = 0;
    } else {
        nVtable = *(int *)((char *)pObject + 0x20);
        *(void **)(*(int *)(nVtable + 0x20) + 0x20 + (int)(unsigned long)pObject) =
            ConstructEffStreamRecordSlotTable(
                pStorage,
                *(int *)(*(int *)(nVtable + 0x18) + 0x28 + (int)(unsigned long)pObject));
    }

    *(int *)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x1c) + 0x20 + (int)(unsigned long)pObject) =
        *(int *)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x20) + 0x20 + (int)(unsigned long)pObject);
    *(int *)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x20) + 0x24 + (int)(unsigned long)pObject) =
        *(int *)((char *)g_pGlobalStateEff512ByteStream + 0x18) + *(int *)((char *)pObject + 0x3c);
    *(int *)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x1c) + 0x24 + (int)(unsigned long)pObject) =
        *(int *)(*(int *)(*(int *)((char *)pObject + 0x20) + 0x20) + 0x24 + (int)(unsigned long)pObject);
    *(int *)((char *)pObject + 0x54) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x0047A910
void DestroyGlobalStateFileBackedEffComposite(int nObjectBasePlus0x60) {
    int *pnTransportVtable;
    int nThunkOffset;
    void *pRecordSlotTable;

    pnTransportVtable = (int *)(nObjectBasePlus0x60 - 0x40);
    *(void **)(nObjectBasePlus0x60 - 0x60) = g_NETWORK_GlobalStateCompositeFinalVtable;
    *(void **)(*(int *)(*pnTransportVtable + 4) + nObjectBasePlus0x60 - 0x40) =
        g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable;
    *(void **)(*(int *)(*pnTransportVtable + 8) + nObjectBasePlus0x60 - 0x40) =
        g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable;
    *(void **)(*(int *)(*pnTransportVtable + 0xc) + nObjectBasePlus0x60 - 0x40) =
        g_NETWORK_GlobalStateCompositeFinalDualThunkVtable;
    *(void **)(*(int *)(*pnTransportVtable + 0x14) + nObjectBasePlus0x60 - 0x40) =
        g_NETWORK_GlobalStateCompositeFinalFileThunkVtable;
    *(void **)(*(int *)(*pnTransportVtable + 0x18) + nObjectBasePlus0x60 - 0x40) =
        g_NETWORK_GlobalStateCompositeFinalFatalThunkVtable;

    *(int *)(*(int *)(*pnTransportVtable + 4) + nObjectBasePlus0x60 - 0x44) = *(int *)(*pnTransportVtable + 4) - 0x40;
    *(int *)(*(int *)(*pnTransportVtable + 8) + nObjectBasePlus0x60 - 0x44) = *(int *)(*pnTransportVtable + 8) - 0x70;
    *(int *)(*(int *)(*pnTransportVtable + 0xc) + nObjectBasePlus0x60 - 0x44) = *(int *)(*pnTransportVtable + 0xc) - 0xe8;
    *(int *)(*(int *)(*pnTransportVtable + 0x18) + nObjectBasePlus0x60 - 0x44) =
        *(int *)(*pnTransportVtable + 0x18) - 0x15c;

    pRecordSlotTable = *(void **)(*(int *)(*pnTransportVtable + 0x1c) + nObjectBasePlus0x60 - 0x40);
    if (pRecordSlotTable != 0) {
        ((void (*)(int))*(void **)(*(int *)pRecordSlotTable + 0x14))(1);
    }

    if (g_pszFileBasedNetworkConfiguredPath != 0) {
        FreeVSMemBlock(g_pszFileBasedNetworkConfiguredPath);
        g_pszFileBasedNetworkConfiguredPath = 0;
    }

    if (g_pGlobalStateEff512ByteStream != 0) {
        ((void (*)(int))*(void **)(*(int *)g_pGlobalStateEff512ByteStream + 0x14))(1);
        g_pGlobalStateEff512ByteStream = 0;
    }

    ReleaseEffTransportRuntimeBuffers(nObjectBasePlus0x60 - 0x3c);
    DestroyTimedEffStream((void *)((((nObjectBasePlus0x60 == 0x60) - 1) & (int)(unsigned long)pnTransportVtable) + 4));
}

// FUNCTION: LEMBALL 0x0047BB60
void *DeleteGlobalStateFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    DestroyGlobalStateFileBackedEffComposite((int)(unsigned long)pObject);
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x154);
    *(void **)((char *)pObject + 0x104) = (void *)0x00498070;
    if (*(void **)((char *)pObject + 0x108) != 0) {
        FreeVSMemBlock(*(void **)((char *)pObject + 0x108));
    }
    DestroyDualHandleEffStream((char *)pObject + 0xa8);
    DestroyTimedEffStream((char *)pObject + 0x30);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x60);
    }
    return (char *)pObject - 0x60;
}

// FUNCTION: LEMBALL 0x0047AF30
void *ConstructDualSlotTableFileBackedEffComposite(void *pObject, int fConstructEmbeddedObjects) {
    int nThunkOffset;
    void *pStorage;
    int nVtable;

    if (fConstructEmbeddedObjects != 0) {
        *(void **)((char *)pObject + 0x2c) = g_NETWORK_DualSlotCompositeTimedStreamVtable;
        *(void **)((char *)pObject + 4) = g_NETWORK_DualSlotCompositeTransportVtable;
        *(void **)((char *)pObject + 0xd8) = g_NETWORK_DualSlotCompositeDualStreamVtable;
        *(void **)((char *)pObject + 0x150) = g_NETWORK_DualSlotCompositeDualThunkVtable;
        *(void **)((char *)pObject + 0x164) = g_NETWORK_DualSlotCompositeFileWrapperVtable;
        *(void **)((char *)pObject + 0x184) = g_NETWORK_DualSlotCompositeTimedFileThunkVtable;
        *(void **)((char *)pObject + 0x198) = g_NETWORK_DualSlotCompositeEmbeddedStackVtable;
        *(void **)((char *)pObject + 0x1ac) = g_NETWORK_DualSlotCompositeEmbeddedStackDataVtable;
        *(void **)((char *)pObject + 0x1b4) = g_NETWORK_DualSlotCompositeEmbeddedStackBaseVtable;

        ConstructEffStreamChannelState((char *)pObject + 100);
        ConstructTimedEffStream((char *)pObject + 0x94, 0);
        ConstructDualHandleEffStream((char *)pObject + 0x10c, 0);

        *(void **)(*(int *)(*(int *)((char *)pObject + 0x164) + 4) + 0x164 + (int)(unsigned long)pObject) =
            (void *)0x004990D8;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x164) + 8) + 0x164 + (int)(unsigned long)pObject) =
            (void *)0x004932C8;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x164) + 0xc) + 0x164 + (int)(unsigned long)pObject) =
            (void *)0x004932C8;

        nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x164) + 4);
        *(int *)(nThunkOffset + 0x160 + (int)(unsigned long)pObject) = nThunkOffset - 8;
        nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x164) + 8);
        *(int *)(nThunkOffset + 0x160 + (int)(unsigned long)pObject) = nThunkOffset - 0x38;
        nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x164) + 0xc);
        *(int *)(nThunkOffset + 0x160 + (int)(unsigned long)pObject) = nThunkOffset - 0xb0;

        *(int *)((char *)pObject + 0x16c) = 0;
        *(void **)((char *)pObject + 0x168) = (void *)0x00498070;
        *(int *)((char *)pObject + 0x170) = 1;
        *(void **)((char *)pObject + 0x168) = (void *)0x0049A628;
        *(void **)((char *)pObject + 0x180) = (void *)0x0049A53C;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x184) + 4) + 0x184 + (int)(unsigned long)pObject) =
            (void *)0x0049A500;
        *(void **)(*(int *)(*(int *)((char *)pObject + 0x184) + 8) + 0x184 + (int)(unsigned long)pObject) =
            (void *)0x0049A4F0;

        ConstructDualFileBackedEffChannel((char *)pObject + 0x18c, 0);
        ConstructTimedFileBackedEffChannel((char *)pObject + 0x1a0, 0);
        ConstructEmbeddedFileBackedEffChannelStack((char *)pObject + 0x1b4, 0);
    }

    *(int *)((char *)pObject + 0x34) = 0;
    *(int *)((char *)pObject + 0x30) = 0x004932C8;
    ResetEffStreamStateFields((char *)pObject + 0x30);
    *(int *)((char *)pObject + 0x48) = *(int *)((char *)pObject + 0x48) + 2;
    *(unsigned short *)((char *)pObject + 0x5c) = 0;
    *(void **)((char *)pObject + 0x30) = g_NETWORK_GlobalStateCompositeInitialVtable;

    *(void **)(*(int *)(*(int *)((char *)pObject + 0x2c) + 4) + 0x2c + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x2c) + 8) + 0x2c + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0xc) + 0x2c + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeInitialDualThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0x14) + 0x2c + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeInitialFileThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0x18) + 0x2c + (int)(unsigned long)pObject) =
        g_NETWORK_GlobalStateCompositeInitialFatalThunkVtable;
    *(int *)(*(int *)(*(int *)((char *)pObject + 0x2c) + 4) + 0x28 + (int)(unsigned long)pObject) = 0;
    *(int *)(*(int *)(*(int *)((char *)pObject + 0x2c) + 8) + 0x28 + (int)(unsigned long)pObject) = 0;
    *(int *)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0xc) + 0x28 + (int)(unsigned long)pObject) = 0;

    ConstructCompositeEffTransportStack(pObject, 0);

    *(void **)pObject = g_NETWORK_DualSlotCompositeFinalVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x2c) + 4) + 0x2c + (int)(unsigned long)pObject) =
        g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x2c) + 8) + 0x2c + (int)(unsigned long)pObject) =
        g_NETWORK_DualSlotCompositeFinalTimedThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0xc) + 0x2c + (int)(unsigned long)pObject) =
        g_NETWORK_DualSlotCompositeFinalDualThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0x14) + 0x2c + (int)(unsigned long)pObject) =
        g_NETWORK_DualSlotCompositeFinalFileThunkVtable;
    *(void **)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0x18) + 0x2c + (int)(unsigned long)pObject) =
        g_NETWORK_DualSlotCompositeFinalFatalThunkVtable;

    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x2c) + 4);
    *(int *)(nThunkOffset + 0x28 + (int)(unsigned long)pObject) = nThunkOffset - 0x38;
    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x2c) + 8);
    *(int *)(nThunkOffset + 0x28 + (int)(unsigned long)pObject) = nThunkOffset - 0x68;
    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x2c) + 0xc);
    *(int *)(nThunkOffset + 0x28 + (int)(unsigned long)pObject) = nThunkOffset - 0xe0;
    nThunkOffset = *(int *)(*(int *)((char *)pObject + 0x2c) + 0x18);
    *(int *)(nThunkOffset + 0x28 + (int)(unsigned long)pObject) = nThunkOffset - 0x154;

    *(int *)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0x18) + 0x34 + (int)(unsigned long)pObject) = 3;

    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0x1c) + 0x2c + (int)(unsigned long)pObject) = 0;
    } else {
        nVtable = *(int *)((char *)pObject + 0x2c);
        *(void **)(*(int *)(nVtable + 0x1c) + 0x2c + (int)(unsigned long)pObject) =
            ConstructEffStreamRecordSlotTable(
                pStorage,
                *(int *)(*(int *)(nVtable + 0x18) + 0x34 + (int)(unsigned long)pObject));
    }

    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0x20) + 0x2c + (int)(unsigned long)pObject) = 0;
    } else {
        nVtable = *(int *)((char *)pObject + 0x2c);
        *(void **)(*(int *)(nVtable + 0x20) + 0x2c + (int)(unsigned long)pObject) =
            ConstructEffStreamRecordSlotTable(
                pStorage,
                *(int *)(*(int *)(nVtable + 0x18) + 0x34 + (int)(unsigned long)pObject));
    }

    *(int *)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0x1c) + 0x30 + (int)(unsigned long)pObject) =
        *(int *)((char *)pObject + 0x48);
    *(int *)(*(int *)(*(int *)((char *)pObject + 0x2c) + 0x20) + 0x30 + (int)(unsigned long)pObject) =
        *(int *)((char *)pObject + 0x48);
    return pObject;
}

// FUNCTION: LEMBALL 0x0047B2A0
void DestroyDualSlotTableFileBackedEffComposite(int nObjectBasePlus0x64) {
    int *pnTransportVtable;
    int *pRecordTable;

    pnTransportVtable = (int *)(nObjectBasePlus0x64 - 0x38);
    *(void **)(nObjectBasePlus0x64 - 100) = g_NETWORK_DualSlotCompositeFinalVtable;
    *(void **)(*(int *)(*pnTransportVtable + 4) + nObjectBasePlus0x64 - 0x38) = g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable;
    *(void **)(*(int *)(*pnTransportVtable + 8) + nObjectBasePlus0x64 - 0x38) = g_NETWORK_DualSlotCompositeFinalTimedThunkVtable;
    *(void **)(*(int *)(*pnTransportVtable + 0xc) + nObjectBasePlus0x64 - 0x38) = g_NETWORK_DualSlotCompositeFinalDualThunkVtable;
    *(void **)(*(int *)(*pnTransportVtable + 0x14) + nObjectBasePlus0x64 - 0x38) = g_NETWORK_DualSlotCompositeFinalFileThunkVtable;
    *(void **)(*(int *)(*pnTransportVtable + 0x18) + nObjectBasePlus0x64 - 0x38) = g_NETWORK_DualSlotCompositeFinalFatalThunkVtable;
    *(int *)(*(int *)(*pnTransportVtable + 4) + nObjectBasePlus0x64 - 0x3c) = *(int *)(*pnTransportVtable + 4) - 0x38;
    *(int *)(*(int *)(*pnTransportVtable + 8) + nObjectBasePlus0x64 - 0x3c) = *(int *)(*pnTransportVtable + 8) - 0x68;
    *(int *)(*(int *)(*pnTransportVtable + 0xc) + nObjectBasePlus0x64 - 0x3c) = *(int *)(*pnTransportVtable + 0xc) - 0xe0;
    *(int *)(*(int *)(*pnTransportVtable + 0x18) + nObjectBasePlus0x64 - 0x3c) = *(int *)(*pnTransportVtable + 0x18) - 0x154;

    pRecordTable = *(int **)(*(int *)(*pnTransportVtable + 0x1c) + nObjectBasePlus0x64 - 0x38);
    if (pRecordTable != 0) {
        ((void (*)(int))*(void **)(*pRecordTable + 0x14))(1);
    }

    pRecordTable = *(int **)(*(int *)(*pnTransportVtable + 0x20) + nObjectBasePlus0x64 - 0x38);
    if (pRecordTable != 0) {
        ((void (*)(int))*(void **)(*pRecordTable + 0x14))(1);
    }

    RestoreCompositeEffTransportVtables(nObjectBasePlus0x64 - 0x34);
    DestroyEffStreamBase((void *)((((nObjectBasePlus0x64 == 100) - 1) & (int)(unsigned long)pnTransportVtable) + 4));
}

// FUNCTION: LEMBALL 0x0047B370
void WriteDualSlotTableFileBackedEffHeader(int nObjectBasePlus0x154) {
    ((void (*)(int))*(void **)(*(int *)(*(int *)(nObjectBasePlus0x154 - 0x154) + 0x14) + nObjectBasePlus0x154 - 0x154))(0);
    WriteEffStreamToLockedFile(
        (void *)(unsigned long)(*(int *)(*(int *)(nObjectBasePlus0x154 - 0x154) + 0x20) + nObjectBasePlus0x154 - 0x154),
        (int *)(unsigned long)(nObjectBasePlus0x154 - 0x150),
        0,
        0);
}

// FUNCTION: LEMBALL 0x0047B9D0
void *DeleteCompositeFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory) {
    DestroyEffStreamBase((char *)pObject - 0x34);
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x150);
    *(void **)((char *)pObject + 0x104) = (void *)0x00498070;
    if (*(void **)((char *)pObject + 0x108) != 0) {
        FreeVSMemBlock(*(void **)((char *)pObject + 0x108));
    }
    DestroyDualHandleEffStream((char *)pObject + 0xa8);
    DestroyTimedEffStream((char *)pObject + 0x30);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x38);
    }
    return (char *)pObject - 0x38;
}

// FUNCTION: LEMBALL 0x0047BCC0
void *DeleteTimedFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory) {
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject);
    *(void **)((char *)pObject + 0xa4) = (void *)0x00498070;
    if (*(void **)((char *)pObject + 0xa8) != 0) {
        FreeVSMemBlock(*(void **)((char *)pObject + 0xa8));
    }
    DestroyTimedEffStream((char *)pObject + 0x30);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x18);
    }
    return (char *)pObject - 0x18;
}

// FUNCTION: LEMBALL 0x0047BE50
void *DeleteEmbeddedFileBackedEffChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x150);
    *(void **)((char *)pObject + 0x104) = (void *)0x00498070;
    if (*(void **)((char *)pObject + 0x108) != 0) {
        FreeVSMemBlock(*(void **)((char *)pObject + 0x108));
    }
    DestroyDualHandleEffStream((char *)pObject + 0xa8);
    DestroyTimedEffStream((char *)pObject + 0x30);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 8);
    }
    return (char *)pObject - 8;
}

// FUNCTION: LEMBALL 0x0047BF40
void *DeleteDualSlotTableFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    DestroyDualSlotTableFileBackedEffComposite((int)(unsigned long)pObject);
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x154);
    *(void **)((char *)pObject + 0x104) = (void *)0x00498070;
    if (*(void **)((char *)pObject + 0x108) != 0) {
        FreeVSMemBlock(*(void **)((char *)pObject + 0x108));
    }
    DestroyDualHandleEffStream((char *)pObject + 0xa8);
    DestroyTimedEffStream((char *)pObject + 0x30);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 100);
    }
    return (char *)pObject - 100;
}

// FUNCTION: LEMBALL 0x00479880
int OpenFileBackedEffHandleWithRetry(void *pObject, int nHandle) {
    DWORD dwStartTime;
    DWORD dwNow;

    if (((int (*)(int, int, int))**(void ***)(*(int *)(*(int *)((char *)pObject + 4) + 4) + 4 + (int)(unsigned long)pObject))(
            nHandle,
            1,
            0) == 0) {
        if (((int (*)(int, int))(*(void **)(*(int *)(*(int *)(*(int *)((char *)pObject + 4) + 4) + 4 +
                                                      (int)(unsigned long)pObject) +
                                             4)))(nHandle, 1) == 0) {
            dwStartTime = timeGetTime();
            do {
                dwNow = timeGetTime();
            } while (dwNow - dwStartTime < 100);

            if (((int (*)(int, int, int))
                     **(void ***)(*(int *)(*(int *)((char *)pObject + 4) + 4) + 4 + (int)(unsigned long)pObject))(
                    nHandle,
                    1,
                    0) == 0) {
                return 0;
            }
        } else {
            ((void (*)())**(void ***)pObject)();
        }
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00479900
BYTE MapNestedEffCallbackFailureToStatus6(int nObjectBasePlus0x1c) {
    int nResult;

    nResult = ((int (*)())(*(void **)(*(int *)(*(int *)(*(int *)(nObjectBasePlus0x1c - 0x1c) + 4) +
                                      nObjectBasePlus0x1c - 0x1c) +
                               0xc)))();
    return (BYTE)(-(nResult == 0) & 6);
}

// FUNCTION: LEMBALL 0x00479A50
int LoadEffStreamFromFileBackedRange(void *pObject, void *pStream, int fKeepLocked, int fAlreadyLocked) {
    DWORD dwOffset;
    DWORD cbRead;
    DWORD dwStartTime;
    DWORD dwNow;

    dwOffset = ((DWORD (*)())(*(void **)(*(int *)(*(int *)(*(int *)((char *)pObject + 0xc) + 0xc) + 0xc +
                                          (int)(unsigned long)pObject) +
                                       0x28)))();
    cbRead = *(DWORD *)((char *)pStream + 0x18);

    if (fAlreadyLocked == 0) {
        dwStartTime = timeGetTime();
        while (LockWin32FileRange((void *)(unsigned long)(*(int *)(*(int *)((char *)pObject + 0xc) + 0xc) + 0xc +
                                                       (int)(unsigned long)pObject),
                                  dwOffset,
                                  cbRead) == 0) {
            dwNow = timeGetTime();
            if (99 < dwNow - dwStartTime) {
                return 0;
            }
        }
    }

    if (ReadWin32FileWrapper((void *)(unsigned long)(*(int *)(*(int *)((char *)pObject + 0xc) + 0xc) + 0xc +
                                                  (int)(unsigned long)pObject),
                             (LPVOID)g_pEffTransportPacketBuffer,
                             cbRead) == 0) {
        UnlockWin32FileRange((void *)(unsigned long)(*(int *)(*(int *)((char *)pObject + 0xc) + 0xc) + 0xc +
                                                   (int)(unsigned long)pObject),
                             dwOffset,
                             cbRead);
        return 0;
    }

    LoadEffStreamFromMemory(pStream, (int)(unsigned long)g_pEffTransportPacketBuffer);
    if (fKeepLocked == 0 &&
        UnlockWin32FileRange((void *)(unsigned long)(*(int *)(*(int *)((char *)pObject + 0xc) + 0xc) + 0xc +
                                                   (int)(unsigned long)pObject),
                             dwOffset,
                             cbRead) == 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00479B30
int LoadFileBackedEffRecordPayload(void *pObject, int nSlotIndex) {
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    NETWORK_EffStreamRecordSlot *pSlot;
    int nResult;
    DWORD dwNow;

    pRecordTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    nResult = ((int (*)(int))(*(void **)(*(int *)(*(int *)(*(int *)((char *)pObject + 0xc) + 0xc) + 0xc +
                                            (int)(unsigned long)pObject) +
                                     0x20)))(
        g_cbEffTransportMaxPacketBytes * nSlotIndex + *(int *)((char *)pObject + 8));
    if (nResult == 0) {
        return 0;
    }

    pSlot = &pRecordTable->m_pSlots[nSlotIndex];
    nResult = ReadWin32FileWrapper((void *)(unsigned long)(*(int *)(*(int *)((char *)pObject + 0xc) + 0xc) + 0xc +
                                                      (int)(unsigned long)pObject),
                                   (LPVOID)g_pEffTransportPacketBuffer,
                                   pSlot->m_cbPayload);
    if (nResult == 0) {
        ((void (*)())(*(void **)(*(int *)(*(int *)(*(int *)((char *)pObject + 0xc) + 4) + 0xc +
                                     (int)(unsigned long)pObject) +
                                8)))();
        return 0;
    }

    dwNow = timeGetTime();
    *(DWORD *)(*(int *)(*(int *)((char *)pObject + 0xc) + 8) + 0x48 + (int)(unsigned long)pObject) = dwNow;
    g_cbEffTransportCurrentPacketBytes = (int)pSlot->m_cbPayload;
    (*(void (**)(int))(*(int *)g_pEffTransportPeerAddressState + 8))((int)(unsigned long)pSlot->m_szSourceName);
    if (*(int *)(*(int *)(*(int *)((char *)pObject + 0xc) + 4) + 0x1c + (int)(unsigned long)pObject) == 0) {
        ((void (*)())(*(void **)(*(int *)(*(int *)(*(int *)((char *)pObject + 0xc) + 8) + 0xc +
                                 (int)(unsigned long)pObject) +
                            0x18)))();
    }
    ProcessEffTransportPacketHeader((int *)(unsigned long)(*(int *)(*(int *)((char *)pObject + 0xc) + 8) + 0xc +
                                                       (int)(unsigned long)pObject));
    return 1;
}

// FUNCTION: LEMBALL 0x0047A470
DWORD IncrementFileBackedEffStreamMarker(int *pObject) {
    ((void (*)(int))(*(void **)(*(int *)(*(int *)(*pObject + 0x14) + (int)(unsigned long)pObject) + 0x20)))(0);
    if (LoadEffStreamFromFileBackedRange((void *)(unsigned long)(*(int *)(*pObject + 0x1c) + (int)(unsigned long)pObject),
                                         pObject + 1,
                                         1,
                                         0) == 0) {
        return 0;
    }
    *(short *)(pObject + 0xc) = (short)pObject[0xc] + 1;
    ((void (*)(int))(*(void **)(*(int *)(*(int *)(*pObject + 0x14) + (int)(unsigned long)pObject) + 0x20)))(0);
    return WriteEffStreamToLockedFile((void *)(unsigned long)(*(int *)(*pObject + 0x20) + (int)(unsigned long)pObject),
                                      pObject + 1,
                                      0,
                                      1);
}

// FUNCTION: LEMBALL 0x0047A4D0
DWORD DecrementFileBackedEffStreamMarker(int *pObject) {
    ((void (*)(int))(*(void **)(*(int *)(*(int *)(*pObject + 0x14) + (int)(unsigned long)pObject) + 0x20)))(0);
    if (LoadEffStreamFromFileBackedRange((void *)(unsigned long)(*(int *)(*pObject + 0x1c) + (int)(unsigned long)pObject),
                                         pObject + 1,
                                         1,
                                         0) == 0) {
        return 0;
    }
    *(short *)(pObject + 0xc) = (short)pObject[0xc] - 1;
    ((void (*)(int))(*(void **)(*(int *)(*(int *)(*pObject + 0x14) + (int)(unsigned long)pObject) + 0x20)))(0);
    return WriteEffStreamToLockedFile((void *)(unsigned long)(*(int *)(*pObject + 0x20) + (int)(unsigned long)pObject),
                                      pObject + 1,
                                      0,
                                      1);
}

// FUNCTION: LEMBALL 0x0047A530
BYTE DecrementFileBackedEffMarkerAndReleaseIfZero(int nObjectBasePlus0x38) {
    int *pMarkerObject;
    BYTE bStatus;

    pMarkerObject = (int *)(unsigned long)(nObjectBasePlus0x38 - 0x38);
    DecrementFileBackedEffStreamMarker(pMarkerObject);
    bStatus = MapNestedEffCallbackFailureToStatus6(*(int *)(*pMarkerObject + 0x18) + nObjectBasePlus0x38 - 0x18);
    if (*(short *)(nObjectBasePlus0x38 - 8) == 0) {
        ((void (*)())(*(void **)(*(int *)(*(int *)(*pMarkerObject + 0x14) + nObjectBasePlus0x38 - 0x38) + 0x14)))();
    }
    return bStatus;
}

// FUNCTION: LEMBALL 0x00479C10
void ServicePendingFileBackedEffRecords(int *pObject) {
    NETWORK_EffStreamRecordSlot *pSlot;
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    int nThunkOffset;
    int nLockResult;
    void *pRuntimeWindow;
    int i;
    int nSlotCount;
    int fLoadedOne;

    nThunkOffset = *(int *)(pObject[3] + 4);
    if ((*(int *)((char *)pObject + nThunkOffset + 0x1c) == 0 && *(int *)((char *)pObject + nThunkOffset + 0x28) == 0) ||
        *(int *)((char *)pObject + nThunkOffset + 0x18) == 0) {
        return;
    }

    nLockResult = LockWin32FileRange((void *)(unsigned long)(*(int *)(pObject[3] + 0xc) + 0xc + (int)(unsigned long)pObject),
                                     pObject[1],
                                     *(DWORD *)(*pObject + 0x18));
    if (nLockResult == 0) {
        pRuntimeWindow = 0;
        if (g_pActiveNetworkRuntimeWindow != 0) {
            pRuntimeWindow = (char *)g_pActiveNetworkRuntimeWindow - 0x10;
        }
        ScheduleNetworkRuntimeTimerEvent(pRuntimeWindow, 0x32);
        return;
    }

    if (pObject[4] == -1) {
        ((int (*)(int))(*(void **)(*(int *)(*(int *)(pObject[3] + 0xc) + 0xc + (int)(unsigned long)pObject) + 0x20)))(pObject[1]);
        ReadWin32FileWrapper((void *)(unsigned long)(*(int *)(pObject[3] + 0xc) + 0xc + (int)(unsigned long)pObject),
                             (LPVOID)g_pEffTransportPacketBuffer,
                             *(DWORD *)(*pObject + 0x18));
        nLockResult = UnlockWin32FileRange((void *)(unsigned long)(*(int *)(pObject[3] + 0xc) + 0xc + (int)(unsigned long)pObject),
                                           pObject[1],
                                           *(DWORD *)(*pObject + 0x18));
        if (nLockResult != 0) {
            pRecordTable = (NETWORK_EffStreamRecordSlotTable *)(unsigned long)*pObject;
            nSlotCount = 0;
            LoadEffStreamFromMemory((void *)(unsigned long)*pObject, (int)(unsigned long)g_pEffTransportPacketBuffer);
            fLoadedOne = 0;
            if (0 < *(int *)(*(int *)(pObject[3] + 0x10) + 0x14 + (int)(unsigned long)pObject)) {
                do {
                    pSlot = &pRecordTable->m_pSlots[nSlotCount];
                    if (pSlot->m_wObservedMarker < pSlot->m_wCommittedMarker) {
                        if (fLoadedOne) {
                            pObject[4] = nSlotCount;
                            return;
                        }
                        LoadFileBackedEffRecordPayload(pObject, nSlotCount);
                        pSlot->m_wObservedMarker = pSlot->m_wCommittedMarker;
                        fLoadedOne = 1;
                    }
                    ++nSlotCount;
                } while (nSlotCount < *(int *)(*(int *)(pObject[3] + 0x10) + 0x14 + (int)(unsigned long)pObject));
            }
        }
    } else {
        ((int (*)(int))(*(void **)(*(int *)(*(int *)(pObject[3] + 0xc) + 0xc + (int)(unsigned long)pObject) + 0x20)))(
            *(int *)(*(int *)(*pObject + 0x2c) + 0x18) * pObject[4] + pObject[1]);
        ReadWin32FileWrapper((void *)(unsigned long)(*(int *)(pObject[3] + 0xc) + 0xc + (int)(unsigned long)pObject),
                             (LPVOID)g_pEffTransportPacketBuffer,
                             *(DWORD *)(*pObject + 0x18));
        nLockResult = UnlockWin32FileRange((void *)(unsigned long)(*(int *)(pObject[3] + 0xc) + 0xc + (int)(unsigned long)pObject),
                                           pObject[1],
                                           *(DWORD *)(*pObject + 0x18));
        if (nLockResult != 0) {
            pRecordTable = (NETWORK_EffStreamRecordSlotTable *)(unsigned long)*pObject;
            pSlot = &pRecordTable->m_pSlots[pObject[4]];
            LoadEffStreamFromMemory(pSlot, (int)(unsigned long)g_pEffTransportPacketBuffer);
            LoadFileBackedEffRecordPayload(pObject, pObject[4]);
            pSlot->m_wObservedMarker = pSlot->m_wCommittedMarker;
            i = pObject[4];
            nSlotCount = *(int *)(*(int *)(pObject[3] + 0x10) + 0x14 + (int)(unsigned long)pObject);
            if (i < nSlotCount) {
                do {
                    pSlot = &pRecordTable->m_pSlots[i];
                    if (pSlot->m_wObservedMarker < pSlot->m_wCommittedMarker) {
                        pObject[4] = i;
                        break;
                    }
                    ++i;
                } while (i < nSlotCount);
            }
            if (*(int *)(*(int *)(pObject[3] + 0x10) + 0x14 + (int)(unsigned long)pObject) == i) {
                pObject[4] = -1;
            }
        }
    }
}

// FUNCTION: LEMBALL 0x0047A0B0
int AppendFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    NETWORK_EffStreamRecordSlot *pSlot;
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    char *pszSource;
    char *pszTarget;
    unsigned int cch;
    unsigned int count;
    int nCurrentSlot;
    DWORD dwSlotOffset;
    DWORD cbLocked;
    int fFailed;
    DWORD dwNow;

    if (*(int *)(*(int *)(*(int *)((char *)pObject - 0x3c) + 4) + (int)(unsigned long)pObject - 0x18) == 0) {
        return 0;
    }

    pRecordTable = (NETWORK_EffStreamRecordSlotTable *)((char *)pObject - 0x48);
    nCurrentSlot = *(int *)((char *)pObject - 0x38);
    pSlot = &pRecordTable->m_pSlots[nCurrentSlot];

    dwSlotOffset = *(int *)((char *)&pRecordTable->m_pSlots[0] + 0x18) * nCurrentSlot +
                   *(int *)((char *)pObject - 0x44);
    ((int (*)(DWORD))(*(void **)(*(int *)(*(int *)(*(int *)((char *)pObject - 0x3c) + 0xc) + (int)(unsigned long)pObject - 0x3c) + 0x20)))(
        dwSlotOffset);

    pszSource = ((char *(*)())(*(void **)g_pEffTransportRuntimeService))();
    cch = 0xffffffff;
    do {
        pszTarget = pszSource;
        if (cch == 0) {
            break;
        }
        --cch;
        pszTarget = pszSource + 1;
        if (*pszSource == '\0') {
            break;
        }
        pszSource = pszTarget;
    } while (1);
    cch = ~cch;
    pszSource = pszTarget - cch;
    pszTarget = pSlot->m_szSourceName;
    for (count = cch >> 2; count != 0; --count) {
        *(int *)pszTarget = *(int *)pszSource;
        pszSource += 4;
        pszTarget += 4;
    }
    for (count = cch & 3; count != 0; --count) {
        *pszTarget++ = *pszSource++;
    }

    pszSource = ((char *(*)())**(void ***)(*(int *)(*(int *)((char *)pObject - 0x3c) + 8) +
                                           0x34 + (int)(unsigned long)pObject))();
    cch = 0xffffffff;
    do {
        pszTarget = pszSource;
        if (cch == 0) {
            break;
        }
        --cch;
        pszTarget = pszSource + 1;
        if (*pszSource == '\0') {
            break;
        }
        pszSource = pszTarget;
    } while (1);
    cch = ~cch;
    pszSource = pszTarget - cch;
    pszTarget = pSlot->m_szTargetName;
    for (count = cch >> 2; count != 0; --count) {
        *(int *)pszTarget = *(int *)pszSource;
        pszSource += 4;
        pszTarget += 4;
    }
    for (count = cch & 3; count != 0; --count) {
        *pszTarget++ = *pszSource++;
    }

    pSlot->m_cbPayload = cbPayload;
    cbLocked = WriteEffStreamToLockedFile(pRecordTable,
                                          (int *)pSlot,
                                          1,
                                          0);
    fFailed = 1;
    if (cbLocked != 0) {
        ((int (*)(DWORD))(*(void **)(*(int *)(*(int *)(*(int *)((char *)pObject - 0x3c) + 0xc) + (int)(unsigned long)pObject - 0x3c) + 0x20)))(
            nCurrentSlot * g_cbEffTransportMaxPacketBytes + *(int *)((char *)pObject - 0x40));
        if (WriteWin32FileWrapper((void *)(unsigned long)(*(int *)(*(int *)((char *)pObject - 0x3c) + 0xc) +
                                                       (int)(unsigned long)pObject - 0x3c),
                                  pvPayload,
                                  cbPayload) != 0) {
            fFailed = 0;
        }
    }

    if (fFailed == 0) {
        *(int *)((char *)pObject - 0x38) = nCurrentSlot + 1;
        dwNow = timeGetTime();
        *(DWORD *)(*(int *)(*(int *)((char *)pObject - 0x3c) + 8) + (int)(unsigned long)pObject) = dwNow;
    }
    if (cbLocked != 0) {
        UnlockWin32FileRange((void *)(unsigned long)(*(int *)(*(int *)((char *)pObject - 0x3c) + 0xc) +
                                                   (int)(unsigned long)pObject - 0x3c),
                             dwSlotOffset,
                             cbLocked);
    }
    return fFailed == 0;
}

// FUNCTION: LEMBALL 0x0047A420
int AppendAdjustedFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    int *pnIndex;
    int fAppended;

    fAppended = AppendFileBackedEffRecordSlot((void *)(unsigned long)(*(int *)(*(int *)((char *)pObject - 0x38) + 0x20) +
                                                                   0x10 + (int)(unsigned long)pObject),
                                              pvPayload,
                                              cbPayload);
    if (fAppended != 0) {
        pRecordTable = (NETWORK_EffStreamRecordSlotTable *)(unsigned long)(*(int *)(*(int *)((char *)pObject - 0x38) + 0x18) -
                                                                           0x30 + (int)(unsigned long)pObject);
        pnIndex = (int *)(unsigned long)(*(int *)(*(int *)((char *)pObject - 0x38) + 0x20) - 0x28 +
                                         (int)(unsigned long)pObject);
        *pnIndex = *pnIndex % pRecordTable->m_cSlots;
    }
    return fAppended;
}

// FUNCTION: LEMBALL 0x0047AA10
void InitializeGlobalStateFileBackedEffStorage(int nObjectBasePlus0x15c) {
    DWORD cbPacket;
    unsigned int count;
    unsigned char *pByte;
    unsigned int i;
    void *pZeroBuffer;
    void *pFileWrapper;

    ((void (*)(int))(*(void **)(*(int *)(*(int *)(*(int *)(nObjectBasePlus0x15c - 0x15c) + 0x14) + nObjectBasePlus0x15c - 0x15c) +
                                 0x20)))(0);
    WriteEffStreamToLockedFile(
        (void *)(unsigned long)(*(int *)(*(int *)(nObjectBasePlus0x15c - 0x15c) + 0x20) + nObjectBasePlus0x15c - 0x15c),
        (int *)(unsigned long)(nObjectBasePlus0x15c - 0x158),
        0,
        0);
    WriteEffStreamToLockedFile(
        (void *)(unsigned long)(*(int *)(*(int *)(nObjectBasePlus0x15c - 0x15c) + 0x20) + nObjectBasePlus0x15c - 0x15c),
        (int *)(unsigned long)g_pGlobalStateEff512ByteStream,
        0,
        0);
    WriteEffStreamToLockedFile(
        (void *)(unsigned long)(*(int *)(*(int *)(nObjectBasePlus0x15c - 0x15c) + 0x20) + nObjectBasePlus0x15c - 0x15c),
        (int *)(unsigned long)*(int *)(*(int *)(*(int *)(nObjectBasePlus0x15c - 0x15c) + 0x20) + nObjectBasePlus0x15c - 0x15c),
        0,
        0);

    pZeroBuffer = AllocateVSMemBlock(g_cbEffTransportMaxPacketBytes);
    cbPacket = g_cbEffTransportMaxPacketBytes;
    pByte = (unsigned char *)pZeroBuffer;
    for (count = g_cbEffTransportMaxPacketBytes >> 2; count != 0; --count) {
        *(int *)pByte = 0;
        pByte += 4;
    }
    for (count = cbPacket & 3; count != 0; --count) {
        *pByte++ = 0;
    }

    pFileWrapper =
        (void *)(unsigned long)(*(int *)(*(int *)(*(int *)(nObjectBasePlus0x15c - 0x15c) + 0x14) + nObjectBasePlus0x15c - 0x15c));
    i = 0;
    if (0 < *(int *)(*(int *)(*(int *)(nObjectBasePlus0x15c - 0x15c) + 0x18) + nObjectBasePlus0x15c - 0x154)) {
        do {
            ++i;
            WriteWin32FileWrapper(pFileWrapper, pZeroBuffer, g_cbEffTransportMaxPacketBytes);
        } while (i < *(int *)(*(int *)(*(int *)(nObjectBasePlus0x15c - 0x15c) + 0x18) + nObjectBasePlus0x15c - 0x154));
    }
    FreeVSMemBlock(pZeroBuffer);
}
