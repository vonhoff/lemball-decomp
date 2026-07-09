#include "../game/game_app.h"
#include "../engine/memory_arena.h"

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

struct NETWORK_EffStreamChannelState {
    void **m_pVtable;
    int m_nReserved04;
    int m_nSelectedPeer08;
    int m_nReserved0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1c;
    unsigned short m_nSelectedPort20;
    unsigned short m_nReserved22;
    int m_nReserved24;
    void *m_pSideBuffer28;
};

struct NETWORK_EffStreamBase {
    void **m_pVtable;
    int m_nReserved04;
    int m_pPayloadBuffer08;
    int m_fOwnsPayload0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nBufferEnd18;
    int m_nReserved1c;
    int m_nReserved20;
    int m_nReserved24;
    int m_fBusy28;
    int m_pTagBuffer2c;
    unsigned short m_nWord30;
    unsigned short m_nWord32;
    unsigned short m_nWord34;
    unsigned short m_nWord36;
};

struct NETWORK_DualHandleEffStream {
    void **m_pVtable;
    int m_nReserved04;
    int m_pPayloadBuffer08;
    int m_fOwnsPayload0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nBufferEnd18;
    int m_nReserved1c;
    int m_nReserved20;
    int m_nReserved24;
    int m_fBusy28;
    int m_nState2c;
    int m_nReserved30;
    int m_nReserved34;
    int m_nReserved38;
    int m_nReserved3c;
    int m_nReserved40;
    void **m_pChannelStateThunk44;
    void *m_pPrimaryHandleArray48;
    void *m_pPrimaryHandleObject4c;
    void *m_pSecondaryHandleArray50;
    void *m_pSecondaryHandleObject54;
    unsigned char m_abChannelState58[0x2c];
};

struct NETWORK_TimedEffStream {
    void **m_pVtable;
    int m_nReserved04;
    int m_pPayloadBuffer08;
    int m_fOwnsPayload0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nBufferEnd18;
    int m_nReserved1c;
    int m_nReserved20;
    int m_nReserved24;
    int m_fBusy28;
    int *m_pTagBuffer2c;
    unsigned short m_nWord30;
    unsigned short m_nWord32;
    unsigned short m_nWord34;
    unsigned short m_nWord36;
    int m_nReserved38;
    DWORD m_dwLastTick3c;
    int m_nReserved40;
    void **m_pChannelStateThunk44;
    void *m_pPrimaryHandleArray48;
    void *m_pSecondaryHandleArray4c;
    void *m_pTertiaryHandleArray50;
    int m_nReserved54;
    int m_nReserved58;
    int m_nHandleIndex5c;
    int m_nReserved60;
    int m_nReserved64;
    int m_nReserved68;
    int m_nReserved6c;
    void *m_pRuntimeService70;
    unsigned char m_abChannelState78[0x2c];
};

struct NETWORK_RuntimeChannelStack {
    void **m_pVtable;
    void **m_pOuterThunk04;
    int m_nReserved08;
    int m_nReserved0c;
    unsigned char *m_pbRuntimeFlags10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1c;
    unsigned char m_abChannelState24[0x30];
    unsigned char m_abTimedStream54[0x78];
    unsigned char m_abDualStreamcc[0x78];
    void **m_pEmbeddedThunk124;
};

struct NETWORK_RuntimeWindowVtable {
    void *m_pReserved00[11];
    void *(*m_pCreateServiceObject)(void);
    void (*m_pPumpMessages)(void *pWindow);
};

static void *g_NETWORK_EffStreamChannelStateVtable = (void *)0x00498F30;
static void *g_NETWORK_EffStreamBaseVtable = (void *)0x00498F40;
static void *g_NETWORK_DualHandleEffStreamChannelStateVtable = (void *)0x00498F58;
static void *g_NETWORK_DualHandleEffStreamPrimaryThunkVtable = (void *)0x00498F60;
static void *g_NETWORK_DualHandleEffStreamVtable = (void *)0x00498F70;
static void *g_NETWORK_TimedEffStreamChannelStateVtable = (void *)0x00498F98;
static void *g_NETWORK_TimedEffStreamPrimaryThunkVtable = (void *)0x00498FA0;
static void *g_NETWORK_TimedEffStreamVtable = (void *)0x00498FB0;
static void *g_NETWORK_RuntimeChannelStackChannelStateThunk = (void *)0x00499048;
static void *g_NETWORK_RuntimeChannelStackTimedStreamThunk = (void *)0x00499058;
static void *g_NETWORK_RuntimeChannelStackDualStreamThunk = (void *)0x00499060;
static void *g_NETWORK_RuntimeChannelStackThunk = (void *)0x00499068;
static void *g_NETWORK_RuntimeChannelStackDualThunk = (void *)0x00499080;
static void *g_NETWORK_RuntimeChannelStackTimedThunk = (void *)0x004990A8;
static void *g_NETWORK_RuntimeChannelStackFatalThunk = (void *)0x004990D8;
static void *g_NETWORK_RuntimeChannelStackVtable = (void *)0x004990E8;

void *g_pEffTransportPeerAddressState = 0;

extern "C" DWORD timeGetTime(void);

extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pEffTransportRuntimeService;
extern void ResetEffStreamStateFields(void *pEffStreamSubobject);

extern void DestroyVSMemObjectPointerArray(void *pObjectArray);

static void *CreateActiveNetworkRuntimeServiceObject(void) {
    return ((NETWORK_RuntimeWindowVtable *)*(void **)g_pActiveNetworkRuntimeWindow)->m_pCreateServiceObject();
}

static void PumpActiveNetworkRuntimeWindow(void) {
    ((NETWORK_RuntimeWindowVtable *)*(void **)g_pActiveNetworkRuntimeWindow)->m_pPumpMessages(g_pActiveNetworkRuntimeWindow);
}

// FUNCTION: LEMBALL 0x0040ABD0
int ReturnTrueVtableCallback(void) {
    return 1;
}

static void *g_NETWORK_ReturnTrueVtable[1] = {
    (void *)ReturnTrueVtableCallback,
};

// FUNCTION: LEMBALL 0x0045F680
void *ConstructEffStreamChannelState(void *pChannelState) {
    NETWORK_EffStreamChannelState *pState;

    pState = (NETWORK_EffStreamChannelState *)pChannelState;
    pState->m_pVtable = (void **)g_NETWORK_EffStreamChannelStateVtable;
    pState->m_nSelectedPeer08 = -1;
    pState->m_nSelectedPort20 = 0xffff;
    pState->m_nReserved10 = 0;
    pState->m_nReserved0c = 0;
    pState->m_nReserved14 = 0;
    pState->m_nReserved18 = 0;
    pState->m_nReserved1c = 0;
    pState->m_nReserved24 = 0;
    pState->m_nReserved04 = 0;
    pState->m_pSideBuffer28 = AllocateVSMemBlock(0x10);
    return pChannelState;
}

// FUNCTION: LEMBALL 0x0045F6C0
void DestroyEffStreamChannelState(void *pChannelState) {
    NETWORK_EffStreamChannelState *pState;

    pState = (NETWORK_EffStreamChannelState *)pChannelState;
    pState->m_pVtable = (void **)g_NETWORK_EffStreamChannelStateVtable;
    FreeVSMemBlock(pState->m_pSideBuffer28);
}

// FUNCTION: LEMBALL 0x0045F750
void *ConstructEffStreamBase(void *pStream) {
    NETWORK_EffStreamBase *pBase;

    pBase = (NETWORK_EffStreamBase *)pStream;
    pBase->m_pVtable = (void **)g_NETWORK_ReturnTrueVtable;
    pBase->m_nReserved04 = 0;
    ResetEffStreamStateFields(pStream);
    pBase->m_nWord34 = 0;
    pBase->m_pVtable = (void **)g_NETWORK_EffStreamBaseVtable;
    pBase->m_nBufferEnd18 = pBase->m_nBufferEnd18 + 0x10;
    pBase->m_nWord36 = 0;
    pBase->m_nWord30 = 0;
    pBase->m_nWord32 = 0;
    return pStream;
}

// FUNCTION: LEMBALL 0x0045EEA0
void DestroyEffStreamBase(void *pStream) {
    NETWORK_EffStreamBase *pBase;
    DWORD dwStartTime;

    pBase = (NETWORK_EffStreamBase *)pStream;
    pBase->m_pVtable = (void **)g_NETWORK_ReturnTrueVtable;
    if (pBase->m_fBusy28 != 0) {
        dwStartTime = timeGetTime();
        while (pBase->m_fBusy28 != 0 && timeGetTime() - dwStartTime < 2000) {
            if (g_pActiveNetworkRuntimeWindow != 0) {
                PumpActiveNetworkRuntimeWindow();
            }
        }
    }
    if (pBase->m_nReserved14 != 0) {
        FreeVSMemBlock((void *)(unsigned long)pBase->m_pPayloadBuffer08);
        pBase->m_pPayloadBuffer08 = 0;
    }
}

// FUNCTION: LEMBALL 0x0045F8D0
void ReleaseEffStreamPrimaryHandleGroup(int nStream) {
    NETWORK_DualHandleEffStream *pStream;
    void *pHandleArray;
    void *pHandleObject;

    pStream = (NETWORK_DualHandleEffStream *)(unsigned long)nStream;
    pHandleArray = pStream->m_pPrimaryHandleArray48;
    if (pHandleArray != 0) {
        DestroyVSMemObjectPointerArray(pHandleArray);
        FreeVSMemBlock(pHandleArray);
    }

    pHandleObject = pStream->m_pPrimaryHandleObject4c;
    if (pHandleObject != 0) {
        ((void (*)(int))**(void ***)pHandleObject)(1);
    }
}

// FUNCTION: LEMBALL 0x0045F900
void ReleaseEffStreamSecondaryHandleGroup(int nStream) {
    NETWORK_DualHandleEffStream *pStream;
    void *pHandleArray;
    void *pHandleObject;

    pStream = (NETWORK_DualHandleEffStream *)(unsigned long)nStream;
    pHandleArray = pStream->m_pSecondaryHandleArray50;
    if (pHandleArray != 0) {
        DestroyVSMemObjectPointerArray(pHandleArray);
        FreeVSMemBlock(pHandleArray);
    }

    pHandleObject = pStream->m_pSecondaryHandleObject54;
    if (pHandleObject != 0) {
        ((void (*)(int))**(void ***)pHandleObject)(1);
    }
}

// FUNCTION: LEMBALL 0x0045F820
void *ConstructDualHandleEffStream(void *pStream, int fConstructChannelState) {
    NETWORK_DualHandleEffStream *pDualStream;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pDualStream = (NETWORK_DualHandleEffStream *)pStream;
    if (fConstructChannelState != 0) {
        pDualStream->m_pChannelStateThunk44 = (void **)g_NETWORK_DualHandleEffStreamChannelStateVtable;
        ConstructEffStreamChannelState(pDualStream->m_abChannelState58);
    }

    ConstructEffStreamBase(pStream);
    pDualStream->m_pVtable = (void **)g_NETWORK_DualHandleEffStreamVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pDualStream->m_pChannelStateThunk44;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pDualStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_DualHandleEffStreamPrimaryThunkVtable;
    pDualStream->m_pPrimaryHandleArray48 = 0;
    pDualStream->m_pPrimaryHandleObject4c = 0;
    pDualStream->m_pSecondaryHandleArray50 = 0;
    pDualStream->m_pSecondaryHandleObject54 = 0;
    pDualStream->m_nState2c = 0;

    if (g_pEffTransportPeerAddressState == 0) {
        g_pEffTransportPeerAddressState = CreateActiveNetworkRuntimeServiceObject();
    }

    return pStream;
}

// FUNCTION: LEMBALL 0x0045F8A0
void DestroyDualHandleEffStream(void *pStream) {
    NETWORK_DualHandleEffStream *pDualStream;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pDualStream = (NETWORK_DualHandleEffStream *)pStream;
    pDualStream->m_pVtable = (void **)g_NETWORK_DualHandleEffStreamVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pDualStream->m_pChannelStateThunk44;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pDualStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_DualHandleEffStreamPrimaryThunkVtable;
    ReleaseEffStreamPrimaryHandleGroup((int)(unsigned long)pStream);
    ReleaseEffStreamSecondaryHandleGroup((int)(unsigned long)pStream);
    DestroyEffStreamBase(pStream);
}

// FUNCTION: LEMBALL 0x0045FDF0
void ReleaseTimedEffStreamPrimaryHandle(int nStream) {
    NETWORK_TimedEffStream *pStream;
    void *pHandleArray;

    pStream = (NETWORK_TimedEffStream *)(unsigned long)nStream;
    pHandleArray = pStream->m_pPrimaryHandleArray48;
    if (pHandleArray != 0) {
        DestroyVSMemObjectPointerArray(pHandleArray);
        FreeVSMemBlock(pHandleArray);
    }
}

// FUNCTION: LEMBALL 0x0045FE10
void ReleaseTimedEffStreamSecondaryHandles(int nStream) {
    NETWORK_TimedEffStream *pStream;
    void *pHandleArray;

    pStream = (NETWORK_TimedEffStream *)(unsigned long)nStream;
    pHandleArray = pStream->m_pSecondaryHandleArray4c;
    if (pHandleArray != 0) {
        DestroyVSMemObjectPointerArray(pHandleArray);
        FreeVSMemBlock(pHandleArray);
    }

    pHandleArray = pStream->m_pTertiaryHandleArray50;
    if (pHandleArray != 0) {
        DestroyVSMemObjectPointerArray(pHandleArray);
        FreeVSMemBlock(pHandleArray);
    }
}

// FUNCTION: LEMBALL 0x0045FCE0
void *ConstructTimedEffStream(void *pStream, int fConstructChannelState) {
    DWORD dwNow;
    int *pHandleArray;
    NETWORK_TimedEffStream *pTimedStream;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pTimedStream = (NETWORK_TimedEffStream *)pStream;
    if (fConstructChannelState != 0) {
        pTimedStream->m_pChannelStateThunk44 = (void **)g_NETWORK_TimedEffStreamChannelStateVtable;
        ConstructEffStreamChannelState(pTimedStream->m_abChannelState78);
    }

    ConstructEffStreamBase(pStream);
    pTimedStream->m_pVtable = (void **)g_NETWORK_TimedEffStreamVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pTimedStream->m_pChannelStateThunk44;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pTimedStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TimedEffStreamPrimaryThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x34;

    dwNow = timeGetTime();
    pTimedStream->m_dwLastTick3c = dwNow - 1000;

    pHandleArray = (int *)AllocateVSMemBlock(0x10);
    pTimedStream->m_pTagBuffer2c = pHandleArray;
    *pHandleArray = 0x56533039;

    pTimedStream->m_pPrimaryHandleArray48 = 0;
    pTimedStream->m_pSecondaryHandleArray4c = 0;
    pTimedStream->m_pTertiaryHandleArray50 = 0;
    pTimedStream->m_nReserved54 = 0;
    pTimedStream->m_nHandleIndex5c = -1;
    pTimedStream->m_pRuntimeService70 = CreateActiveNetworkRuntimeServiceObject();
    return pStream;
}

// FUNCTION: LEMBALL 0x0045FD80
void DestroyTimedEffStream(void *pStream) {
    NETWORK_TimedEffStream *pTimedStream;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;

    pTimedStream = (NETWORK_TimedEffStream *)pStream;
    pTimedStream->m_pVtable = (void **)g_NETWORK_TimedEffStreamVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pTimedStream->m_pChannelStateThunk44;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pTimedStream + 0x44 + pOffsets->m_nPrimaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_TimedEffStreamPrimaryThunkVtable;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x34;

    FreeVSMemBlock(pTimedStream->m_pRuntimeService70);
    pTimedStream->m_pRuntimeService70 = 0;
    FreeVSMemBlock(pTimedStream->m_pTagBuffer2c);
    ReleaseTimedEffStreamPrimaryHandle((int)(unsigned long)pStream);
    ReleaseTimedEffStreamSecondaryHandles((int)(unsigned long)pStream);
    DestroyEffStreamBase(pStream);
}

// FUNCTION: LEMBALL 0x004604E0
void ReleaseEffTransportRuntimeBuffers(int nChannelStateBase) {
    NETWORK_RuntimeChannelStack *pChannelStack;
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;

    pChannelStack = (NETWORK_RuntimeChannelStack *)(unsigned long)(nChannelStateBase - 0x24);
    pChannelStack->m_pVtable = (void **)g_NETWORK_RuntimeChannelStackVtable;
    pOffsets = (NETWORK_ConstructionAdjustorVtable *)pChannelStack->m_pOuterThunk04;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pChannelStack + 4 + pOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)((char *)pChannelStack + 4 + pOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)((char *)pChannelStack + 4 + pOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
    pSecondaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackTimedThunk;
    pTertiaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackDualThunk;
    pPrimaryThunk->m_nThisDelta = pOffsets->m_nPrimaryOffset - 0x20;
    pSecondaryThunk->m_nThisDelta = pOffsets->m_nSecondaryOffset - 0x50;
    pTertiaryThunk->m_nThisDelta = pOffsets->m_nTertiaryOffset - 200;

    if (pChannelStack->m_pbRuntimeFlags10 != 0) {
        FreeVSMemBlock(pChannelStack->m_pbRuntimeFlags10);
    }
    FreeVSMemBlock(pChannelStack->m_pbRuntimeFlags10);
    FreeVSMemBlock(g_pEffTransportRuntimeService);
    g_pEffTransportRuntimeService = 0;
    if (*(void **)&g_pEffTransportPeerAddressState != 0) {
        FreeVSMemBlock(g_pEffTransportPeerAddressState);
        g_pEffTransportPeerAddressState = 0;
    }
    if (*(void **)0x004A1E34 != 0) {
        FreeVSMemBlock(*(void **)0x004A1E34);
        *(int *)0x004A1E34 = 0;
    }
}

// FUNCTION: LEMBALL 0x00460350
void *ConstructEffTransportRuntimeChannelStack(void *pChannelStack, int fConstructEmbeddedObjects) {
    int i;
    NETWORK_RuntimeChannelStack *pStack;
    NETWORK_ConstructionAdjustorVtable *pEmbeddedOffsets;
    NETWORK_ConstructionAdjustorVtable *pOuterOffsets;
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;

    pStack = (NETWORK_RuntimeChannelStack *)pChannelStack;
    if (fConstructEmbeddedObjects != 0) {
        pStack->m_pOuterThunk04 = (void **)g_NETWORK_RuntimeChannelStackThunk;
        *(void **)((char *)pChannelStack + 0x98) = g_NETWORK_RuntimeChannelStackDualStreamThunk;
        *(void **)((char *)pChannelStack + 0x110) = g_NETWORK_RuntimeChannelStackTimedStreamThunk;
        pStack->m_pEmbeddedThunk124 = (void **)g_NETWORK_RuntimeChannelStackChannelStateThunk;

        ConstructEffStreamChannelState(pStack->m_abChannelState24);
        ConstructTimedEffStream(pStack->m_abTimedStream54, 0);
        ConstructDualHandleEffStream(pStack->m_abDualStreamcc, 0);

        pEmbeddedOffsets = (NETWORK_ConstructionAdjustorVtable *)pStack->m_pEmbeddedThunk124;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pStack + 0x124 + pEmbeddedOffsets->m_nPrimaryOffset - 4);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)((char *)pStack + 0x124 + pEmbeddedOffsets->m_nSecondaryOffset - 4);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)((char *)pStack + 0x124 + pEmbeddedOffsets->m_nTertiaryOffset - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackChannelStateThunk;
        pSecondaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pPrimaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nPrimaryOffset - 8;
        pSecondaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nSecondaryOffset - 0x38;
        pTertiaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nTertiaryOffset - 0xb0;
    }

    pStack->m_pVtable = (void **)g_NETWORK_RuntimeChannelStackVtable;
    pOuterOffsets = (NETWORK_ConstructionAdjustorVtable *)pStack->m_pOuterThunk04;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)((char *)pStack + 4 + pOuterOffsets->m_nPrimaryOffset - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)((char *)pStack + 4 + pOuterOffsets->m_nSecondaryOffset - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)((char *)pStack + 4 + pOuterOffsets->m_nTertiaryOffset - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunk;
    pSecondaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackTimedThunk;
    pTertiaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackDualThunk;
    pPrimaryThunk->m_nThisDelta = pOuterOffsets->m_nPrimaryOffset - 0x20;
    pSecondaryThunk->m_nThisDelta = pOuterOffsets->m_nSecondaryOffset - 0x50;
    pTertiaryThunk->m_nThisDelta = pOuterOffsets->m_nTertiaryOffset - 200;

    pStack->m_nReserved14 = 0;
    pStack->m_nReserved08 = 0;
    pStack->m_nReserved0c = 0;
    pStack->m_nReserved1c = 0;

    g_pEffTransportRuntimeService = CreateActiveNetworkRuntimeServiceObject();
    pStack->m_pbRuntimeFlags10 = (unsigned char *)AllocateVSMemBlock(0x200);
    for (i = 0; i < 0x200; ++i) {
        pStack->m_pbRuntimeFlags10[i] = 0;
    }

    return pChannelStack;
}

// FUNCTION: LEMBALL 0x00462BC0
void *DeleteEffTransportRuntimeChannelStackWrapper(void *pChannelState, unsigned char fDelete) {
    ReleaseEffTransportRuntimeBuffers((int)(unsigned long)pChannelState);
    DestroyDualHandleEffStream((char *)pChannelState + 0xa8);
    DestroyTimedEffStream((char *)pChannelState + 0x30);
    DestroyEffStreamChannelState(pChannelState);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock((char *)pChannelState - 0x24);
    }
    return (char *)pChannelState - 0x24;
}
