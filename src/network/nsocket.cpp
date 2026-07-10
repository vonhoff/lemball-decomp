#include "../game/game_app.h"
#include "../engine/memory_arena.h"
#include "../engine/runtime_init.h"
#include "../platform/message_window.h"
#include "network/nstream.h"
#include <string.h>
#include <stdlib.h>

#ifdef ntohs
#undef ntohs
#endif
#ifdef htons
#undef htons
#endif

extern int ReturnTrueVtableCallback(void);

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

struct NETWORK_SocketWindowEffChannel {
    void **m_pVtable;
    void *m_hWnd;
    int m_nMessageBase;
    int m_nMessageLimit;
    void **m_pEmbeddedStateVtable;
    int m_nSocketHandle;
    int m_nLookupHandle;
    unsigned char m_abChannelState[0x30];
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

struct NETWORK_HostLookupCompletionAdapter {
    void HandleRemoteHostLookupComplete(int nError);
    void HandleLocalHostLookupComplete(int nError);
    void HandleServicePortLookupComplete(int nError);
    void HandleRemoteServiceHostLookupComplete(int nError);
    void HandleRemoteServicePortLookupComplete(int nError);
};

struct NETWORK_InetAddressEntry {
    virtual const char *GetAddressString(void);
    void StoreAddressString(const char *pszAddress);
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
    void **m_pTimedStreamVtable8c;
    unsigned char m_abReserved90[0x40];
    void **m_pSocketChannelVtabled0;
    unsigned char m_abReservedd4[0x0c];
    unsigned char m_abSocketWindowC0[0x20];
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
    NETWORK_ConstructionAdjustorVtable *m_pDualStreamVtableA4;
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
    void **m_pTimedSocketBundleDataVtable158;
    unsigned char m_abTimedSocketBundle15c[0x14];
};

struct NETWORK_TcpipSocketChannelStackLayout {
    NETWORK_ConstructionAdjustorVtable *m_pBaseOffsets00;
    int m_nReserved04;
    unsigned char m_abChannelState08[0x30];
    unsigned char m_abTimedStream38[0x44];
    NETWORK_ConstructionAdjustorVtable *m_pTimedStreamVtable7c;
    unsigned char m_abTimedStream80[0x30];
    unsigned char m_abDualStreamB0[0x44];
    NETWORK_ConstructionAdjustorVtable *m_pDualStreamVtablef4;
    unsigned char m_abReservedf8[0x10];
    NETWORK_ConstructionAdjustorVtable *m_pDualThunkOffsets108;
    unsigned char m_abReserved10c[0x04];
    unsigned char m_abSocketWindow110[0x10];
    NETWORK_ConstructionAdjustorVtable *m_pTimedSocketBundleVtable120;
    unsigned char m_abSocketWindow124[0x08];
    NETWORK_ConstructionAdjustorVtable *m_pSocketWindowVtable12c;
    void **m_pTimedSocketBundleDataVtable130;
    unsigned char m_abTimedSocketBundle134[0x14];
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

static void *g_NETWORK_CompositeEffTransportChannelStateVtable;
static void *g_NETWORK_CompositeEffTransportTimedStreamVtable;
static void *g_NETWORK_CompositeEffTransportDualStreamVtable;
static void *g_NETWORK_CompositeEffTransportDualThunkVtable;
static void *g_NETWORK_CompositeEffTransportVtable;
static void *g_NETWORK_CompositeEffTransportFatalThunkVtable;
static void *g_NETWORK_CompositeEffTransportTimedThunkVtable;
static void *g_NETWORK_CompositeEffTransportDualThunk;

static void *g_NETWORK_SocketWindowEffChannelEmbeddedStateVtable;
static void *g_NETWORK_SocketWindowEffChannelVtable;
static void *g_NETWORK_SocketWindowEffChannelThunkVtable;

static void *g_NETWORK_TimedSocketBundleBaseVtable;
static void *g_NETWORK_TimedSocketBundleTimedStreamVtable;
static void *g_NETWORK_TimedSocketBundleSocketChannelVtable;
static void *g_NETWORK_TimedSocketBundleChannelThunkVtable;
static void *g_NETWORK_TimedSocketBundleTimedThunkVtable;
static void *g_NETWORK_TimedSocketBundleSocketThunkVtable;

static void *g_NETWORK_TcpipCompositeTimedStreamVtable;
static void *g_NETWORK_TcpipCompositeTransportStackVtable;
static void *g_NETWORK_TcpipCompositeDualStreamVtable;
static void *g_NETWORK_TcpipCompositeDualThunkVtable;
static void *g_NETWORK_TcpipCompositeSocketWindowVtable;
static void *g_NETWORK_TcpipCompositeTimedSocketThunkVtable;
static void *g_NETWORK_TcpipCompositeTimedSocketBundleVtable;
static void *g_NETWORK_TcpipCompositeTimedSocketBundleDataVtable;
static void *g_NETWORK_TcpipCompositeRuntimeThunkVtable;
static void *g_NETWORK_TcpipCompositeTimedThunkVtable;
static void *g_NETWORK_TcpipCompositeDualThunk;
static void *g_NETWORK_TcpipCompositeSocketThunkVtable;
static void *g_NETWORK_TcpipCompositeVtable;
static void *g_NETWORK_TcpipCompositeOuterFatalThunkVtable;
static void *g_NETWORK_TcpipCompositeOuterTimedThunkVtable;
static void *g_NETWORK_TcpipCompositeOuterDualThunkVtable;
static void *g_NETWORK_TcpipCompositeOuterSocketThunkVtable;
static void *g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
static void *g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
static void *g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
static void *g_NETWORK_ReturnTrueVtable[1] = {
    (void *)ReturnTrueVtableCallback,
};
static void *g_NETWORK_RuntimeChannelStackFatalThunk;

static void *g_NETWORK_TcpipSocketStackTimedStreamVtable;
static void *g_NETWORK_TcpipSocketStackDualStreamVtable;
static void *g_NETWORK_TcpipSocketStackBaseVtable;
static void *g_NETWORK_TcpipSocketStackDualThunkVtable;
static void *g_NETWORK_TcpipSocketStackTimedSocketBundleVtable;
static void *g_NETWORK_TcpipSocketStackSocketWindowVtable;
static void *g_NETWORK_TcpipSocketStackTimedSocketDataVtable;
static void *g_NETWORK_TcpipSocketStackRuntimeThunkVtable;
static void *g_NETWORK_TcpipSocketStackTimedThunkVtable;
static void *g_NETWORK_TcpipSocketStackDualThunk;
static void *g_NETWORK_TcpipSocketStackSocketThunkVtable;

static void *g_NETWORK_AllocatedTcpipCompositeTransportVtable;
static void *g_NETWORK_AllocatedTcpipCompositeSocketStackVtable;
static void *g_NETWORK_AllocatedTcpipCompositeDualStreamVtable;
static void *g_NETWORK_AllocatedTcpipCompositeDualThunkVtable;
static void *g_NETWORK_AllocatedTcpipCompositeSocketWindowVtable;
static void *g_NETWORK_AllocatedTcpipCompositeTimedSocketBundleVtable;
static void *g_NETWORK_AllocatedTcpipCompositeTimedSocketThunkVtable;
static void *g_NETWORK_AllocatedTcpipCompositeTimedSocketDataVtable;
static void *g_NETWORK_AllocatedTcpipCompositeVtable;
static void *g_NETWORK_AllocatedTcpipCompositeFatalThunkVtable;
static void *g_NETWORK_AllocatedTcpipCompositeTimedThunkVtable;
static void *g_NETWORK_AllocatedTcpipCompositeDualThunk;
static void *g_NETWORK_AllocatedTcpipCompositeSocketThunkVtable;

static const char *g_NETWORK_SocketWindowClassName;
static int *g_pfSocketWindowClassRegistered;
static void *g_NETWORK_InetAddressEntryVtable;
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
extern void SetEffStreamChannelAsyncErrorStatus(void *pChannelState, int nStatus);
extern int ProcessEffTransportPacketHeader(int *pPacketHeader);

extern void *ConstructEffTransportRuntimeChannelStack(void *pChannelStack, int fConstructEmbeddedObjects);
extern void *ConstructEffStreamChannelState(void *pChannelState);
extern void DestroyEffStreamChannelState(void *pChannelState);
extern void *ConstructTimedEffStream(void *pStream, int fConstructChannelState);
extern void DestroyTimedEffStream(void *pStream);
extern void *ConstructDualHandleEffStream(void *pStream, int fConstructChannelState);
extern void DestroyDualHandleEffStream(void *pStream);
extern void InvokeTimedEffStreamServiceCallback(void *pStream, void *pArgument);

void *ConstructTcpipSocketChannelStack(void *pObject, int fConstructEmbeddedObjects);

// FUNCTION: LEMBALL 0x00460A90
void *ConstructCompositeEffTransportStack(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_CompositeEffTransportStackLayout *pComposite;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    char *pbObjectBase;

    pComposite = (NETWORK_CompositeEffTransportStackLayout *)pObject;
    pbObjectBase = (char *)pObject;

    if (fConstructEmbeddedObjects != 0) {
        pComposite->m_pChannelStateOffsets04 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_CompositeEffTransportChannelStateVtable;
        pComposite->m_pTimedStreamOffsetsA4 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_CompositeEffTransportTimedStreamVtable;
        *(void **)(pbObjectBase + 0x11c) = g_NETWORK_CompositeEffTransportDualStreamVtable;
        *(void **)(pbObjectBase + 0x130) = g_NETWORK_CompositeEffTransportDualThunkVtable;

        ConstructEffStreamChannelState(pComposite->m_abChannelState30);
        ConstructTimedEffStream(pComposite->m_abTimedStream60, 0);
        ConstructDualHandleEffStream(pbObjectBase + 0xd8, 0);

        pOffsets = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_CompositeEffTransportDualThunkVtable;
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
    pTertiaryThunk->m_pVtable = g_NETWORK_CompositeEffTransportDualThunk;
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
    return pObject;
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
    pTertiaryThunk->m_pVtable = g_NETWORK_CompositeEffTransportDualThunk;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x2c;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x5c;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xd4;
}

// FUNCTION: LEMBALL 0x0046FCF0
void *ConstructSocketWindowEffChannel(void *pObject, int fConstructEmbeddedState) {
    NETWORK_SocketWindowEffChannel *pChannel;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pChannel = (NETWORK_SocketWindowEffChannel *)pObject;

    if (fConstructEmbeddedState != 0) {
        pChannel->m_pEmbeddedStateVtable = (void **)g_NETWORK_SocketWindowEffChannelEmbeddedStateVtable;
        ConstructEffStreamChannelState(pChannel->m_abChannelState);
    }

    ((PLATFORM_InvisibleMessageWindow *)pChannel)
        ->Construct(g_NETWORK_SocketWindowClassName, g_pfSocketWindowClassRegistered);
    pChannel->m_pVtable = (void **)g_NETWORK_SocketWindowEffChannelVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pChannel->m_pEmbeddedStateVtable;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pChannel + 0x0c + pOffsets->m_nPrimaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_SocketWindowEffChannelThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x10;
    pChannel->m_nSocketHandle = 0;
    pChannel->m_nLookupHandle = 0;
    pChannel->m_nMessageBase = 0x440;
    pChannel->m_nMessageLimit = 0x45f;
    return pObject;
}

// FUNCTION: LEMBALL 0x0046FD70
void DestroySocketWindowEffChannel(int nChannelStateBase) {
    NETWORK_SocketWindowEffChannel *pChannel;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pChannel = (NETWORK_SocketWindowEffChannel *)(unsigned long)(nChannelStateBase - 0x20);
    pChannel->m_pVtable = (void **)g_NETWORK_SocketWindowEffChannelVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pChannel->m_pEmbeddedStateVtable;
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
    return ProcessEffTransportPacketHeader((int *)(pbThis + *(int *)(*(int *)pbThis + 8)));
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
        SetEffStreamChannelAsyncErrorStatus((char *)this + nThisDelta - 0x8c, (int)nError);
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
    SetEffStreamChannelAsyncErrorStatus((char *)this + nChannelDelta + 0x10, (int)nError);
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
    return ProcessEffTransportPacketHeader((int *)((char *)this +
                                                    *(int *)(*(int *)this + 8)));
}

// FUNCTION: LEMBALL 0x00470030
void *ConstructTimedSocketEffChannelBundle(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_TimedSocketEffChannelBundleLayout *pBundle;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    char *pbObjectBase;

    pBundle = (NETWORK_TimedSocketEffChannelBundleLayout *)pObject;
    pbObjectBase = (char *)pObject;

    if (fConstructEmbeddedObjects != 0) {
        pBundle->m_pVtable00 = (void **)g_NETWORK_TimedSocketBundleBaseVtable;
        pBundle->m_pTimedStreamVtable8c = (void **)g_NETWORK_TimedSocketBundleTimedStreamVtable;
        pBundle->m_pSocketChannelVtabled0 = (void **)g_NETWORK_TimedSocketBundleSocketChannelVtable;
        ConstructEffStreamChannelState(pBundle->m_abChannelState18);
        ConstructTimedEffStream(pBundle->m_abTimedStream48, 0);
        ConstructSocketWindowEffChannel(pBundle->m_abSocketWindowC0, 0);
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
    return pObject;
}

// FUNCTION: LEMBALL 0x004700F0
void NETWORK_TimedSocketAsyncLookupAdapter::CaptureAsyncLookupAddressForTimedStream(void *pLookupResult) {
    void *pTimedStream;

    pTimedStream = (void *)(unsigned long)(*(int *)(*(int *)((char *)this - 0x48) + 8) +
                                           (int)(unsigned long)this - 0x48);
    InvokeTimedEffStreamServiceCallback(pTimedStream, pLookupResult);
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
        SetEffStreamChannelAsyncErrorStatus((char *)this + nThisDelta - 0xc0,
                                             (int)(nEvent >> 16));
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
void *ConstructTcpipEffTransportComposite(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_TcpipEffTransportCompositeLayout *pComposite;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;

    pComposite = (NETWORK_TcpipEffTransportCompositeLayout *)pObject;
    if (fConstructEmbeddedObjects != 0) {
        pComposite->m_pOuterOffsets04 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTimedStreamVtable;
        pComposite->m_pTransportOffsets20 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTransportStackVtable;
        pComposite->m_pDualStreamVtableA4 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeDualStreamVtable;
        pComposite->m_pDualThunkOffsets11c = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeDualThunkVtable;
        pComposite->m_pSocketThunkOffsets130 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeSocketWindowVtable;
        pComposite->m_pTimedSocketThunkOffsets148 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTimedSocketThunkVtable;
        pComposite->m_pTimedSocketBundleOffsets154 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipCompositeTimedSocketBundleVtable;
        pComposite->m_pTimedSocketBundleDataVtable158 = (void **)g_NETWORK_TcpipCompositeTimedSocketBundleDataVtable;

        ConstructEffStreamChannelState(pComposite->m_abChannelState30);
        ConstructTimedEffStream(pComposite->m_abTimedStream60, 0);
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

        ConstructSocketWindowEffChannel(pComposite->m_abSocketWindow138, 0);

        pOffsets = pComposite->m_pTimedSocketBundleOffsets154;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x150 + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x150 + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x150 + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
        pPrimaryThunk->m_nThisDelta = 0;

        ConstructTimedSocketEffChannelBundle(&pComposite->m_pTimedSocketBundleDataVtable158, 0);
    }

    ConstructEffTransportRuntimeChannelStack(pObject, 0);
    pOffsets = pComposite->m_pTransportOffsets20;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nTertiaryOffset);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)((char *)pComposite + 0x1c + pOffsets->m_nQuaternaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_TcpipCompositeRuntimeThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TcpipCompositeTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TcpipCompositeDualThunk;
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
    return pObject;
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
    DestroyDualHandleEffStream(pView->m_abDualStreamA8);
    DestroyTimedEffStream(pView->m_abTimedStream30);
    DestroyEffStreamChannelState(pView->m_abChannelState00);
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
    DestroyDualHandleEffStream(pView->m_abDualStreamA8);
    DestroyTimedEffStream(pView->m_abTimedStream30);
    DestroyEffStreamChannelState(pView->m_abChannelState00);
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
    DestroyDualHandleEffStream(pView->m_abDualStreamA8);
    DestroyTimedEffStream(pView->m_abTimedStream30);
    DestroyEffStreamChannelState(pView->m_abChannelState00);
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
    DestroyDualHandleEffStream(pView->m_abDualStream2c);
    DestroyEffStreamChannelState(pView->m_abChannelState00);
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
    DestroyTimedEffStream(pView->m_abTimedStream30);
    DestroyEffStreamChannelState(pView->m_abChannelState00);
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
    DestroyDualHandleEffStream(pView->m_abDualStreamA8);
    DestroyTimedEffStream(pView->m_abTimedStream30);
    DestroyEffStreamChannelState(pView->m_abChannelState00);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x004715E0
void *AllocateTcpipEffTransportComposite(void) {
    NETWORK_TcpipEffTransportCompositeLayout *pComposite;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;
    char *pbObjectBase;

    pComposite = (NETWORK_TcpipEffTransportCompositeLayout *)AllocateVSMemBlock(0x170);
    if (pComposite == 0) {
        return 0;
    }

    pbObjectBase = (char *)pComposite;
    pComposite->m_pOuterOffsets04 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeTransportVtable;
    pComposite->m_pTransportOffsets20 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeSocketStackVtable;
    pComposite->m_pDualStreamVtableA4 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeDualStreamVtable;
    pComposite->m_pDualThunkOffsets11c = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeDualThunkVtable;
    pComposite->m_pSocketThunkOffsets130 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeSocketWindowVtable;
    pComposite->m_pTimedSocketBundleOffsets154 =
        (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeTimedSocketBundleVtable;
    pComposite->m_pTimedSocketThunkOffsets148 =
        (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_AllocatedTcpipCompositeTimedSocketThunkVtable;
    pComposite->m_pTimedSocketBundleDataVtable158 = (void **)g_NETWORK_AllocatedTcpipCompositeTimedSocketDataVtable;

    ConstructEffStreamChannelState(pComposite->m_abChannelState30);
    ConstructTimedEffStream(pComposite->m_abTimedStream60, 0);
    ConstructDualHandleEffStream(pComposite->m_abDualStreamD8, 0);

    pOffsets = pComposite->m_pSocketThunkOffsets130;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x130 + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x130 + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x130 + pOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
    pSecondaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 8;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x38;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 0xb0;

    ConstructSocketWindowEffChannel(pComposite->m_abSocketWindow138, 0);

    pOffsets = pComposite->m_pTimedSocketThunkOffsets148;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x154 + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x154 + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x154 + pOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
    pPrimaryThunk->m_nThisDelta = 0;

    ConstructTimedSocketEffChannelBundle(&pComposite->m_pTimedSocketBundleDataVtable158, 0);
    ConstructCompositeEffTransportStack(pComposite, 0);
    ConstructTcpipSocketChannelStack((char *)pComposite + 0x2c, 0);

    pComposite->m_pVtable00 = (void **)g_NETWORK_AllocatedTcpipCompositeVtable;
    pOffsets = pComposite->m_pOuterOffsets04;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nPrimaryOffset);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nSecondaryOffset);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nTertiaryOffset);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pOffsets->m_nQuaternaryOffset);
    pPrimaryThunk->m_pVtable = g_NETWORK_AllocatedTcpipCompositeFatalThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_AllocatedTcpipCompositeTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_AllocatedTcpipCompositeDualThunk;
    pQuaternaryThunk->m_pVtable = g_NETWORK_AllocatedTcpipCompositeSocketThunkVtable;
    pPrimaryThunk->m_nThisDelta = 0;
    pSecondaryThunk->m_nThisDelta = 0;
    pTertiaryThunk->m_nThisDelta = 0;
    pQuaternaryThunk->m_nThisDelta = 0;
    return pComposite;
}

// FUNCTION: LEMBALL 0x00471830
void *ConstructTcpipSocketChannelStack(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_TcpipSocketChannelStackLayout *pStack;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;
    char *pbObjectBase;

    pStack = (NETWORK_TcpipSocketChannelStackLayout *)pObject;
    pbObjectBase = (char *)pObject;

    if (fConstructEmbeddedObjects != 0) {
        pStack->m_pTimedStreamVtable7c = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackTimedStreamVtable;
        pStack->m_pDualStreamVtablef4 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackDualStreamVtable;
        pStack->m_pBaseOffsets00 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackBaseVtable;
        pStack->m_pDualThunkOffsets108 = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackDualThunkVtable;
        pStack->m_pTimedSocketBundleDataVtable130 = (void **)g_NETWORK_TcpipSocketStackTimedSocketDataVtable;
        pStack->m_pTimedSocketBundleVtable120 =
            (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackTimedSocketBundleVtable;
        pStack->m_pSocketWindowVtable12c = (NETWORK_ConstructionAdjustorVtable *)g_NETWORK_TcpipSocketStackSocketWindowVtable;

        ConstructEffStreamChannelState(pStack->m_abChannelState08);
        ConstructTimedEffStream(pStack->m_abTimedStream38, 0);
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

        ConstructSocketWindowEffChannel(pStack->m_abSocketWindow110, 0);

        pOffsets = pStack->m_pSocketWindowVtable12c;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x128 + pOffsets->m_nPrimaryOffset);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x128 + pOffsets->m_nSecondaryOffset);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x128 + pOffsets->m_nTertiaryOffset);
        pPrimaryThunk->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
        pPrimaryThunk->m_nThisDelta = 0;

        ConstructTimedSocketEffChannelBundle(&pStack->m_pTimedSocketBundleDataVtable130, 0);
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
    return pObject;
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
        SetEffStreamChannelAsyncErrorStatus(pbThis + nDelta - 0x134,
                                             (int)(nEvent >> 16));
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
    DestroyEffStreamChannelState(this);
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
    SetEffStreamChannelAsyncErrorStatus((char *)this + nDelta - 0x10, WSAGetLastError());
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
