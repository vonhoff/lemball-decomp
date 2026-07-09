#include "../game/game_app.h"
#include "../engine/memory_arena.h"
#include "../platform/message_window.h"

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

static void *g_NETWORK_CompositeEffTransportChannelStateVtable = (void *)0x00499128;
static void *g_NETWORK_CompositeEffTransportTimedStreamVtable = (void *)0x00499120;
static void *g_NETWORK_CompositeEffTransportDualStreamVtable = (void *)0x00499118;
static void *g_NETWORK_CompositeEffTransportDualThunkVtable = (void *)0x00499108;
static void *g_NETWORK_CompositeEffTransportVtable = (void *)0x004991A8;
static void *g_NETWORK_CompositeEffTransportFatalThunkVtable = (void *)0x00499198;
static void *g_NETWORK_CompositeEffTransportTimedThunkVtable = (void *)0x00499168;
static void *g_NETWORK_CompositeEffTransportDualThunk = (void *)0x00499140;

static void *g_NETWORK_SocketWindowEffChannelEmbeddedStateVtable = (void *)0x0049A048;
static void *g_NETWORK_SocketWindowEffChannelVtable = (void *)0x0049A060;
static void *g_NETWORK_SocketWindowEffChannelThunkVtable = (void *)0x0049A050;

static void *g_NETWORK_TimedSocketBundleBaseVtable = (void *)0x0049A078;
static void *g_NETWORK_TimedSocketBundleTimedStreamVtable = (void *)0x0049A070;
static void *g_NETWORK_TimedSocketBundleSocketChannelVtable = (void *)0x0049A068;
static void *g_NETWORK_TimedSocketBundleChannelThunkVtable = (void *)0x0049A0C0;
static void *g_NETWORK_TimedSocketBundleTimedThunkVtable = (void *)0x0049A090;
static void *g_NETWORK_TimedSocketBundleSocketThunkVtable = (void *)0x0049A088;

static void *g_NETWORK_TcpipCompositeTimedStreamVtable = (void *)0x0049A138;
static void *g_NETWORK_TcpipCompositeTransportStackVtable = (void *)0x0049A118;
static void *g_NETWORK_TcpipCompositeDualStreamVtable = (void *)0x0049A110;
static void *g_NETWORK_TcpipCompositeDualThunkVtable = (void *)0x0049A108;
static void *g_NETWORK_TcpipCompositeSocketWindowVtable = (void *)0x0049A0F8;
static void *g_NETWORK_TcpipCompositeTimedSocketThunkVtable = (void *)0x0049A0F0;
static void *g_NETWORK_TcpipCompositeTimedSocketBundleVtable = (void *)0x0049A0E0;
static void *g_NETWORK_TcpipCompositeTimedSocketBundleDataVtable = (void *)0x0049A0D0;
static void *g_NETWORK_TcpipCompositeRuntimeThunkVtable = (void *)0x0049A280;
static void *g_NETWORK_TcpipCompositeTimedThunkVtable = (void *)0x0049A250;
static void *g_NETWORK_TcpipCompositeDualThunk = (void *)0x0049A228;
static void *g_NETWORK_TcpipCompositeSocketThunkVtable = (void *)0x0049A220;
static void *g_NETWORK_TcpipCompositeVtable = (void *)0x0049A1C8;
static void *g_NETWORK_TcpipCompositeOuterFatalThunkVtable = (void *)0x0049A1B8;
static void *g_NETWORK_TcpipCompositeOuterTimedThunkVtable = (void *)0x0049A188;
static void *g_NETWORK_TcpipCompositeOuterDualThunkVtable = (void *)0x0049A160;
static void *g_NETWORK_TcpipCompositeOuterSocketThunkVtable = (void *)0x0049A158;
static void *g_NETWORK_TimedSocketBundlePrimaryThunkVtable = (void *)0x0049A210;
static void *g_NETWORK_TimedSocketBundleSecondaryThunkVtable = (void *)0x0049A1E8;
static void *g_NETWORK_TimedSocketBundleTertiaryThunkVtable = (void *)0x0049A1E4;
static void *g_NETWORK_ReturnTrueVtable[1] = {
    (void *)ReturnTrueVtableCallback,
};
static void *g_NETWORK_RuntimeChannelStackFatalThunk = (void *)0x004990D8;

static void *g_NETWORK_TcpipSocketStackTimedStreamVtable = (void *)0x0049A430;
static void *g_NETWORK_TcpipSocketStackDualStreamVtable = (void *)0x0049A428;
static void *g_NETWORK_TcpipSocketStackBaseVtable = (void *)0x0049A438;
static void *g_NETWORK_TcpipSocketStackDualThunkVtable = (void *)0x0049A418;
static void *g_NETWORK_TcpipSocketStackTimedSocketBundleVtable = (void *)0x0049A410;
static void *g_NETWORK_TcpipSocketStackSocketWindowVtable = (void *)0x0049A400;
static void *g_NETWORK_TcpipSocketStackTimedSocketDataVtable = (void *)0x0049A3F0;
static void *g_NETWORK_TcpipSocketStackRuntimeThunkVtable = (void *)0x0049A280;
static void *g_NETWORK_TcpipSocketStackTimedThunkVtable = (void *)0x0049A250;
static void *g_NETWORK_TcpipSocketStackDualThunk = (void *)0x0049A228;
static void *g_NETWORK_TcpipSocketStackSocketThunkVtable = (void *)0x0049A220;

static void *g_NETWORK_AllocatedTcpipCompositeTransportVtable = (void *)0x0049A348;
static void *g_NETWORK_AllocatedTcpipCompositeSocketStackVtable = (void *)0x0049A328;
static void *g_NETWORK_AllocatedTcpipCompositeDualStreamVtable = (void *)0x0049A320;
static void *g_NETWORK_AllocatedTcpipCompositeDualThunkVtable = (void *)0x0049A318;
static void *g_NETWORK_AllocatedTcpipCompositeSocketWindowVtable = (void *)0x0049A308;
static void *g_NETWORK_AllocatedTcpipCompositeTimedSocketBundleVtable = (void *)0x0049A300;
static void *g_NETWORK_AllocatedTcpipCompositeTimedSocketThunkVtable = (void *)0x0049A2F0;
static void *g_NETWORK_AllocatedTcpipCompositeTimedSocketDataVtable = (void *)0x0049A2E0;
static void *g_NETWORK_AllocatedTcpipCompositeVtable = (void *)0x0049A3D8;
static void *g_NETWORK_AllocatedTcpipCompositeFatalThunkVtable = (void *)0x0049A3C8;
static void *g_NETWORK_AllocatedTcpipCompositeTimedThunkVtable = (void *)0x0049A398;
static void *g_NETWORK_AllocatedTcpipCompositeDualThunk = (void *)0x0049A370;
static void *g_NETWORK_AllocatedTcpipCompositeSocketThunkVtable = (void *)0x0049A368;

static const char *g_NETWORK_SocketWindowClassName = (const char *)0x004A2464;
static int *g_pfSocketWindowClassRegistered = (int *)0x004A23B8;

extern "C" DWORD timeGetTime(void);

extern void *g_pEffTransportPeerAddressState;

extern void *ConstructEffTransportRuntimeChannelStack(void *pChannelStack, int fConstructEmbeddedObjects);
extern void ReleaseEffTransportRuntimeBuffers(int nChannelStateBase);
extern void *ConstructEffStreamChannelState(void *pChannelState);
extern void DestroyEffStreamChannelState(void *pChannelState);
extern void *ConstructTimedEffStream(void *pStream, int fConstructChannelState);
extern void DestroyTimedEffStream(void *pStream);
extern void *ConstructDualHandleEffStream(void *pStream, int fConstructChannelState);
extern void DestroyDualHandleEffStream(void *pStream);

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

    ReleaseEffTransportRuntimeBuffers(nObjectBasePlus0x30 - 0xc);
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
