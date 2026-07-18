#include "FRONTEND/MAINMENU.H"

#include "ENGINE/GDI/VSGDI.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/MEDIA/EFFSTRM.H"
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
extern void *LoadPalResource(int nResourceId);
extern int *LoadBinResource(int nResourceId);
extern void LoadVariantRenderResource(void *pManager, int nResourceId);
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
extern void *LEMBALL_FASTCALL InitializeFramedScreenRenderChildEntry(
    void *pObject);
extern void LEMBALL_FASTCALL RestoreLevelSelectionRenderChildEntryVtable(
    void *pObject);
extern void SetLevelScreenStatusIndicatorMode(int nMode, int nValue);
extern void LEMBALL_FASTCALL SetStatusIndicatorActive(
    void *pObject, int nUnused, int fActive);
extern void AppendPointerQueueEntry(void *pQueue, void *pEntry);
extern int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresent(
    int *pPrimaryContext);
extern int LEMBALL_FASTCALL IsPrimaryContextWindowReadyForPresentThunk(
    int *pPrimaryContext);
extern void TogglePrimaryContextDisplayMode(
    ShellUiPrimaryContextShell *pShell);
extern void PASCAL SetAudioMusicAndEffectVolumeByte(unsigned char nVolume);
extern void PASCAL SetVariantResourceEffectMasterVolume(unsigned char nVolume);
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
extern void noop_screen_callback_00446f90(void);
extern void update_options_screen(void);
extern int PASCAL return_zero_screen_callback_00446f60(void *pUnused);
extern int PASCAL return_zero_screen_callback_00446fa0(void *pUnused);
extern void noop_main_menu_screen_hook(void);
extern "C" int __cdecl _purecall(void);

extern void *g_pLevelProgressState;
extern void *g_pActiveNetworkLobbyScreen;
extern void *g_pNetworkLobbyVsnetRuntime;
extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pActiveNetworkLobbyTransportController;
extern void *g_pSharedRenderDispatchQueue;
extern void *g_pMainResourceArchive;
extern int g_nSelectedNetworkLobbyPeerId;
extern void *g_pSessionRandomState;
extern void *g_pVariantResourceEntryManager;
extern void *g_GAME_RenderQueueNodeVtableSlots[4];
extern void LEMBALL_FASTCALL UnregisterNetworkLobbyVsnetRuntimeFromTransport(
    void *pVsnetRuntime);
extern void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableA(
    void *pObject);
extern void *LEMBALL_FASTCALL ConstructWindowOwnerRenderContext(void *pObject);
extern void LEMBALL_FASTCALL DestroyWindowOwnerRenderContext(void *pObject);
extern void *LEMBALL_FASTCALL InitializeEmptyGeometryOwnerRect(void *pObject);
extern void LEMBALL_FASTCALL AppendGeometryOwnerChild(
    void *pOwner, int nUnusedEdx, void *pChild);
extern void LEMBALL_FASTCALL RemoveGeometryOwnerChild(
    void *pOwner, int nUnusedEdx, void *pChild);
extern void ThunkRefreshNetworkLobbyPlayerEntries(int nNetworkLobbyScreen);

// GLOBAL: LEMBALL 0x004a7b64
int g_nQueuedVariantChildSlotManagerModeSelectedResourceId;

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

class GameBaseModeScreen {
public:
    void QueueActiveBaseModeUserAction(int nAction, int nPhase);
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
    unsigned char m_abReserved39C[0x08];
    void *m_pInputPanelOwner3A4;
    const int *m_pLayout3A8;
    const int *m_pKeyResourceIds3AC;
    char m_szPassword3B0[0x10];
    int m_aKeyPositions3C0[12][2];
    int m_cPassword420;
    void *m_apKeyOverlays424[12];
    unsigned char m_abInputPanelPointSink454[0x10];
    unsigned char m_abInputPanelFrameSelector464[0x10];
    void *m_pInputPanelRenderContext474;
    int m_xSelected478;
    int m_ySelected47C;
    int m_nSelectedKey480;
    int m_fPasswordValid484;
    int m_nResultState488;
    DWORD m_dwResultStart48C;
    DWORD m_dwResultDeadline490;
};

struct GameNetworkLobbyScreenView : public GameFramedScreenBase {
    void *m_pTextBuffer398;
    int m_fTextPrompt39C;
    int m_nReserved3A0;
    int m_nPendingPrompt3A4;
    int m_nStatus3A8;
    int m_nDrawnStatus3AC;
    DWORD m_dwStatusStart3B0;
    DWORD m_dwStatusDuration3B4;
    int m_nReserved3B8;
    int m_nReserved3BC;
    int m_fBlink3C0;
    int m_fRefreshPlayers3C4;
    DWORD m_dwBlinkStart3C8;
    void *m_pPlayerEntries3CC;
    int m_nSelectedPlayer3D0;
    int m_nResolvedPlayer3D4;
    int m_cVisiblePlayers3D8;
    int m_fPromptActive3DC;
    int m_fPendingText3E0;
    const char *m_pszLocalAddress3E4;
    const char *m_pszLocalComputer3E8;
    unsigned char m_abPointSink3EC[0x10];
    unsigned char m_abReserved3FC[0x18];
    int m_anPaletteRemaps414[6];
    const int *m_pLayout42C;
    int m_nDeferredStatus430;
    int m_nButtonSelection434;
};

struct GameBoundedTextBuffer {
    int m_cCapacity00;
    int m_cLength04;
    int m_cAllocated08;
    char *m_pszText0C;
};

struct GameNetworkLobbyPeerStateView {
    char m_abReserved00[0x2c];
    char m_szName2C[9];
    char m_szComputer35[0x17];
    int m_fActive4C;
};

struct GameNetworkLobbyPeerDescriptorView {
    char m_abReserved00[8];
    int m_nAddressOwnerOffset08;
};

struct GameNetworkLobbyTrackedPeerView {
    void **m_pVtable00;
    GameNetworkLobbyPeerDescriptorView *m_pDescriptor04;
};

struct GameNetworkLobbyAddressView {
    virtual const char *GetAddressString(void) = 0;
};

struct GameNetworkLobbyRuntimeView {
    char m_abReserved00[0x14];
    GameNetworkLobbyPeerStateView *m_pPeerStates14;
    char m_abReserved18[8];
    GameNetworkLobbyTrackedPeerView *m_apTrackedPeers20[10];
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
typedef char GameNetworkLobbyScreenView_size_must_be_0x438[
    sizeof(GameNetworkLobbyScreenView) == 0x438 ? 1 : -1];

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

class GameLevelTitleGlyphListInterface {
public:
    virtual void *GetGlyphListResource(void) = 0;
    virtual void *FindGlyph(unsigned int ch) = 0;
};

class GameGeometryDispatchInterface {
public:
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
    virtual void BeginFrame(void) = 0;
};

class GameUploadOwnerInterface {
public:
    virtual void Reserved00(void) = 0;
    virtual void Reserved01(void) = 0;
    virtual int GetUploadState(void) = 0;
};

class GameFramedScreenVirtualInterface {
public:
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
    virtual void DrawOverlayVariant(void) = 0;
    virtual void DrawScreenContents(void) = 0;
    virtual void FinishBuildingRenderEntries(void) = 0;
};

class GameNetworkRuntimeVirtualInterface {
public:
    virtual void Reserved(void) = 0;
    virtual void Close(int nCloseFlag) = 0;
};

class GamePrimaryContextSlot24Interface {
public:
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
    virtual int IsMainMenuTimeoutAllowed(void) = 0;
};
typedef int (LEMBALL_FASTCALL *GameIntNoArgVirtual)(void *, int);
typedef int (LEMBALL_FASTCALL *GameIntOneArgVirtual)(void *, int, void *);

static void LEMBALL_FASTCALL DestroyMainMenuScreenThunk00401190(
    void *pObject);
static void LEMBALL_FASTCALL DestroyFramedScreenBaseThunk00402081(
    void *pObject);
static void LEMBALL_FASTCALL ResetFramedScreenVariantBuffersThunk(
    void *pObject);
static int LEMBALL_FASTCALL MainMenuSlot21Thunk(
    void *pObject, int nUnusedEdx, void *pArgument);
static int LEMBALL_FASTCALL FramedScreenSlot15Thunk(
    void *pObject, int nUnusedEdx, void *pArgument);
static void LEMBALL_FASTCALL FramedScreenSlot14Thunk(void *pObject);
static void LEMBALL_FASTCALL FramedScreenSlot20Thunk(void *pObject);
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
static void *LEMBALL_FASTCALL DeleteFramedScreenBaseThunk(
    void *pObject, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteAdjustedFramedScreenBaseThunk(
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
static short *GetFunctionalVariantFrameExtent(
    void *pManager, short *pExtent, int nResourceId, int iFrame);
static void LEMBALL_FASTCALL DrawFunctionalLevelSelectionText(
    void *pObject);
static void LEMBALL_FASTCALL RefreshFunctionalLevelSelectionMetadata(
    GameLevelSelectionScreenView *pScreen);
static void MeasureFunctionalLevelTitleExtent(
    void *pResource, short *pExtent, const char *pszText);
static int ReadFunctionalLevelMetadata(
    int nSkill, int nLevel, GameLevelMetadataView *pMetadata);
static void SelectFunctionalLevel(
    GameLevelSelectionScreenView *pScreen, int nDirection);
static GameProgressiveVariantFrameSelector *
ConstructLevelSelectionFrameSelector(void *pManager, int nResourceId);
static void InitializeFunctionalLevelSelectionPaletteRemap(
    GameLevelSelectionScreenView *pScreen);
static void ReleaseFunctionalLevelSelectionPaletteRemap(
    GameLevelSelectionScreenView *pScreen);
static void LEMBALL_FASTCALL RefreshAndDrawFramedScreenBase(
    void *pObject, int, void *);
static void LEMBALL_FASTCALL DrawFramedScreenBase(void *pObject);
static void LEMBALL_FASTCALL DrawFramedScreenFrameLayer(void *pObject);
static void LEMBALL_FASTCALL EmitFramedScreenOverlayVariantEntry(
    void *pObject);
static void LEMBALL_FASTCALL EmitQueuedVariantChildSlotEntry(
    void *pObject);
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
static void LEMBALL_FASTCALL DestroyFramedScreenBase(void *pObject);
static int LEMBALL_FASTCALL HandleMainMenuInput(
    void *pObject, int, void *pEvent);
static void LEMBALL_FASTCALL UpdateMainMenuScreen(void *pObject);
static void *LEMBALL_FASTCALL DeleteMainMenuScreen(
    void *pObject, int, int fDelete);
static void *LEMBALL_FASTCALL DeleteAdjustedMainMenuScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteFramedScreenBase(
    void *pObject, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteAdjustedFramedScreenBase(
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
static void LEMBALL_FASTCALL ActivatePaintballSequenceMedia(void *pObject,
                                                            int nUnused);
static void DeactivatePaintballSequenceMedia(
    GamePaintballSequenceScreenView *pScreen);
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
static void LEMBALL_FASTCALL DrawPasswordEntryInputArea(void *pObject);
static void LEMBALL_FASTCALL DrawPasswordEntryText(void *pObject);
static void InitializePasswordEntryRenderContext(
    GamePasswordEntryScreenView *pScreen);
static void *LEMBALL_FASTCALL DeletePasswordEntryScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteAdjustedPasswordEntryScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteNetworkLobbyScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static void *LEMBALL_FASTCALL DeleteAdjustedNetworkLobbyScreen(
    void *pObject, int nUnusedEdx, int fDelete);
static int LEMBALL_FASTCALL HandleNetworkLobbyScreenInput(
    void *pObject, int nUnusedEdx, void *pEvent);
static void LEMBALL_FASTCALL UpdateNetworkLobbyScreen(void *pObject);
static void LEMBALL_FASTCALL DrawNetworkLobbyScreen(void *pObject);

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
























// GLOBAL: LEMBALL 0x00497af0
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

// GLOBAL: LEMBALL 0x00497ae0
void *g_GAME_MainMenuQueueVtable[4] = {
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteAdjustedMainMenuScreenThunk,
    (void *)HandleFramedScreenQueueEventThunk,
    0
};

// GLOBAL: LEMBALL 0x00497ad8
void *g_GAME_MainMenuCallbackVtable[2] = {
    (void *)VariantResourceBundleNoopThunk,
    0
};

static void *g_GAME_OptionsScreenVtable[22];
static void *g_GAME_OptionsQueueVtable[4];
static int g_fOptionsScreenVtableInitialized;
static void *g_GAME_PaintballSequenceScreenVtable[22];
static void *g_GAME_PaintballSequenceQueueVtable[4];
static void *g_GAME_PaintballSequenceMediaOwnerVtable[50];
static int g_fPaintballSequenceVtableInitialized;
static int g_fPaintballSequenceMediaOwnerVtableInitialized;
static void *g_GAME_PasswordEntryScreenVtable[22];
static void *g_GAME_PasswordEntryQueueVtable[4];
static int g_fPasswordEntryVtableInitialized;
static void *g_GAME_NetworkLobbyScreenVtable[22];
static void *g_GAME_NetworkLobbyQueueVtable[4];
static int g_fNetworkLobbyVtableInitialized;

static const int g_GAME_NetworkLobbyLayoutStandard[40] = {
    60, 375, 258, 375, 455, 375, 8, 11,
    624, 192, 8, 213, 624, 48, 8, 271,
    624, 96, 312, 283, 34, 21, 25, 318,
    76, 222, 97, 67, 16, 70, 265, 0,
    500, 0, 10, 32, 609, 29, 271, 0
};
static const int g_GAME_NetworkLobbyLayoutCompact[40] = {
    30, 186, 129, 186, 227, 186, 8, 3,
    304, 96, 8, 101, 304, 30, 8, 135,
    304, 48, 159, 143, 15, 9, 19, 158,
    32, 109, 49, 34, 12, 34, 129, 2,
    244, 0, 0, 14, 297, 12, 128, 0
};
static const int g_GAME_NetworkLobbyButtonResourcesStandard[3] = {
    0x1bf, 0x1c0, 0x1b3
};
static const int g_GAME_NetworkLobbyButtonResourcesCompact[3] = {
    0x1f3, 0x1f4, 0x1e7
};

static const char *g_GAME_NetworkLobbyStatusText[10] = {
    "No Message",
    "Play with a LAN or specific computer",
    "Enter your name:",
    "Enter your opponents I.P. Address:",
    "Looking for player at ",
    "Looking for players on local network",
    "Initialising Network...",
    "Invalid name or address specified",
    "You must enter your name",
    "Network Facilities not available"
};
static const char g_szNetworkLobbyDividerStandard[] =
    "________________________________";
static const char g_szNetworkLobbyDividerCompact[] =
    "_________________________________";

// GLOBAL: LEMBALL 0x004a0368
char g_szNetworkLobbyLocalPlayerName[0x10] = "";
// GLOBAL: LEMBALL 0x004a0378
char g_szNetworkLobbyJoinAddress[0x10] = "";

static GameBoundedTextBuffer *ConstructBoundedTextBuffer(
    GameBoundedTextBuffer *pBuffer, int cCapacity) {
    pBuffer->m_pszText0C =
        (char *)AllocateVSMemBlock(cCapacity + 1);
    pBuffer->m_cAllocated08 = cCapacity;
    pBuffer->m_cCapacity00 = cCapacity;
    pBuffer->m_cLength04 = 0;
    if (pBuffer->m_pszText0C != 0) {
        pBuffer->m_pszText0C[0] = '\0';
    }
    return pBuffer;
}

void LEMBALL_FASTCALL DestroyFunctionalFramedScreenBase(void *pScreen) {
    DestroyFramedScreenBaseThunk00402081(pScreen);
}

static void ClearBoundedTextBuffer(GameBoundedTextBuffer *pBuffer) {
    if (pBuffer == 0 || pBuffer->m_pszText0C == 0) {
        return;
    }
    pBuffer->m_cLength04 = 0;
    pBuffer->m_pszText0C[0] = '\0';
}

static void AppendBoundedTextCharacter(
    GameBoundedTextBuffer *pBuffer, char ch) {
    if (pBuffer->m_cLength04 < pBuffer->m_cCapacity00) {
        pBuffer->m_pszText0C[pBuffer->m_cLength04++] = ch;
        pBuffer->m_pszText0C[pBuffer->m_cLength04] = '\0';
    }
}

static void CopyBoundedText(
    GameBoundedTextBuffer *pBuffer, const char *pszText) {
    if (pBuffer == 0 || pBuffer->m_pszText0C == 0) {
        return;
    }
    strncpy(pBuffer->m_pszText0C, pszText, pBuffer->m_cAllocated08);
    pBuffer->m_pszText0C[pBuffer->m_cAllocated08] = '\0';
    pBuffer->m_cLength04 = (int)strlen(pBuffer->m_pszText0C);
}

static void SetNetworkLobbyStatus(
    GameNetworkLobbyScreenView *pScreen, int nStatus) {
    pScreen->m_nStatus3A8 = nStatus;
    pScreen->m_dwStatusDuration3B4 = 0;
    pScreen->m_fSelectorDirty360 = 1;
    pScreen->m_fBlink3C0 = 1;
    pScreen->m_dwBlinkStart3C8 = timeGetTime();
}

static void SetTimedNetworkLobbyStatus(
    GameNetworkLobbyScreenView *pScreen, int nStatus, DWORD dwDuration) {
    pScreen->m_nStatus3A8 = nStatus;
    pScreen->m_dwStatusStart3B0 = timeGetTime();
    pScreen->m_dwStatusDuration3B4 = dwDuration;
    pScreen->m_fSelectorDirty360 = 1;
    pScreen->m_fBlink3C0 = 1;
    pScreen->m_dwBlinkStart3C8 = timeGetTime();
}

static void ShowNetworkLobbyTextPrompt(
    GameNetworkLobbyScreenView *pScreen, int nPrompt, int fClear) {
    GameBoundedTextBuffer *pBuffer;

    pBuffer = (GameBoundedTextBuffer *)pScreen->m_pTextBuffer398;
    if (pBuffer == 0 || pBuffer->m_pszText0C == 0) {
        SetTimedNetworkLobbyStatus(pScreen, 9, 6000);
        return;
    }
    pScreen->m_fTextPrompt39C = 0;
    if (nPrompt != 3) {
        pScreen->m_nReserved3A0 = nPrompt;
    }
    if (fClear != 0) {
        ClearBoundedTextBuffer(pBuffer);
    }
    pBuffer->m_cCapacity00 =
        pScreen->m_nReserved3A0 == 1 ? 8 : 20;
    SetNetworkLobbyStatus(pScreen, pScreen->m_nReserved3A0 + 1);
    pScreen->m_fTextPrompt39C = 1;
}

static void CommitNetworkLobbyTextPrompt(
    GameNetworkLobbyScreenView *pScreen) {
    GameBoundedTextBuffer *pBuffer;

    pBuffer = (GameBoundedTextBuffer *)pScreen->m_pTextBuffer398;
    pScreen->m_fTextPrompt39C = 0;
    if (pScreen->m_nReserved3A0 == 1) {
        if (pBuffer->m_pszText0C[0] == '\0') {
            SetTimedNetworkLobbyStatus(pScreen, 8, 6000);
            return;
        }
        strncpy(g_szNetworkLobbyLocalPlayerName,
                pBuffer->m_pszText0C,
                sizeof(g_szNetworkLobbyLocalPlayerName) - 1);
        g_szNetworkLobbyLocalPlayerName[
            sizeof(g_szNetworkLobbyLocalPlayerName) - 1] = '\0';
        if (pScreen->m_nReserved3BC == 0) {
            g_szNetworkLobbyJoinAddress[0] = '\0';
            pScreen->m_fPendingText3E0 = 1;
        } else {
            pScreen->m_nPendingPrompt3A4 = 2;
        }
    } else if (pScreen->m_nReserved3A0 == 2) {
        strncpy(g_szNetworkLobbyJoinAddress,
                pBuffer->m_pszText0C,
                sizeof(g_szNetworkLobbyJoinAddress) - 1);
        g_szNetworkLobbyJoinAddress[
            sizeof(g_szNetworkLobbyJoinAddress) - 1] = '\0';
        pScreen->m_fPendingText3E0 = 1;
    }
}

static void BeginNetworkLobbyTextPrompt(
    GameNetworkLobbyScreenView *pScreen, int nAddressPrompt) {
    GameBoundedTextBuffer *pBuffer;

    pBuffer = (GameBoundedTextBuffer *)pScreen->m_pTextBuffer398;
    pScreen->m_nReserved3BC = nAddressPrompt;
    pScreen->m_nReserved3B8 = 0;
    pScreen->m_fTextPrompt39C = 0;
    pScreen->m_nDeferredStatus430 = 0;
    if (g_szNetworkLobbyLocalPlayerName[0] == '\0') {
        ShowNetworkLobbyTextPrompt(pScreen, 1, 1);
    } else {
        CopyBoundedText(pBuffer, g_szNetworkLobbyLocalPlayerName);
        ShowNetworkLobbyTextPrompt(pScreen, 1, 0);
    }
}

static unsigned char g_GAME_NetworkLobbyPaletteRemaps[6][8] = {
    {2, 0xf1, 0x51, 0x5d, 0x3d, 0, 0, 0},
    {2, 0xf1, 0x51, 0x45, 0x20, 0, 0, 0},
    {2, 0xf1, 0x51, 0x65, 0x31, 0, 0, 0},
    {2, 0xf1, 0x51, 0xad, 0xbd, 0, 0, 0},
    {2, 0xf1, 0x51, 0x8c, 0xad, 0, 0, 0},
    {2, 0xf1, 0x51, 0xa8, 0x6c, 0, 0, 0}
};

static void *g_GAME_NetworkLobbyPlayerEntryVtable[8];
static void **g_pNetworkLobbyPlayerEntryBaseVtable;
static int g_fNetworkLobbyPlayerEntryVtableInitialized;

static void LEMBALL_FASTCALL MarkNetworkLobbyPlayerEntryActive(
    void *pEntry) {
    *(int *)((char *)pEntry + 0x38) = 1;
}

static void LEMBALL_FASTCALL ClearNetworkLobbyPlayerEntryActive(
    void *pEntry) {
    *(int *)((char *)pEntry + 0x38) = 0;
}

static void LEMBALL_FASTCALL MarkNetworkLobbyPlayerEntryEventDirty(
    void *pEntry, int, const VS_XYPair *, int nEvent) {
    if (nEvent == 0 || nEvent == 3) {
        *(int *)((char *)pEntry + 0x3c) = 1;
    }
}

static void InitializeNetworkLobbyPlayerEntryVtable(void **pBaseVtable) {
    int i;

    if (g_fNetworkLobbyPlayerEntryVtableInitialized != 0) {
        return;
    }
    g_pNetworkLobbyPlayerEntryBaseVtable = pBaseVtable;
    for (i = 0; i < 8; ++i) {
        g_GAME_NetworkLobbyPlayerEntryVtable[i] = pBaseVtable[i];
    }
    g_GAME_NetworkLobbyPlayerEntryVtable[4] =
        (void *)MarkNetworkLobbyPlayerEntryActive;
    g_GAME_NetworkLobbyPlayerEntryVtable[5] =
        (void *)ClearNetworkLobbyPlayerEntryActive;
    g_GAME_NetworkLobbyPlayerEntryVtable[6] =
        (void *)MarkNetworkLobbyPlayerEntryEventDirty;
    g_fNetworkLobbyPlayerEntryVtableInitialized = 1;
}

static void ConstructNetworkLobbyPlayerEntry(void *pEntry) {
    InitializeEmptyGeometryOwnerRect(pEntry);
    InitializeNetworkLobbyPlayerEntryVtable(*(void ***)pEntry);
    *(void ***)pEntry = g_GAME_NetworkLobbyPlayerEntryVtable;
    *(int *)((char *)pEntry + 0x38) = 0;
    *(int *)((char *)pEntry + 0x3c) = 0;
    *(int *)((char *)pEntry + 0x40) = 0;
}

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

static void InitializePaintballSequenceMediaOwnerVtable(void *pOwner) {
    int i;

    if (g_fPaintballSequenceMediaOwnerVtableInitialized != 0) {
        return;
    }
    for (i = 0; i < 50; ++i) {
        g_GAME_PaintballSequenceMediaOwnerVtable[i] =
            (*(void ***)pOwner)[i];
    }
    g_GAME_PaintballSequenceMediaOwnerVtable[49] =
        (void *)ActivatePaintballSequenceMedia;
    g_fPaintballSequenceMediaOwnerVtableInitialized = 1;
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
    g_GAME_PasswordEntryScreenVtable[18] = (void *)DrawPasswordEntryInputArea;
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

static void InitializeFunctionalNetworkLobbyVtables(void) {
    int i;

    if (g_fNetworkLobbyVtableInitialized != 0) {
        return;
    }
    for (i = 0; i < 22; ++i) {
        g_GAME_NetworkLobbyScreenVtable[i] = g_GAME_MainMenuScreenVtable[i];
    }
    g_GAME_NetworkLobbyScreenVtable[0] = (void *)DeleteNetworkLobbyScreen;
    g_GAME_NetworkLobbyScreenVtable[14] = (void *)UpdateNetworkLobbyScreen;
    g_GAME_NetworkLobbyScreenVtable[15] = (void *)HandleNetworkLobbyScreenInput;
    g_GAME_NetworkLobbyScreenVtable[16] = (void *)MainMenuSlot18Thunk;
    g_GAME_NetworkLobbyScreenVtable[17] = (void *)MainMenuSlot19Thunk;
    g_GAME_NetworkLobbyScreenVtable[18] = (void *)MainMenuSlot18Thunk;
    g_GAME_NetworkLobbyScreenVtable[19] = (void *)DrawNetworkLobbyScreen;
    g_GAME_NetworkLobbyQueueVtable[0] =
        (void *)InitializeNonZrleVariantRenderEntry;
    g_GAME_NetworkLobbyQueueVtable[1] =
        (void *)DeleteAdjustedNetworkLobbyScreen;
    g_GAME_NetworkLobbyQueueVtable[2] =
        (void *)HandleFramedScreenQueueEventThunk;
    g_GAME_NetworkLobbyQueueVtable[3] = 0;
    g_fNetworkLobbyVtableInitialized = 1;
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

static const int g_GAME_LevelSelectionBackResourcesStandard[1] = {0x1b3};
static const int g_GAME_LevelSelectionPlayResourcesStandard[1] = {0x1b6};
static const int g_GAME_LevelSelectionPreviousResourcesStandard[2] = {
    0x1b1, 0x1b2
};
static const int g_GAME_LevelSelectionNextResourcesStandard[2] = {
    0x1bd, 0x1be
};
static const int g_GAME_LevelSelectionBackResourcesCompact[1] = {0x1e7};
static const int g_GAME_LevelSelectionPlayResourcesCompact[1] = {0x1ea};
static const int g_GAME_LevelSelectionPreviousResourcesCompact[2] = {
    0x1e5, 0x1e6
};
static const int g_GAME_LevelSelectionNextResourcesCompact[2] = {
    0x1f1, 0x1f2
};

static const int g_GAME_LevelSelectionPaletteSourceIndices[10] = {
    0xa4, 0xa2, 0x80, 0x64, 0x4b, 0x5a, 0x34, 0x37, 0x3a, 0x96
};
static const int g_GAME_LevelSelectionPaletteTargetIndices[10] = {
    0xb4, 0xb7, 0xb9, 0xbc, 0xbe, 0xbc, 0xbf, 0xbe, 0xbf, 0xb8
};

// GLOBAL: LEMBALL 0x00497894
static void *g_GAME_FramedScreenCallbackVtable[1] = {
    (void *)VariantResourceBundleNoopThunk
};

// GLOBAL: LEMBALL 0x00497898
static void *g_GAME_FramedScreenQueueVtable[4] = {
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteAdjustedFramedScreenBaseThunk,
    (void *)HandleFramedScreenQueueEventThunk,
    0
};

// GLOBAL: LEMBALL 0x004978a8
static void *g_GAME_FramedScreenBaseVtable[22] = {
    (void *)DeleteFramedScreenBaseThunk,
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
    (void *)FramedScreenSlot14Thunk,
    (void *)FramedScreenSlot15Thunk,
    (void *)_purecall,
    (void *)_purecall,
    (void *)MainMenuSlot18Thunk,
    (void *)MainMenuSlot19Thunk,
    (void *)FramedScreenSlot20Thunk,
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

// FUNCTION: LEMBALL 0x00401505
static int LEMBALL_FASTCALL FramedScreenSlot15Thunk(
    void *pObject, int nUnusedEdx, void *pArgument)
{
    (void)pObject;
    (void)nUnusedEdx;
    return return_zero_screen_callback_00446f60(pArgument);
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

// FUNCTION: LEMBALL 0x00402081
static void LEMBALL_FASTCALL DestroyFramedScreenBaseThunk00402081(
    void *pObject)
{
    DestroyFramedScreenBase(pObject);
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

// FUNCTION: LEMBALL 0x004022de
static void *LEMBALL_FASTCALL DeleteAdjustedFramedScreenBaseThunk(
    void *pObject, int nUnusedEdx, int fDelete)
{
    return DeleteAdjustedFramedScreenBase(pObject, nUnusedEdx, fDelete);
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

// FUNCTION: LEMBALL 0x00402ac7
static void LEMBALL_FASTCALL FramedScreenSlot20Thunk(void *pObject) {
    (void)pObject;
    noop_screen_callback_00446f90();
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

// FUNCTION: LEMBALL 0x00403247
static void LEMBALL_FASTCALL FramedScreenSlot14Thunk(void *pObject) {
    (void)pObject;
    update_options_screen();
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

// FUNCTION: LEMBALL 0x004037e7
static void *LEMBALL_FASTCALL DeleteFramedScreenBaseThunk(
    void *pObject, int nUnusedEdx, int fDelete)
{
    return DeleteFramedScreenBase(pObject, nUnusedEdx, fDelete);
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

// FUNCTION: LEMBALL 0x00445790
static void LEMBALL_FASTCALL DestroyFramedScreenBase(void *pObject) {
    GameFramedScreenBase *pScreen;
    DWORD dwStartTime;
    DWORD dwNow;

    pScreen = (GameFramedScreenBase *)pObject;
    pScreen->m_pVtable00 = g_GAME_FramedScreenBaseVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_FramedScreenCallbackVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_FramedScreenQueueVtable;
    g_pActiveNetworkLobbyScreen = 0;
    if (pScreen->m_fNetworkScreen8C != 0 &&
        pScreen->m_nRequestedMode374 == 2) {
        if (g_pNetworkLobbyVsnetRuntime != 0) {
            UnregisterNetworkLobbyVsnetRuntimeFromTransport(
                g_pNetworkLobbyVsnetRuntime);
        }
        if (g_pActiveNetworkRuntimeWindow != 0) {
            dwStartTime = timeGetTime();
            do {
                dwNow = timeGetTime();
                if (dwNow - dwStartTime >= 2000) {
                    break;
                }
            } while (*(int *)((char *)g_pActiveNetworkRuntimeWindow + 0x48) != 0);
        }
        if (g_pNetworkLobbyVsnetRuntime != 0) {
            ((GameNetworkRuntimeVirtualInterface *)
                 g_pNetworkLobbyVsnetRuntime)->Close(1);
            g_pNetworkLobbyVsnetRuntime = 0;
        }
    }
    if (pScreen->m_pTimedFrameSelector34C != 0) {
        FreeVSMemBlock(pScreen->m_pTimedFrameSelector34C);
    }
    ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->UnregisterOrderedRenderDispatchClient(
            pScreen->m_abRenderQueueNode04, 0);
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        ReleaseFramedScreenFrameResources(pScreen);
    }
    if (pScreen->m_pMappedListResources98 != 0) {
        ((MappedListResourceTable *)pScreen->m_pMappedListResources98)
            ->Destroy();
        FreeVSMemBlock(pScreen->m_pMappedListResources98);
    }
    ((MogLoadResourceArchive *)g_pMainResourceArchive)
        ->PruneUnreferencedCachedResourceObjects();
    DestroyFramedScreenRenderEntryGroup(
        pScreen->m_abRenderEntryGroupC8);
    DestroyVariantRenderManager(pScreen->m_abVariantRenderManager14);
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_RenderQueueNodeVtableSlots;
    RestorePackagedSpriteRenderEntryVtableA(pScreen);
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
    ((GameGeometryDispatchInterface *)pGeometryDispatch)->BeginFrame();
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
    unsigned char *pEntry;
    short Rect[4];

    pScreen = (GameFramedScreenBase *)pObject;
    pUploadOwner = *(void **)((char *)pScreen->m_pRenderContext88 + 0x0c);
    nUploadState = ((GameUploadOwnerInterface *)pUploadOwner)->GetUploadState();
    InitializeHelperUploadStatePending(nUploadState);
    AppendPointerQueueEntry(
        pScreen->m_pRenderContext88,
        pScreen->m_abRenderEntryGroupC8 +
            pScreen->m_iRenderEntryGroup328 * 0x260 + 0x1bc);

    if (pScreen->m_fBuildingRenderEntries364 != 0) {
        if (pScreen->m_fTileBackgroundAC == 0) {
            Rect[0] = pScreen->m_cxScreen37C;
            Rect[1] = pScreen->m_cyScreen37E;
            Rect[2] = 0;
            Rect[3] = 0;
            pEntry = pScreen->m_abRenderEntryGroupC8 +
                     pScreen->m_iRenderEntryGroup328 * 0x260 + 0x1c0 +
                     pScreen->m_iPackagedEntry36C * 0x10;
            *(short *)(pEntry + 4) = Rect[0];
            *(short *)(pEntry + 6) = Rect[1];
            *(short *)(pEntry + 8) = Rect[2];
            *(short *)(pEntry + 10) = Rect[3];
            *(int *)(pEntry + 12) = 0;
            ((GameMainMenuRenderEntryInterface *)pEntry)
                ->AppendToQueue(pScreen->m_pRenderContext88);
            ++pScreen->m_iPackagedEntry36C;
        }
        DrawFramedScreenFrameLayer(pScreen);
        ((GameFramedScreenVirtualInterface *)pScreen)
            ->FinishBuildingRenderEntries();
    }
    if (pScreen->m_fBuildingRenderEntries364 == 0) {
        EmitFramedScreenOverlayVariantEntry(pScreen);
        ((GameFramedScreenVirtualInterface *)pScreen)->DrawOverlayVariant();
    }
    ((GameFramedScreenVirtualInterface *)pScreen)->DrawScreenContents();
    if (pScreen->m_fBuildingRenderEntries364 == 0) {
        if (pScreen->m_pSelectorManager334 != 0) {
        EmitMenuSelectorResourceEntries(
            (GameMenuSelectorManager *)pScreen->m_pSelectorManager334);
        }
        if (pScreen->m_pQueuedVariantManager338 != 0) {
            EmitQueuedVariantChildSlotEntry(
                pScreen->m_pQueuedVariantManager338);
        }
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

// FUNCTION: LEMBALL 0x00446050
static void LEMBALL_FASTCALL EmitFramedScreenOverlayVariantEntry(
    void *pObject)
{
    GameFramedScreenBase *pScreen;
    short Rect[4];

    pScreen = (GameFramedScreenBase *)pObject;
    if (pScreen->m_pTimedFrameSelector34C != 0) {
        *(DWORD *)((char *)pScreen->m_pTimedFrameSelector34C + 0x14) =
            timeGetTime();
        ((LevelVariantRenderManagerView *)
             pScreen->m_abVariantRenderManager14)
            ->EmitVariantRenderResourceEntry(
                Rect, &pScreen->m_xOverlay380,
                pScreen->m_nTimedVariantResourceId394, 0,
                pScreen->m_pTimedFrameSelector34C, 0);
    }
}

// FUNCTION: LEMBALL 0x004460a0
static void LEMBALL_FASTCALL ResetFramedScreenVariantBuffers(
    void *pObject)

{
    GameFramedScreenBase *pScreen;

    pScreen = (GameFramedScreenBase *)pObject;
    ((LevelVariantRenderManagerView *)pScreen->m_abVariantRenderManager14)
        ->ResetOrFlipVariantRenderBuffers();
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

// FUNCTION: LEMBALL 0x00447000
static void *LEMBALL_FASTCALL DeleteFramedScreenBase(
    void *pObject, int, int fDelete)
{
    DestroyFramedScreenBaseThunk00402081(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004472f0
static void *LEMBALL_FASTCALL DeleteAdjustedFramedScreenBase(
    void *pObject, int nUnusedEdx, int fDelete)
{
    return DeleteFramedScreenBase(
        (char *)pObject - 4, nUnusedEdx, fDelete);
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
    const int *pPlayResources;
    const int *pSinglePlayerResources;
    const int *pMultiPlayerResources;
    const int *pDisplayResources;
    const int *pPackResources;
    const int *pOptionsResources;
    GameMenuSelectorManager *pSelectorManager;
    GameMenuSelectorManager **ppSelectorManager;
    void *pForegroundBitmap;
    short cxForeground;
    int cRenderGroups;

    pScreen = (GameMainMenuScreenView *)pObject;
    if (pScreen->m_fCompactLayout32C != 0) {
        pScreen->m_panWidgetLayout39C = g_GAME_MainMenuWidgetLayoutCompact;
        pPlayResources = g_GAME_MainMenuWidgetResourcesCompact;
        pMultiPlayerResources = g_GAME_MainMenuWidgetResourcesCompact + 1;
        pDisplayResources = g_GAME_MainMenuWidgetResourcesCompact + 11;
        pSinglePlayerResources = g_GAME_MainMenuWidgetResourcesCompact + 2;
        pPackResources = g_GAME_MainMenuWidgetResourcesCompact + 6;
        pOptionsResources = g_GAME_MainMenuWidgetResourcesCompact + 3;
    } else {
        pScreen->m_panWidgetLayout39C = g_GAME_MainMenuWidgetLayoutStandard;
        pPlayResources = g_GAME_MainMenuWidgetResourcesStandard;
        pMultiPlayerResources = g_GAME_MainMenuWidgetResourcesStandard + 1;
        pDisplayResources = g_GAME_MainMenuWidgetResourcesStandard + 10;
        pSinglePlayerResources = g_GAME_MainMenuWidgetResourcesStandard + 2;
        pPackResources = g_GAME_MainMenuWidgetResourcesStandard + 5;
        pOptionsResources = g_GAME_MainMenuWidgetResourcesStandard + 4;
    }

    cRenderGroups = 1;
    pForegroundEntry = pScreen->m_abRenderEntryGroupC8 + 0x14;
    do {
        pForegroundBitmap = pScreen->m_pForegroundBitmapC0;
        cxForeground = *(short *)((char *)pForegroundBitmap + 0x48);
        *(short *)(pForegroundEntry + 0) =
            (short)((*(short *)((char *)pScreen->m_pPrimaryContext84 + 8) -
                     cxForeground) / 2);
        *(short *)(pForegroundEntry + 2) = 0;
        *(void **)(pForegroundEntry + 0x0c) = pForegroundBitmap;
        *(int *)(pForegroundEntry + 0x10) = 0x800;
        *(int *)(pForegroundEntry + 0x14) = 0;
        pForegroundEntry += 0x260;
        --cRenderGroups;
    } while (cRenderGroups != 0);

    ppSelectorManager =
        (GameMenuSelectorManager **)&pScreen->m_pSelectorManager334;
    pSelectorManager = (GameMenuSelectorManager *)
        AllocateVSMemBlock(sizeof(GameMenuSelectorManager));
    if (pSelectorManager != 0) {
        *ppSelectorManager = ConstructMenuSelectorManager(
            pSelectorManager, 0, pScreen->m_pPrimaryContext84,
            pScreen->m_pRenderContext88, 6,
            pScreen->m_fCompactLayout32C);
    } else {
        *ppSelectorManager = 0;
    }

    AppendMenuSelectorChild(
        *ppSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[0],
        pScreen->m_panWidgetLayout39C[1],
        pPlayResources, 1, 0, 0, 0,
        &pScreen->m_nPlaySelection3A0, (int)0xacef00a6U);
    AppendMenuSelectorChild(
        *ppSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[2],
        pScreen->m_panWidgetLayout39C[3],
        pSinglePlayerResources, 1, 0, 0, 0,
        &pScreen->m_nSharedSelection3B8, (int)0xacef0001U);
    AppendMenuSelectorChild(
        *ppSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[4],
        pScreen->m_panWidgetLayout39C[5],
        pMultiPlayerResources, 1, 0, 0, 0,
        &pScreen->m_nMultiplayerSelection3A4, (int)0xacef00a7U);
    AppendMenuSelectorChild(
        *ppSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[8],
        pScreen->m_panWidgetLayout39C[9],
        pDisplayResources, 1, 0, 0, 0,
        &pScreen->m_nDisplayModeSelection3B0, (int)0xacef00a5U);
    AppendMenuSelectorChild(
        *ppSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[6],
        pScreen->m_panWidgetLayout39C[7],
        pPackResources, 0, 0, 3, 0,
        &pScreen->m_nPackSelection3B4, -1);
    AppendMenuSelectorChild(
        *ppSelectorManager, 0,
        pScreen->m_panWidgetLayout39C[10],
        pScreen->m_panWidgetLayout39C[11],
        pOptionsResources, 1, 0, 0, 0,
        &pScreen->m_nSharedSelection3B8, (int)0xacef00a4U);
    InitializeMenuSelectorCurrentState(*ppSelectorManager, 0, 0);
    InitializeMenuSelectorChildContext(*ppSelectorManager);
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
    int nPackSelection;

    nPackSelection =
        ((GameMainMenuScreenView *)pObject)->m_nPackSelection3B4;
    *(void ***)pObject = g_GAME_MainMenuScreenVtable;
    *(void ***)((char *)pObject + 4) =
        g_GAME_MainMenuQueueVtable;
    *(void ***)((char *)pObject + 0x14) =
        g_GAME_MainMenuCallbackVtable;
    pScreen = (GameMainMenuScreenView *)pObject;
    switch (nPackSelection) {
        case 0:
            pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
            pProgress->m_nCurrentLevel00 = pProgress->m_anSnapshotPackCaps3C[0];
            pProgress->m_nCurrentPack08 = 0;
            break;
        case 1:
            pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
            pProgress->m_nCurrentLevel00 = pProgress->m_anSnapshotPackCaps3C[1];
            pProgress->m_nCurrentPack08 = 1;
            break;
        case 2:
            pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
            pProgress->m_nCurrentLevel00 = pProgress->m_anSnapshotPackCaps3C[2];
            pProgress->m_nCurrentPack08 = 2;
            break;
        case 3:
            pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
            pProgress->m_nCurrentLevel00 = pProgress->m_anSnapshotPackCaps3C[3];
            pProgress->m_nCurrentPack08 = 3;
            break;
    }
    g_nLastMainMenuPackSelection = pScreen->m_nPackSelection3B4;
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        DestroyMainMenuWidgetsThunk(pScreen);
    }
    DestroyFramedScreenBaseThunk00402081(pScreen);
}

// FUNCTION: LEMBALL 0x00448620
static int LEMBALL_FASTCALL HandleMainMenuInput(
    void *pObject, int, void *pEvent)

{
    GameMainMenuScreenView *pScreen;
    GameLevelProgressStateView *pProgress;
    int nType;
    int nCommand;

    pScreen = (GameMainMenuScreenView *)pObject;
    nType = *(unsigned short *)pEvent;
    if (nType < 3) {
        ++*(int *)((char *)pScreen + 0x10);
        return 0;
    }
    if (nType <= 4) {
        pScreen->m_dwTimeout398 = timeGetTime() + 20000U;
        return 0;
    }
    if (nType != 12) {
        ++*(int *)((char *)pScreen + 0x10);
        return 0;
    }
    pScreen->m_dwTimeout398 = timeGetTime() + 20000U;
    pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
    nCommand = *(int *)((char *)pEvent + 8);
    switch ((unsigned int)nCommand) {
            case 0xacef0001U:
                pScreen->m_nRequestedMode374 = 3;
                pScreen->m_nTransitionStatus378 = 1;
                g_fNextMainMenuTimeoutIsTitle = 1;
                return 1;
            case 0xacef00a4U:
                pScreen->m_nRequestedMode374 = 0x10;
                pScreen->m_nTransitionStatus378 = 1;
                g_fNextMainMenuTimeoutIsTitle = 1;
                return 1;
            case 0xacef00a5U:
                TogglePrimaryContextDisplayMode(
                    (ShellUiPrimaryContextShell *)pScreen->m_pPrimaryContext84);
                return 1;
            case 0xacef00a6U:
            case 0xacef00a7U:
                pProgress->m_nCurrentLevel00 =
                    pProgress->m_anSnapshotPackCaps3C[
                        pScreen->m_nPackSelection3B4];
                pProgress->m_nCurrentPack08 = pScreen->m_nPackSelection3B4;
                pScreen->m_nTransitionStatus378 = 1;
                g_fNextMainMenuTimeoutIsTitle = 1;
                if (nCommand == (int)0xacef00a6U) {
                    pScreen->m_nRequestedMode374 = 4;
                    return 1;
                }
                pScreen->m_nRequestedMode374 = 0x0c;
                return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00448800
static void LEMBALL_FASTCALL UpdateMainMenuScreen(void *pObject) {
    GameMainMenuScreenView *pScreen;
    GameLevelProgressStateView *pProgress;
    DWORD dwNow;

    pScreen = (GameMainMenuScreenView *)pObject;
    if (g_fStartupTestAllLevels != 0) {
        pProgress = (GameLevelProgressStateView *)g_pLevelProgressState;
        pProgress->m_nCurrentLevel00 = pProgress->m_anSnapshotPackCaps3C[0];
        pProgress->m_nCurrentPack08 = 0;
        pScreen->m_nTransitionStatus378 = 1;
        pScreen->m_nRequestedMode374 = 4;
    }

    dwNow = timeGetTime();
    if (IsPrimaryContextWindowReadyForPresentThunk(
            (int *)pScreen->m_pPrimaryContext84) == 0 ||
        ((GamePrimaryContextSlot24Interface *)
             pScreen->m_pPrimaryContext84)
                ->IsMainMenuTimeoutAllowed() == 0) {
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

// FUNCTION: LEMBALL 0x00448dc0
static void LEMBALL_FASTCALL ReleaseOptionsScreenWidgets(void *pObject) {
    GameOptionsScreenView *pScreen;

    pScreen = (GameOptionsScreenView *)pObject;
    if (pScreen->m_pSelectorManager334 != 0) {
        ((GameMainMenuSelectorManagerInterface *)
             pScreen->m_pSelectorManager334)->Delete((void *)1);
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
        SetAudioMusicAndEffectVolumeByte((unsigned char)(
            (unsigned int)pSelectorEvent->m_nPayload0C * 0xffU /
            (unsigned int)pSelectorEvent->m_nPayload10));
        return 1;
    }
    if (nCommand == 0xacff0001U) {
        unsigned char nVolume;

        g_nOptionsMusicSliderValue = pSelectorEvent->m_nPayload0C;
        nVolume = (unsigned char)(
            (unsigned int)pSelectorEvent->m_nPayload0C * 0xffU /
            (unsigned int)pSelectorEvent->m_nPayload10);
        AppendCStringToStream(g_pStartupOutputStream,
                              "Setting music volume ");
        AppendCharToFormattedStream(g_pStartupOutputStream,
                                    (char)nVolume);
        AppendCStringToStream(g_pStartupOutputStream, "\n");
        SetVariantResourceEffectMasterVolume(nVolume);
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
    ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
        ->SetVariantResourceEffectsEnabled(g_nOptionsEffectsSliderValue);
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
    ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
        ->SetVariantResourceEffectsEnabled(g_nOptionsEffectsSliderValue);
    ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
        ->SetVariantResourceMusicEnabled(g_nOptionsMusicSliderValue);
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
    unsigned char *pMediaOwner;
    unsigned int uRandom;

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
    pMediaOwner = pScreen->m_abMediaOwner520;
    ConstructResourceGeometryHelperWindowOwner(pMediaOwner);
    InitializePaintballSequenceMediaOwnerVtable(pMediaOwner);
    *(void ***)pMediaOwner = g_GAME_PaintballSequenceMediaOwnerVtable;
    pScreen->m_fSecondVariant518 = fSecondVariant;
    pScreen->m_fMediaEnabled604 = g_fSkipStartupSequence == 0;
    *(void **)(pMediaOwner + 0xbc) = pScreen;
    *(void **)(pMediaOwner + 0xc0) = pPrimaryContext;
    *(int *)(pMediaOwner + 0xc4) = fSecondVariant;
    *(int *)(pMediaOwner + 0xc8) =
        fSecondVariant != 0 ? 0x11e : 0x121;
    *(int *)(pMediaOwner + 0xcc) =
        fSecondVariant != 0 ? 0x126 : 0x129;
    uRandom = (*(unsigned int *)g_pSessionRandomState * 0x29U + 0x1fU) &
              0x7fffffU;
    *(unsigned int *)g_pSessionRandomState = uRandom;
    *(unsigned int *)(pMediaOwner + 0xd0) = uRandom % 3U;
    *(int *)(pMediaOwner + 0xd4) = 0;
    *(int *)(pMediaOwner + 0x94) = 1;
    ((GameDynamicCString *)(pMediaOwner + 0x98))
        ->AssignDynamicCString("lemball");
    *(int *)(pMediaOwner + 0x90) = 1;
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
    if (pScreen->m_fMediaEnabled604 != 0) {
        unsigned char *pMediaOwner = pScreen->m_abMediaOwner520;
        int nBaseResource = pScreen->m_fCompactLayout32C == 0
            ? *(int *)(pMediaOwner + 0xcc)
            : *(int *)(pMediaOwner + 0xc8);
        UpdateEmbeddedMciMediaPath(
            pMediaOwner,
            nBaseResource + *(int *)(pMediaOwner + 0xd0));
    }
}

static void LEMBALL_FASTCALL ReleasePaintballSequenceResources(
    void *pObject) {
    GamePaintballSequenceScreenView *pScreen;

    pScreen = (GamePaintballSequenceScreenView *)pObject;
    if (pScreen->m_pQueuedVariantManager338 != 0) {
        ((GameMainMenuSelectorManagerInterface *)
             pScreen->m_pQueuedVariantManager338)
            ->Delete((void *)1);
        pScreen->m_pQueuedVariantManager338 = 0;
    }
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
    short aRect[4];

    pScreen = (GamePaintballSequenceScreenView *)pObject;
    if (pScreen->m_fMediaStarted5FC == 0 &&
        pScreen->m_dwMediaStart5F8 < timeGetTime() &&
        pScreen->m_fMediaEnabled604 != 0 &&
        IsPrimaryContextWindowReadyForPresentThunk(
            (int *)pScreen->m_pPrimaryContext84) != 0) {
        aRect[0] = (short)pScreen->m_pLayout510[22];
        aRect[1] = (short)pScreen->m_pLayout510[23];
        aRect[2] = (short)pScreen->m_pLayout510[20];
        aRect[3] = (short)pScreen->m_pLayout510[21];
        ((void (LEMBALL_FASTCALL *)(void *, int, short *, void *,
                                    const char *))
             (*(void ***)pScreen->m_abMediaOwner520)[1])(
            pScreen->m_abMediaOwner520, 0, aRect,
            pScreen->m_pPrimaryContext84, "Paintball Sequence");
        StartEmbeddedMciWindowPlaybackOnce(pScreen->m_abMediaOwner520);
        pScreen->m_fMediaStarted5FC = 1;
    }
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

static void LEMBALL_FASTCALL ActivatePaintballSequenceMedia(
    void *pObject, int) {
    unsigned char *pMediaOwner;

    pMediaOwner = (unsigned char *)pObject;
    if (g_fSkipStartupSequence == 0) {
        StartEmbeddedMciWindowPlaybackOnce(pMediaOwner);
    }
    if (*(int *)(pMediaOwner + 0xd4) == 0) {
        if (g_pVariantResourceEntryManager != 0) {
            ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
                ->SetVariantResourceMusicEnabled(1);
        }
        *(int *)(pMediaOwner + 0xd4) = 1;
    }
}

static void DeactivatePaintballSequenceMedia(
    GamePaintballSequenceScreenView *pScreen) {
    if (pScreen->m_fMediaStarted5FC != 0 &&
        *(int *)(pScreen->m_abMediaOwner520 + 4) == 1) {
        ((void (LEMBALL_FASTCALL *)(void *, int))
             (*(void ***)pScreen->m_abMediaOwner520)[29])(
            pScreen->m_abMediaOwner520, 0);
        pScreen->m_fMediaStarted5FC = 0;
        pScreen->m_dwMediaStart5F8 = timeGetTime() + 40U;
    }
}

static void DestroyPaintballSequenceScreen(
    GamePaintballSequenceScreenView *pScreen) {
    pScreen->m_pVtable00 = g_GAME_PaintballSequenceScreenVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_PaintballSequenceQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    DeactivatePaintballSequenceMedia(pScreen);
    if (pScreen->m_fFrameResourcesLoaded9C != 0) {
        ReleasePaintballSequenceResources(pScreen);
    }
    if (*(int *)(pScreen->m_abMediaOwner520 + 0xd4) == 0 &&
        g_pVariantResourceEntryManager != 0) {
        ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
            ->SetVariantResourceMusicEnabled(1);
        *(int *)(pScreen->m_abMediaOwner520 + 0xd4) = 1;
    }
    DestroyResourceGeometryHelperWindowOwner(
        pScreen->m_abMediaOwner520);
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

void *LEMBALL_FASTCALL ConstructFunctionalNetworkLobbyScreen(
    void *pObject, int, void *pPrimaryContext,
    void *pRenderContext, short *paSize) {
    GameNetworkLobbyScreenView *pScreen;
    int *pEntryArray;
    int nPaletteResourceId;
    const int *panButtonResources;
    void *pButtonManager;
    int i;

    pScreen = (GameNetworkLobbyScreenView *)pObject;
    ConstructFramedScreenBaseFields(
        pScreen, 0, pPrimaryContext, pRenderContext, paSize,
        0x0c, 0x32, 200, 0, 100, 0x28);
    InitializeFunctionalNetworkLobbyVtables();
    pScreen->m_pVtable00 = g_GAME_NetworkLobbyScreenVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_NetworkLobbyQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    memset((char *)pScreen + 0x398, 0, 0x438 - 0x398);
    pScreen->m_pTextBuffer398 = AllocateVSMemBlock(0x10);
    if (pScreen->m_pTextBuffer398 != 0) {
        ConstructBoundedTextBuffer(
            (GameBoundedTextBuffer *)pScreen->m_pTextBuffer398, 0x28);
    }
    pScreen->m_nStatus3A8 = 1;
    pScreen->m_nDrawnStatus3AC = 1;
    pScreen->m_dwBlinkStart3C8 = timeGetTime();
    pScreen->m_nSelectedPlayer3D0 = -1;
    pScreen->m_nResolvedPlayer3D4 = -1;
    pEntryArray = (int *)AllocateVSMemBlock(0x2ac);
    if (pEntryArray != 0) {
        pEntryArray[0] = 10;
        pScreen->m_pPlayerEntries3CC = pEntryArray + 1;
        for (i = 0; i < 10; ++i) {
            ConstructNetworkLobbyPlayerEntry(
                (char *)pScreen->m_pPlayerEntries3CC + i * 0x44);
        }
    }
    nPaletteResourceId =
        *(int *)((char *)pScreen->m_pPrimaryContext84 + 0x54);
    for (i = 0; i < 6; ++i) {
        pScreen->m_anPaletteRemaps414[i] =
            (int)(unsigned long)RegisterPaletteRemapVariant(
                nPaletteResourceId,
                g_GAME_NetworkLobbyPaletteRemaps[i], 2);
    }
    if (pScreen->m_pPlayerEntries3CC != 0) {
        for (i = 0; i < 10; ++i) {
            AppendGeometryOwnerChild(
                *(void **)((char *)pScreen->m_pPrimaryContext84 + 0x1c),
                0, (char *)pScreen->m_pPlayerEntries3CC + i * 0x44);
        }
    }
    pScreen->m_pLayout42C = pScreen->m_fCompactLayout32C != 0
        ? g_GAME_NetworkLobbyLayoutCompact
        : g_GAME_NetworkLobbyLayoutStandard;
    if (pScreen->m_pPlayerEntries3CC != 0) {
        ThunkRefreshNetworkLobbyPlayerEntries(
            (int)(unsigned long)pScreen);
    }
    panButtonResources = pScreen->m_fCompactLayout32C != 0
        ? g_GAME_NetworkLobbyButtonResourcesCompact
        : g_GAME_NetworkLobbyButtonResourcesStandard;
    pButtonManager = AllocateVSMemBlock(0x14c);
    if (pButtonManager != 0) {
        ConstructQueuedVariantChildSlotManager(
            pButtonManager, 0, pScreen->m_pPrimaryContext84,
            pScreen->m_pRenderContext88, 4,
            pScreen->m_fCompactLayout32C, 0);
        pScreen->m_pQueuedVariantManager338 = pButtonManager;
        AppendQueuedVariantChildSlot(
            pButtonManager, 0,
            pScreen->m_pLayout42C[0], pScreen->m_pLayout42C[1],
            panButtonResources, 1, 0, 0, 0,
            &pScreen->m_nButtonSelection434, (int)0xacef000cU);
        AppendQueuedVariantChildSlot(
            pButtonManager, 0,
            pScreen->m_pLayout42C[2], pScreen->m_pLayout42C[3],
            panButtonResources + 1, 1, 0, 0, 0,
            &pScreen->m_nButtonSelection434, (int)0xacef000dU);
        AppendQueuedVariantChildSlot(
            pButtonManager, 0,
            pScreen->m_pLayout42C[4], pScreen->m_pLayout42C[5],
            panButtonResources + 2, 1, 0, 0, 0,
            &pScreen->m_nButtonSelection434, (int)0xacef000eU);
        SelectQueuedVariantChildSlot(pButtonManager, 0, 0);
        InitializeQueuedVariantChildContext(pButtonManager);
    }
    pScreen->m_fDrawFrameA8 = 1;
    pScreen->m_fTileBackgroundAC = 1;
    pScreen->m_fDrawForegroundB0 = 0;
    ActivateFramedScreenBase(pScreen);
    return pScreen;
}

static void LEMBALL_FASTCALL DrawNetworkLobbyScreen(void *pObject) {
    GameNetworkLobbyScreenView *pScreen;
    MappedListResourceTable *pMappedList;
    const char *pszStatus;
    short Point[2];
    short Relative[2];
    int nStatus;
    GameBoundedTextBuffer *pBuffer;
    char szDisplayText[0x40];
    int nRemap;
    GameNetworkLobbyRuntimeView *pRuntime;
    GameNetworkLobbyPeerStateView *pPeerState;
    GameNetworkLobbyTrackedPeerView *pPeer;
    GameNetworkLobbyAddressView *pAddress;
    const char *pszAddress;
    int iPeer;
    int iVisible;
    int nColor;
    int nPeerOffset;

#define SUBMIT_CENTERED_LOBBY_TEXT(text, x, y, remap) \
    Point[0] = (short)((x) - (int)strlen(text) * 4); \
    Point[1] = (short)(y); \
    pMappedList->SubmitRenderEntry( \
        pScreen->m_pRenderContext88, Point, Relative, \
        pScreen->m_nMappedListRemapResourceId384, \
        (void *)(text), 0x20, (remap))

    pScreen = (GameNetworkLobbyScreenView *)pObject;
    if (pScreen->m_pMappedListResources98 == 0 ||
        pScreen->m_pLayout42C == 0) {
        return;
    }
    pMappedList = (MappedListResourceTable *)pScreen->m_pMappedListResources98;
    pMappedList->ResetDrawEntryPool();
    Relative[0] = 0;
    Relative[1] = 0;
    if (pScreen->m_fBuildingRenderEntries364 != 0) {
        Point[0] = 0;
        Point[1] = (short)pScreen->m_pLayout42C[20];
        pMappedList->SubmitRenderEntry(
            pScreen->m_pRenderContext88, Point, Relative,
            pScreen->m_nMappedListRemapResourceId384,
            (void *)(pScreen->m_fCompactLayout32C != 0
                ? g_szNetworkLobbyDividerCompact
                : g_szNetworkLobbyDividerStandard), 0x20, 0);
        SUBMIT_CENTERED_LOBBY_TEXT(
            "Name", pScreen->m_pLayout42C[26],
            pScreen->m_pLayout42C[21], 0);
        SUBMIT_CENTERED_LOBBY_TEXT(
            "I.P. Address", pScreen->m_pLayout42C[30],
            pScreen->m_pLayout42C[21], 0);
        SUBMIT_CENTERED_LOBBY_TEXT(
            "Computer", pScreen->m_pLayout42C[32],
            pScreen->m_pLayout42C[21], 0);
        if (g_szNetworkLobbyLocalPlayerName[0] != '\0') {
            SUBMIT_CENTERED_LOBBY_TEXT(
                g_szNetworkLobbyLocalPlayerName,
                pScreen->m_pLayout42C[26],
                pScreen->m_pLayout42C[25],
                pScreen->m_anPaletteRemaps414[0]);
            if (pScreen->m_pszLocalAddress3E4 != 0 &&
                pScreen->m_pszLocalAddress3E4[0] != '\0') {
                SUBMIT_CENTERED_LOBBY_TEXT(
                    pScreen->m_pszLocalAddress3E4,
                    pScreen->m_pLayout42C[30],
                    pScreen->m_pLayout42C[25],
                    pScreen->m_anPaletteRemaps414[0]);
            }
            if (pScreen->m_pszLocalComputer3E8 != 0 &&
                pScreen->m_pszLocalComputer3E8[0] != '\0') {
                strncpy(szDisplayText, pScreen->m_pszLocalComputer3E8, 20);
                szDisplayText[20] = '\0';
                SUBMIT_CENTERED_LOBBY_TEXT(
                    szDisplayText, pScreen->m_pLayout42C[32],
                    pScreen->m_pLayout42C[25],
                    pScreen->m_anPaletteRemaps414[0]);
            }
        }
        return;
    }
    nStatus = pScreen->m_nStatus3A8;
    if ((unsigned int)nStatus >= 10U) {
        nStatus = 9;
    }
    pszStatus = g_GAME_NetworkLobbyStatusText[nStatus];
    nRemap = 0;
    if (nStatus == 4) {
        if (g_szNetworkLobbyJoinAddress[0] == '\0') {
            pszStatus = g_GAME_NetworkLobbyStatusText[5];
        } else {
            strncpy(szDisplayText, pszStatus, sizeof(szDisplayText) - 1);
            szDisplayText[sizeof(szDisplayText) - 1] = '\0';
            strncat(szDisplayText, g_szNetworkLobbyJoinAddress,
                    sizeof(szDisplayText) - strlen(szDisplayText) - 1);
            pszStatus = szDisplayText;
        }
        nRemap = pScreen->m_anPaletteRemaps414[5];
    } else if (nStatus >= 7) {
        nRemap = pScreen->m_anPaletteRemaps414[3];
    }
    if ((nStatus != 4 && nStatus < 7) || pScreen->m_fBlink3C0 != 0) {
        SUBMIT_CENTERED_LOBBY_TEXT(
            pszStatus, pScreen->m_pLayout42C[18],
            pScreen->m_pLayout42C[19], nRemap);
    }
    pRuntime = (GameNetworkLobbyRuntimeView *)g_pNetworkLobbyVsnetRuntime;
    if (pRuntime != 0 && pRuntime->m_pPeerStates14 != 0 &&
        pScreen->m_pPlayerEntries3CC != 0) {
        iVisible = 0;
        for (iPeer = 0; iPeer < 10 && iVisible < 4; ++iPeer) {
            pPeerState = pRuntime->m_pPeerStates14 + iPeer;
            pPeer = pRuntime->m_apTrackedPeers20[iPeer];
            if (pPeer == 0 || pPeerState->m_fActive4C == 0) {
                continue;
            }
            nPeerOffset = iPeer * 0x44;
            nColor = 1;
            if (*(int *)((char *)pScreen->m_pPlayerEntries3CC +
                         nPeerOffset + 0x38) != 0 ||
                pScreen->m_nResolvedPlayer3D4 == iPeer) {
                ++nColor;
            }
            if ((pScreen->m_nSelectedPlayer3D0 == iPeer &&
                 (*(int *)((char *)pScreen->m_pPlayerEntries3CC +
                           nPeerOffset + 0x40) == 0 ||
                  pScreen->m_fBlink3C0 != 0)) ||
                (pScreen->m_nSelectedPlayer3D0 != iPeer &&
                 *(int *)((char *)pScreen->m_pPlayerEntries3CC +
                          nPeerOffset + 0x40) != 0 &&
                 pScreen->m_fBlink3C0 == 0)) {
                nColor += 3;
            }
            if (nColor != 1) {
                strncpy(szDisplayText, pPeerState->m_szName2C, 9);
                szDisplayText[9] = '\0';
                SUBMIT_CENTERED_LOBBY_TEXT(
                    szDisplayText, pScreen->m_pLayout42C[26],
                    pScreen->m_pLayout42C[27] +
                        iVisible * pScreen->m_pLayout42C[35],
                    pScreen->m_anPaletteRemaps414[nColor]);
                pszAddress = "";
                if (pPeer->m_pDescriptor04 != 0) {
                    pAddress = *(GameNetworkLobbyAddressView **)(
                        (char *)pPeer +
                        pPeer->m_pDescriptor04->m_nAddressOwnerOffset08 +
                        0x74);
                    if (pAddress != 0) {
                        pszAddress = pAddress->GetAddressString();
                    }
                }
                if (pszAddress != 0) {
                    SUBMIT_CENTERED_LOBBY_TEXT(
                        pszAddress, pScreen->m_pLayout42C[30],
                        pScreen->m_pLayout42C[27] +
                            iVisible * pScreen->m_pLayout42C[35],
                        pScreen->m_anPaletteRemaps414[nColor]);
                }
                strncpy(szDisplayText, pPeerState->m_szComputer35, 20);
                szDisplayText[20] = '\0';
                SUBMIT_CENTERED_LOBBY_TEXT(
                    szDisplayText, pScreen->m_pLayout42C[32],
                    pScreen->m_pLayout42C[27] +
                        iVisible * pScreen->m_pLayout42C[35],
                    pScreen->m_anPaletteRemaps414[nColor]);
            }
            ++iVisible;
        }
    }
    pBuffer = (GameBoundedTextBuffer *)pScreen->m_pTextBuffer398;
    if (pScreen->m_fTextPrompt39C != 0 && pBuffer != 0 &&
        pBuffer->m_pszText0C != 0) {
        strncpy(szDisplayText, pBuffer->m_pszText0C,
                sizeof(szDisplayText) - 2);
        szDisplayText[sizeof(szDisplayText) - 2] = '\0';
        if (pScreen->m_fBlink3C0 == 0) {
            strcat(szDisplayText, "_");
        }
        Point[0] = (short)pScreen->m_pLayout42C[22];
        Point[1] = (short)pScreen->m_pLayout42C[23];
        pMappedList->SubmitRenderEntry(
            pScreen->m_pRenderContext88, Point, Relative,
            pScreen->m_nMappedListRemapResourceId384,
            szDisplayText, 0x20, pScreen->m_anPaletteRemaps414[1]);
    }
    pScreen->m_nDrawnStatus3AC = pScreen->m_nStatus3A8;
#undef SUBMIT_CENTERED_LOBBY_TEXT
}

static void LEMBALL_FASTCALL UpdateNetworkLobbyScreen(void *pObject) {
    GameNetworkLobbyScreenView *pScreen;
    DWORD dwNow;

    pScreen = (GameNetworkLobbyScreenView *)pObject;
    if (pScreen->m_nDrawnStatus3AC != pScreen->m_nStatus3A8) {
        return;
    }
    if (pScreen->m_fPendingText3E0 != 0) {
        pScreen->m_nReserved3B8 = 1;
        if (StartNetworkLobbyTransportSession(
                g_pActiveNetworkLobbyTransportController) != 0 &&
            g_pNetworkLobbyVsnetRuntime != 0) {
            ConfigureNetworkLobbyTransportHostName(
                g_pNetworkLobbyVsnetRuntime,
                g_szNetworkLobbyJoinAddress);
            SetNetworkLobbyStatus(pScreen, 4);
        } else {
            SetTimedNetworkLobbyStatus(pScreen, 9, 6000);
        }
        pScreen->m_fPendingText3E0 = 0;
    }
    dwNow = timeGetTime();
    if (dwNow - pScreen->m_dwBlinkStart3C8 > 499U) {
        pScreen->m_fBlink3C0 = pScreen->m_fBlink3C0 == 0;
        pScreen->m_dwBlinkStart3C8 = dwNow;
        pScreen->m_fSelectorDirty360 = 1;
    }
    if (pScreen->m_dwStatusDuration3B4 != 0 &&
        dwNow - pScreen->m_dwStatusStart3B0 >
            pScreen->m_dwStatusDuration3B4) {
        pScreen->m_nStatus3A8 = 1;
        pScreen->m_dwStatusDuration3B4 = 0;
        pScreen->m_fSelectorDirty360 = 1;
    }
    if (pScreen->m_nPendingPrompt3A4 != 0) {
        ShowNetworkLobbyTextPrompt(
            pScreen, pScreen->m_nPendingPrompt3A4, 1);
        pScreen->m_nPendingPrompt3A4 = 0;
    }
    if (pScreen->m_fRefreshPlayers3C4 != 0 &&
        pScreen->m_pPlayerEntries3CC != 0) {
        ThunkRefreshNetworkLobbyPlayerEntries(
            (int)(unsigned long)pScreen);
        pScreen->m_fRefreshPlayers3C4 = 0;
        pScreen->m_fSelectorDirty360 = 1;
    }
}

static int LEMBALL_FASTCALL HandleNetworkLobbyScreenInput(
    void *pObject, int, void *pEvent) {
    GameNetworkLobbyScreenView *pScreen;
    GameMenuSelectorEvent *pInput;
    GameBoundedTextBuffer *pBuffer;
    unsigned int nCode;
    char ch;

    pScreen = (GameNetworkLobbyScreenView *)pObject;
    pInput = (GameMenuSelectorEvent *)pEvent;
    nCode = (unsigned int)pInput->m_nPayload08;
    if (pInput->m_nType00 == 0x0c) {
        if (nCode == 0xacef000cU && pScreen->m_fTextPrompt39C == 0) {
            BeginNetworkLobbyTextPrompt(pScreen, 0);
            return 1;
        }
        if (nCode == 0xacef000dU && pScreen->m_fTextPrompt39C == 0) {
            BeginNetworkLobbyTextPrompt(pScreen, 1);
            return 1;
        }
        if (nCode == 0xacef000eU) {
            pScreen->m_nTransitionStatus378 = 1;
            pScreen->m_nRequestedMode374 = 2;
            return 1;
        }
    }
    if ((pInput->m_nType00 == 3 || pInput->m_nType00 == 4) &&
        nCode == 0x1bU) {
        pScreen->m_nTransitionStatus378 = 1;
        pScreen->m_nRequestedMode374 = 2;
        return 1;
    }
    if (pScreen->m_fTextPrompt39C != 0 &&
        (pInput->m_nType00 == 3 || pInput->m_nType00 == 4)) {
        if (pInput->m_nType00 == 3) {
            return 1;
        }
        pBuffer = (GameBoundedTextBuffer *)pScreen->m_pTextBuffer398;
        if (pBuffer == 0 || pBuffer->m_pszText0C == 0) {
            return 1;
        }
        ch = 0;
        if (nCode >= 5U && nCode <= 0x1eU) {
            ch = (char)('a' + nCode - 5U);
        } else if (nCode >= 0x39U && nCode <= 0x42U) {
            ch = (char)('0' + nCode - 0x39U);
        } else if (nCode == 0x1fU) {
            ch = ' ';
        } else if (nCode == 0x20U) {
            ch = '.';
        } else if (nCode == 0x4cU) {
            CommitNetworkLobbyTextPrompt(pScreen);
            return 1;
        } else if (nCode == 0x4dU || nCode == 0x4eU) {
            if (pBuffer->m_cLength04 != 0) {
                --pBuffer->m_cLength04;
                pBuffer->m_pszText0C[pBuffer->m_cLength04] = '\0';
                pScreen->m_fSelectorDirty360 = 1;
            }
            return 1;
        }
        if (ch != 0) {
            AppendBoundedTextCharacter(pBuffer, ch);
            pScreen->m_fSelectorDirty360 = 1;
        }
        return 1;
    }
    ++*(int *)((char *)pScreen + 0x10);
    return 0;
}

static void DestroyNetworkLobbyScreen(
    GameNetworkLobbyScreenView *pScreen) {
    int i;

    pScreen->m_pVtable00 = g_GAME_NetworkLobbyScreenVtable;
    *(void ***)pScreen->m_abRenderQueueNode04 =
        g_GAME_NetworkLobbyQueueVtable;
    *(void ***)pScreen->m_abVariantRenderManager14 =
        g_GAME_MainMenuCallbackVtable;
    g_pActiveNetworkLobbyScreen = 0;
    if (pScreen->m_pQueuedVariantManager338 != 0) {
        DestroyQueuedVariantChildSlotManager(
            pScreen->m_pQueuedVariantManager338);
        FreeVSMemBlock(pScreen->m_pQueuedVariantManager338);
        pScreen->m_pQueuedVariantManager338 = 0;
    }
    if (pScreen->m_pTextBuffer398 != 0) {
        GameBoundedTextBuffer *pBuffer;

        pBuffer = (GameBoundedTextBuffer *)pScreen->m_pTextBuffer398;
        if (pBuffer->m_pszText0C != 0) {
            FreeVSMemBlock(pBuffer->m_pszText0C);
        }
        FreeVSMemBlock(pBuffer);
        pScreen->m_pTextBuffer398 = 0;
    }
    if (pScreen->m_pPlayerEntries3CC != 0) {
        for (i = 0; i < 10; ++i) {
            RemoveGeometryOwnerChild(
                *(void **)((char *)pScreen->m_pPrimaryContext84 + 0x1c),
                0, (char *)pScreen->m_pPlayerEntries3CC + i * 0x44);
            *(void ***)((char *)pScreen->m_pPlayerEntries3CC + i * 0x44) =
                g_pNetworkLobbyPlayerEntryBaseVtable;
        }
        FreeVSMemBlock((char *)pScreen->m_pPlayerEntries3CC - 4);
        pScreen->m_pPlayerEntries3CC = 0;
    }
    for (i = 0; i < 6; ++i) {
        ReleasePaletteRemapVariant(
            (void *)(unsigned long)pScreen->m_anPaletteRemaps414[i]);
        pScreen->m_anPaletteRemaps414[i] = 0;
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

static void *LEMBALL_FASTCALL DeleteNetworkLobbyScreen(
    void *pObject, int, int fDelete) {
    DestroyNetworkLobbyScreen((GameNetworkLobbyScreenView *)pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

static void *LEMBALL_FASTCALL DeleteAdjustedNetworkLobbyScreen(
    void *pObject, int nUnusedEdx, int fDelete) {
    return DeleteNetworkLobbyScreen(
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
    pScreen->m_cPassword420 = 10;
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
    int iRow;
    int iColumn;
    int iAction;
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
        *(short *)(pForegroundEntry + 0) =
            (short)pScreen->m_pLayout3A8[0];
        *(short *)(pForegroundEntry + 2) =
            (short)pScreen->m_pLayout3A8[1];
        *(void **)(pForegroundEntry + 0x0c) = pScreen->m_pForegroundBitmapC0;
        *(int *)(pForegroundEntry + 0x10) = 0x800;
        *(int *)(pForegroundEntry + 0x14) = 0;
    }
    i = 0;
    Point.m_y = (short)pScreen->m_pLayout3A8[3];
    for (iRow = 0; iRow < 4; ++iRow) {
        Point.m_x = (short)pScreen->m_pLayout3A8[2];
        for (iColumn = 0; iColumn < 3; ++iColumn) {
            iAction = g_GAME_PasswordKeyPermutation[i];
            pOverlay = AllocateVSMemBlock(0x130);
            if (pOverlay != 0) {
                ConstructAlignedZrleChildOverlayFromPoint(
                    pOverlay, 0, &Point, pScreen->m_pPrimaryContext84,
                    pScreen->m_pKeyResourceIds3AC[iAction], 3);
                *(int *)((char *)pOverlay + 0xf8) = 0;
                nContext = *(int *)(*(int *)((char *)pOverlay + 0x4c) + 0x0c);
                nRenderObject = nContext +
                    *(int *)(*(int *)(nContext + 0x40) + 4) + 0x70;
                *(int *)nRenderObject = 0;
                *(void **)((char *)pOverlay + 0xf4) =
                    g_pSharedRenderDispatchQueue;
                *(int *)((char *)pOverlay + 0xcc) =
                    (int)(0xabcd00b0U + iAction);
            }
            pScreen->m_apKeyOverlays424[iAction] = pOverlay;
            pScreen->m_aKeyPositions3C0[i][0] =
                Point.m_x - (short)pScreen->m_pLayout3A8[2];
            pScreen->m_aKeyPositions3C0[i][1] =
                Point.m_y - (short)pScreen->m_pLayout3A8[3];
            Point.m_x = (short)(Point.m_x +
                pScreen->m_pLayout3A8[24] + pScreen->m_pLayout3A8[4]);
            ++i;
        }
        Point.m_y = (short)(Point.m_y +
            pScreen->m_pLayout3A8[25] + pScreen->m_pLayout3A8[5]);
    }
    pScreen->m_xSelected478 =
        pScreen->m_aKeyPositions3C0[pScreen->m_nSelectedKey480][0];
    pScreen->m_ySelected47C =
        pScreen->m_aKeyPositions3C0[pScreen->m_nSelectedKey480][1];
    InitializePasswordEntryRenderContext(pScreen);
}

static void InitializePasswordEntryRenderContext(
    GamePasswordEntryScreenView *pScreen) {
    typedef void (LEMBALL_FASTCALL *InitializeContextVirtual)(
        void *, int, short *, void *, int);
    short Rect[4];
    short nKeyStep;

    nKeyStep = (short)(pScreen->m_pLayout3A8[24] +
                       pScreen->m_pLayout3A8[4]);
    pScreen->m_pInputPanelOwner3A4 = AllocateVSMemBlock(0x90);
    if (pScreen->m_pInputPanelOwner3A4 == 0) {
        pScreen->m_pInputPanelRenderContext474 = 0;
        return;
    }
    ConstructWindowOwnerRenderContext(pScreen->m_pInputPanelOwner3A4);
    *(int *)((char *)pScreen->m_pInputPanelOwner3A4 + 0x50) += 10;
    Rect[0] = (short)(nKeyStep * 3);
    Rect[1] = (short)(nKeyStep * 4);
    Rect[2] = (short)(pScreen->m_pLayout3A8[2] - 1);
    Rect[3] = (short)(pScreen->m_pLayout3A8[3] - 1);
    ((InitializeContextVirtual)
        (*(void ***)pScreen->m_pInputPanelOwner3A4)[1])(
            pScreen->m_pInputPanelOwner3A4, 0, Rect,
            pScreen->m_pPrimaryContext84, 0);
    pScreen->m_pInputPanelRenderContext474 =
        *(void **)((char *)pScreen->m_pInputPanelOwner3A4 + 0x4c);
}

static void LEMBALL_FASTCALL ReleasePasswordEntryResources(void *pObject) {
    GamePasswordEntryScreenView *pScreen;
    typedef void *(LEMBALL_FASTCALL *DeleteOverlayVirtual)(
        void *, int, int);
    int i;
    pScreen = (GamePasswordEntryScreenView *)pObject;
    for (i = 0; i < 12; ++i) {
        if (pScreen->m_apKeyOverlays424[i] != 0) {
            void *pDeleteInterface =
                (char *)pScreen->m_apKeyOverlays424[i] + 0x90;
            ((DeleteOverlayVirtual)(*(void ***)pDeleteInterface)[0])(
                pDeleteInterface, 0, 1);
            pScreen->m_apKeyOverlays424[i] = 0;
        }
    }
    if (pScreen->m_nPanelResourceId398 != 0) {
        ((LevelVariantRenderManagerView *)pScreen->m_abVariantRenderManager14)
            ->ReleaseVariantRenderResource(pScreen->m_nPanelResourceId398);
        pScreen->m_nPanelResourceId398 = 0;
    }
    if (pScreen->m_pInputPanelOwner3A4 != 0) {
        typedef void (LEMBALL_FASTCALL *ReleaseContextVirtual)(void *, int);
        if (*(int *)((char *)pScreen->m_pInputPanelOwner3A4 + 4) == 1) {
            ((ReleaseContextVirtual)
                (*(void ***)pScreen->m_pInputPanelOwner3A4)[29])(
                    pScreen->m_pInputPanelOwner3A4, 0);
        }
        DestroyWindowOwnerRenderContext(pScreen->m_pInputPanelOwner3A4);
        FreeVSMemBlock(pScreen->m_pInputPanelOwner3A4);
        pScreen->m_pInputPanelOwner3A4 = 0;
        pScreen->m_pInputPanelRenderContext474 = 0;
    }
}

static void RotatePasswordEntrySelection(
    GamePasswordEntryScreenView *pScreen, int nDirection) {
    int nPosition;

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
    pScreen->m_xSelected478 = pScreen->m_aKeyPositions3C0[nPosition][0];
    pScreen->m_ySelected47C = pScreen->m_aKeyPositions3C0[nPosition][1];
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
        } else if (g_pVariantResourceEntryManager != 0) {
            ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
                ->PlayVariantResourceEffect(0x19);
        }
    } else if (nAction == 10) {
        if (pScreen->m_cPassword420 != 0) {
            --pScreen->m_cPassword420;
            pScreen->m_szPassword3B0[pScreen->m_cPassword420] = '-';
        } else if (g_pVariantResourceEntryManager != 0) {
            ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
                ->PlayVariantResourceEffect(0x19);
        }
    } else if (nAction == 11) {
        pScreen->m_fPasswordValid484 =
            ValidateCurrentLevelPassword(pScreen->m_szPassword3B0);
        ((GameNoArgVirtual)pScreen->m_pVtable00[19])(pScreen, 0);
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

static void DispatchPasswordOverlayInput(void *pOverlay, int iSlot) {
    typedef void (LEMBALL_FASTCALL *DispatchOverlayInputProc)(
        void *, int, short *, int);
    short Point[2];
    void *pInputInterface;

    if (pOverlay == 0) {
        return;
    }
    Point[0] = 0;
    Point[1] = 0;
    pInputInterface = (char *)pOverlay + 0x90;
    ((DispatchOverlayInputProc)(*(void ***)pInputInterface)[iSlot])(
        pInputInterface, 0, Point, 0);
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
    if (pSelectorEvent->m_nType00 == 3 ||
        pSelectorEvent->m_nType00 == 4) {
        void *pOverlay;
        int iSlot;

        pOverlay = 0;
        iSlot = pSelectorEvent->m_nType00 == 3 ? 2 : 1;
        if (nCode == 0x1fU || nCode == 0x22U) {
            pOverlay = pScreen->m_apKeyOverlays424[
                g_GAME_PasswordKeyPermutation[pScreen->m_nSelectedKey480]];
        } else if (nCode == 0x4cU) {
            pOverlay = pScreen->m_apKeyOverlays424[11];
        } else if (nCode == 0x4dU || nCode == 0x4eU) {
            pOverlay = pScreen->m_apKeyOverlays424[10];
        } else if (nCode > 0x38U && nCode < 0x43U) {
            pOverlay = pScreen->m_apKeyOverlays424[nCode - 0x39U];
        } else if (pSelectorEvent->m_nType00 == 4 &&
                   nCode >= 1U && nCode <= 4U) {
            RotatePasswordEntrySelection(pScreen, (int)nCode);
            return 1;
        }
        if (pOverlay != 0) {
            DispatchPasswordOverlayInput(pOverlay, iSlot);
            return 1;
        }
        return 0;
    }
    if (pSelectorEvent->m_nType00 == 0x0b) {
        if (g_pVariantResourceEntryManager != 0) {
            ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
                ->PlayVariantResourceEffect(0x25);
        }
        return 0;
    }
    if (pSelectorEvent->m_nType00 == 0x0c &&
        nCode >= 0xabcd00b0U && nCode <= 0xabcd00bbU) {
        nAction = (int)(nCode - 0xabcd00b0U);
    }
    if (nAction >= 0) {
        ApplyPasswordEntryAction(pScreen, nAction);
        return 1;
    }
    if (pSelectorEvent->m_nType00 == 0x0c) {
        return 0;
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

static void LEMBALL_FASTCALL DrawPasswordEntryInputArea(void *pObject) {
    GamePasswordEntryScreenView *pScreen;
    MappedListResourceTable *pMappedList;
    short Point[2];
    short Relative[2];
    short Rect[4];
    void *pSavedContext;

    pScreen = (GamePasswordEntryScreenView *)pObject;
    if (pScreen->m_pMappedListResources98 == 0 ||
        pScreen->m_pLayout3A8 == 0) {
        return;
    }
    pMappedList = (MappedListResourceTable *)pScreen->m_pMappedListResources98;
    if (pScreen->m_pInputPanelRenderContext474 != 0) {
        Point[0] = 0;
        Point[1] = 0;
        pSavedContext = *(void **)(pScreen->m_abVariantRenderManager14 + 0x1c);
        *(void **)(pScreen->m_abVariantRenderManager14 + 0x1c) =
            pScreen->m_pInputPanelRenderContext474;
        ((LevelVariantRenderManagerView *)
             pScreen->m_abVariantRenderManager14)
            ->EmitVariantRenderResourceEntry(
                Rect, Point, pScreen->m_nPanelResourceId398, 0, 0, 0);
        *(void **)(pScreen->m_abVariantRenderManager14 + 0x1c) =
            pSavedContext;
    }
    Relative[0] = 0;
    Relative[1] = 0;
    Point[0] = (short)(pScreen->m_cxScreen37C -
        pScreen->m_pLayout3A8[22] - pScreen->m_cPassword420 * 8);
    Point[1] = (short)pScreen->m_pLayout3A8[23];
    pMappedList->SubmitRenderEntry(
        pScreen->m_pRenderContext88, Point, Relative,
        pScreen->m_nMappedListRemapResourceId384,
        pScreen->m_szPassword3B0, 0x20, 0);
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
    InitializeFramedScreenRenderChildEntry(pScreen->m_abRenderChild398);
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
    InitializeFunctionalLevelSelectionPaletteRemap(pScreen);
    return pScreen;
}

static void InitializeFunctionalLevelSelectionPaletteRemap(
    GameLevelSelectionScreenView *pScreen) {
    void *pPalette;
    int i;

    pPalette = LoadPalResource(0x10a);
    pScreen->m_pPaletteRemap3CC =
        (unsigned char *)AllocateVSMemBlock(0x100);
    for (i = 0; i < 0x100; ++i) {
        pScreen->m_pPaletteRemap3CC[i] = (unsigned char)i;
    }
    for (i = 0; i < 10; ++i) {
        if (g_GAME_LevelSelectionPaletteTargetIndices[i] != 0) {
            pScreen->m_pPaletteRemap3CC[
                g_GAME_LevelSelectionPaletteSourceIndices[i]] =
                (unsigned char)g_GAME_LevelSelectionPaletteTargetIndices[i];
        }
    }
    pScreen->m_nPaletteRemap3D0 = (int)(unsigned long)
        RegisterPaletteRemapVariant(
            0x10a, pScreen->m_pPaletteRemap3CC, 0);
    ReleaseTypedResourceObjectReference(pPalette);
}

static void ReleaseFunctionalLevelSelectionPaletteRemap(
    GameLevelSelectionScreenView *pScreen) {
    ReleasePaletteRemapVariant(
        (void *)(unsigned long)pScreen->m_nPaletteRemap3D0);
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
        g_MenuSelectorCurrentFrameCallbackVtable + 1;
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
    ReleaseFunctionalLevelSelectionPaletteRemap(pScreen);
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
    RestoreLevelSelectionRenderChildEntryVtable(pScreen->m_abRenderChild398);
    DestroyVariantRenderManager(pScreen->m_abVariantRenderManager14);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pScreen);
    }
    return pObject;
}

static int LEMBALL_FASTCALL HandleFunctionalLevelSelectionInput(
    void *pObject, int, void *pEvent) {
    GameLevelSelectionScreenView *pScreen;
    int nCode;

    pScreen = (GameLevelSelectionScreenView *)pObject;
    if (*(unsigned short *)pEvent != 0x0c) {
        ++*(int *)((char *)pScreen + 0x10);
        return 0;
    }
    nCode = *(int *)((char *)pEvent + 8);
    switch ((unsigned int)nCode) {
    case 0xacef000cU:
        if (pScreen->m_fNetworkScreen8C == 0) {
            pScreen->m_nRequestedMode374 = 2;
            pScreen->m_nTransitionStatus378 = 1;
        } else {
            ((GameBaseModeScreen *)pScreen)
                ->QueueActiveBaseModeUserAction(2, 0);
        }
        return 1;
    case 0xacef000dU:
        if (pScreen->m_fNetworkScreen8C == 0) {
            pScreen->m_nRequestedMode374 = 5;
            pScreen->m_nTransitionStatus378 = 1;
        } else {
            ((GameBaseModeScreen *)pScreen)
                ->QueueActiveBaseModeUserAction(3, 0);
        }
        return 1;
    case 0xacef000eU:
        if (pScreen->m_fNextLocked480 == 1) {
            pScreen->m_fFrameDirty330 = 1;
        } else if (pScreen->m_fNetworkScreen8C == 0) {
            SelectFunctionalLevel(pScreen, 1);
        } else {
            ((GameBaseModeScreen *)pScreen)
                ->QueueActiveBaseModeUserAction(0, 0);
        }
        return 1;
    case 0xacef000fU:
        if (pScreen->m_fPreviousLocked484 == 1) {
            pScreen->m_fFrameDirty330 = 1;
        } else if (pScreen->m_fNetworkScreen8C == 0) {
            SelectFunctionalLevel(pScreen, -1);
        } else {
            ((GameBaseModeScreen *)pScreen)
                ->QueueActiveBaseModeUserAction(1, 0);
        }
        return 1;
    default:
        return 0;
    }
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
    GameLevelSelectionScreenView *pScreen;
    const int *pLayout;
    const int *pBackResources;
    const int *pPlayResources;
    unsigned char *pForegroundEntry;
    unsigned char *pLevelEntry;
    void *pBitmap;
    void *pButtonManager;
    int nResource0;
    int nResource1;
    int nResource2;

    pScreen = (GameLevelSelectionScreenView *)pObject;
    if (pScreen->m_fCompactLayout32C == 1) {
        pLayout = g_GAME_LevelSelectionLayoutCompact;
        pBackResources = g_GAME_LevelSelectionBackResourcesCompact;
        pPlayResources = g_GAME_LevelSelectionPlayResourcesCompact;
        pBitmap = LoadBitmapResource(0x13e);
        nResource0 = 0x180;
        nResource1 = 0x181;
        nResource2 = 0x182;
        pScreen->m_nTimedVariantResourceId394 = 0x17c;
        pScreen->m_pPreviousResourceIds3E8 =
            g_GAME_LevelSelectionPreviousResourcesCompact;
        pScreen->m_pNextResourceIds3E4 =
            g_GAME_LevelSelectionNextResourcesCompact;
    } else {
        pLayout = g_GAME_LevelSelectionLayoutStandard;
        pBackResources = g_GAME_LevelSelectionBackResourcesStandard;
        pPlayResources = g_GAME_LevelSelectionPlayResourcesStandard;
        pBitmap = LoadBitmapResource(0x133);
        nResource0 = 0x153;
        nResource1 = 0x154;
        nResource2 = 0x155;
        pScreen->m_nTimedVariantResourceId394 = 0x14f;
        pScreen->m_pPreviousResourceIds3E8 =
            g_GAME_LevelSelectionPreviousResourcesStandard;
        pScreen->m_pNextResourceIds3E4 =
            g_GAME_LevelSelectionNextResourcesStandard;
    }
    *(void **)((char *)pScreen + 0x3bc) = pBitmap;
    *(const int **)((char *)pScreen + 0x3d4) = pLayout;
    *(int *)((char *)pScreen + 0x3d8) = nResource0;
    *(int *)((char *)pScreen + 0x3dc) = nResource1;
    *(int *)((char *)pScreen + 0x3e0) = nResource2;
    pScreen->m_xOverlay380 = (short)pLayout[16] + (short)pLayout[14];
    pScreen->m_yOverlay382 = (short)pLayout[15] + (short)pLayout[17];
    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              nResource0);
    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              nResource1);
    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              pScreen->m_nTimedVariantResourceId394);
    LoadVariantRenderResource(pScreen->m_abVariantRenderManager14,
                              nResource2);

    pForegroundEntry = pScreen->m_abRenderEntryGroupC8 + 0x14;
    *(short *)(pForegroundEntry + 0) = (short)pLayout[8];
    *(short *)(pForegroundEntry + 2) = (short)pLayout[9];
    *(void **)(pForegroundEntry + 0x0c) = pScreen->m_pForegroundBitmapC0;
    *(int *)(pForegroundEntry + 0x10) = 0x800;
    *(int *)(pForegroundEntry + 0x14) = 0;
    pLevelEntry = pScreen->m_abRenderChild398 + 4;
    *(short *)(pLevelEntry + 0) = (short)pLayout[14];
    *(short *)(pLevelEntry + 2) = (short)pLayout[15];
    *(void **)(pLevelEntry + 0x0c) = pBitmap;
    *(int *)(pLevelEntry + 0x10) = 0x800;
    *(int *)(pLevelEntry + 0x14) = 0;

    pScreen->m_nReserved47C = 0;
    pScreen->m_fNextLocked480 = 0;
    pScreen->m_fPreviousLocked484 = 0;
    pButtonManager = AllocateVSMemBlock(0x14c);
    if (pButtonManager != 0) {
        pScreen->m_pQueuedVariantManager338 =
            ConstructQueuedVariantChildSlotManager(
                pButtonManager, 0, pScreen->m_pPrimaryContext84,
                pScreen->m_pRenderContext88, 4,
                pScreen->m_fCompactLayout32C, 0);
    } else {
        pScreen->m_pQueuedVariantManager338 = 0;
    }
    AppendQueuedVariantChildSlot(
        pScreen->m_pQueuedVariantManager338, 0, pLayout[0], pLayout[1],
        pPlayResources, 1, 0, 0, 0, &pScreen->m_nReserved47C,
        (int)0xacef000dU);
    AppendQueuedVariantChildSlot(
        pScreen->m_pQueuedVariantManager338, 0, pLayout[2], pLayout[3],
        pBackResources, 1, 0, 0, 0, &pScreen->m_nReserved47C,
        (int)0xacef000cU);
    AppendQueuedVariantChildSlot(
        pScreen->m_pQueuedVariantManager338, 0, pLayout[4], pLayout[5],
        pScreen->m_pPreviousResourceIds3E8, 1, 0, 1, 0,
        &pScreen->m_fPreviousLocked484, (int)0xacef000fU);
    AppendQueuedVariantChildSlot(
        pScreen->m_pQueuedVariantManager338, 0, pLayout[6], pLayout[7],
        pScreen->m_pNextResourceIds3E4, 1, 0, 1, 0,
        &pScreen->m_fNextLocked480, (int)0xacef000eU);
    SelectQueuedVariantChildSlot(pScreen->m_pQueuedVariantManager338, 0, 0);
    InitializeQueuedVariantChildContext(pScreen->m_pQueuedVariantManager338);
    RefreshFunctionalLevelSelectionMetadata(pScreen);
}

static void LEMBALL_FASTCALL ReleaseFunctionalLevelSelectionResources(
    void *pObject) {
    GameFramedScreenBase *pScreen;
    LevelVariantRenderManagerView *pVariantManager;

    pScreen = (GameFramedScreenBase *)pObject;
    pVariantManager = (LevelVariantRenderManagerView *)
        pScreen->m_abVariantRenderManager14;
    if (pScreen->m_pQueuedVariantManager338 != 0) {
        DestroyQueuedVariantChildSlotManager(
            pScreen->m_pQueuedVariantManager338);
    }
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
    short Extent[2];
    short Point[2];
    short Rect[4];
    int cxFrame;
    int x;
    int i;

    pScreen = (GameLevelSelectionScreenView *)pObject;
    pLayout = pScreen->m_pLayout3D4;
    if (pLayout == 0) {
        return;
    }
    Point[0] = (short)pLayout[30];
    Point[1] = (short)pLayout[31];
    ((LevelVariantRenderManagerView *)pScreen->m_abVariantRenderManager14)
        ->EmitVariantRenderResourceEntry(
            Rect, Point, pScreen->m_nResourceId3D8, 0,
            pScreen->m_pSelector3C4, 0);
    Point[0] = (short)pLayout[24];
    Point[1] = (short)pLayout[25];
    ((LevelVariantRenderManagerView *)pScreen->m_abVariantRenderManager14)
        ->EmitVariantRenderResourceEntry(
            Rect, Point, pScreen->m_nResourceId3DC, 0,
            pScreen->m_pSelector3C0, 0);
    Point[0] = (short)pLayout[18];
    Point[1] = (short)pLayout[19];
    ((LevelVariantRenderManagerView *)pScreen->m_abVariantRenderManager14)
        ->EmitVariantRenderResourceEntry(
            Rect, Point, pScreen->m_nResourceId3E0, 0,
            pScreen->m_pSelector3C8, 0);

    if (pScreen->m_fNetworkScreen8C != 0) {
        Point[0] = (short)pLayout[42];
        Point[1] = (short)pLayout[43];
        ((LevelVariantRenderManagerView *)
            pScreen->m_abVariantRenderManager14)
                ->EmitVariantRenderResourceEntry(
                    Rect, Point, pScreen->m_nResourceId3D8, 0,
                    pScreen->m_pSelector3C4,
                    pScreen->m_nPaletteRemap3D0);
        GetFunctionalVariantFrameExtent(
            pScreen->m_abVariantRenderManager14, Extent,
            pScreen->m_nResourceId3D8, 0);
        cxFrame = Extent[0];
        x = pLayout[10] - cxFrame / 4 + pLayout[12];
        for (i = 0; i < pScreen->m_cNetworkStatus3F0; ++i) {
            x -= cxFrame + cxFrame / 8;
            Point[0] = (short)x;
            Point[1] = (short)pLayout[47];
            ((LevelVariantRenderManagerView *)
                pScreen->m_abVariantRenderManager14)
                    ->EmitVariantRenderResourceEntry(
                        Rect, Point, pScreen->m_nResourceId3D8, 0,
                        pScreen->m_pSelector3C4,
                        pScreen->m_nPaletteRemap3D0);
        }
    }

    GetFunctionalVariantFrameExtent(
        pScreen->m_abVariantRenderManager14, Extent,
        pScreen->m_nResourceId3D8, 0);
    cxFrame = Extent[0];
    x = pLayout[10] - cxFrame / 4 + pLayout[12];
    for (i = 0; i < pScreen->m_cStatus3EC; ++i) {
        x -= cxFrame + cxFrame / 8;
        Point[0] = (short)x;
        Point[1] = (short)pLayout[35];
        ((LevelVariantRenderManagerView *)
            pScreen->m_abVariantRenderManager14)
                ->EmitVariantRenderResourceEntry(
                    Rect, Point, pScreen->m_nResourceId3D8, 0,
                    pScreen->m_pSelector3C4, 0);
    }

    GetFunctionalVariantFrameExtent(
        pScreen->m_abVariantRenderManager14, Extent,
        pScreen->m_nResourceId3DC, 0);
    x = pLayout[10] - pLayout[50] + pLayout[12];
    if (pScreen->m_cSkillStatus3F4 < 5) {
        for (i = 0; i < pScreen->m_cSkillStatus3F4; ++i) {
            x -= Extent[0];
            Point[0] = (short)x;
            Point[1] = (short)pLayout[29];
            ((LevelVariantRenderManagerView *)
                pScreen->m_abVariantRenderManager14)
                    ->EmitVariantRenderResourceEntry(
                        Rect, Point, pScreen->m_nResourceId3DC, 0,
                        pScreen->m_pSelector3C0, 0);
        }
    }
}

static short *GetFunctionalVariantFrameExtent(
    void *pObject, short *pExtent, int nResourceId, int iFrame) {
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

static void LEMBALL_FASTCALL DrawFunctionalLevelSelectionText(
    void *pObject) {
    static const char *s_apszSkillNames[5] = {
        "Fun", " Tricky", "Taxing", "Mayhem", "Network"
    };
    static const int s_anStatusLabelPoints[4] = { 10, 13, 16, 22 };
    GameLevelSelectionScreenView *pScreen;
    MappedListResourceTable *pMappedList;
    void *pFontResource;
    const char *pszTime;
    const char *pszSkill;
    short Point[2];
    short Relative[2];
    short Extent[2];
    int cStatusLabels;
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

    cStatusLabels = pScreen->m_fNetworkScreen8C != 0 ? 4 : 3;
    for (i = 0; i < cStatusLabels; ++i) {
        Point[0] = (short)pScreen->m_pLayout3D4[
            s_anStatusLabelPoints[i] * 2];
        Point[1] = (short)pScreen->m_pLayout3D4[
            s_anStatusLabelPoints[i] * 2 + 1];
        pMappedList->SubmitRenderEntry(
            pScreen->m_pRenderContext88, Point, Relative,
            pScreen->m_nMappedListRemapResourceId384,
            (void *)"x", 0x20, 0);
    }

    pszTime = pScreen->m_abTime45C;
    if (pScreen->m_abTime45C[0] >= ':') {
        pszTime = "Infinite";
    }
    Point[0] = (short)pScreen->m_pLayout3D4[22];
    Point[1] = (short)pScreen->m_pLayout3D4[23];
    pMappedList->SubmitRenderEntry(
        pScreen->m_pRenderContext88, Point, Relative,
        pScreen->m_nMappedListRemapResourceId384,
        (void *)pszTime, 0x20, 0);
    nPack = ((GameLevelProgressStateView *)g_pLevelProgressState)
                ->m_nCurrentPack08;
    if (nPack < 0 || 4 < nPack) {
        nPack = 0;
    }
    pszSkill = s_apszSkillNames[nPack];
    pFontResource = pMappedList->GetMappedResource(
        pScreen->m_nMappedListRemapResourceId384);
    MeasureFunctionalLevelTitleExtent(pFontResource, Extent, pszSkill);
    Point[0] = (short)(pScreen->m_pLayout3D4[40] - Extent[0] / 2);
    Point[1] = (short)pScreen->m_pLayout3D4[41];
    pMappedList->SubmitRenderEntry(
        pScreen->m_pRenderContext88, Point, Relative,
        pScreen->m_nMappedListRemapResourceId384,
        (void *)pszSkill, 0x20, 0);
    if (4 < pScreen->m_cSkillStatus3F4) {
        Point[0] = (short)pScreen->m_pLayout3D4[22];
        Point[1] = (short)pScreen->m_pLayout3D4[53];
        pMappedList->SubmitRenderEntry(
            pScreen->m_pRenderContext88, Point, Relative,
            pScreen->m_nMappedListRemapResourceId384,
            (void *)"None", 0x20, 0);
    }
}

static void LEMBALL_FASTCALL RefreshFunctionalLevelSelectionMetadata(
    GameLevelSelectionScreenView *pScreen) {
    GameLevelProgressStateView *pProgress;
    const char *pszTitle;
    void *pFontResource;
    char szLine[0x20];
    short Extent[2];
    int iLine;
    int iSource;
    int iLineStart;
    int iLastSpaceSource;
    int iLastSpaceDest;
    int cchLine;
    int y;
    int cchTitle;
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
        strcpy(Metadata.m_szTitle08, "UN-NAMED LEVEL");
    }
    pScreen->m_fSelectorDirty360 = 1;
    pScreen->m_fFrameDirty330 = 1;
    memset(pScreen->m_aszTitleLines3FC, 0,
           sizeof(pScreen->m_aszTitleLines3FC));
    for (iLine = 0; iLine < 3; ++iLine) {
        pScreen->m_aTitlePositions464[iLine][0] = -1;
        pScreen->m_aTitlePositions464[iLine][1] = -1;
    }

    pszTitle = Metadata.m_szTitle08;
    cchTitle = strlen(pszTitle);
    iSource = 0;
    y = pScreen->m_pLayout3D4[37];
    pFontResource = ((MappedListResourceTable *)
        pScreen->m_pMappedListResources98)->GetMappedResource(
            pScreen->m_nMappedListRemapResourceId384);
    for (iLine = 0; iLine < 3 && iSource < cchTitle; ++iLine) {
        while (pszTitle[iSource] == ' ') {
            ++iSource;
        }
        iLineStart = iSource;
        iLastSpaceSource = -1;
        iLastSpaceDest = -1;
        cchLine = 0;
        szLine[0] = '\0';
        while (iSource < cchTitle && cchLine < 0x1f) {
            if (pszTitle[iSource] == ' ') {
                iLastSpaceSource = iSource;
                iLastSpaceDest = cchLine;
            }
            szLine[cchLine++] = pszTitle[iSource++];
            szLine[cchLine] = '\0';
            MeasureFunctionalLevelTitleExtent(
                pFontResource, Extent, szLine);
            if (pScreen->m_pLayout3D4[38] < Extent[0]) {
                if (iLastSpaceDest >= 0) {
                    cchLine = iLastSpaceDest;
                    iSource = iLastSpaceSource + 1;
                } else if (1 < cchLine) {
                    --cchLine;
                    --iSource;
                }
                break;
            }
        }
        if (cchLine == 0 && iSource == iLineStart) {
            szLine[cchLine++] = pszTitle[iSource++];
        }
        while (0 < cchLine && szLine[cchLine - 1] == ' ') {
            --cchLine;
        }
        szLine[cchLine] = '\0';
        strcpy(pScreen->m_aszTitleLines3FC[iLine], szLine);
        MeasureFunctionalLevelTitleExtent(pFontResource, Extent, szLine);
        pScreen->m_aTitlePositions464[iLine][0] =
            pScreen->m_pLayout3D4[36] +
            (pScreen->m_pLayout3D4[38] - Extent[0]) / 2;
        pScreen->m_aTitlePositions464[iLine][1] = y;
        y += Extent[1];
    }
    pScreen->m_nLevelTime3F8 = Metadata.m_nTimeSeconds06;
    pScreen->m_abTime45C[0] =
        (char)(Metadata.m_nTimeSeconds06 / 60 + '0');
    pScreen->m_abTime45C[1] = ':';
    pScreen->m_abTime45C[2] =
        (char)((Metadata.m_nTimeSeconds06 % 60) / 10 + '0');
    pScreen->m_abTime45C[3] =
        (char)(Metadata.m_nTimeSeconds06 % 10 + '0');
    pScreen->m_abTime45C[4] = '\0';
    pScreen->m_cSkillStatus3F4 = Metadata.m_cSkill00;
    pScreen->m_cStatus3EC = Metadata.m_cPlayers02;
    if (pScreen->m_fNetworkScreen8C != 0) {
        pScreen->m_cNetworkStatus3F0 = Metadata.m_cNetworkPlayers04;
    }
    pScreen->m_fPreviousLocked484 = nCurrentLevel == 0;
    if (nPack == 4) {
        pScreen->m_fNextLocked480 = nCurrentLevel > 10;
    } else {
        nUnlockedCap = pProgress->m_anUnlockedPackCaps28[nPack];
        pScreen->m_fNextLocked480 = nCurrentLevel == nUnlockedCap;
    }
    RefreshQueuedVariantChildByToken(
        pScreen->m_pQueuedVariantManager338, 0, (int)0xacef000eU);
    RefreshQueuedVariantChildByToken(
        pScreen->m_pQueuedVariantManager338, 0, (int)0xacef000fU);
}

static void MeasureFunctionalLevelTitleExtent(
    void *pResource, short *pExtent, const char *pszText) {
    void *pGlyph;
    void *pGlyphList;
    int i;
    int cx;
    int cy;

    cx = 0;
    cy = 0;
    if (pResource != 0) {
        pGlyphList = *(void **)((char *)pResource + 0x78);
        for (i = 0; pszText[i] != '\0'; ++i) {
            pGlyph = ((GameLevelTitleGlyphListInterface *)pGlyphList)
                ->FindGlyph((unsigned char)pszText[i]);
            if (pGlyph == 0) {
                pGlyph = ((GameLevelTitleGlyphListInterface *)pGlyphList)
                    ->FindGlyph('I');
                if (pGlyph == 0) {
                    pGlyph = *(void **)((char *)pResource + 0x7c);
                }
            }
            if (pGlyph != 0) {
                cx += *(short *)((char *)pGlyph + 0x4c) + 1;
                if (cy < *(short *)((char *)pGlyph + 0x4a) +
                         *(short *)((char *)pGlyph + 0x4e)) {
                    cy = *(short *)((char *)pGlyph + 0x4a) +
                         *(short *)((char *)pGlyph + 0x4e);
                }
            }
        }
        if (0 < cx) {
            --cx;
        }
    }
    pExtent[0] = (short)cx;
    pExtent[1] = (short)cy;
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
    pProgress->m_anSnapshotPackCaps3C[nPack] =
        pProgress->m_nCurrentLevel00;
    RefreshFunctionalLevelSelectionMetadata(pScreen);
}

static void LEMBALL_FASTCALL NoopFunctionalLevelSelectionHook(void *) {
}

// FUNCTION: LEMBALL 0x0044fa00
static void LEMBALL_FASTCALL EmitQueuedVariantChildSlotEntry(
    void *pObject)
{
    char *pManager;
    char *pGeometryOwner;
    char *pGeometryDispatch;
    void *pRenderContext;
    void *pSavedOwner;
    int nDispatchOffset;
    int nPointBias;
    short Point[2];
    short Rect[4];

    pManager = (char *)pObject;
    if (*(int *)(pManager + 0x144) == 0) {
        return;
    }
    pRenderContext = *(void **)(pManager + 0x12c);
    nPointBias = *(int *)(pManager + 0x130) == 1 ? -1 : -2;
    pGeometryOwner = *(char **)((char *)pRenderContext + 0x0c);
    nDispatchOffset =
        *(int *)((char *)*(void **)(pGeometryOwner + 0x40) + 4);
    pGeometryDispatch = pGeometryOwner + 0x40 + nDispatchOffset;
    *(short *)(pManager + 0x100) =
        *(short *)(pGeometryDispatch + 0x14);
    *(short *)(pManager + 0x102) =
        *(short *)(pGeometryDispatch + 0x16);
    *(short *)(pManager + 0x104) = 0;
    *(short *)(pManager + 0x106) = 0;
    *(int *)(pManager + 0x108) = 0x10000;
    ((GameMainMenuRenderEntryInterface *)(pManager + 0xfc))
        ->AppendToQueue(pRenderContext);
    *(int *)(pManager + 0x124) = 0;
    Point[0] = (short)(*(int *)(pManager + 0x98) + nPointBias);
    Point[1] = (short)(*(int *)(pManager + 0x9c) + nPointBias);
    pSavedOwner = *(void **)(pManager + 0x2c);
    *(void **)(pManager + 0x2c) = pRenderContext;
    ((LevelVariantRenderManagerView *)(pManager + 0x10))
        ->EmitVariantRenderResourceEntry(
            Rect, Point,
            g_nQueuedVariantChildSlotManagerModeSelectedResourceId, 0,
            pManager + 0x118, 0);
    *(void **)(pManager + 0x2c) = pSavedOwner;
    ((LevelVariantRenderManagerView *)(pManager + 0x10))
        ->ResetOrFlipVariantRenderBuffers();
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
