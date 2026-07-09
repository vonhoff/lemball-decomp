#include "../game/game_app.h"
#include "../engine/runtime_init.h"
#include "../engine/memory_arena.h"
#include "../platform/message_window.h"
#include "network/nruntime.h"

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
struct NETWORK_SimpleEffTransportRecordSlotTable;

struct NETWORK_PeerPayloadSenderStateOffsets {
    void *m_pReserved00;
    int m_nSendStateBaseOffset04;
};

struct NETWORK_PeerPayloadSenderRuntimeStateView {
    char m_abReserved00[0x44];
    NETWORK_EffTransportHandleCallback *m_pHandleCallback44;
    char m_abReserved48[0x08];
    int m_fRetryServiceEnabled50;
    char m_abReserved54[0x08];
    int m_fKeepaliveEnabled5c;
    char m_abReserved60[0x08];
    int m_fTransportActive68;
};

struct NETWORK_PeerPayloadSenderPendingWriteState {
    int *m_pBusyStream00;
    int m_nReserved04;
    int m_nPendingFragmentSequence08;
    int m_nPendingFragmentIndex0c;
};

struct NETWORK_PeerPayloadSenderState {
    void *m_pVtable;
    int m_nReserved04;
    int m_pPayloadBuffer08;
    int m_fOwnsPayload0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nBufferEnd18;
    int m_nReserved1c;
    int m_nReserved20;
    int m_fReliable24;
    int m_fBusy28;
    int m_pPacketHeader2c;
    unsigned short m_wReliableSequence30;
    unsigned short m_wUnreliableSequence32;
    unsigned short m_wReserved34;
    unsigned short m_wFragmentSequence36;
    DWORD m_dwLastGlobalWriteTick38;
    int m_nReserved3c;
    int m_nReserved40;
    NETWORK_PeerPayloadSenderStateOffsets *m_pOffsets44;
    int m_nReserved48;
    NETWORK_SimpleEffTransportRecordSlotTable *m_pRecordTable4c;
    int m_nReserved50;
    void *m_pPendingFragmentedStream54;
    int m_nPendingFragmentSequence58;
    int m_nPendingFragmentIndex5c;
    unsigned int m_aFragmentPrefixWords60[4];
};

struct NETWORK_EffStreamPayloadState {
    void **m_pVtable;
    int m_nEventCode04;
    int m_pPayloadBuffer08;
    int m_fOwnsPayload0c;
    int m_nWriteSessionCount10;
    int m_nReserved14;
    int m_nSerializedLength18;
    int m_nWriteCursor1c;
    int m_nReadCursor20;
    int m_fReliable24;
    int m_fBusy28;
};

struct NETWORK_EffTransportPacketHeader {
    int m_nReserved00;
    int m_cbPayload04;
    unsigned short m_nStreamEvent08;
    unsigned short m_wSequence0a;
    unsigned short m_wFragmentIndex0c;
    unsigned char m_fReliable0e;
    unsigned char m_abReserved0f[3];
};

struct NETWORK_PeerPayloadSenderSlot {
    char m_abUnknown00[4];
    NETWORK_PeerPayloadSenderRaw m_Sender;
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

struct NETWORK_RuntimeWindowSendGateState {
    char m_abUnknown00[0x24];
    int m_nChannelOwner24;
    char m_abUnknown28[8];
    int m_fSendGateActive30;
};

struct NETWORK_EffTransportConnectOffsets {
    int m_nReserved00;
    int m_nSendStateViewOffset04;
    int m_nReliableTickViewOffset08;
    int m_nTimedTickViewOffset0c;
};

struct NETWORK_EffTransportConnectOwner {
    void *m_pVtable00;
    NETWORK_EffTransportConnectOffsets *m_pOffsets04;
    char m_abReserved08[0x0c];
    int m_nReserved14;
    int m_fTrackActivePeer1c;
    int m_fConnected24;
};

struct NETWORK_EffTransportConnectStateView {
    char m_abReserved00[0x14];
    int m_fConnected14;
    int m_nCurrentState18;
    int m_fTrackActivePeer1c;
    int m_nPreviousState20;
};

struct NETWORK_EffTransportTickStateView {
    char m_abReserved00[0x40];
    DWORD m_dwLastTick40;
};

struct NETWORK_EffTransportAckDispatchOffsets {
    int m_nReserved00;
    int m_nReserved04;
    int m_nAckRecordOwnerViewOffset08;
};

struct NETWORK_EffTransportAckDispatchOwner {
    void *m_pVtable00;
    NETWORK_EffTransportAckDispatchOffsets *m_pOffsets04;
};

struct NETWORK_EffPrefixedControlDispatchOwner {
    char m_abReserved00[0x18];
    int m_nPeerMode18;
    char m_abReserved1c[0x0c];
    void *m_pPeerKey28;
};

struct NETWORK_EffDispatchEventPayloadView {
    char m_abReserved00[4];
    int m_nPayloadBuffer04;
    char m_abReserved08[0x1c];
    void *m_pOwnedPayload24;
};

struct NETWORK_EffTransportGlobalReadStream {
    void *m_pVtable;
    int m_nEventCode;
    char m_abUnknown08[0x24];
    unsigned short m_wSequence2c;
    unsigned short m_wPacketKind2e;
};

struct NETWORK_AckedEffTransportRecord {
    int m_nReserved00;
    int m_pSerializedStream04;
    unsigned char m_fClaimed08;
    char m_abUnknown09[3];
    void *m_pPayload0c;
};

struct NETWORK_AckedEffTransportRecordTable {
    int m_cRecords;
    int m_nReserved04;
    NETWORK_AckedEffTransportRecord **m_apRecords08;
};

struct NETWORK_AckedEffTransportRecordOwner {
    char m_abUnknown00[0x4c];
    NETWORK_AckedEffTransportRecordTable *m_pRecordTable4c;
};

struct NETWORK_SimpleEffTransportRecordSlot {
    void **m_pVtable;
    int m_pSerializedStream04;
    unsigned char m_fAvailable08;
    char m_abUnknown09[3];
    void *m_pPayload0c;
    DWORD m_dwLastRetryTick10;
    unsigned char m_cRetryCount14;
    char m_abReserved15[3];
};

struct NETWORK_SimpleEffTransportRecordSlotTable {
    int m_cRecords;
    int m_nReserved04;
    NETWORK_SimpleEffTransportRecordSlot **m_apRecords08;
};

struct NETWORK_VSMemObjectPointerArray {
    int m_cObjects;
    int m_nReserved04;
    int m_pObjectArray08;
};

struct NETWORK_EffTransportRecordBuffer {
    void **m_pVtable;
    int m_pSerializedBuffer04;
    int m_cbPayload08;
    int m_fAllocatePayload0c;
    int m_nReserved10;
    int m_nReserved14;
};

static void *g_NETWORK_EffTransportRecordBufferVtable = (void *)0x004991CC;
static void *g_NETWORK_DeleteSimpleEffTransportRecordSlotVtable = (void *)0x004991D0;
static void *g_NETWORK_DeleteEffTransportRecordBufferVtable = (void *)0x004991D4;

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

char *g_pszEffTransportBroadcastStatusPayload = 0;
char *g_pszFileBasedNetworkLocalHostName = 0;
int g_cbEffTransportBroadcastStatusPayload = 0;
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
extern void EndEffStreamWriteSession(void *pStream);
extern void LoadEffStreamFromGlobalRange(void *pStream);
extern int LoadEffStreamFromMemory(void *pStream, int nSourceBuffer);
extern void SaveEffStreamToMemoryRange(void *pStream, int nTargetBuffer, int cbRange);
void UnlinkAndDeleteEffTransportPeer(void *pRuntimeState, int nPeer);
void MarkEffTransportPeerActivityTime(int nPeer);
int SendReliableEffTransportPayload(void *pPayloadSender, void *pStream);
int SendFragmentedEffTransportPayload(void *pPayloadSender, void *pStream);

// FUNCTION: LEMBALL 0x004601B0
void NETWORK_PeerPayloadSender::WriteEffStreamWithGlobalSession(void) {
    BeginEffStreamWriteSession(g_pEffTransportGlobalReadStream);
    SendEffStreamPayloadWithTransportHeader((int)(unsigned long)g_pEffTransportGlobalReadStream);
    EndEffStreamWriteSession(g_pEffTransportGlobalReadStream);
}

// FUNCTION: LEMBALL 0x004601E0
void *ClaimAckedEffTransportRecordPayload(void *pObject) {
    NETWORK_AckedEffTransportRecordOwner *pOwner;
    NETWORK_AckedEffTransportRecordTable *pRecordTable;
    NETWORK_AckedEffTransportRecord *pRecord;
    NETWORK_EffTransportGlobalReadStream *pReadStream;
    NETWORK_EffTransportPacketHeader *pSerializedHeader;
    void *pPayload;
    int nRecordIndex;

    pPayload = 0;
    pReadStream = (NETWORK_EffTransportGlobalReadStream *)g_pEffTransportGlobalReadStream;
    LoadEffStreamFromMemory(pReadStream, (int)(unsigned long)((char *)g_pEffTransportPacketBuffer + 0x10));
    if (pReadStream->m_wPacketKind2e != 0x100) {
        return 0;
    }

    pOwner = (NETWORK_AckedEffTransportRecordOwner *)pObject;
    pRecordTable = pOwner->m_pRecordTable4c;
    nRecordIndex = pReadStream->m_wSequence2c % pRecordTable->m_cRecords;
    pRecord = pRecordTable->m_apRecords08[nRecordIndex];
    pSerializedHeader = (NETWORK_EffTransportPacketHeader *)(unsigned long)pRecord->m_pSerializedStream04;

    LoadEffStreamFromMemory(pObject, pRecord->m_pSerializedStream04);
    if (pSerializedHeader->m_wSequence0a == pReadStream->m_wSequence2c && pRecord->m_fClaimed08 == 0) {
        pRecord->m_fClaimed08 = 1;
        pPayload = pRecord->m_pPayload0c;
    }
    LoadEffStreamFromMemory(pObject, pRecord->m_pSerializedStream04);
    return pPayload;
}

// FUNCTION: LEMBALL 0x004611E0
void CopyIntoSimpleEffTransportRecordSlot(void *pObject, const void *pvSource, unsigned int cbSource, void *pPayload) {
    unsigned int i;
    const unsigned char *pbSource;
    unsigned char *pbTarget;
    NETWORK_SimpleEffTransportRecordSlot *pSlot;

    pSlot = (NETWORK_SimpleEffTransportRecordSlot *)pObject;
    pSlot->m_pPayload0c = pPayload;
    pSlot->m_cRetryCount14 = 0;

    pbSource = (const unsigned char *)pvSource;
    pbTarget = (unsigned char *)(unsigned long)pSlot->m_pSerializedStream04;
    cbSource &= 0xffff;

    for (i = cbSource >> 2; i != 0; --i) {
        *(unsigned int *)pbTarget = *(const unsigned int *)pbSource;
        pbSource += 4;
        pbTarget += 4;
    }

    for (i = cbSource & 3; i != 0; --i) {
        *pbTarget++ = *pbSource++;
    }
}

// FUNCTION: LEMBALL 0x00461210
void *ConstructEffTransportRecordSlotTable(void *pObject, int cRecords, unsigned int cbRecord) {
    NETWORK_SimpleEffTransportRecordSlotTable *pTable;

    pTable = (NETWORK_SimpleEffTransportRecordSlotTable *)pObject;
    pTable->m_nReserved04 = cbRecord & 0xffff;
    pTable->m_cRecords = cRecords;
    if (cRecords > 0) {
        pTable->m_apRecords08 = (NETWORK_SimpleEffTransportRecordSlot **)AllocateVSMemBlock((unsigned int)(cRecords << 2));
    } else {
        pTable->m_apRecords08 = 0;
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00461190
void *ConstructSimpleEffTransportRecordSlot(void *pObject, unsigned int cbRecord) {
    NETWORK_SimpleEffTransportRecordSlot *pSlot;

    pSlot = (NETWORK_SimpleEffTransportRecordSlot *)pObject;
    pSlot->m_pVtable = (void **)g_NETWORK_DeleteSimpleEffTransportRecordSlotVtable;
    pSlot->m_pSerializedStream04 = (int)(unsigned long)AllocateVSMemBlock(cbRecord & 0xffff);
    pSlot->m_fAvailable08 = 1;
    pSlot->m_cRetryCount14 = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x004611C0
void DestroySimpleEffTransportRecordSlot(void *pObject) {
    NETWORK_SimpleEffTransportRecordSlot *pSlot;

    pSlot = (NETWORK_SimpleEffTransportRecordSlot *)pObject;
    pSlot->m_pVtable = (void **)g_NETWORK_DeleteSimpleEffTransportRecordSlotVtable;
    FreeVSMemBlock((void *)(unsigned long)pSlot->m_pSerializedStream04);
    pSlot->m_pVtable = (void **)g_NETWORK_EffTransportRecordBufferVtable;
}

// FUNCTION: LEMBALL 0x00461250
void DestroyVSMemObjectPointerArray(void *pObject) {
    NETWORK_VSMemObjectPointerArray *pArray;
    NETWORK_DeleteObject *pDeleteObject;
    int i;
    int nOffset;

    pArray = (NETWORK_VSMemObjectPointerArray *)pObject;
    if (pArray->m_pObjectArray08 != 0) {
        nOffset = 0;
        for (i = 0; i < pArray->m_cObjects; ++i) {
            pDeleteObject = *(NETWORK_DeleteObject **)(unsigned long)(pArray->m_pObjectArray08 + nOffset);
            if (pDeleteObject != 0) {
                pDeleteObject->m_pVtable->m_pDelete(1);
            }
            nOffset += 4;
        }
        FreeVSMemBlock((void *)(unsigned long)pArray->m_pObjectArray08);
    }
}

// FUNCTION: LEMBALL 0x00461340
void *ConstructSimpleEffTransportRecordSlotTable(void *pObject, int cRecords, unsigned int cbRecord) {
    NETWORK_SimpleEffTransportRecordSlotTable *pTable;
    void *pSlotStorage;
    int i;
    int nOffset;

    pTable = (NETWORK_SimpleEffTransportRecordSlotTable *)ConstructEffTransportRecordSlotTable(pObject, cRecords, cbRecord);
    if (pTable->m_apRecords08 != 0) {
        nOffset = 0;
        for (i = 0; i < pTable->m_cRecords; ++i) {
            pSlotStorage = AllocateVSMemBlock(0x18);
            if (pSlotStorage == 0) {
                *(void **)(unsigned long)((int)(unsigned long)pTable->m_apRecords08 + nOffset) = 0;
            } else {
                *(void **)(unsigned long)((int)(unsigned long)pTable->m_apRecords08 + nOffset) =
                    ConstructSimpleEffTransportRecordSlot(pSlotStorage, (unsigned int)pTable->m_nReserved04);
            }
            nOffset += 4;
        }
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004613A0
void CopyPayloadIntoSimpleRecordSlot(void *pObject, int nIndex, const void *pvSource, unsigned int cbSource, void *pPayload) {
    NETWORK_SimpleEffTransportRecordSlotTable *pTable;

    pTable = (NETWORK_SimpleEffTransportRecordSlotTable *)pObject;
    CopyIntoSimpleEffTransportRecordSlot(pTable->m_apRecords08[nIndex], pvSource, cbSource, pPayload);
}

// FUNCTION: LEMBALL 0x00461990
int ClaimSimpleRecordSlotForPacketId(void *pObject, unsigned int nPacketId, const void *pvSource, unsigned int cbSource,
                                     void *pPayload) {
    NETWORK_SimpleEffTransportRecordSlotTable *pTable;
    NETWORK_SimpleEffTransportRecordSlot *pSlot;
    int nIndex;

    pTable = (NETWORK_SimpleEffTransportRecordSlotTable *)pObject;
    nIndex = (int)((unsigned long)(nPacketId & 0xffff) % (unsigned long)pTable->m_cRecords);
    pSlot = pTable->m_apRecords08[nIndex];
    if (pSlot->m_fAvailable08 == 0) {
        return 0;
    }

    CopyPayloadIntoSimpleRecordSlot(pTable, nIndex, pvSource, cbSource, pPayload);
    return (int)(unsigned long)pSlot;
}

// FUNCTION: LEMBALL 0x004613D0
void *ConstructEffTransportRecordBuffer(void *pObject, int fAllocatePayload, int cbPayload, unsigned int cbRecord) {
    NETWORK_EffTransportRecordBuffer *pBuffer;
    NETWORK_EffTransportPacketHeader *pSerializedHeader;
    int cbSerialized;

    pBuffer = (NETWORK_EffTransportRecordBuffer *)pObject;
    pBuffer->m_pVtable = (void **)g_NETWORK_DeleteEffTransportRecordBufferVtable;
    pBuffer->m_fAllocatePayload0c = fAllocatePayload;
    pBuffer->m_nReserved14 = 0;
    pBuffer->m_nReserved10 = 0;
    cbSerialized = (cbRecord & 0xffff) - 0x10;
    pBuffer->m_cbPayload08 = cbSerialized;
    if (fAllocatePayload != 0 && cbSerialized != 0) {
        pBuffer->m_pSerializedBuffer04 = (int)(unsigned long)AllocateVSMemBlock((unsigned int)(cbPayload + 0x10));
        pSerializedHeader = (NETWORK_EffTransportPacketHeader *)(unsigned long)pBuffer->m_pSerializedBuffer04;
        pSerializedHeader->m_wSequence0a = 0;
    } else {
        pBuffer->m_pSerializedBuffer04 = 0;
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00461440
void DestroyEffTransportRecordBuffer(void *pObject) {
    NETWORK_EffTransportRecordBuffer *pBuffer;

    pBuffer = (NETWORK_EffTransportRecordBuffer *)pObject;
    pBuffer->m_pVtable = (void **)g_NETWORK_DeleteEffTransportRecordBufferVtable;
    if (pBuffer->m_pSerializedBuffer04 != 0) {
        FreeVSMemBlock((void *)(unsigned long)pBuffer->m_pSerializedBuffer04);
    }
    pBuffer->m_pVtable = (void **)g_NETWORK_EffTransportRecordBufferVtable;
}

// FUNCTION: LEMBALL 0x00462A20
void *DeleteSimpleEffTransportRecordSlotWrapper(void *pObject, BYTE fFreeMemory) {
    DestroySimpleEffTransportRecordSlot(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462A40
void *DeleteEffTransportRecordBufferWrapper(void *pObject, BYTE fFreeMemory) {
    DestroyEffTransportRecordBuffer(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045FEA0
int SendReliableEffTransportPayload(void *pPayloadSender, void *pStream) {
    NETWORK_PeerPayloadSenderState *pSender;
    NETWORK_EffTransportPacketHeader *pHeader;
    NETWORK_EffStreamPayloadState *pPayload;
    NETWORK_SimpleEffTransportRecordSlot *pRecord;
    void *pvPayload;
    int cbPayload;

    pSender = (NETWORK_PeerPayloadSenderState *)pPayloadSender;
    pPayload = (NETWORK_EffStreamPayloadState *)pStream;
    pHeader = (NETWORK_EffTransportPacketHeader *)(unsigned long)pSender->m_pPacketHeader2c;
    pvPayload = (void *)(unsigned long)pPayload->m_pPayloadBuffer08;
    cbPayload = pPayload->m_nWriteCursor1c - pPayload->m_pPayloadBuffer08;

    pHeader->m_fReliable0e = 1;
    SaveEffStreamToMemoryRange(pPayloadSender, (int)(unsigned long)pvPayload, 0);
    pRecord = (NETWORK_SimpleEffTransportRecordSlot *)(unsigned long)ClaimSimpleRecordSlotForPacketId(
        pSender->m_pRecordTable4c, pHeader->m_wSequence0a, pvPayload, (unsigned int)cbPayload, pStream);
    if (pRecord == 0) {
        return 0;
    }

    if (((NETWORK_PeerPayloadSender *)pSender)->SendPayload(pvPayload, cbPayload) != 0) {
        pRecord->m_dwLastRetryTick10 = timeGetTime();
        pRecord->m_fAvailable08 = 0;
        ++pRecord->m_cRetryCount14;
    } else {
        pRecord->m_dwLastRetryTick10 = timeGetTime() - 1000;
        pRecord->m_fAvailable08 = 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0045FF20
int RetryPendingEffTransportRecord(void *pObject, int nRecord) {
    NETWORK_SimpleEffTransportRecordSlot *pRecord;
    NETWORK_EffTransportPacketHeader *pSerializedHeader;
    int nSendResult;

    pRecord = (NETWORK_SimpleEffTransportRecordSlot *)(unsigned long)nRecord;
    pSerializedHeader = (NETWORK_EffTransportPacketHeader *)(unsigned long)pRecord->m_pSerializedStream04;
    LoadEffStreamFromMemory(pObject, pRecord->m_pSerializedStream04);
    LoadEffStreamFromMemory(pObject, pRecord->m_pSerializedStream04);
    nSendResult =
        ((NETWORK_PeerPayloadSender *)pObject)
        ->SendPayload((void *)(unsigned long)pRecord->m_pSerializedStream04, pSerializedHeader->m_cbPayload04);
    if (nSendResult != 0) {
        pRecord->m_dwLastRetryTick10 = timeGetTime();
        ++pRecord->m_cRetryCount14;
        pRecord->m_fAvailable08 = 0;
    }
    return nSendResult;
}

// FUNCTION: LEMBALL 0x0045FF70
int SendFragmentedEffTransportPayload(void *pPayloadSender, void *pStream) {
    const int cbFragmentStep = g_cbEffTransportMaxPacketBytes - 0x10;
    NETWORK_PeerPayloadSenderState *pSender;
    NETWORK_EffTransportPacketHeader *pHeader;
    NETWORK_EffStreamPayloadState *pPayload;
    unsigned int cFragments;
    unsigned int cbPayload;
    unsigned int cbPacket;
    unsigned int iFragmentsSent;
    unsigned char *pbPayload;
    int nSendResult;

    pSender = (NETWORK_PeerPayloadSenderState *)pPayloadSender;
    pPayload = (NETWORK_EffStreamPayloadState *)pStream;
    if (pSender->m_nPendingFragmentIndex5c == -1) {
        pSender->m_nPendingFragmentIndex5c = 0;
        ++pSender->m_wFragmentSequence36;
        pSender->m_nPendingFragmentSequence58 = (short)pSender->m_wFragmentSequence36;
        pSender->m_pPendingFragmentedStream54 = pStream;
    }

    pbPayload = (unsigned char *)(unsigned long)pPayload->m_pPayloadBuffer08;
    cbPayload = (unsigned int)(pPayload->m_nWriteCursor1c - pPayload->m_pPayloadBuffer08);
    cFragments = (cbPayload + cbFragmentStep - 1) / (unsigned int)cbFragmentStep;

    if (pSender->m_nPendingFragmentIndex5c != 0) {
        pbPayload += cbFragmentStep * pSender->m_nPendingFragmentIndex5c;
        cbPayload -= cbFragmentStep * pSender->m_nPendingFragmentIndex5c;
    }

    pHeader = (NETWORK_EffTransportPacketHeader *)(unsigned long)pSender->m_pPacketHeader2c;
    pHeader->m_wSequence0a = (unsigned short)pSender->m_nPendingFragmentSequence58;

    iFragmentsSent = 0;
    while ((unsigned int)pSender->m_nPendingFragmentIndex5c < cFragments && iFragmentsSent < 9) {
        pHeader->m_wFragmentIndex0c = (unsigned short)pSender->m_nPendingFragmentIndex5c;
        cbPacket = g_cbEffTransportMaxPacketBytes;
        if (cbPayload < cbPacket) {
            cbPacket = cbPayload;
        }

        if (pSender->m_nPendingFragmentIndex5c == 0) {
            SaveEffStreamToMemoryRange(pPayloadSender, (int)(unsigned long)pbPayload, 0);
            nSendResult = ((NETWORK_PeerPayloadSender *)pSender)->SendPayload(pbPayload, (int)cbPacket);
        } else {
            pSender->m_aFragmentPrefixWords60[0] = *(unsigned int *)(unsigned long)(pbPayload + 0);
            pSender->m_aFragmentPrefixWords60[1] = *(unsigned int *)(unsigned long)(pbPayload + 4);
            pSender->m_aFragmentPrefixWords60[2] = *(unsigned int *)(unsigned long)(pbPayload + 8);
            pSender->m_aFragmentPrefixWords60[3] = *(unsigned int *)(unsigned long)(pbPayload + 12);
            SaveEffStreamToMemoryRange(pPayloadSender, (int)(unsigned long)pbPayload, 0);
            nSendResult = ((NETWORK_PeerPayloadSender *)pSender)->SendPayload(pbPayload, (int)cbPacket);
            *(unsigned int *)(unsigned long)(pbPayload + 0) = pSender->m_aFragmentPrefixWords60[0];
            *(unsigned int *)(unsigned long)(pbPayload + 4) = pSender->m_aFragmentPrefixWords60[1];
            *(unsigned int *)(unsigned long)(pbPayload + 8) = pSender->m_aFragmentPrefixWords60[2];
            *(unsigned int *)(unsigned long)(pbPayload + 12) = pSender->m_aFragmentPrefixWords60[3];
        }

        if (nSendResult == 0) {
            break;
        }

        pbPayload += cbPacket - 0x10;
        cbPayload = cbPayload - cbPacket + 0x10;
        ++pSender->m_nPendingFragmentIndex5c;
        ++iFragmentsSent;
    }

    if ((unsigned int)pSender->m_nPendingFragmentIndex5c == cFragments) {
        pSender->m_nPendingFragmentIndex5c = -1;
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x004600D0
int NETWORK_PeerPayloadSender::SendEffStreamPayloadWithTransportHeader(int nStream) {
    NETWORK_PeerPayloadSenderState *pSender;
    NETWORK_PeerPayloadSenderStateOffsets *pOffsets;
    NETWORK_PeerPayloadSenderRuntimeStateView *pRuntimeState;
    NETWORK_EffTransportPacketHeader *pHeader;
    NETWORK_EffStreamPayloadState *pPayload;
    void *pvPayload;
    int cbPayload;

    pSender = (NETWORK_PeerPayloadSenderState *)this;
    pOffsets = pSender->m_pOffsets44;
    pRuntimeState = (NETWORK_PeerPayloadSenderRuntimeStateView *)((char *)pSender + pOffsets->m_nSendStateBaseOffset04);
    if (pRuntimeState->m_fTransportActive68 == 0) {
        return 0;
    }

    pPayload = (NETWORK_EffStreamPayloadState *)(unsigned long)nStream;
    if (pPayload->m_fBusy28 == 0) {
        pPayload->m_fBusy28 = 1;
    }

    pHeader = (NETWORK_EffTransportPacketHeader *)(unsigned long)pSender->m_pPacketHeader2c;
    pvPayload = (void *)(unsigned long)pPayload->m_pPayloadBuffer08;
    cbPayload = pPayload->m_nWriteCursor1c - pPayload->m_pPayloadBuffer08;
    pHeader->m_cbPayload04 = cbPayload;
    pHeader->m_nStreamEvent08 = (unsigned short)pPayload->m_nEventCode04;

    if ((unsigned int)cbPayload > (unsigned int)g_cbEffTransportMaxPacketBytes) {
        if (pSender->m_nPendingFragmentIndex5c == -1) {
            cbPayload = SendFragmentedEffTransportPayload(pSender, pPayload);
        } else {
            pPayload->m_fBusy28 = 0;
            cbPayload = 0;
        }
    } else {
        pHeader->m_wFragmentIndex0c = 0x100;
        if (pPayload->m_fReliable24 == 0) {
            ++pSender->m_wUnreliableSequence32;
            pHeader->m_wSequence0a = pSender->m_wUnreliableSequence32;
            pHeader->m_fReliable0e = 0;
            SaveEffStreamToMemoryRange(this, (int)(unsigned long)pvPayload, 0);
            cbPayload = ((NETWORK_PeerPayloadSender *)pSender)->SendPayload(pvPayload, pHeader->m_cbPayload04);
        } else {
            ++pSender->m_wReliableSequence30;
            pHeader->m_wSequence0a = pSender->m_wReliableSequence30;
            cbPayload = SendReliableEffTransportPayload(pSender, pPayload);
        }
    }

    pPayload->m_fBusy28 = 0;
    return cbPayload;
}

// FUNCTION: LEMBALL 0x00460A40
void ClearEffTransportSendGate(int nOwner) {
    ((NETWORK_ChannelOwnerObject *)(unsigned long)nOwner)->m_nSendGate = 0;
}

// FUNCTION: LEMBALL 0x00460260
void ClearEffTransportPendingWriteState(int nAdjustedObject) {
    NETWORK_PeerPayloadSenderPendingWriteState *pPendingState;

    pPendingState = (NETWORK_PeerPayloadSenderPendingWriteState *)(unsigned long)(nAdjustedObject - 0x24);
    if (pPendingState->m_nPendingFragmentIndex0c != -1) {
        pPendingState->m_nPendingFragmentIndex0c = -1;
        pPendingState->m_pBusyStream00[10] = 0;
    }
}

// FUNCTION: LEMBALL 0x00460280
void ServiceEffTransportRetries(void *pObject) {
    NETWORK_PeerPayloadSenderState *pSender;
    NETWORK_PeerPayloadSenderRuntimeStateView *pRuntimeState;
    NETWORK_SimpleEffTransportRecordSlotTable *pRecordTable;
    NETWORK_SimpleEffTransportRecordSlot *pRecord;
    DWORD dwNow;
    int i;
    int cRecords;

    pSender = (NETWORK_PeerPayloadSenderState *)pObject;
    pRuntimeState =
        (NETWORK_PeerPayloadSenderRuntimeStateView *)((char *)pSender + pSender->m_pOffsets44->m_nSendStateBaseOffset04);
    if (pRuntimeState->m_fTransportActive68 == 0 || pRuntimeState->m_fRetryServiceEnabled50 == 0) {
        return;
    }

    if (pSender->m_nPendingFragmentIndex5c != -1 && SendFragmentedEffTransportPayload(pSender, pSender->m_pPendingFragmentedStream54) != 0) {
        ((NETWORK_EffStreamPayloadState *)pSender->m_pPendingFragmentedStream54)->m_fBusy28 = 0;
    }

    i = 0;
    pRecordTable = pSender->m_pRecordTable4c;
    cRecords = pRecordTable->m_cRecords;
    while (1) {
        if (cRecords <= i) {
            if (pRuntimeState->m_fKeepaliveEnabled5c == 0) {
                return;
            }

            dwNow = timeGetTime();
            if (dwNow - pSender->m_dwLastGlobalWriteTick38 < 0x3e9) {
                return;
            }

            ((NETWORK_PeerPayloadSender *)pSender)
                ->SendEffStreamPayloadWithTransportHeader((int)(unsigned long)g_pEffTransportGlobalWriteStream);
            return;
        }

        pRecord = pRecordTable->m_apRecords08[i % cRecords];
        if (pRecord->m_fAvailable08 == 0) {
            dwNow = timeGetTime();
            if (1000 < dwNow - pRecord->m_dwLastRetryTick10) {
                if (*(unsigned int *)((char *)g_pActiveNetworkRuntimeWindow + 0x60) == 0 ||
                    pRecord->m_cRetryCount14 != *(unsigned char *)((char *)g_pActiveNetworkRuntimeWindow + 0x60)) {
                    RetryPendingEffTransportRecord(pSender, (int)(unsigned long)pRecord);
                    ++i;
                    continue;
                }

                pRecord->m_fAvailable08 = 1;
                pRuntimeState->m_pHandleCallback44->m_pVtable->m_pNotifyClosed(1);
            }
        }
        ++i;
    }
}

// FUNCTION: LEMBALL 0x00460D70
void QueueEffTransportConnectEvent(void *pObject) {
    NETWORK_EffDispatchEvent kEvent;
    NETWORK_EffTransportConnectStateView *pSendState;
    NETWORK_EffTransportTickStateView *pReliableTickState;
    NETWORK_EffTransportTickStateView *pTimedTickState;
    NETWORK_EffTransportConnectOffsets *pOffsets;
    NETWORK_EffTransportConnectOwner *pOwner;
    NETWORK_RuntimeWindowSendGateState *pWindowState;
    DWORD dwNow;

    pOwner = (NETWORK_EffTransportConnectOwner *)((char *)pObject - 0xd8);
    pOffsets = pOwner->m_pOffsets04;
    pSendState = (NETWORK_EffTransportConnectStateView *)((char *)pOwner + pOffsets->m_nSendStateViewOffset04);
    pReliableTickState = (NETWORK_EffTransportTickStateView *)((char *)pOwner + pOffsets->m_nReliableTickViewOffset08);
    pTimedTickState = (NETWORK_EffTransportTickStateView *)((char *)pOwner + pOffsets->m_nTimedTickViewOffset0c);
    kEvent.m_nType = 3;
    kEvent.m_nCode = 0;
    kEvent.m_pStream = (int *)pOwner;
    kEvent.m_pPeer = 0;
    pOwner->m_fConnected24 = 1;
    pSendState->m_nCurrentState18 = 0;
    pSendState->m_nPreviousState20 = pSendState->m_nCurrentState18;
    pSendState->m_fConnected14 = 1;
    if (pOwner->m_fTrackActivePeer1c != 0) {
        pSendState->m_fTrackActivePeer1c = 1;
        dwNow = timeGetTime();
        pReliableTickState->m_dwLastTick40 = dwNow - 1000;
        dwNow = timeGetTime();
        pTimedTickState->m_dwLastTick40 = dwNow;
        pWindowState = (NETWORK_RuntimeWindowSendGateState *)g_pActiveNetworkRuntimeWindow;
        if (pWindowState->m_fSendGateActive30 != 0) {
            ClearEffTransportSendGate(pWindowState->m_nChannelOwner24);
        }
    }
    ((void (*)(NETWORK_EffDispatchEvent *))(*(void ***)g_pEffTransportDispatchQueue)[2])(&kEvent);
}

// FUNCTION: LEMBALL 0x00460A50
void SendLoadedEffEventToPeer(void *pOwner, void *pPeerKey, void *pStream) {
    NETWORK_ChannelOwnerObject *pChannelOwner;
    NETWORK_PeerPayloadSenderSlot *pPayloadSenderSlot;
    NETWORK_PeerPayloadSenderRaw *pPayloadSender;

    LoadEffStreamFromGlobalRange(pStream);
    pChannelOwner = (NETWORK_ChannelOwnerObject *)pOwner;
    pPayloadSenderSlot =
        (NETWORK_PeerPayloadSenderSlot *)((char *)pChannelOwner + pChannelOwner->m_pOffsets->m_nPayloadSenderSlotOffset);
    pPayloadSender = &pPayloadSenderSlot->m_Sender;
    ((NETWORK_PeerPayloadSender *)pPayloadSender)->WritePeerKey(pPeerKey);
    ((NETWORK_PeerPayloadSender *)pPayloadSender)
        ->SendEffStreamPayloadWithTransportHeader((int)(unsigned long)pStream);
}

// FUNCTION: LEMBALL 0x00460C60
void SetEffTransportPeerNameAndPort(void *pPeer, char *pszName, void *pKey, short nPort) {
    NETWORK_EffTransportPeer *pTransportPeer;
    NETWORK_PeerPayloadSenderSlot *pPayloadSenderSlot;
    NETWORK_PeerPayloadSenderRaw *pPayloadSender;
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
    ((NETWORK_PeerPayloadSender *)pPayloadSender)->SetAssignedPort(nPort);
    ((NETWORK_PeerPayloadSender *)pPayloadSender)->WritePeerKey(pKey);
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
    NETWORK_PeerPayloadSenderRaw *pPayloadSender;
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
        ((NETWORK_PeerPayloadSender *)pPayloadSender)->SetAssignedPort(nAssignedPort);
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
    NETWORK_PeerPayloadSenderRaw *pPayloadSender;
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
    ((NETWORK_PeerPayloadSender *)pPayloadSender)->SetAssignedPort(nAssignedPort);
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
    NETWORK_PeerPayloadSenderRaw *pPayloadSender;
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
        nResult = ((NETWORK_PeerPayloadSender *)pPayloadSender)
                      ->SendEffStreamPayloadWithTransportHeader((int)(unsigned long)pStream);
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
            EndEffStreamWriteSession(pStream);
        }
        return nResult;
    }

    pStream[10] = 0;
    return 0;
}

// FUNCTION: LEMBALL 0x00460F60
void *DispatchAckedEffTransportPayloadEvent(void *pObject) {
    NETWORK_EffDispatchEvent kEvent;
    NETWORK_EffTransportAckDispatchOwner *pOwner;
    void *pPayload;

    pOwner = (NETWORK_EffTransportAckDispatchOwner *)((char *)pObject - 0x60);
    pPayload = ClaimAckedEffTransportRecordPayload(
        (char *)pObject + pOwner->m_pOffsets04->m_nAckRecordOwnerViewOffset08 - 0x5c);
    if (pPayload != 0) {
        kEvent.m_nType = 6;
        kEvent.m_nCode = 0;
        kEvent.m_pStream = (int *)pOwner;
        kEvent.m_pPeer = pPayload;
        ((void (*)(NETWORK_EffDispatchEvent *))(*(void ***)g_pEffTransportSecondaryDispatchQueue)[2])(&kEvent);
    }
    return pPayload;
}

// FUNCTION: LEMBALL 0x00460FF0
void QueueEffTransportPayloadEvent(void *pObject, unsigned short nType, void *pPayload) {
    NETWORK_EffDispatchEvent kEvent;

    kEvent.m_nType = nType;
    kEvent.m_nCode = 0;
    kEvent.m_pStream = (int *)((char *)pObject - 0xd8);
    kEvent.m_pPeer = pPayload;
    ((void (*)(NETWORK_EffDispatchEvent *))(*(void ***)g_pEffTransportSecondaryDispatchQueue)[2])(&kEvent);
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

// FUNCTION: LEMBALL 0x00460610
void ConfigureEffTransportConnectHostString(void *pObject, void *pHostName) {
    void (**ppVtable)(void);

    if (*(void **)((char *)pObject + 0x1c) != 0) {
        FreeVSMemBlock(*(void **)((char *)pObject + 0x1c));
        *(void **)((char *)pObject + 0x1c) = 0;
    }

    *(int *)((char *)pObject + 0xc) = 2;
    ppVtable = *(void (***) (void))pObject;
    ((void (*)(void *))ppVtable[2])(pHostName);
}

// FUNCTION: LEMBALL 0x00460650
void PrepareEffTransportBroadcastStatusPayload(void *pObject, char *pszHostName) {
    NETWORK_HandleGroupAdjustorSlot *pAdjustorSlot;
    char szNumber[16];
    char *pszTarget;
    int cbText;

    pAdjustorSlot =
        (NETWORK_HandleGroupAdjustorSlot *)((char *)pObject + ((NETWORK_EffTransportPeer *)pObject)->m_pOffsets->m_nHandleGroupAdjustorSlotOffset);
    ConfigureEffStreamPrimaryHandleGroup((char *)&pAdjustorSlot->m_Adjustor + pAdjustorSlot->m_Adjustor.m_pOffsets->m_nHandleStreamOffset,
                                         3, 3, 0);
    ReleaseTimedEffStreamPrimaryHandle((int)(unsigned long)((char *)&pAdjustorSlot->m_Adjustor +
                                                            pAdjustorSlot->m_Adjustor.m_pOffsets->m_nTimedStreamOffset));

    pAdjustorSlot =
        (NETWORK_HandleGroupAdjustorSlot *)((char *)pObject + ((NETWORK_EffTransportPeer *)pObject)->m_pOffsets->m_nHandleGroupAdjustorSlotOffset);
    ConfigureEffStreamSecondaryHandleGroup((char *)&pAdjustorSlot->m_Adjustor + pAdjustorSlot->m_Adjustor.m_pOffsets->m_nHandleStreamOffset,
                                           1, 0);
    ConfigureTimedEffStreamSecondaryHandle((char *)&pAdjustorSlot->m_Adjustor +
                                               pAdjustorSlot->m_Adjustor.m_pOffsets->m_nTimedStreamOffset,
                                           1);

    if (g_pszEffTransportBroadcastStatusPayload == 0) {
        g_pszEffTransportBroadcastStatusPayload = (char *)AllocateVSMemBlock(0x410);
        pszTarget = g_pszEffTransportBroadcastStatusPayload + 0x10;

        strcpy(pszTarget, "ViSOS VSNET v");
        FormatSignedIntToRadixString(*(int *)0x004A1E08, szNumber, 10);
        strcat(pszTarget, szNumber);
        strcat(pszTarget, ".");
        FormatSignedIntToRadixString(*(int *)0x004A1E0C, szNumber, 10);
        strcat(pszTarget, szNumber);
        strcat(pszTarget, " ");
        strcat(pszTarget, pszHostName);
        strcat(pszTarget, " is Broadcasting.");

        cbText = (int)strlen(pszTarget);
        g_cbEffTransportBroadcastStatusPayload = cbText;
    }
}

// FUNCTION: LEMBALL 0x004607F0
void ReleaseGlobalEffTransportBuffer(void *pObject) {
    NETWORK_ChannelOwnerObject *pChannelOwner;
    NETWORK_TransportEndpointSlot *pEndpointSlot;
    NETWORK_EndpointState *pEndpointState;

    if (g_pszFileBasedNetworkLocalHostName != 0) {
        FreeVSMemBlock(g_pszFileBasedNetworkLocalHostName);
        g_pszFileBasedNetworkLocalHostName = 0;
    }

    pChannelOwner = (NETWORK_ChannelOwnerObject *)pObject;
    pEndpointSlot =
        (NETWORK_TransportEndpointSlot *)((char *)pChannelOwner + pChannelOwner->m_pOffsets->m_nEndpointStateSlotOffset);
    pEndpointState = &pEndpointSlot->m_State;
    ResetEffTransportHandleGroup((int *)pEndpointState);
}

// FUNCTION: LEMBALL 0x00460830
void DispatchPrefixedEffTransportControlStream(void *pObject, int nDispatchType, void *pEvent) {
    NETWORK_EffDispatchEventPayloadView *pEventView;
    NETWORK_EffPrefixedControlDispatchOwner *pOwner;
    NETWORK_RuntimeServiceObject *pPeerAddressState;
    char *pszPayloadText;
    int nIsLocalPeer;
    int nPayloadBuffer;

    pOwner = (NETWORK_EffPrefixedControlDispatchOwner *)((char *)pObject - 0xd8);
    pEventView = (NETWORK_EffDispatchEventPayloadView *)pEvent;
    pPeerAddressState = (NETWORK_RuntimeServiceObject *)g_pEffTransportPeerAddressState;
    ((void (*)(void))**(void ***)pPeerAddressState)();
    nIsLocalPeer =
        pPeerAddressState->m_pVtable->m_pValidatePeerKey(g_pEffTransportRuntimeService);

    nPayloadBuffer = pEventView->m_nPayloadBuffer04;
    pszPayloadText = (char *)(unsigned long)(nPayloadBuffer + 0x10);
    if (nIsLocalPeer == 0 &&
        ((pOwner->m_nPeerMode18 != 2 ||
          (pOwner->m_pPeerKey28 != 0 && pPeerAddressState->m_pVtable->m_pValidatePeerKey(pOwner->m_pPeerKey28) != 0)) &&
         strncmp(pszPayloadText, g_pszEffTransportBroadcastStatusPayload + 0x10,
                 g_cbEffTransportBroadcastStatusPayload + 1) == 0)) {
        DispatchEffTransportConnectControlStream(g_pActiveNetworkRuntimeWindow, g_pEffTransportPeerAddressState,
                                                 nPayloadBuffer + g_cbEffTransportBroadcastStatusPayload + 0x11);
    }

    if (nDispatchType == 7) {
        pEventView->m_pOwnedPayload24 = 0;
    }
}

// FUNCTION: LEMBALL 0x004608F0
void LoadEffStreamFromGlobalRange(void *pStream) {
    SaveEffStreamToMemoryRange(pStream, (int)(unsigned long)g_pszEffTransportBroadcastStatusPayload,
                               g_cbEffTransportBroadcastStatusPayload + 0x11);
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
void WINAPI ConfigureFileBasedNetworkPathsWrapper(char *pszLocalHost, char *pszBasePath) {
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
