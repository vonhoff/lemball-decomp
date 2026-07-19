#ifndef LEMBALL_GAME_MAIN_MENU_SCREEN_H
#define LEMBALL_GAME_MAIN_MENU_SCREEN_H

#include "ENGINE/CORE/COMMON.H"

extern void *g_GAME_MainMenuScreenVtable[22];
void *LEMBALL_FASTCALL ConstructFunctionalLevelSelectionScreen(
    void *pObject, int nUnusedEdx, void *pPrimaryContext,
    void *pRenderContext, short *paSize);
extern void *g_GAME_MainMenuQueueVtable[4];
extern void *g_GAME_MainMenuCallbackVtable[2];
extern void *g_GAME_IntroSequenceScreenVtable[22];
extern void *g_GAME_IntroSequenceQueueVtable[4];
extern void *g_GAME_IntroSequenceCallbackVtable[2];
int LEMBALL_FASTCALL GetIntroSequenceMediaNotifyFlags(void *pOwner);
void LEMBALL_STDCALL IgnoreIntroSequenceMediaStateChange(int nState);
void LEMBALL_STDCALL IgnoreIntroSequenceMediaPositionChange(int nPosition);
void IgnoreIntroSequenceMediaNotificationThunk(void);
void LEMBALL_FASTCALL HandleIntroSequenceMediaCompletionThunk(void *pOwner);
void LEMBALL_FASTCALL NoopIntroSequenceScreenHookThunk(void *pObject);
void LEMBALL_FASTCALL DestroyFramedScreenBaseThunk00402081(
    void *pObject);
void LEMBALL_FASTCALL DestroyIntroSequenceScreenThunk(void *pObject);

class GameIntroSequenceScreen {
public:
    int HandleInput(short *pEvent);
    void RefreshMediaPath(void);
    void PollPlaybackCompletion(void);
    void UpdatePlayback(void);
};

struct GameFramedScreenBase {
    void *ConstructFramedScreenBaseFields(
        void *pPrimaryContext, void *pRenderContext, short *paSize,
        int nScreenType, int cExtraPackagedEntries,
        int cExtraActionEntries, int nVariantStateCount,
        int cMappedListEntries, int nMappedListMode);
    void UpdateRuntime(void);
    void RecenterIfNeeded(void);
    void SetScreenSize(short *paSize);
    void ReleaseFrameResources(void);

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
    int m_nPaletteResourceIdB4;
    int m_nAppliedPaletteResourceIdB8;
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

void LEMBALL_FASTCALL ActivateFramedScreenBase(void *pScreen);
void LEMBALL_FASTCALL ActivateFramedScreenBaseThunk(void *pScreen);
void LEMBALL_FASTCALL DestroyFunctionalFramedScreenBase(void *pScreen);
void *LEMBALL_FASTCALL ConstructFunctionalOptionsScreen(
    void *pObject, int nUnusedEdx, void *pPrimaryContext,
    void *pRenderContext, short *paSize);
void *LEMBALL_FASTCALL ConstructFunctionalPaintballSequenceScreen(
    void *pObject, int nUnusedEdx, void *pPrimaryContext,
    void *pRenderContext, short *paSize, int fSecondVariant);
void *LEMBALL_FASTCALL ConstructFunctionalPasswordEntryScreen(
    void *pObject, int nUnusedEdx, void *pPrimaryContext,
    void *pRenderContext, short *paSize);
void *LEMBALL_FASTCALL ConstructFunctionalNetworkLobbyScreen(
    void *pObject, int nUnusedEdx, void *pPrimaryContext,
    void *pRenderContext, short *paSize);

#endif
