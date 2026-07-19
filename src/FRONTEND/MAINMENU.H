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

void *LEMBALL_FASTCALL ConstructFramedScreenBaseFields(
    void *pScreen, int nUnusedEdx, void *pPrimaryContext,
    void *pRenderContext, short *paSize, int nScreenType,
    int cExtraPackagedEntries, int cExtraActionEntries,
    int nVariantStateCount, int cMappedListEntries,
    int nMappedListMode);
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
