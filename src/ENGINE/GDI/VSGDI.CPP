#include "ENGINE/GDI/VSGDI.H"
#include "ENGINE/CORE/SAFEVT.H"
#include "SHELL/ENTRY.H"
#include "SHELL/OPTIONS.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/COMMON.H"
#include "GAME.H"
#include "ENGINE/GDI/VSWINDOW.H"
#include "LEVEL/DRAWTEXT.H"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <string.h>

DECOMP_SIZE_ASSERT(VsGdiRect, 0x08);
DECOMP_SIZE_ASSERT(VsGdiDisplayState, 0x1c);
DECOMP_SIZE_ASSERT(VsGdiDisplayStateDispatch, 0x04);
DECOMP_SIZE_ASSERT(VsGdiSelectedGraphicsDriverRuntime, 0x0c);
DECOMP_SIZE_ASSERT(VsGdiMetricsDisplayState, 0x1c);
DECOMP_SIZE_ASSERT(VsGdiHelperSurface, 0x560);
DECOMP_SIZE_ASSERT(VsGdiDibDisplayState, 0x30);
DECOMP_SIZE_ASSERT(VsGdiDirectDrawDisplayState, 0x4a8);

extern LRESULT CALLBACK WindowOwnerWindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
extern "C" DWORD WINAPI timeGetTime(void);
extern void LEMBALL_FASTCALL EnsureHelperGroup0BackingBuffer(int nHelperGroup);
extern void *LEMBALL_FASTCALL ConstructWindowOwnerRenderContext(
    void *pPrimaryContext);
extern void *__fastcall get_draw_command_rect_payload(void *pCommand);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void *LoadTwoArrayListResource(int nResourceId);
extern void *g_pLevelProgressState;

void __stdcall PromoteQueuedRenderPointSinkTargetStateThunk(void *pPointSink);
void LEMBALL_FASTCALL QueueQueuedRenderPointSinkThunk(
    void *pPointSink, void *, void *pQueue);
void LEMBALL_FASTCALL QueueQueuedRenderPointSink(
    void *pPointSink, void *, void *pQueue);
void __stdcall PromoteQueuedRenderPointSinkTargetState(void *pPointSink);
char *LookupDirectDrawErrorString(unsigned int nError);
void NoopDirectDrawCallback(unsigned int nResult, unsigned int nUnused);
int LEMBALL_FASTCALL CreateAndStoreDisplayPalette(
    void *pDisplayState, int, void *pPalette);
int LEMBALL_FASTCALL SelectAndRealizeDisplayPalette(
    void *pDisplayState, int, void *pDisplayDc);
int LEMBALL_STDCALL ApplyDisplaySurfacePalette(
    void *pSurface, int nFirstEntry, int cEntries,
    const unsigned int *paColors);
static int LEMBALL_STDCALL CopyDisplaySurfaceRect(
    void *pBinding, short *pDestinationRect,
    void *pSourceBinding, short *pSourcePoint);
static int LEMBALL_STDCALL StretchDisplaySurfaceRect(
    void *pDestinationBinding, short *pDestinationRect,
    void *pSourceBinding, short *pSourceRect);
static int LEMBALL_FASTCALL ApplyDirectDrawSurfacePalette(
    void *, int, void *, int, int, const unsigned int *);
int LEMBALL_FASTCALL UsesExistingDisplayPaletteAsFallback(
    void *pDisplayState, int);
void LEMBALL_FASTCALL SetHelperTargetParamWrapperDisplayDc(
    void *pParamWrapper, int, void *hDc);
void *LEMBALL_FASTCALL DeleteHelperTargetParamWrapper(
    void *pParamWrapper, int, int fDelete);
void LEMBALL_FASTCALL NoopHelperTargetParamWrapperArgument(
    void *, int, void *);
void *LEMBALL_FASTCALL DeleteHelperTargetParamWrapperBase(
    void *pParamWrapper, int, int fDelete);
int LEMBALL_FASTCALL ReturnTrueHelperTargetParamWrapperA(void *, int);
int LEMBALL_FASTCALL ReturnTrueHelperTargetParamWrapperB(void *, int);
int LEMBALL_FASTCALL GetHelperTargetParamWrapperDisplayDc(
    void *pParamWrapper, int);
int LEMBALL_FASTCALL GetHelperTargetParamWrapperReserved(
    void *pParamWrapper, int);
void *LEMBALL_FASTCALL DeleteHelperTargetParamWrapperInterface(
    void *pParamWrapper, int, int fDelete);
void *LEMBALL_FASTCALL VsGdiDeleteDisplayBitmap(
    void *pBitmap, int, int fDelete);
void LEMBALL_FASTCALL BuildGeometryHelperFromRenderRect(void *pOwner);
static void LEMBALL_FASTCALL RestoreCompactResourceGeometryHelperVtable(void *pvHelper);
void *ConstructHelperGroup0RowBuffer(void *pvHelperGroup, int fConstructCompactHelper);
void LEMBALL_FASTCALL DestroyHelperGroup0RowBuffer(void *pvHelperGroup);
int LEMBALL_FASTCALL GetHelperGroup0DirtyFlag(void *pGroupState, int);
void LEMBALL_FASTCALL ReleaseHelperGroup0BackingBuffer(int nHelperGroup);
void LEMBALL_FASTCALL EnsureHelperGroup0BackingBufferThunk(void *pHelperGroup);
void *ConstructHelperGroup1RowBuffer(void *pvHelperGroup, int fConstructCompactHelper);
void LEMBALL_FASTCALL DestroyHelperGroup1RowBuffer(void *pvHelperGroup);
void ReleaseHelperGroup1BackingBuffer(int nHelperGroup);
void LEMBALL_FASTCALL EnsureHelperGroup1BackingBufferBody(void *pHelperGroup);
void LEMBALL_FASTCALL EnsureHelperGroup1BackingBuffer(void *pHelperGroup);
void LEMBALL_FASTCALL CopyRectTupleIfExtentEmpty(void *pTarget,
                                                 int,
    short *paRect);
int LEMBALL_FASTCALL GetHelperGroup1DirtyFlag(void *pGroupState, int);
void LEMBALL_FASTCALL ResetHelperUploadStateMap(void *pTracker);
void LEMBALL_FASTCALL EnsureHelperUploadStateMapCapacity(void *pTracker);
void LEMBALL_FASTCALL SetHelperUploadTrackerExtent(
    void *pTracker, int, int nExtent);
void LEMBALL_FASTCALL InitializeHelperUploadStatePending(int nUploadState);
void LEMBALL_FASTCALL ResetHelperUploadRectListAndStateMap(int nUploadState);
void LEMBALL_FASTCALL PromoteHelperUploadStateToActive(int nUploadState);
void LEMBALL_FASTCALL DestroyGeometryHelperPointerArray(void *pHelper);
void AppendPointerQueueEntry(void *pQueue, void *pEntry);
void LEMBALL_FASTCALL DispatchAndClearPointerQueue(void *pQueue);
void *LEMBALL_FASTCALL ConstructStatusIndicatorRenderClientBase(int *pRenderClient);
void LEMBALL_FASTCALL InitializeStatusIndicatorRenderClientDefaults(int *pRenderClient);
int LEMBALL_FASTCALL HandleStatusIndicatorInputEvent(
    void *pRenderClient, int, RenderDispatchQueueEntry *pEntry);
int InitializeResourceGeometryHelperRuntime(void);
int ShutdownResourceGeometryHelperRuntime(void);
void DestroyResourceGeometryHelperSlotManager(void *pvSlotManager);
int FindFirstFreeResourceGeometryHelperSlotIndex(void *pvSlotManager);
void BuildAndApplyLogPalette(unsigned int *paOutputColors,
                             void *pPaletteResource,
                             int,
    const unsigned int *paFallbackColors);
void LEMBALL_FASTCALL DestroyResourceGeometryHelperTarget(void *pvTarget);
int LEMBALL_FASTCALL GetResourceGeometryHelperTargetActiveUploadState(void *pTarget);
void LEMBALL_FASTCALL ApplyHelperTargetPaletteAndPropagate(
    void *pCompactTarget, int, void *pPalette);
void LEMBALL_FASTCALL SetHelperTargetRenderExtent(
    void *pCompactTarget, int, const VsGdiRect *pExtent);
void LEMBALL_FASTCALL BindHelperTargetDisplayDc(void *pTarget, int, void *hDc);
void LEMBALL_FASTCALL PropagateGlobalPaletteToResourceGeometries(void *pTarget);
void LEMBALL_FASTCALL PropagateGlobalPaletteToResourceGeometriesBody(void *pTarget);
int RefreshHelperTargetBindingIfChanged(void *pTarget);
void InvokeRootHelperTargetPostQueueCallback(int nTarget);
void LEMBALL_FASTCALL FillHelperSurfaceRectCommandAdjustmentThunk(
    void *pCompactTarget, int nUnused, void *pCommand);
void LEMBALL_FASTCALL ForwardBitmapCommandToAdjustedSurfaceThunk(void);
void LEMBALL_FASTCALL ForwardBitmapCommandToAdjustedSurface(
    void *pCompactTarget, int, void *pCommand, void *pBitmap);
void LEMBALL_FASTCALL BlitBitmapRowsToScanlinesAdjustmentThunk(
    void *pCompactTarget, int nUnused, void *pCommand, void *pBitmap);
void LEMBALL_FASTCALL SetHelperTargetRenderExtentAdjustmentThunk(
    void *pCompactTarget, int nUnused, const VsGdiRect *pExtent);
void LEMBALL_FASTCALL ApplyHelperTargetPaletteAdjustmentThunk(
    void *pCompactTarget, int nUnused, void *pPalette);
void LEMBALL_FASTCALL PresentCompactResourceGeometryHelperAdjustmentThunk(
    void *pCompactTarget);
void LEMBALL_FASTCALL PresentCompactResourceGeometryHelper(
    void *pCompactTarget);
unsigned char *LEMBALL_FASTCALL GetHelperTargetTokenAdjustmentThunk(
    void *pCompactTarget);
unsigned char *LEMBALL_FASTCALL GetHelperTargetToken(void *pTarget);
int LEMBALL_FASTCALL GetHelperGroup0DirtyFlagAdjustmentThunk(
    void *pCompactTarget, int nUnused);
int LEMBALL_FASTCALL GetHelperGroup1DirtyFlagAdjustmentThunk(
    void *pCompactTarget, int nUnused);
void *LEMBALL_FASTCALL InitializeResourceGeometryRowBuffer(void *pvRowBuffer);
void LEMBALL_FASTCALL DestroyResourceGeometryRowBuffer(void *pvRowBuffer);
void LEMBALL_FASTCALL ClearResourceGeometryRowBufferFields(void *pvRowBuffer);
void LEMBALL_FASTCALL EnsureHelperBackingRowIndexCapacity(VsGdiHelperSurface *pSurface);
void LEMBALL_FASTCALL RebuildResourceGeometryRowPointerTable(VsGdiHelperSurface *pSurface);
void LEMBALL_FASTCALL InitializeArrowCursorState(int *pState);
void LEMBALL_FASTCALL InitializeArrowCursorPosition(void *pRenderClient);
void LEMBALL_FASTCALL HideArrowCursor(void *pRenderClient);
void LEMBALL_FASTCALL ShowArrowCursor(void *pRenderClient);
void *LEMBALL_FASTCALL DeleteStatusIndicatorRenderClientBaseThunk(
    void *pRenderClient, int nUnused, unsigned char fDelete);
void LEMBALL_FASTCALL NoopStatusIndicatorRenderClientSlot(void *pRenderClient);
int LEMBALL_FASTCALL DispatchStatusIndicatorRenderEntry(
    void *pRenderClient, int nUnused, void *pEntry);
void *LEMBALL_FASTCALL DeleteArrowCursorRenderClientThunk(
    void *pRenderClient, int nUnused, unsigned char fDelete);
void LEMBALL_FASTCALL CompactHelperReservedArgumentSlot(void *, int, void *);
int LEMBALL_FASTCALL CompactHelperReturnZeroSlot15(void *);
int LEMBALL_FASTCALL CompactHelperReturnZeroSlot16(void *);
void LEMBALL_FASTCALL CompactHelperReservedArgumentSlot17(void *, int, void *);
int LEMBALL_FASTCALL CompactHelperReturnZeroSlot18(void *);
void *LEMBALL_FASTCALL DeleteStatusRenderEntryThunk(
    void *, int, unsigned char);
void LEMBALL_FASTCALL QueueStatusRenderPopThunk(void *, int, void *);
void LEMBALL_FASTCALL ExecuteStatusRenderPopThunk(void *, int, void *);
void LEMBALL_FASTCALL SetStatusRenderEntryFieldThunk(void *, int, int);
int LEMBALL_FASTCALL ReturnZeroStatusRenderThunk(void *);
void *LEMBALL_FASTCALL DeleteStatusPointSinkEntryThunk(
    void *, int, unsigned char);
void LEMBALL_FASTCALL QueueStatusPointPushThunk(void *, int, void *);
void LEMBALL_FASTCALL ExecuteStatusPointPushThunk(void *, int, void *);
void *LEMBALL_FASTCALL DeleteStatusPointSinkVariantThunk(
    void *, int, unsigned char);
void LEMBALL_FASTCALL AppendStatusPointSinkToQueueThunk(void *, int, void *);
void LEMBALL_FASTCALL DispatchStatusPointSinkOwnerThunk(void *, int, void *);
void DeleteRenderPointRectSinkThunk00401898(void *, unsigned char);
void LEMBALL_FASTCALL QueueRenderPointRectSinkThunk00402DD3(
    void *, int, void *);
void LEMBALL_FASTCALL DispatchRenderPointRectSinkThunk00401C71(
    void *, int, void *);
void LEMBALL_FASTCALL CopyRectRowsToHelperGroup0Buffer(
    void *pGroup0RowBuffer, int, void *pCopyCommand);
void LEMBALL_FASTCALL FillRectInHelperGroup0Buffer(
    void *pGroup0RowBuffer, int, void *pFillCommand);
void LEMBALL_FASTCALL CopyHelperGroup0RectToTarget(
    void *pGroup0RowBuffer, int, void *pCopyRect);
void LEMBALL_FASTCALL FillHelperSurfaceRectCommand(
    void *pCompactTarget, int, void *pCommand);
void LEMBALL_FASTCALL BlitBitmapRowsToScanlines(
    void *pCompactTarget, int, void *pCommand, void *pBitmap);

// GLOBAL: LEMBALL 0x004a0e7c
int g_fStartupFullscreen = 0;
// GLOBAL: LEMBALL 0x004a0e70
int g_fStartupGraphicsDriverWing = 0;
// GLOBAL: LEMBALL 0x004a0e74
int g_fStartupGraphicsDriverCds = 1;
// GLOBAL: LEMBALL 0x004a0e78
int g_fStartupGraphicsDriverGdk = 0;
// GLOBAL: LEMBALL 0x004a0768
void *g_pSelectedGraphicsDriverRuntime = 0;
// GLOBAL: LEMBALL 0x004a2008
static void *g_pResourceGeometryHelperSlotManager = 0;
// GLOBAL: LEMBALL 0x004a200c
void *g_pResourceGeometryHelperTarget = 0;
// GLOBAL: LEMBALL 0x004a20c0
void *g_pSingletonAnimWindowOwner = 0;
// GLOBAL: LEMBALL 0x004a9bf4
int *g_pArrowCursorStatusIndicatorRenderClient;
// GLOBAL: LEMBALL 0x004a9bf8
unsigned int g_adwGlobalResourceGeometryPaletteTable[256];
VsGdiDisplayState *g_pDisplayState = 0;
extern int g_fFullscreenDisplayModeSuspended;
int g_nDebugPresentCount = 0;
int g_nDebugStretchCount = 0;
int g_nDebugFlushCount = 0;
int g_nDebugRootFlushCount = 0;
int g_nDebugLastRectCount = -2;

/* Ghidra 0046c3bd..0046c40a: twenty consecutive RGB triples supply
 * the fallback system colors.  004a2058 holds the two application-reserved
 * output colors. */
static const unsigned char g_aabVSGDIFallbackSystemColors[20][3] = {
    { 0x00, 0x00, 0x00 }, { 0x80, 0x00, 0x00 },
    { 0x00, 0x80, 0x00 }, { 0x80, 0x80, 0x00 },
    { 0x00, 0x00, 0x80 }, { 0x80, 0x00, 0x80 },
    { 0x00, 0x80, 0x80 }, { 0xc0, 0xc0, 0xc0 },
    { 0xc0, 0xdc, 0xc0 }, { 0xa6, 0xca, 0xf0 },
    { 0xff, 0xfb, 0xf0 }, { 0xa0, 0xa0, 0xa4 },
    { 0x80, 0x80, 0x80 }, { 0xff, 0x00, 0x00 },
    { 0x00, 0xff, 0x00 }, { 0xff, 0xff, 0x00 },
    { 0x00, 0x00, 0xff }, { 0xff, 0x00, 0xff },
    { 0x00, 0xff, 0xff }, { 0xff, 0xff, 0xff }
};
static const unsigned char g_aabVSGDIReservedOutputColors[2][3] = {
    { 0xff, 0xff, 0xff }, { 0x00, 0x00, 0x00 }
};

// GLOBAL: LEMBALL 0x004a0b58
static const char g_VSGDI_DdrawDll[] = "DDRAW.DLL";
// GLOBAL: LEMBALL 0x004a0ac8
static const char g_VSGDI_Dspdib32Dll[] = "DSPDIB32.DLL";
static const char *g_VSGDI_BaseWindowClass = "VS_Base_Window_Class";
// GLOBAL: LEMBALL 0x004a0b64
static const char g_VSGDI_DirectDrawCreate[] = "DirectDrawCreate";
// GLOBAL: LEMBALL 0x004a0bc0
static const char g_VSGDI_DirectDrawWindowClass[] = "DirectDrawClass";
// GLOBAL: LEMBALL 0x004a0bb4
static const char g_VSGDI_DirectDrawWindowTitle[] = "DirectDraw";
// GLOBAL: LEMBALL 0x004a0aa8
static const char *g_pVSGDI_UnknownDirectDrawError = "UNKNOWN DIRECT DRAW ERROR: ";
// GLOBAL: LEMBALL 0x004a8190
static char g_VSGDI_DirectDrawErrorText[64];
// GLOBAL: LEMBALL 0x004a0ad8
static const char g_VSGDI_DisplayDibStart32[] = "DisplayDibStart32";
// GLOBAL: LEMBALL 0x004a0aec
static const char g_VSGDI_DisplayDibEnd32[] = "DisplayDibEnd32";
// GLOBAL: LEMBALL 0x004a0afc
static const char g_VSGDI_DisplayDibGetAddress32[] = "DisplayDibGetAddress32";
// GLOBAL: LEMBALL 0x004a0b14
static const char g_VSGDI_DisplayDib32[] = "DisplayDib32";
static const char g_VSGDI_ErrorRegisterClass[] =
    "Unable to register DD base window class\n";
static const char g_VSGDI_ErrorSetCooperativeLevel[] =
    "Direct Draw Set Coorperative Level (DD object) failed : ";
static const char g_VSGDI_ErrorSetDisplayMode[] = "Direct Draw Set Display Mode failed : ";
static const char g_VSGDI_ErrorCreatePrimarySurface[] =
    "Direct Draw Create Primary Surface failed : ";
static const char g_VSGDI_ErrorLineEnd[] = "\n";
// GLOBAL: LEMBALL 0x004a8290
FARPROC g_pApplyFullscreenDisplayModeThunk;


#define VsGdiSAFE_TABLE(name) static void *name[32]
// GLOBAL: LEMBALL 0x00499508
static void *g_VSGDI_CompactResourceGeometryHelperVtable[20] = {
    (void *)_purecall, (void *)_purecall, (void *)_purecall,
    (void *)_purecall, (void *)_purecall, (void *)_purecall,
    (void *)_purecall, (void *)_purecall, (void *)_purecall,
    (void *)_purecall, (void *)CompactHelperReservedArgumentSlot,
    (void *)_purecall, (void *)_purecall, (void *)_purecall,
    (void *)_purecall, (void *)CompactHelperReturnZeroSlot15,
    (void *)CompactHelperReturnZeroSlot16,
    (void *)CompactHelperReservedArgumentSlot17,
    (void *)CompactHelperReturnZeroSlot18, 0,
};
void LEMBALL_FASTCALL CompactHelperSubobjectSlot0(void *);
void LEMBALL_FASTCALL CompactHelperSubobjectSlot1(void *, int, void *);
void LEMBALL_FASTCALL CompactHelperSubobjectSlot2(void *);
void LEMBALL_FASTCALL CompactHelperSubobjectSlot3(void *);
void LEMBALL_FASTCALL CompactHelperSubobjectSlot4(void *);
void LEMBALL_FASTCALL CompactHelperSubobjectSlot5(void *);
void LEMBALL_FASTCALL CompactHelperSubobjectSlot9(
    void *, int, void *, void *);
void LEMBALL_FASTCALL CompactHelperSubobjectSlot11(void *);
void LEMBALL_FASTCALL CompactHelperSubobjectSlot17(void *);
void LEMBALL_FASTCALL CompactHelperSubobjectSlot18(void *);
void LEMBALL_FASTCALL ResourceGeometryRowBufferSlot4(void *);
void LEMBALL_FASTCALL ResourceGeometryRowBufferSlot5(void *);
void LEMBALL_FASTCALL ResourceGeometryRowBufferSlot6(void *);
void LEMBALL_FASTCALL ResourceGeometryRowBufferSlot8(void *);

// GLOBAL: LEMBALL 0x0049a470
static void *g_VSGDI_ResourceGeometryRowBufferVtable[14] = {
    (void *)RebuildResourceGeometryRowPointerTable,
    0,
    (void *)EnterObjectCriticalSection,
    (void *)LeaveObjectCriticalSection,
    (void *)ResourceGeometryRowBufferSlot4,
    (void *)ResourceGeometryRowBufferSlot5,
    (void *)ResourceGeometryRowBufferSlot6,
    0,
    (void *)ResourceGeometryRowBufferSlot8,
    (void *)_purecall,
    (void *)_purecall,
    0,
    (void *)EnterObjectCriticalSection,
    (void *)LeaveObjectCriticalSection,
};
VsGdiSAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup0Vtable);
VsGdiSAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable);
VsGdiSAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup1Vtable);
VsGdiSAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable);
VsGdiSAFE_TABLE(g_VSGDI_ResourceGeometryHelperTargetConstructionVtable);
VsGdiSAFE_TABLE(g_VSGDI_CompactResourceGeometryHelperConstructionVtable);
VsGdiSAFE_TABLE(g_VSGDI_ResourceGeometryHelperTargetVtable);
VsGdiSAFE_TABLE(g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable);
// GLOBAL: LEMBALL 0x00499e40
static void *g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable[4] = {
    (void *)FillRectInHelperGroup0Buffer,
    (void *)CopyRectRowsToHelperGroup0Buffer,
    (void *)CopyHelperGroup0RectToTarget,
    0,
};
// GLOBAL: LEMBALL 0x00499df0
static void *g_VSGDI_CompactResourceGeometryHelperSubobjectVtable[20] = {
    (void *)CompactHelperSubobjectSlot0,
    (void *)CompactHelperSubobjectSlot1,
    (void *)CompactHelperSubobjectSlot2,
    (void *)CompactHelperSubobjectSlot3,
    (void *)CompactHelperSubobjectSlot4,
    (void *)CompactHelperSubobjectSlot5,
    (void *)FillHelperSurfaceRectCommandAdjustmentThunk,
    (void *)ForwardBitmapCommandToAdjustedSurfaceThunk,
    (void *)BlitBitmapRowsToScanlinesAdjustmentThunk,
    (void *)CompactHelperSubobjectSlot9,
    (void *)SetHelperTargetRenderExtentAdjustmentThunk,
    (void *)CompactHelperSubobjectSlot11,
    (void *)ApplyHelperTargetPaletteAdjustmentThunk,
    (void *)PresentCompactResourceGeometryHelperAdjustmentThunk,
    (void *)GetHelperTargetTokenAdjustmentThunk,
    (void *)GetHelperGroup0DirtyFlagAdjustmentThunk,
    (void *)GetHelperGroup1DirtyFlagAdjustmentThunk,
    (void *)CompactHelperSubobjectSlot17,
    (void *)CompactHelperSubobjectSlot18,
    0,
};
struct VsGdiTargetParamWrapperVtable {
    void *(LEMBALL_FASTCALL *m_pDelete)(void *, int, int);
    void (LEMBALL_FASTCALL *m_pSetDisplayDc)(void *, int, void *);
    void *(LEMBALL_FASTCALL *m_pDeleteBase)(void *, int, int);
    void (LEMBALL_FASTCALL *m_pNoopArgument)(void *, int, void *);
    void *(LEMBALL_FASTCALL *m_pDeleteInterface)(void *, int, int);
    int (LEMBALL_FASTCALL *m_pReturnTrueA)(void *, int);
    int (LEMBALL_FASTCALL *m_pReturnTrueB)(void *, int);
    int (LEMBALL_FASTCALL *m_pGetDisplayDc)(void *, int);
    int (LEMBALL_FASTCALL *m_pGetReserved)(void *, int);
    void *m_pTerminator;
};
// GLOBAL: LEMBALL 0x00498770
static VsGdiTargetParamWrapperVtable g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable = {
        DeleteHelperTargetParamWrapper,
        SetHelperTargetParamWrapperDisplayDc,
        DeleteHelperTargetParamWrapperBase,
        NoopHelperTargetParamWrapperArgument,
        DeleteHelperTargetParamWrapperInterface,
        ReturnTrueHelperTargetParamWrapperA,
        ReturnTrueHelperTargetParamWrapperB,
        GetHelperTargetParamWrapperDisplayDc,
        GetHelperTargetParamWrapperReserved,
        0,
    };
VsGdiSAFE_TABLE(g_VSGDI_ResourceGeometryHelperSlotManagerVtable);
// GLOBAL: LEMBALL 0x00499d38
static void *g_VSGDI_StatusIndicatorBaseVtable[8] = {
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteStatusIndicatorRenderClientBaseThunk,
    (void *)HandleStatusIndicatorInputEvent,
    (void *)DispatchStatusIndicatorRenderEntry,
    (void *)_purecall,
    (void *)_purecall,
    (void *)NoopStatusIndicatorRenderClientSlot,
    0,
};
// GLOBAL: LEMBALL 0x00499d58
static void *g_VSGDI_ArrowCursorRuntimeVtable[8] = {
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteArrowCursorRenderClientThunk,
    (void *)HandleStatusIndicatorInputEvent,
    (void *)DispatchStatusIndicatorRenderEntry,
    (void *)HideArrowCursor,
    (void *)ShowArrowCursor,
    (void *)InitializeArrowCursorPosition,
    0,
};
// GLOBAL: LEMBALL 0x00496e50
static void *g_VSGDI_StatusIndicatorRenderClientVtable[8] = {
    (void *)DeleteStatusRenderEntryThunk,
    (void *)QueueStatusRenderPopThunk,
    (void *)ExecuteStatusRenderPopThunk,
    0,
    (void *)SetStatusRenderEntryFieldThunk,
    (void *)ReturnZeroStatusRenderThunk,
    0,
    0,
};
// GLOBAL: LEMBALL 0x00496e70
static void *g_VSGDI_StatusIndicatorPointSinkVtable[12] = {
    (void *)DeleteStatusPointSinkEntryThunk,
    (void *)QueueStatusPointPushThunk,
    (void *)ExecuteStatusPointPushThunk,
    0,
    (void *)DeleteStatusPointSinkVariantThunk,
    (void *)AppendStatusPointSinkToQueueThunk,
    (void *)DispatchStatusPointSinkOwnerThunk,
    0,
    (void *)DeleteRenderPointRectSinkThunk00401898,
    (void *)QueueRenderPointRectSinkThunk00402DD3,
    (void *)DispatchRenderPointRectSinkThunk00401C71,
    0,
};

static void *LEMBALL_FASTCALL VsGdiDeleteDisplayState(
    void *pDisplayState, int nUnused, int fDelete);
int LEMBALL_FASTCALL HandleStatusIndicatorInputEvent(
    void *pRenderClient, int nUnused, RenderDispatchQueueEntry *pEntry);
void LEMBALL_FASTCALL InitializeArrowCursorPosition(void *pRenderClient);
void LEMBALL_FASTCALL HideArrowCursor(void *pRenderClient);
void LEMBALL_FASTCALL ShowArrowCursor(void *pRenderClient);

void LEMBALL_FASTCALL ApplyHelperTargetPaletteAdjustmentThunk(
    void *pCompactTarget, int nUnused, void *pPalette);
void LEMBALL_FASTCALL SetHelperTargetRenderExtentAdjustmentThunk(
    void *pCompactTarget, int nUnused, const VsGdiRect *pExtent);
int LEMBALL_FASTCALL GetHelperGroup0DirtyFlagAdjustmentThunk(
    void *pCompactTarget, int nUnused);
int LEMBALL_FASTCALL GetHelperGroup1DirtyFlagAdjustmentThunk(
    void *pCompactTarget, int nUnused);
unsigned char *LEMBALL_FASTCALL GetHelperTargetTokenAdjustmentThunk(
    void *pCompactTarget);
void LEMBALL_FASTCALL FillHelperSurfaceRectCommandAdjustmentThunk(
    void *pCompactTarget, int nUnused, void *pCommand);
void LEMBALL_FASTCALL ForwardBitmapCommandToAdjustedSurfaceThunk(void);
void LEMBALL_FASTCALL BlitBitmapRowsToScanlines(
    void *pCompactTarget, int nUnused, void *pCommand, void *pBitmap);
void LEMBALL_FASTCALL BlitBitmapRowsToScanlinesAdjustmentThunk(
    void *pCompactTarget, int nUnused, void *pCommand, void *pBitmap);
void LEMBALL_FASTCALL PresentCompactResourceGeometryHelper(
    void *pCompactTarget);
void LEMBALL_FASTCALL PresentCompactResourceGeometryHelperAdjustmentThunk(
    void *pCompactTarget);
void LEMBALL_FASTCALL CopyRectRowsToHelperGroup0Buffer(
    void *pGroup0RowBuffer, int nUnused, void *pCopyCommand);
void LEMBALL_FASTCALL FillRectInHelperGroup0Buffer(
    void *pGroup0RowBuffer, int nUnused, void *pFillCommand);
void LEMBALL_FASTCALL CopyHelperGroup0RectToTarget(
    void *pGroup0RowBuffer, int nUnused, void *pCopyRect);
void LEMBALL_FASTCALL SetHelperTargetParamWrapperDisplayDc(
    void *pParamWrapper, int nUnused, void *hDc);
void *LEMBALL_FASTCALL DeleteHelperTargetParamWrapper(
    void *pParamWrapper, int nUnused, int fDelete);
void LEMBALL_FASTCALL NoopHelperTargetParamWrapperArgument(
    void *pParamWrapper, int nUnused, void *pArgument);
void *LEMBALL_FASTCALL DeleteHelperTargetParamWrapperBase(
    void *pParamWrapper, int nUnused, int fDelete);
void *LEMBALL_FASTCALL DeleteHelperTargetParamWrapperInterface(
    void *pParamWrapper, int nUnused, int fDelete);
int LEMBALL_FASTCALL ReturnTrueHelperTargetParamWrapperA(
    void *pParamWrapper, int nUnused);
int LEMBALL_FASTCALL ReturnTrueHelperTargetParamWrapperB(
    void *pParamWrapper, int nUnused);
int LEMBALL_FASTCALL GetHelperTargetParamWrapperDisplayDc(
    void *pParamWrapper, int nUnused);
int LEMBALL_FASTCALL GetHelperTargetParamWrapperReserved(
    void *pParamWrapper, int nUnused);


#undef VsGdiSAFE_TABLE

struct VsGdiSafeTableInitializer {
    VsGdiSafeTableInitializer(void) {
        void **tables[] = {
            g_VSGDI_ResourceGeometryHelperGroup0Vtable,
            g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable,
            g_VSGDI_ResourceGeometryHelperGroup1Vtable,
            g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable,
            g_VSGDI_ResourceGeometryHelperTargetConstructionVtable,
            g_VSGDI_CompactResourceGeometryHelperConstructionVtable,
            g_VSGDI_ResourceGeometryHelperTargetVtable,
            g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable,
            g_VSGDI_ResourceGeometryHelperSlotManagerVtable
        };
        int i;
        int j;
        for (i = 0; i < 9; ++i)
            for (j = 0; j < 32; ++j)
                tables[i][j] = (void *)NetworkSafeVtableNoop;

        g_VSGDI_ResourceGeometryHelperTargetVtable[2] =
            (void *)GetResourceGeometryHelperTargetActiveUploadState;
    }
};
static VsGdiSafeTableInitializer g_VSGDI_SafeTableInitializer;

struct VsGdiCompactHelperAdjustorTable {
    int m_nThisDelta;
    int m_nSubobjectOffset;
};

/* Ghidra 00466440/00466670: both temporary row-group adjustors are {-4, 0x54}. */
static VsGdiCompactHelperAdjustorTable g_VSGDI_ResourceGeometryHelperGroup0ConstructionAdjustorVtable = {
    -4, 0x54
};
static VsGdiCompactHelperAdjustorTable g_VSGDI_ResourceGeometryHelperGroup1ConstructionAdjustorVtable = {
    -4, 0x54
};

static VsGdiCompactHelperAdjustorTable g_VSGDI_CompactHelperTargetAdjustor = { -0x40, 0x51c };
static VsGdiCompactHelperAdjustorTable g_VSGDI_HelperGroup1Adjustor = { -4, 0x514 };
static VsGdiCompactHelperAdjustorTable g_VSGDI_HelperGroup0Adjustor = { -4, 0x4c0 };

static const char g_VSGDI_UnallocatedHelperSlotMessage[] = "Unallocated helper slot found\n";
static int g_nResourceGeometryHelperSlotCount = 0;
static void *g_pResourceGeometryListHead = 0;

struct VsGdiHelperTargetParamWrapper {
    void *m_pVtable;
    int m_nWrappedParam;
    int m_nReserved;
};

struct VsGdiResourceGeometryHelperSlot {
    int m_nOwnerOrState0;
    int m_nOwnerOrState4;
    int m_nOwnerOrState8;
    int m_nOwnerOrStateC;
    int m_nOwnerOrState10;
    int m_fAvailable;
    int m_nOwnerOrState18;
};

struct VsGdiResourceGeometryHelperSlotManager {
    void *m_pVtable;
    VsGdiResourceGeometryHelperSlot *m_pSlots;
    int m_nReserved08;
    int m_nReserved0C;
    int m_cSlots;

    VsGdiResourceGeometryHelperSlotManager *InitializeResourceGeometryHelperSlotManager(int cSlots);
    void *AllocateResourceGeometryHelperSlot(short *pRect, void *pWrappedTarget);
    void RemoveResourceGeometryHelperSlot(void *pTarget);
    int FindResourceGeometryHelperSlot(void *pTarget);
    void DeactivateResourceGeometryHelperSlot(void *pTarget);
};










struct VsGdiResourceGeometryLinkNode {
    void *m_pTarget;
    VsGdiResourceGeometryLinkNode *m_pPrev;
    VsGdiResourceGeometryLinkNode *m_pNext;
};

struct VsGdiResourceGeometryLinkList {
    VsGdiResourceGeometryLinkNode *m_pTail;
    VsGdiResourceGeometryLinkNode *m_pHead;
    int m_cNodes;
};

struct VsGdiCompactResourceGeometryHelperFields {
    unsigned char m_abReserved00[0x3c];
    int m_nThisAdjust;
    void *m_pVtable;
    unsigned char m_abReserved44[0x1c];
    void *m_pLinkedTarget;
    unsigned char m_abReserved64[0xc];
    int m_fForwardingEnabled;
    int m_nQueuedState;
    int m_nReserved78;
};

struct VsGdiResourceGeometryHelperTargetView {
    unsigned char m_abRowBuffer[0x44];
    unsigned char m_abHelperGroup1[0x54];
    unsigned char m_abHelperGroup0[0x54];
    short m_nReservedEC;
    short m_nReservedEE;
    void *m_pBackingBitmap;
    int m_nReservedF4;
    void *m_pDisplayBinding;
    unsigned char m_abReservedFC[0x42c];
    VsGdiResourceGeometryLinkList m_ChildTargetLinks;
    unsigned char m_abCriticalSection[0x18];
    int m_fCriticalSectionReady;
    int m_nActiveUploadState;
    unsigned char m_abReserved554[8];

    VsGdiResourceGeometryHelperTargetView *ConstructResourceGeometryHelperTarget(
        int nWrappedParam, int fConstructCompactHelper);
    VsGdiResourceGeometryHelperTargetView *InitializeResourceGeometryHelper(
        const VsGdiRect *pRect, void *pWrappedTarget, int fConstructCompactHelper);
    void MarkOrForwardHelperTargetDirtyRect(short *pRect);
    void FlushResourceGeometryHelperTargetUpdates(
        VsGdiResourceGeometryHelperTargetView *pPeerTarget);
};

struct VsGdiGeometryHelperPointerArray {
    void *m_pEntries;
    int m_nReserved;
    int m_nEntryCount;
    VsGdiResourceGeometryHelperTargetView *m_pTarget;

    VsGdiGeometryHelperPointerArray *InitializeGeometryHelperPointerArray(
        short *pRect, int nEntryParameter, void *pWrappedTarget);
};

struct VsGdiHelperSurfaceVariableBlock {
    unsigned char m_abReserved00[0x4c];
    short m_aRequestedRect[4];
    short m_aWorkingRect[4];
    int m_nReserved5C;
    void *m_pLinkedTarget;
    short m_aBackingRect[2];
    unsigned char m_abReserved68[0x18];
    int m_nBackingWidthHint;
};

struct VsGdiHelperSurfaceBindingSurface {
    void **m_pVtable;
    unsigned char m_abReserved04[0x10];
    short m_nOriginY;
    short m_nOriginX;
};

struct VsGdiHelperDispatchQueue {
    void **m_ppEntries;
    int m_cEntries;
    int m_nReserved08;
    VsGdiHelperSurface *m_pHelperTarget;
};

struct VsGdiQuantizedHelperUploadRectTracker {
    int InitializeQuantizedHelperUploadRectTracker(
        int cRectEntries, int nTrackerExtent, int nCellExtent);
    void MarkHelperUploadCellsForRect(short *pRect);
    void MarkHelperUploadRectWithState(short *pRect,
                                       unsigned char bState);
    int ConsumeMarkedHelperUploadRun(unsigned char bTargetState,
                                     int nEntryState,
                                     unsigned char bReplacementState);
    int BuildHelperUploadRectList(void);
    short *GetHelperUploadRectEntryByIndex(int iRect);
    int GetHelperUploadActiveRectStartIndex(void);
};



/* The target+0x98 row-buffer view receives a two-point/two-extent copy
 * command.  Ghidra 00474dd0 and table 00499e40 slot 1 show that the source
 * rows belong to the target base while the destination rows belong to this
 * helper-group view. */

void LEMBALL_FASTCALL InitializeHelperUploadStatePending(int nUploadState);









extern void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
extern int g_fRootHelperGeometryDispatchSuppressed;
extern void *g_pQueuedRenderPointSinkFinalizeThunk;
static int GetHelperSurfaceVariableBlockOffset(const VsGdiHelperSurface *pSurface);


static void ReleaseResourceGeometryRowBufferStorage(int nRowBuffer) {
    if (*(int *)(unsigned long)(nRowBuffer + 4) != 0) {
        FreeVSMemBlock((void *)(unsigned long)*(int *)(unsigned long)(nRowBuffer + 4));
    }
    ClearResourceGeometryRowBufferFields((void *)(unsigned long)nRowBuffer);
}



struct VsGdiCompactResourceGeometryHelper {
    void *InitializeCompactResourceGeometryHelper(void);
};

extern "C" unsigned long LEMBALL_STDCALL DispatchVisualSciencesWingDriverMessage(
    void *pState, void *hDriver, unsigned int nMessage,
    void *pParam1, unsigned int nParam2);

class VsGdiAnimWindowOwner {
public:
    void *ConstructResourceGeometryHelperWindowOwner(void);
    void BindAsSingletonAnimWindow(void);
    void RebuildResourceGeometryHelperWindow(void);
    void InstallEmbeddedMciDrawCallback(void);
    void StartEmbeddedMciWindowPlaybackOnce(void);
    void ResumeEmbeddedMciWindowPlayback(void);
    void UpdateMediaPathFromListResource(int nResourceId);
    void SelectIntroSequenceMediaPath(void);

private:
    unsigned char m_abReserved00[0x90];
    int m_nReserved90;
    int m_nReserved94;
    GameDynamicCString m_MediaName98;
    int m_nA0;
    int m_nA4;
    int m_nA8;
    int m_nAC;
    GameDynamicCString m_MediaPathB0;
    HWND m_hMciWindowB8;
};

static void *g_VSGDI_AnimWindowOwnerVtable[50];
static int g_fVSGDIAnimWindowOwnerVtableInitialized;









void *InitializeResourceGeometryHelperSlotManager(void *pvSlotManager, int cSlots) {
    return ((VsGdiResourceGeometryHelperSlotManager *)pvSlotManager)
        ->InitializeResourceGeometryHelperSlotManager(cSlots);
}





struct VsGdiResourceGeometryHelperSlotManagerVtableFixup {
    VsGdiResourceGeometryHelperSlotManagerVtableFixup(void) {
        union {
            void *(VsGdiResourceGeometryHelperSlotManager::*p)(short *, void *);
            void *v;
        } allocateMethod;
        union {
            void (VsGdiResourceGeometryHelperSlotManager::*p)(void *);
            void *v;
        } voidMethod;
        union {
            int (VsGdiResourceGeometryHelperSlotManager::*p)(void *);
            void *v;
        } findMethod;
        allocateMethod.p = &VsGdiResourceGeometryHelperSlotManager::
            AllocateResourceGeometryHelperSlot;
        voidMethod.p = &VsGdiResourceGeometryHelperSlotManager::
            RemoveResourceGeometryHelperSlot;
        g_VSGDI_ResourceGeometryHelperSlotManagerVtable[1] = voidMethod.v;
        findMethod.p = &VsGdiResourceGeometryHelperSlotManager::
            FindResourceGeometryHelperSlot;
        g_VSGDI_ResourceGeometryHelperSlotManagerVtable[2] = findMethod.v;
        voidMethod.p = &VsGdiResourceGeometryHelperSlotManager::
            DeactivateResourceGeometryHelperSlot;
        g_VSGDI_ResourceGeometryHelperSlotManagerVtable[0] = allocateMethod.v;
        g_VSGDI_ResourceGeometryHelperSlotManagerVtable[3] = voidMethod.v;
    }
};
static VsGdiResourceGeometryHelperSlotManagerVtableFixup
    g_VSGDI_ResourceGeometryHelperSlotManagerVtableFixup;



void *ConstructResourceGeometryHelperTarget(void *pvTarget, int nWrappedParam, int fConstructCompactHelper) {
    return ((VsGdiResourceGeometryHelperTargetView *)pvTarget)
        ->ConstructResourceGeometryHelperTarget(nWrappedParam, fConstructCompactHelper);
}



struct VsGdiResourceGeometryHelperSlotReleaseInterface {
    virtual void Reserved0(void) = 0;
    virtual void ReleaseResourceGeometryHelperSlot(void *pTarget) = 0;
};


struct VsGdiDisplayDcBindingInterface {
    virtual void Reserved0(void) = 0;
    virtual void BindDisplayDc(void *hDc) = 0;
};


struct VsGdiSystemPalettePayload {
    unsigned short m_nVersion;
    unsigned short m_cEntries;
    unsigned char m_abEntries[256][4];
};

struct VsGdiDisplayPaletteDispatch {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void Reserved3(void) = 0;
    virtual void Reserved4(void) = 0;
    virtual void Reserved5(void) = 0;
    virtual int ApplySurfacePalette(void *pSurface,
                                    int nFirstEntry,
                                    int cEntries,
                                    const unsigned int *paColors) = 0;
    virtual void Reserved7(void) = 0;
    virtual void Reserved8(void) = 0;
    virtual void Reserved9(void) = 0;
    virtual void Reserved10(void) = 0;
    virtual int ApplySystemPalette(void *pPalette) = 0;
    virtual int SelectAndRealizePalette(void *pDisplayDc) = 0;
    virtual int UsesExistingPaletteAsFallback(void) = 0;
};


















struct VsGdiLogPalette256 {
    unsigned short m_nVersion;
    unsigned short m_cEntries;
    struct VsGdiPaletteEntry {
        unsigned char m_bRed;
        unsigned char m_bGreen;
        unsigned char m_bBlue;
        unsigned char m_bFlags;
    } m_aEntries[256];
};

struct VsGdiDisplayPaletteStateView {
    void *m_pVtable;
    HMODULE m_hGraphicsModule;
    void *m_hPalette;
};

struct VsGdiDisplayDcView {
    void *m_pVtable;
    void *m_hDc;
};

extern "C" {
__declspec(dllimport) void *WINAPI GetDC(HWND hWnd);
__declspec(dllimport) int WINAPI ReleaseDC(HWND hWnd, void *hDc);
__declspec(dllimport) UINT WINAPI GetSystemPaletteEntries(
    void *hDc, UINT nFirstEntry, UINT cEntries, void *pEntries);
__declspec(dllimport) BOOL WINAPI DeleteObject(HGDIOBJ hObject);
__declspec(dllimport) void *WINAPI CreatePalette(const void *pPalette);
__declspec(dllimport) void *WINAPI SelectPalette(
    void *hDc, void *hPalette, BOOL fForceBackground);
__declspec(dllimport) UINT WINAPI RealizePalette(void *hDc);
__declspec(dllimport) UINT WINAPI SetDIBColorTable(
    void *hDc, UINT nFirstEntry, UINT cEntries, const void *pColors);
__declspec(dllimport) void *WINAPI CreateCompatibleDC(void *hDc);
__declspec(dllimport) BOOL WINAPI DeleteDC(void *hDc);
__declspec(dllimport) void *WINAPI CreateDIBSection(
    void *hDc, const void *pBitmapInfo, UINT nUsage, void **ppBits,
    HANDLE hSection, DWORD dwOffset);
__declspec(dllimport) BOOL WINAPI BitBlt(
    void *hDcDestination, int xDestination, int yDestination,
    int cx, int cy, void *hDcSource, int xSource, int ySource,
    DWORD dwRasterOperation);
__declspec(dllimport) BOOL WINAPI StretchBlt(
    void *hDcDestination, int xDestination, int yDestination,
    int cxDestination, int cyDestination, void *hDcSource,
    int xSource, int ySource, int cxSource, int cySource,
    DWORD dwRasterOperation);
__declspec(dllimport) HGDIOBJ WINAPI SelectObject(
    void *hDc, HGDIOBJ hObject);
__declspec(dllimport) BOOL WINAPI GdiFlush(void);
}










































static int GetHelperSurfaceVariableBlockOffset(const VsGdiHelperSurface *pSurface) {
    return *(const int *)((const char *)(unsigned long)*(const int *)((const char *)pSurface + 0x40) + 4);
}


struct VsGdiResourceGeometryVtableFixup {
    VsGdiResourceGeometryVtableFixup(void) {
        union {
            void (VsGdiResourceGeometryHelperTargetView::*p)(short *);
            void *v;
        } markDirtyMethod;
        markDirtyMethod.p = &VsGdiResourceGeometryHelperTargetView::MarkOrForwardHelperTargetDirtyRect;
        g_VSGDI_ResourceGeometryHelperTargetVtable[1] = markDirtyMethod.v;
    }
};
static VsGdiResourceGeometryVtableFixup g_VSGDI_ResourceGeometryVtableFixup;




static const char *g_apszGraphicsDriverNames[] = {
    "NO",
    "CDS",
    "VGA (Full Screen 320*200)",
    "VGA (Full Screen 320*240)",
    "Direct Draw (Full Screen 640*480)",
    "Direct Draw (Full Screen 320*200)",
    "Direct Draw (Windowed 640*480)",
    "Direct Draw (Windowed 320*200)",
    "Auto Select",
};

static const char *g_apszGraphicsDriverFallbackMessages[] = {
    "None",
    "Defaulting to normal 640*480 mode (using CreateDIBSection)",
    "Unable to find DispDib32 Libraries for full screen 320*200 mode (dspdib16.dll & dspdib32.dll) - please reinstall",
    "Unable to find DispDib32 Libraries for full screen 320*240 mode (dspdib16.dll & dspdib32.dll) - please reinstall",
    "Unable to find Direct Draw libraries for full screen 640*480 mode (ddraw.dll) - please reinstall the DirectX libraries",
    "Unable to find Direct Draw libraries for full screen 320*200 mode (ddraw.dll) - please reinstall the DirectX libraries",
};

static void CallDeleteVirtual(void *pObject, unsigned int nByteOffset, int nDeleteFlag) {
    void **pVtable;

    pVtable = *(void ***)pObject;
    ((void *(LEMBALL_FASTCALL *)(void *, int, int))
         pVtable[nByteOffset / sizeof(void *)])(pObject, 0, nDeleteFlag);
}

static int IsDisplayStateReady(const VsGdiDisplayState *pDisplayState) {
    return *(const int *)((const char *)pDisplayState + 0xc) != 0;
}

struct VsGdiDisplayBitmap {
    void **m_pVtable;
    void *m_pBits;
    int m_nStride;
    HGDIOBJ m_hPreviousBitmap;
    HGDIOBJ m_hBitmap;
};

struct VsGdiDisplayBitmapDispatch {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual int Bits(void) = 0;
    virtual int Origin(void) = 0;
};

struct VsGdiDisplayBitmapVtable {
    void *(LEMBALL_FASTCALL *m_pDelete)(void *, int, int);
    int (LEMBALL_FASTCALL *m_pReserved1)(void *, int);
    int (LEMBALL_FASTCALL *m_pReserved2)(void *, int);
    int (LEMBALL_FASTCALL *m_pDisplayDc)(void *, int);
    int (LEMBALL_FASTCALL *m_pReserved)(void *, int);
    void *m_pTerminator;
};

struct VsGdiDibRectDispatch {
    virtual void Reserved0(void) = 0;
    virtual void Reserved1(void) = 0;
    virtual void Reserved2(void) = 0;
    virtual void Reserved3(void) = 0;
    virtual void Reserved4(void) = 0;
    virtual void Reserved5(void) = 0;
    virtual void Reserved6(void) = 0;
    virtual void Reserved7(void) = 0;
    virtual int CopyDisplayRect(void *pDestinationBinding,
                                short *pDestinationRect,
                                void *pSourceBinding,
                                short *pSourcePoint) = 0;
};

struct VsGdiDeleteDispatch {
    virtual ~VsGdiDeleteDispatch(void) = 0;
};
// GLOBAL: LEMBALL 0x00498798
static VsGdiDisplayBitmapVtable g_VSGDI_DisplayBitmapVtable = {
    VsGdiDeleteDisplayBitmap,
    ReturnTrueHelperTargetParamWrapperA,
    ReturnTrueHelperTargetParamWrapperB,
    GetHelperTargetParamWrapperDisplayDc,
    GetHelperTargetParamWrapperReserved,
    0,
};
static void *g_VSGDI_MetricsDisplayVtable[14];
static void *g_VSGDI_DibDisplayVtable[14];
static void *g_VSGDI_DirectDrawDisplayVtable[14];

struct VsGdiDirectDrawBinding {
    void **m_pVtable;
    int m_nSurfaceSlot;
};

// SIZE 0x7c
struct VsGdiDirectDrawSurfaceBitmap {
    void **m_pVtable;                    // 0x00
    void *m_pCachedPixels;               // 0x04
    int m_nCachedPitch;                  // 0x08
    void *m_pSurface;                    // 0x0c
    unsigned char m_abSurfaceDesc[0x6c]; // 0x10
};

static int LEMBALL_FASTCALL LockDirectDrawSurfaceBitmap(
    VsGdiDirectDrawSurfaceBitmap *pBitmap) {
    typedef unsigned int (__stdcall *DIRECTDRAW_LOCK)(
        void *, void *, void *, unsigned int, void *);
    unsigned int uResult;

    do {
        uResult = ((DIRECTDRAW_LOCK)(*(void ***)pBitmap->m_pSurface)[25])(
            pBitmap->m_pSurface, 0, pBitmap->m_abSurfaceDesc, 0, 0);
        if (uResult == 0) {
            pBitmap->m_pCachedPixels =
                *(void **)(pBitmap->m_abSurfaceDesc + 0x24);
            pBitmap->m_nCachedPitch =
                *(int *)(pBitmap->m_abSurfaceDesc + 0x10);
            return 1;
        }
    } while (uResult != 0x887601c2UL);
    return 0;
}

static int LEMBALL_FASTCALL UnlockDirectDrawSurfaceBitmap(
    VsGdiDirectDrawSurfaceBitmap *pBitmap) {
    typedef unsigned int (__stdcall *DIRECTDRAW_UNLOCK)(void *, void *);
    unsigned int uResult;

    do {
        uResult = ((DIRECTDRAW_UNLOCK)(*(void ***)pBitmap->m_pSurface)[32])(
            pBitmap->m_pSurface, pBitmap->m_pCachedPixels);
        if (uResult == 0) {
            return 1;
        }
    } while (uResult != 0x887601c2UL);
    return 0;
}

static int LEMBALL_FASTCALL GetDirectDrawSurfaceBitmapPixels(
    VsGdiDirectDrawSurfaceBitmap *pBitmap) {
    if (pBitmap->m_pCachedPixels == 0) {
        LockDirectDrawSurfaceBitmap(pBitmap);
        UnlockDirectDrawSurfaceBitmap(pBitmap);
    }
    return (int)(unsigned long)pBitmap->m_pCachedPixels;
}

static int LEMBALL_FASTCALL GetDirectDrawSurfaceBitmapPitch(
    VsGdiDirectDrawSurfaceBitmap *pBitmap) {
    return pBitmap->m_nCachedPitch;
}

static void *LEMBALL_FASTCALL DeleteDirectDrawSurfaceBitmap(
    VsGdiDirectDrawSurfaceBitmap *pBitmap, int nUnused, int fDelete) {
    (void)nUnused;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pBitmap);
    }
    return pBitmap;
}

static void *g_VSGDI_DirectDrawSurfaceBitmapVtable[6] = {
    (void *)DeleteDirectDrawSurfaceBitmap,
    (void *)LockDirectDrawSurfaceBitmap,
    (void *)UnlockDirectDrawSurfaceBitmap,
    (void *)GetDirectDrawSurfaceBitmapPixels,
    (void *)GetDirectDrawSurfaceBitmapPitch,
    0,
};

static void *LEMBALL_FASTCALL DeleteDirectDrawBinding(
    void *pBinding, int nUnused, int fDelete) {
    (void)nUnused;
    *(void ***)pBinding =
        (void **)((char *)&g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable + 8);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pBinding);
    }
    return pBinding;
}

static void *g_VSGDI_DirectDrawBindingVtable[2] = {
    (void *)DeleteDirectDrawBinding,
    (void *)NoopHelperTargetParamWrapperArgument,
};

static void *LEMBALL_FASTCALL VsGdiDeleteDisplayState(
    void *pDisplayState, int nUnused, int fDelete) {
    (void)nUnused;
    if (fDelete != 0)
        FreeVSMemBlock(pDisplayState);
    return pDisplayState;
}












static int VsGdiDisplayStateReady(void *pDisplayState) {
    return IsDisplayStateReady((const VsGdiDisplayState *)pDisplayState);
}

static void InitializeDisplayStateVtables(void) {
    int i;
    union { void *(VsGdiDisplayState::*p)(void); void *v; } createBindingMethod;
    union { int (VsGdiDisplayState::*p)(void *); void *v; } releaseBindingMethod;
    union { int (VsGdiDisplayState::*p)(void *); void *v; } bitmapInfoMethod;
    union { void *(VsGdiDisplayState::*p)(int, void *); void *v; } createBitmapMethod;
    union { int (VsGdiDisplayState::*p)(void *); void *v; } bitmapObjectMethod;
    union { void *(VsGdiDisplayState::*p)(int, void *); void *v; } bitmapReadyMethod;
    union { void *(VsGdiDisplayState::*p)(int, void *); void *v; } bitmapDeleteMethod;
    union { int (VsGdiDisplayState::*p)(void) const; void *v; } readyMethod;
    union { int (VsGdiDirectDrawDisplayState::*p)(int, void *); void *v; }
        bindDirectDrawSurfaceMethod;
    union { int (VsGdiDirectDrawDisplayState::*p)(int, void *); void *v; }
        releaseDirectDrawSurfaceMethod;
    union {
        int (VsGdiDirectDrawDisplayState::*p)(void *, short *, void *, short *);
        void *v;
    } copyDirectDrawSurfaceMethod;
    union {
        int (VsGdiDirectDrawDisplayState::*p)(void *, short *, void *, short *);
        void *v;
    } stretchDirectDrawSurfaceMethod;
    union { int (VsGdiDirectDrawDisplayState::*p)(void *); void *v; }
        applyDirectDrawPaletteMethod;
    union { void *(VsGdiDirectDrawDisplayState::*p)(int); void *v; }
        deleteDirectDrawDisplayStateMethod;
    union { void *(VsGdiDirectDrawDisplayState::*p)(void); void *v; }
        createDirectDrawBindingMethod;
    union { int (VsGdiDirectDrawDisplayState::*p)(void *); void *v; }
        releaseDirectDrawBindingMethod;
    union { int (VsGdiDirectDrawDisplayState::*p)(void *); void *v; }
        initializeDirectDrawBitmapInfoMethod;
    union { void *(VsGdiDirectDrawDisplayState::*p)(int, void *); void *v; }
        createDirectDrawDisplayBitmapMethod;
    union { int (VsGdiDirectDrawDisplayState::*p)(void *); void *v; }
        releaseDirectDrawDisplayBitmapMethod;
    union { void *(VsGdiDibDisplayState::*p)(int); void *v; }
        deleteDibDisplayStateMethod;
    union {
        int (VsGdiDibDisplayState::*p)(void *, short *, void *, short *);
        void *v;
    } stretchDibRectMethod;
    union { void *(VsGdiMetricsDisplayState::*p)(int); void *v; }
        deleteMetricsDisplayStateMethod;

    for (i = 0; i < 14; ++i) {
        g_VSGDI_MetricsDisplayVtable[i] = (void *)NetworkSafeVtableNoop;
        g_VSGDI_DibDisplayVtable[i] = (void *)NetworkSafeVtableNoop;
        g_VSGDI_DirectDrawDisplayVtable[i] = (void *)NetworkSafeVtableNoop;
    }
    deleteMetricsDisplayStateMethod.p = &VsGdiMetricsDisplayState::Delete;
    g_VSGDI_MetricsDisplayVtable[0] = deleteMetricsDisplayStateMethod.v;
    deleteDibDisplayStateMethod.p = &VsGdiDibDisplayState::Delete;
    g_VSGDI_DibDisplayVtable[0] = deleteDibDisplayStateMethod.v;
    deleteDirectDrawDisplayStateMethod.p =
        &VsGdiDirectDrawDisplayState::Delete;
    g_VSGDI_DirectDrawDisplayVtable[0] =
        deleteDirectDrawDisplayStateMethod.v;
    createBindingMethod.p = &VsGdiDisplayState::CreateDisplayBinding;
    releaseBindingMethod.p = &VsGdiDisplayState::ReleaseDisplayBinding;
    bitmapInfoMethod.p = &VsGdiDisplayState::InitializeDisplayBitmapInfo;
    createBitmapMethod.p = &VsGdiDisplayState::CreateDisplayBitmap;
    bitmapObjectMethod.p = &VsGdiDisplayState::ReleaseDisplayBitmap;
    bitmapReadyMethod.p = &VsGdiDisplayState::BindDisplayBitmap;
    bitmapDeleteMethod.p = &VsGdiDisplayState::DeleteDisplayBitmap;
    bindDirectDrawSurfaceMethod.p =
        &VsGdiDirectDrawDisplayState::BindDirectDrawSurface;
    releaseDirectDrawSurfaceMethod.p =
        &VsGdiDirectDrawDisplayState::ReleaseDirectDrawSurface;
    copyDirectDrawSurfaceMethod.p =
        &VsGdiDirectDrawDisplayState::CopyDirectDrawSurfaceRect;
    stretchDirectDrawSurfaceMethod.p =
        &VsGdiDirectDrawDisplayState::StretchDirectDrawSurfaceRect;
    applyDirectDrawPaletteMethod.p =
        &VsGdiDirectDrawDisplayState::ApplyDirectDrawPalette;
    readyMethod.p = &VsGdiDisplayState::IsReady;
    createDirectDrawBindingMethod.p =
        &VsGdiDirectDrawDisplayState::CreateDirectDrawDisplayBinding;
    releaseDirectDrawBindingMethod.p =
        &VsGdiDirectDrawDisplayState::ReleaseDirectDrawDisplayBinding;
    initializeDirectDrawBitmapInfoMethod.p =
        &VsGdiDirectDrawDisplayState::InitializeDirectDrawBitmapInfo;
    createDirectDrawDisplayBitmapMethod.p =
        &VsGdiDirectDrawDisplayState::CreateDirectDrawDisplayBitmap;
    releaseDirectDrawDisplayBitmapMethod.p =
        &VsGdiDirectDrawDisplayState::ReleaseDirectDrawDisplayBitmap;
    g_VSGDI_MetricsDisplayVtable[1] = createBindingMethod.v;
    g_VSGDI_DibDisplayVtable[1] = createBindingMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[1] = createDirectDrawBindingMethod.v;
    g_VSGDI_MetricsDisplayVtable[2] = releaseBindingMethod.v;
    g_VSGDI_DibDisplayVtable[2] = releaseBindingMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[2] = releaseDirectDrawBindingMethod.v;
    g_VSGDI_MetricsDisplayVtable[3] = bitmapInfoMethod.v;
    g_VSGDI_DibDisplayVtable[3] = bitmapInfoMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[3] = initializeDirectDrawBitmapInfoMethod.v;
    g_VSGDI_MetricsDisplayVtable[4] = createBitmapMethod.v;
    g_VSGDI_DibDisplayVtable[4] = createBitmapMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[4] = createDirectDrawDisplayBitmapMethod.v;
    g_VSGDI_MetricsDisplayVtable[5] = bitmapObjectMethod.v;
    g_VSGDI_DibDisplayVtable[5] = bitmapObjectMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[5] = releaseDirectDrawDisplayBitmapMethod.v;
    g_VSGDI_MetricsDisplayVtable[6] =
        (void *)ApplyDisplaySurfacePalette;
    g_VSGDI_DirectDrawDisplayVtable[6] =
        (void *)ApplyDirectDrawSurfacePalette;
    g_VSGDI_MetricsDisplayVtable[7] =
        (void *)StretchDisplaySurfaceRect;
    stretchDibRectMethod.p = &VsGdiDibDisplayState::StretchDisplayRect;
    g_VSGDI_DibDisplayVtable[7] = stretchDibRectMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[7] = stretchDirectDrawSurfaceMethod.v;
    g_VSGDI_MetricsDisplayVtable[8] =
        (void *)CopyDisplaySurfaceRect;
    g_VSGDI_DirectDrawDisplayVtable[8] = copyDirectDrawSurfaceMethod.v;
    {
        union {
            int (VsGdiDibDisplayState::*p)(void *, short *, void *, short *);
            void *v;
        } copyDibRectMethod;
        copyDibRectMethod.p = &VsGdiDibDisplayState::CopyDisplayRect;
        g_VSGDI_DibDisplayVtable[8] = copyDibRectMethod.v;
    }
    g_VSGDI_MetricsDisplayVtable[9] = bitmapReadyMethod.v;
    g_VSGDI_DibDisplayVtable[9] = bitmapReadyMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[9] = bindDirectDrawSurfaceMethod.v;
    g_VSGDI_MetricsDisplayVtable[10] = bitmapDeleteMethod.v;
    g_VSGDI_DibDisplayVtable[10] = bitmapDeleteMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[10] = releaseDirectDrawSurfaceMethod.v;
    g_VSGDI_MetricsDisplayVtable[11] =
        (void *)CreateAndStoreDisplayPalette;
    g_VSGDI_DirectDrawDisplayVtable[11] = applyDirectDrawPaletteMethod.v;
    g_VSGDI_MetricsDisplayVtable[12] =
        (void *)SelectAndRealizeDisplayPalette;
    g_VSGDI_DirectDrawDisplayVtable[12] =
        (void *)SelectAndRealizeDisplayPalette;
    g_VSGDI_MetricsDisplayVtable[13] =
        (void *)UsesExistingDisplayPaletteAsFallback;
    g_VSGDI_DibDisplayVtable[13] = readyMethod.v;
    g_VSGDI_DirectDrawDisplayVtable[13] = readyMethod.v;
}

struct VsGdiDisplayStateVtableInitializer {
    VsGdiDisplayStateVtableInitializer(void) {
        InitializeDisplayStateVtables();
    }
};
static VsGdiDisplayStateVtableInitializer g_VSGDI_DisplayStateVtableInitializer;

inline VsGdiDisplayState::VsGdiDisplayState(void) {
    m_pVtable = 0;
    m_hGraphicsModule = 0;
    m_pReserved08 = 0;
    m_fReady = 0;
    m_hDisplayWindow = 0;
    m_cxDisplay = 0;
    m_cyDisplay = 0;
    m_dwReserved18 = 0;
}

int VsGdiDisplayState::IsReady(void) const {
    return m_fReady;
}

short VsGdiDisplayState::Width(void) const {
    return m_cxDisplay;
}

short VsGdiDisplayState::Height(void) const {
    return m_cyDisplay;
}

VsGdiHelperSurface::VsGdiHelperSurface(void) {
    memset(m_abState, 0, sizeof(m_abState));
}




int VsGdiHelperSurface::IsReady(void) const {
    return *(const int *)((const char *)this + 0x524) != 0;
}

short VsGdiHelperSurface::BackingWidth(void) const {
    return *(const short *)((const char *)this + GetHelperSurfaceVariableBlockOffset(this) + 0x64);
}

short VsGdiHelperSurface::BackingHeight(void) const {
    return *(const short *)((const char *)this + GetHelperSurfaceVariableBlockOffset(this) + 0x66);
}


int VsGdiMetricsDisplayState::Create(HWND hWnd) {
    m_hDisplayWindow = hWnd;
    return m_fReady;
}


int VsGdiDibDisplayState::Create(HWND hWnd) {
    m_hDisplayWindow = hWnd;
    return m_fReady;
}


int VsGdiDirectDrawDisplayState::Create(HWND hWnd) {
    m_hDisplayWindow = hWnd;
    (void)g_hApplicationInstance;
    (void)g_VSGDI_BaseWindowClass;
    (void)g_VSGDI_DirectDrawWindowClass;
    (void)g_VSGDI_DirectDrawWindowTitle;
    (void)g_VSGDI_ErrorRegisterClass;
    return m_fReady;
}





int InitializeSelectedGraphicsDriver(int nRequestedDriver) {
    return ((VsGdiSelectedGraphicsDriverRuntime *)g_pSelectedGraphicsDriverRuntime)
        ->InitializeSelectedGraphicsDriver(nRequestedDriver);
}

int GetSelectedGraphicsDriverId(void) {
    if (g_pSelectedGraphicsDriverRuntime == 0) {
        return VsGdiDRIVER_AUTO;
    }

    return ((VsGdiSelectedGraphicsDriverRuntime *)g_pSelectedGraphicsDriverRuntime)->m_nSelectedDriver;
}

VsGdiDisplayState *GetDisplayState(void) {
    return g_pDisplayState;
}

// FUNCTION: LEMBALL 0x00402fea
void __stdcall PromoteQueuedRenderPointSinkTargetStateThunk(void *pPointSink) {
    PromoteQueuedRenderPointSinkTargetState(pPointSink);
}

// FUNCTION: LEMBALL 0x0040381e
void LEMBALL_FASTCALL QueueQueuedRenderPointSinkThunk(
    void *pPointSink, void *, void *pQueue)

{
    QueueQueuedRenderPointSink(pPointSink, 0, pQueue);
}

// FUNCTION: LEMBALL 0x00432380
void LEMBALL_FASTCALL QueueQueuedRenderPointSink(
    void *pPointSink, void *, void *pQueue)

{
    AppendPointerQueueEntry(pQueue, pPointSink);
}

// FUNCTION: LEMBALL 0x00432390
void __stdcall PromoteQueuedRenderPointSinkTargetState(void *pPointSink) {
    int nUploadState;
    struct TargetState {
        virtual void Reserved0();
        virtual void Reserved1();
        virtual int GetUploadState();
    };

    nUploadState = ((TargetState *)*(void ***)((char *)pPointSink + 0x0c))->GetUploadState();
    PromoteHelperUploadStateToActive(nUploadState);
}

// FUNCTION: LEMBALL 0x00456720
char *LookupDirectDrawErrorString(unsigned int nError) {
    const char *pszPrefix = g_pVSGDI_UnknownDirectDrawError;

    strcpy(g_VSGDI_DirectDrawErrorText, pszPrefix);
    FormatSignedIntToRadixString(
        (int)nError,
        g_VSGDI_DirectDrawErrorText + strlen(pszPrefix),
        10);
    return g_VSGDI_DirectDrawErrorText;
}

// FUNCTION: LEMBALL 0x00456780
void NoopDirectDrawCallback(unsigned int nResult, unsigned int nUnused) {
    (void)nResult;
    (void)nUnused;
}

// FUNCTION: LEMBALL 0x00456790
VsGdiDisplayState::~VsGdiDisplayState(void) {
    m_pVtable = g_VSGDI_MetricsDisplayVtable;
    if (m_hGraphicsModule != 0) {
        FreeLibrary(m_hGraphicsModule);
        m_hGraphicsModule = 0;
    }
    if (m_pReserved08 != 0) {
        DeleteObject((HGDIOBJ)m_pReserved08);
    }
}

// FUNCTION: LEMBALL 0x004567c0
int LEMBALL_FASTCALL CreateAndStoreDisplayPalette(
    void *pDisplayState, int, void *pPalette)

{
    VsGdiDisplayPaletteStateView *pState;

    pState = (VsGdiDisplayPaletteStateView *)pDisplayState;
    if (pState->m_hPalette != 0) {
        DeleteObject(pState->m_hPalette);
    }
    pState->m_hPalette = CreatePalette(pPalette);
    return pState->m_hPalette != 0;
}

// FUNCTION: LEMBALL 0x004567f0
int LEMBALL_FASTCALL SelectAndRealizeDisplayPalette(
    void *pDisplayState, int, void *pDisplayDc)

{
    VsGdiDisplayPaletteStateView *pState;
    VsGdiDisplayDcView *pDc;

    pState = (VsGdiDisplayPaletteStateView *)pDisplayState;
    if (pState->m_hPalette != 0) {
        pDc = (VsGdiDisplayDcView *)pDisplayDc;
        SelectPalette(pDc->m_hDc, pState->m_hPalette, 0);
        RealizePalette(pDc->m_hDc);
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00456970
unsigned int VsGdiDisplayState::DispatchSplitRectAtBackingRowBoundary(
    void *pDestination, short *pDestinationRect,
    void *pSource, short *pSourceRect, void *pHelperTarget)

{
    short *pFirstRect;
    short *pSecondRect;
    short aScaledRect[4];
    int nScale;
    unsigned int nFirstResult;
    unsigned int nSecondResult;
    typedef int (LEMBALL_FASTCALL *StretchRectProc)(
        void *, int, void *, short *, void *, short *);

    pFirstRect = 0;
    pSecondRect = 0;
    *(void **)((char *)this + 0x18) = pHelperTarget;
    nScale = pDestinationRect[0] / pSourceRect[0];
    ((VsGdiHelperSurface *)pHelperTarget)
        ->SplitRectAtBackingRowBoundary(
            pSourceRect, &pFirstRect, &pSecondRect);
    nFirstResult = 0;
    nSecondResult = 0;
    if (pFirstRect != 0) {
        aScaledRect[0] = (short)(pFirstRect[0] * nScale);
        aScaledRect[1] = (short)(pFirstRect[1] * nScale);
        aScaledRect[2] = pDestinationRect[2];
        aScaledRect[3] = pDestinationRect[3];
        nFirstResult = ((StretchRectProc)(*(void ***)this)[7])(
            this, 0, pDestination, aScaledRect, pSource, pFirstRect);
    }
    if (pSecondRect != 0) {
        aScaledRect[0] = (short)(pSecondRect[0] * nScale);
        aScaledRect[1] = (short)(pSecondRect[1] * nScale);
        aScaledRect[2] = pDestinationRect[2];
        aScaledRect[3] = (short)(pFirstRect[1] * nScale +
                                 pDestinationRect[3]);
        nSecondResult = ((StretchRectProc)(*(void ***)this)[7])(
            this, 0, pDestination, aScaledRect, pSource, pSecondRect);
    }
    return nFirstResult | nSecondResult;
}

// FUNCTION: LEMBALL 0x00456a90
VsGdiMetricsDisplayState::VsGdiMetricsDisplayState(void) {
    m_pVtable = g_VSGDI_MetricsDisplayVtable;
    m_fReady = 1;
    m_cxDisplay = (short)GetSystemMetrics(0);
    m_cyDisplay = (short)GetSystemMetrics(1);
}

// FUNCTION: LEMBALL 0x00456ae0
void *VsGdiDisplayState::CreateDisplayBinding(void) {
    VsGdiHelperTargetParamWrapper *pBinding;
    void *hDc;

    hDc = CreateCompatibleDC(0);
    if (hDc != 0) {
        pBinding = (VsGdiHelperTargetParamWrapper *)AllocateVSMemBlock(0xc);
        if (pBinding != 0) {
            pBinding->m_pVtable =
                (void **)&g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable;
            pBinding->m_nWrappedParam = (int)(unsigned long)hDc;
            pBinding->m_nReserved = 0;
            return pBinding;
        }
        return 0;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00456b20
int VsGdiDisplayState::ReleaseDisplayBinding(void *pBinding) {
    int nResult;

    if (pBinding == 0)
        return 0;
    nResult = DeleteDC((void *)(unsigned long)
        ((VsGdiHelperTargetParamWrapper *)pBinding)->m_nWrappedParam);
    DeleteHelperTargetParamWrapper(pBinding, 0, 1);
    return nResult;
}

// FUNCTION: LEMBALL 0x00456b50
int VsGdiDisplayState::InitializeDisplayBitmapInfo(void *pBitmapInfo) {
    unsigned char *pbInfo = (unsigned char *)pBitmapInfo;
    *(unsigned short *)(pbInfo + 0xc) = 1;
    *(unsigned int *)pbInfo = 0x28;
    *(unsigned int *)(pbInfo + 0x10) = 0;
    *(unsigned int *)(pbInfo + 0x14) = 0;
    *(unsigned int *)(pbInfo + 0x18) = 0;
    *(unsigned int *)(pbInfo + 0x1c) = 0;
    *(unsigned int *)(pbInfo + 0x20) = 0;
    *(int *)(pbInfo + 0x08) = -1;
    *(unsigned short *)(pbInfo + 0x0e) = 8;
    *(unsigned int *)(pbInfo + 0x24) = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x00456b90
void *VsGdiDisplayState::CreateDisplayBitmap(int nDriver, void *pBitmapInfo) {
    VsGdiDisplayBitmap *pBitmap;
    void *pBits;
    HGDIOBJ hBitmap;

    hBitmap = (HGDIOBJ)CreateDIBSection(
        (void *)(unsigned long)
            ((VsGdiHelperTargetParamWrapper *)(unsigned long)nDriver)
                ->m_nWrappedParam,
        pBitmapInfo, 0, &pBits, 0, 0);
    if (hBitmap == 0)
        return 0;
    pBitmap = (VsGdiDisplayBitmap *)AllocateVSMemBlock(0x14);
    if (pBitmap != 0) {
        pBitmap->m_pVtable =
            (void **)&g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable + 4;
        pBitmap->m_pBits = 0;
        pBitmap->m_nStride = 0;
        pBitmap->m_pVtable = (void **)&g_VSGDI_DisplayBitmapVtable;
        pBitmap->m_hBitmap = hBitmap;
    } else {
        pBitmap = 0;
    }
    pBitmap->m_nStride = *(int *)((char *)pBitmapInfo + 4);
    pBitmap->m_pBits = pBits;
    ((VsGdiHelperTargetParamWrapper *)(unsigned long)nDriver)->m_nReserved =
        (int)(unsigned long)pBitmap;
    return pBitmap;
}

// FUNCTION: LEMBALL 0x00456c10
int VsGdiDisplayState::ReleaseDisplayBitmap(void *pBitmap) {
    VsGdiDisplayBitmap *pDisplayBitmap;
    int nResult;

    pDisplayBitmap = (VsGdiDisplayBitmap *)pBitmap;
    if (pDisplayBitmap == 0)
        return 1;
    nResult = DeleteObject(pDisplayBitmap->m_hBitmap);
    delete (VsGdiDeleteDispatch *)pDisplayBitmap;
    return nResult;
}

// FUNCTION: LEMBALL 0x00456c50
int LEMBALL_STDCALL ApplyDisplaySurfacePalette(
    void *pSurface, int nFirstEntry, int cEntries,
    const unsigned int *paColors)

{
    return SetDIBColorTable(((VsGdiDisplayDcView *)pSurface)->m_hDc,
                            nFirstEntry,
                            cEntries,
                            paColors);
}

// FUNCTION: LEMBALL 0x00456c70
static int LEMBALL_STDCALL CopyDisplaySurfaceRect(
    void *pDestinationBinding, short *pDestinationRect,
    void *pSourceBinding, short *pSourcePoint)

{
    void *hDestinationDc;
    void *hSourceDc;

    hDestinationDc = (void *)(unsigned long)
        ((VsGdiHelperTargetParamWrapper *)pDestinationBinding)->m_nWrappedParam;
    hSourceDc = (void *)(unsigned long)
        ((VsGdiHelperTargetParamWrapper *)pSourceBinding)->m_nWrappedParam;
    return BitBlt(
        hDestinationDc,
        pDestinationRect[2], pDestinationRect[3],
        pDestinationRect[0], pDestinationRect[1],
        hSourceDc,
        pSourcePoint[0], pSourcePoint[1],
        0x00cc0020);
}

// FUNCTION: LEMBALL 0x00456cc0
static int LEMBALL_STDCALL StretchDisplaySurfaceRect(
    void *pDestinationBinding, short *pDestinationRect,
    void *pSourceBinding, short *pSourceRect)

{
    return StretchBlt(
        (void *)(unsigned long)
            ((VsGdiHelperTargetParamWrapper *)pDestinationBinding)
                ->m_nWrappedParam,
        pDestinationRect[2], pDestinationRect[3],
        pDestinationRect[0], pDestinationRect[1],
        (void *)(unsigned long)
            ((VsGdiHelperTargetParamWrapper *)pSourceBinding)
                ->m_nWrappedParam,
        pSourceRect[2], pSourceRect[3],
        pSourceRect[0], pSourceRect[1],
        0x00cc0020);
}

// FUNCTION: LEMBALL 0x00456d10
void *VsGdiDisplayState::BindDisplayBitmap(int nDriver, void *pBitmap) {
    VsGdiDisplayBitmap *pDisplayBitmap;
    HGDIOBJ hPreviousBitmap;

    pDisplayBitmap = (VsGdiDisplayBitmap *)pBitmap;
    hPreviousBitmap = SelectObject(
        (void *)(unsigned long)
            ((VsGdiHelperTargetParamWrapper *)(unsigned long)nDriver)
                ->m_nWrappedParam,
        pDisplayBitmap->m_hBitmap);
    if (hPreviousBitmap == 0)
        return 0;
    pDisplayBitmap->m_hPreviousBitmap = hPreviousBitmap;
    return pDisplayBitmap;
}

// FUNCTION: LEMBALL 0x00456d40
void *VsGdiDisplayState::DeleteDisplayBitmap(int nDriver, void *pBitmap) {
    VsGdiDisplayBitmap *pDisplayBitmap;
    HGDIOBJ hResult;

    pDisplayBitmap = (VsGdiDisplayBitmap *)pBitmap;
    hResult = SelectObject(
        (void *)(unsigned long)
            ((VsGdiHelperTargetParamWrapper *)(unsigned long)nDriver)
                ->m_nWrappedParam,
        pDisplayBitmap->m_hPreviousBitmap);
    return hResult != 0 ? pDisplayBitmap : 0;
}

// FUNCTION: LEMBALL 0x00456d70
VsGdiDibDisplayState::VsGdiDibDisplayState(const unsigned short *pDisplaySize) {
    FARPROC pfnThunk;

    m_pVtable = g_VSGDI_DibDisplayVtable;
    m_fReady = 0;
    m_cxDisplay = (short)pDisplaySize[0];
    m_cyDisplay = (short)pDisplaySize[1];
    m_pfnDisplayDibStart32 = 0;
    m_pfnDisplayDibEnd32 = 0;
    m_pfnDisplayDibGetAddress32 = 0;
    g_pApplyFullscreenDisplayModeThunk = 0;
    m_hGraphicsModule = LoadLibraryA(g_VSGDI_Dspdib32Dll);
    if (m_hGraphicsModule != 0) {
        m_pfnDisplayDibStart32 = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDibStart32);
        m_pfnDisplayDibEnd32 = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDibEnd32);
        m_pfnDisplayDibGetAddress32 = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDibGetAddress32);
        pfnThunk = GetProcAddress(m_hGraphicsModule, g_VSGDI_DisplayDib32);
        g_pApplyFullscreenDisplayModeThunk = pfnThunk;
        m_pDisplayAddress = ((void *(*)())m_pfnDisplayDibGetAddress32)();
        ((void (*)())m_pfnDisplayDibStart32)();
        m_fReady = 1;
    }
}

// FUNCTION: LEMBALL 0x00456e20
VsGdiDibDisplayState::~VsGdiDibDisplayState(void) {
    typedef void (__stdcall *DISPLAY_DIB_MESSAGE)(int, int, int);
    typedef void (__stdcall *DISPLAY_DIB_END)(void);

    m_pVtable = g_VSGDI_DibDisplayVtable;
    if (m_pfnDisplayDibEnd32 != 0) {
        ((DISPLAY_DIB_MESSAGE)g_pApplyFullscreenDisplayModeThunk)(0, 0, 0x4000);
        ((DISPLAY_DIB_END)m_pfnDisplayDibEnd32)();
    }
}

// FUNCTION: LEMBALL 0x00457020
int VsGdiDibDisplayState::StretchDisplayRect(
    void *pDestinationBinding, short *pDestinationRect,
    void *pSourceBinding, short *pSourceRect)
{
    short aSourcePoint[2];
    short *pAdjustedSourceRect;

    pAdjustedSourceRect = pSourceRect != 0 ? pSourceRect + 2 : 0;
    aSourcePoint[0] = pAdjustedSourceRect[0];
    aSourcePoint[1] = pAdjustedSourceRect[1];
    return ((VsGdiDibRectDispatch *)this)->CopyDisplayRect(
        pDestinationBinding, pDestinationRect,
        pSourceBinding, aSourcePoint);
}

int VsGdiDibDisplayState::CopyDisplayRect(
    void *pDestinationBinding, short *pDestinationRect,
    void *pSourceBinding, short *pSourcePoint) {
    VsGdiRect destinationRect;
    VsGdiDisplayBitmap *pSourceBitmap;
    unsigned char *pDestination;
    const unsigned char *pSource;
    int nSourceStride;
    int iRow;

    if (g_fFullscreenDisplayModeSuspended != 0) {
        return BitBlt(
            (void *)(unsigned long)
                ((VsGdiHelperTargetParamWrapper *)pDestinationBinding)
                    ->m_nWrappedParam,
            pDestinationRect[2], pDestinationRect[3],
            pDestinationRect[0], pDestinationRect[1],
            (void *)(unsigned long)
                ((VsGdiHelperTargetParamWrapper *)pSourceBinding)
                    ->m_nWrappedParam,
            pSourcePoint[0], pSourcePoint[1], 0x00cc0020);
    }

    destinationRect.m_cx = pDestinationRect[0];
    destinationRect.m_cy = pDestinationRect[1];
    destinationRect.m_x = pDestinationRect[2];
    destinationRect.m_y = pDestinationRect[3];
    if (destinationRect.m_x < 0) {
        destinationRect.m_cx =
            (short)(destinationRect.m_cx + destinationRect.m_x);
        destinationRect.m_x = 0;
    }
    if (destinationRect.m_x + destinationRect.m_cx > m_cxDisplay)
        destinationRect.m_cx = (short)(m_cxDisplay - destinationRect.m_x);
    if (destinationRect.m_y < 0) {
        destinationRect.m_cy =
            (short)(destinationRect.m_cy + destinationRect.m_y);
        destinationRect.m_y = 0;
    }
    if (destinationRect.m_y + destinationRect.m_cy > m_cyDisplay)
        destinationRect.m_cy = (short)(m_cyDisplay - destinationRect.m_y);
    if (destinationRect.m_cx <= 0 || destinationRect.m_cy <= 0) {
        destinationRect.m_x = 0;
        destinationRect.m_y = 0;
        destinationRect.m_cx = 0;
        destinationRect.m_cy = 0;
    }
    if (destinationRect.m_cx * destinationRect.m_cy == 0)
        return 1;

    pSourceBitmap = (VsGdiDisplayBitmap *)(unsigned long)
        ((VsGdiHelperTargetParamWrapper *)pSourceBinding)->m_nReserved;
    nSourceStride = ((VsGdiDisplayBitmapDispatch *)pSourceBitmap)->Origin();
    pDestination = (unsigned char *)m_pDisplayAddress +
        destinationRect.m_y * m_cxDisplay + destinationRect.m_x;
    pSource = (const unsigned char *)
        ((VsGdiDisplayBitmapDispatch *)pSourceBitmap)->Bits() +
        pSourcePoint[1] * nSourceStride + pSourcePoint[0];
    for (iRow = 0; iRow < destinationRect.m_cy; ++iRow) {
        memcpy(pDestination, pSource, destinationRect.m_cx);
        pDestination += m_cxDisplay;
        pSource += nSourceStride;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00457410
VsGdiDirectDrawDisplayState::VsGdiDirectDrawDisplayState(const unsigned short *pDisplaySize, int fCreateWindow) {
    typedef unsigned int (__stdcall *DIRECTDRAW_CREATE)(void *, void **, void *);
    typedef unsigned int (__stdcall *DIRECTDRAW_SET_COOPERATIVE_LEVEL)(void *, HWND, unsigned int);
    typedef unsigned int (__stdcall *DIRECTDRAW_SET_DISPLAY_MODE)(void *, int, int, int);
    typedef unsigned int (__stdcall *DIRECTDRAW_CREATE_SURFACE)(void *, void *, void **, void *);
    unsigned char aSurfaceDescription[0x6c];
    unsigned int uCooperativeFlags;
    unsigned int uResult;
    WNDCLASSA WindowClass;
    void **ppDirectDrawObject = &m_pDirectDrawObject;

    (void)fCreateWindow;
    m_pVtable = g_VSGDI_DirectDrawDisplayVtable;
    m_cxDisplay = (short)pDisplaySize[0];
    m_cyDisplay = (short)pDisplaySize[1];
    m_fReady = 0;
    *ppDirectDrawObject = 0;
    m_pPrimarySurface = 0;
    m_pOffscreenSurface = 0;
    m_pPalette = 0;
    m_fDirectDrawCursorVisible = 0;
    m_fWindowOwnershipInitialized = 1;
    m_pDirectDrawPalette = 0;
    m_hGraphicsModule = LoadLibraryA(g_VSGDI_DdrawDll);
    if (m_hGraphicsModule == 0) {
        return;
    }
    m_pfnDirectDrawCreate = GetProcAddress(
        m_hGraphicsModule, g_VSGDI_DirectDrawCreate);
    if (m_pfnDirectDrawCreate == 0) {
        return;
    }

    uResult = ((DIRECTDRAW_CREATE)m_pfnDirectDrawCreate)(0, ppDirectDrawObject, 0);
    if (uResult != 0) {
        NoopDirectDrawCallback(uResult, 0);
        return;
    }

    uCooperativeFlags = 8;
    if (fCreateWindow != 0) {
        WindowClass.style = 0xb;
        WindowClass.lpfnWndProc = WindowOwnerWindowProc;
        WindowClass.cbClsExtra = 0;
        WindowClass.cbWndExtra = 0;
        WindowClass.hInstance = g_hApplicationInstance;
        WindowClass.hIcon = LoadIconA((HINSTANCE)0, (LPCSTR)0x7f00);
        WindowClass.hbrBackground = GetStockObject(4);
        WindowClass.hCursor = LoadCursorA((HINSTANCE)0, (LPCSTR)0x7f00);
        WindowClass.lpszMenuName = 0;
        WindowClass.lpszClassName = g_VSGDI_DirectDrawWindowClass;
        {
            unsigned short nClass = RegisterClassA(&WindowClass);
            ShowCursor(0);
            if (nClass == 0) {
            g_pErrorOutputStream->AppendCStringToStream(
                g_VSGDI_ErrorRegisterClass);
            return;
            }
        }
        m_hDisplayWindow = CreateWindowExA(
            8, g_VSGDI_DirectDrawWindowClass, g_VSGDI_DirectDrawWindowTitle,
            0x80000000, 0, 0, m_cxDisplay, m_cyDisplay,
            (HWND)0, (HMENU)0, g_hApplicationInstance, (LPVOID)0);
        if (m_hDisplayWindow == 0) {
            return;
        }
        ShowWindow(m_hDisplayWindow, 5);
        UpdateWindow(m_hDisplayWindow);
        SetForegroundWindow(m_hDisplayWindow);
        uCooperativeFlags = 0x11;
    }

    {
        void **pDirectDrawVtable = *(void ***)*ppDirectDrawObject;
        uResult = ((DIRECTDRAW_SET_COOPERATIVE_LEVEL)pDirectDrawVtable[0x50 / 4])(
            *ppDirectDrawObject, m_hDisplayWindow, uCooperativeFlags);
        if (uResult != 0) {
            g_pErrorOutputStream
                ->AppendCStringToStream(g_VSGDI_ErrorSetCooperativeLevel)
                ->AppendCStringToStream(
                    LookupDirectDrawErrorString(uResult & 0xfff))
                ->AppendCStringToStream(g_VSGDI_ErrorLineEnd);
            return;
        }
        if ((uCooperativeFlags & 1) != 0) {
            uResult = ((DIRECTDRAW_SET_DISPLAY_MODE)pDirectDrawVtable[0x54 / 4])(
                *ppDirectDrawObject, m_cxDisplay, m_cyDisplay, 8);
            if (uResult != 0) {
                g_pErrorOutputStream
                    ->AppendCStringToStream(g_VSGDI_ErrorSetDisplayMode)
                    ->AppendCStringToStream(
                        LookupDirectDrawErrorString(uResult & 0xfff))
                    ->AppendCStringToStream(g_VSGDI_ErrorLineEnd);
                return;
            }
        }
        *(unsigned int *)aSurfaceDescription = 0x6c;
        *(unsigned int *)(aSurfaceDescription + 4) = 0;
        *(unsigned int *)(aSurfaceDescription + 0x68) = 0x200;
        uResult = ((DIRECTDRAW_CREATE_SURFACE)pDirectDrawVtable[0x18 / 4])(
            *ppDirectDrawObject, aSurfaceDescription, &m_pPrimarySurface, 0);
        if (uResult == 0) {
            m_fReady = 1;
        } else {
            g_pErrorOutputStream
                ->AppendCStringToStream(g_VSGDI_ErrorCreatePrimarySurface)
                ->AppendCStringToStream(
                    LookupDirectDrawErrorString(uResult & 0xfff))
                ->AppendCStringToStream(g_VSGDI_ErrorLineEnd);
        }
    }
}

// FUNCTION: LEMBALL 0x004576e0
VsGdiDirectDrawDisplayState::~VsGdiDirectDrawDisplayState(void) {
    typedef unsigned int (__stdcall *DIRECTDRAW_RELEASE)(void *);

    m_pVtable = g_VSGDI_DirectDrawDisplayVtable;
    if (m_pDirectDrawObject != 0) {
        if (m_pPrimarySurface != 0) {
            ((DIRECTDRAW_RELEASE)(*(void ***)m_pPrimarySurface)[2])(
                m_pPrimarySurface);
            m_pPrimarySurface = 0;
        }
        if (m_pPalette != 0) {
            m_pPalette = 0;
        }
        if (m_fDirectDrawCursorVisible != 0) {
            m_fDirectDrawCursorVisible = 0;
        }
        if (m_pDirectDrawPalette != 0) {
            ((DIRECTDRAW_RELEASE)(*(void ***)m_pDirectDrawPalette)[2])(
                m_pDirectDrawPalette);
            m_pDirectDrawPalette = 0;
        }
        ((DIRECTDRAW_RELEASE)(*(void ***)m_pDirectDrawObject)[2])(
            m_pDirectDrawObject);
        m_pDirectDrawObject = 0;
        ShowCursor(1);
    }
    if (m_hDisplayWindow != 0) {
        m_hDisplayWindow = 0;
    }
}

// FUNCTION: LEMBALL 0x00457760
void *VsGdiDirectDrawDisplayState::CreateDirectDrawDisplayBinding(void) {
    VsGdiDirectDrawBinding *pBinding;
    int nSurfaceSlot;
    void **ppSurfaceSlots;

    nSurfaceSlot = m_fWindowOwnershipInitialized;
    ppSurfaceSlots = (void **)&m_pfnDirectDrawCreate;
    ppSurfaceSlots[nSurfaceSlot] = 0;
    ++m_fWindowOwnershipInitialized;
    pBinding = (VsGdiDirectDrawBinding *)AllocateVSMemBlock(
        sizeof(VsGdiDirectDrawBinding));
    if (pBinding != 0) {
        pBinding->m_pVtable = g_VSGDI_DirectDrawBindingVtable;
        pBinding->m_nSurfaceSlot = nSurfaceSlot;
        return pBinding;
    }
    return 0;
}

int VsGdiDirectDrawDisplayState::ReleaseDirectDrawDisplayBinding(
    void *pBinding) {
    if (pBinding != 0) {
        ((void *(LEMBALL_FASTCALL *)(void *, int, int))
             (*(void ***)pBinding)[0])(pBinding, 0, 1);
    }
    return 1;
}

int VsGdiDirectDrawDisplayState::InitializeDirectDrawBitmapInfo(
    void *pBitmapInfo) {
    unsigned char *pbInfo;

    pbInfo = (unsigned char *)pBitmapInfo;
    *(unsigned short *)(pbInfo + 0x0c) = 1;
    *(unsigned int *)(pbInfo + 0x00) = 0x28;
    *(unsigned int *)(pbInfo + 0x10) = 0;
    *(unsigned int *)(pbInfo + 0x14) = 0;
    *(unsigned int *)(pbInfo + 0x18) = 0;
    *(unsigned int *)(pbInfo + 0x1c) = 0;
    *(unsigned int *)(pbInfo + 0x20) = 0;
    *(int *)(pbInfo + 0x08) = -1;
    *(unsigned short *)(pbInfo + 0x0e) = 8;
    *(unsigned int *)(pbInfo + 0x24) = 0;
    return 1;
}

void *VsGdiDirectDrawDisplayState::CreateDirectDrawDisplayBitmap(
    int nDriver, void *pBitmapInfo) {
    typedef unsigned int (__stdcall *DIRECTDRAW_CREATE_SURFACE)(
        void *, void *, void **, void *);
    typedef unsigned int (__stdcall *DIRECTDRAW_GET_SURFACE_DESC)(
        void *, void *);
    unsigned char aSurfaceDescription[0x6c];
    VsGdiDirectDrawSurfaceBitmap *pBitmap;
    void *pSurface;
    int nHeight;
    unsigned int uResult;

    (void)nDriver;
    *(unsigned int *)(aSurfaceDescription + 0x00) = 0x6c;
    *(unsigned int *)(aSurfaceDescription + 0x04) = 6;
    nHeight = *(int *)((char *)pBitmapInfo + 8);
    if (nHeight < 0) {
        nHeight = -nHeight;
    }
    *(int *)(aSurfaceDescription + 0x08) = nHeight;
    *(int *)(aSurfaceDescription + 0x0c) =
        *(int *)((char *)pBitmapInfo + 4);
    *(unsigned int *)(aSurfaceDescription + 0x68) = 0x40;
    pSurface = 0;
    uResult = ((DIRECTDRAW_CREATE_SURFACE)
        (*(void ***)m_pDirectDrawObject)[6])(
            m_pDirectDrawObject, aSurfaceDescription, &pSurface, 0);
    if (uResult != 0) {
        return 0;
    }

    pBitmap = (VsGdiDirectDrawSurfaceBitmap *)AllocateVSMemBlock(
        sizeof(VsGdiDirectDrawSurfaceBitmap));
    if (pBitmap == 0) {
        return 0;
    }
    pBitmap->m_pVtable = g_VSGDI_DirectDrawSurfaceBitmapVtable;
    pBitmap->m_pCachedPixels = 0;
    pBitmap->m_nCachedPitch = 0;
    pBitmap->m_pSurface = pSurface;
    *(unsigned int *)(pBitmap->m_abSurfaceDesc + 0x00) = 0x6c;
    uResult = ((DIRECTDRAW_GET_SURFACE_DESC)(*(void ***)pSurface)[22])(
        pSurface, pBitmap->m_abSurfaceDesc);
    if (uResult != 0) {
        g_pErrorOutputStream
            ->AppendCStringToStream(
                "Direct Draw Surface Get Description Failed : ")
            ->AppendCStringToStream(LookupDirectDrawErrorString(uResult & 0xfff))
            ->AppendCStringToStream("\n");
    }
    return pBitmap;
}

int VsGdiDirectDrawDisplayState::ReleaseDirectDrawDisplayBitmap(
    void *pBitmap) {
    if (pBitmap != 0) {
        ((void *(LEMBALL_FASTCALL *)(void *, int, int))
             (*(void ***)pBitmap)[0])(pBitmap, 0, 1);
    }
    return 1;
}

// FUNCTION: LEMBALL 0x004578c0
static int LEMBALL_FASTCALL ApplyDirectDrawSurfacePalette(void *, int, void *, int, int, const unsigned int *) {
    return 0x100;
}

// FUNCTION: LEMBALL 0x004578d0
int VsGdiDirectDrawDisplayState::CopyDirectDrawSurfaceRect(void *, short *pDestinationRect, void *pSourceBinding, short *pSourcePoint) {
    typedef unsigned int (__stdcall *DIRECTDRAW_BLT_FAST)(
        void *, unsigned int, unsigned int, void *, void *, unsigned int);
    struct DirectDrawRect {
        int m_nLeft;
        int m_nTop;
        int m_nRight;
        int m_nBottom;
    } SourceRect;
    VsGdiHelperTargetParamWrapper *pBinding;
    VsGdiDisplayBitmap *pBitmap;
    void **ppSurfaceSlots;
    void *pSourceSurface;
    short cx;
    short cy;
    short x;
    short y;
    short *pDestinationPoint;
    unsigned int uResult;

    cx = pDestinationRect[0];
    cy = pDestinationRect[1];
    pDestinationPoint = pDestinationRect != 0
        ? pDestinationRect + 2 : 0;
    x = pDestinationPoint[0];
    y = pDestinationPoint[1];
    if (x < 0) {
        cx = (short)(cx + x);
        x = 0;
    }
    if ((short)(x + cx) > m_cxDisplay)
        cx = (short)(m_cxDisplay - x);
    if (y < 0) {
        cy = (short)(cy + y);
        y = 0;
    }
    if ((short)(y + cy) > m_cyDisplay)
        cy = (short)(m_cyDisplay - y);
    if (cx <= 0 || cy <= 0) {
        cx = 0;
        cy = 0;
        x = 0;
        y = 0;
    }
    if ((int)cx * (int)cy == 0)
        return 1;

    SourceRect.m_nLeft = pSourcePoint[0];
    SourceRect.m_nTop = pSourcePoint[1];
    SourceRect.m_nRight = SourceRect.m_nLeft + cx;
    SourceRect.m_nBottom = SourceRect.m_nTop + cy;
    pBinding = (VsGdiHelperTargetParamWrapper *)pSourceBinding;
    ppSurfaceSlots = (void **)&m_pfnDirectDrawCreate;
    pBitmap = (VsGdiDisplayBitmap *)
        ppSurfaceSlots[pBinding->m_nWrappedParam];
    pSourceSurface = pBitmap->m_hPreviousBitmap;
    uResult = ((DIRECTDRAW_BLT_FAST)(*(void ***)m_pPrimarySurface)[7])(
        m_pPrimarySurface, x, y, pSourceSurface, &SourceRect, 0);
    if (uResult != 0) {
        g_pErrorOutputStream
            ->AppendCStringToStream("Blit failed: ")
            ->AppendSignedIntToStreamVariant(SourceRect.m_nLeft)
            ->AppendCStringToStream(", ")
            ->AppendSignedIntToStreamVariant(SourceRect.m_nTop)
            ->AppendCStringToStream(", ")
            ->AppendSignedIntToStreamVariant(x)
            ->AppendCStringToStream(", ")
            ->AppendSignedIntToStreamVariant(y)
            ->AppendCStringToStream(" - ")
            ->AppendCStringToStream(LookupDirectDrawErrorString(uResult & 0xfff))
            ->AppendCStringToStream("\n");
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00457a60
int VsGdiDirectDrawDisplayState::StretchDirectDrawSurfaceRect(void *, short *pDestinationRect, void *pSourceBinding, short *pSourceRect) {
    typedef unsigned int (__stdcall *DIRECTDRAW_BLT)(
        void *, void *, void *, void *, unsigned int, void *);
    struct DirectDrawRect {
        int m_nLeft;
        int m_nTop;
        int m_nRight;
        int m_nBottom;
    } DestinationRect, SourceRect;
    VsGdiHelperTargetParamWrapper *pBinding;
    VsGdiDisplayBitmap *pBitmap;
    void **ppSurfaceSlots;
    void *pSourceSurface;
    short cx;
    short cy;
    short x;
    short y;
    short *pDestinationPoint;
    unsigned int uResult;

    cx = pDestinationRect[0];
    cy = pDestinationRect[1];
    pDestinationPoint = pDestinationRect != 0
        ? pDestinationRect + 2 : 0;
    x = pDestinationPoint[0];
    y = pDestinationPoint[1];
    if (x < 0) {
        cx = (short)(cx + x);
        x = 0;
    }
    if ((short)(x + cx) > m_cxDisplay)
        cx = (short)(m_cxDisplay - x);
    if (y < 0) {
        cy = (short)(cy + y);
        y = 0;
    }
    if ((short)(y + cy) > m_cyDisplay)
        cy = (short)(m_cyDisplay - y);
    if (cx <= 0 || cy <= 0) {
        cx = 0;
        cy = 0;
        x = 0;
        y = 0;
    }
    if ((int)cx * (int)cy == 0)
        return 1;
    if ((int)pSourceRect[0] * (int)pSourceRect[1] == 0)
        return 1;

    DestinationRect.m_nLeft = x;
    DestinationRect.m_nTop = y;
    DestinationRect.m_nRight = x + cx;
    DestinationRect.m_nBottom = y + cy;
    SourceRect.m_nLeft = pSourceRect[2];
    SourceRect.m_nTop = pSourceRect[3];
    SourceRect.m_nRight = SourceRect.m_nLeft + pSourceRect[0];
    SourceRect.m_nBottom = SourceRect.m_nTop + pSourceRect[1];
    pBinding = (VsGdiHelperTargetParamWrapper *)pSourceBinding;
    ppSurfaceSlots = (void **)&m_pfnDirectDrawCreate;
    pBitmap = (VsGdiDisplayBitmap *)
        ppSurfaceSlots[pBinding->m_nWrappedParam];
    pSourceSurface = pBitmap->m_hPreviousBitmap;
    uResult = ((DIRECTDRAW_BLT)(*(void ***)m_pPrimarySurface)[5])(
        m_pPrimarySurface, &DestinationRect, pSourceSurface,
        &SourceRect, 0, 0);
    if (uResult != 0) {
        g_pErrorOutputStream
            ->AppendCStringToStream("Blit failed: ")
            ->AppendSignedIntToStreamVariant(DestinationRect.m_nLeft)
            ->AppendCStringToStream(", ")
            ->AppendSignedIntToStreamVariant(DestinationRect.m_nTop)
            ->AppendCStringToStream(", ")
            ->AppendSignedIntToStreamVariant(DestinationRect.m_nRight)
            ->AppendCStringToStream(", ")
            ->AppendSignedIntToStreamVariant(DestinationRect.m_nBottom)
            ->AppendCStringToStream(" - ")
            ->AppendCStringToStream(LookupDirectDrawErrorString(uResult & 0xfff))
            ->AppendCStringToStream("\n");
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00457c50
int VsGdiDirectDrawDisplayState::BindDirectDrawSurface(int nDriver, void *pSurface) {
    VsGdiHelperTargetParamWrapper *pBinding;
    void **ppSurfaceSlots;

    pBinding = (VsGdiHelperTargetParamWrapper *)(unsigned long)nDriver;
    ppSurfaceSlots = (void **)&m_pfnDirectDrawCreate;
    ppSurfaceSlots[pBinding->m_nWrappedParam] = pSurface;
    return (int)(unsigned long)pSurface;
}

// FUNCTION: LEMBALL 0x00457c70
int VsGdiDirectDrawDisplayState::ReleaseDirectDrawSurface(int, void *pSurface) {
    return (int)(unsigned long)pSurface;
}

// FUNCTION: LEMBALL 0x00457c80
int VsGdiDirectDrawDisplayState::ApplyDirectDrawPalette(void *pPalette) {
    typedef unsigned int (__stdcall *DIRECTDRAW_BLT)(
        void *, void *, void *, void *, unsigned int, void *);
    typedef unsigned int (__stdcall *DIRECTDRAW_CREATE_PALETTE)(
        void *, unsigned int, void *, void **, void *);
    typedef unsigned int (__stdcall *DIRECTDRAW_SET_PALETTE_ENTRIES)(
        void *, unsigned int, unsigned int, unsigned int, void *);
    typedef unsigned int (__stdcall *DIRECTDRAW_SET_PALETTE)(void *, void *);
    struct DirectDrawPaletteData {
        unsigned short m_nReserved00;
        unsigned short m_cEntries02;
        unsigned int m_aEntries04[1];
    };
    unsigned char aBltEffects[100];
    DirectDrawPaletteData *pPaletteData;
    unsigned int uResult;

    *(unsigned int *)aBltEffects = sizeof(aBltEffects);
    *(unsigned int *)(aBltEffects + 0x50) = 0;
    uResult = ((DIRECTDRAW_BLT)(*(void ***)m_pPrimarySurface)[5])(
        m_pPrimarySurface, 0, 0, 0, 0x400, aBltEffects);
    if (uResult != 0) {
        g_pErrorOutputStream
            ->AppendCStringToStream(
                "Direct Draw Initial rectangle blit failed : ")
            ->AppendCStringToStream(
                LookupDirectDrawErrorString(uResult & 0xfff))
            ->AppendCStringToStream("\n");
        return 0;
    }

    pPaletteData = (DirectDrawPaletteData *)pPalette;
    if (m_pDirectDrawPalette != 0) {
        uResult = ((DIRECTDRAW_SET_PALETTE_ENTRIES)
            (*(void ***)m_pDirectDrawPalette)[6])(
                m_pDirectDrawPalette, 0, 0, pPaletteData->m_cEntries02,
                pPaletteData->m_aEntries04);
        if (uResult != 0) {
            g_pErrorOutputStream
                ->AppendCStringToStream(
                    "Direct Draw Set Palette Entries failed: ")
                ->AppendCStringToStream(
                    LookupDirectDrawErrorString(uResult & 0xfff))
                ->AppendCStringToStream("\n");
            return 0;
        }
    } else {
        uResult = ((DIRECTDRAW_CREATE_PALETTE)
            (*(void ***)m_pDirectDrawObject)[5])(
                m_pDirectDrawObject, 0xc, pPaletteData->m_aEntries04,
                &m_pDirectDrawPalette, 0);
        if (uResult != 0) {
            g_pErrorOutputStream
                ->AppendCStringToStream("Direct Draw Create Palette failed: ")
                ->AppendCStringToStream(
                    LookupDirectDrawErrorString(uResult & 0xfff))
                ->AppendCStringToStream("\n");
            return 0;
        }
        uResult = ((DIRECTDRAW_SET_PALETTE)
            (*(void ***)m_pPrimarySurface)[31])(
                m_pPrimarySurface, m_pDirectDrawPalette);
        if (uResult != 0) {
            g_pErrorOutputStream
                ->AppendCStringToStream("Direct Draw Set Palette failed: ")
                ->AppendCStringToStream(
                    LookupDirectDrawErrorString(uResult & 0xfff))
                ->AppendCStringToStream("\n");
            return 0;
        }
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00457e10
int VsGdiSelectedGraphicsDriverRuntime::InitializeSelectedGraphicsDriver(int nRequestedDriver) {
    int nSelectedDriver;
    VsGdiSelectedGraphicsDriverRuntime *pRuntime;

    pRuntime = this;

    if (nRequestedDriver < 9) {
        g_pStatusOutputStream
            ->AppendCStringToStream("Initialising graphics device driver: ")
            ->AppendCStringToStream(
                g_apszGraphicsDriverNames[nRequestedDriver])
            ->AppendCStringToStream("\n");
    }

    nSelectedDriver = nRequestedDriver;
    if (nRequestedDriver == VsGdiDRIVER_AUTO) {
        if (g_fStartupGraphicsDriverGdk != 0) {
            nSelectedDriver = (-(g_fStartupFullscreen == 0) & 2) + 4;
        } else {
            nSelectedDriver = (-(g_fStartupFullscreen == 0) & -2) + 3;
        }
    }

    {
        unsigned short aDisplaySize[2];

    switch (nSelectedDriver) {
    case VsGdiDRIVER_METRICS:
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x1c);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                g_pDisplayState = new (pvDisplayState) VsGdiMetricsDisplayState;
            }
        }
        break;
    case VsGdiDRIVER_DIB_320_200:
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x30);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                aDisplaySize[0] = 0x140;
                aDisplaySize[1] = 200;
                g_pDisplayState = new (pvDisplayState) VsGdiDibDisplayState(aDisplaySize);
            }
        }
        break;
    case VsGdiDRIVER_DIB_640_480:
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x30);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                aDisplaySize[0] = 0x140;
                aDisplaySize[1] = 0xf0;
                g_pDisplayState = new (pvDisplayState) VsGdiDibDisplayState(aDisplaySize);
            }
        }
        break;
    case VsGdiDRIVER_DDRAW_320_200:
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x4a8);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                aDisplaySize[0] = 0x280;
                aDisplaySize[1] = 0x1e0;
                g_pDisplayState = new (pvDisplayState) VsGdiDirectDrawDisplayState(aDisplaySize, 1);
            }
        }
        break;
    case VsGdiDRIVER_DDRAW_640_480:
        nSelectedDriver = VsGdiDRIVER_DDRAW_320_200;
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x4a8);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                aDisplaySize[0] = 0x280;
                aDisplaySize[1] = 0x1e0;
                g_pDisplayState = new (pvDisplayState) VsGdiDirectDrawDisplayState(aDisplaySize, 1);
            }
        }
        break;
    default:
        g_pErrorOutputStream->AppendCStringToStream(
            "No valid driver selected to initialise\n");
        return 0;
    }
    }

    if (g_pDisplayState == 0 || !IsDisplayStateReady(g_pDisplayState)) {
        if (g_pDisplayState != 0) {
            CallDeleteVirtual(g_pDisplayState, 0, 1);
        }
        {
            void *pvDisplayState;

            pvDisplayState = AllocateVSMemBlock(0x1c);
            if (pvDisplayState == 0) {
                g_pDisplayState = 0;
            } else {
                g_pDisplayState = new (pvDisplayState) VsGdiMetricsDisplayState;
            }
        }
        if (g_pDisplayState == 0 || !IsDisplayStateReady(g_pDisplayState)) {
            g_pErrorOutputStream->AppendCStringToStream(
                "No valid driver available\n");
            return 0;
        }

        if (pRuntime->m_fFallbackWarningShown == 0) {
            GameDynamicCString WarningMessage;
            GameDynamicCString WarningResult;

            WarningMessage.ConstructDynamicCStringFromCString(
                g_apszGraphicsDriverFallbackMessages[nSelectedDriver]);
            WarningMessage.AppendDynamicCStringAndCopyResult(
                &WarningResult,
                ". Defaulting to normal window mode (using CreateDIBSection)");
            WarningResult.DestroyDynamicCString();
            MessageBoxA((HWND)0, WarningMessage.m_pszText, "WARNING", 0x12000);
            pRuntime->m_fFallbackWarningShown = 1;
            WarningMessage.DestroyDynamicCString();
        }
        nSelectedDriver = VsGdiDRIVER_METRICS;
    }

    if (nRequestedDriver != nSelectedDriver) {
        g_pStatusOutputStream
            ->AppendCStringToStream("[ Auto selected: ")
            ->AppendCStringToStream(
                g_apszGraphicsDriverNames[nSelectedDriver])
            ->AppendCStringToStream(" ]\n");
    }

    if (pRuntime != 0) {
        pRuntime->m_nSelectedDriver = nSelectedDriver;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x004581d0
void VsGdiSelectedGraphicsDriverRuntime::ResizeActiveDisplayState(unsigned short *pSize) {
    VsGdiDisplayState *pDisplayState;

    if (g_pDisplayState != 0) {
        pDisplayState = g_pDisplayState;
        *(unsigned short *)((char *)pDisplayState + 0x14) = pSize[0];
        *(unsigned short *)((char *)pDisplayState + 0x16) = pSize[1];
        NotifyWindowOwnersDisplayChanged(&m_nSelectedDriver);
    }
}

// FUNCTION: LEMBALL 0x00458200
int LEMBALL_FASTCALL UsesExistingDisplayPaletteAsFallback(
    void *pDisplayState, int)

{
    return ((VsGdiDisplayPaletteStateView *)pDisplayState)->m_hPalette != 0;
}

// FUNCTION: LEMBALL 0x00458230
void *VsGdiMetricsDisplayState::Delete(int fDelete) {
    void *pThis;

    pThis = this;
    this->VsGdiDisplayState::~VsGdiDisplayState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pThis);
    }
    return pThis;
}

// FUNCTION: LEMBALL 0x00458250
void LEMBALL_FASTCALL SetHelperTargetParamWrapperDisplayDc(
    void *pParamWrapper, int, void *hDc)

{
    *(void **)((char *)pParamWrapper + 4) = hDc;
}

// FUNCTION: LEMBALL 0x00458260
void *LEMBALL_FASTCALL DeleteHelperTargetParamWrapper(
    void *pParamWrapper, int, int fDelete)

{
    *(void ***)pParamWrapper =
        (void **)&g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable + 2;
    if ((fDelete & 1) != 0)
        FreeVSMemBlock(pParamWrapper);
    return pParamWrapper;
}

// FUNCTION: LEMBALL 0x00458280
void LEMBALL_FASTCALL NoopHelperTargetParamWrapperArgument(
    void *, int, void *)

{
}

// FUNCTION: LEMBALL 0x00458290
void *LEMBALL_FASTCALL DeleteHelperTargetParamWrapperBase(
    void *pParamWrapper, int, int fDelete)

{
    *(void ***)pParamWrapper =
        (void **)&g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable + 2;
    if ((fDelete & 1) != 0)
        FreeVSMemBlock(pParamWrapper);
    return pParamWrapper;
}

// FUNCTION: LEMBALL 0x004582b0
int LEMBALL_FASTCALL ReturnTrueHelperTargetParamWrapperA(void *, int) {
    return 1;
}

// FUNCTION: LEMBALL 0x004582c0
int LEMBALL_FASTCALL ReturnTrueHelperTargetParamWrapperB(void *, int) {
    return 1;
}

// FUNCTION: LEMBALL 0x004582d0
int LEMBALL_FASTCALL GetHelperTargetParamWrapperDisplayDc(
    void *pParamWrapper, int)

{
    return *(int *)((char *)pParamWrapper + 4);
}

// FUNCTION: LEMBALL 0x004582e0
int LEMBALL_FASTCALL GetHelperTargetParamWrapperReserved(
    void *pParamWrapper, int)

{
    return *(int *)((char *)pParamWrapper + 8);
}

// FUNCTION: LEMBALL 0x004582f0
void *LEMBALL_FASTCALL DeleteHelperTargetParamWrapperInterface(
    void *pParamWrapper, int, int fDelete)

{
    *(void ***)pParamWrapper =
        (void **)&g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable + 4;
    if ((fDelete & 1) != 0)
        FreeVSMemBlock(pParamWrapper);
    return pParamWrapper;
}

// FUNCTION: LEMBALL 0x00458310
void *LEMBALL_FASTCALL VsGdiDeleteDisplayBitmap(
    void *pBitmap, int, int fDelete)

{
    *(void ***)pBitmap =
        (void **)&g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable + 4;
    if ((fDelete & 1) != 0)
        FreeVSMemBlock(pBitmap);
    return pBitmap;
}

// FUNCTION: LEMBALL 0x00458330
void *VsGdiDibDisplayState::Delete(int fDelete) {
    void *pThis;

    pThis = this;
    this->~VsGdiDibDisplayState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pThis);
    }
    return pThis;
}

// FUNCTION: LEMBALL 0x00458360
void *VsGdiDirectDrawDisplayState::Delete(int fDelete) {
    void *pThis;

    pThis = this;
    this->~VsGdiDirectDrawDisplayState();
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pThis);
    }
    return pThis;
}

// FUNCTION: LEMBALL 0x00463c30
void LEMBALL_FASTCALL BuildGeometryHelperFromRenderRect(void *pOwner) {
    unsigned char *pbOwner;
    void *pParentOwner;
    void *pWrappedTarget;
    short aRect[4];
    short aWindowRect[4];
    short sOriginX;
    short sOriginY;
    unsigned int uOwnerFlags;
    unsigned int uWindowStyle;
    VsGdiResourceGeometryHelperTargetView *pTarget;
    VsGdiGeometryHelperPointerArray *pHelper;
    typedef unsigned int (LEMBALL_FASTCALL *GetOwnerFlagsProc)(void *);
    typedef void (LEMBALL_FASTCALL *SetWindowRectProc)(void *, int, short *);

    pbOwner = (unsigned char *)pOwner;
    ((VS_RootGeometryOwner *)pOwner)
        ->VS_RootGeometryOwner::RetainRootGeometryOwner();

    pParentOwner = *(void **)(pbOwner + 0x20);
    if (pParentOwner == 0) {
        pWrappedTarget = g_pResourceGeometryHelperTarget;
    } else {
        pWrappedTarget = *(void **)((char *)*(void **)(pbOwner + 0x20) + 0x4c);
        pWrappedTarget = *(void **)((char *)pWrappedTarget + 0x0c);
    }

    aRect[0] = *(short *)(pbOwner + 0x08);
    aRect[1] = *(short *)(pbOwner + 0x0a);
    aRect[2] = *(short *)(pbOwner + 0x18);
    aRect[3] = *(short *)(pbOwner + 0x1a);

    if ((short)*(short *)(pbOwner + 0x10) *
            (short)*(short *)(pbOwner + 0x12) != 0) {
        aRect[0] = *(short *)(pbOwner + 0x10);
        aRect[1] = *(short *)(pbOwner + 0x12);
        aRect[2] = *(short *)(pbOwner + 0x14) + *(short *)(pbOwner + 0x18);
        aRect[3] = *(short *)(pbOwner + 0x16) + *(short *)(pbOwner + 0x1a);
    }
    if (pParentOwner == 0) {
        aRect[0] = (short)((int)aRect[0] / *(int *)(pbOwner + 0x38));
        aRect[1] = (short)((int)aRect[1] / *(int *)(pbOwner + 0x38));
    }

    pHelper = (VsGdiGeometryHelperPointerArray *)AllocateVSMemBlock(sizeof(*pHelper));
    if (pHelper == 0) {
        *(void **)(pbOwner + 0x4c) = 0;
        return;
    }
    pHelper = pHelper->InitializeGeometryHelperPointerArray(
        aRect, *(int *)(pbOwner + 0x50), pWrappedTarget);
    *(VsGdiGeometryHelperPointerArray **)(pbOwner + 0x4c) = pHelper;

    pTarget = pHelper->m_pTarget;
    if (pParentOwner == 0) {
        *(short *)((char *)pTarget +
                   *(int *)(*(int *)((char *)pTarget + 0x40) + 4) + 0x6c) =
            (short)*(int *)(pbOwner + 0x38);
    }

    sOriginX = *(short *)(pbOwner + 0x14);
    sOriginY = *(short *)(pbOwner + 0x16);
    if (pParentOwner != 0) {
        sOriginX = (short)(sOriginX - *(short *)((char *)pParentOwner + 0x18));
        sOriginY = (short)(sOriginY - *(short *)((char *)pParentOwner + 0x1a));
    }
    *(short *)((char *)pTarget +
               *(int *)(*(int *)((char *)pTarget + 0x40) + 4) + 0x5c) = sOriginX;
    *(short *)((char *)pTarget +
               *(int *)(*(int *)((char *)pTarget + 0x40) + 4) + 0x5e) = sOriginY;

    uOwnerFlags = ((GetOwnerFlagsProc)(*(void ***)pOwner)[0x64 / sizeof(void *)])(
        pOwner);
    if ((uOwnerFlags & 0x40000000u) != 0 && *(HWND *)(pbOwner + 0x44) != 0) {
        uWindowStyle = GetWindowLongA(*(HWND *)(pbOwner + 0x44), -16);
        if ((uWindowStyle & 0x40000000u) != 0) {
            aWindowRect[0] = *(short *)(pbOwner + 0x14) -
                             *(short *)((char *)*(void **)(pbOwner + 0x48) + 0x18);
            aWindowRect[1] = *(short *)(pbOwner + 0x16) -
                             *(short *)((char *)*(void **)(pbOwner + 0x48) + 0x1a);
            aWindowRect[2] = aRect[2];
            aWindowRect[3] = aRect[3];
            *(short *)((char *)pTarget + 0xec) = aWindowRect[0];
            *(short *)((char *)pTarget + 0xee) = aWindowRect[1];
            ((SetWindowRectProc)(*(void ***)*(void **)(pbOwner + 0x48))[0x84 / sizeof(void *)])(
                *(void **)(pbOwner + 0x48), 0, aWindowRect);
        }
    }
    *(int *)((char *)pTarget + 0x10) =
        *(int *)g_pSelectedGraphicsDriverRuntime != 3;
}

// FUNCTION: LEMBALL 0x00465aa0
void GamePrimaryContext::SampleRootHelperGeometryAndDispatchRenderGroups(int nToken) {
    int *pContext;
    VsGdiHelperDispatchQueue *pDispatchQueue;
    VsGdiHelperSurfaceBindingSurface *pBindingSurface;
    VsGdiHelperSurface *pHelperTarget;
    void *pQueuedPointSink;
    int nUploadState;
    int nResult;
    short x1;
    short y1;
    short y2;
    typedef unsigned char *(LEMBALL_FASTCALL *TokenVirtual)(void *, int);
    typedef int (LEMBALL_FASTCALL *IntVirtual)(void *, int);
    typedef void (LEMBALL_FASTCALL *VoidVirtual)(void *, int);
    typedef void (LEMBALL_FASTCALL *OneArgVirtual)(void *, int, void *);
    typedef void (LEMBALL_FASTCALL *DeleteVirtual)(void *, int, int);

    pContext = (int *)this;
    if (g_fRootHelperGeometryDispatchSuppressed != 0 || pContext[1] != 1 || pContext[0x13] == 0) {
        return;
    }

    if (nToken == -1) {
        nToken = 0;
    }

    pDispatchQueue = (VsGdiHelperDispatchQueue *)(unsigned long)pContext[0x13];
    pHelperTarget = pDispatchQueue->m_pHelperTarget;
    pBindingSurface =
        (VsGdiHelperSurfaceBindingSurface *)((char *)pHelperTarget + GetHelperSurfaceVariableBlockOffset(pHelperTarget) + 0x40);
    ((TokenVirtual)pBindingSurface->m_pVtable[0xe])(
        pBindingSurface, 0);

    x1 = pBindingSurface->m_nOriginX;
    y1 = pBindingSurface->m_nOriginY;

    pQueuedPointSink = AllocateVSMemBlock(4);
    if (pQueuedPointSink != 0) {
        *(void **)pQueuedPointSink = g_pQueuedRenderPointSinkFinalizeThunk;
    }

    nUploadState = ((IntVirtual)(*(void ***)pHelperTarget)[2])(
        pHelperTarget, 0);
    InitializeHelperUploadStatePending(nUploadState);
    ((OneArgVirtual)(*(void ***)pQueuedPointSink)[1])(
        pQueuedPointSink, 0, pDispatchQueue);

    *(short *)((char *)pContext + 0x5c) = *(short *)&nUploadState;
    *(short *)((char *)pContext + 0x5e) = *((short *)&nUploadState + 1);
    *(short *)((char *)pContext + 0x60) = y1;
    *(short *)((char *)pContext + 0x62) = x1;
    pContext[0x19] = nToken;
    ((OneArgVirtual)(*(void ***)((char *)pContext + 0x58))[1])(
        (char *)pContext + 0x58, 0, pDispatchQueue);

    nResult = ((IntVirtual)pBindingSurface->m_pVtable[0xf])(
        pBindingSurface, 0);
    y2 = *((short *)&nResult + 1);
    if (nResult != 0) {
        *(short *)((char *)pContext + 0x6c) = 0;
        *(short *)((char *)pContext + 0x6e) = 0;
        *(short *)((char *)pContext + 0x70) = (short)nResult;
        *(short *)((char *)pContext + 0x72) = y2;
        *(short *)((char *)pContext + 0x74) = *(short *)&nUploadState;
        *(short *)((char *)pContext + 0x76) = *((short *)&nUploadState + 1);
        ((OneArgVirtual)(*(void ***)((char *)pContext + 0x68))[1])(
            (char *)pContext + 0x68, 0, pDispatchQueue);
    }

    nResult = ((IntVirtual)pBindingSurface->m_pVtable[0x10])(
        pBindingSurface, 0);
    if (nResult != 0) {
        *(short *)((char *)pContext + 0x80) = (short)nResult;
        *(short *)((char *)pContext + 0x82) = *((short *)&nResult + 1);
        *(short *)((char *)pContext + 0x84) = *(short *)&nUploadState;
        *(short *)((char *)pContext + 0x86) = *((short *)&nUploadState + 1);
        pContext[0x1f] = 0;
        ((OneArgVirtual)(*(void ***)((char *)pContext + 0x78))[1])(
            (char *)pContext + 0x78, 0, pDispatchQueue);
    }

    DispatchAndClearPointerQueue(pDispatchQueue);
    pDispatchQueue->m_cEntries = 0;
    ((VoidVirtual)(*(void ***)pContext)[0x2d])(pContext, 0);

    if (pQueuedPointSink != 0) {
        ((DeleteVirtual)(*(void ***)pQueuedPointSink)[0])(
            pQueuedPointSink, 0, 1);
    }
}

// FUNCTION: LEMBALL 0x004663d0
void *VsGdiCompactResourceGeometryHelper::InitializeCompactResourceGeometryHelper(void) {
    char *pbHelper;
    unsigned short wZero;

    pbHelper = (char *)this;
    wZero = 0;
    *(unsigned short *)(pbHelper + 6) = wZero;
    *(unsigned short *)(pbHelper + 4) = wZero;
    *(unsigned short *)(pbHelper + 10) = wZero;
    *(unsigned short *)(pbHelper + 8) = wZero;
    *(unsigned short *)(pbHelper + 0xe) = wZero;
    *(unsigned short *)(pbHelper + 0xc) = wZero;
    *(unsigned short *)(pbHelper + 0x12) = wZero;
    *(unsigned short *)(pbHelper + 0x10) = wZero;
    *(unsigned short *)(pbHelper + 0x16) = wZero;
    *(unsigned short *)(pbHelper + 0x14) = wZero;
    *(unsigned short *)(pbHelper + 0x1a) = wZero;
    *(unsigned short *)(pbHelper + 0x18) = wZero;
    *(unsigned short *)(pbHelper + 0x1e) = wZero;
    *(unsigned short *)(pbHelper + 0x1c) = wZero;
    *(void **)pbHelper = &g_VSGDI_CompactResourceGeometryHelperVtable;
    *(int *)(pbHelper + 0x3c) = 1;
    *(unsigned short *)(pbHelper + 0x26) = wZero;
    *(int *)(pbHelper + 0x40) = 0;
    *(unsigned short *)(pbHelper + 0x24) = wZero;
    *(unsigned short *)(pbHelper + 0x2a) = wZero;
    *(unsigned short *)(pbHelper + 0x28) = wZero;
    return this;
}

// FUNCTION: LEMBALL 0x00466430
static void LEMBALL_FASTCALL RestoreCompactResourceGeometryHelperVtable(void *pvHelper) {
    *(void **)pvHelper = g_VSGDI_CompactResourceGeometryHelperVtable;
}

// FUNCTION: LEMBALL 0x00466440
void *ConstructHelperGroup0RowBuffer(void *pvHelperGroup, int fConstructCompactHelper) {
    int iCompactDelta;
    int *pHelperGroup;

    pHelperGroup = (int *)pvHelperGroup;
    if (fConstructCompactHelper != 0) {
        *(void **)((char *)pHelperGroup + 4) = &g_VSGDI_ResourceGeometryHelperGroup0ConstructionAdjustorVtable;
        ((VsGdiCompactResourceGeometryHelper *)((char *)pHelperGroup + 0x58))
            ->InitializeCompactResourceGeometryHelper();
    }
    InitializeResourceGeometryRowBuffer((char *)pHelperGroup + 8);
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup0Vtable;
    iCompactDelta = *(int *)((char *)(unsigned long)pHelperGroup[1] + 4);
    *(void **)((char *)pHelperGroup + 4 + iCompactDelta) =
        (void *)&g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable;
    *(int *)((char *)pHelperGroup + iCompactDelta) = iCompactDelta - 0x54;
    *(short *)((char *)pHelperGroup + 0x50) = 0;
    *(int *)((char *)pHelperGroup + 0x4c) = 0;
    *(short *)((char *)pHelperGroup + 0x52) = 0;
    *(int *)((char *)pHelperGroup + 0x48) = 0;
    return pHelperGroup;
}

// FUNCTION: LEMBALL 0x004664b0
void LEMBALL_FASTCALL DestroyHelperGroup0RowBuffer(void *pvHelperGroup) {
    int iCompactDelta;
    int *pHelperGroup;
    int *pRowBuffer;

    pHelperGroup = (int *)pvHelperGroup;
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup0Vtable;
    pRowBuffer = pHelperGroup + 2;
    iCompactDelta = *(int *)((char *)(unsigned long)pHelperGroup[1] + 4);
    *(void **)((char *)pRowBuffer + iCompactDelta - 4) = (void *)&g_VSGDI_ResourceGeometryHelperGroup0CompactSubobjectVtable;
    *(int *)((char *)pRowBuffer + iCompactDelta - 8) = iCompactDelta - 0x54;
    DestroyResourceGeometryRowBuffer(pRowBuffer);
}

// FUNCTION: LEMBALL 0x004664e0
int LEMBALL_FASTCALL GetHelperGroup0DirtyFlag(void *pGroupState, int) {
    void *pLinkedTarget;
    void *pLinkedCompact;
    void **pVtable;
    int nCompactDelta;

    nCompactDelta = *(int *)((char *)*(void **)((char *)pGroupState - 0x54) + 4);
    pLinkedTarget = *(void **)((char *)pGroupState + nCompactDelta - 0x34);
    if (pLinkedTarget != g_pResourceGeometryHelperTarget) {
        nCompactDelta = *(int *)((char *)*(void **)((char *)pLinkedTarget + 0x40) + 4);
        pLinkedCompact =
            (char *)pLinkedTarget + 0x40 + nCompactDelta;
        pVtable = *(void ***)pLinkedCompact;
        return ((int (LEMBALL_FASTCALL *)(void *, int))pVtable[15])(
            pLinkedCompact, 0);
    }
    return *(int *)((char *)pGroupState - 0x10);
}

// FUNCTION: LEMBALL 0x00466510
void LEMBALL_FASTCALL ReleaseHelperGroup0BackingBuffer(int nHelperGroup) {
    if (*(int *)(unsigned long)(nHelperGroup + 0x4c) != 0) {
        FreeVSMemBlock((void *)(unsigned long)*(int *)(unsigned long)(nHelperGroup + 0x4c));
        *(int *)(unsigned long)(nHelperGroup + 0x4c) = 0;
        *(short *)(unsigned long)(nHelperGroup + 0x52) = 0;
        *(short *)(unsigned long)(nHelperGroup + 0x50) = 0;
    }
    ReleaseResourceGeometryRowBufferStorage(nHelperGroup + 8);
}

// FUNCTION: LEMBALL 0x00466660
void LEMBALL_FASTCALL EnsureHelperGroup0BackingBufferThunk(void *pHelperGroup) {
    EnsureHelperGroup0BackingBuffer((int)(unsigned long)pHelperGroup);
}

// FUNCTION: LEMBALL 0x00466670
void *ConstructHelperGroup1RowBuffer(void *pvHelperGroup, int fConstructCompactHelper) {
    int iCompactDelta;
    int *pHelperGroup;

    pHelperGroup = (int *)pvHelperGroup;
    if (fConstructCompactHelper != 0) {
        *(void **)((char *)pHelperGroup + 4) = &g_VSGDI_ResourceGeometryHelperGroup1ConstructionAdjustorVtable;
        ((VsGdiCompactResourceGeometryHelper *)((char *)pHelperGroup + 0x58))
            ->InitializeCompactResourceGeometryHelper();
    }
    InitializeResourceGeometryRowBuffer((char *)pHelperGroup + 8);
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup1Vtable;
    iCompactDelta = *(int *)((char *)(unsigned long)pHelperGroup[1] + 4);
    *(void **)((char *)pHelperGroup + 4 + iCompactDelta) =
        (void *)&g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable;
    *(int *)((char *)pHelperGroup + iCompactDelta) = iCompactDelta - 0x54;
    *(short *)((char *)pHelperGroup + 0x50) = 0;
    *(int *)((char *)pHelperGroup + 0x48) = 0;
    *(short *)((char *)pHelperGroup + 0x52) = 0;
    *(int *)((char *)pHelperGroup + 0x4c) = 0;
    return pHelperGroup;
}

// FUNCTION: LEMBALL 0x004666e0
void LEMBALL_FASTCALL DestroyHelperGroup1RowBuffer(void *pvHelperGroup) {
    int *pHelperGroup;

    pHelperGroup = (int *)pvHelperGroup;
    *(void **)pHelperGroup = &g_VSGDI_ResourceGeometryHelperGroup1Vtable;
    pHelperGroup += 2;
    *(void **)((char *)pHelperGroup +
               *(int *)((char *)(unsigned long)pHelperGroup[-1] + 4) - 4) =
        (void *)&g_VSGDI_ResourceGeometryHelperGroup1CompactSubobjectVtable;
    *(int *)((char *)pHelperGroup +
             *(int *)((char *)(unsigned long)pHelperGroup[-1] + 4) - 8) =
        *(int *)((char *)(unsigned long)pHelperGroup[-1] + 4) - 0x54;
    DestroyResourceGeometryRowBuffer(pHelperGroup);
}

// FUNCTION: LEMBALL 0x00466710
void ReleaseHelperGroup1BackingBuffer(int nHelperGroup) {
    if (*(int *)(unsigned long)(nHelperGroup + 0x4c) != 0) {
        FreeVSMemBlock((void *)(unsigned long)*(int *)(unsigned long)(nHelperGroup + 0x4c));
        *(int *)(unsigned long)(nHelperGroup + 0x4c) = 0;
        *(short *)(unsigned long)(nHelperGroup + 0x52) = 0;
        *(short *)(unsigned long)(nHelperGroup + 0x50) = 0;
    }
    ReleaseResourceGeometryRowBufferStorage(nHelperGroup + 8);
}

// FUNCTION: LEMBALL 0x00466740
void LEMBALL_FASTCALL EnsureHelperGroup1BackingBufferBody(void *pHelperGroup) {
    VsGdiHelperSurface *pSurface;
    short *pExtent;
    short aRequestedRect[4];
    short aBackingSize[2];
    int nCompactDelta;
    unsigned int cbExisting;
    unsigned int cbRequired;

    nCompactDelta = *(int *)((char *)*(void **)((char *)pHelperGroup + 4) + 4);
    pExtent = (short *)((char *)pHelperGroup + nCompactDelta + 0x18);
    aRequestedRect[0] = (short)(pExtent[0] * 2);
    aRequestedRect[1] = pExtent[1];
    aRequestedRect[2] = 0;
    aRequestedRect[3] = 0;
    pSurface = (VsGdiHelperSurface *)((char *)pHelperGroup + 8);
    pSurface->ComputeBackingDimensions(
        aBackingSize,
        aRequestedRect,
        *(int *)((char *)pHelperGroup + nCompactDelta + 0x44) * 2);

    cbExisting = (unsigned short)*(short *)((char *)pHelperGroup + 0x50) *
                 (unsigned short)*(short *)((char *)pHelperGroup + 0x52) * 2;
    cbRequired = (unsigned int)((int)aBackingSize[0] *
                                (int)aBackingSize[1]);
    if (cbExisting < cbRequired) {
        ReleaseHelperGroup1BackingBuffer(
            (int)(unsigned long)pHelperGroup);
    }

    if ((int)pExtent[0] * (int)pExtent[1] != 0) {
        if (*(void **)((char *)pHelperGroup + 0x4c) == 0) {
            *(short *)((char *)pHelperGroup + 0x50) =
                (short)((unsigned int)(int)aBackingSize[0] >> 1);
            *(short *)((char *)pHelperGroup + 0x52) = aBackingSize[1];
            *(void **)((char *)pHelperGroup + 0x4c) =
                AllocateVSMemBlock(
                    (unsigned short)*(short *)((char *)pHelperGroup + 0x50) *
                    (unsigned short)aBackingSize[1] * 2);
        }
        if (*(void **)((char *)pHelperGroup + 0x4c) == 0) {
            *(int *)((char *)pHelperGroup + 0x48) = 0;
        }
        pSurface->ConfigureBackingStrideAndOrigin(
            (int)(unsigned long)*(void **)((char *)pHelperGroup + 0x4c),
            (int)aBackingSize[0]);
    }
}

// FUNCTION: LEMBALL 0x00466870
void LEMBALL_FASTCALL EnsureHelperGroup1BackingBuffer(void *pHelperGroup) {
    EnsureHelperGroup1BackingBufferBody(pHelperGroup);
}

// FUNCTION: LEMBALL 0x00466880
void LEMBALL_FASTCALL CopyRectTupleIfExtentEmpty(void *pTarget,
                                                 int,
    short *paRect)

{
    short *paCurrentRect;
    short *paPosition;

    paCurrentRect = (short *)((char *)pTarget + 4);
    if ((int)paCurrentRect[0] * (int)paCurrentRect[1] != 0 &&
        (int)paRect[0] * (int)paRect[1] != 0) {
        return;
    }

    paCurrentRect[0] = paRect[0];
    paCurrentRect[1] = paRect[1];
    paPosition = paRect != 0 ? paRect + 2 : 0;
    paCurrentRect[2] = paPosition[0];
    paCurrentRect[3] = paPosition[1];
}

// FUNCTION: LEMBALL 0x00466990
int LEMBALL_FASTCALL GetHelperGroup1DirtyFlag(void *pGroupState, int) {
    return *(int *)((char *)pGroupState - 0x10);
}

// FUNCTION: LEMBALL 0x004669a0
int VsGdiQuantizedHelperUploadRectTracker::InitializeQuantizedHelperUploadRectTracker(int cRectEntries, int nTrackerExtent, int nCellExtent) {
    short *pTracker;
    short *pRect;
    int i;
    short cxTracker;
    short cyTracker;
    short cxCell;
    short cyCell;

    pTracker = (short *)this;
    pTracker[0xb] = 0;
    pTracker[0xa] = 0;
    pTracker[0xd] = 0;
    pTracker[0xc] = 0;
    pTracker[0xf] = 0;
    pTracker[0xe] = 0;
    pTracker[0x19] = 0;
    pTracker[0x18] = 0;
    pTracker[0x1b] = 0;
    pTracker[0x1a] = 0;

    pRect = 0;
    if (cRectEntries != 0) {
        pRect = (short *)AllocateVSMemBlock((unsigned int)(cRectEntries * 0xc));
    }
    if (pRect == 0) {
        *(void **)((char *)this + 0xc) = 0;
    } else {
        for (i = 0; i < cRectEntries; ++i) {
            pRect[0] = 0;
            pRect[1] = 0;
            pRect[2] = 0;
            pRect[3] = 0;
            pRect[4] = 0;
            pRect[5] = 0;
            pRect += 6;
        }
        *(short **)((char *)this + 0xc) = pRect - cRectEntries * 6;
    }

    *(int *)this = cRectEntries;
    *(int *)((char *)this + 0x18) = nCellExtent;
    *(int *)((char *)this + 0x14) = nTrackerExtent;
    cxTracker = (short)nTrackerExtent;
    cyTracker = (short)((unsigned int)nTrackerExtent >> 16);
    cxCell = (short)nCellExtent;
    cyCell = (short)((unsigned int)nCellExtent >> 16);
    *(short *)((char *)this + 0x1c) =
        (short)((cxTracker + cxCell - 1) / cxCell);
    *(short *)((char *)this + 0x1e) =
        (short)((cyTracker + cyCell - 1) / cyCell);
    *(int *)((char *)this + 0x44) = 0;
    *(int *)((char *)this + 0x10) = 0;
    EnsureHelperUploadStateMapCapacity(this);
    InitializeHelperUploadStatePending((int)(unsigned long)this);
    return (int)(unsigned long)this;
}

// FUNCTION: LEMBALL 0x00466a90
void LEMBALL_FASTCALL ResetHelperUploadStateMap(void *pTracker) {
    if (*(void **)((char *)pTracker + 0x10) != 0) {
        FreeVSMemBlock(*(void **)((char *)pTracker + 0x10));
        *(void **)((char *)pTracker + 0x10) = 0;
    }
    InitializeHelperUploadStatePending((int)(unsigned long)pTracker);
}

// FUNCTION: LEMBALL 0x00466ac0
void LEMBALL_FASTCALL EnsureHelperUploadStateMapCapacity(void *pTracker) {
    int nMapEntries;

    if (*(int *)((char *)pTracker + 0x10) == 0) {
        nMapEntries = (int)*(short *)((char *)pTracker + 0x1c) *
            (int)*(short *)((char *)pTracker + 0x1e);
        *(int *)((char *)pTracker + 0x20) = nMapEntries;
        *(void **)((char *)pTracker + 0x10) = AllocateVSMemBlock((unsigned int)nMapEntries);
    }
}

// FUNCTION: LEMBALL 0x00466af0
void LEMBALL_FASTCALL SetHelperUploadTrackerExtent(
    void *pTracker, int, int nExtent)

{
    short cxExtent;
    short cyExtent;
    short cxCell;
    short cyCell;
    short cxMap;
    short cyMap;

    cxExtent = (short)nExtent;
    cyExtent = (short)((unsigned int)nExtent >> 16);
    if (*(short *)((char *)pTracker + 0x14) == cxExtent &&
        *(short *)((char *)pTracker + 0x16) == cyExtent) {
        return;
    }

    cxCell = *(short *)((char *)pTracker + 0x18);
    cyCell = *(short *)((char *)pTracker + 0x1a);
    cxMap = (short)(cxExtent / cxCell);
    cyMap = (short)(cyExtent / cyCell);
    *(short *)((char *)pTracker + 0x14) = cxExtent;
    *(short *)((char *)pTracker + 0x16) = cyExtent;
    *(short *)((char *)pTracker + 0x1c) = cxMap;
    *(short *)((char *)pTracker + 0x1e) = cyMap;
    if ((int)cxMap * (int)cyMap > *(int *)((char *)pTracker + 0x20)) {
        ResetHelperUploadStateMap(pTracker);
    }
    if ((int)cxExtent * (int)cyExtent > 0) {
        EnsureHelperUploadStateMapCapacity(pTracker);
    }
}

// FUNCTION: LEMBALL 0x00466b60
void LEMBALL_FASTCALL InitializeHelperUploadStatePending(int nUploadState) {
    *(unsigned short *)(unsigned long)(nUploadState + 0x30) = 0;
    *(int *)(unsigned long)(nUploadState + 0x2c) = 0;
    *(char *)(unsigned long)(nUploadState + 0x48) = 'P';
    *(unsigned short *)(unsigned long)(nUploadState + 0x32) = 0;
    *(unsigned short *)(unsigned long)(nUploadState + 0x34) = 0;
    *(unsigned short *)(unsigned long)(nUploadState + 0x36) = 0;
    *(int *)(unsigned long)(nUploadState + 4) = -1;
    *(int *)(unsigned long)(nUploadState + 8) = -1;
}

// FUNCTION: LEMBALL 0x00466b90
void LEMBALL_FASTCALL ResetHelperUploadRectListAndStateMap(int nUploadState) {
    if (*(int *)(unsigned long)(nUploadState + 0xc) != 0) {
        FreeVSMemBlock((void *)(unsigned long)*(int *)(unsigned long)(nUploadState + 0xc));
        *(int *)(unsigned long)(nUploadState + 0xc) = 0;
    }
    ResetHelperUploadStateMap((void *)(unsigned long)nUploadState);
}

// FUNCTION: LEMBALL 0x00466be0
void VsGdiQuantizedHelperUploadRectTracker::MarkHelperUploadCellsForRect(short *pRect) {
    char *pTracker;
    short cxCell;
    short cyCell;
    int nFirstColumn;
    int nFirstRow;
    unsigned int cColumns;
    int cRows;
    int nGridWidth;
    int nRow;
    unsigned char *pRow;
    unsigned int i;

    pTracker = (char *)this;
    if (*(int *)(pTracker + 0x10) == 0)
        return;

    cxCell = *(short *)(pTracker + 0x18);
    cyCell = *(short *)(pTracker + 0x1a);
    nFirstColumn = pRect[2] / cxCell;
    nFirstRow = pRect[3] / cyCell;
    cColumns = (unsigned int)((pRect[0] + pRect[2] - 1 + cxCell) / cxCell - nFirstColumn);
    cRows = (pRect[1] + pRect[3] - 1 + cyCell) / cyCell - nFirstRow;
    if (*(short *)(pTracker + 0x1e) < nFirstRow + cRows)
        cRows = *(short *)(pTracker + 0x1e) - nFirstRow;
    nGridWidth = *(short *)(pTracker + 0x1c);
    if (nGridWidth < nFirstColumn + (int)cColumns)
        cColumns = (unsigned int)(nGridWidth - nFirstColumn);

    if ((int)cColumns <= 0 || cRows <= 0)
        return;

    pRow = (unsigned char *)(*(int *)(pTracker + 0x10) +
        nGridWidth * nFirstRow + nFirstColumn);
    nRow = cRows;
    do {
        for (i = 0; i < cColumns; ++i)
            pRow[i] = *(unsigned char *)(pTracker + 0x48);
        pRow += nGridWidth;
        --nRow;
    } while (nRow != 0);
    *(int *)(pTracker + 0x2c) += (int)pRect[0] * (int)pRect[1];
}

// FUNCTION: LEMBALL 0x00466ce0
void VsGdiQuantizedHelperUploadRectTracker::MarkHelperUploadRectWithState(
    short *pRect, unsigned char bState)

{
    unsigned char bPreviousState;
    bPreviousState = *(unsigned char *)((char *)this + 0x48);
    *(unsigned char *)((char *)this + 0x48) = bState;
    MarkHelperUploadCellsForRect(pRect);
    *(unsigned char *)((char *)this + 0x48) = bPreviousState;
}

// FUNCTION: LEMBALL 0x00466d10
void LEMBALL_FASTCALL PromoteHelperUploadStateToActive(int nUploadState) {
    unsigned int nState;

    nState = *(unsigned char *)(unsigned long)(nUploadState + 0x48);
    switch (nState) {
    case 'P':
        *(char *)(unsigned long)(nUploadState + 0x48) = 'A';
        break;
    case 'p':
        *(char *)(unsigned long)(nUploadState + 0x48) = 'a';
        break;
    }
}

// FUNCTION: LEMBALL 0x00466d40
int VsGdiQuantizedHelperUploadRectTracker::ConsumeMarkedHelperUploadRun(
    unsigned char bTargetState, int nEntryState,
                                     unsigned char bReplacementState)

{
    char *pTracker;
    unsigned char *pMap;
    unsigned char *pRow;
    short *pEntry;
    int cxGrid, cyGrid, cxCell, cyCell;
    int xCell, yCell, xRunEnd, cRunCells, cRunRows, x;

    pTracker = (char *)this;
    pMap = *(unsigned char **)(pTracker + 0x10);
    cxGrid = *(short *)(pTracker + 0x1c);
    cyGrid = *(short *)(pTracker + 0x1e);
    cxCell = *(short *)(pTracker + 0x18);
    cyCell = *(short *)(pTracker + 0x1a);
    xCell = *(int *)(pTracker + 0x24);
    yCell = *(int *)(pTracker + 0x28);
    while (yCell < cyGrid) {
        pRow = pMap + yCell * cxGrid;
        while (xCell < cxGrid && pRow[xCell] != bTargetState)
            ++xCell;
        if (xCell < cxGrid)
            break;
        xCell = 0;
        ++yCell;
    }
    if (yCell >= cyGrid)
        return 0;

    pRow = pMap + yCell * cxGrid;
    xRunEnd = xCell;
    while (xRunEnd < cxGrid && pRow[xRunEnd] == bTargetState) {
        pRow[xRunEnd++] = bReplacementState;
    }
    cRunCells = xRunEnd - xCell;
    cRunRows = 1;
    while (yCell + cRunRows < cyGrid) {
        pRow = pMap + (yCell + cRunRows) * cxGrid;
        x = xCell;
        while (x < cxGrid && pRow[x] == bTargetState)
            ++x;
        if (x - xCell != cRunCells)
            break;
        for (x = 0; x < cRunCells; ++x)
            pRow[xCell + x] = bReplacementState;
        ++cRunRows;
    }

    pEntry = (short *)(*(char **)(pTracker + 0x0c) +
                       *(int *)(pTracker + 4) * 0x0c);
    pEntry[0] = (short)(cRunCells * cxCell);
    pEntry[1] = (short)(cRunRows * cyCell);
    pEntry[2] = (short)(xCell * cxCell);
    pEntry[3] = (short)(yCell * cyCell);
    *(int *)(pEntry + 4) = nEntryState;
    ++*(int *)(pTracker + 4);
    *(int *)(pTracker + 0x24) = xRunEnd < cxGrid ? xRunEnd : 0;
    *(int *)(pTracker + 0x28) = yCell;
    return 1;
}

// FUNCTION: LEMBALL 0x00466ef0
int VsGdiQuantizedHelperUploadRectTracker::BuildHelperUploadRectList(void)

{
    char *pTracker;
    pTracker = (char *)this;
    if (*(void **)(pTracker + 0x10) == 0 ||
        *(void **)(pTracker + 0x0c) == 0)
        return 0;
    if (*(int *)(pTracker + 4) == -1) {
        *(int *)(pTracker + 4) = 0;
        *(int *)(pTracker + 0x24) = 0;
        *(int *)(pTracker + 0x28) = 0;
        while (ConsumeMarkedHelperUploadRun(1, 1, 0)) {}
        *(int *)(pTracker + 0x24) = 0;
        *(int *)(pTracker + 0x28) = 0;
        while (ConsumeMarkedHelperUploadRun('P', 1, 1)) {}
        *(int *)(pTracker + 0x24) = 0;
        *(int *)(pTracker + 0x28) = 0;
        while (ConsumeMarkedHelperUploadRun('p', 0, 1)) {}
        *(int *)(pTracker + 8) = *(int *)(pTracker + 4);
        if (*(unsigned char *)(pTracker + 0x48) == 'A' ||
            *(unsigned char *)(pTracker + 0x48) == 'a') {
            *(int *)(pTracker + 0x24) = 0;
            *(int *)(pTracker + 0x28) = 0;
            while (ConsumeMarkedHelperUploadRun('A', 1, 1)) {}
            *(int *)(pTracker + 0x24) = 0;
            *(int *)(pTracker + 0x28) = 0;
            while (ConsumeMarkedHelperUploadRun('a', 0, 1)) {}
        }
    }
    return *(int *)(pTracker + 4);
}

// FUNCTION: LEMBALL 0x00467000
short *VsGdiQuantizedHelperUploadRectTracker::GetHelperUploadRectEntryByIndex(
    int iRect)

{
    if (*(int *)((char *)this + 4) == -1)
        BuildHelperUploadRectList();
    return (short *)(*(char **)((char *)this + 0x0c) + iRect * 0x0c);
}

// FUNCTION: LEMBALL 0x00467020
int VsGdiQuantizedHelperUploadRectTracker::GetHelperUploadActiveRectStartIndex(
    void)

{
    if (*(void **)((char *)this + 0x10) == 0)
        return 0;
    if (*(int *)((char *)this + 4) == -1)
        BuildHelperUploadRectList();
    return *(int *)((char *)this + 8);
}

// FUNCTION: LEMBALL 0x00467060
VsGdiGeometryHelperPointerArray *
VsGdiGeometryHelperPointerArray::InitializeGeometryHelperPointerArray(
    short *pRect, int nEntryParameter, void *pWrappedTarget)

{
    int i;
    int nEntryCount;
    void *pSlotManager;
    if ((int)pRect[0] * (int)pRect[1] > 1) {
        nEntryCount = nEntryParameter + 3;
        m_nEntryCount = nEntryCount;
        m_pEntries = AllocateVSMemBlock((unsigned int)nEntryCount * 4);

        pSlotManager = g_pResourceGeometryHelperSlotManager;
        m_pTarget = (VsGdiResourceGeometryHelperTargetView *)
            ((VsGdiResourceGeometryHelperSlotManager *)pSlotManager)
                ->AllocateResourceGeometryHelperSlot(pRect, pWrappedTarget);
        m_nReserved = 0;
        for (i = 0; i < m_nEntryCount; ++i) {
            ((void **)m_pEntries)[i] = 0;
        }
    }
    return this;
}

// FUNCTION: LEMBALL 0x004670d0
void LEMBALL_FASTCALL DestroyGeometryHelperPointerArray(void *pHelper) {
    VsGdiGeometryHelperPointerArray *pArray;

    pArray = (VsGdiGeometryHelperPointerArray *)pHelper;
    FreeVSMemBlock(pArray->m_pEntries);
    ((VsGdiResourceGeometryHelperSlotReleaseInterface *)
         g_pResourceGeometryHelperSlotManager)
        ->ReleaseResourceGeometryHelperSlot(pArray->m_pTarget);
}

// FUNCTION: LEMBALL 0x004670f0
void AppendPointerQueueEntry(void *pQueue, void *pEntry) {
    VsGdiHelperDispatchQueue *pDispatchQueue;

    pDispatchQueue = (VsGdiHelperDispatchQueue *)pQueue;
    pDispatchQueue->m_ppEntries[pDispatchQueue->m_cEntries] = pEntry;
    pDispatchQueue->m_cEntries = pDispatchQueue->m_cEntries + 1;
}

// FUNCTION: LEMBALL 0x00467110
void LEMBALL_FASTCALL DispatchAndClearPointerQueue(void *pQueue) {
    void *pQueuedEntry;
    VsGdiHelperDispatchQueue *pDispatchQueue;
    int nByteOffset;
    int i;
    typedef void (LEMBALL_FASTCALL *EntryDispatchVirtual)(
        void *, int, void *);

    pDispatchQueue = (VsGdiHelperDispatchQueue *)pQueue;
    if (RefreshHelperTargetBindingIfChanged(
            pDispatchQueue->m_pHelperTarget) != 0) {
        nByteOffset = 0;
        i = 0;
        if (0 < pDispatchQueue->m_cEntries) {
            do {
                pQueuedEntry = *(void **)((char *)pDispatchQueue->m_ppEntries + nByteOffset);
                if (IsPointerInsideManagedMemoryRegions(pQueuedEntry) != 0) {
                    ((EntryDispatchVirtual)(*(void ***)pQueuedEntry)[2])(
                        pQueuedEntry, 0, pDispatchQueue);
                }
                nByteOffset += 4;
                ++i;
            } while (i < pDispatchQueue->m_cEntries);
        }
        InvokeRootHelperTargetPostQueueCallback((int)(unsigned long)pDispatchQueue->m_pHelperTarget);
    }
}

// FUNCTION: LEMBALL 0x0046aec0
void *LEMBALL_FASTCALL ConstructStatusIndicatorRenderClientBase(int *pRenderClient) {
    InitializeRenderQueueNodeBase(pRenderClient);
    *(void **)((char *)pRenderClient + 0x40) = g_VSGDI_StatusIndicatorRenderClientVtable;
    *(void **)((char *)pRenderClient + 0x44) = g_VSGDI_StatusIndicatorPointSinkVtable;
    *(short *)((char *)pRenderClient + 0x12) = 0;
    pRenderClient[0x12] = 0;
    *(short *)((char *)pRenderClient + 0x10) = 0;
    pRenderClient[0x13] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x16) = 0;
    pRenderClient[0x14] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x14) = 0;
    pRenderClient[0x15] = 0xaa55aa55;
    pRenderClient[0x16] = 0xaa55aa55;
    pRenderClient[0x17] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x76) = 0;
    pRenderClient[0x18] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x74) = 0;
    pRenderClient[0x19] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x7a) = 0;
    pRenderClient[0x1a] = 0xaa55aa55;
    *(short *)((char *)pRenderClient + 0x78) = 0;
    *(void **)pRenderClient = g_VSGDI_StatusIndicatorBaseVtable;
    InitializeStatusIndicatorRenderClientDefaults(pRenderClient);
    return pRenderClient;
}

// FUNCTION: LEMBALL 0x0046afd0
void LEMBALL_FASTCALL InitializeStatusIndicatorRenderClientDefaults(int *pRenderClient) {
    int *pEntryBlock;
    int *pEntry;
    int i;
    int nZero;
    DWORD dwNow;

    nZero = 0;
    pRenderClient[0xa] = nZero;
    pEntryBlock = (int *)AllocateVSMemBlock(0x1c);
    if (pEntryBlock == 0) {
        pRenderClient[7] = 0;
    } else {
        pEntryBlock[0] = 1;
        pEntry = pEntryBlock + 1;
        for (i = 0; i <= 1; ++i) {
            new (pEntry) LevelPackagedRectQueueEntry;
            pEntry += 6;
        }
        pRenderClient[7] = (int)(unsigned long)(pEntryBlock + 1);
    }
    pEntry = (int *)(unsigned long)pRenderClient[7];
    *(short *)((char *)pEntry + 4) = 0;
    *(short *)((char *)pEntry + 6) = 0;
    pEntry[2] = 0;
    pEntry[3] = 0;
    pEntry[4] = 0;
    ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
        ->RegisterOrderedRenderDispatchClient(pRenderClient, -0x19);
    pRenderClient[0xd] = 0;
    pRenderClient[0xc] = 0;
    pRenderClient[0xb] = 0;
    pRenderClient[6] = 0;
    pRenderClient[0xe] = 0;
    pRenderClient[0xf] = 1;
    pRenderClient[8] = 0;
    pRenderClient[0x12] = 1;
    pRenderClient[0x1f] = 3;
    pRenderClient[0x20] = 4;
    pRenderClient[0x21] = 1;
    pRenderClient[0x24] = 0;
    pRenderClient[0x22] = 2;
    pRenderClient[0x23] = 0x1f;
    pRenderClient[0x25] = 0x49;
    pRenderClient[0x13] = 0x199;
    pRenderClient[0x14] = 0x8000;
    pRenderClient[0x17] = 0;
    pRenderClient[0x15] = (int)*(short *)((char *)pRenderClient + 0x10) << 12;
    pRenderClient[0x18] = 0;
    pRenderClient[0x19] = 0;
    pRenderClient[0x1a] = 0;
    pRenderClient[0x16] = (int)*(short *)((char *)pRenderClient + 0x12) << 12;
    dwNow = timeGetTime();
    pRenderClient[0x1b] = (int)dwNow;
    pRenderClient[0x1c] = (int)dwNow;
}

// FUNCTION: LEMBALL 0x0046b0e0
int LEMBALL_FASTCALL HandleStatusIndicatorInputEvent(
    void *pRenderClient, int, RenderDispatchQueueEntry *pEntry)

{
    int *pState;
    unsigned short wType;
    unsigned int dwCode;
    unsigned int dwNow;
    RenderDispatchQueueEntry OutputEntry;

    pState = (int *)pRenderClient;
    if (pState[6] == 0) {
        return 0;
    }

    wType = *(unsigned short *)&pEntry->m_awords[0];
    dwCode = pEntry->m_awords[2];
    if (wType == 3 || wType == 4) {
        if (pState[12] == 0) {
            return 0;
        }

        dwNow = timeGetTime();
        if (pState[13] != 0) {
            if (dwCode == (unsigned int)pState[0x23]) {
                OutputEntry.m_awords[3] = 0x43;
            } else if (dwCode == (unsigned int)pState[0x25]) {
                OutputEntry.m_awords[3] = 0x44;
            } else if (dwCode == (unsigned int)pState[0x24]) {
                OutputEntry.m_awords[3] = 0x45;
            } else {
                return 0;
            }

            *(unsigned short *)&OutputEntry.m_awords[0] =
                wType == 4 ? 9 : 8;
            OutputEntry.m_awords[1] = pEntry->m_awords[1];
            OutputEntry.m_awords[2] =
                ((unsigned int)(unsigned short)*(short *)((char *)pState + 0x12) << 16) |
                (unsigned short)*(short *)((char *)pState + 0x10);
            OutputEntry.m_awords[4] = 0;
            ((int (LEMBALL_FASTCALL *)(void *, int, RenderDispatchQueueEntry *))
                 (*(void ***)g_pSharedRenderDispatchQueue)[2])(
                g_pSharedRenderDispatchQueue, 0, &OutputEntry);
            return 0;
        }

        if (wType == 3) {
            if (dwCode == (unsigned int)pState[0x21] ||
                dwCode == (unsigned int)pState[0x22]) {
                pState[0x18] = 0;
                pState[0x1a] = 0;
            } else if (dwCode == (unsigned int)pState[0x1f] ||
                       dwCode == (unsigned int)pState[0x20]) {
                pState[0x17] = 0;
                pState[0x19] = 0;
            }
            return 0;
        }

        if (dwCode == (unsigned int)pState[0x21]) {
            pState[0x1c] = (int)dwNow;
            pState[0x1a] = -pState[0x13];
        } else if (dwCode == (unsigned int)pState[0x22]) {
            pState[0x1c] = (int)dwNow;
            pState[0x1a] = pState[0x13];
        } else if (dwCode == (unsigned int)pState[0x1f]) {
            pState[0x1b] = (int)dwNow;
            pState[0x19] = -pState[0x13];
        } else if (dwCode == (unsigned int)pState[0x20]) {
            pState[0x1b] = (int)dwNow;
            pState[0x19] = pState[0x13];
        }
        return 0;
    }

    if (wType == 7 && pState[0x0b] != 0 && pEntry->m_awords[4] == 0) {
        short x;
        short y;

        x = (short)(pEntry->m_awords[2] & 0xffff);
        y = (short)(pEntry->m_awords[2] >> 16);
        *(short *)((char *)pState + 0x10) = x;
        *(short *)((char *)pState + 0x12) = y;
        pState[0x15] = (int)x << 12;
        pState[0x16] = (int)y << 12;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0046ba80
int InitializeResourceGeometryHelperRuntime(void) {
    VsGdiSelectedGraphicsDriverRuntime *pSelectedGraphicsDriverRuntime;
    VsGdiResourceGeometryHelperSlotManager *pSlotManager;
    void *pvTarget;
    int *piArrowCursorClient;

    (void)g_VSGDI_BaseWindowClass;

    pSelectedGraphicsDriverRuntime =
        (VsGdiSelectedGraphicsDriverRuntime *)AllocateVSMemBlock(0xc);
    if (pSelectedGraphicsDriverRuntime != 0) {
        pSelectedGraphicsDriverRuntime->m_pWindowOwner = 0;
        pSelectedGraphicsDriverRuntime->m_fFallbackWarningShown = 0;
        g_pSelectedGraphicsDriverRuntime = pSelectedGraphicsDriverRuntime;
    } else {
        g_pSelectedGraphicsDriverRuntime = 0;
    }
    ((VsGdiSelectedGraphicsDriverRuntime *)g_pSelectedGraphicsDriverRuntime)
        ->InitializeSelectedGraphicsDriver(VsGdiDRIVER_AUTO);

    pSlotManager = (VsGdiResourceGeometryHelperSlotManager *)AllocateVSMemBlock(sizeof(VsGdiResourceGeometryHelperSlotManager));
    if (pSlotManager != 0) {
        g_pResourceGeometryHelperSlotManager =
            pSlotManager->InitializeResourceGeometryHelperSlotManager(
                (int)g_StartupGraphicsDriverConfig.m_cbSize);
    } else {
        g_pResourceGeometryHelperSlotManager = 0;
    }

    pvTarget = AllocateVSMemBlock(0x5a0);
    if (pvTarget != 0) {
        g_pResourceGeometryHelperTarget =
            ((VsGdiResourceGeometryHelperTargetView *)pvTarget)
                ->ConstructResourceGeometryHelperTarget(0, 1);
    } else {
        g_pResourceGeometryHelperTarget = 0;
    }

    piArrowCursorClient = (int *)AllocateVSMemBlock(0xa4);
    if (piArrowCursorClient != 0) {
        ConstructStatusIndicatorRenderClientBase(piArrowCursorClient);
        *(void **)piArrowCursorClient = g_VSGDI_ArrowCursorRuntimeVtable;
        InitializeArrowCursorState(piArrowCursorClient);
        g_pArrowCursorStatusIndicatorRenderClient = piArrowCursorClient;
    } else {
        g_pArrowCursorStatusIndicatorRenderClient = 0;
    }

    if (g_pResourceGeometryHelperSlotManager != 0 && g_pResourceGeometryHelperTarget != 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0046bb70
int ShutdownResourceGeometryHelperRuntime(void) {
    if (g_pArrowCursorStatusIndicatorRenderClient != 0) {
        CallDeleteVirtual(g_pArrowCursorStatusIndicatorRenderClient, 4, 1);
    }
    if (g_pResourceGeometryHelperTarget != 0) {
        DestroyResourceGeometryHelperTarget(g_pResourceGeometryHelperTarget);
        RestoreCompactResourceGeometryHelperVtable((char *)g_pResourceGeometryHelperTarget + 0x55c);
        FreeVSMemBlock(g_pResourceGeometryHelperTarget);
    }
    if (g_pResourceGeometryHelperSlotManager != 0) {
        DestroyResourceGeometryHelperSlotManager(g_pResourceGeometryHelperSlotManager);
        FreeVSMemBlock(g_pResourceGeometryHelperSlotManager);
    }
    g_pResourceGeometryHelperTarget = 0;
    g_pResourceGeometryHelperSlotManager = 0;
    if (g_pDisplayState != 0) {
        CallDeleteVirtual(g_pDisplayState, 0, 1);
    }
    if (g_pSelectedGraphicsDriverRuntime != 0) {
        FreeVSMemBlock(g_pSelectedGraphicsDriverRuntime);
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0046bc00
VsGdiResourceGeometryHelperSlotManager *VsGdiResourceGeometryHelperSlotManager::InitializeResourceGeometryHelperSlotManager(int cSlots) {
    VsGdiResourceGeometryHelperSlotManager *pSlotManager;
    VsGdiResourceGeometryHelperSlot *pSlot;
    int cRemaining;

    pSlotManager = this;
    pSlotManager->m_pVtable = &g_VSGDI_ResourceGeometryHelperSlotManagerVtable;
    pSlotManager->m_pSlots =
        (VsGdiResourceGeometryHelperSlot *)AllocateVSMemBlock((unsigned int)(cSlots * sizeof(*pSlotManager->m_pSlots)));
    pSlotManager->m_cSlots = cSlots;
    pSlotManager->m_nReserved08 = 0;
    pSlotManager->m_nReserved0C = 0;
    if (0 < cSlots) {
        pSlot = pSlotManager->m_pSlots;
        cRemaining = cSlots;
        do {
            pSlot->m_nOwnerOrState0 = 0;
            pSlot->m_nOwnerOrState8 = 0;
            pSlot->m_nOwnerOrState4 = 0;
            pSlot->m_nOwnerOrState10 = 0;
            pSlot->m_nOwnerOrStateC = 0;
            pSlot->m_fAvailable = 1;
            pSlot->m_nOwnerOrState18 = 0;
            ++pSlot;
            --cRemaining;
        } while (cRemaining != 0);
    }
    g_pResourceGeometryHelperTarget = 0;
    return pSlotManager;
}

// FUNCTION: LEMBALL 0x0046bc90
void DestroyResourceGeometryHelperSlotManager(void *pvSlotManager) {
    VsGdiResourceGeometryHelperSlotManager *pSlotManager;
    VsGdiResourceGeometryHelperSlot *pSlot;
    int i;

    pSlotManager = (VsGdiResourceGeometryHelperSlotManager *)pvSlotManager;
    pSlotManager->m_pVtable = &g_VSGDI_ResourceGeometryHelperSlotManagerVtable;
    i = 0;
    if (0 < pSlotManager->m_cSlots) {
        pSlot = pSlotManager->m_pSlots;
        do {
            if (pSlot->m_fAvailable == 0) {
                AppendErrorCString(g_VSGDI_UnallocatedHelperSlotMessage);
            }
            ++pSlot;
            ++i;
        } while (i < pSlotManager->m_cSlots);
    }
    FreeVSMemBlock(pSlotManager->m_pSlots);
}

// FUNCTION: LEMBALL 0x0046bce0
int FindFirstFreeResourceGeometryHelperSlotIndex(void *pvSlotManager) {
    VsGdiResourceGeometryHelperSlotManager *pSlotManager;
    VsGdiResourceGeometryHelperSlot *pSlot;
    int i;

    pSlotManager = (VsGdiResourceGeometryHelperSlotManager *)pvSlotManager;
    i = 0;
    if (0 < pSlotManager->m_cSlots) {
        pSlot = pSlotManager->m_pSlots;
        do {
            if (pSlot->m_fAvailable != 0) {
                return i;
            }
            ++pSlot;
            ++i;
        } while (i < pSlotManager->m_cSlots);
    }
    return -1;
}

// FUNCTION: LEMBALL 0x0046bd10
void *VsGdiResourceGeometryHelperSlotManager::AllocateResourceGeometryHelperSlot(
    short *pRect, void *pWrappedTarget)

{
    VsGdiResourceGeometryHelperSlotManager *pSlotManager;
    VsGdiResourceGeometryHelperSlot *pSlot;
    VsGdiResourceGeometryHelperTargetView *pTarget;
    int i;

    pSlotManager = this;
    i = FindFirstFreeResourceGeometryHelperSlotIndex(pSlotManager);
    if (i < 0)
        return 0;

    pSlot = pSlotManager->m_pSlots + i;
    pTarget = (VsGdiResourceGeometryHelperTargetView *)AllocateVSMemBlock(0x5a0);
    if (pTarget != 0) {
        pTarget->InitializeResourceGeometryHelper((const VsGdiRect *)pRect, pWrappedTarget, 1);
    }
    pSlot->m_nOwnerOrState0 = (int)(unsigned long)pTarget;
    pSlot->m_nOwnerOrState4 = (int)(unsigned long)pWrappedTarget;
    pSlot->m_nOwnerOrState8 = 0;
    pSlot->m_nOwnerOrStateC = 0;
    pSlot->m_nOwnerOrState10 = pWrappedTarget == g_pResourceGeometryHelperTarget;
    pSlot->m_fAvailable = 0;
    pSlot->m_nOwnerOrState18 = 0;
    if (pSlot->m_nOwnerOrState10 != 0)
        ++g_nResourceGeometryHelperSlotCount;
    return pTarget;
}

// FUNCTION: LEMBALL 0x0046bed0
void VsGdiResourceGeometryHelperSlotManager::RemoveResourceGeometryHelperSlot(
    void *pTarget)
{
    VsGdiResourceGeometryHelperSlot *pSlot;
    int i;

    i = FindResourceGeometryHelperSlot(pTarget);
    pSlot = m_pSlots + i;
    if (pSlot->m_nOwnerOrState0 != 0) {
        DestroyResourceGeometryHelperTarget(
            (void *)(unsigned long)pSlot->m_nOwnerOrState0);
        RestoreCompactResourceGeometryHelperVtable(
            (char *)(unsigned long)pSlot->m_nOwnerOrState0 + 0x55c);
        FreeVSMemBlock((void *)(unsigned long)pSlot->m_nOwnerOrState0);
    }
    if (pSlot->m_nOwnerOrState10 != 0) {
        --m_nReserved0C;
    }
    pSlot->m_nOwnerOrState0 = 0;
    pSlot->m_nOwnerOrState8 = 0;
    pSlot->m_nOwnerOrState4 = 0;
    pSlot->m_nOwnerOrState10 = 0;
    pSlot->m_nOwnerOrStateC = 0;
    pSlot->m_fAvailable = 1;
}

// FUNCTION: LEMBALL 0x0046bf70
int VsGdiResourceGeometryHelperSlotManager::FindResourceGeometryHelperSlot(
    void *pTarget)

{
    int i;
    for (i = 0; i < m_cSlots; ++i) {
        if ((void *)(unsigned long)m_pSlots[i].m_nOwnerOrState0 == pTarget)
            return i;
    }
    return -1;
}

// FUNCTION: LEMBALL 0x0046bfd0
void VsGdiResourceGeometryHelperSlotManager::DeactivateResourceGeometryHelperSlot(
    void *pTarget)

{
    VsGdiResourceGeometryHelperSlot *pSlot;
    void *pTimer;
    DWORD dwNow;
    int i;
    typedef void (LEMBALL_FASTCALL *TimerUpdateProc)(void *, int, unsigned int);

    i = FindResourceGeometryHelperSlot(pTarget);
    pSlot = m_pSlots + i;
    pSlot->m_nOwnerOrStateC = 1;
    pTimer = (void *)(unsigned long)pSlot->m_nOwnerOrState8;
    if (pTimer != 0) {
        if (*(int *)((char *)pTimer + 0x24) != 0) {
            dwNow = timeGetTime();
            ((TimerUpdateProc)(*(void ***)pTimer)[1])(
                pTimer, 0, dwNow - *(DWORD *)((char *)pTimer + 0x20));
            *(int *)((char *)pTimer + 0x24) = 0;
        }
        *(DWORD *)((char *)pTimer + 0x20) = timeGetTime();
        *(int *)((char *)pTimer + 0x24) = 1;
    }
    ((VsGdiResourceGeometryHelperTargetView *)pTarget)
        ->FlushResourceGeometryHelperTargetUpdates(
            (VsGdiResourceGeometryHelperTargetView *)
                g_pResourceGeometryHelperTarget);
}

// FUNCTION: LEMBALL 0x0046c050
VsGdiResourceGeometryHelperTargetView *VsGdiResourceGeometryHelperTargetView::InitializeResourceGeometryHelper(const VsGdiRect *pRect, void *pWrappedTarget, int fConstructCompactHelper) {
    int iCompactDelta;
    VsGdiCompactResourceGeometryHelperFields *pCompactHelper;
    VsGdiResourceGeometryLinkList *pGlobalList;
    VsGdiResourceGeometryLinkNode *pLink;
    VsGdiResourceGeometryLinkList *pChildList;
    VsGdiQuantizedHelperUploadRectTracker *pTracker;
    VsGdiRect TrackerExtent;
    VsGdiRect CellExtent;

    if (fConstructCompactHelper != 0) {
        *(void **)((char *)this + 0x40) = &g_VSGDI_CompactHelperTargetAdjustor;
        *(void **)((char *)this + 0x48) = &g_VSGDI_HelperGroup1Adjustor;
        *(void **)((char *)this + 0x9c) = &g_VSGDI_HelperGroup0Adjustor;
        ((VsGdiCompactResourceGeometryHelper *)((char *)this + 0x55c))
            ->InitializeCompactResourceGeometryHelper();
    }

    InitializeResourceGeometryRowBuffer(this);
    *(void **)this = &g_VSGDI_ResourceGeometryHelperTargetConstructionVtable;
    iCompactDelta = *(int *)((char *)(unsigned long)*(int *)((char *)this + 0x40) + 4);
    pCompactHelper = (VsGdiCompactResourceGeometryHelperFields *)((char *)this + iCompactDelta);
    pCompactHelper->m_pVtable = &g_VSGDI_CompactResourceGeometryHelperConstructionVtable;
    ConstructHelperGroup1RowBuffer((char *)this + 0x44, 0);
    ConstructHelperGroup0RowBuffer((char *)this + 0x98, 0);
    m_nReservedEE = 0;
    m_ChildTargetLinks.m_pTail = 0;
    m_nReservedEC = 0;
    m_ChildTargetLinks.m_pHead = 0;
    m_ChildTargetLinks.m_cNodes = 0;
    *(void **)this = &g_VSGDI_ResourceGeometryHelperTargetVtable;
    *(void **)((char *)this + 0x44) = &g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable;
    *(void **)((char *)this + 0x98) = &g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable;
    pCompactHelper->m_pVtable = &g_VSGDI_CompactResourceGeometryHelperSubobjectVtable;
    pCompactHelper->m_nThisAdjust = iCompactDelta - 0x51c;
    pCompactHelper->m_pLinkedTarget = pWrappedTarget;
    pCompactHelper->m_fForwardingEnabled = 1;
    pCompactHelper->m_nQueuedState = 0;
    pCompactHelper->m_nReserved78 = 0;

    pChildList = (VsGdiResourceGeometryLinkList *)((char *)pWrappedTarget + 0x528);
    pLink = (VsGdiResourceGeometryLinkNode *)AllocateVSMemBlock(0xc);
    if (pLink != 0) {
        pLink->m_pTarget = this;
        pLink->m_pPrev = 0;
        pLink->m_pNext = pChildList->m_pHead;
    }
    if (pChildList->m_pHead != 0)
        pChildList->m_pHead->m_pPrev = pLink;
    pChildList->m_pHead = pLink;
    if (pChildList->m_pTail == 0)
        pChildList->m_pTail = pLink;
    ++pChildList->m_cNodes;

    if (g_pResourceGeometryListHead == 0) {
        g_pResourceGeometryListHead = AllocateVSMemBlock(0xc);
        if (g_pResourceGeometryListHead != 0) {
            pGlobalList = (VsGdiResourceGeometryLinkList *)g_pResourceGeometryListHead;
            pGlobalList->m_pTail = 0;
            pGlobalList->m_pHead = 0;
            pGlobalList->m_cNodes = 0;
        }
    }
    pGlobalList = (VsGdiResourceGeometryLinkList *)g_pResourceGeometryListHead;
    pLink = (VsGdiResourceGeometryLinkNode *)AllocateVSMemBlock(0xc);
    if (pLink != 0) {
        pLink->m_pTarget = this;
        pLink->m_pPrev = 0;
        pLink->m_pNext = pGlobalList->m_pHead;
    }
    if (pGlobalList->m_pHead != 0)
        pGlobalList->m_pHead->m_pPrev = pLink;
    pGlobalList->m_pHead = pLink;
    if (pGlobalList->m_pTail == 0)
        pGlobalList->m_pTail = pLink;
    ++pGlobalList->m_cNodes;

    *(short *)((char *)this + iCompactDelta + 0x6c) = 1;
    *(int *)((char *)this + 0xf0) = 0;
    m_pBackingBitmap = 0;
    m_pDisplayBinding = 0;
    *(int *)((char *)this + iCompactDelta + 0x80) = 0;
    InitializeCriticalSection(m_abCriticalSection);
    m_fCriticalSectionReady = 1;

    TrackerExtent.m_x = 8;
    TrackerExtent.m_y = 8;
    TrackerExtent.m_cx = 0;
    TrackerExtent.m_cy = 0;
    CellExtent.m_x = 8;
    CellExtent.m_y = 8;
    CellExtent.m_cx = 0;
    CellExtent.m_cy = 0;
    pTracker = (VsGdiQuantizedHelperUploadRectTracker *)AllocateVSMemBlock(0x4c);
    if (pTracker != 0) {
        pTracker->InitializeQuantizedHelperUploadRectTracker(
            pWrappedTarget == g_pResourceGeometryHelperTarget ? 0x1000 : 0,
            *(int *)&TrackerExtent,
            *(int *)&CellExtent);
    }
    *(void **)((char *)this + 0x550) = pTracker;

    if (pWrappedTarget == g_pResourceGeometryHelperTarget) {
        VsGdiDisplayStateDispatch *pDisplayDispatch =
            (VsGdiDisplayStateDispatch *)g_pDisplayState;
        if (pDisplayDispatch != 0) {
            (void)pDisplayDispatch->IsReady();
            m_pDisplayBinding = pDisplayDispatch->CreateDisplayBinding();
        }
    }
    ((VsGdiHelperSurface *)this)->UpdateWorkingRectAndBacking(pRect);
    return this;
}

// FUNCTION: LEMBALL 0x0046c380
void BuildAndApplyLogPalette(unsigned int *paOutputColors,
                             void *pPaletteResource,
                             int,
    const unsigned int *paFallbackColors)

{
    VsGdiLogPalette256 Palette;
    VsGdiLogPalette256::VsGdiPaletteEntry *pEntry;
    const unsigned char *pSource;
    unsigned int uColor;
    void *hDc;
    int cColors;
    int i;

    hDc = GetDC((HWND)0);
    if ((GetSystemPaletteEntries(hDc, 0, 10, Palette.m_aEntries) |
         GetSystemPaletteEntries(hDc, 0xf6, 10,
                                 Palette.m_aEntries + 0xf6)) == 0) {
        for (i = 0; i < 10; ++i) {
            pEntry = Palette.m_aEntries + i;
            pEntry->m_bRed = g_aabVSGDIFallbackSystemColors[i][0];
            pEntry->m_bGreen = g_aabVSGDIFallbackSystemColors[i][1];
            pEntry->m_bBlue = g_aabVSGDIFallbackSystemColors[i][2];

            pEntry = Palette.m_aEntries + 0xf6 + i;
            pEntry->m_bRed = g_aabVSGDIFallbackSystemColors[10 + i][0];
            pEntry->m_bGreen = g_aabVSGDIFallbackSystemColors[10 + i][1];
            pEntry->m_bBlue = g_aabVSGDIFallbackSystemColors[10 + i][2];
        }
    }
    if (hDc != 0) {
        ReleaseDC((HWND)0, hDc);
    }

    Palette.m_nVersion = 0x300;
    Palette.m_cEntries = 0x100;
    for (i = 0; i < 10; ++i) {
        pEntry = Palette.m_aEntries + i;
        paOutputColors[i] =
            ((unsigned int)pEntry->m_bRed << 16) |
            ((unsigned int)pEntry->m_bGreen << 8) |
            pEntry->m_bBlue;
        pEntry->m_bFlags = 0;

        pEntry = Palette.m_aEntries + 0xf6 + i;
        paOutputColors[0xf6 + i] =
            ((unsigned int)pEntry->m_bRed << 16) |
            ((unsigned int)pEntry->m_bGreen << 8) |
            pEntry->m_bBlue;
        pEntry->m_bFlags = 0;
    }

    for (i = 0; i < 2; ++i) {
        paOutputColors[10 + i] =
            ((unsigned int)g_aabVSGDIReservedOutputColors[i][0] << 16) |
            ((unsigned int)g_aabVSGDIReservedOutputColors[i][1] << 8) |
            g_aabVSGDIReservedOutputColors[i][2];
    }

    if (pPaletteResource == 0) {
        if (paFallbackColors == 0) {
            for (i = 12; i < 0xf6; ++i) {
                unsigned char bColor;

                bColor = (unsigned char)-i;
                pEntry = Palette.m_aEntries + i;
                pEntry->m_bRed = bColor;
                pEntry->m_bGreen = bColor;
                pEntry->m_bBlue = bColor;
                pEntry->m_bFlags = 1;
                paOutputColors[i] =
                    ((unsigned int)bColor << 16) |
                    ((unsigned int)bColor << 8) | bColor;
            }
        } else {
            for (i = 12; i < 0xf6; ++i) {
                uColor = paFallbackColors[i];
                pEntry = Palette.m_aEntries + i;
                pEntry->m_bRed = (unsigned char)(uColor >> 16);
                pEntry->m_bGreen = (unsigned char)(uColor >> 8);
                pEntry->m_bBlue = (unsigned char)uColor;
                pEntry->m_bFlags = 1;
                paOutputColors[i] = uColor & 0xffffff;
            }
        }
    } else {
        cColors = *(int *)((char *)pPaletteResource + 0x48) - 10;
        if (cColors > 0xf6) {
            cColors = 0xec;
        }
        if (cColors > 12) {
            cColors -= 12;
            pSource = *(const unsigned char **)((char *)pPaletteResource + 0x38) +
                      12 * 4;
            for (i = 12; cColors != 0; ++i, --cColors, pSource += 4) {
                pEntry = Palette.m_aEntries + i;
                pEntry->m_bRed = pSource[0];
                pEntry->m_bGreen = pSource[1];
                pEntry->m_bBlue = pSource[2];
                pEntry->m_bFlags = 1;
                paOutputColors[i] =
                    ((unsigned int)pSource[0] << 16) |
                    ((unsigned int)pSource[1] << 8) | pSource[2];
            }
        }
    }

    ((VsGdiDisplayPaletteDispatch *)g_pDisplayState)
        ->ApplySystemPalette(&Palette);
}

// FUNCTION: LEMBALL 0x0046c5d0
VsGdiResourceGeometryHelperTargetView *VsGdiResourceGeometryHelperTargetView::ConstructResourceGeometryHelperTarget(int nWrappedParam, int fConstructCompactHelper) {
    int iCompactDelta;
    VsGdiCompactResourceGeometryHelperFields *pCompactHelper;
    VsGdiHelperTargetParamWrapper *pParamWrapper;
    VsGdiResourceGeometryHelperTargetView *pTarget;

    pTarget = this;
    if (fConstructCompactHelper != 0) {
        *(void **)((char *)pTarget + 0x40) = &g_VSGDI_CompactHelperTargetAdjustor;
        *(void **)((char *)pTarget + 0x48) = &g_VSGDI_HelperGroup1Adjustor;
        *(void **)((char *)pTarget + 0x9c) = &g_VSGDI_HelperGroup0Adjustor;
        ((VsGdiCompactResourceGeometryHelper *)((char *)pTarget + 0x55c))
            ->InitializeCompactResourceGeometryHelper();
    }
    InitializeResourceGeometryRowBuffer(pTarget);
    *(void **)pTarget = &g_VSGDI_ResourceGeometryHelperTargetConstructionVtable;
    iCompactDelta = *(int *)((char *)(unsigned long)*(int *)((char *)pTarget + 0x40) + 4);
    pCompactHelper = (VsGdiCompactResourceGeometryHelperFields *)((char *)pTarget + iCompactDelta);
    pCompactHelper->m_pVtable = &g_VSGDI_CompactResourceGeometryHelperConstructionVtable;
    ConstructHelperGroup1RowBuffer((char *)pTarget + 0x44, 0);
    ConstructHelperGroup0RowBuffer((char *)pTarget + 0x98, 0);
    pTarget->m_nReservedEE = 0;
    pTarget->m_ChildTargetLinks.m_pTail = 0;
    pTarget->m_nReservedEC = 0;
    pTarget->m_ChildTargetLinks.m_pHead = 0;
    pTarget->m_ChildTargetLinks.m_cNodes = 0;
    *(void **)pTarget = &g_VSGDI_ResourceGeometryHelperTargetVtable;
    *(void **)((char *)pTarget + 0x44) = &g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable;
    *(void **)((char *)pTarget + 0x98) = &g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable;
    pCompactHelper->m_pVtable = &g_VSGDI_CompactResourceGeometryHelperSubobjectVtable;
    pCompactHelper->m_nThisAdjust = iCompactDelta - 0x51c;

    pParamWrapper = (VsGdiHelperTargetParamWrapper *)AllocateVSMemBlock(0xc);
    if (pParamWrapper == 0) {
        pTarget->m_pDisplayBinding = 0;
    } else {
        pParamWrapper->m_pVtable =
            (void **)&g_VSGDI_ResourceGeometryHelperTargetParamWrapperVtable;
        pParamWrapper->m_nWrappedParam = nWrappedParam;
        pParamWrapper->m_nReserved = 0;
        pTarget->m_pDisplayBinding = pParamWrapper;
    }
    pTarget->m_pBackingBitmap = 0;
    pTarget->m_nActiveUploadState = 0;
    pCompactHelper->m_pLinkedTarget = 0;
    InitializeCriticalSection(pTarget->m_abCriticalSection);
    pTarget->m_fCriticalSectionReady = 1;
    pCompactHelper->m_fForwardingEnabled = 0;
    return pTarget;
}

// FUNCTION: LEMBALL 0x0046c710
void LEMBALL_FASTCALL DestroyResourceGeometryHelperTarget(void *pvTarget) {
    int iCompactDelta;
    int iCriticalSectionReady;
    unsigned int pFollowingNode;
    VsGdiCompactResourceGeometryHelperFields *pCompactHelper;
    VsGdiResourceGeometryLinkList *pGlobalList;
    VsGdiResourceGeometryLinkList *pLinkedTargetList;
    VsGdiResourceGeometryLinkNode *pLink;
    VsGdiResourceGeometryLinkNode *pNextNode;
    VsGdiResourceGeometryLinkNode *pPreviousLink;
    VsGdiResourceGeometryHelperTargetView *pLinkedTarget;
    VsGdiResourceGeometryHelperTargetView *pTarget;

    pTarget = (VsGdiResourceGeometryHelperTargetView *)pvTarget;
    *(void **)pTarget = &g_VSGDI_ResourceGeometryHelperTargetVtable;
    *(void **)((char *)pTarget + 0x44) = &g_VSGDI_ResourceGeometryHelperGroup1RowBufferVtable;
    *(void **)((char *)pTarget + 0x98) = &g_VSGDI_ResourceGeometryHelperGroup0RowBufferVtable;
    iCompactDelta = *(int *)((char *)(unsigned long)*(int *)((char *)pTarget + 0x40) + 4);
    pCompactHelper = (VsGdiCompactResourceGeometryHelperFields *)((char *)pTarget + iCompactDelta);
    pCompactHelper->m_pVtable = &g_VSGDI_CompactResourceGeometryHelperSubobjectVtable;
    pCompactHelper->m_nThisAdjust = iCompactDelta - 0x51c;

    iCriticalSectionReady = pTarget->m_fCriticalSectionReady;
    if (iCriticalSectionReady != 0) {
        EnterCriticalSection(pTarget->m_abCriticalSection);
    }
    if (pTarget->m_pBackingBitmap != 0 && g_pDisplayState != 0) {
        ((void (LEMBALL_FASTCALL *)(void *, int, int, int))
             (*(void ***)g_pDisplayState)[0x28 / sizeof(void *)])(
            g_pDisplayState, 0,
            (int)(unsigned long)pTarget->m_pDisplayBinding,
            (int)(unsigned long)pTarget->m_pBackingBitmap);
        ((void (LEMBALL_FASTCALL *)(void *, int, int))
             (*(void ***)g_pDisplayState)[0x14 / sizeof(void *)])(
            g_pDisplayState, 0,
            (int)(unsigned long)pTarget->m_pBackingBitmap);
        pTarget->m_pBackingBitmap = 0;
    }
    if (pCompactHelper->m_pLinkedTarget == g_pResourceGeometryHelperTarget) {
        ReleaseHelperGroup0BackingBuffer((int)(unsigned long)((char *)pTarget + 0x98));
        ReleaseHelperGroup1BackingBuffer((int)(unsigned long)((char *)pTarget + 0x44));
    }
    if (pTarget->m_pDisplayBinding != 0 && g_pDisplayState != 0 &&
        (*(void ***)g_pDisplayState)[2] != 0) {
        ((void (LEMBALL_FASTCALL *)(void *, int, int))
             (*(void ***)g_pDisplayState)[8 / sizeof(void *)])(
            g_pDisplayState, 0,
            (int)(unsigned long)pTarget->m_pDisplayBinding);
        pTarget->m_pDisplayBinding = 0;
    }
    if (pTarget->m_nActiveUploadState != 0) {
        ResetHelperUploadRectListAndStateMap(pTarget->m_nActiveUploadState);
        FreeVSMemBlock((void *)(unsigned long)pTarget->m_nActiveUploadState);
        pTarget->m_nActiveUploadState = 0;
    }
    pLinkedTarget = (VsGdiResourceGeometryHelperTargetView *)pCompactHelper->m_pLinkedTarget;
    if (pLinkedTarget != 0) {
        pLinkedTargetList = &pLinkedTarget->m_ChildTargetLinks;
        pLink = pLinkedTargetList->m_pTail;
        while (pLink != 0) {
            if (pLink->m_pTarget == pvTarget) {
                break;
            }
            pLink = pLink->m_pPrev;
        }
        if (pLink != 0) {
            pPreviousLink = pLink->m_pPrev;
            pNextNode = pLink->m_pNext;
            FreeVSMemBlock(pLink);
            if (pPreviousLink == 0) {
                pLinkedTargetList->m_pHead = pNextNode;
            } else {
                pPreviousLink->m_pNext = pNextNode;
            }
            if (pNextNode == 0) {
                pLinkedTargetList->m_pTail = pPreviousLink;
            } else {
                pNextNode->m_pPrev = pPreviousLink;
            }
            pLinkedTargetList->m_cNodes = pLinkedTargetList->m_cNodes - 1;
        }
        pCompactHelper->m_pLinkedTarget = 0;
    }
    if (iCriticalSectionReady != 0) {
        LeaveCriticalSection(pTarget->m_abCriticalSection);
        DeleteCriticalSection(pTarget->m_abCriticalSection);
        pTarget->m_fCriticalSectionReady = 0;
        pGlobalList = (VsGdiResourceGeometryLinkList *)g_pResourceGeometryListHead;
        if (pGlobalList != 0) {
            pLink = pGlobalList->m_pTail;
            while (pLink != 0) {
                if (pLink->m_pTarget == pvTarget) {
                    break;
                }
                pLink = pLink->m_pPrev;
            }
            if (pLink != 0) {
                pPreviousLink = pLink->m_pPrev;
                pNextNode = pLink->m_pNext;
                FreeVSMemBlock(pLink);
                if (pPreviousLink == 0) {
                    pGlobalList->m_pHead = pNextNode;
                } else {
                    pPreviousLink->m_pNext = pNextNode;
                }
                if (pNextNode == 0) {
                    pGlobalList->m_pTail = pPreviousLink;
                } else {
                    pNextNode->m_pPrev = pPreviousLink;
                }
                pGlobalList->m_cNodes = pGlobalList->m_cNodes - 1;
            }
            if (pGlobalList->m_cNodes == 0) {
                pNextNode = pGlobalList->m_pTail;
                while (pNextNode != 0) {
                    pFollowingNode = (unsigned int)(unsigned long)pNextNode->m_pPrev;
                    FreeVSMemBlock((void *)(unsigned long)pNextNode);
                    pNextNode = (VsGdiResourceGeometryLinkNode *)(unsigned long)pFollowingNode;
                }
                FreeVSMemBlock(pGlobalList);
                g_pResourceGeometryListHead = 0;
            }
        }
    }
    pNextNode = pTarget->m_ChildTargetLinks.m_pTail;
    if (pNextNode != 0) {
        do {
            pFollowingNode = (unsigned int)(unsigned long)pNextNode->m_pPrev;
            FreeVSMemBlock((void *)(unsigned long)pNextNode);
            pNextNode = (VsGdiResourceGeometryLinkNode *)(unsigned long)pFollowingNode;
        } while (pNextNode != 0);
    }
    DestroyHelperGroup0RowBuffer((char *)pTarget + 0x98);
    DestroyHelperGroup1RowBuffer((char *)pTarget + 0x44);
    DestroyResourceGeometryRowBuffer(pTarget);
}

// FUNCTION: LEMBALL 0x0046cb20
void VsGdiResourceGeometryHelperTargetView::MarkOrForwardHelperTargetDirtyRect(short *pRect) {
    char *pTarget;
    char *pCompactHelper;
    short *pOrigin;
    short *pTail;
    short aForwardedRect[4];
    VsGdiResourceGeometryHelperTargetView *pLinkedTarget;

    pCompactHelper = (char *)this + GetHelperSurfaceVariableBlockOffset((VsGdiHelperSurface *)this);
    pLinkedTarget = *(VsGdiResourceGeometryHelperTargetView **)(pCompactHelper + 0x60);
    if (pLinkedTarget != g_pResourceGeometryHelperTarget &&
        *(int *)(pCompactHelper + 0x74) != 0 &&
        *(int *)(pCompactHelper + 0x70) != 0) {
        pTarget = (char *)this + 0x50 + GetHelperSurfaceVariableBlockOffset((VsGdiHelperSurface *)this);
        pOrigin = (short *)pTarget;
        aForwardedRect[0] = pRect[0];
        aForwardedRect[1] = pRect[1];
        pTail = pRect + 2;
        aForwardedRect[2] = pTail[0] + pOrigin[0];
        aForwardedRect[3] = pTail[1] + pOrigin[1];
        ((VsGdiResourceGeometryHelperTargetView *)pLinkedTarget)
            ->MarkOrForwardHelperTargetDirtyRect(aForwardedRect);
        return;
    }

    ((VsGdiQuantizedHelperUploadRectTracker *)
         *(void **)((char *)this + 0x550))
        ->MarkHelperUploadCellsForRect(pRect);
}

// FUNCTION: LEMBALL 0x0046cbd0
int LEMBALL_FASTCALL GetResourceGeometryHelperTargetActiveUploadState(void *pTarget) {
    return *(int *)((char *)pTarget + 0x550);
}

// FUNCTION: LEMBALL 0x0046cda0
void VsGdiResourceGeometryHelperTargetView::
FlushResourceGeometryHelperTargetUpdates(
    VsGdiResourceGeometryHelperTargetView *pPeerTarget)

{
    char *pVariableBlock;
    VsGdiResourceGeometryHelperTargetView *pLinkedTarget;
    VsGdiQuantizedHelperUploadRectTracker *pTracker;
    short *pEntry;
    short *pOrigin;
    short aDestinationRect[4];
    int nVariableBlockOffset, nRectCount, iRect;
    short nScale;
    typedef int (LEMBALL_FASTCALL *IntNoArgProc)(void *, int);
    typedef void (LEMBALL_FASTCALL *RectProc)(void *, int, short *);
    typedef int (LEMBALL_FASTCALL *PaletteProc)(void *, int, void *);

    ++g_nDebugFlushCount;

    nVariableBlockOffset =
        *(int *)((char *)*(void **)((char *)this + 0x40) + 4);
    pVariableBlock = (char *)this + nVariableBlockOffset;
    pLinkedTarget = *(VsGdiResourceGeometryHelperTargetView **)(
        pVariableBlock + 0x60);
    if (pLinkedTarget == g_pResourceGeometryHelperTarget) {
        ++g_nDebugRootFlushCount;
        if (m_pBackingBitmap == 0)
            return;
        EnterCriticalSection((char *)this + 0x534);
        EnterCriticalSection((char *)pPeerTarget + 0x534);
        ((PaletteProc)(*(void ***)g_pDisplayState)[12])(
            g_pDisplayState, 0, pPeerTarget->m_pDisplayBinding);
        pTracker = (VsGdiQuantizedHelperUploadRectTracker *)
            (unsigned long)m_nActiveUploadState;
        if ((int)*(short *)(pVariableBlock + 0x44) *
                (int)*(short *)(pVariableBlock + 0x46) > 0) {
            pTracker->MarkHelperUploadRectWithState(
                (short *)(pVariableBlock + 0x44), 0);
        }
        iRect = 0;
        nRectCount = pTracker->BuildHelperUploadRectList();
        g_nDebugLastRectCount = nRectCount;
        while (iRect < nRectCount) {
            pEntry = pTracker->GetHelperUploadRectEntryByIndex(iRect);
            aDestinationRect[0] = pEntry[0];
            aDestinationRect[1] = pEntry[1];
            aDestinationRect[2] = pEntry[2];
            aDestinationRect[3] = pEntry[3];
            if (g_fFullscreenDisplayModeSuspended == 0) {
                aDestinationRect[2] =
                    (short)(aDestinationRect[2] + m_nReservedEC);
                aDestinationRect[3] =
                    (short)(aDestinationRect[3] + m_nReservedEE);
            }
            pOrigin = (short *)(pVariableBlock + 0x5c);
            aDestinationRect[2] =
                (short)(aDestinationRect[2] + pOrigin[0]);
            aDestinationRect[3] =
                (short)(aDestinationRect[3] + pOrigin[1]);
            nScale = *(short *)(pVariableBlock + 0x6c);
            if (nScale != 1) {
                aDestinationRect[0] = (short)(aDestinationRect[0] * nScale);
                aDestinationRect[1] = (short)(aDestinationRect[1] * nScale);
                aDestinationRect[2] = (short)(aDestinationRect[2] * nScale);
                aDestinationRect[3] = (short)(aDestinationRect[3] * nScale);
            }
            g_pDisplayState->DispatchSplitRectAtBackingRowBoundary(
                pPeerTarget->m_pDisplayBinding, aDestinationRect,
                m_pDisplayBinding, pEntry, this);
            ++iRect;
            nRectCount = pTracker->BuildHelperUploadRectList();
        }
        GdiFlush();
        if (((IntNoArgProc)(*(void ***)(pVariableBlock + 0x40))[15])(
                pVariableBlock + 0x40, 0) != 0) {
            iRect = 0;
            nRectCount = pTracker->BuildHelperUploadRectList();
            while (iRect < nRectCount) {
                pEntry = pTracker->GetHelperUploadRectEntryByIndex(iRect);
                ((RectProc)(*(void ***)(m_abHelperGroup0))[2])(
                    m_abHelperGroup0, 0, pEntry);
                ++iRect;
                nRectCount = pTracker->BuildHelperUploadRectList();
            }
        }
        LeaveCriticalSection((char *)this + 0x534);
        LeaveCriticalSection((char *)pPeerTarget + 0x534);
        return;
    }
    if (*(int *)(pVariableBlock + 0x74) != 0)
        return;
    if (*(int *)(pVariableBlock + 0x78) != 0) {
        ((RectProc)(*(void ***)pLinkedTarget)[1])(
            pLinkedTarget, 0, (short *)(pVariableBlock + 0x54));
        *(int *)(pVariableBlock + 0x78) = 0;
        return;
    }
    if (*(int *)(pVariableBlock + 0x70) != 0) {
        pTracker = (VsGdiQuantizedHelperUploadRectTracker *)(unsigned long)
            ((IntNoArgProc)(*(void ***)this)[2])(this, 0);
        if (pTracker->BuildHelperUploadRectList() -
                pTracker->GetHelperUploadActiveRectStartIndex() > 0) {
            ((RectProc)(*(void ***)pLinkedTarget)[1])(
                pLinkedTarget, 0, (short *)(pVariableBlock + 0x54));
        }
    }
}

// FUNCTION: LEMBALL 0x0046d040
void LEMBALL_FASTCALL ApplyHelperTargetPaletteAndPropagate(
    void *pCompactTarget, int, void *pPalette)

{
    const unsigned int *paFallbackColors;

    paFallbackColors =
        ((VsGdiDisplayPaletteDispatch *)g_pDisplayState)
                ->UsesExistingPaletteAsFallback()
            ? g_adwGlobalResourceGeometryPaletteTable
            : 0;
    BuildAndApplyLogPalette(g_adwGlobalResourceGeometryPaletteTable,
                            pPalette,
                            0,
                            paFallbackColors);
    PropagateGlobalPaletteToResourceGeometries(
        (char *)pCompactTarget - 0x55c);
}

// FUNCTION: LEMBALL 0x0046d090
void VsGdiHelperSurface::UpdateWorkingRectAndBacking(const VsGdiRect *pRect) {
    short aRectBuffer[4];
    VsGdiHelperSurfaceVariableBlock *pLinkedVariableBlock;
    VsGdiHelperSurfaceVariableBlock *pVariableBlock;
    short *paRequestedRect;
    short *paWorkingRect;
    short *paTargetWorkingRect;
    short *paBackingObject;
    void *pBackingBitmap;
    void *pDisplayBitmapInfo;
    VsGdiHelperSurface *pLinkedTarget;
    short sWidth;
    short sHeight;
    int nVariableBlockOffset;
    int nBitmapStride;
    int nBitmapOrigin;
    void **pBackingVtable;
    VsGdiDisplayStateDispatch *pDisplayDispatch;
    const short *paRectWords;
    const short *paTailRect;

    EnterCriticalSection((char *)this + 0x534);

    nVariableBlockOffset = GetHelperSurfaceVariableBlockOffset(this);
    pVariableBlock = (VsGdiHelperSurfaceVariableBlock *)((char *)this + nVariableBlockOffset);
    paRectWords = (const short *)pRect;

    paRequestedRect = pVariableBlock->m_aRequestedRect;
    paRequestedRect[0] = paRectWords[0];
    paRequestedRect[1] = paRectWords[1];
    paTailRect = paRectWords + 2;
    if (paRectWords == 0) {
        paTailRect = 0;
    }
    paRequestedRect[2] = paTailRect[0];
    paRequestedRect[3] = paTailRect[1];

    paWorkingRect = pVariableBlock->m_aWorkingRect;
    paWorkingRect[0] = paRectWords[0];
    paWorkingRect[1] = paRectWords[1];
    if (paRectWords != 0) {
        paTailRect = paRectWords + 2;
    } else {
        paTailRect = 0;
    }
    paWorkingRect[2] = paTailRect[0];
    paWorkingRect[3] = paTailRect[1];

    pLinkedTarget = (VsGdiHelperSurface *)pVariableBlock->m_pLinkedTarget;
    if (g_pResourceGeometryHelperTarget != (void *)pLinkedTarget) {
        pLinkedVariableBlock = (VsGdiHelperSurfaceVariableBlock *)((char *)pLinkedTarget + GetHelperSurfaceVariableBlockOffset(pLinkedTarget));
        paTargetWorkingRect = pLinkedVariableBlock->m_aWorkingRect;
        sWidth = paTargetWorkingRect[0];
        sHeight = paTargetWorkingRect[1];

        if (pVariableBlock->m_aWorkingRect[2] < 0) {
            pVariableBlock->m_aWorkingRect[0] = (short)(pVariableBlock->m_aWorkingRect[0] + pVariableBlock->m_aWorkingRect[2]);
            pVariableBlock->m_aWorkingRect[2] = 0;
        }
        if (sWidth < (short)(pVariableBlock->m_aWorkingRect[0] + pVariableBlock->m_aWorkingRect[2])) {
            pVariableBlock->m_aWorkingRect[0] = (short)(sWidth - pVariableBlock->m_aWorkingRect[2]);
        }
        if (pVariableBlock->m_aWorkingRect[3] < 0) {
            pVariableBlock->m_aWorkingRect[1] = (short)(pVariableBlock->m_aWorkingRect[1] + pVariableBlock->m_aWorkingRect[3]);
            pVariableBlock->m_aWorkingRect[3] = 0;
        }
        if (sHeight < (short)(pVariableBlock->m_aWorkingRect[1] + pVariableBlock->m_aWorkingRect[3])) {
            pVariableBlock->m_aWorkingRect[1] = (short)(sHeight - pVariableBlock->m_aWorkingRect[3]);
        }
        if (pVariableBlock->m_aWorkingRect[0] <= 0 || pVariableBlock->m_aWorkingRect[1] <= 0) {
            pVariableBlock->m_aWorkingRect[1] = 0;
            pVariableBlock->m_aWorkingRect[0] = 0;
            pVariableBlock->m_aWorkingRect[3] = 0;
            pVariableBlock->m_aWorkingRect[2] = 0;
        }

        paWorkingRect = pVariableBlock->m_aWorkingRect;
        pVariableBlock->m_aBackingRect[0] = paWorkingRect[0];
        pVariableBlock->m_aBackingRect[1] = paWorkingRect[1];
        ComputeBackingDimensions(aRectBuffer, paWorkingRect, (int)paWorkingRect[0]);
        *(int *)((char *)this + 0x524) = 0;
        EnsureHelperBackingRowIndexCapacity(this);
        return;
    }

    *(unsigned short *)&pVariableBlock->m_aWorkingRect[0] = (unsigned short)(pVariableBlock->m_aWorkingRect[0] + 3U) & 0xfffc;
    pVariableBlock->m_aBackingRect[0] = pVariableBlock->m_aWorkingRect[0];
    pVariableBlock->m_aBackingRect[1] = pVariableBlock->m_aWorkingRect[1];

    paWorkingRect = pVariableBlock->m_aWorkingRect;
        ComputeBackingDimensions(aRectBuffer, paWorkingRect, pVariableBlock->m_nBackingWidthHint);
    sWidth = aRectBuffer[0];
    sHeight = aRectBuffer[1];

    if (*(int *)((char *)this + 0xf0) != 0) {
        pDisplayDispatch = (VsGdiDisplayStateDispatch *)g_pDisplayState;
        pDisplayDispatch->DeleteDisplayBitmap(*(int *)((char *)this + 0xf8),
                                               (void *)(unsigned long)*(int *)((char *)this + 0xf0));
        pDisplayDispatch->ReleaseDisplayBitmap(
            (void *)(unsigned long)*(int *)((char *)this + 0xf0));
        *(int *)((char *)this + 0xf0) = 0;
    }

    if (pVariableBlock->m_aWorkingRect[0] == 0 || pVariableBlock->m_aWorkingRect[1] == 0) {
        *(int *)((char *)this + 0x524) = 0;
    } else {
        if (*(int *)((char *)this + 0xf0) == 0) {
            pDisplayDispatch = (VsGdiDisplayStateDispatch *)g_pDisplayState;
            pDisplayDispatch->InitializeDisplayBitmapInfo((char *)this + 0xfc);
            *(short *)((char *)this + 0x108) = 1;
            *(int *)((char *)this + 0x100) = (int)sWidth;
            *(int *)((char *)this + 0x104) = (int)sHeight * *(int *)((char *)this + 0x104);
            *(int *)((char *)this + 0x10c) = 0;
            *(int *)((char *)this + 0x110) = 0;
            *(int *)((char *)this + 0x114) = 0;
            *(int *)((char *)this + 0x118) = 0;
            *(int *)((char *)this + 0x11c) = 0;
            *(int *)((char *)this + 0x120) = 0;
            pDisplayBitmapInfo = (char *)this + 0xfc;
            *(int *)pDisplayBitmapInfo = 0x28;
            *(short *)((char *)this + 0x10a) = 8;
            pDisplayDispatch = (VsGdiDisplayStateDispatch *)g_pDisplayState;
            pBackingBitmap = pDisplayDispatch->CreateDisplayBitmap(
                *(int *)((char *)this + 0xf8), pDisplayBitmapInfo);
            *(void **)((char *)this + 0xf0) = pBackingBitmap;
            if (pBackingBitmap != 0) {
                pDisplayDispatch->BindDisplayBitmap(*(int *)((char *)this + 0xf8),
                                                     (void *)pBackingBitmap);
                *(int *)((char *)this + 0x524) =
                    (int)pVariableBlock->m_aWorkingRect[0] * (int)pVariableBlock->m_aWorkingRect[1];
            }
        }

        if (*(int *)((char *)this + 0xf0) == 0) {
            TriggerReleaseAssertFailure("AllocatedBitmap", "VSGDI.CPP", 0x2e0);
        }

        paBackingObject = (short *)(unsigned long)*(int *)((char *)this + 0xf0);
        pBackingVtable = *(void ***)paBackingObject;
        nBitmapOrigin = ((VsGdiDisplayBitmapDispatch *)paBackingObject)->Origin();
        nBitmapStride = ((VsGdiDisplayBitmapDispatch *)paBackingObject)->Bits();
        ConfigureBackingStrideAndOrigin(nBitmapStride, nBitmapOrigin);
        PromoteHelperUploadStateToActive(*(int *)((char *)this + 0x550));

        aRectBuffer[0] = 0;
        aRectBuffer[1] = 0;
        aRectBuffer[2] = 0;
        aRectBuffer[3] = 0;
        aRectBuffer[0] = pVariableBlock->m_aRequestedRect[0];
        aRectBuffer[1] = pVariableBlock->m_aRequestedRect[1];
        ((VsGdiResourceGeometryHelperTargetView *)this)
            ->MarkOrForwardHelperTargetDirtyRect(aRectBuffer);
    }

    LeaveCriticalSection((char *)this + 0x534);
}

// FUNCTION: LEMBALL 0x0046d420
void LEMBALL_FASTCALL SetHelperTargetRenderExtent(
    void *pCompactTarget, int, const VsGdiRect *pExtent)

{
    VsGdiResourceGeometryLinkNode *pNode;
    VsGdiResourceGeometryHelperTargetView *pChild;
    VsGdiRect Rect;
    void *pChildCompact;
    void **pVtable;
    void *pTracker;
    int nCompactDelta;
    int nExtent;

    nCompactDelta = *(int *)((char *)*(void **)((char *)pCompactTarget - 0x51c) + 4);
    Rect.m_x = pExtent->m_x;
    Rect.m_y = pExtent->m_y;
    Rect.m_cx = *(short *)((char *)pCompactTarget + 0x10);
    Rect.m_cy = *(short *)((char *)pCompactTarget + 0x12);

    pTracker = *(void **)((char *)pCompactTarget - 0x0c);
    if (pTracker != 0) {
        nExtent = (unsigned short)Rect.m_x |
                  ((unsigned int)(unsigned short)Rect.m_y << 16);
        SetHelperUploadTrackerExtent(pTracker, 0, nExtent);
    }

    ((VsGdiHelperSurface *)((char *)pCompactTarget - 0x55c))
        ->UpdateWorkingRectAndBacking(&Rect);

    if (*(void **)((char *)pCompactTarget + nCompactDelta - 0x4fc) ==
        g_pResourceGeometryHelperTarget) {
        pVtable = *(void ***)pCompactTarget;
        if (((int (LEMBALL_FASTCALL *)(void *, int))pVtable[15])(
                pCompactTarget, 0) != 0) {
            EnsureHelperGroup0BackingBufferThunk(
                (char *)pCompactTarget - 0x4c4);
        }
        if (((int (LEMBALL_FASTCALL *)(void *, int))pVtable[16])(
                pCompactTarget, 0) != 0) {
            EnsureHelperGroup1BackingBuffer(
                (char *)pCompactTarget - 0x518);
        }
    }

    pNode = *(VsGdiResourceGeometryLinkNode **)(
        (char *)pCompactTarget - 0x34);
    while (pNode != 0) {
        pChild = (VsGdiResourceGeometryHelperTargetView *)pNode->m_pTarget;
        nCompactDelta = *(int *)((char *)*(void **)((char *)pChild + 0x40) + 4);
        pChildCompact = (char *)pChild + 0x40 + nCompactDelta;
        Rect.m_x = *(short *)((char *)pChild + nCompactDelta + 0x4c);
        Rect.m_y = *(short *)((char *)pChild + nCompactDelta + 0x4e);
        pVtable = *(void ***)pChildCompact;
        ((void (LEMBALL_FASTCALL *)(void *, int, const VsGdiRect *))
             pVtable[10])(pChildCompact, 0, &Rect);
        pNode = pNode->m_pPrev;
    }
}

// FUNCTION: LEMBALL 0x0046d7e0
void LEMBALL_FASTCALL BindHelperTargetDisplayDc(void *pTarget, int, void *hDc) {
    void *pDisplayBinding;

    pDisplayBinding = *(void **)((char *)pTarget + 0xf8);
    ((VsGdiDisplayDcBindingInterface *)pDisplayBinding)->BindDisplayDc(hDc);
}

struct VsGdiBitmapInfoHeaderView {
    unsigned int m_cbSize00;
    int m_cx04;
    int m_cy08;
    unsigned short m_cPlanes0C;
    unsigned short m_cBitsPerPixel0E;
};

// FUNCTION: LEMBALL 0x0046d800
void VsGdiHelperSurface::CopyClippedBitmapIntoHelperBufferAndNotify(
    const void *pBitmapInfo, const void *pBitmapBits)
{
    const VsGdiBitmapInfoHeaderView *pHeader;
    unsigned char *pSourceRow;
    unsigned char **ppDestinationRows;
    short *pWorkingSize;
    short aRect[4];
    int nVariableBlockOffset;
    int nCopyWidth;
    int nCopyHeight;
    int nSourceStride;
    int iRow;

    if (*(int *)((char *)this + 0x550) == 0) {
        return;
    }

    EnterCriticalSection((char *)this + 0x534);
    pHeader = (const VsGdiBitmapInfoHeaderView *)pBitmapInfo;
    nVariableBlockOffset = *(int *)(*(char **)((char *)this + 0x40) + 4);
    pWorkingSize = (short *)((char *)this + nVariableBlockOffset + 0x54);
    nCopyWidth = pWorkingSize[0] <= pHeader->m_cx04
        ? pWorkingSize[0] : pHeader->m_cx04;
    nCopyHeight = pWorkingSize[1] <= pHeader->m_cy08
        ? pWorkingSize[1] : pHeader->m_cy08;
    nSourceStride =
        ((pHeader->m_cBitsPerPixel0E * pHeader->m_cx04 + 0x1f) & ~0x1f) / 8;
    pSourceRow = (unsigned char *)pBitmapBits +
        (pHeader->m_cy08 - 1) * nSourceStride;
    ppDestinationRows = *(unsigned char ***)((char *)this + 4);

    for (iRow = 0; iRow < nCopyHeight; ++iRow) {
        memcpy(ppDestinationRows[iRow], pSourceRow, nCopyWidth);
        nCopyHeight = pWorkingSize[1] <= pHeader->m_cy08
            ? pWorkingSize[1] : pHeader->m_cy08;
        pSourceRow -= nSourceStride;
    }

    aRect[0] = pWorkingSize[0];
    aRect[1] = pWorkingSize[1];
    aRect[2] = 0;
    aRect[3] = 0;
    InitializeHelperUploadStatePending(*(int *)((char *)this + 0x550));
    ((void (LEMBALL_FASTCALL *)(void *, int, short *))(*(void ***)this)[1])(
        this, 0, aRect);
    LeaveCriticalSection((char *)this + 0x534);
}

// FUNCTION: LEMBALL 0x0046d920
void LEMBALL_FASTCALL PropagateGlobalPaletteToResourceGeometries(void *pTarget) {
    PropagateGlobalPaletteToResourceGeometriesBody(pTarget);
}

// FUNCTION: LEMBALL 0x0046d930
void LEMBALL_FASTCALL PropagateGlobalPaletteToResourceGeometriesBody(void *pTarget) {
    VsGdiSystemPalettePayload Palette;
    VsGdiResourceGeometryLinkNode *pNode;
    VsGdiResourceGeometryHelperTargetView *pListedTarget;
    unsigned char *pEntry;
    unsigned int uColor;
    int i;

    Palette.m_nVersion = 0x300;
    Palette.m_cEntries = 0x100;
    for (i = 0; i < 0x100; ++i) {
        uColor = g_adwGlobalResourceGeometryPaletteTable[i];
        pEntry = Palette.m_abEntries[i];
        pEntry[0] = (unsigned char)(uColor >> 16);
        pEntry[1] = (unsigned char)(uColor >> 8);
        pEntry[2] = (unsigned char)uColor;
        pEntry[3] = 4;
    }

    ((VsGdiDisplayPaletteDispatch *)g_pDisplayState)
        ->ApplySystemPalette(&Palette);

    pNode = g_pResourceGeometryListHead != 0
                ? *(VsGdiResourceGeometryLinkNode **)g_pResourceGeometryListHead
                : 0;
    while (pNode != 0) {
        pListedTarget =
            (VsGdiResourceGeometryHelperTargetView *)pNode->m_pTarget;
        memcpy((char *)pTarget + 0x124,
               g_adwGlobalResourceGeometryPaletteTable,
               sizeof(g_adwGlobalResourceGeometryPaletteTable));
        if (pListedTarget->m_pDisplayBinding != 0) {
            ((VsGdiDisplayPaletteDispatch *)g_pDisplayState)
                ->ApplySurfacePalette(pListedTarget->m_pDisplayBinding,
                                      0,
                                      0x100,
                                      g_adwGlobalResourceGeometryPaletteTable);
        }
        pNode = pNode->m_pPrev;
    }
}

// FUNCTION: LEMBALL 0x0046d9f0
int RefreshHelperTargetBindingIfChanged(void *pTarget) {
    typedef int (LEMBALL_FASTCALL *BindingReadyProc)(void *pBinding);
    typedef int (LEMBALL_FASTCALL *BindingAddressProc)(void *pBinding);
    int *pnCachedBindingValue;
    VsGdiHelperSurfaceVariableBlock *pVariableBlock;
    void *pBindingSource;
    VsGdiHelperSurface *pLinkedTarget;
    int *pBackingRow;
    int nBindingValue;

    if (*(int *)((char *)pTarget + 4) == 0) {
        return 0;
    }

    pVariableBlock =
        (VsGdiHelperSurfaceVariableBlock *)((char *)pTarget + GetHelperSurfaceVariableBlockOffset((const VsGdiHelperSurface *)pTarget));
    pnCachedBindingValue = (int *)((char *)pTarget + 0xc);
    pLinkedTarget = (VsGdiHelperSurface *)pVariableBlock->m_pLinkedTarget;
    if ((void *)pLinkedTarget == g_pResourceGeometryHelperTarget) {
        pBindingSource = *(void **)((char *)pTarget + 0xf0);
        if (pBindingSource == 0) {
            return 0;
        }
        if (((BindingReadyProc)(*(void ***)pBindingSource)[1])(
                pBindingSource) == 0) {
            return 0;
        }
        nBindingValue =
            ((BindingAddressProc)(*(void ***)pBindingSource)[3])(
                pBindingSource);
        if (nBindingValue != 0 && *pnCachedBindingValue != nBindingValue) {
            *pnCachedBindingValue = nBindingValue;
            EnsureHelperBackingRowIndexCapacity((VsGdiHelperSurface *)pTarget);
            return 1;
        }
    } else {
        if (pLinkedTarget == 0) {
            return 0;
        }
        if (RefreshHelperTargetBindingIfChanged(pLinkedTarget) == 0) {
            return 0;
        }
        pBackingRow = *(int **)(unsigned long)(*(int *)((char *)pLinkedTarget + 4) + pVariableBlock->m_aWorkingRect[3] * 4);
        nBindingValue = (int)(unsigned long)(
            (char *)pBackingRow + pVariableBlock->m_aWorkingRect[2]);
        if (nBindingValue != *pnCachedBindingValue) {
            EnsureHelperBackingRowIndexCapacity((VsGdiHelperSurface *)pTarget);
            return 1;
        }
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0046daa0
void InvokeRootHelperTargetPostQueueCallback(int nTarget) {
    typedef void (LEMBALL_FASTCALL *PostQueueProc)(void *pBinding);
    void *pBindingSource;
    VsGdiHelperSurfaceVariableBlock *pVariableBlock;
    VsGdiHelperSurface *pRootTarget;
    VsGdiHelperSurface *pTarget;

    pTarget = (VsGdiHelperSurface *)(unsigned long)nTarget;
    do {
        pRootTarget = pTarget;
        pVariableBlock =
            (VsGdiHelperSurfaceVariableBlock *)((char *)pRootTarget + GetHelperSurfaceVariableBlockOffset(pRootTarget));
        pTarget = (VsGdiHelperSurface *)pVariableBlock->m_pLinkedTarget;
    } while (g_pResourceGeometryHelperTarget != (void *)pTarget);

    pBindingSource = *(void **)((char *)pRootTarget + 0xf0);
    ((PostQueueProc)(*(void ***)pBindingSource)[2])(pBindingSource);
}

// FUNCTION: LEMBALL 0x0046dba0
__declspec(naked) void LEMBALL_FASTCALL FillHelperSurfaceRectCommandAdjustmentThunk(
    void *pCompactTarget, int nUnused, void *pCommand)
{
    __asm {
        sub ecx, dword ptr [ecx - 4]
        jmp FillHelperSurfaceRectCommand
    }
}

// FUNCTION: LEMBALL 0x0046dbb0
__declspec(naked) void LEMBALL_FASTCALL ForwardBitmapCommandToAdjustedSurfaceThunk(void)
{
    __asm {
        sub ecx, dword ptr [ecx - 4]
        jmp offset ForwardBitmapCommandToAdjustedSurface
    }
}

// FUNCTION: LEMBALL 0x0046dbc0
void LEMBALL_FASTCALL ForwardBitmapCommandToAdjustedSurface(
    void *pCompactTarget, int, void *pCommand, void *pBitmap)

{
    typedef void (LEMBALL_FASTCALL *BitmapCommandVirtual)(
        void *, int, void *, void *);

    int nSurfaceOffset;
    void *pSurface;

    nSurfaceOffset =
        *(int *)((char *)*(void **)((char *)pCompactTarget - 0x51c) + 4);
    pSurface = (char *)pCompactTarget + nSurfaceOffset - 0x51c;
    ((BitmapCommandVirtual)(*(void ***)pSurface)[8])(
        pSurface, 0, pCommand, pBitmap);
}

// FUNCTION: LEMBALL 0x0046dbf0
__declspec(naked) void LEMBALL_FASTCALL BlitBitmapRowsToScanlinesAdjustmentThunk(
    void *pCompactTarget, int nUnused, void *pCommand, void *pBitmap)
{
    __asm {
        sub ecx, dword ptr [ecx - 4]
        jmp offset BlitBitmapRowsToScanlines
    }
}

// FUNCTION: LEMBALL 0x0046dc10
__declspec(naked) void LEMBALL_FASTCALL SetHelperTargetRenderExtentAdjustmentThunk(
    void *pCompactTarget, int nUnused, const VsGdiRect *pExtent)
{
    __asm {
        sub ecx, dword ptr [ecx - 4]
        jmp offset SetHelperTargetRenderExtent
    }
}

// FUNCTION: LEMBALL 0x0046dc30
__declspec(naked) void LEMBALL_FASTCALL ApplyHelperTargetPaletteAdjustmentThunk(
    void *pCompactTarget, int nUnused, void *pPalette)
{
    __asm {
        sub ecx, dword ptr [ecx - 4]
        jmp offset ApplyHelperTargetPaletteAndPropagate
    }
}

// FUNCTION: LEMBALL 0x0046dc40
void LEMBALL_FASTCALL PresentCompactResourceGeometryHelperAdjustmentThunk(
    void *pCompactTarget)

{
    pCompactTarget = (char *)pCompactTarget -
        *(int *)((char *)pCompactTarget - 4);
    PresentCompactResourceGeometryHelper(pCompactTarget);
}

// FUNCTION: LEMBALL 0x0046dc50
void LEMBALL_FASTCALL PresentCompactResourceGeometryHelper(
    void *pCompactTarget)

{
    struct SlotManagerDispatch {
        virtual void Reserved00(void) = 0;
        virtual void Reserved04(void) = 0;
        virtual void Reserved08(void) = 0;
        virtual void DeactivateResourceGeometryHelperSlot(void *pTarget) = 0;
    };
    void *pTarget;
    pTarget = (char *)pCompactTarget - 0x55c;
    ((SlotManagerDispatch *)g_pResourceGeometryHelperSlotManager)
        ->DeactivateResourceGeometryHelperSlot(pTarget);
}

// FUNCTION: LEMBALL 0x0046dc70
unsigned char *LEMBALL_FASTCALL GetHelperTargetTokenAdjustmentThunk(
    void *pCompactTarget)

{
    pCompactTarget =
        (char *)pCompactTarget - *(int *)((char *)pCompactTarget - 4);
    return GetHelperTargetToken(pCompactTarget);
}

// FUNCTION: LEMBALL 0x0046dc80
unsigned char *LEMBALL_FASTCALL GetHelperTargetToken(void *pTarget) {
    return (unsigned char *)pTarget - 8;
}

// FUNCTION: LEMBALL 0x0046dc90
int LEMBALL_FASTCALL GetHelperGroup0DirtyFlagAdjustmentThunk(
    void *pCompactTarget, int nUnused)

{
    return GetHelperGroup0DirtyFlag(
        (char *)pCompactTarget - 0x46c, nUnused);
}

// FUNCTION: LEMBALL 0x0046dca0
int LEMBALL_FASTCALL GetHelperGroup1DirtyFlagAdjustmentThunk(
    void *pCompactTarget, int nUnused)

{
    return GetHelperGroup1DirtyFlag(
        (char *)pCompactTarget - 0x4c0, nUnused);
}

// FUNCTION: LEMBALL 0x0046dd60
void VsGdiAnimWindowOwner::BindAsSingletonAnimWindow(void) {
    m_nA0 = 0;
    m_nA4 = 0;
    m_nA8 = 0;
    m_nAC = 0;
    m_hMciWindowB8 = 0;
    if (g_pSingletonAnimWindowOwner != 0) {
        MessageBoxA(0, "Unable to support more than one Anim Window",
                    "!!ERROR!!", 0x1000);
        LongjmpStartupExceptionFrame(0xaaaa);
    }
    g_pSingletonAnimWindowOwner = this;
}

// FUNCTION: LEMBALL 0x0046ddc0
void *VsGdiAnimWindowOwner::ConstructResourceGeometryHelperWindowOwner(void) {
    void **pBaseVtable;
    union {
        void (VsGdiAnimWindowOwner::*pMethod)(void);
        void *pAddress;
    } rebuildMethod;
    int i;

    ConstructWindowOwnerRenderContext(this);
    pBaseVtable = *(void ***)this;
    m_MediaName98.ConstructDynamicCString();
    m_nReserved94 = 0;
    m_nReserved90 = 0;
    m_MediaPathB0.ConstructDynamicCString();
    if (g_fVSGDIAnimWindowOwnerVtableInitialized == 0) {
        for (i = 0; i < 50; ++i) {
            g_VSGDI_AnimWindowOwnerVtable[i] = pBaseVtable[i];
        }
        rebuildMethod.pMethod =
            &VsGdiAnimWindowOwner::RebuildResourceGeometryHelperWindow;
        g_VSGDI_AnimWindowOwnerVtable[6] = rebuildMethod.pAddress;
        g_fVSGDIAnimWindowOwnerVtableInitialized = 1;
    }
    *(void ***)this = g_VSGDI_AnimWindowOwnerVtable;
    BindAsSingletonAnimWindow();
    return this;
}

void *LEMBALL_FASTCALL ConstructResourceGeometryHelperWindowOwner(
    void *pOwner)
{
    return ((VsGdiAnimWindowOwner *)pOwner)
        ->ConstructResourceGeometryHelperWindowOwner();
}

// FUNCTION: LEMBALL 0x0046ded0
void VsGdiAnimWindowOwner::RebuildResourceGeometryHelperWindow(void) {
    typedef HWND (*MciWndCreateProc)(
        HWND, HINSTANCE, DWORD, const char *);
    HMODULE hMsvfw32;
    MciWndCreateProc pMciWndCreate;

    BuildGeometryHelperFromRenderRect(this);
    if (m_hMciWindowB8 != 0) {
        SendMessageA(m_hMciWindowB8, 0x10, 0, 0);
        m_hMciWindowB8 = 0;
    }
    hMsvfw32 = LoadLibraryA("MSVFW32.DLL");
    pMciWndCreate = hMsvfw32 != 0
        ? (MciWndCreateProc)GetProcAddress(hMsvfw32, "MCIWndCreateA")
        : 0;
    m_hMciWindowB8 = pMciWndCreate != 0
        ? pMciWndCreate(*(HWND *)((char *)this + 0x44),
                        g_hApplicationInstance, 0x50001f0a, 0)
        : 0;
    SendMessageA(m_hMciWindowB8, 0x499, 0,
                 (LPARAM)m_MediaPathB0.m_pszText);
}

struct VsGdiMciDrawCallbackParameters {
    DWORD m_dwCallback00;
    DWORD m_dwFlags04;
    void *m_pDrawProc08;
    DWORD m_adwReserved0C[4];
};

// FUNCTION: LEMBALL 0x0046dfe0
void VsGdiAnimWindowOwner::InstallEmbeddedMciDrawCallback(void) {
    VsGdiMciDrawCallbackParameters Parameters;
    MCIDEVICEID nDeviceId;

    Parameters.m_pDrawProc08 = (void *)DispatchVisualSciencesWingDriverMessage;
    Parameters.m_dwFlags04 = 0x8000;
    nDeviceId = (MCIDEVICEID)SendMessageA(
        (HWND)m_hMciWindowB8, 0x464, 0, 0);
    if (nDeviceId != 0 &&
        mciSendCommandA(nDeviceId, 0x876, 0x1100000,
                        (DWORD_PTR)&Parameters) != 0) {
        MessageBoxA(0, "Unable to set MCI Draw Procedure",
                    "MCI ERROR", 0x10);
    }
}

// FUNCTION: LEMBALL 0x0046e300
void VsGdiAnimWindowOwner::StartEmbeddedMciWindowPlaybackOnce(void) {
    if (m_nA4 == 0) {
        SendMessageA(m_hMciWindowB8, 0x807, 0, -1);
        SendMessageA(m_hMciWindowB8, 0x806, 0, 0);
        m_nA4 = 1;
    }
}

void LEMBALL_FASTCALL StartEmbeddedMciWindowPlaybackOnce(void *pOwner) {
    ((VsGdiAnimWindowOwner *)pOwner)->StartEmbeddedMciWindowPlaybackOnce();
}

// FUNCTION: LEMBALL 0x0046e3c0
void VsGdiAnimWindowOwner::ResumeEmbeddedMciWindowPlayback(void) {
    if (m_nA8 != 0) {
        SendMessageA(m_hMciWindowB8, 0x855, 0, 0);
        m_nA8 = 0;
    }
}

void LEMBALL_FASTCALL ResumeEmbeddedMciWindowPlayback(void *pOwner) {
    ((VsGdiAnimWindowOwner *)pOwner)->ResumeEmbeddedMciWindowPlayback();
}

struct VsGdiMediaListResourceView {
    void **m_pVtable00;
    int m_nReserved04;
    int m_cLocks08;
    int m_nReserved0C;
    int m_nLoadState10;
    unsigned char m_abReserved14[0x10];
    int m_nReserved24;
    unsigned char m_abReserved28[0x50];
    char *m_pTextEntry78;
};

void VsGdiAnimWindowOwner::UpdateMediaPathFromListResource(int nResourceId) {
    VsGdiMediaListResourceView *pResource;
    const char *pszMediaName;
    const char *pszBasePath;
    GameDynamicCString MediaPath;
    GameDynamicCString TemporaryPath;

    pszMediaName = "test";
    pResource = 0;
    if (nResourceId != 0) {
        m_nAC = nResourceId;
        pResource = (VsGdiMediaListResourceView *)
            LoadTwoArrayListResource(nResourceId);
        if (pResource->m_nLoadState10 == 0) {
            ((void (LEMBALL_FASTCALL *)(void *, int))
                pResource->m_pVtable00[7])(pResource, 0);
        } else {
            pResource->m_nReserved24 = 0;
        }
        ++pResource->m_cLocks08;
        pszMediaName = *(char **)(pResource->m_pTextEntry78 + 0x38);
    }

    MediaPath.ConstructDynamicCString();
    if (m_nReserved90 != 0) {
        MediaPath.AssignDynamicCStringFromDynamic(&m_MediaName98);
        if (MediaPath.m_pszText[GetDynamicCStringLength(&MediaPath) - 1] != '\\') {
            MediaPath.AppendDynamicCStringAndCopyResult(&TemporaryPath, "\\");
            TemporaryPath.DestroyDynamicCString();
        }
    }
    MediaPath.AppendDynamicCStringAndCopyResult(&TemporaryPath, pszMediaName);
    TemporaryPath.DestroyDynamicCString();
    MediaPath.AppendDynamicCStringAndCopyResult(&TemporaryPath, ".avi");
    TemporaryPath.DestroyDynamicCString();

    if (m_nReserved94 == 0) {
        m_MediaPathB0.AssignDynamicCString(
            g_abProcessCurrentDirectoryBuffer);
        if (m_MediaPathB0.m_pszText[
                GetDynamicCStringLength(&m_MediaPathB0) - 1] != '\\') {
            m_MediaPathB0.AppendDynamicCStringAndCopyResult(
                &TemporaryPath, "\\");
            TemporaryPath.DestroyDynamicCString();
        }
    } else {
        pszBasePath = FindCdromFilePathBySuffix(MediaPath.m_pszText);
        if (pszBasePath == 0) {
            pszBasePath = g_abProcessCurrentDirectoryBuffer;
        }
        m_MediaPathB0.AssignDynamicCString(pszBasePath);
        if (m_MediaPathB0.m_pszText[
                GetDynamicCStringLength(&m_MediaPathB0) - 1] != '\\') {
            m_MediaPathB0.AppendDynamicCStringAndCopyResult(
                &TemporaryPath, "\\");
            TemporaryPath.DestroyDynamicCString();
        }
    }
    m_MediaPathB0.AppendDynamicCStringObjectAndCopyResult(
        &TemporaryPath, &MediaPath);
    TemporaryPath.DestroyDynamicCString();
    if (pResource != 0) {
        --pResource->m_cLocks08;
        ReleaseTypedResourceObjectReference(pResource);
    }
    if (m_hMciWindowB8 != 0) {
        SendMessageA(m_hMciWindowB8, 0x499, 0,
                     (LPARAM)m_MediaPathB0.m_pszText);
    }
    m_nA0 = 1;
    MediaPath.DestroyDynamicCString();
}

void VsGdiAnimWindowOwner::SelectIntroSequenceMediaPath(void) {
    int nMode;

    if (*(int *)((char *)this + 0xc4) == 0) {
        UpdateMediaPathFromListResource(0x116);
        return;
    }
    nMode = *(int *)((char *)g_pLevelProgressState + 8);
    if (nMode == 0 || nMode == 3) {
        UpdateMediaPathFromListResource(0x117);
    } else if (nMode == 1 || nMode == 2) {
        UpdateMediaPathFromListResource(0x118);
    }
}

void LEMBALL_FASTCALL SelectIntroSequenceMediaPath(void *pOwner) {
    ((VsGdiAnimWindowOwner *)pOwner)->SelectIntroSequenceMediaPath();
}

// FUNCTION: LEMBALL 0x00472290
void *LEMBALL_FASTCALL InitializeResourceGeometryRowBuffer(void *pvRowBuffer) {
    int *pRowBuffer;

    pRowBuffer = (int *)pvRowBuffer;
    *(short *)((char *)pRowBuffer + 0x2e) = 0;
    *(short *)(pRowBuffer + 0xb) = 0;
    *(short *)((char *)pRowBuffer + 0x32) = 0;
    *(short *)(pRowBuffer + 0xc) = 0;
    *(short *)((char *)pRowBuffer + 0x36) = 0;
    *(short *)(pRowBuffer + 0xd) = 0;
    *pRowBuffer = (int)(unsigned long)&g_VSGDI_ResourceGeometryRowBufferVtable;
    *(short *)((char *)pRowBuffer + 0x3a) = 0;
    *(short *)(pRowBuffer + 0xe) = 0;
    *(short *)((char *)pRowBuffer + 0x3e) = 0;
    *(short *)(pRowBuffer + 0xf) = 0;
    ClearResourceGeometryRowBufferFields(pRowBuffer);
    return pRowBuffer;
}

// FUNCTION: LEMBALL 0x004722d0
void LEMBALL_FASTCALL DestroyResourceGeometryRowBuffer(void *pvRowBuffer) {
    *(void **)pvRowBuffer = &g_VSGDI_ResourceGeometryRowBufferVtable;
    ReleaseResourceGeometryRowBufferStorage((int)(unsigned long)pvRowBuffer);
}

// FUNCTION: LEMBALL 0x00472310
void LEMBALL_FASTCALL ClearResourceGeometryRowBufferFields(void *pvRowBuffer) {
    char *pRowBuffer;

    pRowBuffer = (char *)pvRowBuffer;
    *(int *)(pRowBuffer + 0x10) = 0;
    *(int *)(pRowBuffer + 0x08) = 0;
    *(int *)(pRowBuffer + 0x0c) = 0;
    *(int *)(pRowBuffer + 0x18) = 0;
    *(int *)(pRowBuffer + 0x14) = 0;
    *(int *)(pRowBuffer + 0x04) = 0;
    *(int *)(pRowBuffer + 0x1c) = 0;
    *(int *)(pRowBuffer + 0x24) = 0;
    *(int *)(pRowBuffer + 0x20) = 0;
    *(int *)(pRowBuffer + 0x28) = 0;
}

// FUNCTION: LEMBALL 0x00472340
void LEMBALL_FASTCALL EnsureHelperBackingRowIndexCapacity(VsGdiHelperSurface *pSurface) {
    int cRows;
    int cbRowIndexBytes;

    cRows = (int)*(short *)((char *)pSurface + 0x2e);
    if (*(int *)((char *)pSurface + 0x28) < cRows) {
        if (*(int *)((char *)pSurface + 4) != 0) {
            FreeVSMemBlock((void *)(unsigned long)*(int *)((char *)pSurface + 4));
            *(int *)((char *)pSurface + 4) = 0;
        }
        if (0 < *(short *)((char *)pSurface + 0x2e)) {
            cbRowIndexBytes = cRows << 2;
            *(int *)((char *)pSurface + 4) = (int)(unsigned long)AllocateVSMemBlock((unsigned int)cbRowIndexBytes);
            pSurface->ClearBackingBorderRows();
        }
        *(int *)((char *)pSurface + 0x28) = cRows;
        return;
    }
    pSurface->ClearBackingBorderRows();
}

// FUNCTION: LEMBALL 0x004723a0
void VsGdiHelperSurface::ClearBackingBorderRows(void) {
    unsigned int cbBorderRow;
    unsigned int cDwords;
    unsigned int cTailBytes;
    char *pbBackingBase;
    char *pbTarget;
    int nAbsoluteStride;
    int nStrideSign;

    *(int *)((char *)this + 0x18) = 0;
    *(int *)((char *)this + 0x14) = 0;
    RebuildResourceGeometryRowPointerTable(this);

    cbBorderRow = (unsigned int)*(int *)((char *)this + 0x20);
    if (0 < (int)cbBorderRow) {
        pbBackingBase = (char *)(unsigned long)*(int *)((char *)this + 0xc);
        pbTarget = pbBackingBase;
        cDwords = cbBorderRow >> 2;
        while (cDwords != 0) {
            *(unsigned int *)pbTarget = 0;
            pbTarget = pbTarget + 4;
            --cDwords;
        }
        cTailBytes = cbBorderRow & 3;
        while (cTailBytes != 0) {
            *pbTarget = 0;
            ++pbTarget;
            --cTailBytes;
        }

        nAbsoluteStride = *(int *)((char *)this + 0x1c);
        nStrideSign = nAbsoluteStride >> 0x1f;
        nAbsoluteStride = (nAbsoluteStride ^ nStrideSign) - nStrideSign;
        pbTarget = pbBackingBase + nAbsoluteStride * (int)*(short *)((char *)this + 0x2e) + cbBorderRow;
        cDwords = cbBorderRow >> 2;
        while (cDwords != 0) {
            *(unsigned int *)pbTarget = 0;
            pbTarget = pbTarget + 4;
            --cDwords;
        }
        cTailBytes = cbBorderRow & 3;
        while (cTailBytes != 0) {
            *pbTarget = 0;
            ++pbTarget;
            --cTailBytes;
        }
    }
}

// FUNCTION: LEMBALL 0x00472400
void LEMBALL_FASTCALL RebuildResourceGeometryRowPointerTable(VsGdiHelperSurface *pSurface) {
    int nRowByteOffset;
    int iRowIndex;
    int iLogicalRow;

    nRowByteOffset = *(int *)((char *)pSurface + 8);
    iLogicalRow = *(int *)((char *)pSurface + 0x14);
    iRowIndex = 0;
    if (0 < *(short *)((char *)pSurface + 0x2e)) {
        do {
            *(int *)(*(int *)((char *)pSurface + 4) + iLogicalRow * 4) =
                *(int *)((char *)pSurface + 0x18) + nRowByteOffset;
            ++iLogicalRow;
            nRowByteOffset = nRowByteOffset + *(int *)((char *)pSurface + 0x1c);
            if ((int)*(short *)((char *)pSurface + 0x2e) <= iLogicalRow) {
                iLogicalRow = iLogicalRow - (int)*(short *)((char *)pSurface + 0x2e);
            }
            ++iRowIndex;
        } while (iRowIndex < (int)*(short *)((char *)pSurface + 0x2e));
    }
}

// FUNCTION: LEMBALL 0x004725f0
void VsGdiHelperSurface::ComputeBackingDimensions(short *paOut, short *paRect, int nWidth) {
    VsGdiHelperSurface *pSurface = this;
    short cxRect;
    short cyRect;
    int cBorderRows;

    cxRect = paRect[0];
    *(short *)((char *)pSurface + 0x2c) = cxRect;
    cyRect = paRect[1];
    *(short *)((char *)pSurface + 0x2e) = cyRect;
    if (nWidth == 0 || *(int *)((char *)pSurface + 0x10) != 0) {
        *(int *)((char *)pSurface + 0x20) = 0;
    } else {
        *(int *)((char *)pSurface + 0x20) = nWidth - cxRect;
    }
    if (cxRect == 0) {
        *(int *)((char *)pSurface + 0x24) = 0;
    } else {
        *(int *)((char *)pSurface + 0x24) =
            (cxRect - 1 + *(int *)((char *)pSurface + 0x20) * 2) / (int)cxRect;
    }
    cBorderRows = *(int *)((char *)pSurface + 0x24);
    paOut[0] = cxRect;
    paOut[1] = (short)cBorderRows + cyRect;
}

// FUNCTION: LEMBALL 0x00472670
void VsGdiHelperSurface::ConfigureBackingStrideAndOrigin(int nStride, int nOrigin) {
    VsGdiHelperSurface *pSurface = this;
    int nRowBase;

    nRowBase = *(int *)((char *)pSurface + 0x20) + nStride;
    *(int *)((char *)pSurface + 0xc) = nStride;
    *(int *)((char *)pSurface + 8) = nRowBase;
    *(int *)((char *)pSurface + 0x1c) = nOrigin;
    if (nOrigin < 0) {
        *(int *)((char *)pSurface + 8) =
            (1 - (int)*(short *)((char *)pSurface + 0x2e)) * nOrigin + nRowBase;
    }
    EnsureHelperBackingRowIndexCapacity(pSurface);
}

// FUNCTION: LEMBALL 0x004726b0
void VsGdiHelperSurface::SplitRectAtBackingRowBoundary(short *pRect, short **ppFirstRect, short **ppSecondRect) {
    short *pFirstRect;
    short *pSecondRect;
    int yBoundary;
    pFirstRect = (short *)((char *)this + 0x30);
    pFirstRect[0] = pRect[0];
    pFirstRect[1] = pRect[1];
    pFirstRect[2] = pRect[2];
    pFirstRect[3] = pRect[3];
    *ppFirstRect = pFirstRect;
    *ppSecondRect = 0;
    yBoundary = *(int *)((char *)this + 0x14);
    if (yBoundary < (short)(pRect[1] + pRect[3]) &&
        pRect[3] < yBoundary) {
        pFirstRect[1] = (short)yBoundary - pFirstRect[3];
        pSecondRect = (short *)((char *)this + 0x38);
        pSecondRect[0] = pRect[0];
        pSecondRect[1] = (short)(pRect[1] - yBoundary + pRect[3]);
        pSecondRect[2] = pRect[2];
        pSecondRect[3] = (short)yBoundary;
        *ppSecondRect = pSecondRect;
    }
}

// FUNCTION: LEMBALL 0x00472760
void VsGdiHelperSurface::ClearBackingBorderRowsThunk(void) {
    ClearBackingBorderRows();
}

// FUNCTION: LEMBALL 0x00474b50
void LEMBALL_FASTCALL InitializeArrowCursorState(int *pState) {
    struct CursorState {
        virtual void Reserved00();
        virtual void Reserved01();
        virtual void Reserved02();
        virtual void Reserved03();
        virtual void Reserved04();
        virtual void Reserved05();
        virtual void InitializeCursor();
    };

    pState[0x27] = 0;
    pState[0x27] = (int)(unsigned long)LoadCursorA((HINSTANCE)0, (LPCSTR)0x7f00);
    ((CursorState *)pState)->InitializeCursor();
}

// FUNCTION: LEMBALL 0x00474b80
void LEMBALL_FASTCALL InitializeArrowCursorPosition(void *pRenderClient) {
    tagPOINT Point;

    GetCursorPos(&Point);
    *(short *)((char *)pRenderClient + 0x10) = (short)Point.x;
    *(short *)((char *)pRenderClient + 0x12) = (short)Point.y;
}

// FUNCTION: LEMBALL 0x00474be0
void LEMBALL_FASTCALL HideArrowCursor(void *pRenderClient) {
    SetCursor((HCURSOR)0);
    ShowCursor(0);
    *(int *)((char *)pRenderClient + 0x3c) = 0;
}

// FUNCTION: LEMBALL 0x00474c00
void LEMBALL_FASTCALL ShowArrowCursor(void *pRenderClient) {
    ShowCursor(1);
    *(int *)((char *)pRenderClient + 0x3c) = 1;
}

// FUNCTION: LEMBALL 0x00474dd0
void LEMBALL_FASTCALL CopyRectRowsToHelperGroup0Buffer(
    void *pGroup0RowBuffer, int, void *pCopyCommand)

{
    unsigned char **ppSourceRows;
    unsigned char **ppDestinationRows;
    unsigned char *pbCommand;
    int cbRow;
    int cRows;
    int xSource;
    int ySource;
    int xDestination;
    int yDestination;
    int iRow;

    pbCommand = (unsigned char *)pCopyCommand;
    cbRow = (int)*(short *)(pbCommand + 8);
    cRows = (int)*(short *)(pbCommand + 0x0a);
    if (cbRow == 0 || cRows == 0)
        return;

    xSource = (int)*(short *)(pbCommand + 4);
    ySource = (int)*(short *)(pbCommand + 6);
    xDestination = (int)*(short *)(pbCommand + 0x0c);
    yDestination = (int)*(short *)(pbCommand + 0x0e);
    if (cRows <= 0)
        return;

    ppSourceRows = *(unsigned char ***)((char *)pGroup0RowBuffer - 0x94);
    ppDestinationRows =
        *(unsigned char ***)((char *)pGroup0RowBuffer + 0x0c);
    for (iRow = 0; iRow < cRows; ++iRow) {
        memcpy(ppDestinationRows[yDestination + iRow] + xDestination,
               ppSourceRows[ySource + iRow] + xSource,
               (unsigned int)cbRow);
    }
}

// FUNCTION: LEMBALL 0x00474e60
void LEMBALL_FASTCALL FillRectInHelperGroup0Buffer(
    void *pGroup0RowBuffer, int, void *pFillCommand)

{
    unsigned char **ppRows;
    unsigned char *pbCommand;
    int cbRow;
    int cRows;
    int xDestination;
    int yDestination;
    int nColor;
    int iRow;

    pbCommand = (unsigned char *)pFillCommand;
    cbRow = (int)*(short *)(pbCommand + 8);
    cRows = (int)*(short *)(pbCommand + 0x0a);
    if (cbRow == 0 || cRows == 0)
        return;

    nColor = *(int *)(pbCommand + 4);
    xDestination = (int)*(short *)(pbCommand + 0x0c);
    yDestination = (int)*(short *)(pbCommand + 0x0e);
    if (cRows <= 0)
        return;

    ppRows = *(unsigned char ***)((char *)pGroup0RowBuffer + 0x0c);
    for (iRow = 0; iRow < cRows; ++iRow) {
        memset(ppRows[yDestination + iRow] + xDestination,
               (unsigned char)nColor,
               (unsigned int)cbRow);
    }
}

// FUNCTION: LEMBALL 0x00474ee0
void LEMBALL_FASTCALL CopyHelperGroup0RectToTarget(
    void *pGroup0RowBuffer, int, void *pCopyRect)

{
    unsigned char **ppTargetRows;
    unsigned char **ppGroupRows;
    short *paRect;
    int cbRow;
    int cRows;
    int x;
    int y;
    int iRow;

    paRect = (short *)pCopyRect;
    cbRow = (int)paRect[0];
    cRows = (int)paRect[1];
    if (cbRow * cRows == 0)
        return;

    x = (int)paRect[2];
    y = (int)paRect[3];
    if (x + cbRow > (int)*(unsigned short *)((char *)pGroup0RowBuffer + 0x50))
        cbRow = (int)*(unsigned short *)((char *)pGroup0RowBuffer + 0x50) - x;
    if (y + cRows > (int)*(unsigned short *)((char *)pGroup0RowBuffer + 0x52))
        cRows = (int)*(unsigned short *)((char *)pGroup0RowBuffer + 0x52) - y;
    if (cRows <= 0)
        return;

    ppTargetRows = *(unsigned char ***)((char *)pGroup0RowBuffer - 0x94);
    ppGroupRows = *(unsigned char ***)((char *)pGroup0RowBuffer + 0x0c);
    for (iRow = 0; iRow < cRows; ++iRow) {
        memcpy(ppTargetRows[y + iRow] + x,
               ppGroupRows[y + iRow] + x,
               (unsigned int)cbRow);
    }
}

// FUNCTION: LEMBALL 0x00475080
void LEMBALL_FASTCALL FillHelperSurfaceRectCommand(
    void *pCompactTarget, int, void *pCommand)

{
    const VsGdiRect *pCommandRect;
    VsGdiRect Rect;

    pCommandRect = (const VsGdiRect *)get_draw_command_rect_payload(pCommand);
    Rect.m_x = pCommandRect->m_x;
    Rect.m_y = pCommandRect->m_y;
    Rect.m_cx = pCommandRect->m_cx;
    Rect.m_cy = pCommandRect->m_cy;
    ((VsGdiHelperSurface *)((char *)pCompactTarget - 0x55c))
        ->FillRect(Rect, *(int *)((char *)pCommand + 0x0c));
}

// FUNCTION: LEMBALL 0x004756e0
void VsGdiHelperSurface::FillRect(VsGdiRect Rect, int nColor) {
    typedef void (LEMBALL_FASTCALL *DirtyRectVirtual)(
        void *, int, VsGdiRect *);

    VsGdiRect ClippedRect;
    short *paRect;
    short *paClippedRect;
    unsigned char **ppRows;
    int iRow;

    memset(&ClippedRect, 0, sizeof(ClippedRect));
    paRect = (short *)&Rect;
    paClippedRect = (short *)&ClippedRect;
    if (ClipRenderRectToSurfaceBounds(&Rect, &ClippedRect)) {
        if (paClippedRect[0] < 1 || paClippedRect[1] < 1)
            return;
        paRect[0] = paClippedRect[0];
        paRect[1] = paClippedRect[1];
    }

    ppRows = *(unsigned char ***)((char *)this + 4);
    for (iRow = 0; iRow < paRect[1]; ++iRow) {
        memset(ppRows[paRect[3] + iRow] + paRect[2],
               (unsigned char)nColor,
               (unsigned int)(unsigned short)paRect[0]);
    }
    ((DirtyRectVirtual)(*(void ***)this)[1])(this, 0, &Rect);
}

// FUNCTION: LEMBALL 0x00476580
int VsGdiHelperSurface::ClipRenderRectToSurfaceBounds(
    VsGdiRect *pRect, VsGdiRect *pClippedRect)

{
    short *paRect;
    short *paClippedRect;
    short sOriginX;
    short sOriginY;
    short sRectX;
    short sRectY;
    short sRight;
    short sBottom;
    short *paSurfaceBounds;
    int nVariableBlockOffset;
    int fRightInside;

    paRect = (short *)pRect;
    paClippedRect = (short *)pClippedRect;
    nVariableBlockOffset = GetHelperSurfaceVariableBlockOffset(this);
    paSurfaceBounds = (short *)((char *)this + nVariableBlockOffset + 0x64);
    sOriginX = paSurfaceBounds[2];
    sOriginY = paSurfaceBounds[3];
    sRectX = paRect[2];
    sRectY = paRect[3];

    if ((short)(sOriginX + paSurfaceBounds[0]) < sRectX ||
        (short)(sOriginY + paSurfaceBounds[1]) < sRectY ||
        (short)(paRect[0] + sRectX) < sOriginX ||
        (short)(paRect[1] + sRectY) < sOriginY) {
        return true;
    }

    if (sRectX < sOriginX) {
        paClippedRect[2] = (short)(sOriginX - sRectX);
        paRect[2] = sOriginX;
        paRect[0] = (short)(paRect[0] - paClippedRect[2]);
    }
    if (sRectY < sOriginY) {
        paClippedRect[3] = (short)(sOriginY - sRectY);
        paRect[3] = sOriginY;
        paRect[1] = (short)(paRect[1] - paClippedRect[3]);
    }

    nVariableBlockOffset = GetHelperSurfaceVariableBlockOffset(this);
    paSurfaceBounds = (short *)((char *)this + nVariableBlockOffset + 0x64);
    sRight = (short)(paSurfaceBounds[2] + paSurfaceBounds[0]);
    fRightInside = (short)(paRect[2] + paRect[0]) <= sRight;
    if (fRightInside) {
        paClippedRect[0] = paRect[0];
    } else {
        paClippedRect[0] = (short)(sRight - paRect[2]);
        paRect[0] = paClippedRect[0];
    }

    nVariableBlockOffset = GetHelperSurfaceVariableBlockOffset(this);
    paSurfaceBounds = (short *)((char *)this + nVariableBlockOffset + 0x64);
    sBottom = (short)(paSurfaceBounds[3] + paSurfaceBounds[1]);
    if (sBottom < (short)(paRect[3] + paRect[1])) {
        paClippedRect[1] = (short)(sBottom - paRect[3]);
        paRect[1] = paClippedRect[1];
        return true;
    }
    paClippedRect[1] = paRect[1];
    return !fRightInside || sRectY < sOriginY || sRectX < sOriginX;
}

// FUNCTION: LEMBALL 0x004787f0
void LEMBALL_FASTCALL BlitBitmapRowsToScanlines(
    void *pCompactTarget, int, void *pCommand, void *pBitmap)

{
    typedef void (LEMBALL_FASTCALL *DirtyRectVirtual)(
        void *, int, VsGdiRect *);
    typedef unsigned char *(LEMBALL_FASTCALL *GetBitmapDataVirtual)(
        void *);

    unsigned char *pCommandBytes;
    unsigned char *pBitmapBytes;
    VsGdiHelperSurface *pSurface;
    VsGdiRect Rect;
    VsGdiRect ClippedRect;
    unsigned char **ppRows;
    unsigned char *pSource;
    unsigned char *pDestination;
    int cxBitmap;
    int cyBitmap;
    int xSource;
    int ySource;
    int iSourceRow;
    int iDestinationRow;
    int nDestinationRowStep;
    int i;
    unsigned int dwFlags;

    pCommandBytes = (unsigned char *)pCommand;
    pBitmapBytes = (unsigned char *)pBitmap;
    pSurface = (VsGdiHelperSurface *)((char *)pCompactTarget - 0x55c);

    Rect.m_x = *(short *)(pCommandBytes + 8);
    Rect.m_y = *(short *)(pCommandBytes + 0x0a);
    Rect.m_cx = *(short *)(pCommandBytes + 4);
    Rect.m_cy = *(short *)(pCommandBytes + 6);
    xSource = *(short *)(pCommandBytes + 0x0c);
    ySource = *(short *)(pCommandBytes + 0x0e);
    if (Rect.m_x == 0 && Rect.m_y == 0) {
        Rect.m_x = *(short *)(pBitmapBytes + 0x48);
        Rect.m_y = *(short *)(pBitmapBytes + 0x4a);
    }

    cxBitmap = *(short *)(pBitmapBytes + 0x48);
    cyBitmap = *(short *)(pBitmapBytes + 0x4a);
    if (cxBitmap * cyBitmap == 0) {
        return;
    }

    memset(&ClippedRect, 0, sizeof(ClippedRect));
    if (pSurface->ClipRenderRectToSurfaceBounds(&Rect, &ClippedRect)) {
        if (ClippedRect.m_x < 1 || ClippedRect.m_y < 1) {
            return;
        }
        Rect.m_x = ClippedRect.m_x;
        Rect.m_y = ClippedRect.m_y;
    }

    ((DirtyRectVirtual)(*(void ***)pSurface)[1])(
        pSurface, 0, &Rect);

    dwFlags = *(unsigned int *)(pCommandBytes + 0x14);
    iDestinationRow = Rect.m_cy;
    nDestinationRowStep = 1;
    if ((dwFlags & 2) != 0) {
        nDestinationRowStep = -1;
        iDestinationRow += Rect.m_y - 1;
    }

    pSource = ((GetBitmapDataVirtual)(*(void ***)pBitmap)[10])(pBitmap);
    pSource += (ySource + ClippedRect.m_cy) * cxBitmap +
               xSource + ClippedRect.m_cx;
    ppRows = *(unsigned char ***)((char *)pSurface + 4);

    for (iSourceRow = 0; iSourceRow < Rect.m_y; ++iSourceRow) {
        pDestination = ppRows[iDestinationRow] + Rect.m_cx;
        if ((dwFlags & 0x800) != 0) {
            for (i = 0; i < Rect.m_x; ++i) {
                if (pSource[i] != 0) {
                    pDestination[i] = pSource[i];
                }
            }
        } else {
            memcpy(pDestination, pSource, (unsigned short)Rect.m_x);
        }
        iDestinationRow += nDestinationRowStep;
        pSource += cxBitmap;
    }
}
