#include "../game.h"

static void *g_EFF_BaseStreamVtable = (void *)0x004932c8;
static void *g_EFF_PayloadSize8Vtable = (void *)0x00497878;
static void *g_EFF_NetworkLobbyU32PayloadVtable = (void *)0x00498598;
static void *g_EFF_NetworkLobbyPeerClearCloseVtable = (void *)0x004985b8;
static void *g_EFF_NetworkLobbyPeerDirtyConfirmVtable = (void *)0x004985d8;
void *g_pEffTransportSecondaryDispatchQueue = 0;

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
    *(void **)pStream = g_EFF_BaseStreamVtable;
    pStream[1] = 8;
    ResetEffStreamStateFields(pObject);
    *(void **)pStream = g_EFF_PayloadSize8Vtable;
    pStream[6] += 8;
    pStream[9] = 1;
    return pObject;
}

// FUNCTION: LEMBALL 0x004524B0
void *ConstructNetworkLobbyU32PayloadStream(void *pObject, int nEventCode) {
    int *pStream;

    pStream = (int *)pObject;
    *(void **)pStream = g_EFF_BaseStreamVtable;
    pStream[1] = nEventCode;
    ResetEffStreamStateFields(pObject);
    *(void **)pStream = g_EFF_NetworkLobbyU32PayloadVtable;
    pStream[6] += 4;
    pStream[9] = 1;
    pStream[11] = 0;
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
    if (pRuntimeWindow != 0) {
        *(void **)((char *)pRuntimeWindow + 0x44) = pClient;
    }
    RegisterOrderedRenderDispatchClient(g_pEffTransportSecondaryDispatchQueue, pClient, 0);
}

// FUNCTION: LEMBALL 0x004625B0
void UnregisterEffTransportEventClient(void *pRuntimeWindow) {
    void *pClient;

    pClient = *(void **)((char *)pRuntimeWindow + 0x44);
    if (pClient != 0) {
        UnregisterOrderedRenderDispatchClient(g_pEffTransportSecondaryDispatchQueue, pClient, 0);
        *(void **)((char *)pRuntimeWindow + 0x44) = 0;
    }
}
