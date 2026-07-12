#include "game/window_owner.h"

#include "engine/memory_arena.h"
#include "engine/graphics_driver.h"
#include "engine/runtime_init.h"
#include "main.h"
#include "network/safe_vtable.h"
#include "platform/win32.h"

static const char g_szVsBaseWindowClassName[] = "VS_Base_Window_Class";
static const char *g_pszVsBaseWindowClassName = g_szVsBaseWindowClassName;
static const char g_szUnableToRegisterBaseWindowClassError[] =
    "Unable to register base window class";
static const char g_szFatalGetLastErrorPrefix[] = " GetLastError()=";
static const char g_szFatalGetLastErrorSeparator[] = ", ";
static const char g_szFatalErrorTitle[] = "FATAL ERROR";

static void *g_pSafeWindowOwnerVtable = NetworkGetSafeVtable();

static int g_fWindowMessagePassthroughMode = 0;
// GLOBAL: LEMBALL 0x004a1f64
static void *g_pCachedCurrentContextWindow = 0;
// GLOBAL: LEMBALL 0x004a1f68
static void *g_pCachedCurrentContextOwner = 0;
// GLOBAL: LEMBALL 0x004a1f74
static int g_nLastCursorScreenX = -1;
// GLOBAL: LEMBALL 0x004a1f78
static int g_nLastCursorScreenY = -1;
static int g_nLiveRootZrleGeometryOwnerCount = 0;
static int g_fFullscreenActivationState = 0;
static int g_fFullscreenDisplayModeSuspended = 0;
static int g_nMouseButtonCaptureRefCount = 0;
static int g_anSavedMouseAccelerationParams[3];
static int g_nSavedMouseTrailsCount = 0;
static const char g_szQuittingStatusMessage[] = "QUIT\n";
static const char g_szQuitStatusMessage[] = "QUIT\n";

extern int g_fRootHelperGeometryDispatchSuppressed;
extern void *g_pApplyFullscreenDisplayModeThunk;
extern void *g_pSharedRenderDispatchQueue;
extern "C" DWORD WINAPI timeGetTime(void);

int g_nLiveWindowOwnerBaseCount = 0;
void *g_pRootZrleGeometryOwnerRegistry = 0;
void *g_pRootGeometryOwnerVtable = g_pSafeWindowOwnerVtable;
void *g_pWindowOwnerBaseVtable = g_pSafeWindowOwnerVtable;

struct WINDOW_OWNER_REGISTRY_NODE {
    void *m_pOwner;
    WINDOW_OWNER_REGISTRY_NODE *m_pNext;
    WINDOW_OWNER_REGISTRY_NODE *m_pPrevious;
};

struct WINDOW_OWNER_STATE_DISPATCH {
    virtual void Reserved00(void) = 0;
    virtual void Reserved01(void) = 0;
    virtual void Reserved02(void) = 0;
    virtual void Reserved03(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved05(void) = 0;
    virtual void Reserved06(void) = 0;
    virtual void Reserved07(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved09(void) = 0;
    virtual void Reserved0A(void) = 0;
    virtual void Reserved0B(void) = 0;
    virtual void Reserved0C(void) = 0;
    virtual void Reserved0D(void) = 0;
    virtual void Reserved0E(void) = 0;
    virtual void Reserved0F(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved11(void) = 0;
    virtual void Reserved12(void) = 0;
    virtual void Reserved13(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved15(void) = 0;
    virtual void Reserved16(void) = 0;
    virtual void Reserved17(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved19(void) = 0;
    virtual void Reserved1A(void) = 0;
    virtual void SetState(int nState) = 0;
};

struct RENDER_DISPATCH_QUEUE_APPEND {
    virtual void Reserved00(void) = 0;
    virtual void Reserved01(void) = 0;
    virtual void Append(void *pEvent) = 0;
};

static void SetWindowOwnerState(void *pOwner, int nState) {
    ((WINDOW_OWNER_STATE_DISPATCH *)pOwner)->SetState(nState);
}

static void AppendWindowOwnerEvent(void *pEvent) {
    ((RENDER_DISPATCH_QUEUE_APPEND *)g_pSharedRenderDispatchQueue)->Append(pEvent);
}

// FUNCTION: LEMBALL 0x004324A0
void LEMBALL_FASTCALL CacheCurrentContextWindow(void *pOwner) {
    g_pCachedCurrentContextWindow = *(void **)((char *)pOwner + 0x44);
    g_pCachedCurrentContextOwner = pOwner;
}

// FUNCTION: LEMBALL 0x00465F80
void LEMBALL_FASTCALL RetainRootZrleGeometryOwner(void *pOwner) {
    WINDOW_OWNER_REGISTRY_NODE *pNode;
    WINDOW_OWNER_REGISTRY_NODE *pPrevious;

    ++g_nLiveRootZrleGeometryOwnerCount;
    if (*(void **)((char *)pOwner + 0x20) == 0) {
        pNode = (WINDOW_OWNER_REGISTRY_NODE *)AllocateVSMemBlock(0x0c);
        if (pNode != 0) {
            pNode->m_pOwner = pOwner;
            pNode->m_pNext = 0;
            *(void **)((char *)pNode + 8) = 0;
        }
        pPrevious = (WINDOW_OWNER_REGISTRY_NODE *)
            *(void **)((char *)g_pRootZrleGeometryOwnerRegistry + 4);
        *(void **)((char *)pNode + 8) = pPrevious;
        if (pPrevious != 0) {
            pPrevious->m_pNext = pNode;
        }
        *(void **)((char *)g_pRootZrleGeometryOwnerRegistry + 4) = pNode;
        if (*(void **)g_pRootZrleGeometryOwnerRegistry == 0) {
            *(void **)g_pRootZrleGeometryOwnerRegistry = pNode;
        }
        ++*(int *)((char *)g_pRootZrleGeometryOwnerRegistry + 8);
    }
    ++*(int *)((char *)pOwner + 4);
}

struct WINDOW_OWNER_FLAGS_INTERFACE {
    virtual void Reserved00(void) = 0;
    virtual void Reserved01(void) = 0;
    virtual void Reserved02(void) = 0;
    virtual void Reserved03(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved05(void) = 0;
    virtual void Reserved06(void) = 0;
    virtual void Reserved07(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved09(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved11(void) = 0;
    virtual void Reserved12(void) = 0;
    virtual void Reserved13(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved15(void) = 0;
    virtual void Reserved16(void) = 0;
    virtual void Reserved17(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved19(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void Reserved21(void) = 0;
    virtual void Reserved22(void) = 0;
    virtual void Reserved23(void) = 0;
    virtual void Reserved24(void) = 0;
    virtual unsigned int GetOwnerFlags(void) = 0;
};

// FUNCTION: LEMBALL 0x004580C0
void LEMBALL_FASTCALL NotifyWindowOwnersDisplayChanged(const int *pnSelectedDriver) {
    WINDOW_OWNER_REGISTRY_NODE *pNode;

    if (g_pRootZrleGeometryOwnerRegistry == 0) {
        return;
    }

    pNode = *(WINDOW_OWNER_REGISTRY_NODE **)g_pRootZrleGeometryOwnerRegistry;
    while (pNode != 0) {
        void *pOwner;
        unsigned int uOwnerFlags;
        int fDisplayChanged;
        void (LEMBALL_FASTCALL *pOwnerCallback)(void *);
        HWND hOwnerWindow;

        pOwner = pNode->m_pOwner;
        hOwnerWindow = *(HWND *)((char *)pOwner + 0x44);
        if (hOwnerWindow != 0) {
            uOwnerFlags = ((WINDOW_OWNER_FLAGS_INTERFACE *)pOwner)->GetOwnerFlags();
            if ((uOwnerFlags & 0x80000000u) != 0) {
                fDisplayChanged = 1;
                if (*pnSelectedDriver == 3) {
                    fDisplayChanged = 0;
                }
                *(unsigned int *)((char *)*(void **)((char *)*(void **)((char *)pOwner + 0x4c) + 0x0c) + 0x10) =
                    (unsigned int)fDisplayChanged;
            }
            SendMessageA(hOwnerWindow, 0x1c, 1, 0);
            pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                (*(void ***)pOwner)[0x5c / sizeof(void *)];
            pOwnerCallback(pOwner);
        }
        pNode = pNode->m_pNext;
    }
}

// FUNCTION: LEMBALL 0x00463000
int PackEventXYWords(unsigned int uLowWord, int nHighWord) {
    return (nHighWord << 16) + (uLowWord & 0xffff);
}

// FUNCTION: LEMBALL 0x00458180
int LEMBALL_FASTCALL IsDisplayModeWindowedRange(const int *pnDisplayMode) {
    return 1 < *pnDisplayMode && *pnDisplayMode < 6;
}

struct WINDOW_OWNER_COMMAND_TABLE {
    int LookupWindowCommandEventPayload(void *, int nCommandId);
};

struct WINDOW_OWNER_COMMAND_EVENT {
    unsigned short m_nType;
    unsigned short m_nReserved;
    unsigned int m_dwTime;
    int m_nPayload;
};

struct WINDOW_OWNER_KEY_EVENT {
    unsigned short m_nType;
    unsigned short m_nReserved;
    unsigned int m_nReserved08;
    LPARAM m_lParam;
};

struct WINDOW_OWNER_CURSOR_EVENT {
    unsigned short m_nType;
    unsigned short m_nReserved;
    DWORD m_dwTime;
    int m_nPackedPosition;
    int m_nReserved0c;
    int m_nReserved10;
};

struct WINDOW_OWNER_POSITION_INTERFACE {
    virtual void Reserved00(void) = 0;
    virtual void Reserved01(void) = 0;
    virtual void Reserved02(void) = 0;
    virtual void Reserved03(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Reserved05(void) = 0;
    virtual void Reserved06(void) = 0;
    virtual void Reserved07(void) = 0;
    virtual void Reserved08(void) = 0;
    virtual void Reserved09(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual void Reserved11(void) = 0;
    virtual void Reserved12(void) = 0;
    virtual void Reserved13(void) = 0;
    virtual void Reserved14(void) = 0;
    virtual void Reserved15(void) = 0;
    virtual void Reserved16(void) = 0;
    virtual void Reserved17(void) = 0;
    virtual void Reserved18(void) = 0;
    virtual void Reserved19(void) = 0;
    virtual void Reserved20(void) = 0;
    virtual void Reserved21(void) = 0;
    virtual void Reserved22(void) = 0;
    virtual void Reserved23(void) = 0;
    virtual void Reserved24(void) = 0;
    virtual void Reserved25(void) = 0;
    virtual void Reserved26(void) = 0;
    virtual void Reserved27(void) = 0;
    virtual void Reserved28(void) = 0;
    virtual void Reserved29(void) = 0;
    virtual void Reserved30(void) = 0;
    virtual void Reserved31(void) = 0;
    virtual void Reserved32(void) = 0;
    virtual void Reserved33(void) = 0;
    virtual void SetWindowPosition(tagPOINT *pPoint) = 0;
};

// FUNCTION: LEMBALL 0x00465750
int WINDOW_OWNER_COMMAND_TABLE::LookupWindowCommandEventPayload(void *, int nCommandId) {
    int *pEntryList;
    int *pEntry;

    pEntryList = *(int **)((char *)this + 0x3c);
    if (pEntryList == 0) {
        return 0;
    }

    while (*pEntryList != 0) {
        pEntry = (int *)*pEntryList;
        while (*pEntry != 0) {
            if (pEntry[1] == nCommandId) {
                return pEntry[2];
            }
            pEntry += 6;
        }
        ++pEntryList;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00464520
LRESULT CALLBACK WindowOwnerWindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    void *pOwner;
    DWORD dwMessageTime;
    CREATESTRUCTA *pCreateStruct;
    tagPOINT Point;
    int nSelectedDisplayMode;
    int nEventCode;
    int anEvent[3];
    short xEvent;
    short yEvent;
    unsigned int uOwnerFlags;
    unsigned int (LEMBALL_FASTCALL *pGetOwnerFlags)(void *);
    int (LEMBALL_FASTCALL *pOwnerState)(void *);
    LRESULT (LEMBALL_FASTCALL *pOwnerResult)(void *);
    void (LEMBALL_FASTCALL *pOwnerCallback)(void *);
    VSGDI_DisplayState *pDisplayState;
    void *pSelectedDriver;

    if (g_fWindowMessagePassthroughMode != 0 &&
        (uMessage != 0x1c || wParam != 0)) {
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);
    }

    dwMessageTime = GetMessageTime();

    pOwner = (void *)(unsigned long)GetWindowLongA(hWnd, GWL_USERDATA);
    pDisplayState = GetDisplayState();
    if (pDisplayState == 0) {
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);
    }

    pSelectedDriver = g_pSelectedGraphicsDriverRuntime;
    if (*(HWND *)((char *)pDisplayState + 0x10) == hWnd) {
        if (pSelectedDriver == 0 || *(void **)((char *)pSelectedDriver + 4) == 0) {
            return DefWindowProcA(hWnd, uMessage, wParam, lParam);
        }
        pOwner = *(void **)((char *)pSelectedDriver + 4);
    } else if (pOwner == 0 && uMessage != WM_CREATE) {
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);
    }

    if (uMessage == WM_CREATE) {
        pCreateStruct = (CREATESTRUCTA *)(unsigned long)lParam;
        pOwner = pCreateStruct->lpCreateParams;
        SetWindowLongA(hWnd, GWL_USERDATA, (LONG)(unsigned long)pOwner);
        if (pOwner == 0) {
            return 0;
        }

        *(void **)((char *)pOwner + 0x44) = hWnd;
        Point.x = 0;
        Point.y = 0;
        ClientToScreen(hWnd, &Point);

        nSelectedDisplayMode = GetSelectedGraphicsDriverId();
        if ((*(unsigned int *)(*(void **)((char *)pOwner + 0x48)) & 0x40000000) != 0 &&
            IsDisplayModeWindowedRange(&nSelectedDisplayMode)) {
            Point.x += *(short *)((char *)*(void **)((char *)pOwner + 0x48) + 0x18);
            Point.y += *(short *)((char *)*(void **)((char *)pOwner + 0x48) + 0x1a);
        }
        *(short *)((char *)pOwner + 0x0c) = (short)Point.x;
        *(short *)((char *)pOwner + 0x0e) = (short)Point.y;
        *(short *)((char *)pOwner + 0x18) = *(short *)((char *)pOwner + 0x0c);
        *(short *)((char *)pOwner + 0x1a) = *(short *)((char *)pOwner + 0x0e);

        pGetOwnerFlags = (unsigned int (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x64 / sizeof(void *)];
        (void)pGetOwnerFlags(pOwner);
        pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x18 / sizeof(void *)];
        pOwnerCallback(pOwner);
        pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x3c / sizeof(void *)];
        pOwnerCallback(pOwner);
        return 0;
    }

    if (uMessage == WM_QUIT) {
        AppendStartupCString(g_szQuittingStatusMessage);
        ReleaseCapture();
        return 0;
    }

    if (uMessage == WM_DESTROY) {
        if (pOwner != 0) {
            if (hWnd == g_pCachedCurrentContextWindow) {
                g_pCachedCurrentContextWindow = 0;
                g_pCachedCurrentContextOwner = 0;
            }
            *(void **)((char *)pOwner + 0x44) = 0;
            pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                (*(void ***)pOwner)[0x74 / sizeof(void *)];
            pOwnerCallback(pOwner);
        }
        if (g_nLiveRootZrleGeometryOwnerCount == 0) {
            g_fRootHelperGeometryDispatchSuppressed = 1;
            AppendStartupCString(g_szQuitStatusMessage);
        }
        return 0;
    }

    if (uMessage == 3 && pOwner != 0) {
        nSelectedDisplayMode = GetSelectedGraphicsDriverId();
        if (!IsDisplayModeWindowedRange(&nSelectedDisplayMode)) {
            Point.x = 0;
            Point.y = 0;
            ClientToScreen(hWnd, &Point);
            ((WINDOW_OWNER_POSITION_INTERFACE *)pOwner)->SetWindowPosition(&Point);
        }
        return 0;
    }

    if (uMessage == 5 && pOwner != 0) {
        *(short *)((char *)pOwner + 0x08) = (short)(lParam & 0xffff);
        *(short *)((char *)pOwner + 0x0a) = (short)(((unsigned long)lParam) >> 16);

        if (wParam == 0) {
            pOwnerState = (int (LEMBALL_FASTCALL *)(void *))
                (*(void ***)pOwner)[0x68 / sizeof(void *)];
            if (pOwnerState(pOwner) != 2) {
                SetWindowOwnerState(pOwner, 2);
                pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                    (*(void ***)pOwner)[0x54 / sizeof(void *)];
                pOwnerCallback(pOwner);
            }
        } else if (wParam == 1) {
            pOwnerState = (int (LEMBALL_FASTCALL *)(void *))
                (*(void ***)pOwner)[0x68 / sizeof(void *)];
            if (pOwnerState(pOwner) != 0) {
                SetWindowOwnerState(pOwner, 0);
                pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                    (*(void ***)pOwner)[0x4c / sizeof(void *)];
                pOwnerCallback(pOwner);
            }
        } else if (wParam == 2) {
            pOwnerState = (int (LEMBALL_FASTCALL *)(void *))
                (*(void ***)pOwner)[0x68 / sizeof(void *)];
            if (pOwnerState(pOwner) != 1) {
                SetWindowOwnerState(pOwner, 1);
                pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                    (*(void ***)pOwner)[0x50 / sizeof(void *)];
                pOwnerCallback(pOwner);
            }
        }

        pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x20 / sizeof(void *)];
        pOwnerCallback(pOwner);
        pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x44 / sizeof(void *)];
        pOwnerCallback(pOwner);
        return 0;
    }

    if (uMessage == 0x1c) {
        nSelectedDisplayMode = GetSelectedGraphicsDriverId();
        if (nSelectedDisplayMode == 1) {
            g_fFullscreenDisplayModeSuspended = 1;
            g_fFullscreenActivationState = 0;
            InvalidateRect(hWnd, 0, 0);
        } else if (nSelectedDisplayMode == 2 || nSelectedDisplayMode == 3) {
            if (wParam == 0) {
                g_fFullscreenActivationState = 0;
                g_fFullscreenDisplayModeSuspended = 1;
                InvalidateRect(hWnd, 0, 0);
            } else {
                if (pOwner != 0) {
                    pOwnerState = (int (LEMBALL_FASTCALL *)(void *))
                        (*(void ***)pOwner)[0x68 / sizeof(void *)];
                    if (pOwnerState(pOwner) == 0) {
                        SendMessageA(hWnd, 0x112, 0xf120, 0);
                    }
                }
                g_fFullscreenDisplayModeSuspended = 0;
                g_fFullscreenActivationState = 1;
                InvalidateRect(hWnd, 0, 0);
            }
        }

        if (pOwner != 0) {
            pOwnerResult = (LRESULT (LEMBALL_FASTCALL *)(void *))
                (*(void ***)pOwner)[0xa0 / sizeof(void *)];
            return pOwnerResult(pOwner);
        }
        return 0;
    }

    if (uMessage == 0x7e) {
        if (pSelectedDriver != 0) {
            unsigned short anDisplaySize[2];

            anDisplaySize[0] = (unsigned short)lParam;
            anDisplaySize[1] = (unsigned short)((unsigned long)lParam >> 16);
            ((VSGDI_SelectedGraphicsDriverRuntime *)pSelectedDriver)
                ->ResizeActiveDisplayState(anDisplaySize);
        }
        return 0;
    }

    if (uMessage == 0x100 || uMessage == 0x101) {
        WINDOW_OWNER_KEY_EVENT Event;

        Event.m_nType = uMessage == 0x100 ? 2 : 1;
        Event.m_lParam = lParam;
        AppendWindowOwnerEvent(&Event);
        return 0;
    }

    if (uMessage == 0x111 && pOwner != 0) {
        int nPayload;
        WINDOW_OWNER_COMMAND_EVENT Event;

        nPayload = ((WINDOW_OWNER_COMMAND_TABLE *)pOwner)
            ->LookupWindowCommandEventPayload(0, (int)wParam);
        if (nPayload == 0) {
            pOwnerResult = (LRESULT (LEMBALL_FASTCALL *)(void *))
                (*(void ***)pOwner)[0xa0 / sizeof(void *)];
            return pOwnerResult(pOwner);
        }

        Event.m_nType = 0xf;
        Event.m_nReserved = 0;
        Event.m_dwTime = timeGetTime();
        Event.m_nPayload = nPayload;
        AppendWindowOwnerEvent(&Event);
        return 0;
    }

    if ((uMessage == 0x201 || uMessage == 0x203 || uMessage == 0x204 ||
         uMessage == 0x206 || uMessage == 0x207 || uMessage == 0x209) &&
        pOwner != 0) {
        pGetOwnerFlags = (unsigned int (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x64 / sizeof(void *)];
        uOwnerFlags = pGetOwnerFlags(pOwner);
        nEventCode = 0;
        if ((uOwnerFlags & 0x1000) == 0) {
            if (uMessage == 0x201 || uMessage == 0x203) {
                nEventCode = 0x43;
            } else if (uMessage == 0x204 || uMessage == 0x206) {
                nEventCode = 0x44;
            } else {
                nEventCode = 0x45;
            }
        } else {
            if (uMessage == 0x201) {
                nEventCode = 0x43;
            } else if (uMessage == 0x203) {
                nEventCode = 0x46;
            } else if (uMessage == 0x204) {
                nEventCode = 0x44;
            } else if (uMessage == 0x206) {
                nEventCode = 0x47;
            } else if (uMessage == 0x207) {
                nEventCode = 0x45;
            } else {
                nEventCode = 0x48;
            }
        }

        xEvent = (short)((short)lParam + *(short *)((char *)pOwner + 0x0c));
        yEvent = (short)(((unsigned long)lParam >> 16) +
                         *(short *)((char *)pOwner + 0x0e));
        anEvent[0] = nEventCode;
        anEvent[1] = PackEventXYWords((unsigned short)xEvent, yEvent);
        anEvent[2] = 0;
        AppendWindowOwnerEvent(anEvent);
        if (g_nMouseButtonCaptureRefCount == 0) {
            SetCapture(hWnd);
        }
        ++g_nMouseButtonCaptureRefCount;
        return 0;
    }

    if (uMessage == 0x202 || uMessage == 0x205 || uMessage == 0x208) {
        nEventCode = uMessage == 0x202 ? 0x43 :
                     (uMessage == 0x205 ? 0x44 : 0x45);
        xEvent = (short)((short)lParam +
                         (pOwner != 0 ? *(short *)((char *)pOwner + 0x0c) : 0));
        yEvent = (short)(((unsigned long)lParam >> 16) +
                         (pOwner != 0 ? *(short *)((char *)pOwner + 0x0e) : 0));
        anEvent[0] = nEventCode;
        anEvent[1] = PackEventXYWords((unsigned short)xEvent, yEvent);
        anEvent[2] = 0;
        AppendWindowOwnerEvent(anEvent);
        if (g_nMouseButtonCaptureRefCount != 0) {
            --g_nMouseButtonCaptureRefCount;
            if (g_nMouseButtonCaptureRefCount == 0) {
                ReleaseCapture();
            }
        }
        return 0;
    }

    if (uMessage == 7 || uMessage == 8) {
        if (g_pCachedCurrentContextWindow != 0 &&
            g_pCachedCurrentContextOwner != 0) {
            pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                (*(void ***)g_pCachedCurrentContextOwner)[0x94 / sizeof(void *)];
            pOwnerCallback(g_pCachedCurrentContextOwner);
            pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                (*(void ***)g_pCachedCurrentContextOwner)[0x9c / sizeof(void *)];
            pOwnerCallback(g_pCachedCurrentContextOwner);
        }
        if (pOwner != 0) {
            pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                (*(void ***)pOwner)[0x90 / sizeof(void *)];
            pOwnerCallback(pOwner);
            pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                (*(void ***)pOwner)[0x98 / sizeof(void *)];
            pOwnerCallback(pOwner);
        }
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);
    }

    (void)wParam;
    return DefWindowProcA(hWnd, uMessage, wParam, lParam);
}

// FUNCTION: LEMBALL 0x00473790
void __cdecl ShowFatalGetLastErrorMessageAndExit(const char *pszMessage) {
    VSINIT_FixedBufferStream Buffer;
    VSINIT_FormattedOutputStream Message;
    char szBuffer[0x80];
    unsigned int dwError;

    dwError = GetLastError();
    ConstructFixedBufferStream(&Buffer, szBuffer, sizeof(szBuffer), 0);
    ConstructFormattedOutputStream(&Message, &Buffer, 1);
    AppendCStringToStream(&Message, pszMessage);
    Message.AppendCharToStreamVariant('\n');
    AppendCStringToStream(&Message, g_szFatalGetLastErrorPrefix);
    Message.AppendSignedIntToStreamVariant((int)dwError);
    AppendCStringToStream(&Message, g_szFatalGetLastErrorSeparator);
    AppendHexUIntToStream(&Message, dwError);
    RestoreStreamFormatSubobjectVtable(&Message.m_TargetState);
    DestroyFixedBufferStream(&Buffer);
    ConstructStreamFormatState(&Message.m_TargetState);
    MessageBoxA(0, szBuffer, g_szFatalErrorTitle, 0);
    ExitProcess(0xaaaa);
}

// FUNCTION: LEMBALL 0x00465050
void LEMBALL_STDCALL PollCursorPositionEvent(void) {
    tagPOINT Point;
    WINDOW_OWNER_CURSOR_EVENT Event;

    GetCursorPos(&Point);
    if (Point.x == g_nLastCursorScreenX &&
        Point.y == g_nLastCursorScreenY) {
        return;
    }

    g_nLastCursorScreenX = Point.x;
    g_nLastCursorScreenY = Point.y;
    if (IsDisplayModeWindowedRange(
            (const int *)g_pSelectedGraphicsDriverRuntime) &&
        g_pCachedCurrentContextOwner != 0) {
        g_nLastCursorScreenX +=
            *(short *)((char *)g_pCachedCurrentContextOwner + 0x0c);
        g_nLastCursorScreenY +=
            *(short *)((char *)g_pCachedCurrentContextOwner + 0x0e);
    }

    Event.m_nType = 7;
    Event.m_dwTime = timeGetTime();
    Event.m_nPackedPosition =
        PackEventXYWords(g_nLastCursorScreenX, g_nLastCursorScreenY);
    Event.m_nReserved0c = 0;
    Event.m_nReserved10 = 0;
    ((RENDER_DISPATCH_QUEUE_APPEND *)g_pSharedRenderDispatchQueue)
        ->Append(&Event);
}

// FUNCTION: LEMBALL 0x00465110
int RegisterBaseWindowClass(void) {
    WNDCLASSA WindowClass;
    HICON hIcon;
    HCURSOR hCursor;
    ATOM nClass;

    WindowClass.style = 0x0b;
    WindowClass.lpfnWndProc = WindowOwnerWindowProc;
    WindowClass.cbClsExtra = 0;
    WindowClass.cbWndExtra = 4;
    WindowClass.hInstance = g_hApplicationInstance;
    hIcon = g_hApplicationIcon;
    if (hIcon == 0) {
        hIcon = LoadIconA((HINSTANCE)0, (LPCSTR)0x7f00);
    }
    WindowClass.hIcon = hIcon;
    WindowClass.hbrBackground = (HBRUSH)GetStockObject(4);
    WindowClass.lpszMenuName = 0;
    WindowClass.lpszClassName = g_pszVsBaseWindowClassName;
    hCursor = LoadCursorA((HINSTANCE)0, (LPCSTR)0x7f00);
    WindowClass.hCursor = hCursor;

    nClass = RegisterClassA(&WindowClass);
    SetCursor(hCursor);
    ShowCursor(1);
    if (nClass == 0) {
        ShowFatalGetLastErrorMessageAndExit(g_szUnableToRegisterBaseWindowClassError);
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00465CC0
void *LEMBALL_FASTCALL ConstructRootGeometryOwner(void *pOwner) {
    void *pRegistry;

    *(short *)((char *)pOwner + 0x0a) = 0;
    *(short *)((char *)pOwner + 0x08) = 0;
    *(short *)((char *)pOwner + 0x0e) = 0;
    *(short *)((char *)pOwner + 0x0c) = 0;
    *(short *)((char *)pOwner + 0x12) = 0;
    *(short *)((char *)pOwner + 0x10) = 0;
    *(int *)((char *)pOwner + 0x24) = 0;
    *(short *)((char *)pOwner + 0x16) = 0;
    *(int *)((char *)pOwner + 0x28) = 0;
    *(short *)((char *)pOwner + 0x14) = 0;
    *(int *)((char *)pOwner + 0x2c) = 0;
    *(short *)((char *)pOwner + 0x1a) = 0;
    *(void **)pOwner = g_pRootGeometryOwnerVtable;
    *(short *)((char *)pOwner + 0x18) = 0;
    *(int *)((char *)pOwner + 0x1c) = 0;
    *(int *)((char *)pOwner + 0x34) = 1;

    if (g_nLiveWindowOwnerBaseCount++ == 0) {
        pRegistry = AllocateVSMemBlock(0xc);
        if (pRegistry != 0) {
            *(int *)((char *)pRegistry + 0) = 0;
            *(int *)((char *)pRegistry + 4) = 0;
            *(int *)((char *)pRegistry + 8) = 0;
        } else {
            pRegistry = 0;
        }
        g_pRootZrleGeometryOwnerRegistry = pRegistry;
    }

    *(int *)((char *)pOwner + 0x38) = 1;
    *(int *)((char *)pOwner + 4) = 0;
    return pOwner;
}
