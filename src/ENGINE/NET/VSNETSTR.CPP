#include "GAME.H"
#include "ENGINE/MEDIA/EFFSTRM.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/MEDIA/VSSTRM.H"
#include "ENGINE/NET/VSNET.H"
#include "ENGINE/CORE/SAFEVT.H"
#include <string.h>

struct VsNetConstructionAdjustorVtable {
    void *m_pReserved00;
    int m_nPrimaryOffset;
    int m_nSecondaryOffset;
    int m_nTertiaryOffset;
    int m_nQuaternaryOffset;
};

struct VsNetAdjustorSubobject {
    int m_nThisDelta;
    void *m_pVtable;
};

struct VsNetAdjustorThunkHeader {
    int m_nThisDelta;
};

struct VsNetThunkAdjustorGroup {
    VsNetConstructionAdjustorVtable *m_pOffsets;
    VsNetAdjustorSubobject *m_pPrimary;
    VsNetAdjustorSubobject *m_pSecondary;
    VsNetAdjustorSubobject *m_pTertiary;
};

struct VsNetEffStreamSerializeVtable {
    void *m_pReserved00;
    void (*m_pLoadFromMemoryHeader)(void *pThis);
    void (*m_pLoadFromMemoryBody)(void *pThis);
    void (*m_pBeginWriteHeader)(void *pThis);
    void (*m_pBeginWriteBody)(void *pThis);
};

struct VsNetChannelStateRuntime {
    virtual void CancelPending(void) = 0;
};

struct VsNetRuntimeService {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Invoke(void *pArgument) = 0;
};

struct VsNetChannelOwnerObject {
    void ServiceEffTransportConnectRequest(void);
};

/* Ghidra: original table at 004990e8 has 7 callable slots:
   claim flag, clear flag, fatal, fatal, service, fatal, fatal.
   The following zero is table padding, not a virtual function. */
struct VsNetRuntimeChannelStackVtableModel {
    virtual short ClaimRuntimeFlag(const char *pszFlags);
    virtual void ClearRuntimeFlag(short nFlag);
    virtual void FatalRuntimeSlot02(void) { _purecall(); }
    virtual void FatalRuntimeSlot03(void) { _purecall(); }
    virtual void ServiceRuntimeConnect(void) {
        ((VsNetChannelOwnerObject *)this)->ServiceEffTransportConnectRequest();
    }
    virtual void FatalRuntimeSlot05(void) { _purecall(); }
    virtual void FatalRuntimeSlot06(void) { _purecall(); }
};

// FUNCTION: LEMBALL 0x004605D0
short VsNetRuntimeChannelStackVtableModel::ClaimRuntimeFlag(const char *pszFlags) {
    VsNetRuntimeChannelStack *pStack = (VsNetRuntimeChannelStack *)this;
    short i;

    for (i = 0; i < 0x200; ++i) {
        if (pszFlags[i] == 0 && pStack->m_pbRuntimeFlags10[i] == 0) {
            pStack->m_pbRuntimeFlags10[i] = 1;
            return i;
        }
    }
    return -1;
}

// FUNCTION: LEMBALL 0x004629C0
void VsNetRuntimeChannelStackVtableModel::ClearRuntimeFlag(short nFlag) {
    ((VsNetRuntimeChannelStack *)this)->m_pbRuntimeFlags10[nFlag] = 0;
}

struct VsNetRuntimeChannelStackWrapperView {
    unsigned char m_abChannelState00[0x30];
    unsigned char m_abTimedStream30[0x78];
    unsigned char m_abDualStreamA8[0x78];
};

struct VsNetRuntimeWindowInterface {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0C(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved1C(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void Reserved24(void) = 0;
    virtual void Reserved28(void) = 0;
    virtual void *CreateServiceObject(void) = 0;
    virtual void PumpMessages(void) = 0;
};

void *DeleteEffStreamChannelStateWrapper004628E0(void *pObject, BYTE fDelete);
void *DeleteEffChannelStreamStackWrapper(void *pChannelState, BYTE fDelete);
void *DeleteEffChannelStreamStackAdjustedThunk(void *pObject, BYTE fDelete);
void ClearRuntimeChannelPendingWriteAdjustedThunk(void *pObject, void *pUnused);
void EffStreamChannelStateRet4Thunk(BYTE fDelete);
static void *g_NETWORK_EffStreamChannelStateVtable[] = {
    (void *)_purecall,
    (void *)DeleteEffStreamChannelStateWrapper004628E0,
    (void *)_purecall,
    (void *)EffStreamChannelStateRet4Thunk,
};
struct VsNetEffStreamBaseVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }

    virtual void WriteTaggedHeader(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        VsNetEffStreamTag *pTag = pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(pTag->m_nWord00);
        pStream->WriteEffStreamU32BE(pTag->m_nWord04);
        pStream->WriteEffStreamU16BE(pTag->m_nWord08);
        pStream->WriteEffStreamU16BE(pTag->m_nWord0a);
        pStream->WriteEffStreamU16BE(pTag->m_nWord0c);
        *(unsigned char *)(pStream->m_nReserved1c) = pTag->m_bByte0e;
        ++pStream->m_nReserved1c;
    }

    virtual void Noop(void) {}

    virtual void ReverseTaggedPayload(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pb;
        unsigned char b;

        pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        b = pb[0]; pb[0] = pb[3]; pb[3] = b;
        b = pb[1]; pb[1] = pb[2]; pb[2] = b;
        pStream->m_nReserved20 += 4;
        pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        b = pb[0]; pb[0] = pb[3]; pb[3] = b;
        b = pb[1]; pb[1] = pb[2]; pb[2] = b;
        pStream->m_nReserved20 += 4;
        for (int i = 0; i < 3; ++i) {
            pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
            b = pb[0]; pb[0] = pb[1]; pb[1] = b;
            pStream->m_nReserved20 += 2;
        }
        ++pStream->m_nReserved20;
    }

    virtual void *Delete(BYTE fDelete) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        pStream->DestroyEffStreamBase();
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pStream);
        }
        return pStream;
    }
};

static int g_NETWORK_DualHandleEffStreamChannelStateConstructionOffsets[2] = {
    -0x44, 0x14,
};
void *DeleteDualEffStreamWithChannelStateWrapper(void *pObject, BYTE fDelete);
void *DeleteTimedEffStreamWithChannelStateWrapper(void *pObject, BYTE fDelete);

struct VsNetDualPrimaryThunkVtableModel {
    virtual void Fatal(void) { _purecall(); }
    virtual void *Delete(BYTE fDelete) {
        return DeleteDualEffStreamWithChannelStateWrapper((char *)this - 0x58, fDelete);
    }
    virtual void FatalSecondary(void) { _purecall(); }
    virtual void Ret4(BYTE fDelete) { (void)fDelete; }
};

static VsNetDualPrimaryThunkVtableModel g_NETWORK_DualPrimaryThunkVtableModel;
static void *g_NETWORK_DualHandleEffStreamPrimaryThunkVtable =
    *(void ***)&g_NETWORK_DualPrimaryThunkVtableModel;
struct VsNetDualHandleEffStreamVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pb;
        unsigned char b;
        pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        b = pb[0]; pb[0] = pb[3]; pb[3] = b;
        b = pb[1]; pb[1] = pb[2]; pb[2] = b;
        pStream->m_nReserved20 += 4;
        pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        b = pb[0]; pb[0] = pb[3]; pb[3] = b;
        b = pb[1]; pb[1] = pb[2]; pb[2] = b;
        pStream->m_nReserved20 += 4;
        for (int i = 0; i < 3; ++i) {
            pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
            b = pb[0]; pb[0] = pb[1]; pb[1] = b;
            pStream->m_nReserved20 += 2;
        }
        ++pStream->m_nReserved20;
    }
    virtual void Noop(void) {}
    virtual void WriteTaggedHeader(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        VsNetEffStreamTag *pTag = pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(pTag->m_nWord00);
        pStream->WriteEffStreamU32BE(pTag->m_nWord04);
        pStream->WriteEffStreamU16BE(pTag->m_nWord08);
        pStream->WriteEffStreamU16BE(pTag->m_nWord0a);
        pStream->WriteEffStreamU16BE(pTag->m_nWord0c);
        *(unsigned char *)pStream->m_nReserved1c = pTag->m_bByte0e;
        ++pStream->m_nReserved1c;
    }
    virtual void *Delete(BYTE fDelete) {
        return DeleteDualEffStreamWithChannelStateWrapper(this, fDelete);
    }
    virtual void DeleteRet(void) {}
    virtual void Fatal(void) { _purecall(); }
    virtual void DeleteRet8(BYTE fDelete, BYTE fReserved) {
        (void)fDelete;
        (void)fReserved;
    }
};

static VsNetDualHandleEffStreamVtableModel g_NETWORK_DualHandleEffStreamVtableModel;
static void *g_NETWORK_DualHandleEffStreamVtable =
    *(void ***)&g_NETWORK_DualHandleEffStreamVtableModel;
static int g_NETWORK_TimedEffStreamChannelStateConstructionOffsets[2] = {
    -0x44, 0x34,
};
struct VsNetTimedPrimaryThunkVtableModel {
    virtual void Fatal(void) { _purecall(); }
    virtual void *Delete(BYTE fDelete) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 0x78;
        return DeleteTimedEffStreamWithChannelStateWrapper(pAdjusted, fDelete);
    }
    virtual void FatalSecondary(void) { _purecall(); }
    virtual void ClearPendingWrite(void *pUnused) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4);
        ((VsNetEffTransportPendingWriteState *)pAdjusted)->Clear(pUnused);
    }
};

static VsNetTimedPrimaryThunkVtableModel g_NETWORK_TimedPrimaryThunkVtableModel;
static void *g_NETWORK_TimedEffStreamPrimaryThunkVtable =
    *(void ***)&g_NETWORK_TimedPrimaryThunkVtableModel;
struct VsNetAckedEffTransportRecordOwner {
    void *ClaimAckedEffTransportRecordPayload(void);
};
extern int PASCAL ReturnTrueVtableCallback(void *pObject);
extern int ReturnTrueVtableCallbackSecondary(void);
extern int PASCAL ReturnTrueVtableCallbackThunk(void *pObject);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void ReturnVoidVtableCallback(void);
extern void ReverseEffTransportPayload(void *pObject);
extern void NoopVtableCallbackThunk(void);
extern void WriteEffTransportTaggedHeader(void *pObject);
void *DeleteEffStreamBaseWrapper00462900(void *pObject, BYTE fDelete);

// GLOBAL: LEMBALL 0x00498f40
static void *g_NETWORK_EffStreamBaseVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReverseEffTransportPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteEffStreamBaseWrapper00462900,
};
struct VsNetCompositeEffTransportStackWrapperView {
    void *DeleteCompositeEffTransportRuntimeWrapper(BYTE fDelete);
};
extern void *DeleteCompositeEffTransportRuntimeAdjustedThunk(void *pObject, BYTE fDelete);
extern void WriteEffTransportGlobalSessionAdjustedThunk(void *pObject);
struct VsNetAdjustedEffTransportPeerView {
    void LEMBALL_FASTCALL CloseAdjustedEffTransportPeerByKey(void);
};
extern void *g_NETWORK_TcpipSocketStackDualRet8;

struct VsNetTimedEffStreamVtableModel : VsNetEffStreamBaseVtableModel {
    virtual void *Delete(BYTE fDelete) {
        return DeleteTimedEffStreamWithChannelStateWrapper(this, fDelete);
    }

    virtual void WriteGlobalSession(void) {
        ((VsNetPeerPayloadSender *)this)->WriteEffStreamWithGlobalSession();
    }

    virtual void *ClaimTimedRecord(void) {
        return ((VsNetAckedEffTransportRecordOwner *)this)
            ->ClaimAckedEffTransportRecordPayload();
    }
    virtual void Fatal(void) { _purecall(); }
};

static VsNetTimedEffStreamVtableModel g_NETWORK_TimedEffStreamVtableModel;
static void *g_NETWORK_TimedEffStreamVtable =
    *(void ***)&g_NETWORK_TimedEffStreamVtableModel;
/* Exact construction-offset data recovered from the runtime stack stores. */
static int g_NETWORK_RuntimeChannelStackChannelStateConstructionOffsets[6] = {
    0, -0x100, -0xd0, -0x58, -0x44, -0xec,
};
/* Separate callable table.  The offset table above is not a vtable. */
static void *g_NETWORK_RuntimeChannelStackChannelStateVtable[4] = {
    (void *)_purecall,
    (void *)DeleteEffChannelStreamStackAdjustedThunk,
    (void *)_purecall,
    (void *)ClearRuntimeChannelPendingWriteAdjustedThunk,
};
static int g_NETWORK_RuntimeChannelStackTimedStreamConstructionOffsets[6] = {
    -0x44, -0x74, -4, 0x20, 0x50, 0xc8,
};
static int g_NETWORK_RuntimeChannelStackDualStreamConstructionOffsets[6] = {
    -0x44, 0x14, -0x44, -0x74, -4, 0x20,
};
static int g_NETWORK_RuntimeChannelStackOuterConstructionOffsets[6] = {
    -4, 0x20, 0x50, 0xc8, 0x120, 0,
};

// FUNCTION: LEMBALL 0x00462D20
static void *DeleteRuntimeTimedChannelStackAdjusted(void *pObject, BYTE fDelete) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x30;
    return DeleteEffChannelStreamStackWrapper(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00462D30
static void WriteRuntimeTimedGlobalSessionAdjusted(void *pObject) {
    int *pOffsets;

    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    pOffsets = *(int **)((char *)pObject - 0x38);
    ((VsNetPeerPayloadSender *)((char *)pObject + pOffsets[2] - 0x38))
        ->WriteEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x00462D40
static void *ClaimRuntimeTimedRecordAdjusted(void *pObject) {
    int *pOffsets;

    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    pOffsets = *(int **)((char *)pObject - 0x38);
    return ((VsNetAckedEffTransportRecordOwner *)
                ((char *)pObject + pOffsets[2] - 0x38))
        ->ClaimAckedEffTransportRecordPayload();
}

// FUNCTION: LEMBALL 0x00462D50
static void *DeleteRuntimeDualChannelStackAdjusted(void *pObject, BYTE fDelete) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0xa8;
    return DeleteEffChannelStreamStackWrapper(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00462D60
static void WriteRuntimeDualGlobalSessionAdjusted(void *pObject) {
    int *pOffsets;

    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x78;
    pOffsets = *(int **)((char *)pObject - 0x38);
    ((VsNetPeerPayloadSender *)((char *)pObject + pOffsets[2] - 0x38))
        ->WriteEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x00462D70
static void *ClaimRuntimeDualRecordAdjusted(void *pObject) {
    int *pOffsets;

    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x78;
    pOffsets = *(int **)((char *)pObject - 0x38);
    return ((VsNetAckedEffTransportRecordOwner *)
                ((char *)pObject + pOffsets[2] - 0x38))
        ->ClaimAckedEffTransportRecordPayload();
}

void RuntimeDualUnusedCallback(void) {
}

void *g_NETWORK_RuntimeChannelStackTimedStreamVtable[10] = {
    (void *)ReturnTrueVtableCallback,
    (void *)ReturnTrueVtableCallbackSecondary,
    (void *)ReverseEffTransportPayload,
    (void *)ReturnVoidVtableCallback,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteRuntimeTimedChannelStackAdjusted,
    (void *)WriteRuntimeTimedGlobalSessionAdjusted,
    (void *)ClaimRuntimeTimedRecordAdjusted,
    (void *)_purecall,
    (void *)_purecall,
};
void *g_NETWORK_RuntimeChannelStackDualStreamVtable[10] = {
    (void *)ReturnTrueVtableCallback,
    (void *)ReturnTrueVtableCallbackSecondary,
    (void *)ReverseEffTransportPayload,
    (void *)ReturnVoidVtableCallback,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteRuntimeDualChannelStackAdjusted,
    (void *)RuntimeDualUnusedCallback,
    (void *)WriteRuntimeDualGlobalSessionAdjusted,
    (void *)ClaimRuntimeDualRecordAdjusted,
    *(void **)g_NETWORK_TcpipSocketStackDualRet8,
};

// FUNCTION: LEMBALL 0x00462BB0
static void *DeleteRuntimeOuterCompositeAdjusted(void *pObject, BYTE fDelete) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetCompositeEffTransportStackWrapperView *)pObject)
        ->DeleteCompositeEffTransportRuntimeWrapper(fDelete);
}

// FUNCTION: LEMBALL 0x00462C00
static void CloseRuntimeOuterPeerAdjusted(void *pObject) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    ((VsNetAdjustedEffTransportPeerView *)pObject)
        ->CloseAdjustedEffTransportPeerByKey();
}

// FUNCTION: LEMBALL 0x00462C40
static void *ClaimRuntimeOuterRecordAdjusted(void *pObject) {
    int *pOffsets;

    pObject = (char *)pObject - *(int *)((char *)pObject - 4) + 0x108;
    pOffsets = *(int **)((char *)pObject - 0x38);
    return ((VsNetAckedEffTransportRecordOwner *)
                ((char *)pObject + pOffsets[2] - 0x38))
        ->ClaimAckedEffTransportRecordPayload();
}

// FUNCTION: LEMBALL 0x00462C60
static void *DeleteRuntimeOuterDualAdjusted(void *pObject, BYTE fDelete) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0xa8;
    return ((VsNetCompositeEffTransportStackWrapperView *)pObject)
        ->DeleteCompositeEffTransportRuntimeWrapper(fDelete);
}

// FUNCTION: LEMBALL 0x00462C70
static void WriteRuntimeOuterDualGlobalSessionAdjusted(void *pObject) {
    int *pOffsets;

    pObject = (char *)pObject - *(int *)((char *)pObject - 4) + 0x90;
    pOffsets = *(int **)((char *)pObject - 0x38);
    ((VsNetPeerPayloadSender *)((char *)pObject + pOffsets[2] - 0x38))
        ->WriteEffStreamWithGlobalSession();
}

static void *g_NETWORK_RuntimeChannelStackOuterFatalThunkVtable[4] = {
    (void *)_purecall,
    (void *)DeleteRuntimeOuterCompositeAdjusted,
    (void *)_purecall,
    (void *)CloseRuntimeOuterPeerAdjusted,
};
static void *g_NETWORK_RuntimeChannelStackOuterTimedThunkVtable[8] = {
    (void *)ReturnTrueVtableCallback,
    (void *)ReturnTrueVtableCallbackSecondary,
    (void *)ReverseEffTransportPayload,
    (void *)ReturnVoidVtableCallback,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteCompositeEffTransportRuntimeAdjustedThunk,
    (void *)WriteEffTransportGlobalSessionAdjustedThunk,
    (void *)ClaimRuntimeOuterRecordAdjusted,
};
static void *g_NETWORK_RuntimeChannelStackOuterDualThunkVtable[8] = {
    (void *)ReturnTrueVtableCallback,
    (void *)ReturnTrueVtableCallbackSecondary,
    (void *)ReverseEffTransportPayload,
    (void *)ReturnVoidVtableCallback,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteRuntimeOuterDualAdjusted,
    (void *)RuntimeDualUnusedCallback,
    (void *)WriteRuntimeOuterDualGlobalSessionAdjusted,
};

static void *g_NETWORK_RuntimeChannelStackDualThunk =
    g_NETWORK_RuntimeChannelStackOuterDualThunkVtable;
static void *g_NETWORK_RuntimeChannelStackTimedThunk =
    g_NETWORK_RuntimeChannelStackOuterTimedThunkVtable;
static void *g_NETWORK_RuntimeChannelStackFatalThunk =
    g_NETWORK_RuntimeChannelStackOuterFatalThunkVtable;
static VsNetRuntimeChannelStackVtableModel g_NETWORK_RuntimeChannelStackVtableModel;
void *g_NETWORK_RuntimeChannelStackVtable =
    *(void ***)&g_NETWORK_RuntimeChannelStackVtableModel;

void *g_pEffTransportPeerAddressState = 0;
extern int g_nEffTransportAsyncErrorStatus;

extern "C" DWORD WINAPI timeGetTime(void);

extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pEffTransportRuntimeService;
extern char *g_pszEffTransportBroadcastStatusPayload;
extern char *g_pszFileBasedNetworkLocalHostName;
extern void *g_pEffTransportPacketBuffer;
extern int g_cbEffTransportMaxPacketBytes;

extern void *ConstructShiftedLockedRecordSlotTable(void *pObject, int cPacketIds,
                                                    unsigned int cbRecord);
extern void *ConstructRangeEffTransportRecordBufferTable(void *pObject, int nFirstPacket,
                                                          int nLastPacket, int cbPayload,
                                                          unsigned int cbRecord);
extern void *ConstructRingLockedRecordSlotTable(void *pObject, int cRecords,
                                                 unsigned int cbRecord);
extern void *ConstructRingEffTransportRecordBufferTable(void *pObject, int cRecords,
                                                         int cbPayload, unsigned int cbRecord);
struct VsNetSimpleEffTransportRecordSlotTable {
    void *ConstructSimpleRecordSlotTableWrapper(int cRecords, unsigned int cbRecord);
};

struct VsNetVSMemObjectPointerArray {
    int m_cObjects;
    int m_nReserved04;
    int m_pObjectArray08;

    void DestroyVSMemObjectPointerArray(void);
};

static void *CreateActiveNetworkRuntimeServiceObject(void) {
    return ((VsNetRuntimeWindowInterface *)g_pActiveNetworkRuntimeWindow)->CreateServiceObject();
}

static void PumpActiveNetworkRuntimeWindow(void) {
    ((VsNetRuntimeWindowInterface *)g_pActiveNetworkRuntimeWindow)->PumpMessages();
}

// FUNCTION: LEMBALL 0x0040ABD0
int PASCAL ReturnTrueVtableCallback(void *) {
    return 1;
}

// FUNCTION: LEMBALL 0x00401564
int PASCAL ReturnTrueVtableCallbackThunk(void *pObject) {
    return ReturnTrueVtableCallback(pObject);
}

// FUNCTION: LEMBALL 0x0040ABE0
int ReturnTrueVtableCallbackSecondary(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x004026AD
int ReturnTrueVtableCallbackSecondaryThunk(void) {
    return ReturnTrueVtableCallbackSecondary();
}

// FUNCTION: LEMBALL 0x0040ABF0
void NoopVtableCallback(void) {}

// FUNCTION: LEMBALL 0x00401451
void NoopVtableCallbackThunk(void) {
    NoopVtableCallback();
}

// FUNCTION: LEMBALL 0x0045EF10
void VsNetEffStreamBase::WriteEffStreamU32BE(unsigned int nValue) {
    *(unsigned char *)(m_nReserved1c + 3) = (unsigned char)nValue;
    *(unsigned char *)(m_nReserved1c + 2) = (unsigned char)(nValue >> 8);
    *(unsigned char *)(m_nReserved1c + 1) = (unsigned char)(nValue >> 16);
    *(unsigned char *)m_nReserved1c = (unsigned char)(nValue >> 24);
    m_nReserved1c += 4;
}

// FUNCTION: LEMBALL 0x0045EF40
void VsNetEffStreamBase::WriteEffStreamU16BE(unsigned short nValue) {
    *(unsigned char *)(m_nReserved1c + 1) = (unsigned char)nValue;
    *(unsigned char *)m_nReserved1c = (unsigned char)(nValue >> 8);
    m_nReserved1c += 2;
}

// FUNCTION: LEMBALL 0x0045EF60
void VsNetEffStreamBase::WriteEffStreamU8(unsigned char nValue) {
    *m_nReserved1c = nValue;
    ++m_nReserved1c;
}

// FUNCTION: LEMBALL 0x0045EFC0
void VsNetEffStreamBase::WriteEffStreamBytes(const void *pvSource, unsigned int cbWrite) {
    unsigned int i;
    const unsigned char *pbSource;
    unsigned char *pbTarget;
    pbSource = (const unsigned char *)pvSource;
    pbTarget = (unsigned char *)(unsigned long)m_nReserved1c;

    for (i = cbWrite >> 2; i != 0; --i) {
        *(unsigned int *)pbTarget = *(const unsigned int *)pbSource;
        pbSource += 4;
        pbTarget += 4;
    }

    for (i = cbWrite & 3; i != 0; --i) {
        *pbTarget++ = *pbSource++;
    }

    m_nReserved1c += cbWrite;
}

// FUNCTION: LEMBALL 0x0045EF70
void VsNetEffStreamBase::WriteEffStreamCString(const char *pszSource) {
    WriteEffStreamBytes(pszSource, (unsigned int)strlen(pszSource) + 1);
}

// FUNCTION: LEMBALL 0x0045F010
void VsNetEffStreamBase::ReadEffStreamU32BE(unsigned char *pbTarget) {
    pbTarget[0] = *(unsigned char *)(m_nReserved20 + 3);
    pbTarget[1] = *(unsigned char *)(m_nReserved20 + 2);
    pbTarget[2] = *(unsigned char *)(m_nReserved20 + 1);
    pbTarget[3] = *(unsigned char *)m_nReserved20;
    m_nReserved20 += 4;
}

// FUNCTION: LEMBALL 0x0045EFF0
unsigned int VsNetEffStreamBase::ReadEffStreamU32BEValue(void) {
    unsigned int nValue;

    ReadEffStreamU32BE((unsigned char *)&nValue);
    return nValue;
}

// FUNCTION: LEMBALL 0x0045F040
void VsNetEffStreamBase::SwapEffStreamU32BEAndAdvance(void) {
    unsigned char nByte0;
    unsigned char nByte1;

    nByte0 = m_nReserved20[0];
    nByte1 = m_nReserved20[1];
    m_nReserved20[0] = m_nReserved20[3];
    m_nReserved20[1] = m_nReserved20[2];
    m_nReserved20[2] = nByte1;
    m_nReserved20[3] = nByte0;
    m_nReserved20 += 4;
}

// FUNCTION: LEMBALL 0x0045F070
unsigned short VsNetEffStreamBase::ReadEffStreamU16BEValue(void) {
    unsigned short nValue;

    ReadEffStreamU16BE((unsigned char *)&nValue);
    return nValue;
}

// FUNCTION: LEMBALL 0x0045F090
void VsNetEffStreamBase::ReadEffStreamU16BE(unsigned char *pbTarget) {
    pbTarget[0] = *(unsigned char *)(m_nReserved20 + 1);
    pbTarget[1] = *(unsigned char *)m_nReserved20;
    m_nReserved20 += 2;
}

// FUNCTION: LEMBALL 0x0045F0B0
void VsNetEffStreamBase::SwapEffStreamU16BEAndAdvance(void) {
    unsigned char nByte0;
    unsigned char nByte1;

    nByte1 = m_nReserved20[1];
    nByte0 = m_nReserved20[0];
    m_nReserved20[1] = nByte0;
    m_nReserved20[0] = nByte1;
    m_nReserved20 += 2;
}

// FUNCTION: LEMBALL 0x0045F0D0
unsigned char VsNetEffStreamBase::ReadEffStreamU8Value(void) {
    unsigned char nValue;

    ReadEffStreamU8(&nValue);
    return nValue;
}

// FUNCTION: LEMBALL 0x0045F0F0
void VsNetEffStreamBase::ReadEffStreamU8(unsigned char *pbTarget) {
    *pbTarget = *m_nReserved20;
    ++m_nReserved20;
}

// FUNCTION: LEMBALL 0x0045F110
void VsNetEffStreamBase::AdvanceEffStreamReadCursorByte(void) {
    ++m_nReserved20;
}

// FUNCTION: LEMBALL 0x0045F120
void VsNetEffStreamBase::ReadEffStreamCString(char **ppszTarget) {
    *ppszTarget = (char *)(unsigned long)m_nReserved20;
    m_nReserved20 += (int)strlen(*ppszTarget) + 1;
}

// FUNCTION: LEMBALL 0x0045F140
void VsNetEffStreamBase::ReadEffStreamRangePointer(void **ppvTarget, int cbRead) {
    *ppvTarget = m_nReserved20;
    m_nReserved20 += cbRead;
}

// FUNCTION: LEMBALL 0x0045F160
void VsNetEffStreamBase::ReadEffStreamCStringIntoBuffer(char *pszTarget) {
    strcpy(pszTarget, (char *)m_nReserved20);
    m_nReserved20 += strlen(pszTarget) + 1;
}

// FUNCTION: LEMBALL 0x0045F1B0
void VsNetEffStreamBase::ReadEffStreamBytes(void *pvTarget, unsigned int cbRead) {
    unsigned int i;
    unsigned char *pbTarget;
    unsigned char *pbSource;
    pbTarget = (unsigned char *)pvTarget;
    pbSource = (unsigned char *)(unsigned long)m_nReserved20;

    for (i = cbRead >> 2; i != 0; --i) {
        *(unsigned int *)pbTarget = *(unsigned int *)pbSource;
        pbSource += 4;
        pbTarget += 4;
    }

    for (i = cbRead & 3; i != 0; --i) {
        *pbTarget++ = *pbSource++;
    }

    m_nReserved20 += cbRead;
}

// FUNCTION: LEMBALL 0x0045F250
int VsNetEffStreamBase::SaveEffStreamToMemoryRange(int nTargetBuffer, int cbRange) {
    VsNetEffStreamSerializeVtable *pVtable;
    int (*pfnWriteBody)(void *pThis);

    m_pPayloadBuffer08 = nTargetBuffer;
    pVtable = (VsNetEffStreamSerializeVtable *)m_pVtable;
    m_nReserved1c = (unsigned char *)(unsigned long)(nTargetBuffer + cbRange);
    m_fOwnsPayload0c = m_nBufferEnd18 + nTargetBuffer + cbRange;
    pVtable->m_pBeginWriteHeader(this);
    pfnWriteBody = (int (*)(void *))pVtable->m_pBeginWriteBody;
    return pfnWriteBody(this);
}

// FUNCTION: LEMBALL 0x0045F680
VsNetEffStreamChannelState *VsNetEffStreamChannelState::ConstructEffStreamChannelState(void) {
    m_pVtable = (void **)g_NETWORK_EffStreamChannelStateVtable;
    m_nSelectedPeer08 = -1;
    m_nSelectedPort20 = 0xffff;
    m_nReserved10 = 0;
    m_nReserved0c = 0;
    m_nReserved14 = 0;
    m_nReserved18 = 0;
    m_nReserved1c = 0;
    m_nReserved24 = 0;
    m_nReserved04 = 0;
    m_pSideBuffer28 = AllocateVSMemBlock(0x10);
    return this;
}

// FUNCTION: LEMBALL 0x0045F6C0
void VsNetEffStreamChannelState::DestroyEffStreamChannelState(void) {
    m_pVtable = (void **)g_NETWORK_EffStreamChannelStateVtable;
    FreeVSMemBlock(m_pSideBuffer28);
}

// FUNCTION: LEMBALL 0x0045F6E0
void VsNetEffStreamChannelState::SetEffStreamChannelAsyncErrorStatus(int nStatus) {
    m_nReserved04 = nStatus;
    g_nEffTransportAsyncErrorStatus = nStatus;
    if (nStatus != 0 && m_nReserved0c != 0) {
        ((VsNetChannelStateRuntime *)this)->CancelPending();
        m_nReserved10 = 0;
        m_nReserved0c = 0;
        m_nSelectedPeer08 = -1;
        m_nReserved14 = 0;
    }
}

// FUNCTION: LEMBALL 0x0045F750
VsNetEffStreamBase *VsNetEffStreamBase::ConstructEffStreamBase(void) {
    m_pVtable = (void **)g_GAME_EffStreamConstructionVtable;
    m_nReserved04 = 0;
    ((GameEffStream *)this)->ResetStateFields();
    m_nWord34 = 0;
    m_pVtable = g_NETWORK_EffStreamBaseVtable;
    m_nBufferEnd18 += 0x10;
    m_nWord36 = 0;
    m_nWord30 = 0;
    m_nWord32 = 0;
    return this;
}

// FUNCTION: LEMBALL 0x0045EEA0
void VsNetEffStreamBase::DestroyEffStreamBase(void) {
    DWORD dwStartTime;

    m_pVtable = (void **)g_GAME_EffStreamConstructionVtable;
    if (m_fBusy28 != 0) {
        dwStartTime = timeGetTime();
        while (m_fBusy28 != 0 && timeGetTime() - dwStartTime < 2000) {
            if (g_pActiveNetworkRuntimeWindow != 0) {
                PumpActiveNetworkRuntimeWindow();
            }
        }
    }
    if (m_nReserved14 != 0) {
        FreeVSMemBlock((void *)(unsigned long)m_pPayloadBuffer08);
        m_pPayloadBuffer08 = 0;
    }
}

// FUNCTION: LEMBALL 0x0047B860
void VsNetEffStreamBase::DestroyEffStreamBaseThunk(void) {
    DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x0045F8D0
void VsNetDualHandleEffStream::ReleaseEffStreamPrimaryHandleGroup(void) {
    void *pHandleArray;
    void *pHandleObject;

    pHandleArray = m_pPrimaryHandleArray48;
    if (pHandleArray != 0) {
        ((VsNetVSMemObjectPointerArray *)pHandleArray)->DestroyVSMemObjectPointerArray();
        FreeVSMemBlock(pHandleArray);
    }

    pHandleObject = m_pPrimaryHandleObject4c;
    if (pHandleObject != 0) {
        ((void (*)(int))**(void ***)pHandleObject)(1);
    }
}

// FUNCTION: LEMBALL 0x0045F900
void VsNetDualHandleEffStream::ReleaseEffStreamSecondaryHandleGroup(void) {
    void *pHandleArray;
    void *pHandleObject;

    pHandleArray = m_pSecondaryHandleArray50;
    if (pHandleArray != 0) {
        ((VsNetVSMemObjectPointerArray *)pHandleArray)->DestroyVSMemObjectPointerArray();
        FreeVSMemBlock(pHandleArray);
    }

    pHandleObject = m_pSecondaryHandleObject54;
    if (pHandleObject != 0) {
        ((void (*)(int))**(void ***)pHandleObject)(1);
    }
}

// FUNCTION: LEMBALL 0x0045F930
void VsNetDualHandleEffStream::ConfigureEffStreamPrimaryHandleGroup(
    int nPacketCount, int cbPayload, int nMode) {
    void *pHandleArray;
    void *pHandleObject;

    ReleaseEffStreamPrimaryHandleGroup();
    pHandleArray = AllocateVSMemBlock(0x10);
    if (pHandleArray == 0) {
        m_pPrimaryHandleArray48 = 0;
    } else {
        m_pPrimaryHandleArray48 = ConstructShiftedLockedRecordSlotTable(
            pHandleArray, nPacketCount, (unsigned int)g_cbEffTransportMaxPacketBytes);
    }

    pHandleObject = AllocateVSMemBlock(0x28);
    if (pHandleObject == 0) {
        m_pPrimaryHandleObject4c = 0;
    } else {
        m_pPrimaryHandleObject4c = ConstructRangeEffTransportRecordBufferTable(
            pHandleObject, nPacketCount + 1, cbPayload, nMode,
            (unsigned int)g_cbEffTransportMaxPacketBytes);
    }
}

// FUNCTION: LEMBALL 0x0045F9B0
void VsNetDualHandleEffStream::ConfigureEffStreamSecondaryHandleGroup(
    int nPacketCount, int cbPayload) {
    void *pHandleArray;
    void *pHandleObject;

    ReleaseEffStreamSecondaryHandleGroup();
    pHandleArray = AllocateVSMemBlock(0x14);
    if (pHandleArray == 0) {
        m_pSecondaryHandleArray50 = 0;
    } else {
        m_pSecondaryHandleArray50 = ConstructRingLockedRecordSlotTable(
            pHandleArray, nPacketCount, (unsigned int)g_cbEffTransportMaxPacketBytes);
    }

    pHandleObject = AllocateVSMemBlock(0x20);
    if (pHandleObject == 0) {
        m_pSecondaryHandleObject54 = 0;
    } else {
        m_pSecondaryHandleObject54 = ConstructRingEffTransportRecordBufferTable(
            pHandleObject, nPacketCount, cbPayload,
            (unsigned int)g_cbEffTransportMaxPacketBytes);
    }

    if (g_pEffTransportPacketBuffer == 0) {
        g_pEffTransportPacketBuffer = AllocateVSMemBlock(
            (unsigned int)g_cbEffTransportMaxPacketBytes);
    }
}

// FUNCTION: LEMBALL 0x0045F820
void *ConstructDualHandleEffStream(void *pStream, int fConstructChannelState) {
    VsNetDualHandleEffStream *pDualStream;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;

    pDualStream = (VsNetDualHandleEffStream *)pStream;
    if (fConstructChannelState != 0) {
        pDualStream->m_pChannelStateConstructionOffsets44 = (void **)g_NETWORK_DualHandleEffStreamChannelStateConstructionOffsets;
        ((VsNetEffStreamChannelState *)pDualStream->m_abChannelState58)->ConstructEffStreamChannelState();
    }

    pDualStream->ConstructEffStreamBase();
    pDualStream->m_pVtable = (void **)g_NETWORK_DualHandleEffStreamVtable;
    pOffsets = (VsNetConstructionAdjustorVtable *)pDualStream->m_pChannelStateConstructionOffsets44;
    pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pDualStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_DualHandleEffStreamPrimaryThunkVtable;
    pDualStream->m_pPrimaryHandleArray48 = 0;
    pDualStream->m_pPrimaryHandleObject4c = 0;
    pDualStream->m_pSecondaryHandleArray50 = 0;
    pDualStream->m_pSecondaryHandleObject54 = 0;
    pDualStream->m_pTagBuffer2c = 0;

    if (g_pEffTransportPeerAddressState == 0) {
        g_pEffTransportPeerAddressState = CreateActiveNetworkRuntimeServiceObject();
    }

    return pStream;
}

// FUNCTION: LEMBALL 0x0045F8A0
void VsNetDualHandleEffStream::DestroyDualHandleEffStream(void) {
    VsNetDualHandleEffStream *pDualStream;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;

    pDualStream = this;
    pDualStream->m_pVtable = (void **)g_NETWORK_DualHandleEffStreamVtable;
    pOffsets = (VsNetConstructionAdjustorVtable *)pDualStream->m_pChannelStateConstructionOffsets44;
    pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pDualStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_DualHandleEffStreamPrimaryThunkVtable;
    ReleaseEffStreamPrimaryHandleGroup();
    ReleaseEffStreamSecondaryHandleGroup();
    DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x0045FDF0
void VsNetTimedEffStream::ReleaseTimedEffStreamPrimaryHandle(void) {
    void *pHandleArray;

    pHandleArray = m_pPrimaryHandleArray48;
    if (pHandleArray != 0) {
        ((VsNetVSMemObjectPointerArray *)pHandleArray)->DestroyVSMemObjectPointerArray();
        FreeVSMemBlock(pHandleArray);
    }
}

// FUNCTION: LEMBALL 0x0045FE50
void VsNetTimedEffStream::ReleaseTimedEffStreamPrimaryHandleThunk(
    int nUnused0, int nUnused1, int nUnused2) {
    (void)nUnused0;
    (void)nUnused1;
    (void)nUnused2;
    ReleaseTimedEffStreamPrimaryHandle();
}

// FUNCTION: LEMBALL 0x0045FE10
void VsNetTimedEffStream::ReleaseTimedEffStreamSecondaryHandles(void) {
    void *pHandleArray;

    pHandleArray = m_pSecondaryHandleArray4c;
    if (pHandleArray != 0) {
        ((VsNetVSMemObjectPointerArray *)pHandleArray)->DestroyVSMemObjectPointerArray();
        FreeVSMemBlock(pHandleArray);
    }

    pHandleArray = m_pTertiaryHandleArray50;
    if (pHandleArray != 0) {
        ((VsNetVSMemObjectPointerArray *)pHandleArray)->DestroyVSMemObjectPointerArray();
        FreeVSMemBlock(pHandleArray);
    }
}

// FUNCTION: LEMBALL 0x0045FE60
void VsNetTimedEffStream::ConfigureTimedEffStreamSecondaryHandle(int nPacketCount) {
    void *pHandleArray;

    ReleaseTimedEffStreamSecondaryHandles();
    pHandleArray = AllocateVSMemBlock(0x0c);
    if (pHandleArray == 0) {
        m_pSecondaryHandleArray4c = 0;
    } else {
        m_pSecondaryHandleArray4c =
            ((VsNetSimpleEffTransportRecordSlotTable *)pHandleArray)
                ->ConstructSimpleRecordSlotTableWrapper(
                    nPacketCount, (unsigned int)g_cbEffTransportMaxPacketBytes);
    }
}

// FUNCTION: LEMBALL 0x0045FCE0
VsNetTimedEffStream *VsNetTimedEffStream::ConstructTimedEffStream(int fConstructChannelState) {
    DWORD dwNow;
    int *pHandleArray;
    VsNetTimedEffStream *pTimedStream;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;

    pTimedStream = this;
    if (fConstructChannelState != 0) {
        pTimedStream->m_pChannelStateConstructionOffsets44 = (void **)g_NETWORK_TimedEffStreamChannelStateConstructionOffsets;
        ((VsNetEffStreamChannelState *)pTimedStream->m_abChannelState78)->ConstructEffStreamChannelState();
    }

    ConstructEffStreamBase();
    pTimedStream->m_pVtable = (void **)g_NETWORK_TimedEffStreamVtable;
    pOffsets = (VsNetConstructionAdjustorVtable *)pTimedStream->m_pChannelStateConstructionOffsets44;
    pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pTimedStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TimedEffStreamPrimaryThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x34;

    dwNow = timeGetTime();
    pTimedStream->m_dwLastTick3c = dwNow - 1000;

    pHandleArray = (int *)AllocateVSMemBlock(0x10);
    pTimedStream->m_pTagBuffer2c = (VsNetEffStreamTag *)pHandleArray;
    *pHandleArray = 0x56533039;

    pTimedStream->m_pPrimaryHandleArray48 = 0;
    pTimedStream->m_pSecondaryHandleArray4c = 0;
    pTimedStream->m_pTertiaryHandleArray50 = 0;
    pTimedStream->m_nReserved54 = 0;
    pTimedStream->m_nHandleIndex5c = -1;
    pTimedStream->m_pRuntimeService70 = CreateActiveNetworkRuntimeServiceObject();
    return this;
}

// FUNCTION: LEMBALL 0x0045FD80
void VsNetTimedEffStream::DestroyTimedEffStream(void) {
    VsNetTimedEffStream *pTimedStream;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;

    pTimedStream = this;
    pTimedStream->m_pVtable = (void **)g_NETWORK_TimedEffStreamVtable;
    pOffsets = (VsNetConstructionAdjustorVtable *)pTimedStream->m_pChannelStateConstructionOffsets44;
    pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pTimedStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TimedEffStreamPrimaryThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x34;

    FreeVSMemBlock(pTimedStream->m_pRuntimeService70);
    pTimedStream->m_pRuntimeService70 = 0;
    FreeVSMemBlock(pTimedStream->m_pTagBuffer2c);
    ReleaseTimedEffStreamPrimaryHandle();
    ReleaseTimedEffStreamSecondaryHandles();
    DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x0045FDE0
void VsNetTimedEffStream::InvokeTimedEffStreamServiceCallback(void *pArgument) {
    ((VsNetRuntimeService *)m_pRuntimeService70)->Invoke(pArgument);
}

// FUNCTION: LEMBALL 0x004604E0
void VsNetRuntimeChannelStackReleaseFront::Release(void) {
    VsNetRuntimeChannelStack *pChannelStack;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    void *pRuntimeService;

    pChannelStack = (VsNetRuntimeChannelStack *)((char *)this - 0x24);
    pChannelStack->m_pVtable = (void **)g_NETWORK_RuntimeChannelStackVtable;
    pOffsets = (VsNetConstructionAdjustorVtable *)pChannelStack->m_pOuterConstructionOffsets04;
    pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pChannelStack + 4 + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)((char *)pChannelStack + 4 + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)((char *)pChannelStack + 4 + pOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
    pSecondaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackTimedThunk;
    pTertiaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackDualThunk;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x20;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x50;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 200;

    if (pChannelStack->m_pRuntimeSideBuffer1c != 0) {
        FreeVSMemBlock(pChannelStack->m_pRuntimeSideBuffer1c);
    }
    FreeVSMemBlock(pChannelStack->m_pbRuntimeFlags10);
    pRuntimeService = g_pEffTransportRuntimeService;
    FreeVSMemBlock(pRuntimeService);
    g_pEffTransportRuntimeService = 0;
    if (g_pszFileBasedNetworkLocalHostName != 0) {
        FreeVSMemBlock(g_pszFileBasedNetworkLocalHostName);
        g_pszFileBasedNetworkLocalHostName = 0;
    }
    if (g_pszEffTransportBroadcastStatusPayload != 0) {
        FreeVSMemBlock(g_pszEffTransportBroadcastStatusPayload);
        g_pszEffTransportBroadcastStatusPayload = 0;
    }
}

// FUNCTION: LEMBALL 0x00460350
void *VsNetRuntimeChannelStack::ConstructEffTransportRuntimeChannelStack(int fConstructEmbeddedObjects) {
    volatile unsigned int nConstructionFlags;
    int i;

    nConstructionFlags = 0;
    if (fConstructEmbeddedObjects != 0) {
        m_pOuterConstructionOffsets04 = (void **)g_NETWORK_RuntimeChannelStackOuterConstructionOffsets;
        ((VsNetTimedEffStream *)m_abTimedStream54)->m_pChannelStateConstructionOffsets44 =
            (void **)g_NETWORK_RuntimeChannelStackTimedStreamConstructionOffsets;
        ((VsNetDualHandleEffStream *)m_abDualStreamcc)->m_pChannelStateConstructionOffsets44 =
            (void **)g_NETWORK_RuntimeChannelStackDualStreamConstructionOffsets;
        m_pEmbeddedConstructionOffsets124 = (void **)g_NETWORK_RuntimeChannelStackChannelStateConstructionOffsets;

        nConstructionFlags |= 1;
        ((VsNetEffStreamChannelState *)m_abChannelState24)->ConstructEffStreamChannelState();
        nConstructionFlags |= 2;
        ((VsNetTimedEffStream *)m_abTimedStream54)->ConstructTimedEffStream(0);
        nConstructionFlags |= 4;
        ConstructDualHandleEffStream((VsNetDualHandleEffStream *)m_abDualStreamcc, 0);

        nConstructionFlags |= 8;
        ((VsNetAdjustorSubobject *)((char *)this + 0x124 +
            ((VsNetConstructionAdjustorVtable *)m_pEmbeddedConstructionOffsets124)->m_nPrimaryOffset - 4))->m_pVtable =
            g_NETWORK_RuntimeChannelStackChannelStateVtable;
        /* 00499008 and 00498fe0 are four-slot stream-prefix tables, not
         * one-slot return-value tables.  Their first four entries are the
         * shared stream prefix: true, true, write-tagged-header, noop. */
        ((VsNetAdjustorSubobject *)((char *)this + 0x124 +
            ((VsNetConstructionAdjustorVtable *)m_pEmbeddedConstructionOffsets124)->m_nSecondaryOffset - 4))->m_pVtable =
            g_NETWORK_RuntimeChannelStackTimedStreamVtable;
        ((VsNetAdjustorSubobject *)((char *)this + 0x124 +
            ((VsNetConstructionAdjustorVtable *)m_pEmbeddedConstructionOffsets124)->m_nTertiaryOffset - 4))->m_pVtable =
            g_NETWORK_RuntimeChannelStackDualStreamVtable;
        ((VsNetAdjustorSubobject *)((char *)this + 0x124 +
            ((VsNetConstructionAdjustorVtable *)m_pEmbeddedConstructionOffsets124)->m_nPrimaryOffset - 4))->m_nThisDelta =
            ((VsNetConstructionAdjustorVtable *)m_pEmbeddedConstructionOffsets124)->m_nPrimaryOffset - 8;
        ((VsNetAdjustorSubobject *)((char *)this + 0x124 +
            ((VsNetConstructionAdjustorVtable *)m_pEmbeddedConstructionOffsets124)->m_nSecondaryOffset - 4))->m_nThisDelta =
            ((VsNetConstructionAdjustorVtable *)m_pEmbeddedConstructionOffsets124)->m_nSecondaryOffset - 0x38;
        ((VsNetAdjustorSubobject *)((char *)this + 0x124 +
            ((VsNetConstructionAdjustorVtable *)m_pEmbeddedConstructionOffsets124)->m_nTertiaryOffset - 4))->m_nThisDelta =
            ((VsNetConstructionAdjustorVtable *)m_pEmbeddedConstructionOffsets124)->m_nTertiaryOffset - 0xb0;
    }

    m_pVtable = (void **)g_NETWORK_RuntimeChannelStackVtable;
    ((VsNetAdjustorSubobject *)((char *)this + 4 +
        ((VsNetConstructionAdjustorVtable *)m_pOuterConstructionOffsets04)->m_nPrimaryOffset - 4))->m_pVtable =
        g_NETWORK_RuntimeChannelStackFatalThunk;
    ((VsNetAdjustorSubobject *)((char *)this + 4 +
        ((VsNetConstructionAdjustorVtable *)m_pOuterConstructionOffsets04)->m_nSecondaryOffset - 4))->m_pVtable =
        g_NETWORK_RuntimeChannelStackTimedThunk;
    ((VsNetAdjustorSubobject *)((char *)this + 4 +
        ((VsNetConstructionAdjustorVtable *)m_pOuterConstructionOffsets04)->m_nTertiaryOffset - 4))->m_pVtable =
        g_NETWORK_RuntimeChannelStackDualThunk;
    ((VsNetAdjustorSubobject *)((char *)this + 4 +
        ((VsNetConstructionAdjustorVtable *)m_pOuterConstructionOffsets04)->m_nPrimaryOffset - 4))->m_nThisDelta =
        ((VsNetConstructionAdjustorVtable *)m_pOuterConstructionOffsets04)->m_nPrimaryOffset - 0x20;
    ((VsNetAdjustorSubobject *)((char *)this + 4 +
        ((VsNetConstructionAdjustorVtable *)m_pOuterConstructionOffsets04)->m_nSecondaryOffset - 4))->m_nThisDelta =
        ((VsNetConstructionAdjustorVtable *)m_pOuterConstructionOffsets04)->m_nSecondaryOffset - 0x50;
    ((VsNetAdjustorSubobject *)((char *)this + 4 +
        ((VsNetConstructionAdjustorVtable *)m_pOuterConstructionOffsets04)->m_nTertiaryOffset - 4))->m_nThisDelta =
        ((VsNetConstructionAdjustorVtable *)m_pOuterConstructionOffsets04)->m_nTertiaryOffset - 200;

    m_nReserved14 = 0;
    m_nReserved08 = 0;
    m_nReserved0c = 0;
    m_pRuntimeSideBuffer1c = 0;

    g_pEffTransportRuntimeService =
        ((VsNetRuntimeWindowInterface *)g_pActiveNetworkRuntimeWindow)->CreateServiceObject();
    m_pbRuntimeFlags10 = (unsigned char *)AllocateVSMemBlock(0x200);
    i = 0;
    do {
        ++i;
        m_pbRuntimeFlags10[i - 1] = 0;
    } while (i < 0x200);

    return this;
}

// FUNCTION: LEMBALL 0x00462810
void *DeleteEffStreamBaseWrapper00462810(void *pObject, BYTE fDelete) {
    ((VsNetEffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462830
void *DeleteEffStreamBaseWrapper00462830(void *pObject, BYTE fDelete) {
    ((VsNetEffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462850
void *DeleteEffStreamBaseWrapper00462850(void *pObject, BYTE fDelete) {
    ((VsNetEffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462870
void *DeleteEffStreamBaseWrapper00462870(void *pObject, BYTE fDelete) {
    ((VsNetEffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462890
void *DeleteEffStreamBaseWrapper00462890(void *pObject, BYTE fDelete) {
    ((VsNetEffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004628B0
void *DeleteEffStreamBaseWrapper004628B0(void *pObject, BYTE fDelete) {
    ((VsNetEffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004628D0
void EffStreamChannelStateRet4Thunk(BYTE fDelete) {
    (void)fDelete;
}

// FUNCTION: LEMBALL 0x004628E0
void *DeleteEffStreamChannelStateWrapper004628E0(void *pObject, BYTE fDelete) {
    ((VsNetEffStreamChannelState *)pObject)->DestroyEffStreamChannelState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462900
void *DeleteEffStreamBaseWrapper00462900(void *pObject, BYTE fDelete) {
    ((VsNetEffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462940
void *DeleteDualEffStreamWithChannelStateWrapper(void *pObject, BYTE fDelete) {
    ((VsNetDualHandleEffStream *)pObject)->DestroyDualHandleEffStream();
    ((VsNetEffStreamChannelState *)((char *)pObject + 0x58))->DestroyEffStreamChannelState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462990
void *DeleteTimedEffStreamWithChannelStateWrapper(void *pObject, BYTE fDelete) {
    ((VsNetTimedEffStream *)pObject)->DestroyTimedEffStream();
    ((VsNetEffStreamChannelState *)((char *)pObject + 0x78))->DestroyEffStreamChannelState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462B40
void LEMBALL_FASTCALL WriteEffStreamTwoU16Fields(void *pObject) {
    ((VsNetEffStreamBase *)pObject)->WriteEffStreamU16BE(*(unsigned short *)((char *)pObject + 0x2c));
    ((VsNetEffStreamBase *)pObject)->WriteEffStreamU16BE(*(unsigned short *)((char *)pObject + 0x2e));
}

// FUNCTION: LEMBALL 0x00462B00
void ReturnVoidVtableCallback(void) {}

// FUNCTION: LEMBALL 0x00462B60
void LEMBALL_FASTCALL ReadEffStreamTwoU16Fields(void *pObject) {
    ((VsNetEffStreamBase *)pObject)->ReadEffStreamU16BE((unsigned char *)((char *)pObject + 0x2c));
    ((VsNetEffStreamBase *)pObject)->ReadEffStreamU16BE((unsigned char *)((char *)pObject + 0x2e));
}

// FUNCTION: LEMBALL 0x00462B80
void *DeleteEffStreamBase(void *pObject, BYTE fDelete) {
    ((VsNetEffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462CB0
void *DeleteEffChannelStreamStackWrapper(void *pChannelState, BYTE fDelete) {
    VsNetRuntimeChannelStackWrapperView *pView;

    pView = (VsNetRuntimeChannelStackWrapperView *)pChannelState;
    ((VsNetDualHandleEffStream *)((char *)pChannelState + 0xa8))->DestroyDualHandleEffStream();
    ((VsNetTimedEffStream *)((char *)pChannelState + 0x30))->DestroyTimedEffStream();
    ((VsNetEffStreamChannelState *)pChannelState)->DestroyEffStreamChannelState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock((char *)pChannelState - 8);
    }
    return (char *)pView - 8;
}

// FUNCTION: LEMBALL 0x00462CA0
void *DeleteEffChannelStreamStackAdjustedThunk(void *pObject, BYTE fDelete) {
    return DeleteEffChannelStreamStackWrapper(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta,
        fDelete);
}

// FUNCTION: LEMBALL 0x00462CF0
    void ClearRuntimeChannelPendingWriteAdjustedThunk(void *pObject, void *pUnused) {
    int nVirtualBaseOffset;
    char *pPendingWrite;

    pObject = (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta;
    nVirtualBaseOffset = *(int *)(*(int *)((char *)pObject - 8) + 8);
    pPendingWrite = (char *)pObject + nVirtualBaseOffset + 0x70;
    ((VsNetEffTransportPendingWriteState *)pPendingWrite)->Clear(pUnused);
}
// FUNCTION: LEMBALL 0x00462ED0
int WINAPI InitializeNonZrleVariantRenderEntry(int nValue) {
    return nValue;
}
