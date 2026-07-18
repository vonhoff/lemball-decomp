#include "FRONTEND/MAINMENU.H"

#include "ENGINE/GDI/VSGDI.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/VSINIT.H"
#include "GAME.H"
#include "SHELL/SHELLUI.H"
#include "SHELL/OPTIONS.H"
#include "RESOURCE/MOGLOAD.H"
#include "RESOURCE/VARMGR.H"
#include "LEVEL/DRAWTEXT.H"
#include "FRONTEND/MENUSEL.H"
#include "ENGINE/GDI/VSGEOM.H"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <string.h>

extern "C" DWORD WINAPI timeGetTime(void);

extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void VariantResourceBundleNoopThunk(void);
extern void *LoadBitmapResource(int nResourceId);
extern int *LoadBinResource(int nResourceId);
extern void LoadVariantRenderResource(void *pManager, int nResourceId);
extern void EmitVariantRenderResourceEntry(
    void *pManager, short *pRect, const short *pPoint,
    int nResourceId, int nFlags, void *pFrameSelector, int nValue);
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
extern void LEMBALL_FASTCALL DestroyFramedScreenRenderEntryGroup(
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
extern void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntryThunk(
    void *pObject);
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
extern void *g_pVariantResourceEntryManager;
extern void SetVariantResourceEffectsEnabled(int fEnabled);

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

struct GameLevelMetadataView {
    unsigned short m_cSkill00;
    unsigned short m_cPlayers02;
    unsigned short m_cNetworkPlayers04;
    unsigned short m_nTimeSeconds06;
    char m_szTitle08[0x24];
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

struct GameOptionsScreenView : public GameFramedScreenBase {
    int m_fZoomDisabled398;
    int m_nBackSelection39C;
    int m_nReserved3A0;
    int m_fSkipStartupSequence3A4;
    const int *m_panWidgetLayout3A8;
};

struct GamePaintballSequenceScreenView : public GameFramedScreenBase {
    unsigned char m_abPrimaryBitmapEntry398[0x48];
    unsigned char m_abSecondaryBitmapEntry3E0[0x118];
    char *m_pszPassword4F8;
    void *m_pPrimaryBitmap4FC;
    void *m_pSecondaryBitmap500;
    int m_nPrimaryBitmapResourceId504;
    int m_nButtonResourceId508;
    int m_nSecondaryBitmapResourceId50C;
    const int *m_pLayout510;
    int m_nActionSelection514;
    int m_fSecondVariant518;
    int m_fEffectStarted51C;
    unsigned char m_abMediaOwner520[0xd8];
    DWORD m_dwMediaStart5F8;
    int m_fMediaStarted5FC;
    DWORD m_dwEffectStart600;
    int m_fMediaEnabled604;
    int m_nReserved608;
};

struct GameLevelSelectionScreenView : public GameFramedScreenBase {
    unsigned char m_abRenderChild398[0x24];
    void *m_pLevelBitmap3BC;
    GameProgressiveVariantFrameSelector *m_pSelector3C0;
    GameProgressiveVariantFrameSelector *m_pSelector3C4;
    GameProgressiveVariantFrameSelector *m_pSelector3C8;
    unsigned char *m_pPaletteRemap3CC;
    int m_nPaletteRemap3D0;
    const int *m_pLayout3D4;
    int m_nResourceId3D8;
    int m_nResourceId3DC;
    int m_nResourceId3E0;
    const int *m_pNextResourceIds3E4;
    const int *m_pPreviousResourceIds3E8;
    int m_cStatus3EC;
    int m_cNetworkStatus3F0;
    int m_cSkillStatus3F4;
    int m_nLevelTime3F8;
    char m_aszTitleLines3FC[3][0x20];
    char m_abTime45C[8];
    int m_aTitlePositions464[3][2];
    int m_nReserved47C;
    int m_fNextLocked480;
    int m_fPreviousLocked484;
};

struct GamePasswordEntryScreenView : public GameFramedScreenBase {
    int m_nPanelResourceId398;
    unsigned char m_abReserved39C[0x0c];
    const int *m_pLayout3A8;
    const int *m_pKeyResourceIds3AC;
    char m_szPassword3B0[0x10];
    int m_aKeyPositions3C0[12][2];
    int m_cPassword420;
    void *m_apKeyOverlays424[12];
    unsigned char m_abInputPanelContext454[0x24];
    int m_xSelected478;
    int m_ySelected47C;
    int m_nSelectedKey480;
    int m_fPasswordValid484;
    int m_nResultState488;
    DWORD m_dwResultStart48C;
    DWORD m_dwResultDeadline490;
};

typedef char GameFramedScreenBase_size_must_be_0x398[
    sizeof(GameFramedScreenBase) == 0x398 ? 1 : -1];
typedef char GameMainMenuScreenView_size_must_be_0x3bc[
    sizeof(GameMainMenuScreenView) == 0x3bc ? 1 : -1];
typedef char GameOptionsScreenView_size_must_be_0x3ac[
    sizeof(GameOptionsScreenView) == 0x3ac ? 1 : -1];
typedef char GamePaintballSequenceScreenView_size_must_be_0x60c[
    sizeof(GamePaintballSequenceScreenView) == 0x60c ? 1 : -1];
typedef char GameLevelSelectionScreenView_size_must_be_0x488[
    sizeof(GameLevelSelectionScreenView) == 0x488 ? 1 : -1];
typedef char GamePasswordEntryScreenView_size_must_be_0x494[
    sizeof(GamePasswordEntryScreenView) == 0x494 ? 1 : -1];

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
static void *LEMBALL_FASTCALL DeleteFunctionalLevelSelectionScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static int LEMBALL_FASTCALL HandleFunctionalLevelSelectionInput(
    void *pObject, int nUnusedEdx, void *pEvent);
static void LEMBALL_FASTCALL UpdateFunctionalLevelSelectionScreen(
    void *pObject);
static void LEMBALL_FASTCALL InitializeFunctionalLevelSelectionResources(
    void *pObject);
static void LEMBALL_FASTCALL ReleaseFunctionalLevelSelectionResources(
    void *pObject);
static void LEMBALL_FASTCALL NoopFunctionalLevelSelectionHook(
    void *pObject);
static void LEMBALL_FASTCALL DrawFunctionalLevelSelectionStatus(
    void *pObject);
static void LEMBALL_FASTCALL DrawFunctionalLevelSelectionText(
    void *pObject);
static void RefreshFunctionalLevelSelectionMetadata(
    GameLevelSelectionScreenView *pScreen);
static int ReadFunctionalLevelMetadata(
    int nSkill, int nLevel, GameLevelMetadataView *pMetadata);
static void SelectFunctionalLevel(
    GameLevelSelectionScreenView *pScreen, int nDirection);
static GameProgressiveVariantFrameSelector *
    ConstructLevelSelectionFrameSelector(void *pManager, int nResourceId);
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
static void LEMBALL_FASTCALL InitializeOptionsScreenWidgets(void *pObject);
static void LEMBALL_FASTCALL ReleaseOptionsScreenWidgets(void *pObject);
static int LEMBALL_FASTCALL HandleOptionsScreenInput(
    void *pObject, int nUnusedEdx, void *pEvent);
static void *LEMBALL_FASTCALL DeleteOptionsScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteAdjustedOptionsScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static void LEMBALL_FASTCALL InitializePaintballSequenceResources(void *pObject);
static void LEMBALL_FASTCALL ReleasePaintballSequenceResources(void *pObject);
static void LEMBALL_FASTCALL DrawPaintballSequenceFrame(void *pObject);
static void LEMBALL_FASTCALL UpdatePaintballSequenceScreen(void *pObject);
static int LEMBALL_FASTCALL HandlePaintballSequenceInput(
    void *pObject, int nUnusedEdx, void *pEvent);
static void *LEMBALL_FASTCALL DeletePaintballSequenceScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteAdjustedPaintballSequenceScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static int LEMBALL_FASTCALL HandlePasswordEntryInput(
    void *pObject, int nUnusedEdx, void *pEvent);
static void LEMBALL_FASTCALL UpdatePasswordEntryScreen(void *pObject);
static void LEMBALL_FASTCALL InitializePasswordEntryResources(void *pObject);
static void LEMBALL_FASTCALL ReleasePasswordEntryResources(void *pObject);
static void LEMBALL_FASTCALL DrawPasswordEntryText(void *pObject);
static void *LEMBALL_FASTCALL DeletePasswordEntryScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteAdjustedPasswordEntryScreen(
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

// GLOBAL: LEMBALL 0x0049f578
static const int g_GAME_OptionsZoomResourcesStandard[2] = {0x1b5, 0x1b4};
static const int g_GAME_OptionsEffectsResourcesStandard[2] = {0x1d5, 0x1d2};
static const int g_GAME_OptionsMusicResourcesStandard[2] = {0x1d3, 0x1d4};
static const int g_GAME_OptionsBackResourcesStandard[2] = {0x1b3, 0x1ab};
static const int g_GAME_OptionsIntroResourcesStandard[2] = {0x1bb, 0x1bc};
static const int g_GAME_OptionsZoomResourcesCompact[2] = {0x1e9, 0x1e8};
static const int g_GAME_OptionsEffectsResourcesCompact[2] = {0x208, 0x205};
static const int g_GAME_OptionsMusicResourcesCompact[2] = {0x206, 0x207};
static const int g_GAME_OptionsBackResourcesCompact[2] = {0x1e7, 0x1df};
static const int g_GAME_OptionsIntroResourcesCompact[2] = {0x1ef, 0x1f0};
static const int g_GAME_OptionsLayoutStandard[12] = {
    0x20, 0x74, 0x1e0, 0x74, 0x30, 0xe8,
    0x1d0, 0xe8, 0x40, 0x15c, 0x1a0, 0x15c
};
static const int g_GAME_OptionsLayoutCompact[12] = {
    0x10, 0x3a, 0xf0, 0x3a, 0x18, 0x74,
    0xe8, 0x74, 0x20, 0xae, 0xd0, 0xae
};

// GLOBAL: LEMBALL 0x004a7770
int g_nOptionsEffectsSliderValue;
// GLOBAL: LEMBALL 0x004a7774
int g_nOptionsMusicSliderValue;

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

static void *g_GAME_OptionsScreenVtable[22];
static void *g_GAME_OptionsQueueVtable[4];
static int g_fOptionsScreenVtableInitialized;
static void *g_GAME_PaintballSequenceScreenVtable[22];
static void *g_GAME_PaintballSequenceQueueVtable[4];
static int g_fPaintballSequenceVtableInitialized;
static void *g_GAME_PasswordEntryScreenVtable[22];
static void *g_GAME_PasswordEntryQueueVtable[4];
static int g_fPasswordEntryVtableInitialized;

static const int g_GAME_PasswordLayoutStandard[32] = {
    0, 0, 384, 32, 16, 16, 16, 176,
    16, 208, 16, 240, 16, 272, 192, 176,
    192, 208, 192, 240, 192, 272, 64, 368,
    64, 64, 320, 368, 320, 40, 32, 400
};
static const int g_GAME_PasswordLayoutCompact[32] = {
    0, 0, 192, 16, 8, 8, 8, 88,
    8, 104, 8, 120, 8, 136, 96, 88,
    96, 104, 96, 120, 96, 136, 32, 184,
    32, 32, 160, 184, 160, 24, 16, 200
};
static const int g_GAME_PasswordResourcesStandard[12] = {
    0x159, 0x15a, 0x15b, 0x15c, 0x15d, 0x15e,
    0x15f, 0x160, 0x161, 0x162, 0x163, 0x164
};
static const int g_GAME_PasswordResourcesCompact[12] = {
    0x186, 0x187, 0x188, 0x189, 0x18a, 0x18b,
    0x18c, 0x18d, 0x18e, 0x18f, 0x190, 0x191
};
static const int g_GAME_PasswordKeyPermutation[12] = {
    7, 8, 9, 4, 5, 6, 1, 2, 3, 10, 0, 11
};

static void InitializeFunctionalOptionsVtables(void) {
    int i;

    if (g_fOptionsScreenVtableInitialized != 0) {
        return;
    }
    for (i = 0; i < 22; ++i) {
        g_GAME_OptionsScreenVtable[i] = g_GAME_MainMenuScreenVtable[i];
    }
    g_GAME_OptionsScreenVtable[0] = (void *)DeleteOptionsScreen;
    g_GAME_OptionsScreenVtable[14] = (void *)MainMenuSlot13Thunk;
    g_GAME_OptionsScreenVtable[15] = (void *)HandleOptionsScreenInput;
    g_GAME_OptionsScreenVtable[16] = (void *)InitializeOptionsScreenWidgets;
    g_GAME_OptionsScreenVtable[17] = (void *)ReleaseOptionsScreenWidgets;
    g_GAME_OptionsQueueVtable[0] =
        (void *)InitializeNonZrleVariantRenderEntry;
    g_GAME_OptionsQueueVtable[1] =
        (void *)DeleteAdjustedOptionsScreen;
    g_GAME_OptionsQueueVtable[2] =
        (void *)HandleFramedScreenQueueEventThunk;
    g_GAME_OptionsQueueVtable[3] = 0;
    g_fOptionsScreenVtableInitialized = 1;
}

static void InitializeFunctionalPaintballSequenceVtables(void) {
    int i;

    if (g_fPaintballSequenceVtableInitialized != 0) {
        return;
    }
    for (i = 0; i < 22; ++i) {
        g_GAME_PaintballSequenceScreenVtable[i] =
            g_GAME_MainMenuScreenVtable[i];
    }
    g_GAME_PaintballSequenceScreenVtable[0] =
        (void *)DeletePaintballSequenceScreen;
    g_GAME_PaintballSequenceScreenVtable[14] =
        (void *)UpdatePaintballSequenceScreen;
    g_GAME_PaintballSequenceScreenVtable[15] =
        (void *)HandlePaintballSequenceInput;
    g_GAME_PaintballSequenceScreenVtable[16] =
        (void *)InitializePaintballSequenceResources;
    g_GAME_PaintballSequenceScreenVtable[17] =
        (void *)ReleasePaintballSequenceResources;
    g_GAME_PaintballSequenceScreenVtable[18] =
        (void *)DrawPaintballSequenceFrame;
    g_GAME_PaintballSequenceQueueVtable[0] =
        (void *)InitializeNonZrleVariantRenderEntry;
    g_GAME_PaintballSequenceQueueVtable[1] =
        (void *)DeleteAdjustedPaintballSequenceScreen;
    g_GAME_PaintballSequenceQueueVtable[2] =
        (void *)HandleFramedScreenQueueEventThunk;
    g_GAME_PaintballSequenceQueueVtable[3] = 0;
    g_fPaintballSequenceVtableInitialized = 1;
}

static void InitializeFunctionalPasswordEntryVtables(void) {
    int i;
    if (g_fPasswordEntryVtableInitialized != 0) return;
    for (i = 0; i < 22; ++i) {
        g_GAME_PasswordEntryScreenVtable[i] = g_GAME_MainMenuScreenVtable[i];
    }
    g_GAME_PasswordEntryScreenVtable[0] = (void *)DeletePasswordEntryScreen;
    g_GAME_PasswordEntryScreenVtable[14] = (void *)UpdatePasswordEntryScreen;
    g_GAME_PasswordEntryScreenVtable[15] = (void *)HandlePasswordEntryInput;
    g_GAME_PasswordEntryScreenVtable[16] = (void *)InitializePasswordEntryResources;
    g_GAME_PasswordEntryScreenVtable[17] = (void *)ReleasePasswordEntryResources;
    g_GAME_PasswordEntryScreenVtable[19] = (void *)DrawPasswordEntryText;
    g_GAME_PasswordEntryQueueVtable[0] =
        (void *)InitializeNonZrleVariantRenderEntry;
    g_GAME_PasswordEntryQueueVtable[1] =
        (void *)DeleteAdjustedPasswordEntryScreen;
    g_GAME_PasswordEntryQueueVtable[2] =
        (void *)HandleFramedScreenQueueEventThunk;
    g_GAME_PasswordEntryQueueVtable[3] = 0;
    g_fPasswordEntryVtableInitialized = 1;
}

static void *g_GAME_FunctionalLevelSelectionVtable[22];
static int g_fFunctionalLevelSelectionVtableInitialized;

static const int g_GAME_LevelSelectionLayoutStandard[54] = {
    25, 375, 178, 375, 331, 375, 484, 375,
    10, 0, 336, 12, 288, 292, 32, 160,
    138, 76, 352, 266, 404, 267, 440, 267,
    352, 203, 404, 219, 440, 203, 352, 153,
    404, 158, 440, 153, 352, 56, 256, 88,
    488, 22, 352, 305, 404, 310, 440, 305,
    288, 352, 15, 0, 0, 221
};

static const int g_GAME_LevelSelectionLayoutCompact[54] = {
    12, 186, 89, 186, 165, 186, 242, 186,
    6, 0, 168, 6, 144, 160, 16, 80,
    69, 38, 176, 135, 202, 135, 220, 135,
    176, 102, 202, 112, 220, 102, 176, 77,
    202, 81, 220, 77, 176, 28, 128, 44,
    244, 12, 176, 153, 202, 157, 220, 153,
    144, 176, 9, 0, 0, 113
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
    pScreen->m_dwConstructionTimeBC =
        ((RenderDispatchQueue *)g_pSharedRenderDispatchQueue)->m_nReserved30;
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

    pGroup = pScreen->m_abRenderEntryGroupC8 +
             pScreen->m_iRenderEntryGroup328 * 0x260;
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
    unsigned char *pEntry;

    pScreen = (GameFramedScreenBase *)pObject;
    pUploadOwner = *(void **)((char *)pScreen->m_pRenderContext88 + 0x0c);
    nUploadState = ((GameIntNoArgVirtual)(*(void ***)pUploadOwner)[2])(
        pUploadOwner, 0);
    InitializeHelperUploadStatePending(nUploadState);
    pGroup = pScreen->m_abRenderEntryGroupC8 +
             pScreen->m_iRenderEntryGroup328 * 0x260;
    AppendPointerQueueEntry(pScreen->m_pRenderContext88, pGroup + 0x1bc);

    if (pScreen->m_fBuildingRenderEntries364 != 0) {
        if (pScreen->m_fTileBackgroundAC == 0) {
            pEntry = pGroup + 0x1c0 +
                     pScreen->m_iPackagedEntry36C * 0x10;
            *(short *)(pEntry + 4) = pScreen->m_cxScreen37C;
            *(short *)(pEntry + 6) = pScreen->m_cyScreen37E;
            *(short *)(pEntry + 8) = 0;
            *(short *)(pEntry + 10) = 0;
            *(int *)(pEntry + 12) = 0;
            ((GameMainMenuRenderEntryInterface *)pEntry)
                ->AppendToQueue(pScreen->m_pRenderContext88);
            ++pScreen->m_iPackagedEntry36C;
        }
        DrawFramedScreenFrameLayer(pScreen);
        ((GameNoArgVirtual)pScreen->m_pVtable00[20])(pScreen, 0);
    } else {
        ((GameNoArgVirtual)pScreen->m_pVtable00[18])(pScreen, 0);
    }
    ((GameNoArgVirtual)pScreen->m_pVtable00[19])(pScreen, 0);
    if (pScreen->m_fBuildingRenderEntries364 == 0 &&
        pScreen->m_pSelectorManager334 != 0) {
        UpdateMenuSelectorAnimation(
            (GameMenuSelectorManager *)pScreen->m_pSelectorManager334);
        EmitMenuSelectorResourceEntries(
            (GameMenuSelectorManager *)pScreen->m_pSelectorManager334);
    }
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
    pGroup = pScreen->m_abRenderEntryGroupC8 +
             pScreen->m_iRenderEntryGroup328 * 0x260;
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
    if (pScreen->m_pMappedListResources98 != 0) {
        ((MappedListResourceTable *)pScreen->m_pMappedListResources98)
            ->LoadListResourceIntoSlot(
                pScreen->m_nMappedListRemapResourceId384);
    }
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
    const int *pResources;
    const int *pPlayResources;
    const int *pSinglePlayerResources;
    const int *pMultiPlayerResources;
    const int *pDisplayResources;
    const int *pPackResources;
    const int *pOptionsResources;
    GameMenuSelectorManager *pSelectorManager;
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

    pResources = pScreen->m_fCompactLayout32C == 0
        ? g_GAME_MainMenuWidgetResourcesStandard
        : g_GAME_MainMenuWidgetResourcesCompact;
    pPlayResources = pResources;
    pSinglePlayerResources = pResources + 2;
    pMultiPlayerResources = pResources + 1;
    pDisplayResources = pResources +
        (pScreen->m_fCompactLayout32C == 0 ? 10 : 11);
    pPackResources = pResources +
        (pScreen->m_fCompactLayout32C == 0 ? 5 : 6);
    pOptionsResources = pResources +
        (pScreen->m_fCompactLayout32C == 0 ? 4 : 3);

    pSelectorManager = (GameMenuSelectorManager *)
        AllocateVSMemBlock(sizeof(GameMenuSelectorManager));
    if (pSelectorManager != 0) {
        ConstructMenuSelectorManager(
            pSelectorManager, 0, pScreen->m_pPrimaryContext84,
            pScreen->m_pRenderContext88, 6,
            pScreen->m_fCompactLayout32C);
    }
    pScreen->m_pSelectorManager334 = pSelectorManager;

    AppendMenuSelectorChild(
        pSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[0],
        pScreen->m_panWidgetLayout39C[1],
        pPlayResources, 1, 0, 0, 0,
        &pScreen->m_nPlaySelection3A0, (int)0xacef00a6U);
    AppendMenuSelectorChild(
        pSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[2],
        pScreen->m_panWidgetLayout39C[3],
        pSinglePlayerResources, 1, 0, 0, 0,
        &pScreen->m_nSharedSelection3B8, (int)0xacef0001U);
    AppendMenuSelectorChild(
        pSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[4],
        pScreen->m_panWidgetLayout39C[5],
        pMultiPlayerResources, 1, 0, 0, 0,
        &pScreen->m_nMultiplayerSelection3A4, (int)0xacef00a7U);
    AppendMenuSelectorChild(
        pSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[8],
        pScreen->m_panWidgetLayout39C[9],
        pDisplayResources, 1, 0, 0, 0,
        &pScreen->m_nDisplayModeSelection3B0, (int)0xacef00a5U);
    AppendMenuSelectorChild(
        pSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[6],
        pScreen->m_panWidgetLayout39C[7],
        pPackResources, 0, 0, 3, 0,
        &pScreen->m_nPackSelection3B4, -1);
    AppendMenuSelectorChild(
        pSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[10],
        pScreen->m_panWidgetLayout39C[11],
        pOptionsResources, 1, 0, 0, 0,
        &pScreen->m_nSharedSelection3B8, (int)0xacef00a4U);
    InitializeMenuSelectorCurrentState(pSelectorManager, 0, 0);
    InitializeMenuSelectorChildContext(pSelectorManager);
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
    }
    g_pActiveNetworkLobbyScreen = 0;
    if (pScreen->m_pTimedFrameSelector34C != 0) {
        FreeVSMemBlock(pScreen->m_pTimedFrameSelector34C);
        pScreen->m_pTimedFrameSelector34C = 0;
    }
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(
                pScreen->m_abRenderQueueNode04, 0);
    }
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        ReleaseFramedScreenFrameResources(pScreen);
    }
    if (pScreen->m_pMappedListResources98 != 0) {
        ((MappedListResourceTable *)pScreen->m_pMappedListResources98)
            ->Destroy();
        FreeVSMemBlock(pScreen->m_pMappedListResources98);
        pScreen->m_pMappedListResources98 = 0;
    }
    ((MogLoadResourceArchive *)g_pMainResourceArchive)
        ->PruneUnreferencedCachedResourceObjects();
    DestroyFramedScreenRenderEntryGroup(
        pScreen->m_abRenderEntryGroupC8);
    DestroyVariantRenderManager(pScreen->m_abVariantRenderManager14);
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

void *LEMBALL_FASTCALL ConstructFunctionalOptionsScreen(
    void *pObject, int, void *pPrimaryContext,
    void *pRenderContext, short *paSize) {
    GameOptionsScreenView *pScreen;

    pScreen = (GameOptionsScreenView *)pObject;
    ConstructFramedScreenBaseFields(
        pScreen, 0, pPrimaryContext, pRenderContext, paSize,
        3, 0, 0, 0, 0, 0);
    InitializeFunctionalOptionsVtables();
    pScreen->m_pVtable00 = g_GAME_OptionsScreenVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_OptionsQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;

    if (g_fMusicOptionAvailable == 0) {
        g_fMusicEnabled = 0;
    }
    if (g_fEffectsOptionAvailable == 0) {
        g_fEffectsEnabled = 0;
    }
    if (g_fZoomOptionAvailable == 0) {
        g_fZoomedViewportEnabled = 0;
    }
    if (g_fStartupAnimationsEnabled == 0) {
        g_fSkipStartupSequence = 1;
    }
    pScreen->m_fZoomDisabled398 = g_fZoomedViewportEnabled == 0;
    pScreen->m_fSkipStartupSequence3A4 = g_fSkipStartupSequence;
    pScreen->m_nReserved3A0 = 0;
    pScreen->m_nBackSelection39C = 0;
    g_nOptionsEffectsSliderValue = g_fEffectsEnabled;
    g_nOptionsMusicSliderValue = g_fMusicEnabled;
    pScreen->m_fDrawFrameA8 = 1;
    pScreen->m_fTileBackgroundAC = 1;
    pScreen->m_fDrawForegroundB0 = 1;
    ActivateFramedScreenBase(pScreen);
    return pScreen;
}

static void LEMBALL_FASTCALL InitializeOptionsScreenWidgets(void *pObject) {
    GameOptionsScreenView *pScreen;
    GameMenuSelectorManager *pManager;
    unsigned char *pForegroundEntry;
    const int *pZoomResources;
    const int *pEffectsResources;
    const int *pMusicResources;
    const int *pBackResources;
    const int *pIntroResources;
    short cxForeground;
    int fCompact;

    pScreen = (GameOptionsScreenView *)pObject;
    fCompact = pScreen->m_fCompactLayout32C != 0;
    if (fCompact == 0) {
        pScreen->m_panWidgetLayout3A8 = g_GAME_OptionsLayoutStandard;
        pZoomResources = g_GAME_OptionsZoomResourcesStandard;
        pEffectsResources = g_GAME_OptionsEffectsResourcesStandard;
        pMusicResources = g_GAME_OptionsMusicResourcesStandard;
        pBackResources = g_GAME_OptionsBackResourcesStandard;
        pIntroResources = g_GAME_OptionsIntroResourcesStandard;
    } else {
        pScreen->m_panWidgetLayout3A8 = g_GAME_OptionsLayoutCompact;
        pZoomResources = g_GAME_OptionsZoomResourcesCompact;
        pEffectsResources = g_GAME_OptionsEffectsResourcesCompact;
        pMusicResources = g_GAME_OptionsMusicResourcesCompact;
        pBackResources = g_GAME_OptionsBackResourcesCompact;
        pIntroResources = g_GAME_OptionsIntroResourcesCompact;
        pScreen->m_fZoomDisabled398 = 1;
    }

    pForegroundEntry = pScreen->m_abRenderEntryGroupC8 + 0x14;
    cxForeground = *(short *)((char *)pScreen->m_pForegroundBitmapC0 + 0x48);
    *(short *)(pForegroundEntry + 0) =
        (short)((pScreen->m_cxScreen37C - cxForeground) / 2);
    *(short *)(pForegroundEntry + 2) = 0;
    *(void **)(pForegroundEntry + 0x0c) = pScreen->m_pForegroundBitmapC0;
    *(int *)(pForegroundEntry + 0x10) = 0x800;
    *(int *)(pForegroundEntry + 0x14) = 0;

    pManager = (GameMenuSelectorManager *)
        AllocateVSMemBlock(sizeof(GameMenuSelectorManager));
    if (pManager == 0) {
        pScreen->m_pSelectorManager334 = 0;
        return;
    }
    ConstructMenuSelectorManager(
        pManager, 0, pScreen->m_pPrimaryContext84,
        pScreen->m_pRenderContext88, 8, fCompact);
    pScreen->m_pSelectorManager334 = pManager;

    AppendMenuSelectorChild(pManager, 0,
        pScreen->m_panWidgetLayout3A8[0],
        pScreen->m_panWidgetLayout3A8[1], pZoomResources,
        0, fCompact, 1, 0, &pScreen->m_fZoomDisabled398,
        (int)0xacef0004U);
    AppendMenuSelectorChild(pManager, 0,
        pScreen->m_panWidgetLayout3A8[2],
        pScreen->m_panWidgetLayout3A8[3], pIntroResources,
        0, 0, 1, 0, &pScreen->m_fSkipStartupSequence3A4,
        (int)0xacef0007U);
    AppendMenuSelectorChild(pManager, 0,
        pScreen->m_panWidgetLayout3A8[4],
        pScreen->m_panWidgetLayout3A8[5], pEffectsResources,
        0, 0, 1, 0, &g_nOptionsEffectsSliderValue,
        (int)0xacef0005U);
    AppendMenuSelectorChild(pManager, 0,
        pScreen->m_panWidgetLayout3A8[6],
        pScreen->m_panWidgetLayout3A8[7], pMusicResources,
        0, 0, 1, 0, &g_nOptionsMusicSliderValue,
        (int)0xacef0006U);
    AppendMenuSelectorChild(pManager, 0,
        pScreen->m_panWidgetLayout3A8[10],
        pScreen->m_panWidgetLayout3A8[11], pBackResources,
        1, 0, 0, 0, &pScreen->m_nBackSelection39C,
        (int)0xacef0008U);
    InitializeMenuSelectorCurrentState(pManager, 0, 0);
    InitializeMenuSelectorChildContext(pManager);
}

static void LEMBALL_FASTCALL ReleaseOptionsScreenWidgets(void *pObject) {
    GameOptionsScreenView *pScreen;

    pScreen = (GameOptionsScreenView *)pObject;
    if (pScreen->m_pSelectorManager334 != 0) {
        ((GameMainMenuSelectorManagerInterface *)
             pScreen->m_pSelectorManager334)->Delete((void *)1);
        pScreen->m_pSelectorManager334 = 0;
    }
}

static int LEMBALL_FASTCALL HandleOptionsScreenInput(
    void *pObject, int, void *pEvent) {
    GameOptionsScreenView *pScreen;
    GameMenuSelectorEvent *pSelectorEvent;
    unsigned int nCommand;

    pScreen = (GameOptionsScreenView *)pObject;
    pSelectorEvent = (GameMenuSelectorEvent *)pEvent;
    if (pSelectorEvent->m_nType00 != 0x0c) {
        ++*(int *)((char *)pScreen + 0x10);
        return 0;
    }
    nCommand = (unsigned int)pSelectorEvent->m_nPayload08;
    if (nCommand == 0xacef0008U) {
        pScreen->m_nTransitionStatus378 = 1;
        pScreen->m_nRequestedMode374 = 2;
        return 1;
    }
    if (nCommand == 0xacef0009U) {
        pScreen->m_nTransitionStatus378 = 1;
        pScreen->m_nRequestedMode374 = 0x11;
        return 1;
    }
    if (nCommand >= 0xacef0005U && nCommand <= 0xacef0006U) {
        return 1;
    }
    if (nCommand == 0xacff0000U) {
        g_nOptionsEffectsSliderValue = pSelectorEvent->m_nPayload0C;
        return 1;
    }
    if (nCommand == 0xacff0001U) {
        g_nOptionsMusicSliderValue = pSelectorEvent->m_nPayload0C;
        return 1;
    }
    return 0;
}

static void DestroyOptionsScreen(GameOptionsScreenView *pScreen) {
    pScreen->m_pVtable00 = g_GAME_OptionsScreenVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 = g_GAME_OptionsQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    if (pScreen->m_fCompactLayout32C == 0) {
        g_fZoomedViewportEnabled = pScreen->m_fZoomDisabled398 == 0;
    }
    g_fSkipStartupSequence = pScreen->m_fSkipStartupSequence3A4;
    SetVariantResourceEffectsEnabled(g_nOptionsEffectsSliderValue);
    if (g_pVariantResourceEntryManager != 0) {
        ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
            ->SetVariantResourceMusicEnabled(g_nOptionsMusicSliderValue);
    }
    g_fMusicEnabled = g_nOptionsMusicSliderValue;
    g_fEffectsEnabled = g_nOptionsEffectsSliderValue;
    if (g_fMusicOptionAvailable == 0) g_fMusicEnabled = 0;
    if (g_fEffectsOptionAvailable == 0) g_fEffectsEnabled = 0;
    if (g_fZoomOptionAvailable == 0) g_fZoomedViewportEnabled = 0;
    if (g_fStartupAnimationsEnabled == 0) g_fSkipStartupSequence = 1;
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        ReleaseOptionsScreenWidgets(pScreen);
    }
    g_pActiveNetworkLobbyScreen = 0;
    if (pScreen->m_pTimedFrameSelector34C != 0) {
        FreeVSMemBlock(pScreen->m_pTimedFrameSelector34C);
        pScreen->m_pTimedFrameSelector34C = 0;
    }
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(
                pScreen->m_abRenderQueueNode04, 0);
    }
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        ReleaseFramedScreenFrameResources(pScreen);
    }
    if (pScreen->m_pMappedListResources98 != 0) {
        ((MappedListResourceTable *)pScreen->m_pMappedListResources98)
            ->Destroy();
        FreeVSMemBlock(pScreen->m_pMappedListResources98);
        pScreen->m_pMappedListResources98 = 0;
    }
    ((MogLoadResourceArchive *)g_pMainResourceArchive)
        ->PruneUnreferencedCachedResourceObjects();
    DestroyFramedScreenRenderEntryGroup(pScreen->m_abRenderEntryGroupC8);
    DestroyVariantRenderManager(pScreen->m_abVariantRenderManager14);
}

static void *LEMBALL_FASTCALL DeleteOptionsScreen(
    void *pObject, int, int fDelete) {
    DestroyOptionsScreen((GameOptionsScreenView *)pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

static void *LEMBALL_FASTCALL DeleteAdjustedOptionsScreen(
    void *pObject, int nUnusedEdx, int fDelete) {
    return DeleteOptionsScreen((char *)pObject - 4, nUnusedEdx, fDelete);
}

void *LEMBALL_FASTCALL ConstructFunctionalPaintballSequenceScreen(
    void *pObject, int, void *pPrimaryContext,
    void *pRenderContext, short *paSize, int fSecondVariant) {
    GamePaintballSequenceScreenView *pScreen;

    pScreen = (GamePaintballSequenceScreenView *)pObject;
    ConstructFramedScreenBaseFields(
        pScreen, 0, pPrimaryContext, pRenderContext, paSize,
        0x0e, 0x32, 200, 0, 0x28, 0x30);
    InitializeFunctionalPaintballSequenceVtables();
    pScreen->m_pVtable00 = g_GAME_PaintballSequenceScreenVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_PaintballSequenceQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    InitializePackagedSpriteRenderEntryThunk(
        pScreen->m_abPrimaryBitmapEntry398);
    InitializePackagedSpriteRenderEntryThunk(
        pScreen->m_abSecondaryBitmapEntry3E0);
    pScreen->m_fSecondVariant518 = fSecondVariant;
    pScreen->m_fMediaEnabled604 = g_fSkipStartupSequence == 0;
    pScreen->m_nActionSelection514 = 0;
    pScreen->m_fEffectStarted51C = 0;
    pScreen->m_fMediaStarted5FC = 0;
    pScreen->m_nReserved608 = 0;
    pScreen->m_dwMediaStart5F8 = timeGetTime() + 40U;
    pScreen->m_fDrawFrameA8 = 1;
    pScreen->m_fTileBackgroundAC = 1;
    pScreen->m_fDrawForegroundB0 = 1;
    ActivateFramedScreenBase(pScreen);
    return pScreen;
}

static void ConfigurePaintballBitmapEntry(
    unsigned char *pEntry, void *pBitmap, short x, short y) {
    *(short *)(pEntry + 0) = x;
    *(short *)(pEntry + 2) = y;
    *(void **)(pEntry + 0x0c) = pBitmap;
    *(int *)(pEntry + 0x10) = 0x800;
    *(int *)(pEntry + 0x14) = 0;
}

static void LEMBALL_FASTCALL InitializePaintballSequenceResources(
    void *pObject) {
    GamePaintballSequenceScreenView *pScreen;
    int nPrimaryResource;
    int nSecondaryResource;
    short x;

    pScreen = (GamePaintballSequenceScreenView *)pObject;
    if (pScreen->m_fCompactLayout32C == 0) {
        nPrimaryResource = pScreen->m_fSecondVariant518 == 0
            ? 0x135 : 0x134;
        nSecondaryResource = pScreen->m_fSecondVariant518 == 0
            ? 0x137 : 0x138;
        pScreen->m_nButtonResourceId508 =
            pScreen->m_fSecondVariant518 == 0 ? 0x157 : 0x158;
    } else {
        nPrimaryResource = pScreen->m_fSecondVariant518 == 0
            ? 0x140 : 0x13f;
        nSecondaryResource = pScreen->m_fSecondVariant518 == 0
            ? 0x142 : 0x143;
        pScreen->m_nButtonResourceId508 =
            pScreen->m_fSecondVariant518 == 0 ? 0x184 : 0x185;
    }
    pScreen->m_nPrimaryBitmapResourceId504 = nPrimaryResource;
    pScreen->m_nSecondaryBitmapResourceId50C = nSecondaryResource;
    pScreen->m_pPrimaryBitmap4FC = LoadBitmapResource(nPrimaryResource);
    pScreen->m_pSecondaryBitmap500 = pScreen->m_fMediaEnabled604 == 0
        ? LoadBitmapResource(nSecondaryResource) : 0;
    if (pScreen->m_pPrimaryBitmap4FC != 0) {
        x = (short)(pScreen->m_cxScreen37C -
            *(short *)((char *)pScreen->m_pPrimaryBitmap4FC + 0x48));
        ConfigurePaintballBitmapEntry(
            pScreen->m_abPrimaryBitmapEntry398,
            pScreen->m_pPrimaryBitmap4FC, x, 0);
    }
    if (pScreen->m_pSecondaryBitmap500 != 0) {
        ConfigurePaintballBitmapEntry(
            pScreen->m_abSecondaryBitmapEntry3E0,
            pScreen->m_pSecondaryBitmap500, 0, 0);
    }
}

static void LEMBALL_FASTCALL ReleasePaintballSequenceResources(
    void *pObject) {
    GamePaintballSequenceScreenView *pScreen;

    pScreen = (GamePaintballSequenceScreenView *)pObject;
    if (pScreen->m_pPrimaryBitmap4FC != 0) {
        ReleaseTypedResourceObjectReference(pScreen->m_pPrimaryBitmap4FC);
        pScreen->m_pPrimaryBitmap4FC = 0;
    }
    if (pScreen->m_pSecondaryBitmap500 != 0) {
        ReleaseTypedResourceObjectReference(pScreen->m_pSecondaryBitmap500);
        pScreen->m_pSecondaryBitmap500 = 0;
    }
}

static void LEMBALL_FASTCALL DrawPaintballSequenceFrame(void *pObject) {
    GamePaintballSequenceScreenView *pScreen;

    pScreen = (GamePaintballSequenceScreenView *)pObject;
    if (pScreen->m_pPrimaryBitmap4FC != 0) {
        ((GameMainMenuRenderEntryInterface *)
            pScreen->m_abPrimaryBitmapEntry398)
            ->AppendToQueue(pScreen->m_pRenderContext88);
    }
    if (pScreen->m_pSecondaryBitmap500 != 0) {
        ((GameMainMenuRenderEntryInterface *)
            pScreen->m_abSecondaryBitmapEntry3E0)
            ->AppendToQueue(pScreen->m_pRenderContext88);
    }
}

static void FinishPaintballSequence(
    GamePaintballSequenceScreenView *pScreen, int nMode) {
    pScreen->m_nTransitionStatus378 = 1;
    pScreen->m_nRequestedMode374 = nMode;
}

static int LEMBALL_FASTCALL HandlePaintballSequenceInput(
    void *pObject, int, void *pEvent) {
    GamePaintballSequenceScreenView *pScreen;
    GameMenuSelectorEvent *pSelectorEvent;
    unsigned int nCode;

    pScreen = (GamePaintballSequenceScreenView *)pObject;
    pSelectorEvent = (GameMenuSelectorEvent *)pEvent;
    nCode = (unsigned int)pSelectorEvent->m_nPayload08;
    if (pSelectorEvent->m_nType00 == 0x0c) {
        if (nCode == 0xacef0010U) {
            FinishPaintballSequence(pScreen, 4);
            return 1;
        }
        if (nCode == 0xacef0011U) {
            FinishPaintballSequence(pScreen, 2);
            return 1;
        }
        return 0;
    }
    if (pSelectorEvent->m_nType00 == 4) {
        if (nCode == 0x1bU) {
            FinishPaintballSequence(pScreen, 2);
            return 1;
        }
        if (nCode == 0x0dU || nCode == 0x20U) {
            FinishPaintballSequence(pScreen, 4);
            return 1;
        }
        return 0;
    }
    ++*(int *)((char *)pScreen + 0x10);
    return 0;
}

static void LEMBALL_FASTCALL UpdatePaintballSequenceScreen(void *pObject) {
    GamePaintballSequenceScreenView *pScreen;

    pScreen = (GamePaintballSequenceScreenView *)pObject;
    if (pScreen->m_fEffectStarted51C == 0) {
        if (g_pVariantResourceEntryManager != 0) {
            ((GameVariantResourceEntryManager *)
                g_pVariantResourceEntryManager)->PlayVariantResourceEffect(
                    pScreen->m_fSecondVariant518 == 0 ? 0x28 : 0x27);
        }
        pScreen->m_fEffectStarted51C = 1;
        pScreen->m_dwEffectStart600 = timeGetTime();
    }
}

static void DestroyPaintballSequenceScreen(
    GamePaintballSequenceScreenView *pScreen) {
    pScreen->m_pVtable00 = g_GAME_PaintballSequenceScreenVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_PaintballSequenceQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        ReleasePaintballSequenceResources(pScreen);
    }
    g_pActiveNetworkLobbyScreen = 0;
    if (pScreen->m_pTimedFrameSelector34C != 0) {
        FreeVSMemBlock(pScreen->m_pTimedFrameSelector34C);
        pScreen->m_pTimedFrameSelector34C = 0;
    }
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(
                pScreen->m_abRenderQueueNode04, 0);
    }
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        ReleaseFramedScreenFrameResources(pScreen);
    }
    if (pScreen->m_pMappedListResources98 != 0) {
        ((MappedListResourceTable *)pScreen->m_pMappedListResources98)
            ->Destroy();
        FreeVSMemBlock(pScreen->m_pMappedListResources98);
        pScreen->m_pMappedListResources98 = 0;
    }
    ((MogLoadResourceArchive *)g_pMainResourceArchive)
        ->PruneUnreferencedCachedResourceObjects();
    DestroyFramedScreenRenderEntryGroup(pScreen->m_abRenderEntryGroupC8);
    DestroyVariantRenderManager(pScreen->m_abVariantRenderManager14);
}

static void *LEMBALL_FASTCALL DeletePaintballSequenceScreen(
    void *pObject, int, int fDelete) {
    DestroyPaintballSequenceScreen(
        (GamePaintballSequenceScreenView *)pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

static void *LEMBALL_FASTCALL DeleteAdjustedPaintballSequenceScreen(
    void *pObject, int nUnusedEdx, int fDelete) {
    return DeletePaintballSequenceScreen(
        (char *)pObject - 4, nUnusedEdx, fDelete);
}

void *LEMBALL_FASTCALL ConstructFunctionalPasswordEntryScreen(
    void *pObject, int, void *pPrimaryContext,
    void *pRenderContext, short *paSize) {
    GamePasswordEntryScreenView *pScreen;

    pScreen = (GamePasswordEntryScreenView *)pObject;
    ConstructFramedScreenBaseFields(
        pScreen, 0, pPrimaryContext, pRenderContext, paSize,
        0x10, 10, 10, 0, 0x28, 0x30);
    InitializeFunctionalPasswordEntryVtables();
    pScreen->m_pVtable00 = g_GAME_PasswordEntryScreenVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_PasswordEntryQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    memset((char *)pScreen + 0x398, 0, 0x494 - 0x398);
    strncpy(pScreen->m_szPassword3B0, FormatCurrentLevelPassword(), 10);
    pScreen->m_szPassword3B0[10] = '\0';
    pScreen->m_cPassword420 = (int)strlen(pScreen->m_szPassword3B0);
    pScreen->m_nSelectedKey480 = 4;
    pScreen->m_fDrawFrameA8 = 1;
    pScreen->m_fTileBackgroundAC = 1;
    pScreen->m_fDrawForegroundB0 = 1;
    ActivateFramedScreenBase(pScreen);
    return pScreen;
}

static void LEMBALL_FASTCALL InitializePasswordEntryResources(void *pObject) {
    GamePasswordEntryScreenView *pScreen;
    unsigned char *pForegroundEntry;
    void *pOverlay;
    int nContext;
    int nRenderObject;
    short cxForeground;
    int i;
    VS_XYPair Point;

    pScreen = (GamePasswordEntryScreenView *)pObject;
    if (pScreen->m_fCompactLayout32C != 0) {
        pScreen->m_pLayout3A8 = g_GAME_PasswordLayoutCompact;
        pScreen->m_pKeyResourceIds3AC = g_GAME_PasswordResourcesCompact;
        pScreen->m_nPanelResourceId398 = 0x192;
    } else {
        pScreen->m_pLayout3A8 = g_GAME_PasswordLayoutStandard;
        pScreen->m_pKeyResourceIds3AC = g_GAME_PasswordResourcesStandard;
        pScreen->m_nPanelResourceId398 = 0x165;
    }
    LoadVariantRenderResource(
        pScreen->m_abVariantRenderManager14,
        pScreen->m_nPanelResourceId398);
    if (pScreen->m_pForegroundBitmapC0 != 0) {
        pForegroundEntry = pScreen->m_abRenderEntryGroupC8 + 0x14;
        cxForeground = *(short *)((char *)pScreen->m_pForegroundBitmapC0 + 0x48);
        *(short *)(pForegroundEntry + 0) =
            (short)((pScreen->m_cxScreen37C - cxForeground) / 2);
        *(short *)(pForegroundEntry + 2) = 0;
        *(void **)(pForegroundEntry + 0x0c) = pScreen->m_pForegroundBitmapC0;
        *(int *)(pForegroundEntry + 0x10) = 0x800;
        *(int *)(pForegroundEntry + 0x14) = 0;
    }
    Point.m_x = (short)pScreen->m_pLayout3A8[28];
    Point.m_y = (short)pScreen->m_pLayout3A8[29];
    for (i = 0; i < 12; ++i) {
        pOverlay = AllocateVSMemBlock(0x130);
        if (pOverlay != 0) {
            ConstructAlignedZrleChildOverlayFromPoint(
                pOverlay, 0, &Point, pScreen->m_pPrimaryContext84,
                pScreen->m_pKeyResourceIds3AC[i], 3);
            *(int *)((char *)pOverlay + 0xf8) = 0;
            nContext = *(int *)(*(int *)((char *)pOverlay + 0x4c) + 0x0c);
            nRenderObject = nContext +
                *(int *)(*(int *)(nContext + 0x40) + 4) + 0x70;
            *(int *)nRenderObject = 0;
            *(void **)((char *)pOverlay + 0xf4) =
                g_pSharedRenderDispatchQueue;
            *(int *)((char *)pOverlay + 0xcc) =
                (int)(0xabcd00b0U + i);
            pScreen->m_aKeyPositions3C0[i][0] =
                *(short *)((char *)pOverlay + 0xc0);
            pScreen->m_aKeyPositions3C0[i][1] =
                *(short *)((char *)pOverlay + 0xc2);
        }
        pScreen->m_apKeyOverlays424[i] = pOverlay;
    }
    i = g_GAME_PasswordKeyPermutation[pScreen->m_nSelectedKey480];
    pScreen->m_xSelected478 = pScreen->m_aKeyPositions3C0[i][0];
    pScreen->m_ySelected47C = pScreen->m_aKeyPositions3C0[i][1];
}

static void LEMBALL_FASTCALL ReleasePasswordEntryResources(void *pObject) {
    GamePasswordEntryScreenView *pScreen;
    int i;
    pScreen = (GamePasswordEntryScreenView *)pObject;
    for (i = 0; i < 12; ++i) {
        if (pScreen->m_apKeyOverlays424[i] != 0) {
            DestroyAlignedZrleChildOverlay(
                (char *)pScreen->m_apKeyOverlays424[i] + 0x90);
            FreeVSMemBlock(pScreen->m_apKeyOverlays424[i]);
            pScreen->m_apKeyOverlays424[i] = 0;
        }
    }
    if (pScreen->m_nPanelResourceId398 != 0) {
        ((LevelVariantRenderManagerView *)pScreen->m_abVariantRenderManager14)
            ->ReleaseVariantRenderResource(pScreen->m_nPanelResourceId398);
        pScreen->m_nPanelResourceId398 = 0;
    }
}

static void RotatePasswordEntrySelection(
    GamePasswordEntryScreenView *pScreen, int nDirection) {
    int nPosition;
    int nAction;

    nPosition = pScreen->m_nSelectedKey480;
    switch (nDirection) {
    case 1:
        nPosition = (nPosition + 9) % 12;
        break;
    case 2:
        nPosition = (nPosition + 3) % 12;
        break;
    case 3:
        nPosition = (nPosition % 3 == 0) ? nPosition + 2 : nPosition - 1;
        break;
    case 4:
        nPosition = (nPosition % 3 == 2) ? nPosition - 2 : nPosition + 1;
        break;
    }
    pScreen->m_nSelectedKey480 = nPosition;
    nAction = g_GAME_PasswordKeyPermutation[nPosition];
    pScreen->m_xSelected478 = pScreen->m_aKeyPositions3C0[nAction][0];
    pScreen->m_ySelected47C = pScreen->m_aKeyPositions3C0[nAction][1];
    pScreen->m_fSelectorDirty360 = 1;
    pScreen->m_fFrameDirty330 = 1;
}

static void ApplyPasswordEntryAction(
    GamePasswordEntryScreenView *pScreen, int nAction) {
    int nEffect;

    if (pScreen->m_nResultState488 == 1) return;
    if (nAction < 10) {
        if (pScreen->m_cPassword420 < 10) {
            pScreen->m_szPassword3B0[pScreen->m_cPassword420++] =
                (char)('0' + nAction);
            pScreen->m_szPassword3B0[pScreen->m_cPassword420] = '\0';
        } else if (g_pVariantResourceEntryManager != 0) {
            ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
                ->PlayVariantResourceEffect(0x19);
        }
    } else if (nAction == 10) {
        if (pScreen->m_cPassword420 != 0) {
            --pScreen->m_cPassword420;
            pScreen->m_szPassword3B0[pScreen->m_cPassword420] = '\0';
        } else if (g_pVariantResourceEntryManager != 0) {
            ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
                ->PlayVariantResourceEffect(0x19);
        }
    } else if (nAction == 11) {
        pScreen->m_fPasswordValid484 =
            ValidateCurrentLevelPassword(pScreen->m_szPassword3B0);
        SnapshotCurrentLevelProgress();
        nEffect = pScreen->m_fPasswordValid484 != 0 ? 0x13 : 0x22;
        if (g_pVariantResourceEntryManager != 0) {
            ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
                ->PlayVariantResourceEffect(nEffect);
        }
        pScreen->m_nResultState488 = 1;
        pScreen->m_dwResultStart48C = timeGetTime();
        pScreen->m_dwResultDeadline490 = pScreen->m_dwResultStart48C + 1000U;
    }
    pScreen->m_fSelectorDirty360 = 1;
    pScreen->m_fFrameDirty330 = 1;
}

static int LEMBALL_FASTCALL HandlePasswordEntryInput(
    void *pObject, int, void *pEvent) {
    GamePasswordEntryScreenView *pScreen;
    GameMenuSelectorEvent *pSelectorEvent;
    unsigned int nCode;
    int nAction;

    pScreen = (GamePasswordEntryScreenView *)pObject;
    pSelectorEvent = (GameMenuSelectorEvent *)pEvent;
    nCode = (unsigned int)pSelectorEvent->m_nPayload08;
    nAction = -1;
    if (pSelectorEvent->m_nType00 == 0x0c &&
        nCode >= 0xabcd00b0U && nCode <= 0xabcd00bbU) {
        nAction = (int)(nCode - 0xabcd00b0U);
    } else if (pSelectorEvent->m_nType00 == 3 ||
               pSelectorEvent->m_nType00 == 4) {
        if (nCode >= 0x39U && nCode <= 0x42U) nAction = (int)(nCode - 0x39U);
        else if (nCode >= '0' && nCode <= '9') nAction = (int)(nCode - '0');
        else if (nCode == 0x4dU || nCode == 0x4eU || nCode == 8U) nAction = 10;
        else if (nCode == 0x4cU || nCode == 13U) nAction = 11;
        else if (nCode == 0x1fU || nCode == 0x22U || nCode == 0x20U)
            nAction = g_GAME_PasswordKeyPermutation[
                pScreen->m_nSelectedKey480];
        else if (nCode >= 1U && nCode <= 4U) {
            RotatePasswordEntrySelection(pScreen, (int)nCode);
            return 1;
        }
        else if (nCode == 0x1bU) {
            pScreen->m_nTransitionStatus378 = 1;
            pScreen->m_nRequestedMode374 = 2;
            return 1;
        }
    }
    if (nAction >= 0) {
        ApplyPasswordEntryAction(pScreen, nAction);
        return 1;
    }
    ++*(int *)((char *)pScreen + 0x10);
    return 0;
}

static void LEMBALL_FASTCALL UpdatePasswordEntryScreen(void *pObject) {
    GamePasswordEntryScreenView *pScreen;
    pScreen = (GamePasswordEntryScreenView *)pObject;
    if (pScreen->m_nResultState488 == 1 &&
        (int)(timeGetTime() - pScreen->m_dwResultDeadline490) >= 0) {
        pScreen->m_nTransitionStatus378 = 1;
        pScreen->m_nRequestedMode374 = 2;
    }
}

static void LEMBALL_FASTCALL DrawPasswordEntryText(void *pObject) {
    GamePasswordEntryScreenView *pScreen;
    MappedListResourceTable *pMappedList;
    short Point[2];
    short Relative[2];
    const char *pszResult;

    pScreen = (GamePasswordEntryScreenView *)pObject;
    if (pScreen->m_fBuildingRenderEntries364 == 0 ||
        pScreen->m_pMappedListResources98 == 0 ||
        pScreen->m_pLayout3A8 == 0) return;
    pMappedList = (MappedListResourceTable *)pScreen->m_pMappedListResources98;
    pMappedList->ResetDrawEntryPool();
    Relative[0] = 0;
    Relative[1] = 0;
    Point[0] = (short)pScreen->m_pLayout3A8[22];
    Point[1] = (short)pScreen->m_pLayout3A8[23];
    pMappedList->SubmitRenderEntry(
        pScreen->m_pRenderContext88, Point, Relative,
        pScreen->m_nMappedListRemapResourceId384,
        pScreen->m_szPassword3B0, 0x20, 0);
    if (pScreen->m_nResultState488 == 1) {
        pszResult = pScreen->m_fPasswordValid484 != 0
            ? "Password OK!" : "Invalid Password";
        Point[0] = (short)pScreen->m_pLayout3A8[30];
        Point[1] = (short)pScreen->m_pLayout3A8[31];
        pMappedList->SubmitRenderEntry(
            pScreen->m_pRenderContext88, Point, Relative,
            pScreen->m_nMappedListRemapResourceId384,
            (void *)pszResult, 0x20, 0);
    }
}

static void DestroyPasswordEntryScreen(GamePasswordEntryScreenView *pScreen) {
    pScreen->m_pVtable00 = g_GAME_PasswordEntryScreenVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 = g_GAME_PasswordEntryQueueVtable;
    if (pScreen->m_fFrameResourcesLoaded9C != 0)
        ReleasePasswordEntryResources(pScreen);
    g_pActiveNetworkLobbyScreen = 0;
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(
                pScreen->m_abRenderQueueNode04, 0);
    }
    if (pScreen->m_fFrameResourcesLoaded9C != 0)
        ReleaseFramedScreenFrameResources(pScreen);
    if (pScreen->m_pMappedListResources98 != 0) {
        ((MappedListResourceTable *)pScreen->m_pMappedListResources98)->Destroy();
        FreeVSMemBlock(pScreen->m_pMappedListResources98);
        pScreen->m_pMappedListResources98 = 0;
    }
    ((MogLoadResourceArchive *)g_pMainResourceArchive)
        ->PruneUnreferencedCachedResourceObjects();
    DestroyFramedScreenRenderEntryGroup(pScreen->m_abRenderEntryGroupC8);
    DestroyVariantRenderManager(pScreen->m_abVariantRenderManager14);
}

static void *LEMBALL_FASTCALL DeletePasswordEntryScreen(
    void *pObject, int, int fDelete) {
    DestroyPasswordEntryScreen((GamePasswordEntryScreenView *)pObject);
    if ((fDelete & 1) != 0) FreeVSMemBlock(pObject);
    return pObject;
}

static void *LEMBALL_FASTCALL DeleteAdjustedPasswordEntryScreen(
    void *pObject, int nUnusedEdx, int fDelete) {
    return DeletePasswordEntryScreen((char *)pObject - 4,
                                     nUnusedEdx, fDelete);
}

static int ConsumeLevelSelectionChangedFlag(void) {
    if (g_fLevelSelectionChanged != 0) {
        g_fLevelSelectionChanged = 0;
        return 1;
    }
    return 0;
}

static void InitializeFunctionalLevelSelectionVtable(void) {
    int i;

    if (g_fFunctionalLevelSelectionVtableInitialized != 0) {
        return;
    }
    for (i = 0; i < 22; ++i) {
        g_GAME_FunctionalLevelSelectionVtable[i] =
            g_GAME_MainMenuScreenVtable[i];
    }
    g_GAME_FunctionalLevelSelectionVtable[0] =
        (void *)DeleteFunctionalLevelSelectionScreen;
    g_GAME_FunctionalLevelSelectionVtable[14] =
        (void *)UpdateFunctionalLevelSelectionScreen;
    g_GAME_FunctionalLevelSelectionVtable[15] =
        (void *)HandleFunctionalLevelSelectionInput;
    g_GAME_FunctionalLevelSelectionVtable[16] =
        (void *)InitializeFunctionalLevelSelectionResources;
    g_GAME_FunctionalLevelSelectionVtable[17] =
        (void *)ReleaseFunctionalLevelSelectionResources;
    g_GAME_FunctionalLevelSelectionVtable[18] =
        (void *)DrawFunctionalLevelSelectionStatus;
    g_GAME_FunctionalLevelSelectionVtable[19] =
        (void *)DrawFunctionalLevelSelectionText;
    g_GAME_FunctionalLevelSelectionVtable[20] =
        (void *)NoopFunctionalLevelSelectionHook;
    g_GAME_FunctionalLevelSelectionVtable[21] =
        (void *)NoopFunctionalLevelSelectionHook;
    g_fFunctionalLevelSelectionVtableInitialized = 1;
}

void *LEMBALL_FASTCALL ConstructFunctionalLevelSelectionScreen(
    void *pObject, int, void *pPrimaryContext,
    void *pRenderContext, short *paSize) {
    GameLevelSelectionScreenView *pScreen;

    pScreen = (GameLevelSelectionScreenView *)pObject;
    ConstructFramedScreenBaseFields(
        pScreen, 0, pPrimaryContext, pRenderContext, paSize,
        4, 0x32, 200, 0, 0x28, 0x30);
    InitializeFunctionalLevelSelectionVtable();
    pScreen->m_pVtable00 = g_GAME_FunctionalLevelSelectionVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_MainMenuQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    pScreen->m_fDrawFrameA8 = 1;
    pScreen->m_fTileBackgroundAC = 1;
    pScreen->m_fDrawForegroundB0 = 1;
    ActivateFramedScreenBase(pScreen);
    pScreen->m_pSelector3C4 = ConstructLevelSelectionFrameSelector(
        pScreen->m_abVariantRenderManager14, pScreen->m_nResourceId3D8);
    pScreen->m_pSelector3C0 = ConstructLevelSelectionFrameSelector(
        pScreen->m_abVariantRenderManager14, pScreen->m_nResourceId3DC);
    pScreen->m_pSelector3C8 = ConstructLevelSelectionFrameSelector(
        pScreen->m_abVariantRenderManager14, pScreen->m_nResourceId3E0);
    return pScreen;
}

static GameProgressiveVariantFrameSelector *
ConstructLevelSelectionFrameSelector(void *pManager, int nResourceId) {
    GameProgressiveVariantFrameSelector *pSelector;

    pSelector = (GameProgressiveVariantFrameSelector *)
        AllocateVSMemBlock(sizeof(GameProgressiveVariantFrameSelector));
    if (pSelector == 0) {
        return 0;
    }
    pSelector->m_pVtable = g_MenuSelectorFrameSelectorCtorVtable;
    pSelector->m_cFrames =
        ((LevelVariantRenderManagerView *)pManager)
            ->GetVariantRenderFrameCount(nResourceId);
    pSelector->m_pVtable = g_MenuSelectorTimedFrameSelectorVtable;
    pSelector->m_nDirection = 1;
    pSelector->m_pVtable =
        g_MenuSelectorProgressiveFrameSelectorVtable;
    ((GameProgressiveSelectorVtableView *)pSelector)->SetDuration(500);
    pSelector->m_dwFixedTime = -1;
    return pSelector;
}

static void *LEMBALL_FASTCALL DeleteFunctionalLevelSelectionScreen(
    void *pObject, int, int fDelete) {
    GameLevelSelectionScreenView *pScreen;

    pScreen = (GameLevelSelectionScreenView *)pObject;
    pScreen->m_pVtable00 = g_GAME_FunctionalLevelSelectionVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_MainMenuQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    FreeVSMemBlock(pScreen->m_pSelector3C4);
    FreeVSMemBlock(pScreen->m_pSelector3C0);
    FreeVSMemBlock(pScreen->m_pSelector3C8);
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        ReleaseFunctionalLevelSelectionResources(pScreen);
    }
    g_pActiveNetworkLobbyScreen = 0;
    if (pScreen->m_pTimedFrameSelector34C != 0) {
        FreeVSMemBlock(pScreen->m_pTimedFrameSelector34C);
        pScreen->m_pTimedFrameSelector34C = 0;
    }
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(
                pScreen->m_abRenderQueueNode04, 0);
    }
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        ReleaseFramedScreenFrameResources(pScreen);
    }
    if (pScreen->m_pMappedListResources98 != 0) {
        ((MappedListResourceTable *)pScreen->m_pMappedListResources98)
            ->Destroy();
        FreeVSMemBlock(pScreen->m_pMappedListResources98);
        pScreen->m_pMappedListResources98 = 0;
    }
    ((MogLoadResourceArchive *)g_pMainResourceArchive)
        ->PruneUnreferencedCachedResourceObjects();
    DestroyFramedScreenRenderEntryGroup(
        pScreen->m_abRenderEntryGroupC8);
    DestroyVariantRenderManager(pScreen->m_abVariantRenderManager14);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pScreen);
    }
    return pObject;
}

static int LEMBALL_FASTCALL HandleFunctionalLevelSelectionInput(
    void *pObject, int, void *pEvent) {
    GameLevelSelectionScreenView *pScreen;
    unsigned short nType;
    int nCode;

    pScreen = (GameLevelSelectionScreenView *)pObject;
    nType = *(unsigned short *)pEvent;
    nCode = *(int *)((char *)pEvent + 8);
    if (nType == 4) {
        if (nCode == 0x1b || nCode == 0x23) {
            pScreen->m_nRequestedMode374 = 2;
        } else if (nCode == 0x0d || nCode == 0x1f || nCode == 0x4c) {
            pScreen->m_nRequestedMode374 = 5;
        } else if (nCode == 0x27 || nCode == 0x28 ||
                   nCode == 2 || nCode == 4) {
            if (pScreen->m_fNextLocked480 == 0) {
                SelectFunctionalLevel(pScreen, 1);
            }
            return 1;
        } else if (nCode == 0x25 || nCode == 0x26 ||
                   nCode == 1 || nCode == 3) {
            if (pScreen->m_fPreviousLocked484 == 0) {
                SelectFunctionalLevel(pScreen, -1);
            }
            return 1;
        } else {
            return 0;
        }
    } else if (nType == 0x0c) {
        if ((unsigned int)nCode == 0xacef000cU) {
            pScreen->m_nRequestedMode374 = 2;
        } else if ((unsigned int)nCode == 0xacef000dU) {
            pScreen->m_nRequestedMode374 = 5;
        } else if ((unsigned int)nCode == 0xacef000eU) {
            if (pScreen->m_fNextLocked480 == 0) {
                SelectFunctionalLevel(pScreen, 1);
            }
            return 1;
        } else if ((unsigned int)nCode == 0xacef000fU) {
            if (pScreen->m_fPreviousLocked484 == 0) {
                SelectFunctionalLevel(pScreen, -1);
            }
            return 1;
        } else {
            return 0;
        }
    } else {
        ++*(int *)((char *)pScreen + 0x10);
        return 0;
    }
    pScreen->m_nTransitionStatus378 = 1;
    return 1;
}

static void LEMBALL_FASTCALL UpdateFunctionalLevelSelectionScreen(
    void *pObject) {
    GameFramedScreenBase *pScreen;

    pScreen = (GameFramedScreenBase *)pObject;
    if (g_fStartupTestAllLevels != 0) {
        pScreen->m_nTransitionStatus378 = 1;
        pScreen->m_nRequestedMode374 = 5;
    }
}

static void LEMBALL_FASTCALL InitializeFunctionalLevelSelectionResources(
    void *pObject) {
    GameFramedScreenBase *pScreen;
    const int *pLayout;
    unsigned char *pForegroundEntry;
    void *pBitmap;
    int nResource0;
    int nResource1;
    int nResource2;

    pScreen = (GameFramedScreenBase *)pObject;
    if (pScreen->m_fCompactLayout32C == 1) {
        pLayout = g_GAME_LevelSelectionLayoutCompact;
        pBitmap = LoadBitmapResource(0x13e);
        nResource0 = 0x180;
        nResource1 = 0x181;
        nResource2 = 0x182;
        pScreen->m_nTimedVariantResourceId394 = 0x17c;
    } else {
        pLayout = g_GAME_LevelSelectionLayoutStandard;
        pBitmap = LoadBitmapResource(0x133);
        nResource0 = 0x153;
        nResource1 = 0x154;
        nResource2 = 0x155;
        pScreen->m_nTimedVariantResourceId394 = 0x14f;
    }
    *(void **)((char *)pScreen + 0x3bc) = pBitmap;
    *(const int **)((char *)pScreen + 0x3d4) = pLayout;
    *(int *)((char *)pScreen + 0x3d8) = nResource0;
    *(int *)((char *)pScreen + 0x3dc) = nResource1;
    *(int *)((char *)pScreen + 0x3e0) = nResource2;
    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              nResource0);
    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              nResource1);
    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              pScreen->m_nTimedVariantResourceId394);
    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              nResource2);

    pForegroundEntry = pScreen->m_abRenderEntryGroupC8 + 0x14;
    *(short *)(pForegroundEntry + 0) = (short)pLayout[14];
    *(short *)(pForegroundEntry + 2) = (short)pLayout[15];
    *(void **)(pForegroundEntry + 0x0c) = pBitmap;
    *(int *)(pForegroundEntry + 0x10) = 0x800;
    *(int *)(pForegroundEntry + 0x14) = 0;
    {
        GameLevelSelectionScreenView *pLevelScreen;

        pLevelScreen = (GameLevelSelectionScreenView *)pScreen;
        pLevelScreen->m_aTitlePositions464[0][0] = pLayout[36];
        pLevelScreen->m_aTitlePositions464[0][1] = pLayout[37];
        pLevelScreen->m_aTitlePositions464[1][0] = -1;
        pLevelScreen->m_aTitlePositions464[1][1] = -1;
        pLevelScreen->m_aTitlePositions464[2][0] = -1;
        pLevelScreen->m_aTitlePositions464[2][1] = -1;
        RefreshFunctionalLevelSelectionMetadata(pLevelScreen);
    }
    pScreen->m_fSelectorDirty360 = 1;
}

static void LEMBALL_FASTCALL ReleaseFunctionalLevelSelectionResources(
    void *pObject) {
    GameFramedScreenBase *pScreen;
    LevelVariantRenderManagerView *pVariantManager;

    pScreen = (GameFramedScreenBase *)pObject;
    pVariantManager = (LevelVariantRenderManagerView *)
        pScreen->m_abVariantRenderManager14;
    if (*(void **)((char *)pScreen + 0x3bc) != 0) {
        ReleaseTypedResourceObjectReference(
            *(void **)((char *)pScreen + 0x3bc));
        *(void **)((char *)pScreen + 0x3bc) = 0;
    }
    pVariantManager->ReleaseVariantRenderResource(
        *(int *)((char *)pScreen + 0x3d8));
    pVariantManager->ReleaseVariantRenderResource(
        *(int *)((char *)pScreen + 0x3dc));
    pVariantManager->ReleaseVariantRenderResource(
        pScreen->m_nTimedVariantResourceId394);
    pVariantManager->ReleaseVariantRenderResource(
        *(int *)((char *)pScreen + 0x3e0));
}

static void LEMBALL_FASTCALL DrawFunctionalLevelSelectionStatus(
    void *pObject) {
    GameLevelSelectionScreenView *pScreen;
    const int *pLayout;
    short Point[2];
    short Rect[4];

    pScreen = (GameLevelSelectionScreenView *)pObject;
    pLayout = pScreen->m_pLayout3D4;
    if (pLayout == 0) {
        return;
    }
    Point[0] = (short)pLayout[30];
    Point[1] = (short)pLayout[31];
    EmitVariantRenderResourceEntry(
        pScreen->m_abVariantRenderManager14, Rect, Point,
        pScreen->m_nResourceId3D8, 0, pScreen->m_pSelector3C4, 0);
    Point[0] = (short)pLayout[24];
    Point[1] = (short)pLayout[25];
    EmitVariantRenderResourceEntry(
        pScreen->m_abVariantRenderManager14, Rect, Point,
        pScreen->m_nResourceId3DC, 0, pScreen->m_pSelector3C0, 0);
    Point[0] = (short)pLayout[18];
    Point[1] = (short)pLayout[19];
    EmitVariantRenderResourceEntry(
        pScreen->m_abVariantRenderManager14, Rect, Point,
        pScreen->m_nResourceId3E0, 0, pScreen->m_pSelector3C8, 0);
}

static void LEMBALL_FASTCALL DrawFunctionalLevelSelectionText(
    void *pObject) {
    static const char *s_apszSkillNames[5] = {
        "Fun", "Tricky", "Taxing", "Mayhem", "Network"
    };
    GameLevelSelectionScreenView *pScreen;
    MappedListResourceTable *pMappedList;
    short Point[2];
    short Relative[2];
    int i;
    int nPack;

    pScreen = (GameLevelSelectionScreenView *)pObject;
    if (pScreen->m_fBuildingRenderEntries364 == 0 ||
        pScreen->m_pMappedListResources98 == 0) {
        return;
    }
    pMappedList = (MappedListResourceTable *)
        pScreen->m_pMappedListResources98;
    pMappedList->ResetDrawEntryPool();
    Relative[0] = 0;
    Relative[1] = 0;
    for (i = 0; i < 3; ++i) {
        if (pScreen->m_aTitlePositions464[i][0] != -1 &&
            pScreen->m_aszTitleLines3FC[i][0] != '\0') {
            Point[0] = (short)pScreen->m_aTitlePositions464[i][0];
            Point[1] = (short)pScreen->m_aTitlePositions464[i][1];
            pMappedList->SubmitRenderEntry(
                pScreen->m_pRenderContext88, Point, Relative,
                pScreen->m_nMappedListRemapResourceId384,
                pScreen->m_aszTitleLines3FC[i], 0x20, 0);
        }
    }
    Point[0] = (short)pScreen->m_pLayout3D4[22];
    Point[1] = (short)pScreen->m_pLayout3D4[23];
    pMappedList->SubmitRenderEntry(
        pScreen->m_pRenderContext88, Point, Relative,
        pScreen->m_nMappedListRemapResourceId384,
        pScreen->m_abTime45C, 0x20, 0);
    nPack = ((GameLevelProgressStateView *)g_pLevelProgressState)
                ->m_nCurrentPack08;
    if (nPack < 0 || 4 < nPack) {
        nPack = 0;
    }
    Point[0] = (short)pScreen->m_pLayout3D4[40];
    Point[1] = (short)pScreen->m_pLayout3D4[41];
    pMappedList->SubmitRenderEntry(
        pScreen->m_pRenderContext88, Point, Relative,
        pScreen->m_nMappedListRemapResourceId384,
        (void *)s_apszSkillNames[nPack], 0x20, 0);
}

static void RefreshFunctionalLevelSelectionMetadata(
    GameLevelSelectionScreenView *pScreen) {
    GameLevelProgressStateView *pProgress;
    int nPack;
    int nCurrentLevel;
    int nUnlockedCap;
    GameLevelMetadataView Metadata;

    pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
    nPack = pProgress->m_nCurrentPack08;
    nCurrentLevel = pProgress->m_anSnapshotPackCaps3C[nPack];
    memset(&Metadata, 0, sizeof(Metadata));
    if (ReadFunctionalLevelMetadata(nPack, nCurrentLevel, &Metadata) == 0 ||
        Metadata.m_szTitle08[0] == '\0') {
        wsprintfA(Metadata.m_szTitle08, "LEVEL %d", nCurrentLevel + 1);
    }
    strncpy(pScreen->m_aszTitleLines3FC[0], Metadata.m_szTitle08, 0x1f);
    pScreen->m_aszTitleLines3FC[0][0x1f] = '\0';
    pScreen->m_aszTitleLines3FC[1][0] = '\0';
    pScreen->m_aszTitleLines3FC[2][0] = '\0';
    pScreen->m_nLevelTime3F8 = Metadata.m_nTimeSeconds06;
    wsprintfA(pScreen->m_abTime45C, "%d:%02d",
              Metadata.m_nTimeSeconds06 / 60,
              Metadata.m_nTimeSeconds06 % 60);
    pScreen->m_cSkillStatus3F4 = Metadata.m_cSkill00;
    pScreen->m_cStatus3EC = Metadata.m_cPlayers02;
    pScreen->m_cNetworkStatus3F0 = Metadata.m_cNetworkPlayers04;
    pScreen->m_fPreviousLocked484 = nCurrentLevel == 0;
    if (nPack == 4) {
        pScreen->m_fNextLocked480 = nCurrentLevel >= 10;
    } else {
        nUnlockedCap = pProgress->m_anUnlockedPackCaps28[nPack];
        pScreen->m_fNextLocked480 = nCurrentLevel >= nUnlockedCap;
    }
    pScreen->m_fSelectorDirty360 = 1;
    pScreen->m_fFrameDirty330 = 1;
}

static int GetFunctionalLevelBinResourceId(int nSkill, int nLevel) {
    static const int s_anFirstResourceIds[5] = {
        0x244, 0x25d, 0x277, 0x294, 0x2aa
    };

    if ((unsigned int)nSkill >= 5U) {
        nSkill = 4;
    }
    return s_anFirstResourceIds[nSkill] + nLevel;
}

static int ReadFunctionalLevelMetadata(
    int nSkill, int nLevel, GameLevelMetadataView *pMetadata) {
    typedef void (LEMBALL_FASTCALL *LoadProc)(void *, int);
    typedef unsigned char *(LEMBALL_FASTCALL *GetDataProc)(void *, int);
    int *pResource;
    unsigned char *pChunk;
    unsigned char *pPayload;
    unsigned int nChunkType;
    unsigned int cbChunk;
    unsigned int cbAligned;
    unsigned short nVersion;
    unsigned short cEntries;
    unsigned short nTotal;
    unsigned int i;

    pResource = LoadBinResource(
        GetFunctionalLevelBinResourceId(nSkill, nLevel));
    if (pResource == 0) {
        return 0;
    }
    if (pResource[4] == 0) {
        ((LoadProc)(*(void ***)pResource)[7])(pResource, 0);
    } else {
        pResource[9] = 0;
    }
    ++pResource[2];
    pChunk = ((GetDataProc)(*(void ***)pResource)[10])(pResource, 0);
    if (pChunk == 0) {
        --pResource[2];
        ReleaseTypedResourceObjectReference(pResource);
        return 0;
    }

    for (;;) {
        nChunkType = *(unsigned int *)(pChunk + 0);
        cbChunk = *(unsigned int *)(pChunk + 4);
        pPayload = pChunk + 8;
        if (nChunkType == 0x41492020U) {
            nVersion = 0;
            if (4 < cbChunk - 8) {
                nVersion = *(unsigned short *)pPayload;
                pPayload += 2;
            }
            pMetadata->m_nTimeSeconds06 =
                *(unsigned short *)(pPayload + 2);
            if (nVersion < 4) {
                pMetadata->m_cPlayers02 = 4;
                pMetadata->m_cSkill00 = 1;
            } else {
                pMetadata->m_cPlayers02 =
                    *(unsigned short *)(pPayload + 4);
                pMetadata->m_cSkill00 =
                    *(unsigned short *)(pPayload + 6);
            }
        } else if (nChunkType == 0x4e414d45U) {
            strcpy(pMetadata->m_szTitle08, (const char *)pPayload);
            pMetadata->m_szTitle08[0x20] = '\0';
        } else if (nChunkType == 0x4e455457U ||
                   nChunkType == 0x504c5331U) {
            cEntries = *(unsigned short *)pPayload;
            nTotal = 0;
            pPayload += 2;
            for (i = 0; i < cEntries; ++i) {
                pPayload += 6;
                nTotal = (unsigned short)(
                    nTotal + *(unsigned short *)pPayload);
                pPayload += 2;
            }
            if (nChunkType == 0x4e455457U) {
                if (g_nSelectedNetworkLobbyPeerId == 0 ||
                    *(int *)(g_nSelectedNetworkLobbyPeerId + 0x1c) == 1) {
                    pMetadata->m_cNetworkPlayers04 = nTotal;
                } else {
                    pMetadata->m_cPlayers02 = nTotal;
                }
            } else if (g_nSelectedNetworkLobbyPeerId == 0 ||
                       *(int *)(g_nSelectedNetworkLobbyPeerId + 0x1c) == 1) {
                pMetadata->m_cPlayers02 = nTotal;
            } else {
                pMetadata->m_cNetworkPlayers04 = nTotal;
            }
        } else if (nChunkType == 0x454e443fU) {
            --pResource[2];
            ReleaseTypedResourceObjectReference(pResource);
            return 1;
        }
        cbAligned = cbChunk;
        if ((cbAligned & 3U) != 0) {
            cbAligned = (cbAligned & ~3U) + 4;
        }
        pChunk += cbAligned;
    }
}

static void SelectFunctionalLevel(
    GameLevelSelectionScreenView *pScreen, int nDirection) {
    GameLevelProgressStateView *pProgress;
    static const int s_anLevelCaps[5] = { 0x18, 0x19, 0x1c, 0x15, 0x0b };
    int nPack;

    pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
    nPack = pProgress->m_nCurrentPack08;
    if (nDirection < 0) {
        if (0 < pProgress->m_nCurrentLevel00) {
            --pProgress->m_nCurrentLevel00;
        }
    } else if (pProgress->m_nCurrentLevel00 < s_anLevelCaps[nPack]) {
        ++pProgress->m_nCurrentLevel00;
        if (nPack != 4 &&
            pProgress->m_anUnlockedPackCaps28[nPack] <
                pProgress->m_nCurrentLevel00) {
            pProgress->m_anUnlockedPackCaps28[nPack] =
                pProgress->m_nCurrentLevel00;
        }
    }
    if (nPack < 4) {
        pProgress->m_anSnapshotPackCaps3C[nPack] =
            pProgress->m_nCurrentLevel00;
    }
    RefreshFunctionalLevelSelectionMetadata(pScreen);
}

static void LEMBALL_FASTCALL NoopFunctionalLevelSelectionHook(void *) {
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
