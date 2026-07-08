#include "../game.h"

static void *g_EFF_GenericPayloadVtable = 0;
static void *g_EFF_NetworkLobbyPeerClearCloseVtable = 0;
static void *g_EFF_NetworkLobbyPeerDirtyConfirmVtable = 0;

// FUNCTION: LEMBALL 0x0045EE80
void ResetEffStreamStateFields(void *pEffStreamSubobject) {
    char *pStream;

    pStream = (char *)pEffStreamSubobject;
    *(void **)(pStream + 0x24) = 0;
    *(void **)(pStream + 0x28) = 0;
    *(void **)(pStream + 0x14) = 0;
    *(void **)(pStream + 0x08) = 0;
    *(void **)(pStream + 0x18) = 0;
    *(void **)(pStream + 0x10) = 0;
}

// FUNCTION: LEMBALL 0x004453B0
void *ConstructEffStreamPayloadSize8(void *pObject) {
    int *pStream;

    pStream = (int *)pObject;
    *(void **)pStream = &g_EFF_GenericPayloadVtable;
    pStream[1] = 8;
    ResetEffStreamStateFields(pObject);
    *(void **)pStream = &g_EFF_GenericPayloadVtable;
    pStream[6] += 8;
    pStream[9] = 1;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452510
void *ConstructNetworkLobbyPeerClearCloseStream(void *pObject) {
    ConstructEffStreamPayloadSize8(pObject);
    *(void **)pObject = &g_EFF_NetworkLobbyPeerClearCloseVtable;
    return pObject;
}

// FUNCTION: LEMBALL 0x00452530
void *ConstructNetworkLobbyPeerDirtyConfirmStream(void *pObject) {
    ConstructEffStreamPayloadSize8(pObject);
    *(void **)pObject = &g_EFF_NetworkLobbyPeerDirtyConfirmVtable;
    return pObject;
}

// FUNCTION: LEMBALL 0x00462590
void RegisterEffTransportEventClient(void *pRuntimeWindow, void *pClient) {
    if (pRuntimeWindow != 0) {
        *(void **)((char *)pRuntimeWindow + 0x44) = pClient;
    }
    RegisterOrderedRenderDispatchClient(g_pSharedRenderDispatchQueue, pClient, 0);
}
