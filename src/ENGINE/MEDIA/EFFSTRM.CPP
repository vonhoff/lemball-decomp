#include "GAME.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/MEDIA/VSSTRM.H"
#include "ENGINE/CORE/SAFEVT.H"
#include "ENGINE/MEDIA/EFFSTRM.H"
#include "ENGINE/NET/VSNET_FORWARD.H"
#include <string.h>

extern int PASCAL ReturnTrueVtableCallbackThunk(void *pObject);
extern void *g_pNetworkLobbyVsnetRuntime;
extern char *g_pszPendingNetworkLobbyStatusText;
extern char *g_pszFileBasedNetworkLocalHostName;
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void NoopVtableCallbackThunk(void);
extern int LEMBALL_FASTCALL HandleNetworkLobbyVsnetRuntimeEvent(
    void *pVsnetRuntime, int nUnusedEdx, unsigned short *pEvent);
extern void LEMBALL_FASTCALL ReadNetworkLevelChunkDeltaStream(void *pObject);
void LEMBALL_FASTCALL ReadNetworkLobbyPeerEntryStream(void *pObject);
void LEMBALL_FASTCALL ReadNetworkLobbyPeerEntryPayload(void *pObject);
void LEMBALL_FASTCALL WriteNetworkLobbyPeerEntryPayload(void *pObject);
void LEMBALL_FASTCALL WriteNetworkLobbyU32PayloadStream(void *pObject);
void *LEMBALL_FASTCALL DeleteNetworkLobbySelectedPeerStatusStream(
    void *pObject, void *pUnused, BYTE fDelete);
void *LEMBALL_FASTCALL DeleteNetworkLobbyPeerEntryStream(
    void *pObject, void *pUnused, int fDelete);





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
    (void *)ReadNetworkLobbyPeerEntryPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteNetworkLobbyPeerEntryPayload,
    (void *)DeleteNetworkLobbyPeerEntryStream,
};

/* 004985F8 slot +20 is read_network_level_chunk_delta_stream.  Other
 * inherited slots remain compiler-owned callable placeholders until their
 * targets are recovered. */
struct GameNetworkLobbyVsnetRuntimeVtableModel {
    virtual void Reserved00(void) {}
    virtual void Reserved04(void) {}
    virtual int HandleRuntimeEvent(unsigned short *pEvent) {
        return HandleNetworkLobbyVsnetRuntimeEvent(this, 0, pEvent);
    }
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

static GameNetworkLobbyVsnetRuntimeVtableModel g_EFF_NetworkLobbyVsnetRuntimeVtableModel;
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
// GLOBAL: LEMBALL 0x004a0138
static const char g_EFF_NetworkRuntimeKey[] = "Paintball v0.1";
void *LEMBALL_FASTCALL ConstructNetworkLobbySelectedPeerStatusStream(
    void *pObject);
void *LEMBALL_FASTCALL ConstructNetworkLobbyPeerEntryStream(void *pObject);
void LEMBALL_FASTCALL WriteNetworkLobbyU32PayloadStream(void *pObject);
void LEMBALL_FASTCALL ReadNetworkLobbyU32PayloadStream(void *pObject);
void *LEMBALL_FASTCALL ConstructNetworkLobbyPeerClearCloseStream(
    void *pObject);
void *LEMBALL_FASTCALL ConstructNetworkLobbyPeerDirtyConfirmStream(
    void *pObject);
void DestroyNetworkLobbyVsnetRuntime(void *pObject);
void LEMBALL_FASTCALL UnregisterNetworkLobbyVsnetRuntimeFromTransport(void *pVsnetRuntime);
void LEMBALL_FASTCALL MarkNetworkLobbySelectedPeerDisconnected(void *pVsnetRuntime);
void LEMBALL_FASTCALL ServiceNetworkLobbySelectedPeerUpdates(void *pVsnetRuntime);
void LEMBALL_FASTCALL ReadNetworkLobbyPeerEntryStream(void *pObject);
void *LEMBALL_FASTCALL DeleteNetworkLobbySelectedPeerStatusStream(void *pObject, void *pUnused, BYTE fDelete);
int HasShiftedRecordSlotPayloadForStream(void *pTransportPeer, void *pStream);
void LoadStreamFromShiftedRecordSlotAndClear(void *pTransportPeer, void *pStream);
void SetEffTransportPrimaryHandleParams(void *pRuntimeWindow, int nPrimaryHandleCount, int nPrimaryHandleStride, int nFlags);
void SetEffTransportSecondaryHandleParams(void *pRuntimeWindow, int nSecondaryHandleCount, int nSecondaryHandleStride);

// GLOBAL: LEMBALL 0x004a1e1c
void *g_pEffTransportDispatchQueue = 0;
// GLOBAL: LEMBALL 0x004a1e20
void *g_pEffTransportSecondaryDispatchQueue = 0;

typedef int (LEMBALL_FASTCALL *GameEffStreamLoadHeaderProc)(GameEffStream *);
typedef void (LEMBALL_FASTCALL *GameEffStreamVoidProc)(GameEffStream *);

struct GameEffStreamVtable {
    void *m_pReserved00;
    GameEffStreamLoadHeaderProc m_pLoadFromMemoryHeader;
    GameEffStreamVoidProc m_pLoadFromMemoryBody;
    GameEffStreamVoidProc m_pBeginWriteHeader;
    GameEffStreamVoidProc m_pBeginWriteBody;
    void (*m_pDelete)(int);
};

struct GameNetworkLobbyPeerEntryStream {
    GameEffStreamVtable *m_pVtable;
    int m_nEventCode;
    int m_pvOwnedBuffer;
    int m_pvBufferEnd;
    int m_cWriteSessions;
    int m_fOwnsBuffer;
    int m_cbSerializedLength;
    int m_pvWriteCursor;
    int m_pvReadCursor;
    int m_fHasPayload;
    int m_fWritePending;
    char m_szPlayerName[9];
    char m_szHostName[23];
    int m_nPeerEntryState;
};
typedef char GameNetworkLobbyPeerEntryStream_size_must_be_0x50[
    sizeof(GameNetworkLobbyPeerEntryStream) == 0x50 ? 1 : -1];

struct GameDeleteObject {
    GameEffStreamVtable *m_pVtable;
};

struct GameRecordSlotTable {
    int m_cSlots;
    int m_nReserved04;
    void **m_apSlots08;

    void *GetShiftedRecordSlotByPacketId(int nPacketId);
};

struct GameEffTransportPeerDescriptor {
    void *m_pReserved00;
    int m_nEndpointStateSlotOffset;
    int m_nPayloadSenderSlotOffset;
    int m_nReserved0C;
    int m_nHandleGroupAdjustorSlotOffset;
};

struct GameEffTransportHandleCallbackVtable {
    int (*m_pBeginReset)(void);
    void *m_pReserved04;
    void (*m_pFinishReset)(void);
    void (*m_pNotifyClosed)(int);
};

struct GameEffTransportHandleCallback {
    GameEffTransportHandleCallbackVtable *m_pVtable;
};

struct GameEffTransportHandleGroup {
    GameEffTransportHandleCallback *m_pCallback;
    int m_nReserved04;
    int m_nReserved08;
    int m_fPrimaryActive;
    int m_fSecondaryActive;

    void Reset(void);
};

struct GameEffTransportEndpointState {
    int m_nReserved00;
    GameEffTransportHandleGroup m_HandleGroup;
    int m_fPrimaryHandlePresent;
    int m_fSecondaryHandlePresent;
    char m_abUnknown18[0x0c];
    short m_nAssignedPort;
};

struct GameEffTransportEndpointSlot {
    char m_abUnknown00[4];
    GameEffTransportEndpointState m_State;
};

struct GameEffTransportHandleOwner {
    char m_abUnknown00[0x48];
    void *m_pShiftedRecordSlotTable;
};

struct GameEffTransportHandleOwnerSlot {
    char m_abUnknown00[4];
    GameEffTransportHandleOwner m_HandleOwner;
};

struct GameEffTransportPeer {
    void **m_pVtable;
    GameEffTransportPeerDescriptor *m_pDescriptor;
    char *m_pszName;
    char *m_pszAddress;
    GameEffTransportPeer *m_pNext;
    GameEffTransportPeer *m_pPrevious;
    int m_cNewPortRequests;
    int m_nReserved1C;
    int m_fClosed;
    int m_fConnectComplete;
    DWORD m_dwConnectStartTick;
    int m_pConnectionState;
};

struct GameNetworkLobbyVsnetRuntime {
    void *m_pVtable;
    char m_abUnknown04[0x0c];
    GameEffStream *m_pPeerEntryWriteStream;
    GameEffStream *m_pPeerEntryTable;
    GameEffStream *m_pSelectedPeerStatusStream;
    GameEffStream *m_pPeerClearCloseStream;
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

    GameNetworkLobbyVsnetRuntime *Construct(char *pszFileBasedNetworkRoot);
    int RegisterWithTransport(void);
};

struct GameEffTransportRuntimeWindow {
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

struct GameEffTransportRuntimeWindowVtable {
    void *m_pReserved00[8];
    void (LEMBALL_FASTCALL *m_pServiceRuntime)(void *, int);
};

struct GamePeerEntryStreamTable {
    int m_cEntries;
    GameNetworkLobbyPeerEntryStream m_aEntries[10];
};

struct GameEffStreamDeleteVtable {
    void *m_pReserved00[5];
    void *(LEMBALL_FASTCALL *m_pDelete)(void *, void *, int);
};

struct GameShiftedRecordSlot {
    int m_nReserved00;
    int m_pSerializedPayload;
    void **m_pLockVtable;
    char m_abUnknown0c[0x18];
    int m_fHasPayload;
};

struct GameShiftedRecordLockVtable {
    void (*m_pLock)(void);
    void (*m_pUnlock)(void);
};

struct GameRenderQueueServiceVtable {
    void *m_pReserved00[2];
    void (*m_pQueueWriteEvent)(void *pEvent);
};

struct GameEffStreamLoadInterface {
    virtual void Reserved00(void) = 0;
    virtual int LoadFromMemoryHeader(void) = 0;
    virtual void LoadFromMemoryBody(void) = 0;
};

struct GameRenderQueueServiceInterface {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void QueueWriteEvent(void *pEvent) = 0;
};

struct GameEffTransportRuntimeServiceInterface {
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

struct GameEffQueuedWriteEvent {
    unsigned short m_nEventType;
    unsigned short m_aPadding[3];
    int m_fWrite;
    void *m_pStream;
    int m_nPayload;
};

struct GameNonZrleVariantRenderEntryInitializeVtable {
    char m_abUnknown00[0x18];
    void (*m_pDelete)(int);
};

extern "C" DWORD WINAPI timeGetTime(void);

extern int g_nSelectedNetworkLobbyPeerId;
extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pNonZrleVariantRenderEntryInitializeVtable[2];
struct VsNetEffTransportPeer {
    void Close(void);
};
struct VsNetRuntimeStartView {
    int StartEffTransportRuntimeAndWaitReady(int nRuntimeKey, int cbMaxPacket);
};
extern void *LEMBALL_FASTCALL InitializeRenderQueueNodeBase(void *pRenderQueueNode);
extern int StartFileBasedNetworkMessageThread(void);
extern int StartTcpipNetworkMessageThread(void);
extern void WINAPI ConfigureFileBasedNetworkPathsWrapper(char *pszBasePath, char *pszNetworkPath);
extern int StopFileBasedNetworkMessageThread(void);
extern int StopTcpipNetworkMessageThread(void);
void *LEMBALL_FASTCALL ConstructNetworkLobbyPeerClearCloseStream(
    void *pObject);

























#include "ENGINE/CORE/SAFEVT.H"


// FUNCTION: LEMBALL 0x004013c0
GameEffStream *GameEffStream::ConstructPayloadSize8Thunk(void) {
    return ConstructPayloadSize8();
}

// FUNCTION: LEMBALL 0x004453b0
GameEffStream *GameEffStream::ConstructPayloadSize8(void) {
    m_pVtable =
        (GameEffStreamVtable *)g_GAME_EffStreamConstructionVtable;
    m_nEventCode = 8;
    ResetStateFields();
    m_pVtable = (GameEffStreamVtable *)g_EFF_PayloadSize8Vtable;
    m_cbSerializedLength += 8;
    m_fHasPayload = 1;
    return this;
}

// FUNCTION: LEMBALL 0x004523b0
void *LEMBALL_FASTCALL ConstructNetworkLobbySelectedPeerStatusStream(void *pObject) {
    GameEffStream *pStream;

    pStream = (GameEffStream *)pObject;
    pStream->m_pVtable =
        (GameEffStreamVtable *)g_GAME_EffStreamConstructionVtable;
    pStream->m_nEventCode = 4;
    pStream->ResetStateFields();
    pStream->m_pVtable = (GameEffStreamVtable *)g_EFF_NetworkLobbySelectedPeerStatusVtable;
    pStream->m_cbSerializedLength += 4;
    return pObject;
}

// FUNCTION: LEMBALL 0x004523e0
void *LEMBALL_FASTCALL ConstructNetworkLobbyPeerEntryStream(void *pObject) {
    GameNetworkLobbyPeerEntryStream *pStream;
    GameEffStream *pBaseStream;

    pStream = (GameNetworkLobbyPeerEntryStream *)pObject;
    pBaseStream = (GameEffStream *)pObject;
    pStream->m_pVtable =
        (GameEffStreamVtable *)g_GAME_EffStreamConstructionVtable;
    pStream->m_nEventCode = 5;
    pBaseStream->ResetStateFields();
    pStream->m_pVtable =
        (GameEffStreamVtable *)g_EFF_NetworkLobbyPeerEntryVtable;
    pStream->m_szPlayerName[0] = '\0';
    pStream->m_nPeerEntryState = 0;
    pStream->m_cbSerializedLength += 0x1e;
    pStream->m_fHasPayload = 1;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452420
void LEMBALL_FASTCALL WriteNetworkLobbyPeerEntryPayload(void *pObject) {
    VsNetEffStreamBase *pStream;
    GameDynamicCString HostName;
    GameDynamicCString LowercaseHostName;
    char szHostName[21];

    pStream = (VsNetEffStreamBase *)pObject;
    pStream->WriteEffStreamCString(g_pszPendingNetworkLobbyStatusText);
    strncpy(szHostName, g_pszFileBasedNetworkLocalHostName, 20);
    szHostName[20] = '\0';
    HostName.ConstructDynamicCStringFromCString(szHostName);
    HostName.LowercaseAndCopyDynamicCString(&LowercaseHostName);
    HostName.DestroyDynamicCString();
    pStream->WriteEffStreamCString(LowercaseHostName.m_pszText);
    LowercaseHostName.DestroyDynamicCString();
}

// FUNCTION: LEMBALL 0x00452490
void LEMBALL_FASTCALL ReadNetworkLobbyPeerEntryPayload(void *pObject) {
    GameNetworkLobbyPeerEntryStream *pEntry;
    VsNetEffStreamBase *pStream;

    pEntry = (GameNetworkLobbyPeerEntryStream *)pObject;
    pStream = (VsNetEffStreamBase *)pObject;
    pStream->ReadEffStreamCStringIntoBuffer(pEntry->m_szPlayerName);
    pStream->ReadEffStreamCStringIntoBuffer(pEntry->m_szHostName);
    pEntry->m_nPeerEntryState = 1;
}

// FUNCTION: LEMBALL 0x004524b0
GameEffStream *GameEffStream::ConstructNetworkLobbyU32PayloadStream(int nEventCode) {
    m_pVtable =
        (GameEffStreamVtable *)g_GAME_EffStreamConstructionVtable;
    m_nEventCode = nEventCode;
    ResetStateFields();
    m_pVtable = (GameEffStreamVtable *)g_EFF_NetworkLobbyU32PayloadVtable;
    m_cbSerializedLength += 4;
    m_fHasPayload = 1;
    m_nU32Payload = 0;
    return this;
}

// FUNCTION: LEMBALL 0x004524f0
void LEMBALL_FASTCALL WriteNetworkLobbyU32PayloadStream(void *pObject) {
    VsNetEffStreamBase *pStream;

    pStream = (VsNetEffStreamBase *)pObject;
    pStream->WriteEffStreamU32BE(*(unsigned int *)((char *)pObject + 0x2c));
}

// FUNCTION: LEMBALL 0x00452500
void LEMBALL_FASTCALL ReadNetworkLobbyU32PayloadStream(void *pObject) {
    VsNetEffStreamBase *pStream;

    pStream = (VsNetEffStreamBase *)pObject;
    *(unsigned int *)((char *)pObject + 0x2c) = pStream->ReadEffStreamU32BEValue();
}

// FUNCTION: LEMBALL 0x00452510
void *LEMBALL_FASTCALL ConstructNetworkLobbyPeerClearCloseStream(void *pObject) {
    ((GameEffStream *)pObject)->ConstructNetworkLobbyU32PayloadStream(6);
    *(void **)pObject = g_EFF_NetworkLobbyPeerClearCloseVtable;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452530
void *LEMBALL_FASTCALL ConstructNetworkLobbyPeerDirtyConfirmStream(void *pObject) {
    ((GameEffStream *)pObject)->ConstructNetworkLobbyU32PayloadStream(7);
    *(void **)pObject = g_EFF_NetworkLobbyPeerDirtyConfirmVtable;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452550
GameNetworkLobbyVsnetRuntime *GameNetworkLobbyVsnetRuntime::Construct(char *pszFileBasedNetworkRoot) {
    GameNetworkLobbyVsnetRuntime *pRuntime;
    GamePeerEntryStreamTable *pPeerEntryTable;
    GameNetworkLobbyPeerEntryStream *pPeerEntryStream;
    GameEffStream *pStream;
    int i;
    int fStarted;

    pRuntime = this;
    fStarted = 0;

    InitializeRenderQueueNodeBase(this);
    pRuntime->m_pVtable = g_EFF_NetworkLobbyVsnetRuntimeVtable;
    pRuntime->m_fFileBasedThreadStarted = 0;
    pRuntime->m_fTcpipThreadStarted = 0;
    pRuntime->m_nDesiredSelectedPeerStatus = 0;
    pRuntime->m_nReserved54 = 0;

    pPeerEntryTable = (GamePeerEntryStreamTable *)AllocateVSMemBlock(0x324);
    if (pPeerEntryTable == 0) {
        pRuntime->m_pPeerEntryTable = 0;
    } else {
        pPeerEntryTable->m_cEntries = 10;
        pPeerEntryStream = pPeerEntryTable->m_aEntries;
        for (i = 0; i < 10; ++i) {
            ConstructNetworkLobbyPeerEntryStream(pPeerEntryStream);
            ++pPeerEntryStream;
        }
        pRuntime->m_pPeerEntryTable =
            (GameEffStream *)pPeerEntryTable->m_aEntries;
    }

    pStream = (GameEffStream *)AllocateVSMemBlock(0x30);
    if (pStream == 0) {
        pRuntime->m_pSelectedPeerStatusStream = 0;
    } else {
        pRuntime->m_pSelectedPeerStatusStream =
            (GameEffStream *)ConstructNetworkLobbySelectedPeerStatusStream(pStream);
    }

    pRuntime->m_dwSelectedPeerStatusTick = (int)(timeGetTime() - 2000);
    pRuntime->m_nObservedSelectedPeerStatus = 0;
    pRuntime->m_nDesiredSelectedPeerStatus = 0;

    pStream = (GameEffStream *)AllocateVSMemBlock(0x50);
    if (pStream == 0) {
        pRuntime->m_pPeerEntryWriteStream = 0;
    } else {
        pRuntime->m_pPeerEntryWriteStream =
            (GameEffStream *)ConstructNetworkLobbyPeerEntryStream(pStream);
    }

    pStream = (GameEffStream *)AllocateVSMemBlock(0x30);
    if (pStream == 0) {
        pRuntime->m_pPeerClearCloseStream = 0;
    } else {
        pRuntime->m_pPeerClearCloseStream =
            (GameEffStream *)ConstructNetworkLobbyPeerClearCloseStream(pStream);
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

        if (((VsNetRuntimeStartView *)g_pActiveNetworkRuntimeWindow)
                ->StartEffTransportRuntimeAndWaitReady(
                    (int)(unsigned long)g_EFF_NetworkRuntimeKey,
                    0x400) != 0) {
            SetEffTransportSecondaryHandleParams(g_pActiveNetworkRuntimeWindow, 100, 0x10);
            SetEffTransportPrimaryHandleParams(g_pActiveNetworkRuntimeWindow, 4, 4, 0);
            pRuntime->m_fSelectedPeerDisconnected = 0;
            g_nSelectedNetworkLobbyPeerId = 0;
            pRuntime->m_fSelectionNeedsReset = 0;
        }
    }

    return this;
}

// FUNCTION: LEMBALL 0x004526e0
void DestroyNetworkLobbyVsnetRuntime(void *pObject) {
    GameNetworkLobbyVsnetRuntime *pRuntime;
    void *pOwnedObject;

    pRuntime = (GameNetworkLobbyVsnetRuntime *)pObject;
    pRuntime->m_pVtable = g_EFF_NetworkLobbyVsnetRuntimeVtable;

    if (pRuntime->m_fFileBasedThreadStarted != 0) {
        StopFileBasedNetworkMessageThread();
    }
    if (pRuntime->m_fTcpipThreadStarted != 0) {
        StopTcpipNetworkMessageThread();
    }

    pOwnedObject = pRuntime->m_pPeerClearCloseStream;
    if (pOwnedObject != 0) {
        ((GameEffStreamDeleteVtable *)*(void **)pOwnedObject)
            ->m_pDelete(pOwnedObject, 0, 1);
    }

    pOwnedObject = pRuntime->m_pSelectedPeerStatusStream;
    if (pOwnedObject != 0) {
        ((GameEffStreamDeleteVtable *)*(void **)pOwnedObject)
            ->m_pDelete(pOwnedObject, 0, 1);
    }

    pOwnedObject = pRuntime->m_pPeerEntryWriteStream;
    if (pOwnedObject != 0) {
        ((GameEffStreamDeleteVtable *)*(void **)pOwnedObject)
            ->m_pDelete(pOwnedObject, 0, 1);
    }

    pOwnedObject = pRuntime->m_pPeerEntryTable;
    if (pOwnedObject != 0) {
        ((GameEffStreamDeleteVtable *)*(void **)pOwnedObject)
            ->m_pDelete(pOwnedObject, 0, 3);
    }

    pRuntime->m_pVtable = g_pNonZrleVariantRenderEntryInitializeVtable;
}

// FUNCTION: LEMBALL 0x00452740
int GameNetworkLobbyVsnetRuntime::RegisterWithTransport(void) {
    GameEffTransportRuntimeWindow *pWindow;

    pWindow = (GameEffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow;
    if (pWindow != 0 && pWindow->m_fRuntimeActive != 0) {
        pWindow->m_pPrimaryDispatchClient = this;
        ((GameEffTransportRuntimeWindowVtable *)pWindow->m_pVtable)
            ->m_pServiceRuntime(pWindow, 0);
        ((GameRenderDispatchQueue *)g_pEffTransportSecondaryDispatchQueue)
            ->RegisterOrderedRenderDispatchClient(this, 0x19);
        return 1;
    }
    return 0;
}

void ConfigureNetworkLobbyTransportHostName(void *pRuntime,
                                            char *pszHostName) {
    GameEffTransportRuntimeWindow *pWindow;
    void *pOwner;
    void **pOwnerVtable;

    pWindow = (GameEffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow;
    if (pRuntime == 0 || pWindow == 0) {
        return;
    }
    pOwner = *(void **)((char *)pWindow + 0x24);
    if (pOwner == 0) {
        return;
    }
    pOwnerVtable = *(void ***)pOwner;
    ((void (*)(void *))pOwnerVtable[6])(pOwner);
    ConfigureActiveEffTransportConnectHost(pRuntime, pszHostName);
    *(int *)((char *)pWindow + 0x34) = 1;
    ((void (*)(void *))pOwnerVtable[5])(pOwner);
}

// FUNCTION: LEMBALL 0x004527c0
void LEMBALL_FASTCALL UnregisterNetworkLobbyVsnetRuntimeFromTransport(void *pVsnetRuntime) {
    GameNetworkLobbyVsnetRuntime *pRuntime;
    GameEffTransportRuntimeWindow *pWindow;
    DWORD dwStartTime;

    pRuntime = (GameNetworkLobbyVsnetRuntime *)pVsnetRuntime;
    if (g_nSelectedNetworkLobbyPeerId != 0) {
        pRuntime->m_pPeerClearCloseStream->m_nU32Payload = 1;
        pRuntime->m_pPeerClearCloseStream
            ->QueueEffStreamWriteEvent(g_nSelectedNetworkLobbyPeerId);
        dwStartTime = timeGetTime();
        while (pRuntime->m_pPeerClearCloseStream->m_fWritePending != 0) {
            if (timeGetTime() - dwStartTime >= 1000) {
                break;
            }
        }
        ((VsNetEffTransportPeer *)(unsigned long)g_nSelectedNetworkLobbyPeerId)->Close();
        g_nSelectedNetworkLobbyPeerId = 0;
    }

    pWindow = (GameEffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow;
    if (pWindow != 0 && pWindow->m_fRuntimeActive != 0) {
        ((GameRenderDispatchQueue *)g_pEffTransportSecondaryDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(pVsnetRuntime, 0x19);
        pWindow->m_pReserved40 = pVsnetRuntime;
        ((GameEffTransportRuntimeWindowVtable *)pWindow->m_pVtable)
            ->m_pServiceRuntime(pWindow, 0);
    }
}

// FUNCTION: LEMBALL 0x00452ab0
void LEMBALL_FASTCALL MarkNetworkLobbySelectedPeerDisconnected(void *pVsnetRuntime) {
    ((GameNetworkLobbyVsnetRuntime *)pVsnetRuntime)->m_fSelectionNeedsReset = 1;
}

// FUNCTION: LEMBALL 0x00452ac0
void LEMBALL_FASTCALL ServiceNetworkLobbySelectedPeerUpdates(void *pVsnetRuntime) {
    GameNetworkLobbyVsnetRuntime *pRuntime;
    GameEffTransportPeer *pSelectedPeer;
    GameEffTransportHandleOwner *pHandleOwner;
    DWORD dwNow;
    int nStreamValue;

    pRuntime = (GameNetworkLobbyVsnetRuntime *)pVsnetRuntime;
    if (pRuntime->m_fSelectionNeedsReset != 0) {
        g_nSelectedNetworkLobbyPeerId = 0;
        pRuntime->m_fSelectionNeedsReset = 0;
    }
    if (g_nSelectedNetworkLobbyPeerId != 0) {
        pSelectedPeer = (GameEffTransportPeer *)(unsigned long)g_nSelectedNetworkLobbyPeerId;
        pHandleOwner = &((GameEffTransportHandleOwnerSlot *)((char *)pSelectedPeer +
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

// FUNCTION: LEMBALL 0x00452e50
void LEMBALL_FASTCALL ReadNetworkLobbyPeerEntryStream(void *pObject) {
    VsNetEffStreamBase *pStream;

    pStream = (VsNetEffStreamBase *)pObject;
    *(unsigned int *)((char *)pObject + 0x2c) = pStream->ReadEffStreamU32BEValue();
}

// FUNCTION: LEMBALL 0x00452e60
void *LEMBALL_FASTCALL DeleteNetworkLobbySelectedPeerStatusStream(void *pObject, void *pUnused, BYTE fDelete) {
    VsNetEffStreamBase *pStream;

    (void)pUnused;
    pStream = (VsNetEffStreamBase *)pObject;
    pStream->DestroyEffStreamBase();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00452f50
void *LEMBALL_FASTCALL DeleteNetworkLobbyPeerEntryStream(void *pObject, void *, int fDelete) {
    char *pEntry;
    char *pAllocation;
    int cEntries;

    if ((fDelete & 2) != 0) {
        pAllocation = (char *)pObject - sizeof(int);
        cEntries = *(int *)pAllocation;
        pEntry = (char *)pObject + cEntries * 0x50;
        while (0 < cEntries) {
            pEntry -= 0x50;
            ((VsNetEffStreamBase *)pEntry)->DestroyEffStreamBase();
            --cEntries;
        }
        FreeVSMemBlock(pAllocation);
    } else {
        ((VsNetEffStreamBase *)pObject)->DestroyEffStreamBase();
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pObject);
        }
    }
    return pObject;
}

struct NetworkLobbyVsnetRuntimeDeleteView {
    virtual void Reserved00(void) = 0;
    virtual void Delete(int fDelete) = 0;
};

extern void LEMBALL_FASTCALL CloseNetworkLobbyTransportPeers(
    void *pController);

// FUNCTION: LEMBALL 0x00455130
void LEMBALL_FASTCALL StartNetworkLobbyTransportSession(void *pController) {
    int *pnControllerFields;
    void *pRuntimeStorage;
    void *pEventClient;
    NetworkLobbyVsnetRuntimeDeleteView *pVsnetRuntime;

    pnControllerFields = (int *)pController;
    pnControllerFields[0x28 / 4] = 0;
    if (pnControllerFields[0x2c / 4] != 0) {
        return;
    }
    pRuntimeStorage = AllocateVSMemBlock(0x6c);
    if (pRuntimeStorage != 0) {
        g_pNetworkLobbyVsnetRuntime =
            ((GameNetworkLobbyVsnetRuntime *)pRuntimeStorage)->Construct(0);
    } else {
        g_pNetworkLobbyVsnetRuntime = 0;
    }
    if (((GameNetworkLobbyVsnetRuntime *)g_pNetworkLobbyVsnetRuntime)
            ->RegisterWithTransport() == 0) {
        UnregisterNetworkLobbyVsnetRuntimeFromTransport(
            g_pNetworkLobbyVsnetRuntime);
        if (g_pNetworkLobbyVsnetRuntime != 0) {
            pVsnetRuntime = (NetworkLobbyVsnetRuntimeDeleteView *)
                g_pNetworkLobbyVsnetRuntime;
            pVsnetRuntime->Delete(1);
        }
        g_pNetworkLobbyVsnetRuntime = 0;
        pnControllerFields[0x28 / 4] = 1;
        return;
    }
    pEventClient = pController == 0 ? 0 : (char *)pController + 0xc;
    ((GameEffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow)
        ->RegisterEffTransportEventClient(pEventClient);
    pnControllerFields[0x2c / 4] = 1;
}

// FUNCTION: LEMBALL 0x004552a0
void LEMBALL_FASTCALL StopNetworkLobbyTransportSession(void *pController) {
    int *pnControllerFields;
    NetworkLobbyVsnetRuntimeDeleteView *pVsnetRuntime;
    DWORD dwStartTime;

    pnControllerFields = (int *)pController;
    CloseNetworkLobbyTransportPeers(pController);
    if (g_pNetworkLobbyVsnetRuntime != 0) {
        UnregisterNetworkLobbyVsnetRuntimeFromTransport(
            g_pNetworkLobbyVsnetRuntime);
    }
    if (g_pActiveNetworkRuntimeWindow != 0) {
        dwStartTime = timeGetTime();
        do {
            if (timeGetTime() - dwStartTime >= 2000) {
                break;
            }
        } while (*(int *)((char *)g_pActiveNetworkRuntimeWindow + 0x48) != 0);
    }
    if (g_pNetworkLobbyVsnetRuntime != 0) {
        pVsnetRuntime = (NetworkLobbyVsnetRuntimeDeleteView *)
            g_pNetworkLobbyVsnetRuntime;
        pVsnetRuntime->Delete(1);
        g_pNetworkLobbyVsnetRuntime = 0;
    }
    pnControllerFields[0x28 / 4] = 0;
    pnControllerFields[0x2c / 4] = 0;
}

// FUNCTION: LEMBALL 0x0045ee80
void GameEffStream::ResetStateFields(void) {
    m_fHasPayload = 0;
    m_fWritePending = 0;
    m_fOwnsBuffer = 0;
    m_pvOwnedBuffer = 0;
    m_cbSerializedLength = 0;
    m_cWriteSessions = 0;
}

// FUNCTION: LEMBALL 0x0045f1e0
void GameEffStream::BeginEffStreamWriteSession(void) {
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

// FUNCTION: LEMBALL 0x0045f240
void GameEffStream::EndEffStreamWriteSession(void) {
    --m_cWriteSessions;
}

// FUNCTION: LEMBALL 0x0045f280
int GameEffStream::LoadEffStreamFromMemory(int nSourceBuffer) {
    GameEffStreamVtable *pVtable;

    pVtable = m_pVtable;
    m_pvReadCursor = nSourceBuffer;
    if (pVtable->m_pLoadFromMemoryHeader(this) != 0) {
        pVtable->m_pLoadFromMemoryBody(this);
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045f2b0
void GameEffStream::QueueEffStreamWriteEvent(int nPayload) {
    GameEffQueuedWriteEvent event;

    if (nPayload != 0) {
        event.m_nPayload = nPayload;
        event.m_fWrite = 1;
        event.m_nEventType = 0xb;
        event.m_pStream = this;
        BeginEffStreamWriteSession();
        m_fWritePending = 1;
        ((GameRenderQueueServiceInterface *)g_pEffTransportDispatchQueue)
            ->QueueWriteEvent(&event);
        ((GameEffTransportRuntimeServiceInterface *)
             g_pActiveNetworkRuntimeWindow)
            ->ServiceRuntime();
    }
}

// FUNCTION: LEMBALL 0x0045f720
void GameEffTransportHandleGroup::Reset(void) {
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

// FUNCTION: LEMBALL 0x0045fc30
int HasShiftedRecordSlotPayloadForStream(void *pTransportPeer, void *pStream) {
    GameEffTransportHandleOwner *pHandleOwner;
    GameShiftedRecordSlot *pSlot;

    pHandleOwner = (GameEffTransportHandleOwner *)pTransportPeer;
    pSlot = (GameShiftedRecordSlot *)
        ((GameRecordSlotTable *)pHandleOwner->m_pShiftedRecordSlotTable)
            ->GetShiftedRecordSlotByPacketId(((GameEffStream *)pStream)->m_nEventCode);
    return pSlot->m_fHasPayload;
}

// FUNCTION: LEMBALL 0x0045fc50
void LoadStreamFromShiftedRecordSlotAndClear(void *pTransportPeer, void *pStream) {
    GameEffTransportHandleOwner *pHandleOwner;
    GameShiftedRecordSlot *pSlot;
    GameShiftedRecordLockVtable *pLockVtable;

    pHandleOwner = (GameEffTransportHandleOwner *)pTransportPeer;
    pSlot = (GameShiftedRecordSlot *)
        ((GameRecordSlotTable *)pHandleOwner->m_pShiftedRecordSlotTable)
            ->GetShiftedRecordSlotByPacketId(((GameEffStream *)pStream)->m_nEventCode);
    pLockVtable = (GameShiftedRecordLockVtable *)pSlot->m_pLockVtable;
    pLockVtable->m_pLock();
    ((GameEffStream *)pStream)->LoadEffStreamFromMemory(pSlot->m_pSerializedPayload + 0x10);
    pLockVtable->m_pUnlock();
    pSlot->m_fHasPayload = 0;
}

// FUNCTION: LEMBALL 0x004615f0
void *GameRecordSlotTable::GetShiftedRecordSlotByPacketId(int nPacketId) {
    if (nPacketId >= 3) {
        nPacketId -= 3;
    }
    return m_apSlots08[nPacketId];
}

// FUNCTION: LEMBALL 0x00462550
void SetEffTransportPrimaryHandleParams(void *pRuntimeWindow, int nPrimaryHandleCount, int nPrimaryHandleStride, int nFlags) {
    GameEffTransportRuntimeWindow *pWindow;

    pWindow = (GameEffTransportRuntimeWindow *)pRuntimeWindow;
    pWindow->m_nPrimaryHandleCount = nPrimaryHandleCount;
    pWindow->m_nPrimaryHandleStride = nPrimaryHandleStride;
    pWindow->m_nPrimaryHandleFlags = nFlags;
}

// FUNCTION: LEMBALL 0x00462570
void SetEffTransportSecondaryHandleParams(void *pRuntimeWindow, int nSecondaryHandleCount, int nSecondaryHandleStride) {
    GameEffTransportRuntimeWindow *pWindow;

    pWindow = (GameEffTransportRuntimeWindow *)pRuntimeWindow;
    pWindow->m_nSecondaryHandleCount = nSecondaryHandleCount;
    pWindow->m_nSecondaryHandleStride = nSecondaryHandleStride;
}

// FUNCTION: LEMBALL 0x00462590
void GameEffTransportRuntimeWindow::RegisterEffTransportEventClient(void *pClient) {
    m_pSecondaryDispatchClient = pClient;
    ((GameRenderDispatchQueue *)g_pEffTransportSecondaryDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(pClient, 0);
}

// FUNCTION: LEMBALL 0x004625b0
void GameEffTransportRuntimeWindow::UnregisterEffTransportEventClient(void) {
    void *pClient;

    pClient = m_pSecondaryDispatchClient;
    if (pClient != 0) {
        ((GameRenderDispatchQueue *)g_pEffTransportSecondaryDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(pClient, 0);
        m_pSecondaryDispatchClient = 0;
    }
}
