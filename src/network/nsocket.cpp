#include "../game/game_app.h"
#include "../engine/memory_arena.h"
#include "../platform/message_window.h"

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
static void *g_NETWORK_ReturnTrueVtable = (void *)0x004932C8;
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
    NETWORK_ThunkAdjustorGroup kEmbeddedThunkGroup;
    NETWORK_ThunkAdjustorGroup kOuterThunkGroup;

    if (fConstructEmbeddedObjects != 0) {
        *(void **)((char *)pObject + 4) = g_NETWORK_CompositeEffTransportChannelStateVtable;
        *(void **)((char *)pObject + 0xa4) = g_NETWORK_CompositeEffTransportTimedStreamVtable;
        *(void **)((char *)pObject + 0x11c) = g_NETWORK_CompositeEffTransportDualStreamVtable;
        *(void **)((char *)pObject + 0x130) = g_NETWORK_CompositeEffTransportDualThunkVtable;

        ConstructEffStreamChannelState((char *)pObject + 0x30);
        ConstructTimedEffStream((char *)pObject + 0x60, 0);
        ConstructDualHandleEffStream((char *)pObject + 0xd8, 0);

        kEmbeddedThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)((char *)pObject + 0x130);
        kEmbeddedThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x130 +
                                                                       kEmbeddedThunkGroup.m_pOffsets->m_nPrimaryOffset -
                                                                       4);
        kEmbeddedThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x130 +
                                                                         kEmbeddedThunkGroup.m_pOffsets->m_nSecondaryOffset -
                                                                         4);
        kEmbeddedThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x130 +
                                                                        kEmbeddedThunkGroup.m_pOffsets->m_nTertiaryOffset -
                                                                        4);

        kEmbeddedThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
        kEmbeddedThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_ReturnTrueVtable;
        kEmbeddedThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_ReturnTrueVtable;

        kEmbeddedThunkGroup.m_pPrimary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nPrimaryOffset - 8;
        kEmbeddedThunkGroup.m_pSecondary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x38;
        kEmbeddedThunkGroup.m_pTertiary->m_nThisDelta = kEmbeddedThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xb0;
    }

    *(void **)pObject = g_NETWORK_CompositeEffTransportVtable;
    kOuterThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)((char *)pObject + 4);
    kOuterThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 4 +
                                                                kOuterThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kOuterThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 4 +
                                                                  kOuterThunkGroup.m_pOffsets->m_nSecondaryOffset - 4);
    kOuterThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + 4 +
                                                                 kOuterThunkGroup.m_pOffsets->m_nTertiaryOffset - 4);
    kOuterThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_CompositeEffTransportFatalThunkVtable;
    kOuterThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_CompositeEffTransportTimedThunkVtable;
    kOuterThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_CompositeEffTransportDualThunk;
    kOuterThunkGroup.m_pPrimary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x2c;
    kOuterThunkGroup.m_pSecondary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x5c;
    kOuterThunkGroup.m_pTertiary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xd4;

    *(int *)((char *)pObject + 0xc) = 0;
    *(int *)((char *)pObject + 8) = 0;
    *(int *)((char *)pObject + 0x14) = 0;
    *(int *)((char *)pObject + 0x10) = 0;
    *(int *)((char *)pObject + 0x1c) = 0;
    *(int *)((char *)pObject + 0x20) = 0;
    *(int *)((char *)pObject + 0x24) = 0;
    *(int *)((char *)pObject + 0x18) = 0;
    *(DWORD *)((char *)pObject + 0x28) = timeGetTime();
    return pObject;
}

// FUNCTION: LEMBALL 0x00460C00
void RestoreCompositeEffTransportVtables(int nObjectBasePlus0x30) {
    char *pObject;
    NETWORK_ThunkAdjustorGroup kOuterThunkGroup;

    pObject = (char *)(unsigned long)(nObjectBasePlus0x30 - 0x30);
    *(void **)(pObject) = g_NETWORK_CompositeEffTransportVtable;
    kOuterThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)(pObject + 4);
    kOuterThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pObject + 4 +
                                                                kOuterThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kOuterThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pObject + 4 +
                                                                  kOuterThunkGroup.m_pOffsets->m_nSecondaryOffset - 4);
    kOuterThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pObject + 4 +
                                                                 kOuterThunkGroup.m_pOffsets->m_nTertiaryOffset - 4);
    kOuterThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_CompositeEffTransportFatalThunkVtable;
    kOuterThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_CompositeEffTransportTimedThunkVtable;
    kOuterThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_CompositeEffTransportDualThunk;
    kOuterThunkGroup.m_pPrimary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x2c;
    kOuterThunkGroup.m_pSecondary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x5c;
    kOuterThunkGroup.m_pTertiary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xd4;
}

// FUNCTION: LEMBALL 0x0046FCF0
void *ConstructSocketWindowEffChannel(void *pObject, int fConstructEmbeddedState) {
    NETWORK_SocketWindowEffChannel *pChannel;
    NETWORK_ThunkAdjustorGroup kThunkGroup;

    pChannel = (NETWORK_SocketWindowEffChannel *)pObject;

    if (fConstructEmbeddedState != 0) {
        pChannel->m_pEmbeddedStateVtable = (void **)g_NETWORK_SocketWindowEffChannelEmbeddedStateVtable;
        ConstructEffStreamChannelState(pChannel->m_abChannelState);
    }

    ((PLATFORM_InvisibleMessageWindow *)pChannel)
        ->Construct(g_NETWORK_SocketWindowClassName, g_pfSocketWindowClassRegistered);
    pChannel->m_pVtable = (void **)g_NETWORK_SocketWindowEffChannelVtable;
    kThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)pChannel->m_pEmbeddedStateVtable;
    kThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pChannel + 0x10 +
                                                           kThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_SocketWindowEffChannelThunkVtable;
    kThunkGroup.m_pPrimary->m_nThisDelta = kThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x10;
    pChannel->m_nSocketHandle = 0;
    pChannel->m_nLookupHandle = 0;
    pChannel->m_nMessageBase = 0x440;
    pChannel->m_nMessageLimit = 0x45f;
    return pObject;
}

// FUNCTION: LEMBALL 0x0046FD70
void DestroySocketWindowEffChannel(int nChannelStateBase) {
    NETWORK_SocketWindowEffChannel *pChannel;
    NETWORK_ThunkAdjustorGroup kThunkGroup;

    pChannel = (NETWORK_SocketWindowEffChannel *)(unsigned long)(nChannelStateBase - 0x20);
    pChannel->m_pVtable = (void **)g_NETWORK_SocketWindowEffChannelVtable;
    kThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)pChannel->m_pEmbeddedStateVtable;
    kThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pChannel + 0x10 +
                                                           kThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_SocketWindowEffChannelThunkVtable;
    kThunkGroup.m_pPrimary->m_nThisDelta = kThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x10;

    if (pChannel->m_nLookupHandle != 0) {
        FreeVSMemBlock((void *)(unsigned long)pChannel->m_nLookupHandle);
    }

    DestroyInvisibleMessageWindow((PLATFORM_InvisibleMessageWindow *)pChannel);
}

// FUNCTION: LEMBALL 0x00470030
void *ConstructTimedSocketEffChannelBundle(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_TimedSocketEffChannelBundleHeader *pBundle;
    NETWORK_ThunkAdjustorGroup kThunkGroup;

    pBundle = (NETWORK_TimedSocketEffChannelBundleHeader *)pObject;

    if (fConstructEmbeddedObjects != 0) {
        pBundle->m_pVtable = (void **)g_NETWORK_TimedSocketBundleBaseVtable;
        *(void **)((char *)pObject + 0x8c) = g_NETWORK_TimedSocketBundleTimedStreamVtable;
        *(void **)((char *)pObject + 0xd0) = g_NETWORK_TimedSocketBundleSocketChannelVtable;
        ConstructEffStreamChannelState((char *)pObject + 0x18);
        ConstructTimedEffStream((char *)pObject + 0x48, 0);
        ConstructSocketWindowEffChannel((char *)pObject + 0xc0, 0);
    }

    kThunkGroup.m_pOffsets = (NETWORK_ConstructionAdjustorVtable *)pBundle->m_pVtable;
    kThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + kThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + kThunkGroup.m_pOffsets->m_nSecondaryOffset - 4);
    kThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + kThunkGroup.m_pOffsets->m_nTertiaryOffset - 4);
    kThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_TimedSocketBundleChannelThunkVtable;
    kThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_TimedSocketBundleTimedThunkVtable;
    kThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_TimedSocketBundleSocketThunkVtable;
    kThunkGroup.m_pPrimary->m_nThisDelta = kThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x18;
    kThunkGroup.m_pSecondary->m_nThisDelta = kThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x48;
    kThunkGroup.m_pTertiary->m_nThisDelta = kThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xc0;
    pBundle->m_nMode = 2;
    return pObject;
}

// FUNCTION: LEMBALL 0x00470270
void *ConstructTcpipEffTransportComposite(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_ThunkAdjustorGroup kSocketWindowThunkGroup;
    NETWORK_ThunkAdjustorGroup kTimedSocketThunkGroup;
    NETWORK_QuadThunkAdjustorGroup kRuntimeThunkGroup;
    NETWORK_QuadThunkAdjustorGroup kOuterThunkGroup;
    if (fConstructEmbeddedObjects != 0) {
        *(void **)((char *)pObject + 4) = g_NETWORK_TcpipCompositeTimedStreamVtable;
        *(void **)((char *)pObject + 0x20) = g_NETWORK_TcpipCompositeTransportStackVtable;
        *(void **)((char *)pObject + 0xa4) = g_NETWORK_TcpipCompositeDualStreamVtable;
        *(void **)((char *)pObject + 0x11c) = g_NETWORK_TcpipCompositeDualThunkVtable;
        *(void **)((char *)pObject + 0x130) = g_NETWORK_TcpipCompositeSocketWindowVtable;
        *(void **)((char *)pObject + 0x148) = g_NETWORK_TcpipCompositeTimedSocketThunkVtable;
        *(void **)((char *)pObject + 0x154) = g_NETWORK_TcpipCompositeTimedSocketBundleVtable;
        *(void **)((char *)pObject + 0x158) = g_NETWORK_TcpipCompositeTimedSocketBundleDataVtable;

        ConstructEffStreamChannelState((char *)pObject + 0x30);
        ConstructTimedEffStream((char *)pObject + 0x60, 0);
        ConstructDualHandleEffStream((char *)pObject + 0xd8, 0);

        kSocketWindowThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)((char *)pObject + 0x130);
        kSocketWindowThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x130 +
                                                                           kSocketWindowThunkGroup.m_pOffsets->m_nPrimaryOffset -
                                                                           4);
        kSocketWindowThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x130 +
                                                                             kSocketWindowThunkGroup.m_pOffsets->m_nSecondaryOffset -
                                                                             4);
        kSocketWindowThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x130 +
                                                                            kSocketWindowThunkGroup.m_pOffsets->m_nTertiaryOffset -
                                                                            4);

        kSocketWindowThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
        kSocketWindowThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_ReturnTrueVtable;
        kSocketWindowThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_ReturnTrueVtable;

        kSocketWindowThunkGroup.m_pPrimary->m_nThisDelta = kSocketWindowThunkGroup.m_pOffsets->m_nPrimaryOffset - 8;
        kSocketWindowThunkGroup.m_pSecondary->m_nThisDelta = kSocketWindowThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x38;
        kSocketWindowThunkGroup.m_pTertiary->m_nThisDelta = kSocketWindowThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xb0;

        ConstructSocketWindowEffChannel((char *)pObject + 0x138, 0);
        kTimedSocketThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)((char *)pObject + 0x154);
        kTimedSocketThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x154 +
                                                                          kTimedSocketThunkGroup.m_pOffsets->m_nPrimaryOffset -
                                                                          4);
        kTimedSocketThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x154 +
                                                                            kTimedSocketThunkGroup.m_pOffsets->m_nSecondaryOffset -
                                                                            4);
        kTimedSocketThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x154 +
                                                                           kTimedSocketThunkGroup.m_pOffsets->m_nTertiaryOffset -
                                                                           4);
        kTimedSocketThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
        kTimedSocketThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
        kTimedSocketThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
        kTimedSocketThunkGroup.m_pPrimary->m_nThisDelta = 0;
        ConstructTimedSocketEffChannelBundle((char *)pObject + 0x158, 0);
    }

    ConstructEffTransportRuntimeChannelStack(pObject, 0);
    kRuntimeThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)((char *)pObject + 0x20);
    kRuntimeThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x20 +
                                                                  kRuntimeThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kRuntimeThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x20 +
                                                                    kRuntimeThunkGroup.m_pOffsets->m_nSecondaryOffset - 4);
    kRuntimeThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x20 +
                                                                   kRuntimeThunkGroup.m_pOffsets->m_nTertiaryOffset - 4);
    kRuntimeThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x20 +
                                                                     kRuntimeThunkGroup.m_pOffsets->m_nQuaternaryOffset -
                                                                     4);
    kRuntimeThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_TcpipCompositeRuntimeThunkVtable;
    kRuntimeThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_TcpipCompositeTimedThunkVtable;
    kRuntimeThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_TcpipCompositeDualThunk;
    kRuntimeThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_TcpipCompositeSocketThunkVtable;
    kRuntimeThunkGroup.m_pPrimary->m_nThisDelta = 0;
    kRuntimeThunkGroup.m_pSecondary->m_nThisDelta = 0;
    kRuntimeThunkGroup.m_pTertiary->m_nThisDelta = 0;
    kRuntimeThunkGroup.m_pQuaternary->m_nThisDelta = 0;

    *(void **)pObject = g_NETWORK_TcpipCompositeVtable;
    kOuterThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)((char *)pObject + 4);
    kOuterThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 4 +
                                                                kOuterThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kOuterThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 4 +
                                                                  kOuterThunkGroup.m_pOffsets->m_nSecondaryOffset - 4);
    kOuterThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + 4 +
                                                                 kOuterThunkGroup.m_pOffsets->m_nTertiaryOffset - 4);
    kOuterThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)((char *)pObject + 4 +
                                                                   kOuterThunkGroup.m_pOffsets->m_nQuaternaryOffset - 4);
    kOuterThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_TcpipCompositeOuterFatalThunkVtable;
    kOuterThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_TcpipCompositeOuterTimedThunkVtable;
    kOuterThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_TcpipCompositeOuterDualThunkVtable;
    kOuterThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_TcpipCompositeOuterSocketThunkVtable;
    kOuterThunkGroup.m_pPrimary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x2c;
    kOuterThunkGroup.m_pSecondary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x5c;
    kOuterThunkGroup.m_pTertiary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xd4;
    kOuterThunkGroup.m_pQuaternary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nQuaternaryOffset - 0x134;

    *(int *)((char *)pObject + 0x24) = 0;
    *(int *)((char *)pObject + 0x28) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x004704E0
void DestroyTcpipEffTransportComposite(int nObjectBasePlus0x30) {
    char *pObject;
    NETWORK_QuadThunkAdjustorGroup kOuterThunkGroup;

    pObject = (char *)(unsigned long)(nObjectBasePlus0x30 - 0x30);
    *(void **)pObject = g_NETWORK_TcpipCompositeVtable;
    kOuterThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)(pObject + 4);
    kOuterThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pObject + 4 +
                                                                kOuterThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kOuterThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pObject + 4 +
                                                                  kOuterThunkGroup.m_pOffsets->m_nSecondaryOffset - 4);
    kOuterThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pObject + 4 +
                                                                 kOuterThunkGroup.m_pOffsets->m_nTertiaryOffset - 4);
    kOuterThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)(pObject + 4 +
                                                                   kOuterThunkGroup.m_pOffsets->m_nQuaternaryOffset - 4);
    kOuterThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_TcpipCompositeOuterFatalThunkVtable;
    kOuterThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_TcpipCompositeOuterTimedThunkVtable;
    kOuterThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_TcpipCompositeOuterDualThunkVtable;
    kOuterThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_TcpipCompositeOuterSocketThunkVtable;
    kOuterThunkGroup.m_pPrimary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nPrimaryOffset - 0x2c;
    kOuterThunkGroup.m_pSecondary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x5c;
    kOuterThunkGroup.m_pTertiary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xd4;
    kOuterThunkGroup.m_pQuaternary->m_nThisDelta = kOuterThunkGroup.m_pOffsets->m_nQuaternaryOffset - 0x134;

    if (*(void **)(pObject + 0x24) != 0) {
        FreeVSMemBlock(*(void **)(pObject + 0x24));
    }

    ReleaseEffTransportRuntimeBuffers(nObjectBasePlus0x30 - 0xc);
}

// FUNCTION: LEMBALL 0x00462DB0
void *DeleteCompositeEffTransportStackWrapper(void *pObject, BYTE fFreeMemory) {
    RestoreCompositeEffTransportVtables((int)(unsigned long)pObject);
    DestroyDualHandleEffStream((char *)pObject + 0xa8);
    DestroyTimedEffStream((char *)pObject + 0x30);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x30);
    }
    return (char *)pObject - 0x30;
}

// FUNCTION: LEMBALL 0x00471B10
void *DeleteCompositeTcpipSocketChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    RestoreCompositeEffTransportVtables((int)(unsigned long)pObject - 4);
    DestroySocketWindowEffChannel((int)(unsigned long)pObject + 0x128);
    DestroyDualHandleEffStream((char *)pObject + 0xa8);
    DestroyTimedEffStream((char *)pObject + 0x30);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x34);
    }
    return (char *)pObject - 0x34;
}

// FUNCTION: LEMBALL 0x00471CD0
void *DeleteTcpipSocketChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    DestroySocketWindowEffChannel((int)(unsigned long)pObject + 0x128);
    DestroyDualHandleEffStream((char *)pObject + 0xa8);
    DestroyTimedEffStream((char *)pObject + 0x30);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 8);
    }
    return (char *)pObject - 8;
}

// FUNCTION: LEMBALL 0x00471DE0
void *DeleteDualSocketWindowChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    DestroySocketWindowEffChannel((int)(unsigned long)pObject + 0xa4);
    DestroyDualHandleEffStream((char *)pObject + 0x2c);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 8);
    }
    return (char *)pObject - 8;
}

// FUNCTION: LEMBALL 0x00471E80
void *DeleteTimedSocketWindowChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    DestroySocketWindowEffChannel((int)(unsigned long)pObject + 200);
    DestroyTimedEffStream((char *)pObject + 0x30);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x18);
    }
    return (char *)pObject - 0x18;
}

// FUNCTION: LEMBALL 0x00471F60
void *DeleteTcpipEffTransportCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    DestroyTcpipEffTransportComposite((int)(unsigned long)pObject);
    DestroySocketWindowEffChannel((int)(unsigned long)pObject + 0x128);
    DestroyDualHandleEffStream((char *)pObject + 0xa8);
    DestroyTimedEffStream((char *)pObject + 0x30);
    DestroyEffStreamChannelState(pObject);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x30);
    }
    return (char *)pObject - 0x30;
}

// FUNCTION: LEMBALL 0x004715E0
void *AllocateTcpipEffTransportComposite(void) {
    int *pObject;
    char *pbObject;
    NETWORK_ThunkAdjustorGroup kSocketWindowThunkGroup;
    NETWORK_ThunkAdjustorGroup kTimedSocketThunkGroup;
    NETWORK_QuadThunkAdjustorGroup kOuterThunkGroup;

    pObject = (int *)AllocateVSMemBlock(0x170);
    if (pObject == 0) {
        return 0;
    }
    pbObject = (char *)pObject;

    pObject[1] = (int)(unsigned long)g_NETWORK_AllocatedTcpipCompositeTransportVtable;
    pObject[0xb] = (int)(unsigned long)g_NETWORK_AllocatedTcpipCompositeSocketStackVtable;
    pObject[0x2a] = (int)(unsigned long)g_NETWORK_AllocatedTcpipCompositeDualStreamVtable;
    pObject[0x48] = (int)(unsigned long)g_NETWORK_AllocatedTcpipCompositeDualThunkVtable;
    pObject[0x4d] = (int)(unsigned long)g_NETWORK_AllocatedTcpipCompositeSocketWindowVtable;
    pObject[0x53] = (int)(unsigned long)g_NETWORK_AllocatedTcpipCompositeTimedSocketBundleVtable;
    pObject[0x56] = (int)(unsigned long)g_NETWORK_AllocatedTcpipCompositeTimedSocketThunkVtable;
    pObject[0x57] = (int)(unsigned long)g_NETWORK_AllocatedTcpipCompositeTimedSocketDataVtable;

    ConstructEffStreamChannelState(pObject + 0xd);
    ConstructTimedEffStream(pObject + 0x19, 0);
    ConstructDualHandleEffStream(pObject + 0x37, 0);

    kSocketWindowThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)(pbObject + 0x134);
    kSocketWindowThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbObject + 0x134 +
                                                                       kSocketWindowThunkGroup.m_pOffsets->m_nPrimaryOffset -
                                                                       4);
    kSocketWindowThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbObject + 0x134 +
                                                                         kSocketWindowThunkGroup.m_pOffsets->m_nSecondaryOffset -
                                                                         4);
    kSocketWindowThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbObject + 0x134 +
                                                                        kSocketWindowThunkGroup.m_pOffsets->m_nTertiaryOffset -
                                                                        4);
    kSocketWindowThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
    kSocketWindowThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_ReturnTrueVtable;
    kSocketWindowThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_ReturnTrueVtable;
    kSocketWindowThunkGroup.m_pPrimary->m_nThisDelta = kSocketWindowThunkGroup.m_pOffsets->m_nPrimaryOffset - 8;
    kSocketWindowThunkGroup.m_pSecondary->m_nThisDelta = kSocketWindowThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x38;
    kSocketWindowThunkGroup.m_pTertiary->m_nThisDelta = kSocketWindowThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xb0;

    ConstructSocketWindowEffChannel(pObject + 0x4f, 0);
    kTimedSocketThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)(pbObject + 0x158);
    kTimedSocketThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbObject + 0x158 +
                                                                      kTimedSocketThunkGroup.m_pOffsets->m_nPrimaryOffset -
                                                                      4);
    kTimedSocketThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbObject + 0x158 +
                                                                        kTimedSocketThunkGroup.m_pOffsets->m_nSecondaryOffset -
                                                                        4);
    kTimedSocketThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbObject + 0x158 +
                                                                       kTimedSocketThunkGroup.m_pOffsets->m_nTertiaryOffset -
                                                                       4);
    kTimedSocketThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
    kTimedSocketThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
    kTimedSocketThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
    kTimedSocketThunkGroup.m_pPrimary->m_nThisDelta = 0;
    ConstructTimedSocketEffChannelBundle(pObject + 0x57, 0);
    ConstructCompositeEffTransportStack(pObject, 0);
    ConstructTcpipSocketChannelStack(pObject + 0xb, 0);

    pObject[0] = (int)(unsigned long)g_NETWORK_AllocatedTcpipCompositeVtable;
    kOuterThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)(pbObject + 4);
    kOuterThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)(pbObject + 4 +
                                                                kOuterThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kOuterThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)(pbObject + 4 +
                                                                  kOuterThunkGroup.m_pOffsets->m_nSecondaryOffset - 4);
    kOuterThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)(pbObject + 4 +
                                                                 kOuterThunkGroup.m_pOffsets->m_nTertiaryOffset - 4);
    kOuterThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)(pbObject + 4 +
                                                                   kOuterThunkGroup.m_pOffsets->m_nQuaternaryOffset - 4);
    kOuterThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_AllocatedTcpipCompositeFatalThunkVtable;
    kOuterThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_AllocatedTcpipCompositeTimedThunkVtable;
    kOuterThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_AllocatedTcpipCompositeDualThunk;
    kOuterThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_AllocatedTcpipCompositeSocketThunkVtable;
    kOuterThunkGroup.m_pPrimary->m_nThisDelta = 0;
    kOuterThunkGroup.m_pSecondary->m_nThisDelta = 0;
    kOuterThunkGroup.m_pTertiary->m_nThisDelta = 0;
    kOuterThunkGroup.m_pQuaternary->m_nThisDelta = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00471830
void *ConstructTcpipSocketChannelStack(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_ThunkAdjustorGroup kDualThunkGroup;
    NETWORK_ThunkAdjustorGroup kSocketWindowThunkGroup;
    NETWORK_QuadThunkAdjustorGroup kRuntimeThunkGroup;

    if (fConstructEmbeddedObjects != 0) {
        *(void **)((char *)pObject + 0x7c) = g_NETWORK_TcpipSocketStackTimedStreamVtable;
        *(void **)((char *)pObject + 0xf4) = g_NETWORK_TcpipSocketStackDualStreamVtable;
        *(void **)pObject = g_NETWORK_TcpipSocketStackBaseVtable;
        *(void **)((char *)pObject + 0x108) = g_NETWORK_TcpipSocketStackDualThunkVtable;
        *(void **)((char *)pObject + 0x130) = g_NETWORK_TcpipSocketStackTimedSocketDataVtable;
        *(void **)((char *)pObject + 0x120) = g_NETWORK_TcpipSocketStackTimedSocketBundleVtable;
        *(void **)((char *)pObject + 300) = g_NETWORK_TcpipSocketStackSocketWindowVtable;

        ConstructEffStreamChannelState((char *)pObject + 8);
        ConstructTimedEffStream((char *)pObject + 0x38, 0);
        ConstructDualHandleEffStream((char *)pObject + 0xb0, 0);

        kDualThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)((char *)pObject + 0x108);
        kDualThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x108 +
                                                                   kDualThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
        kDualThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x108 +
                                                                     kDualThunkGroup.m_pOffsets->m_nSecondaryOffset - 4);
        kDualThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + 0x108 +
                                                                    kDualThunkGroup.m_pOffsets->m_nTertiaryOffset - 4);
        kDualThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
        kDualThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_ReturnTrueVtable;
        kDualThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_ReturnTrueVtable;
        kDualThunkGroup.m_pPrimary->m_nThisDelta = kDualThunkGroup.m_pOffsets->m_nPrimaryOffset - 8;
        kDualThunkGroup.m_pSecondary->m_nThisDelta = kDualThunkGroup.m_pOffsets->m_nSecondaryOffset - 0x38;
        kDualThunkGroup.m_pTertiary->m_nThisDelta = kDualThunkGroup.m_pOffsets->m_nTertiaryOffset - 0xb0;

        ConstructSocketWindowEffChannel((char *)pObject + 0x110, 0);
        kSocketWindowThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)((char *)pObject + 300);
        kSocketWindowThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject + 300 +
                                                                           kSocketWindowThunkGroup.m_pOffsets->m_nPrimaryOffset -
                                                                           4);
        kSocketWindowThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject + 300 +
                                                                             kSocketWindowThunkGroup.m_pOffsets->m_nSecondaryOffset -
                                                                             4);
        kSocketWindowThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject + 300 +
                                                                            kSocketWindowThunkGroup.m_pOffsets->m_nTertiaryOffset -
                                                                            4);
        kSocketWindowThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_TimedSocketBundlePrimaryThunkVtable;
        kSocketWindowThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_TimedSocketBundleSecondaryThunkVtable;
        kSocketWindowThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_TimedSocketBundleTertiaryThunkVtable;
        kSocketWindowThunkGroup.m_pPrimary->m_nThisDelta = 0;
        ConstructTimedSocketEffChannelBundle((char *)pObject + 0x130, 0);
    }

    kRuntimeThunkGroup.m_pOffsets = *(NETWORK_ConstructionAdjustorVtable **)pObject;
    kRuntimeThunkGroup.m_pPrimary = (NETWORK_AdjustorSubobject *)((char *)pObject +
                                                                  kRuntimeThunkGroup.m_pOffsets->m_nPrimaryOffset - 4);
    kRuntimeThunkGroup.m_pSecondary = (NETWORK_AdjustorSubobject *)((char *)pObject +
                                                                    kRuntimeThunkGroup.m_pOffsets->m_nSecondaryOffset - 4);
    kRuntimeThunkGroup.m_pTertiary = (NETWORK_AdjustorSubobject *)((char *)pObject +
                                                                   kRuntimeThunkGroup.m_pOffsets->m_nTertiaryOffset - 4);
    kRuntimeThunkGroup.m_pQuaternary = (NETWORK_AdjustorSubobject *)((char *)pObject +
                                                                     kRuntimeThunkGroup.m_pOffsets->m_nQuaternaryOffset - 4);
    kRuntimeThunkGroup.m_pPrimary->m_pVtable = g_NETWORK_TcpipSocketStackRuntimeThunkVtable;
    kRuntimeThunkGroup.m_pSecondary->m_pVtable = g_NETWORK_TcpipSocketStackTimedThunkVtable;
    kRuntimeThunkGroup.m_pTertiary->m_pVtable = g_NETWORK_TcpipSocketStackDualThunk;
    kRuntimeThunkGroup.m_pQuaternary->m_pVtable = g_NETWORK_TcpipSocketStackSocketThunkVtable;
    kRuntimeThunkGroup.m_pPrimary->m_nThisDelta = 0;
    kRuntimeThunkGroup.m_pSecondary->m_nThisDelta = 0;
    kRuntimeThunkGroup.m_pTertiary->m_nThisDelta = 0;
    kRuntimeThunkGroup.m_pQuaternary->m_nThisDelta = 0;
    return pObject;
}
