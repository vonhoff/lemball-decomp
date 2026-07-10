#include "../game/game_app.h"
#include "../engine/memory_arena.h"
#include "network/stream.h"
#include "network/safe_vtable.h"

struct GAME_EffStream {
    void ResetStateFields(void);
};

struct NETWORK_ConstructionAdjustorVtable {
    void *m_pReserved00;
    int m_nPrimaryOffset;
    int m_nSecondaryOffset;
    int m_nTertiaryOffset;
    int m_nQuaternaryOffset;
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

struct NETWORK_EffStreamSerializeVtable {
    void *m_pReserved00;
    void (*m_pLoadFromMemoryHeader)(void *pThis);
    void (*m_pLoadFromMemoryBody)(void *pThis);
    void (*m_pBeginWriteHeader)(void *pThis);
    void (*m_pBeginWriteBody)(void *pThis);
};

struct NETWORK_ChannelStateRuntime {
    virtual void CancelPending(void) = 0;
};

struct NETWORK_RuntimeService {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Invoke(void *pArgument) = 0;
};

struct NETWORK_RuntimeChannelStack {
    void **m_pVtable;
    void **m_pOuterConstructionOffsets04;
    int m_nReserved08;
    int m_nReserved0c;
    unsigned char *m_pbRuntimeFlags10;
    int m_nReserved14;
    int m_nReserved18;
    void *m_pRuntimeSideBuffer1c;
    unsigned char m_abChannelState24[0x30];
    unsigned char m_abTimedStream54[0x78];
    unsigned char m_abDualStreamcc[0x78];
    void **m_pEmbeddedConstructionOffsets124;

    void *ConstructEffTransportRuntimeChannelStack(int fConstructEmbeddedObjects);
};

extern void CrtFatalRuntimeError0x19(void);
extern void ServiceEffTransportConnectRequest(void *pObject);

/* Ghidra: original table at 004990e8 has 8 slots:
   claim flag, clear flag, fatal, fatal, service, fatal, fatal, null.
   Use compiler-owned vtable storage; slot 7 is safe no-op for runtime use. */
struct NETWORK_RuntimeChannelStackVtableModel {
    virtual short ClaimRuntimeFlag(const char *pszFlags) {
        NETWORK_RuntimeChannelStack *pStack = (NETWORK_RuntimeChannelStack *)this;
        short i;
        for (i = 0; i < 0x200; ++i) {
            if (pszFlags[i] == 0 && pStack->m_pbRuntimeFlags10[i] == 0) {
                pStack->m_pbRuntimeFlags10[i] = 1;
                return i;
            }
        }
        return -1;
    }
    virtual void ClearRuntimeFlag(short nFlag) {
        ((NETWORK_RuntimeChannelStack *)this)->m_pbRuntimeFlags10[nFlag] = 0;
    }
    virtual void FatalRuntimeSlot02(void) { CrtFatalRuntimeError0x19(); }
    virtual void FatalRuntimeSlot03(void) { CrtFatalRuntimeError0x19(); }
    virtual void ServiceRuntimeConnect(void) {
        ServiceEffTransportConnectRequest(this);
    }
    virtual void FatalRuntimeSlot05(void) { CrtFatalRuntimeError0x19(); }
    virtual void FatalRuntimeSlot06(void) { CrtFatalRuntimeError0x19(); }
    virtual void UnusedRuntimeSlot07(void) {}
};

struct NETWORK_RuntimeChannelStackWrapperView {
    unsigned char m_abChannelState00[0x30];
    unsigned char m_abTimedStream30[0x78];
    unsigned char m_abDualStreamA8[0x78];
};

struct NETWORK_RuntimeWindowInterface {
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
void EffStreamChannelStateRet4Thunk(BYTE fDelete);
static void *g_NETWORK_EffStreamChannelStateVtable[] = {
    (void *)CrtFatalRuntimeError0x19,
    (void *)DeleteEffStreamChannelStateWrapper004628E0,
    (void *)CrtFatalRuntimeError0x19,
    (void *)EffStreamChannelStateRet4Thunk,
};
struct NETWORK_EffStreamBaseVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }

    virtual void WriteTaggedHeader(void) {
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
        unsigned char *pbTag = (unsigned char *)pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
        pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
        *(unsigned char *)(pStream->m_nReserved1c) = pbTag[0xe];
        ++pStream->m_nReserved1c;
    }

    virtual void Noop(void) {}

    virtual void ReverseTaggedPayload(void) {
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
        pStream->DestroyEffStreamBase();
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pStream);
        }
        return pStream;
    }
};

static NETWORK_EffStreamBaseVtableModel g_NETWORK_EffStreamBaseVtableModel;
static void *g_NETWORK_EffStreamBaseVtable =
    *(void ***)&g_NETWORK_EffStreamBaseVtableModel;
static int g_NETWORK_DualHandleEffStreamChannelStateConstructionOffsets[2] = {
    -0x44, 0x14,
};
void *DeleteDualEffStreamWithChannelStateWrapper(void *pObject, BYTE fDelete);
void *DeleteTimedEffStreamWithChannelStateWrapper(void *pObject, BYTE fDelete);

struct NETWORK_DualPrimaryThunkVtableModel {
    virtual void Fatal(void) { CrtFatalRuntimeError0x19(); }
    virtual void *Delete(BYTE fDelete) {
        return DeleteDualEffStreamWithChannelStateWrapper((char *)this - 0x58, fDelete);
    }
    virtual void FatalSecondary(void) { CrtFatalRuntimeError0x19(); }
    virtual void Ret4(BYTE fDelete) { (void)fDelete; }
};

static NETWORK_DualPrimaryThunkVtableModel g_NETWORK_DualPrimaryThunkVtableModel;
static void *g_NETWORK_DualHandleEffStreamPrimaryThunkVtable =
    *(void ***)&g_NETWORK_DualPrimaryThunkVtableModel;
struct NETWORK_DualHandleEffStreamVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
        unsigned char *pbTag = (unsigned char *)pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
        pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
        *(unsigned char *)pStream->m_nReserved1c = pbTag[0xe];
        ++pStream->m_nReserved1c;
    }
    virtual void *Delete(BYTE fDelete) {
        NETWORK_DualHandleEffStream *pStream = (NETWORK_DualHandleEffStream *)this;
        pStream->DestroyDualHandleEffStream();
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pStream);
        }
        return pStream;
    }
    virtual void DeleteRet(void) {}
    virtual void Fatal(void) { CrtFatalRuntimeError0x19(); }
    virtual void FatalSecondary(void) { CrtFatalRuntimeError0x19(); }
    virtual void DeleteRet8(BYTE fDelete, BYTE fReserved) {
        (void)fDelete;
        (void)fReserved;
    }
};

static NETWORK_DualHandleEffStreamVtableModel g_NETWORK_DualHandleEffStreamVtableModel;
static void *g_NETWORK_DualHandleEffStreamVtable =
    *(void ***)&g_NETWORK_DualHandleEffStreamVtableModel;
static int g_NETWORK_TimedEffStreamChannelStateConstructionOffsets[2] = {
    -0x44, 0x34,
};
struct NETWORK_TimedPrimaryThunkVtableModel {
    virtual void Fatal(void) { CrtFatalRuntimeError0x19(); }
    virtual void *Delete(BYTE fDelete) {
        return DeleteTimedEffStreamWithChannelStateWrapper((char *)this - 0x78, fDelete);
    }
    virtual void FatalSecondary(void) { CrtFatalRuntimeError0x19(); }
    virtual void SecondaryThunk(void) {}
};

static NETWORK_TimedPrimaryThunkVtableModel g_NETWORK_TimedPrimaryThunkVtableModel;
static void *g_NETWORK_TimedEffStreamPrimaryThunkVtable =
    *(void ***)&g_NETWORK_TimedPrimaryThunkVtableModel;
struct NETWORK_PeerPayloadSender {
    void WriteEffStreamWithGlobalSession(void);
};
extern void *ClaimAckedEffTransportRecordPayload(void *pObject);

struct NETWORK_TimedEffStreamVtableModel : NETWORK_EffStreamBaseVtableModel {
    virtual void *Delete(BYTE fDelete) {
        NETWORK_TimedEffStream *pStream = (NETWORK_TimedEffStream *)this;
        pStream->DestroyTimedEffStream();
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pStream);
        }
        return pStream;
    }

    virtual void WriteGlobalSession(void) {
        ((NETWORK_PeerPayloadSender *)this)->WriteEffStreamWithGlobalSession();
    }

    virtual void *ClaimTimedRecord(void) {
        return ClaimAckedEffTransportRecordPayload(this);
    }
};

static NETWORK_TimedEffStreamVtableModel g_NETWORK_TimedEffStreamVtableModel;
static void *g_NETWORK_TimedEffStreamVtable =
    *(void ***)&g_NETWORK_TimedEffStreamVtableModel;
/* Exact construction-offset data recovered from the runtime stack stores. */
static int g_NETWORK_RuntimeChannelStackChannelStateConstructionOffsets[6] = {
    0, -0x100, -0xd0, -0x58, -0x44, -0x14,
};
/* Separate callable table.  The offset table above is not a vtable. */
static void *g_NETWORK_RuntimeChannelStackChannelStateVtable[4] = {
    (void *)CrtFatalRuntimeError0x19,
    (void *)NetworkSafeVtableNoop,
    (void *)CrtFatalRuntimeError0x19,
    (void *)NetworkSafeVtableNoop,
};
static int g_NETWORK_RuntimeChannelStackTimedStreamConstructionOffsets[6] = {
    -0x44, -0x74, -0x44, -0x8c, -4, 0x20,
};
static int g_NETWORK_RuntimeChannelStackDualStreamConstructionOffsets[6] = {
    -0x44, -0xec, -0x44, -0x74, -4, 0x20,
};
static int g_NETWORK_RuntimeChannelStackOuterConstructionOffsets[6] = {
    -4, 0x20, 0x50, 0xc8, 0x120, 0,
};

struct NETWORK_RuntimeThunkFallbackVtableModel {
    virtual void Fatal(void) { CrtFatalRuntimeError0x19(); }
    virtual void *Delete(BYTE fDelete) {
        (void)fDelete;
        return this;
    }
    virtual void FatalSecondary(void) { CrtFatalRuntimeError0x19(); }
    virtual void Noop(BYTE fDelete) { (void)fDelete; }
};

static NETWORK_RuntimeThunkFallbackVtableModel g_NETWORK_RuntimeThunkFallbackVtableModel;
static void *g_NETWORK_RuntimeChannelStackDualThunk =
    *(void ***)&g_NETWORK_RuntimeThunkFallbackVtableModel;
static void *g_NETWORK_RuntimeChannelStackTimedThunk =
    *(void ***)&g_NETWORK_RuntimeThunkFallbackVtableModel;
static void *g_NETWORK_RuntimeChannelStackFatalThunk =
    *(void ***)&g_NETWORK_RuntimeThunkFallbackVtableModel;
static NETWORK_RuntimeChannelStackVtableModel g_NETWORK_RuntimeChannelStackVtableModel;
static void *g_NETWORK_RuntimeChannelStackVtable =
    *(void ***)&g_NETWORK_RuntimeChannelStackVtableModel;

void *g_pEffTransportPeerAddressState = 0;
extern int g_nEffTransportAsyncErrorStatus;

extern "C" DWORD timeGetTime(void);

extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pEffTransportRuntimeService;
extern void *g_pEffTransportScratchBuffer;

extern void DestroyVSMemObjectPointerArray(void *pObjectArray);

static void *CreateActiveNetworkRuntimeServiceObject(void) {
    return ((NETWORK_RuntimeWindowInterface *)g_pActiveNetworkRuntimeWindow)->CreateServiceObject();
}

static void PumpActiveNetworkRuntimeWindow(void) {
    ((NETWORK_RuntimeWindowInterface *)g_pActiveNetworkRuntimeWindow)->PumpMessages();
}

// FUNCTION: LEMBALL 0x0040ABD0
int ReturnTrueVtableCallback(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x0040ABE0
int ReturnTrueVtableCallbackSecondary(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x0040ABF0
void NoopVtableCallback(void) {}

static void *g_NETWORK_ReturnTrueVtable[1] = {
    (void *)ReturnTrueVtableCallback,
};

// FUNCTION: LEMBALL 0x0045EF10
void NETWORK_EffStreamBase::WriteEffStreamU32BE(unsigned int nValue) {
    *(unsigned char *)(m_nReserved1c + 3) = (unsigned char)nValue;
    *(unsigned char *)(m_nReserved1c + 2) = (unsigned char)(nValue >> 8);
    *(unsigned char *)(m_nReserved1c + 1) = (unsigned char)(nValue >> 16);
    *(unsigned char *)m_nReserved1c = (unsigned char)(nValue >> 24);
    m_nReserved1c += 4;
}

// FUNCTION: LEMBALL 0x0045EF40
void NETWORK_EffStreamBase::WriteEffStreamU16BE(unsigned short nValue) {
    *(unsigned char *)(m_nReserved1c + 1) = (unsigned char)nValue;
    *(unsigned char *)m_nReserved1c = (unsigned char)(nValue >> 8);
    m_nReserved1c += 2;
}

// FUNCTION: LEMBALL 0x0045EFC0
void NETWORK_EffStreamBase::WriteEffStreamBytes(const void *pvSource, unsigned int cbWrite) {
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

// FUNCTION: LEMBALL 0x0045F010
void NETWORK_EffStreamBase::ReadEffStreamU32BE(unsigned char *pbTarget) {
    pbTarget[0] = *(unsigned char *)(m_nReserved20 + 3);
    pbTarget[1] = *(unsigned char *)(m_nReserved20 + 2);
    pbTarget[2] = *(unsigned char *)(m_nReserved20 + 1);
    pbTarget[3] = *(unsigned char *)m_nReserved20;
    m_nReserved20 += 4;
}

// FUNCTION: LEMBALL 0x0045F090
void NETWORK_EffStreamBase::ReadEffStreamU16BE(unsigned char *pbTarget) {
    pbTarget[0] = *(unsigned char *)(m_nReserved20 + 1);
    pbTarget[1] = *(unsigned char *)m_nReserved20;
    m_nReserved20 += 2;
}

// FUNCTION: LEMBALL 0x0045F1B0
void NETWORK_EffStreamBase::ReadEffStreamBytes(void *pvTarget, unsigned int cbRead) {
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
void NETWORK_EffStreamBase::SaveEffStreamToMemoryRange(int nTargetBuffer, int cbRange) {
    NETWORK_EffStreamSerializeVtable *pVtable;

    m_pPayloadBuffer08 = nTargetBuffer;
    pVtable = (NETWORK_EffStreamSerializeVtable *)m_pVtable;
    m_nReserved1c = nTargetBuffer + cbRange;
    m_fOwnsPayload0c = m_nBufferEnd18 + nTargetBuffer + cbRange;
    pVtable->m_pBeginWriteHeader(this);
    pVtable->m_pBeginWriteBody(this);
}

// FUNCTION: LEMBALL 0x0045F680
NETWORK_EffStreamChannelState *NETWORK_EffStreamChannelState::ConstructEffStreamChannelState(void) {
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
void NETWORK_EffStreamChannelState::DestroyEffStreamChannelState(void) {
    m_pVtable = (void **)g_NETWORK_EffStreamChannelStateVtable;
    FreeVSMemBlock(m_pSideBuffer28);
}

// FUNCTION: LEMBALL 0x0045F6E0
void NETWORK_EffStreamChannelState::SetEffStreamChannelAsyncErrorStatus(int nStatus) {
    m_nReserved04 = nStatus;
    g_nEffTransportAsyncErrorStatus = nStatus;
    if (nStatus != 0 && m_nReserved0c != 0) {
        ((NETWORK_ChannelStateRuntime *)this)->CancelPending();
        m_nReserved10 = 0;
        m_nReserved0c = 0;
        m_nSelectedPeer08 = -1;
        m_nReserved14 = 0;
    }
}

// FUNCTION: LEMBALL 0x0045F750
NETWORK_EffStreamBase *NETWORK_EffStreamBase::ConstructEffStreamBase(void) {
    m_pVtable = (void **)g_NETWORK_ReturnTrueVtable;
    m_nReserved04 = 0;
    ((GAME_EffStream *)this)->ResetStateFields();
    m_nWord34 = 0;
    m_pVtable = (void **)g_NETWORK_EffStreamBaseVtable;
    m_nBufferEnd18 += 0x10;
    m_nWord36 = 0;
    m_nWord30 = 0;
    m_nWord32 = 0;
    return this;
}

// FUNCTION: LEMBALL 0x0045EEA0
void NETWORK_EffStreamBase::DestroyEffStreamBase(void) {
    DWORD dwStartTime;

    m_pVtable = (void **)g_NETWORK_ReturnTrueVtable;
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
void NETWORK_EffStreamBase::DestroyEffStreamBaseThunk(void) {
    DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x0045F8D0
void NETWORK_DualHandleEffStream::ReleaseEffStreamPrimaryHandleGroup(void) {
    void *pHandleArray;
    void *pHandleObject;

    pHandleArray = m_pPrimaryHandleArray48;
    if (pHandleArray != 0) {
        DestroyVSMemObjectPointerArray(pHandleArray);
        FreeVSMemBlock(pHandleArray);
    }

    pHandleObject = m_pPrimaryHandleObject4c;
    if (pHandleObject != 0) {
        ((void (*)(int))**(void ***)pHandleObject)(1);
    }
}

// FUNCTION: LEMBALL 0x0045F900
void NETWORK_DualHandleEffStream::ReleaseEffStreamSecondaryHandleGroup(void) {
    void *pHandleArray;
    void *pHandleObject;

    pHandleArray = m_pSecondaryHandleArray50;
    if (pHandleArray != 0) {
        DestroyVSMemObjectPointerArray(pHandleArray);
        FreeVSMemBlock(pHandleArray);
    }

    pHandleObject = m_pSecondaryHandleObject54;
    if (pHandleObject != 0) {
        ((void (*)(int))**(void ***)pHandleObject)(1);
    }
}

// FUNCTION: LEMBALL 0x0045F820
void *ConstructDualHandleEffStream(void *pStream, int fConstructChannelState) {
    NETWORK_DualHandleEffStream *pDualStream;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pDualStream = (NETWORK_DualHandleEffStream *)pStream;
    if (fConstructChannelState != 0) {
        pDualStream->m_pChannelStateConstructionOffsets44 = (void **)g_NETWORK_DualHandleEffStreamChannelStateConstructionOffsets;
        ((NETWORK_EffStreamChannelState *)pDualStream->m_abChannelState58)->ConstructEffStreamChannelState();
    }

    pDualStream->ConstructEffStreamBase();
    pDualStream->m_pVtable = (void **)g_NETWORK_DualHandleEffStreamVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pDualStream->m_pChannelStateConstructionOffsets44;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pDualStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_DualHandleEffStreamPrimaryThunkVtable;
    pDualStream->m_pPrimaryHandleArray48 = 0;
    pDualStream->m_pPrimaryHandleObject4c = 0;
    pDualStream->m_pSecondaryHandleArray50 = 0;
    pDualStream->m_pSecondaryHandleObject54 = 0;
    pDualStream->m_nState2c = 0;

    if (g_pEffTransportPeerAddressState == 0) {
        g_pEffTransportPeerAddressState = CreateActiveNetworkRuntimeServiceObject();
    }

    return pStream;
}

// FUNCTION: LEMBALL 0x0045F8A0
void NETWORK_DualHandleEffStream::DestroyDualHandleEffStream(void) {
    NETWORK_DualHandleEffStream *pDualStream;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pDualStream = this;
    pDualStream->m_pVtable = (void **)g_NETWORK_DualHandleEffStreamVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pDualStream->m_pChannelStateConstructionOffsets44;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pDualStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_DualHandleEffStreamPrimaryThunkVtable;
    ReleaseEffStreamPrimaryHandleGroup();
    ReleaseEffStreamSecondaryHandleGroup();
    DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x0045FDF0
void NETWORK_TimedEffStream::ReleaseTimedEffStreamPrimaryHandle(void) {
    void *pHandleArray;

    pHandleArray = m_pPrimaryHandleArray48;
    if (pHandleArray != 0) {
        DestroyVSMemObjectPointerArray(pHandleArray);
        FreeVSMemBlock(pHandleArray);
    }
}

// THUNK: LEMBALL 0x0045FE50
void NETWORK_TimedEffStream::ReleaseTimedEffStreamPrimaryHandleThunk(
    int nUnused0, int nUnused1, int nUnused2) {
    (void)nUnused0;
    (void)nUnused1;
    (void)nUnused2;
    ReleaseTimedEffStreamPrimaryHandle();
}

// FUNCTION: LEMBALL 0x0045FE10
void NETWORK_TimedEffStream::ReleaseTimedEffStreamSecondaryHandles(void) {
    void *pHandleArray;

    pHandleArray = m_pSecondaryHandleArray4c;
    if (pHandleArray != 0) {
        DestroyVSMemObjectPointerArray(pHandleArray);
        FreeVSMemBlock(pHandleArray);
    }

    pHandleArray = m_pTertiaryHandleArray50;
    if (pHandleArray != 0) {
        DestroyVSMemObjectPointerArray(pHandleArray);
        FreeVSMemBlock(pHandleArray);
    }
}

// FUNCTION: LEMBALL 0x0045FCE0
NETWORK_TimedEffStream *NETWORK_TimedEffStream::ConstructTimedEffStream(int fConstructChannelState) {
    DWORD dwNow;
    int *pHandleArray;
    NETWORK_TimedEffStream *pTimedStream;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pTimedStream = this;
    if (fConstructChannelState != 0) {
        pTimedStream->m_pChannelStateConstructionOffsets44 = (void **)g_NETWORK_TimedEffStreamChannelStateConstructionOffsets;
        ((NETWORK_EffStreamChannelState *)pTimedStream->m_abChannelState78)->ConstructEffStreamChannelState();
    }

    ConstructEffStreamBase();
    pTimedStream->m_pVtable = (void **)g_NETWORK_TimedEffStreamVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pTimedStream->m_pChannelStateConstructionOffsets44;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pTimedStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TimedEffStreamPrimaryThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x34;

    dwNow = timeGetTime();
    pTimedStream->m_dwLastTick3c = dwNow - 1000;

    pHandleArray = (int *)AllocateVSMemBlock(0x10);
    pTimedStream->m_pTagBuffer2c = pHandleArray;
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
void NETWORK_TimedEffStream::DestroyTimedEffStream(void) {
    NETWORK_TimedEffStream *pTimedStream;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pTimedStream = this;
    pTimedStream->m_pVtable = (void **)g_NETWORK_TimedEffStreamVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pTimedStream->m_pChannelStateConstructionOffsets44;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pTimedStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
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
void NETWORK_TimedEffStream::InvokeTimedEffStreamServiceCallback(void *pArgument) {
    ((NETWORK_RuntimeService *)m_pRuntimeService70)->Invoke(pArgument);
}

// FUNCTION: LEMBALL 0x004604E0
void NETWORK_RuntimeChannelStackReleaseFront::Release(void) {
    NETWORK_RuntimeChannelStack *pChannelStack;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    void *pRuntimeService;
    void *pPeerAddressState;
    void **ppScratchBuffer;

    pChannelStack = (NETWORK_RuntimeChannelStack *)((char *)this - 0x24);
    pChannelStack->m_pVtable = (void **)g_NETWORK_RuntimeChannelStackVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pChannelStack->m_pOuterConstructionOffsets04;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pChannelStack + 4 + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)((char *)pChannelStack + 4 + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)((char *)pChannelStack + 4 + pOffsets->m_nTertiaryOffset - 4);
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
    pPeerAddressState = g_pEffTransportPeerAddressState;
    if (pPeerAddressState != 0) {
        FreeVSMemBlock(pPeerAddressState);
        g_pEffTransportPeerAddressState = 0;
    }
    ppScratchBuffer = &g_pEffTransportScratchBuffer;
    if (*ppScratchBuffer != 0) {
        FreeVSMemBlock(*ppScratchBuffer);
        *ppScratchBuffer = 0;
    }
}

// FUNCTION: LEMBALL 0x00460350
void *NETWORK_RuntimeChannelStack::ConstructEffTransportRuntimeChannelStack(int fConstructEmbeddedObjects) {
    int i;
    NETWORK_RuntimeChannelStack *pStack;
    NETWORK_TimedEffStream *pTimedStream;
    NETWORK_DualHandleEffStream *pDualStream;
    NETWORK_ConstructionAdjustorVtable *pEmbeddedOffsets;
    NETWORK_ConstructionAdjustorVtable *pOuterOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    char *pbObjectBase;

    pStack = this;
    pbObjectBase = (char *)this;
    if (fConstructEmbeddedObjects != 0) {
        pStack->m_pOuterConstructionOffsets04 = (void **)g_NETWORK_RuntimeChannelStackOuterConstructionOffsets;
        pTimedStream = (NETWORK_TimedEffStream *)pStack->m_abTimedStream54;
        pDualStream = (NETWORK_DualHandleEffStream *)pStack->m_abDualStreamcc;
        pTimedStream->m_pChannelStateConstructionOffsets44 = (void **)g_NETWORK_RuntimeChannelStackDualStreamConstructionOffsets;
        pDualStream->m_pChannelStateConstructionOffsets44 = (void **)g_NETWORK_RuntimeChannelStackTimedStreamConstructionOffsets;
        pStack->m_pEmbeddedConstructionOffsets124 = (void **)g_NETWORK_RuntimeChannelStackChannelStateConstructionOffsets;

        ((NETWORK_EffStreamChannelState *)pStack->m_abChannelState24)->ConstructEffStreamChannelState();
        pTimedStream->ConstructTimedEffStream(0);
        ConstructDualHandleEffStream(pDualStream, 0);

        pEmbeddedOffsets = (NETWORK_ConstructionAdjustorVtable *)pStack->m_pEmbeddedConstructionOffsets124;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x124 + pEmbeddedOffsets->m_nPrimaryOffset - 4);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x124 + pEmbeddedOffsets->m_nSecondaryOffset - 4);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x124 + pEmbeddedOffsets->m_nTertiaryOffset - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackChannelStateVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pPrimaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nPrimaryOffset - 8;
        pSecondaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nSecondaryOffset - 0x38;
        pTertiaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nTertiaryOffset - 0xb0;
    }

    pStack->m_pVtable = (void **)g_NETWORK_RuntimeChannelStackVtable;
    pOuterOffsets = (NETWORK_ConstructionAdjustorVtable *)pStack->m_pOuterConstructionOffsets04;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 4 + pOuterOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 4 + pOuterOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 4 + pOuterOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
    pSecondaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackTimedThunk;
    pTertiaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackDualThunk;
    pPrimaryThunk->m_nThisDelta = pOuterOffsets->m_nPrimaryOffset - 0x20;
    pSecondaryThunk->m_nThisDelta = pOuterOffsets->m_nSecondaryOffset - 0x50;
    pTertiaryThunk->m_nThisDelta = pOuterOffsets->m_nTertiaryOffset - 200;

    pStack->m_nReserved14 = 0;
    pStack->m_nReserved08 = 0;
    pStack->m_nReserved0c = 0;
    pStack->m_pRuntimeSideBuffer1c = 0;

    g_pEffTransportRuntimeService =
        ((NETWORK_RuntimeWindowInterface *)g_pActiveNetworkRuntimeWindow)->CreateServiceObject();
    pStack->m_pbRuntimeFlags10 = (unsigned char *)AllocateVSMemBlock(0x200);
    i = 0;
    do {
        ++i;
        pStack->m_pbRuntimeFlags10[i - 1] = 0;
    } while (i < 0x200);

    return this;
}

// FUNCTION: LEMBALL 0x00462BC0
void *DeleteEffTransportRuntimeChannelStackWrapper(void *pChannelState, unsigned char fDelete) {
    NETWORK_RuntimeChannelStackWrapperView *pView;
    char *pbAllocationBase;

    pView = (NETWORK_RuntimeChannelStackWrapperView *)pChannelState;
    pbAllocationBase = (char *)pChannelState - 0x24;
    ((NETWORK_RuntimeChannelStackReleaseFront *)pChannelState)->Release();
    ((NETWORK_DualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((NETWORK_TimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    ((NETWORK_EffStreamChannelState *)pView->m_abChannelState00)->DestroyEffStreamChannelState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00462830
void *DeleteEffStreamBaseWrapper00462830(void *pObject, BYTE fDelete) {
    ((NETWORK_EffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462850
void *DeleteEffStreamBaseWrapper00462850(void *pObject, BYTE fDelete) {
    ((NETWORK_EffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462870
void *DeleteEffStreamBaseWrapper00462870(void *pObject, BYTE fDelete) {
    ((NETWORK_EffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462890
void *DeleteEffStreamBaseWrapper00462890(void *pObject, BYTE fDelete) {
    ((NETWORK_EffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004628B0
void *DeleteEffStreamBaseWrapper004628B0(void *pObject, BYTE fDelete) {
    ((NETWORK_EffStreamBase *)pObject)->DestroyEffStreamBase();
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
    ((NETWORK_EffStreamChannelState *)pObject)->DestroyEffStreamChannelState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462900
void *DeleteEffStreamBaseWrapper00462900(void *pObject, BYTE fDelete) {
    ((NETWORK_EffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462940
void *DeleteDualEffStreamWithChannelStateWrapper(void *pObject, BYTE fDelete) {
    ((NETWORK_DualHandleEffStream *)pObject)->DestroyDualHandleEffStream();
    ((NETWORK_EffStreamChannelState *)((char *)pObject + 0x58))->DestroyEffStreamChannelState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462990
void *DeleteTimedEffStreamWithChannelStateWrapper(void *pObject, BYTE fDelete) {
    ((NETWORK_TimedEffStream *)pObject)->DestroyTimedEffStream();
    ((NETWORK_EffStreamChannelState *)((char *)pObject + 0x78))->DestroyEffStreamChannelState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462B40
void WriteEffStreamTwoU16Fields(void *pObject) {
    ((NETWORK_EffStreamBase *)pObject)->WriteEffStreamU16BE(*(unsigned short *)((char *)pObject + 0x2c));
    ((NETWORK_EffStreamBase *)pObject)->WriteEffStreamU16BE(*(unsigned short *)((char *)pObject + 0x2e));
}

// FUNCTION: LEMBALL 0x00462B80
void *DeleteEffStreamBase(void *pObject, BYTE fDelete) {
    ((NETWORK_EffStreamBase *)pObject)->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462CB0
void *DeleteEffChannelStreamStackWrapper(void *pChannelState, BYTE fDelete) {
    NETWORK_RuntimeChannelStackWrapperView *pView;

    pView = (NETWORK_RuntimeChannelStackWrapperView *)pChannelState;
    ((NETWORK_DualHandleEffStream *)((char *)pChannelState + 0xa8))->DestroyDualHandleEffStream();
    ((NETWORK_TimedEffStream *)((char *)pChannelState + 0x30))->DestroyTimedEffStream();
    ((NETWORK_EffStreamChannelState *)pChannelState)->DestroyEffStreamChannelState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock((char *)pChannelState - 8);
    }
    return (char *)pView - 8;
}
// FUNCTION: LEMBALL 0x00462ED0
int WINAPI InitializeNonZrleVariantRenderEntry(int nValue) {
    return nValue;
}
