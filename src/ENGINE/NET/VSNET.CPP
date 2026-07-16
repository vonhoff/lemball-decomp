#include "GAME.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/GDI/VSINVIS.H"
#include "ENGINE/NET/VSNET.H"
#include "ENGINE/MEDIA/VSSTRM.H"
#include "ENGINE/CORE/SAFEVT.H"

extern void *g_GAME_EffStreamConstructionVtable[6];

#include <string.h>
#include <new>

extern int g_nEffTransportAsyncErrorStatus;
extern int PASCAL ReturnTrueVtableCallback(void *pObject);
extern int PASCAL ReturnTrueVtableCallbackThunk(void *pObject);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void NoopVtableCallbackThunk(void);
extern void ReturnVoidVtableCallback(void);
extern void *g_NETWORK_RuntimeChannelStackVtable;
extern void LEMBALL_FASTCALL ReadEffStreamTwoU16Fields(void *pObject);
extern void *DeleteEffStreamBaseWrapper00462810(void *pObject, BYTE fDelete);
extern void *DeleteEffStreamBaseWrapper00462830(void *pObject, BYTE fDelete);
extern void *DeleteEffStreamBaseWrapper00462850(void *pObject, BYTE fDelete);
extern void *DeleteEffStreamBaseWrapper00462870(void *pObject, BYTE fDelete);
extern void *DeleteEffStreamBaseWrapper00462890(void *pObject, BYTE fDelete);
extern void *DeleteEffStreamBaseWrapper004628B0(void *pObject, BYTE fDelete);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pEffTransportDispatchQueue;
extern int DrainRenderDispatchQueueEntries(void *pDispatchQueue, unsigned int cEntries);
extern void *AllocateTcpipEffTransportRuntimeOwner(void);
extern void *AllocateTcpipEffTransportComposite(void);

struct VsNetLockedEffStreamFileRangeView {
    int LoadLockedEffStreamFromFileRange(void);
    int WriteLockedEffStreamToFileRange(void);
};

struct CFileNetworkAddress {
    virtual char *GetAddressString(void);
    virtual void SelectAddressType(int nType);
    virtual void CopyAddressFromString(const char *pszAddress);
    virtual void CopyAddressObject(CFileNetworkAddress *pSource);
    virtual int CompareAddress(CFileNetworkAddress *pOther);
    virtual int InitializeAddress(int nUnused);
};

// FUNCTION: LEMBALL 0x0046F920
char *CFileNetworkAddress::GetAddressString(void) {
    return (char *)this + 4;
}

// FUNCTION: LEMBALL 0x004794E0
void CFileNetworkAddress::SelectAddressType(int nType) {
    if (nType == 0) {
        strcpy((char *)this + 4, "LAN");
    } else if (nType == 1) {
        strcpy((char *)this + 4, "WAN");
    }
}

// FUNCTION: LEMBALL 0x0046F8B0
void CFileNetworkAddress::CopyAddressFromString(const char *pszAddress) {
    strcpy((char *)this + 4, pszAddress);
}

// FUNCTION: LEMBALL 0x0046F880
void CFileNetworkAddress::CopyAddressObject(CFileNetworkAddress *pSource) {
    strcpy((char *)this + 4, (char *)pSource + 4);
}

// FUNCTION: LEMBALL 0x0046F8E0
int CFileNetworkAddress::CompareAddress(CFileNetworkAddress *pOther) {
    return strcmp((char *)this + 4, (char *)pOther + 4) == 0;
}

// FUNCTION: LEMBALL 0x00479510
int CFileNetworkAddress::InitializeAddress(int nUnused) {
    (void)nUnused;
    AppendCStringToStream(
        g_pErrorOutputStream,
        "You haven't implemented major/minor distinction in 'CFileNetworkAddress'es\n");
    AppendCStringToStream(
        g_pErrorOutputStream,
        "use count entry in broadcast file\n");
    return 0;
}

static CFileNetworkAddress g_CFileNetworkAddressVtableModel;
static void *g_CFileNetworkAddressVtable =
    *(void ***)&g_CFileNetworkAddressVtableModel;

// FUNCTION: LEMBALL 0x0046F860
static void *AllocateFileNetworkAddress(void) {
    CFileNetworkAddress *pObject;

    pObject = (CFileNetworkAddress *)AllocateVSMemBlock(0x20);
    if (pObject == 0) {
        return 0;
    }
    *(void ***)pObject = (void **)g_CFileNetworkAddressVtable;
    *((char *)pObject + 4) = 0;
    return pObject;
}

extern "C" BOOL WINAPI WaitMessage(void);
extern "C" BOOL WINAPI KillTimer(HWND hWnd, unsigned int uIDEvent);
extern "C" unsigned int WINAPI SetTimer(HWND hWnd, unsigned int uIDEvent,
                                         UINT uElapse, void *lpTimerFunc);
extern "C" int WINAPI WSAStartup(unsigned short nVersion, void *pData);
extern "C" int WINAPI WSAGetLastError(void);
extern "C" int WINAPI WSACleanup(void);
extern "C" HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES pThreadAttributes,
                                      DWORD cbStack,
                                      DWORD(WINAPI *pfnStartAddress)(LPVOID),
                                      LPVOID pvThreadParam,
                                      DWORD dwCreationFlags,
                                      LPDWORD pdwThreadId);
extern "C" BOOL WINAPI SetThreadPriority(HANDLE hThread, int nPriority);
extern "C" void WINAPI ExitProcess(UINT uExitCode);

struct VsNetGlobalStateCompositeLayout {
    void *ConstructGlobalStateFileBackedEffComposite(int fConstructEmbeddedObjects);
};
struct VsNetDualSlotCompositeLayout {
    void *ConstructDualSlotTableFileBackedEffComposite(int fConstructEmbeddedObjects);
};

/*
 * The original tables live in the image's read-only data.  Keeping these
 * globals as zero-initialized placeholders makes the first message-thread
 * callback an indirect call through address zero.  Until each transport
 * table is recovered, use compiler-owned fallback tables.  The fallback
 * reports an unavailable transport and leaves the process in the normal
 * startup error path; it never fabricates a peer or calls through null.
 */
static void NetworkRuntimeTransportUnavailable(void *pObject) {
    /* Runtime state starts with the render node; shutdown is at +0x14. */
    *(int *)((char *)pObject + 0x14) = 1;
    g_nEffTransportAsyncErrorStatus = 1;
}

static void NetworkRuntimeTransportNoop(void *pObject) {
    (void)pObject;
}

static int NetworkRuntimeCreateNoChannelOwner(void *pObject) {
    (void)pObject;
    return 0;
}

static void *NetworkRuntimeWindowCreateNoChannelOwner(void *pObject) {
    (void)pObject;
    return 0;
}

struct VsNetRuntimeFallbackTransportVtable {
    void *m_aReserved00[3];
    void (*m_pInitializeTransport)(void *);
    void *m_aReserved10[4];
    void (*m_pServiceTransport)(void *);
    void *m_pReserved24;
    int (*m_pCreateChannelOwner)(void *);
    void *m_pReserved2c;
    void (*m_pWaitForTransportMessage)(void *);
};

struct VsNetRuntimeFallbackWindowVtable {
    void *m_aReserved00[11];
    void *(*m_pCreateChannelOwner)(void *);
};

static VsNetRuntimeFallbackTransportVtable g_NETWORK_RuntimeFallbackTransportVtable = {
    /* Emergency table: every dispatch slot must be callable on failure. */
    { (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop },
    NetworkRuntimeTransportUnavailable,
    { (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop },
    NetworkRuntimeTransportNoop,
    (void *)NetworkRuntimeTransportNoop,
    NetworkRuntimeCreateNoChannelOwner,
    (void *)NetworkRuntimeTransportNoop,
    NetworkRuntimeTransportNoop,
};

static VsNetRuntimeFallbackWindowVtable g_NETWORK_RuntimeFallbackWindowVtable = {
    { (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop,
      (void *)NetworkRuntimeTransportNoop },
    NetworkRuntimeWindowCreateNoChannelOwner,
};

struct VsNetEffTransportRuntimeState;
// FUNCTION: LEMBALL 0x00471480
struct VsNetTcpipRuntimeWindowVtableModel {
    virtual int HandleTcpipRuntimeWindowMessage(unsigned int nMessage,
                                                unsigned int nWParam,
                                                unsigned int nLParam) {
        VsNetEffTransportRuntimeState *pState;
        void **ppStateVtable;
        void (*pStateCallback)(void *);
        void *pDispatchQueue;

        (void)nWParam;
        (void)nLParam;
        if (nMessage != 0x444 && nMessage != 0x113) {
            return -1;
        }
        if (nMessage == 0x444) {
            pDispatchQueue = g_pEffTransportDispatchQueue;
            if (pDispatchQueue != 0 && *(int *)((char *)pDispatchQueue + 0x28) != 0) {
                DrainRenderDispatchQueueEntries(
                    pDispatchQueue, *(unsigned int *)((char *)pDispatchQueue + 0x28));
            }
        }
        pState = (VsNetEffTransportRuntimeState *)
            ((char *)this + 0x10);
        ppStateVtable = *(void ***)pState;
        pStateCallback = (void (*)(void *))ppStateVtable[5];
        pStateCallback(pState);
        return 0;
    }
};
static VsNetTcpipRuntimeWindowVtableModel
    g_NETWORK_TcpipRuntimeWindowVtableModel;

static void *g_NETWORK_TcpipRuntimeWindowFallbackVtable[32] = {
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
};

/* These are table addresses.  Do not load first slot from table: that
 * produces code address, not dispatch-table address. */
static VsNetRuntimeFallbackTransportVtable *g_NETWORK_FileBasedRuntimeTransportVtable =
    &g_NETWORK_RuntimeFallbackTransportVtable;
void *g_NETWORK_TcpipRuntimeWindowVtable =
    g_NETWORK_TcpipRuntimeWindowFallbackVtable;
static VsNetRuntimeFallbackTransportVtable *g_NETWORK_TcpipRuntimeTransportVtable =
    &g_NETWORK_RuntimeFallbackTransportVtable;
static void *g_NETWORK_EffTransportGlobalWriteStreamVtable[4] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReturnVoidVtableCallback,
    (void *)NoopVtableCallbackThunk,
};
static void *g_NETWORK_EffTransportGlobalReadStreamVtable[4] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReadEffStreamTwoU16Fields,
    (void *)NoopVtableCallbackThunk,
};
int CompareEffTransportRequestStreamName(void *pObject);
void WriteEffTransportRequestStreamName(void *pObject);
void __fastcall LoadEffTransportRequestConnectStream(void *pObject);
void __fastcall WriteEffTransportRequestPortFields(void *pObject);
void LEMBALL_FASTCALL LoadEffTransportRequestNewPortStream(void *pObject);
void LEMBALL_FASTCALL WriteEffTransportRequestNewPortStream(void *pObject);
void __fastcall LoadEffTransportAuthoriseConnectStream(void *pObject);
void LEMBALL_FASTCALL WriteEffTransportAuthoriseConnectStream(void *pObject);
void __fastcall LoadEffTransportGoAheadConnectStream(void *pObject);
void LEMBALL_FASTCALL WriteEffTransportGoAheadConnectStream(void *pObject);
void LEMBALL_FASTCALL LoadEffTransportFailedConnectStream(void *pObject);
void LEMBALL_FASTCALL WriteEffTransportFailedConnectStream(void *pObject);

static void *g_NETWORK_RequestConnectControlStreamNameVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)CompareEffTransportRequestStreamName,
    (void *)_purecall,
    (void *)WriteEffTransportRequestStreamName,
    (void *)_purecall,
    (void *)DeleteEffStreamBaseWrapper00462810,
};
static void *g_NETWORK_RequestConnectControlStreamVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk, (void *)CompareEffTransportRequestStreamName,
    (void *)LoadEffTransportRequestConnectStream, (void *)WriteEffTransportRequestStreamName,
    (void *)WriteEffTransportRequestPortFields, (void *)DeleteEffStreamBaseWrapper00462830,
};
static void *g_NETWORK_RequestNewPortControlStreamVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk, (void *)CompareEffTransportRequestStreamName,
    (void *)LoadEffTransportRequestNewPortStream, (void *)WriteEffTransportRequestStreamName,
    (void *)WriteEffTransportRequestNewPortStream, (void *)DeleteEffStreamBaseWrapper00462850,
};
static void *g_NETWORK_AuthoriseConnectControlStreamVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk, (void *)CompareEffTransportRequestStreamName,
    (void *)LoadEffTransportAuthoriseConnectStream, (void *)WriteEffTransportRequestStreamName,
    (void *)WriteEffTransportAuthoriseConnectStream, (void *)DeleteEffStreamBaseWrapper00462870,
};
static void *g_NETWORK_GoAheadConnectControlStreamVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk, (void *)CompareEffTransportRequestStreamName,
    (void *)LoadEffTransportGoAheadConnectStream, (void *)WriteEffTransportRequestStreamName,
    (void *)WriteEffTransportGoAheadConnectStream, (void *)DeleteEffStreamBaseWrapper00462890,
};
static void *g_NETWORK_FailedConnectControlStreamVtable[6] = {
    (void *)ReturnTrueVtableCallbackThunk, (void *)CompareEffTransportRequestStreamName,
    (void *)LoadEffTransportFailedConnectStream, (void *)WriteEffTransportRequestStreamName,
    (void *)WriteEffTransportFailedConnectStream, (void *)DeleteEffStreamBaseWrapper004628B0,
};

struct VsNetRenderQueueNode {
    void **m_pVtable;
    int m_uMagic;
    void *m_pReserved08;
    void *m_pReserved0C;
};

struct VsNetActiveRuntimeWindow {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0c(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved1c(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void Reserved24(void) = 0;
    virtual void Reserved28(void) = 0;
    virtual void *CreateChannelOwner(void) = 0;
};

struct VsNetConnectRequestPeerKey {
    virtual void Reserved00(void) = 0;
    virtual void SetConnectionMode(int nMode) = 0;
    virtual void Reserved08(void) = 0;
    virtual void SetPeerKey(void *pPeerKey) = 0;
};

struct VsNetChannelOwnerVtable {
    short (*m_pAllocatePort)(void *);
    void (*m_pReleasePort)(short);
    void *m_pReserved08;
    int (*m_pBindRuntimeKey)(void *, int);
    void (*m_pShutdown)(void *);
    void *m_pReserved14;
    void (*m_pLockPeerList)(void *);
    void (*m_pUnlockPeerList)(void *);
    int (*m_pAllocatePeer)(void *);
};

struct VsNetTransportEndpointOffsets;

struct VsNetChannelOwnerObject {
    VsNetChannelOwnerVtable *m_pVtable;
    VsNetTransportEndpointOffsets *m_pOffsets;
    int m_nSendGate;
    int m_nReserved0C;
    unsigned char *m_pbAllocatedPorts;

    void SendLoadedEffEventToPeer(void *pPeerKey, void *pStream);
    void ClearEffTransportSendGate(void);
    void ReleaseGlobalEffTransportBuffer(void);
    void ServiceEffTransportConnectRequest(void);
};

struct VsNetTransportEndpointOffsets {
    void *m_pReserved00;
    int m_nEndpointStateSlotOffset;
    int m_nPayloadSenderSlotOffset;
    int m_nReserved0C;
    int m_nHandleGroupAdjustorSlotOffset;
};

struct VsNetEffTransportHandleCallbackVtable {
    int (*m_pBeginReset)(void);
    void *m_pReserved04;
    void (*m_pFinishReset)(void);
    void (*m_pNotifyClosed)(int);
};

struct VsNetEffTransportHandleCallback {
    VsNetEffTransportHandleCallbackVtable *m_pVtable;
};

struct VsNetEffTransportHandleGroup {
    VsNetEffTransportHandleCallback *m_pCallback;
    int m_nReserved04;
    int m_nReserved08;
    int m_fPrimaryHandlePresent;
    int m_fSecondaryHandlePresent;
};

struct GameEffTransportHandleGroup {
    void Reset(void);
};

struct GameEffStream {
    void ResetStateFields(void);
    int LoadEffStreamFromMemory(int nSourceBuffer);
    void BeginEffStreamWriteSession(void);
    void EndEffStreamWriteSession(void);
};

struct VsNetEndpointState {
    int m_nReserved00;
    VsNetEffTransportHandleGroup m_HandleGroup;
    int m_fPrimaryHandleActive;
    int m_fSecondaryHandleActive;
    short m_nAssignedPort;
};

struct VsNetTransportEndpointSlot {
    char m_abUnknown00[4];
    VsNetEndpointState m_State;
};

struct VsNetTransportPeerMatcher {
    void **m_pVtable;
    char m_abUnknown04[0x70];
};

struct VsNetTransportPeerMatcherVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void *m_pReserved08;
    void *m_pReserved0C;
    int (*m_pMatchPeerKey)(void *);
};

struct VsNetHandleGroupAdjustorOffsets {
    void *m_pReserved00;
    void *m_pReserved04;
    int m_nTimedStreamOffset;
    int m_nHandleStreamOffset;
};

struct VsNetHandleGroupAdjustor {
    VsNetHandleGroupAdjustorOffsets *m_pOffsets;
};

struct VsNetHandleGroupAdjustorSlot {
    char m_abUnknown00[4];
    VsNetHandleGroupAdjustor m_Adjustor;
};

struct VsNetEffTransportPeer;
struct VsNetSimpleEffTransportRecordSlotTable;

struct VsNetPeerPayloadSenderStateOffsets {
    void *m_pReserved00;
    int m_nSendStateBaseOffset04;
};

struct VsNetPeerPayloadSenderRuntimeStateView {
    char m_abReserved00[0x44];
    VsNetEffTransportHandleCallback *m_pHandleCallback44;
    char m_abReserved48[0x08];
    int m_fRetryServiceEnabled50;
    char m_abReserved54[0x08];
    int m_fKeepaliveEnabled5c;
    char m_abReserved60[0x08];
    int m_fTransportActive68;
};

struct VsNetPeerPayloadSenderPendingWriteState {
    int *m_pBusyStream00;
    int m_nReserved04;
    int m_nPendingFragmentSequence08;
    int m_nPendingFragmentIndex0c;
};

struct VsNetPeerPayloadSenderState {
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
    VsNetPeerPayloadSenderStateOffsets *m_pOffsets44;
    int m_nReserved48;
    VsNetSimpleEffTransportRecordSlotTable *m_pRecordTable4c;
    int m_nReserved50;
    void *m_pPendingFragmentedStream54;
    int m_nPendingFragmentSequence58;
    int m_nPendingFragmentIndex5c;
    unsigned int m_aFragmentPrefixWords60[4];

    int SendReliableEffTransportPayload(void *pStream);
    int SendFragmentedEffTransportPayload(void *pStream);
    void ServiceEffTransportRetries(void);
};

struct VsNetEffStreamPayloadState {
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

struct VsNetEffTransportPacketHeader {
    int m_nReserved00;
    int m_cbPayload04;
    unsigned short m_nStreamEvent08;
    unsigned short m_wSequence0a;
    unsigned short m_wFragmentIndex0c;
    unsigned char m_fReliable0e;
    unsigned char m_abReserved0f[3];
};

struct VsNetPeerPayloadSenderSlot {
    char m_abUnknown00[4];
    VsNetPeerPayloadSenderRaw m_Sender;
    char m_abUnknown08[0x6c];
    VsNetTransportPeerMatcher *m_pMatcher;
};

struct VsNetDeleteObjectVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void *m_pReserved08;
    void *m_pReserved0C;
    void *m_pReserved10;
    void (*m_pDelete)(int);
};

struct VsNetDeleteObject {
    VsNetDeleteObjectVtable *m_pVtable;
};

struct VsNetRuntimeServiceVtable {
    char *(*m_pGetLocalHostName)(void);
    void *m_pReserved04;
    void (*m_pSetLocalHostName)(char *);
    void *m_pReserved0C;
    void *m_pReserved10;
    int (*m_pValidatePeerKey)(void *);
};

struct VsNetRuntimeServiceObject {
    VsNetRuntimeServiceVtable *m_pVtable;
};

struct VsNetEffControlStreamHeader {
    void *m_pVtable;
    int m_nEventCode;
    char m_abUnknown08[0x10];
    int m_cbSerializedLength;
    char m_abUnknown1C[0x10];
    char *m_pszStreamName;
};

struct VsNetRequestConnectControlStream {
    VsNetEffControlStreamHeader m_Header;
    short m_nBasePort;
    short m_nReserved32;
    void *m_pPortBinding;
    char *m_pszHostName;
};

struct VsNetRequestNewPortControlStream {
    VsNetRequestConnectControlStream m_Base;
    VsNetEffTransportPeer *m_pPeer;
};

struct VsNetAuthoriseConnectControlStream {
    VsNetEffControlStreamHeader m_Header;
    short m_nAssignedPort;
    short m_nReserved32;
    VsNetEffTransportPeer *m_pPeer;
};

struct VsNetFailedConnectControlStream {
    VsNetEffControlStreamHeader m_Header;
    char *m_pszFailureMessage;
};

struct VsNetEffTransportPeerVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void *m_pReserved08;
    void (*m_pSetPeerKey)(void *);
    void (*m_pCompleteConnect)(void);
};

struct VsNetEffDispatchEvent {
    unsigned short m_nType;
    unsigned short m_anReserved[3];
    int m_nCode;
    int *m_pStream;
    void *m_pPeer;
};

struct VsNetRuntimeWindowSendGateState {
    char m_abUnknown00[0x24];
    int m_nChannelOwner24;
    char m_abUnknown28[8];
    int m_fSendGateActive30;
};

struct VsNetEffTransportConnectOffsets {
    int m_nReserved00;
    int m_nSendStateViewOffset04;
    int m_nReliableTickViewOffset08;
    int m_nTimedTickViewOffset0c;
};

struct VsNetEffTransportConnectOwner {
    void *m_pVtable00;
    VsNetEffTransportConnectOffsets *m_pOffsets04;
    char m_abReserved08[0x0c];
    int m_nReserved14;
    int m_fTrackActivePeer1c;
    int m_fConnected24;
};

struct VsNetEffTransportConnectCallback {
    void QueueEffTransportConnectEvent(void);
};

struct VsNetEffTransportConnectStateView {
    char m_abReserved00[0x14];
    int m_fConnected14;
    int m_nCurrentState18;
    int m_fTrackActivePeer1c;
    int m_nPreviousState20;
};

struct VsNetEffTransportTickStateView {
    char m_abReserved00[0x40];
    DWORD m_dwLastTick40;
};

struct VsNetEffTransportAckDispatchOffsets {
    int m_nReserved00;
    int m_nReserved04;
    int m_nAckRecordOwnerViewOffset08;
};

struct VsNetEffTransportAckDispatchOwner {
    void *m_pVtable00;
    VsNetEffTransportAckDispatchOffsets *m_pOffsets04;
};

struct VsNetEffPrefixedControlDispatchOwner {
    char m_abReserved00[0x18];
    int m_nPeerMode18;
    char m_abReserved1c[0x0c];
    void *m_pPeerKey28;
};

struct VsNetEffDispatchEventPayloadView {
    char m_abReserved00[4];
    int m_nPayloadBuffer04;
    char m_abReserved08[0x1c];
    void *m_pOwnedPayload24;
};

struct VsNetEffTransportGlobalReadStream {
    void *m_pVtable;
    int m_nEventCode;
    char m_abUnknown08[0x24];
    unsigned short m_wSequence2c;
    unsigned short m_wPacketKind2e;
};

struct VsNetAckedEffTransportRecord {
    int m_nReserved00;
    int m_pSerializedStream04;
    unsigned char m_fClaimed08;
    char m_abUnknown09[3];
    void *m_pPayload0c;
};

struct VsNetAckedEffTransportRecordTable {
    int m_cRecords;
    int m_nReserved04;
    VsNetAckedEffTransportRecord **m_apRecords08;
};

struct VsNetAckedEffTransportRecordOwner {
    char m_abUnknown00[0x4c];
    VsNetAckedEffTransportRecordTable *m_pRecordTable4c;

    void *ClaimAckedEffTransportRecordPayload(void);
};

struct VsNetSimpleEffTransportRecordSlot {
    void **m_pVtable;
    int m_pSerializedStream04;
    unsigned char m_fAvailable08;
    char m_abUnknown09[3];
    void *m_pPayload0c;
    DWORD m_dwLastRetryTick10;
    unsigned char m_cRetryCount14;
    char m_abReserved15[3];

    void *ConstructSimpleEffTransportRecordSlot(unsigned int cbRecord);
    void DestroySimpleEffTransportRecordSlot(void);
    void CopyIntoSimpleEffTransportRecordSlot(const void *pvSource,
                                              unsigned int cbSource, void *pPayload);
};

struct VsNetSimpleEffTransportRecordSlotTable {
    int m_cRecords;
    int m_nReserved04;
    VsNetSimpleEffTransportRecordSlot **m_apRecords08;

    void *ConstructEffTransportRecordSlotTable(int cRecords, unsigned int cbRecord);
    void *ConstructLockedEffTransportRecordSlotTable(int cRecords, unsigned int cbRecord);
    void *ConstructSimpleEffTransportRecordSlotTable(int cRecords, unsigned int cbRecord);
    void *ConstructSimpleRecordSlotTableWrapper(int cRecords, unsigned int cbRecord);
    void CopyPayloadIntoSimpleRecordSlot(int nIndex, const void *pvSource,
                                         unsigned int cbSource, void *pPayload);
    unsigned char GetNextEffTransportRecordClaimFlag(unsigned int nSequence);
    void *ClaimSimpleRecordSlotForPacketId(unsigned int nPacketId,
                                           const void *pvSource, unsigned int cbSource,
                                           void *pPayload);
};

struct VsNetVSMemObjectPointerArray {
    int m_cObjects;
    int m_nReserved04;
    int m_pObjectArray08;

    void DestroyVSMemObjectPointerArray(void);
};

struct VsNetEffTransportRecordBuffer {
    void **m_pVtable;
    int m_pSerializedBuffer04;
    int m_cbPayload08;
    int m_fAllocatePayload0c;
    int m_nReserved10;
    int m_nReserved14;

    void *ConstructEffTransportRecordBuffer(int fAllocatePayload, int cbPayload,
                                            unsigned int cbRecord);
    void DestroyEffTransportRecordBuffer(void);
    void DestroyRangeEffTransportRecordBufferTable(void);
    void DestroyRingEffTransportRecordBufferTable(void);
};

static void *g_NETWORK_EffTransportRecordBufferVtable = 0;
static void *g_NETWORK_DeleteSimpleEffTransportRecordSlotVtable = 0;
static void *g_NETWORK_DeleteEffTransportRecordBufferVtable = 0;

struct VsNetLockedEffTransportRecordCriticalSection {
    VsNetLockedEffTransportRecordCriticalSection(void) {
        InitializeCriticalSection(m_abCriticalSection);
    }

    ~VsNetLockedEffTransportRecordCriticalSection(void) {
        DeleteCriticalSection(m_abCriticalSection);
    }

    virtual void Enter(void) {
        EnterCriticalSection(m_abCriticalSection);
    }

    virtual void Leave(void) {
        LeaveCriticalSection(m_abCriticalSection);
    }

    unsigned char m_abCriticalSection[0x18];
};

struct VsNetLockedEffTransportRecordSlot {
    VsNetLockedEffTransportRecordSlot(unsigned int cbPayload);

    virtual void Delete(unsigned char) {
    }

    void CopyIntoLockedEffTransportRecordSlot(const void *pvSource, unsigned int cbSource);
    void DestroyLockedEffTransportRecordSlot(void);

    void *m_pPayload04;
    VsNetLockedEffTransportRecordCriticalSection m_CriticalSection08;
    int m_fPayloadPresent24;
    int m_fAvailable28;
};


struct VsNetEffTransportPeer {
    void **m_pVtable;
    VsNetTransportEndpointOffsets *m_pOffsets;
    char *m_pszPeerName;
    char *m_pszPeerAddress;
    VsNetEffTransportPeer *m_pNext;
    VsNetEffTransportPeer *m_pPrevious;
    int m_cNewPortRequests;
    int m_nReserved1C;
    int m_fClosed;
    int m_fConnectComplete;
    DWORD m_dwConnectStartTick;

    void FreeEffTransportPeerBuffers(void);
    void SetEffTransportPeerNameAndPort(char *pszName, void *pKey, short nPort);
    int IsEffTransportPeerStillConnecting(void);
    void MarkEffTransportPeerActivityTime(void);
    int SendEffStreamToActivePeer(int *pStream);
    void Close(void);
    void ConfigureEffTransportConnectHostString(void *pHostName);
    void PrepareEffTransportBroadcastStatusPayload(char *pszHostName);
};

void VsNetCompositeEffTransportVtableModel::FatalSlot00(void) {
    _purecall();
}

void VsNetCompositeEffTransportVtableModel::FatalSlot02(void) {
    _purecall();
}

void VsNetCompositeEffTransportVtableModel::FatalSlot03(void) {
    _purecall();
}

static VsNetCompositeEffTransportVtableModel
    g_NETWORK_CompositeEffTransportVtableModel;
static void *g_NETWORK_CompositeEffTransportFallbackVtable[32] = {
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
    (void *)NetworkSafeVtableNoop, (void *)NetworkSafeVtableNoop,
};
void *g_NETWORK_CompositeEffTransportVtable =
    g_NETWORK_CompositeEffTransportFallbackVtable;

// FUNCTION: LEMBALL 0x00460FB0
void VsNetEffTransportPeer::Close(void) {
    VsNetTransportEndpointSlot *pEndpointSlot;
    VsNetEndpointState *pEndpointState;

    pEndpointSlot = (VsNetTransportEndpointSlot *)((char *)this +
                                                       m_pOffsets->m_nEndpointStateSlotOffset);
    pEndpointState = &pEndpointSlot->m_State;
    if (pEndpointState->m_fPrimaryHandleActive != 0 &&
        pEndpointState->m_fSecondaryHandleActive != 0) {
        ((GameEffTransportHandleGroup *)&pEndpointState->m_HandleGroup)->Reset();
        if (m_fConnectComplete != 0) {
            pEndpointState->m_HandleGroup.m_pCallback->m_pVtable->m_pNotifyClosed(1);
        }
    }
    m_fClosed = 1;
}

struct VsNetRenderDispatchQueue {
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

struct VsNetEffTransportRuntimeState {
    VsNetRenderQueueNode m_RenderQueueNode;
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
    int m_nReserved5c;
    int m_cbMaxPacketBytes;
    unsigned int m_nTimerId64;

    void *ConstructEffTransportRuntimeState(void);
    void InitializeFileBasedNetworkTransport(void);
    unsigned int GetNetworkRuntimeTimerId(void);
    void ServiceFileBasedNetworkTransport(void);
    void WaitForFileBasedNetworkMessage(void);
    void *CreateFileBasedChannelOwner(void);
    void FallbackInitializeTransport(void);
    void FallbackServiceTransport(void);
    void *FallbackCreateChannelOwner(void);
    void FallbackWaitForTransportMessage(void);
    void InitializeTcpipNetworkTransport(void);
    void ShutdownTcpipNetworkTransport(void);
    void ServiceTcpipNetworkTransport(void);
    void *CreateTcpipChannelOwner(void);
    void *CreateTcpipSecondaryOwner(void);
    void CleanupEffTransportRuntimeState(void);
    int LoadActiveFileRange(void);
    int WriteActiveFileRange(void);
    int FallbackRuntimeStateSlot(void);
    void *CreateFileNetworkAddress(void);
    void *CreateGlobalStateFileBackedComposite(void);
    void *DeleteEffTransportRuntimeWindow(BYTE fFreeMemory);
    void DestroyEffTransportRuntimeGlobals(void);
    int InitializeEffTransportRuntimeGlobals(void);
    int ProcessEffTransportDispatchEvent(VsNetEffDispatchEvent *pEvent);
    int MarkExistingEffTransportPeerActive(int nPeer);
    int FindMatchingActiveEffTransportPeer(void *pPeerKey);
    void CloseMatchingEffTransportPeer(void *pPeerKey);
    void HandleEffTransportRequestConnect(void *pPeerKey);
    void HandleEffTransportRequestNewPort(void *pPeerKey);
    void DispatchEffTransportConnectControlStream(void *pPeerKey, int nSourceBuffer);
    void ForwardEffTransportFailedConnect(void *pPeerKey);
    void HandleEffTransportGoAheadConnect(void);
    int BroadcastEffStreamToActivePeers(int *pStream);
    void ReleaseEffTransportRuntimeOwnerState(void);
    void UnlinkAndDeleteEffTransportPeer(int nPeer);
};

struct VsNetEffTransportRuntimeOwnerDispatch {
    virtual void Slot00(void) = 0;
    virtual void Slot04(void) = 0;
    virtual void Slot08(void) = 0;
    virtual void Slot0C(void) = 0;
    virtual void ShutdownOwnerState(void) = 0;
    virtual void Slot14(void) = 0;
    virtual void LockPeerList(void) = 0;
    virtual void UnlockPeerList(void) = 0;
};

struct VsNetChannelOwnerDispatch {
    virtual short AllocatePort(void *pBinding) = 0;
    virtual void ReleasePort(short nPort) = 0;
    virtual void Slot08(void) = 0;
    virtual int BindRuntimeKey(void *pKey, int nMode) = 0;
    virtual void Shutdown(void) = 0;
    virtual void Slot14(void) = 0;
    virtual void LockPeerList(void) = 0;
    virtual void UnlockPeerList(void) = 0;
    virtual int AllocatePeer(void) = 0;
};

struct VsNetEffTransportRuntimeStateVtable {
    void *m_pReserved00;
    void *(VsNetEffTransportRuntimeState::*m_pDelete)(BYTE);
    int (VsNetEffTransportRuntimeState::*m_pProcess)(VsNetEffDispatchEvent *);
    void (VsNetEffTransportRuntimeState::*m_pInitializeTransport)(void);
    unsigned int (VsNetEffTransportRuntimeState::*m_pGetTimerId)(void);
    void (VsNetEffTransportRuntimeState::*m_pCleanupRuntimeState)(void);
    int (VsNetEffTransportRuntimeState::*m_pLoadFileRange)(void);
    int (VsNetEffTransportRuntimeState::*m_pWriteFileRange)(void);
    void (VsNetEffTransportRuntimeState::*m_pServiceTransport)(void);
    void *(VsNetEffTransportRuntimeState::*m_pCreateSecondaryOwner)(void);
    void *(VsNetEffTransportRuntimeState::*m_pCreateChannelOwner)(void);
    void *(VsNetEffTransportRuntimeState::*m_pCreateFileNetworkAddress)(void);
    void (VsNetEffTransportRuntimeState::*m_pWaitForTransportMessage)(void);
};

void *DeleteEffTransportRuntimeGlobalsWrapper(void *pObject, BYTE fFreeMemory);

/* Construction/cleanup table at 004991E0.  Its fatal slots are deliberate:
 * unsupported runtime-state operations terminate through the CRT wrapper. */
static void *g_NETWORK_RuntimeConstructionStateVtable[14] = {
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteEffTransportRuntimeGlobalsWrapper,
    (void *)NetworkSafeVtableNoop,
    (void *)_purecall,
    (void *)_purecall,
    (void *)NetworkSafeVtableNoop,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)NetworkSafeVtableNoop,
};

static VsNetEffTransportRuntimeStateVtable g_NETWORK_RuntimeFallbackStateVtable = {
    (void *)NetworkRuntimeTransportNoop,
    &VsNetEffTransportRuntimeState::DeleteEffTransportRuntimeWindow,
    &VsNetEffTransportRuntimeState::ProcessEffTransportDispatchEvent,
    &VsNetEffTransportRuntimeState::FallbackInitializeTransport,
    &VsNetEffTransportRuntimeState::GetNetworkRuntimeTimerId,
    &VsNetEffTransportRuntimeState::CleanupEffTransportRuntimeState,
    &VsNetEffTransportRuntimeState::FallbackRuntimeStateSlot,
    &VsNetEffTransportRuntimeState::FallbackRuntimeStateSlot,
    &VsNetEffTransportRuntimeState::FallbackServiceTransport,
    &VsNetEffTransportRuntimeState::CreateGlobalStateFileBackedComposite,
    &VsNetEffTransportRuntimeState::FallbackCreateChannelOwner,
    &VsNetEffTransportRuntimeState::CreateFileNetworkAddress,
    &VsNetEffTransportRuntimeState::FallbackWaitForTransportMessage,
};

static VsNetEffTransportRuntimeStateVtable g_NETWORK_FileBasedRuntimeStateVtable = {
    (void *)InitializeNonZrleVariantRenderEntry,
    &VsNetEffTransportRuntimeState::DeleteEffTransportRuntimeWindow,
    &VsNetEffTransportRuntimeState::ProcessEffTransportDispatchEvent,
    &VsNetEffTransportRuntimeState::InitializeFileBasedNetworkTransport,
    &VsNetEffTransportRuntimeState::GetNetworkRuntimeTimerId,
    &VsNetEffTransportRuntimeState::CleanupEffTransportRuntimeState,
    &VsNetEffTransportRuntimeState::LoadActiveFileRange,
    &VsNetEffTransportRuntimeState::WriteActiveFileRange,
    &VsNetEffTransportRuntimeState::ServiceFileBasedNetworkTransport,
    &VsNetEffTransportRuntimeState::CreateGlobalStateFileBackedComposite,
    &VsNetEffTransportRuntimeState::CreateFileBasedChannelOwner,
    &VsNetEffTransportRuntimeState::CreateFileNetworkAddress,
    &VsNetEffTransportRuntimeState::WaitForFileBasedNetworkMessage,
};

/* Raw table 0049A2A8: TCPIP state dispatch.  Slot +10 is teardown, whereas
 * the file table's slot +10 returns the timer id. */
struct VsNetTcpipRuntimeStateVtable {
    void *m_pReserved00;
    void *(VsNetEffTransportRuntimeState::*m_pDelete)(BYTE);
    int (VsNetEffTransportRuntimeState::*m_pProcess)(VsNetEffDispatchEvent *);
    void (VsNetEffTransportRuntimeState::*m_pInitializeTransport)(void);
    void (VsNetEffTransportRuntimeState::*m_pShutdownTransport)(void);
    void (VsNetEffTransportRuntimeState::*m_pCleanupRuntimeState)(void);
    int (VsNetEffTransportRuntimeState::*m_pLoadFileRange)(void);
    int (VsNetEffTransportRuntimeState::*m_pWriteFileRange)(void);
    void (VsNetEffTransportRuntimeState::*m_pServiceTransport)(void);
    void *(VsNetEffTransportRuntimeState::*m_pCreateSecondaryOwner)(void);
    void *(VsNetEffTransportRuntimeState::*m_pCreateChannelOwner)(void);
    void *(VsNetEffTransportRuntimeState::*m_pCreateFileNetworkAddress)(void);
    void (VsNetEffTransportRuntimeState::*m_pWaitForTransportMessage)(void);
};

VsNetTcpipRuntimeStateVtable g_NETWORK_TcpipRuntimeStateVtable = {
    (void *)NetworkRuntimeTransportNoop,
    &VsNetEffTransportRuntimeState::DeleteEffTransportRuntimeWindow,
    &VsNetEffTransportRuntimeState::ProcessEffTransportDispatchEvent,
    &VsNetEffTransportRuntimeState::InitializeTcpipNetworkTransport,
    &VsNetEffTransportRuntimeState::ShutdownTcpipNetworkTransport,
    &VsNetEffTransportRuntimeState::CleanupEffTransportRuntimeState,
    &VsNetEffTransportRuntimeState::FallbackRuntimeStateSlot,
    &VsNetEffTransportRuntimeState::FallbackRuntimeStateSlot,
    &VsNetEffTransportRuntimeState::ServiceTcpipNetworkTransport,
    &VsNetEffTransportRuntimeState::CreateTcpipSecondaryOwner,
    &VsNetEffTransportRuntimeState::CreateTcpipChannelOwner,
    &VsNetEffTransportRuntimeState::FallbackCreateChannelOwner,
    &VsNetEffTransportRuntimeState::WaitForFileBasedNetworkMessage,
};

static VsNetEffTransportRuntimeStateVtable *g_NETWORK_EffTransportRuntimeStateVtable =
    (VsNetEffTransportRuntimeStateVtable *)g_NETWORK_RuntimeConstructionStateVtable;

struct VsNetRuntimeStartView {
    int StartEffTransportRuntimeAndWaitReady(int nRuntimeKey, int cbMaxPacket);
};

struct VsNetRuntimeWindowBase {
    char m_abWindowBase[0x10];
    VsNetEffTransportRuntimeState m_RuntimeState;

    void *ConstructTcpipNetworkRuntimeWindow(void);
    void ScheduleNetworkRuntimeTimerEvent(UINT nMilliseconds);
};

struct VsNetFileBasedRuntimeWindow {
    VsNetRuntimeWindowBase m_Base;
    int m_nReserved78;

    void *ConstructFileBasedNetworkRuntimeWindow(void);
};

struct VsNetFileRuntimeWindowVtableModel {
    // FUNCTION: LEMBALL 0x0046F7E0
    virtual int HandleFileRuntimeWindowMessage(unsigned int nMessage, unsigned int nWParam, unsigned int nLParam) {
        VsNetFileBasedRuntimeWindow *pWindow;
        VsNetEffTransportRuntimeState *pState;
        VsNetEffTransportRuntimeStateVtable *pStateVtable;
        (void)nWParam;
        (void)nLParam;
        pWindow = (VsNetFileBasedRuntimeWindow *)this;
        if (nMessage != 0x444 && nMessage != 0x113) {
            return -1;
        }
        if (nMessage == 0x444) {
            if (pWindow->m_nReserved78 != 0) {
                pWindow->m_Base.ScheduleNetworkRuntimeTimerEvent(0x14);
            }
            if (g_pEffTransportDispatchQueue != 0 &&
                *(int *)((char *)g_pEffTransportDispatchQueue + 0x28) != 0) {
                DrainRenderDispatchQueueEntries(
                    g_pEffTransportDispatchQueue,
                    *(unsigned int *)((char *)g_pEffTransportDispatchQueue + 0x28));
            }
        }
        pState = &pWindow->m_Base.m_RuntimeState;
        pStateVtable = (VsNetEffTransportRuntimeStateVtable *)
            pState->m_RenderQueueNode.m_pVtable;
        (pState->*pStateVtable->m_pCleanupRuntimeState)();
        return 0;
    }
    virtual char *GetAddressString(void) {
        return ((CFileNetworkAddress *)this)
            ->CFileNetworkAddress::GetAddressString();
    }
    virtual void SelectAddressType(int nType) {
        ((CFileNetworkAddress *)this)
            ->CFileNetworkAddress::SelectAddressType(nType);
    }
    virtual void CopyAddressFromString(const char *pszAddress) {
        ((CFileNetworkAddress *)this)
            ->CFileNetworkAddress::CopyAddressFromString(pszAddress);
    }
    virtual void CopyAddressObject(void *pSource) {
        ((CFileNetworkAddress *)this)->CFileNetworkAddress::CopyAddressObject(
            (CFileNetworkAddress *)pSource);
    }
    virtual int CompareAddress(void *pOther) {
        return ((CFileNetworkAddress *)this)->CFileNetworkAddress::CompareAddress(
            (CFileNetworkAddress *)pOther);
    }
    virtual int InitializeAddress(int nUnused) {
        return ((CFileNetworkAddress *)this)
            ->CFileNetworkAddress::InitializeAddress(nUnused);
    }
    virtual void Reserved07(void) {}
    virtual void Reserved08(void) {}
    virtual void Reserved09(void) {}
    virtual void Reserved0A(void) {}
    virtual void Reserved0B(void) {}
    virtual void Reserved0C(void) {}
    virtual void Reserved0D(void) {}
    virtual void Reserved0E(void) {}
    virtual void Reserved0F(void) {}
};

static VsNetFileRuntimeWindowVtableModel g_NETWORK_FileRuntimeWindowVtableModel;
static void *g_NETWORK_FileBasedRuntimeWindowVtable =
    *(void ***)&g_NETWORK_FileRuntimeWindowVtableModel;

char *g_pszEffTransportBroadcastStatusPayload = 0;
static int g_nEffTransportVersionMajor = 0;
static int g_nEffTransportVersionMinor = 9;
static int g_nTcpipWsaError = 0;
static int g_nTcpipMaxUdpDatagram = 0;
static const char g_szEffTransportBroadcastPrefix[] = "ViSOS (VSNET v";
static const char g_szEffTransportVersionSeparator[] = ".";
static const char g_szEffTransportVersionSpace[] = " ";
static const char g_szEffTransportBroadcastSuffix[] = ") is Broadcasting.";
char *g_pszFileBasedNetworkLocalHostName = 0;
int g_cbEffTransportBroadcastStatusPayload = 0;
char *g_pszFileBasedNetworkConfiguredPath = 0;
int g_fFileBasedNetworkPathConfigured = 0;
short g_nEffTransportServiceBasePort = 0;
// GLOBAL: LEMBALL 0x004A1E4C
int g_nEffTransportAsyncErrorStatus = 0;
// GLOBAL: LEMBALL 0x004a1e24
int g_cbEffTransportMaxPacketBytes = 0;
void *g_pEffTransportRequestConnectControlStream = 0;
void *g_pEffTransportRequestNewPortControlStream = 0;
void *g_pEffTransportAuthoriseConnectControlStream = 0;
void *g_pEffTransportGoAheadConnectControlStream = 0;
void *g_pEffTransportFailedConnectControlStream = 0;

extern void *g_pEffTransportPacketBuffer;
extern void *g_pEffTransportPeerAddressState;
extern void *g_pEffTransportRuntimeService;

void *g_pEffTransportGlobalWriteStream = 0;
void *g_pEffTransportGlobalReadStream = 0;
void *g_pEffTransportRuntimeService = 0;
int g_fFileBasedRuntimeWindowClassRegistered = 0;
int g_fTcpipRuntimeWindowClassRegistered = 0;
DWORD g_dwFileBasedNetworkThreadId = 0;
DWORD g_dwTcpipNetworkThreadId = 0;
HANDLE g_hFileBasedNetworkThread = 0;
HANDLE g_hTcpipNetworkThread = 0;

extern "C" DWORD WINAPI timeGetTime(void);

extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pEffTransportDispatchQueue;
extern void *g_pEffTransportSecondaryDispatchQueue;
extern void *LEMBALL_FASTCALL InitializeRenderQueueNodeBase(void *pRenderQueueNode);
extern void *ConstructRenderDispatchQueue(void *pQueue, int cEntries);
extern void *g_pNonZrleVariantRenderEntryInitializeVtable[2];
extern int DrainRenderDispatchQueueEntries(void *pDispatchQueue, unsigned int cEntries);
extern void CheckEffTransportIdleTimeout(void *pObject);

struct VsNetAdjustedEffTransportPeerView {
    void CloseAdjustedEffTransportPeerByKey(void *pUnused);
};

struct GameEffTransportRuntimeWindow {
    void UnregisterEffTransportEventClient(void);
};

// FUNCTION: LEMBALL 0x004605B0
void VsNetAdjustedEffTransportPeerView::CloseAdjustedEffTransportPeerByKey(void *pUnused) {
    char *pbAdjusted;
    void *pPeerKey;

    (void)pUnused;
    pbAdjusted = (char *)this;
    pbAdjusted += *(int *)(*(int *)(pbAdjusted - 0x20) + 8);
    pPeerKey = *(void **)(pbAdjusted + 0x50);
    ((VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow)
        ->CloseMatchingEffTransportPeer(pPeerKey);
}

struct VsNetErrorDispatchQueue {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void QueueEvent(void *pEvent) = 0;
};

// FUNCTION: LEMBALL 0x004609F0
void QueueEffTransportErrorEvent(int nError) {
    unsigned char abEvent[20];
    VsNetErrorDispatchQueue *pDispatchQueue;

    *(unsigned short *)abEvent = 2;
    *(int *)(abEvent + 8) = nError;
    pDispatchQueue = (VsNetErrorDispatchQueue *)g_pEffTransportDispatchQueue;
    pDispatchQueue->QueueEvent(abEvent);
}
extern void *g_pEffTransportPeerAddressState;
extern void *g_pEffTransportPacketBuffer;
extern int g_cbEffTransportCurrentPacketBytes;
extern void WINAPI LoadEffStreamFromGlobalRange(void *pStream);

struct VsNetGlobalPacketRecordBuffer {
    void *m_pReserved00;
    unsigned char *m_pbSerialized04;
    int m_cbFragmentStep08;
    char m_abReserved0c[4];
    int m_cbFragment10;
    int m_cbSerialized14;

    void AppendGlobalPacketIntoRecordBuffer(void);
};

struct VsNetGlobalPacketRangeState {
    char m_abReserved00[0x1c];
    int m_nFirstPacket1c;
    VsNetGlobalPacketRecordBuffer **m_appBuffers20;
    int m_nNextSequence24;

    void *AppendGlobalPacketToRangeRecordSlot(void);
};

struct VsNetGlobalPacketRingState {
    int m_cBuffers00;
    int m_nReserved04;
    VsNetLockedEffTransportRecordSlot **m_appBuffers08;
    int m_nNextSequence0c;
    int m_fOverflow10;

    void *ClaimOrderedGlobalPacketLockedSlot(void);
    void *ClaimNextReadyLockedRecordSlot(void);
};

struct VsNetGlobalPacketAppendRingState {
    char m_abReserved00[0x18];
    int m_cBuffers18;
    VsNetGlobalPacketRecordBuffer **m_appBuffers1c;

    void *AppendGlobalPacketToRingRecordSlot(void);
};

/* The locked-packet owner stores its record-slot pointer directly at +8. */
struct VsNetGlobalLockedPacketState {
    char m_abReserved00[8];
    VsNetLockedEffTransportRecordSlot **m_appSlots08;

    void CopyGlobalPacketIntoLockedRecordSlot(int nIndex);
    void *ClaimGlobalPacketLockedRecordSlot(void);
};

// FUNCTION: LEMBALL 0x004612F0
void VsNetGlobalLockedPacketState::CopyGlobalPacketIntoLockedRecordSlot(int nIndex) {
    m_appSlots08[nIndex]
        ->CopyIntoLockedEffTransportRecordSlot(
            g_pEffTransportPacketBuffer, (unsigned int)g_cbEffTransportCurrentPacketBytes);
}

// FUNCTION: LEMBALL 0x00461470
void VsNetGlobalPacketRecordBuffer::AppendGlobalPacketIntoRecordBuffer(void) {
    unsigned int cbPacket;
    unsigned int cbPayload;
    unsigned short nFragment;
    unsigned char *pbSource;
    unsigned char *pbTarget;

    cbPacket = (unsigned int)g_cbEffTransportCurrentPacketBytes;
    nFragment = *(unsigned short *)((char *)g_pEffTransportPacketBuffer + 0x0c);
    if (nFragment != 0) {
        if (m_cbFragment10 == 0) {
            memcpy(m_pbSerialized04, g_pEffTransportPacketBuffer, 0x10);
            m_cbSerialized14 += 0x10;
        }
        cbPayload = cbPacket - 0x10;
        pbSource = (unsigned char *)g_pEffTransportPacketBuffer + 0x10;
        pbTarget = m_pbSerialized04 + 0x10 + m_cbFragmentStep08 * nFragment;
        memcpy(pbTarget, pbSource, cbPayload);
        m_cbSerialized14 += cbPayload;
        ++m_cbFragment10;
        return;
    }
    if (m_cbFragment10 == 0) {
        memcpy(m_pbSerialized04, g_pEffTransportPacketBuffer, cbPacket);
        m_cbSerialized14 += cbPacket;
        ++m_cbFragment10;
        return;
    }
    cbPayload = cbPacket - 0x10;
    memcpy(m_pbSerialized04 + 0x10,
           (unsigned char *)g_pEffTransportPacketBuffer + 0x10, cbPayload);
    m_cbSerialized14 += cbPayload;
    ++m_cbFragment10;
}

// FUNCTION: LEMBALL 0x00461580
void *VsNetGlobalLockedPacketState::ClaimGlobalPacketLockedRecordSlot(void) {
    VsNetLockedEffTransportRecordSlot *pSlot;
    unsigned short nPacket;
    unsigned int nIndex;
    unsigned short nSequence;
    int fClaimed;

    nPacket = *(unsigned short *)((char *)g_pEffTransportPacketBuffer + 8);
    nIndex = nPacket;
    if (nPacket > 2) {
        nIndex -= 3;
    }
    pSlot = m_appSlots08[nIndex];
    fClaimed = 0;
    nSequence = *(unsigned short *)((char *)pSlot->m_pPayload04 + 0x0a);
    if (nPacket < 3 ||
        (*(unsigned short *)((char *)g_pEffTransportPacketBuffer + 0x0a) != nSequence &&
         (short)(*(unsigned short *)((char *)g_pEffTransportPacketBuffer + 0x0a) - nSequence) >= 0)) {
        fClaimed = pSlot->m_fPayloadPresent24 == 0;
        CopyGlobalPacketIntoLockedRecordSlot((int)nIndex);
    }
    if (fClaimed != 0) {
        return m_appSlots08[nIndex];
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00461700
void *VsNetGlobalPacketRangeState::AppendGlobalPacketToRangeRecordSlot(void) {
    int nIndex;
    VsNetGlobalPacketRecordBuffer *pBuffer;
    unsigned short nSequence;

    nIndex = (int)*(unsigned short *)((char *)g_pEffTransportPacketBuffer + 8) - m_nFirstPacket1c;
    pBuffer = m_appBuffers20[nIndex];
    if (*(unsigned short *)((char *)g_pEffTransportPacketBuffer + 8) > 2 &&
        (short)(*(unsigned short *)((char *)g_pEffTransportPacketBuffer + 0x0a) -
                *(unsigned short *)(pBuffer->m_pbSerialized04 + 0x0a)) < 0) {
        return 0;
    }
    nSequence = *(unsigned short *)((char *)g_pEffTransportPacketBuffer + 0x0a);
    if ((int)nSequence < m_nNextSequence24) {
        return 0;
    }
    if (*(unsigned short *)(pBuffer->m_pbSerialized04 + 0x0a) != nSequence &&
        pBuffer->m_cbFragment10 > 0) {
        m_nNextSequence24 = nSequence + 1;
        return 0;
    }
    pBuffer->AppendGlobalPacketIntoRecordBuffer();
    return m_appBuffers20[nIndex];
}

// FUNCTION: LEMBALL 0x004617A0
void *VsNetGlobalPacketRingState::ClaimOrderedGlobalPacketLockedSlot(void) {
    VsNetLockedEffTransportRecordSlot *pSlot;
    unsigned int nIndex;
    unsigned short nSequence;

    nSequence = *(unsigned short *)((char *)g_pEffTransportPacketBuffer + 0x0a);
    nIndex = (unsigned int)nSequence % (unsigned int)m_cBuffers00;
    pSlot = m_appBuffers08[nIndex];
    if (pSlot->m_fPayloadPresent24 == 0 && pSlot->m_fAvailable28 != 0) {
        pSlot->CopyIntoLockedEffTransportRecordSlot(
            g_pEffTransportPacketBuffer, (unsigned int)g_cbEffTransportCurrentPacketBytes);
        if (m_nNextSequence0c - (unsigned int)nSequence == -1) {
            ++m_nNextSequence0c;
            pSlot->m_fAvailable28 = 1;
            return pSlot;
        }
        pSlot->m_fAvailable28 = 0;
        return 0;
    }
    m_fOverflow10 = 1;
    return 0;
}

// FUNCTION: LEMBALL 0x00461810
void *VsNetGlobalPacketRingState::ClaimNextReadyLockedRecordSlot(void) {
    VsNetLockedEffTransportRecordSlot *pSlot;

    pSlot = m_appBuffers08[
        (unsigned int)m_nNextSequence0c % (unsigned int)m_cBuffers00];
    if (pSlot->m_fPayloadPresent24 != 0 && pSlot->m_fAvailable28 == 0) {
        ++m_nNextSequence0c;
        pSlot->m_fAvailable28 = 1;
        return pSlot;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00461930
void *VsNetGlobalPacketAppendRingState::AppendGlobalPacketToRingRecordSlot(void) {
    int nIndex;
    unsigned short nSequence;

    nSequence = *(unsigned short *)((char *)g_pEffTransportPacketBuffer + 0x0a);
    nIndex = (int)nSequence % m_cBuffers18;
    m_appBuffers1c[nIndex]->AppendGlobalPacketIntoRecordBuffer();
    return m_appBuffers1c[nIndex];
}

struct VsNetEffTransportPacketProcessorVtable {
    void *m_apReserved00[7];
    void (*m_pDispatchPacket)(void *, int, void *);
    void (*m_pSendHeader)(void *);
    void (*m_pNotifyControl)(void *);
};

struct VsNetEffTransportPacketProcessor {
    VsNetEffTransportPacketProcessorVtable *m_pVtable;
    char m_abReserved04[0x28];
    VsNetEffTransportPacketHeader *m_pPacketHeader2c;
    char m_abReserved30[0x14];
    void *m_pErrorOwner44;
    void *m_pLockedPackets48;
    VsNetGlobalPacketRangeState *m_pRangePackets4c;
    VsNetGlobalPacketRingState *m_pOrderedPackets50;
    VsNetGlobalPacketAppendRingState *m_pUnorderedPackets54;

    int ProcessEffTransportPacketHeader(void);
};

// FUNCTION: LEMBALL 0x0045FA40
int VsNetEffTransportPacketProcessor::ProcessEffTransportPacketHeader(void) {
    VsNetEffTransportPacketProcessor *pProcessor = this;
    VsNetEffTransportPacketHeader *pHeader;
    VsNetGlobalPacketRingState *pOrdered;
    void *pSlot;
    unsigned short wKind;
    unsigned short wSequence;

    pProcessor->m_pPacketHeader2c = (VsNetEffTransportPacketHeader *)g_pEffTransportPacketBuffer;
    ((GameEffStream *)this)->LoadEffStreamFromMemory((int)(unsigned long)g_pEffTransportPacketBuffer);
    pHeader = pProcessor->m_pPacketHeader2c;
    if (pHeader->m_nReserved00 != 0x56533039) {
        return 0;
    }
    wKind = pHeader->m_nStreamEvent08;
    if (wKind == 1) {
        return 0;
    }
    if (wKind == 2) {
        pProcessor->m_pVtable->m_pNotifyControl(pProcessor);
        return 0;
    }
    pOrdered = pProcessor->m_pOrderedPackets50;
    wSequence = pHeader->m_wSequence0a;
    if (pHeader->m_wFragmentIndex0c == 0x100) {
        if (pHeader->m_fReliable0e == 0) {
            pSlot = ((VsNetGlobalLockedPacketState *)pProcessor->m_pLockedPackets48)
                ->ClaimGlobalPacketLockedRecordSlot();
            if (pSlot != 0) {
                pProcessor->m_pVtable->m_pDispatchPacket(pProcessor, 7, pSlot);
                return 1;
            }
        } else {
            if ((int)wSequence <= pOrdered->m_nNextSequence0c) {
                ((VsNetEffTransportGlobalReadStream *)g_pEffTransportGlobalReadStream)->m_wSequence2c = wSequence;
                ((VsNetEffTransportGlobalReadStream *)g_pEffTransportGlobalReadStream)->m_wPacketKind2e = 0x100;
                pProcessor->m_pVtable->m_pSendHeader(pProcessor);
                return 0;
            }
            pSlot = pOrdered->ClaimOrderedGlobalPacketLockedSlot();
            if (pSlot != 0) {
                do {
                    pProcessor->m_pVtable->m_pDispatchPacket(pProcessor, 5, pSlot);
                    ((VsNetEffTransportGlobalReadStream *)g_pEffTransportGlobalReadStream)->m_wSequence2c =
                        *(unsigned short *)((char *)g_pEffTransportPacketBuffer + 0x0a);
                    ((VsNetEffTransportGlobalReadStream *)g_pEffTransportGlobalReadStream)->m_wPacketKind2e =
                        *(unsigned short *)((char *)g_pEffTransportPacketBuffer + 0x0c);
                    pProcessor->m_pVtable->m_pSendHeader(pProcessor);
                    pSlot = pOrdered->ClaimNextReadyLockedRecordSlot();
                } while (pSlot != 0);
                return 1;
            }
            if (pOrdered->m_fOverflow10 != 0) {
                /* The original calls an adjusted runtime error callback here. */
                void **ppOwner = (void **)pProcessor->m_pErrorOwner44;
                if (ppOwner != 0 && ppOwner[1] != 0) {
                    typedef void (*ErrorCallback)(void *, int);
                    ErrorCallback pError = (ErrorCallback)(*(void **)((char *)ppOwner[1] + 0x44));
                    if (pError != 0) {
                        pError((char *)pProcessor + (int)(unsigned long)ppOwner[1], 1);
                    }
                }
            }
        }
    } else if (pHeader->m_fReliable0e == 0) {
        pSlot = pProcessor->m_pRangePackets4c->AppendGlobalPacketToRangeRecordSlot();
        if (pSlot != 0) {
            pProcessor->m_pVtable->m_pDispatchPacket(pProcessor, 9, pSlot);
            return 1;
        }
    } else {
        pSlot = pProcessor->m_pUnorderedPackets54->AppendGlobalPacketToRingRecordSlot();
        if (pSlot != 0) {
            pProcessor->m_pVtable->m_pDispatchPacket(pProcessor, 8, pSlot);
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x004601B0
void VsNetPeerPayloadSender::WriteEffStreamWithGlobalSession(void) {
    ((GameEffStream *)g_pEffTransportGlobalReadStream)->BeginEffStreamWriteSession();
    SendEffStreamPayloadWithTransportHeader((int)(unsigned long)g_pEffTransportGlobalReadStream);
    ((GameEffStream *)g_pEffTransportGlobalReadStream)->EndEffStreamWriteSession();
}

// FUNCTION: LEMBALL 0x004601E0
void *VsNetAckedEffTransportRecordOwner::ClaimAckedEffTransportRecordPayload(void) {
    VsNetAckedEffTransportRecordOwner *pOwner;
    VsNetAckedEffTransportRecordTable *pRecordTable;
    VsNetAckedEffTransportRecord *pRecord;
    VsNetEffTransportGlobalReadStream *pReadStream;
    VsNetEffTransportPacketHeader *pSerializedHeader;
    void *pPayload;
    int nRecordIndex;

    pPayload = 0;
    pReadStream = (VsNetEffTransportGlobalReadStream *)g_pEffTransportGlobalReadStream;
    ((GameEffStream *)pReadStream)
        ->LoadEffStreamFromMemory((int)(unsigned long)((char *)g_pEffTransportPacketBuffer + 0x10));
    if (pReadStream->m_wPacketKind2e != 0x100) {
        return 0;
    }

    pOwner = this;
    pRecordTable = pOwner->m_pRecordTable4c;
    nRecordIndex = pReadStream->m_wSequence2c % pRecordTable->m_cRecords;
    pRecord = pRecordTable->m_apRecords08[nRecordIndex];
    pSerializedHeader = (VsNetEffTransportPacketHeader *)(unsigned long)pRecord->m_pSerializedStream04;

    ((GameEffStream *)this)->LoadEffStreamFromMemory(pRecord->m_pSerializedStream04);
    if (pSerializedHeader->m_wSequence0a == pReadStream->m_wSequence2c && pRecord->m_fClaimed08 == 0) {
        pRecord->m_fClaimed08 = 1;
        pPayload = pRecord->m_pPayload0c;
    }
    ((GameEffStream *)this)->LoadEffStreamFromMemory(pRecord->m_pSerializedStream04);
    return pPayload;
}

// FUNCTION: LEMBALL 0x004611E0
void VsNetSimpleEffTransportRecordSlot::CopyIntoSimpleEffTransportRecordSlot(
    const void *pvSource, unsigned int cbSource, void *pPayload)
{
    unsigned int i;
    const unsigned char *pbSource;
    unsigned char *pbTarget;
    m_pPayload0c = pPayload;
    m_cRetryCount14 = 0;

    pbSource = (const unsigned char *)pvSource;
    pbTarget = (unsigned char *)(unsigned long)m_pSerializedStream04;
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
void *VsNetSimpleEffTransportRecordSlotTable::ConstructEffTransportRecordSlotTable(
    int cRecords, unsigned int cbRecord)
{
    m_nReserved04 = cbRecord & 0xffff;
    m_cRecords = cRecords;
    if (cRecords > 0) {
        m_apRecords08 = (VsNetSimpleEffTransportRecordSlot **)AllocateVSMemBlock(
            (unsigned int)(cRecords << 2));
    } else {
        m_apRecords08 = 0;
    }
    return this;
}

// FUNCTION: LEMBALL 0x00461090
VsNetLockedEffTransportRecordSlot::VsNetLockedEffTransportRecordSlot(
    unsigned int cbPayload)
{
    m_pPayload04 = AllocateVSMemBlock(cbPayload & 0xffff);
    m_fAvailable28 = 1;
    m_fPayloadPresent24 = 0;
    *(unsigned short *)((char *)m_pPayload04 + 0x0a) = 0;
}

// FUNCTION: LEMBALL 0x004610F0
void VsNetLockedEffTransportRecordSlot::DestroyLockedEffTransportRecordSlot(void) {
    FreeVSMemBlock(m_pPayload04);
    m_CriticalSection08.~VsNetLockedEffTransportRecordCriticalSection();
}

// FUNCTION: LEMBALL 0x00461140
void VsNetLockedEffTransportRecordSlot::CopyIntoLockedEffTransportRecordSlot(
    const void *pvSource, unsigned int cbSource)
{
    unsigned int cbCopy;

    m_CriticalSection08.Enter();
    cbCopy = cbSource & 0xffff;
    memcpy(m_pPayload04, pvSource, cbCopy);
    m_fPayloadPresent24 = 1;
    m_CriticalSection08.Leave();
}

// FUNCTION: LEMBALL 0x00462A00
void *DeleteLockedEffTransportRecordSlotWrapper(void *pObject, BYTE fFreeMemory) {
    ((VsNetLockedEffTransportRecordSlot *)pObject)->DestroyLockedEffTransportRecordSlot();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00461190
void *VsNetSimpleEffTransportRecordSlot::ConstructSimpleEffTransportRecordSlot(
    unsigned int cbRecord)
{
    m_pVtable = (void **)g_NETWORK_DeleteSimpleEffTransportRecordSlotVtable;
    m_pSerializedStream04 = (int)(unsigned long)AllocateVSMemBlock(cbRecord & 0xffff);
    m_fAvailable08 = 1;
    m_cRetryCount14 = 0;
    return this;
}

// FUNCTION: LEMBALL 0x004611C0
void VsNetSimpleEffTransportRecordSlot::DestroySimpleEffTransportRecordSlot(void) {
    m_pVtable = (void **)g_NETWORK_DeleteSimpleEffTransportRecordSlotVtable;
    FreeVSMemBlock((void *)(unsigned long)m_pSerializedStream04);
    m_pVtable = (void **)g_NETWORK_EffTransportRecordBufferVtable;
}

// FUNCTION: LEMBALL 0x00461250
void VsNetVSMemObjectPointerArray::DestroyVSMemObjectPointerArray(void) {
    VsNetDeleteObject *pDeleteObject;
    int i;
    int nOffset;

    if (m_pObjectArray08 != 0) {
        nOffset = 0;
        for (i = 0; i < m_cObjects; ++i) {
            pDeleteObject = *(VsNetDeleteObject **)(unsigned long)(m_pObjectArray08 + nOffset);
            if (pDeleteObject != 0) {
                pDeleteObject->m_pVtable->m_pDelete(1);
            }
            nOffset += 4;
        }
        FreeVSMemBlock((void *)(unsigned long)m_pObjectArray08);
    }
}

// FUNCTION: LEMBALL 0x00461340
void *VsNetSimpleEffTransportRecordSlotTable::ConstructSimpleEffTransportRecordSlotTable(
    int cRecords, unsigned int cbRecord)
{
    void *pSlotStorage;
    int i;
    int nOffset;

    ConstructEffTransportRecordSlotTable(cRecords, cbRecord);
    if (m_apRecords08 != 0) {
        nOffset = 0;
        for (i = 0; i < m_cRecords; ++i) {
            pSlotStorage = AllocateVSMemBlock(0x18);
            if (pSlotStorage == 0) {
                *(void **)(unsigned long)((int)(unsigned long)m_apRecords08 + nOffset) = 0;
            } else {
                *(void **)(unsigned long)((int)(unsigned long)m_apRecords08 + nOffset) =
                    ((VsNetSimpleEffTransportRecordSlot *)pSlotStorage)
                        ->ConstructSimpleEffTransportRecordSlot((unsigned int)m_nReserved04);
            }
            nOffset += 4;
        }
    }
    return this;
}

static void *g_NETWORK_DeleteRangeEffTransportRecordBufferTableVtable = 0;
static void *g_NETWORK_DeleteRingEffTransportRecordBufferTableVtable = 0;

// FUNCTION: LEMBALL 0x00461290
void *VsNetSimpleEffTransportRecordSlotTable::ConstructLockedEffTransportRecordSlotTable(
    int cRecords, unsigned int cbRecord)
{
    VsNetSimpleEffTransportRecordSlotTable *pTable;
    void *pSlotStorage;
    int i;
    int nOffset;

    pTable = (VsNetSimpleEffTransportRecordSlotTable *)
        ConstructEffTransportRecordSlotTable(cRecords, cbRecord);
    if (pTable->m_apRecords08 != 0) {
        nOffset = 0;
        for (i = 0; i < pTable->m_cRecords; ++i) {
            pSlotStorage = AllocateVSMemBlock(0x2c);
            if (pSlotStorage == 0) {
                *(void **)((char *)pTable->m_apRecords08 + nOffset) = 0;
            } else {
                *(void **)((char *)pTable->m_apRecords08 + nOffset) =
                    new (pSlotStorage) VsNetLockedEffTransportRecordSlot(
                        (unsigned int)pTable->m_nReserved04);
            }
            nOffset += 4;
        }
    }
    return this;
}

// FUNCTION: LEMBALL 0x00461560
void *ConstructShiftedLockedRecordSlotTable(void *pObject, int cPacketIds, unsigned int cbRecord) {
    ((VsNetSimpleEffTransportRecordSlotTable *)pObject)
        ->ConstructLockedEffTransportRecordSlotTable(cPacketIds - 2, cbRecord);
    *(int *)((char *)pObject + 0x0c) = cPacketIds - 2;
    return pObject;
}

// FUNCTION: LEMBALL 0x00461610
void *ConstructRangeEffTransportRecordBufferTable(void *pObject, int nFirstPacket,
                                                  int nLastPacket, int cbPayload,
                                                  unsigned int cbRecord)
{
    VsNetEffTransportRecordBuffer *pTable;
    VsNetEffTransportRecordBuffer *pChild;
    int cRecords;
    int i;

    pTable = (VsNetEffTransportRecordBuffer *)pObject;
    cRecords = nLastPacket - nFirstPacket + 1;
    pTable->ConstructEffTransportRecordBuffer(cRecords, cbPayload, cbRecord);
    pTable->m_pVtable = (void **)g_NETWORK_DeleteRangeEffTransportRecordBufferTableVtable;
    *(int *)((char *)pObject + 0x1c) = nFirstPacket;
    *(int *)((char *)pObject + 0x18) = cRecords;
    *(int *)((char *)pObject + 0x24) = 0;
    if (cbPayload < 1) {
        *(void **)((char *)pObject + 0x20) = 0;
        return pObject;
    }
    *(void **)((char *)pObject + 0x20) = AllocateVSMemBlock(cRecords * 4);
    for (i = 0; i < cRecords; ++i) {
        pChild = (VsNetEffTransportRecordBuffer *)AllocateVSMemBlock(0x18);
        if (pChild == 0) {
            ((VsNetEffTransportRecordBuffer **)*(void **)((char *)pObject + 0x20))[i] = 0;
        } else {
            ((VsNetEffTransportRecordBuffer **)*(void **)((char *)pObject + 0x20))[i] =
                (VsNetEffTransportRecordBuffer *)pChild->ConstructEffTransportRecordBuffer(
                    cRecords, cbPayload, cbRecord);
        }
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004616B0
void VsNetEffTransportRecordBuffer::DestroyRangeEffTransportRecordBufferTable(void) {
    VsNetEffTransportRecordBuffer *pTable;
    VsNetDeleteObject *pChild;
    int i;
    int cRecords;

    pTable = this;
    pTable->m_pVtable = (void **)g_NETWORK_DeleteRangeEffTransportRecordBufferTableVtable;
    cRecords = *(int *)((char *)this + 0x18);
    if (*(void **)((char *)this + 0x20) != 0) {
        for (i = 0; i < cRecords; ++i) {
            pChild = ((VsNetDeleteObject **)*(void **)((char *)this + 0x20))[i];
            if (pChild != 0) {
                pChild->m_pVtable->m_pDelete(1);
            }
        }
        FreeVSMemBlock(*(void **)((char *)this + 0x20));
    }
    DestroyEffTransportRecordBuffer();
}

// FUNCTION: LEMBALL 0x00461780
void *ConstructRingLockedRecordSlotTable(void *pObject, int cRecords, unsigned int cbRecord) {
    ((VsNetSimpleEffTransportRecordSlotTable *)pObject)
        ->ConstructLockedEffTransportRecordSlotTable(cRecords, cbRecord);
    *(int *)((char *)pObject + 0x0c) = 0;
    *(int *)((char *)pObject + 0x10) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00461840
void *ConstructRingEffTransportRecordBufferTable(void *pObject, int cRecords,
                                                 int cbPayload, unsigned int cbRecord)
{
    VsNetEffTransportRecordBuffer *pTable;
    VsNetEffTransportRecordBuffer *pChild;
    int i;

    pTable = (VsNetEffTransportRecordBuffer *)pObject;
    pTable->ConstructEffTransportRecordBuffer(cRecords, cbPayload, cbRecord);
    pTable->m_pVtable = (void **)g_NETWORK_DeleteRingEffTransportRecordBufferTableVtable;
    *(int *)((char *)pObject + 0x18) = cRecords;
    if (cbPayload < 1) {
        *(void **)((char *)pObject + 0x1c) = 0;
        return pObject;
    }
    *(void **)((char *)pObject + 0x1c) = AllocateVSMemBlock(cRecords * 4);
    for (i = 0; i < cRecords; ++i) {
        pChild = (VsNetEffTransportRecordBuffer *)AllocateVSMemBlock(0x18);
        if (pChild == 0) {
            ((VsNetEffTransportRecordBuffer **)*(void **)((char *)pObject + 0x1c))[i] = 0;
        } else {
            ((VsNetEffTransportRecordBuffer **)*(void **)((char *)pObject + 0x1c))[i] =
                (VsNetEffTransportRecordBuffer *)pChild->ConstructEffTransportRecordBuffer(
                    cRecords, cbPayload, cbRecord);
        }
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004618E0
void VsNetEffTransportRecordBuffer::DestroyRingEffTransportRecordBufferTable(void) {
    VsNetEffTransportRecordBuffer *pTable;
    VsNetDeleteObject *pChild;
    int i;
    int cRecords;

    pTable = this;
    pTable->m_pVtable = (void **)g_NETWORK_DeleteRingEffTransportRecordBufferTableVtable;
    cRecords = *(int *)((char *)this + 0x18);
    if (*(void **)((char *)this + 0x1c) != 0) {
        for (i = 0; i < cRecords; ++i) {
            pChild = ((VsNetDeleteObject **)*(void **)((char *)this + 0x1c))[i];
            if (pChild != 0) {
                pChild->m_pVtable->m_pDelete(1);
            }
        }
        FreeVSMemBlock(*(void **)((char *)this + 0x1c));
    }
    DestroyEffTransportRecordBuffer();
}

// FUNCTION: LEMBALL 0x00462A60
void *DeleteRangeEffTransportRecordBufferTableWrapper(void *pObject, BYTE fFreeMemory) {
    ((VsNetEffTransportRecordBuffer *)pObject)
        ->DestroyRangeEffTransportRecordBufferTable();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462A80
void *DeleteRingEffTransportRecordBufferTableWrapper(void *pObject, BYTE fFreeMemory) {
    ((VsNetEffTransportRecordBuffer *)pObject)
        ->DestroyRingEffTransportRecordBufferTable();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00461970
void *VsNetSimpleEffTransportRecordSlotTable::ConstructSimpleRecordSlotTableWrapper(
    int cRecords, unsigned int cbRecord)
{
    ConstructSimpleEffTransportRecordSlotTable(cRecords, cbRecord);
    return this;
}

// FUNCTION: LEMBALL 0x004619D0
unsigned char VsNetSimpleEffTransportRecordSlotTable::GetNextEffTransportRecordClaimFlag(
    unsigned int nSequence)
{
    int nIndex;

    nIndex = (int)(nSequence & 0xffff) + 1;
    nIndex %= m_cRecords;
    return m_apRecords08[nIndex]->m_fAvailable08;
}

// FUNCTION: LEMBALL 0x004613A0
void VsNetSimpleEffTransportRecordSlotTable::CopyPayloadIntoSimpleRecordSlot(
    int nIndex, const void *pvSource, unsigned int cbSource, void *pPayload)
{
    m_apRecords08[nIndex]->CopyIntoSimpleEffTransportRecordSlot(
        pvSource, cbSource, pPayload);
}

// FUNCTION: LEMBALL 0x00461990
void *VsNetSimpleEffTransportRecordSlotTable::ClaimSimpleRecordSlotForPacketId(
    unsigned int nPacketId, const void *pvSource, unsigned int cbSource, void *pPayload)
{
    VsNetSimpleEffTransportRecordSlot *pSlot;
    int nIndex;

    nIndex = (int)(nPacketId & 0xffff) % m_cRecords;
    pSlot = m_apRecords08[nIndex];
    if (pSlot->m_fAvailable08 == 0) {
        return 0;
    }

    CopyPayloadIntoSimpleRecordSlot(nIndex, pvSource, cbSource, pPayload);
    return pSlot;
}

// FUNCTION: LEMBALL 0x004613D0
void *VsNetEffTransportRecordBuffer::ConstructEffTransportRecordBuffer(
    int fAllocatePayload, int cbPayload, unsigned int cbRecord)
{
    VsNetEffTransportPacketHeader *pSerializedHeader;
    int cbSerialized;

    m_pVtable = (void **)g_NETWORK_DeleteEffTransportRecordBufferVtable;
    m_fAllocatePayload0c = fAllocatePayload;
    m_nReserved14 = 0;
    m_nReserved10 = 0;
    cbSerialized = (cbRecord & 0xffff) - 0x10;
    m_cbPayload08 = cbSerialized;
    if (fAllocatePayload != 0 && cbSerialized != 0) {
        m_pSerializedBuffer04 = (int)(unsigned long)AllocateVSMemBlock((unsigned int)(cbPayload + 0x10));
        pSerializedHeader = (VsNetEffTransportPacketHeader *)(unsigned long)m_pSerializedBuffer04;
        pSerializedHeader->m_wSequence0a = 0;
    } else {
        m_pSerializedBuffer04 = 0;
    }
    return this;
}

// FUNCTION: LEMBALL 0x00461440
void VsNetEffTransportRecordBuffer::DestroyEffTransportRecordBuffer(void) {
    m_pVtable = (void **)g_NETWORK_DeleteEffTransportRecordBufferVtable;
    if (m_pSerializedBuffer04 != 0) {
        FreeVSMemBlock((void *)(unsigned long)m_pSerializedBuffer04);
    }
    m_pVtable = (void **)g_NETWORK_EffTransportRecordBufferVtable;
}

// FUNCTION: LEMBALL 0x00462A20
void *DeleteSimpleEffTransportRecordSlotWrapper(void *pObject, BYTE fFreeMemory) {
    ((VsNetSimpleEffTransportRecordSlot *)pObject)
        ->DestroySimpleEffTransportRecordSlot();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00462A40
void *DeleteEffTransportRecordBufferWrapper(void *pObject, BYTE fFreeMemory) {
    ((VsNetEffTransportRecordBuffer *)pObject)->DestroyEffTransportRecordBuffer();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

/* The original tables at 004991d0..004991dc are overlapping compiler
 * views.  Keep every view callable; only the delete entry is needed by the
 * record containers reconstructed here. */
static void *g_NETWORK_EffTransportRecordBufferVtableStorage[16] = {
    (void *)DeleteSimpleEffTransportRecordSlotWrapper,
    (void *)DeleteEffTransportRecordBufferWrapper,
    (void *)DeleteRangeEffTransportRecordBufferTableWrapper,
    (void *)DeleteRingEffTransportRecordBufferTableWrapper,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)_purecall,
    (void *)_purecall,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
};

static void *g_NETWORK_DeleteSimpleEffTransportRecordSlotVtableStorage[16] = {
    (void *)DeleteSimpleEffTransportRecordSlotWrapper,
    (void *)DeleteEffTransportRecordBufferWrapper,
    (void *)DeleteRangeEffTransportRecordBufferTableWrapper,
    (void *)DeleteRingEffTransportRecordBufferTableWrapper,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)_purecall,
    (void *)_purecall,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
};

static void *g_NETWORK_DeleteRangeEffTransportRecordBufferTableVtableStorage[16] = {
    (void *)DeleteRangeEffTransportRecordBufferTableWrapper,
    (void *)DeleteEffTransportRecordBufferWrapper,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
};

static void *g_NETWORK_DeleteRingEffTransportRecordBufferTableVtableStorage[16] = {
    (void *)DeleteRingEffTransportRecordBufferTableWrapper,
    (void *)DeleteEffTransportRecordBufferWrapper,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
};

struct VsNetInitializeRecordVtablePointers {
    VsNetInitializeRecordVtablePointers(void) {
        g_NETWORK_EffTransportRecordBufferVtable =
            g_NETWORK_EffTransportRecordBufferVtableStorage;
        g_NETWORK_DeleteSimpleEffTransportRecordSlotVtable =
            g_NETWORK_DeleteSimpleEffTransportRecordSlotVtableStorage;
        g_NETWORK_DeleteEffTransportRecordBufferVtable =
            g_NETWORK_EffTransportRecordBufferVtableStorage + 1;
        g_NETWORK_DeleteRangeEffTransportRecordBufferTableVtable =
            g_NETWORK_DeleteRangeEffTransportRecordBufferTableVtableStorage;
        g_NETWORK_DeleteRingEffTransportRecordBufferTableVtable =
            g_NETWORK_DeleteRingEffTransportRecordBufferTableVtableStorage;
    }
};

static VsNetInitializeRecordVtablePointers g_NETWORK_InitializeRecordVtablePointers;

// FUNCTION: LEMBALL 0x0045FEA0
int VsNetPeerPayloadSenderState::SendReliableEffTransportPayload(void *pStream) {
    VsNetEffTransportPacketHeader *pHeader;
    VsNetEffStreamPayloadState *pPayload;
    VsNetSimpleEffTransportRecordSlot *pRecord;
    void *pvPayload;
    int cbPayload;

    VsNetPeerPayloadSenderState *pSender = this;
    pPayload = (VsNetEffStreamPayloadState *)pStream;
    pHeader = (VsNetEffTransportPacketHeader *)(unsigned long)pSender->m_pPacketHeader2c;
    pvPayload = (void *)(unsigned long)pPayload->m_pPayloadBuffer08;
    cbPayload = pPayload->m_nWriteCursor1c - pPayload->m_pPayloadBuffer08;

    pHeader->m_fReliable0e = 1;
    ((VsNetEffStreamBase *)this)->SaveEffStreamToMemoryRange((int)(unsigned long)pvPayload, 0);
    pRecord = (VsNetSimpleEffTransportRecordSlot *)(unsigned long)
        ((VsNetSimpleEffTransportRecordSlotTable *)pSender->m_pRecordTable4c)
            ->ClaimSimpleRecordSlotForPacketId(
                pHeader->m_wSequence0a, pvPayload, (unsigned int)cbPayload, pStream);
    if (pRecord == 0) {
        return 0;
    }

    if (((VsNetPeerPayloadSender *)pSender)->SendPayload(pvPayload, cbPayload) != 0) {
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
    VsNetSimpleEffTransportRecordSlot *pRecord;
    VsNetEffTransportPacketHeader *pSerializedHeader;
    int nSendResult;

    pRecord = (VsNetSimpleEffTransportRecordSlot *)(unsigned long)nRecord;
    pSerializedHeader = (VsNetEffTransportPacketHeader *)(unsigned long)pRecord->m_pSerializedStream04;
    ((GameEffStream *)pObject)->LoadEffStreamFromMemory(pRecord->m_pSerializedStream04);
    ((GameEffStream *)pObject)->LoadEffStreamFromMemory(pRecord->m_pSerializedStream04);
    nSendResult =
        ((VsNetPeerPayloadSender *)pObject)
        ->SendPayload((void *)(unsigned long)pRecord->m_pSerializedStream04, pSerializedHeader->m_cbPayload04);
    if (nSendResult != 0) {
        pRecord->m_dwLastRetryTick10 = timeGetTime();
        ++pRecord->m_cRetryCount14;
        pRecord->m_fAvailable08 = 0;
    }
    return nSendResult;
}

// FUNCTION: LEMBALL 0x0045FF70
int VsNetPeerPayloadSenderState::SendFragmentedEffTransportPayload(void *pStream) {
    const int cbFragmentStep = g_cbEffTransportMaxPacketBytes - 0x10;
    VsNetPeerPayloadSenderState *pSender;
    VsNetEffTransportPacketHeader *pHeader;
    VsNetEffStreamPayloadState *pPayload;
    unsigned int cFragments;
    unsigned int cbPayload;
    unsigned int cbPacket;
    unsigned int iFragmentsSent;
    unsigned char *pbPayload;
    int nSendResult;

    pSender = this;
    pPayload = (VsNetEffStreamPayloadState *)pStream;
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

    pHeader = (VsNetEffTransportPacketHeader *)(unsigned long)pSender->m_pPacketHeader2c;
    pHeader->m_wSequence0a = (unsigned short)pSender->m_nPendingFragmentSequence58;

    iFragmentsSent = 0;
    while ((unsigned int)pSender->m_nPendingFragmentIndex5c < cFragments && iFragmentsSent < 9) {
        pHeader->m_wFragmentIndex0c = (unsigned short)pSender->m_nPendingFragmentIndex5c;
        cbPacket = g_cbEffTransportMaxPacketBytes;
        if (cbPayload < cbPacket) {
            cbPacket = cbPayload;
        }

        if (pSender->m_nPendingFragmentIndex5c == 0) {
            ((VsNetEffStreamBase *)this)->SaveEffStreamToMemoryRange((int)(unsigned long)pbPayload, 0);
            nSendResult = ((VsNetPeerPayloadSender *)pSender)->SendPayload(pbPayload, (int)cbPacket);
        } else {
            pSender->m_aFragmentPrefixWords60[0] = *(unsigned int *)(unsigned long)(pbPayload + 0);
            pSender->m_aFragmentPrefixWords60[1] = *(unsigned int *)(unsigned long)(pbPayload + 4);
            pSender->m_aFragmentPrefixWords60[2] = *(unsigned int *)(unsigned long)(pbPayload + 8);
            pSender->m_aFragmentPrefixWords60[3] = *(unsigned int *)(unsigned long)(pbPayload + 12);
            ((VsNetEffStreamBase *)this)->SaveEffStreamToMemoryRange((int)(unsigned long)pbPayload, 0);
            nSendResult = ((VsNetPeerPayloadSender *)pSender)->SendPayload(pbPayload, (int)cbPacket);
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
int VsNetPeerPayloadSender::SendEffStreamPayloadWithTransportHeader(int nStream) {
    VsNetPeerPayloadSenderState *pSender;
    VsNetPeerPayloadSenderStateOffsets *pOffsets;
    VsNetPeerPayloadSenderRuntimeStateView *pRuntimeState;
    VsNetEffTransportPacketHeader *pHeader;
    VsNetEffStreamPayloadState *pPayload;
    int cbPayload;

    pSender = (VsNetPeerPayloadSenderState *)this;
    pOffsets = pSender->m_pOffsets44;
    pRuntimeState = (VsNetPeerPayloadSenderRuntimeStateView *)((char *)pSender + pOffsets->m_nSendStateBaseOffset04);
    if (pRuntimeState->m_fTransportActive68 == 0) {
        return 0;
    }

    pPayload = (VsNetEffStreamPayloadState *)(unsigned long)nStream;
    if (pPayload->m_fBusy28 == 0) {
        pPayload->m_fBusy28 = 1;
    }

    pHeader = (VsNetEffTransportPacketHeader *)(unsigned long)pSender->m_pPacketHeader2c;
    cbPayload = pPayload->m_nWriteCursor1c - pPayload->m_pPayloadBuffer08;
    pHeader->m_cbPayload04 = cbPayload;
    pHeader->m_nStreamEvent08 = (unsigned short)pPayload->m_nEventCode04;

    if ((unsigned int)cbPayload > (unsigned int)g_cbEffTransportMaxPacketBytes) {
        if (pSender->m_nPendingFragmentIndex5c == -1) {
            cbPayload = pSender->SendFragmentedEffTransportPayload(pPayload);
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
            ((VsNetEffStreamBase *)this)->SaveEffStreamToMemoryRange(
                pPayload->m_pPayloadBuffer08, 0);
            cbPayload = ((VsNetPeerPayloadSender *)pSender)->SendPayload(
                (void *)(unsigned long)pPayload->m_pPayloadBuffer08, pHeader->m_cbPayload04);
        } else {
            ++pSender->m_wReliableSequence30;
            pHeader->m_wSequence0a = pSender->m_wReliableSequence30;
            cbPayload = pSender->SendReliableEffTransportPayload(pPayload);
        }
    }

    pPayload->m_fBusy28 = 0;
    return cbPayload;
}

// FUNCTION: LEMBALL 0x00460A40
void VsNetChannelOwnerObject::ClearEffTransportSendGate(void) {
    m_nSendGate = 0;
}

// FUNCTION: LEMBALL 0x00460260
void VsNetEffTransportPendingWriteState::Clear(void *) {
    int *pnOwnerBusy;

    if (*(int *)((char *)this - 0x1c) != -1) {
        pnOwnerBusy = *(int **)((char *)this - 0x24);
        *(int *)((char *)this - 0x1c) = -1;
        pnOwnerBusy[0x28 / sizeof(int)] = 0;
    }
}

// FUNCTION: LEMBALL 0x00460280
void VsNetPeerPayloadSenderState::ServiceEffTransportRetries(void) {
    VsNetPeerPayloadSenderState *pSender;
    VsNetPeerPayloadSenderRuntimeStateView *pRuntimeState;
    VsNetSimpleEffTransportRecordSlotTable *pRecordTable;
    VsNetSimpleEffTransportRecordSlot *pRecord;
    DWORD dwNow;
    int i;
    int cRecords;

    pSender = this;
    pRuntimeState =
        (VsNetPeerPayloadSenderRuntimeStateView *)((char *)pSender + pSender->m_pOffsets44->m_nSendStateBaseOffset04);
    if (pRuntimeState->m_fTransportActive68 == 0 || pRuntimeState->m_fRetryServiceEnabled50 == 0) {
        return;
    }

    if (pSender->m_nPendingFragmentIndex5c != -1 &&
        pSender->SendFragmentedEffTransportPayload(pSender->m_pPendingFragmentedStream54) != 0) {
        ((VsNetEffStreamPayloadState *)pSender->m_pPendingFragmentedStream54)->m_fBusy28 = 0;
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

            ((VsNetPeerPayloadSender *)pSender)
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
                ++i;
                pRuntimeState->m_pHandleCallback44->m_pVtable->m_pNotifyClosed(1);
                continue;
            }
        }
        ++i;
    }
}

// FUNCTION: LEMBALL 0x00460D20
void VsNetEffTransportPeer::FreeEffTransportPeerBuffers(void) {
    char *pbObject;
    int nOffsets;
    int nGroupDelta;

    pbObject = (char *)this;
    if (*(void **)(pbObject + 8) != 0) {
        FreeVSMemBlock(*(void **)(pbObject + 8));
        *(void **)(pbObject + 8) = 0;
    }
    if (*(void **)(pbObject + 0x0c) != 0) {
        FreeVSMemBlock(*(void **)(pbObject + 0x0c));
        *(void **)(pbObject + 0x0c) = 0;
    }
    nOffsets = *(int *)(pbObject + 4);
    nGroupDelta = *(int *)(nOffsets + 4);
    ((GameEffTransportHandleGroup *)(pbObject + nGroupDelta + 4))->Reset();
}

// FUNCTION: LEMBALL 0x00460D70
void VsNetEffTransportConnectCallback::QueueEffTransportConnectEvent(void) {
    VsNetEffDispatchEvent kEvent;
    VsNetEffTransportConnectStateView *pSendState;
    VsNetEffTransportTickStateView *pReliableTickState;
    VsNetEffTransportTickStateView *pTimedTickState;
    VsNetEffTransportConnectOffsets *pOffsets;
    VsNetEffTransportConnectOwner *pOwner;
    VsNetRuntimeWindowSendGateState *pWindowState;
    DWORD dwNow;

    pOwner = (VsNetEffTransportConnectOwner *)((char *)this - 0xd8);
    pOffsets = pOwner->m_pOffsets04;
    pSendState = (VsNetEffTransportConnectStateView *)((char *)pOwner + pOffsets->m_nSendStateViewOffset04);
    pReliableTickState = (VsNetEffTransportTickStateView *)((char *)pOwner + pOffsets->m_nReliableTickViewOffset08);
    pTimedTickState = (VsNetEffTransportTickStateView *)((char *)pOwner + pOffsets->m_nTimedTickViewOffset0c);
    kEvent.m_nType = 3;
    kEvent.m_nCode = 0;
    kEvent.m_pStream = (int *)pOwner;
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
        pWindowState = (VsNetRuntimeWindowSendGateState *)g_pActiveNetworkRuntimeWindow;
        if (pWindowState->m_fSendGateActive30 != 0) {
            ((VsNetChannelOwnerObject *)(unsigned long)pWindowState->m_nChannelOwner24)
                ->ClearEffTransportSendGate();
        }
    }
    ((void (*)(VsNetEffDispatchEvent *))(*(void ***)g_pEffTransportDispatchQueue)[2])(&kEvent);
}

// FUNCTION: LEMBALL 0x00460A50
void VsNetChannelOwnerObject::SendLoadedEffEventToPeer(void *pPeerKey, void *pStream) {
    VsNetPeerPayloadSenderSlot *pPayloadSenderSlot;
    VsNetPeerPayloadSenderRaw *pPayloadSender;

    LoadEffStreamFromGlobalRange(pStream);
    pPayloadSenderSlot =
        (VsNetPeerPayloadSenderSlot *)((char *)this + m_pOffsets->m_nPayloadSenderSlotOffset);
    pPayloadSender = &pPayloadSenderSlot->m_Sender;
    ((VsNetPeerPayloadSender *)pPayloadSender)->WritePeerKey(pPeerKey);
    ((VsNetPeerPayloadSender *)pPayloadSender)
        ->SendEffStreamPayloadWithTransportHeader((int)(unsigned long)pStream);
}

// FUNCTION: LEMBALL 0x00460910
void VsNetChannelOwnerObject::ServiceEffTransportConnectRequest(void) {
    char *pbObject;
    int nOffsets;
    int nPrimaryDelta;
    int nAdjustDelta;
    char *pbAdjustor;
    DWORD dwNow;
    int fSend;
    VsNetActiveRuntimeWindow *pRuntimeWindow;
    VsNetConnectRequestPeerKey *pPeerKey;

    pbObject = (char *)this;
    nOffsets = *(int *)(pbObject + 4);
    nPrimaryDelta = *(int *)(nOffsets + 4);
    if (*(int *)(pbObject + nPrimaryDelta + 0x14) == 0) {
        return;
    }

    nAdjustDelta = *(int *)(nOffsets + 0x10);
    pbAdjustor = pbObject + nAdjustDelta + 4;
    ((VsNetPeerPayloadSenderState *)
         (pbAdjustor + *(int *)(*(int *)pbAdjustor + 8)))->ServiceEffTransportRetries();
    CheckEffTransportIdleTimeout(pbAdjustor + *(int *)(*(int *)pbAdjustor + 0xc));
    if (*(int *)(pbObject + 8) == 0) {
        return;
    }

    dwNow = timeGetTime();
    if (dwNow - *(DWORD *)(pbObject + 0x18) <= 1000) {
        return;
    }

    fSend = 1;
    *(short *)((char *)g_pEffTransportRequestConnectControlStream + 0x30) =
        g_nEffTransportServiceBasePort;
    *(void **)((char *)g_pEffTransportRequestConnectControlStream + 0x38) =
        g_pszFileBasedNetworkLocalHostName;
    *(int *)((char *)g_pEffTransportRequestConnectControlStream + 0x34) =
        *(int *)(pbObject + 0x10);

    pRuntimeWindow = (VsNetActiveRuntimeWindow *)g_pActiveNetworkRuntimeWindow;
    pPeerKey = (VsNetConnectRequestPeerKey *)pRuntimeWindow->CreateChannelOwner();
    if (*(int *)(pbObject + 0x0c) == 2) {
        if (*(int *)(pbObject + 0x1c) == 0) {
            fSend = 0;
        } else {
            pPeerKey->SetPeerKey((void *)(unsigned long)*(int *)(pbObject + 0x1c));
        }
    } else {
        pPeerKey->SetConnectionMode(*(int *)(pbObject + 0x0c));
    }
    if (fSend != 0) {
        SendLoadedEffEventToPeer(pPeerKey, g_pEffTransportRequestConnectControlStream);
    }
    FreeVSMemBlock(pPeerKey);
    *(DWORD *)(pbObject + 0x18) = dwNow;
}

struct VsNetEffTransportRetryOwnerView {
    char m_abReserved00[0x24];
    void ArmEffTransportConnectRequestRetry(void);
};

// FUNCTION: LEMBALL 0x00460A20
void VsNetEffTransportRetryOwnerView::ArmEffTransportConnectRequestRetry(void) {
    DWORD dwNow;

    dwNow = timeGetTime();
    *(int *)((char *)this + 8) = 1;
    *(DWORD *)((char *)this + 0x18) = dwNow - 1000;
}

// FUNCTION: LEMBALL 0x00460C60
void VsNetEffTransportPeer::SetEffTransportPeerNameAndPort(char *pszName, void *pKey, short nPort) {
    VsNetEffTransportPeer *pTransportPeer;
    VsNetPeerPayloadSenderSlot *pPayloadSenderSlot;
    VsNetPeerPayloadSenderRaw *pPayloadSender;
    char chTerminator;
    unsigned int cchName;
    unsigned int cDwords;
    char *pszSource;
    char *pszDest;

    pTransportPeer = this;
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
    pszSource = pszName;
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
        (VsNetPeerPayloadSenderSlot *)((char *)pTransportPeer + pTransportPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
    pPayloadSender = &pPayloadSenderSlot->m_Sender;
    ((VsNetPeerPayloadSender *)pPayloadSender)->SetAssignedPort(nPort);
    /* The original routine forwards the first argument again here.  The
     * request key is consumed by the caller, not by this peer-name setter. */
    ((VsNetPeerPayloadSender *)pPayloadSender)->WritePeerKey(pszName);
}

// FUNCTION: LEMBALL 0x00460CE0
int VsNetEffTransportPeer::IsEffTransportPeerStillConnecting(void) {
    DWORD dwNow;

    if (m_fConnectComplete == 0) {
        dwNow = timeGetTime();
        if (4000 < dwNow - m_dwConnectStartTick) {
            Close();
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00460D10
void VsNetEffTransportPeer::MarkEffTransportPeerActivityTime(void) {
    m_dwConnectStartTick = timeGetTime();
}

// FUNCTION: LEMBALL 0x00462130
int VsNetEffTransportRuntimeState::MarkExistingEffTransportPeerActive(int nPeer) {
    VsNetEffTransportRuntimeState *pState;
    VsNetEffTransportPeer *pPeer;

    pState = this;
    pPeer = (VsNetEffTransportPeer *)pState->m_pFirstPeer;
    while (pPeer != 0) {
        if ((int)(unsigned long)pPeer == nPeer) {
            if (pPeer->IsEffTransportPeerStillConnecting() == 0) {
                return 0;
            }
            if (pPeer->m_fClosed == 0) {
                pPeer->MarkEffTransportPeerActivityTime();
                return 1;
            }
            return 0;
        }
        pPeer = pPeer->m_pNext;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00462180
int VsNetEffTransportRuntimeState::FindMatchingActiveEffTransportPeer(void *pPeerKey) {
    VsNetEffTransportRuntimeState *pState;
    VsNetEffTransportPeer *pPeer;
    VsNetPeerPayloadSenderSlot *pPayloadSenderSlot;
    VsNetTransportPeerMatcher *pPeerMatcher;

    pState = this;
    pPeer = (VsNetEffTransportPeer *)pState->m_pFirstPeer;
    while (pPeer != 0) {
        if (pPeer->m_fClosed == 0) {
            pPayloadSenderSlot =
                (VsNetPeerPayloadSenderSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
            pPeerMatcher = pPayloadSenderSlot->m_pMatcher;
            if (((VsNetTransportPeerMatcherVtable *)pPeerMatcher->m_pVtable)->m_pMatchPeerKey(pPeerKey) != 0) {
                break;
            }
        }
        pPeer = pPeer->m_pNext;
    }
    return (int)(unsigned long)pPeer;
}

// FUNCTION: LEMBALL 0x004621C0
void VsNetEffTransportRuntimeState::CloseMatchingEffTransportPeer(void *pPeerKey) {
    int nPeer;

    nPeer = FindMatchingActiveEffTransportPeer(pPeerKey);
    if (nPeer != 0) {
        ((VsNetEffTransportPeer *)(unsigned long)nPeer)->Close();
    }
}

// FUNCTION: LEMBALL 0x0045FBB0
void CheckEffTransportIdleTimeout(void *pObject) {
    int nStateDelta;
    char *pbState;
    DWORD dwNow;
    VsNetEffTransportHandleGroup *pHandleGroup;

    nStateDelta = *(int *)(*(int *)((char *)pObject + 0x44) + 4);
    pbState = (char *)pObject + nStateDelta;
    pHandleGroup = (VsNetEffTransportHandleGroup *)((char *)pObject + nStateDelta + 0x44);
    if (*(int *)(pbState + 0x54) != 0) {
        if (*(int *)(pbState + 0x5c) == 0) {
            return;
        }
        dwNow = timeGetTime();
        if (dwNow - *(DWORD *)((char *)pObject + 0x3c) <= 10000) {
            return;
        }
        ((GameEffTransportHandleGroup *)pHandleGroup)->Reset();
        pHandleGroup->m_pCallback->m_pVtable->m_pNotifyClosed(1);
        return;
    }
    if (*(int *)(pbState + 0x60) == 0) {
        return;
    }
    dwNow = timeGetTime();
    if (dwNow - *(DWORD *)((char *)pObject + 0x3c) > 10000) {
        ((GameEffTransportHandleGroup *)pHandleGroup)->Reset();
        pHandleGroup->m_pCallback->m_pVtable->m_pNotifyClosed(0);
    }
}

// FUNCTION: LEMBALL 0x00462040
int AllocateEffTransportPeerEntry(void *pRuntimeState) {
    VsNetEffTransportRuntimeState *pState;
    VsNetChannelOwnerObject *pOwner;
    VsNetEffTransportPeer *pPeer;
    VsNetEffTransportPeer *pNextPeer;
    VsNetHandleGroupAdjustorSlot *pAdjustorSlot;
    VsNetHandleGroupAdjustor *pAdjustor;
    int nSecondaryHandleBytes;
    int fPeerListLocked;

    pState = (VsNetEffTransportRuntimeState *)pRuntimeState;
    pOwner = (VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner;
    fPeerListLocked = 0;

    pPeer = (VsNetEffTransportPeer *)pState->m_pFirstPeer;
    while (pPeer != 0) {
        pNextPeer = pPeer->m_pNext;
        if (pPeer->m_fClosed != 0) {
            if (fPeerListLocked == 0) {
                fPeerListLocked = 1;
                pOwner->m_pVtable->m_pLockPeerList(pRuntimeState);
            }
            pState->UnlinkAndDeleteEffTransportPeer((int)(unsigned long)pPeer);
        }
        pPeer = pNextPeer;
    }

    if (fPeerListLocked != 0) {
        pOwner->m_pVtable->m_pUnlockPeerList(pRuntimeState);
    }

    pPeer = (VsNetEffTransportPeer *)(unsigned long)pOwner->m_pVtable->m_pAllocatePeer(pRuntimeState);
    if (pState->m_pFirstPeer == 0) {
        pState->m_pFirstPeer = pPeer;
    } else {
        ((VsNetEffTransportPeer *)pState->m_pLastPeer)->m_pNext = pPeer;
    }

    pPeer->m_pPrevious = (VsNetEffTransportPeer *)pState->m_pLastPeer;
    pState->m_pLastPeer = pPeer;

    pAdjustorSlot =
        (VsNetHandleGroupAdjustorSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nHandleGroupAdjustorSlotOffset);
    pAdjustor = &pAdjustorSlot->m_Adjustor;
    ((VsNetDualHandleEffStream *)((char *)pAdjustor +
                                     pAdjustor->m_pOffsets->m_nHandleStreamOffset))
        ->ConfigureEffStreamPrimaryHandleGroup(pState->m_nPrimaryHandleGroupCount,
                                               pState->m_nPrimaryHandleGroupBytes,
                                               pState->m_nPrimaryHandleGroupMode);
    ((VsNetTimedEffStream *)((char *)pAdjustor + pAdjustor->m_pOffsets->m_nTimedStreamOffset))
        ->ReleaseTimedEffStreamPrimaryHandle();

    nSecondaryHandleBytes = pState->m_nSecondaryHandleGroupBytes;
    pAdjustorSlot =
        (VsNetHandleGroupAdjustorSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nHandleGroupAdjustorSlotOffset);
    pAdjustor = &pAdjustorSlot->m_Adjustor;
    ((VsNetDualHandleEffStream *)((char *)pAdjustor +
                                     pAdjustor->m_pOffsets->m_nHandleStreamOffset))
        ->ConfigureEffStreamSecondaryHandleGroup(nSecondaryHandleBytes,
                                                 pState->m_nPrimaryHandleGroupMode);
    ((VsNetTimedEffStream *)((char *)pAdjustor +
                                pAdjustor->m_pOffsets->m_nTimedStreamOffset))
        ->ConfigureTimedEffStreamSecondaryHandle(nSecondaryHandleBytes);
    return (int)(unsigned long)pPeer;
}

// FUNCTION: LEMBALL 0x00461FC0
void VsNetEffTransportRuntimeState::UnlinkAndDeleteEffTransportPeer(int nPeer) {
    VsNetEffTransportRuntimeState *pState;
    VsNetChannelOwnerObject *pOwner;
    VsNetEffTransportPeer *pHeadPeer;
    VsNetEffTransportPeer *pPeer;
    VsNetEffTransportPeer *pNextPeer;
    VsNetEffTransportPeer *pPreviousPeer;
    VsNetTransportEndpointSlot *pEndpointSlot;
    VsNetEndpointState *pEndpoint;
    VsNetPeerPayloadSenderSlot *pPayloadSenderSlot;
    VsNetDeleteObject *pDeletePeer;
    DWORD dwReleasedPortKey;

    pState = this;
    pHeadPeer = (VsNetEffTransportPeer *)pState->m_pFirstPeer;
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

    pOwner = (VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner;
    pEndpointSlot = (VsNetTransportEndpointSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nEndpointStateSlotOffset);
    pEndpoint = &pEndpointSlot->m_State;
    dwReleasedPortKey = ((DWORD)(unsigned long)pPeer->m_pOffsets->m_nEndpointStateSlotOffset & 0xffff0000UL) |
                        (unsigned short)pEndpoint->m_nAssignedPort;
    pOwner->m_pVtable->m_pReleasePort(dwReleasedPortKey);

    pPeer->FreeEffTransportPeerBuffers();
    if (pPeer != 0) {
        pPayloadSenderSlot =
            (VsNetPeerPayloadSenderSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
        pDeletePeer = (VsNetDeleteObject *)&pPayloadSenderSlot->m_Sender;
        pDeletePeer->m_pVtable->m_pDelete(1);
    }

    if (pPreviousPeer != 0) {
        pPreviousPeer->m_pNext = pNextPeer;
    }
    if (pNextPeer != 0) {
        pNextPeer->m_pPrevious = pPreviousPeer;
    }
}

// FUNCTION: LEMBALL 0x004621E0
void VsNetEffTransportRuntimeState::HandleEffTransportRequestConnect(void *pPeerKey) {
    VsNetEffTransportRuntimeState *pState;
    VsNetEffTransportPeer *pPeer;
    VsNetTransportEndpointSlot *pEndpointSlot;
    VsNetEndpointState *pPeerTransportState;
    VsNetRequestConnectControlStream *pRequestConnectStream;
    VsNetAuthoriseConnectControlStream *pAuthoriseConnectStream;
    short nAssignedPort;

    pState = this;
    pRequestConnectStream = (VsNetRequestConnectControlStream *)g_pEffTransportRequestConnectControlStream;
    pAuthoriseConnectStream = (VsNetAuthoriseConnectControlStream *)g_pEffTransportAuthoriseConnectControlStream;
    pPeer = (VsNetEffTransportPeer *)(unsigned long)FindMatchingActiveEffTransportPeer(pPeerKey);
    if (pPeer != 0 && pPeer->m_fClosed == 0 &&
        pPeer->IsEffTransportPeerStillConnecting() != 0) {
        return;
    }

    pPeer = (VsNetEffTransportPeer *)(unsigned long)AllocateEffTransportPeerEntry(this);
    nAssignedPort = ((VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner)->m_pVtable->m_pAllocatePort(
        pRequestConnectStream->m_pPortBinding);
    if (nAssignedPort != -1) {
        pPeer->SetEffTransportPeerNameAndPort(pRequestConnectStream->m_pszHostName, pPeerKey, nAssignedPort);
        pEndpointSlot = (VsNetTransportEndpointSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nEndpointStateSlotOffset);
        pPeerTransportState = &pEndpointSlot->m_State;
        pAuthoriseConnectStream->m_nAssignedPort = pPeerTransportState->m_nAssignedPort;
        pAuthoriseConnectStream->m_pPeer = pPeer;
        ((VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner)
            ->SendLoadedEffEventToPeer(pPeerKey, pAuthoriseConnectStream);
        return;
    }

    pPeer->Close();
}

// FUNCTION: LEMBALL 0x00462280
void VsNetEffTransportRuntimeState::HandleEffTransportRequestNewPort(void *pPeerKey) {
    VsNetEffTransportRuntimeState *pState;
    VsNetChannelOwnerObject *pOwner;
    VsNetEffTransportPeer *pPeer;
    VsNetTransportEndpointSlot *pEndpointSlot;
    VsNetEndpointState *pPeerTransportState;
    VsNetPeerPayloadSenderSlot *pPayloadSenderSlot;
    VsNetPeerPayloadSenderRaw *pPayloadSender;
    VsNetRequestNewPortControlStream *pRequestNewPortStream;
    VsNetAuthoriseConnectControlStream *pAuthoriseConnectStream;
    VsNetFailedConnectControlStream *pFailedConnectStream;
    short nAssignedPort;
    int cNewPortRequests;
    DWORD dwReleasedPortKey;

    pState = this;
    pOwner = (VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner;
    pRequestNewPortStream = (VsNetRequestNewPortControlStream *)g_pEffTransportRequestNewPortControlStream;
    pAuthoriseConnectStream = (VsNetAuthoriseConnectControlStream *)g_pEffTransportAuthoriseConnectControlStream;
    pFailedConnectStream = (VsNetFailedConnectControlStream *)g_pEffTransportFailedConnectControlStream;
    pPeer = pRequestNewPortStream->m_pPeer;
    if (MarkExistingEffTransportPeerActive((int)(unsigned long)pPeer) == 0) {
        return;
    }

    pEndpointSlot = (VsNetTransportEndpointSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nEndpointStateSlotOffset);
    pPeerTransportState = &pEndpointSlot->m_State;
    dwReleasedPortKey = ((DWORD)(unsigned long)pPeer->m_pOffsets->m_nEndpointStateSlotOffset & 0xffff0000UL) |
                        (unsigned short)pPeerTransportState->m_nAssignedPort;
    pOwner->m_pVtable->m_pReleasePort(dwReleasedPortKey);

    cNewPortRequests = pPeer->m_cNewPortRequests + 1;
    pPeer->m_cNewPortRequests = cNewPortRequests;
    if (5 < cNewPortRequests) {
        pFailedConnectStream->m_pszFailureMessage = (char *)"To many new-port requests";
        ((VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner)
            ->SendLoadedEffEventToPeer(pPeerKey, pRequestNewPortStream);
        return;
    }

    nAssignedPort = pOwner->m_pVtable->m_pAllocatePort(pRequestNewPortStream->m_Base.m_pPortBinding);
    if (nAssignedPort != -1) {
        pPayloadSenderSlot =
            (VsNetPeerPayloadSenderSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
        pPayloadSender = &pPayloadSenderSlot->m_Sender;
        ((VsNetPeerPayloadSender *)pPayloadSender)->SetAssignedPort(nAssignedPort);
        pAuthoriseConnectStream->m_nAssignedPort = pPeerTransportState->m_nAssignedPort;
        pAuthoriseConnectStream->m_pPeer = pPeer;
        ((VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner)
            ->SendLoadedEffEventToPeer(pPeerKey, pAuthoriseConnectStream);
    }
}

// FUNCTION: LEMBALL 0x00462340
void HandleEffTransportAuthoriseConnect(void *pRuntimeState, void *pPeerKey) {
    VsNetEffTransportRuntimeState *pState;
    VsNetRuntimeServiceObject *pRuntimeService;
    VsNetChannelOwnerObject *pOwner;
    VsNetEffTransportPeer *pPeer;
    VsNetEffTransportPeer *pNewPeer;
    VsNetRequestNewPortControlStream *pRequestNewPortStream;
    VsNetAuthoriseConnectControlStream *pAuthoriseConnectStream;
    VsNetAuthoriseConnectControlStream *pGoAheadConnectStream;
    VsNetPeerPayloadSenderSlot *pPayloadSenderSlot;
    VsNetPeerPayloadSenderRaw *pPayloadSender;
    unsigned char *pbAssignedPort;
    short nAssignedPort;

    pState = (VsNetEffTransportRuntimeState *)pRuntimeState;
    pRuntimeService = (VsNetRuntimeServiceObject *)g_pEffTransportRuntimeService;
    pOwner = (VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner;
    pRequestNewPortStream = (VsNetRequestNewPortControlStream *)g_pEffTransportRequestNewPortControlStream;
    pAuthoriseConnectStream = (VsNetAuthoriseConnectControlStream *)g_pEffTransportAuthoriseConnectControlStream;
    pGoAheadConnectStream = (VsNetAuthoriseConnectControlStream *)g_pEffTransportGoAheadConnectControlStream;

    pPeer = (VsNetEffTransportPeer *)(unsigned long)
                ((VsNetEffTransportRuntimeState *)pRuntimeState)->FindMatchingActiveEffTransportPeer(pPeerKey);
    if (pPeer != 0 && pPeer->m_fClosed == 0 &&
        pPeer->IsEffTransportPeerStillConnecting() != 0) {
        if (pRuntimeService->m_pVtable->m_pValidatePeerKey(pPeerKey) != 0) {
            return;
        }
        pPeer->Close();
    }

    if (pState->m_nReserved30 != 0) {
        pOwner->ClearEffTransportSendGate();
    }

    nAssignedPort = pAuthoriseConnectStream->m_nAssignedPort;
    pbAssignedPort = pOwner->m_pbAllocatedPorts + nAssignedPort;
    if (*pbAssignedPort != 0) {
        pRequestNewPortStream->m_pPeer = pAuthoriseConnectStream->m_pPeer;
        pRequestNewPortStream->m_Base.m_nBasePort = g_nEffTransportServiceBasePort;
        pRequestNewPortStream->m_Base.m_pPortBinding = pOwner->m_pbAllocatedPorts;
        pRequestNewPortStream->m_Base.m_pszHostName = g_pszFileBasedNetworkLocalHostName;
        pOwner->SendLoadedEffEventToPeer(pPeerKey, pRequestNewPortStream);
        return;
    }

    *pbAssignedPort = 1;
    pNewPeer = (VsNetEffTransportPeer *)(unsigned long)AllocateEffTransportPeerEntry(pRuntimeState);
    pPayloadSenderSlot =
        (VsNetPeerPayloadSenderSlot *)((char *)pNewPeer + pNewPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
    pPayloadSender = &pPayloadSenderSlot->m_Sender;
    ((VsNetPeerPayloadSender *)pPayloadSender)->SetAssignedPort(nAssignedPort);
    ((VsNetEffTransportPeerVtable *)pNewPeer->m_pVtable)->m_pSetPeerKey(pPeerKey);
    pGoAheadConnectStream->m_nAssignedPort = nAssignedPort;
    pGoAheadConnectStream->m_pPeer = pAuthoriseConnectStream->m_pPeer;
    pOwner->SendLoadedEffEventToPeer(pPeerKey, pGoAheadConnectStream);
}

// FUNCTION: LEMBALL 0x00461DB0
void VsNetEffTransportRuntimeState::DestroyEffTransportRuntimeGlobals(void) {
    *(void **)this = g_NETWORK_RuntimeConstructionStateVtable;
    ((GameEffTransportRuntimeWindow *)this)
        ->UnregisterEffTransportEventClient();
    if (g_pEffTransportSecondaryDispatchQueue != 0) {
        (*(void (**)(int))(*(void ***)g_pEffTransportSecondaryDispatchQueue + 4))(1);
    }
    g_pEffTransportSecondaryDispatchQueue = 0;
    ((GameRenderDispatchQueue *)g_pEffTransportDispatchQueue)
        ->UnregisterOrderedRenderDispatchClient(this, 0x19);
    if (g_pEffTransportDispatchQueue != 0) {
        (*(void (**)(int))(*(void ***)g_pEffTransportDispatchQueue + 4))(1);
    }
    g_pEffTransportDispatchQueue = 0;
    *(void **)this = g_pNonZrleVariantRenderEntryInitializeVtable;
}

// FUNCTION: LEMBALL 0x0046F970
void *VsNetEffTransportRuntimeState::DeleteEffTransportRuntimeWindow(BYTE fFreeMemory) {
    char *pWindowBase;

    pWindowBase = (char *)this - 0x10;
    DestroyEffTransportRuntimeGlobals();
    DestroyInvisibleMessageWindow((VS_InvisibleWindow *)pWindowBase);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pWindowBase);
    }
    return pWindowBase;
}

// FUNCTION: LEMBALL 0x00462AD0
void *DeleteEffTransportRuntimeGlobalsWrapper(void *pObject, BYTE fFreeMemory) {
    ((VsNetEffTransportRuntimeState *)pObject)->DestroyEffTransportRuntimeGlobals();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00461E10
void VsNetEffTransportRuntimeState::ReleaseEffTransportRuntimeOwnerState(void) {
    VsNetEffTransportRuntimeState *pState;
    VsNetEffTransportPeer *pPeer;
    VsNetEffTransportPeer *pNextPeer;
    VsNetTransportEndpointSlot *pEndpointSlot;
    VsNetEndpointState *pPeerTransportState;
    short nAssignedPort;
    VsNetDeleteObject *pDeletePeer;
    VsNetDeleteObject *pDeleteObject;
    VsNetPeerPayloadSenderSlot *pPayloadSenderSlot;

    pState = this;
    if (pState->m_fTransportInitialized == 0) {
        return;
    }

    pState->m_fTransportInitialized = 0;
    pState->m_fRuntimeActive = 0;

    pPeer = (VsNetEffTransportPeer *)pState->m_pFirstPeer;
    if (pPeer != 0) {
        ((VsNetEffTransportRuntimeOwnerDispatch *)pState)->LockPeerList();
        while (pPeer != 0) {
            pNextPeer = pPeer->m_pNext;
            pEndpointSlot = (VsNetTransportEndpointSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nEndpointStateSlotOffset);
            pPeerTransportState = &pEndpointSlot->m_State;
            nAssignedPort = pPeerTransportState->m_nAssignedPort;
            pPeer->FreeEffTransportPeerBuffers();
            if (pPeer != 0) {
                pPayloadSenderSlot =
                    (VsNetPeerPayloadSenderSlot *)((char *)pPeer + pPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
                pDeletePeer = (VsNetDeleteObject *)&pPayloadSenderSlot->m_Sender;
                pDeletePeer->m_pVtable->m_pDelete(1);
            }
            pPeer = pNextPeer;
            if (nAssignedPort != -1) {
                ((VsNetChannelOwnerDispatch *)pState->m_pActiveChannelOwner)
                    ->ReleasePort(nAssignedPort);
            }
        }
        ((VsNetEffTransportRuntimeOwnerDispatch *)pState)->UnlockPeerList();
    }

    if (pState->m_pActiveChannelOwner != 0) {
        ((VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner)
            ->ReleaseGlobalEffTransportBuffer();
        if (pState->m_pActiveChannelOwner != 0) {
            pPayloadSenderSlot =
                (VsNetPeerPayloadSenderSlot *)((char *)pState->m_pActiveChannelOwner +
                                                  ((VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner)
                                                      ->m_pOffsets->m_nPayloadSenderSlotOffset);
            pDeleteObject = (VsNetDeleteObject *)&pPayloadSenderSlot->m_Sender;
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
        ((VsNetDeleteObject *)g_pEffTransportGlobalReadStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportGlobalReadStream = 0;
    if (g_pEffTransportGlobalWriteStream != 0) {
        ((VsNetDeleteObject *)g_pEffTransportGlobalWriteStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportGlobalWriteStream = 0;
    if (g_pEffTransportRequestConnectControlStream != 0) {
        ((VsNetDeleteObject *)g_pEffTransportRequestConnectControlStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportRequestConnectControlStream = 0;
    if (g_pEffTransportRequestNewPortControlStream != 0) {
        ((VsNetDeleteObject *)g_pEffTransportRequestNewPortControlStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportRequestNewPortControlStream = 0;
    if (g_pEffTransportAuthoriseConnectControlStream != 0) {
        ((VsNetDeleteObject *)g_pEffTransportAuthoriseConnectControlStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportAuthoriseConnectControlStream = 0;
    if (g_pEffTransportGoAheadConnectControlStream != 0) {
        ((VsNetDeleteObject *)g_pEffTransportGoAheadConnectControlStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportGoAheadConnectControlStream = 0;
    if (g_pEffTransportFailedConnectControlStream != 0) {
        ((VsNetDeleteObject *)g_pEffTransportFailedConnectControlStream)->m_pVtable->m_pDelete(1);
    }
    g_pEffTransportFailedConnectControlStream = 0;
    ((VsNetEffTransportRuntimeOwnerDispatch *)pState)->ShutdownOwnerState();
}

// FUNCTION: LEMBALL 0x00460E40
int VsNetEffTransportPeer::SendEffStreamToActivePeer(int *pStream) {
    VsNetEffTransportPeer *pTransportPeer;
    VsNetTransportEndpointSlot *pEndpointSlot;
    VsNetEndpointState *pPeerTransportState;
    VsNetPeerPayloadSenderSlot *pPayloadSenderSlot;
    VsNetPeerPayloadSenderRaw *pPayloadSender;
    VsNetEffDispatchEvent kFailureEvent;
    int fOpenedWriteSession;
    int nResult;

    pTransportPeer = this;
    pEndpointSlot =
        (VsNetTransportEndpointSlot *)((char *)pTransportPeer + pTransportPeer->m_pOffsets->m_nEndpointStateSlotOffset);
    pPeerTransportState = &pEndpointSlot->m_State;
    if (pPeerTransportState->m_fSecondaryHandleActive != 0 && pTransportPeer->m_fClosed == 0) {
        fOpenedWriteSession = pStream[4] < 1;
        if (fOpenedWriteSession != 0) {
            ((GameEffStream *)pStream)->BeginEffStreamWriteSession();
        }

        pPayloadSenderSlot =
            (VsNetPeerPayloadSenderSlot *)((char *)pTransportPeer +
                                              pTransportPeer->m_pOffsets->m_nPayloadSenderSlotOffset);
        pPayloadSender = &pPayloadSenderSlot->m_Sender;
        nResult = ((VsNetPeerPayloadSender *)pPayloadSender)
                      ->SendEffStreamPayloadWithTransportHeader((int)(unsigned long)pStream);
        if (nResult == 0) {
            kFailureEvent.m_nType = 1;
            kFailureEvent.m_nCode = 0xc;
            if (pStream[9] == 0) {
                kFailureEvent.m_nCode = 0xb;
            }
            kFailureEvent.m_pStream = pStream;
            kFailureEvent.m_pPeer = this;
            ((void (*)(VsNetEffDispatchEvent *))(*(void ***)g_pEffTransportDispatchQueue)[2])(&kFailureEvent);
        }

        if (fOpenedWriteSession != 0) {
            ((GameEffStream *)pStream)->EndEffStreamWriteSession();
        }
        return nResult;
    }

    pStream[10] = 0;
    return 0;
}

struct VsNetCompleteEffTransportPendingWriteView {
    void CompleteEffTransportPendingWrite(int fQueueEvent);
};

// FUNCTION: LEMBALL 0x00460F00
void VsNetCompleteEffTransportPendingWriteView::CompleteEffTransportPendingWrite(
    int fQueueEvent)
{
    char *pbObject;
    int nOwnerState;
    int nStateDelta;
    int nPendingDelta;
    VsNetEffDispatchEvent kEvent;

    pbObject = (char *)this;
    *(int *)(pbObject - 0x10) = 1;
    nOwnerState = *(int *)(*(int *)(pbObject - 0x2c) + 0x10);
    nStateDelta = *(int *)(pbObject + nOwnerState - 0x2c + 8);
    nPendingDelta = nStateDelta + nOwnerState - 0x24 + 0x70;
    ((VsNetEffTransportPendingWriteState *)(pbObject + nPendingDelta))->Clear(
        (void *)(unsigned long)fQueueEvent);
    if (fQueueEvent != 0) {
        kEvent.m_nType = 10;
        kEvent.m_nCode = 0;
        kEvent.m_pStream = (int *)(pbObject - 0x30);
        ((void (*)(VsNetEffDispatchEvent *))(*(void ***)g_pEffTransportDispatchQueue)[2])(&kEvent);
    }
}

// FUNCTION: LEMBALL 0x00461030
void VsNetCompositeEffTransportVtableModel::ServiceEffTransportPeerEntry(void) {
    VsNetEffTransportPeer *pPeer;
    char *pbPeer;
    int nOffsets;
    DWORD dwNow;

    pPeer = (VsNetEffTransportPeer *)this;
    pbPeer = (char *)pPeer;
    if (*(int *)(pbPeer + 0x20) != 0) {
        return;
    }
    if (*(int *)(pbPeer + 0x24) == 0) {
        nOffsets = *(int *)(pbPeer + 4);
        if (*(int *)(pbPeer + *(int *)(nOffsets + 4) + 0x20) == 0) {
            dwNow = timeGetTime();
            if (dwNow - *(DWORD *)(pbPeer + 0x28) <= 4000) {
                return;
            }
            pPeer->Close();
            return;
        }
    }
    nOffsets = *(int *)(pbPeer + 4);
    CheckEffTransportIdleTimeout(pbPeer + *(int *)(nOffsets + 0x0c) + 4);
    ((VsNetPeerPayloadSenderState *)(pbPeer + *(int *)(nOffsets + 8) + 4))
        ->ServiceEffTransportRetries();
}

struct VsNetDispatchAckedEffTransportPayloadView {
    void *DispatchAckedEffTransportPayloadEvent(void);
};

// FUNCTION: LEMBALL 0x00460F60
void *VsNetDispatchAckedEffTransportPayloadView::DispatchAckedEffTransportPayloadEvent(void) {
    VsNetEffDispatchEvent kEvent;
    VsNetEffTransportAckDispatchOwner *pOwner;
    void *pPayload;

    pOwner = (VsNetEffTransportAckDispatchOwner *)((char *)this - 0x60);
    pPayload = ((VsNetAckedEffTransportRecordOwner *)
                    ((char *)this + pOwner->m_pOffsets04->m_nAckRecordOwnerViewOffset08 - 0x5c))
                   ->ClaimAckedEffTransportRecordPayload();
    if (pPayload != 0) {
        kEvent.m_nType = 6;
        kEvent.m_nCode = 0;
        kEvent.m_pStream = (int *)pOwner;
        kEvent.m_pPeer = pPayload;
        ((void (*)(VsNetEffDispatchEvent *))(*(void ***)g_pEffTransportSecondaryDispatchQueue)[2])(&kEvent);
    }
    return pPayload;
}

struct VsNetQueueEffTransportPayloadEventView {
    void QueueEffTransportPayloadEvent(unsigned short nType, void *pPayload);
};

// FUNCTION: LEMBALL 0x00460FF0
void VsNetQueueEffTransportPayloadEventView::QueueEffTransportPayloadEvent(
    unsigned short nType, void *pPayload)
{
    VsNetEffDispatchEvent kEvent;

    kEvent.m_nType = nType;
    kEvent.m_nCode = 0;
    kEvent.m_pStream = (int *)((char *)this - 0xd8);
    kEvent.m_pPeer = pPayload;
    ((void (*)(VsNetEffDispatchEvent *))(*(void ***)g_pEffTransportSecondaryDispatchQueue)[2])(&kEvent);
}

// FUNCTION: LEMBALL 0x00462460
void VsNetEffTransportRuntimeState::ForwardEffTransportFailedConnect(void *pPeerKey) {
    ((VsNetChannelOwnerObject *)m_pActiveChannelOwner)
        ->SendLoadedEffEventToPeer(pPeerKey, g_pEffTransportFailedConnectControlStream);
}

// FUNCTION: LEMBALL 0x00462480
void VsNetEffTransportRuntimeState::HandleEffTransportGoAheadConnect(void) {
    VsNetEffTransportPeer *pPeer;

    pPeer = ((VsNetAuthoriseConnectControlStream *)g_pEffTransportGoAheadConnectControlStream)->m_pPeer;
    if (MarkExistingEffTransportPeerActive((int)(unsigned long)pPeer) != 0) {
        ((VsNetEffTransportPeerVtable *)pPeer->m_pVtable)->m_pCompleteConnect();
    }
}

// FUNCTION: LEMBALL 0x004624A0
void VsNetEffTransportRuntimeState::DispatchEffTransportConnectControlStream(void *pPeerKey,
                                                                                  int nSourceBuffer)
{
    ((void (*)(void))**(void ***)pPeerKey)();
    if (((GameEffStream *)g_pEffTransportRequestConnectControlStream)
            ->LoadEffStreamFromMemory(nSourceBuffer) != 0) {
        HandleEffTransportRequestConnect(pPeerKey);
        return;
    }
    if (((GameEffStream *)g_pEffTransportRequestNewPortControlStream)
            ->LoadEffStreamFromMemory(nSourceBuffer) != 0) {
        HandleEffTransportRequestNewPort(pPeerKey);
        return;
    }
    if (((GameEffStream *)g_pEffTransportAuthoriseConnectControlStream)
            ->LoadEffStreamFromMemory(nSourceBuffer) != 0) {
        HandleEffTransportAuthoriseConnect(this, pPeerKey);
        return;
    }
    if (((GameEffStream *)g_pEffTransportGoAheadConnectControlStream)
            ->LoadEffStreamFromMemory(nSourceBuffer) != 0) {
        HandleEffTransportGoAheadConnect();
        return;
    }
    if (((GameEffStream *)g_pEffTransportFailedConnectControlStream)
            ->LoadEffStreamFromMemory(nSourceBuffer) != 0) {
        ForwardEffTransportFailedConnect(pPeerKey);
    }
}

// FUNCTION: LEMBALL 0x00460610
void VsNetEffTransportPeer::ConfigureEffTransportConnectHostString(void *pHostName) {
    if (m_nReserved1C != 0) {
        void *pBuffer = (void *)(unsigned long)m_nReserved1C;
        m_nReserved1C = 0;
        FreeVSMemBlock(pBuffer);
    }

    *(int *)((char *)this + 0xc) = 2;
    void (**ppVtable)(void);
    ppVtable = *(void (***) (void))this;
    ((void (*)(void *))ppVtable[2])(pHostName);
}

// FUNCTION: LEMBALL 0x00460650
void VsNetEffTransportPeer::PrepareEffTransportBroadcastStatusPayload(char *pszHostName) {
    VsNetHandleGroupAdjustor *pAdjustor;
    char *pszTarget;
    int cbText;

    pAdjustor = (VsNetHandleGroupAdjustor *)
        ((char *)this + m_pOffsets->m_nHandleGroupAdjustorSlotOffset + 4);
    ((VsNetDualHandleEffStream *)((char *)pAdjustor +
                                     pAdjustor->m_pOffsets->m_nHandleStreamOffset))
        ->ConfigureEffStreamPrimaryHandleGroup(3, 3, 0);
    ((VsNetTimedEffStream *)((char *)pAdjustor +
                                pAdjustor->m_pOffsets->m_nTimedStreamOffset))
        ->ReleaseTimedEffStreamPrimaryHandle();

    pAdjustor = (VsNetHandleGroupAdjustor *)
        ((char *)this + m_pOffsets->m_nHandleGroupAdjustorSlotOffset + 4);
    ((VsNetDualHandleEffStream *)((char *)pAdjustor +
                                     pAdjustor->m_pOffsets->m_nHandleStreamOffset))
        ->ConfigureEffStreamSecondaryHandleGroup(1, 0);
    ((VsNetTimedEffStream *)((char *)pAdjustor +
                                pAdjustor->m_pOffsets->m_nTimedStreamOffset))
        ->ConfigureTimedEffStreamSecondaryHandle(1);

    if (g_pszEffTransportBroadcastStatusPayload == 0) {
        g_pszEffTransportBroadcastStatusPayload = (char *)AllocateVSMemBlock(0x410);
        pszTarget = g_pszEffTransportBroadcastStatusPayload + 0x10;

        strcpy(pszTarget, g_szEffTransportBroadcastPrefix);
        FormatSignedIntToRadixString(g_nEffTransportVersionMajor,
                                     pszTarget + strlen(pszTarget), 10);
        *(unsigned short *)(pszTarget + strlen(pszTarget)) =
            *(const unsigned short *)g_szEffTransportVersionSeparator;
        FormatSignedIntToRadixString(g_nEffTransportVersionMinor,
                                     pszTarget + strlen(pszTarget), 10);
        *(unsigned short *)(pszTarget + strlen(pszTarget)) =
            *(const unsigned short *)g_szEffTransportVersionSpace;
        strcpy(pszTarget + strlen(pszTarget), pszHostName);
        strcpy(pszTarget + strlen(pszTarget), g_szEffTransportBroadcastSuffix);

        cbText = (int)strlen(pszTarget);
        g_cbEffTransportBroadcastStatusPayload = cbText;
    }
}

// FUNCTION: LEMBALL 0x004607F0
void VsNetChannelOwnerObject::ReleaseGlobalEffTransportBuffer(void) {
    VsNetChannelOwnerObject *pChannelOwner;
    VsNetTransportEndpointSlot *pEndpointSlot;
    VsNetEndpointState *pEndpointState;

    if (g_pszFileBasedNetworkLocalHostName != 0) {
        FreeVSMemBlock(g_pszFileBasedNetworkLocalHostName);
        g_pszFileBasedNetworkLocalHostName = 0;
    }

    pChannelOwner = this;
    pEndpointSlot =
        (VsNetTransportEndpointSlot *)((char *)this + m_pOffsets->m_nEndpointStateSlotOffset);
    pEndpointState = &pEndpointSlot->m_State;
    ((GameEffTransportHandleGroup *)pEndpointState)->Reset();
}

struct VsNetPrefixedEffTransportControlView {
    void DispatchPrefixedEffTransportControlStream(int nDispatchType, void *pEvent);
};

// FUNCTION: LEMBALL 0x00460830
void VsNetPrefixedEffTransportControlView::DispatchPrefixedEffTransportControlStream(
    int nDispatchType, void *pEvent)
{
    VsNetEffDispatchEventPayloadView *pEventView;
    VsNetEffPrefixedControlDispatchOwner *pOwner;
    VsNetRuntimeServiceObject *pPeerAddressState;
    char *pszPayloadText;
    int nIsLocalPeer;
    int nPayloadBuffer;

    pOwner = (VsNetEffPrefixedControlDispatchOwner *)((char *)this - 0xd8);
    pEventView = (VsNetEffDispatchEventPayloadView *)pEvent;
    pPeerAddressState = (VsNetRuntimeServiceObject *)g_pEffTransportPeerAddressState;
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
        ((VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow)
            ->DispatchEffTransportConnectControlStream(
                g_pEffTransportPeerAddressState,
                nPayloadBuffer + g_cbEffTransportBroadcastStatusPayload + 0x11);
    }

    if (nDispatchType == 7) {
        pEventView->m_pOwnedPayload24 = 0;
    }
}

// FUNCTION: LEMBALL 0x004608F0
void WINAPI LoadEffStreamFromGlobalRange(void *pStream) {
    ((VsNetEffStreamBase *)pStream)->SaveEffStreamToMemoryRange(
        (int)(unsigned long)g_pszEffTransportBroadcastStatusPayload,
        g_cbEffTransportBroadcastStatusPayload + 0x11);
}

// FUNCTION: LEMBALL 0x00462720
int VsNetEffTransportRuntimeState::BroadcastEffStreamToActivePeers(int *pStream) {
    VsNetEffTransportRuntimeState *pState;
    VsNetEffTransportPeer *pPeer;
    int fSendBlocked;
    int cActivePeers;

    pState = this;
    fSendBlocked = 0;
    cActivePeers = 0;
    pPeer = (VsNetEffTransportPeer *)pState->m_pFirstPeer;

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
                if (pPeer->SendEffStreamToActivePeer(pStream) != 0) {
                    fSendBlocked = 0;
                    pPeer = pPeer->m_pNext;
                    continue;
                }
            }
            fSendBlocked = 1;
        }

        pPeer = pPeer->m_pNext;
    }
}

// FUNCTION: LEMBALL 0x004627B0
int VsNetEffTransportRuntimeState::ProcessEffTransportDispatchEvent(
    VsNetEffDispatchEvent *pEvent) {
    if (pEvent->m_nType == 0xb) {
        if (pEvent->m_nCode == 1) {
            ((VsNetEffTransportPeer *)pEvent->m_pPeer)
                ->SendEffStreamToActivePeer(pEvent->m_pStream);
            ((GameEffStream *)pEvent->m_pStream)->EndEffStreamWriteSession();
            return 1;
        }
    } else if (pEvent->m_nType == 0xc) {
        if (pEvent->m_nCode == 1) {
            BroadcastEffStreamToActivePeers(pEvent->m_pStream);
            ((GameEffStream *)pEvent->m_pStream)->EndEffStreamWriteSession();
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045F360
void WriteEffTransportRequestStreamName(void *pObject) {
    VsNetEffStreamBase *pStream;

    pStream = (VsNetEffStreamBase *)pObject;
    pStream->WriteEffStreamCString(*(char **)((char *)pObject + 0x2c));
}

// FUNCTION: LEMBALL 0x0045F370
int CompareEffTransportRequestStreamName(void *pObject) {
    const unsigned char *pbStream;
    const unsigned char *pbName;

    pbStream = (const unsigned char *)(unsigned long)*(int *)((char *)pObject + 0x20);
    pbName = (const unsigned char *)*(char **)((char *)pObject + 0x2c);
    while (*pbStream == *pbName) {
        if (*pbStream == 0) {
            return 1;
        }
        ++pbStream;
        ++pbName;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045F430
void __fastcall LoadEffTransportRequestConnectStream(void *pObject) {
    VsNetEffStreamBase *pStream;

    pStream = (VsNetEffStreamBase *)pObject;
    *(short *)((char *)pObject + 0x30) = (short)pStream->ReadEffStreamU32BEValue();
    pStream->ReadEffStreamRangePointer((void **)((char *)pObject + 0x34), 0x200);
    pStream->ReadEffStreamCString((char **)((char *)pObject + 0x38));
}

// FUNCTION: LEMBALL 0x0045F460
void __fastcall WriteEffTransportRequestPortFields(void *pObject) {
    VsNetRequestConnectControlStream *pControlStream;
    VsNetEffStreamBase *pStream;

    pControlStream = (VsNetRequestConnectControlStream *)pObject;
    pStream = (VsNetEffStreamBase *)pObject;
    pStream->WriteEffStreamU32BE((unsigned int)pControlStream->m_nBasePort);
    pStream->WriteEffStreamBytes(pControlStream->m_pPortBinding, 0x200);
    pStream->WriteEffStreamCString(pControlStream->m_pszHostName);
}

// FUNCTION: LEMBALL 0x0045F4B0
void LEMBALL_FASTCALL LoadEffTransportRequestNewPortStream(void *pObject) {
    VsNetRequestNewPortControlStream *pControlStream;
    VsNetEffStreamBase *pStream;

    pControlStream = (VsNetRequestNewPortControlStream *)pObject;
    LoadEffTransportRequestConnectStream(pObject);
    pStream = (VsNetEffStreamBase *)pObject;
    pControlStream->m_pPeer =
        (VsNetEffTransportPeer *)pStream->ReadEffStreamU32BEValue();
}

// FUNCTION: LEMBALL 0x0045F4D0
void LEMBALL_FASTCALL WriteEffTransportRequestNewPortStream(void *pObject) {
    VsNetRequestNewPortControlStream *pControlStream;
    VsNetEffStreamBase *pStream;

    pControlStream = (VsNetRequestNewPortControlStream *)pObject;
    pStream = (VsNetEffStreamBase *)pObject;
    WriteEffTransportRequestPortFields(pObject);
    pStream->WriteEffStreamU32BE((unsigned int)pControlStream->m_pPeer);
}

// FUNCTION: LEMBALL 0x0045F540
void __fastcall LoadEffTransportAuthoriseConnectStream(void *pObject) {
    VsNetEffStreamBase *pStream;

    pStream = (VsNetEffStreamBase *)pObject;
    *(unsigned short *)((char *)pObject + 0x30) = pStream->ReadEffStreamU16BEValue();
    *(unsigned int *)((char *)pObject + 0x34) = pStream->ReadEffStreamU32BEValue();
}

// FUNCTION: LEMBALL 0x0045F560
void LEMBALL_FASTCALL WriteEffTransportAuthoriseConnectStream(void *pObject) {
    VsNetAuthoriseConnectControlStream *pControlStream;
    VsNetEffStreamBase *pStream;

    pControlStream = (VsNetAuthoriseConnectControlStream *)pObject;
    pStream = (VsNetEffStreamBase *)pObject;
    pStream->WriteEffStreamU16BE(pControlStream->m_nAssignedPort);
    pStream->WriteEffStreamU32BE((unsigned int)pControlStream->m_pPeer);
}

// FUNCTION: LEMBALL 0x0045F5D0
void __fastcall LoadEffTransportGoAheadConnectStream(void *pObject) {
    VsNetEffStreamBase *pStream;

    pStream = (VsNetEffStreamBase *)pObject;
    *(unsigned short *)((char *)pObject + 0x30) = pStream->ReadEffStreamU16BEValue();
    *(unsigned int *)((char *)pObject + 0x34) = pStream->ReadEffStreamU32BEValue();
}

// FUNCTION: LEMBALL 0x0045F5F0
void LEMBALL_FASTCALL WriteEffTransportGoAheadConnectStream(void *pObject) {
    VsNetAuthoriseConnectControlStream *pControlStream;
    VsNetEffStreamBase *pStream;

    pControlStream = (VsNetAuthoriseConnectControlStream *)pObject;
    pStream = (VsNetEffStreamBase *)pObject;
    pStream->WriteEffStreamU16BE(pControlStream->m_nAssignedPort);
    pStream->WriteEffStreamU32BE((unsigned int)pControlStream->m_pPeer);
}

// FUNCTION: LEMBALL 0x0045F660
void LEMBALL_FASTCALL LoadEffTransportFailedConnectStream(void *pObject) {
    ((VsNetEffStreamBase *)pObject)->ReadEffStreamCString(
        (char **)((char *)pObject + 0x30));
}

// FUNCTION: LEMBALL 0x0045F670
void LEMBALL_FASTCALL WriteEffTransportFailedConnectStream(void *pObject) {
    ((VsNetEffStreamBase *)pObject)->WriteEffStreamCString(
        *(char **)((char *)pObject + 0x30));
}

// FUNCTION: LEMBALL 0x0045F3D0
void *ConstructEffTransportRequestConnectControlStream(void *pObject, char *pszStreamName) {
    VsNetRequestConnectControlStream *pStream;
    unsigned int cbName;

    pStream = (VsNetRequestConnectControlStream *)pObject;
    pStream->m_Header.m_pVtable = g_GAME_EffStreamConstructionVtable;
    pStream->m_Header.m_nEventCode = 0;
    ((GameEffStream *)pObject)->ResetStateFields();
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
    VsNetRequestNewPortControlStream *pStream;

    ConstructEffTransportRequestConnectControlStream(pObject, pszStreamName);
    pStream = (VsNetRequestNewPortControlStream *)pObject;
    pStream->m_Base.m_Header.m_pVtable = g_NETWORK_RequestNewPortControlStreamVtable;
    pStream->m_Base.m_Header.m_cbSerializedLength = pStream->m_Base.m_Header.m_cbSerializedLength + 0x10;
    return pObject;
}

// FUNCTION: LEMBALL 0x0045F4F0
void *ConstructEffTransportAuthoriseConnectControlStream(void *pObject, char *pszStreamName) {
    VsNetAuthoriseConnectControlStream *pStream;
    unsigned int cbName;

    pStream = (VsNetAuthoriseConnectControlStream *)pObject;
    pStream->m_Header.m_pVtable = g_GAME_EffStreamConstructionVtable;
    pStream->m_Header.m_nEventCode = 0;
    ((GameEffStream *)pObject)->ResetStateFields();
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
    VsNetAuthoriseConnectControlStream *pStream;
    unsigned int cbName;

    pStream = (VsNetAuthoriseConnectControlStream *)pObject;
    pStream->m_Header.m_pVtable = g_GAME_EffStreamConstructionVtable;
    pStream->m_Header.m_nEventCode = 0;
    ((GameEffStream *)pObject)->ResetStateFields();
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
    VsNetFailedConnectControlStream *pStream;
    unsigned int cbName;

    pStream = (VsNetFailedConnectControlStream *)pObject;
    pStream->m_Header.m_pVtable = g_GAME_EffStreamConstructionVtable;
    pStream->m_Header.m_nEventCode = 0;
    ((GameEffStream *)pObject)->ResetStateFields();
    pStream->m_Header.m_pVtable = g_NETWORK_RequestConnectControlStreamNameVtable;
    pStream->m_Header.m_pszStreamName = pszStreamName;

    cbName = (unsigned int)strlen(pszStreamName) + 1;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + cbName;

    pStream->m_Header.m_pVtable = g_NETWORK_FailedConnectControlStreamVtable;
    pStream->m_Header.m_cbSerializedLength = pStream->m_Header.m_cbSerializedLength + 0x101;
    return pObject;
}

// FUNCTION: LEMBALL 0x004619F0
void *VsNetEffTransportRuntimeState::ConstructEffTransportRuntimeState(void) {
    VsNetEffTransportRuntimeState *pState;
    void *pQueue;

    pState = this;
    InitializeRenderQueueNodeBase(this);
    pState->m_RenderQueueNode.m_pVtable = (void **)g_NETWORK_EffTransportRuntimeStateVtable;
    g_nEffTransportAsyncErrorStatus = 0;
    pState->m_pFirstPeer = 0;
    pState->m_pActiveChannelOwner = 0;
    pState->m_nReserved34 = 0;
    pState->m_pLastPeer = 0;
    pState->m_nReserved30 = 0;
    pState->m_fRuntimeActive = 0;
    pState->m_fTransportInitialized = 0;
    pState->m_nReserved40 = 0;
    pState->m_nReserved3C = 0;
    pState->m_nReserved38 = 0;
    pState->m_nReserved44 = 0;
    pState->m_nReserved48 = 0;
    pState->m_nRuntimeKey = 0;
    pState->m_fShutdownRequested = 0;
    pState->m_cbMaxPacketBytes = 0x50;
    pState->m_fStartRequested = 0;

    pQueue = AllocateVSMemBlock(0x58);
    if (pQueue == 0) {
        g_pEffTransportDispatchQueue = 0;
    } else {
        g_pEffTransportDispatchQueue = ConstructRenderDispatchQueue(pQueue, 0x1e);
    }
    ((GameRenderDispatchQueue *)g_pEffTransportDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(this, 0x19);

    pQueue = AllocateVSMemBlock(0x58);
    if (pQueue == 0) {
        g_pEffTransportSecondaryDispatchQueue = 0;
    } else {
        g_pEffTransportSecondaryDispatchQueue = ConstructRenderDispatchQueue(pQueue, 0x1e);
    }

    return this;
}

// FUNCTION: LEMBALL 0x00461BD0
int VsNetEffTransportRuntimeState::InitializeEffTransportRuntimeGlobals(void) {
    VsNetEffTransportRuntimeState *pState;
    VsNetEffTransportRuntimeStateVtable *pVtable;
    VsNetChannelOwnerObject *pOwner;
    void *pObject;

    pState = this;
    pVtable = (VsNetEffTransportRuntimeStateVtable *)pState->m_RenderQueueNode.m_pVtable;
    pState->m_fStartRequested = 0;
    (this->*pVtable->m_pInitializeTransport)();
    if (g_nEffTransportAsyncErrorStatus != 0) {
        return 0;
    }

    pObject = AllocateVSMemBlock(0x3c);
    if (pObject == 0) {
        g_pEffTransportRequestConnectControlStream = 0;
    } else {
        g_pEffTransportRequestConnectControlStream =
            ConstructEffTransportRequestConnectControlStream(pObject, (char *)"Request Connect");
    }

    pObject = AllocateVSMemBlock(0x40);
    if (pObject == 0) {
        g_pEffTransportRequestNewPortControlStream = 0;
    } else {
        g_pEffTransportRequestNewPortControlStream =
            ConstructEffTransportRequestNewPortControlStream(pObject, (char *)"Request New Port");
    }

    pObject = AllocateVSMemBlock(0x38);
    if (pObject == 0) {
        g_pEffTransportAuthoriseConnectControlStream = 0;
    } else {
        g_pEffTransportAuthoriseConnectControlStream =
            ConstructEffTransportAuthoriseConnectControlStream(pObject, (char *)"Authorise Connect");
    }

    pObject = AllocateVSMemBlock(0x38);
    if (pObject == 0) {
        g_pEffTransportGoAheadConnectControlStream = 0;
    } else {
        g_pEffTransportGoAheadConnectControlStream =
            ConstructEffTransportGoAheadConnectControlStream(pObject, (char *)"Go Ahead Connect");
    }

    pObject = AllocateVSMemBlock(0x34);
    if (pObject == 0) {
        g_pEffTransportFailedConnectControlStream = 0;
    } else {
        g_pEffTransportFailedConnectControlStream =
            ConstructEffTransportFailedConnectControlStream(pObject, (char *)"Failed Connect");
    }

    pObject = AllocateVSMemBlock(0x2c);
    if (pObject == 0) {
        g_pEffTransportGlobalWriteStream = 0;
    } else {
        *(void **)pObject = g_GAME_EffStreamConstructionVtable;
        *(int *)((char *)pObject + 4) = 1;
        ((GameEffStream *)pObject)->ResetStateFields();
        *(void **)pObject = g_NETWORK_EffTransportGlobalWriteStreamVtable;
        ((GameEffStream *)pObject)->BeginEffStreamWriteSession();
        g_pEffTransportGlobalWriteStream = pObject;
    }

    pObject = AllocateVSMemBlock(0x30);
    if (pObject == 0) {
        g_pEffTransportGlobalReadStream = 0;
    } else {
        *(void **)pObject = g_GAME_EffStreamConstructionVtable;
        *(int *)((char *)pObject + 4) = 2;
        ((GameEffStream *)pObject)->ResetStateFields();
        *(void **)pObject = g_NETWORK_EffTransportGlobalReadStreamVtable;
        *(int *)((char *)pObject + 0x18) = *(int *)((char *)pObject + 0x18) + 4;
        g_pEffTransportGlobalReadStream = pObject;
    }

    pState->m_pActiveChannelOwner = (this->*pVtable->m_pCreateChannelOwner)();
    pState->m_fTransportInitialized = 1;
    pState->m_fRuntimeActive = 1;
    (pState->*pVtable->m_pServiceTransport)();

    if (pState->m_fRuntimeActive != 0 && g_nEffTransportAsyncErrorStatus == 0) {
        pOwner = (VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner;
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
int VsNetRuntimeStartView::StartEffTransportRuntimeAndWaitReady(int nRuntimeKey, int cbMaxPacket) {
    VsNetEffTransportRuntimeState *pState;
    VsNetEffTransportRuntimeStateVtable *pVtable;
    DWORD dwStartTime;
    DWORD dwNow;
    DWORD dwDelayStart;

    pState = (VsNetEffTransportRuntimeState *)this;
    pVtable = (VsNetEffTransportRuntimeStateVtable *)pState->m_RenderQueueNode.m_pVtable;
    pState->m_nRuntimeKey = nRuntimeKey;
    pState->m_fStartRequested = 1;
    g_cbEffTransportMaxPacketBytes = cbMaxPacket;
    (pState->*pVtable->m_pServiceTransport)();

    dwStartTime = timeGetTime();
    if (pState->m_fTransportInitialized == 0) {
        do {
            if (g_nEffTransportAsyncErrorStatus != 0) {
                break;
            }
            dwNow = timeGetTime();
            if (dwNow - dwStartTime >= 10000) {
                break;
            }
            (pState->*pVtable->m_pWaitForTransportMessage)();
        } while (pState->m_fTransportInitialized == 0);

        if (pState->m_fTransportInitialized == 0) {
            return 0;
        }
    }

    dwStartTime = timeGetTime();
    if (pState->m_fRuntimeActive != 0) {
        do {
            VsNetChannelOwnerObject *pActiveOwner;

            if (pState->m_fRuntimeActive != 0) {
                pActiveOwner = (VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner;
                if (pActiveOwner != 0) {
                    if (*(int *)((char *)pActiveOwner +
                                 pActiveOwner->m_pOffsets->m_nEndpointStateSlotOffset +
                                 0x14) != 0) {
                        break;
                    }
                }
            }

            if (g_nEffTransportAsyncErrorStatus != 0) {
                break;
            }
            dwNow = timeGetTime();
            if (dwNow - dwStartTime >= 10000) {
                break;
            }

            dwDelayStart = timeGetTime();
            do {
                dwNow = timeGetTime();
            } while (dwNow - dwDelayStart < 100);

            (pState->*pVtable->m_pServiceTransport)();
        } while (pState->m_fRuntimeActive != 0);
    }

    if (pState->m_fRuntimeActive != 0) {
        VsNetChannelOwnerObject *pActiveOwner;

        pActiveOwner = (VsNetChannelOwnerObject *)pState->m_pActiveChannelOwner;
        if (pActiveOwner != 0) {
            if (*(int *)((char *)pActiveOwner +
                         pActiveOwner->m_pOffsets->m_nEndpointStateSlotOffset +
                         0x14) != 0 &&
                g_nEffTransportAsyncErrorStatus == 0) {
                return 1;
            }
        }
    }

    pState->m_fShutdownRequested = 1;
    (pState->*pVtable->m_pServiceTransport)();

    dwStartTime = timeGetTime();
    do {
        if (pState->m_fTransportInitialized == 0) {
            break;
        }
        dwNow = timeGetTime();
    } while (dwNow - dwStartTime < 10000);

    return 0;
}

// FUNCTION: LEMBALL 0x0046F6B0
void *VsNetFileBasedRuntimeWindow::ConstructFileBasedNetworkRuntimeWindow(void) {
    ((VS_InvisibleWindow *)this)
        ->Construct("File-based Network", &g_fFileBasedRuntimeWindowClassRegistered);
    m_Base.m_RuntimeState.ConstructEffTransportRuntimeState();
    *(void **)this = (void *)g_NETWORK_FileBasedRuntimeWindowVtable;
    m_nReserved78 = 0;
    m_Base.m_RuntimeState.m_RenderQueueNode.m_pVtable =
        (void **)&g_NETWORK_FileBasedRuntimeStateVtable;
    return this;
}

// FUNCTION: LEMBALL 0x0046F210
DWORD FileBasedNetworkMessageThreadMain(void) {
    VsNetEffTransportRuntimeState *pActiveRuntime;
    VsNetEffTransportRuntimeStateVtable *pVtable;
    VsNetRenderDispatchQueue *pDispatchQueue;
    MSG Msg;
    void *pRuntimeWindow;
    int fRuntimeStarted;

    pRuntimeWindow = AllocateVSMemBlock(0x7c);
    if (pRuntimeWindow != 0) {
        pRuntimeWindow =
            ((VsNetFileBasedRuntimeWindow *)pRuntimeWindow)
                ->ConstructFileBasedNetworkRuntimeWindow();
    }

    if (pRuntimeWindow == 0) {
        g_pActiveNetworkRuntimeWindow = 0;
    } else {
        g_pActiveNetworkRuntimeWindow = (char *)pRuntimeWindow + 0x10;
    }

    if (g_pActiveNetworkRuntimeWindow == 0) {
        return 0;
    }

    pActiveRuntime = (VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
    fRuntimeStarted = pActiveRuntime->m_fStartRequested;
    while (fRuntimeStarted == 0 && pActiveRuntime->m_fRuntimeActive == 0 &&
           pActiveRuntime->m_fShutdownRequested == 0) {
        WaitMessage();
        GetMessageA(&Msg, 0, 0, 0);
        fRuntimeStarted = pActiveRuntime->m_fStartRequested;
    }

    if (pActiveRuntime->m_fStartRequested != 0) {
        ((VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow)
            ->InitializeEffTransportRuntimeGlobals();
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

            pDispatchQueue = (VsNetRenderDispatchQueue *)g_pEffTransportDispatchQueue;
            while (pDispatchQueue != 0 && pDispatchQueue->m_cQueuedEntries != 0) {
                DrainRenderDispatchQueueEntries(g_pEffTransportDispatchQueue, (unsigned int)pDispatchQueue->m_cQueuedEntries);
            }

            fRuntimeStarted = pActiveRuntime->m_fRuntimeActive;
        }
    }

    if (g_pActiveNetworkRuntimeWindow != 0) {
        pActiveRuntime = (VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
        pVtable = (VsNetEffTransportRuntimeStateVtable *)pActiveRuntime->m_RenderQueueNode.m_pVtable;
        (pActiveRuntime->*pVtable->m_pDelete)(1);
    }
    g_pActiveNetworkRuntimeWindow = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x0046F3B0
int StartFileBasedNetworkMessageThread(void) {
    DWORD dwStartTime;

    g_hFileBasedNetworkThread = CreateThread(
        0, 0, (LPTHREAD_START_ROUTINE)FileBasedNetworkMessageThreadMain, 0, 0,
        &g_dwFileBasedNetworkThreadId);
    if (g_hFileBasedNetworkThread == 0) {
        MessageBoxA(0, "Unable to start 'VSNET Message loop' thread\n", "ERROR", 0);
        ExitProcess(0xbbbb);
    }

    SetThreadPriority(g_hFileBasedNetworkThread, 2);

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pActiveNetworkRuntimeWindow == 0) {
    }
    if (g_pActiveNetworkRuntimeWindow == 0) {
        AppendCStringToStream(g_pErrorOutputStream, "Network initialisation timed out\n");
        return 0;
    }

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pEffTransportDispatchQueue == 0) {
    }
    if (g_pEffTransportDispatchQueue == 0) {
        AppendCStringToStream(g_pErrorOutputStream, "Network queue initialisation timed out\n");
        return 0;
    }

    return 1;
}

// FUNCTION: LEMBALL 0x0046F480
int StopFileBasedNetworkMessageThread(void) {
    VsNetEffTransportRuntimeState *pActiveRuntime;
    VsNetEffTransportRuntimeStateVtable *pVtable;
    DWORD dwStartTime;

    if (g_pActiveNetworkRuntimeWindow == 0) {
        return 0;
    }

    pActiveRuntime = (VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
    pVtable = (VsNetEffTransportRuntimeStateVtable *)pActiveRuntime->m_RenderQueueNode.m_pVtable;
    pActiveRuntime->m_fShutdownRequested = 1;
    (pActiveRuntime->*pVtable->m_pServiceTransport)();

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pActiveNetworkRuntimeWindow != 0) {
    }
    if (g_pActiveNetworkRuntimeWindow != 0) {
        AppendCStringToStream(g_pErrorOutputStream, "Network quit timed out\n");
        return 0;
    }

    return 1;
}

// FUNCTION: LEMBALL 0x0046F740
void VsNetRuntimeWindowBase::ScheduleNetworkRuntimeTimerEvent(UINT nMilliseconds) {
    VsInitFormattedOutputStream *pStream;

    pStream = AppendCStringToStream(g_pStatusOutputStream,
                                    "Setting next timer event to ");
    AppendUnsignedIntToStreamVariant(pStream, nMilliseconds);
    AppendCStringToStream(pStream, "ms from now\n");

    KillTimer(*(HWND *)((char *)this + 4),
              *(unsigned int *)((char *)this + 0x74));
    *(unsigned int *)((char *)this + 0x74) =
        SetTimer(*(HWND *)((char *)this + 4), 0x12345679,
                 nMilliseconds, (void *)0);
    *(unsigned int *)((char *)this + 0x78) =
        *(unsigned int *)((char *)this + 0x78) == 0;
}

// FUNCTION: LEMBALL 0x0046F6F0
void VsNetEffTransportRuntimeState::InitializeFileBasedNetworkTransport(void) {
    AppendCStringToStream(g_pStatusOutputStream, "Network Initialised:\n");
    AppendCStringToStream(g_pStatusOutputStream, "Windows file-based networking\n");
    m_nTimerId64 = SetTimer(*(HWND *)((char *)this - 0x0c),
                            0x12345679, 0x14, (void *)0);
}

// FUNCTION: LEMBALL 0x0046F730
unsigned int VsNetEffTransportRuntimeState::GetNetworkRuntimeTimerId(void) {
    return m_nTimerId64;
}

void VsNetEffTransportRuntimeState::FallbackInitializeTransport(void) {
    m_fShutdownRequested = 1;
    g_nEffTransportAsyncErrorStatus = 1;
}

void VsNetEffTransportRuntimeState::FallbackServiceTransport(void) {
}

void *VsNetEffTransportRuntimeState::FallbackCreateChannelOwner(void) {
    return 0;
}

void VsNetEffTransportRuntimeState::FallbackWaitForTransportMessage(void) {
}

// FUNCTION: LEMBALL 0x004713F0
void VsNetEffTransportRuntimeState::InitializeTcpipNetworkTransport(void) {
    char abWsaData[0x190];
    int nWsaResult;
    int nMaxUdpDatagram;

    nWsaResult = WSAStartup(0x101, abWsaData);
    if (nWsaResult != 0) {
        g_nTcpipWsaError = WSAGetLastError();
        g_nEffTransportAsyncErrorStatus = 1;
        return;
    }

    nMaxUdpDatagram = *(int *)(abWsaData + 0x18c) & 0xffff;
    if (g_nTcpipMaxUdpDatagram <= nMaxUdpDatagram) {
        g_nTcpipMaxUdpDatagram = nMaxUdpDatagram;
    }
    m_nTimerId64 = SetTimer(*(HWND *)((char *)this - 0x0c),
                            0x12345678, 10, (void *)0);
}

// FUNCTION: LEMBALL 0x00471460
void VsNetEffTransportRuntimeState::ShutdownTcpipNetworkTransport(void) {
    KillTimer(*(HWND *)((char *)this - 0x0c), m_nTimerId64);
    WSACleanup();
}

// FUNCTION: LEMBALL 0x004625E0
void VsNetEffTransportRuntimeState::CleanupEffTransportRuntimeState(void) {
    VsNetEffTransportRuntimeState *pActiveState;
    void *pOwner;
    void *pPeer;
    void *pNextPeer;
    void (**ppVtable)(void);

    if (m_nReserved3C != 0) {
        ((GameRenderDispatchQueue *)g_pEffTransportDispatchQueue)
            ->RegisterOrderedRenderDispatchClient((void *)(unsigned long)m_nReserved3C, 0);
        m_nReserved48 = 1;
        m_nReserved38 = m_nReserved3C;
        m_nReserved3C = 0;
    }
    if (m_nReserved40 != 0) {
        ((GameRenderDispatchQueue *)g_pEffTransportDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient((void *)(unsigned long)m_nReserved40, 0);
        m_nReserved40 = 0;
        m_nReserved38 = 0;
        m_nReserved48 = 0;
    }

    pActiveState = (VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
    if (pActiveState != 0 && pActiveState->m_fTransportInitialized != 0) {
        pActiveState->InitializeEffTransportRuntimeGlobals();
        return;
    }
    if (m_fShutdownRequested != 0) {
        ReleaseEffTransportRuntimeOwnerState();
        return;
    }

    pOwner = m_pActiveChannelOwner;
    if (pOwner != 0) {
        ppVtable = *(void (***)(void))pOwner;
        ppVtable[4]();
    }

    pPeer = m_pFirstPeer;
    while (pPeer != 0) {
        ppVtable = *(void (***)(void))pPeer;
        ppVtable[1]();
        pNextPeer = *(void **)((char *)pPeer + 0x10);
        pPeer = pNextPeer;
    }

    if (m_nReserved38 != 0) {
        ppVtable = *(void (***)(void))(unsigned long)m_nReserved38;
        ppVtable[3]();
    }
}

// FUNCTION: LEMBALL 0x0046F7C0
int VsNetEffTransportRuntimeState::LoadActiveFileRange(void) {
    return ((VsNetLockedEffStreamFileRangeView *)m_pActiveChannelOwner)
        ->LoadLockedEffStreamFromFileRange();
}

// FUNCTION: LEMBALL 0x0046F7D0
int VsNetEffTransportRuntimeState::WriteActiveFileRange(void) {
    return ((VsNetLockedEffStreamFileRangeView *)m_pActiveChannelOwner)
        ->WriteLockedEffStreamToFileRange();
}

int VsNetEffTransportRuntimeState::FallbackRuntimeStateSlot(void) {
    return 0;
}

void *VsNetEffTransportRuntimeState::CreateFileNetworkAddress(void) {
    return AllocateFileNetworkAddress();
}

// FUNCTION: LEMBALL 0x0046F930
void *VsNetEffTransportRuntimeState::CreateGlobalStateFileBackedComposite(void) {
    void *pObject;

    pObject = AllocateVSMemBlock(0x1b8);
    if (pObject == 0) {
        return 0;
    }
    return ((VsNetGlobalStateCompositeLayout *)pObject)
        ->ConstructGlobalStateFileBackedEffComposite(1);
}

// FUNCTION: LEMBALL 0x004714D0
void VsNetEffTransportRuntimeState::ServiceTcpipNetworkTransport(void) {
    PostMessageA(*(HWND *)((char *)this - 0x0c), 0x444, 0, 0);
}

void *VsNetEffTransportRuntimeState::CreateTcpipChannelOwner(void) {
    return AllocateTcpipEffTransportRuntimeOwner();
}

void *VsNetEffTransportRuntimeState::CreateTcpipSecondaryOwner(void) {
    return AllocateTcpipEffTransportComposite();
}

// FUNCTION: LEMBALL 0x0046F840
void VsNetEffTransportRuntimeState::ServiceFileBasedNetworkTransport(void) {
    PostMessageA(*(HWND *)((char *)this - 0x0c), 0x444, 0, 0);
}

// FUNCTION: LEMBALL 0x00462AC0
void VsNetEffTransportRuntimeState::WaitForFileBasedNetworkMessage(void) {
}

// FUNCTION: LEMBALL 0x0046F950
void *VsNetEffTransportRuntimeState::CreateFileBasedChannelOwner(void) {
    void *pObject;

    pObject = AllocateVSMemBlock(0x1b4);
    if (pObject == 0) {
        return 0;
    }
    return ((VsNetDualSlotCompositeLayout *)pObject)
        ->ConstructDualSlotTableFileBackedEffComposite(1);
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

    AppendCStringToStream(g_pStatusOutputStream, "Found Local host OK: ");
    AppendCStringToStream(g_pStatusOutputStream, g_pszFileBasedNetworkLocalHostName);
    AppendCStringToStream(g_pStatusOutputStream, "\n");

    g_fFileBasedNetworkPathConfigured = 1;
    cchPath = strlen(pszBasePath);
    g_pszFileBasedNetworkConfiguredPath = (char *)AllocateVSMemBlock((unsigned int)(cchPath + 0xe));
    memcpy(g_pszFileBasedNetworkConfiguredPath, pszBasePath, cchPath + 1);

    if (g_pszFileBasedNetworkConfiguredPath[cchPath - 1] != '\\' &&
        g_pszFileBasedNetworkConfiguredPath[cchPath - 1] != ':') {
        strcat(g_pszFileBasedNetworkConfiguredPath, "\\");
    }

    strcat(g_pszFileBasedNetworkConfiguredPath, "VSNETv");

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
void *VsNetRuntimeWindowBase::ConstructTcpipNetworkRuntimeWindow(void) {
    ((VS_InvisibleWindow *)this)
        ->Construct("TCPIP Network", &g_fTcpipRuntimeWindowClassRegistered);
    m_RuntimeState.ConstructEffTransportRuntimeState();
    *(void **)this = (void *)g_NETWORK_TcpipRuntimeWindowVtable;
    m_RuntimeState.m_RenderQueueNode.m_pVtable =
        (void **)&g_NETWORK_TcpipRuntimeStateVtable;
    return this;
}

// FUNCTION: LEMBALL 0x0046FA10
DWORD TcpipNetworkMessageThreadMain(void) {
    VsNetEffTransportRuntimeState *pActiveRuntime;
    VsNetEffTransportRuntimeStateVtable *pVtable;
    VsNetRenderDispatchQueue *pDispatchQueue;
    MSG Msg;
    void *pRuntimeWindow;
    int fRuntimeStarted;

    pRuntimeWindow = AllocateVSMemBlock(0x78);
    if (pRuntimeWindow != 0) {
        pRuntimeWindow =
            ((VsNetRuntimeWindowBase *)pRuntimeWindow)
                ->ConstructTcpipNetworkRuntimeWindow();
    }

    if (pRuntimeWindow == 0) {
        g_pActiveNetworkRuntimeWindow = 0;
    } else {
        g_pActiveNetworkRuntimeWindow = (char *)pRuntimeWindow + 0x10;
    }

    if (g_pActiveNetworkRuntimeWindow == 0) {
        return 0;
    }

    pActiveRuntime = (VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
    fRuntimeStarted = pActiveRuntime->m_fStartRequested;
    while (fRuntimeStarted == 0 && pActiveRuntime->m_fRuntimeActive == 0 &&
           pActiveRuntime->m_fShutdownRequested == 0) {
        WaitMessage();
        GetMessageA(&Msg, 0, 0, 0);
        fRuntimeStarted = pActiveRuntime->m_fStartRequested;
    }

    if (pActiveRuntime->m_fStartRequested != 0) {
        ((VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow)
            ->InitializeEffTransportRuntimeGlobals();
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

            pDispatchQueue = (VsNetRenderDispatchQueue *)g_pEffTransportDispatchQueue;
            while (pDispatchQueue != 0 && pDispatchQueue->m_cQueuedEntries != 0) {
                DrainRenderDispatchQueueEntries(g_pEffTransportDispatchQueue, (unsigned int)pDispatchQueue->m_cQueuedEntries);
            }

            fRuntimeStarted = pActiveRuntime->m_fRuntimeActive;
        }
    }

    if (g_pActiveNetworkRuntimeWindow != 0) {
        pActiveRuntime = (VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
        pVtable = (VsNetEffTransportRuntimeStateVtable *)pActiveRuntime->m_RenderQueueNode.m_pVtable;
        (pActiveRuntime->*pVtable->m_pDelete)(1);
    }
    g_pActiveNetworkRuntimeWindow = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x0046FBB0
int StartTcpipNetworkMessageThread(void) {
    DWORD dwStartTime;

    g_hTcpipNetworkThread = CreateThread(
        0, 0, (LPTHREAD_START_ROUTINE)TcpipNetworkMessageThreadMain, 0, 0,
        &g_dwTcpipNetworkThreadId);
    if (g_hTcpipNetworkThread == 0) {
        MessageBoxA(0, "Unable to start 'VSNET Message loop' thread\n", "ERROR", 0);
        ExitProcess(0xbbbb);
    }

    SetThreadPriority(g_hTcpipNetworkThread, 2);

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pActiveNetworkRuntimeWindow == 0) {
    }
    if (g_pActiveNetworkRuntimeWindow == 0) {
        AppendCStringToStream(g_pErrorOutputStream, "Network initialisation timed out\n");
        return 0;
    }

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pEffTransportDispatchQueue == 0) {
    }
    if (g_pEffTransportDispatchQueue == 0) {
        AppendCStringToStream(g_pErrorOutputStream, "Network queue initialisation timed out\n");
        return 0;
    }

    return 1;
}

// FUNCTION: LEMBALL 0x0046FC80
int StopTcpipNetworkMessageThread(void) {
    VsNetEffTransportRuntimeState *pActiveRuntime;
    VsNetEffTransportRuntimeStateVtable *pVtable;
    DWORD dwStartTime;

    if (g_pActiveNetworkRuntimeWindow == 0) {
        return 0;
    }

    pActiveRuntime = (VsNetEffTransportRuntimeState *)g_pActiveNetworkRuntimeWindow;
    pVtable = (VsNetEffTransportRuntimeStateVtable *)pActiveRuntime->m_RenderQueueNode.m_pVtable;
    pActiveRuntime->m_fShutdownRequested = 1;
    (pActiveRuntime->*pVtable->m_pServiceTransport)();

    dwStartTime = timeGetTime();
    while (timeGetTime() - dwStartTime <= 9999 && g_pActiveNetworkRuntimeWindow != 0) {
    }
    if (g_pActiveNetworkRuntimeWindow != 0) {
        AppendCStringToStream(g_pErrorOutputStream, "Network quit timed out\n");
        return 0;
    }

    return 1;
}
