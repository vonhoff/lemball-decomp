#include "GDI/VSWINDOW.H"
#include "GAME.H"

#include "MEMORY/VSMEM.H"
#include "GDI/VSGDI.H"
#include "CORE/VSINIT.H"
#include "SHELL/ENTRY.H"
#include "CORE/WIN32.H"

DECOMP_SIZE_ASSERT(VS_XYPair, 0x04);
DECOMP_SIZE_ASSERT(VS_RootGeometryOwner, 0x3c);
DECOMP_SIZE_ASSERT(VS_WindowOwnerBase, 0x4c);

static const char g_szVsBaseWindowClassName[] = "VS_Base_Window_Class";
static const char *g_pszVsBaseWindowClassName = g_szVsBaseWindowClassName;
static const char g_szUnableToRegisterVSBaseWindowClassError[] =
    "Unable to register base window class";
static const char g_szUnableToCreateWindowError[] =
    "Unable to create window";
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
// GLOBAL: LEMBALL 0x004a0770
int g_fFullscreenDisplayModeSuspended = 0;
static int g_nMouseButtonCaptureRefCount = 0;
static int g_anSavedMouseAccelerationParams[3];
static int g_nSavedMouseTrailsCount = 0;
static const char g_szQuittingStatusMessage[] = "QUIT\n";
static const char g_szQuitStatusMessage[] = "QUIT\n";

extern int g_fRootHelperGeometryDispatchSuppressed;
extern FARPROC g_pApplyFullscreenDisplayModeThunk;
extern void *g_pSharedRenderDispatchQueue;
extern void *g_pSharedGeometryHelper;
extern int RegisterVSBaseWindowClass(void);
extern unsigned int MapWindowOwnerFlagsToWin32Style(unsigned int uFlags);
int LEMBALL_FASTCALL IsDisplayModeWindowedRange(const int *pnDisplayMode);
extern "C" DWORD WINAPI timeGetTime(void);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);

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

struct WINDOW_OWNER_PAINTSTRUCT {
    void *hdc;
    BOOL fErase;
    WINDOW_OWNER_RECT rcPaint;
    BOOL fRestore;
    BOOL fIncUpdate;
    unsigned char rgbReserved[32];
};

extern "C" void *WINAPI BeginPaint(HWND hWnd,
                                     WINDOW_OWNER_PAINTSTRUCT *pPaint);
extern "C" BOOL WINAPI EndPaint(HWND hWnd,
                                 const WINDOW_OWNER_PAINTSTRUCT *pPaint);
extern "C" void *WINAPI GetDC(HWND hWnd);
extern "C" int WINAPI ReleaseDC(HWND hWnd, void *hDc);

// GLOBAL: LEMBALL 0x004A1FEC
int g_nLiveWindowOwnerBaseCount = 0;
// GLOBAL: LEMBALL 0x004A1FF0
void *g_pRootZrleGeometryOwnerRegistry = 0;

struct WINDOW_OWNER_REGISTRY_NODE {
    void *m_pOwner;
    WINDOW_OWNER_REGISTRY_NODE *m_pNext;
    WINDOW_OWNER_REGISTRY_NODE *m_pPrevious;
};

struct VS_RootGeometryOwnerChildNode {
    VS_RootGeometryOwner *m_pOwner;
    VS_RootGeometryOwnerChildNode *m_pNext;
    VS_RootGeometryOwnerChildNode *m_pPrevious;
};

struct GameGeometryDispatchHelperDeleteInterface {
    virtual void Delete(int nDeleteFlag) = 0;
};

struct GameGeometryOwnerRectDispatch {
    virtual void *Delete(int nDeleteFlag) = 0;
    virtual void DispatchButtonTransition(const VS_XYPair *pPoint,
                                          int nButton) = 0;
    virtual void DispatchButtonInside(const VS_XYPair *pPoint,
                                      int nButton) = 0;
    virtual void DispatchButtonOutside(const VS_XYPair *pPoint,
                                       int nButton) = 0;
    virtual void DispatchPointerEnter(void) = 0;
    virtual void DispatchPointerLeave(void) = 0;
    virtual void DispatchPointerMove(const VS_XYPair *pPoint) = 0;
    virtual int ContainsPoint(const VS_XYPair *pPoint) = 0;
};

struct GameGeometryOwnerRectState {
    void **m_pVtable00;
    int m_fEnabled04;
    int m_fPointerActive08;
    GameGeometryOwnerRectState *m_pParent0C;
    int m_anTransient10[6];
    int m_fDispatchPointerMove28;
    int m_fDispatchOutsideRelease2C;
    VS_XYPair m_Size30;
    VS_XYPair m_Position34;
};

struct GameRenderDispatchClientState {
    void **m_pVtable00;
    int m_nMagic04;
    void *m_pReserved08;
    int m_cDispatches0C;
};

struct GameGeometryOwnerChildNode {
    GameGeometryOwnerRectState *m_pOwner;
    GameGeometryOwnerChildNode *m_pPrevious;
    GameGeometryOwnerChildNode *m_pNext;
};

struct VS_RootGeometryDispatchHelperState {
    GameGeometryOwnerRectState m_Owner00;
    GameRenderDispatchClientState m_DispatchClient38;
    GameGeometryOwnerChildNode *m_pChildren48;
    GameGeometryOwnerChildNode *m_pChildrenTail4C;
    int m_nScaleFactor50;
    VS_XYPair m_Origin54;
    VS_XYPair m_SourcePosition58;
    GameGeometryOwnerRectState *m_pActiveOwner5C;
};

struct GameGeometryDispatchEvent {
    unsigned short m_nType00;
    unsigned short m_nReserved02;
    unsigned int m_dwTime04;
    unsigned int m_nPackedPosition08;
    int m_nCommand0C;
    int m_fHandled10;
};

typedef char GameGeometryOwnerRectState_size_must_be_0x38[
    sizeof(GameGeometryOwnerRectState) == 0x38 ? 1 : -1];
typedef char VS_RootGeometryDispatchHelperState_size_must_be_0x60[
    sizeof(VS_RootGeometryDispatchHelperState) == 0x60 ? 1 : -1];

void *LEMBALL_FASTCALL DeleteGeometryOwnerRect(
    void *pOwner, int nUnused, int nDeleteFlag);
void LEMBALL_FASTCALL GeometryOwnerButtonTransitionNoop(
    void *pOwner, int nUnused, const VS_XYPair *pPoint, int nButton);
void LEMBALL_FASTCALL GeometryOwnerButtonInsideNoop(
    void *pOwner, int nUnused, const VS_XYPair *pPoint, int nButton);
void LEMBALL_FASTCALL ClearGeometryOwnerButtonState(
    void *pOwner, int nUnused, const VS_XYPair *pPoint, int nButton);
void LEMBALL_FASTCALL GeometryOwnerPointerEnterNoop(void *pOwner);
void LEMBALL_FASTCALL ResetGeometryOwnerButtonsIfOutsideDispatchDisabled(
    void *pOwner);
void LEMBALL_FASTCALL GeometryOwnerPointerMoveNoop(
    void *pOwner, int nUnused, const VS_XYPair *pPoint);
int LEMBALL_FASTCALL GeometryOwnerContainsPoint(
    void *pOwner, int nUnused, const VS_XYPair *pPoint);
void LEMBALL_FASTCALL ResetRootGeometryOwnerButtons(void *pOwner);
void *LEMBALL_FASTCALL DeleteRootGeometryDispatchHelper(
    void *pHelper, int nUnused, int nDeleteFlag);
void *LEMBALL_FASTCALL DeleteRootGeometryDispatchHelperAdjusted(
    void *pDispatchClient, int nUnused, int nDeleteFlag);
int LEMBALL_FASTCALL HandleRootGeometryDispatchEntry(
    void *pDispatchClient, int nUnused, GameGeometryDispatchEvent *pEvent);

// VTABLE: LEMBALL 0x00496EA0
static void *g_GAME_GeometryOwnerRectVtable[8] = {
    (void *)DeleteGeometryOwnerRect,
    (void *)GeometryOwnerButtonTransitionNoop,
    (void *)GeometryOwnerButtonInsideNoop,
    (void *)ClearGeometryOwnerButtonState,
    (void *)GeometryOwnerPointerEnterNoop,
    (void *)ResetGeometryOwnerButtonsIfOutsideDispatchDisabled,
    (void *)GeometryOwnerPointerMoveNoop,
    (void *)GeometryOwnerContainsPoint,
};

// VTABLE: LEMBALL 0x00499D18
static void *g_VS_RootGeometryDispatchHelperVtable[8] = {
    (void *)DeleteRootGeometryDispatchHelper,
    (void *)GeometryOwnerButtonTransitionNoop,
    (void *)GeometryOwnerButtonInsideNoop,
    (void *)ClearGeometryOwnerButtonState,
    (void *)GeometryOwnerPointerEnterNoop,
    (void *)ResetRootGeometryOwnerButtons,
    (void *)GeometryOwnerPointerMoveNoop,
    (void *)GeometryOwnerContainsPoint,
};

// VTABLE: LEMBALL 0x00499D08
static void *g_VS_RootGeometryDispatchClientVtable[3] = {
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteRootGeometryDispatchHelperAdjusted,
    (void *)HandleRootGeometryDispatchEntry,
};

// GLOBAL: LEMBALL 0x004A1FF8
static VS_XYPair *g_pCurrentGeometryDispatchPoint = 0;
// GLOBAL: LEMBALL 0x004A1FFC
static int g_cRootGeometryDispatchHelpers = 0;

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

// FUNCTION: LEMBALL 0x00439A40
void *LEMBALL_FASTCALL DeleteGeometryOwnerRect(
    void *pOwner, int, int nDeleteFlag) {
    *(void ***)pOwner = g_GAME_GeometryOwnerRectVtable;
    if ((nDeleteFlag & 1) != 0) {
        FreeVSMemBlock(pOwner);
    }
    return pOwner;
}

// FUNCTION: LEMBALL 0x00439960
void LEMBALL_FASTCALL GeometryOwnerButtonTransitionNoop(
    void *, int, const VS_XYPair *, int) {
}

// FUNCTION: LEMBALL 0x00439970
void LEMBALL_FASTCALL GeometryOwnerButtonInsideNoop(
    void *, int, const VS_XYPair *, int) {
}

// FUNCTION: LEMBALL 0x00439980
void LEMBALL_FASTCALL ClearGeometryOwnerButtonState(
    void *pOwner, int, const VS_XYPair *, int nButton) {
    *(int *)((char *)pOwner + 0x1c + nButton * 4) = 0;
    *(int *)((char *)pOwner + 0x10 + nButton * 4) = 0;
}

// FUNCTION: LEMBALL 0x004399A0
void LEMBALL_FASTCALL GeometryOwnerPointerEnterNoop(void *) {
}

// FUNCTION: LEMBALL 0x004399B0
void LEMBALL_FASTCALL ResetGeometryOwnerButtonsIfOutsideDispatchDisabled(
    void *pOwner) {
    int i;

    if (*(int *)((char *)pOwner + 0x2c) == 0) {
        for (i = 0; i < 6; ++i) {
            *(int *)((char *)pOwner + 0x10 + i * 4) = 0;
        }
    }
}

// FUNCTION: LEMBALL 0x004399D0
void LEMBALL_FASTCALL GeometryOwnerPointerMoveNoop(
    void *, int, const VS_XYPair *) {
}

// FUNCTION: LEMBALL 0x004399E0
int LEMBALL_FASTCALL GeometryOwnerContainsPoint(
    void *pOwner, int, const VS_XYPair *pPoint) {
    GameGeometryOwnerRectState *pState;

    pState = (GameGeometryOwnerRectState *)pOwner;
    return pState->m_Position34.m_x <= pPoint->m_x &&
           pPoint->m_x <
               (short)(pState->m_Position34.m_x + pState->m_Size30.m_x) &&
           pState->m_Position34.m_y <= pPoint->m_y &&
           pPoint->m_y <
               (short)(pState->m_Position34.m_y + pState->m_Size30.m_y);
}

// FUNCTION: LEMBALL 0x0046AA30
void LEMBALL_FASTCALL ResetRootGeometryOwnerButtons(void *pOwner) {
    int i;

    if (*(int *)((char *)pOwner + 0x2c) == 0) {
        for (i = 0; i < 6; ++i) {
            *(int *)((char *)pOwner + 0x10 + i * 4) = 0;
        }
    }
}

// FUNCTION: LEMBALL 0x0046A350
void LEMBALL_FASTCALL ClearGeometryOwnerRectTransientState(void *pOwner) {
    GameGeometryOwnerRectState *pState;
    int i;

    pState = (GameGeometryOwnerRectState *)pOwner;
    if (pState->m_fPointerActive08 != 0) {
        pState->m_fPointerActive08 = 0;
        ((GameGeometryOwnerRectDispatch *)pOwner)->DispatchPointerLeave();
    }
    for (i = 0; i < 6; ++i) {
        pState->m_anTransient10[i] = 0;
    }
}

// FUNCTION: LEMBALL 0x0046A330
void LEMBALL_FASTCALL ResetGeometryOwnerRectState(void *pOwner) {
    GameGeometryOwnerRectState *pState;

    pState = (GameGeometryOwnerRectState *)pOwner;
    pState->m_fEnabled04 = 0;
    pState->m_fDispatchPointerMove28 = 0;
    pState->m_fDispatchOutsideRelease2C = 0;
    pState->m_pParent0C = 0;
    ClearGeometryOwnerRectTransientState(pOwner);
}

void LEMBALL_FASTCALL RedispatchGeometryOwnerPointerEvent(void *pOwner);

// FUNCTION: LEMBALL 0x0046A530
void LEMBALL_FASTCALL SetGeometryOwnerEnabled(
    void *pOwner, int, int fEnabled) {
    GameGeometryOwnerRectState *pState;

    pState = (GameGeometryOwnerRectState *)pOwner;
    pState->m_fEnabled04 = fEnabled;
    if (fEnabled == 0) {
        ClearGeometryOwnerRectTransientState(pOwner);
    } else if (pState->m_pParent0C != 0) {
        RedispatchGeometryOwnerPointerEvent(pState->m_pParent0C);
    }
}

// FUNCTION: LEMBALL 0x0046A290
void *LEMBALL_FASTCALL InitializeGeometryOwnerRect(
    void *pOwner, int, const VS_XYPair *pRect) {
    GameGeometryOwnerRectState *pState;

    pState = (GameGeometryOwnerRectState *)pOwner;
    pState->m_Size30.m_x = 0;
    pState->m_Size30.m_y = 0;
    pState->m_Position34.m_x = 0;
    pState->m_Position34.m_y = 0;
    pState->m_pVtable00 = g_GAME_GeometryOwnerRectVtable;
    ResetGeometryOwnerRectState(pState);
    pState->m_Size30 = pRect[0];
    pState->m_Position34 = pRect[1];
    SetGeometryOwnerEnabled(pState, 0, 1);
    return pState;
}

// FUNCTION: LEMBALL 0x0046A6E0
void LEMBALL_FASTCALL UnlinkAndFreeGeometryOwnerChildNode(
    void *pOwner, int, GameGeometryOwnerChildNode *pNode) {
    VS_RootGeometryDispatchHelperState *pHelper;

    pHelper = (VS_RootGeometryDispatchHelperState *)pOwner;
    if (pNode->m_pPrevious != 0) {
        pNode->m_pPrevious->m_pNext = pNode->m_pNext;
    } else {
        pHelper->m_pChildren48 = pNode->m_pNext;
    }
    if (pNode->m_pNext != 0) {
        pNode->m_pNext->m_pPrevious = pNode->m_pPrevious;
    } else {
        pHelper->m_pChildrenTail4C = pNode->m_pPrevious;
    }
    FreeVSMemBlock(pNode);
}

// FUNCTION: LEMBALL 0x0046A580
void *LEMBALL_FASTCALL ConstructRootGeometryDispatchHelper(
    void *pObject, int, const VS_XYPair *pRect,
    const VS_XYPair *pOrigin, const VS_XYPair *pSourcePosition) {
    VS_RootGeometryDispatchHelperState *pHelper;

    pHelper = (VS_RootGeometryDispatchHelperState *)pObject;
    InitializeGeometryOwnerRect(&pHelper->m_Owner00, 0, pRect);
    InitializeRenderQueueNodeBase(&pHelper->m_DispatchClient38);
    pHelper->m_Owner00.m_pVtable00 =
        g_VS_RootGeometryDispatchHelperVtable;
    pHelper->m_DispatchClient38.m_pVtable00 =
        g_VS_RootGeometryDispatchClientVtable;
    pHelper->m_Origin54.m_x = 0;
    pHelper->m_Origin54.m_y = 0;
    pHelper->m_SourcePosition58.m_x = 0;
    pHelper->m_SourcePosition58.m_y = 0;

    if (g_cRootGeometryDispatchHelpers++ == 0) {
        g_pCurrentGeometryDispatchPoint =
            (VS_XYPair *)AllocateVSMemBlock(sizeof(VS_XYPair));
        if (g_pCurrentGeometryDispatchPoint != 0) {
            g_pCurrentGeometryDispatchPoint->m_x = 0;
            g_pCurrentGeometryDispatchPoint->m_y = 0;
        }
    }

    pHelper->m_Origin54 = *pOrigin;
    pHelper->m_SourcePosition58 = *pSourcePosition;
    ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(
            &pHelper->m_DispatchClient38, -0x19);
    pHelper->m_pChildren48 = 0;
    pHelper->m_pChildrenTail4C = 0;
    pHelper->m_nScaleFactor50 = 1;
    pHelper->m_pActiveOwner5C = 0;
    return pHelper;
}

// FUNCTION: LEMBALL 0x0046A650
void LEMBALL_FASTCALL DestroyRootGeometryDispatchHelper(void *pObject) {
    VS_RootGeometryDispatchHelperState *pHelper;
    GameGeometryOwnerChildNode *pNode;
    GameGeometryOwnerChildNode *pNext;

    pHelper = (VS_RootGeometryDispatchHelperState *)pObject;
    pHelper->m_Owner00.m_pVtable00 =
        g_VS_RootGeometryDispatchHelperVtable;
    pHelper->m_DispatchClient38.m_pVtable00 =
        g_VS_RootGeometryDispatchClientVtable;
    pNode = pHelper->m_pChildren48;
    while (pNode != 0) {
        pNext = pNode->m_pNext;
        UnlinkAndFreeGeometryOwnerChildNode(pHelper, 0, pNode);
        pNode = pNext;
    }
    ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->UnregisterOrderedRenderDispatchClient(
            &pHelper->m_DispatchClient38, -0x19);
    --g_cRootGeometryDispatchHelpers;
    if (g_cRootGeometryDispatchHelpers == 0) {
        FreeVSMemBlock(g_pCurrentGeometryDispatchPoint);
    }
    pHelper->m_Owner00.m_pVtable00 = g_GAME_GeometryOwnerRectVtable;
}

// FUNCTION: LEMBALL 0x0046AA50
void *LEMBALL_FASTCALL DeleteRootGeometryDispatchHelper(
    void *pHelper, int, int nDeleteFlag) {
    DestroyRootGeometryDispatchHelper(pHelper);
    if ((nDeleteFlag & 1) != 0) {
        FreeVSMemBlock(pHelper);
    }
    return pHelper;
}

// FUNCTION: LEMBALL 0x0046AA70
void *LEMBALL_FASTCALL DeleteRootGeometryDispatchHelperAdjusted(
    void *pDispatchClient, int, int nDeleteFlag) {
    return DeleteRootGeometryDispatchHelper(
        (char *)pDispatchClient - 0x38, 0, nDeleteFlag);
}

// FUNCTION: LEMBALL 0x0046A380
void LEMBALL_FASTCALL DispatchGeometryChildEvent(
    void *pOwner, int, GameGeometryDispatchEvent *pEvent,
    const VS_XYPair *pPoint,
    GameGeometryOwnerRectState *pPreviousActiveOwner) {
    GameGeometryOwnerRectState *pState;
    GameGeometryOwnerRectDispatch *pDispatch;
    int nButton;

    pState = (GameGeometryOwnerRectState *)pOwner;
    pDispatch = (GameGeometryOwnerRectDispatch *)pOwner;
    switch (pEvent->m_nType00) {
    case 8:
    case 9:
        if ((*(unsigned char *)((char *)g_pSharedGeometryHelper + 0x14) &
             1) != 0) {
            return;
        }
        /* fall through */
    case 5:
    case 6:
        switch (pEvent->m_nCommand0C) {
        case 0x43: nButton = 0; break;
        case 0x44: nButton = 1; break;
        case 0x45: nButton = 2; break;
        case 0x46: nButton = 3; break;
        case 0x47: nButton = 4; break;
        default: nButton = 5; break;
        }
        if (pEvent->m_nType00 == 6 || pEvent->m_nType00 == 8) {
            *(int *)((char *)pState + 0x10 + nButton * 4) = 1;
        } else {
            *(int *)((char *)pState + 0x1c + nButton * 4) = 0;
            *(int *)((char *)pState + 0x10 + nButton * 4) = 0;
        }
        if (pEvent->m_nType00 != 5 && pEvent->m_nType00 != 9) {
            pDispatch->DispatchButtonTransition(pPoint, nButton);
            return;
        }
        if (pState->m_Position34.m_x <= pPoint->m_x &&
            pPoint->m_x <
                (short)(pState->m_Position34.m_x + pState->m_Size30.m_x) &&
            pState->m_Position34.m_y <= pPoint->m_y &&
            pPoint->m_y <
                (short)(pState->m_Position34.m_y + pState->m_Size30.m_y)) {
            pDispatch->DispatchButtonInside(pPoint, nButton);
        } else {
            pDispatch->DispatchButtonOutside(pPoint, nButton);
        }
        return;

    case 10:
        if ((*(unsigned char *)((char *)g_pSharedGeometryHelper + 0x14) &
             1) != 0) {
            return;
        }
        /* fall through */
    case 7:
        if (pState->m_fDispatchPointerMove28 != 0) {
            pDispatch->DispatchPointerMove(pPoint);
        }
        if (pState->m_fPointerActive08 != 0 &&
            pState == pPreviousActiveOwner) {
            return;
        }
        pState->m_fPointerActive08 = 1;
        pDispatch->DispatchPointerEnter();
        if (pPreviousActiveOwner != 0 &&
            pPreviousActiveOwner->m_fPointerActive08 != 0) {
            pPreviousActiveOwner->m_fPointerActive08 = 0;
            ((GameGeometryOwnerRectDispatch *)pPreviousActiveOwner)
                ->DispatchPointerLeave();
        }
        return;
    }
}

// FUNCTION: LEMBALL 0x0046A770
void LEMBALL_FASTCALL DispatchNormalizedPointToGeometryChildren(
    void *pOwner, int, const VS_XYPair *pPoint,
    GameGeometryDispatchEvent *pEvent) {
    VS_RootGeometryDispatchHelperState *pHelper;
    GameGeometryOwnerChildNode *pNode;
    GameGeometryOwnerRectState *pChild;
    GameGeometryDispatchEvent DefaultEvent;
    VS_XYPair NormalizedPoint;
    VS_XYPair ScaledSize;
    VS_XYPair ScaledPosition;
    int nEventType;
    int fInside;

    pHelper = (VS_RootGeometryDispatchHelperState *)pOwner;
    if (pEvent == 0) {
        DefaultEvent.m_nType00 = 7;
        DefaultEvent.m_nReserved02 = 0;
        DefaultEvent.m_dwTime04 = 0;
        DefaultEvent.m_nPackedPosition08 = 0;
        DefaultEvent.m_nCommand0C = 0;
        DefaultEvent.m_fHandled10 = 0;
        pEvent = &DefaultEvent;
    }
    nEventType = pEvent->m_nType00;
    NormalizedPoint.m_x = (short)(
        (pPoint->m_x - pHelper->m_Owner00.m_Position34.m_x) /
        pHelper->m_nScaleFactor50);
    NormalizedPoint.m_y = (short)(
        (pPoint->m_y - pHelper->m_Owner00.m_Position34.m_y) /
        pHelper->m_nScaleFactor50);

    pNode = pHelper->m_pChildrenTail4C;
    while (pNode != 0) {
        pChild = pNode->m_pOwner;
        pNode = pNode->m_pPrevious;
        if (pChild->m_fEnabled04 != 0 &&
            !((GameGeometryOwnerRectDispatch *)pChild)
                 ->ContainsPoint(&NormalizedPoint)) {
            if (pChild->m_fPointerActive08 != 0) {
                pChild->m_fPointerActive08 = 0;
                ((GameGeometryOwnerRectDispatch *)pChild)
                    ->DispatchPointerLeave();
            }
            if ((nEventType == 5 || nEventType == 9) &&
                pChild->m_fDispatchOutsideRelease2C != 0) {
                DispatchGeometryChildEvent(
                    pChild, 0, pEvent, &NormalizedPoint,
                    pHelper->m_pActiveOwner5C);
            }
        }
    }

    ScaledSize.m_x = (short)(pHelper->m_Owner00.m_Size30.m_x *
                              pHelper->m_nScaleFactor50);
    ScaledSize.m_y = (short)(pHelper->m_Owner00.m_Size30.m_y *
                              pHelper->m_nScaleFactor50);
    ScaledPosition.m_x = (short)(
        pHelper->m_Owner00.m_Position34.m_x +
        pHelper->m_Origin54.m_x * (pHelper->m_nScaleFactor50 - 1));
    ScaledPosition.m_y = (short)(
        pHelper->m_Owner00.m_Position34.m_y +
        pHelper->m_Origin54.m_y * (pHelper->m_nScaleFactor50 - 1));
    fInside = ScaledPosition.m_x <= pPoint->m_x &&
              pPoint->m_x <
                  (short)(ScaledPosition.m_x + ScaledSize.m_x) &&
              ScaledPosition.m_y <= pPoint->m_y &&
              pPoint->m_y <
                  (short)(ScaledPosition.m_y + ScaledSize.m_y);
    if (!fInside) {
        if (pHelper->m_Owner00.m_fPointerActive08 != 0) {
            pHelper->m_Owner00.m_fPointerActive08 = 0;
            ((GameGeometryOwnerRectDispatch *)&pHelper->m_Owner00)
                ->DispatchPointerLeave();
        }
        if ((nEventType == 5 || nEventType == 9) &&
            pHelper->m_Owner00.m_fDispatchOutsideRelease2C != 0) {
            DispatchGeometryChildEvent(
                &pHelper->m_Owner00, 0, pEvent, &NormalizedPoint,
                pHelper->m_pActiveOwner5C);
            pHelper->m_pActiveOwner5C = &pHelper->m_Owner00;
        }
        return;
    }

    pHelper->m_Owner00.m_fPointerActive08 = 1;
    pNode = pHelper->m_pChildrenTail4C;
    while (pNode != 0) {
        pChild = pNode->m_pOwner;
        if (pChild->m_fEnabled04 != 0 &&
            ((GameGeometryOwnerRectDispatch *)pChild)
                ->ContainsPoint(&NormalizedPoint)) {
            DispatchGeometryChildEvent(
                pChild, 0, pEvent, &NormalizedPoint,
                pHelper->m_pActiveOwner5C);
            pHelper->m_pActiveOwner5C = pChild;
            return;
        }
        pNode = pNode->m_pPrevious;
    }
    DispatchGeometryChildEvent(
        &pHelper->m_Owner00, 0, pEvent, &NormalizedPoint,
        pHelper->m_pActiveOwner5C);
    pHelper->m_pActiveOwner5C = &pHelper->m_Owner00;
}

// FUNCTION: LEMBALL 0x0046A6D0
void LEMBALL_FASTCALL RedispatchGeometryOwnerPointerEvent(void *pOwner) {
    DispatchNormalizedPointToGeometryChildren(
        pOwner, 0, g_pCurrentGeometryDispatchPoint, 0);
}

// FUNCTION: LEMBALL 0x0046A710
int LEMBALL_FASTCALL HandleRootGeometryDispatchEntry(
    void *pDispatchClient, int, GameGeometryDispatchEvent *pEvent) {
    VS_XYPair Point;

    if (pEvent->m_nType00 >= 5 && pEvent->m_nType00 <= 10 &&
        pEvent->m_fHandled10 == 0) {
        Point.m_x = (short)(pEvent->m_nPackedPosition08 & 0xffff);
        Point.m_y = (short)(pEvent->m_nPackedPosition08 >> 16);
        *g_pCurrentGeometryDispatchPoint = Point;
        DispatchNormalizedPointToGeometryChildren(
            (char *)pDispatchClient - 0x38, 0, &Point, pEvent);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0046A560
void LEMBALL_FASTCALL SetGeometryOwnerParent(
    void *pOwner, int, void *pParent) {
    ((GameGeometryOwnerRectState *)pOwner)->m_pParent0C =
        (GameGeometryOwnerRectState *)pParent;
    RedispatchGeometryOwnerPointerEvent(pParent);
}

// FUNCTION: LEMBALL 0x0046A9A0
void LEMBALL_FASTCALL AppendGeometryOwnerChild(
    void *pOwner, int, void *pChildOwner) {
    VS_RootGeometryDispatchHelperState *pHelper;
    GameGeometryOwnerChildNode *pNode;

    pHelper = (VS_RootGeometryDispatchHelperState *)pOwner;
    pNode = (GameGeometryOwnerChildNode *)
        AllocateVSMemBlock(sizeof(GameGeometryOwnerChildNode));
    pNode->m_pOwner = (GameGeometryOwnerRectState *)pChildOwner;
    pNode->m_pPrevious = 0;
    pNode->m_pNext = 0;
    if (pHelper->m_pChildren48 == 0) {
        pHelper->m_pChildren48 = pNode;
    } else {
        pHelper->m_pChildrenTail4C->m_pNext = pNode;
        pNode->m_pPrevious = pHelper->m_pChildrenTail4C;
    }
    pHelper->m_pChildrenTail4C = pNode;
    SetGeometryOwnerParent(pChildOwner, 0, pHelper);
}

// FUNCTION: LEMBALL 0x0046AA00
void LEMBALL_FASTCALL RemoveGeometryOwnerChild(
    void *pOwner, int, void *pChildOwner) {
    VS_RootGeometryDispatchHelperState *pHelper;
    GameGeometryOwnerChildNode *pNode;

    pHelper = (VS_RootGeometryDispatchHelperState *)pOwner;
    pNode = pHelper->m_pChildren48;
    while (pNode != 0 && pNode->m_pOwner != pChildOwner) {
        pNode = pNode->m_pNext;
    }
    if (pNode == 0) {
        return;
    }
    if (pHelper->m_pActiveOwner5C == pChildOwner) {
        pHelper->m_pActiveOwner5C = 0;
    }
    UnlinkAndFreeGeometryOwnerChildNode(pHelper, 0, pNode);
}

// FUNCTION: LEMBALL 0x00465F80
void VS_RootGeometryOwner::RetainRootGeometryOwner(void) {
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

struct VS_RootGeometryDispatchHelperView {
    char m_abReserved00[0x30];
    VS_XYPair m_Size30;
    VS_XYPair m_Position34;
    char m_abReserved38[0x1c];
    VS_XYPair m_Origin54;
    VS_XYPair m_SourcePosition58;

    void StoreRootGeometrySubrectPayload(const VS_XYPair *pRect,
                                         VS_XYPair origin,
                                         const VS_XYPair *pSourcePosition);
};

// FUNCTION: LEMBALL 0x00465A50
VS_XYPair &VS_XYPair::operator=(const VS_XYPair &other) {
    m_x = other.m_x;
    m_y = other.m_y;
    return *this;
}

// FUNCTION: LEMBALL 0x004662E0
void VS_RootGeometryOwner::RefreshWindowOwnerMenuState(void) {
}

// FUNCTION: LEMBALL 0x00465DB0
void VS_RootGeometryOwner::StoreRootGeometrySourceRect(const VS_XYPair *pRect) {
    m_SourceSize10.m_x = pRect[0].m_x;
    m_SourceSize10.m_y = pRect[0].m_y;
    m_SourcePosition14.m_x = pRect[1].m_x;
    m_SourcePosition14.m_y = pRect[1].m_y;
    CanonicalizeRootGeometrySubrect();
}

// FUNCTION: LEMBALL 0x004662F0
void VS_RootGeometryOwner::SetWindowOwnerMenuDefinition(const unsigned int *, void *) {
}

// FUNCTION: LEMBALL 0x00465E60
void VS_RootGeometryOwner::EnsureRootGeometryDispatchHelper(void) {
    VS_XYPair aRect[2];
    VS_XYPair Origin;
    void *pHelper;

    if ((GetWindowOwnerFlags() & 0x800) == 0 ||
        m_pGeometryDispatchHelper1C != 0) {
        return;
    }

    aRect[0] = m_SourceSize10;
    if ((int)aRect[0].m_x * (int)aRect[0].m_y == 0) {
        aRect[0] = m_Size08;
        aRect[1] = m_Position0C;
    } else {
        aRect[1].m_x =
            (short)(m_SourcePosition14.m_x + m_Position0C.m_x);
        aRect[1].m_y =
            (short)(m_SourcePosition14.m_y + m_Position0C.m_y);
    }
    Origin = m_Origin18;
    if (m_pParent20 == 0) {
        Origin.m_x = 0;
        Origin.m_y = 0;
    }

    pHelper = AllocateVSMemBlock(0x60);
    if (pHelper != 0) {
        m_pGeometryDispatchHelper1C =
            ConstructRootGeometryDispatchHelper(
                pHelper, 0, aRect, &Origin, &m_SourcePosition14);
    } else {
        m_pGeometryDispatchHelper1C = 0;
    }
}

// FUNCTION: LEMBALL 0x00465FE0
void VS_RootGeometryOwner::ReleaseRootGeometryOwner(void) {
    WINDOW_OWNER_REGISTRY_NODE *pNode;
    WINDOW_OWNER_REGISTRY_NODE *pNext;
    WINDOW_OWNER_REGISTRY_NODE *pPrevious;
    WINDOW_OWNER_REGISTRY_NODE *pFirst;

    --g_nLiveRootZrleGeometryOwnerCount;
    --m_cRetains04;
    if (m_pGeometryDispatchHelper1C != 0) {
        ((GameGeometryDispatchHelperDeleteInterface *)
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
void VS_RootGeometryDispatchHelperView::StoreRootGeometrySubrectPayload(const VS_XYPair *pRect, VS_XYPair origin, const VS_XYPair *pSourcePosition) {
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
void VS_RootGeometryOwner::CanonicalizeRootGeometrySubrect(void) {
    VS_XYPair aRect[2];
    VS_XYPair origin;
    VS_XYPair callOrigin;

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
        ((VS_RootGeometryDispatchHelperView *)m_pGeometryDispatchHelper1C)
            ->StoreRootGeometrySubrectPayload(
                aRect, callOrigin, &m_SourcePosition14);
    }
}

// FUNCTION: LEMBALL 0x00466260
void VS_RootGeometryOwner::OffsetRootGeometryPosition(unsigned int uPackedDelta) {
    m_Position0C.m_x =
        (short)(m_Position0C.m_x + (short)(uPackedDelta & 0xffff));
    m_Position0C.m_y =
        (short)(m_Position0C.m_y + (short)(uPackedDelta >> 16));
    CanonicalizeRootGeometrySubrectAlt();
}

// FUNCTION: LEMBALL 0x00466160
void VS_RootGeometryOwner::CanonicalizeRootGeometrySubrectAlt(void) {
    VS_XYPair aRect[2];
    VS_XYPair origin;
    VS_XYPair callOrigin;

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
        ((VS_RootGeometryDispatchHelperView *)m_pGeometryDispatchHelper1C)
            ->StoreRootGeometrySubrectPayload(
                aRect, callOrigin, &m_SourcePosition14);
    }
}

// FUNCTION: LEMBALL 0x00466280
void VS_RootGeometryOwner::SetRootGeometryScaleFactor(int) {
    VS_RootGeometryOwnerChildNode *pNode;

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
void VS_RootGeometryOwner::SetRootGeometryRect(const VS_XYPair *pRect) {
    m_Size08.m_x = pRect[0].m_x;
    m_Size08.m_y = pRect[0].m_y;
    m_Position0C.m_x = pRect[1].m_x;
    m_Position0C.m_y = pRect[1].m_y;
}

// FUNCTION: LEMBALL 0x00466330
void VS_RootGeometryOwner::FinalizeRootGeometryRect(const VS_XYPair *) {
}

// FUNCTION: LEMBALL 0x00466340
void VS_RootGeometryOwner::ReservedWindowOwnerSlot23(void) {
}

// FUNCTION: LEMBALL 0x00466350
int VS_RootGeometryOwner::IsCurrentWindowOwnerContext(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x00465A70
void VS_RootGeometryOwner::ReservedWindowOwnerSlot32(void) {
}

// FUNCTION: LEMBALL 0x00465A80
void VS_RootGeometryOwner::ReservedWindowOwnerSlot33(int) {
}

// FUNCTION: LEMBALL 0x00466360
void VS_RootGeometryOwner::ReservedWindowOwnerSlot34(int) {
}

struct GameWindowOwnerMenuEntry {
    const char *m_pszText00;
    unsigned int m_nCommandId04;
    int m_nPosition08;
    int m_fEnabled0C;
    int m_fChecked10;
    int m_nReserved14;
};

// FUNCTION: LEMBALL 0x00465660
void VS_WindowOwnerBase::RefreshWindowOwnerMenuState(void) {
    GameWindowOwnerMenuEntry **ppMenu;
    GameWindowOwnerMenuEntry *pEntry;
    HMENU hMenu;

    hMenu = GetMenu(m_hWindow44);
    ppMenu = (GameWindowOwnerMenuEntry **)m_pMenuDefinition3C;
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
void VS_WindowOwnerBase::SetWindowOwnerMenuDefinition(const unsigned int *pnMenuId, void *pMenuDefinition) {
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
void VS_WindowOwnerBase::SetRootGeometryScaleFactor(int nScaleFactor) {
    WINDOW_OWNER_RECT WindowRect;
    WINDOW_OWNER_RECT ClientRect;

    VS_RootGeometryOwner::SetRootGeometryScaleFactor(nScaleFactor);
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
void VS_WindowOwnerBase::SetRootGeometryRect(const VS_XYPair *pRect) {
    VS_XYPair size;
    VS_XYPair position;
    VS_XYPair *pParentPosition;
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

    pParentPosition = (VS_XYPair *)((char *)m_pParent20 + 0x0c);
    ScreenPosition.x = pParentPosition->m_x + position.m_x;
    ScreenPosition.y = pParentPosition->m_y + position.m_y;
    ClientToScreen(((VS_WindowOwnerBase *)m_pParent20)->m_hWindow44,
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
void VS_WindowOwnerBase::FinalizeRootGeometryRect(const VS_XYPair *pPoint) {
    VS_XYPair aRect[2];

    aRect[0].m_x = m_Size08.m_x;
    aRect[0].m_y = m_Size08.m_y;
    aRect[1].m_x = pPoint->m_x;
    aRect[1].m_y = pPoint->m_y;
    SetRootGeometryRect(aRect);
}

// FUNCTION: LEMBALL 0x00464FA0
void VS_WindowOwnerBase::MoveWindowOwnerOrigin(const VS_XYPair *pPoint) {
    VS_RootGeometryOwnerChildNode *pNode;
    VS_XYPair delta;

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
void VS_WindowOwnerBase::ReservedWindowOwnerSlot15(void) {
}

// FUNCTION: LEMBALL 0x00464500
void VS_WindowOwnerBase::ReservedWindowOwnerSlot16(void) {
}

// FUNCTION: LEMBALL 0x00464510
void VS_WindowOwnerBase::ReservedWindowOwnerSlot17(void) {
}

// FUNCTION: LEMBALL 0x00465A90
unsigned int VS_WindowOwnerBase::GetWindowOwnerFlags(void) {
    return 0;
}

// FUNCTION: LEMBALL 0x004655F0
void VS_WindowOwnerBase::InvalidateWindowOwnerRect(const VS_XYPair *pExtent) {
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
void VS_WindowOwnerBase::DestroyWindowOwnerHwnd(void) {
    VS_RootGeometryOwnerChildNode *pNode;
    VS_RootGeometryOwnerChildNode *pNext;
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
        ((VsGdiSelectedGraphicsDriverRuntime *)
             g_pSelectedGraphicsDriverRuntime)
            ->m_nSelectedDriver;
    if ((nSelectedDriver < 4 || 5 < nSelectedDriver) &&
        m_hWindow44 != 0) {
        DestroyWindow(m_hWindow44);
    }
}

// FUNCTION: LEMBALL 0x00465A40
void VS_WindowOwnerBase::ReservedWindowOwnerSlot34(int) {
}

// FUNCTION: LEMBALL 0x00464F10
void VS_WindowOwnerBase::PropagateWindowOwnerPosition(const VS_XYPair *pPoint) {
    VS_RootGeometryOwnerChildNode *pNode;
    VS_XYPair delta;

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
LRESULT VS_WindowOwnerBase::DispatchWindowMessage(UINT uMessage, WPARAM wParam, LPARAM lParam) {
    return DefWindowProcA(m_hWindow44, uMessage, wParam, lParam);
}

struct WINDOW_OWNER_COMPACT_GEOMETRY_DISPATCH {
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
    virtual void SetRenderExtent(const VS_XYPair *pExtent) = 0;
    virtual void SetRenderOrigin(const VS_XYPair *pOrigin) = 0;
    virtual void Reserved12(void) = 0;
    virtual void Present(void) = 0;
};

// FUNCTION: LEMBALL 0x00465C70
void LEMBALL_FASTCALL UpdateWindowOwnerRootGeometrySourceRect(
    void *pOwner, int, const VS_XYPair *pRect) {
    ((VS_RootGeometryOwner *)pOwner)
        ->VS_RootGeometryOwner::StoreRootGeometrySourceRect(pRect);
}

// FUNCTION: LEMBALL 0x00465C80
void LEMBALL_FASTCALL RefreshWindowOwnerRenderScale(
    void *pOwner, int, int nPreviousScaleFactor) {
    void *pHelper;
    void *pGeometryContext;
    void *pCompactGeometry;

    ((VS_WindowOwnerBase *)pOwner)
        ->VS_WindowOwnerBase::SetRootGeometryScaleFactor(
            nPreviousScaleFactor);
    pHelper = *(void **)((char *)pOwner + 0x4c);
    if (pHelper != 0) {
        pGeometryContext = *(void **)((char *)pHelper + 0x0c);
        if (pGeometryContext != 0 &&
            *(void **)((char *)pOwner + 0x20) == 0) {
            pCompactGeometry =
                (char *)pGeometryContext + 0x40 +
                *(int *)((char *)*(void **)(
                             (char *)pGeometryContext + 0x40) +
                         4);
            *(short *)((char *)pCompactGeometry + 0x2c) =
                (short)*(int *)((char *)pOwner + 0x38);
        }
    }
}

// FUNCTION: LEMBALL 0x00463DF0
void LEMBALL_FASTCALL ReleaseWindowOwnerRenderResources(void *pOwner) {
    void *pHelper;
    short aRect[4];

    pHelper = *(void **)((char *)pOwner + 0x4c);
    if (pHelper != 0) {
        DestroyGeometryHelperPointerArray(pHelper);
        FreeVSMemBlock(pHelper);
        *(void **)((char *)pOwner + 0x4c) = 0;
    }

    if ((((VS_RootGeometryOwner *)pOwner)->GetWindowOwnerFlags() &
         0x40000000u) != 0 &&
        *(HWND *)((char *)pOwner + 0x44) != 0 &&
        ((unsigned int)GetWindowLongA(
             *(HWND *)((char *)pOwner + 0x44), -16) &
         0x40000000u) != 0) {
        aRect[0] = 0;
        aRect[1] = 0;
        aRect[2] = 0;
        aRect[3] = 0;
        ((VS_RootGeometryOwner *)*(void **)((char *)pOwner + 0x48))
            ->ReservedWindowOwnerSlot33((int)(unsigned long)aRect);
    }

    ((VS_RootGeometryOwner *)pOwner)
        ->VS_RootGeometryOwner::ReleaseRootGeometryOwner();
}

// FUNCTION: LEMBALL 0x00463E70
void LEMBALL_FASTCALL CanonicalizeWindowOwnerRenderSubrect(void *pOwner) {
    VS_XYPair Extent;
    VS_XYPair Origin;
    VS_RootGeometryOwner *pParent;
    void *pGeometryContext;
    WINDOW_OWNER_COMPACT_GEOMETRY_DISPATCH *pCompactGeometry;

    ((VS_RootGeometryOwner *)pOwner)
        ->VS_RootGeometryOwner::CanonicalizeRootGeometrySubrect();
    if (*(void **)((char *)pOwner + 0x4c) == 0) {
        return;
    }

    Extent = *(VS_XYPair *)((char *)pOwner + 8);
    if ((int)*(short *)((char *)pOwner + 0x10) *
            (int)*(short *)((char *)pOwner + 0x12) !=
        0) {
        Extent = *(VS_XYPair *)((char *)pOwner + 0x10);
    }
    pGeometryContext = *(void **)(
        (char *)*(void **)((char *)pOwner + 0x4c) + 0x0c);
    if (*(void **)((char *)pOwner + 0x20) == 0) {
        Extent.m_x = (short)(Extent.m_x /
                             *(int *)((char *)pOwner + 0x38));
        Extent.m_y = (short)(Extent.m_y /
                             *(int *)((char *)pOwner + 0x38));
    }
    pCompactGeometry = (WINDOW_OWNER_COMPACT_GEOMETRY_DISPATCH *)(
        (char *)pGeometryContext + 0x40 +
        *(int *)((char *)*(void **)((char *)pGeometryContext + 0x40) + 4));
    pCompactGeometry->SetRenderExtent(&Extent);

    Origin = *(VS_XYPair *)((char *)pOwner + 0x14);
    pParent = *(VS_RootGeometryOwner **)((char *)pOwner + 0x20);
    if (pParent != 0) {
        Origin.m_x = (short)(Origin.m_x -
                             *(short *)((char *)pParent + 0x18));
        Origin.m_y = (short)(Origin.m_y -
                             *(short *)((char *)pParent + 0x1a));
    }
    *(VS_XYPair *)((char *)pCompactGeometry + 0x1c) = Origin;
}

// FUNCTION: LEMBALL 0x00463F30
void LEMBALL_FASTCALL PopulateWindowOwnerRenderAnchor(void *pOwner) {
    void *pGeometryContext;

    ((VS_RootGeometryOwner *)pOwner)
        ->VS_RootGeometryOwner::CanonicalizeRootGeometrySubrectAlt();
    pGeometryContext = *(void **)(
        (char *)*(void **)((char *)pOwner + 0x4c) + 0x0c);
    ((WINDOW_OWNER_COMPACT_GEOMETRY_DISPATCH *)(
         (char *)pGeometryContext + 0x40 +
         *(int *)((char *)*(void **)((char *)pGeometryContext + 0x40) + 4)))
        ->SetRenderOrigin((VS_XYPair *)((char *)pOwner + 0x18));
}

// FUNCTION: LEMBALL 0x00463C20
void LEMBALL_FASTCALL SetWindowOwnerRenderAnchorFromPoint(
    void *pOwner, int, const VS_XYPair *pPoint) {
    ((VS_WindowOwnerBase *)pOwner)
        ->VS_WindowOwnerBase::MoveWindowOwnerOrigin(pPoint);
}

// FUNCTION: LEMBALL 0x00432560
void LEMBALL_FASTCALL ForwardNestedGeometryRectUpdate(
    void *pOwner, int, short *paRect) {
    void *pHelper;
    void *pGeometryContext;

    pHelper = *(void **)((char *)pOwner + 0x4c);
    if (pHelper != 0) {
        pGeometryContext = *(void **)((char *)pHelper + 0x0c);
        if (pGeometryContext != 0) {
            CopyRectTupleIfExtentEmpty(
                (char *)pGeometryContext + 0x40 +
                    *(int *)((char *)*(void **)(
                                 (char *)pGeometryContext + 0x40) +
                             4),
                0,
                paRect);
        }
    }
}

// FUNCTION: LEMBALL 0x00463F70
LRESULT LEMBALL_FASTCALL DispatchWindowOwnerRenderContextMessage(
    void *pOwner, int, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    WINDOW_OWNER_PAINTSTRUCT Paint;
    short cxPaint;
    short cyPaint;
    short xPaint;
    short yPaint;
    short *paDirty;
    int nSelectedDriver;

    paDirty = (short *)((char *)pOwner + 0x88);
    if (uMessage == 0x0f) {
        nSelectedDriver = *(int *)g_pSelectedGraphicsDriverRuntime;
        if (nSelectedDriver >= 4 && nSelectedDriver <= 5) {
            return DefWindowProcA(
                *(HWND *)((char *)GetDisplayState() + 0x10),
                uMessage, wParam, lParam);
        }

        BeginPaint(*(HWND *)((char *)pOwner + 0x44), &Paint);
        xPaint = (short)Paint.rcPaint.left;
        yPaint = (short)Paint.rcPaint.top;
        cxPaint = (short)(Paint.rcPaint.right - Paint.rcPaint.left);
        cyPaint = (short)(Paint.rcPaint.bottom - Paint.rcPaint.top);
        if ((int)cxPaint * (int)cyPaint != 0) {
            if (paDirty[2] > xPaint) {
                paDirty[0] = (short)(paDirty[0] + paDirty[2] - xPaint);
                paDirty[2] = xPaint;
            }
            if ((short)(paDirty[2] + paDirty[0]) <
                (short)(xPaint + cxPaint)) {
                paDirty[0] = (short)(xPaint + cxPaint - paDirty[2]);
            }
            if (paDirty[3] > yPaint) {
                paDirty[1] = (short)(paDirty[1] + paDirty[3] - yPaint);
                paDirty[3] = yPaint;
            }
            if ((short)(paDirty[3] + paDirty[1]) <
                (short)(yPaint + cyPaint)) {
                paDirty[1] = (short)(yPaint + cyPaint - paDirty[3]);
            }
        }
        EndPaint(*(HWND *)((char *)pOwner + 0x44), &Paint);
        return 0;
    }

    if (uMessage == 0x1c) {
        if (wParam != 0 && *(void **)((char *)pOwner + 0x4c) != 0) {
            PropagateGlobalPaletteToResourceGeometries(
                *(void **)((char *)*(void **)((char *)pOwner + 0x4c) +
                           0x0c));
        }
    } else if (uMessage == 0x311 &&
               (int)*(short *)((char *)pOwner + 8) *
                       (int)*(short *)((char *)pOwner + 10) !=
                   0) {
        if (paDirty[2] > 0) {
            paDirty[0] = (short)(paDirty[0] + paDirty[2]);
            paDirty[2] = 0;
        }
        if ((short)(paDirty[2] + paDirty[0]) <
            *(short *)((char *)pOwner + 8)) {
            paDirty[0] = (short)(*(short *)((char *)pOwner + 8) -
                                 paDirty[2]);
        }
        if (paDirty[3] > 0) {
            paDirty[1] = (short)(paDirty[1] + paDirty[3]);
            paDirty[3] = 0;
        }
        if ((short)(paDirty[3] + paDirty[1]) <
            *(short *)((char *)pOwner + 10)) {
            paDirty[1] = (short)(*(short *)((char *)pOwner + 10) -
                                 paDirty[3]);
        }
    }

    if (*(HWND *)((char *)pOwner + 0x44) != 0) {
        return DefWindowProcA(*(HWND *)((char *)pOwner + 0x44),
                              uMessage, wParam, lParam);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00464220
void LEMBALL_FASTCALL PresentWindowOwnerRenderContext(void *pOwner) {
    VS_RootGeometryOwnerChildNode *pNode;
    void *pGeometryContext;
    void *hDc;
    void (LEMBALL_FASTCALL *pPresentChild)(void *);

    if (*(int *)((char *)pOwner + 4) == 0) {
        return;
    }

    hDc = 0;
    if (*(HWND *)((char *)pOwner + 0x44) != 0) {
        hDc = GetDC(*(HWND *)((char *)pOwner + 0x44));
        EnterCriticalSection(
            (char *)g_pResourceGeometryHelperTarget + 0x534);
        BindHelperTargetDisplayDc(g_pResourceGeometryHelperTarget, 0, hDc);
    }

    pNode = *(VS_RootGeometryOwnerChildNode **)((char *)pOwner + 0x24);
    while (pNode != 0) {
        pPresentChild = (void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pNode->m_pOwner)[45];
        pPresentChild(pNode->m_pOwner);
        pNode = pNode->m_pNext;
    }

    pGeometryContext = *(void **)(
        (char *)*(void **)((char *)pOwner + 0x4c) + 0x0c);
    ((WINDOW_OWNER_COMPACT_GEOMETRY_DISPATCH *)(
         (char *)pGeometryContext + 0x40 +
         *(int *)((char *)*(void **)((char *)pGeometryContext + 0x40) + 4)))
        ->Present();

    if (*(HWND *)((char *)pOwner + 0x44) != 0) {
        ReleaseDC(*(HWND *)((char *)pOwner + 0x44), hDc);
        LeaveCriticalSection(
            (char *)g_pResourceGeometryHelperTarget + 0x534);
    }
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
    const int *pSelectedDriver;
    WINDOW_OWNER_REGISTRY_NODE *pNode;
    void *pOwner;
    HWND hOwnerWindow;
    unsigned int uOwnerFlags;
    int fDisplayChanged;
    void (LEMBALL_FASTCALL *pOwnerCallback)(void *);

    pSelectedDriver = pnSelectedDriver;

    if (g_pRootZrleGeometryOwnerRegistry == 0) {
        return;
    }

    pNode = *(WINDOW_OWNER_REGISTRY_NODE **)g_pRootZrleGeometryOwnerRegistry;
    while (pNode != 0) {
        pOwner = pNode->m_pOwner;
        hOwnerWindow = *(HWND *)((char *)pOwner + 0x44);
        if (hOwnerWindow != 0) {
            uOwnerFlags = ((WINDOW_OWNER_FLAGS_INTERFACE *)pOwner)->GetOwnerFlags();
            if ((uOwnerFlags & 0x80000000u) != 0) {
                fDisplayChanged = 1;
                if (*pSelectedDriver == 3) {
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

struct WindowOwnerCommandTable {
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
int WindowOwnerCommandTable::LookupWindowCommandEventPayload(void *, int nCommandId) {
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
    LRESULT (LEMBALL_FASTCALL *pOwnerResult)(
        void *, int, UINT, WPARAM, LPARAM);
    void (LEMBALL_FASTCALL *pOwnerCallback)(void *);
    int fPreviousFullscreenActivationState;

    if (g_fWindowMessagePassthroughMode != 0 &&
        (uMessage != 0x1c || wParam != 0)) {
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);
    }

    dwMessageTime = GetMessageTime();

    pOwner = (void *)(unsigned long)GetWindowLongA(hWnd, GWL_USERDATA);
    if (g_pDisplayState == 0) {
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);
    }

    if (*(HWND *)((char *)g_pDisplayState + 0x10) == hWnd) {
        pOwner = *(void **)((char *)g_pSelectedGraphicsDriverRuntime + 4);
        if (pOwner == 0) {
            return DefWindowProcA(hWnd, uMessage, wParam, lParam);
        }
    } else if (pOwner == 0 && uMessage != WM_CREATE) {
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);
    }

    switch (uMessage) {
    case WM_CREATE: {
        pCreateStruct = (CREATESTRUCTA *)(unsigned long)lParam;
        pOwner = pCreateStruct->lpCreateParams;
        SetWindowLongA(hWnd, GWL_USERDATA, (LONG)(unsigned long)pOwner);
        *(void **)((char *)pOwner + 0x44) = hWnd;
        Point.x = 0;
        Point.y = 0;
        ClientToScreen(hWnd, &Point);

        pGetOwnerFlags = (unsigned int (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x64 / sizeof(void *)];
        uOwnerFlags = pGetOwnerFlags(pOwner);
        if ((uOwnerFlags & 0x40000000u) != 0 &&
            IsDisplayModeWindowedRange(
                (const int *)g_pSelectedGraphicsDriverRuntime)) {
            Point.x += *(short *)((char *)*(void **)((char *)pOwner + 0x48) + 0x18);
            Point.y += *(short *)((char *)*(void **)((char *)pOwner + 0x48) + 0x1a);
        }
        *(short *)((char *)pOwner + 0x0c) = (short)Point.x;
        *(short *)((char *)pOwner + 0x0e) = (short)Point.y;
        *(short *)((char *)pOwner + 0x18) = *(short *)((char *)pOwner + 0x0c);
        *(short *)((char *)pOwner + 0x1a) = *(short *)((char *)pOwner + 0x0e);

        pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x18 / sizeof(void *)];
        pOwnerCallback(pOwner);
        pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x3c / sizeof(void *)];
        pOwnerCallback(pOwner);
        return 0;
    }

    case WM_QUIT:
        g_pStatusOutputStream->AppendCStringToStream(g_szQuittingStatusMessage);
        ReleaseCapture();
        return 0;

    case WM_DESTROY:
        if (hWnd == g_pCachedCurrentContextWindow) {
            g_pCachedCurrentContextWindow = 0;
            g_pCachedCurrentContextOwner = 0;
        }
        *(void **)((char *)pOwner + 0x44) = 0;
        pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x74 / sizeof(void *)];
        pOwnerCallback(pOwner);
        if (g_nLiveRootZrleGeometryOwnerCount == 0) {
            g_fRootHelperGeometryDispatchSuppressed = 1;
            AppendStartupCString(g_szQuitStatusMessage);
        }
        return 0;

    case 3:
        if (!IsDisplayModeWindowedRange(
                (const int *)g_pSelectedGraphicsDriverRuntime)) {
            Point.x = 0;
            Point.y = 0;
            ClientToScreen(hWnd, &Point);
            ((WINDOW_OWNER_POSITION_INTERFACE *)pOwner)->SetWindowPosition(&Point);
        }
        return 0;

    case 5:
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

    case 0x1c:
        fPreviousFullscreenActivationState =
            g_fFullscreenActivationState;
        nSelectedDisplayMode = *(int *)g_pSelectedGraphicsDriverRuntime;
        if (nSelectedDisplayMode == 1) {
            g_fFullscreenDisplayModeSuspended = 1;
            g_fFullscreenActivationState = 0;
            InvalidateRect(hWnd, 0, 0);
        } else if (nSelectedDisplayMode == 2 || nSelectedDisplayMode == 3) {
            if (wParam == 0) {
                ((void (WINAPI *)(int, int, int))g_pApplyFullscreenDisplayModeThunk)(
                    0, 0, 0x4000);
                g_fFullscreenActivationState = 0;
                g_fFullscreenDisplayModeSuspended = 1;
                SendMessageA(hWnd, 0x112, 0xf020, 0);
            } else {
                pOwnerState = (int (LEMBALL_FASTCALL *)(void *))
                    (*(void ***)pOwner)[0x68 / sizeof(void *)];
                if (pOwnerState(pOwner) == 0) {
                    SendMessageA(hWnd, 0x112, 0xf120, 0);
                }
                ((void (WINAPI *)(int, int, int))g_pApplyFullscreenDisplayModeThunk)(
                    0, 0, nSelectedDisplayMode == 3 ? 0x8215 : 0x8211);
                g_fFullscreenDisplayModeSuspended = 0;
                g_fFullscreenActivationState = 1;
                InvalidateRect(hWnd, 0, 0);
            }
        }

        if (fPreviousFullscreenActivationState !=
            g_fFullscreenActivationState) {
            if (g_fFullscreenActivationState != 0) {
                int anMouseAccelerationParams[3];
                WINDOW_OWNER_RECT ClipRect;

                SystemParametersInfoA(3, 0, anMouseAccelerationParams, 0);
                g_anSavedMouseAccelerationParams[0] =
                    anMouseAccelerationParams[0];
                g_anSavedMouseAccelerationParams[1] =
                    anMouseAccelerationParams[1];
                g_anSavedMouseAccelerationParams[2] =
                    anMouseAccelerationParams[2];
                GetSystemMetrics(1);
                anMouseAccelerationParams[2] =
                    *(short *)((char *)g_pDisplayState + 0x14) *
                    anMouseAccelerationParams[2] / GetSystemMetrics(0);
                SystemParametersInfoA(4, 0, anMouseAccelerationParams, 0);
                SystemParametersInfoA(0x10, 0,
                                      &g_nSavedMouseTrailsCount, 0);
                if (g_nSavedMouseTrailsCount != 0) {
                    SystemParametersInfoA(0x11, 0, 0, 0);
                }

                ClipRect.left = 0;
                ClipRect.top = 0;
                ClipRect.right = *(short *)((char *)g_pDisplayState + 0x14);
                ClipRect.bottom = *(short *)((char *)g_pDisplayState + 0x16);
                ClipCursor(&ClipRect);
            } else {
                ClipCursor(0);
                SystemParametersInfoA(4, 0,
                                      g_anSavedMouseAccelerationParams, 0);
                if (g_nSavedMouseTrailsCount != 0) {
                    SystemParametersInfoA(0x11, 0,
                                          (void *)g_nSavedMouseTrailsCount, 0);
                }
            }
        }

        pOwnerResult = (LRESULT (LEMBALL_FASTCALL *)(
            void *, int, UINT, WPARAM, LPARAM))
            (*(void ***)pOwner)[0xa0 / sizeof(void *)];
        return pOwnerResult(pOwner, 0, uMessage, wParam, lParam);

    case 0x7e: {
        if (g_pSelectedGraphicsDriverRuntime != 0) {
            unsigned short anDisplaySize[2];

            anDisplaySize[0] = (unsigned short)lParam;
            anDisplaySize[1] = (unsigned short)((unsigned long)lParam >> 16);
            ((VsGdiSelectedGraphicsDriverRuntime *)
                g_pSelectedGraphicsDriverRuntime)
                ->ResizeActiveDisplayState(anDisplaySize);
        }
        return 0;
    }

    case 0x100:
    case 0x101: {
        WINDOW_OWNER_KEY_EVENT Event;

        Event.m_nType = uMessage == 0x100 ? 2 : 1;
        Event.m_lParam = lParam;
        AppendWindowOwnerEvent(&Event);
        return 0;
    }

    case WM_COMMAND: {
        int nPayload;
        WINDOW_OWNER_COMMAND_EVENT Event;

        nPayload = ((WindowOwnerCommandTable *)pOwner)
            ->LookupWindowCommandEventPayload(0, (int)wParam);
        if (nPayload == 0) {
            pOwnerResult = (LRESULT (LEMBALL_FASTCALL *)(
                void *, int, UINT, WPARAM, LPARAM))
                (*(void ***)pOwner)[0xa0 / sizeof(void *)];
            return pOwnerResult(pOwner, 0, uMessage, wParam, lParam);
        }

        Event.m_nType = 0xf;
        Event.m_nReserved = 0;
        Event.m_dwTime = timeGetTime();
        Event.m_nPayload = nPayload;
        AppendWindowOwnerEvent(&Event);
        return 0;
    }

    case 0x201:
    case 0x203:
    case 0x204:
    case 0x206:
    case 0x207:
    case 0x209:
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

    case 0x202:
    case 0x205:
    case 0x208:
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

    case 7:
        if (g_pCachedCurrentContextWindow != 0) {
            pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                (*(void ***)g_pCachedCurrentContextOwner)[0x94 / sizeof(void *)];
            pOwnerCallback(g_pCachedCurrentContextOwner);
            pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                (*(void ***)g_pCachedCurrentContextOwner)[0x9c / sizeof(void *)];
            pOwnerCallback(g_pCachedCurrentContextOwner);
        }
        pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x90 / sizeof(void *)];
        pOwnerCallback(pOwner);
        pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
            (*(void ***)pOwner)[0x98 / sizeof(void *)];
        pOwnerCallback(pOwner);
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);

    case 8:
        nSelectedDisplayMode = *(int *)g_pSelectedGraphicsDriverRuntime;
        if (nSelectedDisplayMode < 4 || nSelectedDisplayMode > 5) {
            if (g_pCachedCurrentContextWindow != 0) {
                pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                    (*(void ***)g_pCachedCurrentContextOwner)[0x94 / sizeof(void *)];
                pOwnerCallback(g_pCachedCurrentContextOwner);
                pOwnerCallback = (void (LEMBALL_FASTCALL *)(void *))
                    (*(void ***)g_pCachedCurrentContextOwner)[0x9c / sizeof(void *)];
                pOwnerCallback(g_pCachedCurrentContextOwner);
            }
            g_pCachedCurrentContextWindow = 0;
        }
        return DefWindowProcA(hWnd, uMessage, wParam, lParam);

    default:
        return ((VS_WindowOwnerBase *)pOwner)
            ->DispatchWindowMessage(uMessage, wParam, lParam);
    }
}

// FUNCTION: LEMBALL 0x00473790
void __cdecl ShowFatalGetLastErrorMessageAndExit(const char *pszMessage) {
    VsInitFixedBufferStream Buffer;
    VsInitFormattedOutputStream Message;
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
int RegisterVSBaseWindowClass(void) {
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
        ShowFatalGetLastErrorMessageAndExit(g_szUnableToRegisterVSBaseWindowClassError);
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00465CC0
VS_RootGeometryOwner::VS_RootGeometryOwner(void) {
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
VS_RootGeometryOwner::~VS_RootGeometryOwner(void) {
    WINDOW_OWNER_REGISTRY_NODE *pRegistryNode;
    WINDOW_OWNER_REGISTRY_NODE *pNextRegistryNode;
    VS_RootGeometryOwnerChildNode *pChildNode;
    VS_RootGeometryOwnerChildNode *pNextChildNode;
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
VS_WindowOwnerBase::VS_WindowOwnerBase(void) {
    if (g_nLiveWindowOwnerBaseCount == 1) {
        RegisterVSBaseWindowClass();
    }
    m_hWindow44 = 0;
}

// FUNCTION: LEMBALL 0x00465200
void VS_WindowOwnerBase::InitializeWindowOwnerFromRect(
    short *paRect, void *pWindowOwner, const char *pszTitle) {
    WINDOW_OWNER_RECT WindowRect;
    tagPOINT Position;
    unsigned int uFlags;
    unsigned int uWindowStyle;
    unsigned int uMenuResourceId;
    void *pMenuDefinition;
    HMENU hMenu;
    HWND hParentWindow;
    int fHasMenu;
    int nSelectedDriver;

    m_Position0C.m_x = 0;
    m_Position0C.m_y = 0;
    m_Size08.m_x = paRect[0];
    m_Size08.m_y = paRect[1];
    SetWindowOwnerState(2);
    EnsureRootGeometryDispatchHelper();

    m_pParent20 = (VS_RootGeometryOwner *)pWindowOwner;
    m_pWindowContext48 = pWindowOwner;

    uFlags = GetWindowOwnerFlags();
    if (pWindowOwner != 0 && (uFlags & 0x40000000u) == 0) {
        m_pParent20->AppendQueuedRenderSinkValueNode(this);

        Position.x =
            *(short *)((char *)pWindowOwner + 0x0c) + paRect[2];
        Position.y =
            *(short *)((char *)pWindowOwner + 0x0e) + paRect[3];
        ClientToScreen(
            *(HWND *)((char *)pWindowOwner + 0x44), &Position);
        m_Position0C.m_x = (short)Position.x;
        m_Position0C.m_y = (short)Position.y;
        m_Origin18.m_x = paRect[2];
        m_Origin18.m_y = paRect[3];
        m_nScaleFactor38 =
            *(int *)((char *)pWindowOwner + 0x38);

        RetainRootGeometryOwner();
        ReservedWindowOwnerSlot15();
        CanonicalizeRootGeometrySubrect();
        ReservedWindowOwnerSlot17();
        return;
    }

    nSelectedDriver =
        ((VsGdiSelectedGraphicsDriverRuntime *)
             g_pSelectedGraphicsDriverRuntime)
            ->m_nSelectedDriver;
    if (nSelectedDriver >= 4 && nSelectedDriver <= 5) {
        m_pParent20 = 0;
        m_Size08.m_x = paRect[0];
        m_Size08.m_y = paRect[1];
        m_Position0C.m_x = paRect[2];
        m_Position0C.m_y = paRect[3];
        m_Origin18.m_x = paRect[2];
        m_Origin18.m_y = paRect[3];
        ((VsGdiSelectedGraphicsDriverRuntime *)
             g_pSelectedGraphicsDriverRuntime)
            ->m_pWindowOwner = this;
        m_hWindow44 = *(HWND *)((char *)GetDisplayState() + 0x10);

        CacheCurrentContextWindow();
        ReservedWindowOwnerSlot38();
        RetainRootGeometryOwner();
        ReservedWindowOwnerSlot15();
        CanonicalizeRootGeometrySubrect();
        ReservedWindowOwnerSlot17();
        return;
    }

    uWindowStyle = MapWindowOwnerFlagsToWin32Style(uFlags);
    hParentWindow = 0;
    if ((uFlags & 0x40000000u) != 0 && pWindowOwner != 0) {
        m_pParent20 = 0;
        uWindowStyle &= 0x7fffffffu;
        uWindowStyle |= 0x40000000u;
        hParentWindow = *(HWND *)((char *)pWindowOwner + 0x44);
    }

    WindowRect.left = paRect[2];
    WindowRect.top = paRect[3];
    WindowRect.right = paRect[2] + paRect[0];
    WindowRect.bottom = paRect[3] + paRect[1];

    hMenu = 0;
    uMenuResourceId = 0;
    pMenuDefinition = 0;
    fHasMenu = QueryRootGeometryState(
        &uMenuResourceId, &pMenuDefinition);
    if (fHasMenu != 0) {
        m_nMenuResourceId40 = uMenuResourceId;
        m_pMenuDefinition3C = pMenuDefinition;
        hMenu = LoadMenuA(
            g_hApplicationInstance,
            (LPCSTR)(unsigned long)(uMenuResourceId & 0xffff));
    }

    AdjustWindowRect(&WindowRect, uWindowStyle, fHasMenu != 0);
    m_hWindow44 = CreateWindowExA(
        0,
        g_pszVsBaseWindowClassName,
        pszTitle,
        uWindowStyle,
        WindowRect.left,
        WindowRect.top,
        WindowRect.right - WindowRect.left,
        WindowRect.bottom - WindowRect.top,
        hParentWindow,
        hMenu,
        g_hApplicationInstance,
        this);

    if (hMenu != 0) {
        RefreshWindowOwnerMenuState();
    }
    if (m_hWindow44 == 0) {
        ShowFatalGetLastErrorMessageAndExit(
            g_szUnableToCreateWindowError);
    }

    if ((GetWindowOwnerFlags() & 1) != 0) {
        UpdateWindow(m_hWindow44);
        ShowWindow(m_hWindow44, 5);
        SetForegroundWindow(m_hWindow44);
    }
}

// FUNCTION: LEMBALL 0x00465570
VS_WindowOwnerBase::~VS_WindowOwnerBase(void) {
    if (g_nLiveWindowOwnerBaseCount == 1) {
        ShowCursor(0);
    }
}
