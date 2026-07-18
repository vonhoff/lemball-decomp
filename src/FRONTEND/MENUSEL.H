#ifndef LEMBALL_GAME_MENU_SELECTOR_H
#define LEMBALL_GAME_MENU_SELECTOR_H

#include "ENGINE/CORE/COMMON.H"

struct GameMenuSelectorEntry;
struct VS_XYPair;

// SIZE 0x1c
struct GameMenuSelectorEvent {
    short m_nType00;     // 0x00
    short m_nReserved02; // 0x02
    int m_dwTimestamp04; // 0x04
    int m_nPayload08;    // 0x08
    int m_nPayload0C;    // 0x0c
    int m_nPayload10;    // 0x10
    int m_nPayload14;    // 0x14
    int m_nPayload18;    // 0x18
};

// SIZE 0x20
struct GameMenuSelectorMetadata {
    int m_xSide0;        // 0x00
    int m_nPosition;     // 0x04
    int m_xSide1;        // 0x08
    int m_nSide;         // 0x0c
    int m_fSide0Action;  // 0x10
    int m_fSide1Action;  // 0x14
    int m_nSide0Token;   // 0x18
    int m_nSide1Token;   // 0x1c
};

// SIZE 0x1c
struct GameProgressiveVariantFrameSelector {
    void **m_pVtable;  // 0x00
    int m_cFrames;     // 0x04
    int m_nReserved08; // 0x08
    int m_dwStartTime; // 0x0c
    int m_dwDuration;  // 0x10
    int m_dwFixedTime; // 0x14
    int m_nDirection;  // 0x18
};

extern void *g_MenuSelectorFrameSelectorCtorVtable[2];
extern void *g_MenuSelectorTimedFrameSelectorVtable[6];
extern void *g_MenuSelectorProgressiveFrameSelectorVtable[6];
extern void *g_MenuSelectorCurrentFrameCallbackVtable[7];

/* Exact 0x27c manager layout constructed by 0x44c870.  The embedded
 * variant-render manager remains byte-typed because its independently owned
 * class is reconstructed in LevelStatus.cpp. */
// SIZE 0x27c
struct GameMenuSelectorManager {
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
    GameMenuSelectorEvent m_ActionEventB8;
    int m_dwConstructionTimeD4;
    int m_fDelayedEventD8;
    int m_fMovementActiveDC;
    GameMenuSelectorMetadata m_aMetadataE0[8];
    GameMenuSelectorEntry *m_apChildren1E0[8];
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
    GameProgressiveVariantFrameSelector *m_pSideSelector244;
    GameProgressiveVariantFrameSelector *m_pActionTailSelector248;
    GameProgressiveVariantFrameSelector *m_pActionLeadSelector24C;
    GameProgressiveVariantFrameSelector *m_pCompletionSelector250;
    GameProgressiveVariantFrameSelector *m_pActionMiddleSelector254;
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

struct GameMenuSelectorButtonConfig {
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

GameMenuSelectorManager *LEMBALL_FASTCALL ConstructMenuSelectorManager(
    GameMenuSelectorManager *pManager, int nUnusedEdx, void *pRenderOwner,
    void *pRenderContext, int nChildCapacity, int nLayoutMode);
GameMenuSelectorManager *LEMBALL_FASTCALL ConstructMenuSelectorManagerThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, void *pRenderOwner,
    void *pRenderContext, int nChildCapacity, int nLayoutMode);
void LEMBALL_FASTCALL AppendMenuSelectorChild(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int x,
    int nPosition, const int *panResourceIds, int nActivationMode,
    int nFirstResource, int nLastResource, int nReserved,
    int *pSelection, int nCommand);
void LEMBALL_FASTCALL AppendMenuSelectorChildThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int x,
    int nPosition, const int *panResourceIds, int nActivationMode,
    int nFirstResource, int nLastResource, int nReserved,
    int *pSelection, int nCommand);
void LEMBALL_FASTCALL InitializeMenuSelectorCurrentState(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int iMetadata);
void LEMBALL_FASTCALL InitializeMenuSelectorCurrentStateThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int iMetadata);

/* This boundary is implemented only after the exact 0x497e30 descendant
 * window-owner table is available. */
void LEMBALL_FASTCALL InitializeMenuSelectorChildContext(
    GameMenuSelectorManager *pManager);
void LEMBALL_FASTCALL InitializeMenuSelectorChildContextThunk(
    GameMenuSelectorManager *pManager);

void LEMBALL_FASTCALL UpdateMenuSelectorAnimation(
    GameMenuSelectorManager *pManager);
void LEMBALL_FASTCALL UpdateMenuSelectorAnimationThunk(
    GameMenuSelectorManager *pManager);
void LEMBALL_FASTCALL SetMenuSelectorChildrenEnabled(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int fEnabled);
void LEMBALL_FASTCALL SetMenuSelectorChildrenEnabledThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int fEnabled);
void LEMBALL_FASTCALL RebuildMenuSelectorChildRects(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int fForce);
void LEMBALL_FASTCALL RebuildMenuSelectorChildRectsThunk(
    GameMenuSelectorManager *pManager, int nUnusedEdx, int fForce);
void LEMBALL_FASTCALL EmitMenuSelectorResourceEntries(
    GameMenuSelectorManager *pManager);
void *LEMBALL_FASTCALL ConstructAlignedZrleChildOverlayFromPoint(
    void *pObject, int nUnusedEdx, const VS_XYPair *pPoint,
    void *pRenderOwner, int nResourceId, int fAlignment);
void LEMBALL_FASTCALL DestroyAlignedZrleChildOverlay(void *pChildOwner);

void *LEMBALL_FASTCALL ConstructQueuedVariantChildSlotManager(
    void *pManager, int nUnusedEdx, void *pRenderOwner,
    void *pRenderContext, int nChildCapacity,
    int nLayoutMode, int nNavigationMode);
void LEMBALL_FASTCALL AppendQueuedVariantChildSlot(
    void *pManager, int nUnusedEdx, int x, int y,
    const int *panResourceIds, int nActivationMode,
    int nFirstResource, int nLastResource, int nReserved,
    int *pSelection, int nCommand);
void LEMBALL_FASTCALL SelectQueuedVariantChildSlot(
    void *pManager, int nUnusedEdx, int iChild);
void LEMBALL_FASTCALL InitializeQueuedVariantChildContext(
    void *pManager);
void LEMBALL_FASTCALL RefreshQueuedVariantChildByToken(
    void *pManager, int nUnusedEdx, int nToken);
void LEMBALL_FASTCALL RebuildQueuedVariantChildSlotSourceRects(
    void *pManager, int nUnusedEdx, int fForce);
void LEMBALL_FASTCALL DestroyQueuedVariantChildSlotManager(
    void *pManager);

#endif
