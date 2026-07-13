#include "../game/game_app.h"
#include "../engine/memory_arena.h"
#include "../network/stream.h"
#include "../network/safe_vtable.h"
#include "eff_streams.h"

extern int ReturnTrueVtableCallbackThunk(void);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void NoopVtableCallbackThunk(void);
extern void ReadNetworkLevelChunkDeltaStream(void *pObject);

// FUNCTION: LEMBALL 0x004524F0
void LEMBALL_FASTCALL WriteNetworkLobbyU32PayloadStream(void *pObject) {
    NETWORK_EffStreamBase *pStream;

    pStream = (NETWORK_EffStreamBase *)pObject;
    pStream->WriteEffStreamU32BE(*(unsigned int *)((char *)pObject + 0x2c));
}

// FUNCTION: LEMBALL 0x00452500
void LEMBALL_FASTCALL ReadNetworkLobbyU32PayloadStream(void *pObject) {
    NETWORK_EffStreamBase *pStream;

    pStream = (NETWORK_EffStreamBase *)pObject;
    *(unsigned int *)((char *)pObject + 0x2c) = pStream->ReadEffStreamU32BEValue();
}

// FUNCTION: LEMBALL 0x00452E50
void LEMBALL_FASTCALL ReadNetworkLobbyPeerEntryStream(void *pObject) {
    NETWORK_EffStreamBase *pStream;

    pStream = (NETWORK_EffStreamBase *)pObject;
    *(unsigned int *)((char *)pObject + 0x2c) = pStream->ReadEffStreamU32BEValue();
}

// FUNCTION: LEMBALL 0x00452E60
void *LEMBALL_FASTCALL DeleteNetworkLobbySelectedPeerStatusStream(void *pObject, void *pUnused, BYTE fDelete) {
    NETWORK_EffStreamBase *pStream;

    (void)pUnused;
    pStream = (NETWORK_EffStreamBase *)pObject;
    pStream->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

static void *g_EFF_NetworkLobbySelectedPeerStatusVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReadNetworkLobbyPeerEntryStream,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteNetworkLobbyU32PayloadStream,
    (void *)DeleteNetworkLobbySelectedPeerStatusStream,
};
static void *g_EFF_NetworkLobbyPeerEntryVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReadNetworkLobbyPeerEntryStream,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)DeleteNetworkLobbySelectedPeerStatusStream,
};

/* 004985F8 slot +20 is read_network_level_chunk_delta_stream.  Other
 * inherited slots remain compiler-owned callable placeholders until their
 * targets are recovered. */
struct GAME_NetworkLobbyVsnetRuntimeVtableModel {
    virtual void Reserved00(void) {}
    virtual void Reserved04(void) {}
    virtual void Reserved08(void) {}
    virtual void Reserved0c(void) {}
    virtual void Reserved10(void) {}
    virtual void Reserved14(void) {}
    virtual void Reserved18(void) {}
    virtual void Reserved1c(void) {}
    virtual void ServiceRuntime(void) {
        ReadNetworkLevelChunkDeltaStream(this);
    }
    virtual void Reserved24(void) {}
    virtual void Reserved28(void) {}
    virtual void Reserved2c(void) {}
};

static GAME_NetworkLobbyVsnetRuntimeVtableModel g_EFF_NetworkLobbyVsnetRuntimeVtableModel;
static void *g_EFF_NetworkLobbyVsnetRuntimeVtable =
    *(void ***)&g_EFF_NetworkLobbyVsnetRuntimeVtableModel;
static void *g_EFF_PayloadSize8Vtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)DeleteNetworkLobbySelectedPeerStatusStream,
};
static void *g_EFF_NetworkLobbyU32PayloadVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)DeleteNetworkLobbySelectedPeerStatusStream,
};
static void *g_EFF_NetworkLobbyPeerClearCloseVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)DeleteNetworkLobbySelectedPeerStatusStream,
};
static void *g_EFF_NetworkLobbyPeerDirtyConfirmVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)DeleteNetworkLobbySelectedPeerStatusStream,
};
static const char g_EFF_DefaultNetworkPath[] = "t:\\network";
// GLOBAL: LEMBALL 0x004a1e1c
void *g_pEffTransportDispatchQueue = 0;
// GLOBAL: LEMBALL 0x004A1E20
void *g_pEffTransportSecondaryDispatchQueue = 0;

struct GAME_EffStreamVtable {
    void *m_pReserved00;
    int (*m_pLoadFromMemoryHeader)(GAME_EffStream *);
    void (*m_pLoadFromMemoryBody)(GAME_EffStream *);
    void (*m_pBeginWriteHeader)(GAME_EffStream *);
    void (*m_pBeginWriteBody)(GAME_EffStream *);
    void (*m_pDelete)(int);
};

struct GAME_NetworkLobbyPeerEntryStream {
    GAME_EffStream m_Base;
    unsigned char m_fAcceptedPeer;
    char m_abUnknown2d[0x1f];
    int m_nPeerEntryState;
};

struct GAME_DeleteObject {
    GAME_EffStreamVtable *m_pVtable;
};

struct GAME_RecordSlotTable {
    int m_cSlots;
    int m_nReserved04;
    void **m_apSlots08;

    void *GetShiftedRecordSlotByPacketId(int nPacketId);
};

struct GAME_EffTransportPeerDescriptor {
    void *m_pReserved00;
    int m_nEndpointStateSlotOffset;
    int m_nPayloadSenderSlotOffset;
    int m_nReserved0C;
    int m_nHandleGroupAdjustorSlotOffset;
};

struct GAME_EffTransportHandleCallbackVtable {
    int (*m_pBeginReset)(void);
    void *m_pReserved04;
    void (*m_pFinishReset)(void);
    void (*m_pNotifyClosed)(int);
};

struct GAME_EffTransportHandleCallback {
    GAME_EffTransportHandleCallbackVtable *m_pVtable;
};

struct GAME_EffTransportHandleGroup {
    GAME_EffTransportHandleCallback *m_pCallback;
    int m_nReserved04;
    int m_nReserved08;
    int m_fPrimaryActive;
    int m_fSecondaryActive;

    void Reset(void);
};

struct GAME_EffTransportEndpointState {
    int m_nReserved00;
    GAME_EffTransportHandleGroup m_HandleGroup;
    int m_fPrimaryHandlePresent;
    int m_fSecondaryHandlePresent;
    char m_abUnknown18[0x0c];
    short m_nAssignedPort;
};

struct GAME_EffTransportEndpointSlot {
    char m_abUnknown00[4];
    GAME_EffTransportEndpointState m_State;
};

struct GAME_EffTransportHandleOwner {
    char m_abUnknown00[0x48];
    void *m_pShiftedRecordSlotTable;
};

struct GAME_EffTransportHandleOwnerSlot {
    char m_abUnknown00[4];
    GAME_EffTransportHandleOwner m_HandleOwner;
};

struct GAME_EffTransportPeer {
    void **m_pVtable;
    GAME_EffTransportPeerDescriptor *m_pDescriptor;
    char *m_pszName;
    char *m_pszAddress;
    GAME_EffTransportPeer *m_pNext;
    GAME_EffTransportPeer *m_pPrevious;
    int m_cNewPortRequests;
    int m_nReserved1C;
    int m_fClosed;
    int m_fConnectComplete;
    DWORD m_dwConnectStartTick;
    int m_pConnectionState;
};

struct GAME_NetworkLobbyVsnetRuntime {
    void *m_pVtable;
    char m_abUnknown04[0x0c];
    GAME_EffStream *m_pPeerEntryWriteStream;
    GAME_EffStream *m_pPeerEntryTable;
    GAME_EffStream *m_pSelectedPeerStatusStream;
    GAME_EffStream *m_pPeerClearCloseStream;
    char m_abUnknown20[0x28];
    int m_nDesiredSelectedPeerStatus;
    int m_nObservedSelectedPeerStatus;
    DWORD m_dwSelectedPeerStatusTick;
    int m_nReserved54;
    int m_nReserved58;
    int m_fSelectedPeerDisconnected;
    int m_fFileBasedThreadStarted;
    int m_fTcpipThreadStarted;
    int m_fSelectionNeedsReset;
};

struct GAME_EffTransportRuntimeWindow {
    void **m_pVtable;
    int m_nQueueMagic04;
    void *m_pReserved08;
    void *m_pReserved0c;
    int m_nRuntimeKey10;
    int m_fShutdownRequested14;
    int m_fRuntimeActive;
    char m_abUnknown1C[0x20];
    void *m_pPrimaryDispatchClient;
    void *m_pReserved40;
    void *m_pSecondaryDispatchClient;
    int m_nPrimaryHandleCount;
    int m_nPrimaryHandleStride;
    int m_nPrimaryHandleFlags;
    int m_nSecondaryHandleCount;
    int m_nSecondaryHandleStride;

    void RegisterEffTransportEventClient(void *pClient);
    void UnregisterEffTransportEventClient(void);
};

struct GAME_EffTransportRuntimeWindowVtable {
    void *m_pReserved00[8];
    void (*m_pServiceRuntime)(void);
};

struct GAME_PeerEntryStreamTable {
    int m_cEntries;
    GAME_EffStream m_aEntries[10];
};

struct GAME_EffStreamDeleteVtable {
    void *m_pReserved00[5];
    void (*m_pDelete)(int);
};

struct GAME_ShiftedRecordSlot {
    int m_nReserved00;
    int m_pSerializedPayload;
    void **m_pLockVtable;
    char m_abUnknown0c[0x18];
    int m_fHasPayload;
};

struct GAME_ShiftedRecordLockVtable {
    void (*m_pLock)(void);
    void (*m_pUnlock)(void);
};

struct GAME_RenderQueueServiceVtable {
    void *m_pReserved00[2];
    void (*m_pQueueWriteEvent)(void *pEvent);
};

struct GAME_EffStreamLoadInterface {
    virtual void Reserved00(void) = 0;
    virtual int LoadFromMemoryHeader(void) = 0;
    virtual void LoadFromMemoryBody(void) = 0;
};

struct GAME_RenderQueueServiceInterface {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void QueueWriteEvent(void *pEvent) = 0;
};

struct GAME_EffTransportRuntimeServiceInterface {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0C(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved1C(void) = 0;
    virtual void ServiceRuntime(void) = 0;
};

struct GAME_EffQueuedWriteEvent {
    unsigned short m_nEventType;
    unsigned short m_aPadding[3];
    int m_fWrite;
    void *m_pStream;
    int m_nPayload;
};

struct GAME_NonZrleVariantRenderEntryInitializeVtable {
    char m_abUnknown00[0x18];
    void (*m_pDelete)(int);
};

extern "C" DWORD WINAPI timeGetTime(void);

extern int g_nSelectedNetworkLobbyPeerId;
extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pNonZrleVariantRenderEntryInitializeVtable[2];
struct NETWORK_EffTransportPeer {
    void Close(void);
};
struct NETWORK_RuntimeStartView {
    int StartEffTransportRuntimeAndWaitReady(int nRuntimeKey, int cbMaxPacket);
};
extern void *LEMBALL_FASTCALL InitializeRenderQueueNodeBase(void *pRenderQueueNode);
extern int StartFileBasedNetworkMessageThread(void);
extern int StartTcpipNetworkMessageThread(void);
extern void WINAPI ConfigureFileBasedNetworkPathsWrapper(char *pszBasePath, char *pszNetworkPath);
extern int StopFileBasedNetworkMessageThread(void);
extern int StopTcpipNetworkMessageThread(void);
void *ConstructNetworkLobbyPeerClearCloseStream(void *pObject);

// FUNCTION: LEMBALL 0x004615F0
void *GAME_RecordSlotTable::GetShiftedRecordSlotByPacketId(int nPacketId) {
    if (nPacketId > 2) {
        nPacketId -= 3;
    }
    return m_apSlots08[nPacketId];
}

// FUNCTION: LEMBALL 0x0045F280
int GAME_EffStream::LoadEffStreamFromMemory(int nSourceBuffer) {
    GAME_EffStreamLoadInterface *pStream;

    pStream = (GAME_EffStreamLoadInterface *)this;
    m_pvReadCursor = nSourceBuffer;
    if (pStream->LoadFromMemoryHeader() != 0) {
        pStream->LoadFromMemoryBody();
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00462550
void SetEffTransportPrimaryHandleParams(void *pRuntimeWindow, int nPrimaryHandleCount, int nPrimaryHandleStride, int nFlags) {
    GAME_EffTransportRuntimeWindow *pWindow;

    pWindow = (GAME_EffTransportRuntimeWindow *)pRuntimeWindow;
    pWindow->m_nPrimaryHandleCount = nPrimaryHandleCount;
    pWindow->m_nPrimaryHandleStride = nPrimaryHandleStride;
    pWindow->m_nPrimaryHandleFlags = nFlags;
}

// FUNCTION: LEMBALL 0x00462570
void SetEffTransportSecondaryHandleParams(void *pRuntimeWindow, int nSecondaryHandleCount, int nSecondaryHandleStride) {
    GAME_EffTransportRuntimeWindow *pWindow;

    pWindow = (GAME_EffTransportRuntimeWindow *)pRuntimeWindow;
    pWindow->m_nSecondaryHandleCount = nSecondaryHandleCount;
    pWindow->m_nSecondaryHandleStride = nSecondaryHandleStride;
}

// FUNCTION: LEMBALL 0x004523B0
void *ConstructNetworkLobbySelectedPeerStatusStream(void *pObject) {
    GAME_EffStream *pStream;

    pStream = (GAME_EffStream *)pObject;
    pStream->m_pVtable =
        (GAME_EffStreamVtable *)g_GAME_EffStreamConstructionVtable;
    pStream->m_nEventCode = 4;
    pStream->ResetStateFields();
    pStream->m_pVtable = (GAME_EffStreamVtable *)g_EFF_NetworkLobbySelectedPeerStatusVtable;
    pStream->m_cbSerializedLength += 4;
    return pObject;
}

// FUNCTION: LEMBALL 0x004523E0
void *ConstructNetworkLobbyPeerEntryStream(void *pObject) {
    GAME_NetworkLobbyPeerEntryStream *pStream;

    pStream = (GAME_NetworkLobbyPeerEntryStream *)pObject;
    pStream->m_Base.m_pVtable =
        (GAME_EffStreamVtable *)g_GAME_EffStreamConstructionVtable;
    pStream->m_Base.m_nEventCode = 5;
    pStream->m_Base.ResetStateFields();
    pStream->m_Base.m_pVtable = (GAME_EffStreamVtable *)g_EFF_NetworkLobbyPeerEntryVtable;
    pStream->m_fAcceptedPeer = 0;
    pStream->m_nPeerEntryState = 0;
    pStream->m_Base.m_cbSerializedLength += 0x1e;
    pStream->m_Base.m_fHasPayload = 1;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452550
void *ConstructNetworkLobbyVsnetRuntime(void *pObject, char *pszFileBasedNetworkRoot) {
    GAME_NetworkLobbyVsnetRuntime *pRuntime;
    GAME_PeerEntryStreamTable *pPeerEntryTable;
    GAME_EffStream *pPeerEntryStream;
    int i;
    int fStarted;

    pRuntime = (GAME_NetworkLobbyVsnetRuntime *)pObject;
    fStarted = 0;

    InitializeRenderQueueNodeBase(pObject);
    pRuntime->m_pVtable = g_EFF_NetworkLobbyVsnetRuntimeVtable;
    pRuntime->m_fFileBasedThreadStarted = 0;
    pRuntime->m_fTcpipThreadStarted = 0;
    pRuntime->m_nDesiredSelectedPeerStatus = 0;
    pRuntime->m_nReserved54 = 0;

    pPeerEntryTable = (GAME_PeerEntryStreamTable *)AllocateVSMemBlock(0x324);
    if (pPeerEntryTable == 0) {
        pRuntime->m_pPeerEntryTable = 0;
    } else {
        pPeerEntryTable->m_cEntries = 10;
        pPeerEntryStream = pPeerEntryTable->m_aEntries;
        for (i = 0; i < 10; ++i) {
            ConstructNetworkLobbyPeerEntryStream(pPeerEntryStream);
            ++pPeerEntryStream;
        }
        pRuntime->m_pPeerEntryTable = pPeerEntryTable->m_aEntries;
    }

    pPeerEntryStream = (GAME_EffStream *)AllocateVSMemBlock(0x30);
    if (pPeerEntryStream == 0) {
        pRuntime->m_pSelectedPeerStatusStream = 0;
    } else {
        pRuntime->m_pSelectedPeerStatusStream =
            (GAME_EffStream *)ConstructNetworkLobbySelectedPeerStatusStream(pPeerEntryStream);
    }

    pRuntime->m_dwSelectedPeerStatusTick = (int)(timeGetTime() - 2000);
    pRuntime->m_nObservedSelectedPeerStatus = 0;
    pRuntime->m_nDesiredSelectedPeerStatus = 0;

    pPeerEntryStream = (GAME_EffStream *)AllocateVSMemBlock(0x50);
    if (pPeerEntryStream == 0) {
        pRuntime->m_pPeerEntryWriteStream = 0;
    } else {
        pRuntime->m_pPeerEntryWriteStream = (GAME_EffStream *)ConstructNetworkLobbyPeerEntryStream(pPeerEntryStream);
    }

    pPeerEntryStream = (GAME_EffStream *)AllocateVSMemBlock(0x30);
    if (pPeerEntryStream == 0) {
        pRuntime->m_pPeerClearCloseStream = 0;
    } else {
        pRuntime->m_pPeerClearCloseStream = (GAME_EffStream *)ConstructNetworkLobbyPeerClearCloseStream(pPeerEntryStream);
    }

    for (i = 0; i < 10; ++i) {
        ((int *)pRuntime->m_abUnknown20)[i] = 0;
    }

    if (pszFileBasedNetworkRoot != 0) {
        fStarted = StartFileBasedNetworkMessageThread();
        if (fStarted != 0) {
            pRuntime->m_fFileBasedThreadStarted = 1;
        }
    } else {
        fStarted = StartTcpipNetworkMessageThread();
        pRuntime->m_fTcpipThreadStarted = 1;
    }

    if (fStarted != 0) {
        if (pszFileBasedNetworkRoot != 0) {
            ConfigureFileBasedNetworkPathsWrapper(pszFileBasedNetworkRoot, (char *)g_EFF_DefaultNetworkPath);
        }

        if (((NETWORK_RuntimeStartView *)g_pActiveNetworkRuntimeWindow)
                ->StartEffTransportRuntimeAndWaitReady(0, 0x400) != 0) {
            SetEffTransportSecondaryHandleParams(g_pActiveNetworkRuntimeWindow, 100, 0x10);
            SetEffTransportPrimaryHandleParams(g_pActiveNetworkRuntimeWindow, 4, 4, 0);
            pRuntime->m_fSelectedPeerDisconnected = 0;
            g_nSelectedNetworkLobbyPeerId = 0;
            pRuntime->m_fSelectionNeedsReset = 0;
        }
    }

    return pObject;
}

// FUNCTION: LEMBALL 0x004526E0
void DestroyNetworkLobbyVsnetRuntime(void *pObject) {
    GAME_NetworkLobbyVsnetRuntime *pRuntime;
    void *pOwnedObject;

    pRuntime = (GAME_NetworkLobbyVsnetRuntime *)pObject;
    pRuntime->m_pVtable = g_EFF_NetworkLobbyVsnetRuntimeVtable;

    if (pRuntime->m_fFileBasedThreadStarted != 0) {
        StopFileBasedNetworkMessageThread();
    }
    if (pRuntime->m_fTcpipThreadStarted != 0) {
        StopTcpipNetworkMessageThread();
    }

    pOwnedObject = pRuntime->m_pPeerClearCloseStream;
    if (pOwnedObject != 0) {
        ((GAME_EffStreamDeleteVtable *)*(void **)pOwnedObject)->m_pDelete(1);
    }

    pOwnedObject = pRuntime->m_pSelectedPeerStatusStream;
    if (pOwnedObject != 0) {
        ((GAME_EffStreamDeleteVtable *)*(void **)pOwnedObject)->m_pDelete(1);
    }

    pOwnedObject = pRuntime->m_pPeerEntryWriteStream;
    if (pOwnedObject != 0) {
        ((GAME_EffStreamDeleteVtable *)*(void **)pOwnedObject)->m_pDelete(1);
    }

    pOwnedObject = pRuntime->m_pPeerEntryTable;
    if (pOwnedObject != 0) {
        ((GAME_EffStreamDeleteVtable *)*(void **)pOwnedObject)->m_pDelete(3);
    }

    pRuntime->m_pVtable = g_pNonZrleVariantRenderEntryInitializeVtable;
}

// FUNCTION: LEMBALL 0x0045F1E0
void GAME_EffStream::BeginEffStreamWriteSession(void) {
    int cbPayload;

    cbPayload = m_cWriteSessions;
    m_cWriteSessions = cbPayload + 1;
    if (cbPayload == 0) {
        if (m_fOwnsBuffer == 0) {
            cbPayload = (int)(unsigned long)AllocateVSMemBlock((unsigned int)(m_cbSerializedLength + 0x10));
            m_fOwnsBuffer = 1;
            m_pvOwnedBuffer = cbPayload;
            m_pvWriteCursor = cbPayload + 0x10;
            m_pvBufferEnd = m_cbSerializedLength + cbPayload + 0x10;
        } else {
            m_pvWriteCursor = m_pvOwnedBuffer + 0x10;
        }
        m_pVtable->m_pBeginWriteHeader(this);
        m_pVtable->m_pBeginWriteBody(this);
    }
}

// FUNCTION: LEMBALL 0x0045F240
void GAME_EffStream::EndEffStreamWriteSession(void) {
    --m_cWriteSessions;
}

// FUNCTION: LEMBALL 0x0045FC30
int HasShiftedRecordSlotPayloadForStream(void *pTransportPeer, void *pStream) {
    GAME_EffTransportHandleOwner *pHandleOwner;
    GAME_ShiftedRecordSlot *pSlot;

    pHandleOwner = (GAME_EffTransportHandleOwner *)pTransportPeer;
    pSlot = (GAME_ShiftedRecordSlot *)
        ((GAME_RecordSlotTable *)pHandleOwner->m_pShiftedRecordSlotTable)
            ->GetShiftedRecordSlotByPacketId(((GAME_EffStream *)pStream)->m_nEventCode);
    return pSlot->m_fHasPayload;
}

// FUNCTION: LEMBALL 0x0045FC50
void LoadStreamFromShiftedRecordSlotAndClear(void *pTransportPeer, void *pStream) {
    GAME_EffTransportHandleOwner *pHandleOwner;
    GAME_ShiftedRecordSlot *pSlot;
    GAME_ShiftedRecordLockVtable *pLockVtable;

    pHandleOwner = (GAME_EffTransportHandleOwner *)pTransportPeer;
    pSlot = (GAME_ShiftedRecordSlot *)
        ((GAME_RecordSlotTable *)pHandleOwner->m_pShiftedRecordSlotTable)
            ->GetShiftedRecordSlotByPacketId(((GAME_EffStream *)pStream)->m_nEventCode);
    pLockVtable = (GAME_ShiftedRecordLockVtable *)pSlot->m_pLockVtable;
    pLockVtable->m_pLock();
    ((GAME_EffStream *)pStream)->LoadEffStreamFromMemory(pSlot->m_pSerializedPayload + 0x10);
    pLockVtable->m_pUnlock();
    pSlot->m_fHasPayload = 0;
}

// FUNCTION: LEMBALL 0x0045F720
void GAME_EffTransportHandleGroup::Reset(void) {
    int nResult;

    if (m_fPrimaryActive != 0) {
        m_fSecondaryActive = 0;
        m_fPrimaryActive = 0;
        nResult = m_pCallback->m_pVtable->m_pBeginReset();
        if (nResult == -1) {
            m_pCallback->m_pVtable->m_pFinishReset();
        }
    }
}

// FUNCTION: LEMBALL 0x0045F2B0
void GAME_EffStream::QueueEffStreamWriteEvent(int nPayload) {
    GAME_EffQueuedWriteEvent event;

    if (nPayload != 0) {
        event.m_nPayload = nPayload;
        event.m_fWrite = 1;
        event.m_nEventType = 0xb;
        event.m_pStream = this;
        BeginEffStreamWriteSession();
        m_fWritePending = 1;
        ((GAME_RenderQueueServiceInterface *)g_pEffTransportDispatchQueue)
            ->QueueWriteEvent(&event);
        ((GAME_EffTransportRuntimeServiceInterface *)
             g_pActiveNetworkRuntimeWindow)
            ->ServiceRuntime();
    }
}

// FUNCTION: LEMBALL 0x0045EE80
void GAME_EffStream::ResetStateFields(void) {
    m_fHasPayload = 0;
    m_fWritePending = 0;
    m_fOwnsBuffer = 0;
    m_pvOwnedBuffer = 0;
    m_cbSerializedLength = 0;
    m_cWriteSessions = 0;
}

// FUNCTION: LEMBALL 0x004453B0
void *ConstructEffStreamPayloadSize8(void *pObject) {
    GAME_EffStream *pStream;

    pStream = (GAME_EffStream *)pObject;
    pStream->m_pVtable =
        (GAME_EffStreamVtable *)g_GAME_EffStreamConstructionVtable;
    pStream->m_nEventCode = 8;
    pStream->ResetStateFields();
    pStream->m_pVtable = (GAME_EffStreamVtable *)g_EFF_PayloadSize8Vtable;
    pStream->m_cbSerializedLength += 8;
    pStream->m_fHasPayload = 1;
    return pObject;
}

// FUNCTION: LEMBALL 0x004524B0
void *ConstructNetworkLobbyU32PayloadStream(void *pObject, int nEventCode) {
    GAME_EffStream *pStream;

    pStream = (GAME_EffStream *)pObject;
    pStream->m_pVtable =
        (GAME_EffStreamVtable *)g_GAME_EffStreamConstructionVtable;
    pStream->m_nEventCode = nEventCode;
    pStream->ResetStateFields();
    pStream->m_pVtable = (GAME_EffStreamVtable *)g_EFF_NetworkLobbyU32PayloadVtable;
    pStream->m_cbSerializedLength += 4;
    pStream->m_fHasPayload = 1;
    pStream->m_nU32Payload = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452510
void *ConstructNetworkLobbyPeerClearCloseStream(void *pObject) {
    ConstructNetworkLobbyU32PayloadStream(pObject, 6);
    *(void **)pObject = g_EFF_NetworkLobbyPeerClearCloseVtable;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452530
void *ConstructNetworkLobbyPeerDirtyConfirmStream(void *pObject) {
    ConstructNetworkLobbyU32PayloadStream(pObject, 7);
    *(void **)pObject = g_EFF_NetworkLobbyPeerDirtyConfirmVtable;
    return pObject;
}

// FUNCTION: LEMBALL 0x00462590
void GAME_EffTransportRuntimeWindow::RegisterEffTransportEventClient(void *pClient) {
    m_pSecondaryDispatchClient = pClient;
    ((GAME_RenderDispatchQueue *)g_pEffTransportSecondaryDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(pClient, 0);
}

// FUNCTION: LEMBALL 0x004625B0
void GAME_EffTransportRuntimeWindow::UnregisterEffTransportEventClient(void) {
    void *pClient;

    pClient = m_pSecondaryDispatchClient;
    if (pClient != 0) {
        ((GAME_RenderDispatchQueue *)g_pEffTransportSecondaryDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(pClient, 0);
        m_pSecondaryDispatchClient = 0;
    }
}

// FUNCTION: LEMBALL 0x004527C0
void LEMBALL_FASTCALL UnregisterNetworkLobbyVsnetRuntimeFromTransport(void *pVsnetRuntime) {
    GAME_NetworkLobbyVsnetRuntime *pRuntime;
    GAME_EffTransportRuntimeWindow *pWindow;
    DWORD dwStartTime;

    pRuntime = (GAME_NetworkLobbyVsnetRuntime *)pVsnetRuntime;
    if (g_nSelectedNetworkLobbyPeerId != 0) {
        pRuntime->m_pPeerClearCloseStream->m_nU32Payload = 1;
        pRuntime->m_pPeerClearCloseStream
            ->QueueEffStreamWriteEvent(g_nSelectedNetworkLobbyPeerId);
        dwStartTime = timeGetTime();
        while (pRuntime->m_pPeerClearCloseStream->m_fHasPayload != 0) {
            if (timeGetTime() - dwStartTime >= 1000) {
                break;
            }
        }
        ((NETWORK_EffTransportPeer *)(unsigned long)g_nSelectedNetworkLobbyPeerId)->Close();
        g_nSelectedNetworkLobbyPeerId = 0;
    }

    pWindow = (GAME_EffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow;
    if (pWindow != 0 && pWindow->m_fRuntimeActive != 0) {
        ((GAME_RenderDispatchQueue *)g_pEffTransportSecondaryDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(pVsnetRuntime, 0x19);
        pWindow->m_pSecondaryDispatchClient = pVsnetRuntime;
        ((GAME_EffTransportRuntimeWindowVtable *)*(void **)g_pActiveNetworkRuntimeWindow)->m_pServiceRuntime();
    }
}

// FUNCTION: LEMBALL 0x00452AB0
void MarkNetworkLobbySelectedPeerDisconnected(void *pVsnetRuntime) {
    ((GAME_NetworkLobbyVsnetRuntime *)pVsnetRuntime)->m_fSelectionNeedsReset = 1;
}

// FUNCTION: LEMBALL 0x00452AC0
void LEMBALL_FASTCALL ServiceNetworkLobbySelectedPeerUpdates(void *pVsnetRuntime) {
    GAME_NetworkLobbyVsnetRuntime *pRuntime;
    GAME_EffTransportPeer *pSelectedPeer;
    GAME_EffTransportHandleOwner *pHandleOwner;
    DWORD dwNow;
    int nStreamValue;

    pRuntime = (GAME_NetworkLobbyVsnetRuntime *)pVsnetRuntime;
    if (pRuntime->m_fSelectionNeedsReset != 0) {
        g_nSelectedNetworkLobbyPeerId = 0;
        pRuntime->m_fSelectionNeedsReset = 0;
    }
    if (g_nSelectedNetworkLobbyPeerId != 0) {
        pSelectedPeer = (GAME_EffTransportPeer *)(unsigned long)g_nSelectedNetworkLobbyPeerId;
        pHandleOwner = &((GAME_EffTransportHandleOwnerSlot *)((char *)pSelectedPeer +
                                                              pSelectedPeer->m_pDescriptor->m_nHandleGroupAdjustorSlotOffset))
                            ->m_HandleOwner;
        if (HasShiftedRecordSlotPayloadForStream(
                pHandleOwner,
                pRuntime->m_pSelectedPeerStatusStream) != 0) {
            LoadStreamFromShiftedRecordSlotAndClear(
                pHandleOwner,
                pRuntime->m_pSelectedPeerStatusStream);
            nStreamValue = pRuntime->m_pSelectedPeerStatusStream->m_nU32Payload;
            if (pRuntime->m_nObservedSelectedPeerStatus != nStreamValue) {
                pRuntime->m_nObservedSelectedPeerStatus = nStreamValue;
                pRuntime->m_pSelectedPeerStatusStream->m_nU32Payload = pRuntime->m_nDesiredSelectedPeerStatus;
                pRuntime->m_pSelectedPeerStatusStream
                    ->QueueEffStreamWriteEvent(g_nSelectedNetworkLobbyPeerId);
            }
        }
        if (pRuntime->m_nObservedSelectedPeerStatus != pRuntime->m_nDesiredSelectedPeerStatus) {
            dwNow = timeGetTime();
            if (2000 < dwNow - (DWORD)pRuntime->m_dwSelectedPeerStatusTick) {
                pRuntime->m_pSelectedPeerStatusStream->m_nU32Payload = pRuntime->m_nDesiredSelectedPeerStatus;
                pRuntime->m_pSelectedPeerStatusStream
                    ->QueueEffStreamWriteEvent(g_nSelectedNetworkLobbyPeerId);
                pRuntime->m_dwSelectedPeerStatusTick = (int)timeGetTime();
            }
        }
    }
}

// FUNCTION: LEMBALL 0x00452740
int RegisterNetworkLobbyVsnetRuntimeWithTransport(void *pVsnetRuntime) {
    GAME_EffTransportRuntimeWindow *pWindow;

    pWindow = (GAME_EffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow;
    if (pWindow != 0 && pWindow->m_fRuntimeActive != 0) {
        pWindow->m_pPrimaryDispatchClient = pVsnetRuntime;
        ((GAME_EffTransportRuntimeWindowVtable *)*(void **)g_pActiveNetworkRuntimeWindow)->m_pServiceRuntime();
        ((GAME_RenderDispatchQueue *)g_pEffTransportSecondaryDispatchQueue)
            ->RegisterOrderedRenderDispatchClient(pVsnetRuntime, 0x19);
        return 1;
    }
    return 0;
}
#include "../network/safe_vtable.h"
