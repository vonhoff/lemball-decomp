#ifndef LEMBALL_GAME_MENU_SELECTOR_H
#define LEMBALL_GAME_MENU_SELECTOR_H

#include "ENGINE/CORE/COMMON.H"

struct GameMenuSelectorEntry;
struct GameQueuedVariantChildSlotManager;
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
	int m_xSide0;       // 0x00
	int m_nPosition;    // 0x04
	int m_xSide1;       // 0x08
	int m_nSide;        // 0x0c
	int m_fSide0Action; // 0x10
	int m_fSide1Action; // 0x14
	int m_nSide0Token;  // 0x18
	int m_nSide1Token;  // 0x1c
};

// SIZE 0x1c
struct GameProgressiveVariantFrameSelector {
	void** m_pVtable;  // 0x00
	int m_cFrames;     // 0x04
	int m_nReserved08; // 0x08
	int m_dwStartTime; // 0x0c
	int m_dwDuration;  // 0x10
	int m_dwFixedTime; // 0x14
	int m_nDirection;  // 0x18
};

int LEMBALL_FASTCALL GetMenuSelectorStartTimeThunk(GameProgressiveVariantFrameSelector* pSelector);
int LEMBALL_FASTCALL SelectTimedVariantFrameIndex(GameProgressiveVariantFrameSelector* pSelector);
int LEMBALL_FASTCALL SelectProgressiveVariantFrameIndex(GameProgressiveVariantFrameSelector* pSelector);

extern void* g_MenuSelectorFrameSelectorCtorVtable[2];
extern void* g_MenuSelectorTimedFrameSelectorVtable[6];
extern void* g_MenuSelectorProgressiveFrameSelectorVtable[6];
extern void* g_MenuSelectorCurrentFrameCallbackVtable[7];

/* Exact 0x27c manager layout constructed by 0x44c870.  The embedded
 * variant-render manager remains byte-typed because its independently owned
 * class is reconstructed in LevelStatus.cpp. */
// SIZE 0x27c
struct GameMenuSelectorManager {
	void** m_pVtable00;                                              // 0x00
	int m_nQueueTag04;                                               // 0x04
	int m_nQueueReserved08;                                          // 0x08
	int m_cUnhandledEvents0C;                                        // 0x0c
	unsigned char m_abVariantRenderManager10[0x70];                  // 0x10
	int m_cChildren80;                                               // 0x80
	int m_nReserved84;                                               // 0x84
	int m_xCenteredAction88;                                         // 0x88
	int m_nCurrentPosition8C;                                        // 0x8c
	int m_nMovementStartPosition90;                                  // 0x90
	int m_nCurrentSide94;                                            // 0x94
	int m_xActionOrigin98;                                           // 0x98
	int m_nTargetPosition9C;                                         // 0x9c
	int m_nSelectedSideA0;                                           // 0xa0
	int m_nActionStateA4;                                            // 0xa4
	int m_xActionA8;                                                 // 0xa8
	int m_yActionAC;                                                 // 0xac
	int m_xActionDestinationB0;                                      // 0xb0
	int m_fEventQueuedB4;                                            // 0xb4
	GameMenuSelectorEvent m_ActionEventB8;                           // 0xb8
	int m_dwConstructionTimeD4;                                      // 0xd4
	int m_fDelayedEventD8;                                           // 0xd8
	int m_fMovementActiveDC;                                         // 0xdc
	GameMenuSelectorMetadata m_aMetadataE0[8];                       // 0xe0
	GameMenuSelectorEntry* m_apChildren1E0[8];                       // 0x1e0
	unsigned char m_abLocalRectEntry200[0x10];                       // 0x200
	void* m_pRenderContext210;                                       // 0x210
	void* m_pRenderOwner214;                                         // 0x214
	int m_nNextToken218;                                             // 0x218
	int m_dwMovementStart21C;                                        // 0x21c
	int m_dwMovementEnd220;                                          // 0x220
	int m_dwSideTransitionStart224;                                  // 0x224
	int m_dwSideTransitionEnd228;                                    // 0x228
	int m_dwActionLastTick22C;                                       // 0x22c
	int m_dwActionMotionEnd230;                                      // 0x230
	int m_nReserved234;                                              // 0x234
	int m_nReserved238;                                              // 0x238
	int m_dwActionFrameStart23C;                                     // 0x23c
	int m_dwActionFrameEnd240;                                       // 0x240
	GameProgressiveVariantFrameSelector* m_pSideSelector244;         // 0x244
	GameProgressiveVariantFrameSelector* m_pActionTailSelector248;   // 0x248
	GameProgressiveVariantFrameSelector* m_pActionLeadSelector24C;   // 0x24c
	GameProgressiveVariantFrameSelector* m_pCompletionSelector250;   // 0x250
	GameProgressiveVariantFrameSelector* m_pActionMiddleSelector254; // 0x254
	void** m_pCurrentFrameCallbackVtable258;                         // 0x258
	int m_cCurrentFrameCallbackFrames25C;                            // 0x25c
	int m_nCurrentFrameCallbackStart260;                             // 0x260
	int m_nCurrentFrameCallbackFrame264;                             // 0x264
	void* m_pChildContext268;                                        // 0x268
	void* m_pChildRenderContext26C;                                  // 0x26c
	int m_nLayoutMode270;                                            // 0x270
	int m_fCompactLayout274;                                         // 0x274
	int m_fChildrenEnabled278;                                       // 0x278

	GameMenuSelectorManager(void* pRenderOwner, void* pRenderContext, int nChildCapacity, int nLayoutMode);
	void AppendChild(int x,
					 int nPosition,
					 const int* panResourceIds,
					 int nActivationMode,
					 int nFirstResource,
					 int nLastResource,
					 int nReserved,
					 int* pSelection,
					 int nCommand);
	void InitializeCurrentState(int iMetadata);
	void RecordChildMetadata(int x, int nPosition, int fAction, int nToken);
	int HandleEvent(GameMenuSelectorEvent* pEvent);
	void MovePrevious(void);
	void MoveNext(void);
	void SelectSide0(void);
	void SelectSide1(void);
	void StartAction(void);
	void RebuildChildRects(int fForce, int nUnused);
	void InitializeChildContext(void);
	void InitializeChildContextThunk(void);
};

struct GameMenuSelectorButtonConfig {
	int m_x;
	int m_nPosition;
	const int* m_panResourceIds;
	int m_nActivationMode;
	int m_nFirstResource;
	int m_nLastResource;
	int m_nReserved;
	int* m_pSelection;
	int m_nCommand;
};

GameMenuSelectorManager* LEMBALL_FASTCALL ConstructMenuSelectorManagerThunk(GameMenuSelectorManager* pManager,
																			int nUnusedEdx,
																			void* pRenderOwner,
																			void* pRenderContext,
																			int nChildCapacity,
																			int nLayoutMode);
void LEMBALL_FASTCALL UpdateMenuSelectorAnimation(GameMenuSelectorManager* pManager);
void LEMBALL_FASTCALL UpdateMenuSelectorAnimationThunk(GameMenuSelectorManager* pManager);
void LEMBALL_FASTCALL SetMenuSelectorChildrenEnabled(GameMenuSelectorManager* pManager, int nUnusedEdx, int fEnabled);
void LEMBALL_FASTCALL SetMenuSelectorChildrenEnabledThunk(GameMenuSelectorManager* pManager,
														  int nUnusedEdx,
														  int fEnabled);
void LEMBALL_FASTCALL EmitMenuSelectorResourceEntries(GameMenuSelectorManager* pManager);
void* LEMBALL_FASTCALL ConstructAlignedZrleChildOverlayFromPoint(void* pObject,
																 int nUnusedEdx,
																 const VS_XYPair* pPoint,
																 void* pRenderOwner,
																 int nResourceId,
																 int fAlignment);
void LEMBALL_FASTCALL DestroyAlignedZrleChildOverlay(void* pChildOwner);

void* LEMBALL_FASTCALL DeleteSelectorProgressOverlayChildAdjusted(void* pGeometryInterface,
																  int nUnusedEdx,
																  unsigned int fDelete);
int LEMBALL_FASTCALL HandleQueuedVariantChildSlotManagerEvent(GameQueuedVariantChildSlotManager* pManager,
															  int nUnusedEdx,
															  GameMenuSelectorEvent* pEvent);
// Compatibility shim preserving MSVC 4.20 compiler state.
// DeleteQueuedVariantChildSlotManager
void* LEMBALL_FASTCALL DeleteQueuedVariantChildSlotManager(GameQueuedVariantChildSlotManager* pManager,
														   int nUnusedEdx,
														   int fDelete);
void* LEMBALL_FASTCALL ConstructResourceGeometryChildBase(void* pObject, int nUnusedEdx, void* pRenderOwner);
void LEMBALL_FASTCALL InitializeSharedSpriteOwnerState(void* pObject);
void LEMBALL_FASTCALL RefreshCompactZrleOwnerFromResourceHelper(void* pObject);
// FUNCTION: LEMBALL 0x00468300
// RefreshCompactZrleOwnerIfHelperDirty
void LEMBALL_FASTCALL RefreshCompactZrleOwnerIfHelperDirty(void* pObject);
void LEMBALL_FASTCALL QueueCompactZrleOwnerIfDirty(void* pObject, void* pUnusedEdx, const short* pRect);
void LEMBALL_FASTCALL ComputeAlignedZrleChildLayout(void* pObject);

void* LEMBALL_FASTCALL ConstructQueuedVariantChildSlotManager(void* pManager,
															  int nUnusedEdx,
															  void* pRenderOwner,
															  void* pRenderContext,
															  int nChildCapacity,
															  int nLayoutMode,
															  int nNavigationMode);
void LEMBALL_FASTCALL AppendQueuedVariantChildSlot(void* pManager,
												   int nUnusedEdx,
												   int x,
												   int y,
												   const int* panResourceIds,
												   int nActivationMode,
												   int nFirstResource,
												   int nLastResource,
												   int nReserved,
												   int* pSelection,
												   int nCommand);
void LEMBALL_FASTCALL SelectQueuedVariantChildSlot(void* pManager, int nUnusedEdx, int iChild);
void LEMBALL_FASTCALL InitializeQueuedVariantChildContext(void* pManager);
void LEMBALL_FASTCALL RefreshQueuedVariantChildByToken(void* pManager, int nUnusedEdx, int nToken);
void LEMBALL_FASTCALL RebuildQueuedVariantChildSlotSourceRects(void* pManager, int nUnusedEdx, int fForce);
void LEMBALL_FASTCALL DestroyQueuedVariantChildSlotManager(void* pManager);

#endif
