#include "GAME.H"
#include "ENGINE/MEDIA/EFFSTRM.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/GDI/VSGEOM.H"

extern "C" int WINAPI IsBadReadPtr(const void *pAddress,
                                    unsigned int cbAddress);

static const char g_NETWORK_GameConnectionRequestDuringGameText[] =
    "Game connection request during game\n";
extern char g_szNetworkLobbyLocalPlayerName[0x10];

struct VsNetNetworkLobbyPlayerEntryChild {
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

    void Activate(void);
    void Deactivate(void);
    void ResetFlags(void);
};

typedef void (VsNetNetworkLobbyPlayerEntryChild::*VsNetGeometryChildProc)(void);

struct VsNetGeometryChildVtable {
    void (*m_pReserved00)(void);
    void (*m_pReserved04)(void);
    void (*m_pReserved08)(void);
    void (*m_pReserved0c)(void);
    VsNetGeometryChildProc m_pActivate;
    VsNetGeometryChildProc m_pDeactivate;
};

inline void VsNetNetworkLobbyPlayerEntryChild::Activate(void) {
    (this->*((VsNetGeometryChildVtable *)m_pVtable)->m_pActivate)();
}

inline void VsNetNetworkLobbyPlayerEntryChild::Deactivate(void) {
    (this->*((VsNetGeometryChildVtable *)m_pVtable)->m_pDeactivate)();
}

struct VsNetNetworkLobbyPlayerEntryTable {
    VsNetNetworkLobbyPlayerEntryChild m_aEntries[10];
};

struct VsNetEffStreamVtable {
    void *m_pReserved00[11];
};

struct VsNetEffStream {
    void **m_pVtable;
    char m_abUnknown04[0x28];
    int m_nU32Payload;
};

struct VsNetEffTransportPeerDescriptor {
    void **m_pVtable;
    int m_nHandleGroupOffset;
};

struct VsNetEffTransportPeer {
    void **m_pVtable;
    VsNetEffTransportPeerDescriptor *m_pDescriptor;

    void Close(void);
};

struct VsNetEffTransportPeerStateSlot {
    char m_abUnknown00[0x4c];
    int m_fActive;
};

struct VsNetNetworkLobbyVsnetRuntime {
    void *m_pVtable;
    char m_abUnknown04[0x0c];
    VsNetEffStream *m_pPeerEntryWriteStream;
    VsNetEffTransportPeerStateSlot *m_pPeerEntryTable;
    VsNetEffStream *m_pSelectedPeerStatusStream;
    VsNetEffStream *m_pPeerClearCloseStream;
    int m_anTrackedPeers[10];
    char m_abUnknown48[0x0c];
    int m_nDesiredSelectedPeerStatus;
    int m_nObservedSelectedPeerStatus;
    int m_dwSelectedPeerStatusTick;
    int m_fSelectedPeerDisconnected;
    int m_fSelectionNeedsReset;
};

struct VsNetEffTransportRuntimeWindowVtable {
    void *m_pReserved00[8];
    void (*m_pServiceRuntime)(void);
};

struct VsNetEffTransportRuntimeWindow {
    void **m_pVtable;
    char m_abUnknown04[0x14];
    int m_fRuntimeActive;
    char m_abUnknown1c[0x20];
    void *m_pPrimaryDispatchClient;
    void *m_pSecondaryDispatchClient;
};

struct VsNetNetworkLobbyOwnerTable {
    char m_abUnknown00[0x70];
    short m_nSourceHeight70;
    short m_nReserved72;
    short m_nEntryStartY74;
    char m_abUnknown76[0x16];
    int m_nEntryYStep8c;
    int m_nSourceX90;
    unsigned short m_nSourceY94;
};

struct VsNetNetworkLobbyCursorOwner {
    char m_abUnknown00[0x144];
    int m_fWrapSelection;
};

struct VsNetNetworkLobbyScreen {
    char m_abUnknown00[0x338];
    VsNetNetworkLobbyCursorOwner *m_pCursorOwner;
    char m_abUnknown33c[0x38];
    int m_nRequestedMode374;
    int m_nTransitionStatus378;
    char m_abUnknown37c[0x50];
    VsNetNetworkLobbyPlayerEntryTable *m_pEntryTable;
    int m_nCurrentSelectedEntry;
    int m_nResolvedSelection;
    int m_cVisibleEntries;
    int m_fPlayerEntriesHidden3dc;
    char m_abUnknown3e0[0x4c];
    VsNetNetworkLobbyOwnerTable *m_pEntryOwnerTable;

    void RemoveUnavailablePlayerEntries(void);
    void RefreshPlayerEntries(void);
    void ActivateSelectedPlayerEntry(void);
    int SelectNextPlayerEntry(void);
    int SelectPreviousPlayerEntry(void);
    void HidePlayerEntries(void);
    void ShowPlayerEntries(void);
    void ClearPlayerEntryDirty(int iEntry);
    int ConfirmSelectedPlayerEntry(void);
};

struct VsNetVsnetEventType3 {
    unsigned short m_nType;
    unsigned short m_nReserved02;
    int m_nReserved04;
    int m_nReserved08;
    int m_nStatus0c;
    int m_nPeer10;
};

struct VsNetVsnetStreamHeader {
    char m_abUnknown00[0x08];
    short m_nEventCode08;
};

struct VsNetVsnetEventType5 {
    unsigned short m_nType;
    unsigned short m_nReserved02;
    int m_nReserved04;
    int m_nReserved08;
    int m_nStatus0c;
    int m_nPeer10;
    int m_nStreamState14;
};

struct VsNetVsnetEventType10 {
    unsigned short m_nType;
    unsigned short m_nReserved02;
    int m_nReserved04;
    int m_nReserved08;
    int m_nPeer0c;
};

struct VsNetVsnetStreamState {
    char m_abUnknown00[0x04];
    VsNetVsnetStreamHeader *m_pHeader;
    char m_abUnknown08[0x1c];
    int m_fBusy;
};

// GLOBAL: LEMBALL 0x004a0124
char *g_pszPendingNetworkLobbyStatusText = 0;

extern int g_nSelectedNetworkLobbyPeerId;
extern void *g_pNetworkLobbyTransportController;
extern void *g_pActiveNetworkLobbyScreen;
extern void *g_pNetworkLobbyVsnetRuntime;
extern void *g_pLevelProgressState;

extern void LEMBALL_FASTCALL MarkNetworkLobbySelectedPeerDisconnected(void *pVsnetRuntime);
extern void SetGeometryChildEnabled(void *pGeometryChild, int fEnabled);
void LEMBALL_FASTCALL ThunkRefreshNetworkLobbyPlayerEntries(
    int nNetworkLobbyScreen);
void LEMBALL_FASTCALL ThunkActivateSelectedNetworkLobbyPlayerEntry(
    int nNetworkLobbyScreen);
int LEMBALL_FASTCALL ThunkSelectNextNetworkLobbyPlayerEntry(
    int nNetworkLobbyScreen);
int LEMBALL_FASTCALL ThunkSelectPreviousNetworkLobbyPlayerEntry(
    int nNetworkLobbyScreen);
void LEMBALL_FASTCALL ThunkHideNetworkLobbyPlayerEntries(
    int nNetworkLobbyScreen);
void LEMBALL_FASTCALL ThunkShowNetworkLobbyPlayerEntries(
    int nNetworkLobbyScreen);
void LEMBALL_FASTCALL ThunkRemoveUnavailableNetworkLobbyPlayerEntries(
    int nNetworkLobbyScreen);

// FUNCTION: LEMBALL 0x00401a87
int LEMBALL_FASTCALL ThunkConfirmSelectedNetworkLobbyPlayerEntry(int nNetworkLobbyScreen) {
    return ((VsNetNetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen)
        ->ConfirmSelectedPlayerEntry();
}

// FUNCTION: LEMBALL 0x00401f23
void ThunkHandleNetworkLobbyTransportStatusEvent(int nEventType) {
    if (g_pActiveNetworkLobbyScreen != 0) {
        if (nEventType == 10) {
            ThunkRemoveUnavailableNetworkLobbyPlayerEntries(
                (int)(unsigned long)g_pActiveNetworkLobbyScreen);
            return;
        }
        if (nEventType == 0xd) {
            *(int *)((char *)g_pActiveNetworkLobbyScreen + 0x430) = 7;
        }
    }
}

// FUNCTION: LEMBALL 0x0040252c
void LEMBALL_FASTCALL ThunkShowNetworkLobbyPlayerEntries(int nNetworkLobbyScreen) {
    ((VsNetNetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen)
        ->ShowPlayerEntries();
}

// FUNCTION: LEMBALL 0x0040260d
void LEMBALL_FASTCALL ThunkRemoveUnavailableNetworkLobbyPlayerEntries(int nNetworkLobbyScreen) {
    ((VsNetNetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen)
        ->RemoveUnavailablePlayerEntries();
}

// FUNCTION: LEMBALL 0x004026a8
void LEMBALL_FASTCALL ThunkRefreshNetworkLobbyPlayerEntries(int nNetworkLobbyScreen) {
    ((VsNetNetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen)
        ->RefreshPlayerEntries();
}

// FUNCTION: LEMBALL 0x0040296e
void LEMBALL_FASTCALL ThunkActivateSelectedNetworkLobbyPlayerEntry(int nNetworkLobbyScreen) {
    ((VsNetNetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen)
        ->ActivateSelectedPlayerEntry();
}

// FUNCTION: LEMBALL 0x00402f4a
int LEMBALL_FASTCALL ThunkSelectNextNetworkLobbyPlayerEntry(int nNetworkLobbyScreen) {
    return ((VsNetNetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen)
        ->SelectNextPlayerEntry();
}

// FUNCTION: LEMBALL 0x0040325b
void LEMBALL_FASTCALL ThunkHideNetworkLobbyPlayerEntries(int nNetworkLobbyScreen) {
    ((VsNetNetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen)
        ->HidePlayerEntries();
}

// FUNCTION: LEMBALL 0x0040346d
int LEMBALL_FASTCALL ThunkSelectPreviousNetworkLobbyPlayerEntry(int nNetworkLobbyScreen) {
    return ((VsNetNetworkLobbyScreen *)(unsigned long)nNetworkLobbyScreen)
        ->SelectPreviousPlayerEntry();
}

// FUNCTION: LEMBALL 0x00452850
int LEMBALL_FASTCALL HandleNetworkLobbyVsnetRuntimeEvent(void *pVsnetRuntime, int, unsigned short *pEvent) {
    VsNetNetworkLobbyVsnetRuntime *pRuntime;
    VsNetVsnetEventType3 *pType3Event;
    VsNetVsnetEventType5 *pType5Event;
    VsNetVsnetEventType10 *pType10Event;
    VsNetVsnetStreamState *pStreamState;
    int nPeer;
    int nStreamState;
    int nSlot;
    int *pnTrackedPeer;

    pRuntime = (VsNetNetworkLobbyVsnetRuntime *)pVsnetRuntime;
    pType3Event = (VsNetVsnetEventType3 *)pEvent;
    pType5Event = (VsNetVsnetEventType5 *)pEvent;
    pType10Event = (VsNetVsnetEventType10 *)pEvent;
    switch (*pEvent) {
    case 1:
        return 1;

    case 3:
        if (pType3Event->m_nStatus0c == 0) {
            nPeer = pType3Event->m_nPeer10;
            if (g_nSelectedNetworkLobbyPeerId != 0) {
                AppendCStringToStream(g_pStatusOutputStream, g_NETWORK_GameConnectionRequestDuringGameText);
                ((VsNetEffTransportPeer *)(unsigned long)nPeer)->Close();
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
                    g_pActiveNetworkLobbyScreen != 0
                        ? g_szNetworkLobbyLocalPlayerName
                        : 0;
                ((GameEffStream *)(unsigned long)
                     ((int)(unsigned long)pRuntime->m_pPeerEntryTable +
                      nSlot * 0x50))
                    ->QueueEffStreamWriteEvent(*pnTrackedPeer);
                pRuntime->m_fSelectionNeedsReset = 1;
            } else {
                ((VsNetEffTransportPeer *)(unsigned long)nPeer)->Close();
            }
        }
        return 1;

    case 5:
        nPeer = pType5Event->m_nPeer10;
        nStreamState = pType5Event->m_nStreamState14;
        pStreamState = (VsNetVsnetStreamState *)(unsigned long)nStreamState;
        if (pType5Event->m_nStatus0c != 0) {
            return 1;
        }
        if (pStreamState->m_pHeader->m_nEventCode08 != 6) {
            pStreamState->m_fBusy = 0;
            return 1;
        }
        ((GameEffStream *)pRuntime->m_pSelectedPeerStatusStream)
            ->LoadEffStreamFromMemory((int)(unsigned long)((char *)pStreamState->m_pHeader + 0x10));
        pStreamState->m_fBusy = 0;
        if (pRuntime->m_pSelectedPeerStatusStream->m_nU32Payload != 0) {
            ((VsNetEffTransportPeer *)(unsigned long)nPeer)->Close();
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

// FUNCTION: LEMBALL 0x00453250
void VsNetNetworkLobbyPlayerEntryChild::ResetFlags(void) {
    m_fDirty40 = 0;
    m_fDirty3c = 0;
    m_fDirty38 = 0;
}

// FUNCTION: LEMBALL 0x00454b70
void VsNetNetworkLobbyScreen::ClearPlayerEntryDirty(int iEntry) {
    if (iEntry != -1 && iEntry < 10) {
        m_pEntryTable->m_aEntries[iEntry].m_fDirty40 = 0;
        if (m_nCurrentSelectedEntry == iEntry) {
            ThunkShowNetworkLobbyPlayerEntries(
                (int)(unsigned long)this);
        }
    }
}

// FUNCTION: LEMBALL 0x00454bb0
void VsNetNetworkLobbyScreen::ActivateSelectedPlayerEntry(void) {
    if (m_nResolvedSelection != -1) {
        if (m_pEntryTable->m_aEntries[m_nResolvedSelection].m_fEnabled == 0) {
            if (SelectPreviousPlayerEntry() == 0) {
                SelectNextPlayerEntry();
            }
        } else {
            m_pEntryTable->m_aEntries[m_nResolvedSelection].m_fSelected = 1;
            m_pEntryTable->m_aEntries[m_nResolvedSelection].Activate();
        }
    }
}

// FUNCTION: LEMBALL 0x00454c10
int VsNetNetworkLobbyScreen::SelectPreviousPlayerEntry(void) {
    int *pnEntryState;
    int nSelectedEntry;
    int nEntryOffset;

    if (m_cVisibleEntries == 0 || m_nResolvedSelection == 0) {
        return 0;
    }

    if (m_nResolvedSelection == -1) {
        m_pCursorOwner->m_fWrapSelection = 0;
        m_nResolvedSelection = 10;
    }

    nSelectedEntry = m_nResolvedSelection - 1;
    if (nSelectedEntry >= 0) {
        pnEntryState = (int *)((char *)m_pEntryTable + 4 + nSelectedEntry * 0x44);
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

    m_nResolvedSelection = nSelectedEntry;
    nEntryOffset = 0;
    do {
        ((VsNetNetworkLobbyPlayerEntryChild *)
            ((char *)m_pEntryTable + nEntryOffset))->m_fSelected = 0;
        pnEntryState = (int *)((char *)m_pEntryTable + nEntryOffset);
        nEntryOffset += 0x44;
        ((VsNetNetworkLobbyPlayerEntryChild *)pnEntryState)->Deactivate();
    } while (nEntryOffset < 0x2a8);

    m_pEntryTable->m_aEntries[m_nResolvedSelection].m_fSelected = 1;
    m_pEntryTable->m_aEntries[m_nResolvedSelection].Activate();
    return 1;
}

// FUNCTION: LEMBALL 0x00454cf0
int VsNetNetworkLobbyScreen::SelectNextPlayerEntry(void) {
    int *pnEntryState;
    int nSelectedEntry;
    int nEntryOffset;

    if (m_cVisibleEntries == 0) {
        if (m_nResolvedSelection == -1) {
            return 0;
        }
        m_pCursorOwner->m_fWrapSelection = 1;
        m_nResolvedSelection = -1;
    }

    if (m_nResolvedSelection == -1) {
        return 0;
    }

    nSelectedEntry = m_nResolvedSelection + 1;
    if (nSelectedEntry < 10) {
        pnEntryState = (int *)((char *)m_pEntryTable + 4 + nSelectedEntry * 0x44);
        do {
            if (*pnEntryState != 0) {
                break;
            }
            pnEntryState += 0x11;
            ++nSelectedEntry;
        } while (nSelectedEntry < 10);
    }

    m_nResolvedSelection = nSelectedEntry;
    nEntryOffset = 0;
    do {
        ((VsNetNetworkLobbyPlayerEntryChild *)((char *)m_pEntryTable + nEntryOffset))->m_fSelected = 0;
        pnEntryState = (int *)((char *)m_pEntryTable + nEntryOffset);
        nEntryOffset += 0x44;
        ((VsNetNetworkLobbyPlayerEntryChild *)pnEntryState)->Deactivate();
    } while (nEntryOffset < 0x2a8);

    if (m_nResolvedSelection == 10) {
        m_pCursorOwner->m_fWrapSelection = 1;
        m_nResolvedSelection = -1;
        return 1;
    }

    m_pEntryTable->m_aEntries[m_nResolvedSelection].m_fSelected = 1;
    m_pEntryTable->m_aEntries[m_nResolvedSelection].Activate();
    return 1;
}

// FUNCTION: LEMBALL 0x00454df0
void VsNetNetworkLobbyScreen::RefreshPlayerEntries(void) {
    VsNetNetworkLobbyVsnetRuntime *pRuntime;
    void *pEntryChild;
    int *pnPeerIds;
    int *pnPeerId;
    VsNetEffTransportPeerStateSlot *pPeerEntryTable;
    int *pfPeerActive;
    int nEntryOffset;
    short y;
    unsigned short nSourceX;
    unsigned short nSourceY;
    unsigned short nSourceHeight;

    pPeerEntryTable = 0;
    pnPeerIds = 0;
    if (g_pNetworkLobbyVsnetRuntime != 0) {
        pRuntime = (VsNetNetworkLobbyVsnetRuntime *)g_pNetworkLobbyVsnetRuntime;
        pnPeerIds = pRuntime->m_anTrackedPeers;
        pPeerEntryTable = pRuntime->m_pPeerEntryTable;
    }

    y = m_pEntryOwnerTable->m_nEntryStartY74;
    nSourceX = m_pEntryOwnerTable->m_nSourceX90;
    nSourceY = m_pEntryOwnerTable->m_nSourceY94;
    nSourceHeight = m_pEntryOwnerTable->m_nSourceHeight70;
    nEntryOffset = 0;
    pfPeerActive = (int *)((char *)pPeerEntryTable + 0x4c);
    m_cVisibleEntries = 0;
    pnPeerId = pnPeerIds;

    do {
        if (m_cVisibleEntries < 4 && pnPeerIds != 0 &&
            *pnPeerId != 0 && *pfPeerActive != 0) {
            pEntryChild = (void *)((char *)m_pEntryTable + nEntryOffset);
            *(unsigned short *)((char *)pEntryChild + 0x30) = nSourceX;
            *(unsigned short *)((char *)pEntryChild + 0x32) = nSourceY;
            *(unsigned short *)((char *)pEntryChild + 0x34) = nSourceHeight;
            *(short *)((char *)pEntryChild + 0x36) = y;
            ((GameGeometryOwnerRectState *)pEntryChild)->SetEnabled(1);
            y = (short)(y + m_pEntryOwnerTable->m_nEntryYStep8c);
            ++m_cVisibleEntries;
        } else {
            ((GameGeometryOwnerRectState *)
                ((char *)m_pEntryTable + nEntryOffset))->SetEnabled(0);
        }

        nEntryOffset += 0x44;
        pfPeerActive += 0x14;
        ++pnPeerId;
    } while (nEntryOffset < 0x2a8);

    ThunkActivateSelectedNetworkLobbyPlayerEntry((int)(unsigned long)this);
}

// FUNCTION: LEMBALL 0x00454f00
void VsNetNetworkLobbyScreen::RemoveUnavailablePlayerEntries(void) {
    int nEntryOffset;
    int *pnPeerId;
    int *pfPeerActive;
    int nEntryIndex;

    nEntryOffset = 0;
    if (g_pNetworkLobbyVsnetRuntime != 0) {
        pnPeerId = ((VsNetNetworkLobbyVsnetRuntime *)
            g_pNetworkLobbyVsnetRuntime)->m_anTrackedPeers;
        pfPeerActive = (int *)((char *)
            ((VsNetNetworkLobbyVsnetRuntime *)
                g_pNetworkLobbyVsnetRuntime)->m_pPeerEntryTable + 0x4c);
        nEntryIndex = 0;
        do {
            if (*pnPeerId == 0 || *pfPeerActive == 0) {
                ((VsNetNetworkLobbyPlayerEntryChild *)
                    ((char *)m_pEntryTable + nEntryOffset))->ResetFlags();
                if (m_nCurrentSelectedEntry == nEntryIndex) {
                    m_nCurrentSelectedEntry = -1;
                }
            }
            nEntryOffset += 0x44;
            pfPeerActive += 0x14;
            ++pnPeerId;
            ++nEntryIndex;
        } while (nEntryOffset < 0x2a8);
    }

    ThunkRefreshNetworkLobbyPlayerEntries((int)(unsigned long)this);
}

// FUNCTION: LEMBALL 0x00454f80
void VsNetNetworkLobbyScreen::HidePlayerEntries(void) {
    int nEntryOffset;

    m_fPlayerEntriesHidden3dc = 1;
    nEntryOffset = 0;
    do {
        ((GameGeometryOwnerRectState *)
            ((char *)m_pEntryTable + nEntryOffset))->SetEnabled(0);
        nEntryOffset += 0x44;
    } while (nEntryOffset < 0x2a8);
}

// FUNCTION: LEMBALL 0x00454fb0
void VsNetNetworkLobbyScreen::ShowPlayerEntries(void) {
    m_fPlayerEntriesHidden3dc = 0;
    ThunkRefreshNetworkLobbyPlayerEntries((int)(unsigned long)this);
}

// FUNCTION: LEMBALL 0x00454fc0
int VsNetNetworkLobbyScreen::ConfirmSelectedPlayerEntry(void) {
    int fEntriesWereVisible;
    int *pnTrackedPeers;
    char *pProgress;

    fEntriesWereVisible = m_fPlayerEntriesHidden3dc == 0;
    if (g_pNetworkLobbyVsnetRuntime != 0) {
        pnTrackedPeers = (int *)((char *)g_pNetworkLobbyVsnetRuntime + 0x20);
        if (m_nCurrentSelectedEntry != -1 &&
            pnTrackedPeers[m_nCurrentSelectedEntry] != 0) {
            ThunkHideNetworkLobbyPlayerEntries((int)(unsigned long)this);
            g_nSelectedNetworkLobbyPeerId =
                pnTrackedPeers[m_nCurrentSelectedEntry];
            m_nTransitionStatus378 = 1;
            m_nRequestedMode374 = 4;
            pProgress = (char *)g_pLevelProgressState;
            *(int *)pProgress = *(int *)(pProgress + 0x4c);
            *(int *)(pProgress + 8) = 4;
            return fEntriesWereVisible;
        }
    }
    m_fPlayerEntriesHidden3dc = 0;
    ThunkShowNetworkLobbyPlayerEntries((int)(unsigned long)this);
    return 0;
}

// Network-lobby player-entry virtual callbacks. These are logical lobby
// implementations; their distinct annotated addresses remain ABI entry points.

// FUNCTION: LEMBALL 0x00455dc0
void __fastcall mark_network_lobby_player_entry_child_active(void *pEntry) {
    *(int *)((char *)pEntry + 0x38) = 1;
}

// FUNCTION: LEMBALL 0x00455dd0
void __fastcall clear_network_lobby_player_entry_child_active(void *pEntry) {
    *(int *)((char *)pEntry + 0x38) = 0;
}
