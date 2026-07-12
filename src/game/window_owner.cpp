#include "game/window_owner.h"

#include "engine/memory_arena.h"
#include "engine/graphics_driver.h"
#include "engine/runtime_init.h"
#include "main.h"
#include "platform/win32.h"

static const char g_szVsBaseWindowClassName[] = "VS_Base_Window_Class";
static const char *g_pszVsBaseWindowClassName = g_szVsBaseWindowClassName;
static const char g_szUnableToRegisterBaseWindowClassError[] =
    "Unable to register base window class";
static const char g_szFatalGetLastErrorPrefix[] = " GetLastError()=";
static const char g_szFatalGetLastErrorSeparator[] = ", ";
static const char g_szFatalErrorTitle[] = "FATAL ERROR";

static int g_fWindowMessagePassthroughMode = 0;
// GLOBAL: LEMBALL 0x004a1f64
void *g_pCachedCurrentContextWindow = 0;
// GLOBAL: LEMBALL 0x004a1f68
void *g_pCachedCurrentContextOwner = 0;
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
extern int RegisterBaseWindowClass(void);
extern unsigned int MapWindowOwnerFlagsToWin32Style(unsigned int uFlags);
int LEMBALL_FASTCALL IsDisplayModeWindowedRange(const int *pnDisplayMode);
extern "C" DWORD WINAPI timeGetTime(void);

struct WINDOW_OWNER_RECT {
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
};

extern "C" HMENU WINAPI GetMenu(HWND hWnd);
extern "C" UINT WINAPI EnableMenuItem(HMENU hMenu,
                                       UINT uIDEnableItem,
                                       UINT uEnable);
extern "C" UINT WINAPI CheckMenuItem(HMENU hMenu,
                                      UINT uIDCheckItem,
                                      UINT uCheck);
extern "C" HMENU WINAPI LoadMenuA(HINSTANCE hInstance, LPCSTR lpMenuName);
extern "C" BOOL WINAPI SetMenu(HWND hWnd, HMENU hMenu);
extern "C" BOOL WINAPI DestroyMenu(HMENU hMenu);
extern "C" BOOL WINAPI GetWindowRect(HWND hWnd, WINDOW_OWNER_RECT *lpRect);
extern "C" BOOL WINAPI GetClientRect(HWND hWnd, WINDOW_OWNER_RECT *lpRect);
extern "C" BOOL WINAPI AdjustWindowRect(WINDOW_OWNER_RECT *lpRect,
                                          DWORD dwStyle,
                                          BOOL bMenu);
extern "C" BOOL WINAPI SetWindowPos(HWND hWnd,
                                     HWND hWndInsertAfter,
                                     int X,
                                     int Y,
                                     int cx,
                                     int cy,
                                     UINT uFlags);

int g_nLiveWindowOwnerBaseCount = 0;
void *g_pRootZrleGeometryOwnerRegistry = 0;

struct WINDOW_OWNER_REGISTRY_NODE {
    void *m_pOwner;
    WINDOW_OWNER_REGISTRY_NODE *m_pNext;
    WINDOW_OWNER_REGISTRY_NODE *m_pPrevious;
};

struct GAME_RootGeometryOwnerChildNode {
    GAME_RootGeometryOwner *m_pOwner;
    GAME_RootGeometryOwnerChildNode *m_pNext;
    GAME_RootGeometryOwnerChildNode *m_pPrevious;
};

struct GAME_GeometryDispatchHelperDeleteInterface {
    virtual void Delete(int nDeleteFlag) = 0;
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

// FUNCTION: LEMBALL 0x00465F80
void GAME_RootGeometryOwner::RetainRootGeometryOwner(void) {
    WINDOW_OWNER_REGISTRY_NODE *pNode;
    WINDOW_OWNER_REGISTRY_NODE *pPrevious;

    ++g_nLiveRootZrleGeometryOwnerCount;
    if (m_pParent20 == 0) {
        pNode = (WINDOW_OWNER_REGISTRY_NODE *)AllocateVSMemBlock(0x0c);
        if (pNode != 0) {
            pNode->m_pOwner = this;
            pNode->m_pNext = 0;
            pNode->m_pPrevious = 0;
        }
        pPrevious = (WINDOW_OWNER_REGISTRY_NODE *)
            *(void **)((char *)g_pRootZrleGeometryOwnerRegistry + 4);
        pNode->m_pPrevious = pPrevious;
        if (pPrevious != 0) {
            pPrevious->m_pNext = pNode;
        }
        *(void **)((char *)g_pRootZrleGeometryOwnerRegistry + 4) = pNode;
        if (*(void **)g_pRootZrleGeometryOwnerRegistry == 0) {
            *(void **)g_pRootZrleGeometryOwnerRegistry = pNode;
        }
        ++*(int *)((char *)g_pRootZrleGeometryOwnerRegistry + 8);
    }
    ++m_cRetains04;
}

struct GAME_RootGeometryDispatchHelperView {
    char m_abReserved00[0x30];
    GAME_XYPair m_Size30;
    GAME_XYPair m_Position34;
    char m_abReserved38[0x1c];
    GAME_XYPair m_Origin54;
    GAME_XYPair m_SourcePosition58;

    void StoreRootGeometrySubrectPayload(const GAME_XYPair *pRect,
                                         GAME_XYPair origin,
                                         const GAME_XYPair *pSourcePosition);
};

// FUNCTION: LEMBALL 0x00465A50
GAME_XYPair &GAME_XYPair::operator=(const GAME_XYPair &other) {
    m_x = other.m_x;
    m_y = other.m_y;
    return *this;
}

// FUNCTION: LEMBALL 0x004662E0
void GAME_RootGeometryOwner::RefreshWindowOwnerMenuState(void) {
}

// FUNCTION: LEMBALL 0x00465DB0
void GAME_RootGeometryOwner::StoreRootGeometrySourceRect(const GAME_XYPair *pRect) {
    m_SourceSize10.m_x = pRect[0].m_x;
    m_SourceSize10.m_y = pRect[0].m_y;
    m_SourcePosition14.m_x = pRect[1].m_x;
    m_SourcePosition14.m_y = pRect[1].m_y;
    CanonicalizeRootGeometrySubrect();
}

// FUNCTION: LEMBALL 0x004662F0
void GAME_RootGeometryOwner::SetWindowOwnerMenuDefinition(const unsigned int *, void *) {
}

// FUNCTION: LEMBALL 0x00465E60
void GAME_RootGeometryOwner::EnsureRootGeometryDispatchHelper(void) {
    /* Construction of the 0x60-byte dispatch helper is intentionally kept
     * behind its own ownership boundary.  Its two vtables (00499D18 and
     * 00499D08) are not yet closed symbolically, so manufacturing a partial
     * helper here would make later virtual destruction unsafe. */
    if ((GetWindowOwnerFlags() & 0x800) == 0 ||
        m_pGeometryDispatchHelper1C != 0) {
        return;
    }
}

// FUNCTION: LEMBALL 0x00465FE0
void GAME_RootGeometryOwner::ReleaseRootGeometryOwner(void) {
    WINDOW_OWNER_REGISTRY_NODE *pNode;
    WINDOW_OWNER_REGISTRY_NODE *pNext;
    WINDOW_OWNER_REGISTRY_NODE *pPrevious;
    WINDOW_OWNER_REGISTRY_NODE *pFirst;

    --g_nLiveRootZrleGeometryOwnerCount;
    --m_cRetains04;
    if (m_pGeometryDispatchHelper1C != 0) {
        ((GAME_GeometryDispatchHelperDeleteInterface *)
             m_pGeometryDispatchHelper1C)
            ->Delete(1);
        m_pGeometryDispatchHelper1C = 0;
    }

    if (m_pParent20 != 0) {
        m_pParent20->RemoveQueuedRenderSinkValueNode(this);
        return;
    }

    pFirst = *(WINDOW_OWNER_REGISTRY_NODE **)g_pRootZrleGeometryOwnerRegistry;
    pNode = pFirst;
    while (pNode != 0 && pNode->m_pOwner != this) {
        pNode = pNode->m_pNext;
    }
    if (pNode == 0) {
        return;
    }

    pNext = pNode->m_pNext;
    pPrevious = pNode->m_pPrevious;
    FreeVSMemBlock(pNode);
    if (pNext == 0) {
        *(WINDOW_OWNER_REGISTRY_NODE **)
            ((char *)g_pRootZrleGeometryOwnerRegistry + 4) = pPrevious;
    } else {
        pNext->m_pPrevious = pPrevious;
    }
    if (pPrevious == 0) {
        *(WINDOW_OWNER_REGISTRY_NODE **)g_pRootZrleGeometryOwnerRegistry =
            pNext;
    } else {
        pPrevious->m_pNext = pNext;
    }
    --*(int *)((char *)g_pRootZrleGeometryOwnerRegistry + 8);
}

// FUNCTION: LEMBALL 0x00466370
void GAME_RootGeometryDispatchHelperView::StoreRootGeometrySubrectPayload(const GAME_XYPair *pRect, GAME_XYPair origin, const GAME_XYPair *pSourcePosition) {
    m_Size30.m_x = pRect[0].m_x;
    m_Size30.m_y = pRect[0].m_y;
    m_Position34.m_x = pRect[1].m_x;
    m_Position34.m_y = pRect[1].m_y;
    m_Origin54.m_x = origin.m_x;
    m_Origin54.m_y = origin.m_y;
    m_SourcePosition58.m_x = pSourcePosition->m_x;
    m_SourcePosition58.m_y = pSourcePosition->m_y;
}

// FUNCTION: LEMBALL 0x00466060
void GAME_RootGeometryOwner::CanonicalizeRootGeometrySubrect(void) {
    GAME_XYPair aRect[2];
    GAME_XYPair origin;
    GAME_XYPair callOrigin;

    if (m_pGeometryDispatchHelper1C != 0) {
        aRect[0].m_x = m_SourceSize10.m_x;
        aRect[0].m_y = m_SourceSize10.m_y;
        if ((int)aRect[0].m_x * (int)aRect[0].m_y == 0) {
            aRect[0].m_x = m_Size08.m_x;
            aRect[0].m_y = m_Size08.m_y;
            aRect[1].m_x = m_Position0C.m_x;
            aRect[1].m_y = m_Position0C.m_y;
        } else {
            aRect[1].m_x =
                (short)(m_SourcePosition14.m_x + m_Position0C.m_x);
            aRect[1].m_y =
                (short)(m_SourcePosition14.m_y + m_Position0C.m_y);
        }
        origin.m_x = m_Origin18.m_x;
        origin.m_y = m_Origin18.m_y;
        if (m_pParent20 == 0) {
            origin.m_x = 0;
            origin.m_y = 0;
        }
        callOrigin = origin;
        ((GAME_RootGeometryDispatchHelperView *)m_pGeometryDispatchHelper1C)
            ->StoreRootGeometrySubrectPayload(
                aRect, callOrigin, &m_SourcePosition14);
    }
}

// FUNCTION: LEMBALL 0x00466260
void GAME_RootGeometryOwner::OffsetRootGeometryPosition(unsigned int uPackedDelta) {
    m_Position0C.m_x =
        (short)(m_Position0C.m_x + (short)(uPackedDelta & 0xffff));
    m_Position0C.m_y =
        (short)(m_Position0C.m_y + (short)(uPackedDelta >> 16));
    CanonicalizeRootGeometrySubrectAlt();
}

// FUNCTION: LEMBALL 0x00466160
void GAME_RootGeometryOwner::CanonicalizeRootGeometrySubrectAlt(void) {
    GAME_XYPair aRect[2];
    GAME_XYPair origin;
    GAME_XYPair callOrigin;

    if (m_pGeometryDispatchHelper1C != 0) {
        aRect[0].m_x = m_SourceSize10.m_x;
        aRect[0].m_y = m_SourceSize10.m_y;
        if ((int)aRect[0].m_x * (int)aRect[0].m_y == 0) {
            aRect[0].m_x = m_Size08.m_x;
            aRect[0].m_y = m_Size08.m_y;
            aRect[1].m_x = m_Position0C.m_x;
            aRect[1].m_y = m_Position0C.m_y;
        } else {
            aRect[1].m_x =
                (short)(m_SourcePosition14.m_x + m_Position0C.m_x);
            aRect[1].m_y =
                (short)(m_SourcePosition14.m_y + m_Position0C.m_y);
        }
        origin.m_x = m_Origin18.m_x;
        origin.m_y = m_Origin18.m_y;
        if (m_pParent20 == 0) {
            origin.m_x = 0;
            origin.m_y = 0;
        }
        callOrigin = origin;
        ((GAME_RootGeometryDispatchHelperView *)m_pGeometryDispatchHelper1C)
            ->StoreRootGeometrySubrectPayload(
                aRect, callOrigin, &m_SourcePosition14);
    }
}

// FUNCTION: LEMBALL 0x00466280
void GAME_RootGeometryOwner::SetRootGeometryScaleFactor(int) {
    GAME_RootGeometryOwnerChildNode *pNode;

    if (m_pGeometryDispatchHelper1C != 0) {
        *(int *)((char *)m_pGeometryDispatchHelper1C + 0x50) =
            m_nScaleFactor38;
    }
    pNode = m_pChildren24;
    while (pNode != 0) {
        pNode->m_pOwner->SetWindowOwnerScaleFactor(m_nScaleFactor38);
        pNode = pNode->m_pNext;
    }
}

// FUNCTION: LEMBALL 0x00466300
void GAME_RootGeometryOwner::SetRootGeometryRect(const GAME_XYPair *pRect) {
    m_Size08.m_x = pRect[0].m_x;
    m_Size08.m_y = pRect[0].m_y;
    m_Position0C.m_x = pRect[1].m_x;
    m_Position0C.m_y = pRect[1].m_y;
}

// FUNCTION: LEMBALL 0x00466330
void GAME_RootGeometryOwner::FinalizeRootGeometryRect(const GAME_XYPair *) {
}

// FUNCTION: LEMBALL 0x00466340
void GAME_RootGeometryOwner::ReservedWindowOwnerSlot23(void) {
}

// FUNCTION: LEMBALL 0x00466350
int GAME_RootGeometryOwner::IsCurrentWindowOwnerContext(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x00465A70
void GAME_RootGeometryOwner::ReservedWindowOwnerSlot32(void) {
}

// FUNCTION: LEMBALL 0x00465A80
void GAME_RootGeometryOwner::ReservedWindowOwnerSlot33(int) {
}

// FUNCTION: LEMBALL 0x00466360
void GAME_RootGeometryOwner::ReservedWindowOwnerSlot34(int) {
}

struct GAME_WindowOwnerMenuEntry {
    const char *m_pszText00;
    unsigned int m_nCommandId04;
    int m_nPosition08;
    int m_fEnabled0C;
    int m_fChecked10;
    int m_nReserved14;
};

// FUNCTION: LEMBALL 0x00465660
void GAME_WindowOwnerBase::RefreshWindowOwnerMenuState(void) {
    GAME_WindowOwnerMenuEntry **ppMenu;
    GAME_WindowOwnerMenuEntry *pEntry;
    HMENU hMenu;

    hMenu = GetMenu(m_hWindow44);
    ppMenu = (GAME_WindowOwnerMenuEntry **)m_pMenuDefinition3C;
    while (*ppMenu != 0) {
        pEntry = *ppMenu;
        while (pEntry->m_pszText00 != 0) {
            EnableMenuItem(hMenu,
                           pEntry->m_nCommandId04,
                           pEntry->m_fEnabled0C ? 0 : 1);
            CheckMenuItem(hMenu,
                          pEntry->m_nCommandId04,
                          pEntry->m_fChecked10 ? 8 : 0);
            ++pEntry;
        }
        ++ppMenu;
    }
}

// FUNCTION: LEMBALL 0x004656F0
void GAME_WindowOwnerBase::SetWindowOwnerMenuDefinition(const unsigned int *pnMenuId, void *pMenuDefinition) {
    HMENU hOldMenu;

    hOldMenu = GetMenu(m_hWindow44);
    m_nMenuResourceId40 = *pnMenuId;
    m_pMenuDefinition3C = pMenuDefinition;
    if (pMenuDefinition != 0) {
        SetMenu(m_hWindow44,
                LoadMenuA(g_hApplicationInstance,
                          (LPCSTR)(unsigned long)
                              (m_nMenuResourceId40 & 0xffff)));
        RefreshWindowOwnerMenuState();
    }
    if (hOldMenu != 0) {
        DestroyMenu(hOldMenu);
    }
}

// FUNCTION: LEMBALL 0x00465790
void GAME_WindowOwnerBase::SetRootGeometryScaleFactor(int nScaleFactor) {
    WINDOW_OWNER_RECT WindowRect;
    WINDOW_OWNER_RECT ClientRect;

    GAME_RootGeometryOwner::SetRootGeometryScaleFactor(nScaleFactor);
    if (m_hWindow44 != 0 &&
        *(HWND *)((char *)GetDisplayState() + 0x10) != m_hWindow44) {
        GetWindowRect(m_hWindow44, &WindowRect);
        GetClientRect(m_hWindow44, &ClientRect);
        WindowRect.right -= WindowRect.left;
        WindowRect.bottom -= WindowRect.top;
        WindowRect.right -= ClientRect.right;
        WindowRect.bottom -= ClientRect.bottom;
        SetWindowPos(m_hWindow44,
                     0,
                     0,
                     0,
                     ClientRect.right + WindowRect.right,
                     ClientRect.bottom + WindowRect.bottom,
                     6);
    }
}

// FUNCTION: LEMBALL 0x00465820
void GAME_WindowOwnerBase::SetRootGeometryRect(const GAME_XYPair *pRect) {
    GAME_XYPair size;
    GAME_XYPair position;
    GAME_XYPair *pParentPosition;
    tagPOINT ScreenPosition;
    WINDOW_OWNER_RECT WindowRect;
    WINDOW_OWNER_RECT ClientRect;
    unsigned int uStyle;

    size.m_x = pRect[0].m_x;
    size.m_y = pRect[0].m_y;
    position.m_x = pRect[1].m_x;
    position.m_y = pRect[1].m_y;

    if (m_hWindow44 != 0 &&
        *(HWND *)((char *)GetDisplayState() + 0x10) != m_hWindow44) {
        if (IsDisplayModeWindowedRange(
                (const int *)g_pSelectedGraphicsDriverRuntime)) {
            position.m_x = 0;
            position.m_y = 0;
        }

        ClientRect.left = position.m_x;
        ClientRect.top = position.m_y;
        ClientRect.right = (short)(position.m_x + size.m_x);
        ClientRect.bottom = (short)(position.m_y + size.m_y);
        GetWindowRect(m_hWindow44, &WindowRect);
        ScreenPosition.x = 0;
        ScreenPosition.y = 0;
        ClientToScreen(m_hWindow44, &ScreenPosition);
        WindowRect.left += position.m_x - ScreenPosition.x;
        WindowRect.top += position.m_y - ScreenPosition.y;
        uStyle = MapWindowOwnerFlagsToWin32Style(GetWindowOwnerFlags());
        AdjustWindowRect(&ClientRect, uStyle, m_pMenuDefinition3C != 0);
        SetWindowPos(m_hWindow44,
                     0,
                     WindowRect.left,
                     WindowRect.top,
                     ClientRect.right - ClientRect.left,
                     ClientRect.bottom - ClientRect.top,
                     4);
        return;
    }

    pParentPosition = (GAME_XYPair *)((char *)m_pParent20 + 0x0c);
    ScreenPosition.x = pParentPosition->m_x + position.m_x;
    ScreenPosition.y = pParentPosition->m_y + position.m_y;
    ClientToScreen(((GAME_WindowOwnerBase *)m_pParent20)->m_hWindow44,
                   &ScreenPosition);
    m_Position0C.m_x = (short)ScreenPosition.x;
    m_Position0C.m_y = (short)ScreenPosition.y;
    m_Size08.m_x = size.m_x;
    m_Size08.m_y = size.m_y;
    m_Origin18.m_x = position.m_x;
    m_Origin18.m_y = position.m_y;
    CanonicalizeRootGeometrySubrectAlt();
    ReservedWindowOwnerSlot18();
    CanonicalizeRootGeometrySubrect();
    ReservedWindowOwnerSlot17();
}

// FUNCTION: LEMBALL 0x00465A00
void GAME_WindowOwnerBase::FinalizeRootGeometryRect(const GAME_XYPair *pPoint) {
    GAME_XYPair aRect[2];

    aRect[0].m_x = m_Size08.m_x;
    aRect[0].m_y = m_Size08.m_y;
    aRect[1].m_x = pPoint->m_x;
    aRect[1].m_y = pPoint->m_y;
    SetRootGeometryRect(aRect);
}

// FUNCTION: LEMBALL 0x00464FA0
void GAME_WindowOwnerBase::MoveWindowOwnerOrigin(const GAME_XYPair *pPoint) {
    GAME_RootGeometryOwnerChildNode *pNode;
    GAME_XYPair delta;

    delta.m_x = (short)(pPoint->m_x - m_Origin18.m_x);
    delta.m_y = (short)(pPoint->m_y - m_Origin18.m_y);
    pNode = m_pChildren24;
    while (pNode != 0) {
        pNode->m_pOwner->OffsetRootGeometryPosition(
            (unsigned int)(unsigned short)delta.m_x |
            ((unsigned int)(unsigned short)delta.m_y << 16));
        pNode->m_pOwner->ReservedWindowOwnerSlot18();
        pNode = pNode->m_pNext;
    }

    m_Position0C.m_x = (short)(m_Position0C.m_x + delta.m_x);
    m_Position0C.m_y = (short)(m_Position0C.m_y + delta.m_y);
    m_Origin18.m_x = pPoint->m_x;
    m_Origin18.m_y = pPoint->m_y;
    CanonicalizeRootGeometrySubrectAlt();
    ReservedWindowOwnerSlot18();
    if (m_hWindow44 != 0) {
        SetWindowPos(m_hWindow44,
                     0,
                     pPoint->m_x,
                     pPoint->m_y,
                     0,
                     0,
                     5);
    }
}

// FUNCTION: LEMBALL 0x004644F0
void GAME_WindowOwnerBase::ReservedWindowOwnerSlot15(void) {
}

// FUNCTION: LEMBALL 0x00464500
void GAME_WindowOwnerBase::ReservedWindowOwnerSlot16(void) {
}

// FUNCTION: LEMBALL 0x00464510
void GAME_WindowOwnerBase::ReservedWindowOwnerSlot17(void) {
}

// FUNCTION: LEMBALL 0x00465A90
unsigned int GAME_WindowOwnerBase::GetWindowOwnerFlags(void) {
    return 0;
}

// FUNCTION: LEMBALL 0x004655F0
void GAME_WindowOwnerBase::InvalidateWindowOwnerRect(const GAME_XYPair *pExtent) {
    WINDOW_OWNER_RECT Rect;

    Rect.left = 0;
    Rect.top = 0;
    if (pExtent == 0) {
        Rect.right = 1;
        Rect.bottom = 1;
    } else {
        Rect.right = pExtent->m_x;
        Rect.bottom = pExtent->m_y;
    }
    InvalidateRect(m_hWindow44, &Rect, 0);
}

// FUNCTION: LEMBALL 0x004655A0
void GAME_WindowOwnerBase::DestroyWindowOwnerHwnd(void) {
    GAME_RootGeometryOwnerChildNode *pNode;
    GAME_RootGeometryOwnerChildNode *pNext;
    int nSelectedDriver;

    if (m_cRetains04 == 0) {
        return;
    }
    pNode = m_pChildren24;
    while (pNode != 0) {
        pNext = pNode->m_pNext;
        pNode->m_pOwner->DestroyWindowOwnerHwnd();
        pNode = pNext;
    }
    ReservedWindowOwnerSlot16();
    ReleaseRootGeometryOwner();

    nSelectedDriver =
        ((VSGDI_SelectedGraphicsDriverRuntime *)
             g_pSelectedGraphicsDriverRuntime)
            ->m_nSelectedDriver;
    if ((nSelectedDriver < 4 || 5 < nSelectedDriver) &&
        m_hWindow44 != 0) {
        DestroyWindow(m_hWindow44);
    }
}

// FUNCTION: LEMBALL 0x00465A40
void GAME_WindowOwnerBase::ReservedWindowOwnerSlot34(int) {
}

// FUNCTION: LEMBALL 0x00464F10
void GAME_WindowOwnerBase::PropagateWindowOwnerPosition(const GAME_XYPair *pPoint) {
    GAME_RootGeometryOwnerChildNode *pNode;
    GAME_XYPair delta;

    delta.m_x = (short)(pPoint->m_x - m_Position0C.m_x);
    delta.m_y = (short)(pPoint->m_y - m_Position0C.m_y);
    pNode = m_pChildren24;
    while (pNode != 0) {
        pNode->m_pOwner->OffsetRootGeometryPosition(
            (unsigned int)(unsigned short)delta.m_x |
            ((unsigned int)(unsigned short)delta.m_y << 16));
        pNode = pNode->m_pNext;
    }
    m_Position0C.m_x = pPoint->m_x;
    m_Position0C.m_y = pPoint->m_y;
    m_Origin18.m_x = pPoint->m_x;
    m_Origin18.m_y = pPoint->m_y;
    CanonicalizeRootGeometrySubrectAlt();
}

// FUNCTION: LEMBALL 0x00465640
LRESULT GAME_WindowOwnerBase::DispatchWindowMessage(UINT uMessage, WPARAM wParam, LPARAM lParam) {
    return DefWindowProcA(m_hWindow44, uMessage, wParam, lParam);
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
GAME_RootGeometryOwner::GAME_RootGeometryOwner(void) {
    void *pRegistry;

    m_Size08.m_y = 0;
    m_Size08.m_x = 0;
    m_Position0C.m_y = 0;
    m_Position0C.m_x = 0;
    m_SourceSize10.m_y = 0;
    m_SourceSize10.m_x = 0;
    m_pChildren24 = 0;
    m_SourcePosition14.m_y = 0;
    m_pChildrenTail28 = 0;
    m_SourcePosition14.m_x = 0;
    m_cChildren2C = 0;
    m_Origin18.m_y = 0;
    m_Origin18.m_x = 0;
    m_pGeometryDispatchHelper1C = 0;
    m_fGeometryEnabled34 = 1;

    if (g_nLiveWindowOwnerBaseCount++ == 0) {
        pRegistry = AllocateVSMemBlock(0x0c);
        if (pRegistry != 0) {
            *(int *)((char *)pRegistry + 0) = 0;
            *(int *)((char *)pRegistry + 4) = 0;
            *(int *)((char *)pRegistry + 8) = 0;
        } else {
            pRegistry = 0;
        }
        g_pRootZrleGeometryOwnerRegistry = pRegistry;
    }

    m_nScaleFactor38 = 1;
    m_cRetains04 = 0;
}

// FUNCTION: LEMBALL 0x00465D50
GAME_RootGeometryOwner::~GAME_RootGeometryOwner(void) {
    WINDOW_OWNER_REGISTRY_NODE *pRegistryNode;
    WINDOW_OWNER_REGISTRY_NODE *pNextRegistryNode;
    GAME_RootGeometryOwnerChildNode *pChildNode;
    GAME_RootGeometryOwnerChildNode *pNextChildNode;
    void *pRegistry;

    pRegistry = g_pRootZrleGeometryOwnerRegistry;
    --g_nLiveWindowOwnerBaseCount;
    if (g_nLiveWindowOwnerBaseCount == 0 &&
        g_pRootZrleGeometryOwnerRegistry != 0) {
        pRegistryNode =
            *(WINDOW_OWNER_REGISTRY_NODE **)g_pRootZrleGeometryOwnerRegistry;
        while (pRegistryNode != 0) {
            pNextRegistryNode = pRegistryNode->m_pNext;
            FreeVSMemBlock(pRegistryNode);
            pRegistryNode = pNextRegistryNode;
        }
        FreeVSMemBlock(pRegistry);
    }

    pChildNode = m_pChildren24;
    while (pChildNode != 0) {
        pNextChildNode = pChildNode->m_pNext;
        FreeVSMemBlock(pChildNode);
        pChildNode = pNextChildNode;
    }
}

// FUNCTION: LEMBALL 0x004651D0
GAME_WindowOwnerBase::GAME_WindowOwnerBase(void) {
    if (g_nLiveWindowOwnerBaseCount == 1) {
        RegisterBaseWindowClass();
    }
    m_hWindow44 = 0;
}

// FUNCTION: LEMBALL 0x00465570
GAME_WindowOwnerBase::~GAME_WindowOwnerBase(void) {
    if (g_nLiveWindowOwnerBaseCount == 1) {
        ShowCursor(0);
    }
}
