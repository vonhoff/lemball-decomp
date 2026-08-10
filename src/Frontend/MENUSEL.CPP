#include "Control/CGame.h"
#include "Frontend/GameMenuActionFramePointSink.h"
#include "Frontend/MENUSEL.H"
#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"
#include "Platform/Windows/Mixed/Engine/GDI/VSGDI.H"
#include "Platform/Windows/Mixed/Engine/GDI/VSGEOM.H"
#include "Platform/Windows/Mixed/Level/DRAWTEXT.H"
#include "Platform/Windows/Mixed/Resource/VARMGR.H"
#include "Visos/Generic/CVSRect.h"
#include "Visos/Generic/Memory.h"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <stdlib.h>
#include <string.h>

#pragma comment(linker, "/include:?ConstructProgressEntry@GameMenuSelectorEntry@@QAEPAU1@PBFPAX1HHPBHHHHPAHH@Z")
#pragma comment(linker, "/include:?CopyFrom@GameRectQuad@@QAEPAU1@PBF@Z")
#pragma comment(linker, "/include:?DispatchButtonTransition@GameSelectorProgressGeometry@@QAEXPBUVS_XYPair@@H@Z")
#pragma comment(linker, "/include:?DispatchPointerMove@GameSelectorProgressGeometry@@QAEXPBUVS_XYPair@@@Z")
#pragma comment(linker, "/include:?SetChildPresentForModeA@GameSelectorProgressOverlayOwnerView@@QAEXH@Z")
#pragma comment(linker, "/include:?SetChildPresentForModeB@GameSelectorProgressOverlayOwnerView@@QAEXH@Z")

DECOMP_SIZE_ASSERT(GameMenuSelectorEvent, 0x1c);
DECOMP_SIZE_ASSERT(GameMenuSelectorMetadata, 0x20);
DECOMP_SIZE_ASSERT(GameProgressiveVariantFrameSelector, 0x1c);
DECOMP_SIZE_ASSERT(GameMenuSelectorManager, 0x27c);

extern "C" DWORD WINAPI timeGetTime(void);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void VariantResourceBundleNoopThunk(void);
extern void DestroyVariantRenderManager(void* pObject);
extern void* LEMBALL_FASTCALL InitializeHelperLocalRectUpdateEntry(void* pObject);
extern void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableC(void* pObject);
extern void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableA(void* pObject);
extern void LEMBALL_FASTCALL RestoreStatusIndicatorPointSinkEntryVtable(void* pObject);
extern void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(void* pResourceObject);
extern void* LoadZrleOnlyListResource(int nResourceId);
extern void* LoadListResource(int nResourceId);
extern void* g_pNonZrleVariantRenderEntryInitializeVtable[2];
extern void* g_GAME_RenderQueueNodeVtableSlots[4];
extern void* g_pQueuedRenderPointSinkFinalizeThunk;
extern unsigned int return_hresult_80000801_progress_overlay(void);
extern void* g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage[8];
extern int g_nQueuedVariantChildSlotManagerModeSelectedResourceId;
extern void* LEMBALL_FASTCALL DeleteStatusPointSinkEntryThunk(void* pEntry, int nUnusedEdx, unsigned char fDelete);
extern void LEMBALL_FASTCALL QueueStatusPointPushThunk(void* pEntry, int nUnusedEdx, void* pQueue);
extern void LEMBALL_FASTCALL RemoveGeometryOwnerChild(void* pOwner, int nUnusedEdx, void* pChildOwner);
extern void LEMBALL_FASTCALL DestroyWindowOwnerRenderContext(void* pObject);
extern void LEMBALL_FASTCALL ReleasePauseDialogPaletteRemaps(void* pObject);
extern void LEMBALL_FASTCALL ResetBaseModeActionButtonPointSink(void* pObject);
extern void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableABody(void* pObject);
extern void* LEMBALL_FASTCALL InitializePackagedSpriteRenderEntry(void* pObject);
extern void* LEMBALL_FASTCALL InitializeMenuActionFramePointSink(void* pObject);
extern void LEMBALL_FASTCALL configure_level_screen_pause_dialog_options(void* pObject);
extern void LEMBALL_FASTCALL RebuildLevelScreenPauseDialogLayout(void* pObject);
extern int LEMBALL_FASTCALL SelectPauseDialogPaletteRemapForIndex(void* pObject, int nUnusedEdx, int nIndex);
extern void LEMBALL_FASTCALL CTextManagerResetPrimitives(void* pTable);
extern void* LEMBALL_FASTCALL InitializeEmptyGeometryOwnerRect(void* pOwner);
extern void LEMBALL_FASTCALL RestoreQueuedRenderPointSinkVtable(void* pObject);
extern void LEMBALL_FASTCALL AppendGeometryOwnerChild(void* pOwner, int nUnusedEdx, void* pChildOwner);
extern void* g_pVariantResourceEntryManager;
extern int g_fLevelSelectionChanged;
extern int LEMBALL_FASTCALL GetTimedVariantFrameStartTimeThunk(void* pObject);
extern void LEMBALL_FASTCALL StartRepeatAnim(void* pObject, int nUnusedEdx, int nFrameDuration);
extern void LEMBALL_FASTCALL set_object_field_0x10_00435890(void* pObject, void* pUnusedEdx, int nValue);
extern void LEMBALL_FASTCALL set_object_field_0x0c_004358a0(void* pObject, void* pUnusedEdx, int nValue);
extern void LEMBALL_FASTCALL set_object_field_0x18_004358b0(void* pObject, void* pUnusedEdx, int nValue);
extern int LEMBALL_FASTCALL return_object_field_0x0c_00435880(void* pObject);
extern void* LEMBALL_FASTCALL InitializeGeometryOwnerRect(void* pObject, int nUnusedEdx, const VS_XYPair* pRect);
extern void* LEMBALL_FASTCALL InitializePackagedSpriteRenderEntryThunk(void* pObject);
extern void* LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntryThunk(void* pObject);
extern void LEMBALL_FASTCALL SetWindowOwnerRenderAnchorFromPoint(void* pOwner, int nUnusedEdx, const VS_XYPair* pPoint);
extern void LEMBALL_FASTCALL NotifySpriteOwnerVariantCEvent0D(void* pObject);
extern void LEMBALL_FASTCALL NotifySpriteOwnerVariantCEvent0E(void* pObject);
extern void LEMBALL_FASTCALL SetAutoDraw(void* pOwner, void* pUnusedEdx, int nValue);
void LEMBALL_FASTCALL DetachGeometryChildAndDestroyWindowOwner(void* pObject);
int GetIntDirectionSign(int nValue);

class GameSpriteOwnerVariantC {
public:
	int MapSpriteOwnerEventToFrameSlot(int nEvent);
	void NotifyFrameEventC(int nEvent);
	void NotifyFrameEventB(int nEvent);
};
void LEMBALL_FASTCALL RefreshCompactZrleOwnerAfterFrameChange(void* pObject);
void LEMBALL_FASTCALL HandleSpriteOwnerPointerEnter(void* pGeometryOwner);
void LEMBALL_FASTCALL HandleSpriteOwnerPointerLeave(void* pGeometryOwner);
void LEMBALL_FASTCALL HandleSpriteOwnerButtonTransition(void* pGeometryOwner,
														int nUnused,
														const VS_XYPair* pPoint,
														int nButton);
void LEMBALL_FASTCALL HandleSpriteOwnerButtonInside(void* pGeometryOwner,
													int nUnused,
													const VS_XYPair* pPoint,
													int nButton);
void LEMBALL_FASTCALL ResetSpriteOwnerButtonState(void* pGeometryOwner,
												  int nUnused,
												  const VS_XYPair* pPoint,
												  int nButton);
void* LEMBALL_FASTCALL InitializeTinyPointSinkHelper(void* pObject);

struct GameMenuSelectorEntry {
	GameMenuSelectorEntry(void* pRenderOwner,
						  void* pRenderContext,
						  int x,
						  int nPosition,
						  const int* panResourceIds,
						  int nActivationMode,
						  int nFirstResource,
						  int nLastResource,
						  int nReserved,
						  int nToken,
						  int* pSelection,
						  int nCommand);
	GameMenuSelectorEntry* ConstructProgressEntry(const short* paProgressRect,
												  void* pRenderOwner,
												  void* pRenderContext,
												  int x,
												  int nPosition,
												  const int* panResourceIds,
												  int nActivationMode,
												  int nSelectedResource,
												  int nToken,
												  int* pSelection,
												  int nCommand);
	void BuildOverlay(const int* panResourceIds);
	int HandleEvent(GameMenuSelectorEvent* pEvent);
	void RebuildRects(int fForce, int nUnused);

	void** m_pVtable00; // 0x00
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
	void* m_pRenderContext24;
	void* m_pRenderOwner28;
	int m_nActivationMode2C;
	int m_nToken30;
	int m_nCommand34;
	int m_cResources38;
	int m_nLastResource3C;
	int m_nFirstResource40;
	int m_nSelectedResource44;
	int* m_pSelection48;
	const int* m_panResourceIds4C;
	void* m_pAlignedOverlay50;
	void* m_pProgressOverlay54;
	void** m_ppLoadedResources58;
	int m_fEnabled5C;
};

typedef char GameMenuSelectorEntry_size_must_be_0x60[sizeof(GameMenuSelectorEntry) == 0x60 ? 1 : -1];

typedef int(LEMBALL_FASTCALL* GameFrameSelectorIndexProc)(GameProgressiveVariantFrameSelector* pSelector);
class GameProgressiveVariantFrameSelectorInterface {
public:
	virtual int SelectFrame(void) = 0;
	virtual void SetDuration(int nDuration) = 0;
	virtual void SetDurationField(int nDuration) = 0;
	virtual void SetStartTime(int dwStartTime) = 0;
	virtual void SetDirection(int nDirection) = 0;
};

class GameCompactZrleOverlay {
public:
	void RebuildSourceRect(int fForce);
	void ReloadResource(int nResourceId);
};

class GameSelectorProgressOverlayOwner : public GameCompactZrleOverlay {
public:
	GameSelectorProgressOverlayOwner* Construct(const VS_XYPair* pPoint,
												void* pRenderOwner,
												int nResourceId,
												const short* pProgressRect,
												int nPercent);
	void SetAnchorAndNotifyChild(const VS_XYPair* pPoint);
};

class GameSelectorProgressOverlayOwnerView {
public:
	void SetChildPresentForModeA(int nMode);
	void SetChildPresentForModeB(int nMode);
};

struct GameRectQuad {
	short m_cx;
	short m_cy;
	short m_x;
	short m_y;

	GameRectQuad* CopyFrom(const short* pRect);
};

// SIZE 0x38
class GameSelectorProgressGeometry : public GameGeometryOwnerRectState {
public:
	void DispatchButtonTransition(const VS_XYPair* pPoint, int nButton);
	void DispatchPointerMove(const VS_XYPair* pPoint);
};

// SIZE 0x10
struct GameProgressRenderEntry {
	void** m_pVtable00;  // 0x00
	short m_cx04;        // 0x04
	short m_cy06;        // 0x06
	short m_x08;         // 0x08
	short m_y0A;         // 0x0a
	int m_nResourceId0C; // 0x0c
};

// These entries replace inherited GameWindowOwnerRenderContext slots in the
// original hand-built table. Keeping the slot owners in a non-virtual view
// makes address-of-member resolve to each concrete body instead of an
// MSVC-generated vcall stub; the view receives the same this pointer.
class GameSelectorProgressOverlayWindowSlots {
public:
	void InitializeWindowOwnerFromRect(short* pRect, void* pOwner, const char* pszWindowName);
	void MoveWindowOwnerOrigin(const VS_XYPair* pPoint);
	void ReservedWindowOwnerSlot15(void);
	void ReservedWindowOwnerSlot23(void);
	void ApplyWindowOwnerActivation(int nActivate);
};

// SIZE 0x134
class GameSelectorProgressOverlayChild : public GameWindowOwnerRenderContext {
public:
	GameSelectorProgressOverlayChild* Construct(const short* pRect, int nPercent, void* pRenderOwner);
	void SetPercent(int nPercent);

public:
	GameSelectorProgressGeometry m_Geometry90;     // 0x90
	GameRectQuad m_RectC8;                         // 0xc8
	GameProgressRenderEntry m_aRenderEntriesD0[5]; // 0xd0
	int m_nPercent120;                             // 0x120
	int m_cxProgress124;                           // 0x124
	int m_nReserved128;                            // 0x128
	void* m_pRenderOwner12C;                       // 0x12c
	int m_nToken130;                               // 0x130
};

DECOMP_SIZE_ASSERT(GameProgressRenderEntry, 0x10);
DECOMP_SIZE_ASSERT(GameSelectorProgressGeometry, 0x38);
DECOMP_SIZE_ASSERT(GameSelectorProgressOverlayChild, 0x134);

class GameProgressRenderEntryInterface {
public:
	virtual void Delete(unsigned int fDelete) = 0;
	virtual void Queue(void* pQueue) = 0;
};

class GameSelectorProgressGeometryDispatchInterface {
public:
	virtual void Reserved0(void) = 0;
	virtual void Reserved1(void) = 0;
	virtual void Reserved2(void) = 0;
	virtual void Reserved3(void) = 0;
	virtual void Reserved4(void) = 0;
	virtual void Reserved5(void) = 0;
	virtual void DispatchPointerMove(const VS_XYPair* pPoint) = 0;
};

class GameCompactZrleOverlayGeometryInterface {
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
	virtual void Reserved24(void) = 0;
	virtual void Reserved25(void) = 0;
	virtual void Reserved26(void) = 0;
	virtual void Reserved27(void) = 0;
	virtual void Reserved28(void) = 0;
	virtual void Reserved29(void) = 0;
	virtual void Reserved30(void) = 0;
	virtual void Reserved31(void) = 0;
	virtual void Reserved32(void) = 0;
	virtual void Reserved33(void) = 0;
	virtual void Reserved34(void) = 0;
	virtual void Reserved35(void) = 0;
	virtual void Reserved36(void) = 0;
	virtual void Reserved37(void) = 0;
	virtual void Reserved38(void) = 0;
	virtual void Reserved39(void) = 0;
	virtual void Reserved40(void) = 0;
	virtual void Reserved41(void) = 0;
	virtual void SetSourceRect(short* pRect) = 0;
};

typedef int(LEMBALL_FASTCALL* GameMenuSelectorEventProc)(GameMenuSelectorManager* pManager,
														 int nUnusedEdx,
														 GameMenuSelectorEvent* pEvent);
typedef void*(LEMBALL_FASTCALL* GameMenuSelectorDeleteProc)(GameMenuSelectorManager* pManager,
															int nUnusedEdx,
															int fDelete);

void LEMBALL_FASTCALL DestroyMenuSelectorManager(GameMenuSelectorManager* pManager);
void* LEMBALL_FASTCALL DeleteMenuSelectorManager(GameMenuSelectorManager* pManager, int nUnusedEdx, int fDelete);
static void* LEMBALL_FASTCALL DeleteMenuSelectorManagerThunk(GameMenuSelectorManager* pManager,
															 int nUnusedEdx,
															 int fDelete);
static int LEMBALL_FASTCALL HandleMenuSelectorEventThunk(GameMenuSelectorManager* pManager,
														 int nUnusedEdx,
														 GameMenuSelectorEvent* pEvent);
static void LEMBALL_FASTCALL MoveMenuSelectorPreviousThunk(GameMenuSelectorManager* pManager);
static void LEMBALL_FASTCALL MoveMenuSelectorNextThunk(GameMenuSelectorManager* pManager);
static void LEMBALL_FASTCALL SelectMenuSelectorSide0Thunk(GameMenuSelectorManager* pManager);
static void LEMBALL_FASTCALL SelectMenuSelectorSide1Thunk(GameMenuSelectorManager* pManager);
static void LEMBALL_FASTCALL StartMenuSelectorActionThunk(GameMenuSelectorManager* pManager);
void LEMBALL_FASTCALL ReleaseMenuSelectorEntryResources(GameMenuSelectorEntry* pEntry);
void LEMBALL_FASTCALL DestroyMenuSelectorEntry(GameMenuSelectorEntry* pEntry);
void* LEMBALL_FASTCALL DeleteMenuSelectorEntry(GameMenuSelectorEntry* pEntry, int nUnusedEdx, int fDelete);
static void* LEMBALL_FASTCALL delete_menu_selector_entry_thunk(GameMenuSelectorEntry* pEntry,
															   int nUnusedEdx,
															   int fDelete);
static int LEMBALL_FASTCALL handle_menu_selector_entry_event_thunk(GameMenuSelectorEntry* pEntry,
																   int nUnusedEdx,
																   GameMenuSelectorEvent* pEvent);
void LEMBALL_FASTCALL InitializeSharedSpriteOwnerState(void* pObject);
void LEMBALL_FASTCALL ComputeAlignedZrleChildLayout(void* pObject);

// GLOBAL: LEMBALL 0x004a7b44
int g_nMenuSelectorResourceId0;
// GLOBAL: LEMBALL 0x004a7b3c
int g_nMenuSelectorResourceId1;
// GLOBAL: LEMBALL 0x004a7b40
int g_nMenuSelectorResourceId2;
// GLOBAL: LEMBALL 0x004a7b50
int g_nMenuSelectorResourceId3;
// GLOBAL: LEMBALL 0x004a7b38
int g_nMenuSelectorResourceId4;
// GLOBAL: LEMBALL 0x004a7b48
int g_nMenuSelectorResourceId5;
// GLOBAL: LEMBALL 0x004a7b54
int g_nMenuSelectorResourceId6;
// GLOBAL: LEMBALL 0x004a7b58
int g_nMenuSelectorResourceId7;
// GLOBAL: LEMBALL 0x004a7b4c
int g_nMenuSelectorResourceId8;

// GLOBAL: LEMBALL 0x0049fa70
int g_anMenuSelectorActionOffsetsMode0[18] = {0, 12, -8, -2, 52, -2, -13, 28, 116, 0, 0, -13, 19, 28, -16, 0, -24, -2};
// GLOBAL: LEMBALL 0x0049fab8
int g_anMenuSelectorActionOffsetsMode1[18] = {0, 6, -4, -2, 26, -2, -7, 14, 58, 0, 0, 0, 10, 14, -8, 0, -12, -2};

int LEMBALL_FASTCALL GetMenuSelectorStartTimeThunk(GameProgressiveVariantFrameSelector* pSelector)
{
	return return_object_field_0x0c_00435880(pSelector);
}

int LEMBALL_FASTCALL GetTimedAnimFrameNo(GameProgressiveVariantFrameSelector* pSelector);
void LEMBALL_FASTCALL StartPlayThruAnim(GameProgressiveVariantFrameSelector* pSelector, int, int dwDuration);
int LEMBALL_FASTCALL GetPlayThruAnimFrameNo(GameProgressiveVariantFrameSelector* pSelector);

static void LEMBALL_FASTCALL SetMenuSelectorDurationFieldThunk(GameProgressiveVariantFrameSelector* pSelector,
															   int,
															   int nValue)
{
	set_object_field_0x10_00435890(pSelector, 0, nValue);
}

static void LEMBALL_FASTCALL SetMenuSelectorStartTimeFieldThunk(GameProgressiveVariantFrameSelector* pSelector,
																int,
																int nValue)
{
	set_object_field_0x0c_004358a0(pSelector, 0, nValue);
}

static void LEMBALL_FASTCALL SetMenuSelectorDirectionFieldThunk(GameProgressiveVariantFrameSelector* pSelector,
																int,
																int nValue)
{
	set_object_field_0x18_004358b0(pSelector, 0, nValue);
}

/* VTABLE: LEMBALL 0x00496D00 */
void* g_MenuSelectorFrameSelectorCtorVtable[2] = {(void*) _purecall, (void*) GetMenuSelectorStartTimeThunk};

/* VTABLE: LEMBALL 0x00496D08 */
void* g_MenuSelectorTimedFrameSelectorVtable[6] = {(void*) GetTimedAnimFrameNo,
												   (void*) _purecall,
												   (void*) SetMenuSelectorDurationFieldThunk,
												   (void*) SetMenuSelectorStartTimeFieldThunk,
												   (void*) SetMenuSelectorDirectionFieldThunk,
												   0};

/* VTABLE: LEMBALL 0x00496D20 */
void* g_MenuSelectorProgressiveFrameSelectorVtable[6] = {(void*) GetPlayThruAnimFrameNo,
														 (void*) StartPlayThruAnim,
														 (void*) SetMenuSelectorDurationFieldThunk,
														 (void*) SetMenuSelectorStartTimeFieldThunk,
														 (void*) SetMenuSelectorDirectionFieldThunk,
														 0};

/* VTABLE: LEMBALL 0x00496CE4 */
void* g_MenuSelectorCurrentFrameCallbackVtable[7] = {(void*) GetTimedVariantFrameStartTimeThunk,
													 (void*) GetTimedAnimFrameNo,
													 (void*) StartRepeatAnim,
													 (void*) SetMenuSelectorDurationFieldThunk,
													 (void*) SetMenuSelectorStartTimeFieldThunk,
													 (void*) SetMenuSelectorDirectionFieldThunk,
													 0};

/* VTABLE: LEMBALL 0x00497F0C */
static void* g_MenuSelectorVariantRenderManagerVtable[1] = {(void*) VariantResourceBundleNoopThunk};

/* VTABLE: LEMBALL 0x00497F10 */
static void* g_MenuSelectorManagerVtable[3] = {(void*) InitializeNonZrleVariantRenderEntry,
											   (void*) DeleteMenuSelectorManagerThunk,
											   (void*) HandleMenuSelectorEventThunk};

class GameResourceGeometryChildView {
public:
	void SetRenderAnchorWithOriginCompensation(const VS_XYPair* pPoint);
	void NoopWindowOwnerSlot42(int nValue);
};

class GameResourceGeometryChildDeleteView {
public:
	void* Delete(unsigned int fDelete);
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
	void* Delete(unsigned int fDelete);
};

class GameMenuSelectorChildContextView {
public:
	GameMenuSelectorChildContextView* ConstructSelectorManagerChildContextObject(void);
	void MarkRenderObjectDirty(void);
};

class GameQueuedVariantChildContextView {
public:
	void MarkRenderObjectDirty(void);
};

typedef char GameMenuActionFramePointSink_size_must_be_0x20[sizeof(GameMenuActionFramePointSink) == 0x20 ? 1 : -1];

/* VTABLE: LEMBALL 0x00497CF8 */
// GLOBAL: LEMBALL 0x00497cf8
static void* g_MenuSelectorEntryVtable[3] = {(void*) InitializeNonZrleVariantRenderEntry,
											 (void*) delete_menu_selector_entry_thunk,
											 (void*) handle_menu_selector_entry_event_thunk};

extern void* g_VSGDI_StatusIndicatorPointSinkVtable[12];

struct GameMenuActionFramePointSinkVtableData {
	void* (GameMenuActionFramePointSink::*m_pDelete)(unsigned int);
	void (GameMenuActionFramePointSink::*m_pBindToContext)(void*);
	void (GameMenuActionFramePointSink::*m_pDispatchToRenderObject)(void*);
};

static GameMenuActionFramePointSinkVtableData g_MenuActionFramePointSinkVtableCompilerStateShim = {
	&GameMenuActionFramePointSink::Delete,
	&GameMenuActionFramePointSink::BindToContext,
	&GameMenuActionFramePointSink::DispatchToRenderObject,
};

static void* g_ResourceGeometryChildVtable[52];
static void* g_ResourceGeometryChildDeleteVtable[8];
static void** g_pWindowOwnerRenderContextVtable;
static int g_fResourceGeometryChildVtablesInitialized;
static void* g_AlignedZrleChildOverlayVtable[52];
static void* g_AlignedZrleChildOverlayDeleteVtable[8];
static int g_fAlignedZrleChildOverlayVtablesInitialized;
static void* g_SelectorProgressOverlayOwnerVtable[52];
static void* g_SelectorProgressOverlayOwnerDeleteVtable[8];
static int g_fSelectorProgressOverlayOwnerVtablesInitialized;
static void* g_SelectorProgressOverlayChildVtable[46];
static void* g_SelectorProgressGeometryVtable[8];
static void** g_pSelectorProgressBaseWindowOwnerVtable;
static int g_fSelectorProgressOverlayChildVtablesInitialized;
static void* g_MenuSelectorChildContextVtable[46];
static int g_fMenuSelectorChildContextVtableInitialized;
static void* g_QueuedVariantChildContextVtable[46];
static int g_fQueuedVariantChildContextVtableInitialized;

void LEMBALL_FASTCALL DestroySelectorProgressOverlayOwner(void* pDeleteInterface);
void* LEMBALL_FASTCALL DeleteSelectorProgressOverlayOwnerAdjusted(void* pDeleteInterface,
																  int nUnusedEdx,
																  unsigned int fDelete);
void LEMBALL_FASTCALL DestroySelectorProgressOverlayChild(void* pGeometryInterface);
void* LEMBALL_FASTCALL DeleteSelectorProgressOverlayChildAdjusted(void* pGeometryInterface,
																  int nUnusedEdx,
																  unsigned int fDelete);

__inline static void StoreSpriteOwnerLocalPoint(void* pGeometryOwner, const VS_XYPair* pPoint)
{
	VS_XYPair Origin = *(VS_XYPair*) ((char*) pGeometryOwner - 0x78);

	*(short*) ((char*) pGeometryOwner + 0x70) = (short) (pPoint->m_x - Origin.m_x);
	*(short*) ((char*) pGeometryOwner + 0x72) = (short) (pPoint->m_y - Origin.m_y);
}

inline static int ResolveResourceGeometryRenderObject(void* pObject)
{
	int nContext;

	nContext = *(int*) (*(int*) ((char*) pObject + 0x4c) + 0x0c);
	return *(int*) (*(int*) (nContext + 0x40) + 4) + 0x40 + nContext;
}

struct GameResourceGeometryChildContext {
	virtual void ReservedContextSlot0(void) = 0;
	virtual void BindResourceContext(void* pContext) = 0;
};

void LEMBALL_FASTCALL RefreshCompactZrleOwnerIfHelperDirty(void* pObject)
{
	typedef int(LEMBALL_FASTCALL * QueryProc)(void* pObject);
	int nRenderObject;

	if (*(int*) ((char*) pObject + 0x104) == *(int*) ((char*) pObject + 0x108)) {
		nRenderObject = ResolveResourceGeometryRenderObject(pObject);
		if (((QueryProc) (*(void***) nRenderObject)[15])((void*) nRenderObject) == 0) {
			goto refresh;
		}
	}
	nRenderObject = ResolveResourceGeometryRenderObject(pObject);
	*(int*) (nRenderObject + 0x38) = 1;
	*(int*) ((char*) pObject + 0x108) = *(int*) ((char*) pObject + 0x104);
refresh:
	RefreshCompactZrleOwnerFromResourceHelper(pObject);
}

__inline static void InitializeResourceGeometryChildVtables(void** pWindowOwnerVtable, void** pGeometryOwnerVtable)
{
	union PointMethod {
		void (GameResourceGeometryChildView::*p)(const VS_XYPair*);
		void* v;
	} Point;
	union DeleteMethod {
		void* (GameResourceGeometryChildDeleteView::*p)(unsigned int);
		void* v;
	} Delete;
	union IntMethod {
		void (GameResourceGeometryChildView::*p)(int);
		void* v;
	} Int;
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

	Point.p = &GameResourceGeometryChildView::SetRenderAnchorWithOriginCompensation;
	g_ResourceGeometryChildVtable[14] = Point.v;
	g_ResourceGeometryChildVtable[29] = (void*) DetachGeometryChildAndDestroyWindowOwner;
	Int.p = &GameResourceGeometryChildView::NoopWindowOwnerSlot42;
	g_ResourceGeometryChildVtable[42] = Int.v;
	g_ResourceGeometryChildVtable[46] = (void*) RefreshCompactZrleOwnerAfterFrameChange;
	g_ResourceGeometryChildVtable[47] = (void*) _purecall;
	g_ResourceGeometryChildVtable[48] = (void*) _purecall;
	g_ResourceGeometryChildVtable[49] = (void*) _purecall;
	g_ResourceGeometryChildVtable[50] = (void*) _purecall;
	g_ResourceGeometryChildVtable[51] = (void*) _purecall;

	Delete.p = &GameResourceGeometryChildDeleteView::Delete;
	g_ResourceGeometryChildDeleteVtable[0] = Delete.v;
	g_ResourceGeometryChildDeleteVtable[1] = (void*) HandleSpriteOwnerButtonTransition;
	g_ResourceGeometryChildDeleteVtable[2] = (void*) HandleSpriteOwnerButtonInside;
	g_ResourceGeometryChildDeleteVtable[3] = (void*) ResetSpriteOwnerButtonState;
	g_ResourceGeometryChildDeleteVtable[4] = (void*) HandleSpriteOwnerPointerEnter;
	g_ResourceGeometryChildDeleteVtable[5] = (void*) HandleSpriteOwnerPointerLeave;
	g_fResourceGeometryChildVtablesInitialized = 1;
}

__inline static void InitializeAlignedZrleChildOverlayVtables(void)
{
	union VoidMethod {
		void (GameAlignedZrleChildOverlayView::*p)(void);
		void* v;
	} Void;
	union DeleteMethod {
		void* (GameAlignedZrleChildOverlayDeleteView::*p)(unsigned int);
		void* v;
	} Delete;
	union IntMethod {
		void (GameSelectorProgressOverlayOwnerView::*p)(int);
		void* v;
	} Int;
	int i;

	if (g_fAlignedZrleChildOverlayVtablesInitialized != 0) {
		return;
	}
	for (i = 0; i < 52; ++i) {
		g_AlignedZrleChildOverlayVtable[i] = g_ResourceGeometryChildVtable[i];
	}
	for (i = 0; i < 8; ++i) {
		g_AlignedZrleChildOverlayDeleteVtable[i] = g_ResourceGeometryChildDeleteVtable[i];
	}
	Void.p = &GameAlignedZrleChildOverlayView::ReleaseFrameResource;
	g_AlignedZrleChildOverlayVtable[16] = Void.v;
	g_AlignedZrleChildOverlayVtable[42] = (void*) QueueCompactZrleOwnerIfDirty;
	g_AlignedZrleChildOverlayVtable[46] = (void*) RefreshCompactZrleOwnerIfHelperDirty;
	Void.p = &GameAlignedZrleChildOverlayView::QueueAlignedFrame;
	g_AlignedZrleChildOverlayVtable[47] = Void.v;
	Int.p = &GameSelectorProgressOverlayOwnerView::SetChildPresentForModeA;
	g_AlignedZrleChildOverlayVtable[48] = Int.v;
	Int.p = &GameSelectorProgressOverlayOwnerView::SetChildPresentForModeB;
	g_AlignedZrleChildOverlayVtable[49] = Int.v;
	Void.p = &GameAlignedZrleChildOverlayView::HandlePointerEnter;
	g_AlignedZrleChildOverlayVtable[50] = Void.v;
	Void.p = &GameAlignedZrleChildOverlayView::HandlePointerLeave;
	g_AlignedZrleChildOverlayVtable[51] = Void.v;
	g_AlignedZrleChildOverlayDeleteVtable[1] = (void*) HandleSpriteOwnerButtonTransition;
	g_AlignedZrleChildOverlayDeleteVtable[2] = (void*) HandleSpriteOwnerButtonInside;
	g_AlignedZrleChildOverlayDeleteVtable[3] = (void*) ResetSpriteOwnerButtonState;
	g_AlignedZrleChildOverlayDeleteVtable[4] = (void*) HandleSpriteOwnerPointerEnter;
	g_AlignedZrleChildOverlayDeleteVtable[5] = (void*) HandleSpriteOwnerPointerLeave;
	Delete.p = &GameAlignedZrleChildOverlayDeleteView::Delete;
	g_AlignedZrleChildOverlayDeleteVtable[0] = Delete.v;
	g_fAlignedZrleChildOverlayVtablesInitialized = 1;
}

__inline static void* ConstructMenuSelectorChildContextObject(void* pObject)
{
	union VoidMethod {
		void (GameMenuSelectorChildContextView::*p)(void);
		void* v;
	} Void;
	void** pBaseVtable;
	int i;

	((GameWindowOwnerRenderContext*) pObject)->ConstructWindowOwnerRenderContext();
	pBaseVtable = *(void***) pObject;
	if (g_fMenuSelectorChildContextVtableInitialized == 0) {
		for (i = 0; i < 46; ++i) {
			g_MenuSelectorChildContextVtable[i] = pBaseVtable[i];
		}
		Void.p = &GameMenuSelectorChildContextView::MarkRenderObjectDirty;
		g_MenuSelectorChildContextVtable[15] = Void.v;
		g_fMenuSelectorChildContextVtableInitialized = 1;
	}
	*(void***) pObject = g_MenuSelectorChildContextVtable;
	*(int*) ((char*) pObject + 0x50) += 5;
	return pObject;
}

// LINKERILT: LEMBALL 0x00401005
void LEMBALL_FASTCALL destroy_queued_variant_child_slot_manager(void* pObject)
{
	DestroyQueuedVariantChildSlotManager((GameQueuedVariantChildSlotManager*) pObject);
}

struct GameQueuedVariantRangeEntry {
	void RebuildSourceRect(int fForce);
	GameQueuedVariantRangeEntry* Construct(void* pRenderOwner,
										   void* pRenderContext,
										   int x,
										   int y,
										   int nResourceId,
										   int nActivationMode,
										   int nFirstResource,
										   int nLastResource,
										   int nReserved,
										   int nToken,
										   int* pSelection,
										   int nCommand);

	void** m_pVtable00;
	int m_nQueueTag04;
	int m_nQueueReserved08;
	int m_cUnhandledEvents0C;
	short m_nReserved10;
	short m_nReserved12;
	short m_nReserved14;
	short m_nReserved16;
	int m_x18;
	int m_y1C;
	void* m_pRenderContext20;
	void* m_pRenderOwner24;
	int m_nActivationMode28;
	int m_nToken2C;
	int m_nCommand30;
	int m_cResources34;
	int m_nLastResource38;
	int m_nFirstResource3C;
	int m_nSelectedResource40;
	int* m_pSelection44;
	const int* m_panResourceIds48;
	void* m_pOverlay4C;
	void** m_ppLoadedResources50;
	int m_fEnabled54;
};

struct GameQueuedVariantSlotRecord {
	int m_fActive00;
	int m_x04;
	int m_y08;
	int m_nToken0C;
};

struct GameQueuedVariantChildSlotManager {
	void SelectPreviousChildSlot(void);
	void SelectNextChildSlot(void);
	void SelectChildSlot(int iChild);
	void DispatchSelectedChildSlotEvent(void);
	void RefreshAllRangeSelections(void);
	void RefreshChildByToken(int nToken);
	void RebuildChildSourceRects(int fForce);

	void** m_pVtable00;
	int m_nQueueTag04;
	int m_nQueueReserved08;
	int m_cUnhandledEvents0C;
	unsigned char m_abVariantRenderManager10[0x70];
	int m_cChildren80;
	GameMenuSelectorEvent m_ActionEvent84;
	int m_nCurrentChildA0;
	int m_nReservedA4;
	int m_iSelectedChildA8;
	GameQueuedVariantRangeEntry* m_apChildrenAC[4];
	GameQueuedVariantSlotRecord m_aSlotsBC[4];
	unsigned char m_abPointSinkFC[0x10];
	void* m_pRenderContext10C;
	void* m_pRenderOwner110;
	int m_nNextToken114;
	unsigned char m_abCallback118[0x0c];
	int m_nReserved124;
	void* m_pChildContext128;
	void* m_pChildRenderContext12C;
	int m_nLayoutMode130;
	int m_nNavigationMode134;
	int m_fCompactLayout138;
	int m_dwSelectionStart13C;
	int m_dwSelectionEnd140;
	int m_fInputEnabled144;
	int m_fChildrenEnabled148;
};

typedef char GameQueuedVariantRangeEntry_size_must_be_0x58[sizeof(GameQueuedVariantRangeEntry) == 0x58 ? 1 : -1];
typedef char
	GameQueuedVariantChildSlotManager_size_must_be_0x14c[sizeof(GameQueuedVariantChildSlotManager) == 0x14c ? 1 : -1];

int LEMBALL_FASTCALL HandleQueuedVariantRangeEntryEvent(GameQueuedVariantRangeEntry* pEntry,
														int,
														GameMenuSelectorEvent* pEvent);
int LEMBALL_FASTCALL HandleQueuedVariantChildSlotManagerEvent(GameQueuedVariantChildSlotManager* pManager,
															  int,
															  GameMenuSelectorEvent* pEvent);
void* LEMBALL_FASTCALL DeleteQueuedVariantRangeEntry(GameQueuedVariantRangeEntry* pEntry, int, int fDelete);
void* LEMBALL_FASTCALL DeleteQueuedVariantRangeEntryAuto(void* pObject, void*, BYTE fDelete);
void* LEMBALL_FASTCALL DeleteQueuedVariantChildSlotManager(GameQueuedVariantChildSlotManager* pManager,
														   int,
														   int fDelete);

// GLOBAL: LEMBALL 0x00498150
static void* g_QueuedVariantRangeEntryVtable[3] = {(void*) InitializeNonZrleVariantRenderEntry,
												   (void*) DeleteQueuedVariantRangeEntryAuto,
												   (void*) HandleQueuedVariantRangeEntryEvent};
/* VTABLE: LEMBALL 0x0049823C */
static void* g_QueuedVariantChildSlotManagerRenderManagerVtable[1] = {(void*) VariantResourceBundleNoopThunk};
/* VTABLE: LEMBALL 0x00498240 */
static void* g_QueuedVariantChildSlotManagerVtable[3] = {(void*) InitializeNonZrleVariantRenderEntry,
														 (void*) DeleteQueuedVariantChildSlotManager,
														 (void*) HandleQueuedVariantChildSlotManagerEvent};

__inline static void QueueQueuedVariantCommand(int nCommand)
{
	typedef void(LEMBALL_FASTCALL * QueueProc)(void*, int, void*);
	GameMenuSelectorEvent Event;

	Event.m_nType00 = 0x0c;
	Event.m_nReserved02 = 0;
	Event.m_dwTimestamp04 = (int) timeGetTime();
	Event.m_nPayload08 = nCommand;
	Event.m_nPayload0C = 0;
	Event.m_nPayload10 = 0;
	Event.m_nPayload14 = 0;
	Event.m_nPayload18 = 0;
	((QueueProc) (*(void***) g_pSharedRenderDispatchQueue)[2])(g_pSharedRenderDispatchQueue, 0, &Event);
}

void LEMBALL_FASTCALL ReleaseQueuedVariantRangeResources(GameQueuedVariantRangeEntry* pEntry);
void LEMBALL_FASTCALL DestroyQueuedVariantRangeEntryAuto(void* pObject);

__inline static GameQueuedVariantRangeEntry* ConstructQueuedVariantRangeEntry(GameQueuedVariantRangeEntry* pEntry,
																			  void* pRenderOwner,
																			  void* pRenderContext,
																			  int x,
																			  int y,
																			  const int* panResourceIds,
																			  int nActivationMode,
																			  int nFirstResource,
																			  int nLastResource,
																			  int nToken,
																			  int* pSelection,
																			  int nCommand)
{
	VS_XYPair Point;
	int i;

	InitializeRenderQueueNodeBase(pEntry);
	pEntry->m_pVtable00 = g_QueuedVariantRangeEntryVtable;
	pEntry->m_nReserved10 = 0;
	pEntry->m_nReserved12 = 0;
	pEntry->m_nReserved14 = 0;
	pEntry->m_nReserved16 = 0;
	pEntry->m_x18 = x;
	pEntry->m_y1C = y;
	pEntry->m_pRenderContext20 = pRenderContext;
	pEntry->m_pRenderOwner24 = pRenderOwner;
	pEntry->m_nActivationMode28 = nActivationMode;
	pEntry->m_nToken2C = nToken;
	pEntry->m_nCommand30 = nCommand;
	pEntry->m_cResources34 = nLastResource - nFirstResource + 1;
	if (pEntry->m_cResources34 < 2) {
		pEntry->m_cResources34 = 1;
	}
	pEntry->m_nLastResource38 = nLastResource;
	pEntry->m_nFirstResource3C = nFirstResource;
	pEntry->m_nSelectedResource40 = pSelection == 0 ? 0 : *pSelection;
	pEntry->m_pSelection44 = pSelection;
	pEntry->m_panResourceIds48 = panResourceIds;
	pEntry->m_fEnabled54 = 1;
	pEntry->m_ppLoadedResources50 = (void**) AllocateVSMemBlock(pEntry->m_cResources34 * sizeof(void*));
	for (i = 0; i < pEntry->m_cResources34; ++i) {
		pEntry->m_ppLoadedResources50[i] = LoadZrleOnlyListResource(panResourceIds[i]);
	}
	pEntry->m_pOverlay4C = AllocateVSMemBlock(0x130);
	if (pEntry->m_pOverlay4C != 0) {
		Point.m_x = (short) x;
		Point.m_y = (short) y;
		ConstructAlignedZrleChildOverlayFromPoint(pEntry->m_pOverlay4C,
												  0,
												  &Point,
												  pRenderOwner,
												  panResourceIds[pEntry->m_nSelectedResource40 - nFirstResource],
												  3);
		*(int*) ((char*) pEntry->m_pOverlay4C + 0xf8) = 0;
		*(void**) ((char*) pEntry->m_pOverlay4C + 0xf4) = g_pSharedRenderDispatchQueue;
		*(int*) ((char*) pEntry->m_pOverlay4C + 0xcc) = nToken;
	}
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->RegisterOrderedRenderDispatchClient(pEntry, 0);
	return pEntry;
}

static void LEMBALL_FASTCALL RefreshQueuedVariantRangeSelection(GameQueuedVariantRangeEntry* pEntry)
{
	int nResourceId;

	if (pEntry->m_pSelection44 != 0) {
		pEntry->m_nSelectedResource40 = *pEntry->m_pSelection44;
	}
	nResourceId = pEntry->m_panResourceIds48[pEntry->m_nSelectedResource40 - pEntry->m_nFirstResource3C];
	((GameCompactZrleOverlay*) pEntry->m_pOverlay4C)->ReloadResource(nResourceId);
}

__inline static void* ConstructQueuedVariantChildContextObject(void* pObject)
{
	union VoidMethod {
		void (GameQueuedVariantChildContextView::*p)(void);
		void* v;
	} Void;
	void** pBaseVtable;
	int i;

	((GameWindowOwnerRenderContext*) pObject)->ConstructWindowOwnerRenderContext();
	pBaseVtable = *(void***) pObject;
	if (g_fQueuedVariantChildContextVtableInitialized == 0) {
		for (i = 0; i < 46; ++i) {
			g_QueuedVariantChildContextVtable[i] = pBaseVtable[i];
		}
		Void.p = &GameQueuedVariantChildContextView::MarkRenderObjectDirty;
		g_QueuedVariantChildContextVtable[15] = Void.v;
		g_fQueuedVariantChildContextVtableInitialized = 1;
	}
	*(void***) pObject = g_QueuedVariantChildContextVtable;
	++*(int*) ((char*) pObject + 0x50);
	return pObject;
}

__inline static void LEMBALL_FASTCALL RebuildCompactZrleOwnerSourceRectIfNeeded(void* pOverlay, int, int fForce)
{
	int fPrevious;
	short Rect[4];
	typedef void(LEMBALL_FASTCALL * SetRectProc)(void*, int, short*);

	if (*(int*) ((char*) pOverlay + 0xfc) == 0 || fForce != 0) {
		Rect[0] = *(short*) ((char*) pOverlay + 8);
		Rect[1] = *(short*) ((char*) pOverlay + 10);
		Rect[2] = 0;
		Rect[3] = 0;
		fPrevious = *(int*) ((char*) pOverlay + 0xf8);
		*(int*) ((char*) pOverlay + 0xf8) = 1;
		((SetRectProc) (*(void***) pOverlay)[42])(pOverlay, 0, Rect);
		*(int*) ((char*) pOverlay + 0xf8) = fPrevious;
	}
	*(int*) ((char*) pOverlay + 0xfc) = 0;
}

void GameQueuedVariantRangeEntry::RebuildSourceRect(int fForce)
{
	if (m_pOverlay4C != 0) {
		RebuildCompactZrleOwnerSourceRectIfNeeded(m_pOverlay4C, 0, fForce);
	}
}

void* LEMBALL_FASTCALL DeleteQueuedVariantChildSlotManager(GameQueuedVariantChildSlotManager* pManager,
														   int,
														   int fDelete)
{
	DestroyQueuedVariantChildSlotManager(pManager);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pManager);
	}
	return pManager;
}

inline static GameProgressiveVariantFrameSelector* AllocateFrameSelector(void* pVariantManager, int nResourceId)
{
	GameProgressiveVariantFrameSelector* pSelector;

	pSelector = (GameProgressiveVariantFrameSelector*) AllocateVSMemBlock(0x1c);
	if (pSelector != 0) {
		pSelector->m_pVtable = g_MenuSelectorFrameSelectorCtorVtable;
		pSelector->m_cFrames = ((CAnimsManager*) pVariantManager)->GetnAnims(nResourceId);
		pSelector->m_pVtable = g_MenuSelectorTimedFrameSelectorVtable;
		pSelector->m_nDirection = 1;
		pSelector->m_pVtable = g_MenuSelectorProgressiveFrameSelectorVtable;
	}
	return pSelector;
}

inline static void ReleaseMenuSelectorVariantResource(void* pObject, int nResourceId)
{
	int* pManager;
	void** ppResources;
	short* pnResourceIds;
	int iResource;

	pManager = (int*) pObject;
	ppResources = (void**) (unsigned long) pManager[9];
	pnResourceIds = (short*) (unsigned long) pManager[10];
	iResource = pnResourceIds[nResourceId];
	ReleaseTypedResourceObjectReference(ppResources[iResource]);
	ppResources[iResource] = 0;
	pnResourceIds[nResourceId] = (short) pManager[11];
	--pManager[13];
}

__inline static void InitializeSelectorProgressOverlayOwnerVtables(void** pOwnerVtable, void** pDeleteVtable)
{
	union PointMethod {
		void (GameSelectorProgressOverlayOwner::*p)(const VS_XYPair*);
		void* v;
	} Point;
	union ModeMethod {
		void (GameSelectorProgressOverlayOwnerView::*p)(int);
		void* v;
	} Mode;
	int i;

	if (g_fSelectorProgressOverlayOwnerVtablesInitialized != 0) {
		return;
	}
	for (i = 0; i < 52; ++i) {
		g_SelectorProgressOverlayOwnerVtable[i] = pOwnerVtable[i];
	}
	for (i = 0; i < 8; ++i) {
		g_SelectorProgressOverlayOwnerDeleteVtable[i] = pDeleteVtable[i];
	}
	Point.p = &GameSelectorProgressOverlayOwner::SetAnchorAndNotifyChild;
	g_SelectorProgressOverlayOwnerVtable[14] = Point.v;
	Mode.p = &GameSelectorProgressOverlayOwnerView::SetChildPresentForModeA;
	g_SelectorProgressOverlayOwnerVtable[48] = Mode.v;
	Mode.p = &GameSelectorProgressOverlayOwnerView::SetChildPresentForModeB;
	g_SelectorProgressOverlayOwnerVtable[49] = Mode.v;
	g_SelectorProgressOverlayOwnerDeleteVtable[0] = (void*) DeleteSelectorProgressOverlayOwnerAdjusted;
	g_fSelectorProgressOverlayOwnerVtablesInitialized = 1;
}

static int LEMBALL_FASTCALL handle_menu_selector_entry_event_thunk(GameMenuSelectorEntry* pEntry,
																   int nUnusedEdx,
																   GameMenuSelectorEvent* pEvent)
{
	return pEntry->HandleEvent(pEvent);
}

static void* LEMBALL_FASTCALL delete_menu_selector_entry_thunk(GameMenuSelectorEntry* pEntry,
															   int nUnusedEdx,
															   int fDelete)
{
	return DeleteMenuSelectorEntry(pEntry, nUnusedEdx, fDelete);
}

static void* LEMBALL_FASTCALL DeleteMenuSelectorManagerThunk(GameMenuSelectorManager* pManager,
															 int nUnusedEdx,
															 int fDelete)
{
	return DeleteMenuSelectorManager(pManager, nUnusedEdx, fDelete);
}

inline static void BeginMenuSelectorMovement(GameMenuSelectorManager* pManager)
{
	int nDistance;
	pManager->m_dwMovementStart21C = (int) timeGetTime();
	nDistance = abs(pManager->m_nTargetPosition9C - pManager->m_nCurrentPosition8C);
	pManager->m_nMovementStartPosition90 = pManager->m_nCurrentPosition8C;
	pManager->m_fMovementActiveDC = 1;
	pManager->m_dwMovementEnd220 = pManager->m_dwMovementStart21C + nDistance * 3;
}

static void LEMBALL_FASTCALL MoveMenuSelectorPreviousThunk(GameMenuSelectorManager* pManager)
{
	pManager->MovePrevious();
}

static void LEMBALL_FASTCALL MoveMenuSelectorNextThunk(GameMenuSelectorManager* pManager)
{
	pManager->MoveNext();
}

static void LEMBALL_FASTCALL SelectMenuSelectorSide0Thunk(GameMenuSelectorManager* pManager)
{
	pManager->SelectSide0();
}

static void LEMBALL_FASTCALL SelectMenuSelectorSide1Thunk(GameMenuSelectorManager* pManager)
{
	pManager->SelectSide1();
}

inline static void SetFrameSelectorDuration(GameProgressiveVariantFrameSelector* pSelector, int dwDuration)
{
	((GameProgressiveVariantFrameSelectorInterface*) pSelector)->SetDuration(dwDuration);
}

static void LEMBALL_FASTCALL StartMenuSelectorActionThunk(GameMenuSelectorManager* pManager)
{
	pManager->StartAction();
}

static int LEMBALL_FASTCALL HandleMenuSelectorEventThunk(GameMenuSelectorManager* pManager,
														 int nUnusedEdx,
														 GameMenuSelectorEvent* pEvent)
{
	return pManager->HandleEvent(pEvent);
}

static int GetIntDirectionSignThunk(int nValue)
{
	return GetIntDirectionSign(nValue);
}

class GameMenuSelectorEventQueueInterface {
public:
	virtual void Reserved0(void) = 0;
	virtual void Reserved1(void) = 0;
	virtual void QueueEvent(void* pEvent) = 0;
};

inline static void QueueMenuSelectorEvent(GameMenuSelectorEvent* pEvent)
{
	((GameMenuSelectorEventQueueInterface*) g_pSharedRenderDispatchQueue)->QueueEvent(pEvent);
}

void LEMBALL_FASTCALL UpdateMenuSelectorAnimationThunk(GameMenuSelectorManager* pManager)
{
	UpdateMenuSelectorAnimation(pManager);
}

__inline static void InitializeSelectorProgressOverlayChildVtables(void** pWindowOwnerVtable, void** pGeometryVtable)
{
	union WindowRectMethod {
		void (GameSelectorProgressOverlayWindowSlots::*p)(short*, void*, const char*);
		void* v;
	} WindowRect;
	union WindowPointMethod {
		void (GameSelectorProgressOverlayWindowSlots::*p)(const VS_XYPair*);
		void* v;
	} WindowPoint;
	union WindowVoidMethod {
		void (GameSelectorProgressOverlayWindowSlots::*p)(void);
		void* v;
	} WindowVoid;
	union WindowIntMethod {
		void (GameSelectorProgressOverlayWindowSlots::*p)(int);
		void* v;
	} WindowInt;
	union GeometryButtonMethod {
		void (GameSelectorProgressGeometry::*p)(const VS_XYPair*, int);
		void* v;
	} GeometryButton;
	union GeometryPointMethod {
		void (GameSelectorProgressGeometry::*p)(const VS_XYPair*);
		void* v;
	} GeometryPoint;
	int i;

	if (g_fSelectorProgressOverlayChildVtablesInitialized != 0) {
		return;
	}
	for (i = 0; i < 46; ++i) {
		g_SelectorProgressOverlayChildVtable[i] = pWindowOwnerVtable[i];
	}
	for (i = 0; i < 8; ++i) {
		g_SelectorProgressGeometryVtable[i] = pGeometryVtable[i];
	}
	g_pSelectorProgressBaseWindowOwnerVtable = pWindowOwnerVtable;
	WindowRect.p = &GameSelectorProgressOverlayWindowSlots::InitializeWindowOwnerFromRect;
	g_SelectorProgressOverlayChildVtable[1] = WindowRect.v;
	WindowPoint.p = &GameSelectorProgressOverlayWindowSlots::MoveWindowOwnerOrigin;
	g_SelectorProgressOverlayChildVtable[14] = WindowPoint.v;
	WindowVoid.p = &GameSelectorProgressOverlayWindowSlots::ReservedWindowOwnerSlot15;
	g_SelectorProgressOverlayChildVtable[15] = WindowVoid.v;
	WindowVoid.p = &GameSelectorProgressOverlayWindowSlots::ReservedWindowOwnerSlot23;
	g_SelectorProgressOverlayChildVtable[23] = WindowVoid.v;
	g_SelectorProgressOverlayChildVtable[25] = (void*) return_hresult_80000801_progress_overlay;
	WindowInt.p = &GameSelectorProgressOverlayWindowSlots::ApplyWindowOwnerActivation;
	g_SelectorProgressOverlayChildVtable[42] = WindowInt.v;
	GeometryButton.p = &GameSelectorProgressGeometry::DispatchButtonTransition;
	g_SelectorProgressGeometryVtable[0] = (void*) DeleteSelectorProgressOverlayChildAdjusted;
	g_SelectorProgressGeometryVtable[1] = GeometryButton.v;
	GeometryPoint.p = &GameSelectorProgressGeometry::DispatchPointerMove;
	g_SelectorProgressGeometryVtable[6] = GeometryPoint.v;
	g_fSelectorProgressOverlayChildVtablesInitialized = 1;
}

void LEMBALL_FASTCALL SetMenuSelectorChildrenEnabledThunk(GameMenuSelectorManager* pManager,
														  int nUnusedEdx,
														  int fEnabled)
{
	SetMenuSelectorChildrenEnabled(pManager, nUnusedEdx, fEnabled);
}

// Compiler-state shim for the original selector-manager constructor ILT; removal loses exact 0x0044d290 and
// 0x0044ebc0.
GameMenuSelectorManager* LEMBALL_FASTCALL ConstructMenuSelectorManagerThunk(GameMenuSelectorManager* pManager,
																			int nUnusedEdx,
																			void* pRenderOwner,
																			void* pRenderContext,
																			int nChildCapacity,
																			int nLayoutMode)
{
	(void) nUnusedEdx;
	return new (pManager) GameMenuSelectorManager(pRenderOwner, pRenderContext, nChildCapacity, nLayoutMode);
}

// FUNCTION: LEMBALL 0x0043a540
void LEMBALL_FASTCALL DetachGeometryChildAndDestroyWindowOwner(void* pObject)
{
	void* pChildOwner;
	void* pRenderOwner;

	pChildOwner = pObject == 0 ? 0 : (char*) pObject + 0x90;
	pRenderOwner = *(void**) ((char*) pObject + 0xc8);
	RemoveGeometryOwnerChild(*(void**) ((char*) pRenderOwner + 0x1c), 0, pChildOwner);
	((VS_WindowOwnerBase*) pObject)->VS_WindowOwnerBase::DestroyWindowOwnerHwnd();
}
// FUNCTION: LEMBALL 0x0043a580
void GameResourceGeometryChildView::SetRenderAnchorWithOriginCompensation(const VS_XYPair* pPoint)
{
	char* pObject;

	pObject = (char*) this;
	*(int*) (pObject + 0xd8) = 1;
	*(short*) (pObject + 0xc4) -= *(short*) (pObject + 0x18);
	*(short*) (pObject + 0xc6) -= *(short*) (pObject + 0x1a);
	SetWindowOwnerRenderAnchorFromPoint(this, 0, pPoint);
	*(short*) (pObject + 0xc4) += *(short*) (pObject + 0x18);
	*(short*) (pObject + 0xc6) += *(short*) (pObject + 0x1a);
}
// FUNCTION: LEMBALL 0x0043a620
void GameSelectorProgressOverlayOwnerView::SetChildPresentForModeA(int nMode)
{
	if (*(void**) ((char*) this + 0xd0) != 0 && (nMode == 0 || nMode == 3)) {
		*(int*) ((char*) this + 0x104) = 1;
		return;
	}
	*(int*) ((char*) this + 0x104) = 0;
}
// FUNCTION: LEMBALL 0x0043a660
void GameSelectorProgressOverlayOwnerView::SetChildPresentForModeB(int nMode)
{
	if (*(void**) ((char*) this + 0xd0) != 0 && (nMode == 0 || nMode == 3)) {
		*(int*) ((char*) this + 0x104) = 1;
		return;
	}
	*(int*) ((char*) this + 0x104) = 0;
}
// FUNCTION: LEMBALL 0x0043a6a0
void GameAlignedZrleChildOverlayView::HandlePointerEnter(void)
{
	if (*(void**) ((char*) this + 0xd0) != 0 &&
		(*(int*) ((char*) this + 0xa0) != 0 || *(int*) ((char*) this + 0xac) != 0)) {
		*(int*) ((char*) this + 0x104) = 1;
		return;
	}
	*(int*) ((char*) this + 0x104) = 0;
}
// FUNCTION: LEMBALL 0x0043a6e0
void GameAlignedZrleChildOverlayView::HandlePointerLeave(void)
{
	if (*(void**) ((char*) this + 0xd0) != 0 &&
		(*(int*) ((char*) this + 0xa0) != 0 || *(int*) ((char*) this + 0xac) != 0)) {
		*(int*) ((char*) this + 0x104) = 1;
		return;
	}
	*(int*) ((char*) this + 0x104) = 0;
}

// Unreferenced compatibility body preserves original MENUSEL.CPP compiler state.
void* LEMBALL_FASTCALL InitializeMenuActionFramePointSinkCompilerStateShim(void* pObject)
{
	GameMenuActionFramePointSink* pEntry;

	pEntry = (GameMenuActionFramePointSink*) pObject;
	pEntry->m_pVtable00 = (void**) g_pQueuedRenderPointSinkFinalizeThunk;
	pEntry->m_nReserved06 = 0;
	pEntry->m_pVtable00 = (void**) &g_MenuActionFramePointSinkVtableCompilerStateShim;
	pEntry->m_nReserved04 = 0;
	return pEntry;
}
// FUNCTION: LEMBALL 0x004439b0
void* GameMenuActionFramePointSink::Delete(unsigned int fDelete)
{
	m_pVtable00 = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(this);
	}
	return this;
}
// FUNCTION: LEMBALL 0x00443a20
void GameMenuActionFramePointSink::BindToContext(void* pContext)
{
	typedef void(LEMBALL_FASTCALL * LoadProc)(void* pObject);

	if (m_pResource1C[4] == 0) {
		((LoadProc) (*(void***) m_pResource1C)[7])(m_pResource1C);
	}
	else {
		m_pResource1C[9] = 0;
	}
	++m_pResource1C[2];
	((VsGdiHelperDispatchQueue*) pContext)->AppendPointerQueueEntry(this);
}
// FUNCTION: LEMBALL 0x00443a60
void GameMenuActionFramePointSink::DispatchToRenderObject(void* pContext)
{
	typedef void(LEMBALL_FASTCALL * RenderProc)(void* pRenderObject, int nUnusedEdx, void* pEntry, void* pFrame);
	int nContext;
	int nRenderObject;
	void* pFrame;

	nContext = *(int*) ((char*) pContext + 0x0c);
	nRenderObject = *(int*) (*(int*) (nContext + 0x40) + 4) + 0x40 + nContext;
	pFrame = (char*) m_pResource1C[0x1e] + m_iFrame18 * 0x54;
	((RenderProc) (*(void***) nRenderObject)[9])((void*) nRenderObject, 0, this, pFrame);
	--m_pResource1C[2];
}

// FUNCTION: LEMBALL 0x00443d40
void LEMBALL_FASTCALL ReleaseLevelScreenPauseDialogResources(void* pObject)
{
	char* pBytes = (char*) pObject;

	if (*(int*) (pBytes + 0x208) != 0) {
		((CTextManager*) (pBytes + 0xdc))->UnLoadFont(*(int*) (pBytes + 0x204));
		ReleaseTypedResourceObjectReference(*(void**) (pBytes + 0x158));
		ReleaseTypedResourceObjectReference(*(void**) (pBytes + 0x154));
		*(int*) (pBytes + 0x208) = 0;
	}
}

// FUNCTION: LEMBALL 0x00444790
void LEMBALL_FASTCALL DestroyLevelScreenPauseDialog(void* pAdjusted)
{
	typedef void(LEMBALL_FASTCALL * ReleaseProc)(void* pObject);
	typedef void(LEMBALL_FASTCALL * DeleteProc)(void* pObject, int nUnusedEdx, int fDelete);
	char* pBase;
	char* pEntry;
	void* pOwned;
	int i;

	pBase = (char*) pAdjusted - 0x90;
	RemoveGeometryOwnerChild(*(void**) (*(char**) ((char*) pAdjusted + 0x84) + 0x1c), 0, (char*) pAdjusted + 0x14);
	if (*(int*) ((char*) pAdjusted - 0x8c) == 1) {
		((ReleaseProc) (*(void***) pBase)[0x74 / sizeof(void*)])(pBase);
	}
	FreeVSMemBlock(*(void**) ((char*) pAdjusted + 0x168));
	ReleasePauseDialogPaletteRemaps(pBase);
	pOwned = *(void**) ((char*) pAdjusted + 0x14c);
	if (pOwned != 0) {
		((DeleteProc) (*(void***) pOwned)[0])(pOwned, 0, 3);
	}
	ReleaseLevelScreenPauseDialogResources(pBase);
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->UnregisterOrderedRenderDispatchClient(pAdjusted, 0);
	pEntry = (char*) pAdjusted + 0x14c;
	for (i = 3; i >= 0; --i) {
		pEntry -= 0x20;
		ResetBaseModeActionButtonPointSink(pEntry);
	}
	RestorePackagedSpriteRenderEntryVtableABody((char*) pAdjusted + 0xa8);
	((CTextManager*) ((char*) pAdjusted + 0x4c))->Destroy();
	DestroyWindowOwnerRenderContext(pBase);
}

// FUNCTION: LEMBALL 0x00444680
void* LEMBALL_FASTCALL
ConstructLevelScreenPauseDialog(void* pObject, int nUnusedEdx, void* pOwner, void* pContext, void* pOptions)
{
	char* pBytes;
	char* pEntry;
	int i;

	(void) nUnusedEdx;
	pBytes = (char*) pObject;
	((GameWindowOwnerRenderContext*) pObject)->ConstructWindowOwnerRenderContext();
	new (pBytes + 0xdc) CTextManager(0x2b6, 1, 0x0f, 0);
	InitializeRenderQueueNodeBase(pBytes + 0x90);
	InitializeEmptyGeometryOwnerRect(pBytes + 0xa4);
	*(short*) (pBytes + 0x132) = 0;
	*(short*) (pBytes + 0x130) = 0;
	pEntry = pBytes + 0x138;
	for (i = 0; i >= 0; --i) {
		InitializePackagedSpriteRenderEntry(pEntry);
		pEntry += 0x10;
	}
	*(short*) (pBytes + 0x14a) = 0;
	*(short*) (pBytes + 0x148) = 0;
	*(short*) (pBytes + 0x14e) = 0;
	*(short*) (pBytes + 0x14c) = 0;
	*(short*) (pBytes + 0x152) = 0;
	*(short*) (pBytes + 0x150) = 0;
	pEntry = pBytes + 0x15c;
	for (i = 3; i >= 0; --i) {
		InitializeMenuActionFramePointSink(pEntry);
		pEntry += 0x20;
	}
	*(void**) (pBytes + 0x110) = pOwner;
	*(void**) (pBytes + 0x104) = pOptions;
	*(void**) (pBytes + 0x114) = pContext;
	*(int*) (pBytes + 0x108) = 0;
	*(int*) (pBytes + 0x134) = 0;
	configure_level_screen_pause_dialog_options(pObject);
	RebuildLevelScreenPauseDialogLayout(pObject);
	return pObject;
}

// FUNCTION: LEMBALL 0x00444980
void LEMBALL_FASTCALL RebuildPauseDialogEntries(void* pObject, int nUnusedEdx, int nUnusedArg)
{
	char* pBytes;
	char* pEntry;
	void* pRenderQueue;
	void* pRenderObject;
	short aPoint[2];
	int nEntryOffset;
	int nTextOffset;
	int i;

	(void) nUnusedEdx;
	(void) nUnusedArg;
	pBytes = (char*) pObject;
	pRenderQueue = *(void**) (pBytes + 0x4c);
	pRenderObject = *(void**) ((char*) pRenderQueue + 0x0c);
	pRenderObject = (char*) pRenderObject + *(int*) (*(char**) ((char*) pRenderObject + 0x40) + 4) + 0x40;
	((void(LEMBALL_FASTCALL*)(void*))(*(void***) pRenderObject)[0x38 / sizeof(void*)])(pRenderObject);
	((void(LEMBALL_FASTCALL*)(void*, int, void*))(*(void***) (pBytes + 0x138))[1])(pBytes + 0x138, 0, pRenderQueue);
	for (i = 0, nEntryOffset = 0; i < *(int*) (pBytes + 0x134) * 2; ++i, nEntryOffset += 0x20) {
		pEntry = *(char**) (pBytes + 0x1dc) + nEntryOffset;
		((void(LEMBALL_FASTCALL*)(void*, int, void*))(*(void***) pEntry)[1])(pEntry, 0, pRenderQueue);
	}
	pEntry = pBytes + 0x15c;
	for (i = 4; i != 0; --i, pEntry += 0x20) {
		((void(LEMBALL_FASTCALL*)(void*, int, void*))(*(void***) pEntry)[1])(pEntry, 0, pRenderQueue);
	}
	aPoint[0] = 0;
	aPoint[1] = 0;
	for (i = 0, nTextOffset = 0, nEntryOffset = 0; i < *(int*) (pBytes + 0x120);
		 ++i, nTextOffset += 4, nEntryOffset += 8) {
		pEntry = *(char**) (pBytes + 0x1f8) + nEntryOffset;
		if (pEntry != 0) {
			pEntry += 4;
		}
		((CTextManager*) (pBytes + 0xdc))
			->DrawString(pRenderQueue,
						 (short*) pEntry,
						 aPoint,
						 *(int*) (pBytes + 0x204),
						 *(void**) (*(char**) (pBytes + 0x100) + nTextOffset),
						 0x20,
						 SelectPauseDialogPaletteRemapForIndex(pObject, 0, i));
	}
	CTextManagerResetPrimitives(pBytes + 0xdc);
}

extern void* LEMBALL_FASTCALL DeleteRegistrationInfoScreenAuto(void* pObject, void*, BYTE fDelete);
// ABI-THUNK: 83 e9 04; e9 rel32. __fastcall this adjustment followed by tail jump;
// MSVC 4.20 C++ emitted call/ret. Verified with reccmp.
// FUNCTION: LEMBALL 0x0044c1d0
__declspec(naked) void* LEMBALL_FASTCALL DeleteRegistrationInfoScreenAdjusted(void*, int, BYTE)
{
	__asm {
        sub ecx, 4
        jmp offset DeleteRegistrationInfoScreenAuto
	}
}
// FUNCTION: LEMBALL 0x0044c1e0
int GetIntDirectionSign(int nValue)
{
	if (nValue == 0) {
		return 0;
	}
	if (nValue < 0) {
		return -1;
	}
	return 1;
}
// FUNCTION: LEMBALL 0x0044c200
void GameAlignedZrleChildOverlayView::HandleButtonRelease(int nMode)
{
	if (*(void**) ((char*) this + 0xd0) != 0 && (nMode == 0 || nMode == 3)) {
		*(int*) ((char*) this + 0x104) = 1;
		return;
	}
	*(int*) ((char*) this + 0x104) = 0;
}
// FUNCTION: LEMBALL 0x0044c230
void GameAlignedZrleChildOverlayView::HandleButtonPress(int nMode)
{
	if (*(void**) ((char*) this + 0xd0) == 0 || (nMode != 0 && nMode != 3)) {
		*(int*) ((char*) this + 0x104) = 0;
	}
	else {
		*(int*) ((char*) this + 0x104) = 1;
	}
	if (nMode == 0) {
		((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffectThunk(0x25);
	}
}
// FUNCTION: LEMBALL 0x0044c270
GameMenuSelectorEntry::GameMenuSelectorEntry(void* pRenderOwner,
											 void* pRenderContext,
											 int x,
											 int nPosition,
											 const int* panResourceIds,
											 int nActivationMode,
											 int nFirstResource,
											 int nLastResource,
											 int nReserved,
											 int nToken,
											 int* pSelection,
											 int nCommand)
{
	int cResources;

	InitializeRenderQueueNodeBase(this);
	m_pVtable00 = g_MenuSelectorEntryVtable;
	m_nProgressY16 = 0;
	m_nOverlayKind10 = 0;
	m_nProgressX14 = 0;
	m_nProgressHeight1A = 0;
	m_nProgressWidth18 = 0;
	m_pRenderOwner28 = pRenderOwner;
	m_pRenderContext24 = pRenderContext;
	m_fEnabled5C = 1;
	cResources = nLastResource - nFirstResource + 1;
	m_cResources38 = cResources;
	if (cResources < 2) {
		m_cResources38 = 1;
	}
	m_nFirstResource40 = nFirstResource;
	m_nToken30 = nToken;
	m_nLastResource3C = nLastResource;
	m_x1C = x;
	m_nPosition20 = nPosition;
	if (pSelection == 0) {
		m_nSelectedResource44 = 0;
	}
	else {
		m_nSelectedResource44 = *pSelection;
	}
	if (nCommand != -1) {
		m_nCommand34 = nCommand;
	}
	m_pSelection48 = pSelection;
	m_nActivationMode2C = nActivationMode;
	(void) nReserved;
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->RegisterOrderedRenderDispatchClient(this, 0);
	BuildOverlay(panResourceIds);
}
// FUNCTION: LEMBALL 0x0044c330
GameMenuSelectorEntry* GameMenuSelectorEntry::ConstructProgressEntry(const short* paProgressRect,
																	 void* pRenderOwner,
																	 void* pRenderContext,
																	 int x,
																	 int nPosition,
																	 const int* panResourceIds,
																	 int nActivationMode,
																	 int nSelectedResource,
																	 int nToken,
																	 int* pSelection,
																	 int nCommand)
{
	const short* pExtent;

	InitializeRenderQueueNodeBase(this);
	m_nProgressY16 = 0;
	m_nOverlayKind10 = 1;
	m_pVtable00 = g_MenuSelectorEntryVtable;
	m_nProgressX14 = 0;
	m_nProgressHeight1A = 0;
	m_nProgressWidth18 = 0;
	m_nProgressX14 = paProgressRect[0];
	m_nProgressY16 = paProgressRect[1];
	pExtent = paProgressRect != 0 ? paProgressRect + 2 : 0;
	m_nProgressWidth18 = pExtent[0];
	m_nProgressHeight1A = pExtent[1];
	m_pRenderOwner28 = pRenderOwner;
	m_pRenderContext24 = pRenderContext;
	m_nToken30 = nToken;
	m_x1C = x;
	m_nPosition20 = nPosition;
	m_nSelectedResource44 = nSelectedResource;
	m_cResources38 = 1;
	m_nFirstResource40 = 0;
	m_nLastResource3C = 100;
	if (nCommand != -1) {
		m_nCommand34 = nCommand;
	}
	m_pSelection48 = pSelection;
	m_nActivationMode2C = nActivationMode;
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->RegisterOrderedRenderDispatchClient(this, 0);
	BuildOverlay(panResourceIds);
	return this;
}
// FUNCTION: LEMBALL 0x0044c410
void LEMBALL_FASTCALL DestroyMenuSelectorEntry(GameMenuSelectorEntry* pEntry)
{
	pEntry->m_pVtable00 = g_MenuSelectorEntryVtable;
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->UnregisterOrderedRenderDispatchClient(pEntry, 0);
	ReleaseMenuSelectorEntryResources(pEntry);
	pEntry->m_pVtable00 = g_GAME_RenderQueueNodeVtableSlots;
}
// FUNCTION: LEMBALL 0x0044c460
int GameMenuSelectorEntry::HandleEvent(GameMenuSelectorEvent* pEvent)
{
	struct CommandEvent {
		short m_nType00;
		short m_nReserved02;
		int m_dwTimestamp04;
		int m_nPayload08;
		int m_nPayload0C;
		int m_nPayload10;
	};
	typedef void(LEMBALL_FASTCALL * QueueProc)(void*, int, void*);
	CommandEvent Event;
	void* pOverlay;
	int nPercent;
	int nResourceId;
	int nOverlayKind;

	Event.m_nType00 = 0x0c;
	Event.m_dwTimestamp04 = (int) timeGetTime();
	Event.m_nPayload08 = 0;
	Event.m_nPayload0C = 0;
	Event.m_nPayload10 = 0;

	if (pEvent->m_nPayload08 != m_nToken30 || *(unsigned short*) pEvent != 0x0c) {
		return 0;
	}
	nOverlayKind = m_nOverlayKind10;
	switch (nOverlayKind) {
	case 0:
		if (m_nActivationMode2C == 1) {
			Event.m_nPayload08 = m_nCommand34;
			((QueueProc) (*(void***) g_pSharedRenderDispatchQueue)[2])(g_pSharedRenderDispatchQueue, 0, &Event);
			return 0;
		}

		++m_nSelectedResource44;
		if (m_nSelectedResource44 > m_nLastResource3C) {
			m_nSelectedResource44 = m_nFirstResource40;
		}
		if (m_pSelection48 != 0) {
			if (m_cResources38 == 1) {
				if (*m_pSelection48 == 0) {
					*m_pSelection48 = 1;
				}
				else {
					*m_pSelection48 = 0;
				}
			}
			else {
				*m_pSelection48 = m_nSelectedResource44;
			}
		}
		if (m_pAlignedOverlay50 != 0) {
			nResourceId = m_panResourceIds4C[m_nSelectedResource44 - m_nFirstResource40];
			pOverlay = m_pAlignedOverlay50;
		}
		else {
			nResourceId = m_panResourceIds4C[0];
			pOverlay = m_pProgressOverlay54;
		}
		((GameCompactZrleOverlay*) pOverlay)->ReloadResource(nResourceId);
		g_fLevelSelectionChanged = 1;
		return 0;
	case 1:
		break;
	default:
		return 0;
	}

	nPercent = *(int*) (*(int*) ((char*) m_pProgressOverlay54 + 0x134) + 0x120);
	if (nPercent == m_nLastResource3C) {
		nPercent = 0;
	}
	else {
		nPercent += m_nLastResource3C / 4;
		if (nPercent > m_nLastResource3C) {
			nPercent = m_nLastResource3C;
		}
	}
	*m_pSelection48 = nPercent;
	((GameSelectorProgressOverlayChild*) *(void**) ((char*) m_pProgressOverlay54 + 0x134))->SetPercent(nPercent);
	return 0;
}
// FUNCTION: LEMBALL 0x0044c600
void GameMenuSelectorEntry::RebuildRects(int fForce, int)
{
	if (m_pAlignedOverlay50 != 0) {
		((GameCompactZrleOverlay*) m_pAlignedOverlay50)->RebuildSourceRect(fForce);
	}
	if (m_pProgressOverlay54 != 0) {
		((GameCompactZrleOverlay*) m_pProgressOverlay54)->RebuildSourceRect(fForce);
	}
}
// FUNCTION: LEMBALL 0x0044c630
void GameMenuSelectorEntry::BuildOverlay(const int* panResourceIds)
{
	GameMenuSelectorEntry* pEntry;
	void* pOverlay;
	void** ppOverlay;
	int nContext;
	int nRenderObject;
	int i;
	VS_XYPair Point;

	pEntry = this;

	pEntry->m_panResourceIds4C = panResourceIds;
	pEntry->m_ppLoadedResources58 = (void**) AllocateVSMemBlock(pEntry->m_cResources38 * sizeof(void*));
	for (i = 0; i < pEntry->m_cResources38; ++i) {
		pEntry->m_ppLoadedResources58[i] = LoadZrleOnlyListResource(pEntry->m_panResourceIds4C[i]);
	}

	if (pEntry->m_nOverlayKind10 == 0) {
		pOverlay = AllocateVSMemBlock(0x130);
		if (pOverlay != 0) {
			Point.m_x = (short) pEntry->m_x1C;
			Point.m_y = (short) pEntry->m_nPosition20;
			ConstructAlignedZrleChildOverlayFromPoint(
				pOverlay,
				0,
				&Point,
				pEntry->m_pRenderOwner28,
				pEntry->m_panResourceIds4C[pEntry->m_nSelectedResource44 - pEntry->m_nFirstResource40],
				3);
		}
		pEntry->m_pAlignedOverlay50 = pOverlay;
		ppOverlay = &pEntry->m_pAlignedOverlay50;
		nContext = *(int*) (*(int*) ((char*) *ppOverlay + 0x4c) + 0x0c);
		SetAutoDraw(*ppOverlay, 0, 0);
		nRenderObject = nContext + *(int*) (*(int*) (nContext + 0x40) + 4) + 0x70;
		*(int*) nRenderObject = 0;
		*(void**) ((char*) *ppOverlay + 0xf4) = g_pSharedRenderDispatchQueue;
		*(int*) ((char*) *ppOverlay + 0xcc) = pEntry->m_nToken30;
		pEntry->m_pProgressOverlay54 = 0;
		return;
	}

	pOverlay = AllocateVSMemBlock(0x138);
	if (pOverlay != 0) {
		Point.m_x = (short) pEntry->m_x1C;
		Point.m_y = (short) pEntry->m_nPosition20;
		pOverlay = ((GameSelectorProgressOverlayOwner*) pOverlay)
					   ->Construct(&Point,
								   pEntry->m_pRenderOwner28,
								   pEntry->m_panResourceIds4C[0],
								   &pEntry->m_nProgressX14,
								   pEntry->m_nSelectedResource44);
	}
	pEntry->m_pProgressOverlay54 = pOverlay;
	ppOverlay = &pEntry->m_pProgressOverlay54;
	nContext = *(int*) (*(int*) ((char*) *ppOverlay + 0x4c) + 0x0c);
	SetAutoDraw(*ppOverlay, 0, 0);
	nRenderObject = nContext + *(int*) (*(int*) (nContext + 0x40) + 4) + 0x70;
	*(int*) nRenderObject = 0;
	*(void**) ((char*) *ppOverlay + 0xf4) = g_pSharedRenderDispatchQueue;
	*(int*) ((char*) *ppOverlay + 0xcc) = pEntry->m_nToken30;
	pEntry->m_pAlignedOverlay50 = 0;
}
// FUNCTION: LEMBALL 0x0044c7c0
void LEMBALL_FASTCALL ReleaseMenuSelectorEntryResources(GameMenuSelectorEntry* pEntry)
{
	typedef void(LEMBALL_FASTCALL * DeleteOverlayProc)(void* pOverlayDeleteInterface, int nUnusedEdx, int fDelete);
	void* pDeleteInterface;
	int i;

	if (pEntry->m_pAlignedOverlay50 != 0) {
		pDeleteInterface = (char*) pEntry->m_pAlignedOverlay50 + 0x90;
		((DeleteOverlayProc) (*(void***) pDeleteInterface)[0])(pDeleteInterface, 0, 1);
	}
	if (pEntry->m_pProgressOverlay54 != 0) {
		pDeleteInterface = (char*) pEntry->m_pProgressOverlay54 + 0x90;
		((DeleteOverlayProc) (*(void***) pDeleteInterface)[0])(pDeleteInterface, 0, 1);
	}
	for (i = 0; i < pEntry->m_cResources38; ++i) {
		ReleaseTypedResourceObjectReference(pEntry->m_ppLoadedResources58[i]);
	}
	FreeVSMemBlock(pEntry->m_ppLoadedResources58);
	pEntry->m_ppLoadedResources58 = 0;
}
// FUNCTION: LEMBALL 0x0044c830
GameMenuSelectorChildContextView* GameMenuSelectorChildContextView::ConstructSelectorManagerChildContextObject(void)
{
	((GameWindowOwnerRenderContext*) this)->ConstructWindowOwnerRenderContext();
	*(void***) this = g_MenuSelectorChildContextVtable;
	*(int*) ((char*) this + 0x50) += 5;
	return this;
}
// FUNCTION: LEMBALL 0x0044c850
void GameMenuSelectorChildContextView::MarkRenderObjectDirty(void)
{
	int nContext;
	int nRenderObject;

	nContext = *(int*) (*(int*) ((char*) this + 0x4c) + 0x0c);
	nRenderObject = nContext + *(int*) (*(int*) (nContext + 0x40) + 4) + 0x74;
	*(int*) nRenderObject = 1;
}
// FUNCTION: LEMBALL 0x0044c870
GameMenuSelectorManager::GameMenuSelectorManager(void* pRenderOwner, void* pRenderContext, int, int nLayoutMode)
{
	GameMenuSelectorManager* pManager;
	int i;
	int iLocalRect;
	unsigned char* pLocalRectEntry;
	void* pVariantManager;

	pManager = this;
	InitializeRenderQueueNodeBase(pManager);
	pVariantManager = (char*) pManager + 0x10;
	((CAnimsManager*) pVariantManager)->Construct((int) (unsigned long) pRenderContext, 0x2b6, 10, 5, 0, 0);
	pLocalRectEntry = pManager->m_abLocalRectEntry200;
	for (iLocalRect = 0; iLocalRect >= 0; --iLocalRect) {
		InitializeHelperLocalRectUpdateEntry(pLocalRectEntry);
		pLocalRectEntry += 0x10;
	}
	pManager->m_pVtable00 = g_MenuSelectorManagerVtable;
	pManager->m_cCurrentFrameCallbackFrames25C = 1;
	*(void***) pVariantManager = g_MenuSelectorVariantRenderManagerVtable;
	pManager->m_fChildrenEnabled278 = 1;
	pManager->m_nLayoutMode270 = nLayoutMode;
	pManager->m_pRenderOwner214 = pRenderOwner;
	pManager->m_pRenderContext210 = pRenderContext;
	pManager->m_pCurrentFrameCallbackVtable258 = g_MenuSelectorFrameSelectorCtorVtable;
	pManager->m_pCurrentFrameCallbackVtable258 = g_MenuSelectorCurrentFrameCallbackVtable;
	pManager->m_nCurrentFrameCallbackFrame264 = 0;
	pManager->m_nNextToken218 = (int) 0xabcd0000;
	for (i = 0; i < 8; ++i) {
		pManager->m_aMetadataE0[i].m_nPosition = -1;
		pManager->m_aMetadataE0[i].m_xSide0 = -1;
		pManager->m_aMetadataE0[i].m_nSide = 3;
		pManager->m_apChildren1E0[i] = 0;
	}
	pManager->m_nReserved84 = 0;
	pManager->m_cChildren80 = 0;
	pManager->m_fMovementActiveDC = 0;
	pManager->m_nActionStateA4 = 0;
	pManager->m_fEventQueuedB4 = 0;
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->RegisterOrderedRenderDispatchClient(pManager, 0);

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
	}
	else {
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

	((CAnimsManager*) pVariantManager)->LoadAnims(g_nMenuSelectorResourceId0);
	((CAnimsManager*) pVariantManager)->LoadAnims(g_nMenuSelectorResourceId1);
	((CAnimsManager*) pVariantManager)->LoadAnims(g_nMenuSelectorResourceId2);
	((CAnimsManager*) pVariantManager)->LoadAnims(g_nMenuSelectorResourceId3);
	((CAnimsManager*) pVariantManager)->LoadAnims(g_nMenuSelectorResourceId4);
	((CAnimsManager*) pVariantManager)->LoadAnims(g_nMenuSelectorResourceId5);
	((CAnimsManager*) pVariantManager)->LoadAnims(g_nMenuSelectorResourceId6);
	((CAnimsManager*) pVariantManager)->LoadAnims(g_nMenuSelectorResourceId7);
	((CAnimsManager*) pVariantManager)->LoadAnims(g_nMenuSelectorResourceId8);

	pManager->m_pSideSelector244 = AllocateFrameSelector(pVariantManager, g_nMenuSelectorResourceId6);
	pManager->m_pSideSelector244->m_dwFixedTime = -1;
	pManager->m_pActionTailSelector248 = AllocateFrameSelector(pVariantManager, g_nMenuSelectorResourceId4);
	pManager->m_pActionTailSelector248->m_dwFixedTime = -1;
	pManager->m_pActionLeadSelector24C = AllocateFrameSelector(pVariantManager, g_nMenuSelectorResourceId0);
	pManager->m_pActionLeadSelector24C->m_dwFixedTime = -1;
	pManager->m_pCompletionSelector250 = AllocateFrameSelector(pVariantManager, g_nMenuSelectorResourceId7);
	pManager->m_pCompletionSelector250->m_dwFixedTime = -1;
	pManager->m_pActionMiddleSelector254 = AllocateFrameSelector(pVariantManager, g_nMenuSelectorResourceId2);
	pManager->m_pActionMiddleSelector254->m_dwFixedTime = -1;
	pManager->m_dwConstructionTimeD4 = (int) timeGetTime();
}
// FUNCTION: LEMBALL 0x0044cc90
void LEMBALL_FASTCALL SetMenuSelectorChildrenEnabled(GameMenuSelectorManager* pManager, int, int fEnabled)
{
	int i;
	pManager->m_fChildrenEnabled278 = fEnabled;
	for (i = 0; i < pManager->m_cChildren80; ++i) {
		GameMenuSelectorEntry* pEntry = pManager->m_apChildren1E0[i];
		if (pEntry != 0) {
			pEntry->m_fEnabled5C = fEnabled;
			((GameGeometryOwnerRectState*) ((char*) pEntry->m_pAlignedOverlay50 + 0x90))->SetEnabled(fEnabled);
		}
	}
}
// FUNCTION: LEMBALL 0x0044cce0
void GameMenuSelectorManager::InitializeChildContext(void)
{
	typedef void(LEMBALL_FASTCALL *
				 InitializeRectProc)(void* pObject, int nUnusedEdx, short* pRect, void* pOwner, int nFlags);
	short Rect[4];
	void* pContextObject;

	pContextObject = AllocateVSMemBlock(0x90);
	if (pContextObject != 0) {
		ConstructMenuSelectorChildContextObject(pContextObject);
	}
	m_pChildContext268 = pContextObject;
	Rect[0] = *(short*) ((char*) m_pRenderOwner214 + 8);
	Rect[1] = *(short*) ((char*) m_pRenderOwner214 + 10);
	Rect[2] = 0;
	Rect[3] = 0;
	((InitializeRectProc) (*(void***) pContextObject)[1])(pContextObject, 0, Rect, m_pRenderOwner214, 0);
	m_pChildRenderContext26C = *(void**) ((char*) pContextObject + 0x4c);
}
// FUNCTION: LEMBALL 0x0044cd60
void LEMBALL_FASTCALL DestroyMenuSelectorManager(GameMenuSelectorManager* pManager)
{
	int i;
	void* pVariantManager;

	pManager->m_pVtable00 = g_MenuSelectorManagerVtable;
	pVariantManager = (char*) pManager + 0x10;
	*(void***) pVariantManager = g_MenuSelectorVariantRenderManagerVtable;
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->UnregisterOrderedRenderDispatchClient(pManager, 0);
	for (i = 0; i < 8; ++i) {
		if (pManager->m_apChildren1E0[i] != 0) {
			GameMenuSelectorDeleteProc pDelete =
				(GameMenuSelectorDeleteProc) pManager->m_apChildren1E0[i]->m_pVtable00[1];
			pDelete((GameMenuSelectorManager*) pManager->m_apChildren1E0[i], 0, 1);
		}
	}
	ReleaseMenuSelectorVariantResource(pVariantManager, g_nMenuSelectorResourceId0);
	ReleaseMenuSelectorVariantResource(pVariantManager, g_nMenuSelectorResourceId1);
	ReleaseMenuSelectorVariantResource(pVariantManager, g_nMenuSelectorResourceId2);
	ReleaseMenuSelectorVariantResource(pVariantManager, g_nMenuSelectorResourceId3);
	ReleaseMenuSelectorVariantResource(pVariantManager, g_nMenuSelectorResourceId4);
	ReleaseMenuSelectorVariantResource(pVariantManager, g_nMenuSelectorResourceId5);
	ReleaseMenuSelectorVariantResource(pVariantManager, g_nMenuSelectorResourceId6);
	ReleaseMenuSelectorVariantResource(pVariantManager, g_nMenuSelectorResourceId7);
	ReleaseMenuSelectorVariantResource(pVariantManager, g_nMenuSelectorResourceId8);
	FreeVSMemBlock(pManager->m_pSideSelector244);
	FreeVSMemBlock(pManager->m_pActionTailSelector248);
	FreeVSMemBlock(pManager->m_pActionLeadSelector24C);
	FreeVSMemBlock(pManager->m_pCompletionSelector250);
	FreeVSMemBlock(pManager->m_pActionMiddleSelector254);
	if (pManager->m_pChildContext268 != 0) {
		typedef void(LEMBALL_FASTCALL * DetachProc)(void* pObject, int nUnusedEdx);
		if (*(int*) ((char*) pManager->m_pChildContext268 + 4) == 1) {
			((DetachProc) (*(void***) pManager->m_pChildContext268)[29])(pManager->m_pChildContext268, 0);
		}
		DestroyWindowOwnerRenderContext(pManager->m_pChildContext268);
		FreeVSMemBlock(pManager->m_pChildContext268);
	}
	RestorePackagedSpriteRenderEntryVtableC(pManager->m_abLocalRectEntry200);
	DestroyVariantRenderManager(pVariantManager);
	pManager->m_pVtable00 = g_pNonZrleVariantRenderEntryInitializeVtable;
}
// FUNCTION: LEMBALL 0x0044cec0
int GameMenuSelectorManager::HandleEvent(GameMenuSelectorEvent* pEvent)
{
	int dwTimestamp;
	int nType;

	dwTimestamp = pEvent->m_dwTimestamp04;
	if (dwTimestamp - m_dwConstructionTimeD4 < 0) {
		return 0;
	}
	nType = *(unsigned short*) pEvent;
	switch (nType) {
	case 4:
		break;
	default:
		++m_cUnhandledEvents0C;
		return 0;
	}
	switch (pEvent->m_nPayload08) {
	case 1:
		MoveMenuSelectorPreviousThunk(this);
		return 1;
	case 2:
		MoveMenuSelectorNextThunk(this);
		return 1;
	case 3:
		SelectMenuSelectorSide0Thunk(this);
		return 1;
	case 4:
		SelectMenuSelectorSide1Thunk(this);
		return 1;
	case 0x1f:
	case 0x22:
	case 0x4c:
		StartMenuSelectorActionThunk(this);
		break;
	}
	return 0;
}
// FUNCTION: LEMBALL 0x0044cfb0
void GameMenuSelectorManager::AppendChild(int x,
										  int nPosition,
										  const int* panResourceIds,
										  int nActivationMode,
										  int nFirstResource,
										  int nLastResource,
										  int nReserved,
										  int* pSelection,
										  int nCommand)
{
	GameMenuSelectorEntry* pEntry;
	int nToken;

	nToken = ++m_nNextToken218;
	pEntry = (GameMenuSelectorEntry*) AllocateVSMemBlock(sizeof(GameMenuSelectorEntry));
	if (pEntry == 0) {
		m_apChildren1E0[m_cChildren80] = 0;
	}
	else {
		m_apChildren1E0[m_cChildren80] = new (pEntry) GameMenuSelectorEntry(m_pRenderOwner214,
																			m_pRenderContext210,
																			x,
																			nPosition,
																			panResourceIds,
																			nActivationMode,
																			nFirstResource,
																			nLastResource,
																			nReserved,
																			nToken,
																			pSelection,
																			nCommand);
	}
	RecordChildMetadata(x, nPosition, nLastResource > 0, m_apChildren1E0[m_cChildren80]->m_nToken30);
	++m_cChildren80;
}
// FUNCTION: LEMBALL 0x0044d150
void GameMenuSelectorManager::RecordChildMetadata(int x, int nPosition, int fAction, int nToken)
{
	GameMenuSelectorMetadata* pMetadata;
	int i;
	int iMetadata;
	int nSide;
	int xMidpoint;

	iMetadata = -1;
	xMidpoint = *(short*) ((char*) m_pRenderOwner214 + 8) / 2;
	if (x < xMidpoint) {
		nSide = 0;
	}
	else if (x > xMidpoint) {
		nSide = 1;
	}
	for (i = 0; i < 8; ++i) {
		pMetadata = &m_aMetadataE0[i];
		if (pMetadata->m_nSide != 3 && pMetadata->m_nPosition == nPosition) {
			iMetadata = i;
		}
	}

	if (iMetadata != -1) {
		m_aMetadataE0[iMetadata].m_nSide = 2;
		pMetadata = &m_aMetadataE0[iMetadata];
		if (nSide == 0) {
			pMetadata->m_xSide0 = x;
			pMetadata->m_fSide0Action = fAction;
			pMetadata->m_nSide0Token = nToken;
			return;
		}
		pMetadata->m_xSide1 = x;
		pMetadata->m_fSide1Action = fAction;
		pMetadata->m_nSide1Token = nToken;
		return;
	}

	for (i = 0; i < 8; ++i) {
		pMetadata = &m_aMetadataE0[i];
		if (pMetadata->m_nSide == 3) {
			pMetadata->m_nPosition = nPosition;
			pMetadata->m_nSide = nSide;
			if (nSide == 0) {
				pMetadata->m_xSide0 = x;
				pMetadata->m_fSide0Action = fAction;
				pMetadata->m_nSide0Token = nToken;
				return;
			}
			pMetadata->m_xSide1 = x;
			pMetadata->m_fSide1Action = fAction;
			pMetadata->m_nSide1Token = nToken;
			return;
		}
	}
}
// FUNCTION: LEMBALL 0x0044d290
void GameMenuSelectorManager::RebuildChildRects(int fForce, int nUnused)
{
	int nRemaining = 8;
	GameMenuSelectorEntry** ppEntry = m_apChildren1E0;
	int nFirst = fForce;
	int nSecond = nUnused;
	do {
		GameMenuSelectorEntry* pEntry = *ppEntry;
		if (pEntry != 0) {
			pEntry->RebuildRects(nFirst, nSecond);
		}
		++ppEntry;
	} while (--nRemaining != 0);
}
// FUNCTION: LEMBALL 0x0044d2d0
void LEMBALL_FASTCALL EmitMenuSelectorResourceEntries(GameMenuSelectorManager* pManager)
{
	int nContext;
	int nRenderObject;
	int nOwner;
	int i;
	short Point[2];
	short Rect[4];
	const int* pOffsets;

	for (i = 0; i < pManager->m_cChildren80; ++i) {
		GameMenuSelectorEntry* pEntry;

		pEntry = pManager->m_apChildren1E0[i];
		if (pEntry != 0 && pEntry->m_fEnabled5C != 0 && pEntry->m_pAlignedOverlay50 != 0) {
			void* pOverlay;
			char* pFrameSink;

			pOverlay = pEntry->m_pAlignedOverlay50;
			pFrameSink = *(char**) ((char*) pOverlay + 0x124);
			*(short*) (pFrameSink + 4) = (short) pEntry->m_x1C;
			*(short*) (pFrameSink + 6) = (short) pEntry->m_nPosition20;
			*(void**) (pFrameSink + 0x1c) = *(void**) ((char*) pOverlay + 0x11c);
			*(int*) (pFrameSink + 0x18) = 0;
			*(int*) (pFrameSink + 0x0c) = 0;
			*(int*) (pFrameSink + 0x10) = 0;
			((void(LEMBALL_FASTCALL*)(void*, int, void*))(
				*(void***) pFrameSink)[1])(pFrameSink, 0, pManager->m_pRenderContext210);
		}
	}

	nContext = *(int*) ((char*) pManager->m_pChildRenderContext26C + 0x0c);
	nRenderObject = *(int*) (*(int*) (nContext + 0x40) + 4) + 0x40 + nContext;
	((void(LEMBALL_FASTCALL*)(void*, int))(*(void***) nRenderObject)[14])((void*) nRenderObject, 0);
	Point[0] = *(short*) (nRenderObject + 0x14);
	Point[1] = *(short*) (nRenderObject + 0x16);
	*(short*) (pManager->m_abLocalRectEntry200 + 4) = Point[0];
	*(short*) (pManager->m_abLocalRectEntry200 + 6) = Point[1];
	*(short*) (pManager->m_abLocalRectEntry200 + 8) = 0;
	*(short*) (pManager->m_abLocalRectEntry200 + 10) = 0;
	*(int*) (pManager->m_abLocalRectEntry200 + 12) = 0x10000;
	((VsGdiHelperDispatchQueue*) pManager->m_pChildRenderContext26C)
		->AppendPointerQueueEntry(pManager->m_abLocalRectEntry200);

	pOffsets =
		pManager->m_fCompactLayout274 != 1 ? g_anMenuSelectorActionOffsetsMode0 : g_anMenuSelectorActionOffsetsMode1;
	if (pManager->m_nActionStateA4 == 0) {
		pManager->m_nCurrentFrameCallbackFrame264 =
			pManager->m_nCurrentSide94 == 0
				? 0
				: ((CAnimsManager*) pManager->m_abVariantRenderManager10)->GetnAnims(g_nMenuSelectorResourceId6) - 1;
		Point[0] = (short) (pManager->m_xCenteredAction88 + pOffsets[0]);
		Point[1] = (short) (pManager->m_nCurrentPosition8C + pOffsets[1]);
		nOwner = *(int*) (pManager->m_abVariantRenderManager10 + 0x1c);
		*(void**) (pManager->m_abVariantRenderManager10 + 0x1c) = pManager->m_pChildRenderContext26C;
		((CAnimsManager*) pManager->m_abVariantRenderManager10)
			->DrawAnim(Rect, Point, g_nMenuSelectorResourceId6, 0, 0, 0);
		*(int*) (pManager->m_abVariantRenderManager10 + 0x1c) = nOwner;
	}
	((CAnimsManager*) pManager->m_abVariantRenderManager10)->ResetPrimitives();
}
// FUNCTION: LEMBALL 0x0044d830
void GameMenuSelectorManager::MovePrevious(void)
{
	int* pSide;
	int cMetadata;
	int nBest;
	int nSelected;

	pSide = &m_aMetadataE0[0].m_nSide;
	cMetadata = 8;
	nBest = -1;
	nSelected = -1;
	do {
		if (*pSide != 3 && pSide[-2] < m_nTargetPosition9C && nBest < pSide[-2]) {
			if (*pSide != 2) {
				m_nSelectedSideA0 = *pSide;
			}
			nSelected = pSide[-2];
			((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffectThunk(0x11);
			nBest = nSelected;
		}
		pSide += sizeof(GameMenuSelectorMetadata) / sizeof(int);
		--cMetadata;
	} while (cMetadata != 0);
	if (nSelected != -1) {
		m_nTargetPosition9C = nSelected;
	}
	BeginMenuSelectorMovement(this);
}
// FUNCTION: LEMBALL 0x0044d8e0
void GameMenuSelectorManager::MoveNext(void)
{
	int* pSide;
	int cMetadata;
	int nBest;
	int nSelected;

	nBest = 999999;
	nSelected = -1;
	pSide = &m_aMetadataE0[0].m_nSide;
	cMetadata = 8;
	do {
		if (*pSide != 3 && m_nTargetPosition9C < pSide[-2] && pSide[-2] < nBest) {
			if (*pSide != 2) {
				m_nSelectedSideA0 = *pSide;
			}
			nSelected = pSide[-2];
			((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffectThunk(0x11);
			nBest = nSelected;
		}
		pSide += sizeof(GameMenuSelectorMetadata) / sizeof(int);
		--cMetadata;
	} while (cMetadata != 0);
	if (nSelected != -1) {
		m_nTargetPosition9C = nSelected;
	}
	BeginMenuSelectorMovement(this);
}
// FUNCTION: LEMBALL 0x0044d990
void GameMenuSelectorManager::SelectSide0(void)
{
	int i;
	if (m_nCurrentSide94 == m_nSelectedSideA0) {
		for (i = 0; i < 8; ++i) {
			GameMenuSelectorMetadata* pMetadata = &m_aMetadataE0[i];
			if (pMetadata->m_nPosition == m_nTargetPosition9C && (pMetadata->m_nSide == 0 || pMetadata->m_nSide == 2)) {
				m_nSelectedSideA0 = 0;
			}
		}
	}
}
// FUNCTION: LEMBALL 0x0044d9e0
void GameMenuSelectorManager::SelectSide1(void)
{
	int i;
	if (m_nCurrentSide94 == m_nSelectedSideA0) {
		for (i = 0; i < 8; ++i) {
			GameMenuSelectorMetadata* pMetadata = &m_aMetadataE0[i];
			if (pMetadata->m_nPosition == m_nTargetPosition9C && (pMetadata->m_nSide == 1 || pMetadata->m_nSide == 2)) {
				m_nSelectedSideA0 = 1;
			}
		}
	}
}
// FUNCTION: LEMBALL 0x0044da70
void GameMenuSelectorManager::InitializeCurrentState(int iMetadata)
{
	short Extent[2];
	short* pExtent;
	short cxOwner;
	int nSide;

	pExtent = ((CAnimsManager*) ((char*) this + 0x10))->GetAnimSize(Extent, g_nMenuSelectorResourceId6, 0);
	cxOwner = *(short*) ((char*) m_pRenderOwner214 + 8);
	m_xCenteredAction88 = cxOwner / 2 - pExtent[0] / 2;
	m_nCurrentPosition8C = m_aMetadataE0[iMetadata].m_nPosition;
	nSide = m_aMetadataE0[iMetadata].m_nSide;
	if (nSide == 0) {
		m_nCurrentSide94 = 0;
	}
	else if (nSide == 1) {
		m_nCurrentSide94 = 1;
	}
	else {
		m_nCurrentSide94 = 0;
	}
	m_xActionOrigin98 = m_xCenteredAction88;
	m_nTargetPosition9C = m_nCurrentPosition8C;
	m_nActionStateA4 = 0;
	m_nSelectedSideA0 = m_nCurrentSide94;
}
// FUNCTION: LEMBALL 0x0044db20
void GameMenuSelectorManager::StartAction(void)
{
	int i;
	int* pPosition;
	int* panOffsets;
	int nDistance;
	int nPositionOffset;

	m_ActionEventB8.m_nType00 = 0x0c;
	if (m_nCurrentPosition8C != m_nTargetPosition9C || m_nActionStateA4 != 0) {
		return;
	}
	i = 0;
	pPosition = &m_aMetadataE0[0].m_nPosition;
	do {
		if (*pPosition == m_nTargetPosition9C) {
			if (m_nSelectedSideA0 == 0) {
				m_ActionEventB8.m_nPayload08 = m_aMetadataE0[i].m_nSide0Token;
				m_fDelayedEventD8 = m_aMetadataE0[i].m_fSide0Action;
			}
			else {
				m_ActionEventB8.m_nPayload08 = m_aMetadataE0[i].m_nSide1Token;
				m_fDelayedEventD8 = m_aMetadataE0[i].m_fSide1Action;
			}
			break;
		}
		pPosition += sizeof(GameMenuSelectorMetadata) / sizeof(int);
		++i;
	} while (i < 8);
	panOffsets = m_fCompactLayout274 == 1 ? g_anMenuSelectorActionOffsetsMode1 : g_anMenuSelectorActionOffsetsMode0;
	SetFrameSelectorDuration(m_pActionLeadSelector24C, 250);
	SetFrameSelectorDuration(m_pActionMiddleSelector254, 500);
	SetFrameSelectorDuration(m_pActionTailSelector248, 500);
	if (m_nSelectedSideA0 == 0) {
		m_xActionA8 = m_xActionOrigin98 + panOffsets[6];
		m_yActionAC = m_nTargetPosition9C + panOffsets[7];
		m_xActionDestinationB0 = m_aMetadataE0[i].m_xSide0 + panOffsets[8];
		m_ActionEventB8.m_nPayload14 = m_aMetadataE0[i].m_xSide0 + panOffsets[10];
		nPositionOffset = panOffsets[11];
	}
	else {
		m_xActionA8 = m_xActionOrigin98 + panOffsets[12];
		m_yActionAC = m_nTargetPosition9C + panOffsets[13];
		m_xActionDestinationB0 = m_aMetadataE0[i].m_xSide1 + panOffsets[14];
		m_ActionEventB8.m_nPayload14 = m_aMetadataE0[i].m_xSide1 + panOffsets[16];
		nPositionOffset = panOffsets[17];
	}
	m_ActionEventB8.m_nPayload18 = m_aMetadataE0[i].m_nPosition + nPositionOffset;
	m_dwActionLastTick22C = (int) timeGetTime();
	nDistance = abs(m_xActionDestinationB0 - m_xActionA8);
	m_nActionStateA4 = 2;
	m_dwActionMotionEnd230 = m_dwActionLastTick22C + nDistance * 2;
	((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffectThunk(7);
}
// FUNCTION: LEMBALL 0x0044dce0
void LEMBALL_FASTCALL UpdateMenuSelectorAnimation(GameMenuSelectorManager* pManager)
{
	unsigned int dwNow;
	unsigned int dwEventTime;

	dwNow = (unsigned int) timeGetTime();
	if (pManager->m_nCurrentSide94 == pManager->m_nSelectedSideA0 || pManager->m_nActionStateA4 == 1) {
		if (pManager->m_nActionStateA4 == 1) {
			if ((unsigned int) pManager->m_dwSideTransitionEnd228 <= dwNow) {
				pManager->m_nActionStateA4 = 0;
				pManager->m_nCurrentSide94 = pManager->m_nSelectedSideA0;
			}
		}
		else if (pManager->m_nActionStateA4 == 2) {
			if (dwNow < (unsigned int) pManager->m_dwActionMotionEnd230) {
				int x = pManager->m_xActionA8;
				int dwPrevious = pManager->m_dwActionLastTick22C;
				int nDirection = GetIntDirectionSignThunk(pManager->m_xActionDestinationB0 - x);
				pManager->m_dwActionLastTick22C = (int) dwNow;
				pManager->m_xActionA8 = x + nDirection * ((int) (dwNow - dwPrevious) >> 1);
			}
			else {
				((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)
					->PlayVariantResourceEffectThunk(0x0e);
				pManager->m_dwActionFrameStart23C = (int) dwNow;
				pManager->m_dwActionFrameEnd240 = (int) dwNow + 500;
				pManager->m_nActionStateA4 = 3;
				SetFrameSelectorDuration(pManager->m_pCompletionSelector250, 500);
			}
		}
		else if (pManager->m_nActionStateA4 == 3) {
			dwEventTime = (unsigned int) pManager->m_dwActionFrameEnd240;
			if (pManager->m_fDelayedEventD8 != 0) {
				dwEventTime -= 0x177;
			}
			if (dwEventTime <= dwNow && pManager->m_fEventQueuedB4 != 1) {
				pManager->m_ActionEventB8.m_dwTimestamp04 = (int) timeGetTime();
				QueueMenuSelectorEvent(&pManager->m_ActionEventB8);
				pManager->m_fEventQueuedB4 = 1;
			}
			if ((unsigned int) pManager->m_dwActionFrameEnd240 <= dwNow) {
				pManager->m_nActionStateA4 = 0;
				pManager->m_fEventQueuedB4 = 0;
			}
		}
	}
	else {
		pManager->m_dwSideTransitionStart224 = (int) dwNow;
		pManager->m_dwSideTransitionEnd228 = (int) dwNow + 250;
		pManager->m_nActionStateA4 = 1;
		SetFrameSelectorDuration(pManager->m_pSideSelector244, 250);
		if (pManager->m_nSelectedSideA0 == 0) {
			((GameProgressiveVariantFrameSelectorInterface*) pManager->m_pSideSelector244)->SetDirection(-1);
		}
		else {
			((GameProgressiveVariantFrameSelectorInterface*) pManager->m_pSideSelector244)->SetDirection(1);
		}
	}
	if (pManager->m_fMovementActiveDC != 0) {
		if (dwNow < (unsigned int) pManager->m_dwMovementEnd220) {
			pManager->m_nCurrentPosition8C = ((pManager->m_nTargetPosition9C - pManager->m_nMovementStartPosition90) *
											  ((int) dwNow - pManager->m_dwMovementStart21C)) /
												 (pManager->m_dwMovementEnd220 - pManager->m_dwMovementStart21C) +
											 pManager->m_nMovementStartPosition90;
		}
		else {
			pManager->m_fMovementActiveDC = 0;
			pManager->m_nCurrentPosition8C = pManager->m_nTargetPosition9C;
		}
	}
}
// Compatibility shim preserving MSVC 4.20 compiler state.
void* LEMBALL_FASTCALL DeleteMenuSelectorEntry(GameMenuSelectorEntry* pEntry, int, int fDelete)
{
	DestroyMenuSelectorEntry(pEntry);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pEntry);
	}
	return pEntry;
}
// Compatibility shim preserving MSVC 4.20 compiler state.
void* LEMBALL_FASTCALL DeleteMenuSelectorManager(GameMenuSelectorManager* pManager, int, int fDelete)
{
	DestroyMenuSelectorManager(pManager);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pManager);
	}
	return pManager;
}
// FUNCTION: LEMBALL 0x0044e6c0
GameRectQuad* GameRectQuad::CopyFrom(const short* pRect)
{
	const short* pPosition;

	m_cx = pRect[0];
	m_cy = pRect[1];
	if (pRect != 0) {
		pPosition = pRect + 2;
	}
	else {
		pPosition = 0;
	}
	m_x = pPosition[0];
	m_y = pPosition[1];
	return this;
}
// FUNCTION: LEMBALL 0x0044e790
GameSelectorProgressOverlayChild* GameSelectorProgressOverlayChild::Construct(const short* pRect,
																			  int nPercent,
																			  void* pRenderOwner)
{
	void** pWindowOwnerVtable;
	void** pGeometryVtable;
	int i;

	ConstructWindowOwnerRenderContext();
	pWindowOwnerVtable = *(void***) this;
	InitializeGeometryOwnerRect(&m_Geometry90, 0, (const VS_XYPair*) pRect);
	pGeometryVtable = *(void***) &m_Geometry90;
	InitializePackagedSpriteRenderEntryThunk(&m_aRenderEntriesD0[0]);
	for (i = 1; i < 5; ++i) {
		InitializeStatusIndicatorPointSinkEntryThunk(&m_aRenderEntriesD0[i]);
	}
	InitializeSelectorProgressOverlayChildVtables(pWindowOwnerVtable, pGeometryVtable);
	*(void***) this = g_SelectorProgressOverlayChildVtable;
	*(void***) &m_Geometry90 = g_SelectorProgressGeometryVtable;
	m_nReserved128 = 0;
	m_cxProgress124 = pRect[0];
	m_nPercent120 = nPercent;
	m_RectC8.m_cx = pRect[0];
	m_RectC8.m_cy = pRect[1];
	m_RectC8.m_x = pRect[2];
	m_RectC8.m_y = pRect[3];
	m_nFrameInterval += 6;
	m_pRenderOwner12C = pRenderOwner;
	m_nToken130 = -1;
	((GameGeometryOwnerRectState*) &m_Geometry90)->SetEnabled(1);
	m_Geometry90.m_fDispatchPointerMove28 = 1;
	m_Geometry90.m_fDispatchOutsideRelease2C = 1;
	return this;
}
// FUNCTION: LEMBALL 0x0044e8c0
void LEMBALL_FASTCALL DestroySelectorProgressOverlayChild(void* pGeometryInterface)
{
	GameSelectorProgressOverlayChild* pChild;
	void* pRenderOwner;
	int i;

	pChild = (GameSelectorProgressOverlayChild*) ((char*) pGeometryInterface - 0x90);
	*(void***) pGeometryInterface = g_SelectorProgressGeometryVtable;
	*(void***) pChild = g_SelectorProgressOverlayChildVtable;
	pRenderOwner = pChild->m_pRenderOwner12C;
	if (*(int*) ((char*) pRenderOwner + 4) == 1) {
		RemoveGeometryOwnerChild(*(void**) ((char*) pRenderOwner + 0x1c), 0, pGeometryInterface);
	}
	for (i = 4; i > 0; --i) {
		RestoreStatusIndicatorPointSinkEntryVtable(&pChild->m_aRenderEntriesD0[i]);
	}
	RestorePackagedSpriteRenderEntryVtableA(&pChild->m_aRenderEntriesD0[0]);
	*(void***) pChild = g_pSelectorProgressBaseWindowOwnerVtable;
	DestroyWindowOwnerRenderContext(pChild);
}
// FUNCTION: LEMBALL 0x0044e940
void GameSelectorProgressOverlayWindowSlots::ReservedWindowOwnerSlot15(void)
{
	int nContext;
	int nRenderObject;

	nContext = *(int*) (*(int*) ((char*) this + 0x4c) + 0x0c);
	nRenderObject = nContext + *(int*) (*(int*) (nContext + 0x40) + 4) + 0x74;
	*(int*) nRenderObject = 1;
}
// FUNCTION: LEMBALL 0x0044e960
void GameSelectorProgressOverlayWindowSlots::InitializeWindowOwnerFromRect(short* pRect,
																		   void* pOwner,
																		   const char* pszWindowName)
{
	short* pPosition;
	void* pGeometryOwner;

	DispatchWindowOwnerRectInitialization((GamePrimaryContext*) this, 0, pRect, pOwner, pszWindowName);
	*(short*) ((char*) this + 0xc0) = pRect[0];
	*(short*) ((char*) this + 0xc2) = pRect[1];
	if (pRect != 0) {
		pPosition = pRect + 2;
	}
	else {
		pPosition = 0;
	}
	*(short*) ((char*) this + 0xc4) = pPosition[0];
	*(short*) ((char*) this + 0xc6) = pPosition[1];
	pGeometryOwner = (char*) this + 0x90;
	if (this == 0) {
		pGeometryOwner = 0;
	}
	AppendGeometryOwnerChild(*(void**) (*(int*) ((char*) this + 0x12c) + 0x1c), 0, pGeometryOwner);
}
// FUNCTION: LEMBALL 0x0044e9d0
void GameSelectorProgressOverlayWindowSlots::MoveWindowOwnerOrigin(const VS_XYPair* pPoint)
{
	typedef void(LEMBALL_STDCALL * SetAnchorProc)(const VS_XYPair*);
	((SetAnchorProc) SetWindowOwnerRenderAnchorFromPoint)(pPoint);
	*(short*) ((char*) this + 0xcc) = pPoint->m_x;
	*(short*) ((char*) this + 0xce) = pPoint->m_y;
}
// FUNCTION: LEMBALL 0x0044ea00
void GameSelectorProgressOverlayWindowSlots::ApplyWindowOwnerActivation(int nActivate)
{
	char* pObject;
	short cxFill;
	int cy;

	pObject = (char*) this;
	(void) nActivate;
	cy = (int) *(short*) (pObject + 0xca);
	cxFill = (short) (((int) *(short*) (pObject + 0xc8) * *(int*) (pObject + 0x120)) / 100);
	if (*(int*) (pObject + 0x120) == 0) {
		return;
	}

	*(int*) (pObject + 0xdc) = 0xac;
	*(short*) (pObject + 0xd4) = cxFill;
	*(short*) (pObject + 0xd6) = (short) cy;
	*(short*) (pObject + 0xd8) = 0;
	*(short*) (pObject + 0xda) = 0;
	((GameProgressRenderEntryInterface*) (pObject + 0xd0))->Queue(*(void**) (pObject + 0x4c));

	*(short*) (pObject + 0xe4) = 0;
	*(short*) (pObject + 0xe6) = 0;
	*(short*) (pObject + 0xe8) = cxFill;
	*(short*) (pObject + 0xea) = 0;
	*(int*) (pObject + 0xec) = 0xab;
	((GameProgressRenderEntryInterface*) (pObject + 0xe0))->Queue(*(void**) (pObject + 0x4c));

	*(short*) (pObject + 0xf4) = 0;
	*(short*) (pObject + 0xf6) = 0;
	*(short*) (pObject + 0xf8) = 0;
	*(short*) (pObject + 0xfa) = (short) cy;
	*(int*) (pObject + 0xfc) = 0xab;
	((GameProgressRenderEntryInterface*) (pObject + 0xf0))->Queue(*(void**) (pObject + 0x4c));

	*(short*) (pObject + 0x104) = (short) *(int*) (pObject + 0x120);
	*(short*) (pObject + 0x106) = (short) cy;
	*(short*) (pObject + 0x108) = 0;
	*(short*) (pObject + 0x10a) = (short) cy;
	*(int*) (pObject + 0x10c) = 0xbc;
	((GameProgressRenderEntryInterface*) (pObject + 0x100))->Queue(*(void**) (pObject + 0x4c));

	*(short*) (pObject + 0x114) = (short) *(int*) (pObject + 0x120);
	*(short*) (pObject + 0x116) = (short) cy;
	*(short*) (pObject + 0x118) = cxFill;
	*(short*) (pObject + 0x11a) = 0;
	*(int*) (pObject + 0x11c) = 0xbc;
	((GameProgressRenderEntryInterface*) (pObject + 0x110))->Queue(*(void**) (pObject + 0x4c));
}
// FUNCTION: LEMBALL 0x0044eb60
void GameSelectorProgressOverlayChild::SetPercent(int nPercent)
{
	struct ProgressEvent {
		short m_nType00;
		short m_nReserved02;
		int m_dwTimestamp04;
		int m_nPayload08;
		int m_nPayload0C;
		int m_nPayload10;
	};
	ProgressEvent Event;

	if (m_nPercent120 == nPercent) {
		return;
	}
	Event.m_nType00 = 0x0c;
	m_nPercent120 = nPercent;
	Event.m_dwTimestamp04 = (int) timeGetTime();
	Event.m_nPayload08 = m_nToken130;
	Event.m_nPayload0C = m_nPercent120;
	Event.m_nPayload10 = 100;
	((GameMenuSelectorEventQueueInterface*) g_pSharedRenderDispatchQueue)->QueueEvent(&Event);
}
// FUNCTION: LEMBALL 0x0044ebc0
void GameSelectorProgressGeometry::DispatchPointerMove(const VS_XYPair* pPoint)
{
	int x;
	int xMax;

	if (*(int*) ((char*) this + 0x10) == 0) {
		return;
	}
	x = (int) pPoint->m_x - (int) *(short*) ((char*) this + 0x34);
	if (x < 0) {
		x = 0;
	}
	else {
		xMax = (int) *(short*) ((char*) this + 0x38);
		if (xMax < x) {
			x = xMax;
		}
	}
	((GameSelectorProgressOverlayChild*) ((char*) this - 0x90))
		->SetPercent((x * 100) / (int) *(short*) ((char*) this + 0x30));
}
// FUNCTION: LEMBALL 0x0044ec10
void GameSelectorProgressGeometry::DispatchButtonTransition(const VS_XYPair* pPoint, int)
{
	((GameSelectorProgressGeometryDispatchInterface*) this)->DispatchPointerMove(pPoint);
}
// FUNCTION: LEMBALL 0x0044ec20
void GameSelectorProgressOverlayWindowSlots::ReservedWindowOwnerSlot23(void)
{
}
// FUNCTION: LEMBALL 0x0044ec30
GameSelectorProgressOverlayOwner* GameSelectorProgressOverlayOwner::Construct(const VS_XYPair* pPoint,
																			  void* pRenderOwner,
																			  int nResourceId,
																			  const short* pProgressRect,
																			  int nPercent)
{
	GameSelectorProgressOverlayChild* pChild;
	void** pOwnerVtable;
	void** pDeleteVtable;
	short aRect[4];

	ConstructAlignedZrleChildOverlayFromPoint(this, 0, pPoint, pRenderOwner, nResourceId, 3);
	pOwnerVtable = *(void***) this;
	pDeleteVtable = *(void***) ((char*) this + 0x90);
	InitializeSelectorProgressOverlayOwnerVtables(pOwnerVtable, pDeleteVtable);
	*(void***) this = g_SelectorProgressOverlayOwnerVtable;
	*(void***) ((char*) this + 0x90) = g_SelectorProgressOverlayOwnerDeleteVtable;
	*(short*) ((char*) this + 0x130) = 0;
	*(short*) ((char*) this + 0x132) = 0;
	aRect[0] = pProgressRect[0];
	aRect[1] = pProgressRect[1];
	aRect[2] = pProgressRect[2] + pPoint->m_x;
	aRect[3] = pProgressRect[3] + pPoint->m_y;
	*(short*) ((char*) this + 0x130) = pProgressRect[2];
	*(short*) ((char*) this + 0x132) = pProgressRect[3];
	pChild = (GameSelectorProgressOverlayChild*) AllocateVSMemBlock(sizeof(GameSelectorProgressOverlayChild));
	if (pChild != 0) {
		pChild = pChild->Construct(pProgressRect, nPercent, pRenderOwner);
	}
	*(GameSelectorProgressOverlayChild**) ((char*) this + 0x134) = pChild;
	((GameSelectorProgressOverlayWindowSlots*) pChild)->InitializeWindowOwnerFromRect(aRect, pRenderOwner, 0);
	return this;
}
// FUNCTION: LEMBALL 0x0044ed20
void LEMBALL_FASTCALL DestroySelectorProgressOverlayOwner(void* pDeleteInterface)
{
	typedef void*(LEMBALL_FASTCALL * DeleteProc)(void* pObject, int nUnusedEdx, unsigned int fDelete);
	GameSelectorProgressOverlayChild* pChild;
	void* pOwner;

	pOwner = (char*) pDeleteInterface - 0x90;
	*(void***) pDeleteInterface = g_SelectorProgressOverlayOwnerDeleteVtable;
	*(void***) pOwner = g_SelectorProgressOverlayOwnerVtable;
	pChild = *(GameSelectorProgressOverlayChild**) ((char*) pOwner + 0x134);
	if (*(int*) ((char*) pChild + 4) == 1) {
		pChild->DestroyWindowOwnerHwnd();
	}
	if (pChild != 0) {
		void* pChildDeleteInterface = (char*) pChild + 0x90;
		((DeleteProc) (*(void***) pChildDeleteInterface)[0])(pChildDeleteInterface, 0, 1);
	}
	DestroyAlignedZrleChildOverlay(pDeleteInterface);
}
// FUNCTION: LEMBALL 0x0044ed70
void GameSelectorProgressOverlayOwner::SetAnchorAndNotifyChild(const VS_XYPair* pPoint)
{
	VS_XYPair Point;

	*(short*) ((char*) this + 0xc4) -= *(short*) ((char*) this + 0x18);
	*(int*) ((char*) this + 0xd8) = 1;
	*(short*) ((char*) this + 0xc6) -= *(short*) ((char*) this + 0x1a);
	SetWindowOwnerRenderAnchorFromPoint(this, 0, pPoint);
	*(short*) ((char*) this + 0xc4) += *(short*) ((char*) this + 0x18);
	*(short*) ((char*) this + 0xc6) += *(short*) ((char*) this + 0x1a);
	Point.m_x = *(short*) ((char*) this + 0xdc) + *(short*) ((char*) this + 0x130);
	Point.m_y = *(short*) ((char*) this + 0xde) + *(short*) ((char*) this + 0x132);
	((VS_WindowOwnerBase*) *(void**) ((char*) this + 0x134))->MoveWindowOwnerOrigin(&Point);
	Point.m_x = *(short*) ((char*) this + 0x130) + pPoint->m_x;
	Point.m_y = pPoint->m_y + *(short*) ((char*) this + 0x132);
	((VS_WindowOwnerBase*) *(void**) ((char*) this + 0x134))->MoveWindowOwnerOrigin(&Point);
}
// FUNCTION: LEMBALL 0x0044eff0
void* LEMBALL_FASTCALL DeleteSelectorProgressOverlayChildAdjusted(void* pGeometryInterface, int, unsigned int fDelete)
{
	char* pChild;

	pChild = (char*) pGeometryInterface;
	DestroySelectorProgressOverlayChild(pGeometryInterface);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pChild - 0x90);
	}
	return pChild - 0x90;
}
// FUNCTION: LEMBALL 0x0044f030
void* LEMBALL_FASTCALL DeleteSelectorProgressOverlayOwnerAdjusted(void* pDeleteInterface, int, unsigned int fDelete)
{
	char* pOwner;

	pOwner = (char*) pDeleteInterface;
	DestroySelectorProgressOverlayOwner(pDeleteInterface);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pOwner - 0x90);
	}
	return pOwner - 0x90;
}
// FUNCTION: LEMBALL 0x0044f070
GameQueuedVariantRangeEntry* GameQueuedVariantRangeEntry::Construct(void* pRenderOwner,
																	void* pRenderContext,
																	int x,
																	int y,
																	int nResourceId,
																	int nActivationMode,
																	int nFirstResource,
																	int nLastResource,
																	int nReserved,
																	int nToken,
																	int* pSelection,
																	int nCommand)
{
	typedef void(LEMBALL_FASTCALL * LoadProc)(void*, int, int);

	InitializeRenderQueueNodeBase(this);
	m_pVtable00 = g_QueuedVariantRangeEntryVtable;
	m_nReserved10 = 0;
	m_nReserved12 = 0;
	m_nReserved14 = 0;
	m_nReserved16 = 0;
	m_pRenderOwner24 = pRenderOwner;
	m_pRenderContext20 = pRenderContext;
	m_cResources34 = nLastResource - nFirstResource + 1;
	if (m_cResources34 < 2) {
		m_cResources34 = 1;
	}
	m_nFirstResource3C = nFirstResource;
	m_nToken2C = nToken;
	m_nLastResource38 = nLastResource;
	m_x18 = x;
	m_y1C = y;
	m_nSelectedResource40 = pSelection == 0 ? 0 : *pSelection;
	if (nCommand != -1) {
		m_nCommand30 = nCommand;
	}
	m_pSelection44 = pSelection;
	m_nActivationMode28 = nActivationMode;
	m_fEnabled54 = 1;
	(void) nReserved;
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->RegisterOrderedRenderDispatchClient(this, 0);
	((LoadProc) 0x00401bd1)(this, 0, nResourceId);
	return this;
}

// FUNCTION: LEMBALL 0x0044f130
void LEMBALL_FASTCALL DestroyQueuedVariantRangeEntryAuto(void* pObject)
{
	GameQueuedVariantRangeEntry* pEntry;

	pEntry = (GameQueuedVariantRangeEntry*) pObject;
	pEntry->m_pVtable00 = g_QueuedVariantRangeEntryVtable;
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->UnregisterOrderedRenderDispatchClient(pEntry, 0);
	ReleaseQueuedVariantRangeResources(pEntry);
	pEntry->m_pVtable00 = g_GAME_RenderQueueNodeVtableSlots;
}
// FUNCTION: LEMBALL 0x0044f160
int LEMBALL_FASTCALL HandleQueuedVariantRangeEntryEvent(GameQueuedVariantRangeEntry* pEntry,
														int,
														GameMenuSelectorEvent* pEvent)
{
	if (pEvent->m_nPayload08 != pEntry->m_nToken2C) {
		return 0;
	}
	if (pEvent->m_nType00 == 0x0b) {
		((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffect(0x25);
		return 0;
	}
	if (pEvent->m_nType00 == 0x0c && pEntry->m_nActivationMode28 == 1) {
		QueueQueuedVariantCommand(pEntry->m_nCommand30);
	}
	return 0;
}
struct QueuedVariantOverlayDeleteInterface {
	virtual void Delete(int fDelete) = 0;
};

// FUNCTION: LEMBALL 0x0044f370
void LEMBALL_FASTCALL ReleaseQueuedVariantRangeResources(GameQueuedVariantRangeEntry* pEntry)
{
	int nOffset;
	int i;

	if (pEntry->m_pOverlay4C != 0) {
		QueuedVariantOverlayDeleteInterface* pDelete =
			(QueuedVariantOverlayDeleteInterface*) ((char*) pEntry->m_pOverlay4C + 0x90);
		pDelete->Delete(1);
	}
	i = 0;
	if (pEntry->m_cResources34 > 0) {
		nOffset = 0;
		do {
			ReleaseTypedResourceObjectReference(*(void**) ((char*) pEntry->m_ppLoadedResources50 + nOffset));
			nOffset += sizeof(void*);
			++i;
		} while (i < pEntry->m_cResources34);
	}
	FreeVSMemBlock(pEntry->m_ppLoadedResources50);
	pEntry->m_ppLoadedResources50 = 0;
}
// FUNCTION: LEMBALL 0x0044f420
void GameQueuedVariantChildContextView::MarkRenderObjectDirty(void)
{
	int nContext;
	int nRenderObject;

	nContext = *(int*) (*(int*) ((char*) this + 0x4c) + 0x0c);
	nRenderObject = nContext + *(int*) (*(int*) (nContext + 0x40) + 4) + 0x74;
	*(int*) nRenderObject = 1;
}
// FUNCTION: LEMBALL 0x0044f440
void* LEMBALL_FASTCALL ConstructQueuedVariantChildSlotManager(void* pObject,
															  int,
															  void* pRenderOwner,
															  void* pRenderContext,
															  int,
															  int nLayoutMode,
															  int nNavigationMode)
{
	GameQueuedVariantChildSlotManager* pManager;
	int i;

	pManager = (GameQueuedVariantChildSlotManager*) pObject;
	memset(pManager, 0, sizeof(*pManager));
	InitializeRenderQueueNodeBase(pManager);
	pManager->m_pVtable00 = g_QueuedVariantChildSlotManagerVtable;
	((CAnimsManager*) pManager->m_abVariantRenderManager10)
		->Construct((int) (unsigned long) pRenderContext, 0x2b6, 1, 1, 0, 0);
	*(void***) pManager->m_abPointSinkFC = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
	*(void***) pManager->m_abPointSinkFC = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage + 4;
	*(void***) pManager->m_abCallback118 = g_MenuSelectorFrameSelectorCtorVtable;
	*(int*) (pManager->m_abCallback118 + 4) = 1;
	*(void***) pManager->m_abCallback118 = g_MenuSelectorCurrentFrameCallbackVtable;
	*(void***) pManager->m_abVariantRenderManager10 = g_QueuedVariantChildSlotManagerRenderManagerVtable;
	pManager->m_pRenderContext10C = pRenderContext;
	pManager->m_pRenderOwner110 = pRenderOwner;
	pManager->m_nNextToken114 = (int) 0xabcd0000U;
	pManager->m_nLayoutMode130 = nLayoutMode;
	pManager->m_nNavigationMode134 = nNavigationMode;
	pManager->m_fCompactLayout138 = nLayoutMode == 1;
	pManager->m_fInputEnabled144 = 1;
	for (i = 0; i < 4; ++i) {
		pManager->m_apChildrenAC[i] = 0;
		pManager->m_aSlotsBC[i].m_fActive00 = 0;
	}
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->RegisterOrderedRenderDispatchClient(pManager, 0);
	g_nQueuedVariantChildSlotManagerModeSelectedResourceId = nLayoutMode == 1 ? 0x183 : 0x156;
	((CAnimsManager*) pManager->m_abVariantRenderManager10)
		->LoadAnims(g_nQueuedVariantChildSlotManagerModeSelectedResourceId);
	return pManager;
}
// FUNCTION: LEMBALL 0x0044f590
void LEMBALL_FASTCALL InitializeQueuedVariantChildContext(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * InitializeRectProc)(void*, int, short*, void*, int);
	GameQueuedVariantChildSlotManager* pManager;
	short Rect[4];
	void* pContext;

	pManager = (GameQueuedVariantChildSlotManager*) pObject;
	pContext = AllocateVSMemBlock(0x90);
	if (pContext == 0) {
		pManager->m_pChildContext128 = 0;
		pManager->m_pChildRenderContext12C = 0;
		return;
	}
	ConstructQueuedVariantChildContextObject(pContext);
	pManager->m_pChildContext128 = pContext;
	Rect[0] = *(short*) ((char*) pManager->m_pRenderOwner110 + 8);
	Rect[1] = *(short*) ((char*) pManager->m_pRenderOwner110 + 10);
	Rect[2] = 0;
	Rect[3] = 0;
	((InitializeRectProc) (*(void***) pContext)[1])(pContext, 0, Rect, pManager->m_pRenderOwner110, 0);
	pManager->m_pChildRenderContext12C = *(void**) ((char*) pContext + 0x4c);
}
// FUNCTION: LEMBALL 0x0044f610
void LEMBALL_FASTCALL DestroyQueuedVariantChildSlotManager(void* pObject)
{
	GameQueuedVariantChildSlotManager* pManager;
	int i;
	int nResourceId;

	pManager = (GameQueuedVariantChildSlotManager*) pObject;
	pManager->m_pVtable00 = g_QueuedVariantChildSlotManagerVtable;
	((GameRenderDispatchQueue*) g_pSharedRenderDispatchQueue)->UnregisterOrderedRenderDispatchClient(pManager, 0);
	for (i = 0; i < 4; ++i) {
		if (pManager->m_apChildrenAC[i] != 0) {
			DeleteQueuedVariantRangeEntry(pManager->m_apChildrenAC[i], 0, 1);
			pManager->m_apChildrenAC[i] = 0;
		}
	}
	nResourceId = pManager->m_nLayoutMode130 == 1 ? 0x183 : 0x156;
	((CAnimsManager*) pManager->m_abVariantRenderManager10)->UnLoadAnims(nResourceId);
	if (pManager->m_pChildContext128 != 0) {
		typedef void(LEMBALL_FASTCALL * DetachProc)(void*, int);
		if (*(int*) ((char*) pManager->m_pChildContext128 + 4) == 1) {
			((DetachProc) (*(void***) pManager->m_pChildContext128)[29])(pManager->m_pChildContext128, 0);
		}
		DestroyWindowOwnerRenderContext(pManager->m_pChildContext128);
		FreeVSMemBlock(pManager->m_pChildContext128);
		pManager->m_pChildContext128 = 0;
		pManager->m_pChildRenderContext12C = 0;
	}
	DestroyVariantRenderManager(pManager->m_abVariantRenderManager10);
	pManager->m_pVtable00 = g_pNonZrleVariantRenderEntryInitializeVtable;
}
// FUNCTION: LEMBALL 0x0044f6c0
int LEMBALL_FASTCALL HandleQueuedVariantChildSlotManagerEvent(GameQueuedVariantChildSlotManager* pManager,
															  int,
															  GameMenuSelectorEvent* pEvent)
{
	int iChild;

	if (pManager->m_fInputEnabled144 == 0) {
		return 0;
	}
	if (pEvent->m_nType00 != 4) {
		++pManager->m_cUnhandledEvents0C;
		return 0;
	}
	iChild = pManager->m_iSelectedChildA8;
	if (pEvent->m_nPayload08 == 1 || pEvent->m_nPayload08 == 3) {
		if (iChild > 0) {
			pManager->SelectChildSlot(iChild - 1);
		}
		((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffect(0x1b);
		return 1;
	}
	if (pEvent->m_nPayload08 == 2 || pEvent->m_nPayload08 == 4) {
		if (iChild + 1 < pManager->m_cChildren80) {
			pManager->SelectChildSlot(iChild + 1);
		}
		((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffect(0x1b);
		return 1;
	}
	if (pEvent->m_nPayload08 == 0x1f || pEvent->m_nPayload08 == 0x22 || pEvent->m_nPayload08 == 0x4c) {
		QueueQueuedVariantCommand(pManager->m_aSlotsBC[pManager->m_iSelectedChildA8].m_nToken0C == 0
									  ? 0
									  : pManager->m_apChildrenAC[pManager->m_iSelectedChildA8]->m_nCommand30);
	}
	return 0;
}
// FUNCTION: LEMBALL 0x0044f8c0
void LEMBALL_FASTCALL AppendQueuedVariantChildSlot(void* pObject,
												   int,
												   int x,
												   int y,
												   const int* panResourceIds,
												   int nActivationMode,
												   int nFirstResource,
												   int nLastResource,
												   int,
												   int* pSelection,
												   int nCommand)
{
	GameQueuedVariantChildSlotManager* pManager;
	GameQueuedVariantRangeEntry* pEntry;
	int iChild;
	int nToken;

	pManager = (GameQueuedVariantChildSlotManager*) pObject;
	iChild = pManager->m_cChildren80;
	if (iChild >= 4) {
		return;
	}
	nToken = ++pManager->m_nNextToken114;
	pEntry = (GameQueuedVariantRangeEntry*) AllocateVSMemBlock(sizeof(GameQueuedVariantRangeEntry));
	if (pEntry != 0) {
		ConstructQueuedVariantRangeEntry(pEntry,
										 pManager->m_pRenderOwner110,
										 pManager->m_pRenderContext10C,
										 x,
										 y,
										 panResourceIds,
										 nActivationMode,
										 nFirstResource,
										 nLastResource,
										 nToken,
										 pSelection,
										 nCommand);
	}
	pManager->m_apChildrenAC[iChild] = pEntry;
	pManager->m_aSlotsBC[iChild].m_fActive00 = pEntry != 0;
	pManager->m_aSlotsBC[iChild].m_x04 = x;
	pManager->m_aSlotsBC[iChild].m_y08 = y;
	pManager->m_aSlotsBC[iChild].m_nToken0C = nToken;
	++pManager->m_cChildren80;
}
// FUNCTION: LEMBALL 0x0044f9d0
void GameQueuedVariantChildSlotManager::RebuildChildSourceRects(int fForce)
{
	GameQueuedVariantRangeEntry** ppEntry;
	int cRemaining;

	ppEntry = m_apChildrenAC;
	cRemaining = 4;
	do {
		if (*ppEntry != 0) {
			(*ppEntry)->RebuildSourceRect(fForce);
		}
		++ppEntry;
		--cRemaining;
	} while (cRemaining != 0);
}

void LEMBALL_FASTCALL RebuildQueuedVariantChildSlotSourceRects(void* pObject, int, int fForce)
{
	((GameQueuedVariantChildSlotManager*) pObject)->RebuildChildSourceRects(fForce);
}
// FUNCTION: LEMBALL 0x0044fae0
void GameQueuedVariantChildSlotManager::SelectPreviousChildSlot(void)
{
	int iChild;

	iChild = m_iSelectedChildA8 - 1;
	if (iChild >= 0) {
		m_iSelectedChildA8 = iChild;
		SelectChildSlot(iChild);
	}
}
// FUNCTION: LEMBALL 0x0044fb00
void GameQueuedVariantChildSlotManager::SelectNextChildSlot(void)
{
	int iChild;

	iChild = m_iSelectedChildA8 + 1;
	if (iChild < m_cChildren80) {
		m_iSelectedChildA8 = iChild;
		SelectChildSlot(iChild);
	}
}
// FUNCTION: LEMBALL 0x0044fb20
void GameQueuedVariantChildSlotManager::SelectChildSlot(int iChild)
{
	char* pSlotOffset;
	int x;
	int y;
	DWORD dwNow;

	pSlotOffset = (char*) this + iChild * 0x10;
	x = *(int*) (pSlotOffset + 0xc0);
	m_ActionEvent84.m_nPayload14 = x;
	y = *(int*) (pSlotOffset + 0xc4);
	m_ActionEvent84.m_nPayload18 = y;
	m_nCurrentChildA0 = x;
	m_nReservedA4 = y;
	dwNow = timeGetTime();
	m_dwSelectionEnd140 = (int) dwNow;
	m_dwSelectionStart13C = (int) dwNow;
	m_iSelectedChildA8 = iChild;
}

// FUNCTION: LEMBALL 0x0044fb70
void GameQueuedVariantChildSlotManager::DispatchSelectedChildSlotEvent(void)
{
	m_ActionEvent84.m_nType00 = 0x0c;
	m_ActionEvent84.m_dwTimestamp04 = (int) timeGetTime();
	m_ActionEvent84.m_nPayload08 = m_aSlotsBC[m_iSelectedChildA8].m_nToken0C;
	((GameMenuSelectorEventQueueInterface*) g_pSharedRenderDispatchQueue)->QueueEvent(&m_ActionEvent84);
	((GameVariantResourceEntryManager*) g_pVariantResourceEntryManager)->PlayVariantResourceEffectThunk(0x0e);
}

void LEMBALL_FASTCALL SelectQueuedVariantChildSlot(void* pObject, int, int iChild)
{
	((GameQueuedVariantChildSlotManager*) pObject)->SelectChildSlot(iChild);
}

// FUNCTION: LEMBALL 0x0044fc20
void GameQueuedVariantChildSlotManager::RefreshAllRangeSelections(void)
{
	GameQueuedVariantRangeEntry** ppEntry;
	int i;

	i = 0;
	if (m_cChildren80 > 0) {
		ppEntry = m_apChildrenAC;
		do {
			if (*ppEntry != 0) {
				RefreshQueuedVariantRangeSelection(*ppEntry);
			}
			++ppEntry;
			++i;
		} while (i < m_cChildren80);
	}
}
// FUNCTION: LEMBALL 0x0044fc50
void GameQueuedVariantChildSlotManager::RefreshChildByToken(int nToken)
{
	GameQueuedVariantRangeEntry** ppEntry;
	int cChildren;
	int i;

	cChildren = m_cChildren80;
	i = 0;
	if (cChildren > 0) {
		ppEntry = m_apChildrenAC;
		do {
			if (*ppEntry != 0 && (*ppEntry)->m_nCommand30 == nToken) {
				RefreshQueuedVariantRangeSelection(m_apChildrenAC[i]);
				return;
			}
			++ppEntry;
			++i;
		} while (i < cChildren);
	}
}

void LEMBALL_FASTCALL RefreshQueuedVariantChildByToken(void* pObject, int, int nToken)
{
	((GameQueuedVariantChildSlotManager*) pObject)->RefreshChildByToken(nToken);
}

// Compatibility shim preserving MSVC 4.20 compiler state.
void* LEMBALL_FASTCALL DeleteQueuedVariantRangeEntry(GameQueuedVariantRangeEntry* pEntry, int, int fDelete)
{
	DestroyQueuedVariantRangeEntryAuto(pEntry);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pEntry);
	}
	return pEntry;
}
// FUNCTION: LEMBALL 0x00467170
int LEMBALL_FASTCALL GetTimedAnimFrameNo(GameProgressiveVariantFrameSelector* pSelector)
{
	unsigned int dwNow;
	unsigned int iFrame;

	dwNow = (unsigned int) pSelector->m_dwFixedTime;
	if (dwNow == 0xffffffffU) {
		dwNow = (unsigned int) timeGetTime();
	}
	dwNow -= (unsigned int) pSelector->m_dwStartTime;
	iFrame = dwNow % (unsigned int) pSelector->m_dwDuration;
	iFrame = iFrame * (unsigned int) pSelector->m_cFrames;
	iFrame /= (unsigned int) pSelector->m_dwDuration;
	if (pSelector->m_nDirection != 1) {
		iFrame = (unsigned int) pSelector->m_cFrames - iFrame - 1;
	}
	return (int) iFrame;
}
// FUNCTION: LEMBALL 0x004671e0
void LEMBALL_FASTCALL StartPlayThruAnim(GameProgressiveVariantFrameSelector* pSelector, int, int dwDuration)
{
	pSelector->m_dwDuration = dwDuration;
	if (pSelector->m_dwFixedTime == -1) {
		pSelector->m_dwStartTime = (int) timeGetTime();
	}
	else {
		pSelector->m_dwStartTime = pSelector->m_dwFixedTime;
	}
}
// FUNCTION: LEMBALL 0x00467210
int LEMBALL_FASTCALL GetPlayThruAnimFrameNo(GameProgressiveVariantFrameSelector* pSelector)
{
	unsigned int dwElapsed;
	int* pFrames;
	unsigned int iFrame;
	unsigned int dwDuration;

	if (pSelector->m_dwFixedTime == -1) {
		dwElapsed = (unsigned int) timeGetTime() - (unsigned int) pSelector->m_dwStartTime;
	}
	else {
		dwElapsed = (unsigned int) (pSelector->m_dwFixedTime - pSelector->m_dwStartTime);
	}
	dwDuration = (unsigned int) pSelector->m_dwDuration;
	pFrames = &pSelector->m_cFrames;
	if (dwElapsed < dwDuration) {
		iFrame = ((unsigned int) *pFrames * dwElapsed) / dwDuration;
	}
	else {
		iFrame = (unsigned int) *pFrames - 1;
	}
	if (pSelector->m_nDirection != 1) {
		iFrame = (unsigned int) *pFrames - iFrame - 1;
	}
	return (int) iFrame;
}

// FUNCTION: LEMBALL 0x004675d0
void LEMBALL_FASTCALL compute_max_variant_render_frame_extent(void* pView, int nUnused, short* pOut, int nFrame)
{
	int iVariant;
	int cVariants;
	short sExtent[2];
	short sMaxW;
	short sMaxH;

	(void) nUnused;
	sMaxW = 0;
	sMaxH = 0;
	cVariants = ((int(__fastcall*)(void*, int)) 0x467540)(pView, nFrame);
	iVariant = 0;
	while (cVariants > iVariant) {
		((void(__fastcall*)(void*, int, short*, int, int)) 0x467570)(pView, 0, sExtent, iVariant, nFrame);
		if (sExtent[0] > sMaxW) {
			sMaxW = sExtent[0];
		}
		if (sExtent[1] > sMaxH) {
			sMaxH = sExtent[1];
		}
		iVariant++;
		cVariants = ((int(__fastcall*)(void*, int)) 0x467540)(pView, nFrame);
	}
	pOut[0] = sMaxW;
	pOut[1] = sMaxH;
}

class CPVGWnd;

class CPVButton {
public:
	unsigned char m_abState[0x104];

	CPVButton(const CVSRect& Rect, CPVGWnd* pParent);
};

typedef char CPVButtonSizeCheck[sizeof(CPVButton) == 0x104 ? 1 : -1];

// FUNCTION: LEMBALL 0x00467c10
CPVButton::CPVButton(const CVSRect& Rect, CPVGWnd* pParent)
{
	VS_XYPair localRect[2];
	const short* pPosition;
	char* pEntry;
	int i;

	((GameWindowOwnerRenderContext*) this)->ConstructWindowOwnerRenderContext();
	localRect[0].m_x = Rect.width;
	localRect[0].m_y = Rect.height;
	localRect[1].m_x = 0;
	localRect[1].m_y = 0;
	InitializeGeometryOwnerRect((char*) this + 0x90, 0, localRect);
	*(short*) ((char*) this + 0xdc) = 0;
	*(short*) ((char*) this + 0xde) = 0;
	pEntry = (char*) this + 0xe4;
	for (i = 0; i >= 0; --i) {
		InitializeHelperLocalRectUpdateEntry(pEntry);
		pEntry += 0x10;
	}
	*(void***) this = g_ResourceGeometryChildVtable;
	*(short*) ((char*) this + 0x102) = 0;
	*(void***) ((char*) this + 0x90) = g_ResourceGeometryChildDeleteVtable;
	*(short*) ((char*) this + 0x100) = 0;
	pPosition = (&Rect == 0) ? 0 : ((const short*) &Rect + 2);
	*(short*) ((char*) this + 0xdc) = pPosition[0];
	*(short*) ((char*) this + 0xde) = pPosition[1];
	*(CPVGWnd**) ((char*) this + 0xc8) = pParent;
	InitializeSharedSpriteOwnerState(this);
}

struct AlignedZrleChildOverlayRectInitializer {
	virtual void Reserved00(void) = 0;
	virtual void InitializeFromRect(short* pRect, void* pOwner, const char* pszName) = 0;
};

// FUNCTION: LEMBALL 0x00468410
void* LEMBALL_FASTCALL ConstructAlignedZrleChildOverlayFromRect(void* pObject,
																int nUnusedEdx,
																const CVSRect* pRect,
																CPVGWnd* pParent,
																int nResourceId,
																int fAlignment)
{
	char* pBytes;
	char* pEntry;
	short aRect[4];
	int i;

	(void) nUnusedEdx;
	pBytes = (char*) new (pObject) CPVButton(*pRect, pParent);
	*(short*) (pBytes + 0x116) = 0;
	*(short*) (pBytes + 0x114) = 0;
	*(short*) (pBytes + 0x11a) = 0;
	*(short*) (pBytes + 0x118) = 0;
	*(int*) (pBytes + 0x108) = 0;
	*(int*) (pBytes + 0x104) = 0;
	pEntry = pBytes + 0x128;
	for (i = 0; i >= 0; --i) {
		InitializeTinyPointSinkHelper(pEntry);
		pEntry += 8;
	}
	*(void***) pBytes = g_AlignedZrleChildOverlayVtable;
	*(void***) (pBytes + 0x90) = g_AlignedZrleChildOverlayDeleteVtable;
	*(int*) (pBytes + 0x110) = fAlignment;
	*(int*) (pBytes + 0x10c) = nResourceId;
	ComputeAlignedZrleChildLayout(pBytes);
	aRect[0] = *(short*) (pBytes + 0xc0);
	aRect[1] = *(short*) (pBytes + 0xc2);
	aRect[2] = *(short*) (pBytes + 0xdc);
	aRect[3] = *(short*) (pBytes + 0xde);
	((AlignedZrleChildOverlayRectInitializer*) pBytes)->InitializeFromRect(aRect, *(void**) (pBytes + 0xc8), "Button");
	*(short*) (pBytes + 0xc4) += *(short*) (pBytes + 0x18);
	*(short*) (pBytes + 0xc6) += *(short*) (pBytes + 0x1a);
	AppendGeometryOwnerChild(*(void**) (*(char**) (pBytes + 0xc8) + 0x1c), 0, pBytes + 0x90);
	return pObject;
}

// FUNCTION: LEMBALL 0x00467cd0
void* LEMBALL_FASTCALL ConstructResourceGeometryChildBase(void* pObject, int nUnusedEdx, void* pRenderOwner)
{
	void** pWindowOwnerVtable;
	void** pGeometryOwnerVtable;

	(void) nUnusedEdx;
	((GameWindowOwnerRenderContext*) pObject)->ConstructWindowOwnerRenderContext();
	pWindowOwnerVtable = *(void***) pObject;
	InitializeEmptyGeometryOwnerRect((char*) pObject + 0x90);
	pGeometryOwnerVtable = *(void***) ((char*) pObject + 0x90);
	InitializeResourceGeometryChildVtables(pWindowOwnerVtable, pGeometryOwnerVtable);
	*(short*) ((char*) pObject + 0xdc) = 0;
	*(short*) ((char*) pObject + 0xde) = 0;
	InitializeHelperLocalRectUpdateEntry((char*) pObject + 0xe4);
	*(short*) ((char*) pObject + 0x100) = 0;
	*(short*) ((char*) pObject + 0x102) = 0;
	*(void***) pObject = g_ResourceGeometryChildVtable;
	*(void***) ((char*) pObject + 0x90) = g_ResourceGeometryChildDeleteVtable;
	*(void**) ((char*) pObject + 0xc8) = pRenderOwner;
	InitializeSharedSpriteOwnerState(pObject);
	return pObject;
}
// FUNCTION: LEMBALL 0x00467d50
void LEMBALL_FASTCALL InitializeSharedSpriteOwnerState(void* pObject)
{
	void** ppFinalize;

	*(int*) ((char*) pObject + 0xd8) = 1;
	*(int*) ((char*) pObject + 0xf8) = 1;
	*(int*) ((char*) pObject + 0xbc) = 1;
	*(int*) ((char*) pObject + 0xd0) = 0;
	*(int*) ((char*) pObject + 0xd4) = 0;
	*(int*) ((char*) pObject + 0xfc) = 0;
	ppFinalize = (void**) AllocateVSMemBlock(sizeof(void*));
	if (ppFinalize != 0) {
		*ppFinalize = g_pQueuedRenderPointSinkFinalizeThunk;
	}
	*(void**) ((char*) pObject + 0xe0) = ppFinalize;
	*(int*) ((char*) pObject + 0x50) = 2;
	*(int*) ((char*) pObject + 0xf4) = 0;
	*(int*) ((char*) pObject + 0xcc) = 0;
}
// FUNCTION: LEMBALL 0x00467dd0
void LEMBALL_FASTCALL DestroyResourceGeometryChild(void* pChildOwner)
{
	typedef void(LEMBALL_FASTCALL * DeleteProc)(void* pObject, int nUnusedEdx, unsigned int fDelete);
	char* pObject;
	void* pOwnedEntry;

	pObject = (char*) pChildOwner - 0x90;
	*(void***) pObject = g_ResourceGeometryChildVtable;
	*(void***) pChildOwner = g_ResourceGeometryChildDeleteVtable;
	if (*(int*) (*(int*) (pObject + 0xc8) + 4) == 1) {
		DetachGeometryChildAndDestroyWindowOwner(pObject);
	}
	pOwnedEntry = *(void**) (pObject + 0xe0);
	if (pOwnedEntry != 0) {
		((DeleteProc) (*(void***) pOwnedEntry)[0])(pOwnedEntry, 0, 1);
	}
	RestorePackagedSpriteRenderEntryVtableC(pObject + 0xe4);
	*(void***) pObject = g_pWindowOwnerRenderContextVtable;
	DestroyWindowOwnerRenderContext(pObject);
}
// FUNCTION: LEMBALL 0x00467e50
void LEMBALL_FASTCALL RefreshCompactZrleOwnerFromResourceHelper(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * VoidProc)(void* pObject);
	int nRenderObject;
	int nZero;

	nRenderObject = ResolveResourceGeometryRenderObject(pObject);
	((VoidProc) (*(void***) nRenderObject)[14])((void*) nRenderObject);
	nZero = 0;
	if (*(int*) ((char*) pObject + 0xd8) == 0) {
		*(int*) ((char*) pObject + 0xf0) = nZero;
	}
	else {
		--*(int*) ((char*) pObject + 0xd8);
		nRenderObject = ResolveResourceGeometryRenderObject(pObject);
		*(short*) ((char*) pObject + 0xe8) = *(short*) (nRenderObject + 0x14);
		*(short*) ((char*) pObject + 0xea) = *(short*) (nRenderObject + 0x16);
		*(short*) ((char*) pObject + 0xec) = (short) nZero;
		*(short*) ((char*) pObject + 0xee) = (short) nZero;
		*(int*) ((char*) pObject + 0xf0) = 0x10000;
		nRenderObject = ResolveResourceGeometryRenderObject(pObject);
		*(int*) (nRenderObject + 0x38) = 1;
	}
	((GameResourceGeometryChildContext*) ((char*) pObject + 0xe4))
		->BindResourceContext(*(void**) ((char*) pObject + 0x4c));
}
// FUNCTION: LEMBALL 0x00467ef0
void LEMBALL_FASTCALL RefreshCompactZrleOwnerAfterFrameChange(void* pObject)
{
	if (*(int*) ((char*) pObject + 0xd0) != *(int*) ((char*) pObject + 0xd4)) {
		*(int*) ((char*) ResolveResourceGeometryRenderObject(pObject) + 0x38) = 1;
		*(int*) ((char*) pObject + 0xd4) = *(int*) ((char*) pObject + 0xd0);
	}
	RefreshCompactZrleOwnerFromResourceHelper(pObject);
}
// FUNCTION: LEMBALL 0x00467f30
void GameCompactZrleOverlay::RebuildSourceRect(int fForce)
{
	short Rect[4];
	int fPrevious;
	if (*(int*) ((char*) this + 0xfc) == 0 || fForce != 0) {
		Rect[0] = *(short*) ((char*) this + 8);
		fPrevious = *(int*) ((char*) this + 0xf8);
		*(int*) ((char*) this + 0xf8) = 1;
		Rect[1] = *(short*) ((char*) this + 10);
		Rect[2] = 0;
		Rect[3] = 0;
		((GameCompactZrleOverlayGeometryInterface*) this)->SetSourceRect(Rect);
		*(int*) ((char*) this + 0xf8) = fPrevious;
	}
	*(int*) ((char*) this + 0xfc) = 0;
}
// FUNCTION: LEMBALL 0x00467fa0
void LEMBALL_FASTCALL HandleSpriteOwnerPointerEnter(void* pGeometryOwner)
{
	void* pObject;

	if (*(int*) ((char*) pGeometryOwner + 0x10) != 0 || *(int*) ((char*) pGeometryOwner + 0x1c) != 0) {
		*(int*) ((char*) pGeometryOwner + 0x40) = 1;
	}
	pObject = (char*) pGeometryOwner - 0x90;
	NotifySpriteOwnerVariantCEvent0D(pObject);
	((void(LEMBALL_FASTCALL*)(void*))(*(void***) pObject)[50])(pObject);
}
// FUNCTION: LEMBALL 0x00467fd0
void LEMBALL_FASTCALL HandleSpriteOwnerPointerLeave(void* pGeometryOwner)
{
	void* pObject;

	pObject = (char*) pGeometryOwner - 0x90;
	*(int*) ((char*) pGeometryOwner + 0x40) = 0;
	NotifySpriteOwnerVariantCEvent0E(pObject);
	((void(LEMBALL_FASTCALL*)(void*))(*(void***) pObject)[51])(pObject);
}
// FUNCTION: LEMBALL 0x00468050
void LEMBALL_FASTCALL HandleSpriteOwnerButtonTransition(void* pGeometryOwner, int, const VS_XYPair* pPoint, int nButton)
{
	void* pObject;
	int nFrame;

	pObject = (char*) pGeometryOwner - 0x90;
	if (nButton == 0 || nButton == 3) {
		*(int*) ((char*) pGeometryOwner + 0x40) = 1;
	}
	nFrame = ((GameSpriteOwnerVariantC*) pObject)->MapSpriteOwnerEventToFrameSlot(nButton);
	StoreSpriteOwnerLocalPoint(pGeometryOwner, pPoint);
	((GameSpriteOwnerVariantC*) pObject)->NotifyFrameEventB(nFrame);
	((void(LEMBALL_FASTCALL*)(void*, int, int))(*(void***) pObject)[49])(pObject, 0, nFrame);
}
// FUNCTION: LEMBALL 0x004680c0
void LEMBALL_FASTCALL HandleSpriteOwnerButtonInside(void* pGeometryOwner, int, const VS_XYPair* pPoint, int nButton)
{
	void* pObject;
	int nFrame;

	if (*(int*) ((char*) pGeometryOwner + 0x40) == 0) {
		return;
	}
	pObject = (char*) pGeometryOwner - 0x90;
	nFrame = ((GameSpriteOwnerVariantC*) pObject)->MapSpriteOwnerEventToFrameSlot(nButton);
	*(int*) ((char*) pGeometryOwner + 0x40) = 0;
	StoreSpriteOwnerLocalPoint(pGeometryOwner, pPoint);
	((GameSpriteOwnerVariantC*) pObject)->NotifyFrameEventC(nFrame);
	((void(LEMBALL_FASTCALL*)(void*, int, int))(*(void***) pObject)[48])(pObject, 0, nFrame);
}
// FUNCTION: LEMBALL 0x00468130
void LEMBALL_FASTCALL ResetSpriteOwnerButtonState(void* pGeometryOwner, int, const VS_XYPair* pPoint, int)
{
	int i;

	StoreSpriteOwnerLocalPoint(pGeometryOwner, pPoint);
	for (i = 0; i < 6; ++i) {
		*(int*) ((char*) pGeometryOwner + 0x10 + i * 4) = 0;
	}
}
// FUNCTION: LEMBALL 0x00468360
void LEMBALL_FASTCALL QueueCompactZrleOwnerIfDirty(void* pObject, void* pUnusedEdx, const short* pRect)
{
	typedef int(LEMBALL_FASTCALL * QueryProc)(void* pObject);
	typedef void(LEMBALL_FASTCALL * VoidProc)(void* pObject);
	void* pRenderContext;
	void* pOwner;
	int nRenderObject;
	int fHelperDirty;
	int nUploadState;

	(void) pUnusedEdx;
	(void) pRect;

	nRenderObject = ResolveResourceGeometryRenderObject(pObject);
	fHelperDirty = ((QueryProc) (*(void***) nRenderObject)[15])((void*) nRenderObject);
	pRenderContext = *(void**) ((char*) pObject + 0x4c);
	if (!((fHelperDirty != 0 && (*(int*) ((char*) pObject + 0xd0) != 0 ||
								 *(int*) ((char*) pObject + 0x104) != *(int*) ((char*) pObject + 0x108))) ||
		  (*(int*) ((char*) pRenderContext + 4) == 0 &&
		   (*(int*) ((char*) pObject + 0xf8) != 0 || *(int*) ((char*) pObject + 0xd8) != 0 ||
			*(int*) ((char*) pObject + 0xd0) != *(int*) ((char*) pObject + 0xd4))))) {
		return;
	}

	if (((QueryProc) (*(void***) pObject)[26])(pObject) != 0) {
		((VoidProc) (*(void***) pObject)[46])(pObject);
		((VoidProc) (*(void***) pObject)[47])(pObject);
	}
	pOwner = *(void**) ((char*) pRenderContext + 0x0c);
	nUploadState = ((QueryProc) (*(void***) pOwner)[2])(pOwner);
	((VsGdiHelperDispatchQueue*) pRenderContext)->AppendPointerQueueEntry(*(void**) ((char*) pObject + 0xe0));
	InitializeHelperUploadStatePending(nUploadState);
	*(int*) ((char*) pObject + 0xfc) = 1;
}
// FUNCTION: LEMBALL 0x00468530
void* LEMBALL_FASTCALL ConstructAlignedZrleChildOverlayFromPoint(void* pObject,
																 int nUnusedEdx,
																 const VS_XYPair* pPoint,
																 void* pRenderOwner,
																 int nResourceId,
																 int fAlignment)
{
	typedef void(LEMBALL_FASTCALL *
				 InitializeRectProc)(void* pObject, int nUnusedEdx, short* pRect, void* pOwner, const char* pszName);
	short aRect[4];

	(void) nUnusedEdx;
	ConstructResourceGeometryChildBase(pObject, 0, pRenderOwner);
	*(short*) ((char*) pObject + 0x114) = 0;
	*(short*) ((char*) pObject + 0x116) = 0;
	*(int*) ((char*) pObject + 0x104) = 0;
	*(int*) ((char*) pObject + 0x108) = 0;
	*(short*) ((char*) pObject + 0x118) = 0;
	*(short*) ((char*) pObject + 0x11a) = 0;
	InitializeTinyPointSinkHelper((char*) pObject + 0x128);
	InitializeAlignedZrleChildOverlayVtables();
	*(void***) pObject = g_AlignedZrleChildOverlayVtable;
	*(void***) ((char*) pObject + 0x90) = g_AlignedZrleChildOverlayDeleteVtable;
	*(int*) ((char*) pObject + 0x10c) = nResourceId;
	*(int*) ((char*) pObject + 0x110) = fAlignment;
	ComputeAlignedZrleChildLayout(pObject);

	*(short*) ((char*) pObject + 0xdc) = pPoint->m_x;
	*(short*) ((char*) pObject + 0xde) = pPoint->m_y;
	aRect[0] = *(short*) ((char*) pObject + 0xc0);
	aRect[1] = *(short*) ((char*) pObject + 0xc2);
	aRect[2] = pPoint->m_x;
	aRect[3] = pPoint->m_y;
	((InitializeRectProc) (*(void***) pObject)[1])(pObject, 0, aRect, pRenderOwner, "Button");
	*(short*) ((char*) pObject + 0xc4) += *(short*) ((char*) pObject + 0x18);
	*(short*) ((char*) pObject + 0xc6) += *(short*) ((char*) pObject + 0x1a);
	AppendGeometryOwnerChild(*(void**) ((char*) pRenderOwner + 0x1c), 0, (char*) pObject + 0x90);
	return pObject;
}
void* LEMBALL_FASTCALL InitializeMenuActionFramePointSink(void* pObject);
// FUNCTION: LEMBALL 0x004686e0
void LEMBALL_FASTCALL ComputeAlignedZrleChildLayout(void* pObject)
{
	typedef void(LEMBALL_FASTCALL * LoadProc)(void* pResource);
	GameMenuActionFramePointSink* pFrameSink;
	int* pListResource;
	char* pFrame;
	short cxFrame;
	short cyFrame;
	short cxOwner;
	short cyOwner;
	short nAlignment;

	*(int*) ((char*) pObject + 0x120) = 0;
	pFrameSink = (GameMenuActionFramePointSink*) AllocateVSMemBlock(sizeof(int) + sizeof(GameMenuActionFramePointSink));
	if (pFrameSink != 0) {
		*(int*) pFrameSink = 1;
		pFrameSink = (GameMenuActionFramePointSink*) ((char*) pFrameSink + sizeof(int));
		InitializeMenuActionFramePointSink(pFrameSink);
	}
	*(GameMenuActionFramePointSink**) ((char*) pObject + 0x124) = pFrameSink;
	++*(int*) ((char*) pObject + 0x50);

	pListResource = (int*) LoadZrleOnlyListResource(*(int*) ((char*) pObject + 0x10c));
	*(int**) ((char*) pObject + 0x11c) = pListResource;
	if (pListResource[4] == 0) {
		((LoadProc) (*(void***) pListResource)[7])(pListResource);
	}
	else {
		pListResource[9] = 0;
	}
	++pListResource[2];

	pFrame = (char*) pListResource[0x1e];
	cxFrame = *(short*) (pFrame + 0x4c);
	cyFrame = *(short*) (pFrame + 0x4e);
	if (cxFrame < *(short*) (pFrame + 0xa0)) {
		cxFrame = *(short*) (pFrame + 0xa0);
	}
	if (cyFrame < *(short*) (pFrame + 0xa2)) {
		cyFrame = *(short*) (pFrame + 0xa2);
	}
	*(short*) ((char*) pObject + 0x114) = cxFrame;
	*(short*) ((char*) pObject + 0x116) = cyFrame;
	--pListResource[2];

	*(short*) ((char*) pObject + 0x118) = *(short*) ((char*) pObject + 0xc4);
	*(short*) ((char*) pObject + 0x11a) = *(short*) ((char*) pObject + 0xc6);
	cxOwner = *(short*) ((char*) pObject + 0xc0);
	cyOwner = *(short*) ((char*) pObject + 0xc2);
	if (cxOwner < 0) {
		cxOwner = (short) (-cxFrame * cxOwner);
	}
	else if (cxOwner == 0) {
		cxOwner = cxFrame;
	}
	if (cyOwner < 0) {
		cyOwner = (short) (-cyFrame * cyOwner);
	}
	else if (cyOwner == 0) {
		cyOwner = cyFrame;
	}
	*(short*) ((char*) pObject + 0xc0) = cxOwner;
	*(short*) ((char*) pObject + 0xc2) = cyOwner;
	if ((int) cxOwner * (int) cyOwner != 0) {
		((GameGeometryOwnerRectState*) ((char*) pObject + 0x90))->SetEnabled(1);
	}

	nAlignment = *(short*) ((char*) pObject + 0x118);
	if ((*(unsigned int*) ((char*) pObject + 0x110) & 4) != 0) {
		nAlignment = (short) ((cxOwner - cxFrame) / 2);
	}
	else if ((*(unsigned int*) ((char*) pObject + 0x110) & 0x10) != 0) {
		nAlignment = (short) (cxOwner - cxFrame);
	}
	*(short*) ((char*) pObject + 0x118) = nAlignment;

	if ((*(unsigned int*) ((char*) pObject + 0x110) & 8) != 0) {
		*(short*) ((char*) pObject + 0x11a) = (short) ((cyOwner - cyFrame) / 2);
	}
	else if ((*(unsigned int*) ((char*) pObject + 0x110) & 0x20) != 0) {
		*(short*) ((char*) pObject + 0x11a) = (short) (cyOwner - cyFrame);
	}
}
// FUNCTION: LEMBALL 0x004688e0
void GameCompactZrleOverlay::ReloadResource(int nResourceId)
{
	void* pResource;

	pResource = *(void**) ((char*) this + 0x11c);
	if (pResource != 0) {
		ReleaseTypedResourceObjectReference(pResource);
	}
	*(int*) ((char*) this + 0x10c) = nResourceId;
	*(void**) ((char*) this + 0x11c) = LoadZrleOnlyListResource(nResourceId);
	*(int*) ((char*) this + 0xd8) = 1;
}
// FUNCTION: LEMBALL 0x00468920
void LEMBALL_FASTCALL DestroyAlignedZrleChildOverlay(void* pChildOwner)
{
	typedef void(LEMBALL_FASTCALL * DeleteProc)(void* pObject, int nUnusedEdx, unsigned int fDelete);
	char* pObject;
	void* pFrameSink;

	pObject = (char*) pChildOwner - 0x90;
	*(void***) pChildOwner = g_AlignedZrleChildOverlayDeleteVtable;
	*(void***) pObject = g_AlignedZrleChildOverlayVtable;
	if (*(int*) (pObject + 4) == 1) {
		DetachGeometryChildAndDestroyWindowOwner(pObject);
	}
	pFrameSink = *(void**) (pObject + 0x124);
	if (pFrameSink != 0) {
		((DeleteProc) (*(void***) pFrameSink)[0])(pFrameSink, 0, 3);
	}
	RestoreQueuedRenderPointSinkVtable(pObject + 0x128);
	DestroyResourceGeometryChild(pChildOwner);
}

// FUNCTION: LEMBALL 0x00468c10
void LEMBALL_FASTCALL DestroyBufferedResourceGeometryChild(void* pChildOwner)
{
	typedef void(LEMBALL_FASTCALL * DeleteProc)(void* pObject, int nUnusedEdx, unsigned int fDelete);
	char* pObject;
	void* pChild;

	pObject = (char*) pChildOwner - 0x90;
	*(void***) pChildOwner = (void**) 0x499818;
	*(void***) pObject = (void**) 0x499838;
	pChild = *(void**) ((char*) pChildOwner + 0x80);
	if (pChild != 0) {
		((DeleteProc) (*(void***) pChild)[0])((char*) pChild + 0x90, 0, 3);
	}
	pChild = *(void**) ((char*) pChildOwner + 0x7c);
	if (pChild != 0) {
		((DeleteProc) (*(void***) pChild)[0])((char*) pChild + 0x90, 0, 3);
	}
	DestroyResourceGeometryChild(pChildOwner);
}

// FUNCTION: LEMBALL 0x00468ec0
void LEMBALL_FASTCALL DestroySingleChildOverlayOwner(void* pChildOwner)
{
	typedef void(LEMBALL_FASTCALL * DeleteProc)(void* pObject, int nUnusedEdx, unsigned int fDelete);
	char* pObject;
	void* pChild;

	pObject = (char*) pChildOwner - 0x90;
	*(void***) pChildOwner = (void**) 0x499908;
	*(void***) pObject = (void**) 0x499928;
	pChild = *(void**) ((char*) pChildOwner + 0x88);
	if (pChild != 0) {
		((DeleteProc) (*(void***) ((char*) pChild + 0x90))[0])((char*) pChild + 0x90, 0, 1);
	}
	DestroyResourceGeometryChild(pChildOwner);
}

// FUNCTION: LEMBALL 0x004693b0
void LEMBALL_FASTCALL InitializeResourceSpriteGeometryFrames(void* pObject)
{
	LevelScreenDrawEntry* pFrame;

	pFrame = new LevelScreenDrawEntry[1];
	*(LevelScreenDrawEntry**) ((char*) pObject + 0x124) = pFrame;
	++*(int*) ((char*) pObject + 0x50);
	*(int*) ((char*) pObject + 0x11c) = 0;
	*(int*) ((char*) pObject + 0x118) = 0;
	*(int*) ((char*) pObject + 0x120) = 0;
	*(int*) ((char*) pObject + 0x148) = 0;
	*(int*) ((char*) pObject + 0x144) = 0;
	*(void**) ((char*) pObject + 0x134) = LoadListResource(*(int*) ((char*) pObject + 0x130));
	*(int*) ((char*) pObject + 0x13c) = 0;
}

// FUNCTION: LEMBALL 0x00469440
void LEMBALL_FASTCALL DestroyResourceSpriteGeometryChild(void* pChildOwner)
{
	typedef void(LEMBALL_FASTCALL * DeleteProc)(void* pObject, int nUnusedEdx, unsigned int fDelete);
	char* pObject;
	void* pChild;

	pObject = (char*) pChildOwner - 0x90;
	*(void***) pChildOwner = (void**) 0x4999f8;
	*(void***) pObject = (void**) 0x499a18;
	pChild = *(void**) ((char*) pChildOwner + 0xa4);
	((void(LEMBALL_FASTCALL*)(void*)) 0x45d180)(pChild);
	pChild = *(void**) ((char*) pChildOwner + 0x94);
	if (pChild != 0) {
		((DeleteProc) (*(void***) pChild)[0])(pChild, 0, 3);
	}
	DestroyResourceGeometryChild(pChildOwner);
}
// FUNCTION: LEMBALL 0x00468980
void GameAlignedZrleChildOverlayView::ReleaseFrameResource(void)
{
	void* pResource;

	pResource = *(void**) ((char*) this + 0x11c);
	if (pResource != 0) {
		ReleaseTypedResourceObjectReference(pResource);
		*(void**) ((char*) this + 0x11c) = 0;
	}
}
// FUNCTION: LEMBALL 0x004689a0
void GameAlignedZrleChildOverlayView::QueueAlignedFrame(void)
{
	typedef void(LEMBALL_FASTCALL * RefreshProc)(void* pObject, int nUnusedEdx);
	typedef void(LEMBALL_FASTCALL * QueueProc)(void* pObject, int nUnusedEdx, void* pQueueOwner);
	char* pObject;
	char* pFrameSink;
	void* pGeometryContext;
	void* pQueueOwner;
	short x;
	short y;
	int fResourceReady;

	pObject = (char*) this;
	x = *(short*) (pObject + 0x118);
	y = *(short*) (pObject + 0x11a);
	fResourceReady = *(int*) (pObject + 0x104) != 0 && *(int*) (pObject + 0x94) != 0;
	if ((*(unsigned int*) (pObject + 0x110) & 0x40) != 0 && fResourceReady) {
		++x;
		++y;
	}

	pQueueOwner = *(void**) (pObject + 0x4c);
	pGeometryContext = *(void**) ((char*) pQueueOwner + 0x0c);
	pGeometryContext = (char*) pGeometryContext + *(int*) (*(int*) ((char*) pGeometryContext + 0x40) + 4) + 0x40;
	((RefreshProc) (*(void***) pGeometryContext)[14])(pGeometryContext, 0);

	pFrameSink = *(char**) (pObject + 0x124);
	*(short*) (pFrameSink + 4) = x;
	*(short*) (pFrameSink + 6) = y;
	*(void**) (pFrameSink + 0x1c) = *(void**) (pObject + 0x11c);
	*(int*) (pFrameSink + 0x18) = *(int*) (pObject + 0x104);
	*(int*) (pFrameSink + 0x0c) = 0;
	*(void**) (pFrameSink + 0x10) = *(void**) (pObject + 0x120);
	((QueueProc) (*(void***) pFrameSink)[1])(pFrameSink, 0, pQueueOwner);
}
// FUNCTION: LEMBALL 0x00469870
void GameResourceGeometryChildView::NoopWindowOwnerSlot42(int)
{
}
// FUNCTION: LEMBALL 0x00469880
void* GameResourceGeometryChildDeleteView::Delete(unsigned int fDelete)
{
	char* pObject;

	pObject = (char*) this;
	DestroyResourceGeometryChild(this);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject - 0x90);
	}
	return pObject - 0x90;
}
// FUNCTION: LEMBALL 0x004698b0
void* LEMBALL_FASTCALL InitializeTinyPointSinkHelper(void* pObject)
{
	*(void***) pObject = g_VSGDI_StatusIndicatorPointSinkVtable;
	*(int*) ((char*) pObject + 4) = 0;
	return pObject;
}
// FUNCTION: LEMBALL 0x004698c0
void* GameAlignedZrleChildOverlayDeleteView::Delete(unsigned int fDelete)
{
	char* pObject;

	pObject = (char*) this;
	DestroyAlignedZrleChildOverlay(this);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject - 0x90);
	}
	return pObject - 0x90;
}

void LEMBALL_FASTCALL update_action_button_field_0x104_from_fields_0x130_xor_0xd0(void* pObject)
{
	int nValue;

	nValue = *(volatile int*) ((char*) pObject + 0x130);
	nValue ^= *(volatile int*) ((char*) pObject + 0xd0);
	*(int*) ((char*) pObject + 0x104) = nValue;
}

struct MenuSelectorXorFieldView {
	void RecomputeField104FromXor(int nUnused);
};

void MenuSelectorXorFieldView::RecomputeField104FromXor(int nUnused)
{
	int nValue;

	(void) nUnused;
	nValue = *(volatile int*) ((char*) this + 0xd0);
	nValue ^= *(volatile int*) ((char*) this + 0x130);
	*(int*) ((char*) this + 0x104) = nValue;
}

// Compatibility shim for original ILT call site.
void LEMBALL_FASTCALL DestroyMenuSelectorEntryAutoThunk(void* pObject)
{
	DestroyMenuSelectorEntry((GameMenuSelectorEntry*) pObject);
}

// Compatibility shim for original ILT call site.
void LEMBALL_FASTCALL DestroyMenuSelectorManagerAutoThunk(void* pObject)
{
	DestroyMenuSelectorManager((GameMenuSelectorManager*) pObject);
}

#define DEFINE_MENUSEL_NAMED_SCALAR_DELETE(name, destroy)                                                              \
	void* LEMBALL_FASTCALL name(void* pObject, void*, BYTE fDelete)                                                    \
	{                                                                                                                  \
		destroy(pObject);                                                                                              \
		if ((fDelete & 1) != 0) {                                                                                      \
			FreeVSMemBlock(pObject);                                                                                   \
		}                                                                                                              \
		return pObject;                                                                                                \
	}

DEFINE_MENUSEL_NAMED_SCALAR_DELETE(DeleteQueuedVariantChildSlotManagerAuto, destroy_queued_variant_child_slot_manager)
DEFINE_MENUSEL_NAMED_SCALAR_DELETE(DeleteMenuSelectorEntryAuto, DestroyMenuSelectorEntryAutoThunk)
DEFINE_MENUSEL_NAMED_SCALAR_DELETE(DeleteMenuSelectorManagerAuto, DestroyMenuSelectorManagerAutoThunk)

#undef DEFINE_MENUSEL_NAMED_SCALAR_DELETE

// Compatibility shim for original ILT call site.
void LEMBALL_FASTCALL DestroyQueuedVariantRangeEntryAutoThunk(void* pObject)
{
	DestroyQueuedVariantRangeEntryAuto(pObject);
}

void* LEMBALL_FASTCALL DeleteQueuedVariantRangeEntryAuto(void* pObject, void*, BYTE fDelete)
{
	DestroyQueuedVariantRangeEntryAutoThunk(pObject);
	if ((fDelete & 1) != 0) {
		FreeVSMemBlock(pObject);
	}
	return pObject;
}

void LEMBALL_FASTCALL QueueSingleChildOverlayOwnerIfDirtyBody(void* pObject, void* pUnusedEdx, const short* pUnusedRect)
{
	typedef int(LEMBALL_FASTCALL * QueryProc)(void* pObject);
	typedef void(LEMBALL_FASTCALL * VoidProc)(void* pObject);
	void** pVtable;
	void* pRenderContext;
	void* pOwner;
	int nChildEnd;
	int nUploadState;

	(void) pUnusedEdx;
	(void) pUnusedRect;
	pRenderContext = *(void**) ((char*) pObject + 0x4c);
	if (*(int*) ((char*) pRenderContext + 4) != 0) {
		return;
	}
	if (*(int*) ((char*) pObject + 0xf8) == 0 && *(int*) ((char*) pObject + 0xd8) == 0) {
		nChildEnd = *(volatile int*) ((char*) pObject + 0xd4);
		if (nChildEnd == *(int*) ((char*) pObject + 0xd0)) {
			return;
		}
	}
	pVtable = *(void***) pObject;
	if (((QueryProc) pVtable[26])(pObject) != 0) {
		((VoidProc) pVtable[46])(pObject);
		((VoidProc) pVtable[47])(pObject);
	}
	pOwner = *(void**) ((char*) *(void**) ((char*) pObject + 0x4c) + 0x0c);
	nUploadState = ((QueryProc) (*(void***) pOwner)[2])(pOwner);
	((VsGdiHelperDispatchQueue*) *(void**) ((char*) pObject + 0x4c))
		->AppendPointerQueueEntry(*(void**) ((char*) pObject + 0xe0));
	InitializeHelperUploadStatePending(nUploadState);
	*(int*) ((char*) pObject + 0xfc) = 1;
}

void LEMBALL_FASTCALL QueueSingleChildOverlayOwnerIfDirty(void* pObject, void* pUnusedEdx, const short* pRect)
{
	(void) pObject;
	(void) pUnusedEdx;
	((void(PASCAL*)(const short*)) QueueSingleChildOverlayOwnerIfDirtyBody)(pRect);
}

// FUNCTION: LEMBALL 0x0044ded0
void noop_menu_selector_animation_callback(void)
{
}
// FUNCTION: LEMBALL 0x0044fca0
void noop_queued_variant_child_refresh_callback(void)
{
}

// FUNCTION: LEMBALL 0x00443DB0
void LEMBALL_FASTCALL ApplyLevelScreenPauseDialogLayout(void* pObject, int, short* pRect)
{
	char* pThis;
	short* pItems;
	short nY;
	short nX;
	short aPoint[2];
	short aOuter[4];
	short aInset[2];
	int i;
	pThis = (char*) pObject;
	pItems = *(short**) (pThis + 0x1f8);
	nY = (short) *(int*) (pThis + 0x12c);
	if (*(int*) (pThis + 0x104) == 3) {
		nX = (short) ((pRect[0] - pItems[0]) / 2);
		pItems[2] = nX;
		pItems[3] = nY;
		nY += pItems[1] + *(short*) (pThis + 0x14e);
		pItems[6] = nX;
		pItems[7] = nY;
		pItems[10] = nX + pItems[0] - pItems[8];
		pItems[11] = nY;
	}
	else {
		for (i = 0; i < *(int*) (pThis + 0x120); ++i) {
			short* pItem = pItems + i * 4;
			pItem[2] = (short) ((pRect[0] - pItem[0]) / 2);
			pItem[3] = nY;
			nY += pItem[1] + *(short*) (pThis + 0x14e);
		}
	}
	*(short*) (pThis + 0xd4) = pRect[0];
	*(short*) (pThis + 0xd6) = pRect[1];
	*(short*) (pThis + 0xd8) = pRect[2];
	*(short*) (pThis + 0xda) = pRect[3];
	((void(LEMBALL_FASTCALL*)(void*, int, int)) 0x46a530)(pThis + 0xa4, 0, 1);
	*(int*) (pThis + 0xcc) = 1;
	if (*(int*) (pThis + 4) == 1) {
		((void(LEMBALL_FASTCALL*)(void*, int, short*)) 0x465df0)(pObject, 0, pRect);
	}
	else {
		*(int*) (pThis + 0x50) = *(int*) (pThis + 0x134) * 2 + 0x3ed;
		((void(LEMBALL_FASTCALL*)(void*, int, short*, void*, const char*))(
			*(void***) pObject)[1])(pObject, 0, pRect, *(void**) (pThis + 0x114), (const char*) 0x49f108);
		((void(LEMBALL_FASTCALL*)(void*, int, void*)) 0x46a9a0)(*(void**) (*(char**) (pThis + 0x114) + 0x1c),
																0,
																pThis + 0xa4);
	}
	((void(LEMBALL_FASTCALL*)(short*, int, short*)) 0x402248)(aPoint, 0, pRect);
	*(short*) (pThis + 0x150) = 4;
	*(short*) (pThis + 0x152) = 4;
	if (*(int*) (pThis + 0x10c) == 0) {
		*(short*) (pThis + 0x150) = 8;
		*(short*) (pThis + 0x152) = 8;
	}
	aOuter[0] = aPoint[0];
	aOuter[1] = aPoint[1];
	aOuter[2] = 0;
	aOuter[3] = 0;
	aInset[0] = *(short*) (pThis + 0x150);
	aInset[1] = *(short*) (pThis + 0x152);
	((void(LEMBALL_FASTCALL*)(short*, int, short*)) 0x402996)(aOuter + 2, 0, aInset);
	aInset[0] *= 2;
	aInset[1] *= 2;
	((void(LEMBALL_FASTCALL*)(short*, int, short*)) 0x403837)(aOuter, 0, aInset);
	((void(LEMBALL_FASTCALL*)(void*, int, short*)) 0x4012d0)(pThis + 0x13c, 0, aOuter);
	*(int*) (pThis + 0x144) = 0x0c;
}
