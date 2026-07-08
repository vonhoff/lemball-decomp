#include "../game/game_app.h"
#include "../engine/runtime_init.h"

static const char *g_NETWORK_GameConnectionRequestDuringGameText = (const char *)0x004A0148;
static const char *g_NETWORK_LobbyConnectionAcceptedText = (const char *)0x004A0368;

struct NETWORK_GeometryChildVtable {
    void (*m_pReserved00)(void);
    void (*m_pReserved04)(void);
    void (*m_pReserved08)(void);
    void (*m_pReserved0c)(void);
    void (*m_pActivate)(void);
    void (*m_pDeactivate)(void);
};

struct NETWORK_NetworkLobbyPlayerEntryChild {
    void **m_pVtable;
    int m_fEnabled;
    int m_fSelected;
    char m_abUnknown0c[0x24];
    unsigned short m_nSourceX;
    unsigned short m_nSourceY;
    unsigned short m_nSourceWidth;
    short m_nSourceHeight;
    int m_fDirty38;
    int m_fDirty3c;
    int m_fDirty40;

    void Activate(void) {
        ((NETWORK_GeometryChildVtable *)m_pVtable)->m_pActivate();
    }

    void Deactivate(void) {
        ((NETWORK_GeometryChildVtable *)m_pVtable)->m_pDeactivate();
    }
};

struct NETWORK_NetworkLobbyPlayerEntryTable {
    NETWORK_NetworkLobbyPlayerEntryChild m_aEntries[10];
};

struct NETWORK_EffStreamVtable {
    void *m_pReserved00[11];
};

struct NETWORK_EffStream {
    void **m_pVtable;
    char m_abUnknown04[0x28];
    int m_nU32Payload;
};

struct NETWORK_EffTransportPeerDescriptor {
    void **m_pVtable;
    int m_nHandleGroupOffset;
};

struct NETWORK_EffTransportPeer {
    void **m_pVtable;
    NETWORK_EffTransportPeerDescriptor *m_pDescriptor;
};

struct NETWORK_EffTransportPeerStateSlot {
    char m_abUnknown00[0x4c];
    int m_fActive;
};

struct NETWORK_NetworkLobbyVsnetRuntime {
    void *m_pVtable;
    char m_abUnknown04[0x0c];
    NETWORK_EffStream *m_pPeerEntryWriteStream;
    NETWORK_EffTransportPeerStateSlot *m_pPeerEntryTable;
    NETWORK_EffStream *m_pSelectedPeerStatusStream;
    NETWORK_EffStream *m_pPeerClearCloseStream;
    int m_anTrackedPeers[10];
    char m_abUnknown48[0x0c];
    int m_nDesiredSelectedPeerStatus;
    int m_nObservedSelectedPeerStatus;
    int m_dwSelectedPeerStatusTick;
    int m_fSelectedPeerDisconnected;
    int m_fSelectionNeedsReset;
};

struct NETWORK_EffTransportRuntimeWindowVtable {
    void *m_pReserved00[8];
    void (*m_pServiceRuntime)(void);
};

struct NETWORK_EffTransportRuntimeWindow {
    void **m_pVtable;
    char m_abUnknown04[0x14];
    int m_fRuntimeActive;
    char m_abUnknown1c[0x20];
    void *m_pPrimaryDispatchClient;
    void *m_pSecondaryDispatchClient;
};

struct NETWORK_NetworkLobbyOwnerTable {
    char m_abUnknown00[0x70];
    short m_nSourceHeight;
    short m_nEntryStartY;
    char m_abUnknown74[0x18];
    int m_nEntryYStep;
    unsigned short m_nSourceX;
    unsigned short m_nSourceY;
};

struct NETWORK_NetworkLobbyCursorOwner {
    char m_abUnknown00[0x144];
    int m_fWrapSelection;
};

struct NETWORK_NetworkLobbyScreen {
    char m_abUnknown00[0x338];
    NETWORK_NetworkLobbyCursorOwner *m_pCursorOwner;
    char m_abUnknown33c[0x90];
    NETWORK_NetworkLobbyPlayerEntryTable *m_pEntryTable;
    int m_nSelectedEntry;
    int m_nResolvedSelection;
    int m_cVisibleEntries;
    char m_abUnknown3dc[0x50];
    NETWORK_NetworkLobbyOwnerTable *m_pEntryOwnerTable;
};

struct NETWORK_VsnetEventType3 {
    unsigned short m_nType;
    unsigned short m_nReserved02;
    int m_nReserved04;
    int m_nReserved08;
    int m_nStatus0c;
    int m_nPeer10;
};

struct NETWORK_VsnetStreamHeader {
    char m_abUnknown00[0x08];
    short m_nEventCode08;
};

struct NETWORK_VsnetEventType5 {
    unsigned short m_nType;
    unsigned short m_nReserved02;
    int m_nReserved04;
    int m_nReserved08;
    int m_nStatus0c;
    int m_nPeer10;
    int m_nStreamState14;
};

struct NETWORK_VsnetEventType10 {
    unsigned short m_nType;
    unsigned short m_nReserved02;
    int m_nReserved04;
    int m_nReserved08;
    int m_nPeer0c;
};

struct NETWORK_VsnetStreamState {
    char m_abUnknown00[0x04];
    NETWORK_VsnetStreamHeader *m_pHeader;
    char m_abUnknown08[0x1c];
    int m_fBusy;
};

int g_fNetworkLobbyScreenActive = 0;
char *g_pszPendingNetworkLobbyStatusText = 0;

extern int g_nSelectedNetworkLobbyPeerId;
extern void *g_pNetworkLobbyTransportController;
extern void *g_pActiveNetworkLobbyScreen;
extern void *g_pNetworkLobbyVsnetRuntime;

extern void CloseEffTransportPeer(int nPeer);
extern int LoadEffStreamFromMemory(void *pStream, int nSourceBuffer);
extern void QueueEffStreamWriteEvent(void *pObject, int nPayload);
extern void MarkNetworkLobbySelectedPeerDisconnected(void *pVsnetRuntime);
extern void SetGeometryChildEnabled(void *pGeometryChild, int fEnabled);
void ThunkRefreshNetworkLobbyPlayerEntries(int nNetworkLobbyScreen);

// FUNCTION: LEMBALL 0x00453250
void ResetNetworkLobbyPlayerEntryChildFlags(int nEntryChild) {
    NETWORK_NetworkLobbyPlayerEntryChild *pEntry;

    pEntry = (NETWORK_NetworkLobbyPlayerEntryChild *)(unsigned long)nEntryChild;
    pEntry->m_fDirty40 = 0;
    pEntry->m_fDirty3c = 0;
    pEntry->m_fDirty38 = 0;
}

// FUNCTION: LEMBALL 0x00454F00
void RemoveUnavailableNetworkLobbyPlayerEntries(int nNetworkLobbyScreen) {
    NETWORK_NetworkLobbyScreen *pScreen;
    NETWORK_NetworkLobbyVsnetRuntime *pRuntime;
    int *pnPeerId;
    NETWORK_EffTransportPeerStateSlot *pPeerState;
    int nEntryOffset;
    int nEntryIndex;

    pScreen = (NETWORK_NetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen;
    nEntryOffset = 0;
    if (g_pNetworkLobbyVsnetRuntime != 0) {
        pRuntime = (NETWORK_NetworkLobbyVsnetRuntime *)g_pNetworkLobbyVsnetRuntime;
        pnPeerId = pRuntime->m_anTrackedPeers;
        pPeerState = pRuntime->m_pPeerEntryTable;
        nEntryIndex = 0;
        do {
            if (*pnPeerId == 0 || pPeerState->m_fActive == 0) {
                ResetNetworkLobbyPlayerEntryChildFlags((int)(unsigned long)((char *)pScreen->m_pEntryTable + nEntryOffset));
                if (pScreen->m_nResolvedSelection == nEntryIndex) {
                    pScreen->m_nResolvedSelection = -1;
                }
            }
            nEntryOffset += 0x44;
            ++pPeerState;
            ++pnPeerId;
            ++nEntryIndex;
        } while (nEntryOffset < 0x2a8);
    }

    ThunkRefreshNetworkLobbyPlayerEntries(nNetworkLobbyScreen);
}

// FUNCTION: LEMBALL 0x0040346D
int ThunkSelectPreviousNetworkLobbyPlayerEntry(int nNetworkLobbyScreen) {
    NETWORK_NetworkLobbyScreen *pScreen;
    NETWORK_NetworkLobbyPlayerEntryTable *pEntryTable;
    int *pnEntryState;
    int nSelectedEntry;
    int nEntryOffset;

    pScreen = (NETWORK_NetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen;
    pEntryTable = pScreen->m_pEntryTable;
    if (pScreen->m_cVisibleEntries == 0 || pScreen->m_nSelectedEntry == 0) {
        return 0;
    }

    if (pScreen->m_nSelectedEntry == -1) {
        pScreen->m_pCursorOwner->m_fWrapSelection = 0;
        pScreen->m_nSelectedEntry = 10;
    }

    nSelectedEntry = pScreen->m_nSelectedEntry - 1;
    if (nSelectedEntry >= 0) {
        pnEntryState = (int *)((char *)pEntryTable + 4 + nSelectedEntry * 0x44);
        do {
            if (*pnEntryState != 0) {
                break;
            }
            pnEntryState -= 0x11;
            --nSelectedEntry;
        } while (nSelectedEntry >= 0);
    }

    if (nSelectedEntry < 0) {
        return 0;
    }

    pScreen->m_nSelectedEntry = nSelectedEntry;
    nEntryOffset = 0;
    do {
        ((NETWORK_NetworkLobbyPlayerEntryChild *)((char *)pEntryTable + nEntryOffset))->m_fSelected = 0;
        pnEntryState = (int *)((char *)pEntryTable + nEntryOffset);
        nEntryOffset += 0x44;
        ((NETWORK_NetworkLobbyPlayerEntryChild *)pnEntryState)->Deactivate();
    } while (nEntryOffset < 0x2a8);

    pEntryTable->m_aEntries[pScreen->m_nSelectedEntry].m_fSelected = 1;
    pEntryTable->m_aEntries[pScreen->m_nSelectedEntry].Activate();
    return 1;
}

// FUNCTION: LEMBALL 0x00402F4A
int ThunkSelectNextNetworkLobbyPlayerEntry(int nNetworkLobbyScreen) {
    NETWORK_NetworkLobbyScreen *pScreen;
    NETWORK_NetworkLobbyPlayerEntryTable *pEntryTable;
    int *pnEntryState;
    int nSelectedEntry;
    int nEntryOffset;

    pScreen = (NETWORK_NetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen;
    pEntryTable = pScreen->m_pEntryTable;
    if (pScreen->m_cVisibleEntries == 0) {
        if (pScreen->m_nSelectedEntry == -1) {
            return 0;
        }
        pScreen->m_pCursorOwner->m_fWrapSelection = 1;
        pScreen->m_nSelectedEntry = -1;
    }

    if (pScreen->m_nSelectedEntry == -1) {
        return 0;
    }

    nSelectedEntry = pScreen->m_nSelectedEntry + 1;
    if (nSelectedEntry < 10) {
        pnEntryState = (int *)((char *)pEntryTable + 4 + nSelectedEntry * 0x44);
        do {
            if (*pnEntryState != 0) {
                break;
            }
            pnEntryState += 0x11;
            ++nSelectedEntry;
        } while (nSelectedEntry < 10);
    }

    pScreen->m_nSelectedEntry = nSelectedEntry;
    nEntryOffset = 0;
    do {
        ((NETWORK_NetworkLobbyPlayerEntryChild *)((char *)pEntryTable + nEntryOffset))->m_fSelected = 0;
        pnEntryState = (int *)((char *)pEntryTable + nEntryOffset);
        nEntryOffset += 0x44;
        ((NETWORK_NetworkLobbyPlayerEntryChild *)pnEntryState)->Deactivate();
    } while (nEntryOffset < 0x2a8);

    if (pScreen->m_nSelectedEntry == 10) {
        pScreen->m_pCursorOwner->m_fWrapSelection = 1;
        pScreen->m_nSelectedEntry = -1;
        return 1;
    }

    pEntryTable->m_aEntries[pScreen->m_nSelectedEntry].m_fSelected = 1;
    pEntryTable->m_aEntries[pScreen->m_nSelectedEntry].Activate();
    return 1;
}

// FUNCTION: LEMBALL 0x0040296E
void ThunkActivateSelectedNetworkLobbyPlayerEntry(int nNetworkLobbyScreen) {
    NETWORK_NetworkLobbyScreen *pScreen;
    NETWORK_NetworkLobbyPlayerEntryChild *pEntryChild;

    pScreen = (NETWORK_NetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen;
    if (pScreen->m_nSelectedEntry != -1) {
        pEntryChild = &pScreen->m_pEntryTable->m_aEntries[pScreen->m_nSelectedEntry];
        if (pEntryChild->m_fEnabled == 0) {
            if (ThunkSelectPreviousNetworkLobbyPlayerEntry(nNetworkLobbyScreen) == 0) {
                ThunkSelectNextNetworkLobbyPlayerEntry(nNetworkLobbyScreen);
            }
        } else {
            pEntryChild->m_fSelected = 1;
            pEntryChild->Activate();
        }
    }
}

// FUNCTION: LEMBALL 0x004026A8
void ThunkRefreshNetworkLobbyPlayerEntries(int nNetworkLobbyScreen) {
    NETWORK_NetworkLobbyScreen *pScreen;
    NETWORK_NetworkLobbyVsnetRuntime *pRuntime;
    NETWORK_NetworkLobbyOwnerTable *pOwnerTable;
    void *pEntryChild;
    int *pnPeerIds;
    NETWORK_EffTransportPeerStateSlot *pPeerState;
    NETWORK_EffTransportPeerStateSlot *pPeerEntryTable;
    int nEntryOffset;
    short y;

    pScreen = (NETWORK_NetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen;
    pPeerEntryTable = 0;
    pnPeerIds = 0;
    if (g_pNetworkLobbyVsnetRuntime != 0) {
        pRuntime = (NETWORK_NetworkLobbyVsnetRuntime *)g_pNetworkLobbyVsnetRuntime;
        pnPeerIds = pRuntime->m_anTrackedPeers;
        pPeerEntryTable = pRuntime->m_pPeerEntryTable;
    }

    pOwnerTable = pScreen->m_pEntryOwnerTable;
    y = pOwnerTable->m_nEntryStartY;
    nEntryOffset = 0;
    pPeerState = pPeerEntryTable;
    pScreen->m_cVisibleEntries = 0;

    do {
        if (pScreen->m_cVisibleEntries < 4 && pnPeerIds != 0 && *pnPeerIds != 0 && pPeerState->m_fActive != 0) {
            pEntryChild = (void *)(unsigned long)((int)(unsigned long)pScreen->m_pEntryTable + nEntryOffset);
            *(unsigned short *)((char *)pEntryChild + 0x30) = pOwnerTable->m_nSourceX;
            *(unsigned short *)((char *)pEntryChild + 0x32) = pOwnerTable->m_nSourceY;
            *(unsigned short *)((char *)pEntryChild + 0x34) = pOwnerTable->m_nSourceHeight;
            *(short *)((char *)pEntryChild + 0x36) = y;
            SetGeometryChildEnabled(pEntryChild, 1);
            y = (short)(y + pOwnerTable->m_nEntryYStep);
            pScreen->m_cVisibleEntries = pScreen->m_cVisibleEntries + 1;
        } else {
            SetGeometryChildEnabled((void *)(unsigned long)((int)(unsigned long)pScreen->m_pEntryTable + nEntryOffset), 0);
        }

        nEntryOffset += 0x44;
        ++pPeerState;
        ++pnPeerIds;
    } while (nEntryOffset < 0x2a8);

    ThunkActivateSelectedNetworkLobbyPlayerEntry(nNetworkLobbyScreen);
}

// FUNCTION: LEMBALL 0x00401F23
void ThunkHandleNetworkLobbyTransportStatusEvent(int nEventType) {
    if (g_pActiveNetworkLobbyScreen != 0) {
        if (nEventType == 10) {
            RemoveUnavailableNetworkLobbyPlayerEntries((int)(unsigned long)g_pActiveNetworkLobbyScreen);
            return;
        }
        if (nEventType == 0xd) {
            *(int *)((char *)g_pActiveNetworkLobbyScreen + 0x430) = 7;
        }
    }
}

// FUNCTION: LEMBALL 0x00452850
int HandleNetworkLobbyVsnetRuntimeEvent(void *pVsnetRuntime, unsigned short *pEvent) {
    NETWORK_NetworkLobbyVsnetRuntime *pRuntime;
    NETWORK_VsnetEventType3 *pType3Event;
    NETWORK_VsnetEventType5 *pType5Event;
    NETWORK_VsnetEventType10 *pType10Event;
    NETWORK_VsnetStreamState *pStreamState;
    int nPeer;
    int nStreamState;
    int nSlot;
    int *pnTrackedPeer;

    pRuntime = (NETWORK_NetworkLobbyVsnetRuntime *)pVsnetRuntime;
    pType3Event = (NETWORK_VsnetEventType3 *)pEvent;
    pType5Event = (NETWORK_VsnetEventType5 *)pEvent;
    pType10Event = (NETWORK_VsnetEventType10 *)pEvent;
    switch (*pEvent) {
    case 1:
        return 1;

    case 3:
        if (pType3Event->m_nStatus0c == 0) {
            nPeer = pType3Event->m_nPeer10;
            if (g_nSelectedNetworkLobbyPeerId != 0) {
                AppendCStringToStream(g_pStatusOutputStream, g_NETWORK_GameConnectionRequestDuringGameText);
                CloseEffTransportPeer(nPeer);
                return 1;
            }

            nSlot = 0;
            pnTrackedPeer = pRuntime->m_anTrackedPeers;
            while (*pnTrackedPeer != 0) {
                ++pnTrackedPeer;
                ++nSlot;
                if (nSlot >= 10) {
                    break;
                }
            }

            if (nSlot < 10) {
                pnTrackedPeer = &pRuntime->m_anTrackedPeers[nSlot];
                *pnTrackedPeer = nPeer;
                g_pszPendingNetworkLobbyStatusText =
                    g_fNetworkLobbyScreenActive >= 1 ? (char *)g_NETWORK_LobbyConnectionAcceptedText : 0;
                QueueEffStreamWriteEvent((void *)(unsigned long)((int)(unsigned long)pRuntime->m_pPeerEntryTable + nSlot * 0x50),
                                         *pnTrackedPeer);
                pRuntime->m_fSelectionNeedsReset = 1;
            } else {
                CloseEffTransportPeer(nPeer);
            }
        }
        return 1;

    case 5:
        nPeer = pType5Event->m_nPeer10;
        nStreamState = pType5Event->m_nStreamState14;
        pStreamState = (NETWORK_VsnetStreamState *)(unsigned long)nStreamState;
        if (pType5Event->m_nStatus0c != 0) {
            return 1;
        }
        if (pStreamState->m_pHeader->m_nEventCode08 != 6) {
            pStreamState->m_fBusy = 0;
            return 1;
        }
        LoadEffStreamFromMemory(pRuntime->m_pSelectedPeerStatusStream,
                                (int)(unsigned long)((char *)pStreamState->m_pHeader + 0x10));
        pStreamState->m_fBusy = 0;
        if (pRuntime->m_pSelectedPeerStatusStream->m_nU32Payload != 0) {
            CloseEffTransportPeer(nPeer);
        }
        return 1;

    case 10:
        nPeer = pType10Event->m_nPeer0c;
        for (nSlot = 0; nSlot < 10; ++nSlot) {
            if (pRuntime->m_anTrackedPeers[nSlot] == nPeer) {
                pRuntime->m_pPeerEntryTable[nSlot].m_fActive = 0;
                pRuntime->m_anTrackedPeers[nSlot] = 0;
                if (g_pNetworkLobbyTransportController != 0) {
                    ThunkHandleNetworkLobbyTransportStatusEvent(*pEvent);
                }
                if (g_nSelectedNetworkLobbyPeerId == nPeer) {
                    MarkNetworkLobbySelectedPeerDisconnected(pVsnetRuntime);
                }
                pRuntime->m_fSelectionNeedsReset = 1;
                break;
            }
        }
        return 1;

    case 0xd:
        if (g_pNetworkLobbyTransportController != 0) {
            ThunkHandleNetworkLobbyTransportStatusEvent(*pEvent);
        }
        return 1;

    default:
        return 0;
    }
}
