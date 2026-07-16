#include "GAME.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/GDI/VSINVIS.H"
#include "ENGINE/MEDIA/VSSTRM.H"
#include "ENGINE/NET/VSNET.H"
#include "ENGINE/CORE/SAFEVT.H"
#include <string.h>

#include <stdlib.h>

#ifdef ntohs
#undef ntohs
#endif
#ifdef htons
#undef htons
#endif

extern int PASCAL ReturnTrueVtableCallback(void *pObject);
extern int PASCAL ReturnTrueVtableCallbackThunk(void *pObject);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void NoopVtableCallbackThunk(void);
extern void ReturnVoidVtableCallback(void);
extern void SetTimedFileBackedEffChannelWord(void *pObject, unsigned short nValue);
extern void EffStreamChannelStateRet4Thunk(BYTE fDelete);

// FUNCTION: LEMBALL 0x004629D0
static void TcpipCompositeUnusedVtableSlot(void) {
}

struct VsNetConstructionAdjustorVtable {
    void *m_pReserved00;
    int m_nPrimaryOffset;
    int m_nSecondaryOffset;
    int m_nTertiaryOffset;
    int m_nQuaternaryOffset;
    int m_nOffset14;
    int m_nOffset18;
    int m_nOffset1c;
};
extern void *g_NETWORK_TcpipSocketStackDualRet8;
extern void *g_NETWORK_RuntimeChannelStackVtable;
extern void RuntimeDualUnusedCallback(void);
extern "C" int WINAPI WSAAsyncGetHostByName(int nSocket, unsigned int nMessage,
                                               const char *pszHost, void *pBuffer,
                                               int cbBuffer);
struct VsNetPrefixedEffTransportControlView {
    void DispatchPrefixedEffTransportControlStream(int nDispatchType, void *pEvent);
};

struct VsNetAdjustorSubobject {
    int m_nThisDelta;
    void *m_pVtable;
};

struct VsNetCompleteEffTransportPendingWriteView {
    void CompleteEffTransportPendingWrite(int fQueueEvent);
};

struct VsNetThunkAdjustorGroup {
    VsNetConstructionAdjustorVtable *m_pOffsets;
    VsNetAdjustorSubobject *m_pPrimary;
    VsNetAdjustorSubobject *m_pSecondary;
    VsNetAdjustorSubobject *m_pTertiary;
};

struct VsNetQuadThunkAdjustorGroup {
    VsNetConstructionAdjustorVtable *m_pOffsets;
    VsNetAdjustorSubobject *m_pPrimary;
    VsNetAdjustorSubobject *m_pSecondary;
    VsNetAdjustorSubobject *m_pTertiary;
    VsNetAdjustorSubobject *m_pQuaternary;
};

class VsNetSocketWindowEffChannel : public VS_InvisibleWindow {
public:
    VsNetConstructionAdjustorVtable *m_pEmbeddedStateConstructionOffsets;
    int m_nSocketHandle;
    int m_nLookupHandle;
    VsNetEffStreamChannelState m_ChannelState1c;
    unsigned char m_abChannelStateTail48[4];

    void *ConstructSocketWindowEffChannel(int fConstructEmbeddedState);
};

class VsNetTimedSocketAsyncLookupAdapter {
public:
    void CaptureAsyncLookupAddressForTimedStream(void *pLookupResult);
    int SendUdpPayloadToSocketAddress(void *pBuffer, int cbBuffer);
};

class VsNetTimedSocketAsyncMessageAdapter {
public:
    int HandleTimedSocketConnectAsyncMessage(int nMessage, void *pReserved, unsigned int nEvent);
    int DispatchEffSocketAsyncSelectMessage(int nMessage, void *pReserved, unsigned int nEvent);
};

class VsNetSocketAsyncMessageAdapter {
public:
    int ReceiveUdpPayloadFromConnectedSocket(void);
    int ReceiveUdpPayloadFromSenderAddress(void);
    int HandleSocketReceiveAsyncSelectMessage(int nMessage, void *pReserved, unsigned int nEvent);
};

class VsNetSocketAsyncErrorAdapter {
public:
    int HandleSocketAsyncErrorLparam(void *pReserved, unsigned int nEvent, unsigned int *pBuffer);
};

class VsNetTcpipClientAsyncDispatcher {
public:
    int DispatchTcpipClientAsyncMessage(int nMessage, void *pReserved, unsigned int nEvent);
};

struct VsNetUdpSocketBinder {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void PrepareBind(void) = 0;
    void BindUdpSocketAndEnableAsyncSelect(void);
};

class VsNetTcpipHostPortAsyncDispatcher {
public:
    int DispatchTcpipHostPortAsyncMessage(int nMessage, void *pReserved, unsigned int nEvent);
};

class VsNetSocketWindowChannelWrapper {
public:
    void CloseSocketHandleFromChannel(void);
    void *DeleteSocketWindowChannelStateWrapper(BYTE fFreeMemory);
    void CaptureLastWinsockErrorStatus(void);
};
struct VsNetAdjustedEffTransportPeerView {
    void CloseAdjustedEffTransportPeerByKey(void *pUnused);
};
void *DeleteDualSocketWindowChannelStackWrapper(void *pObject, BYTE fFreeMemory);
void *DeleteCompositeTcpipSocketChannelStackWrapper(void *pObject, BYTE fFreeMemory);
void *DeleteTcpipSocketChannelStackWrapper(void *pObject, BYTE fFreeMemory);

struct VsNetTcpipSocketChannelStackWrapperView {
    VsNetEffStreamChannelState m_ChannelState00;
    unsigned char m_abChannelStateTail2c[4];
    unsigned char m_abTimedStream30[0x78];
    unsigned char m_abDualStreamA8[0x44];
    unsigned char m_abReservedec[0x3c];
    unsigned char m_abSocketWindow128[0x20];
    void *DeleteCompositeTcpipSocketChannelStackWrapper(BYTE fFreeMemory);
    void *DeleteTcpipSocketChannelStackWrapper(BYTE fFreeMemory);
    void *DeleteTcpipEffTransportCompositeWrapper(BYTE fFreeMemory);
};

// FUNCTION: LEMBALL 0x00471B60
void CaptureLastWinsockErrorStatusOuterAdjustedThunk(void *pObject) {
    ((VsNetSocketWindowChannelWrapper *)((char *)pObject + 0x128))
        ->CaptureLastWinsockErrorStatus();
}

// FUNCTION: LEMBALL 0x00471AF0
void CaptureLastWinsockErrorStatusOuterDoubleAdjustedThunk(void *pObject) {
    CaptureLastWinsockErrorStatusOuterAdjustedThunk((char *)pObject + 0x128);
}

// FUNCTION: LEMBALL 0x00471B00
void *DeleteCompositeTcpipSocketChannelStackAdjustedThunk(void *pObject,
                                                            BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetTcpipSocketChannelStackWrapperView *)pObject)
        ->DeleteCompositeTcpipSocketChannelStackWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00471B80
void CompleteEffTransportPendingWriteAllocatedThunkBody(void *pObject,
                                                          int fQueueEvent) {
    pObject = (char *)pObject - 4;
    ((VsNetCompleteEffTransportPendingWriteView *)pObject)
        ->CompleteEffTransportPendingWrite(fQueueEvent);
}

// FUNCTION: LEMBALL 0x00471B70
void CompleteEffTransportPendingWriteAllocatedAdjustedThunk(void *pObject,
                                                              int fQueueEvent) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    CompleteEffTransportPendingWriteAllocatedThunkBody(pObject, fQueueEvent);
}

// FUNCTION: LEMBALL 0x00471FB0
void CloseAdjustedEffTransportPeerByKeyAdjustedThunk(void *pObject, void *pArgument) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x0c;
    ((VsNetAdjustedEffTransportPeerView *)pObject)
        ->CloseAdjustedEffTransportPeerByKey(pArgument);
}


struct VsNetSocketWindowEffChannelVtableModel {
    // FUNCTION: LEMBALL 0x004714F0
    virtual int HandleAsyncMessage(int nMessage, void *pReserved, unsigned int nEvent) {
        (void)nMessage;
        (void)pReserved;
        (void)nEvent;
        return -1;
    }
};
// FUNCTION: LEMBALL 0x00471A50
void CloseSocketHandleFromChannelAdjustedThunk(void *pObject) {
    ((VsNetSocketWindowChannelWrapper *)
         ((char *)pObject - *(int *)((char *)pObject - 4)))
        ->CloseSocketHandleFromChannel();
}

// FUNCTION: LEMBALL 0x00471A80
void *DeleteSocketWindowChannelStateAdjustedThunk(void *pObject, BYTE fFreeMemory) {
    return ((VsNetSocketWindowChannelWrapper *)
                ((char *)pObject - *(int *)((char *)pObject - 4)))
        ->DeleteSocketWindowChannelStateWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00471AC0
void CaptureLastWinsockErrorStatusAdjustedThunk(void *pObject) {
    ((VsNetSocketWindowChannelWrapper *)
         ((char *)pObject - *(int *)((char *)pObject - 4)))
        ->CaptureLastWinsockErrorStatus();
}

class VsNetHostLookupCompletionAdapter {
public:
    void HandleRemoteHostLookupComplete(int nError);
    void HandleLocalHostLookupComplete(int nError);
    void HandleServicePortLookupComplete(int nError);
    void HandleRemoteServiceHostLookupComplete(int nError);
    void HandleRemoteServicePortLookupComplete(int nError);
};

class VsNetInetAddressEntry {
public:
    virtual const char *GetAddressString(void);
    virtual void SetAddressMode(int nMode);
    virtual void StoreAddressString(const char *pszAddress);
    virtual void CopyAddressEntry(const VsNetInetAddressEntry *pSource);
    virtual int IsAddressLessOrEqual(const VsNetInetAddressEntry *pSource);
    virtual int IsAddressGreaterOrEqual(const VsNetInetAddressEntry *pSource);
    char m_szAddress[16];
    int m_nAddress;
};

struct VsNetEffDispatchQueue {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void QueueEvent(void *pEvent) = 0;
};

class VsNetTimedSocketErrorChannel {
public:
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void HandleSocketError(void) = 0;
};

class VsNetSocketChannelControl {
public:
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void HandleSocketError(void) = 0;
    virtual void Reserved0c(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved1c(void) = 0;
    virtual void SetRemoteAddress(void *pAddress) = 0;
    virtual void Reserved24(void) = 0;
    virtual void SetAssignedPort(short nPort) = 0;
};

class VsNetTcpipLookupOwner {
public:
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0c(void) = 0;
    virtual void BeginPortSetup(void) = 0;
};

class VsNetTimedSocketPacketChannel {
public:
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0c(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void NotifyPacketReady(void) = 0;
};

class VsNetTimedSocketEffChannelBundleLayout {
public:
    void **m_pVtable00;
    short m_nMode04;
    unsigned char m_abReserved06[0x12];
    VsNetEffStreamChannelState m_ChannelState18;
    unsigned char m_abTimedStream48[0x44];
    void **m_pTimedStreamConstructionOffsets8c;
    unsigned char m_abReserved90[0x40];
    void **m_pSocketChannelConstructionOffsetsd0;
    unsigned char m_abReservedd4[0x0c];
    unsigned char m_abSocketWindowC0[0x20];

    void *ConstructTimedSocketEffChannelBundle(int fConstructEmbeddedObjects);
};

struct VsNetTcpipEffTransportCompositeLayout {
    void **m_pVtable00;
    VsNetConstructionAdjustorVtable *m_pOuterOffsets04;
    int m_nRuntimeField08;
    int m_nRuntimeField0c;
    int m_nRuntimeField10;
    int m_nRuntimeField14;
    int m_nRuntimeField18;
    int m_nRuntimeField1c;
    VsNetConstructionAdjustorVtable *m_pTransportOffsets20;
    int m_nSocketWindowHandle24;
    int m_nLookupHandle28;
    int m_nReserved2c;
    VsNetEffStreamChannelState m_ChannelState30;
    unsigned char m_abTimedStream60[0x44];
    VsNetConstructionAdjustorVtable *m_pDualStreamConstructionOffsetsA4;
    unsigned char m_abTimedStreamA8[0x30];
    unsigned char m_abDualStreamD8[0x44];
    VsNetConstructionAdjustorVtable *m_pDualThunkOffsets11c;
    unsigned char m_abReserved120[0x10];
    VsNetConstructionAdjustorVtable *m_pSocketThunkOffsets130;
    unsigned char m_abReserved134[0x04];
    unsigned char m_abSocketWindow138[0x10];
    VsNetConstructionAdjustorVtable *m_pTimedSocketThunkOffsets148;
    unsigned char m_abSocketWindow14c[0x08];
    VsNetConstructionAdjustorVtable *m_pTimedSocketBundleOffsets154;
    void **m_pTimedSocketBundleConstructionData158;
    unsigned char m_abTimedSocketBundle15c[0x14];

    void *ConstructTcpipEffTransportComposite(int fConstructEmbeddedObjects);
};

struct VsNetTcpipCompositeControlVtableModel {
    virtual void Reserved00(void) {}
    virtual void Reserved04(void) {}
    virtual void ConfigureRemoteHost(const char *pszHost);
    virtual int StartLocalHostLookup(char *pszHost);
    virtual void Reserved10(void) {}
    virtual void EnableAsyncSelect(void);
    virtual void DisableAsyncSelect(void);
};

/* The allocator is a multiple-subobject layout.  Its metadata is not at the
 * offsets used by the standalone 00470270 composite. */
struct VsNetAllocatedTcpipEffTransportCompositeLayout {
    void **m_pVtable00;
    VsNetConstructionAdjustorVtable *m_pOuterOffsets04;
    unsigned char m_abReserved08[0x24];
    VsNetConstructionAdjustorVtable *m_pTransportOffsets2c;
    VsNetEffStreamChannelState m_ChannelState30;
    unsigned char m_abTimedStream60[0x44];
    unsigned char m_abReservedA4[4];
    VsNetConstructionAdjustorVtable *m_pDualStreamOffsetsA8;
    unsigned char m_abReservedAC[0x2c];
    unsigned char m_abDualStreamD8[0x44];
    unsigned char m_abReserved11c[4];
    VsNetConstructionAdjustorVtable *m_pDualThunkOffsets120;
    unsigned char m_abReserved124[0x10];
    VsNetConstructionAdjustorVtable *m_pSocketThunkOffsets134;
    unsigned char m_abReserved138[4];
    unsigned char m_abSocketWindow13c[0x10];
    VsNetConstructionAdjustorVtable *m_pTimedSocketThunkOffsets14c;
    unsigned char m_abReserved150[8];
    VsNetConstructionAdjustorVtable *m_pTimedSocketBundleOffsets158;
    void **m_pTimedSocketBundleConstructionData15c;
};

struct VsNetTcpipSocketChannelStackLayout {
    VsNetConstructionAdjustorVtable *m_pBaseOffsets00;
    int m_nReserved04;
    VsNetEffStreamChannelState m_ChannelState08;
    unsigned char m_abChannelStateTail34[4];
    unsigned char m_abTimedStream38[0x44];
    VsNetConstructionAdjustorVtable *m_pTimedStreamConstructionOffsets7c;
    unsigned char m_abTimedStream80[0x30];
    unsigned char m_abDualStreamB0[0x44];
    VsNetConstructionAdjustorVtable *m_pDualStreamConstructionOffsetsf4;
    unsigned char m_abReservedf8[0x10];
    VsNetConstructionAdjustorVtable *m_pDualThunkOffsets108;
    unsigned char m_abReserved10c[0x04];
    unsigned char m_abSocketWindow110[0x10];
    VsNetConstructionAdjustorVtable *m_pTimedSocketBundleConstructionOffsets120;
    unsigned char m_abSocketWindow124[0x08];
    VsNetConstructionAdjustorVtable *m_pSocketWindowConstructionOffsets12c;
    void **m_pTimedSocketBundleConstructionData130;
    unsigned char m_abTimedSocketBundle134[0x14];

    void *ConstructTcpipSocketChannelStack(int fConstructEmbeddedObjects);
};

struct VsNetCompositeEffTransportStackLayout {
    void **m_pVtable00;
    VsNetConstructionAdjustorVtable *m_pChannelStateOffsets04;
    int m_nField08;
    int m_nField0c;
    int m_nField10;
    int m_nField14;
    int m_nField18;
    int m_nField1c;
    int m_nField20;
    DWORD m_dwLastActivity24;
    VsNetEffStreamChannelState m_ChannelState30;
    unsigned char m_abTimedStream60[0x44];
    VsNetConstructionAdjustorVtable *m_pTimedStreamOffsetsA4;
    unsigned char m_abReservedA8[0x74];
    VsNetConstructionAdjustorVtable *m_pDualThunkOffsets11c;
    unsigned char m_abReserved120[0x14];

    void *ConstructCompositeEffTransportStack(int fConstructEmbeddedObjects);
};

struct VsNetCompositeEffTransportStackWrapperView {
    VsNetEffStreamChannelState m_ChannelState00;
    unsigned char m_abChannelStateTail2c[4];
    unsigned char m_abTimedStream30[0x78];
    unsigned char m_abDualStreamA8[0x44];
    void *DeleteCompositeEffTransportRuntimeWrapper(BYTE fFreeMemory);
};

extern void *g_NETWORK_CompositeEffTransportVtable;

struct VsNetEffTransportConnectCallback {
    void QueueEffTransportConnectEvent(void);
};

struct VsNetDualSocketWindowChannelStackWrapperView {
    VsNetEffStreamChannelState m_ChannelState00;
    unsigned char m_abDualStream2c[0x78];
    unsigned char m_abSocketWindowA4[0x20];
    void *DeleteDualSocketWindowChannelStackWrapper(BYTE fFreeMemory);
};

struct VsNetTimedSocketWindowChannelStackWrapperView {
    VsNetEffStreamChannelState m_ChannelState00;
    unsigned char m_abChannelStateTail2c[4];
    unsigned char m_abTimedStream30[0x98];
    unsigned char m_abSocketWindowC8[0x20];
    void *DeleteTimedSocketWindowChannelStackWrapper(BYTE fFreeMemory);
};

static int g_NETWORK_CompositeEffTransportChannelStateConstructionOffsets[5] = {
    -4, 0x2c, 0x5c, 0xd4, 0x12c,
};
static int g_NETWORK_CompositeEffTransportTimedStreamConstructionOffsets[5] = {
    -0x44, -0x74, -4, 0x2c, 0x5c,
};
static int g_NETWORK_CompositeEffTransportDualStreamConstructionOffsets[4] = {
    -0x44, -0xec, -0x44, -0x74,
};
static int g_NETWORK_CompositeEffTransportDualThunkConstructionOffsets[4] = {
    0, -0x100, -0xd0, -0x58,
};
void ReverseEffTransportPayload(void *pObject);
void WriteEffTransportTaggedHeader(void *pObject);
void *DeleteCompositeEffTransportRuntimeAdjustedThunk(void *pObject, BYTE fFreeMemory);
void *DeleteCompositeEffTransportStackTimedAdjustedThunk(void *pObject, BYTE fFreeMemory);
void WriteEffTransportGlobalSessionAdjustedThunk(void *pObject);
void *DeleteCompositeEffTransportStackAdjustedThunk(void *pObject, BYTE fDelete);
void CompleteEffTransportPendingWriteAdjustedThunk(void *pObject, int fQueueEvent);
static void *g_NETWORK_CompositeEffTransportFatalThunkVtable[4] = {
    (void *)_purecall,
    (void *)DeleteCompositeEffTransportStackAdjustedThunk,
    (void *)_purecall,
    (void *)CompleteEffTransportPendingWriteAdjustedThunk,
};
static void *g_NETWORK_CompositeEffTransportTimedThunkVtable[11] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReverseEffTransportPayload,
    (void *)CloseAdjustedEffTransportPeerByKeyAdjustedThunk,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteCompositeEffTransportStackTimedAdjustedThunk,
    (void *)WriteEffTransportGlobalSessionAdjustedThunk,
    (void *)DeleteCompositeEffTransportRuntimeAdjustedThunk,
    (void *)_purecall,
    (void *)_purecall,
    (void *)SetTimedFileBackedEffChannelWord,
};
static void *g_NETWORK_CompositeEffTransportDualThunkVtable[1] = {
    (void *)NetworkSafeVtableNoop,
};
static void *g_NETWORK_CompositeEffTransportDualThunk =
    g_NETWORK_CompositeEffTransportDualThunkVtable;

/* Embedded channel-state construction data recovered from 0049a048.
 * Only first/primary offsets are consumed by this layout. */
static int g_NETWORK_SocketWindowEffChannelEmbeddedStateConstructionOffsets[2] = {
    -0x10, 0x10,
};
static VsNetConstructionAdjustorVtable *g_NETWORK_SocketWindowEffChannelEmbeddedStateVtable =
    (VsNetConstructionAdjustorVtable *)g_NETWORK_SocketWindowEffChannelEmbeddedStateConstructionOffsets;
static VsNetSocketWindowEffChannelVtableModel
    g_NETWORK_SocketWindowEffChannelVtableModel;
void *g_NETWORK_SocketWindowEffChannelVtable =
    *(void ***)&g_NETWORK_SocketWindowEffChannelVtableModel;
static void *g_NETWORK_SocketWindowEffChannelThunkVtable[4] = {
    (void *)CloseSocketHandleFromChannelAdjustedThunk,
    (void *)DeleteSocketWindowChannelStateAdjustedThunk,
    (void *)CaptureLastWinsockErrorStatusAdjustedThunk,
    (void *)EffStreamChannelStateRet4Thunk,
};

/* Construction offsets recovered from Ghidra data 0049a078/70/68. */
static int g_NETWORK_TimedSocketBundleBaseConstructionOffsets[4] = {
    0, 0x18, 0x48, 0xc0,
};
static int g_NETWORK_TimedSocketBundleTimedStreamConstructionOffsets[4] = {
    -0x44, -0x74, 0, 0x18,
};
static int g_NETWORK_TimedSocketBundleSocketChannelConstructionOffsets[4] = {
    -0x10, -0xb8, -0x44, -0x74,
};
void *DeleteTimedSocketWindowChannelStackWrapper(void *pObject, BYTE fDelete);
void *DeleteTcpipEffTransportCompositeWrapper(void *pObject, BYTE fDelete);
void *DeleteCompositeTcpipSocketChannelStackWrapper(void *pObject, BYTE fDelete);
void *DeleteCompositeEffTransportStackWrapper(void *pObject, BYTE fDelete);
struct VsNetCompleteEffTransportPendingWriteView;
struct VsNetDispatchAckedEffTransportPayloadView {
    void *DispatchAckedEffTransportPayloadEvent(void);
};
struct VsNetQueueEffTransportPayloadEventView {
    void QueueEffTransportPayloadEvent(unsigned short nType, void *pPayload);
};
struct VsNetAckedEffTransportRecordOwner {
    char m_abUnknown00[0x4c];
    void *m_pRecordTable4c;
    void *ClaimAckedEffTransportRecordPayload(void);
};
static void WriteCompositeTcpipGlobalSessionBody(void *pObject);
static void *ClaimTcpipSocketStackCompositeBody(void *pObject);
static void AllocatedTcpipCompositeCaptureLookupAdjustedThunk(
    void *pObject, void *pLookupResult);
static int AllocatedTcpipCompositeSendUdpAdjustedThunk(
    void *pObject, void *pBuffer, int cbBuffer);
static void AllocatedTcpipCompositeSetPortAdjustedThunk(void *pObject,
                                                          short nPort);

// FUNCTION: LEMBALL 0x00471C60
void *DeleteAllocatedTcpipSocketChannelStackAdjustedThunk(void *pObject,
                                                            BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0xa8;
    return ((VsNetTcpipSocketChannelStackWrapperView *)pObject)
        ->DeleteCompositeTcpipSocketChannelStackWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00471C70
void QueueAllocatedEffTransportConnectEventAdjustedThunk(void *pObject) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 4;
    ((VsNetEffTransportConnectCallback *)pObject)->QueueEffTransportConnectEvent();
}

// FUNCTION: LEMBALL 0x00471CA0
void QueueAllocatedEffTransportPayloadEventAdjustedThunk(
    void *pObject, unsigned short nType, void *pPayload) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 4;
    ((VsNetQueueEffTransportPayloadEventView *)pObject)
        ->QueueEffTransportPayloadEvent(nType, pPayload);
}

// FUNCTION: LEMBALL 0x00471CB0
int DispatchAllocatedTcpipHostPortAsyncMessageAdjustedThunk(
    void *pObject, int nMessage, void *pReserved, unsigned int nEvent) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetTcpipHostPortAsyncDispatcher *)pObject)
        ->DispatchTcpipHostPortAsyncMessage(nMessage, pReserved, nEvent);
}

// FUNCTION: LEMBALL 0x00472040
int DispatchTcpipClientAsyncMessageAdjustedThunk(
    void *pObject, int nMessage, void *pReserved, unsigned int nEvent) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetTcpipClientAsyncDispatcher *)pObject)
        ->DispatchTcpipClientAsyncMessage(nMessage, pReserved, nEvent);
}

// FUNCTION: LEMBALL 0x00471B90
void *DeleteCompositeTcpipSocketChannelStackTimedAdjustedThunk(
    void *pObject, BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x30;
    return ((VsNetTcpipSocketChannelStackWrapperView *)pObject)
        ->DeleteCompositeTcpipSocketChannelStackWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00471BA0
void WriteCompositeTcpipGlobalSessionAdjustedThunk(void *pObject) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    WriteCompositeTcpipGlobalSessionBody(pObject);
}

// FUNCTION: LEMBALL 0x00471BB0
static void WriteCompositeTcpipGlobalSessionBody(void *pObject) {
    char *pAdjusted;
    int *pOffsets;

    pAdjusted = (char *)pObject;
    pOffsets = *(int **)(pAdjusted - 0x38);
    pAdjusted += pOffsets[4];
    pOffsets = *(int **)(pAdjusted - 0x38);
    ((VsNetPeerPayloadSender *)(pAdjusted + pOffsets[2] - 0x38))
        ->WriteEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x00471D50
static void *DeleteTcpipSocketStackCompositeAdjustedThunk(void *pObject,
                                                            BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x30;
    return ((VsNetTcpipSocketChannelStackWrapperView *)pObject)
        ->DeleteTcpipSocketChannelStackWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00471D60
static void *ClaimTcpipSocketStackCompositeAdjustedThunk(void *pObject) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ClaimTcpipSocketStackCompositeBody(pObject);
}

// FUNCTION: LEMBALL 0x00471D70
static void *ClaimTcpipSocketStackCompositeBody(void *pObject) {
    char *pAdjusted;
    int *pOffsets;

    pAdjusted = (char *)pObject;
    pOffsets = *(int **)(pAdjusted - 0x38);
    pAdjusted += pOffsets[4];
    pOffsets = *(int **)(pAdjusted - 0x38);
    return ((VsNetAckedEffTransportRecordOwner *)(pAdjusted + pOffsets[2] - 0x38))
        ->ClaimAckedEffTransportRecordPayload();
}

// FUNCTION: LEMBALL 0x00471FD0
void *DeleteCompositeTcpipEffTransportTimedAdjustedThunk(void *pObject,
                                                            BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x30;
    return ((VsNetTcpipSocketChannelStackWrapperView *)pObject)
        ->DeleteTcpipEffTransportCompositeWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00471FE0
void WriteCompositeTcpipEffTransportGlobalSessionAdjustedThunk(void *pObject) {
    char *pAdjusted;
    int *pOffsets;
    char *pSender;

    pAdjusted = (char *)pObject - *(int *)((char *)pObject - 4) - 8;
    pOffsets = *(int **)(pAdjusted - 0x38);
    pAdjusted += pOffsets[4];
    pOffsets = *(int **)(pAdjusted - 0x38);
    pSender = pAdjusted + pOffsets[2] - 0x38;
    ((VsNetPeerPayloadSender *)pSender)->WriteEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x00471FF0
void *ClaimCompositeTcpipEffTransportRecordAdjustedThunk(void *pObject) {
    char *pAdjusted;
    int *pOffsets;

    pAdjusted = (char *)pObject - *(int *)((char *)pObject - 4) - 8;
    pOffsets = *(int **)(pAdjusted - 0x38);
    pAdjusted += pOffsets[4];
    pOffsets = *(int **)(pAdjusted - 0x38);
    return ((VsNetAckedEffTransportRecordOwner *)(pAdjusted + pOffsets[2] - 0x38))
        ->ClaimAckedEffTransportRecordPayload();
}

// FUNCTION: LEMBALL 0x00472000
void *DeleteCompositeTcpipEffTransportDualAdjustedThunk(void *pObject,
                                                           BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0xa8;
    return ((VsNetTcpipSocketChannelStackWrapperView *)pObject)
        ->DeleteTcpipEffTransportCompositeWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00472010
void WriteCompositeTcpipEffTransportDualGlobalSessionAdjustedThunk(void *pObject) {
    char *pAdjusted;
    int *pOffsets;
    char *pSender;

    pAdjusted = (char *)pObject - *(int *)((char *)pObject - 4) - 0x80;
    pOffsets = *(int **)(pAdjusted - 0x38);
    pAdjusted += pOffsets[4];
    pOffsets = *(int **)(pAdjusted - 0x38);
    pSender = pAdjusted + pOffsets[2] - 0x38;
    ((VsNetPeerPayloadSender *)pSender)->WriteEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x00472020
void *ClaimCompositeTcpipEffTransportDualRecordAdjustedThunk(void *pObject) {
    char *pAdjusted;
    int *pOffsets;

    pAdjusted = (char *)pObject - *(int *)((char *)pObject - 4) - 0x80;
    pOffsets = *(int **)(pAdjusted - 0x38);
    pAdjusted += pOffsets[4];
    pOffsets = *(int **)(pAdjusted - 0x38);
    return ((VsNetAckedEffTransportRecordOwner *)(pAdjusted + pOffsets[2] - 0x38))
        ->ClaimAckedEffTransportRecordPayload();
}

// FUNCTION: LEMBALL 0x00472030
void DispatchCompositeTcpipPrefixedControlAdjustedThunk(void *pObject,
                                                          int nDispatchType,
                                                          void *pEvent) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x0c;
    ((VsNetPrefixedEffTransportControlView *)pObject)
        ->DispatchPrefixedEffTransportControlStream(nDispatchType, pEvent);
}

// FUNCTION: LEMBALL 0x00471BD0
void *DispatchCompositeTcpipAckedPayloadAdjustedThunk(void *pObject) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 4;
    return ((VsNetDispatchAckedEffTransportPayloadView *)pObject)
        ->DispatchAckedEffTransportPayloadEvent();
}

// FUNCTION: LEMBALL 0x00471D90
void *DeleteTcpipSocketStackDualAdjustedThunk(void *pObject, BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0xa8;
    return ((VsNetTcpipSocketChannelStackWrapperView *)pObject)
        ->DeleteTcpipSocketChannelStackWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00471CC0
void *DeleteTcpipSocketStackPrimaryAdjustedThunk(void *pObject,
                                                  BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetTcpipSocketChannelStackWrapperView *)pObject)
        ->DeleteTcpipSocketChannelStackWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00471D20
void ClearTcpipSocketStackPendingWriteAdjustedThunk(void *pObject,
                                                     void *pUnused) {
    char *pAdjusted;
    int *pOffsets;

    pAdjusted = (char *)pObject - *(int *)((char *)pObject - 4);
    pOffsets = *(int **)(pAdjusted - 8);
    pAdjusted += pOffsets[4];
    pOffsets = *(int **)(pAdjusted - 8);
    ((VsNetEffTransportPendingWriteState *)(pAdjusted + pOffsets[2] + 0x70))->Clear(pUnused);
}

// FUNCTION: LEMBALL 0x00471DA0
void *ClaimTcpipSocketStackDualAdjustedThunk(void *pObject) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x78;
    return ClaimTcpipSocketStackCompositeBody(pObject);
}

// FUNCTION: LEMBALL 0x00471C80
void WriteAllocatedEffTransportGlobalSessionAdjustedThunk(void *pObject) {
    char *pAdjusted;
    int *pOffsets;
    char *pSender;

    pAdjusted = (char *)pObject - *(int *)((char *)pObject - 4) - 0x78;
    pOffsets = *(int **)(pAdjusted - 0x38);
    pAdjusted += pOffsets[4];
    pOffsets = *(int **)(pAdjusted - 0x38);
    pSender = pAdjusted + pOffsets[2] - 0x38;
    ((VsNetPeerPayloadSender *)pSender)->WriteEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x00471C90
void *DispatchAllocatedAckedEffTransportPayloadAdjustedThunk(void *pObject) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x7c;
    return ((VsNetDispatchAckedEffTransportPayloadView *)pObject)
        ->DispatchAckedEffTransportPayloadEvent();
}

// FUNCTION: LEMBALL 0x00462DA0
void *DeleteCompositeEffTransportStackAdjustedThunk(void *pObject, BYTE fDelete) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return DeleteCompositeEffTransportStackWrapper(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00462DF0
void CompleteEffTransportPendingWriteAdjustedThunk(void *pObject, int fQueueEvent) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    ((VsNetCompleteEffTransportPendingWriteView *)pObject)
        ->CompleteEffTransportPendingWrite(fQueueEvent);
}

// FUNCTION: LEMBALL 0x00462BC0
void *VsNetCompositeEffTransportStackWrapperView::DeleteCompositeEffTransportRuntimeWrapper(
    BYTE fFreeMemory) {
    VsNetCompositeEffTransportStackWrapperView *pView;
    char *pbAllocationBase;
    void *pObject;

    pObject = (void *)this;
    pView = this;
    pbAllocationBase = (char *)this - 0x24;
    ((VsNetRuntimeChannelStackReleaseFront *)pObject)->Release();
    ((VsNetDualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((VsNetTimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00462C10
void *DeleteCompositeEffTransportRuntimeAdjustedThunk(void *pObject, BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x30;
    return ((VsNetCompositeEffTransportStackWrapperView *)pObject)
        ->DeleteCompositeEffTransportRuntimeWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00462C20
void WriteEffTransportGlobalSessionAdjustedThunk(void *pObject) {
    char *pAdjustedObject;
    int *pOffsets;

    pAdjustedObject = (char *)pObject - *(int *)((char *)pObject - 4) + 0x108;
    pOffsets = *(int **)(pAdjustedObject - 0x38);
    ((VsNetPeerPayloadSender *)(pAdjustedObject + pOffsets[2] - 0x38))
        ->WriteEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x00462E00
void *DeleteCompositeEffTransportStackTimedAdjustedThunk(void *pObject, BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x30;
    return DeleteCompositeEffTransportStackWrapper(pObject, fFreeMemory);
}

// FUNCTION: LEMBALL 0x0045F7F0
void ReverseEffTransportPayload(void *pObject) {
    VsNetEffStreamBase *pStream;
    unsigned char *pb;
    unsigned char b;

    pStream = (VsNetEffStreamBase *)pObject;
    pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
    b = pb[0]; pb[0] = pb[3]; pb[3] = b;
    b = pb[1]; pb[1] = pb[2]; pb[2] = b;
    pStream->m_nReserved20 += 4;
    pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
    b = pb[0]; pb[0] = pb[3]; pb[3] = b;
    b = pb[1]; pb[1] = pb[2]; pb[2] = b;
    pStream->m_nReserved20 += 4;
    pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
    b = pb[0]; pb[0] = pb[1]; pb[1] = b;
    pStream->m_nReserved20 += 2;
    pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
    b = pb[0]; pb[0] = pb[1]; pb[1] = b;
    pStream->m_nReserved20 += 2;
    pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
    b = pb[0]; pb[0] = pb[1]; pb[1] = b;
    pStream->m_nReserved20 += 2;
    ++pStream->m_nReserved20;
}

// FUNCTION: LEMBALL 0x0045F790
void WriteEffTransportTaggedHeader(void *pObject) {
    VsNetEffStreamBase *pStream;
    unsigned char *pbTag;

    pStream = (VsNetEffStreamBase *)pObject;
    pbTag = (unsigned char *)(unsigned long)pStream->m_pTagBuffer2c;
    pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
    pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
    pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
    pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
    pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
    *(unsigned char *)(unsigned long)pStream->m_nReserved1c = pbTag[0xe];
    ++pStream->m_nReserved1c;
}

// FUNCTION: LEMBALL 0x00471E60
static void TimedSocketBundleCloseSocketThunk(void *pObject) {
    ((VsNetSocketWindowChannelWrapper *)((char *)pObject + 0xc8))
        ->CloseSocketHandleFromChannel();
}

// FUNCTION: LEMBALL 0x00471E70
static void *TimedSocketBundleDeleteThunk(void *pObject, BYTE fDelete) {
    char *pAdjustedObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetTimedSocketWindowChannelStackWrapperView *)pAdjustedObject)
        ->DeleteTimedSocketWindowChannelStackWrapper(fDelete);
}

// FUNCTION: LEMBALL 0x00471EC0
static void TimedSocketBundleCaptureErrorThunk(void *pObject) {
    ((VsNetSocketWindowChannelWrapper *)((char *)pObject + 0xc8))
        ->CaptureLastWinsockErrorStatus();
}

// FUNCTION: LEMBALL 0x00471EE0
static void TimedSocketBundleClearPendingWriteBody(void *pObject, void *pUnused) {
    int nPendingDelta = *(int *)(*(int *)((char *)pObject - 0x18) + 8);
    ((VsNetEffTransportPendingWriteState *)((char *)pObject + nPendingDelta + 0x60))
        ->Clear(pUnused);
}

// FUNCTION: LEMBALL 0x00471ED0
static void TimedSocketBundleClearPendingWriteThunk(void *pObject, void *pUnused) {
    char *pAdjustedObject = (char *)pObject - *(int *)((char *)pObject - 4);
    TimedSocketBundleClearPendingWriteBody(pAdjustedObject, pUnused);
}

// FUNCTION: LEMBALL 0x00471F00
static void *TimedSocketBundleDeleteAdjustedThunk(void *pObject, BYTE fDelete) {
    char *pAdjustedObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x30;
    return ((VsNetTimedSocketWindowChannelStackWrapperView *)pAdjustedObject)
        ->DeleteTimedSocketWindowChannelStackWrapper(fDelete);
}

// FUNCTION: LEMBALL 0x00471F10
static void TimedSocketBundleCaptureLookupAdjustedThunk(void *pObject, void *pLookupResult) {
    VsNetTimedSocketAsyncLookupAdapter *pAdapter =
        (VsNetTimedSocketAsyncLookupAdapter *)((char *)pObject - *(int *)((char *)pObject - 4));
    pAdapter->CaptureAsyncLookupAddressForTimedStream(pLookupResult);
}

// FUNCTION: LEMBALL 0x00471F20
static int TimedSocketBundleSendUdpAdjustedThunk(void *pObject, void *pBuffer, int cbBuffer) {
    VsNetTimedSocketAsyncLookupAdapter *pAdapter =
        (VsNetTimedSocketAsyncLookupAdapter *)((char *)pObject - *(int *)((char *)pObject - 4));
    return pAdapter->SendUdpPayloadToSocketAddress(pBuffer, cbBuffer);
}

// FUNCTION: LEMBALL 0x00471F40
static int TimedSocketBundleConnectAdjustedThunk(void *pObject, int nMessage,
                                                  void *pReserved, unsigned int nEvent) {
    VsNetTimedSocketAsyncMessageAdapter *pAdapter =
        (VsNetTimedSocketAsyncMessageAdapter *)((char *)pObject - *(int *)((char *)pObject - 4));
    return pAdapter->HandleTimedSocketConnectAsyncMessage(nMessage, pReserved, nEvent);
}

// FUNCTION: LEMBALL 0x00471F50
static void *TimedSocketBundleCompositeDeleteAdjustedThunk(void *pObject, BYTE fDelete) {
    char *pAdjustedObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetTcpipSocketChannelStackWrapperView *)pAdjustedObject)
        ->DeleteTcpipEffTransportCompositeWrapper(fDelete);
}

// FUNCTION: LEMBALL 0x00471DB0
static void TimedSocketBundleDispatchAsyncThunk(void *pObject, int nMessage,
                                                 void *pReserved, unsigned int nEvent) {
    VsNetTimedSocketAsyncMessageAdapter *pAdapter;

    pAdapter = (VsNetTimedSocketAsyncMessageAdapter *)
        ((char *)pObject - *(int *)((char *)pObject - 4));
    pAdapter->DispatchEffSocketAsyncSelectMessage(nMessage, pReserved, nEvent);
}

// FUNCTION: LEMBALL 0x00471DC0
static void TimedSocketBundlePrimaryCloseThunk(void *pObject) {
    ((VsNetSocketWindowChannelWrapper *)((char *)pObject + 0xa4))
        ->CloseSocketHandleFromChannel();
}

// FUNCTION: LEMBALL 0x00471DD0
static void *TimedSocketBundlePrimaryDeleteThunk(void *pObject, BYTE fDelete) {
    char *pAdjustedObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetDualSocketWindowChannelStackWrapperView *)pAdjustedObject)
        ->DeleteDualSocketWindowChannelStackWrapper(fDelete);
}

// FUNCTION: LEMBALL 0x00471E40
static void TimedSocketBundlePrimaryNoopBody(BYTE fDelete) {
    (void)fDelete;
}

// FUNCTION: LEMBALL 0x00471E30
static void TimedSocketBundlePrimaryNoopThunk(void *pObject, BYTE fDelete) {
    (void)pObject;
    TimedSocketBundlePrimaryNoopBody(fDelete);
}

// FUNCTION: LEMBALL 0x00471E20
static void TimedSocketBundlePrimaryCaptureErrorThunk(void *pObject) {
    ((VsNetSocketWindowChannelWrapper *)((char *)pObject + 0xa4))
        ->CaptureLastWinsockErrorStatus();
}

static void *g_NETWORK_TimedSocketBundleChannelThunkVtable[4] = {
    (void *)TimedSocketBundleCloseSocketThunk,
    (void *)TimedSocketBundleDeleteThunk,
    (void *)TimedSocketBundleCaptureErrorThunk,
    (void *)TimedSocketBundleClearPendingWriteThunk,
};
static void SetTcpipSocketChannelAssignedPort(void *pObject, short nPort);
// FUNCTION: LEMBALL 0x00471F30
static void TimedSocketBundleSetAssignedPortThunk(void *pObject, short nPort) {
    SetTcpipSocketChannelAssignedPort((char *)pObject - *(int *)((char *)pObject - 4), nPort);
}

/* Raw table 0049a090: stream base slots, timed payload slots, then socket
 * adapter slots.  Compiler-owned storage keeps every dispatch target valid. */
struct VsNetTimedSocketBundleTimedThunkVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        unsigned char b;
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
        unsigned char *pbTag = (unsigned char *)(unsigned long)pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
        pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
        *(unsigned char *)(pStream->m_nReserved1c) = pbTag[0xe];
        ++pStream->m_nReserved1c;
    }
    virtual void *Delete(BYTE fDelete) {
        return TimedSocketBundleDeleteAdjustedThunk(this, fDelete);
    }
    virtual void WriteGlobalSession(void) {
        ((VsNetPeerPayloadSender *)this)->WriteEffStreamWithGlobalSession();
    }
    virtual void *ClaimRecord(void) {
        return ((VsNetAckedEffTransportRecordOwner *)this)
            ->ClaimAckedEffTransportRecordPayload();
    }
    virtual void CaptureLookup(void *pLookupResult) {
        TimedSocketBundleCaptureLookupAdjustedThunk(this, pLookupResult);
    }
    virtual int SendUdp(void *pBuffer, int cbBuffer) {
        return TimedSocketBundleSendUdpAdjustedThunk(this, pBuffer, cbBuffer);
    }
    virtual void SetAssignedPort(short nPort) {
        TimedSocketBundleSetAssignedPortThunk(this, nPort);
    }
};

static VsNetTimedSocketBundleTimedThunkVtableModel g_NETWORK_TimedSocketBundleTimedThunkVtableModel;
static void *g_NETWORK_TimedSocketBundleTimedThunkVtable =
    *(void ***)&g_NETWORK_TimedSocketBundleTimedThunkVtableModel;
static void TimedSocketBundleSocketThunkNoop(void *pObject) {
    (void)pObject;
}
static void *g_NETWORK_TimedSocketBundleSocketThunkVtable[2] = {
    (void *)TimedSocketBundleConnectAdjustedThunk,
    (void *)TimedSocketBundleSocketThunkNoop,
};

/* Composite construction groups recovered from Ghidra 0049a138..0049a0d0. */
static int g_NETWORK_TcpipCompositeTimedStreamVtable[8] = {
    -4, 0x2c, 0x5c, 0xd4, 0x12c, 0x134, 0x150, 0x154,
};
static int g_NETWORK_TcpipCompositeTransportStackVtable[8] = {
    0, 0x10, 0x40, 0xb8, 0x110, 0x118, 0x134, 0x138,
};
static int g_NETWORK_TcpipCompositeDualStreamVtable[4] = {
    -0x44, -0x74, 0, 0x10,
};
static int g_NETWORK_TcpipCompositeDualThunkVtable[4] = {
    -0x44, -0xec, -0x44, -0x74,
};
static int g_NETWORK_TcpipCompositeSocketWindowVtable[4] = {
    0, -0x100, -0xd0, -0x58,
};
static int g_NETWORK_TcpipCompositeTimedSocketThunkVtable[6] = {
    -0x10, -0x118, 0, -0x100, -0xd0, -0x58,
};
static int g_NETWORK_TcpipCompositeTimedSocketBundleVtable[4] = {
    0, -0x124, -0x7c, -0x1c,
};
static int g_NETWORK_TcpipCompositeTimedSocketBundleDataVtable[4] = {
    0, -0x128, -0xf8, -0x20,
};
static void *g_NETWORK_TcpipCompositeFallbackThunkVtable[1] = {
    (void *)NetworkSafeVtableNoop,
};
/* 0049a158 is a two-base table.  Its suffix at +8 is the dual transport
 * table installed at 0049a160. */
static void *g_NETWORK_TcpipCompositeSocketThunkVtable[12] = {
    (void *)DispatchTcpipClientAsyncMessageAdjustedThunk,
    0,
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReverseEffTransportPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteCompositeTcpipEffTransportDualAdjustedThunk,
    (void *)RuntimeDualUnusedCallback,
    (void *)WriteCompositeTcpipEffTransportDualGlobalSessionAdjustedThunk,
    (void *)ClaimCompositeTcpipEffTransportDualRecordAdjustedThunk,
    (void *)DispatchCompositeTcpipPrefixedControlAdjustedThunk,
};
extern "C" int WINAPI socket(int nAddressFamily, int nType, int nProtocol);
extern "C" int WINAPI ioctlsocket(int nSocket, long nCommand, unsigned long *pValue);
extern "C" int WINAPI bind(int nSocket, const void *pAddress, int cbAddress);
extern "C" int WINAPI WSAAsyncSelect(int nSocket, void *hWnd, unsigned int nMessage,
                                      long nEvents);
extern "C" unsigned short WINAPI htons(unsigned short nValue);
extern "C" DWORD WINAPI timeGetTime(void);
extern void *g_pEffTransportRuntimeService;
extern short g_nEffTransportServiceBasePort;
struct VsNetTcpipRuntimeStateVtable;
extern VsNetTcpipRuntimeStateVtable g_NETWORK_TcpipRuntimeStateVtable;
extern void *g_NETWORK_TcpipRuntimeWindowVtable;
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);

struct VsNetEffTransportRuntimeState {
    void DestroyEffTransportRuntimeGlobals(void);
};
void *AllocateTcpipEffTransportComposite(void);
void *AllocateTcpipEffTransportRuntimeOwner(void);

/* These are the primary composite slots at 0049a3d8.  Keep them as real
   member functions: the binary calls them with the composite in ECX. */
struct VsNetTcpipCompositeVtableModel {
    // FUNCTION: LEMBALL 0x00470DD0
    virtual int BeginRemoteServiceHostLookup(void *pUnused,
                                              const char *pszHost)
    {
        VsNetAllocatedTcpipEffTransportCompositeLayout *pComposite;
        VsNetConstructionAdjustorVtable *pOffsets;
        char *pTimedSocket;
        VsNetSocketChannelControl *pErrorChannel;
        unsigned int cbHost;
        int nLookupHandle;

        (void)pUnused;
        pComposite = (VsNetAllocatedTcpipEffTransportCompositeLayout *)this;
        cbHost = (unsigned int)strlen(pszHost) + 1;
        *(char **)((char *)pComposite + 0x0c) =
            (char *)AllocateVSMemBlock(cbHost);
        memcpy(*(char **)((char *)pComposite + 0x0c), pszHost, cbHost);

        pOffsets = pComposite->m_pOuterOffsets04;
        pTimedSocket = (char *)pComposite + pOffsets->m_nOffset14;
        *(int *)(pTimedSocket + 0x18) =
            (int)(unsigned long)AllocateVSMemBlock(0x400);
        nLookupHandle = WSAAsyncGetHostByName(
            *(int *)(pTimedSocket + 8), 0x440, pszHost,
            (void *)(unsigned long)*(int *)(pTimedSocket + 0x18), 0x400);
        *(int *)(pTimedSocket + 0x1c) = nLookupHandle;
        if (nLookupHandle == 0) {
            pErrorChannel = (VsNetSocketChannelControl *)
                ((char *)pComposite + pOffsets->m_nPrimaryOffset + 4);
            pErrorChannel->HandleSocketError();
        }
        return nLookupHandle != 0;
    }
    virtual void ServiceEffTransportPeerEntry(void)
    {
        ((VsNetCompositeEffTransportVtableModel *)this)
            ->ServiceEffTransportPeerEntry();
    }
    // FUNCTION: LEMBALL 0x00471090
    virtual void CreateTcpipCompositeSocket(void)
    {
        VsNetTcpipEffTransportCompositeLayout *pComposite;
        VsNetConstructionAdjustorVtable *pOffsets;
        VsNetAdjustorSubobject *pSocketChannel;
        int nSocket;
        unsigned long nNonblocking;

        pComposite = (VsNetTcpipEffTransportCompositeLayout *)this;
        nSocket = socket(2, 2, 0);
        pOffsets = pComposite->m_pOuterOffsets04;
        pSocketChannel = (VsNetAdjustorSubobject *)((char *)pComposite +
            pOffsets->m_nSecondaryOffset + 4);
        *(int *)((char *)pSocketChannel + 0x0c) = nSocket;
        if (nSocket == -1) {
            ((VsNetSocketChannelControl *)pSocketChannel)
                ->HandleSocketError();
            return;
        }
        *(int *)((char *)pSocketChannel + 0x10) = 1;
        nNonblocking = 1;
        if (ioctlsocket(*(int *)((char *)pSocketChannel + 0x0c),
                        0x8004667e, &nNonblocking) == -1) {
            ((VsNetSocketChannelControl *)pSocketChannel)
                ->HandleSocketError();
        }
    }
    // FUNCTION: LEMBALL 0x00471110
    virtual void BindTcpipCompositeAddress(void *pAddress)
    {
        VsNetTcpipEffTransportCompositeLayout *pComposite;
        VsNetConstructionAdjustorVtable *pOffsets;
        VsNetSocketChannelControl *pSocketChannel;
        unsigned char abAddress[16];
        int nSocket;

        pComposite = (VsNetTcpipEffTransportCompositeLayout *)this;
        ((VsNetTcpipCompositeVtableModel *)this)
            ->CreateTcpipCompositeSocket();
        pOffsets = pComposite->m_pOuterOffsets04;
        pSocketChannel = (VsNetSocketChannelControl *)((char *)this +
            pOffsets->m_nSecondaryOffset + 4);
        pSocketChannel->SetRemoteAddress(pAddress);
        *(unsigned short *)abAddress = 2;
        *(unsigned short *)(abAddress + 2) = htons(
            (unsigned short)(*(short *)((char *)this +
                pOffsets->m_nSecondaryOffset + 0x24) +
                g_nEffTransportServiceBasePort));
        *(int *)(abAddress + 4) = *(int *)((char *)g_pEffTransportRuntimeService + 0x14);
        nSocket = *(int *)((char *)this + pOffsets->m_nSecondaryOffset + 0x0c);
        if (bind(nSocket, abAddress, 0x10) == -1) {
            pSocketChannel->HandleSocketError();
            return;
        }
        if (WSAAsyncSelect(nSocket,
                (void *)(unsigned long)*(int *)((char *)g_pEffTransportRuntimeService + 0x14),
                0x443, 3) == -1) {
            pSocketChannel->HandleSocketError();
            return;
        }
        *(int *)((char *)this + pOffsets->m_nSecondaryOffset + 0x1c) = 1;
        *(int *)((char *)this + pOffsets->m_nSecondaryOffset + 0x20) = 1;
        *(int *)((char *)this + 0x1c) = 0;
        *(int *)((char *)this + pOffsets->m_nSecondaryOffset + 0x40) =
            (int)timeGetTime() - 1000;
        *(int *)((char *)this + pOffsets->m_nSecondaryOffset + 0x44) =
            (int)timeGetTime();
    }
    virtual void BindUdpCompositeSocket(void) {
        ((VsNetUdpSocketBinder *)this)->BindUdpSocketAndEnableAsyncSelect();
    }
    virtual void Reserved14(void) {
    }
    virtual void Reserved18(void) {
    }
};
static VsNetTcpipCompositeVtableModel g_NETWORK_TcpipCompositeVtableModel;
struct VsNetTcpipCompositeReceiveVtableModel {
    virtual int ReceiveAsyncMessage(int nMessage, void *pReserved, unsigned int nEvent) {
        return ((VsNetSocketAsyncMessageAdapter *)this)
            ->HandleSocketReceiveAsyncSelectMessage(nMessage, pReserved, nEvent);
    }
};
static VsNetTcpipCompositeReceiveVtableModel
    g_NETWORK_TcpipCompositeReceiveVtableModel;
static void *g_NETWORK_TcpipCompositeReceiveVtable =
    *(void ***)&g_NETWORK_TcpipCompositeReceiveVtableModel;
static VsNetTcpipCompositeControlVtableModel
    g_NETWORK_TcpipCompositeControlVtableModel;
static void *g_NETWORK_TcpipCompositeControlVtable =
    *(void ***)&g_NETWORK_TcpipCompositeControlVtableModel;
static void **g_NETWORK_TcpipCompositeDualRecoveredVtable =
    g_NETWORK_TcpipCompositeSocketThunkVtable + 2;
static void *g_NETWORK_TcpipCompositeOuterFatalThunkVtable[4] = {
    (void *)CaptureLastWinsockErrorStatusOuterDoubleAdjustedThunk,
    (void *)TimedSocketBundleCompositeDeleteAdjustedThunk,
    (void *)CaptureLastWinsockErrorStatusOuterAdjustedThunk,
    (void *)CloseAdjustedEffTransportPeerByKeyAdjustedThunk,
};
static void *g_NETWORK_TimedSocketBundlePrimaryThunkVtable[5] = {
    (void *)TimedSocketBundlePrimaryCloseThunk,
    (void *)TimedSocketBundlePrimaryDeleteThunk,
    (void *)TimedSocketBundlePrimaryCaptureErrorThunk,
    (void *)TimedSocketBundlePrimaryNoopThunk,
    (void *)TimedSocketBundleDispatchAsyncThunk,
};
struct VsNetCompositeTimedSocketSecondaryThunkVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        unsigned char b;
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
        unsigned char *pbTag = (unsigned char *)(unsigned long)pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
        pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
        *(unsigned char *)pStream->m_nReserved1c = pbTag[0xe];
        ++pStream->m_nReserved1c;
    }
    virtual void *Delete(BYTE fDelete) {
        return ((VsNetDualSocketWindowChannelStackWrapperView *)((char *)this - 0x2c))
            ->DeleteDualSocketWindowChannelStackWrapper(fDelete);
    }
    virtual void Ret(void) {}
    virtual void Fatal0(void) { _purecall(); }
    virtual void Fatal1(void) { _purecall(); }
    virtual void Ret8(BYTE fDelete, BYTE fReserved) {
        (void)fDelete;
        (void)fReserved;
    }
    virtual void CloseSocket(void) {
        ((VsNetSocketWindowChannelWrapper *)((char *)this + 0xa4))
            ->CloseSocketHandleFromChannel();
    }
};
static VsNetCompositeTimedSocketSecondaryThunkVtableModel g_NETWORK_CompositeTimedSocketSecondaryThunkVtableModel;

/* Raw table 0049a1e4: receive adapter, shared stream prefix, dual-channel
 * methods, then socket-window methods. */
struct VsNetTimedSocketBundleTertiaryThunkVtableModel {
    virtual int HandleReceive(int nMessage, void *pReserved, unsigned int nEvent) {
        return ((VsNetSocketAsyncMessageAdapter *)this)
            ->HandleSocketReceiveAsyncSelectMessage(nMessage, pReserved, nEvent);
    }
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    void ReversePayloadBody(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        unsigned char b;
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
    virtual void ReversePayload(void) { ReversePayloadBody(); }
    virtual void Noop(void) {}
    virtual void WriteTaggedHeader(void) { WriteTaggedHeaderBody(); }
    void WriteTaggedHeaderBody(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pbTag = (unsigned char *)(unsigned long)pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
        pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
        *(unsigned char *)pStream->m_nReserved1c = pbTag[0xe];
        ++pStream->m_nReserved1c;
    }
    virtual void *DeleteDual(BYTE fDelete) {
        return ((VsNetDualSocketWindowChannelStackWrapperView *)((char *)this - 0x2c))
            ->DeleteDualSocketWindowChannelStackWrapper(fDelete);
    }
    virtual void Ret(void) {}
    virtual void Fatal0(void) { _purecall(); }
    virtual void Fatal1(void) { _purecall(); }
    virtual void Ret8(BYTE fDelete, BYTE fReserved) {
        (void)fDelete;
        (void)fReserved;
    }
    virtual void CloseSocket(void) {
        ((VsNetSocketWindowChannelWrapper *)((char *)this + 0xa4))
            ->CloseSocketHandleFromChannel();
    }
    virtual void *DeletePrimary(BYTE fDelete) {
        return TimedSocketBundlePrimaryDeleteThunk(this, fDelete);
    }
    virtual void CaptureError(void) {
        TimedSocketBundlePrimaryCaptureErrorThunk(this);
    }
    virtual void NoopRet4(BYTE fDelete) {
        TimedSocketBundlePrimaryNoopBody(fDelete);
    }
    virtual void DispatchAsync(int nMessage, void *pReserved, unsigned int nEvent) {
        TimedSocketBundleDispatchAsyncThunk(this, nMessage, pReserved, nEvent);
    }
};
static VsNetTimedSocketBundleTertiaryThunkVtableModel
    g_NETWORK_TimedSocketBundleTertiaryThunkVtableModel;
static void *g_NETWORK_TimedSocketBundleTertiaryThunkVtable =
    *(void ***)&g_NETWORK_TimedSocketBundleTertiaryThunkVtableModel;
/* 0049a1e8 is suffix of 0049a1e4: skip receive-dispatch slot. */
static void *g_NETWORK_TimedSocketBundleSecondaryThunkVtable =
    (void **)(g_NETWORK_TimedSocketBundleTertiaryThunkVtable) + 1;
/* Temporary compiler-owned thunk storage.  The original target is the
 * runtime-stack fatal adjustor; never leave this dispatch pointer null. */
struct VsNetSocketRuntimeFatalThunkVtableModel {
    virtual void Fatal(void) { _purecall(); }
    virtual void *Delete(BYTE fDelete) {
        (void)fDelete;
        return this;
    }
    virtual void FatalSecondary(void) { _purecall(); }
    virtual void Noop(BYTE fDelete) { (void)fDelete; }
};

static VsNetSocketRuntimeFatalThunkVtableModel g_NETWORK_SocketRuntimeFatalThunkVtableModel;
static void *g_NETWORK_RuntimeChannelStackFatalThunk =
    *(void ***)&g_NETWORK_SocketRuntimeFatalThunkVtableModel;

/* Socket-stack construction groups recovered from 0049a438..0049a3f0. */
static int g_NETWORK_TcpipSocketStackTimedStreamVtable[4] = {
    -0x44, -0x74, 0, 8,
};
static int g_NETWORK_TcpipSocketStackDualStreamVtable[4] = {
    -0x44, -0xec, -0x44, -0x74,
};
static int g_NETWORK_TcpipSocketStackBaseVtable[6] = {
    0, 8, 0x38, 0xb0, 0x108, 0x110,
};
static int g_NETWORK_TcpipSocketStackDualThunkVtable[4] = {
    0, -0x100, -0xd0, -0x58,
};
static int g_NETWORK_TcpipSocketStackTimedSocketBundleVtable[4] = {
    -0x10, -0x118, 0, -0x100,
};
static int g_NETWORK_TcpipSocketStackSocketWindowVtable[4] = {
    0, -0x124, -0x7c, -0x1c,
};
static int g_NETWORK_TcpipSocketStackTimedSocketDataVtable[4] = {
    0, -0x128, -0xf8, -0x20,
};

struct VsNetTcpipSocketStackCommonThunkVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        unsigned char b;
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
        unsigned char *pbTag = (unsigned char *)(unsigned long)pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
        pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
        *(unsigned char *)pStream->m_nReserved1c = pbTag[0xe];
        ++pStream->m_nReserved1c;
    }
};

struct VsNetTcpipSocketStackRuntimeThunkVtableModel
    : VsNetTcpipSocketStackCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 0xa8;
        return ((VsNetTcpipSocketChannelStackWrapperView *)pAdjusted)
            ->DeleteCompositeTcpipSocketChannelStackWrapper(fDelete);
    }
    virtual void QueueConnect(void) {
        VsNetEffTransportConnectCallback *pCallback =
            (VsNetEffTransportConnectCallback *)
                ((char *)this - *(int *)((char *)this - 4) - 4);
        pCallback->QueueEffTransportConnectEvent();
    }
    virtual void WriteGlobalSession(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4);
        int nOffsets = *(int *)(pAdjusted - 0x38);
        char *pCallback = pAdjusted + *(int *)(nOffsets + 0x10);
        nOffsets = *(int *)(pCallback - 0x38);
        ((VsNetPeerPayloadSender *)(pCallback + *(int *)(nOffsets + 8) - 0x38))
            ->WriteEffStreamWithGlobalSession();
    }
    virtual void *DispatchAcked(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 0x7c;
        return ((VsNetDispatchAckedEffTransportPayloadView *)(pAdjusted - 4))
            ->DispatchAckedEffTransportPayloadEvent();
    }
};

struct VsNetTcpipSocketStackTimedThunkVtableModel
    : VsNetTcpipSocketStackCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 0x30;
        return ((VsNetTcpipSocketChannelStackWrapperView *)pAdjusted)
            ->DeleteTcpipSocketChannelStackWrapper(fDelete);
    }
    virtual void WriteGlobalSession(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4);
        int nOffsets = *(int *)(pAdjusted - 0x38);
        char *pCallback = pAdjusted + *(int *)(nOffsets + 0x10);
        nOffsets = *(int *)(pCallback - 0x38);
        ((VsNetPeerPayloadSender *)(pCallback + *(int *)(nOffsets + 8) - 0x38))
            ->WriteEffStreamWithGlobalSession();
    }
    virtual void *DispatchAcked(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4);
        return ((VsNetDispatchAckedEffTransportPayloadView *)(pAdjusted - 4))
            ->DispatchAckedEffTransportPayloadEvent();
    }
    virtual void CaptureLookup(void *pLookupResult) {
        ((VsNetTimedSocketAsyncLookupAdapter *)((char *)this + 0x140))
            ->CaptureAsyncLookupAddressForTimedStream(pLookupResult);
    }
    virtual int SendUdp(void *pBuffer, int cbBuffer) {
        return ((VsNetTimedSocketAsyncLookupAdapter *)((char *)this + 0x140))
            ->SendUdpPayloadToSocketAddress(pBuffer, cbBuffer);
    }
    virtual void SetAssignedPort(short nPort) {
        SetTcpipSocketChannelAssignedPort((char *)this + 0x140, nPort);
    }
};

/* Raw table 00499140.  This is the dual adjusted view. */
struct VsNetTcpipSocketStackDualThunkVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        unsigned char b;
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
        unsigned char *pbTag = (unsigned char *)(unsigned long)pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
        pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
        *(unsigned char *)pStream->m_nReserved1c = pbTag[0xe];
        ++pStream->m_nReserved1c;
    }
    virtual void *Delete(BYTE fDelete) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 0xa8;
        return DeleteCompositeEffTransportStackWrapper(pAdjusted, fDelete);
    }
    virtual void QueueConnect(void) {
        VsNetEffTransportConnectCallback *pCallback =
            (VsNetEffTransportConnectCallback *)
                ((char *)this - *(int *)((char *)this - 4));
        pCallback->QueueEffTransportConnectEvent();
    }
    virtual void WriteGlobalSession(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) + 0x90;
        int *pOffsets = *(int **)(pAdjusted - 0x38);
        ((VsNetPeerPayloadSender *)(pAdjusted + pOffsets[2] - 0x38))
            ->WriteEffStreamWithGlobalSession();
    }
    virtual void *DispatchAcked(void) {
        void *pObject = (char *)this - *(int *)((char *)this - 4) - 0x78;
        return ((VsNetDispatchAckedEffTransportPayloadView *)pObject)
            ->DispatchAckedEffTransportPayloadEvent();
    }
    virtual void QueuePayload(unsigned short nType, void *pPayload) {
        void *pObject = (char *)this - *(int *)((char *)this - 4);
        ((VsNetQueueEffTransportPayloadEventView *)pObject)
            ->QueueEffTransportPayloadEvent(nType, pPayload);
    }
};

/* Raw table 0049a188.  The three middle entries are the adjusted
 * write/claim views; they are not the connect and dispatch entries used by
 * the other runtime table. */
struct VsNetTcpipSocketStackCompositeRuntimeThunkVtableModel
    : VsNetTcpipSocketStackCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 0x30;
        return ((VsNetTcpipSocketChannelStackWrapperView *)pAdjusted)
            ->DeleteCompositeTcpipSocketChannelStackWrapper(fDelete);
    }
    virtual void WriteGlobalSession(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 8;
        ((VsNetPeerPayloadSender *)pAdjusted)->WriteEffStreamWithGlobalSession();
    }
    virtual void *ClaimRecord(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 8;
        return ((VsNetAckedEffTransportRecordOwner *)pAdjusted)
            ->ClaimAckedEffTransportRecordPayload();
    }
    virtual void CaptureLookup(void *pLookupResult) {
        ((VsNetTimedSocketAsyncLookupAdapter *)((char *)this + 0x140))
            ->CaptureAsyncLookupAddressForTimedStream(pLookupResult);
    }
    virtual int SendUdp(void *pBuffer, int cbBuffer) {
        return ((VsNetTimedSocketAsyncLookupAdapter *)((char *)this + 0x140))
            ->SendUdpPayloadToSocketAddress(pBuffer, cbBuffer);
    }
    virtual void SetAssignedPort(short nPort) {
        SetTcpipSocketChannelAssignedPort((char *)this + 0x140, nPort);
    }
};

static VsNetTcpipSocketStackRuntimeThunkVtableModel
    g_NETWORK_TcpipSocketStackRuntimeThunkVtableModel;
static void *g_NETWORK_TcpipSocketStackRuntimeThunkVtable =
    *(void ***)&g_NETWORK_TcpipSocketStackRuntimeThunkVtableModel;
static VsNetTcpipSocketStackTimedThunkVtableModel
    g_NETWORK_TcpipSocketStackTimedThunkVtableModel;
static void *g_NETWORK_TcpipSocketStackTimedThunkVtable =
    *(void ***)&g_NETWORK_TcpipSocketStackTimedThunkVtableModel;
static VsNetTcpipSocketStackDualThunkVtableModel
    g_NETWORK_TcpipSocketStackDualThunkVtableModel;
struct VsNetTcpipSocketStackDualRet8VtableModel {
    // FUNCTION: LEMBALL 0x00462930
    virtual void Ret8(BYTE fDelete, BYTE fReserved) {
        (void)fDelete;
        (void)fReserved;
    }
};
static VsNetTcpipSocketStackDualRet8VtableModel
    g_NETWORK_TcpipSocketStackDualRet8VtableModel;
/* Keep this cross-TU table reference constant-initialized.  The original
 * binary's startup initializer consumes the pointer before the other socket
 * translation-unit initializers run; dereferencing the C++ model here leaves
 * a zero BSS value in the reconstructed image. */
static void *g_NETWORK_TcpipSocketStackDualRet8FallbackVtable[1] = {
    (void *)NetworkSafeVtableNoop,
};
void *g_NETWORK_TcpipSocketStackDualRet8 =
    g_NETWORK_TcpipSocketStackDualRet8FallbackVtable;
static void *g_NETWORK_TcpipSocketStackDualDispatchVtable[10] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReverseEffTransportPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteTcpipSocketStackDualAdjustedThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteAllocatedEffTransportGlobalSessionAdjustedThunk,
    (void *)ClaimTcpipSocketStackDualAdjustedThunk,
    *(void **)g_NETWORK_TcpipSocketStackDualRet8,
};
static void *g_NETWORK_TcpipSocketStackDualThunk =
    g_NETWORK_TcpipSocketStackDualDispatchVtable;
static VsNetTcpipSocketStackCompositeRuntimeThunkVtableModel
    g_NETWORK_TcpipSocketStackCompositeRuntimeThunkVtableModel;
static void *g_NETWORK_TcpipSocketStackCompositeRuntimeThunkVtable =
    *(void ***)&g_NETWORK_TcpipSocketStackCompositeRuntimeThunkVtableModel;
static void *g_NETWORK_TcpipSocketStackSocketThunkVtable[2] = {
    (void *)TimedSocketBundleDispatchAsyncThunk,
    0,
};
static void *g_NETWORK_TcpipCompositeTimedThunkRecoveredVtable[12] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReverseEffTransportPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteTcpipSocketStackCompositeAdjustedThunk,
    (void *)WriteCompositeTcpipGlobalSessionAdjustedThunk,
    (void *)ClaimTcpipSocketStackCompositeAdjustedThunk,
    (void *)AllocatedTcpipCompositeCaptureLookupAdjustedThunk,
    (void *)AllocatedTcpipCompositeSendUdpAdjustedThunk,
    (void *)AllocatedTcpipCompositeSetPortAdjustedThunk,
    0,
};
static void *g_NETWORK_TcpipCompositeTimedThunkVtable =
    g_NETWORK_TcpipCompositeTimedThunkRecoveredVtable;
static void *g_NETWORK_TcpipCompositeOuterTimedThunkVtable =
    g_NETWORK_TcpipCompositeTimedThunkVtable;
static void *g_NETWORK_TcpipCompositeOuterDualThunkVtable =
    g_NETWORK_TcpipCompositeDualRecoveredVtable;
static void *g_NETWORK_TcpipCompositeOuterSocketThunkVtable =
    g_NETWORK_TcpipCompositeSocketThunkVtable;

/* Allocation-composite construction groups recovered from 0049a348..e0. */
static int g_NETWORK_AllocatedTcpipCompositeTransportVtable[8] = {
    -4, 0x30, 0x60, 0xd8, 0x130, 0x138, 0x154, 0x158,
};
static int g_NETWORK_AllocatedTcpipCompositeSocketStackVtable[8] = {
    0, 8, 0x38, 0xb0, 0x108, 0x110, 0x12c, 0x130,
};
static int g_NETWORK_AllocatedTcpipCompositeDualStreamVtable[4] = {
    -0x44, -0x74, 0, 8,
};
static int g_NETWORK_AllocatedTcpipCompositeDualThunkVtable[4] = {
    -0x44, -0xec, -0x44, -0x74,
};
static int g_NETWORK_AllocatedTcpipCompositeSocketWindowVtable[4] = {
    0, -0x100, -0xd0, -0x58,
};
static int g_NETWORK_AllocatedTcpipCompositeTimedSocketBundleVtable[4] = {
    -0x10, -0x118, 0, -0x100,
};
static int g_NETWORK_AllocatedTcpipCompositeTimedSocketThunkVtable[4] = {
    0, -0x124, -0x7c, -0x1c,
};
static int g_NETWORK_AllocatedTcpipCompositeTimedSocketDataVtable[4] = {
    0, -0x128, -0xf8, -0x20,
};
// FUNCTION: LEMBALL 0x00471BF0
static void AllocatedTcpipCompositeCaptureLookupAdjustedThunk(
    void *pObject, void *pLookupResult) {
    ((VsNetTimedSocketAsyncLookupAdapter *)((char *)pObject + 0x140))
        ->CaptureAsyncLookupAddressForTimedStream(pLookupResult);
}
// FUNCTION: LEMBALL 0x00471C00
static int AllocatedTcpipCompositeSendUdpAdjustedThunk(void *pObject,
                                                        void *pBuffer,
                                                        int cbBuffer) {
    return ((VsNetTimedSocketAsyncLookupAdapter *)((char *)pObject + 0x140))
        ->SendUdpPayloadToSocketAddress(pBuffer, cbBuffer);
}
// FUNCTION: LEMBALL 0x00471C10
static void AllocatedTcpipCompositeSetPortAdjustedThunk(void *pObject,
                                                          short nPort) {
    SetTcpipSocketChannelAssignedPort((char *)pObject + 0x140, nPort);
}
/* 0049a3d8: allocated-composite primary table.  This is distinct from
   0049a1c8, used by the standalone composite constructor. */
static void *g_NETWORK_AllocatedTcpipCompositeVtable[7] = {
    ((void ***)&g_NETWORK_TcpipCompositeVtableModel)[0],
    ((void ***)&g_NETWORK_TcpipCompositeVtableModel)[1],
    ((void ***)&g_NETWORK_TcpipCompositeVtableModel)[2],
    ((void ***)&g_NETWORK_TcpipCompositeVtableModel)[3],
    ((void ***)&g_NETWORK_TcpipCompositeVtableModel)[4],
    (void *)TcpipCompositeUnusedVtableSlot,
    0,
};
static void *g_NETWORK_AllocatedTcpipCompositeFatalThunkVtable[4] = {
    (void *)CaptureLastWinsockErrorStatusOuterDoubleAdjustedThunk,
    (void *)DeleteCompositeTcpipSocketChannelStackAdjustedThunk,
    (void *)CaptureLastWinsockErrorStatusOuterAdjustedThunk,
    (void *)CompleteEffTransportPendingWriteAllocatedAdjustedThunk,
};
static void *g_NETWORK_AllocatedTcpipCompositeTimedThunkVtable[11] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReverseEffTransportPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteCompositeTcpipSocketChannelStackTimedAdjustedThunk,
    (void *)WriteCompositeTcpipGlobalSessionAdjustedThunk,
    (void *)DispatchCompositeTcpipAckedPayloadAdjustedThunk,
    (void *)AllocatedTcpipCompositeCaptureLookupAdjustedThunk,
    (void *)AllocatedTcpipCompositeSendUdpAdjustedThunk,
    (void *)AllocatedTcpipCompositeSetPortAdjustedThunk,
};
static void *g_NETWORK_AllocatedTcpipCompositeDualThunk[10] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReverseEffTransportPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteEffTransportTaggedHeader,
    (void *)DeleteAllocatedTcpipSocketChannelStackAdjustedThunk,
    (void *)QueueAllocatedEffTransportConnectEventAdjustedThunk,
    (void *)WriteAllocatedEffTransportGlobalSessionAdjustedThunk,
    (void *)DispatchAllocatedAckedEffTransportPayloadAdjustedThunk,
    (void *)QueueAllocatedEffTransportPayloadEventAdjustedThunk,
};
static void *g_NETWORK_AllocatedTcpipCompositeSocketThunkVtable[2] = {
    (void *)DispatchAllocatedTcpipHostPortAsyncMessageAdjustedThunk,
    (void *)NoopVtableCallbackThunk,
};

static const char *g_NETWORK_SocketWindowClassName;
static int *g_pfSocketWindowClassRegistered;
static VsNetInetAddressEntry g_NETWORK_InetAddressEntryVtableModel;
static void *g_NETWORK_InetAddressEntryVtable =
    *(void ***)&g_NETWORK_InetAddressEntryVtableModel;
extern void *g_pEffTransportRuntimeService;
extern "C" const char *WINAPI inet_ntoa(unsigned long nAddress);

// FUNCTION: LEMBALL 0x0046F9B0
void VsNetInetAddressEntry::SetAddressMode(int nMode) {
    void **pVtable;

    if (nMode == 0) {
        pVtable = *(void ***)this;
        ((void (*)(void *))pVtable[3])(g_pEffTransportRuntimeService);
        *(unsigned char *)((char *)this + 0x17) = 0xff;
    } else if (nMode == 1) {
        *(int *)((char *)this + 0x14) = -1;
    }
}

// FUNCTION: LEMBALL 0x00471500
void VsNetInetAddressEntry::CopyAddressEntry(
    const VsNetInetAddressEntry *pSource) {
    char *pszAddress;

    pszAddress = (char *)inet_ntoa((unsigned long)pSource->m_nAddress);
    m_nAddress = pSource->m_nAddress;
    strcpy(m_szAddress, pszAddress);
}

// FUNCTION: LEMBALL 0x00471590
int VsNetInetAddressEntry::IsAddressLessOrEqual(
    const VsNetInetAddressEntry *pSource) {
    return m_nAddress <= pSource->m_nAddress;
}

// FUNCTION: LEMBALL 0x0046F9F0
int VsNetInetAddressEntry::IsAddressGreaterOrEqual(
    const VsNetInetAddressEntry *pSource) {
    return m_nAddress > pSource->m_nAddress;
}

// FUNCTION: LEMBALL 0x00471A10
static void *DeleteTcpipRuntimeStateWindowAdjustedThunk(void *pObject,
                                                          BYTE fFreeMemory) {
    char *pWindowBase;

    pWindowBase = (char *)pObject - 0x10;
    ((VsNetEffTransportRuntimeState *)pWindowBase)
        ->DestroyEffTransportRuntimeGlobals();
    DestroyInvisibleMessageWindow((VS_InvisibleWindow *)pWindowBase);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pWindowBase);
    }
    return pWindowBase;
}

// FUNCTION: LEMBALL 0x004715C0
static void *AllocateTcpipInetAddressEntry(void) {
    VsNetInetAddressEntry *pObject;

    pObject = (VsNetInetAddressEntry *)AllocateVSMemBlock(0x18);
    if (pObject == 0) {
        return 0;
    }
    *(void ***)pObject = (void **)g_NETWORK_InetAddressEntryVtable;
    pObject->m_szAddress[0] = 0;
    return pObject;
}

/* 0049a280: four-slot socket front, six-slot address suffix, then the
 * fourteen-slot TCP/IP runtime-state suffix. */
static void *g_NETWORK_TcpipSocketStackRuntimeRecoveredVtable[24] = {
    (void *)CaptureLastWinsockErrorStatusOuterDoubleAdjustedThunk,
    (void *)DeleteTcpipSocketStackPrimaryAdjustedThunk,
    (void *)CaptureLastWinsockErrorStatusOuterAdjustedThunk,
    (void *)ClearTcpipSocketStackPendingWriteAdjustedThunk,
    ((void **)g_NETWORK_InetAddressEntryVtable)[0],
    ((void **)g_NETWORK_InetAddressEntryVtable)[1],
    ((void **)g_NETWORK_InetAddressEntryVtable)[2],
    ((void **)g_NETWORK_InetAddressEntryVtable)[3],
    ((void **)g_NETWORK_InetAddressEntryVtable)[4],
    ((void **)g_NETWORK_InetAddressEntryVtable)[5],
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteTcpipRuntimeStateWindowAdjustedThunk,
    ((void **)&g_NETWORK_TcpipRuntimeStateVtable)[2],
    ((void **)&g_NETWORK_TcpipRuntimeStateVtable)[3],
    ((void **)&g_NETWORK_TcpipRuntimeStateVtable)[4],
    ((void **)&g_NETWORK_TcpipRuntimeStateVtable)[5],
    ((void **)&g_NETWORK_TcpipRuntimeStateVtable)[6],
    ((void **)&g_NETWORK_TcpipRuntimeStateVtable)[7],
    ((void **)&g_NETWORK_TcpipRuntimeStateVtable)[8],
    (void *)AllocateTcpipEffTransportComposite,
    (void *)AllocateTcpipEffTransportRuntimeOwner,
    (void *)AllocateTcpipInetAddressEntry,
    ((void **)&g_NETWORK_TcpipRuntimeStateVtable)[12],
    ((void **)g_NETWORK_TcpipRuntimeWindowVtable)[0],
};
static const char *g_NETWORK_UdpServiceName;
static const char *g_NETWORK_TftpServiceName;

extern "C" DWORD WINAPI timeGetTime(void);
extern "C" int WINAPI sendto(int nSocket, const char *pBuffer, int cbBuffer, int nFlags,
                              const void *pTo, int nToLength);
extern "C" int WINAPI WSAGetLastError(void);
extern "C" int WINAPI closesocket(int nSocket);
extern "C" int WINAPI recv(int nSocket, char *pBuffer, int cbBuffer, int nFlags);
extern "C" int WINAPI recvfrom(int nSocket, char *pBuffer, int cbBuffer, int nFlags,
                                void *pFrom, int *pcbFromLength);
extern "C" const char *WINAPI inet_ntoa(unsigned long nAddress);
extern "C" unsigned long WINAPI inet_addr(const char *pszAddress);
extern "C" int WINAPI socket(int nAddressFamily, int nType, int nProtocol);
extern "C" int WINAPI WSAAsyncGetHostByName(int nSocket, unsigned int nMessage,
                                               const char *pszHost, void *pBuffer,
                                               int cbBuffer);
extern "C" int WINAPI WSACancelAsyncRequest(int nLookupHandle);
extern "C" int WINAPI gethostname(char *pszHost, int cbHost);
/* The MSVC 4.20 Winsock import library used by the original build exports
   these two Winsock 1.1 entry points. The available compatibility libraries
   do not, so retain the ABI here until the original import library is used. */
extern "C" int WINAPI WSAAsyncGetServByName(int nSocket, unsigned int nMessage,
                                              const char *pszService, const char *pszProtocol,
                                              void *pBuffer) {
    (void)nSocket;
    (void)nMessage;
    (void)pszService;
    (void)pszProtocol;
    (void)pBuffer;
    return 0;
}

extern "C" int WINAPI WSAAsyncGetServByNameWithLength(int nSocket, unsigned int nMessage,
                                                        const char *pszService,
                                                        const char *pszProtocol, void *pBuffer,
                                                        int cbBuffer) {
    (void)cbBuffer;
    return WSAAsyncGetServByName(nSocket, nMessage, pszService, pszProtocol, pBuffer);
}
extern "C" unsigned short WINAPI ntohs(unsigned short nValue);
extern "C" unsigned short WINAPI htons(unsigned short nValue);
extern "C" int WINAPI setsockopt(int nSocket, int nLevel, int nOption,
                                  const char *pValue, int cbValue);
extern "C" int WINAPI bind(int nSocket, const void *pAddress, int cbAddress);
extern "C" int WINAPI WSAAsyncSelect(int nSocket, void *hWnd, unsigned int nMessage,
                                      long nEvents);

extern void *g_pEffTransportPeerAddressState;
extern void *g_pEffTransportDispatchQueue;
extern void *g_pEffTransportPacketBuffer;
extern void *g_pEffTransportRuntimeService;
extern int g_fFileBasedNetworkPathConfigured;
extern short g_nEffTransportServiceBasePort;
extern void QueueEffTransportErrorEvent(int nError);
extern int g_cbEffTransportCurrentPacketBytes;
extern int g_cbEffTransportMaxPacketBytes;
extern char *g_pszFileBasedNetworkLocalHostName;
struct VsNetEffTransportPeer {
    void PrepareEffTransportBroadcastStatusPayload(char *pszHostName);
};
static int *g_pEffTransportReceivedPacketBytes;

// FUNCTION: LEMBALL 0x00470580
void VsNetTcpipCompositeControlVtableModel::ConfigureRemoteHost(
    const char *pszHost) {
    VsNetTcpipEffTransportCompositeLayout *pComposite;
    VsNetInetAddressEntry *pAddress;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pErrorChannel;
    int nAddress;
    int nResult;

    pComposite = (VsNetTcpipEffTransportCompositeLayout *)this;
    nAddress = (int)inet_addr(pszHost);
    if (nAddress != -1) {
        pAddress = (VsNetInetAddressEntry *)AllocateVSMemBlock(0x18);
        if (pAddress != 0) {
            *(void ***)pAddress = (void **)g_NETWORK_InetAddressEntryVtable;
            pAddress->m_szAddress[0] = '\0';
            pComposite->m_nRuntimeField1c = (int)(unsigned long)pAddress;
        } else {
            pComposite->m_nRuntimeField1c = 0;
        }
        pAddress = (VsNetInetAddressEntry *)(unsigned long)pComposite->m_nRuntimeField1c;
        pAddress->StoreAddressString(pszHost);
        pAddress->GetAddressString();
        return;
    }

    if (pComposite->m_nLookupHandle28 != 0) {
        nResult = WSACancelAsyncRequest(pComposite->m_nLookupHandle28);
        if (nResult != 0) {
            pOffsets = pComposite->m_pOuterOffsets04;
            pErrorChannel = (VsNetAdjustorSubobject *)((char *)pComposite +
                pOffsets->m_nSecondaryOffset + 4);
            ((void (**)(void *))pErrorChannel->m_pVtable)[2](pErrorChannel);
            return;
        }
        FreeVSMemBlock((void *)(unsigned long)pComposite->m_nSocketWindowHandle24);
        pComposite->m_nSocketWindowHandle24 = 0;
    }

    pComposite->m_nSocketWindowHandle24 =
        (int)(unsigned long)AllocateVSMemBlock(0x400);
    pOffsets = pComposite->m_pOuterOffsets04;
    pComposite->m_nLookupHandle28 = WSAAsyncGetHostByName(
        *(int *)((char *)pComposite + pOffsets->m_nOffset14 + 8),
        0x441, pszHost,
        (void *)(unsigned long)pComposite->m_nSocketWindowHandle24, 0x400);
}

// FUNCTION: LEMBALL 0x00470730
int VsNetTcpipCompositeControlVtableModel::StartLocalHostLookup(char *pszHost) {
    VsNetTcpipEffTransportCompositeLayout *pComposite;
    VsNetConstructionAdjustorVtable *pOffsets;
    char szHost[0x100];
    int cbHost;
    int nLookupHandle;

    pComposite = (VsNetTcpipEffTransportCompositeLayout *)this;
    ((VsNetEffTransportPeer *)this)->PrepareEffTransportBroadcastStatusPayload(pszHost);
    if (gethostname(szHost, sizeof(szHost)) == -1) {
        return 0;
    }

    cbHost = (int)strlen(szHost) + 1;
    if (g_pszFileBasedNetworkLocalHostName != 0) {
        FreeVSMemBlock(g_pszFileBasedNetworkLocalHostName);
    }
    g_pszFileBasedNetworkLocalHostName = (char *)AllocateVSMemBlock((unsigned int)cbHost);
    memcpy(g_pszFileBasedNetworkLocalHostName, szHost, (unsigned int)cbHost);

    pOffsets = pComposite->m_pOuterOffsets04;
    pComposite->m_nRuntimeField18 = (int)(unsigned long)AllocateVSMemBlock(0x400);
    *(int *)((char *)pComposite + pOffsets->m_nPrimaryOffset + 0x18) = 1;
    nLookupHandle = WSAAsyncGetHostByName(
        *(int *)((char *)pComposite + pOffsets->m_nOffset14 + 8), 0x440,
        g_pszFileBasedNetworkLocalHostName,
        (void *)(unsigned long)pComposite->m_nRuntimeField18, 0x400);
    pComposite->m_nLookupHandle28 = nLookupHandle;
    return nLookupHandle != 0;
}

// FUNCTION: LEMBALL 0x00470D30
void VsNetTcpipCompositeControlVtableModel::EnableAsyncSelect(void) {
    VsNetTcpipEffTransportCompositeLayout *pComposite;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pSocketChannel;
    int nResult;

    pComposite = (VsNetTcpipEffTransportCompositeLayout *)this;
    if (pComposite->m_nRuntimeField14 != 0) {
        return;
    }
    pOffsets = pComposite->m_pOuterOffsets04;
    pSocketChannel = (VsNetAdjustorSubobject *)((char *)pComposite +
        pOffsets->m_nSecondaryOffset + 4);
    if (*(int *)((char *)pSocketChannel + 0x14) == 0) {
        return;
    }
    nResult = WSAAsyncSelect(
        *(int *)((char *)pSocketChannel + 0x0c),
        (void *)(unsigned long)*(int *)((char *)pComposite + pOffsets->m_nOffset14 + 8),
        0x443, 3);
    if (nResult == -1) {
        ((void (**)(void *))pSocketChannel->m_pVtable)[2](pSocketChannel);
        return;
    }
    pComposite->m_nRuntimeField14 = 1;
}

// FUNCTION: LEMBALL 0x00470D80
void VsNetTcpipCompositeControlVtableModel::DisableAsyncSelect(void) {
    VsNetTcpipEffTransportCompositeLayout *pComposite;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pSocketChannel;
    int nResult;

    pComposite = (VsNetTcpipEffTransportCompositeLayout *)this;
    if (pComposite->m_nRuntimeField14 == 0) {
        return;
    }
    pOffsets = pComposite->m_pOuterOffsets04;
    pSocketChannel = (VsNetAdjustorSubobject *)((char *)pComposite +
        pOffsets->m_nSecondaryOffset + 4);
    if (*(int *)((char *)pSocketChannel + 0x14) == 0) {
        pComposite->m_nRuntimeField14 = 0;
        return;
    }
    nResult = WSAAsyncSelect(
        *(int *)((char *)pSocketChannel + 0x0c),
        (void *)(unsigned long)*(int *)((char *)pComposite + pOffsets->m_nOffset14 + 8),
        0x443, 2);
    if (nResult == -1) {
        ((void (**)(void *))pSocketChannel->m_pVtable)[2](pSocketChannel);
        return;
    }
    pComposite->m_nRuntimeField14 = 0;
}

// FUNCTION: LEMBALL 0x00471C20
static void SetTcpipSocketChannelAssignedPort(void *pObject, short nPort) {
    char *pbObject;
    int nOffsets;
    int nChannelDelta;
    int nFieldDelta;
    int nBaseDelta;
    unsigned short nSocketPort;

    pbObject = (char *)pObject;
    nOffsets = *(int *)(pbObject - 0x48);
    nChannelDelta = *(int *)(nOffsets + 8);
    nFieldDelta = *(int *)(*(int *)(pbObject + nChannelDelta - 4) + 4);
    *(short *)(pbObject + nFieldDelta + nChannelDelta - 0x48 + 0x64) = nPort;

    nBaseDelta = *(int *)(nOffsets + 4);
    nSocketPort = *(unsigned short *)(pbObject + nBaseDelta - 0x28);
    *(unsigned short *)(pbObject - 0x42) = htons((unsigned short)(nSocketPort + g_nEffTransportServiceBasePort));
}
struct VsNetEffTransportPacketProcessor {
    int ProcessEffTransportPacketHeader(void);
};

extern void *ConstructDualHandleEffStream(void *pStream, int fConstructChannelState);


// FUNCTION: LEMBALL 0x00460A90
void *VsNetCompositeEffTransportStackLayout::ConstructCompositeEffTransportStack(
    int fConstructEmbeddedObjects) {
    VsNetCompositeEffTransportStackLayout *pComposite;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    char *pbObjectBase;

    pComposite = this;
    pbObjectBase = (char *)this;

    if (fConstructEmbeddedObjects != 0) {
        pComposite->m_pChannelStateOffsets04 =
            (VsNetConstructionAdjustorVtable *)g_NETWORK_CompositeEffTransportChannelStateConstructionOffsets;
        pComposite->m_pTimedStreamOffsetsA4 =
            (VsNetConstructionAdjustorVtable *)g_NETWORK_CompositeEffTransportTimedStreamConstructionOffsets;
        *(void **)(pbObjectBase + 0x11c) = g_NETWORK_CompositeEffTransportDualStreamConstructionOffsets;
        *(void **)(pbObjectBase + 0x130) = g_NETWORK_CompositeEffTransportDualThunkConstructionOffsets;

        pComposite->m_ChannelState30.ConstructEffStreamChannelState();
        ((VsNetTimedEffStream *)pComposite->m_abTimedStream60)->ConstructTimedEffStream(0);
        ConstructDualHandleEffStream(pbObjectBase + 0xd8, 0);

        pOffsets = (VsNetConstructionAdjustorVtable *)g_NETWORK_CompositeEffTransportDualThunkConstructionOffsets;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x12c + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x12c + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x12c + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
        pSecondaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackTimedStreamVtable;
        pTertiaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackDualStreamVtable;
        pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
        pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
        pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;
    }

    pComposite->m_pVtable00 = (void **)g_NETWORK_CompositeEffTransportVtable;
    pOffsets = pComposite->m_pChannelStateOffsets04;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_CompositeEffTransportFatalThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_CompositeEffTransportTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x2c;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x5c;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xd4;

    pComposite->m_nField0c = 0;
    pComposite->m_nField08 = 0;
    pComposite->m_nField14 = 0;
    pComposite->m_nField10 = 0;
    pComposite->m_nField1c = 0;
    pComposite->m_nField20 = 0;
    pComposite->m_dwLastActivity24 = 0;
    pComposite->m_nField18 = 0;
    *(DWORD *)(pbObjectBase + 0x28) = timeGetTime();
    return this;
}

// FUNCTION: LEMBALL 0x00460C00
void RestoreCompositeEffTransportVtables(int nObjectBasePlus0x30) {
    VsNetCompositeEffTransportStackLayout *pComposite;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    char *pbObjectBase;

    pbObjectBase = (char *)(unsigned long)(nObjectBasePlus0x30 - 0x30);
    pComposite = (VsNetCompositeEffTransportStackLayout *)pbObjectBase;
    pComposite->m_pVtable00 = (void **)g_NETWORK_CompositeEffTransportVtable;
    pOffsets = pComposite->m_pChannelStateOffsets04;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_CompositeEffTransportFatalThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_CompositeEffTransportTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x2c;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x5c;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xd4;
}

// FUNCTION: LEMBALL 0x0046FCF0
void *VsNetSocketWindowEffChannel::ConstructSocketWindowEffChannel(
    int fConstructEmbeddedState) {
    VsNetSocketWindowEffChannel *pChannel;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;

    pChannel = this;

    if (fConstructEmbeddedState != 0) {
        pChannel->m_pEmbeddedStateConstructionOffsets = g_NETWORK_SocketWindowEffChannelEmbeddedStateVtable;
        pChannel->m_ChannelState1c.ConstructEffStreamChannelState();
    }

    ((VS_InvisibleWindow *)pChannel)
        ->Construct(g_NETWORK_SocketWindowClassName, g_pfSocketWindowClassRegistered);
    pChannel->m_pVTable = g_NETWORK_SocketWindowEffChannelVtable;
    pOffsets = pChannel->m_pEmbeddedStateConstructionOffsets;
    pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pChannel + 0x0c + pOffsets->m_nPrimaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_SocketWindowEffChannelThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x10;
    pChannel->m_nSocketHandle = 0;
    pChannel->m_nLookupHandle = 0;
    pChannel->m_dwReserved0 = 0x440;
    pChannel->m_dwReserved1 = 0x45f;
    return this;
}

// FUNCTION: LEMBALL 0x0046FD70
void DestroySocketWindowEffChannel(int nChannelStateBase) {
    VsNetSocketWindowEffChannel *pChannel;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;

    pChannel = (VsNetSocketWindowEffChannel *)(unsigned long)(nChannelStateBase - 0x20);
    pChannel->m_pVTable = g_NETWORK_SocketWindowEffChannelVtable;
    pOffsets = (VsNetConstructionAdjustorVtable *)pChannel->m_pEmbeddedStateConstructionOffsets;
    pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pChannel + 0x0c + pOffsets->m_nPrimaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_SocketWindowEffChannelThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x10;

    if (pChannel->m_nLookupHandle != 0) {
        FreeVSMemBlock((void *)(unsigned long)pChannel->m_nLookupHandle);
    }

    DestroyInvisibleMessageWindow((VS_InvisibleWindow *)pChannel);
}

// FUNCTION: LEMBALL 0x0046FEE0
int VsNetSocketAsyncMessageAdapter::ReceiveUdpPayloadFromConnectedSocket(void) {
    VsNetTimedSocketErrorChannel *pErrorChannel;
    VsNetTimedSocketPacketChannel *pPacketChannel;
    int nThisDelta;
    int nResult;
    char *pbThis;

    pbThis = (char *)this;
    nThisDelta = *(int *)(*(int *)pbThis + 4);
    nResult = recv(*(int *)(pbThis + nThisDelta + 8), (char *)g_pEffTransportPacketBuffer,
                   g_cbEffTransportMaxPacketBytes, 0);
    g_cbEffTransportCurrentPacketBytes = nResult;
    *(DWORD *)(*(int *)(*(int *)pbThis + 8) + (int)(unsigned long)pbThis + 0x3c) =
        timeGetTime();
    if (nResult == -1) {
        AppendCStringToStream(g_pErrorOutputStream, "Receive error (after receive):");
        g_pErrorOutputStream->AppendIntToStream((unsigned int)WSAGetLastError());
        AppendCStringToStream(g_pErrorOutputStream, "\n");
        pErrorChannel = (VsNetTimedSocketErrorChannel *)(pbThis + nThisDelta);
        pErrorChannel->HandleSocketError();
        return 0;
    }
    if (*(int *)(pbThis + nThisDelta + 0x10) == 0) {
        pPacketChannel = (VsNetTimedSocketPacketChannel *)(pbThis +
                                                               *(int *)(*(int *)pbThis + 8));
        pPacketChannel->NotifyPacketReady();
    }
    *g_pEffTransportReceivedPacketBytes += g_cbEffTransportCurrentPacketBytes;
    return ((VsNetEffTransportPacketProcessor *)(pbThis + *(int *)(*(int *)pbThis + 8)))
        ->ProcessEffTransportPacketHeader();
}

// FUNCTION: LEMBALL 0x0046FF90
int VsNetSocketAsyncMessageAdapter::HandleSocketReceiveAsyncSelectMessage(
    int nMessage, void *pReserved, unsigned int nEvent) {
    int nThisDelta;
    unsigned int nError;
    VsNetSocketAsyncMessageAdapter *pReceiveChannel;

    (void)pReserved;
    if (nMessage == 0x443) {
        nThisDelta = *(int *)(*(int *)((char *)this - 0x8c) + 4);
        if (*(int *)((char *)this + nThisDelta - 0x84) == -1) {
            return 0;
        }
        nError = nEvent >> 16;
        ((VsNetEffStreamChannelState *)((char *)this + nThisDelta - 0x8c))->SetEffStreamChannelAsyncErrorStatus((int)nError);
        if ((nEvent & 0xffff) == 1) {
            pReceiveChannel = (VsNetSocketAsyncMessageAdapter *)((char *)this - 0x8c);
            if (nError == 0) {
                pReceiveChannel->ReceiveUdpPayloadFromConnectedSocket();
                return 0;
            }
            AppendCStringToStream(g_pErrorOutputStream, "Receive error:");
        g_pErrorOutputStream->AppendIntToStream(nError);
            AppendCStringToStream(g_pErrorOutputStream, "\n");
            return 0;
        }
    }
    return -1;
}

// FUNCTION: LEMBALL 0x0046FDB0
int VsNetSocketAsyncErrorAdapter::HandleSocketAsyncErrorLparam(
    void *pReserved, unsigned int nEvent, unsigned int *pBuffer) {
    unsigned int nError;
    int nChannelDelta;

    (void)pReserved;
    nError = nEvent >> 16;
    if (nError == 0) {
        return 0;
    }
    if (nError > 11000 && nError < 0x2afd) {
        return 2;
    }
    nChannelDelta = *(int *)(*(int *)((char *)this + 0x10) + 4);
    ((VsNetEffStreamChannelState *)((char *)this + nChannelDelta + 0x10))->SetEffStreamChannelAsyncErrorStatus((int)nError);
    if (*pBuffer != 0) {
        FreeVSMemBlock((void *)(unsigned long)*pBuffer);
    }
    *pBuffer = 0;
    return 0x0e;
}

// FUNCTION: LEMBALL 0x0046FE10
int VsNetSocketAsyncMessageAdapter::ReceiveUdpPayloadFromSenderAddress(void) {
    unsigned char abFrom[16];
    int cbFrom;
    int nResult;
    int nChannelDelta;
    char *pszAddress;
    VsNetInetAddressEntry *pPeerAddress;
    VsNetSocketChannelControl *pErrorChannel;

    cbFrom = 0x10;
    nChannelDelta = *(int *)(*(int *)this + 4);
    nResult = recvfrom(*(int *)((char *)this + nChannelDelta + 8),
                       (char *)g_pEffTransportPacketBuffer, g_cbEffTransportMaxPacketBytes, 0,
                       abFrom, &cbFrom);
    g_cbEffTransportCurrentPacketBytes = nResult;
    pPeerAddress = (VsNetInetAddressEntry *)g_pEffTransportPeerAddressState;
    if (nResult == -1) {
        AppendCStringToStream(g_pErrorOutputStream, "Receive error (after receive from):");
        g_pErrorOutputStream->AppendIntToStream((unsigned int)WSAGetLastError());
        AppendCStringToStream(g_pErrorOutputStream, "\n");
        pErrorChannel = (VsNetSocketChannelControl *)((char *)this + nChannelDelta);
        pErrorChannel->HandleSocketError();
        return 0;
    }
    pPeerAddress->m_nAddress = *(int *)abFrom;
    pszAddress = (char *)inet_ntoa((unsigned long)pPeerAddress->m_nAddress);
    strcpy(pPeerAddress->m_szAddress, pszAddress);
    return ((VsNetEffTransportPacketProcessor *)((char *)this +
                                                    *(int *)(*(int *)this + 8)))
        ->ProcessEffTransportPacketHeader();
}

// FUNCTION: LEMBALL 0x00470030
void *VsNetTimedSocketEffChannelBundleLayout::ConstructTimedSocketEffChannelBundle(
    int fConstructEmbeddedObjects) {
    VsNetTimedSocketEffChannelBundleLayout *pBundle;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    char *pbObjectBase;

    pBundle = this;
    pbObjectBase = (char *)this;

    if (fConstructEmbeddedObjects != 0) {
        pBundle->m_pVtable00 = (void **)g_NETWORK_TimedSocketBundleBaseConstructionOffsets;
        pBundle->m_pTimedStreamConstructionOffsets8c = (void **)g_NETWORK_TimedSocketBundleTimedStreamConstructionOffsets;
        pBundle->m_pSocketChannelConstructionOffsetsd0 = (void **)g_NETWORK_TimedSocketBundleSocketChannelConstructionOffsets;
        pBundle->m_ChannelState18.ConstructEffStreamChannelState();
        ((VsNetTimedEffStream *)pBundle->m_abTimedStream48)
            ->ConstructTimedEffStream(0);
        ((VsNetSocketWindowEffChannel *)pBundle->m_abSocketWindowC0)
            ->ConstructSocketWindowEffChannel(0);
    }

    pOffsets = (VsNetConstructionAdjustorVtable *)pBundle->m_pVtable00;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleChannelThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSocketThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x18;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x48;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xc0;
    pBundle->m_nMode04 = 2;
    return this;
}

// FUNCTION: LEMBALL 0x004700F0
void VsNetTimedSocketAsyncLookupAdapter::CaptureAsyncLookupAddressForTimedStream(void *pLookupResult) {
    void *pTimedStream;

    pTimedStream = (void *)(unsigned long)(*(int *)(*(int *)((char *)this - 0x48) + 8) +
                                           (int)(unsigned long)this - 0x48);
    ((VsNetTimedEffStream *)pTimedStream)->InvokeTimedEffStreamServiceCallback(pLookupResult);
    *(int *)((char *)this - 0x40) = *(int *)((char *)pLookupResult + 0x14);
}

// FUNCTION: LEMBALL 0x00470120
int VsNetTimedSocketAsyncLookupAdapter::SendUdpPayloadToSocketAddress(void *pBuffer, int cbBuffer) {
    int nSocket;
    int nResult;
    VsNetTimedSocketErrorChannel *pChannel;
    char *pbThis;

    pbThis = (char *)this;
    nSocket = *(int *)(*(int *)(pbThis - 0x48) + 4);
    if (*(int *)(pbThis + nSocket - 0x24) == 0) {
        return 0;
    }

    nResult = sendto(*(int *)(pbThis + nSocket - 0x40), (const char *)pBuffer, cbBuffer, 0,
                     (const void *)(pbThis - 0x44), 0x10);
    *(DWORD *)(*(int *)(*(int *)(pbThis - 0x48) + 8) + (int)(unsigned long)pbThis - 0x0c) =
        timeGetTime();
    if (nResult == -1) {
        if (WSAGetLastError() == 0x2733) {
            return 0;
        }
        pChannel = (VsNetTimedSocketErrorChannel *)(pbThis + nSocket - 0x48);
        pChannel->HandleSocketError();
        return 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x004701A0
int VsNetTimedSocketAsyncMessageAdapter::HandleTimedSocketConnectAsyncMessage(
    int nMessage, void *pReserved, unsigned int nEvent) {
    int nThisDelta;

    (void)pReserved;
    if (nMessage == 0x443) {
        nThisDelta = *(int *)(*(int *)((char *)this - 0xc0) + 4);
        if (*(int *)((char *)this + nThisDelta - 0xb8) == -1) {
            return -1;
        }
        ((VsNetEffStreamChannelState *)((char *)this + nThisDelta - 0xc0))->SetEffStreamChannelAsyncErrorStatus((int)(nEvent >> 16));
        if ((nEvent & 0xffff) == 2) {
            if ((nEvent >> 16) == 0) {
                *(int *)(*(int *)(*(int *)((char *)this - 0xc0) + 4) +
                         (int)(unsigned long)this - 0x9c) = 1;
            }
            return 0;
        }
    }
    return -1;
}

// FUNCTION: LEMBALL 0x00470220
int VsNetTimedSocketAsyncMessageAdapter::DispatchEffSocketAsyncSelectMessage(
    int nMessage, void *pReserved, unsigned int nEvent) {
    int nTimedDelta;
    int nReceiveDelta;
    int nResult;
    VsNetTimedSocketAsyncMessageAdapter *pTimedChannel;
    VsNetSocketAsyncMessageAdapter *pReceiveChannel;
    char *pbThis;

    pbThis = (char *)this;
    nTimedDelta = *(int *)(*(int *)(pbThis - 0x110) + 0x1c);
    pTimedChannel = (VsNetTimedSocketAsyncMessageAdapter *)(pbThis + nTimedDelta - 0x50);
    nResult = pTimedChannel->HandleTimedSocketConnectAsyncMessage(nMessage, pReserved, nEvent);
    if (nResult == -1) {
        nReceiveDelta = *(int *)(*(int *)(pbThis - 0x110) + 0x18);
        pReceiveChannel = (VsNetSocketAsyncMessageAdapter *)(pbThis + nReceiveDelta - 0x84);
        return pReceiveChannel->HandleSocketReceiveAsyncSelectMessage(
            nMessage, pReserved, nEvent);
    }
    return nResult;
}

// FUNCTION: LEMBALL 0x00470270
void *VsNetTcpipEffTransportCompositeLayout::ConstructTcpipEffTransportComposite(
    int fConstructEmbeddedObjects) {
    VsNetTcpipEffTransportCompositeLayout *pComposite;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    VsNetAdjustorSubobject *pQuaternaryThunk;
    volatile unsigned int nConstructionFlags;

    pComposite = this;
    nConstructionFlags = 0;
    if (fConstructEmbeddedObjects != 0) {
        pComposite->m_pOuterOffsets04 = (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTimedStreamVtable;
        pComposite->m_pTransportOffsets20 =
            (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTransportStackVtable;
        pComposite->m_pDualStreamConstructionOffsetsA4 = (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeDualStreamVtable;
        pComposite->m_pDualThunkOffsets11c = (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeDualThunkVtable;
        pComposite->m_pSocketThunkOffsets130 =
            (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeSocketWindowVtable;
        pComposite->m_pTimedSocketThunkOffsets148 =
            (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTimedSocketThunkVtable;
        pComposite->m_pTimedSocketBundleOffsets154 =
            (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTimedSocketBundleVtable;
        pComposite->m_pTimedSocketBundleConstructionData158 = (void **)g_NETWORK_TcpipCompositeTimedSocketBundleDataVtable;

        nConstructionFlags |= 1;
        pComposite->m_ChannelState30.ConstructEffStreamChannelState();
        nConstructionFlags |= 2;
        ((VsNetTimedEffStream *)pComposite->m_abTimedStream60)->ConstructTimedEffStream(0);
        nConstructionFlags |= 4;
        ConstructDualHandleEffStream(pComposite->m_abDualStreamD8, 0);

        nConstructionFlags |= 8;
        pOffsets = pComposite->m_pSocketThunkOffsets130;
        pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + 0x12c + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + 0x12c + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + 0x12c + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
        pSecondaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackTimedStreamVtable;
        pTertiaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackDualStreamVtable;
        pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
        pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
        pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;

        nConstructionFlags |= 0x10;
        ((VsNetSocketWindowEffChannel *)pComposite->m_abSocketWindow138)
            ->ConstructSocketWindowEffChannel(0);

        nConstructionFlags |= 0x20;
        pOffsets = pComposite->m_pTimedSocketBundleOffsets154;
        pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + 0x150 + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + 0x150 + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + 0x150 + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
        pPrimaryThunk->m_nThisDelta = 0;

        ((VsNetTimedSocketEffChannelBundleLayout *)&pComposite->m_pTimedSocketBundleConstructionData158)
            ->ConstructTimedSocketEffChannelBundle(0);
        nConstructionFlags |= 0x40;
    }

    ((VsNetRuntimeChannelStack *)this)->ConstructEffTransportRuntimeChannelStack(0);
    pOffsets = pComposite->m_pTransportOffsets20;
    pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nTertiaryOffset);
    pQuaternaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nOffset14);
    pPrimaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackRuntimeRecoveredVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TcpipCompositeTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
    pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipCompositeSocketThunkVtable;
    pPrimaryThunk->m_nThisDelta = 0;
    pSecondaryThunk->m_nThisDelta = 0;
    pTertiaryThunk->m_nThisDelta = 0;
    pQuaternaryThunk->m_nThisDelta = 0;

    pComposite->m_pVtable00 = *(void ***)&g_NETWORK_TcpipCompositeVtableModel;
    pOffsets = pComposite->m_pOuterOffsets04;
    pPrimaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + pOffsets->m_nTertiaryOffset);
    pQuaternaryThunk = (VsNetAdjustorSubobject *)((char *)pComposite + pOffsets->m_nOffset14);
    pPrimaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterFatalThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterSocketThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x2c;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x5c;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xd4;
    pQuaternaryThunk->m_nThisDelta = pOffsets->m_nOffset14 - 0x134;

    pComposite->m_nSocketWindowHandle24 = 0;
    pComposite->m_nLookupHandle28 = 0;
    return this;
}

// FUNCTION: LEMBALL 0x004704E0
void DestroyTcpipEffTransportComposite(int nObjectBasePlus0x30) {
    VsNetTcpipEffTransportCompositeLayout *pComposite;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    VsNetAdjustorSubobject *pQuaternaryThunk;
    char *pbObjectBase;

    pbObjectBase = (char *)(unsigned long)(nObjectBasePlus0x30 - 0x30);
    pComposite = (VsNetTcpipEffTransportCompositeLayout *)pbObjectBase;
    pComposite->m_pVtable00 = *(void ***)&g_NETWORK_TcpipCompositeVtableModel;
    pOffsets = pComposite->m_pOuterOffsets04;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset);
    pQuaternaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nOffset14);
    pPrimaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterFatalThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterSocketThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x2c;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x5c;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xd4;
    pQuaternaryThunk->m_nThisDelta = pOffsets->m_nOffset14 - 0x134;

    if ((void *)(unsigned long)pComposite->m_nSocketWindowHandle24 != 0) {
        FreeVSMemBlock((void *)(unsigned long)pComposite->m_nSocketWindowHandle24);
    }

    ((VsNetRuntimeChannelStackReleaseFront *)(unsigned long)(nObjectBasePlus0x30 - 0xc))->Release();
}

// FUNCTION: LEMBALL 0x00470650
void VsNetHostLookupCompletionAdapter::HandleRemoteHostLookupComplete(int nError) {
    char abAddress[20];
    char *pbLookup;
    char *pszAddress;
    VsNetInetAddressEntry *pEntry;
    VsNetEffDispatchQueue *pDispatchQueue;
    short cbAddress;
    unsigned char abEvent[20];

    pbLookup = *(char **)((char *)this + 0x24);
    if (nError == 0) {
        cbAddress = *(short *)(pbLookup + 0x0a);
        memcpy(abAddress, *(void **)(*(int *)(pbLookup + 0x0c)), cbAddress);
        pEntry = (VsNetInetAddressEntry *)AllocateVSMemBlock(0x18);
        *(void ***)pEntry = (void **)g_NETWORK_InetAddressEntryVtable;
        pEntry->m_szAddress[0] = '\0';
        *(char **)((char *)this + 0x1c) = (char *)pEntry;
        pEntry->m_nAddress = *(int *)abAddress;
        pszAddress = (char *)inet_ntoa((unsigned long)pEntry->m_nAddress);
        strcpy(pEntry->m_szAddress, pszAddress);
        pEntry->GetAddressString();
    } else {
        AppendCStringToStream(g_pErrorOutputStream, "Specified computer name not found\n");
        if (*(int *)((char *)this + 0x0c) == 2) {
            *(unsigned short *)abEvent = 0x000d;
            *(int *)(abEvent + 8) = 0x0f;
            pDispatchQueue = (VsNetEffDispatchQueue *)g_pEffTransportDispatchQueue;
            pDispatchQueue->QueueEvent(abEvent);
        }
    }
    FreeVSMemBlock(pbLookup);
    *(int *)((char *)this + 0x24) = 0;
}

// FUNCTION: LEMBALL 0x00470840
void VsNetHostLookupCompletionAdapter::HandleLocalHostLookupComplete(int nError) {
    char abAddress[4];
    char *pbLookup;
    char *pszAddress;
    VsNetInetAddressEntry *pLocalAddress;
    VsNetTimedSocketErrorChannel *pErrorChannel;
    int nTimedDelta;
    int nSocketDelta;
    char *pbTimedSocket;
    char *pbSocketChannel;

    if (nError != 0) {
        AppendCStringToStream(g_pErrorOutputStream, "Local host name not found\n");
    } else {
        nTimedDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
        pbLookup = *(char **)((char *)this + nTimedDelta + 0x18);
        memcpy(abAddress, *(void **)(*(int *)(pbLookup + 0x0c)), *(short *)(pbLookup + 0x0a));
        pLocalAddress = (VsNetInetAddressEntry *)g_pEffTransportRuntimeService;
        pLocalAddress->m_nAddress = *(int *)abAddress;
        pszAddress = (char *)inet_ntoa((unsigned long)pLocalAddress->m_nAddress);
        strcpy(pLocalAddress->m_szAddress, pszAddress);
        pLocalAddress->GetAddressString();
    }

    g_fFileBasedNetworkPathConfigured = 1;
    nTimedDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
    FreeVSMemBlock(*(void **)((char *)this + nTimedDelta + 0x18));
    *(int *)((char *)this + nTimedDelta + 0x18) = 0;

    nSocketDelta = *(int *)(*(int *)((char *)this + 4) + 4);
    pbSocketChannel = (char *)this + nSocketDelta;
    *(int *)(pbSocketChannel + 0x0c) = socket(2, 2, 0);
    if (*(int *)(pbSocketChannel + 0x0c) == -1) {
        pErrorChannel = (VsNetTimedSocketErrorChannel *)(pbSocketChannel + 4);
        pErrorChannel->HandleSocketError();
        QueueEffTransportErrorEvent(1);
        return;
    }

    *(int *)(pbSocketChannel + 0x10) = 1;
    pbTimedSocket = (char *)this + nTimedDelta;
    *(int *)(pbTimedSocket + 0x18) = (int)(unsigned long)AllocateVSMemBlock(0x400);
    *(int *)(pbSocketChannel + 0x18) = 1;
    *(int *)(pbTimedSocket + 0x1c) =
        WSAAsyncGetServByName(*(int *)(pbTimedSocket + 8), 0x442,
                              g_NETWORK_TftpServiceName, g_NETWORK_UdpServiceName,
                              (void *)(unsigned long)*(int *)(pbTimedSocket + 0x18));
    if (*(int *)(pbTimedSocket + 0x1c) == 0) {
        pErrorChannel = (VsNetTimedSocketErrorChannel *)(pbSocketChannel + 4);
        pErrorChannel->HandleSocketError();
        QueueEffTransportErrorEvent(2);
    }
}

// FUNCTION: LEMBALL 0x004709C0
void VsNetHostLookupCompletionAdapter::HandleServicePortLookupComplete(int nError) {
    unsigned char abAddress[16];
    unsigned char abEvent[20];
    int nOuterDelta;
    int nSocketDelta;
    int nPort;
    int nResult;
    int nSocket;
    int nSocketOption;
    char *pbLookup;
    char *pbTimedSocket;
    char *pbSocketChannel;
    VsNetSocketChannelControl *pControl;
    VsNetEffDispatchQueue *pDispatchQueue;

    nSocketDelta = *(int *)(*(int *)((char *)this + 4) + 8);
    pControl = (VsNetSocketChannelControl *)((char *)this + nSocketDelta + 4);
    if (nError == 0) {
        nOuterDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
        pbLookup = *(char **)((char *)this + nOuterDelta + 0x18);
        nPort = (int)(unsigned short)ntohs(*(unsigned short *)(pbLookup + 8));
        pControl->SetAssignedPort((short)(nPort - g_nEffTransportServiceBasePort));
    } else {
        AppendCStringToStream(g_pErrorOutputStream, "Failed to determine service port number\n");
        pControl->SetAssignedPort(0);
    }

    nOuterDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
    FreeVSMemBlock(*(void **)((char *)this + nOuterDelta + 0x18));
    *(int *)((char *)this + nOuterDelta + 0x18) = 0;

    nSocketDelta = *(int *)(*(int *)((char *)this + 4) + 4);
    pbSocketChannel = (char *)this + nSocketDelta;
    nSocket = *(int *)(pbSocketChannel + 0x0c);
    nSocketOption = 1;
    nResult = setsockopt(nSocket, -1, 0x20, (const char *)&nSocketOption, 4);
    if (nResult == -1) {
        ((VsNetSocketChannelControl *)(pbSocketChannel + 4))->HandleSocketError();
        QueueEffTransportErrorEvent(3);
        return;
    }

    *(unsigned short *)abAddress = 2;
    *(unsigned short *)(abAddress + 2) =
        htons((unsigned short)(*(short *)(*(int *)(*(int *)((char *)this + 4) + 4) +
                                               (int)(unsigned long)this + 0x24) +
                               g_nEffTransportServiceBasePort));
    *(int *)(abAddress + 4) =
        *(int *)((char *)g_pEffTransportRuntimeService + 0x14);
    nResult = bind(nSocket, abAddress, 0x10);
    if (nResult == -1) {
        ((VsNetSocketChannelControl *)(pbSocketChannel + 4))->HandleSocketError();
        QueueEffTransportErrorEvent(4);
        return;
    }

    nOuterDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
    pbTimedSocket = (char *)this + nOuterDelta;
    nResult = WSAAsyncSelect(nSocket, (void *)(unsigned long)*(int *)(pbTimedSocket + 8),
                             0x443, *(int *)((char *)this + 0x14) != 0 ? 3 : 2);
    if (nResult == -1) {
        ((VsNetSocketChannelControl *)(pbSocketChannel + 4))->HandleSocketError();
        QueueEffTransportErrorEvent(5);
        return;
    }

    *(int *)(pbSocketChannel + 0x14) = 1;
    *(int *)(pbSocketChannel + 0x18) = 0;
    *(int *)((char *)this + 0x18) = (int)(timeGetTime() - 1000);
    *(unsigned short *)abEvent = 2;
    *(int *)(abEvent + 8) = 0;
    pDispatchQueue = (VsNetEffDispatchQueue *)g_pEffTransportDispatchQueue;
    pDispatchQueue->QueueEvent(abEvent);
}

// FUNCTION: LEMBALL 0x004715B0
const char *VsNetInetAddressEntry::GetAddressString(void) {
    return m_szAddress;
}

// FUNCTION: LEMBALL 0x00471550
void VsNetInetAddressEntry::StoreAddressString(const char *pszAddress) {
    m_nAddress = (int)inet_addr(pszAddress);
    strcpy(m_szAddress, pszAddress);
}

// FUNCTION: LEMBALL 0x00462DB0
void *DeleteCompositeEffTransportStackWrapper(void *pObject, BYTE fFreeMemory) {
    VsNetCompositeEffTransportStackWrapperView *pView;
    char *pbAllocationBase;

    pView = (VsNetCompositeEffTransportStackWrapperView *)pObject;
    pbAllocationBase = (char *)pObject - 0x30;
    RestoreCompositeEffTransportVtables((int)(unsigned long)pObject);
    ((VsNetDualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((VsNetTimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00471B10
void *VsNetTcpipSocketChannelStackWrapperView::DeleteCompositeTcpipSocketChannelStackWrapper(
    BYTE fFreeMemory) {
    VsNetTcpipSocketChannelStackWrapperView *pView;
    char *pbAllocationBase;
    void *pObject;

    pObject = (void *)this;
    pView = this;
    pbAllocationBase = (char *)this - 0x34;
    RestoreCompositeEffTransportVtables((int)(unsigned long)pObject - 4);
    DestroySocketWindowEffChannel((int)(unsigned long)pView->m_abSocketWindow128);
    ((VsNetDualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((VsNetTimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00471CD0
void *VsNetTcpipSocketChannelStackWrapperView::DeleteTcpipSocketChannelStackWrapper(
    BYTE fFreeMemory) {
    VsNetTcpipSocketChannelStackWrapperView *pView;
    char *pbAllocationBase;
    void *pObject;

    pObject = (void *)this;
    pView = this;
    pbAllocationBase = (char *)this - 8;
    DestroySocketWindowEffChannel((int)(unsigned long)pView->m_abSocketWindow128);
    ((VsNetDualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((VsNetTimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00471DE0
void *VsNetDualSocketWindowChannelStackWrapperView::DeleteDualSocketWindowChannelStackWrapper(
    BYTE fFreeMemory) {
    VsNetDualSocketWindowChannelStackWrapperView *pView;
    char *pbAllocationBase;
    void *pObject;

    pObject = (void *)this;
    pView = this;
    pbAllocationBase = (char *)this - 8;
    DestroySocketWindowEffChannel((int)(unsigned long)pView->m_abSocketWindowA4);
    ((VsNetDualHandleEffStream *)pView->m_abDualStream2c)->DestroyDualHandleEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00471E80
void *VsNetTimedSocketWindowChannelStackWrapperView::DeleteTimedSocketWindowChannelStackWrapper(
    BYTE fFreeMemory) {
    VsNetTimedSocketWindowChannelStackWrapperView *pView;
    char *pbAllocationBase;
    void *pObject;

    pObject = (void *)this;
    pView = this;
    pbAllocationBase = (char *)this - 0x18;
    DestroySocketWindowEffChannel((int)(unsigned long)pView->m_abSocketWindowC8);
    ((VsNetTimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00471F60
void *VsNetTcpipSocketChannelStackWrapperView::DeleteTcpipEffTransportCompositeWrapper(
    BYTE fFreeMemory) {
    VsNetTcpipSocketChannelStackWrapperView *pView;
    char *pbAllocationBase;
    void *pObject;

    pObject = (void *)this;
    pView = this;
    pbAllocationBase = (char *)this - 0x30;
    DestroyTcpipEffTransportComposite((int)(unsigned long)pObject);
    DestroySocketWindowEffChannel((int)(unsigned long)pView->m_abSocketWindow128);
    ((VsNetDualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((VsNetTimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x004715E0
void *AllocateTcpipEffTransportComposite(void) {
    VsNetAllocatedTcpipEffTransportCompositeLayout *pComposite;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    VsNetAdjustorSubobject *pQuaternaryThunk;
    char *pbObjectBase;

    pComposite = (VsNetAllocatedTcpipEffTransportCompositeLayout *)AllocateVSMemBlock(0x170);
    if (pComposite == 0) {
        return 0;
    }

    pbObjectBase = (char *)pComposite;
    pComposite->m_pOuterOffsets04 = (VsNetConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeTransportVtable;
    pComposite->m_pTransportOffsets2c = (VsNetConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeSocketStackVtable;
    pComposite->m_pDualStreamOffsetsA8 = (VsNetConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeDualStreamVtable;
    pComposite->m_pDualThunkOffsets120 = (VsNetConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeDualThunkVtable;
    pComposite->m_pSocketThunkOffsets134 = (VsNetConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeSocketWindowVtable;
    pComposite->m_pTimedSocketBundleOffsets158 =
        (VsNetConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeTimedSocketBundleVtable;
    pComposite->m_pTimedSocketThunkOffsets14c =
        (VsNetConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeTimedSocketThunkVtable;
    pComposite->m_pTimedSocketBundleConstructionData15c = (void **)g_NETWORK_AllocatedTcpipCompositeTimedSocketDataVtable;

    pComposite->m_ChannelState30.ConstructEffStreamChannelState();
    ((VsNetTimedEffStream *)pComposite->m_abTimedStream60)->ConstructTimedEffStream(0);
    ConstructDualHandleEffStream(pComposite->m_abDualStreamD8, 0);

    pOffsets = pComposite->m_pSocketThunkOffsets134;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x134 + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x134 + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x134 + pOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
    pSecondaryThunk->m_pVtable =
        g_NETWORK_RuntimeChannelStackTimedStreamVtable;
    pTertiaryThunk->m_pVtable =
        g_NETWORK_RuntimeChannelStackDualStreamVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;

    ((VsNetSocketWindowEffChannel *)pComposite->m_abSocketWindow13c)
        ->ConstructSocketWindowEffChannel(0);

    pOffsets = pComposite->m_pTimedSocketBundleOffsets158;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x154 + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x154 + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x154 + pOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
    pPrimaryThunk->m_nThisDelta = 0;

    ((VsNetTimedSocketEffChannelBundleLayout *)&pComposite->m_pTimedSocketBundleConstructionData15c)
        ->ConstructTimedSocketEffChannelBundle(0);
    ((VsNetCompositeEffTransportStackLayout *)pComposite)
        ->ConstructCompositeEffTransportStack(0);
    ((VsNetTcpipSocketChannelStackLayout *)((char *)pComposite + 0x2c))
        ->ConstructTcpipSocketChannelStack(0);

    pComposite->m_pVtable00 = (void **)g_NETWORK_AllocatedTcpipCompositeVtable;
    pOffsets = pComposite->m_pOuterOffsets04;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset);
    pQuaternaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nOffset14);
    pPrimaryThunk->m_pVtable = g_NETWORK_AllocatedTcpipCompositeFatalThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_AllocatedTcpipCompositeTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
    pQuaternaryThunk->m_pVtable = g_NETWORK_AllocatedTcpipCompositeSocketThunkVtable;
    pPrimaryThunk->m_nThisDelta = 0;
    pSecondaryThunk->m_nThisDelta = 0;
    pTertiaryThunk->m_nThisDelta = 0;
    pQuaternaryThunk->m_nThisDelta = 0;
    return pComposite;
}

// FUNCTION: LEMBALL 0x00471810
void *AllocateTcpipEffTransportRuntimeOwner(void) {
    VsNetTcpipEffTransportCompositeLayout *pComposite;

    pComposite = (VsNetTcpipEffTransportCompositeLayout *)AllocateVSMemBlock(0x16c);
    if (pComposite == 0) {
        return 0;
    }
    return pComposite->ConstructTcpipEffTransportComposite(1);
}

// FUNCTION: LEMBALL 0x00471830
void *VsNetTcpipSocketChannelStackLayout::ConstructTcpipSocketChannelStack(
    int fConstructEmbeddedObjects) {
    VsNetTcpipSocketChannelStackLayout *pStack;
    VsNetConstructionAdjustorVtable *pOffsets;
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    VsNetAdjustorSubobject *pQuaternaryThunk;
    char *pbObjectBase;

    pStack = this;
    pbObjectBase = (char *)this;

    if (fConstructEmbeddedObjects != 0) {
        pStack->m_pTimedStreamConstructionOffsets7c = (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackTimedStreamVtable;
        pStack->m_pDualStreamConstructionOffsetsf4 = (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackDualStreamVtable;
        pStack->m_pBaseOffsets00 = (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackBaseVtable;
        pStack->m_pDualThunkOffsets108 = (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackDualThunkVtable;
        pStack->m_pTimedSocketBundleConstructionData130 = (void **)g_NETWORK_TcpipSocketStackTimedSocketDataVtable;
        pStack->m_pTimedSocketBundleConstructionOffsets120 =
            (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackTimedSocketBundleVtable;
        pStack->m_pSocketWindowConstructionOffsets12c = (VsNetConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackSocketWindowVtable;

        pStack->m_ChannelState08.ConstructEffStreamChannelState();
        ((VsNetTimedEffStream *)pStack->m_abTimedStream38)->ConstructTimedEffStream(0);
        ConstructDualHandleEffStream(pStack->m_abDualStreamB0, 0);

        pOffsets = pStack->m_pDualThunkOffsets108;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x104 + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x104 + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x104 + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
        pSecondaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackTimedStreamVtable;
        pTertiaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackDualStreamVtable;
        pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
        pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
        pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;

        ((VsNetSocketWindowEffChannel *)pStack->m_abSocketWindow110)
            ->ConstructSocketWindowEffChannel(0);

        pOffsets = pStack->m_pSocketWindowConstructionOffsets12c;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x128 + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x128 + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x128 + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
        pPrimaryThunk->m_nThisDelta = 0;

        ((VsNetTimedSocketEffChannelBundleLayout *)&pStack->m_pTimedSocketBundleConstructionData130)
            ->ConstructTimedSocketEffChannelBundle(0);
    }

    pOffsets = pStack->m_pBaseOffsets00;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset - 4);
    pQuaternaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pOffsets->m_nOffset14 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackRuntimeRecoveredVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
    pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackSocketThunkVtable;
    pPrimaryThunk->m_nThisDelta = 0;
    pSecondaryThunk->m_nThisDelta = 0;
    pTertiaryThunk->m_nThisDelta = 0;
    pQuaternaryThunk->m_nThisDelta = 0;
    return this;
}

// FUNCTION: LEMBALL 0x00470B90
int VsNetTcpipClientAsyncDispatcher::DispatchTcpipClientAsyncMessage(
    int nMessage, void *pReserved, unsigned int nEvent) {
    char *pbThis;
    int nDelta;
    int nResult;
    VsNetSocketAsyncErrorAdapter *pErrorAdapter;
    VsNetHostLookupCompletionAdapter *pLookupAdapter;
    VsNetSocketAsyncMessageAdapter *pReceiveAdapter;
    VsNetTimedSocketAsyncMessageAdapter *pSocketDispatcher;

    pbThis = (char *)this;
    switch (nMessage) {
    case 0x440:
        nDelta = *(int *)(*(int *)(pbThis - 0x134) + 0x14);
        pErrorAdapter = (VsNetSocketAsyncErrorAdapter *)(pbThis + nDelta - 0x134);
        nResult = pErrorAdapter->HandleSocketAsyncErrorLparam(
            pReserved, nEvent, (unsigned int *)(pbThis + nDelta - 0x120));
        if (nResult != 0x0e) {
            pLookupAdapter = (VsNetHostLookupCompletionAdapter *)(pbThis - 0x138);
            pLookupAdapter->HandleLocalHostLookupComplete(nResult == 2);
        }
        return 0;

    case 0x441:
        *(int *)(pbThis - 0x110) = 0;
        nDelta = *(int *)(*(int *)(pbThis - 0x134) + 0x14);
        pErrorAdapter = (VsNetSocketAsyncErrorAdapter *)(pbThis + nDelta - 0x134);
        nResult = pErrorAdapter->HandleSocketAsyncErrorLparam(
            pReserved, nEvent, (unsigned int *)(pbThis - 0x114));
        if (nResult != 0x0e) {
            pLookupAdapter = (VsNetHostLookupCompletionAdapter *)(pbThis - 0x138);
            pLookupAdapter->HandleRemoteHostLookupComplete(nResult == 2);
        }
        return 0;

    case 0x442:
        nDelta = *(int *)(*(int *)(pbThis - 0x134) + 0x14);
        pErrorAdapter = (VsNetSocketAsyncErrorAdapter *)(pbThis + nDelta - 0x134);
        nResult = pErrorAdapter->HandleSocketAsyncErrorLparam(
            pReserved, nEvent, (unsigned int *)(pbThis + nDelta - 0x120));
        if (nResult != 0x0e) {
            pLookupAdapter = (VsNetHostLookupCompletionAdapter *)(pbThis - 0x138);
            pLookupAdapter->HandleServicePortLookupComplete(nResult == 2);
        }
        return 0;

    case 0x443:
        nDelta = *(int *)(*(int *)(pbThis - 0x134) + 4);
        if (*(int *)(pbThis + nDelta - 0x12c) == -1) {
            return 0;
        }
        ((VsNetEffStreamChannelState *)(pbThis + nDelta - 0x134))->SetEffStreamChannelAsyncErrorStatus((int)(nEvent >> 16));
        if ((nEvent & 0xffff) == 1) {
            if ((nEvent >> 16) == 0) {
                nDelta = *(int *)(*(int *)(pbThis - 0x134) + 0x18);
                pReceiveAdapter = (VsNetSocketAsyncMessageAdapter *)(pbThis + nDelta - 0x134);
                pReceiveAdapter->ReceiveUdpPayloadFromSenderAddress();
            }
            return 0;
        }
        break;
    }

    pSocketDispatcher = (VsNetTimedSocketAsyncMessageAdapter *)(pbThis - 8);
    return pSocketDispatcher->DispatchEffSocketAsyncSelectMessage(nMessage, pReserved, nEvent);
}

// FUNCTION: LEMBALL 0x00471210
void VsNetUdpSocketBinder::BindUdpSocketAndEnableAsyncSelect(void) {
    unsigned char abAddress[16];
    int nPrimaryDelta;
    int nTimedDelta;
    int nSocket;
    int nResult;
    VsNetSocketChannelControl *pErrorChannel;

    PrepareBind();
    nPrimaryDelta = *(int *)(*(int *)((char *)this + 4) + 4);
    *(unsigned short *)abAddress = 2;
    *(unsigned short *)(abAddress + 2) =
        htons((unsigned short)(*(short *)((char *)this + nPrimaryDelta + 0x24) +
                               g_nEffTransportServiceBasePort));
    *(int *)(abAddress + 4) =
        *(int *)((char *)g_pEffTransportRuntimeService + 0x14);
    nSocket = *(int *)((char *)this + nPrimaryDelta + 0x0c);
    nResult = bind(nSocket, abAddress, 0x10);
    if (nResult == -1) {
        pErrorChannel = (VsNetSocketChannelControl *)((char *)this + nPrimaryDelta + 4);
        pErrorChannel->HandleSocketError();
        return;
    }

    nTimedDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
    nResult = WSAAsyncSelect(nSocket, (void *)(unsigned long)*(int *)((char *)this + nTimedDelta + 8),
                             0x443, 3);
    if (nResult == -1) {
        pErrorChannel = (VsNetSocketChannelControl *)((char *)this + nPrimaryDelta + 4);
        pErrorChannel->HandleSocketError();
        return;
    }

    *(int *)((char *)this + nPrimaryDelta + 0x20) = 1;
    *(int *)((char *)this + 0x1c) = 1;
    *(int *)((char *)this +
             *(int *)(*(int *)((char *)this + 4) + 0x0c) + 0x40) =
        (int)timeGetTime();
}

// FUNCTION: LEMBALL 0x004712E0
int VsNetTcpipHostPortAsyncDispatcher::DispatchTcpipHostPortAsyncMessage(
    int nMessage, void *pReserved, unsigned int nEvent) {
    char *pbThis;
    int nDelta;
    int nResult;
    VsNetSocketAsyncErrorAdapter *pErrorAdapter;
    VsNetHostLookupCompletionAdapter *pLookupAdapter;
    VsNetTimedSocketAsyncMessageAdapter *pSocketDispatcher;

    pbThis = (char *)this;
    if (*(int *)(pbThis - 0x11c) != 0) {
        return -1;
    }
    if (nMessage != 0x440 && nMessage != 0x442) {
        pSocketDispatcher = (VsNetTimedSocketAsyncMessageAdapter *)this;
        return pSocketDispatcher->DispatchEffSocketAsyncSelectMessage(nMessage, pReserved, nEvent);
    } else {
        nDelta = *(int *)(*(int *)(pbThis - 0x138) + 0x14);
        pErrorAdapter = (VsNetSocketAsyncErrorAdapter *)(pbThis + nDelta - 0x138);
        nResult = pErrorAdapter->HandleSocketAsyncErrorLparam(
            pReserved, nEvent, (unsigned int *)(pbThis + nDelta - 0x124));
        if (nResult != 0x0e) {
            pLookupAdapter = (VsNetHostLookupCompletionAdapter *)(pbThis - 0x13c);
            if (nMessage == 0x440) {
                pLookupAdapter->HandleRemoteServiceHostLookupComplete(nResult == 2);
            } else {
                pLookupAdapter->HandleRemoteServicePortLookupComplete(nResult == 2);
            }
        }
        return 0;
    }
}

// FUNCTION: LEMBALL 0x00471A60
void VsNetSocketWindowChannelWrapper::CloseSocketHandleFromChannel(void) {
    int nDelta;

    nDelta = *(int *)(*(int *)((char *)this - 0x10) + 4);
    closesocket(*(int *)((char *)this + nDelta - 8));
}

// FUNCTION: LEMBALL 0x00471A90
void *VsNetSocketWindowChannelWrapper::DeleteSocketWindowChannelStateWrapper(BYTE fFreeMemory) {
    void *pAllocationBase;

    DestroySocketWindowEffChannel((int)(unsigned long)this);
    ((VsNetEffStreamChannelState *)this)->DestroyEffStreamChannelState();
    pAllocationBase = (char *)this - 0x20;
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pAllocationBase);
    }
    return pAllocationBase;
}

// FUNCTION: LEMBALL 0x00471AD0
void VsNetSocketWindowChannelWrapper::CaptureLastWinsockErrorStatus(void) {
    int nDelta;

    nDelta = *(int *)(*(int *)((char *)this - 0x10) + 4);
    ((VsNetEffStreamChannelState *)((char *)this + nDelta - 0x10))->SetEffStreamChannelAsyncErrorStatus(WSAGetLastError());
}

// FUNCTION: LEMBALL 0x00470ED0
void VsNetHostLookupCompletionAdapter::HandleRemoteServiceHostLookupComplete(int nError) {
    char abAddress[20];
    VsNetInetAddressEntry localAddress;
    VsNetSocketChannelControl *pSocketChannel;
    VsNetSocketChannelControl *pErrorChannel;
    VsNetTcpipLookupOwner *pOwner;
    char *pbLookup;
    char *pszAddress;
    int nTimedDelta;
    int nSocketDelta;
    int nPort;
    int nLookupResult;
    short cbAddress;

    if (nError != 0) {
        AppendCStringToStream(g_pErrorOutputStream, "Computer specified was not found\n");
        return;
    }

    nTimedDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
    pbLookup = *(char **)((char *)this + nTimedDelta + 0x18);
    cbAddress = *(short *)(pbLookup + 0x0a);
    memcpy(abAddress, *(void **)(*(int *)(pbLookup + 0x0c)), cbAddress);
    *(void ***)&localAddress = (void **)g_NETWORK_InetAddressEntryVtable;
    localAddress.m_szAddress[0] = '\0';
    localAddress.m_nAddress = *(int *)abAddress;
    pszAddress = (char *)inet_ntoa((unsigned long)localAddress.m_nAddress);
    strcpy(localAddress.m_szAddress, pszAddress);

    nSocketDelta = *(int *)(*(int *)((char *)this + 4) + 8);
    pSocketChannel = (VsNetSocketChannelControl *)((char *)this + nSocketDelta + 4);
    pSocketChannel->SetRemoteAddress(&localAddress);

    nPort = atoi(*(char **)((char *)this + 0x0c));
    *(short *)((char *)this +
               *(int *)(*(int *)((char *)this + 4) + 4) + 0x24) = (short)nPort;
    pOwner = (VsNetTcpipLookupOwner *)this;
    if (*(short *)((char *)this +
                   *(int *)(*(int *)((char *)this + 4) + 4) + 0x24) != 0) {
        pOwner->BeginPortSetup();
        return;
    }

    *(int *)((char *)this + *(int *)(*(int *)((char *)this + 4) + 4) + 0x18) = 1;
    nTimedDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
    nLookupResult = WSAAsyncGetServByNameWithLength(
        *(int *)((char *)this + nTimedDelta + 8), 0x442,
        *(const char **)((char *)this + 0x0c), "TCP",
        (void *)(unsigned long)*(int *)((char *)this + nTimedDelta + 0x18), 0x400);
    *(int *)((char *)this + nTimedDelta + 0x1c) = nLookupResult;
    if (nLookupResult == 0) {
        nSocketDelta = *(int *)(*(int *)((char *)this + 4) + 4);
        pErrorChannel = (VsNetSocketChannelControl *)((char *)this + nSocketDelta + 4);
        pErrorChannel->HandleSocketError();
    }
}

// FUNCTION: LEMBALL 0x00471000
void VsNetHostLookupCompletionAdapter::HandleRemoteServicePortLookupComplete(int nError) {
    int nOffsets;
    int nTimedDelta;
    int nPort;
    char *pbLookup;
    VsNetSocketChannelControl *pControl;

    nOffsets = *(int *)((char *)this + 4);
    pControl = (VsNetSocketChannelControl *)((char *)this + *(int *)(nOffsets + 8) + 4);
    if (nError == 0) {
        nTimedDelta = *(int *)(nOffsets + 0x14);
        pbLookup = *(char **)((char *)this + nTimedDelta + 0x18);
        nPort = (int)(unsigned short)ntohs(*(unsigned short *)(pbLookup + 8));
        pControl->SetAssignedPort((short)(nPort - g_nEffTransportServiceBasePort));
    } else {
        pControl->SetAssignedPort(0);
        AppendCStringToStream(g_pErrorOutputStream, "Service port number specified was not found\n");
    }

    nTimedDelta = *(int *)(nOffsets + 0x14);
    FreeVSMemBlock(*(void **)((char *)this + nTimedDelta + 0x18));
    *(int *)((char *)this + nTimedDelta + 0x18) = 0;
    if (nError == 0) {
        ((VsNetTcpipLookupOwner *)this)->BeginPortSetup();
    }
}
