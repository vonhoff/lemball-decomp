#include "../game/game_app.h"
#include "../engine/memory_arena.h"
#include "../engine/runtime_init.h"
#include "../platform/message_window.h"
#include "network/stream.h"
#include "network/runtime.h"
#include "network/safe_vtable.h"
#include <string.h>
#include <stdlib.h>

#ifdef ntohs
#undef ntohs
#endif
#ifdef htons
#undef htons
#endif

extern int ReturnTrueVtableCallback(void);
extern void CrtFatalRuntimeError0x19(void);

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

struct NETWORK_QuadThunkAdjustorGroup {
    NETWORK_ConstructionAdjustorVtable *m_pOffsets;
    NETWORK_AdjustorSubobject *m_pPrimary;
    NETWORK_AdjustorSubobject *m_pSecondary;
    NETWORK_AdjustorSubobject *m_pTertiary;
    NETWORK_AdjustorSubobject *m_pQuaternary;
};

struct NETWORK_SocketWindowEffChannel : PLATFORM_InvisibleMessageWindow {
    void **m_pEmbeddedStateConstructionOffsets;
    int m_nSocketHandle;
    int m_nLookupHandle;
    unsigned char m_abChannelState[0x30];

    void *ConstructSocketWindowEffChannel(int fConstructEmbeddedState);
};

struct NETWORK_TimedSocketEffChannelBundleHeader {
    void **m_pVtable;
    short m_nMode;
};

struct NETWORK_TimedSocketAsyncLookupAdapter {
    void CaptureAsyncLookupAddressForTimedStream(void *pLookupResult);
    int SendUdpPayloadToSocketAddress(void *pBuffer, int cbBuffer);
};

struct NETWORK_TimedSocketAsyncMessageAdapter {
    int HandleTimedSocketConnectAsyncMessage(int nMessage, void *pReserved, unsigned int nEvent);
    void DispatchEffSocketAsyncSelectMessage(int nMessage, void *pReserved, unsigned int nEvent);
};

struct NETWORK_SocketAsyncMessageAdapter {
    int ReceiveUdpPayloadFromConnectedSocket(void);
    int ReceiveUdpPayloadFromSenderAddress(void);
    int HandleSocketReceiveAsyncSelectMessage(int nMessage, void *pReserved, unsigned int nEvent);
};

struct NETWORK_SocketAsyncErrorAdapter {
    int HandleSocketAsyncErrorLparam(void *pReserved, unsigned int nEvent, unsigned int *pBuffer);
};

struct NETWORK_TcpipClientAsyncDispatcher {
    int DispatchTcpipClientAsyncMessage(int nMessage, void *pReserved, unsigned int nEvent);
};

struct NETWORK_UdpSocketBinder {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void PrepareBind(void) = 0;
    void BindUdpSocketAndEnableAsyncSelect(void);
};

struct NETWORK_TcpipHostPortAsyncDispatcher {
    int DispatchTcpipHostPortAsyncMessage(int nMessage, void *pReserved, unsigned int nEvent);
};

struct NETWORK_SocketWindowChannelWrapper {
    void CloseSocketHandleFromChannel(void);
    void *DeleteSocketWindowChannelStateWrapper(BYTE fFreeMemory);
    void CaptureLastWinsockErrorStatus(void);
};
void *DeleteDualSocketWindowChannelStackWrapper(void *pObject, BYTE fFreeMemory);

struct NETWORK_HostLookupCompletionAdapter {
    void HandleRemoteHostLookupComplete(int nError);
    void HandleLocalHostLookupComplete(int nError);
    void HandleServicePortLookupComplete(int nError);
    void HandleRemoteServiceHostLookupComplete(int nError);
    void HandleRemoteServicePortLookupComplete(int nError);
};

struct NETWORK_InetAddressEntry {
    virtual const char *GetAddressString(void);
    virtual void *Delete(BYTE fDelete) {
        (void)fDelete;
        return this;
    }
    virtual void StoreAddressString(const char *pszAddress);
    char m_szAddress[16];
    int m_nAddress;
};

struct NETWORK_EffDispatchQueue {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void QueueEvent(void *pEvent) = 0;
};

struct NETWORK_TimedSocketErrorChannel {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void HandleSocketError(void) = 0;
};

struct NETWORK_SocketChannelControl {
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

struct NETWORK_TcpipLookupOwner {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0c(void) = 0;
    virtual void BeginPortSetup(void) = 0;
};

struct NETWORK_TimedSocketPacketChannel {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved0c(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void NotifyPacketReady(void) = 0;
};

struct NETWORK_TimedSocketEffChannelBundleLayout {
    void **m_pVtable00;
    short m_nMode04;
    unsigned char m_abReserved06[0x12];
    unsigned char m_abChannelState18[0x30];
    unsigned char m_abTimedStream48[0x44];
    void **m_pTimedStreamConstructionOffsets8c;
    unsigned char m_abReserved90[0x40];
    void **m_pSocketChannelConstructionOffsetsd0;
    unsigned char m_abReservedd4[0x0c];
    unsigned char m_abSocketWindowC0[0x20];

    void *ConstructTimedSocketEffChannelBundle(int fConstructEmbeddedObjects);
};

struct NETWORK_TcpipEffTransportCompositeLayout {
    void **m_pVtable00;
    NETWORK_ConstructionAdjustorVtable *m_pOuterOffsets04;
    int m_nRuntimeField08;
    int m_nRuntimeField0c;
    int m_nRuntimeField10;
    int m_nRuntimeField14;
    int m_nRuntimeField18;
    int m_nRuntimeField1c;
    NETWORK_ConstructionAdjustorVtable *m_pTransportOffsets20;
    int m_nSocketWindowHandle24;
    int m_nLookupHandle28;
    int m_nReserved2c;
    unsigned char m_abChannelState30[0x30];
    unsigned char m_abTimedStream60[0x44];
    NETWORK_ConstructionAdjustorVtable *m_pDualStreamConstructionOffsetsA4;
    unsigned char m_abTimedStreamA8[0x30];
    unsigned char m_abDualStreamD8[0x44];
    NETWORK_ConstructionAdjustorVtable *m_pDualThunkOffsets11c;
    unsigned char m_abReserved120[0x10];
    NETWORK_ConstructionAdjustorVtable *m_pSocketThunkOffsets130;
    unsigned char m_abReserved134[0x04];
    unsigned char m_abSocketWindow138[0x10];
    NETWORK_ConstructionAdjustorVtable *m_pTimedSocketThunkOffsets148;
    unsigned char m_abSocketWindow14c[0x08];
    NETWORK_ConstructionAdjustorVtable *m_pTimedSocketBundleOffsets154;
    void **m_pTimedSocketBundleConstructionData158;
    unsigned char m_abTimedSocketBundle15c[0x14];

    void *ConstructTcpipEffTransportComposite(int fConstructEmbeddedObjects);
};

/* The allocator is a multiple-subobject layout.  Its metadata is not at the
 * offsets used by the standalone 00470270 composite. */
struct NETWORK_AllocatedTcpipEffTransportCompositeLayout {
    void **m_pVtable00;
    NETWORK_ConstructionAdjustorVtable *m_pOuterOffsets04;
    unsigned char m_abReserved08[0x24];
    NETWORK_ConstructionAdjustorVtable *m_pTransportOffsets2c;
    unsigned char m_abChannelState30[0x30];
    unsigned char m_abTimedStream60[0x44];
    unsigned char m_abReservedA4[4];
    NETWORK_ConstructionAdjustorVtable *m_pDualStreamOffsetsA8;
    unsigned char m_abReservedAC[0x2c];
    unsigned char m_abDualStreamD8[0x44];
    unsigned char m_abReserved11c[4];
    NETWORK_ConstructionAdjustorVtable *m_pDualThunkOffsets120;
    unsigned char m_abReserved124[0x10];
    NETWORK_ConstructionAdjustorVtable *m_pSocketThunkOffsets134;
    unsigned char m_abReserved138[4];
    unsigned char m_abSocketWindow13c[0x10];
    NETWORK_ConstructionAdjustorVtable *m_pTimedSocketThunkOffsets14c;
    unsigned char m_abReserved150[8];
    NETWORK_ConstructionAdjustorVtable *m_pTimedSocketBundleOffsets158;
    void **m_pTimedSocketBundleConstructionData15c;
};

struct NETWORK_TcpipSocketChannelStackLayout {
    NETWORK_ConstructionAdjustorVtable *m_pBaseOffsets00;
    int m_nReserved04;
    unsigned char m_abChannelState08[0x30];
    unsigned char m_abTimedStream38[0x44];
    NETWORK_ConstructionAdjustorVtable *m_pTimedStreamConstructionOffsets7c;
    unsigned char m_abTimedStream80[0x30];
    unsigned char m_abDualStreamB0[0x44];
    NETWORK_ConstructionAdjustorVtable *m_pDualStreamConstructionOffsetsf4;
    unsigned char m_abReservedf8[0x10];
    NETWORK_ConstructionAdjustorVtable *m_pDualThunkOffsets108;
    unsigned char m_abReserved10c[0x04];
    unsigned char m_abSocketWindow110[0x10];
    NETWORK_ConstructionAdjustorVtable *m_pTimedSocketBundleConstructionOffsets120;
    unsigned char m_abSocketWindow124[0x08];
    NETWORK_ConstructionAdjustorVtable *m_pSocketWindowConstructionOffsets12c;
    void **m_pTimedSocketBundleConstructionData130;
    unsigned char m_abTimedSocketBundle134[0x14];

    void *ConstructTcpipSocketChannelStack(int fConstructEmbeddedObjects);
};

struct NETWORK_CompositeEffTransportStackLayout {
    void **m_pVtable00;
    NETWORK_ConstructionAdjustorVtable *m_pChannelStateOffsets04;
    int m_nField08;
    int m_nField0c;
    int m_nField10;
    int m_nField14;
    int m_nField18;
    int m_nField1c;
    int m_nField20;
    DWORD m_dwLastActivity24;
    unsigned char m_abChannelState30[0x30];
    unsigned char m_abTimedStream60[0x44];
    NETWORK_ConstructionAdjustorVtable *m_pTimedStreamOffsetsA4;
    unsigned char m_abReservedA8[0x74];
    NETWORK_ConstructionAdjustorVtable *m_pDualThunkOffsets11c;
    unsigned char m_abReserved120[0x14];

    void *ConstructCompositeEffTransportStack(int fConstructEmbeddedObjects);
};

struct NETWORK_CompositeEffTransportStackWrapperView {
    unsigned char m_abChannelState00[0x30];
    unsigned char m_abTimedStream30[0x78];
    unsigned char m_abDualStreamA8[0x44];
};

struct NETWORK_TcpipSocketChannelStackWrapperView {
    unsigned char m_abChannelState00[0x30];
    unsigned char m_abTimedStream30[0x78];
    unsigned char m_abDualStreamA8[0x44];
    unsigned char m_abReservedec[0x3c];
    unsigned char m_abSocketWindow128[0x20];
};

struct NETWORK_EffTransportConnectCallback {
    void QueueEffTransportConnectEvent(void);
};

struct NETWORK_DualSocketWindowChannelStackWrapperView {
    unsigned char m_abChannelState00[0x2c];
    unsigned char m_abDualStream2c[0x78];
    unsigned char m_abSocketWindowA4[0x20];
};

struct NETWORK_TimedSocketWindowChannelStackWrapperView {
    unsigned char m_abChannelState00[0x30];
    unsigned char m_abTimedStream30[0x98];
    unsigned char m_abSocketWindowC8[0x20];
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
static void *g_NETWORK_CompositeEffTransportVtable = NetworkGetSafeVtable();
static void *g_NETWORK_CompositeEffTransportFatalThunkVtable = NetworkGetSafeVtable();
static void *g_NETWORK_CompositeEffTransportTimedThunkVtable = NetworkGetSafeVtable();
static void *g_NETWORK_CompositeEffTransportDualThunk = NetworkGetSafeVtable();

static void *g_NETWORK_SocketWindowEffChannelEmbeddedStateVtable = NetworkGetSafeVtable();
static void *g_NETWORK_SocketWindowEffChannelVtable = NetworkGetSafeVtable();
static void *g_NETWORK_SocketWindowEffChannelThunkVtable = NetworkGetSafeVtable();

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
void *DispatchAckedEffTransportPayloadEvent(void *pObject);
// FUNCTION: LEMBALL 0x00471E60
static void TimedSocketBundleCloseSocketThunk(void *pObject) {
    ((NETWORK_SocketWindowChannelWrapper *)((char *)pObject + 0xc8))
        ->CloseSocketHandleFromChannel();
}

// FUNCTION: LEMBALL 0x00471E70
static void *TimedSocketBundleDeleteThunk(void *pObject, BYTE fDelete) {
    char *pAdjustedObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return DeleteTimedSocketWindowChannelStackWrapper(pAdjustedObject, fDelete);
}

// FUNCTION: LEMBALL 0x00471EC0
static void TimedSocketBundleCaptureErrorThunk(void *pObject) {
    ((NETWORK_SocketWindowChannelWrapper *)((char *)pObject + 0xc8))
        ->CaptureLastWinsockErrorStatus();
}

// FUNCTION: LEMBALL 0x00471EE0
static void TimedSocketBundleClearPendingWriteBody(void *pObject, void *pUnused) {
    int nPendingDelta = *(int *)(*(int *)((char *)pObject - 0x18) + 8);
    ((NETWORK_EffTransportPendingWriteState *)((char *)pObject + nPendingDelta + 0x60))
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
    return DeleteTimedSocketWindowChannelStackWrapper(pAdjustedObject, fDelete);
}

// FUNCTION: LEMBALL 0x00471F10
static void TimedSocketBundleCaptureLookupAdjustedThunk(void *pObject, void *pLookupResult) {
    NETWORK_TimedSocketAsyncLookupAdapter *pAdapter =
        (NETWORK_TimedSocketAsyncLookupAdapter *)((char *)pObject - *(int *)((char *)pObject - 4));
    pAdapter->CaptureAsyncLookupAddressForTimedStream(pLookupResult);
}

// FUNCTION: LEMBALL 0x00471F20
static int TimedSocketBundleSendUdpAdjustedThunk(void *pObject, void *pBuffer, int cbBuffer) {
    NETWORK_TimedSocketAsyncLookupAdapter *pAdapter =
        (NETWORK_TimedSocketAsyncLookupAdapter *)((char *)pObject - *(int *)((char *)pObject - 4));
    return pAdapter->SendUdpPayloadToSocketAddress(pBuffer, cbBuffer);
}

// FUNCTION: LEMBALL 0x00471F40
static int TimedSocketBundleConnectAdjustedThunk(void *pObject, int nMessage,
                                                  void *pReserved, unsigned int nEvent) {
    NETWORK_TimedSocketAsyncMessageAdapter *pAdapter =
        (NETWORK_TimedSocketAsyncMessageAdapter *)((char *)pObject - *(int *)((char *)pObject - 4));
    return pAdapter->HandleTimedSocketConnectAsyncMessage(nMessage, pReserved, nEvent);
}

// FUNCTION: LEMBALL 0x00471F50
static void *TimedSocketBundleCompositeDeleteAdjustedThunk(void *pObject, BYTE fDelete) {
    char *pAdjustedObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return DeleteTcpipEffTransportCompositeWrapper(pAdjustedObject, fDelete);
}

// FUNCTION: LEMBALL 0x00471DB0
static void TimedSocketBundleDispatchAsyncThunk(void *pObject, int nMessage,
                                                 void *pReserved, unsigned int nEvent) {
    NETWORK_TimedSocketAsyncMessageAdapter *pAdapter;

    pAdapter = (NETWORK_TimedSocketAsyncMessageAdapter *)
        ((char *)pObject - *(int *)((char *)pObject - 4));
    pAdapter->DispatchEffSocketAsyncSelectMessage(nMessage, pReserved, nEvent);
}

// FUNCTION: LEMBALL 0x00471DC0
static void TimedSocketBundlePrimaryCloseThunk(void *pObject) {
    ((NETWORK_SocketWindowChannelWrapper *)((char *)pObject + 0xa4))
        ->CloseSocketHandleFromChannel();
}

// FUNCTION: LEMBALL 0x00471DD0
static void *TimedSocketBundlePrimaryDeleteThunk(void *pObject, BYTE fDelete) {
    char *pAdjustedObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return DeleteDualSocketWindowChannelStackWrapper(pAdjustedObject, fDelete);
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
    ((NETWORK_SocketWindowChannelWrapper *)((char *)pObject + 0xa4))
        ->CaptureLastWinsockErrorStatus();
}

static void *g_NETWORK_TimedSocketBundleChannelThunkVtable[4] = {
    (void *)TimedSocketBundleCloseSocketThunk,
    (void *)TimedSocketBundleDeleteThunk,
    (void *)TimedSocketBundleCaptureErrorThunk,
    (void *)TimedSocketBundleClearPendingWriteThunk,
};
extern void *ClaimAckedEffTransportRecordPayload(void *pObject);
static void SetTcpipSocketChannelAssignedPort(void *pObject, short nPort);
// FUNCTION: LEMBALL 0x00471F30
static void TimedSocketBundleSetAssignedPortThunk(void *pObject, short nPort) {
    SetTcpipSocketChannelAssignedPort((char *)pObject - *(int *)((char *)pObject - 4), nPort);
}

/* Raw table 0049a090: stream base slots, timed payload slots, then socket
 * adapter slots.  Compiler-owned storage keeps every dispatch target valid. */
struct NETWORK_TimedSocketBundleTimedThunkVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        ((NETWORK_PeerPayloadSender *)this)->WriteEffStreamWithGlobalSession();
    }
    virtual void *ClaimRecord(void) {
        return ClaimAckedEffTransportRecordPayload(this);
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

static NETWORK_TimedSocketBundleTimedThunkVtableModel g_NETWORK_TimedSocketBundleTimedThunkVtableModel;
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
static int g_NETWORK_TcpipCompositeTimedStreamVtable[5] = {
    -4, 0x2c, 0x5c, 0xd4, 0x12c,
};
static int g_NETWORK_TcpipCompositeTransportStackVtable[5] = {
    0, 0x10, 0x40, 0xb8, 0x110,
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
static int g_NETWORK_TcpipCompositeTimedSocketThunkVtable[4] = {
    -0x10, -0x118, 0, -0x100,
};
static int g_NETWORK_TcpipCompositeTimedSocketBundleVtable[4] = {
    0, -0x124, -0x7c, -0x1c,
};
static int g_NETWORK_TcpipCompositeTimedSocketBundleDataVtable[4] = {
    0, -0x128, -0xf8, -0x120,
};
static void *g_NETWORK_TcpipCompositeTimedThunkVtable = NetworkGetSafeVtable();
static void *g_NETWORK_TcpipCompositeDualThunk = NetworkGetSafeVtable();
static void *g_NETWORK_TcpipCompositeSocketThunkVtable = NetworkGetSafeVtable();
static void *g_NETWORK_TcpipCompositeVtable = NetworkGetSafeVtable();
static void *g_NETWORK_TcpipCompositeOuterFatalThunkVtable = NetworkGetSafeVtable();
static void *g_NETWORK_TcpipCompositeOuterTimedThunkVtable = NetworkGetSafeVtable();
static void *g_NETWORK_TcpipCompositeOuterDualThunkVtable = NetworkGetSafeVtable();
static void *g_NETWORK_TcpipCompositeOuterSocketThunkVtable = NetworkGetSafeVtable();
static void *g_NETWORK_TimedSocketBundlePrimaryThunkVtable[5] = {
    (void *)TimedSocketBundlePrimaryCloseThunk,
    (void *)TimedSocketBundlePrimaryDeleteThunk,
    (void *)TimedSocketBundlePrimaryCaptureErrorThunk,
    (void *)TimedSocketBundlePrimaryNoopThunk,
    (void *)TimedSocketBundleDispatchAsyncThunk,
};
struct NETWORK_CompositeTimedSocketSecondaryThunkVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        return DeleteDualSocketWindowChannelStackWrapper((char *)this - 0x2c, fDelete);
    }
    virtual void Ret(void) {}
    virtual void Fatal0(void) { CrtFatalRuntimeError0x19(); }
    virtual void Fatal1(void) { CrtFatalRuntimeError0x19(); }
    virtual void Ret8(BYTE fDelete, BYTE fReserved) {
        (void)fDelete;
        (void)fReserved;
    }
    virtual void CloseSocket(void) {
        ((NETWORK_SocketWindowChannelWrapper *)((char *)this + 0xa4))
            ->CloseSocketHandleFromChannel();
    }
};
static NETWORK_CompositeTimedSocketSecondaryThunkVtableModel g_NETWORK_CompositeTimedSocketSecondaryThunkVtableModel;
static void *g_NETWORK_TimedSocketBundleSecondaryThunkVtable =
    *(void ***)&g_NETWORK_CompositeTimedSocketSecondaryThunkVtableModel;

/* Raw table 0049a1e4: receive adapter, shared stream prefix, dual-channel
 * methods, then socket-window methods. */
struct NETWORK_TimedSocketBundleTertiaryThunkVtableModel {
    virtual int HandleReceive(int nMessage, void *pReserved, unsigned int nEvent) {
        return ((NETWORK_SocketAsyncMessageAdapter *)this)
            ->HandleSocketReceiveAsyncSelectMessage(nMessage, pReserved, nEvent);
    }
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        return DeleteDualSocketWindowChannelStackWrapper((char *)this - 0x2c, fDelete);
    }
    virtual void Ret(void) {}
    virtual void Fatal0(void) { CrtFatalRuntimeError0x19(); }
    virtual void Fatal1(void) { CrtFatalRuntimeError0x19(); }
    virtual void Ret8(BYTE fDelete, BYTE fReserved) {
        (void)fDelete;
        (void)fReserved;
    }
    virtual void CloseSocket(void) {
        ((NETWORK_SocketWindowChannelWrapper *)((char *)this + 0xa4))
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
static NETWORK_TimedSocketBundleTertiaryThunkVtableModel
    g_NETWORK_TimedSocketBundleTertiaryThunkVtableModel;
static void *g_NETWORK_TimedSocketBundleTertiaryThunkVtable =
    *(void ***)&g_NETWORK_TimedSocketBundleTertiaryThunkVtableModel;
static void *g_NETWORK_ReturnTrueVtable[1] = {
    (void *)ReturnTrueVtableCallback,
};
/* Temporary compiler-owned thunk storage.  The original target is the
 * runtime-stack fatal adjustor; never leave this dispatch pointer null. */
struct NETWORK_SocketRuntimeFatalThunkVtableModel {
    virtual void Fatal(void) { CrtFatalRuntimeError0x19(); }
    virtual void *Delete(BYTE fDelete) {
        (void)fDelete;
        return this;
    }
    virtual void FatalSecondary(void) { CrtFatalRuntimeError0x19(); }
    virtual void Noop(BYTE fDelete) { (void)fDelete; }
};

static NETWORK_SocketRuntimeFatalThunkVtableModel g_NETWORK_SocketRuntimeFatalThunkVtableModel;
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

struct NETWORK_TcpipSocketStackCommonThunkVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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

struct NETWORK_TcpipSocketStackRuntimeThunkVtableModel
    : NETWORK_TcpipSocketStackCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 0xa8;
        return DeleteCompositeTcpipSocketChannelStackWrapper(pAdjusted, fDelete);
    }
    virtual void QueueConnect(void) {
        NETWORK_EffTransportConnectCallback *pCallback =
            (NETWORK_EffTransportConnectCallback *)
                ((char *)this - *(int *)((char *)this - 4) - 4);
        pCallback->QueueEffTransportConnectEvent();
    }
    virtual void WriteGlobalSession(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4);
        int nOffsets = *(int *)(pAdjusted - 0x38);
        char *pCallback = pAdjusted + *(int *)(nOffsets + 0x10);
        nOffsets = *(int *)(pCallback - 0x38);
        ((NETWORK_PeerPayloadSender *)(pCallback + *(int *)(nOffsets + 8) - 0x38))
            ->WriteEffStreamWithGlobalSession();
    }
    virtual void *DispatchAcked(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 0x7c;
        return DispatchAckedEffTransportPayloadEvent(pAdjusted - 4);
    }
};

struct NETWORK_TcpipSocketStackTimedThunkVtableModel
    : NETWORK_TcpipSocketStackCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 0x30;
        return DeleteCompositeTcpipSocketChannelStackWrapper(pAdjusted, fDelete);
    }
    virtual void WriteGlobalSession(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4);
        int nOffsets = *(int *)(pAdjusted - 0x38);
        char *pCallback = pAdjusted + *(int *)(nOffsets + 0x10);
        nOffsets = *(int *)(pCallback - 0x38);
        ((NETWORK_PeerPayloadSender *)(pCallback + *(int *)(nOffsets + 8) - 0x38))
            ->WriteEffStreamWithGlobalSession();
    }
    virtual void *DispatchAcked(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4);
        return DispatchAckedEffTransportPayloadEvent(pAdjusted - 4);
    }
    virtual void CaptureLookup(void *pLookupResult) {
        ((NETWORK_TimedSocketAsyncLookupAdapter *)((char *)this + 0x140))
            ->CaptureAsyncLookupAddressForTimedStream(pLookupResult);
    }
    virtual int SendUdp(void *pBuffer, int cbBuffer) {
        return ((NETWORK_TimedSocketAsyncLookupAdapter *)((char *)this + 0x140))
            ->SendUdpPayloadToSocketAddress(pBuffer, cbBuffer);
    }
    virtual void SetAssignedPort(short nPort) {
        SetTcpipSocketChannelAssignedPort((char *)this + 0x140, nPort);
    }
};

/* Raw table 0049a1e4.  Receive adapter is the leading slot; the shared
 * stream view follows it.  This cannot inherit the common model because that
 * would move the receive slot after the stream prefix. */
struct NETWORK_TcpipSocketStackDualThunkVtableModel {
    virtual int HandleReceive(int nMessage, void *pReserved, unsigned int nEvent) {
        return ((NETWORK_SocketAsyncMessageAdapter *)this)
            ->HandleSocketReceiveAsyncSelectMessage(nMessage, pReserved, nEvent);
    }
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        NETWORK_EffStreamBase *pStream = (NETWORK_EffStreamBase *)this;
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
        return DeleteDualSocketWindowChannelStackWrapper((char *)this - 0x2c, fDelete);
    }
    virtual void Ret4(void) {}
    virtual void Fatal0(void) { CrtFatalRuntimeError0x19(); }
    virtual void Fatal1(void) { CrtFatalRuntimeError0x19(); }
    virtual void Ret8(BYTE fDelete, BYTE fReserved) {
        (void)fDelete;
        (void)fReserved;
    }
    virtual void ClosePrimary(void) {
        TimedSocketBundlePrimaryCloseThunk(this);
    }
    virtual void *DeletePrimary(BYTE fDelete) {
        return TimedSocketBundlePrimaryDeleteThunk(this, fDelete);
    }
    virtual void CapturePrimaryError(void) {
        TimedSocketBundlePrimaryCaptureErrorThunk(this);
    }
    virtual void RetPrimary(BYTE fDelete) {
        TimedSocketBundlePrimaryNoopBody(fDelete);
    }
    virtual void *DeleteTimed(BYTE fDelete) {
        (void)fDelete;
        return (char *)this - 0x18;
    }
};

/* Raw table 0049a188.  The three middle entries are the adjusted
 * write/claim views; they are not the connect and dispatch entries used by
 * the other runtime table. */
struct NETWORK_TcpipSocketStackCompositeRuntimeThunkVtableModel
    : NETWORK_TcpipSocketStackCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 0x30;
        return DeleteCompositeTcpipSocketChannelStackWrapper(pAdjusted, fDelete);
    }
    virtual void WriteGlobalSession(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 8;
        ((NETWORK_PeerPayloadSender *)pAdjusted)->WriteEffStreamWithGlobalSession();
    }
    virtual void *ClaimRecord(void) {
        char *pAdjusted = (char *)this - *(int *)((char *)this - 4) - 8;
        return ClaimAckedEffTransportRecordPayload(pAdjusted);
    }
    virtual void CaptureLookup(void *pLookupResult) {
        ((NETWORK_TimedSocketAsyncLookupAdapter *)((char *)this + 0x140))
            ->CaptureAsyncLookupAddressForTimedStream(pLookupResult);
    }
    virtual int SendUdp(void *pBuffer, int cbBuffer) {
        return ((NETWORK_TimedSocketAsyncLookupAdapter *)((char *)this + 0x140))
            ->SendUdpPayloadToSocketAddress(pBuffer, cbBuffer);
    }
    virtual void SetAssignedPort(short nPort) {
        SetTcpipSocketChannelAssignedPort((char *)this + 0x140, nPort);
    }
};

static NETWORK_TcpipSocketStackRuntimeThunkVtableModel
    g_NETWORK_TcpipSocketStackRuntimeThunkVtableModel;
static void *g_NETWORK_TcpipSocketStackRuntimeThunkVtable =
    *(void ***)&g_NETWORK_TcpipSocketStackRuntimeThunkVtableModel;
static NETWORK_TcpipSocketStackTimedThunkVtableModel
    g_NETWORK_TcpipSocketStackTimedThunkVtableModel;
static void *g_NETWORK_TcpipSocketStackTimedThunkVtable =
    *(void ***)&g_NETWORK_TcpipSocketStackTimedThunkVtableModel;
static NETWORK_TcpipSocketStackDualThunkVtableModel
    g_NETWORK_TcpipSocketStackDualThunkVtableModel;
static void *g_NETWORK_TcpipSocketStackDualThunk =
    *(void ***)&g_NETWORK_TcpipSocketStackDualThunkVtableModel;
static NETWORK_TcpipSocketStackCompositeRuntimeThunkVtableModel
    g_NETWORK_TcpipSocketStackCompositeRuntimeThunkVtableModel;
static void *g_NETWORK_TcpipSocketStackCompositeRuntimeThunkVtable =
    *(void ***)&g_NETWORK_TcpipSocketStackCompositeRuntimeThunkVtableModel;
static void *g_NETWORK_TcpipSocketStackSocketThunkVtable = NetworkGetSafeVtable();

/* Allocation-composite construction groups recovered from 0049a348..e0. */
static int g_NETWORK_AllocatedTcpipCompositeTransportVtable[5] = {
    -4, 0x30, 0x60, 0xd8, 0x130,
};
static int g_NETWORK_AllocatedTcpipCompositeSocketStackVtable[5] = {
    0, 8, 0x38, 0xb0, 0x108,
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
static void *g_NETWORK_AllocatedTcpipCompositeVtable = NetworkGetSafeVtable();
static void *g_NETWORK_AllocatedTcpipCompositeFatalThunkVtable = NetworkGetSafeVtable();
static void *g_NETWORK_AllocatedTcpipCompositeTimedThunkVtable = NetworkGetSafeVtable();
static void *g_NETWORK_AllocatedTcpipCompositeDualThunk = NetworkGetSafeVtable();
static void *g_NETWORK_AllocatedTcpipCompositeSocketThunkVtable = NetworkGetSafeVtable();

static const char *g_NETWORK_SocketWindowClassName;
static int *g_pfSocketWindowClassRegistered;
static NETWORK_InetAddressEntry g_NETWORK_InetAddressEntryVtableModel;
static void *g_NETWORK_InetAddressEntryVtable =
    *(void ***)&g_NETWORK_InetAddressEntryVtableModel;
static const char *g_NETWORK_UdpServiceName;
static const char *g_NETWORK_TftpServiceName;

extern "C" DWORD timeGetTime(void);
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
static int *g_pEffTransportReceivedPacketBytes;

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
struct NETWORK_EffTransportPacketProcessor {
    int ProcessEffTransportPacketHeader(void);
};

struct NETWORK_RuntimeChannelStack {
    void *ConstructEffTransportRuntimeChannelStack(int fConstructEmbeddedObjects);
};
extern void *ConstructDualHandleEffStream(void *pStream, int fConstructChannelState);


// FUNCTION: LEMBALL 0x00460A90
void *NETWORK_CompositeEffTransportStackLayout::ConstructCompositeEffTransportStack(
    int fConstructEmbeddedObjects) {
    NETWORK_CompositeEffTransportStackLayout *pComposite;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    char *pbObjectBase;

    pComposite = this;
    pbObjectBase = (char *)this;

    if (fConstructEmbeddedObjects != 0) {
        pComposite->m_pChannelStateOffsets04 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_CompositeEffTransportChannelStateConstructionOffsets;
        pComposite->m_pTimedStreamOffsetsA4 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_CompositeEffTransportTimedStreamConstructionOffsets;
        *(void **)(pbObjectBase + 0x11c) = g_NETWORK_CompositeEffTransportDualStreamConstructionOffsets;
        *(void **)(pbObjectBase + 0x130) = g_NETWORK_CompositeEffTransportDualThunkConstructionOffsets;

        ((NETWORK_EffStreamChannelState *)pComposite->m_abChannelState30)->ConstructEffStreamChannelState();
        ((NETWORK_TimedEffStream *)pComposite->m_abTimedStream60)->ConstructTimedEffStream(0);
        ConstructDualHandleEffStream(pbObjectBase + 0xd8, 0);

        pOffsets = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_CompositeEffTransportDualThunkConstructionOffsets;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x12c + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x12c + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x12c + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
        pSecondaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
        pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
        pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;
    }

    pComposite->m_pVtable00 = (void **)g_NETWORK_CompositeEffTransportVtable;
    pOffsets = pComposite->m_pChannelStateOffsets04;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset);
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
    NETWORK_CompositeEffTransportStackLayout *pComposite;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    char *pbObjectBase;

    pbObjectBase = (char *)(unsigned long)(nObjectBasePlus0x30 - 0x30);
    pComposite = (NETWORK_CompositeEffTransportStackLayout *)pbObjectBase;
    pComposite->m_pVtable00 = (void **)g_NETWORK_CompositeEffTransportVtable;
    pOffsets = pComposite->m_pChannelStateOffsets04;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_CompositeEffTransportFatalThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_CompositeEffTransportTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x2c;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x5c;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xd4;
}

// FUNCTION: LEMBALL 0x0046FCF0
void *NETWORK_SocketWindowEffChannel::ConstructSocketWindowEffChannel(
    int fConstructEmbeddedState) {
    NETWORK_SocketWindowEffChannel *pChannel;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pChannel = this;

    if (fConstructEmbeddedState != 0) {
        pChannel->m_pEmbeddedStateConstructionOffsets = (void **)g_NETWORK_SocketWindowEffChannelEmbeddedStateVtable;
        ((NETWORK_EffStreamChannelState *)pChannel->m_abChannelState)->ConstructEffStreamChannelState();
    }

    ((PLATFORM_InvisibleMessageWindow *)pChannel)
        ->Construct(g_NETWORK_SocketWindowClassName, g_pfSocketWindowClassRegistered);
    pChannel->m_pVTable = g_NETWORK_SocketWindowEffChannelVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pChannel->m_pEmbeddedStateConstructionOffsets;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pChannel + 0x0c + pOffsets->m_nPrimaryOffset);
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
    NETWORK_SocketWindowEffChannel *pChannel;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pChannel = (NETWORK_SocketWindowEffChannel *)(unsigned long)(nChannelStateBase - 0x20);
    pChannel->m_pVTable = g_NETWORK_SocketWindowEffChannelVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pChannel->m_pEmbeddedStateConstructionOffsets;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pChannel + 0x0c + pOffsets->m_nPrimaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_SocketWindowEffChannelThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x10;

    if (pChannel->m_nLookupHandle != 0) {
        FreeVSMemBlock((void *)(unsigned long)pChannel->m_nLookupHandle);
    }

    DestroyInvisibleMessageWindow((PLATFORM_InvisibleMessageWindow *)pChannel);
}

// FUNCTION: LEMBALL 0x0046FEE0
int NETWORK_SocketAsyncMessageAdapter::ReceiveUdpPayloadFromConnectedSocket(void) {
    NETWORK_TimedSocketErrorChannel *pErrorChannel;
    NETWORK_TimedSocketPacketChannel *pPacketChannel;
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
        AppendIntToStream(g_pErrorOutputStream, (unsigned int)WSAGetLastError());
        AppendCStringToStream(g_pErrorOutputStream, "\n");
        pErrorChannel = (NETWORK_TimedSocketErrorChannel *)(pbThis + nThisDelta);
        pErrorChannel->HandleSocketError();
        return 0;
    }
    if (*(int *)(pbThis + nThisDelta + 0x10) == 0) {
        pPacketChannel = (NETWORK_TimedSocketPacketChannel *)(pbThis +
                                                               *(int *)(*(int *)pbThis + 8));
        pPacketChannel->NotifyPacketReady();
    }
    *g_pEffTransportReceivedPacketBytes += g_cbEffTransportCurrentPacketBytes;
    return ((NETWORK_EffTransportPacketProcessor *)(pbThis + *(int *)(*(int *)pbThis + 8)))
        ->ProcessEffTransportPacketHeader();
}

// FUNCTION: LEMBALL 0x0046FF90
int NETWORK_SocketAsyncMessageAdapter::HandleSocketReceiveAsyncSelectMessage(
    int nMessage, void *pReserved, unsigned int nEvent) {
    int nThisDelta;
    unsigned int nError;
    NETWORK_SocketAsyncMessageAdapter *pReceiveChannel;

    (void)pReserved;
    if (nMessage == 0x443) {
        nThisDelta = *(int *)(*(int *)((char *)this - 0x8c) + 4);
        if (*(int *)((char *)this + nThisDelta - 0x84) == -1) {
            return 0;
        }
        nError = nEvent >> 16;
        ((NETWORK_EffStreamChannelState *)((char *)this + nThisDelta - 0x8c))->SetEffStreamChannelAsyncErrorStatus((int)nError);
        if ((nEvent & 0xffff) == 1) {
            pReceiveChannel = (NETWORK_SocketAsyncMessageAdapter *)((char *)this - 0x8c);
            if (nError == 0) {
                pReceiveChannel->ReceiveUdpPayloadFromConnectedSocket();
                return 0;
            }
            AppendCStringToStream(g_pErrorOutputStream, "Receive error:");
            AppendIntToStream(g_pErrorOutputStream, nError);
            AppendCStringToStream(g_pErrorOutputStream, "\n");
            return 0;
        }
    }
    return -1;
}

// FUNCTION: LEMBALL 0x0046FDB0
int NETWORK_SocketAsyncErrorAdapter::HandleSocketAsyncErrorLparam(
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
    ((NETWORK_EffStreamChannelState *)((char *)this + nChannelDelta + 0x10))->SetEffStreamChannelAsyncErrorStatus((int)nError);
    if (*pBuffer != 0) {
        FreeVSMemBlock((void *)(unsigned long)*pBuffer);
    }
    *pBuffer = 0;
    return 0x0e;
}

// FUNCTION: LEMBALL 0x0046FE10
int NETWORK_SocketAsyncMessageAdapter::ReceiveUdpPayloadFromSenderAddress(void) {
    unsigned char abFrom[16];
    int cbFrom;
    int nResult;
    int nChannelDelta;
    char *pszAddress;
    NETWORK_InetAddressEntry *pPeerAddress;
    NETWORK_SocketChannelControl *pErrorChannel;

    cbFrom = 0x10;
    nChannelDelta = *(int *)(*(int *)this + 4);
    nResult = recvfrom(*(int *)((char *)this + nChannelDelta + 8),
                       (char *)g_pEffTransportPacketBuffer, g_cbEffTransportMaxPacketBytes, 0,
                       abFrom, &cbFrom);
    g_cbEffTransportCurrentPacketBytes = nResult;
    pPeerAddress = (NETWORK_InetAddressEntry *)g_pEffTransportPeerAddressState;
    if (nResult == -1) {
        AppendCStringToStream(g_pErrorOutputStream, "Receive error (after receive from):");
        AppendIntToStream(g_pErrorOutputStream, (unsigned int)WSAGetLastError());
        AppendCStringToStream(g_pErrorOutputStream, "\n");
        pErrorChannel = (NETWORK_SocketChannelControl *)((char *)this + nChannelDelta);
        pErrorChannel->HandleSocketError();
        return 0;
    }
    pPeerAddress->m_nAddress = *(int *)abFrom;
    pszAddress = (char *)inet_ntoa((unsigned long)pPeerAddress->m_nAddress);
    strcpy(pPeerAddress->m_szAddress, pszAddress);
    return ((NETWORK_EffTransportPacketProcessor *)((char *)this +
                                                    *(int *)(*(int *)this + 8)))
        ->ProcessEffTransportPacketHeader();
}

// FUNCTION: LEMBALL 0x00470030
void *NETWORK_TimedSocketEffChannelBundleLayout::ConstructTimedSocketEffChannelBundle(
    int fConstructEmbeddedObjects) {
    NETWORK_TimedSocketEffChannelBundleLayout *pBundle;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    char *pbObjectBase;

    pBundle = this;
    pbObjectBase = (char *)this;

    if (fConstructEmbeddedObjects != 0) {
        pBundle->m_pVtable00 = (void **)g_NETWORK_TimedSocketBundleBaseConstructionOffsets;
        pBundle->m_pTimedStreamConstructionOffsets8c = (void **)g_NETWORK_TimedSocketBundleTimedStreamConstructionOffsets;
        pBundle->m_pSocketChannelConstructionOffsetsd0 = (void **)g_NETWORK_TimedSocketBundleSocketChannelConstructionOffsets;
        ((NETWORK_EffStreamChannelState *)pBundle->m_abChannelState18)->ConstructEffStreamChannelState();
        ((NETWORK_TimedEffStream *)pBundle->m_abTimedStream48)->ConstructTimedEffStream(0);
        ((NETWORK_SocketWindowEffChannel *)pBundle->m_abSocketWindowC0)
            ->ConstructSocketWindowEffChannel(0);
    }

    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pBundle->m_pVtable00;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset - 4);
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
void NETWORK_TimedSocketAsyncLookupAdapter::CaptureAsyncLookupAddressForTimedStream(void *pLookupResult) {
    void *pTimedStream;

    pTimedStream = (void *)(unsigned long)(*(int *)(*(int *)((char *)this - 0x48) + 8) +
                                           (int)(unsigned long)this - 0x48);
    ((NETWORK_TimedEffStream *)pTimedStream)->InvokeTimedEffStreamServiceCallback(pLookupResult);
    *(int *)((char *)this - 0x40) = *(int *)((char *)pLookupResult + 0x14);
}

// FUNCTION: LEMBALL 0x00470120
int NETWORK_TimedSocketAsyncLookupAdapter::SendUdpPayloadToSocketAddress(void *pBuffer, int cbBuffer) {
    int nSocket;
    int nResult;
    NETWORK_TimedSocketErrorChannel *pChannel;
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
        pChannel = (NETWORK_TimedSocketErrorChannel *)(pbThis + nSocket - 0x48);
        pChannel->HandleSocketError();
        return 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x004701A0
int NETWORK_TimedSocketAsyncMessageAdapter::HandleTimedSocketConnectAsyncMessage(
    int nMessage, void *pReserved, unsigned int nEvent) {
    int nThisDelta;

    (void)pReserved;
    if (nMessage == 0x443) {
        nThisDelta = *(int *)(*(int *)((char *)this - 0xc0) + 4);
        if (*(int *)((char *)this + nThisDelta - 0xb8) == -1) {
            return -1;
        }
        ((NETWORK_EffStreamChannelState *)((char *)this + nThisDelta - 0xc0))->SetEffStreamChannelAsyncErrorStatus((int)(nEvent >> 16));
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
void NETWORK_TimedSocketAsyncMessageAdapter::DispatchEffSocketAsyncSelectMessage(
    int nMessage, void *pReserved, unsigned int nEvent) {
    int nTimedDelta;
    int nReceiveDelta;
    int nResult;
    NETWORK_TimedSocketAsyncMessageAdapter *pTimedChannel;
    NETWORK_SocketAsyncMessageAdapter *pReceiveChannel;
    char *pbThis;

    pbThis = (char *)this;
    nTimedDelta = *(int *)(*(int *)(pbThis - 0x110) + 0x1c);
    pTimedChannel = (NETWORK_TimedSocketAsyncMessageAdapter *)(pbThis + nTimedDelta - 0x50);
    nResult = pTimedChannel->HandleTimedSocketConnectAsyncMessage(nMessage, pReserved, nEvent);
    if (nResult == -1) {
        nReceiveDelta = *(int *)(*(int *)(pbThis - 0x110) + 0x18);
        pReceiveChannel = (NETWORK_SocketAsyncMessageAdapter *)(pbThis + nReceiveDelta - 0x84);
        pReceiveChannel->HandleSocketReceiveAsyncSelectMessage(nMessage, pReserved, nEvent);
    }
}

// FUNCTION: LEMBALL 0x00470270
void *NETWORK_TcpipEffTransportCompositeLayout::ConstructTcpipEffTransportComposite(
    int fConstructEmbeddedObjects) {
    NETWORK_TcpipEffTransportCompositeLayout *pComposite;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;

    pComposite = this;
    if (fConstructEmbeddedObjects != 0) {
        pComposite->m_pOuterOffsets04 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTimedStreamVtable;
        pComposite->m_pTransportOffsets20 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTransportStackVtable;
        pComposite->m_pDualStreamConstructionOffsetsA4 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeDualStreamVtable;
        pComposite->m_pDualThunkOffsets11c = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeDualThunkVtable;
        pComposite->m_pSocketThunkOffsets130 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeSocketWindowVtable;
        pComposite->m_pTimedSocketThunkOffsets148 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTimedSocketThunkVtable;
        pComposite->m_pTimedSocketBundleOffsets154 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTimedSocketBundleVtable;
        pComposite->m_pTimedSocketBundleConstructionData158 = (void **)g_NETWORK_TcpipCompositeTimedSocketBundleDataVtable;

        ((NETWORK_EffStreamChannelState *)pComposite->m_abChannelState30)->ConstructEffStreamChannelState();
        ((NETWORK_TimedEffStream *)pComposite->m_abTimedStream60)->ConstructTimedEffStream(0);
        ConstructDualHandleEffStream(pComposite->m_abDualStreamD8, 0);

        pOffsets = pComposite->m_pSocketThunkOffsets130;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x12c + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x12c + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x12c + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
        pSecondaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
        pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
        pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;

        ((NETWORK_SocketWindowEffChannel *)pComposite->m_abSocketWindow138)
            ->ConstructSocketWindowEffChannel(0);

        pOffsets = pComposite->m_pTimedSocketBundleOffsets154;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x150 + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x150 + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x150 + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
        pPrimaryThunk->m_nThisDelta = 0;

        ((NETWORK_TimedSocketEffChannelBundleLayout *)&pComposite->m_pTimedSocketBundleConstructionData158)
            ->ConstructTimedSocketEffChannelBundle(0);
    }

    ((NETWORK_RuntimeChannelStack *)this)->ConstructEffTransportRuntimeChannelStack(0);
    pOffsets = pComposite->m_pTransportOffsets20;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nTertiaryOffset);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nQuaternaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackCompositeRuntimeThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TcpipCompositeTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
    pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipCompositeSocketThunkVtable;
    pPrimaryThunk->m_nThisDelta = 0;
    pSecondaryThunk->m_nThisDelta = 0;
    pTertiaryThunk->m_nThisDelta = 0;
    pQuaternaryThunk->m_nThisDelta = 0;

    pComposite->m_pVtable00 = (void **)g_NETWORK_TcpipCompositeVtable;
    pOffsets = pComposite->m_pOuterOffsets04;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + pOffsets->m_nTertiaryOffset);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + pOffsets->m_nQuaternaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterFatalThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterSocketThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x2c;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x5c;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xd4;
    pQuaternaryThunk->m_nThisDelta = pOffsets->m_nQuaternaryOffset - 0x134;

    pComposite->m_nSocketWindowHandle24 = 0;
    pComposite->m_nLookupHandle28 = 0;
    return this;
}

// FUNCTION: LEMBALL 0x004704E0
void DestroyTcpipEffTransportComposite(int nObjectBasePlus0x30) {
    NETWORK_TcpipEffTransportCompositeLayout *pComposite;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;
    char *pbObjectBase;

    pbObjectBase = (char *)(unsigned long)(nObjectBasePlus0x30 - 0x30);
    pComposite = (NETWORK_TcpipEffTransportCompositeLayout *)pbObjectBase;
    pComposite->m_pVtable00 = (void **)g_NETWORK_TcpipCompositeVtable;
    pOffsets = pComposite->m_pOuterOffsets04;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nQuaternaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterFatalThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_TcpipCompositeOuterSocketThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x2c;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x5c;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xd4;
    pQuaternaryThunk->m_nThisDelta = pOffsets->m_nQuaternaryOffset - 0x134;

    if ((void *)(unsigned long)pComposite->m_nSocketWindowHandle24 != 0) {
        FreeVSMemBlock((void *)(unsigned long)pComposite->m_nSocketWindowHandle24);
    }

    ((NETWORK_RuntimeChannelStackReleaseFront *)(unsigned long)(nObjectBasePlus0x30 - 0xc))->Release();
}

// FUNCTION: LEMBALL 0x00470650
void NETWORK_HostLookupCompletionAdapter::HandleRemoteHostLookupComplete(int nError) {
    char abAddress[20];
    char *pbLookup;
    char *pszAddress;
    NETWORK_InetAddressEntry *pEntry;
    NETWORK_EffDispatchQueue *pDispatchQueue;
    unsigned short cbAddress;
    unsigned char abEvent[20];

    pbLookup = *(char **)((char *)this + 0x24);
    if (nError == 0) {
        cbAddress = *(short *)(pbLookup + 0x0a);
        memcpy(abAddress, *(void **)(*(int *)(pbLookup + 0x0c)), cbAddress);
        pEntry = (NETWORK_InetAddressEntry *)AllocateVSMemBlock(0x18);
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
            pDispatchQueue = (NETWORK_EffDispatchQueue *)g_pEffTransportDispatchQueue;
            pDispatchQueue->QueueEvent(abEvent);
        }
    }
    FreeVSMemBlock(pbLookup);
    *(int *)((char *)this + 0x24) = 0;
}

// FUNCTION: LEMBALL 0x00470840
void NETWORK_HostLookupCompletionAdapter::HandleLocalHostLookupComplete(int nError) {
    char abAddress[4];
    char *pbLookup;
    char *pszAddress;
    NETWORK_InetAddressEntry *pLocalAddress;
    NETWORK_TimedSocketErrorChannel *pErrorChannel;
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
        pLocalAddress = (NETWORK_InetAddressEntry *)g_pEffTransportRuntimeService;
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
        pErrorChannel = (NETWORK_TimedSocketErrorChannel *)(pbSocketChannel + 4);
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
        pErrorChannel = (NETWORK_TimedSocketErrorChannel *)(pbSocketChannel + 4);
        pErrorChannel->HandleSocketError();
        QueueEffTransportErrorEvent(2);
    }
}

// FUNCTION: LEMBALL 0x004709C0
void NETWORK_HostLookupCompletionAdapter::HandleServicePortLookupComplete(int nError) {
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
    NETWORK_SocketChannelControl *pControl;
    NETWORK_EffDispatchQueue *pDispatchQueue;

    nSocketDelta = *(int *)(*(int *)((char *)this + 4) + 8);
    pControl = (NETWORK_SocketChannelControl *)((char *)this + nSocketDelta + 4);
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
        ((NETWORK_SocketChannelControl *)(pbSocketChannel + 4))->HandleSocketError();
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
        ((NETWORK_SocketChannelControl *)(pbSocketChannel + 4))->HandleSocketError();
        QueueEffTransportErrorEvent(4);
        return;
    }

    nOuterDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
    pbTimedSocket = (char *)this + nOuterDelta;
    nResult = WSAAsyncSelect(nSocket, (void *)(unsigned long)*(int *)(pbTimedSocket + 8),
                             0x443, *(int *)((char *)this + 0x14) != 0 ? 3 : 2);
    if (nResult == -1) {
        ((NETWORK_SocketChannelControl *)(pbSocketChannel + 4))->HandleSocketError();
        QueueEffTransportErrorEvent(5);
        return;
    }

    *(int *)(pbSocketChannel + 0x14) = 1;
    *(int *)(pbSocketChannel + 0x18) = 0;
    *(int *)((char *)this + 0x18) = (int)(timeGetTime() - 1000);
    *(unsigned short *)abEvent = 2;
    *(int *)(abEvent + 8) = 0;
    pDispatchQueue = (NETWORK_EffDispatchQueue *)g_pEffTransportDispatchQueue;
    pDispatchQueue->QueueEvent(abEvent);
}

// FUNCTION: LEMBALL 0x004715B0
const char *NETWORK_InetAddressEntry::GetAddressString(void) {
    return m_szAddress;
}

// FUNCTION: LEMBALL 0x00471550
void NETWORK_InetAddressEntry::StoreAddressString(const char *pszAddress) {
    m_nAddress = (int)inet_addr(pszAddress);
    strcpy(m_szAddress, pszAddress);
}

// FUNCTION: LEMBALL 0x00462DB0
void *DeleteCompositeEffTransportStackWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_CompositeEffTransportStackWrapperView *pView;
    char *pbAllocationBase;

    pView = (NETWORK_CompositeEffTransportStackWrapperView *)pObject;
    pbAllocationBase = (char *)pObject - 0x30;
    RestoreCompositeEffTransportVtables((int)(unsigned long)pObject);
    ((NETWORK_DualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((NETWORK_TimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    ((NETWORK_EffStreamChannelState *)pView->m_abChannelState00)->DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00471B10
void *DeleteCompositeTcpipSocketChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_TcpipSocketChannelStackWrapperView *pView;
    char *pbAllocationBase;

    pView = (NETWORK_TcpipSocketChannelStackWrapperView *)pObject;
    pbAllocationBase = (char *)pObject - 0x34;
    RestoreCompositeEffTransportVtables((int)(unsigned long)pObject - 4);
    DestroySocketWindowEffChannel((int)(unsigned long)pView->m_abSocketWindow128 + 0x20);
    ((NETWORK_DualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((NETWORK_TimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    ((NETWORK_EffStreamChannelState *)pView->m_abChannelState00)->DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00471CD0
void *DeleteTcpipSocketChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_TcpipSocketChannelStackWrapperView *pView;
    char *pbAllocationBase;

    pView = (NETWORK_TcpipSocketChannelStackWrapperView *)pObject;
    pbAllocationBase = (char *)pObject - 8;
    DestroySocketWindowEffChannel((int)(unsigned long)pView->m_abSocketWindow128 + 0x20);
    ((NETWORK_DualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((NETWORK_TimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    ((NETWORK_EffStreamChannelState *)pView->m_abChannelState00)->DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00471DE0
void *DeleteDualSocketWindowChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_DualSocketWindowChannelStackWrapperView *pView;
    char *pbAllocationBase;

    pView = (NETWORK_DualSocketWindowChannelStackWrapperView *)pObject;
    pbAllocationBase = (char *)pObject - 8;
    DestroySocketWindowEffChannel((int)(unsigned long)pView->m_abSocketWindowA4 + 0x20);
    ((NETWORK_DualHandleEffStream *)pView->m_abDualStream2c)->DestroyDualHandleEffStream();
    ((NETWORK_EffStreamChannelState *)pView->m_abChannelState00)->DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00471E80
void *DeleteTimedSocketWindowChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_TimedSocketWindowChannelStackWrapperView *pView;
    char *pbAllocationBase;

    pView = (NETWORK_TimedSocketWindowChannelStackWrapperView *)pObject;
    pbAllocationBase = (char *)pObject - 0x18;
    DestroySocketWindowEffChannel((int)(unsigned long)pView->m_abSocketWindowC8 + 0x20);
    ((NETWORK_TimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    ((NETWORK_EffStreamChannelState *)pView->m_abChannelState00)->DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x00471F60
void *DeleteTcpipEffTransportCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_TcpipSocketChannelStackWrapperView *pView;
    char *pbAllocationBase;

    pView = (NETWORK_TcpipSocketChannelStackWrapperView *)pObject;
    pbAllocationBase = (char *)pObject - 0x30;
    DestroyTcpipEffTransportComposite((int)(unsigned long)pObject);
    DestroySocketWindowEffChannel((int)(unsigned long)pView->m_abSocketWindow128 + 0x20);
    ((NETWORK_DualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((NETWORK_TimedEffStream *)pView->m_abTimedStream30)->DestroyTimedEffStream();
    ((NETWORK_EffStreamChannelState *)pView->m_abChannelState00)->DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x004715E0
void *AllocateTcpipEffTransportComposite(void) {
    NETWORK_AllocatedTcpipEffTransportCompositeLayout *pComposite;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;
    char *pbObjectBase;

    pComposite = (NETWORK_AllocatedTcpipEffTransportCompositeLayout *)AllocateVSMemBlock(0x170);
    if (pComposite == 0) {
        return 0;
    }

    pbObjectBase = (char *)pComposite;
    pComposite->m_pOuterOffsets04 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeTransportVtable;
    pComposite->m_pTransportOffsets2c = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeSocketStackVtable;
    pComposite->m_pDualStreamOffsetsA8 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeDualStreamVtable;
    pComposite->m_pDualThunkOffsets120 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeDualThunkVtable;
    pComposite->m_pSocketThunkOffsets134 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeSocketWindowVtable;
    pComposite->m_pTimedSocketBundleOffsets158 =
        (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeTimedSocketBundleVtable;
    pComposite->m_pTimedSocketThunkOffsets14c =
        (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeTimedSocketThunkVtable;
    pComposite->m_pTimedSocketBundleConstructionData15c = (void **)g_NETWORK_AllocatedTcpipCompositeTimedSocketDataVtable;

    ((NETWORK_EffStreamChannelState *)pComposite->m_abChannelState30)->ConstructEffStreamChannelState();
    ((NETWORK_TimedEffStream *)pComposite->m_abTimedStream60)->ConstructTimedEffStream(0);
    ConstructDualHandleEffStream(pComposite->m_abDualStreamD8, 0);

    pOffsets = pComposite->m_pSocketThunkOffsets134;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x134 + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x134 + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x134 + pOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
    pSecondaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;

    ((NETWORK_SocketWindowEffChannel *)pComposite->m_abSocketWindow13c)
        ->ConstructSocketWindowEffChannel(0);

    pOffsets = pComposite->m_pTimedSocketBundleOffsets158;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x154 + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x154 + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x154 + pOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
    pPrimaryThunk->m_nThisDelta = 0;

    ((NETWORK_TimedSocketEffChannelBundleLayout *)&pComposite->m_pTimedSocketBundleConstructionData15c)
        ->ConstructTimedSocketEffChannelBundle(0);
    ((NETWORK_CompositeEffTransportStackLayout *)pComposite)
        ->ConstructCompositeEffTransportStack(0);
    ((NETWORK_TcpipSocketChannelStackLayout *)((char *)pComposite + 0x2c))
        ->ConstructTcpipSocketChannelStack(0);

    pComposite->m_pVtable00 = (void **)g_NETWORK_AllocatedTcpipCompositeVtable;
    pOffsets = pComposite->m_pOuterOffsets04;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nQuaternaryOffset);
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

// FUNCTION: LEMBALL 0x00471830
void *NETWORK_TcpipSocketChannelStackLayout::ConstructTcpipSocketChannelStack(
    int fConstructEmbeddedObjects) {
    NETWORK_TcpipSocketChannelStackLayout *pStack;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;
    char *pbObjectBase;

    pStack = this;
    pbObjectBase = (char *)this;

    if (fConstructEmbeddedObjects != 0) {
        pStack->m_pTimedStreamConstructionOffsets7c = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackTimedStreamVtable;
        pStack->m_pDualStreamConstructionOffsetsf4 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackDualStreamVtable;
        pStack->m_pBaseOffsets00 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackBaseVtable;
        pStack->m_pDualThunkOffsets108 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackDualThunkVtable;
        pStack->m_pTimedSocketBundleConstructionData130 = (void **)g_NETWORK_TcpipSocketStackTimedSocketDataVtable;
        pStack->m_pTimedSocketBundleConstructionOffsets120 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackTimedSocketBundleVtable;
        pStack->m_pSocketWindowConstructionOffsets12c = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackSocketWindowVtable;

        ((NETWORK_EffStreamChannelState *)pStack->m_abChannelState08)->ConstructEffStreamChannelState();
        ((NETWORK_TimedEffStream *)pStack->m_abTimedStream38)->ConstructTimedEffStream(0);
        ConstructDualHandleEffStream(pStack->m_abDualStreamB0, 0);

        pOffsets = pStack->m_pDualThunkOffsets108;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x104 + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x104 + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x104 + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
        pSecondaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
        pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
        pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;

        ((NETWORK_SocketWindowEffChannel *)pStack->m_abSocketWindow110)
            ->ConstructSocketWindowEffChannel(0);

        pOffsets = pStack->m_pSocketWindowConstructionOffsets12c;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x128 + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x128 + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x128 + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
        pPrimaryThunk->m_nThisDelta = 0;

        ((NETWORK_TimedSocketEffChannelBundleLayout *)&pStack->m_pTimedSocketBundleConstructionData130)
            ->ConstructTimedSocketEffChannelBundle(0);
    }

    pOffsets = pStack->m_pBaseOffsets00;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset - 4);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nQuaternaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TcpipSocketStackRuntimeThunkVtable;
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
int NETWORK_TcpipClientAsyncDispatcher::DispatchTcpipClientAsyncMessage(
    int nMessage, void *pReserved, unsigned int nEvent) {
    char *pbThis;
    int nDelta;
    int nResult;
    NETWORK_SocketAsyncErrorAdapter *pErrorAdapter;
    NETWORK_HostLookupCompletionAdapter *pLookupAdapter;
    NETWORK_SocketAsyncMessageAdapter *pReceiveAdapter;
    NETWORK_TimedSocketAsyncMessageAdapter *pSocketDispatcher;

    pbThis = (char *)this;
    switch (nMessage) {
    case 0x440:
        nDelta = *(int *)(*(int *)(pbThis - 0x134) + 0x14);
        pErrorAdapter = (NETWORK_SocketAsyncErrorAdapter *)(pbThis + nDelta - 0x134);
        nResult = pErrorAdapter->HandleSocketAsyncErrorLparam(
            pReserved, nEvent, (unsigned int *)(pbThis + nDelta - 0x120));
        if (nResult != 0x0e) {
            pLookupAdapter = (NETWORK_HostLookupCompletionAdapter *)(pbThis - 0x138);
            pLookupAdapter->HandleLocalHostLookupComplete(nResult == 2);
        }
        return 0;

    case 0x441:
        *(int *)(pbThis - 0x110) = 0;
        nDelta = *(int *)(*(int *)(pbThis - 0x134) + 0x14);
        pErrorAdapter = (NETWORK_SocketAsyncErrorAdapter *)(pbThis + nDelta - 0x134);
        nResult = pErrorAdapter->HandleSocketAsyncErrorLparam(
            pReserved, nEvent, (unsigned int *)(pbThis - 0x114));
        if (nResult != 0x0e) {
            pLookupAdapter = (NETWORK_HostLookupCompletionAdapter *)(pbThis - 0x138);
            pLookupAdapter->HandleRemoteHostLookupComplete(nResult == 2);
        }
        return 0;

    case 0x442:
        nDelta = *(int *)(*(int *)(pbThis - 0x134) + 0x14);
        pErrorAdapter = (NETWORK_SocketAsyncErrorAdapter *)(pbThis + nDelta - 0x134);
        nResult = pErrorAdapter->HandleSocketAsyncErrorLparam(
            pReserved, nEvent, (unsigned int *)(pbThis + nDelta - 0x120));
        if (nResult != 0x0e) {
            pLookupAdapter = (NETWORK_HostLookupCompletionAdapter *)(pbThis - 0x138);
            pLookupAdapter->HandleServicePortLookupComplete(nResult == 2);
        }
        return 0;

    case 0x443:
        nDelta = *(int *)(*(int *)(pbThis - 0x134) + 4);
        if (*(int *)(pbThis + nDelta - 0x12c) == -1) {
            return 0;
        }
        ((NETWORK_EffStreamChannelState *)(pbThis + nDelta - 0x134))->SetEffStreamChannelAsyncErrorStatus((int)(nEvent >> 16));
        if ((nEvent & 0xffff) == 1) {
            if ((nEvent >> 16) == 0) {
                nDelta = *(int *)(*(int *)(pbThis - 0x134) + 0x18);
                pReceiveAdapter = (NETWORK_SocketAsyncMessageAdapter *)(pbThis + nDelta - 0x134);
                pReceiveAdapter->ReceiveUdpPayloadFromSenderAddress();
            }
            return 0;
        }
        break;
    }

    pSocketDispatcher = (NETWORK_TimedSocketAsyncMessageAdapter *)(pbThis - 8);
    pSocketDispatcher->DispatchEffSocketAsyncSelectMessage(nMessage, pReserved, nEvent);
    return 0;
}

// FUNCTION: LEMBALL 0x00471210
void NETWORK_UdpSocketBinder::BindUdpSocketAndEnableAsyncSelect(void) {
    unsigned char abAddress[16];
    int nPrimaryDelta;
    int nTimedDelta;
    int nSocket;
    int nResult;
    NETWORK_SocketChannelControl *pErrorChannel;

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
        pErrorChannel = (NETWORK_SocketChannelControl *)((char *)this + nPrimaryDelta + 4);
        pErrorChannel->HandleSocketError();
        return;
    }

    nTimedDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
    nResult = WSAAsyncSelect(nSocket, (void *)(unsigned long)*(int *)((char *)this + nTimedDelta + 8),
                             0x443, 3);
    if (nResult == -1) {
        pErrorChannel = (NETWORK_SocketChannelControl *)((char *)this + nPrimaryDelta + 4);
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
int NETWORK_TcpipHostPortAsyncDispatcher::DispatchTcpipHostPortAsyncMessage(
    int nMessage, void *pReserved, unsigned int nEvent) {
    char *pbThis;
    int nDelta;
    int nResult;
    NETWORK_SocketAsyncErrorAdapter *pErrorAdapter;
    NETWORK_HostLookupCompletionAdapter *pLookupAdapter;
    NETWORK_TimedSocketAsyncMessageAdapter *pSocketDispatcher;

    pbThis = (char *)this;
    if (*(int *)(pbThis - 0x11c) != 0) {
        return -1;
    }
    if (nMessage != 0x440 && nMessage != 0x442) {
        pSocketDispatcher = (NETWORK_TimedSocketAsyncMessageAdapter *)this;
        pSocketDispatcher->DispatchEffSocketAsyncSelectMessage(nMessage, pReserved, nEvent);
        return 0;
    }

    nDelta = *(int *)(*(int *)(pbThis - 0x138) + 0x14);
    pErrorAdapter = (NETWORK_SocketAsyncErrorAdapter *)(pbThis + nDelta - 0x138);
    nResult = pErrorAdapter->HandleSocketAsyncErrorLparam(
        pReserved, nEvent, (unsigned int *)(pbThis + nDelta - 0x124));
    if (nResult != 0x0e) {
        pLookupAdapter = (NETWORK_HostLookupCompletionAdapter *)(pbThis - 0x13c);
        if (nMessage == 0x440) {
            pLookupAdapter->HandleRemoteServiceHostLookupComplete(nResult == 2);
        } else {
            pLookupAdapter->HandleRemoteServicePortLookupComplete(nResult == 2);
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00471A60
void NETWORK_SocketWindowChannelWrapper::CloseSocketHandleFromChannel(void) {
    int nDelta;

    nDelta = *(int *)(*(int *)((char *)this - 0x10) + 4);
    closesocket(*(int *)((char *)this + nDelta - 8));
}

// FUNCTION: LEMBALL 0x00471A90
void *NETWORK_SocketWindowChannelWrapper::DeleteSocketWindowChannelStateWrapper(BYTE fFreeMemory) {
    void *pAllocationBase;

    DestroySocketWindowEffChannel((int)(unsigned long)this);
    ((NETWORK_EffStreamChannelState *)this)->DestroyEffStreamChannelState();
    pAllocationBase = (char *)this - 0x20;
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pAllocationBase);
    }
    return pAllocationBase;
}

// FUNCTION: LEMBALL 0x00471AD0
void NETWORK_SocketWindowChannelWrapper::CaptureLastWinsockErrorStatus(void) {
    int nDelta;

    nDelta = *(int *)(*(int *)((char *)this - 0x10) + 4);
    ((NETWORK_EffStreamChannelState *)((char *)this + nDelta - 0x10))->SetEffStreamChannelAsyncErrorStatus(WSAGetLastError());
}

// FUNCTION: LEMBALL 0x00470ED0
void NETWORK_HostLookupCompletionAdapter::HandleRemoteServiceHostLookupComplete(int nError) {
    NETWORK_InetAddressEntry localAddress;
    NETWORK_SocketChannelControl *pSocketChannel;
    NETWORK_SocketChannelControl *pErrorChannel;
    NETWORK_TcpipLookupOwner *pOwner;
    char *pbLookup;
    char *pszAddress;
    int nTimedDelta;
    int nSocketDelta;
    int nPort;
    int nLookupResult;

    if (nError != 0) {
        AppendCStringToStream(g_pErrorOutputStream, "Computer specified was not found\n");
        return;
    }

    nTimedDelta = *(int *)(*(int *)((char *)this + 4) + 0x14);
    pbLookup = *(char **)((char *)this + nTimedDelta + 0x18);
    *(void ***)&localAddress = (void **)g_NETWORK_InetAddressEntryVtable;
    localAddress.m_szAddress[0] = '\0';
    localAddress.m_nAddress = *(int *)(*(int *)(pbLookup + 0x0c));
    pszAddress = (char *)inet_ntoa((unsigned long)localAddress.m_nAddress);
    strcpy(localAddress.m_szAddress, pszAddress);

    nSocketDelta = *(int *)(*(int *)((char *)this + 4) + 8);
    pSocketChannel = (NETWORK_SocketChannelControl *)((char *)this + nSocketDelta + 4);
    pSocketChannel->SetRemoteAddress(&localAddress);

    nPort = atoi(*(char **)((char *)this + 0x0c));
    *(short *)((char *)this +
               *(int *)(*(int *)((char *)this + 4) + 4) + 0x24) = (short)nPort;
    pOwner = (NETWORK_TcpipLookupOwner *)this;
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
        pErrorChannel = (NETWORK_SocketChannelControl *)((char *)this + nSocketDelta + 4);
        pErrorChannel->HandleSocketError();
    }
}

// FUNCTION: LEMBALL 0x00471000
void NETWORK_HostLookupCompletionAdapter::HandleRemoteServicePortLookupComplete(int nError) {
    int nOffsets;
    int nTimedDelta;
    int nPort;
    char *pbLookup;
    NETWORK_SocketChannelControl *pControl;

    nOffsets = *(int *)((char *)this + 4);
    pControl = (NETWORK_SocketChannelControl *)((char *)this + *(int *)(nOffsets + 8) + 4);
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
        ((NETWORK_TcpipLookupOwner *)this)->BeginPortSetup();
    }
}
