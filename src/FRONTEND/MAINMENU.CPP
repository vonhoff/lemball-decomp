#include "FRONTEND/MAINMENU.H"

#include "ENGINE/GDI/VSGDI.H"
#include "ENGINE/CORE/VSMEM.H"
#include "GAME.H"
#include "SHELL/SHELLUI.H"
#include "SHELL/OPTIONS.H"
#include "RESOURCE/MOGLOAD.H"
#include "LEVEL/DRAWTEXT.H"
#include "FRONTEND/MENUSEL.H"
#include "ENGINE/GDI/VSGEOM.H"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif

extern "C" DWORD WINAPI timeGetTime(void);

extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void VariantResourceBundleNoopThunk(void);
extern void *LoadBitmapResource(int nResourceId);
extern void LoadVariantRenderResource(void *pManager, int nResourceId);
extern void ResetOrFlipVariantRenderBuffers(int nManager);
extern void DestroyVariantRenderManager(void *pManager);
extern void *ConstructVariantRenderManager(void *pManager,
                                           int nOwner,
                                           int cResourceIds,
                                           int cPackagedRectEntries,
                                           int cActionButtonEntries,
                                           int nVariantStateCount,
                                           int fDoubleBuffered);
extern void *LEMBALL_FASTCALL ConstructFramedScreenRenderEntryGroup(
    void *pGroup);
extern void SetLevelScreenStatusIndicatorMode(int nMode, int nValue);
extern void LEMBALL_FASTCALL SetStatusIndicatorActive(
    void *pObject, int nUnused, int fActive);
extern void AppendPointerQueueEntry(void *pQueue, void *pEntry);
extern int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresent(
    int *pPrimaryContext);
extern void TogglePrimaryContextDisplayMode(
    ShellUiPrimaryContextShell *pShell);
extern void LEMBALL_FASTCALL NoopPackagedSpriteRenderEntryThunk00402A22(
    void *pUnused);
extern void LEMBALL_FASTCALL NoopPackagedSpriteRenderEntryThunk00401B77(
    void *pUnusedThis, int nUnusedEdx, void *pArgument);
extern void PASCAL noop_render_entry_callback_004393a0(void *pUnused);
extern void noop_render_entry_callback_004393d0(void);
extern void noop_render_entry_callback_004393f0(void);
extern int LEMBALL_FASTCALL get_intro_sequence_screen_requested_mode(
    void *pScreen);
extern int LEMBALL_FASTCALL poll_intro_sequence_screen_transition_status(
    void *pScreen);
extern void noop_screen_callback_00446f70(void);
extern void noop_screen_callback_00446f80(void);
extern int PASCAL return_zero_screen_callback_00446fa0(void *pUnused);
extern void noop_main_menu_screen_hook(void);

extern void *g_pLevelProgressState;
extern void *g_pActiveNetworkLobbyScreen;
extern void *g_pNetworkLobbyVsnetRuntime;
extern void *g_pSharedRenderDispatchQueue;
extern void *g_pMainResourceArchive;
extern int g_nSelectedNetworkLobbyPeerId;
extern void *g_pSessionRandomState;

struct GameProgressiveSelectorVtableView {
    virtual int SelectFrame(void) = 0;
    virtual void SetProgressiveDuration(int nValue) = 0;
    virtual void SetDuration(int nValue) = 0;
};

struct GameQueuedVariantChildView {
    char m_abReserved00[0x4c];
    void *m_pOverlay4C;
    int m_nReserved50;
    int m_fEnabled54;
};

class GameQueuedVariantManagerView {
public:
    void SetChildrenEnabled(int fEnabled);
    void SetChildrenEnabledThunk(int fEnabled);

    char m_abReserved00[0x80];
    int m_cChildren80;
    char m_abReserved84[0x28];
    GameQueuedVariantChildView *m_apChildrenAC[4];
    char m_aSlotRecordsBC[4][0x10];
    char m_abReservedFC[0x48];
    int m_nCurrentChild144;
    int m_fChildrenEnabled148;
};

class GameIntroSequenceMediaState {
public:
    void ConfigureOwner(void *pScreen, void *pPrimaryContext, int fSkipVariant);

private:
    unsigned char m_abReserved00[0xbc];
    void *m_pScreenBC;
    void *m_pPrimaryContextC0;
    int m_fSkipVariantC4;
};

struct VsNetRuntimeModeView {
    char m_abReserved00[0x48];
    int m_nMode48;
    int m_nModeState4C;
};



struct GameLevelProgressStateView {
    int m_nCurrentLevel00;
    int m_nReserved04;
    int m_nCurrentPack08;
    int m_nReserved0C;
    char m_szPassword10[0x18];
    int m_anUnlockedPackCaps28[5];
    int m_anSnapshotPackCaps3C[4];
    int m_nReserved4C;
};

struct GameFramedScreenBase {
    void **m_pVtable00;
    unsigned char m_abRenderQueueNode04[0x10];
    unsigned char m_abVariantRenderManager14[0x70];
    void *m_pPrimaryContext84;
    void *m_pRenderContext88;
    int m_fNetworkScreen8C;
    int m_nNetworkState90;
    int m_fNetworkReady94;
    void *m_pMappedListResources98;
    int m_fFrameResourcesLoaded9C;
    int m_cMappedListEntriesA0;
    int m_nMappedListModeA4;
    int m_fDrawFrameA8;
    int m_fTileBackgroundAC;
    int m_fDrawForegroundB0;
    int m_nCursorResourceIdB4;
    int m_nAppliedCursorResourceIdB8;
    int m_dwConstructionTimeBC;
    void *m_pForegroundBitmapC0;
    void *m_pBackgroundBitmapC4;
    unsigned char m_abRenderEntryGroupC8[0x260];
    int m_iRenderEntryGroup328;
    int m_fCompactLayout32C;
    int m_fFrameDirty330;
    void *m_pSelectorManager334;
    void *m_pQueuedVariantManager338;
    int m_dwRandomDelay33C;
    int m_dwRandomStart340;
    int m_nReserved344;
    int m_nReserved348;
    void *m_pTimedFrameSelector34C;
    void **m_pFrameSelectorVtable350;
    int m_cFrameSelectorFrames354;
    int m_nFrameSelectorReserved358;
    int m_nFrameSelectorFixedTime35C;
    int m_fSelectorDirty360;
    int m_fBuildingRenderEntries364;
    int m_fRenderEntriesInitialized368;
    int m_iPackagedEntry36C;
    int m_nScreenType370;
    int m_nRequestedMode374;
    int m_nTransitionStatus378;
    short m_cxScreen37C;
    short m_cyScreen37E;
    short m_xOverlay380;
    short m_yOverlay382;
    int m_nMappedListRemapResourceId384;
    int m_nFrameResourceId388;
    int m_nMappedListResourceId38C;
    int m_nFrameResourceId390;
    int m_nTimedVariantResourceId394;
};

struct GameMainMenuScreenView : public GameFramedScreenBase {
    DWORD m_dwTimeout398;
    const int *m_panWidgetLayout39C;
    int m_nPlaySelection3A0;
    int m_nMultiplayerSelection3A4;
    int m_nReserved3A8;
    int m_nReserved3AC;
    int m_nDisplayModeSelection3B0;
    int m_nPackSelection3B4;
    int m_nSharedSelection3B8;
};

typedef char GameFramedScreenBase_size_must_be_0x398[
    sizeof(GameFramedScreenBase) == 0x398 ? 1 : -1];
typedef char GameMainMenuScreenView_size_must_be_0x3bc[
    sizeof(GameMainMenuScreenView) == 0x3bc ? 1 : -1];

typedef void (LEMBALL_FASTCALL *GameNoArgVirtual)(void *, int);
typedef void (LEMBALL_FASTCALL *GameOneArgVirtual)(void *, int, void *);

class GameMainMenuSelectorManagerInterface {
public:
    virtual void Reserved00(void *) = 0;
    virtual void Delete(void *fDelete) = 0;
};

class GameMainMenuRenderEntryInterface {
public:
    virtual void Reserved00(void) = 0;
    virtual void AppendToQueue(void *pRenderContext) = 0;
};
typedef int (LEMBALL_FASTCALL *GameIntNoArgVirtual)(void *, int);
typedef int (LEMBALL_FASTCALL *GameIntOneArgVirtual)(void *, int, void *);

static void LEMBALL_FASTCALL DestroyMainMenuScreenThunk00401190(
    void *pObject);
static void LEMBALL_FASTCALL ResetFramedScreenVariantBuffersThunk(
    void *pObject);
static int LEMBALL_FASTCALL MainMenuSlot21Thunk(
    void *pObject, int nUnusedEdx, void *pArgument);
static void LEMBALL_FASTCALL UpdateFramedScreenRuntimeThunk(void *pObject);
static void LEMBALL_FASTCALL RefreshFramedScreenFrameResourcesThunk(void *pObject);
static int LEMBALL_FASTCALL HandleFramedScreenQueueEventThunk(
    void *pQueueNode, int nUnusedEdx, void *pEvent);
static void LEMBALL_FASTCALL UpdateMainMenuScreenThunk(void *pObject);
static void LEMBALL_FASTCALL InitializeMainMenuWidgetsThunk(void *pObject);
static void LEMBALL_FASTCALL MainMenuSlot06Thunk(void *pObject);
static void LEMBALL_FASTCALL MainMenuSlot19Thunk(void *pObject);
static void LEMBALL_FASTCALL RecenterFramedScreenIfNeededThunk(
    void *pObject);
static void LEMBALL_FASTCALL RefreshAndDrawFramedScreenBaseThunk(
    void *pObject, int nUnusedEdx, void *pFrameArgument);
static int LEMBALL_FASTCALL HandleMainMenuInputThunk(
    void *pObject, int nUnusedEdx, void *pEvent);
static void *LEMBALL_FASTCALL DeleteMainMenuScreenThunk(
    void *pObject, int nUnusedEdx, int fDelete);
static void LEMBALL_FASTCALL MainMenuSlot18Thunk(void *pObject);
static int LEMBALL_FASTCALL GetFramedScreenTransitionStatusThunk(
    void *pObject);
static void LEMBALL_FASTCALL MainMenuSlot05Thunk(
    void *pObject, int nUnusedEdx, void *pArgument);
static void LEMBALL_FASTCALL MainMenuSlot20Thunk(void *pObject);
static int LEMBALL_FASTCALL GetFramedScreenRequestedModeThunk(
    void *pObject);
static void LEMBALL_FASTCALL MainMenuSlot09Thunk(void *pObject);
static void LEMBALL_FASTCALL MainMenuSlot13Thunk(void *pObject);
static void LEMBALL_FASTCALL SetFramedScreenSizeThunk(
    void *pObject, int nUnusedEdx, short *paSize);
static void *LEMBALL_FASTCALL DeleteAdjustedMainMenuScreenThunk(
    void *pObject, int nUnusedEdx, int fDelete);
static void LEMBALL_FASTCALL DestroyMainMenuWidgetsThunk(void *pObject);
void *LEMBALL_FASTCALL ConstructFramedScreenBaseFields(
    void *pObject, int, void *pPrimaryContext, void *pRenderContext,
    short *paSize, int nScreenType, int cExtraPackagedEntries,
    int cExtraActionEntries, int nVariantStateCount,
    int cMappedListEntries, int nMappedListMode);
void LEMBALL_FASTCALL ActivateFramedScreenBase(void *pObject);
static void LEMBALL_FASTCALL RefreshFramedScreenRenderEntryGroup(
    void *pObject);
static int ConsumeLevelSelectionChangedFlag(void);
static void LEMBALL_FASTCALL RefreshAndDrawFramedScreenBase(
    void *pObject, int, void *);
static void LEMBALL_FASTCALL DrawFramedScreenBase(void *pObject);
static void LEMBALL_FASTCALL DrawFramedScreenFrameLayer(void *pObject);
static void LEMBALL_FASTCALL RefreshFramedScreenFrameResources(void *pObject);
static void LEMBALL_FASTCALL LoadFramedScreenFrameResources(void *pObject);
static void LEMBALL_FASTCALL ReleaseFramedScreenFrameResources(void *pObject);
static void LEMBALL_FASTCALL ResetFramedScreenVariantBuffers(
    void *pObject);
static int LEMBALL_FASTCALL HandleFramedScreenQueueEvent(
    void *pQueueNode, int, void *pEvent);
static void LEMBALL_FASTCALL UpdateFramedScreenRuntime(void *pObject);
static void LEMBALL_FASTCALL RecenterFramedScreenIfNeeded(void *pObject);
static void LEMBALL_FASTCALL SetFramedScreenSize(
    void *pObject, int, short *paSize);
static void LEMBALL_FASTCALL InitializeMainMenuWidgets(void *pObject);
static void LEMBALL_FASTCALL DestroyMainMenuWidgets(void *pObject);
static void LEMBALL_FASTCALL DestroyMainMenuScreen(void *pObject);
static int LEMBALL_FASTCALL HandleMainMenuInput(
    void *pObject, int, void *pEvent);
static void LEMBALL_FASTCALL UpdateMainMenuScreen(void *pObject);
static void *LEMBALL_FASTCALL DeleteMainMenuScreen(
    void *pObject, int, int fDelete);
static void *LEMBALL_FASTCALL DeleteAdjustedMainMenuScreen(
    void *pObject, int nUnusedEdx, int fDelete);

// GLOBAL: LEMBALL 0x0049f4f0
int g_fNextMainMenuTimeoutIsTitle = 1;
// GLOBAL: LEMBALL 0x004a6300
int g_nLastMainMenuPackSelection = 0;
// GLOBAL: LEMBALL 0x0049fa68
int g_fLevelSelectionChanged = 0;

static const int g_GAME_MainMenuWidgetLayoutStandard[12] = {
    0x20, 0x74, 0x1e0, 0x74, 0x30, 0xe8,
    0x1d0, 0xe8, 0x50, 0x15c, 0x1a0, 0x15c
};
static const int g_GAME_MainMenuWidgetLayoutCompact[12] = {
    0x10, 0x3a, 0xf0, 0x3a, 0x18, 0x74,
    0xe8, 0x74, 0x28, 0xae, 0xd0, 0xae
};

// GLOBAL: LEMBALL 0x0049f4f4
static const int g_GAME_MainMenuWidgetResourcesStandard[11] = {
    0x1a7, 0x1a6, 0x1b0, 0x1a9, 0x1c3, 0x1ac,
    0x1ad, 0x1ae, 0x1af, 0x1b6, 0x1c4
};

// GLOBAL: LEMBALL 0x0049f520
static const int g_GAME_MainMenuWidgetResourcesCompact[12] = {
    0x1db, 0x1da, 0x1e4, 0x1f7, 0x1dd, 0,
    0x1e0, 0x1e1, 0x1e2, 0x1e3, 0x1ea, 0x1f8
};

static void *g_GAME_SelectFirstVariantFrameVtable[7];

static void LEMBALL_FASTCALL RefreshFramedScreenFrameResources(
    void *pObject);
static void LEMBALL_FASTCALL RefreshAndDrawFramedScreenBase(
    void *pObject, int nUnusedEdx, void *pFrameArgument);
static void LEMBALL_FASTCALL ResetFramedScreenVariantBuffers(void *pObject);
static void LEMBALL_FASTCALL UpdateFramedScreenRuntime(void *pObject);
static void LEMBALL_FASTCALL RecenterFramedScreenIfNeeded(void *pObject);
static void LEMBALL_FASTCALL SetFramedScreenSize(
    void *pObject, int nUnusedEdx, short *paSize);
static void LEMBALL_FASTCALL UpdateMainMenuScreen(void *pObject);
static int LEMBALL_FASTCALL HandleMainMenuInput(
    void *pObject, int nUnusedEdx, void *pEvent);
static void LEMBALL_FASTCALL InitializeMainMenuWidgets(void *pObject);
static void LEMBALL_FASTCALL DestroyMainMenuWidgets(void *pObject);
static void LEMBALL_FASTCALL DestroyMainMenuScreen(void *pObject);
static void *LEMBALL_FASTCALL DeleteMainMenuScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteAdjustedMainMenuScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static int LEMBALL_FASTCALL HandleFramedScreenQueueEvent(
    void *pQueueNode, int nUnusedEdx, void *pEvent);
























void *g_GAME_MainMenuScreenVtable[22] = {
    (void *)DeleteMainMenuScreenThunk,
    (void *)NoopPackagedSpriteRenderEntryThunk00402A22,
    (void *)RefreshAndDrawFramedScreenBaseThunk,
    (void *)NoopPackagedSpriteRenderEntryThunk00401B77,
    (void *)SetFramedScreenSizeThunk,
    (void *)MainMenuSlot05Thunk,
    (void *)MainMenuSlot06Thunk,
    (void *)UpdateFramedScreenRuntimeThunk,
    (void *)ResetFramedScreenVariantBuffersThunk,
    (void *)MainMenuSlot09Thunk,
    (void *)GetFramedScreenRequestedModeThunk,
    (void *)GetFramedScreenTransitionStatusThunk,
    (void *)RecenterFramedScreenIfNeededThunk,
    (void *)MainMenuSlot13Thunk,
    (void *)UpdateMainMenuScreenThunk,
    (void *)HandleMainMenuInputThunk,
    (void *)InitializeMainMenuWidgetsThunk,
    (void *)DestroyMainMenuWidgetsThunk,
    (void *)MainMenuSlot18Thunk,
    (void *)MainMenuSlot19Thunk,
    (void *)MainMenuSlot20Thunk,
    (void *)MainMenuSlot21Thunk
};

void *g_GAME_MainMenuQueueVtable[4] = {
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteAdjustedMainMenuScreenThunk,
    (void *)HandleFramedScreenQueueEventThunk,
    0
};

void *g_GAME_MainMenuCallbackVtable[2] = {
    (void *)VariantResourceBundleNoopThunk,
    0
};

static void *g_GAME_FramedScreenBaseVtable[22] = {
    (void *)DeleteMainMenuScreenThunk,
    (void *)NoopPackagedSpriteRenderEntryThunk00402A22,
    (void *)RefreshAndDrawFramedScreenBaseThunk,
    (void *)NoopPackagedSpriteRenderEntryThunk00401B77,
    (void *)SetFramedScreenSizeThunk,
    (void *)MainMenuSlot05Thunk,
    (void *)MainMenuSlot06Thunk,
    (void *)UpdateFramedScreenRuntimeThunk,
    (void *)ResetFramedScreenVariantBuffersThunk,
    (void *)MainMenuSlot09Thunk,
    (void *)GetFramedScreenRequestedModeThunk,
    (void *)GetFramedScreenTransitionStatusThunk,
    (void *)RecenterFramedScreenIfNeededThunk,
    (void *)MainMenuSlot13Thunk,
    (void *)MainMenuSlot13Thunk,
    (void *)MainMenuSlot21Thunk,
    (void *)MainMenuSlot18Thunk,
    (void *)MainMenuSlot19Thunk,
    (void *)MainMenuSlot18Thunk,
    (void *)MainMenuSlot19Thunk,
    (void *)MainMenuSlot20Thunk,
    (void *)MainMenuSlot21Thunk
};


// FUNCTION: LEMBALL 0x00401190
static void LEMBALL_FASTCALL DestroyMainMenuScreenThunk00401190(
    void *pObject)

{
    DestroyMainMenuScreen(pObject);
}

// FUNCTION: LEMBALL 0x004012ad
static void LEMBALL_FASTCALL ResetFramedScreenVariantBuffersThunk(
    void *pObject)

{
    ResetFramedScreenVariantBuffers(pObject);
}

// FUNCTION: LEMBALL 0x00401528
static int LEMBALL_FASTCALL MainMenuSlot21Thunk(
    void *pObject, int nUnusedEdx, void *pArgument)
{
    (void)pObject;
    (void)nUnusedEdx;
    return return_zero_screen_callback_00446fa0(pArgument);
}

// FUNCTION: LEMBALL 0x00401735
void GameQueuedVariantManagerView::SetChildrenEnabledThunk(int fEnabled) {
    SetChildrenEnabled(fEnabled);
}

// FUNCTION: LEMBALL 0x00401767
static void LEMBALL_FASTCALL UpdateFramedScreenRuntimeThunk(void *pObject) {
    UpdateFramedScreenRuntime(pObject);
}

// FUNCTION: LEMBALL 0x004017ad
static void LEMBALL_FASTCALL RefreshFramedScreenFrameResourcesThunk(void *pObject) {
    RefreshFramedScreenFrameResources(pObject);
}

// FUNCTION: LEMBALL 0x00401889
static int LEMBALL_FASTCALL HandleFramedScreenQueueEventThunk(
    void *pQueueNode, int nUnusedEdx, void *pEvent)

{
    return HandleFramedScreenQueueEvent(pQueueNode, nUnusedEdx, pEvent);
}

// FUNCTION: LEMBALL 0x004018de
static void LEMBALL_FASTCALL UpdateMainMenuScreenThunk(void *pObject) {
    UpdateMainMenuScreen(pObject);
}

// FUNCTION: LEMBALL 0x00401ba4
static void LEMBALL_FASTCALL InitializeMainMenuWidgetsThunk(void *pObject) {
    InitializeMainMenuWidgets(pObject);
}

// FUNCTION: LEMBALL 0x00401dc5
static void LEMBALL_FASTCALL MainMenuSlot06Thunk(void *pObject) {
    (void)pObject;
    noop_main_menu_screen_hook();
}

// FUNCTION: LEMBALL 0x00402185
static void LEMBALL_FASTCALL MainMenuSlot19Thunk(void *pObject) {
    (void)pObject;
    noop_screen_callback_00446f80();
}

// FUNCTION: LEMBALL 0x004021a3
static void LEMBALL_FASTCALL RecenterFramedScreenIfNeededThunk(
    void *pObject)

{
    RecenterFramedScreenIfNeeded(pObject);
}

// FUNCTION: LEMBALL 0x00402252
static void LEMBALL_FASTCALL RefreshAndDrawFramedScreenBaseThunk(
    void *pObject, int nUnusedEdx, void *pFrameArgument)

{
    RefreshAndDrawFramedScreenBase(pObject, nUnusedEdx, pFrameArgument);
}

// FUNCTION: LEMBALL 0x0040242d
static int LEMBALL_FASTCALL HandleMainMenuInputThunk(
    void *pObject, int nUnusedEdx, void *pEvent)

{
    return HandleMainMenuInput(pObject, nUnusedEdx, pEvent);
}

// FUNCTION: LEMBALL 0x00402595
static void *LEMBALL_FASTCALL DeleteMainMenuScreenThunk(
    void *pObject, int nUnusedEdx, int fDelete)

{
    return DeleteMainMenuScreen(pObject, nUnusedEdx, fDelete);
}

// FUNCTION: LEMBALL 0x004026bc
static void LEMBALL_FASTCALL MainMenuSlot18Thunk(void *pObject) {
    (void)pObject;
    noop_screen_callback_00446f70();
}

// FUNCTION: LEMBALL 0x00402a68
static int LEMBALL_FASTCALL GetFramedScreenTransitionStatusThunk(
    void *pObject)

{
    return poll_intro_sequence_screen_transition_status(pObject);
}

// FUNCTION: LEMBALL 0x00402c02
static void LEMBALL_FASTCALL MainMenuSlot05Thunk(
    void *pObject, int nUnusedEdx, void *pArgument)

{
    (void)pObject;
    (void)nUnusedEdx;
    noop_render_entry_callback_004393a0(pArgument);
}

// FUNCTION: LEMBALL 0x0040308a
static void LEMBALL_FASTCALL MainMenuSlot20Thunk(void *pObject) {
    (void)pObject;
    noop_main_menu_screen_hook();
}

// FUNCTION: LEMBALL 0x0040310c
static int LEMBALL_FASTCALL GetFramedScreenRequestedModeThunk(
    void *pObject)

{
    return get_intro_sequence_screen_requested_mode(pObject);
}

// FUNCTION: LEMBALL 0x0040313e
static void LEMBALL_FASTCALL MainMenuSlot09Thunk(void *pObject) {
    (void)pObject;
    noop_render_entry_callback_004393d0();
}

// FUNCTION: LEMBALL 0x0040321f
static void LEMBALL_FASTCALL MainMenuSlot13Thunk(void *pObject) {
    (void)pObject;
    noop_render_entry_callback_004393f0();
}

// FUNCTION: LEMBALL 0x004032ec
static void LEMBALL_FASTCALL SetFramedScreenSizeThunk(
    void *pObject, int nUnusedEdx, short *paSize)

{
    SetFramedScreenSize(pObject, nUnusedEdx, paSize);
}

// FUNCTION: LEMBALL 0x004035d0
static void *LEMBALL_FASTCALL DeleteAdjustedMainMenuScreenThunk(
    void *pObject, int nUnusedEdx, int fDelete)
{
    return DeleteAdjustedMainMenuScreen(pObject, nUnusedEdx, fDelete);
}

// FUNCTION: LEMBALL 0x0040367f
static void LEMBALL_FASTCALL DestroyMainMenuWidgetsThunk(void *pObject) {
    DestroyMainMenuWidgets(pObject);
}

// FUNCTION: LEMBALL 0x00445420
void *LEMBALL_FASTCALL ConstructFramedScreenBaseFields(
    void *pObject, int, void *pPrimaryContext, void *pRenderContext,
    short *paSize, int nScreenType, int cExtraPackagedEntries,
    int cExtraActionEntries, int nVariantStateCount,
    int cMappedListEntries, int nMappedListMode)

{
    GameFramedScreenBase *pScreen;

    pScreen = (GameFramedScreenBase *)pObject;
    pScreen->m_pVtable00 = g_GAME_FramedScreenBaseVtable;
    InitializeRenderQueueNodeBase(pScreen->m_abRenderQueueNode04);
    ConstructVariantRenderManager(
        pScreen->m_abVariantRenderManager14,
        (int)(unsigned long)pRenderContext, 0x2b6,
        cExtraPackagedEntries + 3, cExtraActionEntries + 200,
        nVariantStateCount, 0);
    ConstructFramedScreenRenderEntryGroup(pScreen->m_abRenderEntryGroupC8);

    pScreen->m_cyScreen37E = 0;
    pScreen->m_cFrameSelectorFrames354 = 1;
    pScreen->m_cxScreen37C = 0;
    pScreen->m_nFrameSelectorFixedTime35C = 0;
    pScreen->m_yOverlay382 = 0;
    pScreen->m_xOverlay380 = 0;
    pScreen->m_nScreenType370 = nScreenType;
    pScreen->m_pPrimaryContext84 = pPrimaryContext;
    pScreen->m_pRenderContext88 = pRenderContext;
    pScreen->m_pFrameSelectorVtable350 =
        g_GAME_SelectFirstVariantFrameVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_MainMenuQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    pScreen->m_cxScreen37C = paSize[0];
    pScreen->m_cyScreen37E = paSize[1];
    pScreen->m_cMappedListEntriesA0 = cMappedListEntries;
    pScreen->m_nMappedListModeA4 = nMappedListMode;
    pScreen->m_iPackagedEntry36C = 0;
    pScreen->m_fDrawFrameA8 = 1;
    pScreen->m_fTileBackgroundAC = 1;
    pScreen->m_fDrawForegroundB0 = 0;
    pScreen->m_nAppliedCursorResourceIdB8 = 0;
    pScreen->m_fFrameResourcesLoaded9C = 0;
    pScreen->m_nCursorResourceIdB4 = 0x10a;

    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->RegisterOrderedRenderDispatchClient(
                pScreen->m_abRenderQueueNode04, 0);
    }

    pScreen->m_nRequestedMode374 = 0;
    pScreen->m_nTransitionStatus378 = 0;
    pScreen->m_fRenderEntriesInitialized368 = 0;
    pScreen->m_fBuildingRenderEntries364 = 0;
    pScreen->m_iRenderEntryGroup328 = 0;
    pScreen->m_fFrameDirty330 = 1;
    pScreen->m_fNetworkScreen8C =
        (((GameLevelProgressStateView *)g_pLevelProgressState)
                 ->m_nCurrentPack08 == 4 &&
         g_nSelectedNetworkLobbyPeerId != 0);
    pScreen->m_fNetworkReady94 = 1;
    pScreen->m_nNetworkState90 = 0;
    pScreen->m_pSelectorManager334 = 0;
    pScreen->m_pQueuedVariantManager338 = 0;
    pScreen->m_nTimedVariantResourceId394 = 0;
    pScreen->m_pTimedFrameSelector34C = 0;
    pScreen->m_pMappedListResources98 = 0;
    pScreen->m_dwConstructionTimeBC = timeGetTime();
    return pObject;
}

// FUNCTION: LEMBALL 0x004455f0
void LEMBALL_FASTCALL ActivateFramedScreenBase(void *pObject) {
    GameFramedScreenBase *pScreen;
    MappedListResourceTable *pMappedList;
    GameProgressiveVariantFrameSelector *pSelector;
    int nRuntimeMode;
    VsNetRuntimeModeView *pRuntime;
    DWORD dwNow;

    pScreen = (GameFramedScreenBase *)pObject;
    if (pScreen->m_fDrawFrameA8 != 0) SetLevelScreenStatusIndicatorMode(2, 0);
    else SetLevelScreenStatusIndicatorMode(0, 0);
    if (pScreen->m_cMappedListEntriesA0 > 0) {
        pMappedList = (MappedListResourceTable *)AllocateVSMemBlock(
            sizeof(MappedListResourceTable));
        if (pMappedList != 0) {
            pScreen->m_pMappedListResources98 = new (pMappedList)
                MappedListResourceTable(
                    0x2b6, 1, pScreen->m_cMappedListEntriesA0,
                    pScreen->m_nMappedListModeA4);
        } else {
            pScreen->m_pMappedListResources98 = 0;
        }
    }
    RefreshFramedScreenFrameResourcesThunk(pScreen);
    if (pScreen->m_nTimedVariantResourceId394 != 0) {
        pSelector = (GameProgressiveVariantFrameSelector *)
            AllocateVSMemBlock(sizeof(GameProgressiveVariantFrameSelector));
        if (pSelector == 0) {
            pScreen->m_pTimedFrameSelector34C = 0;
        } else {
            pSelector->m_cFrames =
                ((LevelVariantRenderManagerView *)
                    pScreen->m_abVariantRenderManager14)
                        ->GetVariantRenderFrameCount(
                            pScreen->m_nTimedVariantResourceId394);
            pSelector->m_pVtable = g_MenuSelectorFrameSelectorCtorVtable;
            pSelector->m_pVtable = g_MenuSelectorTimedFrameSelectorVtable;
            pSelector->m_nDirection = 1;
            pSelector->m_pVtable =
                g_MenuSelectorProgressiveFrameSelectorVtable;
            pScreen->m_pTimedFrameSelector34C = pSelector;
        }
        pSelector = (GameProgressiveVariantFrameSelector *)
            pScreen->m_pTimedFrameSelector34C;
        pSelector->m_dwFixedTime = -1;
        ((GameProgressiveSelectorVtableView *)pSelector)->SetDuration(500);
        dwNow = timeGetTime();
        pScreen->m_dwRandomDelay33C = 0;
        pScreen->m_dwRandomStart340 = (int)dwNow;
    }
    g_pActiveNetworkLobbyScreen = pScreen;
    if (pScreen->m_fNetworkScreen8C != 0) {
        pScreen->m_fNetworkReady94 = 0;
        pScreen->m_nNetworkState90 = 1;
        if (pScreen->m_pQueuedVariantManager338 != 0) {
            ((GameQueuedVariantManagerView *)
                pScreen->m_pQueuedVariantManager338)
                    ->SetChildrenEnabledThunk(0);
            ((GameQueuedVariantManagerView *)
                pScreen->m_pQueuedVariantManager338)->m_nCurrentChild144 = 0;
        }
        switch (pScreen->m_nScreenType370) {
        case 4:
            nRuntimeMode = 1;
            break;
        case 5:
            nRuntimeMode = 3;
            break;
        case 14:
        case 15:
            nRuntimeMode = 2;
            break;
        }
        if (nRuntimeMode != 0) {
            pRuntime = (VsNetRuntimeModeView *)g_pNetworkLobbyVsnetRuntime;
            pRuntime->m_nMode48 = nRuntimeMode;
            pRuntime->m_nModeState4C = 0;
        }
    }
}

// FUNCTION: LEMBALL 0x004458e0
static void LEMBALL_FASTCALL RefreshFramedScreenRenderEntryGroup(
    void *pObject)

{
    GameFramedScreenBase *pScreen;
    unsigned char *pGroup;
    int fQueuedVariantChildrenEnabled;
    int fSelectorChildrenEnabled;

    pScreen = (GameFramedScreenBase *)pObject;
    fQueuedVariantChildrenEnabled = 0;
    fSelectorChildrenEnabled = 0;
    pScreen->m_fSelectorDirty360 = 0;
    pScreen->m_fBuildingRenderEntries364 = 1;

    if (pScreen->m_pQueuedVariantManager338 != 0) {
        fQueuedVariantChildrenEnabled =
            ((GameQueuedVariantManagerView *)
                 pScreen->m_pQueuedVariantManager338)
                ->m_fChildrenEnabled148;
        if (fQueuedVariantChildrenEnabled != 0) {
            ((GameQueuedVariantManagerView *)
                 pScreen->m_pQueuedVariantManager338)
                ->SetChildrenEnabledThunk(0);
        }
    }
    if (pScreen->m_pSelectorManager334 != 0) {
        fSelectorChildrenEnabled =
            ((GameMenuSelectorManager *)pScreen->m_pSelectorManager334)
                ->m_fChildrenEnabled278;
        if (fSelectorChildrenEnabled != 0) {
            SetMenuSelectorChildrenEnabledThunk(
                (GameMenuSelectorManager *)pScreen->m_pSelectorManager334,
                0, 0);
        }
    }
    if (pScreen->m_fRenderEntriesInitialized368 == 0) {
        pScreen->m_fRenderEntriesInitialized368 = 1;
    }
    SetStatusIndicatorActive(
        g_pArrowCursorStatusIndicatorRenderClient, 0, 0);
    ((GameNoArgVirtual)(*(void ***)pScreen->m_pPrimaryContext84)[44])(
        pScreen->m_pPrimaryContext84, 0);

    pGroup = pScreen->m_abRenderEntryGroupC8;
    *(short *)(pGroup + 4) = 0;
    *(short *)(pGroup + 6) = 0;
    *(short *)(pGroup + 8) = pScreen->m_cxScreen37C;
    *(short *)(pGroup + 10) = pScreen->m_cyScreen37E;
    *(short *)(pGroup + 12) = 0;
    *(short *)(pGroup + 14) = 0;
    ((GameMainMenuRenderEntryInterface *)pGroup)
        ->AppendToQueue(pScreen->m_pRenderContext88);

    pScreen->m_fBuildingRenderEntries364 = 0;
    if (pScreen->m_pQueuedVariantManager338 != 0 &&
        fQueuedVariantChildrenEnabled != 0) {
        ((GameQueuedVariantManagerView *)
             pScreen->m_pQueuedVariantManager338)
            ->SetChildrenEnabledThunk(1);
    }
    if (pScreen->m_pSelectorManager334 != 0 &&
        fSelectorChildrenEnabled != 0) {
        SetMenuSelectorChildrenEnabledThunk(
            (GameMenuSelectorManager *)pScreen->m_pSelectorManager334,
            0, 1);
    }
    SetStatusIndicatorActive(
        g_pArrowCursorStatusIndicatorRenderClient, 0, 1);
}

// FUNCTION: LEMBALL 0x00445a40
static void LEMBALL_FASTCALL RefreshAndDrawFramedScreenBase(
    void *pObject, int, void *)

{
    GameFramedScreenBase *pScreen;
    char *pGeometryOwner;
    char *pGeometryDispatch;
    int nDispatchOffset;

    pScreen = (GameFramedScreenBase *)pObject;
    if (pScreen->m_pRenderContext88 == 0) {
        return;
    }
    pGeometryOwner =
        *(char **)((char *)pScreen->m_pRenderContext88 + 0x0c);
    nDispatchOffset =
        *(int *)((char *)*(void **)(pGeometryOwner + 0x40) + 4);
    pGeometryDispatch = pGeometryOwner + 0x40 + nDispatchOffset;
    ((GameNoArgVirtual)(*(void ***)pGeometryDispatch)[14])(
        pGeometryDispatch, 0);
    pScreen->m_iRenderEntryGroup328 = 0;
    if (pScreen->m_pSelectorManager334 != 0) {
        if (ConsumeLevelSelectionChangedFlag() != 0 ||
            pScreen->m_fSelectorDirty360 != 0) {
            pScreen->m_fSelectorDirty360 = 1;
        } else {
            pScreen->m_fSelectorDirty360 = 0;
        }
    }
    if (pScreen->m_fSelectorDirty360 != 0) {
        RefreshFramedScreenRenderEntryGroup(pScreen);
    }
    DrawFramedScreenBase(pScreen);
    pScreen->m_fFrameDirty330 = 0;
}

// FUNCTION: LEMBALL 0x00445ac0
static void LEMBALL_FASTCALL DrawFramedScreenBase(void *pObject) {
    GameFramedScreenBase *pScreen;
    void *pUploadOwner;
    int nUploadState;
    unsigned char *pGroup;

    pScreen = (GameFramedScreenBase *)pObject;
    pUploadOwner = *(void **)((char *)pScreen->m_pRenderContext88 + 0x0c);
    nUploadState = ((GameIntNoArgVirtual)(*(void ***)pUploadOwner)[2])(
        pUploadOwner, 0);
    InitializeHelperUploadStatePending(nUploadState);
    pGroup = pScreen->m_abRenderEntryGroupC8;
    AppendPointerQueueEntry(pScreen->m_pRenderContext88, pGroup + 0x1bc);

    if (pScreen->m_fBuildingRenderEntries364 != 0) {
        DrawFramedScreenFrameLayer(pScreen);
        ((GameNoArgVirtual)pScreen->m_pVtable00[20])(pScreen, 0);
    }
    ((GameNoArgVirtual)pScreen->m_pVtable00[19])(pScreen, 0);
}

// FUNCTION: LEMBALL 0x00445c10
static void LEMBALL_FASTCALL DrawFramedScreenFrameLayer(void *pObject) {
    GameFramedScreenBase *pScreen;
    unsigned char *pGroup;
    short cxTile;
    short cyTile;
    int x;
    int y;
    int cColumns;
    int cRows;
    int iEntry;

    pScreen = (GameFramedScreenBase *)pObject;
    pGroup = pScreen->m_abRenderEntryGroupC8;
    if (pScreen->m_fTileBackgroundAC != 0 &&
        pScreen->m_pBackgroundBitmapC4 != 0) {
        cxTile = *(short *)((char *)pScreen->m_pBackgroundBitmapC4 + 0x48);
        cyTile = *(short *)((char *)pScreen->m_pBackgroundBitmapC4 + 0x4a);
        cColumns = (pScreen->m_cxScreen37C + cxTile - 1) / cxTile;
        cRows = (pScreen->m_cyScreen37E + cyTile - 1) / cyTile;
        iEntry = 0;
        for (y = 0; y < cRows; ++y) {
            for (x = 0; x < cColumns + ((y + 1) & 1); ++x) {
                unsigned char *pEntry;
                pEntry = pGroup + 0x34 + iEntry * 0x24;
                *(short *)(pEntry + 4) =
                    (short)(x * cxTile - (cxTile / 2) * ((y + 1) & 1));
                *(short *)(pEntry + 6) = (short)(y * cyTile);
                *(void **)(pEntry + 0x10) = pScreen->m_pBackgroundBitmapC4;
                *(int *)(pEntry + 0x14) = 0;
                *(int *)(pEntry + 0x18) = 0;
                ((GameMainMenuRenderEntryInterface *)pEntry)
                    ->AppendToQueue(pScreen->m_pRenderContext88);
                ++iEntry;
            }
        }
    }

    if (pScreen->m_fDrawForegroundB0 != 0) {
        unsigned char *pEntry;
        pEntry = pGroup + 0x10;
        ((GameMainMenuRenderEntryInterface *)pEntry)
            ->AppendToQueue(pScreen->m_pRenderContext88);
    }

    if (pScreen->m_nAppliedCursorResourceIdB8 !=
        pScreen->m_nCursorResourceIdB4) {
        ((GameOneArgVirtual)(*(void ***)pScreen->m_pPrimaryContext84)[43])(
            pScreen->m_pPrimaryContext84, 0,
            (void *)pScreen->m_nCursorResourceIdB4);
        pScreen->m_nAppliedCursorResourceIdB8 =
            pScreen->m_nCursorResourceIdB4;
    }
}

// FUNCTION: LEMBALL 0x00445e70
static void LEMBALL_FASTCALL RefreshFramedScreenFrameResources(void *pObject) {
    GameFramedScreenBase *pScreen;
    int fWindowReady;

    pScreen = (GameFramedScreenBase *)pObject;
    fWindowReady = IsPrimaryContextWindowReadyForPresent(
        (int *)pScreen->m_pPrimaryContext84);
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        ((GameNoArgVirtual)pScreen->m_pVtable00[17])(pScreen, 0);
        ReleaseFramedScreenFrameResources(pScreen);
    }
    pScreen->m_fCompactLayout32C = g_fCompactPrimaryContextLayout;
    if (fWindowReady != 0) {
        LoadFramedScreenFrameResources(pScreen);
        ((GameNoArgVirtual)pScreen->m_pVtable00[16])(pScreen, 0);
        pScreen->m_fSelectorDirty360 = 1;
    }
}

// FUNCTION: LEMBALL 0x00445ed0
static void LEMBALL_FASTCALL LoadFramedScreenFrameResources(void *pObject) {
    GameFramedScreenBase *pScreen;

    pScreen = (GameFramedScreenBase *)pObject;
    pScreen->m_fFrameResourcesLoaded9C = 1;
    if (pScreen->m_fCompactLayout32C == 0) {
        pScreen->m_pBackgroundBitmapC4 = LoadBitmapResource(0x131);
        pScreen->m_pForegroundBitmapC0 = LoadBitmapResource(0x132);
        pScreen->m_nMappedListResourceId38C = 0x151;
        pScreen->m_nMappedListRemapResourceId384 = 0x113;
        pScreen->m_nFrameResourceId388 = 0x150;
        pScreen->m_nFrameResourceId390 = 0x152;
    } else {
        pScreen->m_pBackgroundBitmapC4 = LoadBitmapResource(0x13c);
        pScreen->m_pForegroundBitmapC0 = LoadBitmapResource(0x13d);
        pScreen->m_nMappedListResourceId38C = 0x17e;
        pScreen->m_nMappedListRemapResourceId384 = 0x114;
        pScreen->m_nFrameResourceId388 = 0x17d;
        pScreen->m_nFrameResourceId390 = 0x17f;
    }

    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              pScreen->m_nFrameResourceId388);
    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              pScreen->m_nMappedListResourceId38C);
    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              pScreen->m_nFrameResourceId390);
}

// FUNCTION: LEMBALL 0x00445fe0
static void LEMBALL_FASTCALL ReleaseFramedScreenFrameResources(void *pObject) {
    GameFramedScreenBase *pScreen;
    LevelVariantRenderManagerView *pVariantManager;

    pScreen = (GameFramedScreenBase *)pObject;
    pVariantManager = (LevelVariantRenderManagerView *)
        pScreen->m_abVariantRenderManager14;
    ReleaseTypedResourceObjectReference(pScreen->m_pForegroundBitmapC0);
    ReleaseTypedResourceObjectReference(pScreen->m_pBackgroundBitmapC4);
    pVariantManager->ReleaseVariantRenderResource(
        pScreen->m_nFrameResourceId388);
    pVariantManager->ReleaseVariantRenderResource(
        pScreen->m_nMappedListResourceId38C);
    pVariantManager->ReleaseVariantRenderResource(
        pScreen->m_nFrameResourceId390);
    pScreen->m_fFrameResourcesLoaded9C = 0;
}

// FUNCTION: LEMBALL 0x004460a0
static void LEMBALL_FASTCALL ResetFramedScreenVariantBuffers(
    void *pObject)

{
    GameFramedScreenBase *pScreen;

    pScreen = (GameFramedScreenBase *)pObject;
    ResetOrFlipVariantRenderBuffers(
        (int)(unsigned long)pScreen->m_abVariantRenderManager14);
    pScreen->m_iPackagedEntry36C = 0;
}

// FUNCTION: LEMBALL 0x00446480
static int LEMBALL_FASTCALL HandleFramedScreenQueueEvent(
    void *pQueueNode, int, void *pEvent)

{
    GameFramedScreenBase *pScreen;

    pScreen = (GameFramedScreenBase *)((char *)pQueueNode - 4);
    if (*(int *)((char *)pQueueNode + 0x8c) != 0) {
        return 0;
    }
    if (*(int *)((char *)pEvent + 4) -
            *(int *)((char *)pQueueNode + 0xb8) <
        0) {
        return 0;
    }
    return ((GameIntOneArgVirtual)pScreen->m_pVtable00[15])(
               pScreen, 0, pEvent) != 0;
}

// FUNCTION: LEMBALL 0x004464d0
static void LEMBALL_FASTCALL UpdateFramedScreenRuntime(void *pObject) {
    GameFramedScreenBase *pScreen;
    DWORD dwNow;

    pScreen = (GameFramedScreenBase *)pObject;
    if (pScreen->m_pTimedFrameSelector34C != 0) {
        dwNow = timeGetTime();
        if (pScreen->m_dwRandomDelay33C + 1000U <
            dwNow - pScreen->m_dwRandomStart340) {
            pScreen->m_dwRandomStart340 = dwNow;
            ((GameOneArgVirtual)
                 (*(void ***)pScreen->m_pTimedFrameSelector34C)[3])(
                pScreen->m_pTimedFrameSelector34C, 0,
                (void *)(unsigned long)dwNow);
            if (g_pSessionRandomState != 0) {
                unsigned int uRandom;
                uRandom = (*(unsigned int *)g_pSessionRandomState * 0x29U +
                           0x1fU) & 0x7fffffU;
                *(unsigned int *)g_pSessionRandomState = uRandom;
                pScreen->m_dwRandomDelay33C = uRandom % 6000U;
            }
        }
    }
    ((GameNoArgVirtual)pScreen->m_pVtable00[14])(pScreen, 0);
}

// FUNCTION: LEMBALL 0x004466e0
static void LEMBALL_FASTCALL RecenterFramedScreenIfNeeded(void *pObject) {
    GameFramedScreenBase *pScreen;
    GamePrimaryContext *pPrimaryContext;
    short aRect[4];

    pScreen = (GameFramedScreenBase *)pObject;
    if (((GameIntNoArgVirtual)(*(void ***)pScreen->m_pPrimaryContext84)[26])(
            pScreen->m_pPrimaryContext84, 0) != 0) {
        pPrimaryContext = (GamePrimaryContext *)pScreen->m_pPrimaryContext84;
        pPrimaryContext->ComputePrimaryContextCenteredScreenRect(aRect, -1, -1);
        ((GameOneArgVirtual)(*(void ***)pPrimaryContext)[7])(
            pPrimaryContext, 0, aRect);
    }
}

// FUNCTION: LEMBALL 0x00446fd0
static void LEMBALL_FASTCALL SetFramedScreenSize(
    void *pObject, int, short *paSize)

{
    GameFramedScreenBase *pScreen;

    pScreen = (GameFramedScreenBase *)pObject;
    pScreen->m_cxScreen37C = paSize[0];
    pScreen->m_cyScreen37E = paSize[1];
    RefreshFramedScreenFrameResourcesThunk(pScreen);
}

// FUNCTION: LEMBALL 0x004477b0
void GameIntroSequenceMediaState::ConfigureOwner(
    void *pScreen, void *pPrimaryContext, int fSkipVariant)
{
    m_fSkipVariantC4 = fSkipVariant;
    m_pScreenBC = pScreen;
    m_pPrimaryContextC0 = pPrimaryContext;
}

// FUNCTION: LEMBALL 0x00448300
static void LEMBALL_FASTCALL InitializeMainMenuWidgets(void *pObject) {
    GameMainMenuScreenView *pScreen;
    unsigned char *pForegroundEntry;
    short cxForeground;

    pScreen = (GameMainMenuScreenView *)pObject;
    pScreen->m_panWidgetLayout39C =
        pScreen->m_fCompactLayout32C == 0
            ? g_GAME_MainMenuWidgetLayoutStandard
            : g_GAME_MainMenuWidgetLayoutCompact;

    pForegroundEntry = pScreen->m_abRenderEntryGroupC8 + 0x14;
    cxForeground = *(short *)((char *)pScreen->m_pForegroundBitmapC0 + 0x48);
    *(short *)(pForegroundEntry + 0) =
        (short)((pScreen->m_cxScreen37C - cxForeground) / 2);
    *(short *)(pForegroundEntry + 2) = 0;
    *(void **)(pForegroundEntry + 0x0c) = pScreen->m_pForegroundBitmapC0;
    *(int *)(pForegroundEntry + 0x10) = 0x800;
    *(int *)(pForegroundEntry + 0x14) = 0;

    /* The six child records are owned by the 0x44c870 selector hierarchy.
       Keep the evidenced boundary explicit until that independently owned
       constructor is linked into this startup path. */
    pScreen->m_pSelectorManager334 = 0;
}

// FUNCTION: LEMBALL 0x00448540
static void LEMBALL_FASTCALL DestroyMainMenuWidgets(void *pObject) {
    GameMainMenuScreenView *pScreen;

    pScreen = (GameMainMenuScreenView *)pObject;
    if (pScreen->m_pSelectorManager334 != 0) {
        ((GameMainMenuSelectorManagerInterface *)
             pScreen->m_pSelectorManager334)
            ->Delete((void *)1);
    }
}

// FUNCTION: LEMBALL 0x00448560
static void LEMBALL_FASTCALL DestroyMainMenuScreen(void *pObject) {
    GameMainMenuScreenView *pScreen;
    GameLevelProgressStateView *pProgress;

    pScreen = (GameMainMenuScreenView *)pObject;
    pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
    pScreen->m_pVtable00 = g_GAME_MainMenuScreenVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_MainMenuQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    if ((unsigned int)pScreen->m_nPackSelection3B4 < 4U) {
        pProgress->m_nCurrentLevel00 =
            pProgress->m_anSnapshotPackCaps3C[pScreen->m_nPackSelection3B4];
        pProgress->m_nCurrentPack08 = pScreen->m_nPackSelection3B4;
    }
    g_nLastMainMenuPackSelection = pScreen->m_nPackSelection3B4;
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        DestroyMainMenuWidgets(pScreen);
        ReleaseFramedScreenFrameResources(pScreen);
    }
    g_pActiveNetworkLobbyScreen = 0;
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(
                pScreen->m_abRenderQueueNode04, 0);
    }
    DestroyVariantRenderManager(pScreen->m_abVariantRenderManager14);
    ((MogLoadResourceArchive *)g_pMainResourceArchive)
        ->PruneUnreferencedCachedResourceObjects();
}

// FUNCTION: LEMBALL 0x00448620
static int LEMBALL_FASTCALL HandleMainMenuInput(
    void *pObject, int, void *pEvent)

{
    GameMainMenuScreenView *pScreen;
    GameLevelProgressStateView *pProgress;
    unsigned short nType;
    int nCommand;

    pScreen = (GameMainMenuScreenView *)pObject;
    pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
    nType = *(unsigned short *)pEvent;
    if (nType < 3 || nType > 4) {
        if (nType != 12) {
            ++*(int *)((char *)pScreen + 0x10);
            return 0;
        }
    }
    pScreen->m_dwTimeout398 = timeGetTime() + 20000U;
    if (nType != 12) {
        return 0;
    }

    nCommand = *(int *)((char *)pEvent + 8);
    switch ((unsigned int)nCommand) {
    case 0xacef0001U:
        pScreen->m_nRequestedMode374 = 3;
        break;
    case 0xacef00a4U:
        pScreen->m_nRequestedMode374 = 0x10;
        break;
    case 0xacef00a5U:
        TogglePrimaryContextDisplayMode(
            (ShellUiPrimaryContextShell *)pScreen->m_pPrimaryContext84);
        return 1;
    case 0xacef00a6U:
    case 0xacef00a7U:
        pProgress->m_nCurrentLevel00 =
            pProgress->m_anSnapshotPackCaps3C[pScreen->m_nPackSelection3B4];
        pProgress->m_nCurrentPack08 = pScreen->m_nPackSelection3B4;
        pScreen->m_nRequestedMode374 =
            nCommand == (int)0xacef00a6U ? 4 : 0x0c;
        break;
    default:
        return 0;
    }

    pScreen->m_nTransitionStatus378 = 1;
    g_fNextMainMenuTimeoutIsTitle = 1;
    return 1;
}

// FUNCTION: LEMBALL 0x00448800
static void LEMBALL_FASTCALL UpdateMainMenuScreen(void *pObject) {
    GameMainMenuScreenView *pScreen;
    GameLevelProgressStateView *pProgress;
    DWORD dwNow;
    int fReady;

    pScreen = (GameMainMenuScreenView *)pObject;
    pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
    if (g_fStartupTestAllLevels != 0) {
        pProgress->m_nCurrentLevel00 = pProgress->m_anSnapshotPackCaps3C[0];
        pProgress->m_nCurrentPack08 = 0;
        pScreen->m_nTransitionStatus378 = 1;
        pScreen->m_nRequestedMode374 = 4;
    }

    dwNow = timeGetTime();
    fReady = IsPrimaryContextWindowReadyForPresent(
        (int *)pScreen->m_pPrimaryContext84);
    if (fReady != 0) {
        fReady = ((GameIntNoArgVirtual)
                      (*(void ***)pScreen->m_pPrimaryContext84)[24])(
            pScreen->m_pPrimaryContext84, 0);
    }
    if (fReady == 0) {
        pScreen->m_dwTimeout398 = timeGetTime() + 20000U;
        return;
    }
    if (dwNow <= pScreen->m_dwTimeout398) {
        return;
    }

    pScreen->m_nTransitionStatus378 = 1;
    if (g_fNextMainMenuTimeoutIsTitle == 0 &&
        g_fSkipStartupSequence == 0) {
        pScreen->m_nRequestedMode374 = 1;
        g_fNextMainMenuTimeoutIsTitle = 1;
    } else {
        pScreen->m_nRequestedMode374 = 0x13;
        g_fNextMainMenuTimeoutIsTitle = 0;
    }
}

// FUNCTION: LEMBALL 0x00448a70
static void *LEMBALL_FASTCALL DeleteMainMenuScreen(
    void *pObject, int, int fDelete)

{
    DestroyMainMenuScreenThunk00401190(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00448aa0
static void *LEMBALL_FASTCALL DeleteAdjustedMainMenuScreen(
    void *pObject, int nUnusedEdx, int fDelete)
{
    return DeleteMainMenuScreen((char *)pObject - 4, nUnusedEdx, fDelete);
}

// FUNCTION: LEMBALL 0x0044c440
static int ConsumeLevelSelectionChangedFlag(void) {
    if (g_fLevelSelectionChanged != 0) {
        g_fLevelSelectionChanged = 0;
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0044fbd0
void GameQueuedVariantManagerView::SetChildrenEnabled(int fEnabled) {
    int i;
    GameQueuedVariantChildView *pChild;

    m_fChildrenEnabled148 = fEnabled;
    for (i = 0; i < m_cChildren80; ++i) {
        pChild = m_apChildrenAC[i];
        if (pChild != 0) {
            pChild->m_fEnabled54 = fEnabled;
            ((GameGeometryOwnerRectState *)((char *)
                pChild->m_pOverlay4C + 0x90))->SetEnabled(fEnabled);
        }
    }
}
