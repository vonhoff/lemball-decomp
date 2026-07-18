#include "FRONTEND/MENUSEL.H"

#include "GAME.H"
#include "RESOURCE/VARMGR.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSGEOM.H"
#include "ENGINE/GDI/VSGDI.H"
#include "LEVEL/DRAWTEXT.H"

DECOMP_SIZE_ASSERT(GameMenuSelectorEvent, 0x1c);
DECOMP_SIZE_ASSERT(GameMenuSelectorMetadata, 0x20);
DECOMP_SIZE_ASSERT(GameProgressiveVariantFrameSelector, 0x1c);
DECOMP_SIZE_ASSERT(GameMenuSelectorManager, 0x27c);

extern "C" DWORD WINAPI timeGetTime(void);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void VariantResourceBundleNoopThunk(void);
extern void *ConstructVariantRenderManager(void *pObject, int nOwner,
                                           int cResourceIds,
                                           int cPackagedRectEntries,
                                           int cActionButtonEntries,
                                           int nUnknown58,
                                           int fDoubleBuffered);
extern void LoadVariantRenderResource(void *pObject, int nResourceId);
extern void EmitVariantRenderResourceEntry(void *pObject, short *pRect,
                                           const short *pPoint,
                                           int nResourceId, int nFlags,
                                           void *pFrameSelector, int nValue);
extern void ResetOrFlipVariantRenderBuffers(int nManager);
extern void DestroyVariantRenderManager(void *pObject);
extern void *LEMBALL_FASTCALL InitializeHelperLocalRectUpdateEntry(
    void *pObject);
extern void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableC(
    void *pObject);
extern void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(
    void *pResourceObject);
extern void *LoadZrleOnlyListResource(int nResourceId);
extern void *g_pNonZrleVariantRenderEntryInitializeVtable[2];
extern void *g_pQueuedRenderPointSinkFinalizeThunk;
extern void *LEMBALL_FASTCALL DeleteStatusPointSinkEntryThunk(
    void *pEntry, int nUnusedEdx, unsigned char fDelete);
extern void LEMBALL_FASTCALL QueueStatusPointPushThunk(
    void *pEntry, int nUnusedEdx, void *pQueue);
extern void AppendPointerQueueEntry(void *pQueue, void *pEntry);
extern void LEMBALL_FASTCALL RemoveGeometryOwnerChild(
    void *pOwner, int nUnusedEdx, void *pChildOwner);
extern void *LEMBALL_FASTCALL ConstructWindowOwnerRenderContext(
    void *pObject);
extern void LEMBALL_FASTCALL DestroyWindowOwnerRenderContext(
    void *pObject);
extern void *LEMBALL_FASTCALL InitializeEmptyGeometryOwnerRect(
    void *pOwner);
extern void LEMBALL_FASTCALL RestoreQueuedRenderPointSinkVtable(
    void *pObject);
extern void LEMBALL_FASTCALL AppendGeometryOwnerChild(
    void *pOwner, int nUnusedEdx, void *pChildOwner);
extern void *g_pVariantResourceEntryManager;
extern int g_fLevelSelectionChanged;
extern int LEMBALL_FASTCALL GetTimedVariantFrameStartTimeThunk(
    void *pObject);
extern void LEMBALL_FASTCALL SetTimedVariantFrameDuration(
    void *pObject, int nUnusedEdx, int nFrameDuration);
extern void LEMBALL_FASTCALL set_object_field_0x10_00435890(
    void *pObject, void *pUnusedEdx, int nValue);
extern void LEMBALL_FASTCALL set_object_field_0x0c_004358a0(
    void *pObject, void *pUnusedEdx, int nValue);
extern void LEMBALL_FASTCALL set_object_field_0x18_004358b0(
    void *pObject, void *pUnusedEdx, int nValue);
extern int LEMBALL_FASTCALL return_object_field_0x0c_00435880(
    void *pObject);

struct GameMenuSelectorEntry {
    void **m_pVtable00;
    int m_nQueueTag04;
    int m_nQueueReserved08;
    int m_cUnhandledEvents0C;
    int m_nOverlayKind10;
    short m_nProgressX14;
    short m_nProgressY16;
    short m_nProgressWidth18;
    short m_nProgressHeight1A;
    int m_x1C;
    int m_nPosition20;
    void *m_pRenderContext24;
    void *m_pRenderOwner28;
    int m_nActivationMode2C;
    int m_nToken30;
    int m_nCommand34;
    int m_cResources38;
    int m_nLastResource3C;
    int m_nFirstResource40;
    int m_nSelectedResource44;
    int *m_pSelection48;
    const int *m_panResourceIds4C;
    void *m_pAlignedOverlay50;
    void *m_pProgressOverlay54;
    void **m_ppLoadedResources58;
    int m_fEnabled5C;
};

typedef char GameMenuSelectorEntry_size_must_be_0x60[
    sizeof(GameMenuSelectorEntry) == 0x60 ? 1 : -1];

typedef int (LEMBALL_FASTCALL *GameFrameSelectorIndexProc)(
    GameProgressiveVariantFrameSelector *pSelector);
typedef void (LEMBALL_FASTCALL *GameFrameSelectorSetProc)(
    GameProgressiveVariantFrameSelector *pSelector, int nUnusedEdx,
    int nValue);
typedef int (LEMBALL_FASTCALL *GameMenuSelectorEventProc)(
    GameMenuSelectorManager *pManager, int nUnusedEdx,
    GameMenuSelectorEvent *pEvent);
typedef void * (LEMBALL_FASTCALL *GameMenuSelectorDeleteProc)(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int fDelete);

static void LEMBALL_FASTCALL DestroyMenuSelectorManager(
    GameMenuSelectorManager *pManager);
static void *LEMBALL_FASTCALL DeleteMenuSelectorManager(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteMenuSelectorManagerThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int fDelete);
static int LEMBALL_FASTCALL HandleMenuSelectorEvent(
    GameMenuSelectorManager *pManager, int nUnusedEdx,
    GameMenuSelectorEvent *pEvent);
static int LEMBALL_FASTCALL HandleMenuSelectorEventThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx,
    GameMenuSelectorEvent *pEvent);
static void LEMBALL_FASTCALL MoveMenuSelectorPrevious(
    GameMenuSelectorManager *pManager);
static void LEMBALL_FASTCALL MoveMenuSelectorPreviousThunk(
    GameMenuSelectorManager *pManager);
static void LEMBALL_FASTCALL MoveMenuSelectorNext(
    GameMenuSelectorManager *pManager);
static void LEMBALL_FASTCALL MoveMenuSelectorNextThunk(
    GameMenuSelectorManager *pManager);
static void LEMBALL_FASTCALL SelectMenuSelectorSide0(
    GameMenuSelectorManager *pManager);
static void LEMBALL_FASTCALL SelectMenuSelectorSide0Thunk(
    GameMenuSelectorManager *pManager);
static void LEMBALL_FASTCALL SelectMenuSelectorSide1(
    GameMenuSelectorManager *pManager);
static void LEMBALL_FASTCALL SelectMenuSelectorSide1Thunk(
    GameMenuSelectorManager *pManager);
static void LEMBALL_FASTCALL StartMenuSelectorAction(
    GameMenuSelectorManager *pManager);
static void LEMBALL_FASTCALL StartMenuSelectorActionThunk(
    GameMenuSelectorManager *pManager);
static void LEMBALL_FASTCALL RecordMenuSelectorChildMetadata(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int x,
    int nPosition, int fAction, int nToken);
static void LEMBALL_FASTCALL ReleaseMenuSelectorEntryResources(
    GameMenuSelectorEntry *pEntry);
static void LEMBALL_FASTCALL BuildMenuSelectorEntryOverlay(
    GameMenuSelectorEntry *pEntry, int nUnusedEdx,
    const int *panResourceIds);
static void LEMBALL_FASTCALL ReloadMenuSelectorOverlayResource(
    void *pOverlay, int nUnusedEdx, int nResourceId);
static void LEMBALL_FASTCALL SetMenuSelectorProgressPercent(
    void *pProgress, int nUnusedEdx, int nPercent);
static int LEMBALL_FASTCALL HandleMenuSelectorEntryEvent(
    GameMenuSelectorEntry *pEntry, int nUnusedEdx,
    GameMenuSelectorEvent *pEvent);
static void LEMBALL_FASTCALL DestroyMenuSelectorEntry(
    GameMenuSelectorEntry *pEntry);
static void *LEMBALL_FASTCALL DeleteMenuSelectorEntry(
    GameMenuSelectorEntry *pEntry, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteMenuSelectorEntryThunk(
    GameMenuSelectorEntry *pEntry, int nUnusedEdx, int fDelete);
static int LEMBALL_FASTCALL HandleMenuSelectorEntryEventThunk(
    GameMenuSelectorEntry *pEntry, int nUnusedEdx,
    GameMenuSelectorEvent *pEvent);
void LEMBALL_FASTCALL InitializeSharedSpriteOwnerState(void *pObject);
void LEMBALL_FASTCALL ComputeAlignedZrleChildLayout(void *pObject);

/* GLOBAL: LEMBALL 0x004a7b44 */
int g_nMenuSelectorResourceId0 = 0;
/* GLOBAL: LEMBALL 0x004a7b3c */
int g_nMenuSelectorResourceId1 = 0;
/* GLOBAL: LEMBALL 0x004a7b40 */
int g_nMenuSelectorResourceId2 = 0;
/* GLOBAL: LEMBALL 0x004a7b50 */
int g_nMenuSelectorResourceId3 = 0;
/* GLOBAL: LEMBALL 0x004a7b38 */
int g_nMenuSelectorResourceId4 = 0;
/* GLOBAL: LEMBALL 0x004a7b48 */
int g_nMenuSelectorResourceId5 = 0;
/* GLOBAL: LEMBALL 0x004a7b54 */
int g_nMenuSelectorResourceId6 = 0;
/* GLOBAL: LEMBALL 0x004a7b58 */
int g_nMenuSelectorResourceId7 = 0;
/* GLOBAL: LEMBALL 0x004a7b4c */
int g_nMenuSelectorResourceId8 = 0;

/* GLOBAL: LEMBALL 0x0049fa70 */
int g_anMenuSelectorActionOffsetsMode0[18] = {
    0, 12, -8, -2, 52, -2, -13, 28, 116,
    0, 0, -13, 19, 28, -16, 0, -24, -2
};
/* GLOBAL: LEMBALL 0x0049fab8 */
int g_anMenuSelectorActionOffsetsMode1[18] = {
    0, 6, -4, -2, 26, -2, -7, 14, 58,
    0, 0, 0, 10, 14, -8, 0, -12, -2
};

/* FUNCTION: LEMBALL 0x00402A59 */
static int LEMBALL_FASTCALL GetMenuSelectorStartTimeThunk(
    GameProgressiveVariantFrameSelector *pSelector) {
    return return_object_field_0x0c_00435880(pSelector);
}

/* FUNCTION: LEMBALL 0x00467170 */
static int LEMBALL_FASTCALL SelectTimedVariantFrameIndex(
    GameProgressiveVariantFrameSelector *pSelector) {
    unsigned int dwNow;
    unsigned int iFrame;

    dwNow = (unsigned int)pSelector->m_dwFixedTime;
    if (dwNow == 0xffffffffU) {
        dwNow = (unsigned int)timeGetTime();
    }
    iFrame = (((dwNow - (unsigned int)pSelector->m_dwStartTime) %
               (unsigned int)pSelector->m_dwDuration) *
              (unsigned int)pSelector->m_cFrames) /
             (unsigned int)pSelector->m_dwDuration;
    if (pSelector->m_nDirection != 1) {
        iFrame = (unsigned int)pSelector->m_cFrames - iFrame - 1;
    }
    return (int)iFrame;
}

/* FUNCTION: LEMBALL 0x004671E0 */
static void LEMBALL_FASTCALL SetProgressiveVariantFrameDuration(
    GameProgressiveVariantFrameSelector *pSelector, int, int dwDuration) {
    pSelector->m_dwDuration = dwDuration;
    if (pSelector->m_dwFixedTime == -1) {
        pSelector->m_dwStartTime = (int)timeGetTime();
    } else {
        pSelector->m_dwStartTime = pSelector->m_dwFixedTime;
    }
}

/* FUNCTION: LEMBALL 0x00467210 */
static int LEMBALL_FASTCALL SelectProgressiveVariantFrameIndex(
    GameProgressiveVariantFrameSelector *pSelector) {
    unsigned int dwElapsed;
    unsigned int iFrame;

    if (pSelector->m_dwFixedTime == -1) {
        dwElapsed = (unsigned int)timeGetTime() -
                    (unsigned int)pSelector->m_dwStartTime;
    } else {
        dwElapsed = (unsigned int)(pSelector->m_dwFixedTime -
                                   pSelector->m_dwStartTime);
    }
    if (dwElapsed < (unsigned int)pSelector->m_dwDuration) {
        iFrame = ((unsigned int)pSelector->m_cFrames * dwElapsed) /
                 (unsigned int)pSelector->m_dwDuration;
    } else {
        iFrame = (unsigned int)pSelector->m_cFrames - 1;
    }
    if (pSelector->m_nDirection != 1) {
        iFrame = (unsigned int)pSelector->m_cFrames - iFrame - 1;
    }
    return (int)iFrame;
}

/* FUNCTION: LEMBALL 0x00402F3B */
static void LEMBALL_FASTCALL SetMenuSelectorDurationFieldThunk(
    GameProgressiveVariantFrameSelector *pSelector, int, int nValue) {
    set_object_field_0x10_00435890(pSelector, 0, nValue);
}

/* FUNCTION: LEMBALL 0x00402DD8 */
static void LEMBALL_FASTCALL SetMenuSelectorStartTimeFieldThunk(
    GameProgressiveVariantFrameSelector *pSelector, int, int nValue) {
    set_object_field_0x0c_004358a0(pSelector, 0, nValue);
}

/* FUNCTION: LEMBALL 0x00402D5B */
static void LEMBALL_FASTCALL SetMenuSelectorDirectionFieldThunk(
    GameProgressiveVariantFrameSelector *pSelector, int, int nValue) {
    set_object_field_0x18_004358b0(pSelector, 0, nValue);
}

/* VTABLE: LEMBALL 0x00496D00 */
void *g_MenuSelectorFrameSelectorCtorVtable[2] = {
    (void *)_purecall,
    (void *)GetMenuSelectorStartTimeThunk
};

/* VTABLE: LEMBALL 0x00496D08 */
void *g_MenuSelectorTimedFrameSelectorVtable[6] = {
    (void *)SelectTimedVariantFrameIndex,
    (void *)_purecall,
    (void *)SetMenuSelectorDurationFieldThunk,
    (void *)SetMenuSelectorStartTimeFieldThunk,
    (void *)SetMenuSelectorDirectionFieldThunk,
    0
};

/* VTABLE: LEMBALL 0x00496D20 */
void *g_MenuSelectorProgressiveFrameSelectorVtable[6] = {
    (void *)SelectProgressiveVariantFrameIndex,
    (void *)SetProgressiveVariantFrameDuration,
    (void *)SetMenuSelectorDurationFieldThunk,
    (void *)SetMenuSelectorStartTimeFieldThunk,
    (void *)SetMenuSelectorDirectionFieldThunk,
    0
};

/* VTABLE: LEMBALL 0x00496CE4 */
static void *g_MenuSelectorCurrentFrameCallbackVtable[7] = {
    (void *)GetTimedVariantFrameStartTimeThunk,
    (void *)SelectTimedVariantFrameIndex,
    (void *)SetTimedVariantFrameDuration,
    (void *)SetMenuSelectorDurationFieldThunk,
    (void *)SetMenuSelectorStartTimeFieldThunk,
    (void *)SetMenuSelectorDirectionFieldThunk,
    0
};

/* VTABLE: LEMBALL 0x00497F0C */
static void *g_MenuSelectorVariantRenderManagerVtable[1] = {
    (void *)VariantResourceBundleNoopThunk
};

/* VTABLE: LEMBALL 0x00497F10 */
static void *g_MenuSelectorManagerVtable[3] = {
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteMenuSelectorManagerThunk,
    (void *)HandleMenuSelectorEventThunk
};

struct GameMenuActionFramePointSink {
    void **m_pVtable00;
    short m_nReserved04;
    short m_nReserved06;
    unsigned char m_abReserved08[0x10];
    int m_iFrame18;
    int *m_pResource1C;

    void *Delete(unsigned int fDelete);
    void BindToContext(void *pContext);
    void DispatchToRenderObject(void *pContext);
};

class GameResourceGeometryChildView {
public:
    void SetRenderAnchorWithOriginCompensation(const VS_XYPair *pPoint);
    int GetQueuedRenderPointSinkState(void);
    void DetachAndDestroyWindowOwner(void);
    void ForwardNestedRectUpdate(short *pRect);
    void QueueIfDirty(void);
    void RefreshIfHelperDirty(void);
};

class GameResourceGeometryChildDeleteView {
public:
    void *Delete(unsigned int fDelete);
};

class GameAlignedZrleChildOverlayView {
public:
    void ReleaseFrameResource(void);
    void QueueAlignedFrame(void);
    void HandleButtonRelease(int nButton);
    void HandleButtonPress(int nButton);
    void HandlePointerEnter(void);
    void HandlePointerLeave(void);
};

class GameAlignedZrleChildOverlayDeleteView {
public:
    void *Delete(unsigned int fDelete);
};

class GameMenuSelectorChildContextView {
public:
    void MarkRenderObjectDirty(void);
};

typedef char GameMenuActionFramePointSink_size_must_be_0x20[
    sizeof(GameMenuActionFramePointSink) == 0x20 ? 1 : -1];

/* VTABLE: LEMBALL 0x00497CF8 */
static void *g_MenuSelectorEntryVtable[3] = {
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteMenuSelectorEntryThunk,
    (void *)HandleMenuSelectorEntryEventThunk
};

/* VTABLE: LEMBALL 0x00496e70 */
static void *g_MenuSelectorTinyPointSinkVtable[2] = {
    (void *)DeleteStatusPointSinkEntryThunk,
    (void *)QueueStatusPointPushThunk
};

static void *g_MenuActionFramePointSinkVtable[3];

struct GameMenuActionFramePointSinkVtableFixup {
    GameMenuActionFramePointSinkVtableFixup(void) {
        union DeleteMethod {
            void *(GameMenuActionFramePointSink::*p)(unsigned int);
            void *v;
        } Delete;
        union BindMethod {
            void (GameMenuActionFramePointSink::*p)(void *);
            void *v;
        } Bind;

        Delete.p = &GameMenuActionFramePointSink::Delete;
        Bind.p = &GameMenuActionFramePointSink::BindToContext;
        g_MenuActionFramePointSinkVtable[0] = Delete.v;
        g_MenuActionFramePointSinkVtable[1] = Bind.v;
        Bind.p = &GameMenuActionFramePointSink::DispatchToRenderObject;
        g_MenuActionFramePointSinkVtable[2] = Bind.v;
    }
};

static GameMenuActionFramePointSinkVtableFixup
    g_MenuActionFramePointSinkVtableFixup;

static void *g_ResourceGeometryChildVtable[52];
static void *g_ResourceGeometryChildDeleteVtable[8];
static void **g_pWindowOwnerRenderContextVtable;
static int g_fResourceGeometryChildVtablesInitialized;
static void *g_AlignedZrleChildOverlayVtable[52];
static void *g_AlignedZrleChildOverlayDeleteVtable[8];
static int g_fAlignedZrleChildOverlayVtablesInitialized;
static void *g_MenuSelectorChildContextVtable[46];
static int g_fMenuSelectorChildContextVtableInitialized;

static int MapSpriteOwnerEventToFrameSlot(int nEvent) {
    switch (nEvent) {
    case 0:
    case 3:
        return 0;
    case 1:
    case 4:
        return 1;
    case 2:
    case 5:
        return 2;
    }
    return 6;
}

static void QueueSpriteOwnerEvent(
    void *pObject, short nType, int nFrame) {
    GameMenuSelectorEvent Event;

    Event.m_nType00 = nType;
    Event.m_nReserved02 = 0;
    Event.m_dwTimestamp04 = (int)timeGetTime();
    Event.m_nPayload08 = *(int *)((char *)pObject + 0xcc);
    Event.m_nPayload0C = (int)(unsigned long)pObject;
    Event.m_nPayload10 = nFrame;
    Event.m_nPayload14 = 0;
    Event.m_nPayload18 = 0;
    ((void (LEMBALL_FASTCALL *)(void *, int, void *))
         (*(void ***)g_pSharedRenderDispatchQueue)[2])(
            g_pSharedRenderDispatchQueue, 0, &Event);
}

static void NotifySpriteOwnerFrameEvent(
    void *pObject, short nType, int nEvent) {
    if (*(int *)((char *)pObject + 0xf8) == 0) {
        *(int *)((char *)pObject + 0xd8) = 1;
    }
    if (*(void **)((char *)pObject + 0xf4) != 0) {
        QueueSpriteOwnerEvent(
            pObject, nType, MapSpriteOwnerEventToFrameSlot(nEvent));
    }
}

static void NotifySpriteOwnerStateEvent(void *pObject, short nType) {
    if (*(void **)((char *)pObject + 0xf4) != 0) {
        QueueSpriteOwnerEvent(pObject, nType, 0);
    }
}

static void StoreSpriteOwnerLocalPoint(
    void *pGeometryOwner, const VS_XYPair *pPoint) {
    *(short *)((char *)pGeometryOwner + 0x70) =
        (short)(pPoint->m_x -
                *(short *)((char *)pGeometryOwner - 0x78));
    *(short *)((char *)pGeometryOwner + 0x72) =
        (short)(pPoint->m_y -
                *(short *)((char *)pGeometryOwner - 0x76));
}

static void LEMBALL_FASTCALL HandleSpriteOwnerPointerEnter(
    void *pGeometryOwner) {
    void *pObject;

    pObject = (char *)pGeometryOwner - 0x90;
    if (*(int *)((char *)pGeometryOwner + 0x10) != 0 ||
        *(int *)((char *)pGeometryOwner + 0x1c) != 0) {
        *(int *)((char *)pGeometryOwner + 0x40) = 1;
    }
    NotifySpriteOwnerStateEvent(pObject, 0x0d);
    ((void (LEMBALL_FASTCALL *)(void *))(*(void ***)pObject)[50])(
        pObject);
}

static void LEMBALL_FASTCALL HandleSpriteOwnerPointerLeave(
    void *pGeometryOwner) {
    void *pObject;

    pObject = (char *)pGeometryOwner - 0x90;
    *(int *)((char *)pGeometryOwner + 0x40) = 0;
    NotifySpriteOwnerStateEvent(pObject, 0x0e);
    ((void (LEMBALL_FASTCALL *)(void *))(*(void ***)pObject)[51])(
        pObject);
}

static void LEMBALL_FASTCALL HandleSpriteOwnerButtonTransition(
    void *pGeometryOwner, int, const VS_XYPair *pPoint, int nButton) {
    void *pObject;
    int nFrame;

    pObject = (char *)pGeometryOwner - 0x90;
    if (nButton == 0 || nButton == 3) {
        *(int *)((char *)pGeometryOwner + 0x40) = 1;
    }
    nFrame = MapSpriteOwnerEventToFrameSlot(nButton);
    StoreSpriteOwnerLocalPoint(pGeometryOwner, pPoint);
    NotifySpriteOwnerFrameEvent(pObject, 0x0b, nFrame);
    ((void (LEMBALL_FASTCALL *)(void *, int, int))
         (*(void ***)pObject)[49])(pObject, 0, nFrame);
}

static void LEMBALL_FASTCALL HandleSpriteOwnerButtonInside(
    void *pGeometryOwner, int, const VS_XYPair *pPoint, int nButton) {
    void *pObject;
    int nFrame;

    if (*(int *)((char *)pGeometryOwner + 0x40) == 0) {
        return;
    }
    pObject = (char *)pGeometryOwner - 0x90;
    nFrame = MapSpriteOwnerEventToFrameSlot(nButton);
    *(int *)((char *)pGeometryOwner + 0x40) = 0;
    StoreSpriteOwnerLocalPoint(pGeometryOwner, pPoint);
    NotifySpriteOwnerFrameEvent(pObject, 0x0c, nFrame);
    ((void (LEMBALL_FASTCALL *)(void *, int, int))
         (*(void ***)pObject)[48])(pObject, 0, nFrame);
}

static void LEMBALL_FASTCALL ResetSpriteOwnerButtonState(
    void *pGeometryOwner, int, const VS_XYPair *pPoint, int) {
    int i;

    StoreSpriteOwnerLocalPoint(pGeometryOwner, pPoint);
    for (i = 0; i < 6; ++i) {
        *(int *)((char *)pGeometryOwner + 0x10 + i * 4) = 0;
    }
}

void *LEMBALL_FASTCALL InitializeTinyPointSinkHelper(void *pObject) {
    *(void ***)pObject = g_MenuSelectorTinyPointSinkVtable;
    *(int *)((char *)pObject + 4) = 0;
    return pObject;
}

void *GameMenuActionFramePointSink::Delete(unsigned int fDelete) {
    GameMenuActionFramePointSink *pEntry;
    int cEntries;

    if ((fDelete & 2) == 0) {
        m_pVtable00 = (void **)g_pQueuedRenderPointSinkFinalizeThunk;
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(this);
        }
        return this;
    }

    cEntries = *(int *)((char *)this - 4);
    pEntry = this + cEntries;
    while (--cEntries >= 0) {
        --pEntry;
        pEntry->m_pVtable00 =
            (void **)g_pQueuedRenderPointSinkFinalizeThunk;
    }
    FreeVSMemBlock((char *)this - 4);
    return this;
}

void GameMenuActionFramePointSink::BindToContext(void *pContext) {
    typedef void (LEMBALL_FASTCALL *LoadProc)(void *pObject);

    if (m_pResource1C[4] == 0) {
        ((LoadProc)(*(void ***)m_pResource1C)[7])(m_pResource1C);
    } else {
        m_pResource1C[9] = 0;
    }
    ++m_pResource1C[2];
    AppendPointerQueueEntry(pContext, this);
}

void GameMenuActionFramePointSink::DispatchToRenderObject(void *pContext) {
    typedef void (LEMBALL_FASTCALL *RenderProc)(
        void *pRenderObject, int nUnusedEdx, void *pEntry,
        void *pFrame);
    int nContext;
    int nRenderObject;
    void *pFrame;

    nContext = *(int *)((char *)pContext + 0x0c);
    nRenderObject = *(int *)(*(int *)(nContext + 0x40) + 4) +
                    0x40 + nContext;
    pFrame = (char *)m_pResource1C[0x1e] + m_iFrame18 * 0x54;
    ((RenderProc)(*(void ***)nRenderObject)[9])(
        (void *)nRenderObject, 0, this, pFrame);
    --m_pResource1C[2];
}

void *LEMBALL_FASTCALL InitializeMenuActionFramePointSink(void *pObject) {
    GameMenuActionFramePointSink *pEntry;

    pEntry = (GameMenuActionFramePointSink *)pObject;
    pEntry->m_pVtable00 = (void **)g_pQueuedRenderPointSinkFinalizeThunk;
    pEntry->m_nReserved06 = 0;
    pEntry->m_pVtable00 = g_MenuActionFramePointSinkVtable;
    pEntry->m_nReserved04 = 0;
    return pEntry;
}

void LEMBALL_FASTCALL DetachGeometryChildAndDestroyWindowOwner(
    void *pObject) {
    void *pChildOwner;
    void *pRenderOwner;

    pChildOwner = pObject == 0 ? 0 : (char *)pObject + 0x90;
    pRenderOwner = *(void **)((char *)pObject + 0xc8);
    RemoveGeometryOwnerChild(
        *(void **)((char *)pRenderOwner + 0x1c), 0, pChildOwner);
    ((VS_WindowOwnerBase *)pObject)
        ->VS_WindowOwnerBase::DestroyWindowOwnerHwnd();
}

static inline int ResolveResourceGeometryRenderObject(void *pObject) {
    int nContext;

    nContext = *(int *)(*(int *)((char *)pObject + 0x4c) + 0x0c);
    return *(int *)(*(int *)(nContext + 0x40) + 4) +
           0x40 + nContext;
}

void LEMBALL_FASTCALL RefreshCompactZrleOwnerFromResourceHelper(
    void *pObject) {
    typedef void (LEMBALL_FASTCALL *VoidProc)(void *pObject);
    typedef void (LEMBALL_FASTCALL *ContextProc)(
        void *pObject, int nUnusedEdx, void *pContext);
    int nRenderObject;

    nRenderObject = ResolveResourceGeometryRenderObject(pObject);
    ((VoidProc)(*(void ***)nRenderObject)[14])((void *)nRenderObject);
    if (*(int *)((char *)pObject + 0xd8) == 0) {
        *(int *)((char *)pObject + 0xf0) = 0;
    } else {
        --*(int *)((char *)pObject + 0xd8);
        nRenderObject = ResolveResourceGeometryRenderObject(pObject);
        *(short *)((char *)pObject + 0xe8) =
            *(short *)(nRenderObject + 0x14);
        *(short *)((char *)pObject + 0xea) =
            *(short *)(nRenderObject + 0x16);
        *(short *)((char *)pObject + 0xec) = 0;
        *(short *)((char *)pObject + 0xee) = 0;
        *(int *)((char *)pObject + 0xf0) = 0x10000;
        nRenderObject = ResolveResourceGeometryRenderObject(pObject);
        *(int *)(nRenderObject + 0x78) = 1;
    }
    ((ContextProc)(*(void ***)((char *)pObject + 0xe4))[1])(
        (char *)pObject + 0xe4, 0,
        *(void **)((char *)pObject + 0x4c));
}

void LEMBALL_FASTCALL RefreshCompactZrleOwnerIfHelperDirty(
    void *pObject) {
    typedef int (LEMBALL_FASTCALL *QueryProc)(void *pObject);
    int nRenderObject;

    if (*(int *)((char *)pObject + 0x104) ==
        *(int *)((char *)pObject + 0x108)) {
        nRenderObject = ResolveResourceGeometryRenderObject(pObject);
        if (((QueryProc)(*(void ***)nRenderObject)[15])(
                (void *)nRenderObject) == 0) {
            RefreshCompactZrleOwnerFromResourceHelper(pObject);
            return;
        }
    }
    nRenderObject = ResolveResourceGeometryRenderObject(pObject);
    *(int *)(nRenderObject + 0x78) = 1;
    *(int *)((char *)pObject + 0x108) =
        *(int *)((char *)pObject + 0x104);
    RefreshCompactZrleOwnerFromResourceHelper(pObject);
}

void LEMBALL_FASTCALL QueueCompactZrleOwnerIfDirty(void *pObject) {
    typedef int (LEMBALL_FASTCALL *QueryProc)(void *pObject);
    typedef void (LEMBALL_FASTCALL *VoidProc)(void *pObject);
    void *pRenderContext;
    void *pOwner;
    int nRenderObject;
    int fHelperDirty;
    int nUploadState;

    nRenderObject = ResolveResourceGeometryRenderObject(pObject);
    fHelperDirty = ((QueryProc)(*(void ***)nRenderObject)[15])(
        (void *)nRenderObject);
    pRenderContext = *(void **)((char *)pObject + 0x4c);
    if (!((fHelperDirty != 0 &&
           (*(int *)((char *)pObject + 0xd0) != 0 ||
            *(int *)((char *)pObject + 0x104) !=
                *(int *)((char *)pObject + 0x108))) ||
          (*(int *)((char *)pRenderContext + 4) == 0 &&
           (*(int *)((char *)pObject + 0xf8) != 0 ||
            *(int *)((char *)pObject + 0xd8) != 0 ||
            *(int *)((char *)pObject + 0xd0) !=
                *(int *)((char *)pObject + 0xd4))))) {
        return;
    }

    if (((QueryProc)(*(void ***)pObject)[26])(pObject) != 0) {
        ((VoidProc)(*(void ***)pObject)[46])(pObject);
        ((VoidProc)(*(void ***)pObject)[47])(pObject);
    }
    pOwner = *(void **)((char *)pRenderContext + 0x0c);
    nUploadState = ((QueryProc)(*(void ***)pOwner)[2])(pOwner);
    AppendPointerQueueEntry(pRenderContext,
                            *(void **)((char *)pObject + 0xe0));
    InitializeHelperUploadStatePending(nUploadState);
    *(int *)((char *)pObject + 0xfc) = 1;
}

void GameResourceGeometryChildView::SetRenderAnchorWithOriginCompensation(
    const VS_XYPair *pPoint) {
    char *pObject;

    pObject = (char *)this;
    *(int *)(pObject + 0xd8) = 1;
    *(short *)(pObject + 0xc4) -= *(short *)(pObject + 0x18);
    *(short *)(pObject + 0xc6) -= *(short *)(pObject + 0x1a);
    SetWindowOwnerRenderAnchorFromPoint(this, 0, pPoint);
    *(short *)(pObject + 0xc4) += *(short *)(pObject + 0x18);
    *(short *)(pObject + 0xc6) += *(short *)(pObject + 0x1a);
}

int GameResourceGeometryChildView::GetQueuedRenderPointSinkState(void) {
    return *(int *)((char *)this + 0x30);
}

void GameResourceGeometryChildView::DetachAndDestroyWindowOwner(void) {
    DetachGeometryChildAndDestroyWindowOwner(this);
}

void GameResourceGeometryChildView::ForwardNestedRectUpdate(short *pRect) {
    ForwardNestedGeometryRectUpdate(this, 0, pRect);
}

void GameResourceGeometryChildView::QueueIfDirty(void) {
    QueueCompactZrleOwnerIfDirty(this);
}

void GameResourceGeometryChildView::RefreshIfHelperDirty(void) {
    RefreshCompactZrleOwnerIfHelperDirty(this);
}

static void InitializeResourceGeometryChildVtables(
    void **pWindowOwnerVtable, void **pGeometryOwnerVtable) {
    union PointMethod {
        void (GameResourceGeometryChildView::*p)(const VS_XYPair *);
        void *v;
    } Point;
    union RectMethod {
        void (GameResourceGeometryChildView::*p)(short *);
        void *v;
    } Rect;
    union VoidMethod {
        void (GameResourceGeometryChildView::*p)(void);
        void *v;
    } Void;
    union IntMethod {
        int (GameResourceGeometryChildView::*p)(void);
        void *v;
    } Int;
    union DeleteMethod {
        void *(GameResourceGeometryChildDeleteView::*p)(unsigned int);
        void *v;
    } Delete;
    int i;

    if (g_fResourceGeometryChildVtablesInitialized != 0) {
        return;
    }
    for (i = 0; i < 46; ++i) {
        g_ResourceGeometryChildVtable[i] = pWindowOwnerVtable[i];
    }
    for (i = 0; i < 8; ++i) {
        g_ResourceGeometryChildDeleteVtable[i] = pGeometryOwnerVtable[i];
    }
    g_pWindowOwnerRenderContextVtable = pWindowOwnerVtable;

    Point.p = &GameResourceGeometryChildView::
        SetRenderAnchorWithOriginCompensation;
    g_ResourceGeometryChildVtable[14] = Point.v;
    Int.p = &GameResourceGeometryChildView::GetQueuedRenderPointSinkState;
    g_ResourceGeometryChildVtable[26] = Int.v;
    Void.p = &GameResourceGeometryChildView::DetachAndDestroyWindowOwner;
    g_ResourceGeometryChildVtable[29] = Void.v;
    Rect.p = &GameResourceGeometryChildView::ForwardNestedRectUpdate;
    g_ResourceGeometryChildVtable[33] = Rect.v;
    Void.p = &GameResourceGeometryChildView::RefreshIfHelperDirty;
    g_ResourceGeometryChildVtable[46] = Void.v;
    g_ResourceGeometryChildVtable[47] = (void *)_purecall;
    g_ResourceGeometryChildVtable[48] = (void *)_purecall;
    g_ResourceGeometryChildVtable[49] = (void *)_purecall;
    g_ResourceGeometryChildVtable[50] = (void *)_purecall;
    g_ResourceGeometryChildVtable[51] = (void *)_purecall;

    Delete.p = &GameResourceGeometryChildDeleteView::Delete;
    g_ResourceGeometryChildDeleteVtable[0] = Delete.v;
    g_fResourceGeometryChildVtablesInitialized = 1;
}

void LEMBALL_FASTCALL DestroyResourceGeometryChild(void *pChildOwner) {
    typedef void (LEMBALL_FASTCALL *DeleteProc)(
        void *pObject, int nUnusedEdx, unsigned int fDelete);
    char *pObject;
    void *pOwnedEntry;

    pObject = (char *)pChildOwner - 0x90;
    *(void ***)pObject = g_ResourceGeometryChildVtable;
    *(void ***)pChildOwner = g_ResourceGeometryChildDeleteVtable;
    if (*(int *)(*(int *)(pObject + 0xc8) + 4) == 1) {
        DetachGeometryChildAndDestroyWindowOwner(pObject);
    }
    pOwnedEntry = *(void **)(pObject + 0xe0);
    if (pOwnedEntry != 0) {
        ((DeleteProc)(*(void ***)pOwnedEntry)[0])(
            pOwnedEntry, 0, 1);
    }
    RestorePackagedSpriteRenderEntryVtableC(pObject + 0xe4);
    *(void ***)pObject = g_pWindowOwnerRenderContextVtable;
    DestroyWindowOwnerRenderContext(pObject);
}

void *GameResourceGeometryChildDeleteView::Delete(unsigned int fDelete) {
    char *pObject;

    pObject = (char *)this - 0x90;
    DestroyResourceGeometryChild(this);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

void *LEMBALL_FASTCALL ConstructResourceGeometryChildBase(
    void *pObject, int nUnusedEdx, void *pRenderOwner) {
    void **pWindowOwnerVtable;
    void **pGeometryOwnerVtable;

    (void)nUnusedEdx;
    ConstructWindowOwnerRenderContext(pObject);
    pWindowOwnerVtable = *(void ***)pObject;
    InitializeEmptyGeometryOwnerRect((char *)pObject + 0x90);
    pGeometryOwnerVtable = *(void ***)((char *)pObject + 0x90);
    InitializeResourceGeometryChildVtables(
        pWindowOwnerVtable, pGeometryOwnerVtable);
    *(short *)((char *)pObject + 0xdc) = 0;
    *(short *)((char *)pObject + 0xde) = 0;
    InitializeHelperLocalRectUpdateEntry((char *)pObject + 0xe4);
    *(short *)((char *)pObject + 0x100) = 0;
    *(short *)((char *)pObject + 0x102) = 0;
    *(void ***)pObject = g_ResourceGeometryChildVtable;
    *(void ***)((char *)pObject + 0x90) =
        g_ResourceGeometryChildDeleteVtable;
    *(void **)((char *)pObject + 0xc8) = pRenderOwner;
    InitializeSharedSpriteOwnerState(pObject);
    return pObject;
}

void GameAlignedZrleChildOverlayView::ReleaseFrameResource(void) {
    void *pResource;

    pResource = *(void **)((char *)this + 0x11c);
    if (pResource != 0) {
        ReleaseTypedResourceObjectReference(pResource);
        *(void **)((char *)this + 0x11c) = 0;
    }
}

void GameAlignedZrleChildOverlayView::QueueAlignedFrame(void) {
    typedef void (LEMBALL_FASTCALL *RefreshProc)(
        void *pObject, int nUnusedEdx);
    typedef void (LEMBALL_FASTCALL *QueueProc)(
        void *pObject, int nUnusedEdx, void *pQueueOwner);
    char *pObject;
    char *pFrameSink;
    void *pGeometryContext;
    void *pQueueOwner;
    short x;
    short y;
    int fResourceReady;

    pObject = (char *)this;
    x = *(short *)(pObject + 0x118);
    y = *(short *)(pObject + 0x11a);
    fResourceReady = *(int *)(pObject + 0x104) != 0 &&
                     *(int *)(pObject + 0x94) != 0;
    if ((*(unsigned int *)(pObject + 0x110) & 0x40) != 0 &&
        fResourceReady) {
        ++x;
        ++y;
    }

    pQueueOwner = *(void **)(pObject + 0x4c);
    pGeometryContext = *(void **)((char *)pQueueOwner + 0x0c);
    pGeometryContext = (char *)pGeometryContext +
        *(int *)(*(int *)((char *)pGeometryContext + 0x40) + 4) + 0x40;
    ((RefreshProc)(*(void ***)pGeometryContext)[14])(
        pGeometryContext, 0);

    pFrameSink = *(char **)(pObject + 0x124);
    *(short *)(pFrameSink + 4) = x;
    *(short *)(pFrameSink + 6) = y;
    *(void **)(pFrameSink + 0x1c) = *(void **)(pObject + 0x11c);
    *(int *)(pFrameSink + 0x18) = *(int *)(pObject + 0x104);
    *(int *)(pFrameSink + 0x0c) = 0;
    *(void **)(pFrameSink + 0x10) = *(void **)(pObject + 0x120);
    ((QueueProc)(*(void ***)pFrameSink)[1])(
        pFrameSink, 0, pQueueOwner);
}

static void SetAlignedZrleChildOverlayFrame(void *pObject, int iFrame) {
    if (*(int *)((char *)pObject + 0x104) != iFrame) {
        *(int *)((char *)pObject + 0x104) = iFrame;
        *(int *)((char *)pObject + 0xd8) = 1;
    }
}

void GameAlignedZrleChildOverlayView::HandleButtonRelease(int) {
    SetAlignedZrleChildOverlayFrame(this, 1);
}

void GameAlignedZrleChildOverlayView::HandleButtonPress(int) {
    SetAlignedZrleChildOverlayFrame(this, 2);
}

void GameAlignedZrleChildOverlayView::HandlePointerEnter(void) {
    SetAlignedZrleChildOverlayFrame(this, 1);
}

void GameAlignedZrleChildOverlayView::HandlePointerLeave(void) {
    SetAlignedZrleChildOverlayFrame(this, 0);
}

static void InitializeAlignedZrleChildOverlayVtables(void) {
    union VoidMethod {
        void (GameAlignedZrleChildOverlayView::*p)(void);
        void *v;
    } Void;
    union DeleteMethod {
        void *(GameAlignedZrleChildOverlayDeleteView::*p)(unsigned int);
        void *v;
    } Delete;
    union IntMethod {
        void (GameAlignedZrleChildOverlayView::*p)(int);
        void *v;
    } Int;
    int i;

    if (g_fAlignedZrleChildOverlayVtablesInitialized != 0) {
        return;
    }
    for (i = 0; i < 52; ++i) {
        g_AlignedZrleChildOverlayVtable[i] =
            g_ResourceGeometryChildVtable[i];
    }
    for (i = 0; i < 8; ++i) {
        g_AlignedZrleChildOverlayDeleteVtable[i] =
            g_ResourceGeometryChildDeleteVtable[i];
    }
    Void.p = &GameAlignedZrleChildOverlayView::ReleaseFrameResource;
    g_AlignedZrleChildOverlayVtable[16] = Void.v;
    Void.p = &GameAlignedZrleChildOverlayView::QueueAlignedFrame;
    g_AlignedZrleChildOverlayVtable[47] = Void.v;
    Int.p = &GameAlignedZrleChildOverlayView::HandleButtonRelease;
    g_AlignedZrleChildOverlayVtable[48] = Int.v;
    Int.p = &GameAlignedZrleChildOverlayView::HandleButtonPress;
    g_AlignedZrleChildOverlayVtable[49] = Int.v;
    Void.p = &GameAlignedZrleChildOverlayView::HandlePointerEnter;
    g_AlignedZrleChildOverlayVtable[50] = Void.v;
    Void.p = &GameAlignedZrleChildOverlayView::HandlePointerLeave;
    g_AlignedZrleChildOverlayVtable[51] = Void.v;
    g_AlignedZrleChildOverlayDeleteVtable[1] =
        (void *)HandleSpriteOwnerButtonTransition;
    g_AlignedZrleChildOverlayDeleteVtable[2] =
        (void *)HandleSpriteOwnerButtonInside;
    g_AlignedZrleChildOverlayDeleteVtable[3] =
        (void *)ResetSpriteOwnerButtonState;
    g_AlignedZrleChildOverlayDeleteVtable[4] =
        (void *)HandleSpriteOwnerPointerEnter;
    g_AlignedZrleChildOverlayDeleteVtable[5] =
        (void *)HandleSpriteOwnerPointerLeave;
    Delete.p = &GameAlignedZrleChildOverlayDeleteView::Delete;
    g_AlignedZrleChildOverlayDeleteVtable[0] = Delete.v;
    g_fAlignedZrleChildOverlayVtablesInitialized = 1;
}

void LEMBALL_FASTCALL DestroyAlignedZrleChildOverlay(void *pChildOwner) {
    typedef void (LEMBALL_FASTCALL *DeleteProc)(
        void *pObject, int nUnusedEdx, unsigned int fDelete);
    char *pObject;
    void *pFrameSink;

    pObject = (char *)pChildOwner - 0x90;
    *(void ***)pChildOwner = g_AlignedZrleChildOverlayDeleteVtable;
    *(void ***)pObject = g_AlignedZrleChildOverlayVtable;
    if (*(int *)(pObject + 4) == 1) {
        DetachGeometryChildAndDestroyWindowOwner(pObject);
    }
    pFrameSink = *(void **)(pObject + 0x124);
    if (pFrameSink != 0) {
        ((DeleteProc)(*(void ***)pFrameSink)[0])(pFrameSink, 0, 3);
    }
    RestoreQueuedRenderPointSinkVtable(pObject + 0x128);
    DestroyResourceGeometryChild(pChildOwner);
}

void *GameAlignedZrleChildOverlayDeleteView::Delete(unsigned int fDelete) {
    char *pObject;

    pObject = (char *)this - 0x90;
    DestroyAlignedZrleChildOverlay(this);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

void *LEMBALL_FASTCALL ConstructAlignedZrleChildOverlayFromPoint(
    void *pObject, int nUnusedEdx, const VS_XYPair *pPoint,
    void *pRenderOwner, int nResourceId, int fAlignment) {
    typedef void (LEMBALL_FASTCALL *InitializeRectProc)(
        void *pObject, int nUnusedEdx, short *pRect,
        void *pOwner, const char *pszName);
    short aRect[4];

    (void)nUnusedEdx;
    ConstructResourceGeometryChildBase(pObject, 0, pRenderOwner);
    *(short *)((char *)pObject + 0x114) = 0;
    *(short *)((char *)pObject + 0x116) = 0;
    *(int *)((char *)pObject + 0x104) = 0;
    *(int *)((char *)pObject + 0x108) = 0;
    *(short *)((char *)pObject + 0x118) = 0;
    *(short *)((char *)pObject + 0x11a) = 0;
    InitializeTinyPointSinkHelper((char *)pObject + 0x128);
    InitializeAlignedZrleChildOverlayVtables();
    *(void ***)pObject = g_AlignedZrleChildOverlayVtable;
    *(void ***)((char *)pObject + 0x90) =
        g_AlignedZrleChildOverlayDeleteVtable;
    *(int *)((char *)pObject + 0x10c) = nResourceId;
    *(int *)((char *)pObject + 0x110) = fAlignment;
    ComputeAlignedZrleChildLayout(pObject);

    *(short *)((char *)pObject + 0xdc) = pPoint->m_x;
    *(short *)((char *)pObject + 0xde) = pPoint->m_y;
    aRect[0] = *(short *)((char *)pObject + 0xc0);
    aRect[1] = *(short *)((char *)pObject + 0xc2);
    aRect[2] = pPoint->m_x;
    aRect[3] = pPoint->m_y;
    ((InitializeRectProc)(*(void ***)pObject)[1])(
        pObject, 0, aRect, pRenderOwner, "Button");
    *(short *)((char *)pObject + 0xc4) +=
        *(short *)((char *)pObject + 0x18);
    *(short *)((char *)pObject + 0xc6) +=
        *(short *)((char *)pObject + 0x1a);
    AppendGeometryOwnerChild(
        *(void **)((char *)pRenderOwner + 0x1c), 0,
        (char *)pObject + 0x90);
    return pObject;
}

void GameMenuSelectorChildContextView::MarkRenderObjectDirty(void) {
    int nContext;
    int nRenderObject;

    nContext = *(int *)(*(int *)((char *)this + 0x4c) + 0x0c);
    nRenderObject = nContext +
        *(int *)(*(int *)(nContext + 0x40) + 4) + 0x74;
    *(int *)nRenderObject = 1;
}

static void *ConstructMenuSelectorChildContextObject(void *pObject) {
    union VoidMethod {
        void (GameMenuSelectorChildContextView::*p)(void);
        void *v;
    } Void;
    void **pBaseVtable;
    int i;

    ConstructWindowOwnerRenderContext(pObject);
    pBaseVtable = *(void ***)pObject;
    if (g_fMenuSelectorChildContextVtableInitialized == 0) {
        for (i = 0; i < 46; ++i) {
            g_MenuSelectorChildContextVtable[i] = pBaseVtable[i];
        }
        Void.p = &GameMenuSelectorChildContextView::MarkRenderObjectDirty;
        g_MenuSelectorChildContextVtable[15] = Void.v;
        g_fMenuSelectorChildContextVtableInitialized = 1;
    }
    *(void ***)pObject = g_MenuSelectorChildContextVtable;
    *(int *)((char *)pObject + 0x50) += 5;
    return pObject;
}

void LEMBALL_FASTCALL InitializeMenuSelectorChildContext(
    GameMenuSelectorManager *pManager) {
    typedef void (LEMBALL_FASTCALL *InitializeRectProc)(
        void *pObject, int nUnusedEdx, short *pRect,
        void *pOwner, int nFlags);
    short Rect[4];
    void *pContextObject;

    pContextObject = AllocateVSMemBlock(0x90);
    if (pContextObject != 0) {
        ConstructMenuSelectorChildContextObject(pContextObject);
    }
    pManager->m_pChildContext268 = pContextObject;
    Rect[0] = *(short *)((char *)pManager->m_pRenderOwner214 + 8);
    Rect[1] = *(short *)((char *)pManager->m_pRenderOwner214 + 10);
    Rect[2] = 0;
    Rect[3] = 0;
    ((InitializeRectProc)(*(void ***)pContextObject)[1])(
        pContextObject, 0, Rect, pManager->m_pRenderOwner214, 0);
    pManager->m_pChildRenderContext26C =
        *(void **)((char *)pContextObject + 0x4c);
}

void LEMBALL_FASTCALL InitializeMenuSelectorChildContextThunk(
    GameMenuSelectorManager *pManager) {
    InitializeMenuSelectorChildContext(pManager);
}

void LEMBALL_FASTCALL ComputeAlignedZrleChildLayout(void *pObject) {
    typedef void (LEMBALL_FASTCALL *LoadProc)(void *pResource);
    GameMenuActionFramePointSink *pFrameSink;
    int *pListResource;
    char *pFrame;
    short cxFrame;
    short cyFrame;
    short cxOwner;
    short cyOwner;
    short nAlignment;

    *(int *)((char *)pObject + 0x120) = 0;
    pFrameSink = (GameMenuActionFramePointSink *)
        AllocateVSMemBlock(sizeof(int) +
                           sizeof(GameMenuActionFramePointSink));
    if (pFrameSink != 0) {
        *(int *)pFrameSink = 1;
        pFrameSink = (GameMenuActionFramePointSink *)
            ((char *)pFrameSink + sizeof(int));
        InitializeMenuActionFramePointSink(pFrameSink);
    }
    *(GameMenuActionFramePointSink **)((char *)pObject + 0x124) =
        pFrameSink;
    ++*(int *)((char *)pObject + 0x50);

    pListResource = (int *)LoadZrleOnlyListResource(
        *(int *)((char *)pObject + 0x10c));
    *(int **)((char *)pObject + 0x11c) = pListResource;
    if (pListResource[4] == 0) {
        ((LoadProc)(*(void ***)pListResource)[7])(pListResource);
    } else {
        pListResource[9] = 0;
    }
    ++pListResource[2];

    pFrame = (char *)pListResource[0x1e];
    cxFrame = *(short *)(pFrame + 0x4c);
    cyFrame = *(short *)(pFrame + 0x4e);
    if (cxFrame < *(short *)(pFrame + 0xa0)) {
        cxFrame = *(short *)(pFrame + 0xa0);
    }
    if (cyFrame < *(short *)(pFrame + 0xa2)) {
        cyFrame = *(short *)(pFrame + 0xa2);
    }
    *(short *)((char *)pObject + 0x114) = cxFrame;
    *(short *)((char *)pObject + 0x116) = cyFrame;
    --pListResource[2];

    *(short *)((char *)pObject + 0x118) =
        *(short *)((char *)pObject + 0xc4);
    *(short *)((char *)pObject + 0x11a) =
        *(short *)((char *)pObject + 0xc6);
    cxOwner = *(short *)((char *)pObject + 0xc0);
    cyOwner = *(short *)((char *)pObject + 0xc2);
    if (cxOwner < 0) {
        cxOwner = (short)(-cxFrame * cxOwner);
    } else if (cxOwner == 0) {
        cxOwner = cxFrame;
    }
    if (cyOwner < 0) {
        cyOwner = (short)(-cyFrame * cyOwner);
    } else if (cyOwner == 0) {
        cyOwner = cyFrame;
    }
    *(short *)((char *)pObject + 0xc0) = cxOwner;
    *(short *)((char *)pObject + 0xc2) = cyOwner;
    if ((int)cxOwner * (int)cyOwner != 0) {
        ((GameGeometryOwnerRectState *)((char *)pObject + 0x90))
            ->SetEnabled(1);
    }

    nAlignment = *(short *)((char *)pObject + 0x118);
    if ((*(unsigned int *)((char *)pObject + 0x110) & 4) != 0) {
        nAlignment = (short)((cxOwner - cxFrame) / 2);
    } else if ((*(unsigned int *)((char *)pObject + 0x110) & 0x10) != 0) {
        nAlignment = (short)(cxOwner - cxFrame);
    }
    *(short *)((char *)pObject + 0x118) = nAlignment;

    if ((*(unsigned int *)((char *)pObject + 0x110) & 8) != 0) {
        *(short *)((char *)pObject + 0x11a) =
            (short)((cyOwner - cyFrame) / 2);
    } else if ((*(unsigned int *)((char *)pObject + 0x110) & 0x20) != 0) {
        *(short *)((char *)pObject + 0x11a) =
            (short)(cyOwner - cyFrame);
    }
}

static inline GameProgressiveVariantFrameSelector *AllocateFrameSelector(
    void *pVariantManager, int nResourceId) {
    GameProgressiveVariantFrameSelector *pSelector;

    pSelector = (GameProgressiveVariantFrameSelector *)
        AllocateVSMemBlock(0x1c);
    if (pSelector != 0) {
        pSelector->m_pVtable = g_MenuSelectorFrameSelectorCtorVtable;
        pSelector->m_cFrames =
            ((LevelVariantRenderManagerView *)pVariantManager)
                ->GetVariantRenderFrameCount(nResourceId);
        pSelector->m_pVtable = g_MenuSelectorTimedFrameSelectorVtable;
        pSelector->m_nDirection = 1;
        pSelector->m_pVtable = g_MenuSelectorProgressiveFrameSelectorVtable;
    }
    return pSelector;
}

/* FUNCTION: LEMBALL 0x0044C870 */
GameMenuSelectorManager *LEMBALL_FASTCALL ConstructMenuSelectorManager(
    GameMenuSelectorManager *pManager, int, void *pRenderOwner,
    void *pRenderContext, int, int nLayoutMode) {
    int i;
    void *pVariantManager;

    InitializeRenderQueueNodeBase(pManager);
    pVariantManager = (char *)pManager + 0x10;
    ConstructVariantRenderManager(pVariantManager,
                                  (int)(unsigned long)pRenderContext,
                                  0x2b6, 10, 5, 0, 0);
    InitializeHelperLocalRectUpdateEntry(pManager->m_abLocalRectEntry200);
    pManager->m_pVtable00 = g_MenuSelectorManagerVtable;
    pManager->m_cCurrentFrameCallbackFrames25C = 1;
    *(void ***)pVariantManager = g_MenuSelectorVariantRenderManagerVtable;
    pManager->m_fChildrenEnabled278 = 1;
    pManager->m_nLayoutMode270 = nLayoutMode;
    pManager->m_pRenderOwner214 = pRenderOwner;
    pManager->m_pRenderContext210 = pRenderContext;
    pManager->m_pCurrentFrameCallbackVtable258 =
        g_MenuSelectorCurrentFrameCallbackVtable;
    pManager->m_nCurrentFrameCallbackFrame264 = 0;
    pManager->m_nNextToken218 = (int)0xabcd0000;
    for (i = 0; i < 8; ++i) {
        pManager->m_aMetadataE0[i].m_xSide0 = -1;
        pManager->m_aMetadataE0[i].m_nPosition = -1;
        pManager->m_aMetadataE0[i].m_nSide = 3;
        pManager->m_apChildren1E0[i] = 0;
    }
    pManager->m_nReserved84 = 0;
    pManager->m_cChildren80 = 0;
    pManager->m_fMovementActiveDC = 0;
    pManager->m_nActionStateA4 = 0;
    pManager->m_fEventQueuedB4 = 0;
    pManager->m_pChildContext268 = 0;
    pManager->m_pChildRenderContext26C = 0;
    ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(pManager, 0);

    if (nLayoutMode == 1) {
        pManager->m_fCompactLayout274 = 1;
        g_nMenuSelectorResourceId0 = 0x173;
        g_nMenuSelectorResourceId1 = 0x174;
        g_nMenuSelectorResourceId2 = 0x175;
        g_nMenuSelectorResourceId3 = 0x176;
        g_nMenuSelectorResourceId4 = 0x177;
        g_nMenuSelectorResourceId5 = 0x178;
        g_nMenuSelectorResourceId6 = 0x179;
        g_nMenuSelectorResourceId7 = 0x17a;
        g_nMenuSelectorResourceId8 = 0x17b;
    } else {
        pManager->m_fCompactLayout274 = 0;
        g_nMenuSelectorResourceId0 = 0x146;
        g_nMenuSelectorResourceId1 = 0x147;
        g_nMenuSelectorResourceId2 = 0x148;
        g_nMenuSelectorResourceId3 = 0x149;
        g_nMenuSelectorResourceId4 = 0x14a;
        g_nMenuSelectorResourceId5 = 0x14b;
        g_nMenuSelectorResourceId6 = 0x14c;
        g_nMenuSelectorResourceId7 = 0x14d;
        g_nMenuSelectorResourceId8 = 0x14e;
    }

    LoadVariantRenderResource(pVariantManager, g_nMenuSelectorResourceId0);
    LoadVariantRenderResource(pVariantManager, g_nMenuSelectorResourceId1);
    LoadVariantRenderResource(pVariantManager, g_nMenuSelectorResourceId2);
    LoadVariantRenderResource(pVariantManager, g_nMenuSelectorResourceId3);
    LoadVariantRenderResource(pVariantManager, g_nMenuSelectorResourceId4);
    LoadVariantRenderResource(pVariantManager, g_nMenuSelectorResourceId5);
    LoadVariantRenderResource(pVariantManager, g_nMenuSelectorResourceId6);
    LoadVariantRenderResource(pVariantManager, g_nMenuSelectorResourceId7);
    LoadVariantRenderResource(pVariantManager, g_nMenuSelectorResourceId8);

    pManager->m_pSideSelector244 =
        AllocateFrameSelector(pVariantManager, g_nMenuSelectorResourceId6);
    pManager->m_pSideSelector244->m_dwFixedTime = -1;
    pManager->m_pActionTailSelector248 =
        AllocateFrameSelector(pVariantManager, g_nMenuSelectorResourceId4);
    pManager->m_pActionTailSelector248->m_dwFixedTime = -1;
    pManager->m_pActionLeadSelector24C =
        AllocateFrameSelector(pVariantManager, g_nMenuSelectorResourceId0);
    pManager->m_pActionLeadSelector24C->m_dwFixedTime = -1;
    pManager->m_pCompletionSelector250 =
        AllocateFrameSelector(pVariantManager, g_nMenuSelectorResourceId7);
    pManager->m_pCompletionSelector250->m_dwFixedTime = -1;
    pManager->m_pActionMiddleSelector254 =
        AllocateFrameSelector(pVariantManager, g_nMenuSelectorResourceId2);
    pManager->m_pActionMiddleSelector254->m_dwFixedTime = -1;
    pManager->m_dwConstructionTimeD4 =
        ((RenderDispatchQueue *)g_pSharedRenderDispatchQueue)->m_nReserved30;
    return pManager;
}

/* FUNCTION: LEMBALL 0x00402432 */
GameMenuSelectorManager *LEMBALL_FASTCALL ConstructMenuSelectorManagerThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, void *pRenderOwner,
    void *pRenderContext, int nChildCapacity, int nLayoutMode) {
    return ConstructMenuSelectorManager(pManager, nUnusedEdx, pRenderOwner,
                                        pRenderContext, nChildCapacity,
                                        nLayoutMode);
}

/* FUNCTION: LEMBALL 0x0044D2D0 */
void LEMBALL_FASTCALL EmitMenuSelectorResourceEntries(GameMenuSelectorManager *pManager) {
    int nContext;
    int nRenderObject;
    int nOwner;
    int i;
    short Point[2];
    short Rect[4];
    const int *pOffsets;

    for (i = 0; i < pManager->m_cChildren80; ++i) {
        GameMenuSelectorEntry *pEntry;

        pEntry = pManager->m_apChildren1E0[i];
        if (pEntry != 0 && pEntry->m_fEnabled5C != 0 &&
            pEntry->m_pAlignedOverlay50 != 0) {
            void *pOverlay;
            char *pFrameSink;

            pOverlay = pEntry->m_pAlignedOverlay50;
            pFrameSink = *(char **)((char *)pOverlay + 0x124);
            *(short *)(pFrameSink + 4) = (short)pEntry->m_x1C;
            *(short *)(pFrameSink + 6) = (short)pEntry->m_nPosition20;
            *(void **)(pFrameSink + 0x1c) =
                *(void **)((char *)pOverlay + 0x11c);
            *(int *)(pFrameSink + 0x18) = 0;
            *(int *)(pFrameSink + 0x0c) = 0;
            *(int *)(pFrameSink + 0x10) = 0;
            ((void (LEMBALL_FASTCALL *)(void *, int, void *))
                (*(void ***)pFrameSink)[1])(
                    pFrameSink, 0, pManager->m_pRenderContext210);
        }
    }

    nContext = *(int *)((char *)pManager->m_pChildRenderContext26C + 0x0c);
    nRenderObject = *(int *)(*(int *)(nContext + 0x40) + 4) +
                    0x40 + nContext;
    ((void (LEMBALL_FASTCALL *)(void *, int))(*(void ***)nRenderObject)[14])(
        (void *)nRenderObject, 0);
    Point[0] = *(short *)(nRenderObject + 0x14);
    Point[1] = *(short *)(nRenderObject + 0x16);
    *(short *)(pManager->m_abLocalRectEntry200 + 4) = Point[0];
    *(short *)(pManager->m_abLocalRectEntry200 + 6) = Point[1];
    *(short *)(pManager->m_abLocalRectEntry200 + 8) = 0;
    *(short *)(pManager->m_abLocalRectEntry200 + 10) = 0;
    *(int *)(pManager->m_abLocalRectEntry200 + 12) = 0x10000;
    AppendPointerQueueEntry(pManager->m_pChildRenderContext26C,
                            pManager->m_abLocalRectEntry200);

    pOffsets = pManager->m_fCompactLayout274 != 1
        ? g_anMenuSelectorActionOffsetsMode0
        : g_anMenuSelectorActionOffsetsMode1;
    if (pManager->m_nActionStateA4 == 0) {
        pManager->m_nCurrentFrameCallbackFrame264 =
            pManager->m_nCurrentSide94 == 0 ? 0 :
            ((LevelVariantRenderManagerView *)
                pManager->m_abVariantRenderManager10)
                ->GetVariantRenderFrameCount(g_nMenuSelectorResourceId6) - 1;
        Point[0] = (short)(pManager->m_xCenteredAction88 + pOffsets[0]);
        Point[1] = (short)(pManager->m_nCurrentPosition8C + pOffsets[1]);
        nOwner = *(int *)(pManager->m_abVariantRenderManager10 + 0x1c);
        *(void **)(pManager->m_abVariantRenderManager10 + 0x1c) =
            pManager->m_pChildRenderContext26C;
        EmitVariantRenderResourceEntry(
            pManager->m_abVariantRenderManager10, Rect, Point,
            g_nMenuSelectorResourceId6, 0, 0, 0);
        *(int *)(pManager->m_abVariantRenderManager10 + 0x1c) = nOwner;
    }
    ResetOrFlipVariantRenderBuffers(
        (int)(unsigned long)pManager->m_abVariantRenderManager10);
}

void LEMBALL_FASTCALL InitializeSharedSpriteOwnerState(void *pObject) {
    void **ppFinalize;

    *(int *)((char *)pObject + 0xd8) = 1;
    *(int *)((char *)pObject + 0xf8) = 1;
    *(int *)((char *)pObject + 0xbc) = 1;
    *(int *)((char *)pObject + 0xd0) = 0;
    *(int *)((char *)pObject + 0xd4) = 0;
    *(int *)((char *)pObject + 0xfc) = 0;
    ppFinalize = (void **)AllocateVSMemBlock(sizeof(void *));
    if (ppFinalize != 0) {
        *ppFinalize = g_pQueuedRenderPointSinkFinalizeThunk;
    }
    *(void **)((char *)pObject + 0xe0) = ppFinalize;
    *(int *)((char *)pObject + 0x50) = 2;
    *(int *)((char *)pObject + 0xf4) = 0;
    *(int *)((char *)pObject + 0xcc) = 0;
}

/* FUNCTION: LEMBALL 0x0044D150 */
static void LEMBALL_FASTCALL RecordMenuSelectorChildMetadata(
    GameMenuSelectorManager *pManager, int, int x,
    int nPosition, int fAction, int nToken) {
    GameMenuSelectorMetadata *pMetadata;
    int i;
    int iMetadata;
    int nSide;

    nSide = x < *(short *)((char *)pManager->m_pRenderOwner214 + 8) / 2
                ? 0 : 1;
    iMetadata = -1;
    for (i = 0; i < 8; ++i) {
        pMetadata = &pManager->m_aMetadataE0[i];
        if (pMetadata->m_nSide != 3 &&
            pMetadata->m_nPosition == nPosition) {
            iMetadata = i;
        }
    }

    if (iMetadata == -1) {
        for (i = 0; i < 8; ++i) {
            if (pManager->m_aMetadataE0[i].m_nSide == 3) {
                iMetadata = i;
                pManager->m_aMetadataE0[i].m_nPosition = nPosition;
                pManager->m_aMetadataE0[i].m_nSide = nSide;
                break;
            }
        }
    } else {
        pManager->m_aMetadataE0[iMetadata].m_nSide = 2;
    }

    if (iMetadata == -1) {
        return;
    }
    pMetadata = &pManager->m_aMetadataE0[iMetadata];
    if (nSide == 0) {
        pMetadata->m_xSide0 = x;
        pMetadata->m_fSide0Action = fAction;
        pMetadata->m_nSide0Token = nToken;
    } else {
        pMetadata->m_xSide1 = x;
        pMetadata->m_fSide1Action = fAction;
        pMetadata->m_nSide1Token = nToken;
    }
}

static void LEMBALL_FASTCALL BuildMenuSelectorEntryOverlay(
    GameMenuSelectorEntry *pEntry, int,
    const int *panResourceIds) {
    void *pOverlay;
    int nContext;
    int nRenderObject;
    int i;
    VS_XYPair Point;

    pEntry->m_panResourceIds4C = panResourceIds;
    pEntry->m_ppLoadedResources58 = (void **)AllocateVSMemBlock(
        pEntry->m_cResources38 * sizeof(void *));
    for (i = 0; i < pEntry->m_cResources38; ++i) {
        pEntry->m_ppLoadedResources58[i] =
            LoadZrleOnlyListResource(panResourceIds[i]);
    }

    if (pEntry->m_nOverlayKind10 != 0) {
        pEntry->m_pAlignedOverlay50 = 0;
        pEntry->m_pProgressOverlay54 = 0;
        return;
    }

    pOverlay = AllocateVSMemBlock(0x130);
    if (pOverlay != 0) {
        Point.m_x = (short)pEntry->m_x1C;
        Point.m_y = (short)pEntry->m_nPosition20;
        ConstructAlignedZrleChildOverlayFromPoint(
            pOverlay, 0, &Point, pEntry->m_pRenderOwner28,
            panResourceIds[pEntry->m_nSelectedResource44 -
                           pEntry->m_nFirstResource40],
            3);
    }
    pEntry->m_pAlignedOverlay50 = pOverlay;
    pEntry->m_pProgressOverlay54 = 0;
    if (pOverlay == 0) {
        return;
    }

    *(int *)((char *)pOverlay + 0xf8) = 0;
    nContext = *(int *)(*(int *)((char *)pOverlay + 0x4c) + 0x0c);
    nRenderObject = nContext +
        *(int *)(*(int *)(nContext + 0x40) + 4) + 0x70;
    *(int *)nRenderObject = 0;
    *(void **)((char *)pOverlay + 0xf4) =
        g_pSharedRenderDispatchQueue;
    *(int *)((char *)pOverlay + 0xcc) = pEntry->m_nToken30;
}

static GameMenuSelectorEntry *LEMBALL_FASTCALL ConstructMenuSelectorEntry(
    GameMenuSelectorEntry *pEntry, int, void *pRenderOwner,
    void *pRenderContext, int x, int nPosition,
    const int *panResourceIds, int nActivationMode,
    int nFirstResource, int nLastResource, int nReserved,
    int nToken, int *pSelection, int nCommand) {
    int cResources;

    InitializeRenderQueueNodeBase(pEntry);
    pEntry->m_pVtable00 = g_MenuSelectorEntryVtable;
    pEntry->m_nOverlayKind10 = 0;
    pEntry->m_nProgressX14 = 0;
    pEntry->m_nProgressY16 = 0;
    pEntry->m_nProgressWidth18 = 0;
    pEntry->m_nProgressHeight1A = 0;
    pEntry->m_pRenderOwner28 = pRenderOwner;
    pEntry->m_pRenderContext24 = pRenderContext;
    pEntry->m_fEnabled5C = 1;
    cResources = nLastResource - nFirstResource + 1;
    pEntry->m_cResources38 = cResources < 2 ? 1 : cResources;
    pEntry->m_nFirstResource40 = nFirstResource;
    pEntry->m_nLastResource3C = nLastResource;
    pEntry->m_nToken30 = nToken;
    pEntry->m_x1C = x;
    pEntry->m_nPosition20 = nPosition;
    pEntry->m_nSelectedResource44 =
        pSelection == 0 ? 0 : *pSelection;
    if (nCommand != -1) {
        pEntry->m_nCommand34 = nCommand;
    }
    pEntry->m_pSelection48 = pSelection;
    pEntry->m_nActivationMode2C = nActivationMode;
    (void)nReserved;
    ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(pEntry, 0);
    BuildMenuSelectorEntryOverlay(pEntry, 0, panResourceIds);
    return pEntry;
}

void LEMBALL_FASTCALL AppendMenuSelectorChild(
    GameMenuSelectorManager *pManager, int, int x,
    int nPosition, const int *panResourceIds, int nActivationMode,
    int nFirstResource, int nLastResource, int nReserved,
    int *pSelection, int nCommand) {
    GameMenuSelectorEntry *pEntry;
    int nToken;
    int iChild;

    nToken = ++pManager->m_nNextToken218;
    iChild = pManager->m_cChildren80;
    pEntry = (GameMenuSelectorEntry *)
        AllocateVSMemBlock(sizeof(GameMenuSelectorEntry));
    if (pEntry != 0) {
        pEntry = ConstructMenuSelectorEntry(
            pEntry, 0, pManager->m_pRenderOwner214,
            pManager->m_pRenderContext210, x, nPosition,
            panResourceIds, nActivationMode, nFirstResource,
            nLastResource, nReserved, nToken, pSelection,
            nCommand);
    }
    pManager->m_apChildren1E0[iChild] = pEntry;
    if (pEntry != 0) {
        RecordMenuSelectorChildMetadata(
            pManager, 0, x, nPosition, nLastResource > 0,
            pEntry->m_nToken30);
    }
    ++pManager->m_cChildren80;
}

void LEMBALL_FASTCALL AppendMenuSelectorChildThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int x,
    int nPosition, const int *panResourceIds, int nActivationMode,
    int nFirstResource, int nLastResource, int nReserved,
    int *pSelection, int nCommand) {
    AppendMenuSelectorChild(
        pManager, nUnusedEdx, x, nPosition, panResourceIds,
        nActivationMode, nFirstResource, nLastResource, nReserved,
        pSelection, nCommand);
}

/* FUNCTION: LEMBALL 0x004688E0 */
static void LEMBALL_FASTCALL ReloadMenuSelectorOverlayResource(
    void *pOverlay, int, int nResourceId) {
    void *pResource;

    pResource = *(void **)((char *)pOverlay + 0x11c);
    if (pResource != 0) {
        ReleaseTypedResourceObjectReference(pResource);
    }
    *(int *)((char *)pOverlay + 0x10c) = nResourceId;
    *(void **)((char *)pOverlay + 0x11c) =
        LoadZrleOnlyListResource(nResourceId);
    *(int *)((char *)pOverlay + 0xd8) = 1;
}

/* FUNCTION: LEMBALL 0x0044C7C0 */
static void LEMBALL_FASTCALL ReleaseMenuSelectorEntryResources(
    GameMenuSelectorEntry *pEntry) {
    typedef void (LEMBALL_FASTCALL *DeleteOverlayProc)(
        void *pOverlayDeleteInterface, int nUnusedEdx, int fDelete);
    void *pDeleteInterface;
    int i;

    if (pEntry->m_pAlignedOverlay50 != 0) {
        pDeleteInterface = (char *)pEntry->m_pAlignedOverlay50 + 0x90;
        ((DeleteOverlayProc)(*(void ***)pDeleteInterface)[0])(
            pDeleteInterface, 0, 1);
    }
    if (pEntry->m_pProgressOverlay54 != 0) {
        pDeleteInterface = (char *)pEntry->m_pProgressOverlay54 + 0x90;
        ((DeleteOverlayProc)(*(void ***)pDeleteInterface)[0])(
            pDeleteInterface, 0, 1);
    }
    for (i = 0; i < pEntry->m_cResources38; ++i) {
        ReleaseTypedResourceObjectReference(pEntry->m_ppLoadedResources58[i]);
    }
    FreeVSMemBlock(pEntry->m_ppLoadedResources58);
    pEntry->m_ppLoadedResources58 = 0;
}

/* FUNCTION: LEMBALL 0x0044EB60 */
static void LEMBALL_FASTCALL SetMenuSelectorProgressPercent(
    void *pProgress, int, int nPercent) {
    typedef void (LEMBALL_FASTCALL *QueueProc)(void *, int, void *);
    GameMenuSelectorEvent Event;

    if (*(int *)((char *)pProgress + 0x120) == nPercent) {
        return;
    }
    Event.m_nType00 = 0x0c;
    Event.m_nReserved02 = 0;
    *(int *)((char *)pProgress + 0x120) = nPercent;
    Event.m_dwTimestamp04 = (int)timeGetTime();
    Event.m_nPayload08 = *(int *)((char *)pProgress + 0x130);
    Event.m_nPayload0C = nPercent;
    Event.m_nPayload10 = 100;
    Event.m_nPayload14 = 0;
    Event.m_nPayload18 = 0;
    ((QueueProc)(*(void ***)g_pSharedRenderDispatchQueue)[2])(
        g_pSharedRenderDispatchQueue, 0, &Event);
}

/* FUNCTION: LEMBALL 0x0044C460 */
static int LEMBALL_FASTCALL HandleMenuSelectorEntryEvent(
    GameMenuSelectorEntry *pEntry, int,
    GameMenuSelectorEvent *pEvent) {
    typedef void (LEMBALL_FASTCALL *QueueProc)(void *, int, void *);
    GameMenuSelectorEvent Event;
    void *pOverlay;
    void *pProgress;
    int nPercent;
    int nResourceId;

    if (pEvent->m_nType00 != 0x0c ||
        pEvent->m_nPayload08 != pEntry->m_nToken30) {
        return 0;
    }
    if (pEntry->m_nOverlayKind10 == 0) {
        if (pEntry->m_nActivationMode2C == 1) {
            Event.m_nType00 = 0x0c;
            Event.m_nReserved02 = 0;
            Event.m_dwTimestamp04 = (int)timeGetTime();
            Event.m_nPayload08 = pEntry->m_nCommand34;
            Event.m_nPayload0C = 0;
            Event.m_nPayload10 = 0;
            Event.m_nPayload14 = 0;
            Event.m_nPayload18 = 0;
            ((QueueProc)(*(void ***)g_pSharedRenderDispatchQueue)[2])(
                g_pSharedRenderDispatchQueue, 0, &Event);
            return 0;
        }

        ++pEntry->m_nSelectedResource44;
        if (pEntry->m_nSelectedResource44 > pEntry->m_nLastResource3C) {
            pEntry->m_nSelectedResource44 = pEntry->m_nFirstResource40;
        }
        if (pEntry->m_pSelection48 != 0) {
            if (pEntry->m_cResources38 == 1) {
                *pEntry->m_pSelection48 =
                    *pEntry->m_pSelection48 == 0 ? 1 : 0;
            } else {
                *pEntry->m_pSelection48 = pEntry->m_nSelectedResource44;
            }
        }
        if (pEntry->m_pAlignedOverlay50 != 0) {
            nResourceId = pEntry->m_panResourceIds4C[
                pEntry->m_nSelectedResource44 - pEntry->m_nFirstResource40];
            pOverlay = pEntry->m_pAlignedOverlay50;
        } else {
            nResourceId = pEntry->m_panResourceIds4C[0];
            pOverlay = pEntry->m_pProgressOverlay54;
        }
        ReloadMenuSelectorOverlayResource(pOverlay, 0, nResourceId);
        g_fLevelSelectionChanged = 1;
        return 0;
    }
    if (pEntry->m_nOverlayKind10 != 1) {
        return 0;
    }

    pProgress = *(void **)((char *)pEntry->m_pProgressOverlay54 + 0x134);
    nPercent = *(int *)((char *)pProgress + 0x120);
    if (nPercent == pEntry->m_nLastResource3C) {
        nPercent = 0;
    } else {
        nPercent += (pEntry->m_nLastResource3C +
                     (((unsigned int)pEntry->m_nLastResource3C >> 31) * 3)) / 4;
        if (nPercent > pEntry->m_nLastResource3C) {
            nPercent = pEntry->m_nLastResource3C;
        }
    }
    *pEntry->m_pSelection48 = nPercent;
    SetMenuSelectorProgressPercent(pProgress, 0, nPercent);
    return 0;
}

/* FUNCTION: LEMBALL 0x00401F28 */
static int LEMBALL_FASTCALL HandleMenuSelectorEntryEventThunk(
    GameMenuSelectorEntry *pEntry, int nUnusedEdx,
    GameMenuSelectorEvent *pEvent) {
    return HandleMenuSelectorEntryEvent(pEntry, nUnusedEdx, pEvent);
}

/* FUNCTION: LEMBALL 0x0044C410 */
static void LEMBALL_FASTCALL DestroyMenuSelectorEntry(
    GameMenuSelectorEntry *pEntry) {
    pEntry->m_pVtable00 = g_MenuSelectorEntryVtable;
    ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->UnregisterOrderedRenderDispatchClient(pEntry, 0);
    ReleaseMenuSelectorEntryResources(pEntry);
    pEntry->m_pVtable00 = g_pNonZrleVariantRenderEntryInitializeVtable;
}

/* FUNCTION: LEMBALL 0x0044E620 */
static void *LEMBALL_FASTCALL DeleteMenuSelectorEntry(
    GameMenuSelectorEntry *pEntry, int, int fDelete) {
    DestroyMenuSelectorEntry(pEntry);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pEntry);
    }
    return pEntry;
}

/* FUNCTION: LEMBALL 0x00401BC2 */
static void *LEMBALL_FASTCALL DeleteMenuSelectorEntryThunk(
    GameMenuSelectorEntry *pEntry, int nUnusedEdx, int fDelete) {
    return DeleteMenuSelectorEntry(pEntry, nUnusedEdx, fDelete);
}

static inline void ReleaseMenuSelectorVariantResource(void *pObject,
                                                      int nResourceId) {
    int *pManager;
    void **ppResources;
    short *pnResourceIds;
    int iResource;

    pManager = (int *)pObject;
    ppResources = (void **)(unsigned long)pManager[9];
    pnResourceIds = (short *)(unsigned long)pManager[10];
    iResource = pnResourceIds[nResourceId];
    ReleaseTypedResourceObjectReference(ppResources[iResource]);
    ppResources[iResource] = 0;
    pnResourceIds[nResourceId] = (short)pManager[11];
    --pManager[13];
}

/* FUNCTION: LEMBALL 0x0044CD60 */
static void LEMBALL_FASTCALL DestroyMenuSelectorManager(
    GameMenuSelectorManager *pManager) {
    int i;
    void *pVariantManager;

    pManager->m_pVtable00 = g_MenuSelectorManagerVtable;
    pVariantManager = (char *)pManager + 0x10;
    *(void ***)pVariantManager = g_MenuSelectorVariantRenderManagerVtable;
    ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->UnregisterOrderedRenderDispatchClient(pManager, 0);
    for (i = 0; i < 8; ++i) {
        if (pManager->m_apChildren1E0[i] != 0) {
            GameMenuSelectorDeleteProc pDelete =
                (GameMenuSelectorDeleteProc)
                    pManager->m_apChildren1E0[i]->m_pVtable00[1];
            pDelete((GameMenuSelectorManager *)
                        pManager->m_apChildren1E0[i], 0, 1);
        }
    }
    ReleaseMenuSelectorVariantResource(pVariantManager,
                                       g_nMenuSelectorResourceId0);
    ReleaseMenuSelectorVariantResource(pVariantManager,
                                       g_nMenuSelectorResourceId1);
    ReleaseMenuSelectorVariantResource(pVariantManager,
                                       g_nMenuSelectorResourceId2);
    ReleaseMenuSelectorVariantResource(pVariantManager,
                                       g_nMenuSelectorResourceId3);
    ReleaseMenuSelectorVariantResource(pVariantManager,
                                       g_nMenuSelectorResourceId4);
    ReleaseMenuSelectorVariantResource(pVariantManager,
                                       g_nMenuSelectorResourceId5);
    ReleaseMenuSelectorVariantResource(pVariantManager,
                                       g_nMenuSelectorResourceId6);
    ReleaseMenuSelectorVariantResource(pVariantManager,
                                       g_nMenuSelectorResourceId7);
    ReleaseMenuSelectorVariantResource(pVariantManager,
                                       g_nMenuSelectorResourceId8);
    FreeVSMemBlock(pManager->m_pSideSelector244);
    FreeVSMemBlock(pManager->m_pActionTailSelector248);
    FreeVSMemBlock(pManager->m_pActionLeadSelector24C);
    FreeVSMemBlock(pManager->m_pCompletionSelector250);
    FreeVSMemBlock(pManager->m_pActionMiddleSelector254);
    if (pManager->m_pChildContext268 != 0) {
        typedef void (LEMBALL_FASTCALL *DetachProc)(
            void *pObject, int nUnusedEdx);
        if (*(int *)((char *)pManager->m_pChildContext268 + 4) == 1) {
            ((DetachProc)(*(void ***)pManager->m_pChildContext268)[29])(
                pManager->m_pChildContext268, 0);
        }
        DestroyWindowOwnerRenderContext(pManager->m_pChildContext268);
        FreeVSMemBlock(pManager->m_pChildContext268);
    }
    RestorePackagedSpriteRenderEntryVtableC(
        pManager->m_abLocalRectEntry200);
    DestroyVariantRenderManager(pVariantManager);
    pManager->m_pVtable00 = g_pNonZrleVariantRenderEntryInitializeVtable;
}

/* FUNCTION: LEMBALL 0x0044E690 */
static void *LEMBALL_FASTCALL DeleteMenuSelectorManager(
    GameMenuSelectorManager *pManager, int, int fDelete) {
    DestroyMenuSelectorManager(pManager);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pManager);
    }
    return pManager;
}

/* FUNCTION: LEMBALL 0x00401C9E */
static void *LEMBALL_FASTCALL DeleteMenuSelectorManagerThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int fDelete) {
    return DeleteMenuSelectorManager(pManager, nUnusedEdx, fDelete);
}

/* FUNCTION: LEMBALL 0x00467570 */
static short *LEMBALL_FASTCALL GetVariantFrameExtent(
    void *pObject, int, short *pExtent, int nResourceId, int iFrame) {
    int *pManager;
    int *pResource;
    short *pFrame;

    pManager = (int *)pObject;
    pResource = *(int **)(unsigned long)(
        pManager[9] +
        (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4);
    if (pResource[0x10] != 0x5a524c45) {
        pFrame = (short *)(unsigned long)(pResource[0x1e] + 0x4c +
                                         iFrame * 0x54);
        pExtent[0] = pFrame[0];
        pExtent[1] = pFrame[1];
    } else {
        pExtent[0] = *(short *)((char *)pResource + 0x4c);
        pExtent[1] = *(short *)((char *)pResource + 0x4e);
    }
    return pExtent;
}

/* FUNCTION: LEMBALL 0x0044DA70 */
void LEMBALL_FASTCALL InitializeMenuSelectorCurrentState(
    GameMenuSelectorManager *pManager, int, int iMetadata) {
    short Extent[2];
    short *pExtent;
    short cxOwner;
    int nSide;

    pExtent = GetVariantFrameExtent((char *)pManager + 0x10, 0, Extent,
                                    g_nMenuSelectorResourceId6, 0);
    cxOwner = *(short *)((char *)pManager->m_pRenderOwner214 + 8);
    pManager->m_xCenteredAction88 = cxOwner / 2 - pExtent[0] / 2;
    pManager->m_nCurrentPosition8C =
        pManager->m_aMetadataE0[iMetadata].m_nPosition;
    nSide = pManager->m_aMetadataE0[iMetadata].m_nSide;
    pManager->m_nCurrentSide94 = nSide == 1 ? 1 : 0;
    pManager->m_xActionOrigin98 = pManager->m_xCenteredAction88;
    pManager->m_nTargetPosition9C = pManager->m_nCurrentPosition8C;
    pManager->m_nActionStateA4 = 0;
    pManager->m_nSelectedSideA0 = pManager->m_nCurrentSide94;
}

/* FUNCTION: LEMBALL 0x004024F5 */
void LEMBALL_FASTCALL InitializeMenuSelectorCurrentStateThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int iMetadata) {
    InitializeMenuSelectorCurrentState(pManager, nUnusedEdx, iMetadata);
}

static inline void BeginMenuSelectorMovement(
    GameMenuSelectorManager *pManager) {
    int nDistance;
    pManager->m_dwMovementStart21C = (int)timeGetTime();
    nDistance = pManager->m_nTargetPosition9C -
                pManager->m_nCurrentPosition8C;
    if (nDistance < 0) {
        nDistance = -nDistance;
    }
    pManager->m_nMovementStartPosition90 =
        pManager->m_nCurrentPosition8C;
    pManager->m_fMovementActiveDC = 1;
    pManager->m_dwMovementEnd220 =
        pManager->m_dwMovementStart21C + nDistance * 3;
}

/* FUNCTION: LEMBALL 0x0044D830 */
static void LEMBALL_FASTCALL MoveMenuSelectorPrevious(
    GameMenuSelectorManager *pManager) {
    int i;
    int nBest;
    int nSelected;

    nBest = -1;
    nSelected = -1;
    for (i = 0; i < 8; ++i) {
        GameMenuSelectorMetadata *pMetadata =
            &pManager->m_aMetadataE0[i];
        if (pMetadata->m_nSide != 3 &&
            pMetadata->m_nPosition < pManager->m_nTargetPosition9C &&
            nBest < pMetadata->m_nPosition) {
            if (pMetadata->m_nSide != 2) {
                pManager->m_nSelectedSideA0 = pMetadata->m_nSide;
            }
            nSelected = pMetadata->m_nPosition;
            ((GameVariantResourceEntryManager *)
                g_pVariantResourceEntryManager)->PlayVariantResourceEffect(0x11);
            nBest = nSelected;
        }
    }
    if (nSelected != -1) {
        pManager->m_nTargetPosition9C = nSelected;
    }
    BeginMenuSelectorMovement(pManager);
}

/* FUNCTION: LEMBALL 0x00402A36 */
static void LEMBALL_FASTCALL MoveMenuSelectorPreviousThunk(
    GameMenuSelectorManager *pManager) {
    MoveMenuSelectorPrevious(pManager);
}

/* FUNCTION: LEMBALL 0x0044D8E0 */
static void LEMBALL_FASTCALL MoveMenuSelectorNext(
    GameMenuSelectorManager *pManager) {
    int i;
    int nBest;
    int nSelected;

    nBest = 999999;
    nSelected = -1;
    for (i = 0; i < 8; ++i) {
        GameMenuSelectorMetadata *pMetadata =
            &pManager->m_aMetadataE0[i];
        if (pMetadata->m_nSide != 3 &&
            pManager->m_nTargetPosition9C < pMetadata->m_nPosition &&
            pMetadata->m_nPosition < nBest) {
            if (pMetadata->m_nSide != 2) {
                pManager->m_nSelectedSideA0 = pMetadata->m_nSide;
            }
            nSelected = pMetadata->m_nPosition;
            ((GameVariantResourceEntryManager *)
                g_pVariantResourceEntryManager)->PlayVariantResourceEffect(0x11);
            nBest = nSelected;
        }
    }
    if (nSelected != -1) {
        pManager->m_nTargetPosition9C = nSelected;
    }
    BeginMenuSelectorMovement(pManager);
}

/* FUNCTION: LEMBALL 0x004010E1 */
static void LEMBALL_FASTCALL MoveMenuSelectorNextThunk(
    GameMenuSelectorManager *pManager) {
    MoveMenuSelectorNext(pManager);
}

/* FUNCTION: LEMBALL 0x0044D990 */
static void LEMBALL_FASTCALL SelectMenuSelectorSide0(
    GameMenuSelectorManager *pManager) {
    int i;
    if (pManager->m_nCurrentSide94 == pManager->m_nSelectedSideA0) {
        for (i = 0; i < 8; ++i) {
            GameMenuSelectorMetadata *pMetadata =
                &pManager->m_aMetadataE0[i];
            if (pMetadata->m_nPosition == pManager->m_nTargetPosition9C &&
                (pMetadata->m_nSide == 0 || pMetadata->m_nSide == 2)) {
                pManager->m_nSelectedSideA0 = 0;
            }
        }
    }
}

/* FUNCTION: LEMBALL 0x004018F7 */
static void LEMBALL_FASTCALL SelectMenuSelectorSide0Thunk(
    GameMenuSelectorManager *pManager) {
    SelectMenuSelectorSide0(pManager);
}

/* FUNCTION: LEMBALL 0x0044D9E0 */
static void LEMBALL_FASTCALL SelectMenuSelectorSide1(
    GameMenuSelectorManager *pManager) {
    int i;
    if (pManager->m_nCurrentSide94 == pManager->m_nSelectedSideA0) {
        for (i = 0; i < 8; ++i) {
            GameMenuSelectorMetadata *pMetadata =
                &pManager->m_aMetadataE0[i];
            if (pMetadata->m_nPosition == pManager->m_nTargetPosition9C &&
                (pMetadata->m_nSide == 1 || pMetadata->m_nSide == 2)) {
                pManager->m_nSelectedSideA0 = 1;
            }
        }
    }
}

/* FUNCTION: LEMBALL 0x00401FF5 */
static void LEMBALL_FASTCALL SelectMenuSelectorSide1Thunk(
    GameMenuSelectorManager *pManager) {
    SelectMenuSelectorSide1(pManager);
}

static inline void SetFrameSelectorDuration(
    GameProgressiveVariantFrameSelector *pSelector, int dwDuration) {
    GameFrameSelectorSetProc pSet =
        (GameFrameSelectorSetProc)pSelector->m_pVtable[1];
    pSet(pSelector, 0, dwDuration);
}

/* FUNCTION: LEMBALL 0x0044DB20 */
static void LEMBALL_FASTCALL StartMenuSelectorAction(
    GameMenuSelectorManager *pManager) {
    int i;
    int *panOffsets;
    GameMenuSelectorMetadata *pMetadata;
    int nDistance;

    pManager->m_ActionEventB8.m_nType00 = 0x0c;
    if (pManager->m_nCurrentPosition8C != pManager->m_nTargetPosition9C ||
        pManager->m_nActionStateA4 != 0) {
        return;
    }
    pMetadata = pManager->m_aMetadataE0;
    for (i = 0; i < 8; ++i, ++pMetadata) {
        if (pMetadata->m_nPosition == pManager->m_nTargetPosition9C) {
            if (pManager->m_nSelectedSideA0 == 0) {
                pManager->m_ActionEventB8.m_nPayload08 =
                    pMetadata->m_nSide0Token;
                pManager->m_fDelayedEventD8 =
                    pMetadata->m_fSide0Action;
            } else {
                pManager->m_ActionEventB8.m_nPayload08 =
                    pMetadata->m_nSide1Token;
                pManager->m_fDelayedEventD8 =
                    pMetadata->m_fSide1Action;
            }
            break;
        }
    }
    panOffsets = pManager->m_fCompactLayout274 == 1
                     ? g_anMenuSelectorActionOffsetsMode1
                     : g_anMenuSelectorActionOffsetsMode0;
    SetFrameSelectorDuration(pManager->m_pActionLeadSelector24C, 250);
    SetFrameSelectorDuration(pManager->m_pActionMiddleSelector254, 500);
    SetFrameSelectorDuration(pManager->m_pActionTailSelector248, 500);
    if (pManager->m_nSelectedSideA0 == 0) {
        pManager->m_xActionA8 = pManager->m_xActionOrigin98 + panOffsets[6];
        pManager->m_yActionAC = pManager->m_nTargetPosition9C + panOffsets[7];
        pManager->m_xActionDestinationB0 = pMetadata->m_xSide0 + panOffsets[8];
        pManager->m_ActionEventB8.m_nPayload14 = pMetadata->m_xSide0 + panOffsets[10];
        pManager->m_ActionEventB8.m_nPayload18 = pMetadata->m_nPosition + panOffsets[11];
    } else {
        pManager->m_xActionA8 = pManager->m_xActionOrigin98 + panOffsets[12];
        pManager->m_yActionAC = pManager->m_nTargetPosition9C + panOffsets[13];
        pManager->m_xActionDestinationB0 = pMetadata->m_xSide1 + panOffsets[14];
        pManager->m_ActionEventB8.m_nPayload14 = pMetadata->m_xSide1 + panOffsets[16];
        pManager->m_ActionEventB8.m_nPayload18 = pMetadata->m_nPosition + panOffsets[17];
    }
    pManager->m_dwActionLastTick22C = (int)timeGetTime();
    nDistance = pManager->m_xActionDestinationB0 - pManager->m_xActionA8;
    if (nDistance < 0) {
        nDistance = -nDistance;
    }
    pManager->m_nActionStateA4 = 2;
    pManager->m_dwActionMotionEnd230 =
        pManager->m_dwActionLastTick22C + nDistance * 2;
    ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
        ->PlayVariantResourceEffect(7);
}

/* FUNCTION: LEMBALL 0x00402E28 */
static void LEMBALL_FASTCALL StartMenuSelectorActionThunk(
    GameMenuSelectorManager *pManager) {
    StartMenuSelectorAction(pManager);
}

/* FUNCTION: LEMBALL 0x0044CEC0 */
static int LEMBALL_FASTCALL HandleMenuSelectorEvent(
    GameMenuSelectorManager *pManager, int,
    GameMenuSelectorEvent *pEvent) {
    if (pEvent->m_dwTimestamp04 - pManager->m_dwConstructionTimeD4 < 0) {
        return 0;
    }
    if (pEvent->m_nType00 != 4) {
        ++pManager->m_cUnhandledEvents0C;
        return 0;
    }
    switch (pEvent->m_nPayload08) {
    case 1:
        MoveMenuSelectorPreviousThunk(pManager);
        return 1;
    case 2:
        MoveMenuSelectorNextThunk(pManager);
        return 1;
    case 3:
        SelectMenuSelectorSide0Thunk(pManager);
        return 1;
    case 4:
        SelectMenuSelectorSide1Thunk(pManager);
        return 1;
    case 0x1f:
    case 0x22:
    case 0x4c:
        StartMenuSelectorActionThunk(pManager);
        break;
    }
    return 0;
}

/* FUNCTION: LEMBALL 0x00401ADC */
static int LEMBALL_FASTCALL HandleMenuSelectorEventThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx,
    GameMenuSelectorEvent *pEvent) {
    return HandleMenuSelectorEvent(pManager, nUnusedEdx, pEvent);
}

/* FUNCTION: LEMBALL 0x0044C1E0 */
static int GetIntDirectionSign(int nValue) {
    if (nValue < 0) {
        return -1;
    }
    if (nValue > 0) {
        return 1;
    }
    return 0;
}

/* FUNCTION: LEMBALL 0x004036C0 */
static int GetIntDirectionSignThunk(int nValue) {
    return GetIntDirectionSign(nValue);
}

static inline void QueueMenuSelectorEvent(
    GameMenuSelectorEvent *pEvent) {
    typedef void (LEMBALL_FASTCALL *QueueProc)(void *, int, void *);
    QueueProc pQueue =
        (QueueProc)(*(void ***)g_pSharedRenderDispatchQueue)[2];
    pQueue(g_pSharedRenderDispatchQueue, 0, pEvent);
}

/* FUNCTION: LEMBALL 0x0044DCE0 */
void LEMBALL_FASTCALL UpdateMenuSelectorAnimation(
    GameMenuSelectorManager *pManager) {
    unsigned int dwNow;
    unsigned int dwEventTime;

    dwNow = (unsigned int)timeGetTime();
    if (pManager->m_nCurrentSide94 == pManager->m_nSelectedSideA0 ||
        pManager->m_nActionStateA4 == 1) {
        if (pManager->m_nActionStateA4 == 1) {
            if ((unsigned int)pManager->m_dwSideTransitionEnd228 <= dwNow) {
                pManager->m_nActionStateA4 = 0;
                pManager->m_nCurrentSide94 = pManager->m_nSelectedSideA0;
            }
        } else if (pManager->m_nActionStateA4 == 2) {
            if (dwNow < (unsigned int)pManager->m_dwActionMotionEnd230) {
                int x = pManager->m_xActionA8;
                int dwPrevious = pManager->m_dwActionLastTick22C;
                int nDirection = GetIntDirectionSignThunk(
                    pManager->m_xActionDestinationB0 - x);
                pManager->m_dwActionLastTick22C = (int)dwNow;
                pManager->m_xActionA8 =
                    x + nDirection * ((int)(dwNow - dwPrevious) >> 1);
            } else {
                ((GameVariantResourceEntryManager *)
                    g_pVariantResourceEntryManager)->PlayVariantResourceEffect(0x0e);
                pManager->m_dwActionFrameStart23C = (int)dwNow;
                pManager->m_dwActionFrameEnd240 = (int)dwNow + 500;
                pManager->m_nActionStateA4 = 3;
                SetFrameSelectorDuration(pManager->m_pCompletionSelector250,
                                         500);
            }
        } else if (pManager->m_nActionStateA4 == 3) {
            dwEventTime = (unsigned int)pManager->m_dwActionFrameEnd240;
            if (pManager->m_fDelayedEventD8 != 0) {
                dwEventTime -= 0x177;
            }
            if (dwEventTime <= dwNow && pManager->m_fEventQueuedB4 != 1) {
                pManager->m_ActionEventB8.m_dwTimestamp04 =
                    (int)timeGetTime();
                QueueMenuSelectorEvent(&pManager->m_ActionEventB8);
                pManager->m_fEventQueuedB4 = 1;
            }
            if ((unsigned int)pManager->m_dwActionFrameEnd240 <= dwNow) {
                pManager->m_nActionStateA4 = 0;
                pManager->m_fEventQueuedB4 = 0;
            }
        }
    } else {
        GameFrameSelectorSetProc pSetDirection;
        pManager->m_dwSideTransitionStart224 = (int)dwNow;
        pManager->m_dwSideTransitionEnd228 = (int)dwNow + 250;
        pManager->m_nActionStateA4 = 1;
        SetFrameSelectorDuration(pManager->m_pSideSelector244, 250);
        pSetDirection = (GameFrameSelectorSetProc)
            pManager->m_pSideSelector244->m_pVtable[4];
        pSetDirection(pManager->m_pSideSelector244, 0,
                      pManager->m_nSelectedSideA0 == 0 ? -1 : 1);
    }
    if (pManager->m_fMovementActiveDC != 0) {
        if (dwNow < (unsigned int)pManager->m_dwMovementEnd220) {
            pManager->m_nCurrentPosition8C =
                ((pManager->m_nTargetPosition9C -
                  pManager->m_nMovementStartPosition90) *
                 ((int)dwNow - pManager->m_dwMovementStart21C)) /
                    (pManager->m_dwMovementEnd220 -
                     pManager->m_dwMovementStart21C) +
                pManager->m_nMovementStartPosition90;
        } else {
            pManager->m_fMovementActiveDC = 0;
            pManager->m_nCurrentPosition8C =
                pManager->m_nTargetPosition9C;
        }
    }
}

/* FUNCTION: LEMBALL 0x004014CE */
void LEMBALL_FASTCALL UpdateMenuSelectorAnimationThunk(
    GameMenuSelectorManager *pManager) {
    UpdateMenuSelectorAnimation(pManager);
}

/* FUNCTION: LEMBALL 0x0044C600 */
static void LEMBALL_FASTCALL RebuildMenuSelectorEntryRects(
    GameMenuSelectorEntry *pEntry, int, int fForce) {
    void *apOverlays[2];
    int i;
    apOverlays[0] = pEntry->m_pAlignedOverlay50;
    apOverlays[1] = pEntry->m_pProgressOverlay54;
    for (i = 0; i < 2; ++i) {
        if (apOverlays[i] != 0) {
            int fPrevious;
            short Rect[4];
            if (*(int *)((char *)apOverlays[i] + 0xfc) == 0 || fForce != 0) {
                Rect[0] = *(short *)((char *)apOverlays[i] + 8);
                Rect[1] = *(short *)((char *)apOverlays[i] + 10);
                Rect[2] = 0;
                Rect[3] = 0;
                fPrevious = *(int *)((char *)apOverlays[i] + 0xf8);
                *(int *)((char *)apOverlays[i] + 0xf8) = 1;
                typedef void (LEMBALL_FASTCALL *SetRectProc)(
                    void *, int, short *);
                ((SetRectProc)(*(void ***)apOverlays[i])[0x2a])(
                    apOverlays[i], 0, Rect);
                *(int *)((char *)apOverlays[i] + 0xf8) = fPrevious;
            }
            *(int *)((char *)apOverlays[i] + 0xfc) = 0;
        }
    }
}

/* FUNCTION: LEMBALL 0x0044D290 */
void LEMBALL_FASTCALL RebuildMenuSelectorChildRects(
    GameMenuSelectorManager *pManager, int, int fForce) {
    int i;
    for (i = 0; i < 8; ++i) {
        if (pManager->m_apChildren1E0[i] != 0) {
            RebuildMenuSelectorEntryRects(pManager->m_apChildren1E0[i],
                                          0, fForce);
        }
    }
}

/* FUNCTION: LEMBALL 0x004012CB */
void LEMBALL_FASTCALL RebuildMenuSelectorChildRectsThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int fForce) {
    RebuildMenuSelectorChildRects(pManager, nUnusedEdx, fForce);
}

/* FUNCTION: LEMBALL 0x0044CC90 */
void LEMBALL_FASTCALL SetMenuSelectorChildrenEnabled(
    GameMenuSelectorManager *pManager, int, int fEnabled) {
    int i;
    pManager->m_fChildrenEnabled278 = fEnabled;
    for (i = 0; i < pManager->m_cChildren80; ++i) {
        GameMenuSelectorEntry *pEntry = pManager->m_apChildren1E0[i];
        if (pEntry != 0) {
            pEntry->m_fEnabled5C = fEnabled;
            if (pEntry->m_pAlignedOverlay50 != 0) {
                ((GameGeometryOwnerRectState *)((char *)
                    pEntry->m_pAlignedOverlay50 + 0x90))->SetEnabled(fEnabled);
            }
        }
    }
}

/* FUNCTION: LEMBALL 0x00401D43 */
void LEMBALL_FASTCALL SetMenuSelectorChildrenEnabledThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int fEnabled) {
    SetMenuSelectorChildrenEnabled(pManager, nUnusedEdx, fEnabled);
}
