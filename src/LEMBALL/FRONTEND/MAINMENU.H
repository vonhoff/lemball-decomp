#ifndef LEMBALL_GAME_MAIN_MENU_SCREEN_H
#define LEMBALL_GAME_MAIN_MENU_SCREEN_H

#include "VISOS/COMMON.H"

extern void *g_GAME_MainMenuScreenVtable[22];
extern void *g_GAME_MainMenuQueueVtable[4];
extern void *g_GAME_MainMenuCallbackVtable[2];

void *LEMBALL_FASTCALL ConstructFramedScreenBaseFields(
    void *pScreen, int nUnusedEdx, void *pPrimaryContext,
    void *pRenderContext, short *paSize, int nScreenType,
    int cExtraPackagedEntries, int cExtraActionEntries,
    int nVariantStateCount, int cMappedListEntries,
    int nMappedListMode);
void LEMBALL_FASTCALL ActivateFramedScreenBase(void *pScreen);

#endif
