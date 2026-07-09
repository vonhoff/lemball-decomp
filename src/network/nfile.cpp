#include "../game/game_app.h"
#include "../engine/memory_arena.h"

extern int ReturnTrueVtableCallback(void);

static void *g_NETWORK_ReturnTrueVtable[1] = {
    (void *)ReturnTrueVtableCallback,
};

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
static void *g_NETWORK_RuntimeChannelStackFatalThunkVtable = (void *)0x004990D8;
static void *g_NETWORK_CrtFatalRuntimeErrorThunk = (void *)0x00498070;
static void *g_NETWORK_OpenWin32FileWrapperVtable = (void *)0x0049A628;
static void *g_NETWORK_CrtFatalRuntimeErrorDeleteThunk = (void *)0x0049A53C;
static void *g_NETWORK_OpenWin32FileWrapperOpenThunk = (void *)0x0049A500;
static void *g_NETWORK_MapNestedEffCallbackFailureThunk = (void *)0x0049A4F0;

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

struct NETWORK_ConstructionAdjustorVtable;

struct NETWORK_EffStreamRecordSlotTable {
    void *m_pVtable;
    int m_nRecordHeaderOffset04;
    int m_nPayloadBaseOffset08;
    NETWORK_ConstructionAdjustorVtable *m_pChannelThunk0c;
    int m_nPendingSlot10;
    int m_nReserved14;
    int m_nAccumulatedStreamLength18;
    int m_nReserved1c;
    int m_nReserved20;
    int m_nReserved24;
    int m_nReserved28;
    NETWORK_EffStreamRecordSlot *m_pSlots;
    unsigned short *m_pwSlotFlags;
    int m_cSlots;
};

struct NETWORK_ConstructionAdjustorVtable {
    void *m_pReserved00;
    int m_nPrimaryOffset;
    int m_nSecondaryOffset;
    int m_nTertiaryOffset;
    int m_nQuaternaryOffset;
    int m_nFatalOffset;
    int m_nRecordTableOffsetA;
    int m_nRecordTableOffsetB;
};

struct NETWORK_FileBackedDispatchOffsets {
    void *m_pReserved00;
    int m_nPrimaryOffset;
    int m_nSecondaryOffset;
    int m_nTertiaryOffset;
    int m_nQuaternaryOffset;
    int m_nReadThunkOffset14;
    int m_nSlotCountOffset18;
    int m_nWriteThunkOffset20;
};

struct NETWORK_FileBackedMarkerOffsets {
    void *m_pReserved00;
    int m_nReserved04;
    int m_nReserved08;
    int m_nReserved0c;
    int m_nReadThunkOffset14;
    int m_nNestedThunkOffset18;
    int m_nStreamOffset1c;
    int m_nWriteThunkOffset20;
};

struct NETWORK_FileBackedAppendOffsets {
    void *m_pReserved00;
    int m_nOutputStateOffset04;
    int m_nNameProviderOffset08;
    int m_nFileWrapperOffset0c;
};

struct NETWORK_AdjustorSubobject {
    int m_nThisDelta;
    void *m_pVtable;
};

struct NETWORK_ThunkAdjustorGroup {
    NETWORK_ConstructionAdjustorVtable *m_pOffsets;
    NETWORK_AdjustorSubobject *m_pPrimary;
    NETWORK_AdjustorSubobject *m_pSecondary;
    NETWORK_AdjustorSubobject *m_pTertiary;
};

struct NETWORK_QuadThunkAdjustorGroup {
    NETWORK_ConstructionAdjustorVtable *m_pOffsets;
    NETWORK_AdjustorSubobject *m_pPrimary;
    NETWORK_AdjustorSubobject *m_pSecondary;
    NETWORK_AdjustorSubobject *m_pTertiary;
    NETWORK_AdjustorSubobject *m_pQuaternary;
};

struct NETWORK_QuintThunkAdjustorGroup {
    NETWORK_ConstructionAdjustorVtable *m_pOffsets;
    NETWORK_AdjustorSubobject *m_pPrimary;
    NETWORK_AdjustorSubobject *m_pSecondary;
    NETWORK_AdjustorSubobject *m_pTertiary;
    NETWORK_AdjustorSubobject *m_pQuaternary;
    NETWORK_AdjustorSubobject *m_pFatal;
};

struct NETWORK_EffTransportRuntimeServiceVtable {
    char *(*m_pGetLocalHostName)(void);
    void *m_pReserved04;
    void (*m_pSelectLocalHostName)(char *pszHostName);
};

struct NETWORK_EffTransportRuntimeService {
    NETWORK_EffTransportRuntimeServiceVtable *m_pVtable;
};

struct NETWORK_FileWrapperOpenVtable {
    int (*m_pOpenWithMode)(int nHandle, int nAccessMode, int fCreate);
    int (*m_pOpenFallback)(int nHandle, int nAccessMode);
};

struct NETWORK_FileWrapperIoVtable {
    void *m_apReserved00[8];
    int (*m_pSeek)(DWORD dwOffset);
    DWORD (*m_pGetOffset)(void);
};

struct NETWORK_FileWrapperObject {
    NETWORK_FileWrapperIoVtable *m_pVtable;
};

struct NETWORK_FileWrapperOpenObject {
    NETWORK_FileWrapperOpenVtable *m_pVtable;
};

struct NETWORK_CallbackThunkVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void *m_pReserved08;
    int (*m_pInvoke)(void);
    void (*m_pRelease)(void);
};

struct NETWORK_CallbackThunkObject {
    NETWORK_CallbackThunkVtable *m_pVtable;
};

struct NETWORK_DeleteVtable {
    void *m_apReserved00[5];
    void (*m_pDelete)(int);
};

struct NETWORK_DeleteObject {
    NETWORK_DeleteVtable *m_pVtable;
};

struct NETWORK_ChannelStateHeader {
    void **m_pVtable;
    int m_nReserved04;
    int m_nSelectedPeer08;
    int m_nReserved0c;
    int m_fBusy10;
    int m_cSlots14;
    int m_fPending18;
    int m_fReceiving1c;
    unsigned short m_nSelectedPort20;
    unsigned short m_nReserved22;
    int m_fChannelOpen24;
};

struct NETWORK_PeerAddressServiceVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void (*m_pSelectPeerName)(char *pszName);
};

struct NETWORK_PeerAddressService {
    NETWORK_PeerAddressServiceVtable *m_pVtable;
};

struct NETWORK_TimedStreamHeader {
    void **m_pVtable;
    int m_nReserved04;
    int m_pPayloadBuffer08;
    int m_fOwnsPayload0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nBufferEnd18;
    int m_nReserved1c;
    int m_nReserved20;
    int m_nReserved24;
    int m_fBusy28;
    int *m_pTagBuffer2c;
    unsigned short m_nWord30;
    unsigned short m_nWord32;
    unsigned short m_nWord34;
    unsigned short m_nWord36;
    int m_nReserved38;
    DWORD m_dwLastTick3c;
};

struct NETWORK_ChannelStateRuntimeVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void (*m_pHandleReadFailure)(void);
};

struct NETWORK_TimedStreamRuntimeVtable {
    void *m_apReserved00[6];
    void (*m_pServiceLoadedPacket)(void);
};

struct NETWORK_FileBackedEffRecordCursor {
    void **m_pVtable;
    int m_nRecordHeaderOffset04;
    int m_nPayloadBaseOffset08;
    NETWORK_AdjustorSubobject m_ChannelStateThunk0c;
    NETWORK_AdjustorSubobject m_TimedStreamThunk1c;
    NETWORK_AdjustorSubobject m_FileWrapperThunk2c;
    int m_nCurrentSlot3c;
};

struct NETWORK_FileBackedMarkerStream {
    int m_anReserved00[11];
    unsigned short m_wReferenceCount2c;
    unsigned short m_wReserved2e;
};

struct NETWORK_FileBackedMarkerState {
    void **m_pVtable;
    NETWORK_FileBackedMarkerStream m_Stream04;
};

struct NETWORK_SlotCountHolder {
    char m_abReserved00[8];
    int m_cSlots;
};

struct NETWORK_StreamNameProviderVtable {
    char *(*m_pGetName)(void);
};

struct NETWORK_StreamNameProvider {
    NETWORK_StreamNameProviderVtable *m_pVtable;
};

struct NETWORK_SideBufferOwnerA4 {
    char m_abReserved00[0xa4];
    void *m_pResetThunk;
    void *m_pSideBuffer;
};

struct NETWORK_SideBufferOwner104 {
    char m_abReserved00[0x104];
    void *m_pResetThunk;
    void *m_pSideBuffer;
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

    pRecordSlot = (NETWORK_EffStreamRecordSlot *)pSlot;
    pRecordSlot->m_pVtable = g_NETWORK_ReturnTrueVtable;
    pRecordSlot->m_nReserved04 = 0;
    ResetEffStreamStateFields(pSlot);
    pRecordSlot->m_wObservedMarker = 1;
    *(int *)((char *)pSlot + 0x18) = *(int *)((char *)pSlot + 0x18) + 0x30;
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

    pTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    pTable->m_pVtable = g_NETWORK_ReturnTrueVtable;
    pTable->m_nRecordHeaderOffset04 = 0;
    ResetEffStreamStateFields(pObject);
    pTable->m_cSlots = cSlots;
    pTable->m_pVtable = g_NETWORK_EffRecordSlotTableVtable;

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
    i = 0;
    if (0 < pTable->m_cSlots) {
        do {
            pTable->m_pwSlotFlags[i] = 0;
            ++i;
        } while (i < pTable->m_cSlots);
    }

    pTable->m_nAccumulatedStreamLength18 += *(int *)((char *)&pTable->m_pSlots[0] + 0x18) * cSlots;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479810
void *ConstructEff512ByteStateStream(void *pObject) {
    NETWORK_EffStreamRecordSlotTable *pStateStream;
    int i;
    unsigned char *pbPayload;

    pStateStream = (NETWORK_EffStreamRecordSlotTable *)pObject;
    pStateStream->m_pVtable = g_NETWORK_ReturnTrueVtable;
    i = 0;
    pStateStream->m_nRecordHeaderOffset04 = 0;
    ResetEffStreamStateFields(pObject);
    pStateStream->m_pVtable = g_NETWORK_EffStateStreamVtable;
    pbPayload = (unsigned char *)AllocateVSMemBlock(0x200);
    *(unsigned char **)((char *)pObject + 0x2c) = pbPayload;
    pStateStream->m_nAccumulatedStreamLength18 += 0x200;
    do {
        ++i;
        pbPayload[i - 1] = 0;
    } while (i < 0x200);
    return pObject;
}

// FUNCTION: LEMBALL 0x00479930
void *ConstructDualFileBackedEffChannel(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_QuadThunkAdjustorGroup kFinalThunkGroup;
    NETWORK_ThunkAdjustorGroup kFileWrapperThunkGroup;

    if (fConstructEmbeddedObjects != 0) {
        *(void **)((char *)pObject + 0xc) = g_NETWORK_FileBackedDualChannelStateVtable;
        *(void **)((char *)pObject + 0x88) = g_NETWORK_FileBackedDualStreamVtable;
        *(void **)((char *)pObject + 0xb4) = g_NETWORK_FileBackedDualThunkVtable;
        ConstructEffStreamChannelState((char *)pObject + 0x18);
        ConstructDualHandleEffStream((char *)pObject + 0x44, 0);
        *(void **)((char *)pObject + 0x9c) = g_NETWORK_CrtFatalRuntimeErrorThunk;
        *(int *)((char *)pObject + 0xa0) = 0;
        *(int *)((char *)pObject + 0xa4) = 1;
        *(void **)((char *)pObject + 0x9c) = g_NETWORK_OpenWin32FileWrapperVtable;
        *(void **)((char *)pObject + 0xb0) = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        kFileWrapperThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)((char *)pObject + 0xb4);
        kFileWrapperThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xb4 +
                                                                          kFileWrapperThunkGroup.m_pOffsets->m_nPrimaryOffset);
        kFileWrapperThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xb4 +
                                                                            kFileWrapperThunkGroup.m_pOffsets->m_nSecondaryOffset);
        kFileWrapperThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_OpenWin32FileWrapperOpenThunk;
        kFileWrapperThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunk;
    }

    ClearFileBackedEffChannelStateWord(pObject);
    kFinalThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)((char *)pObject + 0xc);
    kFinalThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xc +
                                                                kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset + 8);
    kFinalThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xc +
                                                                  kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset + 8);
    kFinalThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xc +
                                                                 kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset + 8);
    kFinalThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xc +
                                                                   kFinalThunkGroup.m_pOffsets->m_nQuaternaryOffset + 8);
    kFinalThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_FileBackedDualPrimaryThunkVtable;
    kFinalThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_FileBackedDualSecondaryThunkVtable;
    kFinalThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_FileBackedDualFileThunkVtable;
    kFinalThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_FileBackedDualFatalThunkVtable;
    kFinalThunkGroup.m_pPrimary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset - 0xc;
    *(int *)((char *)pObject + 0x10) = -1;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479E20
void *ConstructTimedFileBackedEffChannel(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_QuadThunkAdjustorGroup kFinalThunkGroup;
    NETWORK_ThunkAdjustorGroup kFileWrapperThunkGroup;

    if (fConstructEmbeddedObjects != 0) {
        *(void **)((char *)pObject + 0xc) = g_NETWORK_FileBackedTimedChannelStateVtable;
        *(void **)((char *)pObject + 0x8c) = g_NETWORK_FileBackedTimedStreamVtable;
        *(void **)((char *)pObject + 0xd4) = g_NETWORK_FileBackedTimedThunkVtable;
        ConstructEffStreamChannelState((char *)pObject + 0x18);
        ConstructTimedEffStream((char *)pObject + 0x48, 0);
        *(void **)((char *)pObject + 0xbc) = g_NETWORK_CrtFatalRuntimeErrorThunk;
        *(int *)((char *)pObject + 0xc0) = 0;
        *(int *)((char *)pObject + 0xc4) = 1;
        *(void **)((char *)pObject + 0xbc) = g_NETWORK_OpenWin32FileWrapperVtable;
        *(void **)((char *)pObject + 0xd0) = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        kFileWrapperThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)((char *)pObject + 0xd4);
        kFileWrapperThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xd4 +
                                                                          kFileWrapperThunkGroup.m_pOffsets->m_nPrimaryOffset);
        kFileWrapperThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xd4 +
                                                                            kFileWrapperThunkGroup.m_pOffsets->m_nSecondaryOffset);
        kFileWrapperThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_OpenWin32FileWrapperOpenThunk;
        kFileWrapperThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunk;
    }

    ClearFileBackedEffChannelStateWord(pObject);
    kFinalThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)((char *)pObject + 0xc);
    kFinalThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xc +
                                                                kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset + 8);
    kFinalThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xc +
                                                                  kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset + 8);
    kFinalThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xc +
                                                                 kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset + 8);
    kFinalThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0xc +
                                                                   kFinalThunkGroup.m_pOffsets->m_nQuaternaryOffset + 8);
    kFinalThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_FileBackedTimedPrimaryThunkVtable;
    kFinalThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_FileBackedTimedSecondaryThunkVtable;
    kFinalThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_FileBackedTimedFileThunkVtable;
    kFinalThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_FileBackedTimedFatalThunkVtable;
    kFinalThunkGroup.m_pPrimary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset - 0xc;
    kFinalThunkGroup.m_pSecondary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x3c;
    *(int *)((char *)pObject + 0x10) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479F40
void RestoreTimedFileBackedEffChannelVtables(int nTimedFileBasePlus0x0c) {
    char *pbChannelBase;
    NETWORK_QuadThunkAdjustorGroup kFinalThunkGroup;

    pbChannelBase = (char *)(unsigned long)(nTimedFileBasePlus0x0c - 0xc);
    kFinalThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)pbChannelBase;
    kFinalThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbChannelBase + kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset + 8);
    kFinalThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbChannelBase + kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset + 8);
    kFinalThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbChannelBase + kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset + 8);
    kFinalThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)(pbChannelBase +
                                                                   kFinalThunkGroup.m_pOffsets->m_nQuaternaryOffset + 8);
    kFinalThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_FileBackedTimedPrimaryThunkVtable;
    kFinalThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_FileBackedTimedSecondaryThunkVtable;
    kFinalThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_FileBackedTimedFileThunkVtable;
    kFinalThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_FileBackedTimedFatalThunkVtable;
    kFinalThunkGroup.m_pPrimary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset - 0xc;
    kFinalThunkGroup.m_pSecondary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x3c;
}

// FUNCTION: LEMBALL 0x0047A220
void *ConstructEmbeddedFileBackedEffChannelStack(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_ThunkAdjustorGroup kEmbeddedThunkGroup;
    NETWORK_ThunkAdjustorGroup kFileWrapperThunkGroup;
    NETWORK_QuintThunkAdjustorGroup kFinalThunkGroup;
    char *pbDualThunkBase;
    char *pbObjectBase;

    pbObjectBase = (char *)pObject;
    if (fConstructEmbeddedObjects != 0) {
        *(void **)(pbObjectBase + 0x7c) = g_NETWORK_EmbeddedFileBackedTimedStreamVtable;
        *(void **)(pbObjectBase + 0xf4) = g_NETWORK_EmbeddedFileBackedDualStreamVtable;
        *(void **)(pbObjectBase + 0x108) = g_NETWORK_EmbeddedFileBackedDualThunkVtable;
        *(void **)(pbObjectBase + 0x124) = g_NETWORK_EmbeddedFileBackedFileWrapperThunkVtable;
        *(void **)pbObjectBase = g_NETWORK_EmbeddedFileBackedBaseVtable;
        *(void **)(pbObjectBase + 0x138) = g_NETWORK_EmbeddedFileBackedPrimaryThunkVtable;
        *(void **)(pbObjectBase + 0x14c) = g_NETWORK_EmbeddedFileBackedTimedFileViewVtable;

        ConstructEffStreamChannelState(pbObjectBase + 8);
        ConstructTimedEffStream(pbObjectBase + 0x38, 0);
        ConstructDualHandleEffStream(pbObjectBase + 0xb0, 0);

        pbDualThunkBase = pbObjectBase + 0x108;
        kEmbeddedThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)pbDualThunkBase;
        kEmbeddedThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbDualThunkBase +
                                                                       kEmbeddedThunkGroup.m_pOffsets->m_nPrimaryOffset -
                                                                       4);
        kEmbeddedThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbDualThunkBase +
                                                                         kEmbeddedThunkGroup.m_pOffsets->m_nSecondaryOffset -
                                                                         4);
        kEmbeddedThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbDualThunkBase +
                                                                        kEmbeddedThunkGroup.m_pOffsets->m_nTertiaryOffset -
                                                                        4);
        kEmbeddedThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunkVtable;
        kEmbeddedThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_ReturnTrueVtable;
        kEmbeddedThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_ReturnTrueVtable;
        kEmbeddedThunkGroup.m_pPrimary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nPrimaryOffset - 8;
        kEmbeddedThunkGroup.m_pSecondary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x38;
        kEmbeddedThunkGroup.m_pTertiary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xb0;

        *(void **)(pbObjectBase + 0x10c) = g_NETWORK_CrtFatalRuntimeErrorThunk;
        *(int *)(pbObjectBase + 0x110) = 0;
        *(int *)(pbObjectBase + 0x114) = 1;
        *(void **)(pbObjectBase + 0x10c) = g_NETWORK_OpenWin32FileWrapperVtable;
        *(void **)(pbObjectBase + 0x120) = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        kFileWrapperThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)(pbObjectBase + 0x124);
        kFileWrapperThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x124 +
                                                                          kFileWrapperThunkGroup.m_pOffsets->m_nPrimaryOffset);
        kFileWrapperThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x124 +
                                                                            kFileWrapperThunkGroup.m_pOffsets->m_nSecondaryOffset);
        kFileWrapperThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_OpenWin32FileWrapperOpenThunk;
        kFileWrapperThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunk;

        ConstructDualFileBackedEffChannel(pbObjectBase + 300, 0);
        ConstructTimedFileBackedEffChannel(pbObjectBase + 0x140, 0);
    }

    kFinalThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)pbObjectBase;
    kFinalThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbObjectBase + kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kFinalThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbObjectBase + kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset - 4);
    kFinalThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbObjectBase + kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset - 4);
    kFinalThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)(pbObjectBase +
                                                                   kFinalThunkGroup.m_pOffsets->m_nQuaternaryOffset - 4);
    kFinalThunkGroup.m_pFatal = (NETWORK_AdjustorSubobject *)(pbObjectBase + kFinalThunkGroup.m_pOffsets->m_nFatalOffset - 4);
    kFinalThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalPrimaryThunkVtable;
    kFinalThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalTimedThunkVtable;
    kFinalThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalDualThunkVtable;
    kFinalThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalFileThunkVtable;
    kFinalThunkGroup.m_pFatal->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalFatalThunkVtable;
    kFinalThunkGroup.m_pPrimary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset - 8;
    kFinalThunkGroup.m_pSecondary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x38;
    kFinalThunkGroup.m_pTertiary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xb0;
    return pObject;
}

// FUNCTION: LEMBALL 0x0047A570
void *ConstructGlobalStateFileBackedEffComposite(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_ThunkAdjustorGroup kEmbeddedThunkGroup;
    NETWORK_ThunkAdjustorGroup kFileWrapperThunkGroup;
    NETWORK_QuintThunkAdjustorGroup kInitialThunkGroup;
    NETWORK_QuintThunkAdjustorGroup kFinalThunkGroup;
    NETWORK_EffTransportRuntimeService *pRuntimeService;
    char *pbObjectBase;
    char *pbEmbeddedStackBase;
    char *pbInitialStreamBase;
    void *pStorage;

    pbObjectBase = (char *)pObject;
    if (fConstructEmbeddedObjects != 0) {
        *(void **)(pbObjectBase + 0x20) = g_NETWORK_GlobalStateCompositeTimedStreamVtable;
        *(void **)(pbObjectBase + 4) = g_NETWORK_GlobalStateCompositeTransportVtable;
        *(void **)(pbObjectBase + 0xd4) = g_NETWORK_GlobalStateCompositeDualStreamVtable;
        *(void **)(pbObjectBase + 0x14c) = g_NETWORK_GlobalStateCompositeDualThunkVtable;
        *(void **)(pbObjectBase + 0x160) = g_NETWORK_GlobalStateCompositeFileWrapperVtable;
        *(void **)(pbObjectBase + 0x180) = g_NETWORK_GlobalStateCompositeTimedFileThunkVtable;
        *(void **)(pbObjectBase + 0x194) = g_NETWORK_GlobalStateCompositeEmbeddedStackVtable;
        *(void **)(pbObjectBase + 0x1a8) = g_NETWORK_GlobalStateCompositeEmbeddedStackDataVtable;
        *(void **)(pbObjectBase + 0x1b0) = g_NETWORK_GlobalStateCompositeEmbeddedStackBaseVtable;

        ConstructEffStreamChannelState(pbObjectBase + 0x60);
        ConstructTimedEffStream(pbObjectBase + 0x90, 0);
        ConstructDualHandleEffStream(pbObjectBase + 0x108, 0);

        pbEmbeddedStackBase = pbObjectBase + 0x160;
        kEmbeddedThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)pbEmbeddedStackBase;
        kEmbeddedThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase +
                                                                       kEmbeddedThunkGroup.m_pOffsets->m_nPrimaryOffset -
                                                                       4);
        kEmbeddedThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase +
                                                                         kEmbeddedThunkGroup.m_pOffsets->m_nSecondaryOffset -
                                                                         4);
        kEmbeddedThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase +
                                                                        kEmbeddedThunkGroup.m_pOffsets->m_nTertiaryOffset -
                                                                        4);
        kEmbeddedThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunkVtable;
        kEmbeddedThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_ReturnTrueVtable;
        kEmbeddedThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_ReturnTrueVtable;
        kEmbeddedThunkGroup.m_pPrimary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nPrimaryOffset - 8;
        kEmbeddedThunkGroup.m_pSecondary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x38;
        kEmbeddedThunkGroup.m_pTertiary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xb0;

        *(int *)(pbObjectBase + 0x168) = 0;
        *(void **)(pbObjectBase + 0x164) = g_NETWORK_CrtFatalRuntimeErrorThunk;
        *(int *)(pbObjectBase + 0x16c) = 1;
        *(void **)(pbObjectBase + 0x164) = g_NETWORK_OpenWin32FileWrapperVtable;
        *(void **)(pbObjectBase + 0x17c) = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        kFileWrapperThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)(pbObjectBase + 0x180);
        kFileWrapperThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x180 +
                                                                          kFileWrapperThunkGroup.m_pOffsets->m_nPrimaryOffset);
        kFileWrapperThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x180 +
                                                                            kFileWrapperThunkGroup.m_pOffsets->m_nSecondaryOffset);
        kFileWrapperThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_OpenWin32FileWrapperOpenThunk;
        kFileWrapperThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunk;

        ConstructDualFileBackedEffChannel(pbObjectBase + 0x188, 0);
        ConstructTimedFileBackedEffChannel(pbObjectBase + 0x19c, 0);
        ConstructEmbeddedFileBackedEffChannelStack(pbObjectBase + 0x1b0, 0);
    }

    *(int *)(pbObjectBase + 0x28) = 0;
    pbInitialStreamBase = pbObjectBase + 0x24;
    *(void **)pbInitialStreamBase = g_NETWORK_ReturnTrueVtable;
    ResetEffStreamStateFields(pbInitialStreamBase);
    *(int *)(pbObjectBase + 0x3c) = *(int *)(pbObjectBase + 0x3c) + 2;
    *(unsigned short *)(pbObjectBase + 0x50) = 0;
    *(void **)pbInitialStreamBase = g_NETWORK_GlobalStateCompositeInitialVtable;

    kInitialThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)(pbObjectBase + 0x20);
    kInitialThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 +
                                                                  kInitialThunkGroup.m_pOffsets->m_nPrimaryOffset + 0x1c);
    kInitialThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 +
                                                                    kInitialThunkGroup.m_pOffsets->m_nSecondaryOffset + 0x1c);
    kInitialThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 +
                                                                   kInitialThunkGroup.m_pOffsets->m_nTertiaryOffset + 0x1c);
    kInitialThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 +
                                                                     kInitialThunkGroup.m_pOffsets->m_nQuaternaryOffset + 0x1c);
    kInitialThunkGroup.m_pFatal = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 +
                                                                kInitialThunkGroup.m_pOffsets->m_nFatalOffset + 0x1c);
    kInitialThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable;
    kInitialThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable;
    kInitialThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_GlobalStateCompositeInitialDualThunkVtable;
    kInitialThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_GlobalStateCompositeInitialFileThunkVtable;
    kInitialThunkGroup.m_pFatal->m_pVtable = g_NETWORK_GlobalStateCompositeInitialFatalThunkVtable;
    kInitialThunkGroup.m_pPrimary->m_nThisDelta = 0;
    kInitialThunkGroup.m_pSecondary->m_nThisDelta = 0;
    kInitialThunkGroup.m_pTertiary->m_nThisDelta = 0;

    ConstructEffTransportRuntimeChannelStack(pObject, 0);

    *(void **)pObject = g_NETWORK_GlobalStateCompositeFinalVtable;
    kFinalThunkGroup.m_pOffsets = kInitialThunkGroup.m_pOffsets;
    kFinalThunkGroup.m_pPrimary = kInitialThunkGroup.m_pPrimary;
    kFinalThunkGroup.m_pSecondary = kInitialThunkGroup.m_pSecondary;
    kFinalThunkGroup.m_pTertiary = kInitialThunkGroup.m_pTertiary;
    kFinalThunkGroup.m_pQuaternary = kInitialThunkGroup.m_pQuaternary;
    kFinalThunkGroup.m_pFatal = kInitialThunkGroup.m_pFatal;
    kFinalThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable;
    kFinalThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable;
    kFinalThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_GlobalStateCompositeFinalDualThunkVtable;
    kFinalThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_GlobalStateCompositeFinalFileThunkVtable;
    kFinalThunkGroup.m_pFatal->m_pVtable = g_NETWORK_GlobalStateCompositeFinalFatalThunkVtable;
    kFinalThunkGroup.m_pPrimary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x40;
    kFinalThunkGroup.m_pSecondary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x70;
    kFinalThunkGroup.m_pTertiary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xe8;
    kFinalThunkGroup.m_pFatal->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nFatalOffset - 0x15c;

    pRuntimeService = (NETWORK_EffTransportRuntimeService *)g_pEffTransportRuntimeService;
    pRuntimeService->m_pVtable->m_pSelectLocalHostName(g_pszFileBasedNetworkLocalHostName);

    if (g_pGlobalStateEff512ByteStream == 0) {
        pStorage = AllocateVSMemBlock(0x30);
        if (pStorage == 0) {
            g_pGlobalStateEff512ByteStream = 0;
        } else {
            g_pGlobalStateEff512ByteStream = ConstructEff512ByteStateStream(pStorage);
        }
    }

    *(int *)(pbObjectBase + kFinalThunkGroup.m_pOffsets->m_nFatalOffset + 8) = 0x14;
    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(pbObjectBase + 0x20 + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetB) = 0;
    } else {
        *(void **)(pbObjectBase + 0x20 + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetB) =
            ConstructEffStreamRecordSlotTable(pStorage, *(int *)(pbObjectBase + kFinalThunkGroup.m_pOffsets->m_nFatalOffset + 8));
    }

    *(int *)(pbObjectBase + 0x20 + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetA) =
        *(int *)(pbObjectBase + 0x20 + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetB);
    *(int *)(pbObjectBase + 0x24 + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetB) =
        *(int *)((char *)g_pGlobalStateEff512ByteStream + 0x18) + *(int *)(pbObjectBase + 0x3c);
    *(int *)(pbObjectBase + 0x24 + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetA) =
        *(int *)(pbObjectBase + 0x24 + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetB);
    *(int *)((char *)pObject + 0x54) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x0047A910
void DestroyGlobalStateFileBackedEffComposite(int nObjectBasePlus0x60) {
    NETWORK_QuintThunkAdjustorGroup kFinalThunkGroup;
    char *pbObjectBase;
    NETWORK_DeleteObject *pRecordSlotTable;

    pbObjectBase = (char *)(unsigned long)(nObjectBasePlus0x60 - 0x60);
    *(void **)pbObjectBase = g_NETWORK_GlobalStateCompositeFinalVtable;
    kFinalThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)(pbObjectBase + 0x20);
    kFinalThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 +
                                                                kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset + 0x1c);
    kFinalThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 +
                                                                  kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset + 0x1c);
    kFinalThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 +
                                                                 kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset + 0x1c);
    kFinalThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 +
                                                                   kFinalThunkGroup.m_pOffsets->m_nQuaternaryOffset + 0x1c);
    kFinalThunkGroup.m_pFatal = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 +
                                                              kFinalThunkGroup.m_pOffsets->m_nFatalOffset + 0x1c);
    kFinalThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable;
    kFinalThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable;
    kFinalThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_GlobalStateCompositeFinalDualThunkVtable;
    kFinalThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_GlobalStateCompositeFinalFileThunkVtable;
    kFinalThunkGroup.m_pFatal->m_pVtable = g_NETWORK_GlobalStateCompositeFinalFatalThunkVtable;
    kFinalThunkGroup.m_pPrimary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x40;
    kFinalThunkGroup.m_pSecondary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x70;
    kFinalThunkGroup.m_pTertiary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xe8;
    kFinalThunkGroup.m_pFatal->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nFatalOffset - 0x15c;

    pRecordSlotTable = *(NETWORK_DeleteObject **)(pbObjectBase + 0x20 + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetA);
    if (pRecordSlotTable != 0) {
        pRecordSlotTable->m_pVtable->m_pDelete(1);
    }

    if (g_pszFileBasedNetworkConfiguredPath != 0) {
        FreeVSMemBlock(g_pszFileBasedNetworkConfiguredPath);
        g_pszFileBasedNetworkConfiguredPath = 0;
    }

    if (g_pGlobalStateEff512ByteStream != 0) {
        ((NETWORK_DeleteObject *)g_pGlobalStateEff512ByteStream)->m_pVtable->m_pDelete(1);
        g_pGlobalStateEff512ByteStream = 0;
    }

    ReleaseEffTransportRuntimeBuffers(nObjectBasePlus0x60 - 0x3c);
    DestroyTimedEffStream((void *)((((nObjectBasePlus0x60 == 0x60) - 1) & (int)(unsigned long)(pbObjectBase + 0x20)) + 4));
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
    NETWORK_ThunkAdjustorGroup kEmbeddedThunkGroup;
    NETWORK_ThunkAdjustorGroup kFileWrapperThunkGroup;
    NETWORK_QuintThunkAdjustorGroup kInitialThunkGroup;
    NETWORK_QuintThunkAdjustorGroup kFinalThunkGroup;
    char *pbObjectBase;
    char *pbEmbeddedStackBase;
    char *pbInitialStreamBase;
    void *pStorage;

    pbObjectBase = (char *)pObject;

    if (fConstructEmbeddedObjects != 0) {
        *(void **)(pbObjectBase + 0x2c) = g_NETWORK_DualSlotCompositeTimedStreamVtable;
        *(void **)(pbObjectBase + 4) = g_NETWORK_DualSlotCompositeTransportVtable;
        *(void **)(pbObjectBase + 0xd8) = g_NETWORK_DualSlotCompositeDualStreamVtable;
        *(void **)(pbObjectBase + 0x150) = g_NETWORK_DualSlotCompositeDualThunkVtable;
        *(void **)(pbObjectBase + 0x164) = g_NETWORK_DualSlotCompositeFileWrapperVtable;
        *(void **)(pbObjectBase + 0x184) = g_NETWORK_DualSlotCompositeTimedFileThunkVtable;
        *(void **)(pbObjectBase + 0x198) = g_NETWORK_DualSlotCompositeEmbeddedStackVtable;
        *(void **)(pbObjectBase + 0x1ac) = g_NETWORK_DualSlotCompositeEmbeddedStackDataVtable;
        *(void **)(pbObjectBase + 0x1b4) = g_NETWORK_DualSlotCompositeEmbeddedStackBaseVtable;

        ConstructEffStreamChannelState(pbObjectBase + 100);
        ConstructTimedEffStream(pbObjectBase + 0x94, 0);
        ConstructDualHandleEffStream(pbObjectBase + 0x10c, 0);

        pbEmbeddedStackBase = pbObjectBase + 0x164;
        kEmbeddedThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)pbEmbeddedStackBase;
        kEmbeddedThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase +
                                                                       kEmbeddedThunkGroup.m_pOffsets->m_nPrimaryOffset -
                                                                       4);
        kEmbeddedThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase +
                                                                         kEmbeddedThunkGroup.m_pOffsets->m_nSecondaryOffset -
                                                                         4);
        kEmbeddedThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase +
                                                                        kEmbeddedThunkGroup.m_pOffsets->m_nTertiaryOffset -
                                                                        4);
        kEmbeddedThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunkVtable;
        kEmbeddedThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_ReturnTrueVtable;
        kEmbeddedThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_ReturnTrueVtable;
        kEmbeddedThunkGroup.m_pPrimary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nPrimaryOffset - 8;
        kEmbeddedThunkGroup.m_pSecondary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x38;
        kEmbeddedThunkGroup.m_pTertiary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xb0;

        *(int *)(pbObjectBase + 0x16c) = 0;
        *(void **)(pbObjectBase + 0x168) = g_NETWORK_CrtFatalRuntimeErrorThunk;
        *(int *)(pbObjectBase + 0x170) = 1;
        *(void **)(pbObjectBase + 0x168) = g_NETWORK_OpenWin32FileWrapperVtable;
        *(void **)(pbObjectBase + 0x180) = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        kFileWrapperThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)(pbObjectBase + 0x184);
        kFileWrapperThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x184 +
                                                                          kFileWrapperThunkGroup.m_pOffsets->m_nPrimaryOffset);
        kFileWrapperThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x184 +
                                                                            kFileWrapperThunkGroup.m_pOffsets->m_nSecondaryOffset);
        kFileWrapperThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_OpenWin32FileWrapperOpenThunk;
        kFileWrapperThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunk;

        ConstructDualFileBackedEffChannel(pbObjectBase + 0x18c, 0);
        ConstructTimedFileBackedEffChannel(pbObjectBase + 0x1a0, 0);
        ConstructEmbeddedFileBackedEffChannelStack(pbObjectBase + 0x1b4, 0);
    }

    *(int *)(pbObjectBase + 0x34) = 0;
    pbInitialStreamBase = pbObjectBase + 0x30;
    *(void **)pbInitialStreamBase = g_NETWORK_ReturnTrueVtable;
    ResetEffStreamStateFields(pbInitialStreamBase);
    *(int *)(pbObjectBase + 0x48) = *(int *)(pbObjectBase + 0x48) + 2;
    *(unsigned short *)(pbObjectBase + 0x5c) = 0;
    *(void **)pbInitialStreamBase = g_NETWORK_GlobalStateCompositeInitialVtable;

    kInitialThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)(pbObjectBase + 0x2c);
    kInitialThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c +
                                                                  kInitialThunkGroup.m_pOffsets->m_nPrimaryOffset + 0x28);
    kInitialThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c +
                                                                    kInitialThunkGroup.m_pOffsets->m_nSecondaryOffset + 0x28);
    kInitialThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c +
                                                                   kInitialThunkGroup.m_pOffsets->m_nTertiaryOffset + 0x28);
    kInitialThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c +
                                                                     kInitialThunkGroup.m_pOffsets->m_nQuaternaryOffset + 0x28);
    kInitialThunkGroup.m_pFatal = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c +
                                                                kInitialThunkGroup.m_pOffsets->m_nFatalOffset + 0x28);
    kInitialThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable;
    kInitialThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable;
    kInitialThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_GlobalStateCompositeInitialDualThunkVtable;
    kInitialThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_GlobalStateCompositeInitialFileThunkVtable;
    kInitialThunkGroup.m_pFatal->m_pVtable = g_NETWORK_GlobalStateCompositeInitialFatalThunkVtable;
    kInitialThunkGroup.m_pPrimary->m_nThisDelta = 0;
    kInitialThunkGroup.m_pSecondary->m_nThisDelta = 0;
    kInitialThunkGroup.m_pTertiary->m_nThisDelta = 0;

    ConstructCompositeEffTransportStack(pObject, 0);

    *(void **)pObject = g_NETWORK_DualSlotCompositeFinalVtable;
    kFinalThunkGroup.m_pOffsets = kInitialThunkGroup.m_pOffsets;
    kFinalThunkGroup.m_pPrimary = kInitialThunkGroup.m_pPrimary;
    kFinalThunkGroup.m_pSecondary = kInitialThunkGroup.m_pSecondary;
    kFinalThunkGroup.m_pTertiary = kInitialThunkGroup.m_pTertiary;
    kFinalThunkGroup.m_pQuaternary = kInitialThunkGroup.m_pQuaternary;
    kFinalThunkGroup.m_pFatal = kInitialThunkGroup.m_pFatal;
    kFinalThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable;
    kFinalThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_DualSlotCompositeFinalTimedThunkVtable;
    kFinalThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_DualSlotCompositeFinalDualThunkVtable;
    kFinalThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_DualSlotCompositeFinalFileThunkVtable;
    kFinalThunkGroup.m_pFatal->m_pVtable = g_NETWORK_DualSlotCompositeFinalFatalThunkVtable;
    kFinalThunkGroup.m_pPrimary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x38;
    kFinalThunkGroup.m_pSecondary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x68;
    kFinalThunkGroup.m_pTertiary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xe0;
    kFinalThunkGroup.m_pFatal->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nFatalOffset - 0x154;

    *(int *)(pbObjectBase + kFinalThunkGroup.m_pOffsets->m_nFatalOffset + 0xc) = 3;

    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(pbObjectBase + 0x2c + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetA) = 0;
    } else {
        *(void **)(pbObjectBase + 0x2c + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetA) =
            ConstructEffStreamRecordSlotTable(pStorage, *(int *)(pbObjectBase + kFinalThunkGroup.m_pOffsets->m_nFatalOffset + 0xc));
    }

    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(pbObjectBase + 0x2c + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetB) = 0;
    } else {
        *(void **)(pbObjectBase + 0x2c + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetB) =
            ConstructEffStreamRecordSlotTable(pStorage, *(int *)(pbObjectBase + kFinalThunkGroup.m_pOffsets->m_nFatalOffset + 0xc));
    }

    *(int *)(pbObjectBase + 0x30 + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetA) = *(int *)(pbObjectBase + 0x48);
    *(int *)(pbObjectBase + 0x30 + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetB) = *(int *)(pbObjectBase + 0x48);
    return pObject;
}

// FUNCTION: LEMBALL 0x0047B2A0
void DestroyDualSlotTableFileBackedEffComposite(int nObjectBasePlus0x64) {
    NETWORK_QuintThunkAdjustorGroup kFinalThunkGroup;
    char *pbObjectBase;
    NETWORK_DeleteObject *pRecordTable;

    pbObjectBase = (char *)(unsigned long)(nObjectBasePlus0x64 - 100);
    *(void **)pbObjectBase = g_NETWORK_DualSlotCompositeFinalVtable;
    kFinalThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)(pbObjectBase + 0x2c);
    kFinalThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c +
                                                                kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset + 0x28);
    kFinalThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c +
                                                                  kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset + 0x28);
    kFinalThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c +
                                                                 kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset + 0x28);
    kFinalThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c +
                                                                   kFinalThunkGroup.m_pOffsets->m_nQuaternaryOffset + 0x28);
    kFinalThunkGroup.m_pFatal = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c +
                                                              kFinalThunkGroup.m_pOffsets->m_nFatalOffset + 0x28);
    kFinalThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable;
    kFinalThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_DualSlotCompositeFinalTimedThunkVtable;
    kFinalThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_DualSlotCompositeFinalDualThunkVtable;
    kFinalThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_DualSlotCompositeFinalFileThunkVtable;
    kFinalThunkGroup.m_pFatal->m_pVtable = g_NETWORK_DualSlotCompositeFinalFatalThunkVtable;
    kFinalThunkGroup.m_pPrimary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x38;
    kFinalThunkGroup.m_pSecondary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x68;
    kFinalThunkGroup.m_pTertiary->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xe0;
    kFinalThunkGroup.m_pFatal->m_nThisDelta = kFinalThunkGroup.m_pOffsets->m_nFatalOffset - 0x154;

    pRecordTable = *(NETWORK_DeleteObject **)(pbObjectBase + 0x2c + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetA);
    if (pRecordTable != 0) {
        pRecordTable->m_pVtable->m_pDelete(1);
    }

    pRecordTable = *(NETWORK_DeleteObject **)(pbObjectBase + 0x2c + kFinalThunkGroup.m_pOffsets->m_nRecordTableOffsetB);
    if (pRecordTable != 0) {
        pRecordTable->m_pVtable->m_pDelete(1);
    }

    RestoreCompositeEffTransportVtables(nObjectBasePlus0x64 - 0x34);
    DestroyEffStreamBase((void *)((((nObjectBasePlus0x64 == 100) - 1) & (int)(unsigned long)(pbObjectBase + 0x2c)) + 4));
}

// FUNCTION: LEMBALL 0x0047B370
void WriteDualSlotTableFileBackedEffHeader(int nObjectBasePlus0x154) {
    char *pbObjectBase;
    NETWORK_FileBackedDispatchOffsets *pOffsets;
    NETWORK_FileWrapperObject *pReadThunk;
    NETWORK_FileWrapperObject *pWriteThunk;
    unsigned long ulObjectBase;

    pbObjectBase = (char *)(unsigned long)(nObjectBasePlus0x154 - 0x154);
    ulObjectBase = (unsigned long)pbObjectBase;
    pOffsets = *(NETWORK_FileBackedDispatchOffsets **)pbObjectBase;
    pReadThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(0);
    WriteEffStreamToLockedFile(pWriteThunk, (int *)(unsigned long)(nObjectBasePlus0x154 - 0x150), 0, 0);
}

// FUNCTION: LEMBALL 0x0047B9D0
void *DeleteCompositeFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_SideBufferOwner104 *pOwner;

    pOwner = (NETWORK_SideBufferOwner104 *)pObject;
    DestroyEffStreamBase((char *)pObject - 0x34);
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x150);
    pOwner->m_pResetThunk = (void *)0x00498070;
    if (pOwner->m_pSideBuffer != 0) {
        FreeVSMemBlock(pOwner->m_pSideBuffer);
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
    NETWORK_SideBufferOwnerA4 *pOwner;

    pOwner = (NETWORK_SideBufferOwnerA4 *)pObject;
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject);
    pOwner->m_pResetThunk = (void *)0x00498070;
    if (pOwner->m_pSideBuffer != 0) {
        FreeVSMemBlock(pOwner->m_pSideBuffer);
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
    NETWORK_SideBufferOwner104 *pOwner;

    pOwner = (NETWORK_SideBufferOwner104 *)pObject;
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x150);
    pOwner->m_pResetThunk = (void *)0x00498070;
    if (pOwner->m_pSideBuffer != 0) {
        FreeVSMemBlock(pOwner->m_pSideBuffer);
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
    NETWORK_SideBufferOwner104 *pOwner;

    pOwner = (NETWORK_SideBufferOwner104 *)pObject;
    DestroyDualSlotTableFileBackedEffComposite((int)(unsigned long)pObject);
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x154);
    pOwner->m_pResetThunk = (void *)0x00498070;
    if (pOwner->m_pSideBuffer != 0) {
        FreeVSMemBlock(pOwner->m_pSideBuffer);
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
    NETWORK_FileBackedAppendOffsets *pOffsets;
    NETWORK_FileWrapperOpenObject *pOpenThunk;
    void (**ppOuterVtable)(void);
    unsigned long ulObjectBase;
    DWORD dwStartTime;
    DWORD dwNow;

    ulObjectBase = (unsigned long)pObject;
    pOffsets = *(NETWORK_FileBackedAppendOffsets **)((char *)pObject + 4);
    pOpenThunk = (NETWORK_FileWrapperOpenObject *)(unsigned long)(pOffsets->m_nOutputStateOffset04 + 4 + ulObjectBase);
    ppOuterVtable = (void (**)(void))*(void ***)pObject;

    if (pOpenThunk->m_pVtable->m_pOpenWithMode(nHandle, 1, 0) == 0) {
        if (pOpenThunk->m_pVtable->m_pOpenFallback(nHandle, 1) == 0) {
            dwStartTime = timeGetTime();
            do {
                dwNow = timeGetTime();
            } while (dwNow - dwStartTime < 100);

            if (pOpenThunk->m_pVtable->m_pOpenWithMode(nHandle, 1, 0) == 0) {
                return 0;
            }
        } else {
            (*ppOuterVtable)();
        }
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00479900
BYTE MapNestedEffCallbackFailureToStatus6(int nObjectBasePlus0x1c) {
    NETWORK_FileBackedAppendOffsets *pOffsets;
    NETWORK_CallbackThunkObject *pNestedThunk;
    unsigned long ulObjectBase;
    int nResult;

    ulObjectBase = (unsigned long)(nObjectBasePlus0x1c - 0x1c);
    pOffsets = *(NETWORK_FileBackedAppendOffsets **)(unsigned long)(nObjectBasePlus0x1c - 0x1c);
    pNestedThunk = (NETWORK_CallbackThunkObject *)(unsigned long)(pOffsets->m_nOutputStateOffset04 + ulObjectBase);
    nResult = pNestedThunk->m_pVtable->m_pInvoke();
    return (BYTE)(-(nResult == 0) & 6);
}

// FUNCTION: LEMBALL 0x00479A50
int LoadEffStreamFromFileBackedRange(void *pObject, void *pStream, int fKeepLocked, int fAlreadyLocked) {
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_FileWrapperObject *pFileWrapper;
    unsigned long ulObjectBase;
    DWORD dwOffset;
    DWORD cbRead;
    DWORD dwStartTime;
    DWORD dwNow;

    ulObjectBase = (unsigned long)pObject;
    pOffsets = *(NETWORK_ConstructionAdjustorVtable **)((char *)pObject + 0xc);
    pFileWrapper = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nTertiaryOffset + 0xc + ulObjectBase);
    dwOffset = pFileWrapper->m_pVtable->m_pGetOffset();
    cbRead = *(DWORD *)((char *)pStream + 0x18);

    if (fAlreadyLocked == 0) {
        dwStartTime = timeGetTime();
        while (LockWin32FileRange(pFileWrapper, dwOffset, cbRead) == 0) {
            dwNow = timeGetTime();
            if (99 < dwNow - dwStartTime) {
                return 0;
            }
        }
    }

    if (ReadWin32FileWrapper(pFileWrapper, (LPVOID)g_pEffTransportPacketBuffer, cbRead) == 0) {
        UnlockWin32FileRange(pFileWrapper, dwOffset, cbRead);
        return 0;
    }

    LoadEffStreamFromMemory(pStream, (int)(unsigned long)g_pEffTransportPacketBuffer);
    if (fKeepLocked == 0 && UnlockWin32FileRange(pFileWrapper, dwOffset, cbRead) == 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00479B30
int LoadFileBackedEffRecordPayload(void *pObject, int nSlotIndex) {
    NETWORK_ConstructionAdjustorVtable *pChannelOffsets;
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    NETWORK_EffStreamRecordSlot *pSlot;
    NETWORK_FileWrapperObject *pFileWrapper;
    NETWORK_ChannelStateHeader *pChannelState;
    NETWORK_TimedStreamHeader *pTimedStream;
    NETWORK_ChannelStateRuntimeVtable *pChannelStateVtable;
    NETWORK_TimedStreamRuntimeVtable *pTimedStreamVtable;
    NETWORK_PeerAddressService *pPeerAddressService;
    unsigned long ulFileWrapperBase;
    unsigned long ulChannelStateBase;
    unsigned long ulTimedStreamBase;
    int nResult;
    DWORD dwNow;

    pChannelOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)((char *)pObject + 0xc);
    pRecordTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    ulFileWrapperBase = (unsigned long)(pChannelOffsets->m_nTertiaryOffset + 0xc + (int)(unsigned long)pObject);
    ulChannelStateBase = (unsigned long)(pChannelOffsets->m_nPrimaryOffset + 0xc + (int)(unsigned long)pObject);
    ulTimedStreamBase = (unsigned long)(pChannelOffsets->m_nSecondaryOffset + 0xc + (int)(unsigned long)pObject);
    pFileWrapper = (NETWORK_FileWrapperObject *)ulFileWrapperBase;
    pChannelState = (NETWORK_ChannelStateHeader *)ulChannelStateBase;
    pTimedStream = (NETWORK_TimedStreamHeader *)ulTimedStreamBase;
    pChannelStateVtable = (NETWORK_ChannelStateRuntimeVtable *)pChannelState->m_pVtable;
    pTimedStreamVtable = (NETWORK_TimedStreamRuntimeVtable *)pTimedStream->m_pVtable;
    nResult = pFileWrapper->m_pVtable->m_pSeek(g_cbEffTransportMaxPacketBytes * nSlotIndex + *(int *)((char *)pObject + 8));
    if (nResult == 0) {
        return 0;
    }

    pSlot = &pRecordTable->m_pSlots[nSlotIndex];
    nResult = ReadWin32FileWrapper(pFileWrapper, (LPVOID)g_pEffTransportPacketBuffer, pSlot->m_cbPayload);
    if (nResult == 0) {
        pChannelStateVtable->m_pHandleReadFailure();
        return 0;
    }

    dwNow = timeGetTime();
    pTimedStream->m_dwLastTick3c = dwNow;
    g_cbEffTransportCurrentPacketBytes = (int)pSlot->m_cbPayload;
    pPeerAddressService = (NETWORK_PeerAddressService *)g_pEffTransportPeerAddressState;
    pPeerAddressService->m_pVtable->m_pSelectPeerName(pSlot->m_szSourceName);
    if (pChannelState->m_fBusy10 == 0) {
        pTimedStreamVtable->m_pServiceLoadedPacket();
    }
    ProcessEffTransportPacketHeader((int *)((char *)pTimedStream + 0xc));
    return 1;
}

// FUNCTION: LEMBALL 0x0047A470
DWORD IncrementFileBackedEffStreamMarker(int *pObject) {
    NETWORK_FileBackedMarkerOffsets *pOffsets;
    NETWORK_FileBackedMarkerState *pMarkerState;
    NETWORK_FileWrapperObject *pReadThunk;
    NETWORK_FileWrapperObject *pWriteThunk;
    unsigned long ulObjectBase;

    pMarkerState = (NETWORK_FileBackedMarkerState *)pObject;
    ulObjectBase = (unsigned long)pObject;
    pOffsets = *(NETWORK_FileBackedMarkerOffsets **)pObject;
    pReadThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(0);
    if (LoadEffStreamFromFileBackedRange((void *)(unsigned long)(pOffsets->m_nStreamOffset1c + ulObjectBase), pObject + 1, 1, 0) == 0) {
        return 0;
    }
    pMarkerState->m_Stream04.m_wReferenceCount2c = (unsigned short)(pMarkerState->m_Stream04.m_wReferenceCount2c + 1);
    pReadThunk->m_pVtable->m_pSeek(0);
    return WriteEffStreamToLockedFile(pWriteThunk, pObject + 1, 0, 1);
}

// FUNCTION: LEMBALL 0x0047A4D0
DWORD DecrementFileBackedEffStreamMarker(int *pObject) {
    NETWORK_FileBackedMarkerOffsets *pOffsets;
    NETWORK_FileBackedMarkerState *pMarkerState;
    NETWORK_FileWrapperObject *pReadThunk;
    NETWORK_FileWrapperObject *pWriteThunk;
    unsigned long ulObjectBase;

    pMarkerState = (NETWORK_FileBackedMarkerState *)pObject;
    ulObjectBase = (unsigned long)pObject;
    pOffsets = *(NETWORK_FileBackedMarkerOffsets **)pObject;
    pReadThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(0);
    if (LoadEffStreamFromFileBackedRange((void *)(unsigned long)(pOffsets->m_nStreamOffset1c + ulObjectBase), pObject + 1, 1, 0) == 0) {
        return 0;
    }
    pMarkerState->m_Stream04.m_wReferenceCount2c = (unsigned short)(pMarkerState->m_Stream04.m_wReferenceCount2c - 1);
    pReadThunk->m_pVtable->m_pSeek(0);
    return WriteEffStreamToLockedFile(pWriteThunk, pObject + 1, 0, 1);
}

// FUNCTION: LEMBALL 0x0047A530
BYTE DecrementFileBackedEffMarkerAndReleaseIfZero(int nObjectBasePlus0x38) {
    NETWORK_FileBackedMarkerOffsets *pOffsets;
    NETWORK_FileBackedMarkerState *pMarkerState;
    NETWORK_CallbackThunkObject *pReleaseThunk;
    unsigned long ulMarkerStateBase;
    BYTE bStatus;

    pMarkerState = (NETWORK_FileBackedMarkerState *)(unsigned long)(nObjectBasePlus0x38 - 0x38);
    ulMarkerStateBase = (unsigned long)(nObjectBasePlus0x38 - 0x38);
    pOffsets = *(NETWORK_FileBackedMarkerOffsets **)pMarkerState;
    DecrementFileBackedEffStreamMarker((int *)pMarkerState);
    bStatus = MapNestedEffCallbackFailureToStatus6(pOffsets->m_nNestedThunkOffset18 + nObjectBasePlus0x38 - 0x18);
    if (*(short *)(unsigned long)(nObjectBasePlus0x38 - 8) == 0) {
        pReleaseThunk = (NETWORK_CallbackThunkObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulMarkerStateBase);
        pReleaseThunk->m_pVtable->m_pRelease();
    }
    return bStatus;
}

// FUNCTION: LEMBALL 0x00479C10
void ServicePendingFileBackedEffRecords(int *pObject) {
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    NETWORK_ConstructionAdjustorVtable *pChannelOffsets;
    NETWORK_FileWrapperObject *pFileWrapper;
    NETWORK_ChannelStateHeader *pRecordState;
    NETWORK_EffStreamRecordSlot *pSlot;
    unsigned long ulFileWrapperBase;
    unsigned long ulRecordStateBase;
    int nThunkOffset;
    int nLockResult;
    int nSlotStride;
    void *pRuntimeWindow;
    int i;
    int nSlotCount;
    int fLoadedOne;

    pRecordTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    pChannelOffsets = pRecordTable->m_pChannelThunk0c;
    nThunkOffset = pChannelOffsets->m_nPrimaryOffset;
    nSlotStride = *(int *)((char *)&pRecordTable->m_pSlots[0] + 0x18);
    if ((*(int *)((char *)pObject + nThunkOffset + 0x1c) == 0 && *(int *)((char *)pObject + nThunkOffset + 0x28) == 0) ||
        *(int *)((char *)pObject + nThunkOffset + 0x18) == 0) {
        return;
    }

    ulFileWrapperBase = (unsigned long)(pChannelOffsets->m_nTertiaryOffset + 0xc + (int)(unsigned long)pObject);
    ulRecordStateBase = (unsigned long)(pChannelOffsets->m_nQuaternaryOffset + (int)(unsigned long)pObject);
    pFileWrapper = (NETWORK_FileWrapperObject *)ulFileWrapperBase;
    pRecordState = (NETWORK_ChannelStateHeader *)ulRecordStateBase;
    nLockResult = LockWin32FileRange(pFileWrapper, pRecordTable->m_nRecordHeaderOffset04, *(DWORD *)((char *)pObject + 0x18));
    if (nLockResult == 0) {
        pRuntimeWindow = 0;
        if (g_pActiveNetworkRuntimeWindow != 0) {
            pRuntimeWindow = (char *)g_pActiveNetworkRuntimeWindow - 0x10;
        }
        ScheduleNetworkRuntimeTimerEvent(pRuntimeWindow, 0x32);
        return;
    }

    if (pRecordTable->m_nPendingSlot10 == -1) {
        pFileWrapper->m_pVtable->m_pSeek(pRecordTable->m_nRecordHeaderOffset04);
        ReadWin32FileWrapper(pFileWrapper, (LPVOID)g_pEffTransportPacketBuffer, *(DWORD *)((char *)pObject + 0x18));
        nLockResult = UnlockWin32FileRange(pFileWrapper, pRecordTable->m_nRecordHeaderOffset04, *(DWORD *)((char *)pObject + 0x18));
        if (nLockResult != 0) {
            nSlotCount = 0;
            LoadEffStreamFromMemory((void *)(unsigned long)*pObject, (int)(unsigned long)g_pEffTransportPacketBuffer);
            fLoadedOne = 0;
            if (0 < pRecordState->m_cSlots14) {
                do {
                    pSlot = &pRecordTable->m_pSlots[nSlotCount];
                    if (pSlot->m_wObservedMarker < pSlot->m_wCommittedMarker) {
                        if (fLoadedOne) {
                            pRecordTable->m_nPendingSlot10 = nSlotCount;
                            return;
                        }
                        LoadFileBackedEffRecordPayload(pObject, nSlotCount);
                        pSlot->m_wObservedMarker = pSlot->m_wCommittedMarker;
                        fLoadedOne = 1;
                    }
                    ++nSlotCount;
                } while (nSlotCount < pRecordState->m_cSlots14);
            }
        }
    } else {
        pFileWrapper->m_pVtable->m_pSeek(nSlotStride * pRecordTable->m_nPendingSlot10 + pRecordTable->m_nRecordHeaderOffset04);
        ReadWin32FileWrapper(pFileWrapper, (LPVOID)g_pEffTransportPacketBuffer, *(DWORD *)((char *)pObject + 0x18));
        nLockResult = UnlockWin32FileRange(pFileWrapper, pRecordTable->m_nRecordHeaderOffset04, *(DWORD *)((char *)pObject + 0x18));
        if (nLockResult != 0) {
            pSlot = &pRecordTable->m_pSlots[pRecordTable->m_nPendingSlot10];
            LoadEffStreamFromMemory(pSlot, (int)(unsigned long)g_pEffTransportPacketBuffer);
            LoadFileBackedEffRecordPayload(pObject, pRecordTable->m_nPendingSlot10);
            pSlot->m_wObservedMarker = pSlot->m_wCommittedMarker;
            i = pRecordTable->m_nPendingSlot10;
            nSlotCount = pRecordState->m_cSlots14;
            if (i < nSlotCount) {
                do {
                    pSlot = &pRecordTable->m_pSlots[i];
                    if (pSlot->m_wObservedMarker < pSlot->m_wCommittedMarker) {
                        pRecordTable->m_nPendingSlot10 = i;
                        break;
                    }
                    ++i;
                } while (i < nSlotCount);
            }
            if (pRecordState->m_cSlots14 == i) {
                pRecordTable->m_nPendingSlot10 = -1;
            }
        }
    }
}

// FUNCTION: LEMBALL 0x0047A0B0
int AppendFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    NETWORK_FileBackedAppendOffsets *pOffsets;
    NETWORK_EffTransportRuntimeService *pRuntimeService;
    NETWORK_EffStreamRecordSlot *pSlot;
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    NETWORK_ChannelStateHeader *pOutputState;
    NETWORK_FileWrapperObject *pFileWrapper;
    NETWORK_StreamNameProvider *pNameProvider;
    char *pszSource;
    char *pszTarget;
    unsigned int cch;
    unsigned int count;
    int nCurrentSlot;
    DWORD dwSlotOffset;
    DWORD cbLocked;
    int fFailed;
    unsigned long ulObjectBase;
    DWORD dwNow;

    ulObjectBase = (unsigned long)pObject;
    pOffsets = *(NETWORK_FileBackedAppendOffsets **)((char *)pObject - 0x3c);
    pOutputState = (NETWORK_ChannelStateHeader *)(unsigned long)(pOffsets->m_nOutputStateOffset04 + ulObjectBase - 0x3c);
    pFileWrapper = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nFileWrapperOffset0c + ulObjectBase - 0x3c);
    pNameProvider = (NETWORK_StreamNameProvider *)(unsigned long)(pOffsets->m_nNameProviderOffset08 + ulObjectBase);
    if (pOutputState->m_fChannelOpen24 == 0) {
        return 0;
    }

    pRuntimeService = (NETWORK_EffTransportRuntimeService *)g_pEffTransportRuntimeService;
    pRecordTable = (NETWORK_EffStreamRecordSlotTable *)((char *)pObject - 0x48);
    nCurrentSlot = pRecordTable->m_nPendingSlot10;
    pSlot = &pRecordTable->m_pSlots[nCurrentSlot];

    dwSlotOffset = *(int *)((char *)&pRecordTable->m_pSlots[0] + 0x18) * nCurrentSlot +
                   pRecordTable->m_nPayloadBaseOffset08;
    pFileWrapper->m_pVtable->m_pSeek(dwSlotOffset);

    pszSource = pRuntimeService->m_pVtable->m_pGetLocalHostName();
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

    pszSource = pNameProvider->m_pVtable->m_pGetName();
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
        pFileWrapper->m_pVtable->m_pSeek(nCurrentSlot * g_cbEffTransportMaxPacketBytes + pRecordTable->m_nPayloadBaseOffset08);
        if (WriteWin32FileWrapper(pFileWrapper, pvPayload, cbPayload) != 0) {
            fFailed = 0;
        }
    }

    if (fFailed == 0) {
        pRecordTable->m_nPendingSlot10 = nCurrentSlot + 1;
        dwNow = timeGetTime();
        *(DWORD *)pNameProvider = dwNow;
    }
    if (cbLocked != 0) {
        UnlockWin32FileRange(pFileWrapper, dwSlotOffset, cbLocked);
    }
    return fFailed == 0;
}

// FUNCTION: LEMBALL 0x0047A420
int AppendAdjustedFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    NETWORK_ConstructionAdjustorVtable *pAdjustorOffsets;
    char *pbAdjustorBase;
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    int *pnIndex;
    int nRecordWriterBase;
    int nRecordTableBase;
    int nIndexBase;
    int fAppended;

    pbAdjustorBase = (char *)pObject - 0x38;
    pAdjustorOffsets = (NETWORK_ConstructionAdjustorVtable *)*(void **)pbAdjustorBase;
    nRecordWriterBase = pAdjustorOffsets->m_nRecordTableOffsetB + (int)(unsigned long)pbAdjustorBase;
    nRecordTableBase = pAdjustorOffsets->m_nRecordTableOffsetA + (int)(unsigned long)pbAdjustorBase;
    nIndexBase = pAdjustorOffsets->m_nRecordTableOffsetB - 0x38 + (int)(unsigned long)pbAdjustorBase;

    fAppended = AppendFileBackedEffRecordSlot((void *)(unsigned long)(nRecordWriterBase + 0x10), pvPayload, cbPayload);
    if (fAppended != 0) {
        pRecordTable = (NETWORK_EffStreamRecordSlotTable *)(unsigned long)(nRecordTableBase - 0x30);
        pnIndex = (int *)(unsigned long)(nIndexBase - 0x28);
        *pnIndex = *pnIndex % pRecordTable->m_cSlots;
    }
    return fAppended;
}

// FUNCTION: LEMBALL 0x0047AA10
void InitializeGlobalStateFileBackedEffStorage(int nObjectBasePlus0x15c) {
    NETWORK_FileBackedDispatchOffsets *pOffsets;
    NETWORK_FileWrapperObject *pReadThunk;
    NETWORK_FileWrapperObject *pWriteThunk;
    NETWORK_SlotCountHolder *pSlotCount;
    unsigned long ulObjectBase;
    DWORD cbPacket;
    unsigned int count;
    unsigned char *pByte;
    unsigned int i;
    void *pZeroBuffer;

    ulObjectBase = (unsigned long)(nObjectBasePlus0x15c - 0x15c);
    pOffsets = *(NETWORK_FileBackedDispatchOffsets **)ulObjectBase;
    pReadThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pSlotCount = (NETWORK_SlotCountHolder *)(unsigned long)(pOffsets->m_nSlotCountOffset18 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(0);
    WriteEffStreamToLockedFile(pWriteThunk, (int *)(unsigned long)(nObjectBasePlus0x15c - 0x158), 0, 0);
    WriteEffStreamToLockedFile(pWriteThunk, (int *)(unsigned long)g_pGlobalStateEff512ByteStream, 0, 0);
    WriteEffStreamToLockedFile(pWriteThunk, (int *)(unsigned long)*(int *)pWriteThunk, 0, 0);

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

    i = 0;
    if (0 < pSlotCount->m_cSlots) {
        do {
            ++i;
            WriteWin32FileWrapper(pReadThunk, pZeroBuffer, g_cbEffTransportMaxPacketBytes);
        } while (i < pSlotCount->m_cSlots);
    }
    FreeVSMemBlock(pZeroBuffer);
}
