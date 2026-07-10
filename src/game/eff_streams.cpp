#include "../game/game_app.h"
#include "../engine/memory_arena.h"

static void *g_EFF_BaseStreamVtable;
static void *g_EFF_NetworkLobbySelectedPeerStatusVtable;
static void *g_EFF_NetworkLobbyPeerEntryVtable;
static void *g_EFF_NetworkLobbyVsnetRuntimeVtable;
static void *g_EFF_PayloadSize8Vtable;
static void *g_EFF_NetworkLobbyU32PayloadVtable;
static void *g_EFF_NetworkLobbyPeerClearCloseVtable;
static void *g_EFF_NetworkLobbyPeerDirtyConfirmVtable;
static const char g_EFF_DefaultNetworkPath[] = "t:\\network";
void *g_pEffTransportDispatchQueue = 0;
void *g_pEffTransportSecondaryDispatchQueue = 0;

struct GAME_EffStreamVtable {
    void *m_pReserved00;
    int (*m_pLoadFromMemoryHeader)(void);
    void (*m_pLoadFromMemoryBody)(void);
    void (*m_pBeginWriteHeader)(void);
    void (*m_pBeginWriteBody)(void);
    void (*m_pDelete)(int);
};

struct GAME_EffStream {
    GAME_EffStreamVtable *m_pVtable;
    int m_nEventCode;
    int m_pvOwnedBuffer;
    int m_pvBufferEnd;
    int m_cWriteSessions;
    int m_fOwnsBuffer;
    int m_cbSerializedLength;
    int m_pvWriteCursor;
    int m_pvReadCursor;
    int m_fHasPayload;
    int m_nQueuedPayload;
    int m_nU32Payload;
};

struct GAME_NetworkLobbyPeerEntryStream {
    GAME_EffStream m_Base;
    unsigned char m_fAcceptedPeer;
    char m_abUnknown2d[0x1f];
    int m_nPeerEntryState;
};

struct GAME_DeleteObject {
    GAME_EffStreamVtable *m_pVtable;
};

struct GAME_RecordSlotTable {
    int m_cSlots;
    void *m_apSlots[1];
};

struct GAME_EffTransportPeerDescriptor {
    void *m_pReserved00;
    int m_nEndpointStateSlotOffset;
    int m_nPayloadSenderSlotOffset;
    int m_nReserved0C;
    int m_nHandleGroupAdjustorSlotOffset;
};

struct GAME_EffTransportHandleCallbackVtable {
    int (*m_pBeginReset)(void);
    void *m_pReserved04;
    void (*m_pFinishReset)(void);
    void (*m_pNotifyClosed)(int);
};

struct GAME_EffTransportHandleCallback {
    GAME_EffTransportHandleCallbackVtable *m_pVtable;
};

struct GAME_EffTransportHandleGroup {
    GAME_EffTransportHandleCallback *m_pCallback;
    int m_nReserved04;
    int m_nReserved08;
    int m_fPrimaryActive;
    int m_fSecondaryActive;
};

struct GAME_EffTransportEndpointState {
    int m_nReserved00;
    GAME_EffTransportHandleGroup m_HandleGroup;
    int m_fPrimaryHandlePresent;
    int m_fSecondaryHandlePresent;
    char m_abUnknown18[0x0c];
    short m_nAssignedPort;
};

struct GAME_EffTransportEndpointSlot {
    char m_abUnknown00[4];
    GAME_EffTransportEndpointState m_State;
};

struct GAME_EffTransportHandleOwner {
    char m_abUnknown00[0x48];
    void *m_pShiftedRecordSlotTable;
};

struct GAME_EffTransportHandleOwnerSlot {
    char m_abUnknown00[4];
    GAME_EffTransportHandleOwner m_HandleOwner;
};

struct GAME_EffTransportPeer {
    void **m_pVtable;
    GAME_EffTransportPeerDescriptor *m_pDescriptor;
    char *m_pszName;
    char *m_pszAddress;
    GAME_EffTransportPeer *m_pNext;
    GAME_EffTransportPeer *m_pPrevious;
    int m_cNewPortRequests;
    int m_nReserved1C;
    int m_fClosed;
    int m_fConnectComplete;
    DWORD m_dwConnectStartTick;
    int m_pConnectionState;
};

struct GAME_NetworkLobbyVsnetRuntime {
    void *m_pVtable;
    char m_abUnknown04[0x0c];
    GAME_EffStream *m_pPeerEntryWriteStream;
    GAME_EffStream *m_pPeerEntryTable;
    GAME_EffStream *m_pSelectedPeerStatusStream;
    GAME_EffStream *m_pPeerClearCloseStream;
    char m_abUnknown20[0x28];
    int m_nPrimaryHandleCount;
    int m_nPrimaryHandleStride;
    int m_nPrimaryHandleFlags;
    int m_nSecondaryHandleCount;
    int m_nSecondaryHandleStride;
    int m_fFileBasedThreadStarted;
    int m_fTcpipThreadStarted;
    int m_nDesiredSelectedPeerStatus;
    int m_nObservedSelectedPeerStatus;
    int m_dwSelectedPeerStatusTick;
    int m_fSelectedPeerDisconnected;
    int m_fSelectionNeedsReset;
};

struct GAME_EffTransportRuntimeWindow {
    void **m_pVtable;
    char m_abUnknown04[0x14];
    int m_fRuntimeActive;
    char m_abUnknown1C[0x20];
    void *m_pPrimaryDispatchClient;
    void *m_pSecondaryDispatchClient;
    int m_nPrimaryHandleCount;
    int m_nPrimaryHandleStride;
    int m_nPrimaryHandleFlags;
    int m_nSecondaryHandleCount;
    int m_nSecondaryHandleStride;
};

struct GAME_EffTransportRuntimeWindowVtable {
    void *m_pReserved00[8];
    void (*m_pServiceRuntime)(void);
};

struct GAME_PeerEntryStreamTable {
    int m_cEntries;
    GAME_EffStream m_aEntries[10];
};

struct GAME_EffStreamDeleteVtable {
    void *m_pReserved00[5];
    void (*m_pDelete)(int);
};

struct GAME_ShiftedRecordSlot {
    int m_nReserved00;
    int m_pSerializedPayload;
    void **m_pLockVtable;
    char m_abUnknown0c[0x18];
    int m_fHasPayload;
};

struct GAME_ShiftedRecordLockVtable {
    void (*m_pLock)(void);
    void (*m_pUnlock)(void);
};

struct GAME_RenderQueueServiceVtable {
    void *m_pReserved00[2];
    void (*m_pQueueWriteEvent)(void *pEvent);
};

struct GAME_EffQueuedWriteEvent {
    unsigned short m_nEventType;
    unsigned short m_aPadding[3];
    int m_fWrite;
    void *m_pStream;
    int m_nPayload;
};

struct GAME_NonZrleVariantRenderEntryInitializeVtable {
    char m_abUnknown00[0x18];
    void (*m_pDelete)(int);
};

extern "C" DWORD timeGetTime(void);

extern int g_nSelectedNetworkLobbyPeerId;
extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pNonZrleVariantRenderEntryInitializeVtable[2];
extern void InitializeRenderQueueNodeBase(void *pRenderQueueNode);
extern int StartFileBasedNetworkMessageThread(void);
extern int StartTcpipNetworkMessageThread(void);
extern void WINAPI ConfigureFileBasedNetworkPathsWrapper(char *pszBasePath, char *pszNetworkPath);
extern int StartEffTransportRuntimeAndWaitReady(void *pRuntimeWindow, int nRuntimeKey, int cbMaxPacket);
extern int StopFileBasedNetworkMessageThread(void);
extern int StopTcpipNetworkMessageThread(void);
void ResetEffStreamStateFields(void *pEffStreamSubobject);
void *ConstructNetworkLobbyPeerClearCloseStream(void *pObject);

// FUNCTION: LEMBALL 0x004615F0
void *GetShiftedRecordSlotByPacketId(void *pSlotTable, int nPacketId) {
    GAME_RecordSlotTable *pTable;

    if (nPacketId > 2) {
        nPacketId -= 3;
    }
    pTable = (GAME_RecordSlotTable *)pSlotTable;
    return pTable->m_apSlots[nPacketId];
}

// FUNCTION: LEMBALL 0x0045F280
int LoadEffStreamFromMemory(void *pStream, int nSourceBuffer) {
    GAME_EffStream *pEffStream;

    pEffStream = (GAME_EffStream *)pStream;
    pEffStream->m_nQueuedPayload = nSourceBuffer;
    if (pEffStream->m_pVtable->m_pLoadFromMemoryHeader() != 0) {
        pEffStream->m_pVtable->m_pLoadFromMemoryBody();
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00462550
void SetEffTransportPrimaryHandleParams(void *pRuntimeWindow, int nPrimaryHandleCount, int nPrimaryHandleStride, int nFlags) {
    GAME_EffTransportRuntimeWindow *pWindow;

    pWindow = (GAME_EffTransportRuntimeWindow *)pRuntimeWindow;
    pWindow->m_nPrimaryHandleCount = nPrimaryHandleCount;
    pWindow->m_nPrimaryHandleStride = nPrimaryHandleStride;
    pWindow->m_nPrimaryHandleFlags = nFlags;
}

// FUNCTION: LEMBALL 0x00462570
void SetEffTransportSecondaryHandleParams(void *pRuntimeWindow, int nSecondaryHandleCount, int nSecondaryHandleStride) {
    GAME_EffTransportRuntimeWindow *pWindow;

    pWindow = (GAME_EffTransportRuntimeWindow *)pRuntimeWindow;
    pWindow->m_nSecondaryHandleCount = nSecondaryHandleCount;
    pWindow->m_nSecondaryHandleStride = nSecondaryHandleStride;
}

// FUNCTION: LEMBALL 0x004523B0
void *ConstructNetworkLobbySelectedPeerStatusStream(void *pObject) {
    GAME_EffStream *pStream;

    pStream = (GAME_EffStream *)pObject;
    pStream->m_pVtable = (GAME_EffStreamVtable *)g_EFF_BaseStreamVtable;
    pStream->m_nEventCode = 4;
    ResetEffStreamStateFields(pObject);
    pStream->m_pVtable = (GAME_EffStreamVtable *)g_EFF_NetworkLobbySelectedPeerStatusVtable;
    pStream->m_cbSerializedLength += 4;
    return pObject;
}

// FUNCTION: LEMBALL 0x004523E0
void *ConstructNetworkLobbyPeerEntryStream(void *pObject) {
    GAME_NetworkLobbyPeerEntryStream *pStream;

    pStream = (GAME_NetworkLobbyPeerEntryStream *)pObject;
    pStream->m_Base.m_pVtable = (GAME_EffStreamVtable *)g_EFF_BaseStreamVtable;
    pStream->m_Base.m_nEventCode = 5;
    ResetEffStreamStateFields(pObject);
    pStream->m_Base.m_pVtable = (GAME_EffStreamVtable *)g_EFF_NetworkLobbyPeerEntryVtable;
    pStream->m_fAcceptedPeer = 0;
    pStream->m_nPeerEntryState = 0;
    pStream->m_Base.m_cbSerializedLength += 0x1e;
    pStream->m_Base.m_fHasPayload = 1;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452550
void *ConstructNetworkLobbyVsnetRuntime(void *pObject, char *pszFileBasedNetworkRoot) {
    GAME_NetworkLobbyVsnetRuntime *pRuntime;
    GAME_PeerEntryStreamTable *pPeerEntryTable;
    GAME_EffStream *pPeerEntryStream;
    int i;
    int fStarted;

    pRuntime = (GAME_NetworkLobbyVsnetRuntime *)pObject;
    fStarted = 0;

    InitializeRenderQueueNodeBase(pObject);
    pRuntime->m_pVtable = g_EFF_NetworkLobbyVsnetRuntimeVtable;
    pRuntime->m_fFileBasedThreadStarted = 0;
    pRuntime->m_fTcpipThreadStarted = 0;
    pRuntime->m_nDesiredSelectedPeerStatus = 0;

    pPeerEntryTable = (GAME_PeerEntryStreamTable *)AllocateVSMemBlock(0x324);
    if (pPeerEntryTable == 0) {
        pRuntime->m_pPeerEntryTable = 0;
    } else {
        pPeerEntryTable->m_cEntries = 10;
        pPeerEntryStream = pPeerEntryTable->m_aEntries;
        for (i = 0; i < 10; ++i) {
            ConstructNetworkLobbyPeerEntryStream(pPeerEntryStream);
            ++pPeerEntryStream;
        }
        pRuntime->m_pPeerEntryTable = pPeerEntryTable->m_aEntries;
    }

    pPeerEntryStream = (GAME_EffStream *)AllocateVSMemBlock(0x30);
    if (pPeerEntryStream == 0) {
        pRuntime->m_pSelectedPeerStatusStream = 0;
    } else {
        pRuntime->m_pSelectedPeerStatusStream =
            (GAME_EffStream *)ConstructNetworkLobbySelectedPeerStatusStream(pPeerEntryStream);
    }

    pRuntime->m_dwSelectedPeerStatusTick = (int)(timeGetTime() - 2000);
    pRuntime->m_nObservedSelectedPeerStatus = 0;
    pRuntime->m_nDesiredSelectedPeerStatus = 0;

    pPeerEntryStream = (GAME_EffStream *)AllocateVSMemBlock(0x50);
    if (pPeerEntryStream == 0) {
        pRuntime->m_pPeerEntryWriteStream = 0;
    } else {
        pRuntime->m_pPeerEntryWriteStream = (GAME_EffStream *)ConstructNetworkLobbyPeerEntryStream(pPeerEntryStream);
    }

    pPeerEntryStream = (GAME_EffStream *)AllocateVSMemBlock(0x30);
    if (pPeerEntryStream == 0) {
        pRuntime->m_pPeerClearCloseStream = 0;
    } else {
        pRuntime->m_pPeerClearCloseStream = (GAME_EffStream *)ConstructNetworkLobbyPeerClearCloseStream(pPeerEntryStream);
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

        if (StartEffTransportRuntimeAndWaitReady(g_pActiveNetworkRuntimeWindow, 0, 0x400) != 0) {
            SetEffTransportSecondaryHandleParams(g_pActiveNetworkRuntimeWindow, 100, 0x10);
            SetEffTransportPrimaryHandleParams(g_pActiveNetworkRuntimeWindow, 4, 4, 0);
            pRuntime->m_fSelectedPeerDisconnected = 0;
            g_nSelectedNetworkLobbyPeerId = 0;
            pRuntime->m_fSelectionNeedsReset = 0;
        }
    }

    return pObject;
}

// FUNCTION: LEMBALL 0x004526E0
void DestroyNetworkLobbyVsnetRuntime(void *pObject) {
    GAME_NetworkLobbyVsnetRuntime *pRuntime;
    void *pOwnedObject;

    pRuntime = (GAME_NetworkLobbyVsnetRuntime *)pObject;
    pRuntime->m_pVtable = g_EFF_NetworkLobbyVsnetRuntimeVtable;

    if (pRuntime->m_fFileBasedThreadStarted != 0) {
        StopFileBasedNetworkMessageThread();
    }
    if (pRuntime->m_fTcpipThreadStarted != 0) {
        StopTcpipNetworkMessageThread();
    }

    pOwnedObject = pRuntime->m_pPeerClearCloseStream;
    if (pOwnedObject != 0) {
        ((GAME_EffStreamDeleteVtable *)*(void **)pOwnedObject)->m_pDelete(1);
    }

    pOwnedObject = pRuntime->m_pSelectedPeerStatusStream;
    if (pOwnedObject != 0) {
        ((GAME_EffStreamDeleteVtable *)*(void **)pOwnedObject)->m_pDelete(1);
    }

    pOwnedObject = pRuntime->m_pPeerEntryWriteStream;
    if (pOwnedObject != 0) {
        ((GAME_EffStreamDeleteVtable *)*(void **)pOwnedObject)->m_pDelete(1);
    }

    pOwnedObject = pRuntime->m_pPeerEntryTable;
    if (pOwnedObject != 0) {
        ((GAME_EffStreamDeleteVtable *)*(void **)pOwnedObject)->m_pDelete(3);
    }

    pRuntime->m_pVtable = g_pNonZrleVariantRenderEntryInitializeVtable;
}

// FUNCTION: LEMBALL 0x0045F1E0
void BeginEffStreamWriteSession(void *pObject) {
    GAME_EffStream *pStream;
    int cbPayload;

    pStream = (GAME_EffStream *)pObject;
    cbPayload = pStream->m_cWriteSessions;
    pStream->m_cWriteSessions = cbPayload + 1;
    if (cbPayload == 0) {
        if (pStream->m_fOwnsBuffer == 0) {
            cbPayload = (int)(unsigned long)AllocateVSMemBlock((unsigned int)(pStream->m_cbSerializedLength + 0x10));
            pStream->m_fOwnsBuffer = 1;
            pStream->m_pvOwnedBuffer = cbPayload;
            pStream->m_pvWriteCursor = cbPayload + 0x10;
            pStream->m_pvBufferEnd = pStream->m_cbSerializedLength + cbPayload + 0x10;
        } else {
            pStream->m_pvWriteCursor = pStream->m_pvOwnedBuffer + 0x10;
        }
        pStream->m_pVtable->m_pBeginWriteHeader();
        pStream->m_pVtable->m_pBeginWriteBody();
    }
}

// FUNCTION: LEMBALL 0x0045F240
void EndEffStreamWriteSession(void *pObject) {
    GAME_EffStream *pStream;

    pStream = (GAME_EffStream *)pObject;
    --pStream->m_cWriteSessions;
}

// FUNCTION: LEMBALL 0x0045FC30
int HasShiftedRecordSlotPayloadForStream(void *pTransportPeer, void *pStream) {
    GAME_EffTransportHandleOwner *pHandleOwner;
    GAME_ShiftedRecordSlot *pSlot;

    pHandleOwner = (GAME_EffTransportHandleOwner *)pTransportPeer;
    pSlot = (GAME_ShiftedRecordSlot *)GetShiftedRecordSlotByPacketId(pHandleOwner->m_pShiftedRecordSlotTable,
                                                                     ((GAME_EffStream *)pStream)->m_nEventCode);
    return pSlot->m_fHasPayload;
}

// FUNCTION: LEMBALL 0x0045FC50
void LoadStreamFromShiftedRecordSlotAndClear(void *pTransportPeer, void *pStream) {
    GAME_EffTransportHandleOwner *pHandleOwner;
    GAME_ShiftedRecordSlot *pSlot;
    GAME_ShiftedRecordLockVtable *pLockVtable;

    pHandleOwner = (GAME_EffTransportHandleOwner *)pTransportPeer;
    pSlot = (GAME_ShiftedRecordSlot *)GetShiftedRecordSlotByPacketId(pHandleOwner->m_pShiftedRecordSlotTable,
                                                                     ((GAME_EffStream *)pStream)->m_nEventCode);
    pLockVtable = (GAME_ShiftedRecordLockVtable *)pSlot->m_pLockVtable;
    pLockVtable->m_pLock();
    LoadEffStreamFromMemory(pStream, pSlot->m_pSerializedPayload + 0x10);
    pLockVtable->m_pUnlock();
    pSlot->m_fHasPayload = 0;
}

// FUNCTION: LEMBALL 0x0045F720
void ResetEffTransportHandleGroup(int *pHandleGroup) {
    GAME_EffTransportHandleGroup *pGroup;
    int nResult;

    pGroup = (GAME_EffTransportHandleGroup *)pHandleGroup;
    if (pGroup->m_fPrimaryActive != 0) {
        pGroup->m_fSecondaryActive = 0;
        pGroup->m_fPrimaryActive = 0;
        nResult = pGroup->m_pCallback->m_pVtable->m_pBeginReset();
        if (nResult == -1) {
            pGroup->m_pCallback->m_pVtable->m_pFinishReset();
        }
    }
}

// FUNCTION: LEMBALL 0x0045F2B0
void QueueEffStreamWriteEvent(void *pObject, int nPayload) {
    GAME_EffQueuedWriteEvent event;

    if (nPayload != 0) {
        event.m_nPayload = nPayload;
        event.m_fWrite = 1;
        event.m_nEventType = 0xb;
        event.m_pStream = pObject;
        BeginEffStreamWriteSession(pObject);
        ((GAME_EffStream *)pObject)->m_fHasPayload = 1;
        ((GAME_RenderQueueServiceVtable *)*(void **)g_pEffTransportDispatchQueue)->m_pQueueWriteEvent(&event);
        ((GAME_EffTransportRuntimeWindowVtable *)*(void **)g_pActiveNetworkRuntimeWindow)->m_pServiceRuntime();
    }
}

// FUNCTION: LEMBALL 0x00460FB0
void CloseEffTransportPeer(int nPeer) {
    GAME_EffTransportPeer *pPeer;
    GAME_EffTransportEndpointSlot *pEndpointSlot;
    GAME_EffTransportEndpointState *pState;

    pPeer = (GAME_EffTransportPeer *)(unsigned long)nPeer;
    pEndpointSlot =
        (GAME_EffTransportEndpointSlot *)((char *)pPeer + pPeer->m_pDescriptor->m_nEndpointStateSlotOffset);
    pState = &pEndpointSlot->m_State;
    if (pState->m_fPrimaryHandlePresent != 0 && pState->m_fSecondaryHandlePresent != 0) {
        ResetEffTransportHandleGroup((int *)&pState->m_HandleGroup);
        if (pPeer->m_pConnectionState != 0) {
            pState->m_HandleGroup.m_pCallback->m_pVtable->m_pNotifyClosed(1);
        }
    }
    pPeer->m_fClosed = 1;
}

// FUNCTION: LEMBALL 0x0045EE80
void ResetEffStreamStateFields(void *pEffStreamSubobject) {
    GAME_EffStream *pStream;

    pStream = (GAME_EffStream *)pEffStreamSubobject;
    pStream->m_nQueuedPayload = 0;
    pStream->m_fHasPayload = 0;
    pStream->m_fOwnsBuffer = 0;
    pStream->m_pvOwnedBuffer = 0;
    pStream->m_pvBufferEnd = 0;
    pStream->m_pvWriteCursor = 0;
}

// FUNCTION: LEMBALL 0x004453B0
void *ConstructEffStreamPayloadSize8(void *pObject) {
    GAME_EffStream *pStream;

    pStream = (GAME_EffStream *)pObject;
    pStream->m_pVtable = (GAME_EffStreamVtable *)g_EFF_BaseStreamVtable;
    pStream->m_nEventCode = 8;
    ResetEffStreamStateFields(pObject);
    pStream->m_pVtable = (GAME_EffStreamVtable *)g_EFF_PayloadSize8Vtable;
    pStream->m_cbSerializedLength += 8;
    pStream->m_fHasPayload = 1;
    return pObject;
}

// FUNCTION: LEMBALL 0x004524B0
void *ConstructNetworkLobbyU32PayloadStream(void *pObject, int nEventCode) {
    GAME_EffStream *pStream;

    pStream = (GAME_EffStream *)pObject;
    pStream->m_pVtable = (GAME_EffStreamVtable *)g_EFF_BaseStreamVtable;
    pStream->m_nEventCode = nEventCode;
    ResetEffStreamStateFields(pObject);
    pStream->m_pVtable = (GAME_EffStreamVtable *)g_EFF_NetworkLobbyU32PayloadVtable;
    pStream->m_cbSerializedLength += 4;
    pStream->m_fHasPayload = 1;
    pStream->m_nU32Payload = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452510
void *ConstructNetworkLobbyPeerClearCloseStream(void *pObject) {
    ConstructNetworkLobbyU32PayloadStream(pObject, 6);
    *(void **)pObject = g_EFF_NetworkLobbyPeerClearCloseVtable;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452530
void *ConstructNetworkLobbyPeerDirtyConfirmStream(void *pObject) {
    ConstructNetworkLobbyU32PayloadStream(pObject, 7);
    *(void **)pObject = g_EFF_NetworkLobbyPeerDirtyConfirmVtable;
    return pObject;
}

// FUNCTION: LEMBALL 0x00462590
void RegisterEffTransportEventClient(void *pRuntimeWindow, void *pClient) {
    GAME_EffTransportRuntimeWindow *pWindow;

    if (pRuntimeWindow != 0) {
        pWindow = (GAME_EffTransportRuntimeWindow *)pRuntimeWindow;
        pWindow->m_pSecondaryDispatchClient = pClient;
    }
    RegisterOrderedRenderDispatchClient(g_pEffTransportSecondaryDispatchQueue, pClient, 0);
}

// FUNCTION: LEMBALL 0x004625B0
void UnregisterEffTransportEventClient(void *pRuntimeWindow) {
    GAME_EffTransportRuntimeWindow *pWindow;
    void *pClient;

    pWindow = (GAME_EffTransportRuntimeWindow *)pRuntimeWindow;
    pClient = pWindow->m_pSecondaryDispatchClient;
    if (pClient != 0) {
        UnregisterOrderedRenderDispatchClient(g_pEffTransportSecondaryDispatchQueue, pClient, 0);
        pWindow->m_pSecondaryDispatchClient = 0;
    }
}

// FUNCTION: LEMBALL 0x004527C0
void UnregisterNetworkLobbyVsnetRuntimeFromTransport(void *pVsnetRuntime) {
    GAME_NetworkLobbyVsnetRuntime *pRuntime;
    GAME_EffTransportRuntimeWindow *pWindow;
    DWORD dwStartTime;

    pRuntime = (GAME_NetworkLobbyVsnetRuntime *)pVsnetRuntime;
    if (g_nSelectedNetworkLobbyPeerId != 0) {
        pRuntime->m_pPeerClearCloseStream->m_nU32Payload = 1;
        QueueEffStreamWriteEvent(pRuntime->m_pPeerClearCloseStream, g_nSelectedNetworkLobbyPeerId);
        dwStartTime = timeGetTime();
        while (pRuntime->m_pPeerClearCloseStream->m_fHasPayload != 0) {
            if (timeGetTime() - dwStartTime >= 1000) {
                break;
            }
        }
        CloseEffTransportPeer(g_nSelectedNetworkLobbyPeerId);
        g_nSelectedNetworkLobbyPeerId = 0;
    }

    pWindow = (GAME_EffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow;
    if (pWindow != 0 && pWindow->m_fRuntimeActive != 0) {
        UnregisterOrderedRenderDispatchClient(g_pEffTransportSecondaryDispatchQueue, pVsnetRuntime, 0x19);
        pWindow->m_pSecondaryDispatchClient = pVsnetRuntime;
        ((GAME_EffTransportRuntimeWindowVtable *)*(void **)g_pActiveNetworkRuntimeWindow)->m_pServiceRuntime();
    }
}

// FUNCTION: LEMBALL 0x00452AB0
void MarkNetworkLobbySelectedPeerDisconnected(void *pVsnetRuntime) {
    ((GAME_NetworkLobbyVsnetRuntime *)pVsnetRuntime)->m_fSelectionNeedsReset = 1;
}

// FUNCTION: LEMBALL 0x00452AC0
void ServiceNetworkLobbySelectedPeerUpdates(void *pVsnetRuntime) {
    GAME_NetworkLobbyVsnetRuntime *pRuntime;
    GAME_EffTransportPeer *pSelectedPeer;
    GAME_EffTransportHandleOwner *pHandleOwner;
    DWORD dwNow;
    int nStreamValue;

    pRuntime = (GAME_NetworkLobbyVsnetRuntime *)pVsnetRuntime;
    if (pRuntime->m_fSelectionNeedsReset != 0) {
        g_nSelectedNetworkLobbyPeerId = 0;
        pRuntime->m_fSelectionNeedsReset = 0;
    }
    if (g_nSelectedNetworkLobbyPeerId != 0) {
        pSelectedPeer = (GAME_EffTransportPeer *)(unsigned long)g_nSelectedNetworkLobbyPeerId;
        pHandleOwner = &((GAME_EffTransportHandleOwnerSlot *)((char *)pSelectedPeer +
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
                QueueEffStreamWriteEvent(pRuntime->m_pSelectedPeerStatusStream, g_nSelectedNetworkLobbyPeerId);
            }
        }
        if (pRuntime->m_nObservedSelectedPeerStatus != pRuntime->m_nDesiredSelectedPeerStatus) {
            dwNow = timeGetTime();
            if (2000 < dwNow - (DWORD)pRuntime->m_dwSelectedPeerStatusTick) {
                pRuntime->m_pSelectedPeerStatusStream->m_nU32Payload = pRuntime->m_nDesiredSelectedPeerStatus;
                QueueEffStreamWriteEvent(pRuntime->m_pSelectedPeerStatusStream, g_nSelectedNetworkLobbyPeerId);
                pRuntime->m_dwSelectedPeerStatusTick = (int)timeGetTime();
            }
        }
    }
}

// FUNCTION: LEMBALL 0x00452740
int RegisterNetworkLobbyVsnetRuntimeWithTransport(void *pVsnetRuntime) {
    GAME_EffTransportRuntimeWindow *pWindow;

    pWindow = (GAME_EffTransportRuntimeWindow *)g_pActiveNetworkRuntimeWindow;
    if (pWindow != 0 && pWindow->m_fRuntimeActive != 0) {
        pWindow->m_pPrimaryDispatchClient = pVsnetRuntime;
        ((GAME_EffTransportRuntimeWindowVtable *)*(void **)g_pActiveNetworkRuntimeWindow)->m_pServiceRuntime();
        RegisterOrderedRenderDispatchClient(g_pEffTransportSecondaryDispatchQueue, pVsnetRuntime, 0x19);
        return 1;
    }
    return 0;
}
