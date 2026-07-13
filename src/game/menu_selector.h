#ifndef LEMBALL_GAME_MENU_SELECTOR_H
#define LEMBALL_GAME_MENU_SELECTOR_H

#include "engine/common.h"

struct GAME_MenuSelectorEntry;

struct GAME_MenuSelectorEvent {
    short m_nType00;
    short m_nReserved02;
    int m_dwTimestamp04;
    int m_nPayload08;
    int m_nPayload0C;
    int m_nPayload10;
    int m_nPayload14;
    int m_nPayload18;
};

struct GAME_MenuSelectorMetadata {
    int m_xSide0;
    int m_nPosition;
    int m_xSide1;
    int m_nSide;
    int m_fSide0Action;
    int m_fSide1Action;
    int m_nSide0Token;
    int m_nSide1Token;
};

struct GAME_ProgressiveVariantFrameSelector {
    void **m_pVtable;
    int m_cFrames;
    int m_nReserved08;
    int m_dwStartTime;
    int m_dwDuration;
    int m_dwFixedTime;
    int m_nDirection;
};

/* Exact 0x27c manager layout constructed by 0x44c870.  The embedded
 * variant-render manager remains byte-typed because its independently owned
 * class is reconstructed in level_status.cpp. */
struct GAME_MenuSelectorManager {
    void **m_pVtable00;
    int m_nQueueTag04;
    int m_nQueueReserved08;
    int m_cUnhandledEvents0C;
    unsigned char m_abVariantRenderManager10[0x70];
    int m_cChildren80;
    int m_nReserved84;
    int m_xCenteredAction88;
    int m_nCurrentPosition8C;
    int m_nMovementStartPosition90;
    int m_nCurrentSide94;
    int m_xActionOrigin98;
    int m_nTargetPosition9C;
    int m_nSelectedSideA0;
    int m_nActionStateA4;
    int m_xActionA8;
    int m_yActionAC;
    int m_xActionDestinationB0;
    int m_fEventQueuedB4;
    GAME_MenuSelectorEvent m_ActionEventB8;
    int m_dwConstructionTimeD4;
    int m_fDelayedEventD8;
    int m_fMovementActiveDC;
    GAME_MenuSelectorMetadata m_aMetadataE0[8];
    GAME_MenuSelectorEntry *m_apChildren1E0[8];
    unsigned char m_abLocalRectEntry200[0x10];
    void *m_pRenderContext210;
    void *m_pRenderOwner214;
    int m_nNextToken218;
    int m_dwMovementStart21C;
    int m_dwMovementEnd220;
    int m_dwSideTransitionStart224;
    int m_dwSideTransitionEnd228;
    int m_dwActionLastTick22C;
    int m_dwActionMotionEnd230;
    int m_nReserved234;
    int m_nReserved238;
    int m_dwActionFrameStart23C;
    int m_dwActionFrameEnd240;
    GAME_ProgressiveVariantFrameSelector *m_pSideSelector244;
    GAME_ProgressiveVariantFrameSelector *m_pActionTailSelector248;
    GAME_ProgressiveVariantFrameSelector *m_pActionLeadSelector24C;
    GAME_ProgressiveVariantFrameSelector *m_pCompletionSelector250;
    GAME_ProgressiveVariantFrameSelector *m_pActionMiddleSelector254;
    void **m_pCurrentFrameCallbackVtable258;
    int m_cCurrentFrameCallbackFrames25C;
    int m_nCurrentFrameCallbackStart260;
    int m_nCurrentFrameCallbackFrame264;
    void *m_pChildContext268;
    void *m_pChildRenderContext26C;
    int m_nLayoutMode270;
    int m_fCompactLayout274;
    int m_fChildrenEnabled278;
};

struct GAME_MenuSelectorButtonConfig {
    int m_x;
    int m_nPosition;
    const int *m_panResourceIds;
    int m_nActivationMode;
    int m_nFirstResource;
    int m_nLastResource;
    int m_nReserved;
    int *m_pSelection;
    int m_nCommand;
};

typedef char GAME_MenuSelectorEvent_size_must_be_0x1c[
    sizeof(GAME_MenuSelectorEvent) == 0x1c ? 1 : -1];
typedef char GAME_MenuSelectorMetadata_size_must_be_0x20[
    sizeof(GAME_MenuSelectorMetadata) == 0x20 ? 1 : -1];
typedef char GAME_ProgressiveVariantFrameSelector_size_must_be_0x1c[
    sizeof(GAME_ProgressiveVariantFrameSelector) == 0x1c ? 1 : -1];
typedef char GAME_MenuSelectorManager_size_must_be_0x27c[
    sizeof(GAME_MenuSelectorManager) == 0x27c ? 1 : -1];

GAME_MenuSelectorManager *LEMBALL_FASTCALL ConstructMenuSelectorManager(
    GAME_MenuSelectorManager *pManager, int nUnusedEdx, void *pRenderOwner,
    void *pRenderContext, int nChildCapacity, int nLayoutMode);
GAME_MenuSelectorManager *LEMBALL_FASTCALL ConstructMenuSelectorManagerThunk(
    GAME_MenuSelectorManager *pManager, int nUnusedEdx, void *pRenderOwner,
    void *pRenderContext, int nChildCapacity, int nLayoutMode);
void LEMBALL_FASTCALL AppendMenuSelectorChild(
    GAME_MenuSelectorManager *pManager, int nUnusedEdx, int x,
    int nPosition, const int *panResourceIds, int nActivationMode,
    int nFirstResource, int nLastResource, int nReserved,
    int *pSelection, int nCommand);
void LEMBALL_FASTCALL AppendMenuSelectorChildThunk(
    GAME_MenuSelectorManager *pManager, int nUnusedEdx, int x,
    int nPosition, const int *panResourceIds, int nActivationMode,
    int nFirstResource, int nLastResource, int nReserved,
    int *pSelection, int nCommand);
void LEMBALL_FASTCALL InitializeMenuSelectorCurrentState(
    GAME_MenuSelectorManager *pManager, int nUnusedEdx, int iMetadata);
void LEMBALL_FASTCALL InitializeMenuSelectorCurrentStateThunk(
    GAME_MenuSelectorManager *pManager, int nUnusedEdx, int iMetadata);

/* This boundary is implemented only after the exact 0x497e30 descendant
 * window-owner table is available. */
void LEMBALL_FASTCALL InitializeMenuSelectorChildContext(
    GAME_MenuSelectorManager *pManager);
void LEMBALL_FASTCALL InitializeMenuSelectorChildContextThunk(
    GAME_MenuSelectorManager *pManager);

void LEMBALL_FASTCALL UpdateMenuSelectorAnimation(
    GAME_MenuSelectorManager *pManager);
void LEMBALL_FASTCALL UpdateMenuSelectorAnimationThunk(
    GAME_MenuSelectorManager *pManager);
void LEMBALL_FASTCALL SetMenuSelectorChildrenEnabled(
    GAME_MenuSelectorManager *pManager, int nUnusedEdx, int fEnabled);
void LEMBALL_FASTCALL SetMenuSelectorChildrenEnabledThunk(
    GAME_MenuSelectorManager *pManager, int nUnusedEdx, int fEnabled);
void LEMBALL_FASTCALL RebuildMenuSelectorChildRects(
    GAME_MenuSelectorManager *pManager, int nUnusedEdx, int fForce);
void LEMBALL_FASTCALL RebuildMenuSelectorChildRectsThunk(
    GAME_MenuSelectorManager *pManager, int nUnusedEdx, int fForce);

#endif
