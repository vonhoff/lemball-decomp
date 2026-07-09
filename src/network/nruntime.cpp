#include "../game/game_app.h"
#include "../engine/runtime_init.h"
#include "../engine/memory_arena.h"
#include "../platform/message_window.h"

#include <string.h>

extern int ReturnTrueVtableCallback(void);

extern "C" BOOL WINAPI WaitMessage(void);
extern "C" HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES pThreadAttributes,
                                      DWORD cbStack,
                                      DWORD(WINAPI *pfnStartAddress)(LPVOID),
                                      LPVOID pvThreadParam,
                                      DWORD dwCreationFlags,
                                      LPDWORD pdwThreadId);
extern "C" BOOL WINAPI SetThreadPriority(HANDLE hThread, int nPriority);
extern "C" void WINAPI ExitProcess(UINT uExitCode);

static void *g_NETWORK_FileBasedRuntimeWindowVtable = (void *)0x0049A02C;
static void *g_NETWORK_FileBasedRuntimeTransportVtable = (void *)0x00499FF8;
static void *g_NETWORK_TcpipRuntimeWindowVtable = (void *)0x0049A2DC;
static void *g_NETWORK_TcpipRuntimeTransportVtable = (void *)0x0049A2A8;
static void *g_NETWORK_EffTransportRuntimeStateVtable = (void *)0x004991E0;
static void *g_NETWORK_ReturnTrueVtable[1] = {
    (void *)ReturnTrueVtableCallback,
};
static void *g_NETWORK_EffTransportGlobalWriteStreamVtable = (void *)0x00499218;
static void *g_NETWORK_EffTransportGlobalReadStreamVtable = (void *)0x00499230;
static void *g_NETWORK_RequestConnectControlStreamNameVtable = (void *)0x00498EA0;
static void *g_NETWORK_RequestConnectControlStreamVtable = (void *)0x00498EB8;
static void *g_NETWORK_RequestNewPortControlStreamVtable = (void *)0x00498ED0;
static void *g_NETWORK_AuthoriseConnectControlStreamVtable = (void *)0x00498EE8;
static void *g_NETWORK_GoAheadConnectControlStreamVtable = (void *)0x00498F00;
static void *g_NETWORK_FailedConnectControlStreamVtable = (void *)0x00498F18;

struct NETWORK_RenderQueueNode {
    void **m_pVtable;
    int m_uMagic;
    void *m_pReserved08;
    void *m_pReserved0C;
};

struct NETWORK_EffTransportRuntimeStateVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void *m_pReserved08;
    void (*m_pInitializeTransport)(void *);
    void *m_pReserved10;
    void *m_pReserved14;
    void *m_pReserved18;
    void *m_pReserved1C;
    void (*m_pServiceTransport)(void *);
    void *m_pReserved24;
    int (*m_pCreateChannelOwner)(void *);
    void *m_pReserved2C;
    void (*m_pWaitForTransportMessage)(void *);
};

struct NETWORK_ChannelOwnerVtable {
    short (*m_pAllocatePort)(void *);
    void (*m_pReleasePort)(DWORD);
    void *m_pReserved08;
    int (*m_pBindRuntimeKey)(void *, int);
    void (*m_pShutdown)(void *);
    void *m_pReserved14;
    void (*m_pLockPeerList)(void *);
    void (*m_pUnlockPeerList)(void *);
    int (*m_pAllocatePeer)(void *);
};

struct NETWORK_TransportEndpointOffsets;

struct NETWORK_ChannelOwnerObject {
    NETWORK_ChannelOwnerVtable *m_pVtable;
    NETWORK_TransportEndpointOffsets *m_pOffsets;
    int m_nSendGate;
    int m_nReserved0C;
    unsigned char *m_pbAllocatedPorts;
};

struct NETWORK_TransportEndpointOffsets {
    void *m_pReserved00;
    int m_nEndpointStateSlotOffset;
    int m_nPayloadSenderSlotOffset;
    int m_nReserved0C;
    int m_nHandleGroupAdjustorSlotOffset;
};

struct NETWORK_EffTransportHandleCallbackVtable {
    int (*m_pBeginReset)(void);
    void *m_pReserved04;
    void (*m_pFinishReset)(void);
    void (*m_pNotifyClosed)(int);
};

struct NETWORK_EffTransportHandleCallback {
    NETWORK_EffTransportHandleCallbackVtable *m_pVtable;
};

struct NETWORK_EffTransportHandleGroup {
    NETWORK_EffTransportHandleCallback *m_pCallback;
    int m_nReserved04;
    int m_nReserved08;
    int m_fPrimaryHandlePresent;
    int m_fSecondaryHandlePresent;
};

struct NETWORK_EndpointState {
    int m_nReserved00;
    NETWORK_EffTransportHandleGroup m_HandleGroup;
    int m_fPrimaryHandleActive;
    int m_fSecondaryHandleActive;
    char m_abUnknown18[0x0c];
    short m_nAssignedPort;
};

struct NETWORK_TransportEndpointSlot {
    char m_abUnknown00[4];
    NETWORK_EndpointState m_State;
};

struct NETWORK_TransportPeerMatcher {
    void **m_pVtable;
    char m_abUnknown04[0x70];
};

struct NETWORK_TransportPeerMatcherVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void *m_pReserved08;
    void *m_pReserved0C;
    int (*m_pMatchPeerKey)(void *);
};

struct NETWORK_HandleGroupAdjustorOffsets {
    void *m_pReserved00;
    void *m_pReserved04;
    int m_nTimedStreamOffset;
    int m_nHandleStreamOffset;
};

struct NETWORK_HandleGroupAdjustor {
    NETWORK_HandleGroupAdjustorOffsets *m_pOffsets;
};

struct NETWORK_HandleGroupAdjustorSlot {
    char m_abUnknown00[4];
    NETWORK_HandleGroupAdjustor m_Adjustor;
};

struct NETWORK_EffTransportPeer;

struct NETWORK_PeerPayloadSenderVtable {
    void *m_apReserved00[8];
    void (*m_pWritePeerKey)(void *);
    void *m_pReserved24;
    void (*m_pSetAssignedPort)(short);
};

struct NETWORK_PeerPayloadSender {
    NETWORK_PeerPayloadSenderVtable *m_pVtable;
};

struct NETWORK_PeerPayloadSenderSlot {
    char m_abUnknown00[4];
    NETWORK_PeerPayloadSender m_Sender;
    char m_abUnknown08[0x6c];
    NETWORK_TransportPeerMatcher *m_pMatcher;
};

struct NETWORK_DeleteObjectVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void *m_pReserved08;
    void *m_pReserved0C;
    void *m_pReserved10;
    void (*m_pDelete)(int);
};

struct NETWORK_DeleteObject {
    NETWORK_DeleteObjectVtable *m_pVtable;
};

struct NETWORK_RuntimeServiceVtable {
    char *(*m_pGetLocalHostName)(void);
    void *m_pReserved04;
    void (*m_pSetLocalHostName)(char *);
    void *m_pReserved0C;
    void *m_pReserved10;
    int (*m_pValidatePeerKey)(void *);
};

struct NETWORK_RuntimeServiceObject {
    NETWORK_RuntimeServiceVtable *m_pVtable;
};

struct NETWORK_EffControlStreamHeader {
    void *m_pVtable;
    int m_nEventCode;
    char m_abUnknown08[0x10];
    int m_cbSerializedLength;
    char m_abUnknown1C[0x10];
    char *m_pszStreamName;
};

struct NETWORK_RequestConnectControlStream {
    NETWORK_EffControlStreamHeader m_Header;
    short m_nBasePort;
    short m_nReserved32;
    void *m_pPortBinding;
    char *m_pszHostName;
};

struct NETWORK_RequestNewPortControlStream {
    NETWORK_RequestConnectControlStream m_Base;
    NETWORK_EffTransportPeer *m_pPeer;
};

struct NETWORK_AuthoriseConnectControlStream {
    NETWORK_EffControlStreamHeader m_Header;
    short m_nAssignedPort;
    short m_nReserved32;
    NETWORK_EffTransportPeer *m_pPeer;
};

struct NETWORK_FailedConnectControlStream {
    NETWORK_EffControlStreamHeader m_Header;
    char *m_pszFailureMessage;
};

struct NETWORK_EffTransportPeerVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void *m_pReserved08;
    void (*m_pSetPeerKey)(void *);
    void (*m_pCompleteConnect)(void);
};

struct NETWORK_EffDispatchEvent {
    unsigned short m_nType;
    unsigned short m_anReserved[3];
    int m_nCode;
    int *m_pStream;
    void *m_pPeer;
};

struct NETWORK_EffTransportPeer {
    void **m_pVtable;
    NETWORK_TransportEndpointOffsets *m_pOffsets;
    char *m_pszPeerName;
    char *m_pszPeerAddress;
    NETWORK_EffTransportPeer *m_pNext;
    NETWORK_EffTransportPeer *m_pPrevious;
    int m_cNewPortRequests;
    int m_nReserved1C;
    int m_fClosed;
    int m_fConnectComplete;
    DWORD m_dwConnectStartTick;
};

struct NETWORK_RenderDispatchQueue {
    void *m_pVtable;
    int m_nReserved04;
    void *m_pLockVtable;
    char m_abCriticalSection[0x18];
    int m_cEntryCapacity;
    int m_cQueuedEntries;
    int m_cClients;
    int m_nReserved30;
    int m_nReserved34;
    int m_nReserved38;
    int m_nReserved3C;
    int m_cEntriesDropped;
    void *m_pEntryBuffer;
    void *m_pEntryBufferEnd;
    void *m_pHead;
    void *m_pTail;
    void *m_pClientList;
};

struct NETWORK_EffTransportRuntimeState {
    NETWORK_RenderQueueNode m_RenderQueueNode;
    int m_nRuntimeKey;
    int m_fShutdownRequested;
    int m_fRuntimeActive;
    int m_fTransportInitialized;
    int m_fStartRequested;
    void *m_pActiveChannelOwner;
    void *m_pFirstPeer;
    void *m_pLastPeer;
    int m_nReserved30;
    int m_nReserved34;
    int m_nReserved38;
    int m_nReserved3C;
    int m_nReserved40;
    int m_nReserved44;
    int m_nReserved48;
    int m_nPrimaryHandleGroupCount;
    int m_nPrimaryHandleGroupBytes;
    int m_nPrimaryHandleGroupMode;
    int m_nSecondaryHandleGroupBytes;
    int m_cbMaxPacketBytes;
};

struct NETWORK_RuntimeWindowBase {
    char m_abWindowBase[0x10];
    NETWORK_EffTransportRuntimeState m_RuntimeState;
};

struct NETWORK_FileBasedRuntimeWindow {
    NETWORK_RuntimeWindowBase m_Base;
    int m_nReserved78;
};

char *g_pszFileBasedNetworkLocalHostName = 0;
char *g_pszFileBasedNetworkConfiguredPath = 0;
int g_fFileBasedNetworkPathConfigured = 0;
short g_nEffTransportServiceBasePort = 0;
int g_nEffTransportAsyncErrorStatus = 0;
int g_cbEffTransportMaxPacketBytes = 0;
void *g_pEffTransportRequestConnectControlStream = 0;
void *g_pEffTransportRequestNewPortControlStream = 0;
void *g_pEffTransportAuthoriseConnectControlStream = 0;
void *g_pEffTransportGoAheadConnectControlStream = 0;
void *g_pEffTransportFailedConnectControlStream = 0;
void *g_pEffTransportGlobalWriteStream = 0;
void *g_pEffTransportGlobalReadStream = 0;
void *g_pEffTransportRuntimeService = 0;
int g_fFileBasedRuntimeWindowClassRegistered = 0;
int g_fTcpipRuntimeWindowClassRegistered = 0;
DWORD g_dwFileBasedNetworkThreadId = 0;
DWORD g_dwTcpipNetworkThreadId = 0;
HANDLE g_hFileBasedNetworkThread = 0;
HANDLE g_hTcpipNetworkThread = 0;

extern "C" DWORD timeGetTime(void);

extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pEffTransportDispatchQueue;
extern void *g_pEffTransportSecondaryDispatchQueue;
extern void InitializeRenderQueueNodeBase(void *pRenderQueueNode);
extern void *ConstructRenderDispatchQueue(void *pQueue, int cEntries);
extern void RegisterOrderedRenderDispatchClient(void *pDispatchQueue, void *pClient, int nOrder);
extern int DrainRenderDispatchQueueEntries(void *pDispatchQueue, unsigned int cEntries);
extern void ResetEffStreamStateFields(void *pEffStreamSubobject);
extern void BeginEffStreamWriteSession(void *pObject);
extern void ResetEffTransportHandleGroup(int *pHandleGroup);
extern void CloseEffTransportPeer(int nPeer);
extern void ConfigureEffStreamPrimaryHandleGroup(void *pStream, int cHandles, int cbHandleData, int nMode);
extern void ReleaseTimedEffStreamPrimaryHandle(int nTimedStream);
extern void ConfigureEffStreamSecondaryHandleGroup(void *pStream, int cbHandleData, int nMode);
extern void ConfigureTimedEffStreamSecondaryHandle(void *pTimedStream, int cbHandleData);
extern void *g_pEffTransportPeerAddressState;
extern void *g_pEffTransportPacketBuffer;
extern void ReleaseGlobalEffTransportBuffer(void *pObject);
extern void EndEffStreamWriteSession(int nStream);
extern void LoadEffStreamFromGlobalRange(void *pStream);
extern int SendEffStreamPayloadWithTransportHeader(void *pPayloadSender, int nStream);
extern int LoadEffStreamFromMemory(void *pStream, int nSourceBuffer);
void UnlinkAndDeleteEffTransportPeer(void *pRuntimeState, int nPeer);
void MarkEffTransportPeerActivityTime(int nPeer);

// FUNCTION: LEMBALL 0x00460A40
void ClearEffTransportSendGate(int nOwner) {
    ((NETWORK_ChannelOwnerObject *)(unsigned long)nOwner)->m_nSendGate = 0;
}

// FUNCTION: LEMBALL 0x00460A50
void SendLoadedEffEventToPeer(void *pOwner, void *pPeerKey, void *pStream) {
    NETWORK_ChannelOwnerObject *pChannelOwner;
    NETWORK_PeerPayloadSenderSlot *pPayloadSenderSlot;
    NETWORK_PeerPayloadSender *pPayloadSender;

    LoadEffStreamFromGlobalRange(pStream);
    pChannelOwner = (NETWORK_ChannelOwnerObject *)pOwner;
    pPayloadSenderSlot =
        (NETWORK_PeerPayloadSenderSlot *)((char *)pChannelOwner + pChannelOwner->m_pOffsets->m_nPayloadSenderSlotOffset);
    pPayloadSender = &pPayloadSenderSlot->m_Sender;
    pPayloadSender->m_pVtable->m_pWritePeerKey(pPeerKey);
    SendEffStreamPayloadWithTransportHeader(pPayloadSender, (int)(unsigned long)pStream);
}

// FUNCTION: LEMBALL 0x00460C60
void SetEffTransportPeerNameAndPort(void *pPeer, char *pszName, void *pKey, short nPort) {
    NETWORK_EffTransportPeer *pTransportPeer;
    NETWORK_PeerPayloadSenderSlot *pPayloadSenderSlot;
    NETWORK_PeerPayloadSender *pPayloadSender;
    char chTerminator;
    unsigned int cchName;
    unsigned int cDwords;
    char *pszSource;
    char *pszDest;

    pTransportPeer = (NETWORK_EffTransportPeer *)pPeer;
    cchName = 0xffffffffU;
    pszSource = pszName;
    do {
        if (cchName == 0) {
            break;
        }
        --cchName;
        chTerminator = *pszSource;
        ++pszSource;
    } while (chTerminator != '\0');

    pTransportPeer->m_pszPeerName = (char *)AllocateVSMemBlock(~cchName);

    cchName = 0xffffffffU;
    do {
        if (cchName == 0) {
            break;
        }
        pszDest = pszSource;
        --cchName;
        pszDest = pszSource + 1;
        chTerminator = *pszSource;
        pszSource = pszDest;
    } while (chTerminator != '\0');

    cchName = ~cchName;
    pszSource = pszDest - cchName;
    pszDest = pTransportPeer->m_pszPeerName;
    cDwords = cchName >> 2;
    while (cDwords != 0) {
        *(unsigned int *)pszDest = *(const unsigned int *)pszSource;
        pszSource += 4;
        pszDest += 4;
        --cDwords;
    }

    cchName &= 3;
    while (cchName != 0) {
        *pszDest++ = *pszSource++;
        --cchName;
    }

    pPayloadSenderSlot =
        (NETWORK_PeerPayloadSenderSlot *)((char *)pTransportPeer + pTransportPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
    pPayloadSender = &pPayloadSenderSlot->m_Sender;
    pPayloadSender->m_pVtable->m_pSetAssignedPort(nPort);
    pPayloadSender->m_pVtable->m_pWritePeerKey(pKey);
}

// FUNCTION: LEMBALL 0x00460D60
void FreeEffTransportPeerBuffers(int nPeer) {
    NETWORK_EffTransportPeer *pPeer;
    NETWORK_TransportEndpointSlot *pEndpointSlot;

    pPeer = (NETWORK_EffTransportPeer *)(unsigned long)nPeer;
    if (pPeer->m_pszPeerName != 0) {
        FreeVSMemBlock(pPeer->m_pszPeerName);
        pPeer->m_pszPeerName = 0;
    }
    if (pPeer->m_pszPeerAddress != 0) {
        FreeVSMemBlock(pPeer->m_pszPeerAddress);
        pPeer->m_pszPeerAddress = 0;
    }

    pEndpointSlot = (NETWORK_TransportEndpointSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nEndpointStateSlotOffset);
    ResetEffTransportHandleGroup((int *)&pEndpointSlot->m_State);
}

// FUNCTION: LEMBALL 0x00460CE0
int IsEffTransportPeerStillConnecting(int nPeer) {
    NETWORK_EffTransportPeer *pPeer;
    DWORD dwNow;

    pPeer = (NETWORK_EffTransportPeer *)(unsigned long)nPeer;
    if (pPeer->m_fConnectComplete == 0) {
        dwNow = timeGetTime();
        if (4000 < dwNow - pPeer->m_dwConnectStartTick) {
            CloseEffTransportPeer(nPeer);
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00460D10
void MarkEffTransportPeerActivityTime(int nPeer) {
    ((NETWORK_EffTransportPeer *)(unsigned long)nPeer)->m_dwConnectStartTick = timeGetTime();
}

// FUNCTION: LEMBALL 0x00462130
int MarkExistingEffTransportPeerActive(void *pRuntimeState, int nPeer) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_EffTransportPeer *pPeer;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    pPeer = (NETWORK_EffTransportPeer *)pState->m_pFirstPeer;
    while (pPeer != 0) {
        if ((int)(unsigned long)pPeer == nPeer) {
            if (IsEffTransportPeerStillConnecting((int)(unsigned long)pPeer) == 0) {
                return 0;
            }
            if (pPeer->m_fClosed == 0) {
                MarkEffTransportPeerActivityTime((int)(unsigned long)pPeer);
                return 1;
            }
            return 0;
        }
        pPeer = pPeer->m_pNext;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00462180
int FindMatchingActiveEffTransportPeer(void *pRuntimeState, void *pPeerKey) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_EffTransportPeer *pPeer;
    NETWORK_PeerPayloadSenderSlot *pPayloadSenderSlot;
    NETWORK_TransportPeerMatcher *pPeerMatcher;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    pPeer = (NETWORK_EffTransportPeer *)pState->m_pFirstPeer;
    while (pPeer != 0) {
        if (pPeer->m_fClosed == 0) {
            pPayloadSenderSlot =
                (NETWORK_PeerPayloadSenderSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
            pPeerMatcher = pPayloadSenderSlot->m_pMatcher;
            if (((NETWORK_TransportPeerMatcherVtable *)pPeerMatcher->m_pVtable)->m_pMatchPeerKey(pPeerKey) != 0) {
                break;
            }
        }
        pPeer = pPeer->m_pNext;
    }
    return (int)(unsigned long)pPeer;
}

// FUNCTION: LEMBALL 0x00462100
int AllocateEffTransportPeerEntry(void *pRuntimeState) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_ChannelOwnerObject *pOwner;
    NETWORK_EffTransportPeer *pPeer;
    NETWORK_EffTransportPeer *pNextPeer;
    NETWORK_HandleGroupAdjustorSlot *pAdjustorSlot;
    NETWORK_HandleGroupAdjustor *pAdjustor;
    int nSecondaryHandleBytes;
    int fPeerListLocked;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    pOwner = (NETWORK_ChannelOwnerObject *)pState->m_pActiveChannelOwner;
    fPeerListLocked = 0;

    pPeer = (NETWORK_EffTransportPeer *)pState->m_pFirstPeer;
    while (pPeer != 0) {
        pNextPeer = pPeer->m_pNext;
        if (pPeer->m_fClosed != 0) {
            if (fPeerListLocked == 0) {
                fPeerListLocked = 1;
                pOwner->m_pVtable->m_pLockPeerList(pRuntimeState);
            }
            UnlinkAndDeleteEffTransportPeer(pRuntimeState, (int)(unsigned long)pPeer);
        }
        pPeer = pNextPeer;
    }

    if (fPeerListLocked != 0) {
        pOwner->m_pVtable->m_pUnlockPeerList(pRuntimeState);
    }

    pPeer = (NETWORK_EffTransportPeer *)(unsigned long)pOwner->m_pVtable->m_pAllocatePeer(pRuntimeState);
    if (pState->m_pFirstPeer == 0) {
        pState->m_pFirstPeer = pPeer;
    } else {
        ((NETWORK_EffTransportPeer *)pState->m_pLastPeer)->m_pNext = pPeer;
    }

    pPeer->m_pPrevious = (NETWORK_EffTransportPeer *)pState->m_pLastPeer;
    pState->m_pLastPeer = pPeer;

    pAdjustorSlot =
        (NETWORK_HandleGroupAdjustorSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nHandleGroupAdjustorSlotOffset);
    pAdjustor = &pAdjustorSlot->m_Adjustor;
    ConfigureEffStreamPrimaryHandleGroup((char *)pAdjustor + pAdjustor->m_pOffsets->m_nHandleStreamOffset,
                                         pState->m_nPrimaryHandleGroupCount,
                                         pState->m_nPrimaryHandleGroupBytes,
                                         pState->m_nPrimaryHandleGroupMode);
    ReleaseTimedEffStreamPrimaryHandle(
        (int)(unsigned long)((char *)pAdjustor + pAdjustor->m_pOffsets->m_nTimedStreamOffset));

    nSecondaryHandleBytes = pState->m_nSecondaryHandleGroupBytes;
    pAdjustorSlot =
        (NETWORK_HandleGroupAdjustorSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nHandleGroupAdjustorSlotOffset);
    pAdjustor = &pAdjustorSlot->m_Adjustor;
    ConfigureEffStreamSecondaryHandleGroup((char *)pAdjustor + pAdjustor->m_pOffsets->m_nHandleStreamOffset,
                                           nSecondaryHandleBytes,
                                           pState->m_nPrimaryHandleGroupMode);
    ConfigureTimedEffStreamSecondaryHandle((char *)pAdjustor + pAdjustor->m_pOffsets->m_nTimedStreamOffset,
                                           nSecondaryHandleBytes);
    return (int)(unsigned long)pPeer;
}

// FUNCTION: LEMBALL 0x00461FC0
void UnlinkAndDeleteEffTransportPeer(void *pRuntimeState, int nPeer) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_ChannelOwnerObject *pOwner;
    NETWORK_EffTransportPeer *pHeadPeer;
    NETWORK_EffTransportPeer *pPeer;
    NETWORK_EffTransportPeer *pNextPeer;
    NETWORK_EffTransportPeer *pPreviousPeer;
    NETWORK_TransportEndpointSlot *pEndpointSlot;
    NETWORK_EndpointState *pEndpoint;
    NETWORK_PeerPayloadSenderSlot *pPayloadSenderSlot;
    NETWORK_DeleteObject *pDeletePeer;
    DWORD dwReleasedPortKey;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    pHeadPeer = (NETWORK_EffTransportPeer *)pState->m_pFirstPeer;
    pPeer = pHeadPeer;
    if (pPeer == 0) {
        return;
    }

    while ((int)(unsigned long)pPeer != nPeer) {
        pPeer = pPeer->m_pNext;
        if (pPeer == 0) {
            return;
        }
    }

    pNextPeer = pPeer->m_pNext;
    pPreviousPeer = pPeer->m_pPrevious;
    if (pState->m_pLastPeer == pPeer) {
        pState->m_pLastPeer = pPreviousPeer;
    }
    if (pPeer == pHeadPeer) {
        pState->m_pFirstPeer = pNextPeer;
    }

    pOwner = (NETWORK_ChannelOwnerObject *)pState->m_pActiveChannelOwner;
    pEndpointSlot = (NETWORK_TransportEndpointSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nEndpointStateSlotOffset);
    pEndpoint = &pEndpointSlot->m_State;
    dwReleasedPortKey = ((DWORD)(unsigned long)pPeer->m_pOffsets & 0xffff0000UL) |
                        (unsigned short)pEndpoint->m_nAssignedPort;
    pOwner->m_pVtable->m_pReleasePort(dwReleasedPortKey);

    FreeEffTransportPeerBuffers((int)(unsigned long)pPeer);
    if (pPeer != 0) {
        pPayloadSenderSlot =
            (NETWORK_PeerPayloadSenderSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
        pDeletePeer = (NETWORK_DeleteObject *)&pPayloadSenderSlot->m_Sender;
        pDeletePeer->m_pVtable->m_pDelete(1);
    }

    if (pPreviousPeer != 0) {
        pPreviousPeer->m_pNext = pNextPeer;
    }
    if (pNextPeer != 0) {
        pNextPeer->m_pPrevious = pPreviousPeer;
    }
}

// FUNCTION: LEMBALL 0x00462220
void HandleEffTransportRequestConnect(void *pRuntimeState, void *pPeerKey) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_EffTransportPeer *pPeer;
    NETWORK_TransportEndpointSlot *pEndpointSlot;
    NETWORK_EndpointState *pPeerTransportState;
    NETWORK_RequestConnectControlStream *pRequestConnectStream;
    NETWORK_AuthoriseConnectControlStream *pAuthoriseConnectStream;
    short nAssignedPort;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    pRequestConnectStream = (NETWORK_RequestConnectControlStream *)g_pEffTransportRequestConnectControlStream;
    pAuthoriseConnectStream = (NETWORK_AuthoriseConnectControlStream *)g_pEffTransportAuthoriseConnectControlStream;
    pPeer = (NETWORK_EffTransportPeer *)(unsigned long)FindMatchingActiveEffTransportPeer(pRuntimeState, pPeerKey);
    if (pPeer != 0 && pPeer->m_fClosed == 0 &&
        IsEffTransportPeerStillConnecting((int)(unsigned long)pPeer) != 0) {
        return;
    }

    pPeer = (NETWORK_EffTransportPeer *)(unsigned long)AllocateEffTransportPeerEntry(pRuntimeState);
    nAssignedPort = ((NETWORK_ChannelOwnerObject *)pState->m_pActiveChannelOwner)->m_pVtable->m_pAllocatePort(
        pRequestConnectStream->m_pPortBinding);
    if (nAssignedPort != -1) {
        SetEffTransportPeerNameAndPort(pPeer, pRequestConnectStream->m_pszHostName, pPeerKey, nAssignedPort);
        pEndpointSlot = (NETWORK_TransportEndpointSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nEndpointStateSlotOffset);
        pPeerTransportState = &pEndpointSlot->m_State;
        pAuthoriseConnectStream->m_nAssignedPort = pPeerTransportState->m_nAssignedPort;
        pAuthoriseConnectStream->m_pPeer = pPeer;
        SendLoadedEffEventToPeer(pState->m_pActiveChannelOwner, pPeerKey, pAuthoriseConnectStream);
        return;
    }

    CloseEffTransportPeer((int)(unsigned long)pPeer);
}

// FUNCTION: LEMBALL 0x00462300
void HandleEffTransportRequestNewPort(void *pRuntimeState, void *pPeerKey) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_ChannelOwnerObject *pOwner;
    NETWORK_EffTransportPeer *pPeer;
    NETWORK_TransportEndpointSlot *pEndpointSlot;
    NETWORK_EndpointState *pPeerTransportState;
    NETWORK_PeerPayloadSenderSlot *pPayloadSenderSlot;
    NETWORK_PeerPayloadSender *pPayloadSender;
    NETWORK_RequestNewPortControlStream *pRequestNewPortStream;
    NETWORK_AuthoriseConnectControlStream *pAuthoriseConnectStream;
    NETWORK_FailedConnectControlStream *pFailedConnectStream;
    short nAssignedPort;
    int cNewPortRequests;
    DWORD dwReleasedPortKey;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    pOwner = (NETWORK_ChannelOwnerObject *)pState->m_pActiveChannelOwner;
    pRequestNewPortStream = (NETWORK_RequestNewPortControlStream *)g_pEffTransportRequestNewPortControlStream;
    pAuthoriseConnectStream = (NETWORK_AuthoriseConnectControlStream *)g_pEffTransportAuthoriseConnectControlStream;
    pFailedConnectStream = (NETWORK_FailedConnectControlStream *)g_pEffTransportFailedConnectControlStream;
    pPeer = pRequestNewPortStream->m_pPeer;
    if (MarkExistingEffTransportPeerActive(pRuntimeState, (int)(unsigned long)pPeer) == 0) {
        return;
    }

    pEndpointSlot = (NETWORK_TransportEndpointSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nEndpointStateSlotOffset);
    pPeerTransportState = &pEndpointSlot->m_State;
    dwReleasedPortKey = ((DWORD)(unsigned long)pPeer->m_pOffsets & 0xffff0000UL) |
                        (unsigned short)pPeerTransportState->m_nAssignedPort;
    pOwner->m_pVtable->m_pReleasePort(dwReleasedPortKey);

    cNewPortRequests = pPeer->m_cNewPortRequests + 1;
    pPeer->m_cNewPortRequests = cNewPortRequests;
    if (5 < cNewPortRequests) {
        pFailedConnectStream->m_pszFailureMessage = (char *)0x004A1F44;
        SendLoadedEffEventToPeer(pState->m_pActiveChannelOwner, pPeerKey, pRequestNewPortStream);
        return;
    }

    nAssignedPort = pOwner->m_pVtable->m_pAllocatePort(pRequestNewPortStream->m_Base.m_pPortBinding);
    if (nAssignedPort != -1) {
        pPayloadSenderSlot =
            (NETWORK_PeerPayloadSenderSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
        pPayloadSender = &pPayloadSenderSlot->m_Sender;
        pPayloadSender->m_pVtable->m_pSetAssignedPort(nAssignedPort);
        pAuthoriseConnectStream->m_nAssignedPort = pPeerTransportState->m_nAssignedPort;
        pAuthoriseConnectStream->m_pPeer = pPeer;
        SendLoadedEffEventToPeer(pState->m_pActiveChannelOwner, pPeerKey, pAuthoriseConnectStream);
    }
}

// FUNCTION: LEMBALL 0x00462340
void HandleEffTransportAuthoriseConnect(void *pRuntimeState, void *pPeerKey) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_RuntimeServiceObject *pRuntimeService;
    NETWORK_ChannelOwnerObject *pOwner;
    NETWORK_EffTransportPeer *pPeer;
    NETWORK_EffTransportPeer *pNewPeer;
    NETWORK_RequestNewPortControlStream *pRequestNewPortStream;
    NETWORK_AuthoriseConnectControlStream *pAuthoriseConnectStream;
    NETWORK_AuthoriseConnectControlStream *pGoAheadConnectStream;
    NETWORK_PeerPayloadSenderSlot *pPayloadSenderSlot;
    NETWORK_PeerPayloadSender *pPayloadSender;
    unsigned char *pbAssignedPort;
    short nAssignedPort;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    pRuntimeService = (NETWORK_RuntimeServiceObject *)g_pEffTransportRuntimeService;
    pOwner = (NETWORK_ChannelOwnerObject *)pState->m_pActiveChannelOwner;
    pRequestNewPortStream = (NETWORK_RequestNewPortControlStream *)g_pEffTransportRequestNewPortControlStream;
    pAuthoriseConnectStream = (NETWORK_AuthoriseConnectControlStream *)g_pEffTransportAuthoriseConnectControlStream;
    pGoAheadConnectStream = (NETWORK_AuthoriseConnectControlStream *)g_pEffTransportGoAheadConnectControlStream;

    pPeer = (NETWORK_EffTransportPeer *)(unsigned long)FindMatchingActiveEffTransportPeer(pRuntimeState, pPeerKey);
    if (pPeer != 0 && pPeer->m_fClosed == 0 &&
        IsEffTransportPeerStillConnecting((int)(unsigned long)pPeer) != 0) {
        if (pRuntimeService->m_pVtable->m_pValidatePeerKey(pPeerKey) != 0) {
            return;
        }
        CloseEffTransportPeer((int)(unsigned long)pPeer);
    }

    if (pState->m_nReserved30 != 0) {
        ClearEffTransportSendGate((int)(unsigned long)pOwner);
    }

    nAssignedPort = pAuthoriseConnectStream->m_nAssignedPort;
    pbAssignedPort = pOwner->m_pbAllocatedPorts + nAssignedPort;
    if (*pbAssignedPort != 0) {
        pRequestNewPortStream->m_pPeer = pAuthoriseConnectStream->m_pPeer;
        pRequestNewPortStream->m_Base.m_nBasePort = g_nEffTransportServiceBasePort;
        pRequestNewPortStream->m_Base.m_pPortBinding = pOwner->m_pbAllocatedPorts;
        pRequestNewPortStream->m_Base.m_pszHostName = g_pszFileBasedNetworkLocalHostName;
        SendLoadedEffEventToPeer(pOwner, pPeerKey, pRequestNewPortStream);
        return;
    }

    *pbAssignedPort = 1;
    pNewPeer = (NETWORK_EffTransportPeer *)(unsigned long)AllocateEffTransportPeerEntry(pRuntimeState);
    pPayloadSenderSlot =
        (NETWORK_PeerPayloadSenderSlot *)((char *)pNewPeer + pNewPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
    pPayloadSender = &pPayloadSenderSlot->m_Sender;
    pPayloadSender->m_pVtable->m_pSetAssignedPort(nAssignedPort);
    ((NETWORK_EffTransportPeerVtable *)pNewPeer->m_pVtable)->m_pSetPeerKey(pPeerKey);
    pGoAheadConnectStream->m_nAssignedPort = nAssignedPort;
    pGoAheadConnectStream->m_pPeer = pAuthoriseConnectStream->m_pPeer;
    SendLoadedEffEventToPeer(pOwner, pPeerKey, pGoAheadConnectStream);
}

// FUNCTION: LEMBALL 0x00461FA0
void ReleaseEffTransportRuntimeOwnerState(void *pRuntimeState) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_ChannelOwnerObject *pOwner;
    NETWORK_EffTransportPeer *pPeer;
    NETWORK_EffTransportPeer *pNextPeer;
    NETWORK_TransportEndpointSlot *pEndpointSlot;
    NETWORK_EndpointState *pPeerTransportState;
    NETWORK_DeleteObject *pDeletePeer;
    NETWORK_DeleteObject *pDeleteObject;
    NETWORK_PeerPayloadSenderSlot *pPayloadSenderSlot;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    if (pState->m_fTransportInitialized == 0) {
        return;
    }

    pOwner = (NETWORK_ChannelOwnerObject *)pState->m_pActiveChannelOwner;
    pState->m_fTransportInitialized = 0;
    pState->m_fRuntimeActive = 0;

    pPeer = (NETWORK_EffTransportPeer *)pState->m_pFirstPeer;
    if (pPeer != 0) {
        pOwner->m_pVtable->m_pLockPeerList(pRuntimeState);
        while (pPeer != 0) {
            pNextPeer = pPeer->m_pNext;
            pEndpointSlot = (NETWORK_TransportEndpointSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nEndpointStateSlotOffset);
            pPeerTransportState = &pEndpointSlot->m_State;
            FreeEffTransportPeerBuffers((int)(unsigned long)pPeer);
            if (pPeer != 0) {
                pPayloadSenderSlot =
                    (NETWORK_PeerPayloadSenderSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
                pDeletePeer = (NETWORK_DeleteObject *)&pPayloadSenderSlot->m_Sender;
                pDeletePeer->m_pVtable->m_pDelete(1);
            }
            pPeer = pNextPeer;
            if (pPeerTransportState->m_nAssignedPort != -1) {
                pOwner->m_pVtable->m_pReleasePort((DWORD)pPeerTransportState->m_nAssignedPort);
            }
        }
        pOwner->m_pVtable->m_pUnlockPeerList(pRuntimeState);
    }

    if (pState->m_pActiveChannelOwner != 0) {
        ReleaseGlobalEffTransportBuffer(pState->m_pActiveChannelOwner);
        if (pState->m_pActiveChannelOwner != 0) {
            pPayloadSenderSlot =
                (NETWORK_PeerPayloadSenderSlot *)((char *)pState->m_pActiveChannelOwner +
                                                  ((NETWORK_ChannelOwnerObject *)pState->m_pActiveChannelOwner)
                                                      ->m_pOffsets->m_nPayloadSenderSlotOffset);
            pDeleteObject = (NETWORK_DeleteObject *)&pPayloadSenderSlot->m_Sender;
            pDeleteObject->m_pVtable->m_pDelete(1);
        }
    }

    if (g_pEffTransportPacketBuffer != 0) {
        FreeVSMemBlock(g_pEffTransportPacketBuffer);
    }
    g_pEffTransportPacketBuffer = 0;
    if (g_pEffTransportPeerAddressState != 0) {
        FreeVSMemBlock(g_pEffTransportPeerAddressState);
    }
    g_pEffTransportPeerAddressState = 0;

    if (g_pEffTransportGlobalReadStream != 0) {
        ((NETWORK_DeleteObject *)g_pEffTransportGlobalReadStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportGlobalReadStream = 0;
    if (g_pEffTransportGlobalWriteStream != 0) {
        ((NETWORK_DeleteObject *)g_pEffTransportGlobalWriteStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportGlobalWriteStream = 0;
    if (g_pEffTransportRequestConnectControlStream != 0) {
        ((NETWORK_DeleteObject *)g_pEffTransportRequestConnectControlStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportRequestConnectControlStream = 0;
    if (g_pEffTransportRequestNewPortControlStream != 0) {
        ((NETWORK_DeleteObject *)g_pEffTransportRequestNewPortControlStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportRequestNewPortControlStream = 0;
    if (g_pEffTransportAuthoriseConnectControlStream != 0) {
        ((NETWORK_DeleteObject *)g_pEffTransportAuthoriseConnectControlStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportAuthoriseConnectControlStream = 0;
    if (g_pEffTransportGoAheadConnectControlStream != 0) {
        ((NETWORK_DeleteObject *)g_pEffTransportGoAheadConnectControlStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportGoAheadConnectControlStream = 0;
    if (g_pEffTransportFailedConnectControlStream != 0) {
        ((NETWORK_DeleteObject *)g_pEffTransportFailedConnectControlStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportFailedConnectControlStream = 0;
    pOwner->m_pVtable->m_pShutdown(pRuntimeState);
}

// FUNCTION: LEMBALL 0x00460E90
int SendEffStreamToActivePeer(void *pPeer, int *pStream) {
    NETWORK_EffTransportPeer *pTransportPeer;
    NETWORK_TransportEndpointSlot *pEndpointSlot;
    NETWORK_EndpointState *pPeerTransportState;
    NETWORK_PeerPayloadSenderSlot *pPayloadSenderSlot;
    NETWORK_PeerPayloadSender *pPayloadSender;
    NETWORK_EffDispatchEvent kFailureEvent;
    int fWriteSessionOpen;
    int nResult;

    pTransportPeer = (NETWORK_EffTransportPeer *)pPeer;
    pEndpointSlot =
        (NETWORK_TransportEndpointSlot *)((char *)pTransportPeer + pTransportPeer->m_pOffsets->m_nEndpointStateSlotOffset);
    pPeerTransportState = &pEndpointSlot->m_State;
    if (pPeerTransportState->m_fSecondaryHandleActive != 0 && pTransportPeer->m_fClosed == 0) {
        fWriteSessionOpen = pStream[4];
        if (fWriteSessionOpen < 1) {
            BeginEffStreamWriteSession(pStream);
        }

        pPayloadSenderSlot =
            (NETWORK_PeerPayloadSenderSlot *)((char *)pTransportPeer +
                                              pTransportPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
        pPayloadSender = &pPayloadSenderSlot->m_Sender;
        nResult = SendEffStreamPayloadWithTransportHeader(pPayloadSender, (int)(unsigned long)pStream);
        if (nResult == 0) {
            kFailureEvent.m_nType = 1;
            kFailureEvent.m_nCode = 0xc;
            if (pStream[9] == 0) {
                kFailureEvent.m_nCode = 0xb;
            }
            kFailureEvent.m_pStream = pStream;
            kFailureEvent.m_pPeer = pPeer;
            ((void (*)(NETWORK_EffDispatchEvent *))(*(void ***)g_pEffTransportDispatchQueue)[2])(&kFailureEvent);
        }

        if (fWriteSessionOpen < 1) {
            EndEffStreamWriteSession((int)(unsigned long)pStream);
        }
        return nResult;
    }

    pStream[10] = 0;
    return 0;
}

// FUNCTION: LEMBALL 0x00462460
void ForwardEffTransportFailedConnect(void *pRuntimeState, void *pPeerKey) {
    NETWORK_EffTransportRuntimeState *pState;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    SendLoadedEffEventToPeer(pState->m_pActiveChannelOwner, pPeerKey, g_pEffTransportFailedConnectControlStream);
}

// FUNCTION: LEMBALL 0x00462480
void HandleEffTransportGoAheadConnect(void *pRuntimeState) {
    NETWORK_EffTransportPeer *pPeer;

    pPeer = ((NETWORK_AuthoriseConnectControlStream *)g_pEffTransportGoAheadConnectControlStream)->m_pPeer;
    if (MarkExistingEffTransportPeerActive(pRuntimeState, (int)(unsigned long)pPeer) != 0) {
        ((NETWORK_EffTransportPeerVtable *)pPeer->m_pVtable)->m_pCompleteConnect();
    }
}

// FUNCTION: LEMBALL 0x004624C0
void DispatchEffTransportConnectControlStream(void *pRuntimeState, void *pPeerKey, int nSourceBuffer) {
    ((void (*)(void))**(void ***)pPeerKey)();
    if (LoadEffStreamFromMemory(g_pEffTransportRequestConnectControlStream, nSourceBuffer) != 0) {
        HandleEffTransportRequestConnect(pRuntimeState, pPeerKey);
        return;
    }
    if (LoadEffStreamFromMemory(g_pEffTransportRequestNewPortControlStream, nSourceBuffer) != 0) {
        HandleEffTransportRequestNewPort(pRuntimeState, pPeerKey);
        return;
    }
    if (LoadEffStreamFromMemory(g_pEffTransportAuthoriseConnectControlStream, nSourceBuffer) != 0) {
        HandleEffTransportAuthoriseConnect(pRuntimeState, pPeerKey);
        return;
    }
    if (LoadEffStreamFromMemory(g_pEffTransportGoAheadConnectControlStream, nSourceBuffer) != 0) {
        HandleEffTransportGoAheadConnect(pRuntimeState);
        return;
    }
    if (LoadEffStreamFromMemory(g_pEffTransportFailedConnectControlStream, nSourceBuffer) != 0) {
        ForwardEffTransportFailedConnect(pRuntimeState, pPeerKey);
    }
}

// FUNCTION: LEMBALL 0x00462730
int BroadcastEffStreamToActivePeers(void *pRuntimeState, int *pStream) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_EffTransportPeer *pPeer;
    int fSendBlocked;
    int cActivePeers;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    fSendBlocked = 0;
    cActivePeers = 0;
    pPeer = (NETWORK_EffTransportPeer *)pState->m_pFirstPeer;

    while (1) {
        if (pPeer == 0) {
            if (fSendBlocked == 0 && 0 < cActivePeers) {
                return 1;
            }
            return 0;
        }

        if (pPeer->m_fClosed == 0) {
            ++cActivePeers;
            if (fSendBlocked == 0) {
                if (SendEffStreamToActivePeer(pPeer, pStream) != 0) {
                    fSendBlocked = 0;
                    pPeer = pPeer->m_pNext;
                    continue;
                }
                fSendBlocked = 1;
            }
        }

        pPeer = pPeer->m_pNext;
    }
}

// FUNCTION: LEMBALL 0x0045F3D0
void *ConstructEffTransportRequestConnectControlStream(void *pObject, char *pszStreamName) {
    NETWORK_RequestConnectControlStream *pStream;
    unsigned int cbName;

    pStream = (NETWORK_RequestConnectControlStream *)pObject;
    pStream->m_Header.m_pVtable = g_NETWORK_ReturnTrueVtable;
    pStream->m_Header.m_nEventCode = 0;
    ResetEffStreamStateFields(pObject);
    pStream->m_Header.m_pVtable = g_NETWORK_RequestConnectControlStreamNameVtable;
    pStream->m_Header.m_pszStreamName = pszStreamName;

    cbName = (unsigned int)strlen(pszStreamName) + 1;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + cbName;

    pStream->m_Header.m_pVtable = g_NETWORK_RequestConnectControlStreamVtable;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + 0x3c;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + 0x101;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + 0x200;
    return pObject;
}

// FUNCTION: LEMBALL 0x0045F490
void *ConstructEffTransportRequestNewPortControlStream(void *pObject, char *pszStreamName) {
    NETWORK_RequestNewPortControlStream *pStream;

    ConstructEffTransportRequestConnectControlStream(pObject, pszStreamName);
    pStream = (NETWORK_RequestNewPortControlStream *)pObject;
    pStream->m_Base.m_Header.m_pVtable = g_NETWORK_RequestNewPortControlStreamVtable;
    pStream->m_Base.m_Header.m_cbSerializedLength = pStream->m_Base.m_Header.m_cbSerializedLength + 0x10;
    return pObject;
}

// FUNCTION: LEMBALL 0x0045F4F0
void *ConstructEffTransportAuthoriseConnectControlStream(void *pObject, char *pszStreamName) {
    NETWORK_AuthoriseConnectControlStream *pStream;
    unsigned int cbName;

    pStream = (NETWORK_AuthoriseConnectControlStream *)pObject;
    pStream->m_Header.m_pVtable = g_NETWORK_ReturnTrueVtable;
    pStream->m_Header.m_nEventCode = 0;
    ResetEffStreamStateFields(pObject);
    pStream->m_Header.m_pVtable = g_NETWORK_RequestConnectControlStreamNameVtable;
    pStream->m_Header.m_pszStreamName = pszStreamName;

    cbName = (unsigned int)strlen(pszStreamName) + 1;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + cbName;

    pStream->m_Header.m_pVtable = g_NETWORK_AuthoriseConnectControlStreamVtable;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + 0x38;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + 0x101;
    return pObject;
}

// FUNCTION: LEMBALL 0x0045F580
void *ConstructEffTransportGoAheadConnectControlStream(void *pObject, char *pszStreamName) {
    NETWORK_AuthoriseConnectControlStream *pStream;
    unsigned int cbName;

    pStream = (NETWORK_AuthoriseConnectControlStream *)pObject;
    pStream->m_Header.m_pVtable = g_NETWORK_ReturnTrueVtable;
    pStream->m_Header.m_nEventCode = 0;
    ResetEffStreamStateFields(pObject);
    pStream->m_Header.m_pVtable = g_NETWORK_RequestConnectControlStreamNameVtable;
    pStream->m_Header.m_pszStreamName = pszStreamName;

    cbName = (unsigned int)strlen(pszStreamName) + 1;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + cbName;

    pStream->m_Header.m_pVtable = g_NETWORK_GoAheadConnectControlStreamVtable;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + 0x38;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + 0x101;
    return pObject;
}

// FUNCTION: LEMBALL 0x0045F610
void *ConstructEffTransportFailedConnectControlStream(void *pObject, char *pszStreamName) {
    NETWORK_FailedConnectControlStream *pStream;
    unsigned int cbName;

    pStream = (NETWORK_FailedConnectControlStream *)pObject;
    pStream->m_Header.m_pVtable = g_NETWORK_ReturnTrueVtable;
    pStream->m_Header.m_nEventCode = 0;
    ResetEffStreamStateFields(pObject);
    pStream->m_Header.m_pVtable = g_NETWORK_RequestConnectControlStreamNameVtable;
    pStream->m_Header.m_pszStreamName = pszStreamName;

    cbName = (unsigned int)strlen(pszStreamName) + 1;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + cbName;

    pStream->m_Header.m_pVtable = g_NETWORK_FailedConnectControlStreamVtable;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + 0x101;
    return pObject;
}

// FUNCTION: LEMBALL 0x004619F0
void *ConstructEffTransportRuntimeState(void *pRuntimeState) {
    NETWORK_EffTransportRuntimeState *pState;
    void *pQueue;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    InitializeRenderQueueNodeBase(pRuntimeState);
    pState->m_RenderQueueNode.m_pVtable = (void **)g_NETWORK_EffTransportRuntimeStateVtable;
    g_nEffTransportAsyncErrorStatus = 0;
    pState->m_pLastPeer = 0;
    pState->m_pFirstPeer = 0;
    pState->m_nReserved34 = 0;
    pState->m_nReserved30 = 0;
    pState->m_fTransportInitialized = 0;
    pState->m_fStartRequested = 0;
    pState->m_nReserved40 = 0;
    pState->m_nReserved3C = 0;
    pState->m_nReserved38 = 0;
    pState->m_nReserved44 = 0;
    pState->m_nReserved48 = 0;
    pState->m_fShutdownRequested = 0;
    pState->m_fRuntimeActive = 0;
    pState->m_cbMaxPacketBytes = 0x50;
    pState->m_pActiveChannelOwner = 0;

    pQueue = AllocateVSMemBlock(0x58);
    if (pQueue == 0) {
        g_pEffTransportDispatchQueue = 0;
    } else {
        g_pEffTransportDispatchQueue = ConstructRenderDispatchQueue(pQueue, 0x1e);
    }
    RegisterOrderedRenderDispatchClient(g_pEffTransportDispatchQueue, pRuntimeState, 0x19);

    pQueue = AllocateVSMemBlock(0x58);
    if (pQueue == 0) {
        g_pEffTransportSecondaryDispatchQueue = 0;
    } else {
        g_pEffTransportSecondaryDispatchQueue = ConstructRenderDispatchQueue(pQueue, 0x1e);
    }

    return pRuntimeState;
}

// FUNCTION: LEMBALL 0x00461BD0
int InitializeEffTransportRuntimeGlobals(void *pRuntimeState) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_EffTransportRuntimeStateVtable *pVtable;
    NETWORK_ChannelOwnerObject *pOwner;
    void *pObject;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    pVtable = (NETWORK_EffTransportRuntimeStateVtable *)pState->m_RenderQueueNode.m_pVtable;
    pState->m_fStartRequested = 0;
    pVtable->m_pInitializeTransport(pRuntimeState);
    if (g_nEffTransportAsyncErrorStatus != 0) {
        return 0;
    }

    pObject = AllocateVSMemBlock(0x3c);
    if (pObject == 0) {
        g_pEffTransportRequestConnectControlStream = 0;
    } else {
        g_pEffTransportRequestConnectControlStream =
            ConstructEffTransportRequestConnectControlStream(pObject, (char *)0x004A1EE8);
    }

    pObject = AllocateVSMemBlock(0x40);
    if (pObject == 0) {
        g_pEffTransportRequestNewPortControlStream = 0;
    } else {
        g_pEffTransportRequestNewPortControlStream =
            ConstructEffTransportRequestNewPortControlStream(pObject, (char *)0x004A1EF8);
    }

    pObject = AllocateVSMemBlock(0x38);
    if (pObject == 0) {
        g_pEffTransportAuthoriseConnectControlStream = 0;
    } else {
        g_pEffTransportAuthoriseConnectControlStream =
            ConstructEffTransportAuthoriseConnectControlStream(pObject, (char *)0x004A1F0C);
    }

    pObject = AllocateVSMemBlock(0x38);
    if (pObject == 0) {
        g_pEffTransportGoAheadConnectControlStream = 0;
    } else {
        g_pEffTransportGoAheadConnectControlStream =
            ConstructEffTransportGoAheadConnectControlStream(pObject, (char *)0x004A1F20);
    }

    pObject = AllocateVSMemBlock(0x34);
    if (pObject == 0) {
        g_pEffTransportFailedConnectControlStream = 0;
    } else {
        g_pEffTransportFailedConnectControlStream =
            ConstructEffTransportFailedConnectControlStream(pObject, (char *)0x004A1F34);
    }

    pObject = AllocateVSMemBlock(0x2c);
    if (pObject == 0) {
        g_pEffTransportGlobalWriteStream = 0;
    } else {
        *(void **)pObject = g_NETWORK_ReturnTrueVtable;
        *(int *)((char *)pObject + 4) = 1;
        ResetEffStreamStateFields(pObject);
        *(void **)pObject = g_NETWORK_EffTransportGlobalWriteStreamVtable;
        BeginEffStreamWriteSession(pObject);
        g_pEffTransportGlobalWriteStream = pObject;
    }

    pObject = AllocateVSMemBlock(0x30);
    if (pObject == 0) {
        g_pEffTransportGlobalReadStream = 0;
    } else {
        *(void **)pObject = g_NETWORK_ReturnTrueVtable;
        *(int *)((char *)pObject + 4) = 2;
        ResetEffStreamStateFields(pObject);
        *(void **)pObject = g_NETWORK_EffTransportGlobalReadStreamVtable;
        *(int *)((char *)pObject + 0x18) = *(int *)((char *)pObject + 0x18) + 4;
        g_pEffTransportGlobalReadStream = pObject;
    }

    pState->m_pActiveChannelOwner = (void *)(unsigned long)pVtable->m_pCreateChannelOwner(pRuntimeState);
    pState->m_fTransportInitialized = 1;
    pState->m_fRuntimeActive = 1;
    pVtable->m_pServiceTransport(pRuntimeState);

    if (pState->m_fRuntimeActive != 0 && g_nEffTransportAsyncErrorStatus == 0) {
        pOwner = (NETWORK_ChannelOwnerObject *)pState->m_pActiveChannelOwner;
        if (pOwner->m_pVtable->m_pBindRuntimeKey(pOwner, pState->m_nRuntimeKey) == 0) {
            pState->m_fRuntimeActive = 0;
            return 0;
        }
        return g_nEffTransportAsyncErrorStatus == 0;
    }

    pState->m_fRuntimeActive = 0;
    return 0;
}

// FUNCTION: LEMBALL 0x00461AA0
int StartEffTransportRuntimeAndWaitReady(void *pRuntimeState, int nRuntimeKey, int cbMaxPacket) {
    NETWORK_EffTransportRuntimeState *pState;
    NETWORK_EffTransportRuntimeStateVtable *pVtable;
    DWORD dwStartTime;
    DWORD dwNow;
    DWORD dwDelayStart;

    pState = (NETWORK_EffTransportRuntimeState *)pRuntimeState;
    pVtable = (NETWORK_EffTransportRuntimeStateVtable *)pState->m_RenderQueueNode.m_pVtable;
    pState->m_nRuntimeKey = nRuntimeKey;
    pState->m_fStartRequested = 1;
    g_cbEffTransportMaxPacketBytes = cbMaxPacket;
    pVtable->m_pServiceTransport(pRuntimeState);

    dwStartTime = timeGetTime();
    if (pState->m_fTransportInitialized == 0) {
        do {
            if (g_nEffTransportAsyncErrorStatus != 0) {
                break;
            }
            dwNow = timeGetTime();
            if (9999 < dwNow - dwStartTime) {
                break;
            }
            pVtable->m_pWaitForTransportMessage(pRuntimeState);
        } while (pState->m_fTransportInitialized == 0);

        if (pState->m_fTransportInitialized == 0) {
            return 0;
        }
    }

    dwStartTime = timeGetTime();
    if (pState->m_fRuntimeActive != 0) {
        while (1) {
            NETWORK_ChannelOwnerObject *pActiveOwner;
            NETWORK_TransportEndpointSlot *pOwnerEndpointSlot;
            NETWORK_EndpointState *pOwnerTransportState;

            pActiveOwner = (NETWORK_ChannelOwnerObject *)pState->m_pActiveChannelOwner;
            if (pActiveOwner != 0) {
                pOwnerEndpointSlot =
                    (NETWORK_TransportEndpointSlot *)((char *)pActiveOwner + pActiveOwner->m_pOffsets->m_nEndpointStateSlotOffset);
                pOwnerTransportState = &pOwnerEndpointSlot->m_State;
                if (pOwnerTransportState->m_fSecondaryHandleActive != 0) {
                    break;
                }
            }

            if (g_nEffTransportAsyncErrorStatus != 0) {
                break;
            }
            dwNow = timeGetTime();
            if (9999 < dwNow - dwStartTime) {
                break;
            }

            dwDelayStart = timeGetTime();
            do {
                dwNow = timeGetTime();
            } while (dwNow - dwDelayStart < 100);

            pVtable->m_pServiceTransport(pRuntimeState);
            if (pState->m_fRuntimeActive == 0) {
                break;
            }
        }
    }

    if (pState->m_fRuntimeActive != 0) {
        NETWORK_ChannelOwnerObject *pActiveOwner;
        NETWORK_TransportEndpointSlot *pOwnerEndpointSlot;
        NETWORK_EndpointState *pOwnerTransportState;

        pActiveOwner = (NETWORK_ChannelOwnerObject *)pState->m_pActiveChannelOwner;
        if (pActiveOwner != 0) {
            pOwnerEndpointSlot =
                (NETWORK_TransportEndpointSlot *)((char *)pActiveOwner + pActiveOwner->m_pOffsets->m_nEndpointStateSlotOffset);
            pOwnerTransportState = &pOwnerEndpointSlot->m_State;
            if (pOwnerTransportState->m_fSecondaryHandleActive != 0 && g_nEffTransportAsyncErrorStatus == 0) {
                return 1;
            }
        }
    }

    pState->m_fShutdownRequested = 1;
    pVtable->m_pServiceTransport(pRuntimeState);

    dwStartTime = timeGetTime();
    do {
        if (pState->m_fTransportInitialized == 0) {
            return 0;
        }
        dwNow = timeGetTime();
    } while (dwNow - dwStartTime < 10000);

    return 0;
}

// FUNCTION: LEMBALL 0x0046F6B0
void *ConstructFileBasedNetworkRuntimeWindow(void *pRuntimeWindow) {
    NETWORK_FileBasedRuntimeWindow *pWindow;

    pWindow = (NETWORK_FileBasedRuntimeWindow *)pRuntimeWindow;
    ((PLATFORM_InvisibleMessageWindow *)pRuntimeWindow)
        ->Construct((const char *)0x004A2334, &g_fFileBasedRuntimeWindowClassRegistered);
    ConstructEffTransportRuntimeState(&pWindow->m_Base.m_RuntimeState);
    *(void **)pWindow = g_NETWORK_FileBasedRuntimeWindowVtable;
    pWindow->m_nReserved78 = 0;
    pWindow->m_Base.m_RuntimeState.m_RenderQueueNode.m_pVtable = (void **)g_NETWORK_FileBasedRuntimeTransportVtable;
    return pRuntimeWindow;
}

// FUNCTION: LEMBALL 0x0046F210
DWORD WINAPI FileBasedNetworkMessageThreadMain(LPVOID pvThreadParam) {
    NETWORK_EffTransportRuntimeState *pActiveRuntime;
    NETWORK_RenderDispatchQueue *pDispatchQueue;
    MSG Msg;
    void *pRuntimeWindow;
    int fRuntimeStarted;

    (void)pvThreadParam;

    pRuntimeWindow = AllocateVSMemBlock(0x7c);
    if (pRuntimeWindow != 0) {
        pRuntimeWindow = ConstructFileBasedNetworkRuntimeWindow(pRuntimeWindow);
    }

    if (pRuntimeWindow == 0) {
        g_pActiveNetworkRuntimeWindow = 0;
    } else {
        g_pActiveNetworkRuntimeWindow = (char *)pRuntimeWindow + 0x10;
    }

    pActiveRuntime = (NETWORK_EffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
    fRuntimeStarted = pActiveRuntime->m_fStartRequested;
    while (fRuntimeStarted == 0 && pActiveRuntime->m_fRuntimeActive == 0 &&
           pActiveRuntime->m_fShutdownRequested == 0) {
        WaitMessage();
        GetMessageA(&Msg, 0, 0, 0);
        fRuntimeStarted = pActiveRuntime->m_fStartRequested;
    }

    if (pActiveRuntime->m_fStartRequested != 0) {
        InitializeEffTransportRuntimeGlobals(g_pActiveNetworkRuntimeWindow);
    }

    fRuntimeStarted = pActiveRuntime->m_fRuntimeActive;
    while (fRuntimeStarted == 0 && pActiveRuntime->m_fShutdownRequested == 0) {
        WaitMessage();
        GetMessageA(&Msg, 0, 0, 0);
        fRuntimeStarted = pActiveRuntime->m_fRuntimeActive;
    }

    if (pActiveRuntime->m_fShutdownRequested == 0) {
        fRuntimeStarted = pActiveRuntime->m_fRuntimeActive;
        while (fRuntimeStarted != 0) {
            WaitMessage();
            if (PeekMessageA(&Msg, 0, 0, 0, 0) != 0) {
                while (PeekMessageA(&Msg, 0, 0, 0, 0) != 0) {
                    GetMessageA(&Msg, 0, 0, 0);
                    TranslateMessage(&Msg);
                    DispatchMessageA(&Msg);
                }
            }

            pDispatchQueue = (NETWORK_RenderDispatchQueue *)g_pEffTransportDispatchQueue;
            while (pDispatchQueue != 0 && pDispatchQueue->m_cQueuedEntries != 0) {
                DrainRenderDispatchQueueEntries(g_pEffTransportDispatchQueue, (unsigned int)pDispatchQueue->m_cQueuedEntries);
            }

            fRuntimeStarted = pActiveRuntime->m_fRuntimeActive;
        }
    }

    if (g_pActiveNetworkRuntimeWindow != 0) {
        ((void (*)(void *, int))(*(void ***)g_pActiveNetworkRuntimeWindow)[1])(g_pActiveNetworkRuntimeWindow, 1);
    }
    g_pActiveNetworkRuntimeWindow = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x0046F3B0
int StartFileBasedNetworkMessageThread(void) {
    DWORD dwStartTime;

    g_hFileBasedNetworkThread =
        CreateThread(0, 0, FileBasedNetworkMessageThreadMain, 0, 0, &g_dwFileBasedNetworkThreadId);
    if (g_hFileBasedNetworkThread == 0) {
        MessageBoxA(0, (const char *)0x004A2274, (const char *)0x004A226C, 0);
        ExitProcess(0xbbbb);
    }

    SetThreadPriority(g_hFileBasedNetworkThread, 2);

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pActiveNetworkRuntimeWindow == 0) {
    }
    if (g_pActiveNetworkRuntimeWindow == 0) {
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A22A4);
        return 0;
    }

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pEffTransportDispatchQueue == 0) {
    }
    if (g_pEffTransportDispatchQueue == 0) {
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A22C8);
        return 0;
    }

    return 1;
}

// FUNCTION: LEMBALL 0x0046F480
int StopFileBasedNetworkMessageThread(void) {
    NETWORK_EffTransportRuntimeState *pActiveRuntime;
    NETWORK_EffTransportRuntimeStateVtable *pVtable;
    DWORD dwStartTime;

    if (g_pActiveNetworkRuntimeWindow == 0) {
        return 0;
    }

    pActiveRuntime = (NETWORK_EffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
    pVtable = (NETWORK_EffTransportRuntimeStateVtable *)pActiveRuntime->m_RenderQueueNode.m_pVtable;
    pActiveRuntime->m_fShutdownRequested = 1;
    pVtable->m_pServiceTransport(g_pActiveNetworkRuntimeWindow);

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pActiveNetworkRuntimeWindow != 0) {
    }
    if (g_pActiveNetworkRuntimeWindow != 0) {
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A22F0);
        return 0;
    }

    return 1;
}

// FUNCTION: LEMBALL 0x0046F4F0
void ConfigureFileBasedNetworkPaths(char *pszLocalHost, char *pszBasePath) {
    size_t cchHost;
    size_t cchPath;
    size_t cchConfiguredPath;
    char szNumber[16];

    cchHost = strlen(pszLocalHost) + 1;
    g_pszFileBasedNetworkLocalHostName = (char *)AllocateVSMemBlock((unsigned int)cchHost);
    memcpy(g_pszFileBasedNetworkLocalHostName, pszLocalHost, cchHost);

    AppendCStringToStream(g_pStatusOutputStream, (const char *)0x004A2308);
    AppendCStringToStream(g_pStatusOutputStream, g_pszFileBasedNetworkLocalHostName);
    AppendCStringToStream(g_pStatusOutputStream, (const char *)0x004A2320);

    g_fFileBasedNetworkPathConfigured = 1;
    cchPath = strlen(pszBasePath);
    g_pszFileBasedNetworkConfiguredPath = (char *)AllocateVSMemBlock((unsigned int)(cchPath + 0xe));
    memcpy(g_pszFileBasedNetworkConfiguredPath, pszBasePath, cchPath + 1);

    if (g_pszFileBasedNetworkConfiguredPath[cchPath - 1] != '\\' &&
        g_pszFileBasedNetworkConfiguredPath[cchPath - 1] != ':') {
        *(unsigned short *)(g_pszFileBasedNetworkConfiguredPath + cchPath) = *(unsigned short *)0x004A2324;
    }

    cchConfiguredPath = strlen(g_pszFileBasedNetworkConfiguredPath);
    *(unsigned int *)(g_pszFileBasedNetworkConfiguredPath + cchConfiguredPath) = *(unsigned int *)0x004A232C;
    *(unsigned short *)(g_pszFileBasedNetworkConfiguredPath + cchConfiguredPath + 4) = *(unsigned short *)0x004A2330;
    g_pszFileBasedNetworkConfiguredPath[cchConfiguredPath + 6] = *(char *)0x004A2332;

    FormatSignedIntToRadixString(0, szNumber, 10);
    strcat(g_pszFileBasedNetworkConfiguredPath, szNumber);
    FormatSignedIntToRadixString(9, szNumber, 10);
    strcat(g_pszFileBasedNetworkConfiguredPath, szNumber);
}

// FUNCTION: LEMBALL 0x0046F7A0
void ConfigureFileBasedNetworkPathsWrapper(char *pszLocalHost, char *pszBasePath) {
    ConfigureFileBasedNetworkPaths(pszLocalHost, pszBasePath);
}

// FUNCTION: LEMBALL 0x004713C0
void *ConstructTcpipNetworkRuntimeWindow(void *pRuntimeWindow) {
    NETWORK_RuntimeWindowBase *pWindow;

    pWindow = (NETWORK_RuntimeWindowBase *)pRuntimeWindow;
    ((PLATFORM_InvisibleMessageWindow *)pRuntimeWindow)
        ->Construct((const char *)0x004A2754, &g_fTcpipRuntimeWindowClassRegistered);
    ConstructEffTransportRuntimeState(&pWindow->m_RuntimeState);
    *(void **)pWindow = g_NETWORK_TcpipRuntimeWindowVtable;
    pWindow->m_RuntimeState.m_RenderQueueNode.m_pVtable = (void **)g_NETWORK_TcpipRuntimeTransportVtable;
    return pRuntimeWindow;
}

// FUNCTION: LEMBALL 0x0046FA10
DWORD WINAPI TcpipNetworkMessageThreadMain(LPVOID pvThreadParam) {
    NETWORK_EffTransportRuntimeState *pActiveRuntime;
    NETWORK_RenderDispatchQueue *pDispatchQueue;
    MSG Msg;
    void *pRuntimeWindow;
    int fRuntimeStarted;

    (void)pvThreadParam;

    pRuntimeWindow = AllocateVSMemBlock(0x78);
    if (pRuntimeWindow != 0) {
        pRuntimeWindow = ConstructTcpipNetworkRuntimeWindow(pRuntimeWindow);
    }

    if (pRuntimeWindow == 0) {
        g_pActiveNetworkRuntimeWindow = 0;
    } else {
        g_pActiveNetworkRuntimeWindow = (char *)pRuntimeWindow + 0x10;
    }

    pActiveRuntime = (NETWORK_EffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
    fRuntimeStarted = pActiveRuntime->m_fStartRequested;
    while (fRuntimeStarted == 0 && pActiveRuntime->m_fRuntimeActive == 0 &&
           pActiveRuntime->m_fShutdownRequested == 0) {
        WaitMessage();
        GetMessageA(&Msg, 0, 0, 0);
        fRuntimeStarted = pActiveRuntime->m_fStartRequested;
    }

    if (pActiveRuntime->m_fStartRequested != 0) {
        InitializeEffTransportRuntimeGlobals(g_pActiveNetworkRuntimeWindow);
    }

    fRuntimeStarted = pActiveRuntime->m_fRuntimeActive;
    while (fRuntimeStarted == 0 && pActiveRuntime->m_fShutdownRequested == 0) {
        WaitMessage();
        GetMessageA(&Msg, 0, 0, 0);
        fRuntimeStarted = pActiveRuntime->m_fRuntimeActive;
    }

    if (pActiveRuntime->m_fShutdownRequested == 0) {
        fRuntimeStarted = pActiveRuntime->m_fRuntimeActive;
        while (fRuntimeStarted != 0) {
            WaitMessage();
            if (PeekMessageA(&Msg, 0, 0, 0, 0) != 0) {
                while (PeekMessageA(&Msg, 0, 0, 0, 0) != 0) {
                    GetMessageA(&Msg, 0, 0, 0);
                    TranslateMessage(&Msg);
                    DispatchMessageA(&Msg);
                }
            }

            pDispatchQueue = (NETWORK_RenderDispatchQueue *)g_pEffTransportDispatchQueue;
            while (pDispatchQueue != 0 && pDispatchQueue->m_cQueuedEntries != 0) {
                DrainRenderDispatchQueueEntries(g_pEffTransportDispatchQueue, (unsigned int)pDispatchQueue->m_cQueuedEntries);
            }

            fRuntimeStarted = pActiveRuntime->m_fRuntimeActive;
        }
    }

    if (g_pActiveNetworkRuntimeWindow != 0) {
        ((void (*)(void *, int))(*(void ***)g_pActiveNetworkRuntimeWindow)[1])(g_pActiveNetworkRuntimeWindow, 1);
    }
    g_pActiveNetworkRuntimeWindow = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x0046FBB0
int StartTcpipNetworkMessageThread(void) {
    DWORD dwStartTime;

    g_hTcpipNetworkThread = CreateThread(0, 0, TcpipNetworkMessageThreadMain, 0, 0, &g_dwTcpipNetworkThreadId);
    if (g_hTcpipNetworkThread == 0) {
        MessageBoxA(0, (const char *)0x004A23D0, (const char *)0x004A23C8, 0);
        ExitProcess(0xbbbb);
    }

    SetThreadPriority(g_hTcpipNetworkThread, 2);

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pActiveNetworkRuntimeWindow == 0) {
    }
    if (g_pActiveNetworkRuntimeWindow == 0) {
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A2400);
        return 0;
    }

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pEffTransportDispatchQueue == 0) {
    }
    if (g_pEffTransportDispatchQueue == 0) {
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A2424);
        return 0;
    }

    return 1;
}

// FUNCTION: LEMBALL 0x0046FC80
int StopTcpipNetworkMessageThread(void) {
    NETWORK_EffTransportRuntimeState *pActiveRuntime;
    NETWORK_EffTransportRuntimeStateVtable *pVtable;
    DWORD dwStartTime;

    if (g_pActiveNetworkRuntimeWindow == 0) {
        return 0;
    }

    pActiveRuntime = (NETWORK_EffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
    pVtable = (NETWORK_EffTransportRuntimeStateVtable *)pActiveRuntime->m_RenderQueueNode.m_pVtable;
    pActiveRuntime->m_fShutdownRequested = 1;
    pVtable->m_pServiceTransport(g_pActiveNetworkRuntimeWindow);

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pActiveNetworkRuntimeWindow != 0) {
    }
    if (g_pActiveNetworkRuntimeWindow != 0) {
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A244C);
        return 0;
    }

    return 1;
}
