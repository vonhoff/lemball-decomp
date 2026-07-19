#include "GAME.H"
#include "ENGINE/GDI/VSWINDOW.H"
#include "SHELL/OPTIONS.H"
#include "LEVEL/DEMO.H"
#include "RESOURCE/MOGLOAD.H"
#include "ENGINE/GDI/VSGDI.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/CORE/SAFEVT.H"
#include "ENGINE/MEDIA/EFFSTRM.H"
#include "FRONTEND/CURSOR.H"
#include "RESOURCE/VARMGR.H"
#include "LEVEL/DRAWTEXT.H"

#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <string.h>
#include <stdlib.h>

extern "C" DWORD WINAPI timeGetTime(void);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void NoopVtableCallbackThunk(void);
extern void VariantResourceBundleNoopThunk(void);
void LEMBALL_FASTCALL SetTimedVariantFrameDuration(
    void *pObject, int nUnusedEdx, int nFrameDuration);
extern void LEMBALL_FASTCALL DispatchAndClearPointerQueue(void *pQueue);
extern void LEMBALL_FASTCALL PresentPrimaryContextScreen(void *pPrimaryContext);
extern void noop_callback_00456600(void);
extern int g_nLevelFrameClockTimeMs;
extern int g_nLevelFrameClockTick;

struct LevelStatusProgressClient {
    virtual void SetLoadProgress(int nProgress) = 0;
};

struct VsGdiCompactGeometryTokenDispatch {
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
    virtual unsigned char *GetToken(void) = 0;
};

struct VsNetEffDispatchEventPayloadView {
    char m_abReserved00[4];
    int m_pSerializedStream04;
    char m_abReserved08[0x1c];
    void *m_pOwnedPayload24;
};

struct VsNetEffTransportPacketHeader {
    char m_abReserved00[8];
    unsigned short m_nStreamEvent08;
    char m_abReserved0A[6];
};

struct VsNetEffDispatchEvent {
    volatile unsigned short m_nType;
    unsigned short m_anReserved02[3];
    volatile int m_nCode;
    void *m_pPeer0C;
    VsNetEffDispatchEventPayloadView *m_pPayload10;
};

struct GameBaseModeActionStream : public GameEffStream {
    int m_nActionPhase30;
};

struct GameBaseModeEventClient;

struct GameBaseModeDispatchInterface {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual int HandleTransportPayload(
        unsigned short nStreamEvent,
        VsNetEffDispatchEventPayloadView *pPayload,
        void *pPeer) = 0;
    virtual void Reserved0C(void) = 0;
    virtual int HandleTransportEvent(VsNetEffDispatchEvent *pEvent) = 0;
};

struct GameBaseModeEventClient {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual int HandleBaseModeActionButtonEvent(
        VsNetEffDispatchEvent *pEvent);

    int m_nQueueMagic04;
    int m_nQueueClientCount08;
    int m_nQueueReserved0C;
};

struct GameBaseModePrimaryBase : public GameBaseModeDispatchInterface {
    int m_nRequestedMode04;
    int m_nStatus08;
};

struct GameBaseModeObject : public GameBaseModePrimaryBase,
                             public GameBaseModeEventClient {
    int m_fNetworkLobbyActive1C;
    GameBaseModeActionStream *m_pActionStream20;
    void *m_pMainContext24;

    void QueueBaseModeUserAction(int nAction, int nPhase);
    void QueueBaseModeUserActionThunk(int nAction, int nPhase);
};

#define GameBASE_MODE_SCREEN_RESERVED(n) virtual void Reserved##n(void) = 0

struct GameBaseModeScreenInterface {
    GameBASE_MODE_SCREEN_RESERVED(00);
    GameBASE_MODE_SCREEN_RESERVED(01);
    GameBASE_MODE_SCREEN_RESERVED(02);
    GameBASE_MODE_SCREEN_RESERVED(03);
    GameBASE_MODE_SCREEN_RESERVED(04);
    GameBASE_MODE_SCREEN_RESERVED(05);
    GameBASE_MODE_SCREEN_RESERVED(06);
    GameBASE_MODE_SCREEN_RESERVED(07);
    GameBASE_MODE_SCREEN_RESERVED(08);
    GameBASE_MODE_SCREEN_RESERVED(09);
    GameBASE_MODE_SCREEN_RESERVED(0A);
    GameBASE_MODE_SCREEN_RESERVED(0B);
    GameBASE_MODE_SCREEN_RESERVED(0C);
    GameBASE_MODE_SCREEN_RESERVED(0D);
    GameBASE_MODE_SCREEN_RESERVED(0E);
    GameBASE_MODE_SCREEN_RESERVED(0F);
    GameBASE_MODE_SCREEN_RESERVED(10);
    GameBASE_MODE_SCREEN_RESERVED(11);
    GameBASE_MODE_SCREEN_RESERVED(12);
    GameBASE_MODE_SCREEN_RESERVED(13);
    GameBASE_MODE_SCREEN_RESERVED(14);
    virtual int HandleUserAction(int nAction) = 0;
};

#undef GameBASE_MODE_SCREEN_RESERVED

struct GameBaseModeScreen : public GameBaseModeScreenInterface {
    char m_abReserved04[0x8c];
    int m_fActionBusy90;

    void QueueActiveBaseModeUserAction(int nAction, int nPhase);
    void QueueActiveBaseModeUserActionThunk(int nAction, int nPhase);
    void DispatchBaseModeUserAction(int nAction, int nPhase);
    void DispatchBaseModeUserActionThunk(int nAction, int nPhase);
};

#define GameRUNTIME_RESERVED(n) virtual void Reserved##n(void) = 0

struct GameRuntimeServiceInterface {
    GameRUNTIME_RESERVED(00);
    GameRUNTIME_RESERVED(01);
    GameRUNTIME_RESERVED(02);
    GameRUNTIME_RESERVED(03);
    GameRUNTIME_RESERVED(04);
    GameRUNTIME_RESERVED(05);
    GameRUNTIME_RESERVED(06);
    GameRUNTIME_RESERVED(07);
    GameRUNTIME_RESERVED(08);
    GameRUNTIME_RESERVED(09);
    GameRUNTIME_RESERVED(0A);
    GameRUNTIME_RESERVED(0B);
    virtual void ServiceRuntime(void) = 0;
};

#undef GameRUNTIME_RESERVED

extern void *g_pActiveNetworkLobbyTransportController;
extern void *g_pActiveNetworkLobbyScreen;
extern void *g_pActiveNetworkRuntimeWindow;
extern void *g_pVariantResourceEntryManager;
extern int g_nSelectedNetworkLobbyPeerId;
extern void *g_pSharedGeometryHelper;

// GLOBAL: LEMBALL 0x0049f148
static const char g_szUnknownUserActionSpecified[] =
    "Unknown user action specified\n";
// GLOBAL: LEMBALL 0x0049f168
static const char g_szUnknownUserActionReceived[] =
    "Unknown user action received\n";

void LEMBALL_FASTCALL ResetBaseModeActionButtonPointSink(void *pObject);
void *LEMBALL_FASTCALL DestroyBaseModeActionButtonPointSinkArray(
    void *pObject, int nUnused, unsigned int fDelete);
extern void AppendPointerQueueEntry(void *pQueue, void *pEntry);
typedef void (LEMBALL_FASTCALL *LevelNoArgVirtualProc)(void *, int);
typedef void (LEMBALL_FASTCALL *LevelOneArgVirtualProc)(void *, int, void *);
typedef void (LEMBALL_FASTCALL *LevelTwoArgVirtualProc)(
    void *, int, void *, void *);
typedef void *(LEMBALL_FASTCALL *LevelDeleteArrayVirtualProc)(
    void *, int, unsigned int);
static void *DeleteQueuedRenderPointSinkWrapper00432350(void *pObject, BYTE fDelete);
static void *DeleteQueuedRenderPointSinkThunk00403580(void *pObject, BYTE fDelete);
static void *DeleteStatusIndicatorPointSinkEntry00432A90(void *pObject, BYTE fDelete);
static void *DeleteStatusIndicatorPointSinkThunk00401834(void *pObject, BYTE fDelete);
void LEMBALL_FASTCALL RestoreStatusIndicatorPointSinkEntryVtable(void *pObject);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableA(void *pObject);
static void *DestroyStatusIndicatorPointSinkArray00467BB0(void *pObject, unsigned int fDelete);
static void LEMBALL_FASTCALL QueueStatusIndicatorPointSink00432AD0(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL QueueStatusIndicatorPointSinkThunk004018AC(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL DispatchStatusIndicatorPointSink00432AE0(
    void *pObject, int nUnusedEdx, void *pContext);
static void LEMBALL_FASTCALL DispatchStatusIndicatorPointSinkThunk004025AE(
    void *pObject, int nUnusedEdx, void *pContext);
static void LEMBALL_FASTCALL UpdateStatusIndicatorLoadProgress00434D10(
    void *pObject);
static void LEMBALL_FASTCALL UpdateStatusIndicatorLoadProgressThunk00401DB1(
    void *pObject);
static void LEMBALL_FASTCALL SetStatusIndicatorLoadProgress0044B340(
    void *pObject, int nUnusedEdx, int nProgress);
static void LEMBALL_FASTCALL SetStatusIndicatorLoadProgressThunk00402BFD(
    void *pObject, int nUnusedEdx, int nProgress);
int LEMBALL_FASTCALL GetTimedVariantFrameStartTime(void *pObject);
int LEMBALL_FASTCALL GetTimedVariantFrameStartTimeThunk(void *pObject);
static void LEMBALL_FASTCALL QueueHelperLocalRectUpdateEntry00432B10(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL QueueHelperLocalRectUpdateThunk00401E6F(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL DispatchHelperLocalRectUpdateEntry00432B20(
    void *pObject, int nUnusedEdx, void *pContext);
static void LEMBALL_FASTCALL DispatchHelperLocalRectUpdateThunk004037A1(
    void *pObject, int nUnusedEdx, void *pContext);
// GLOBAL: LEMBALL 0x496ca8
void *g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage[8] = {
    (void *)DeleteQueuedRenderPointSinkThunk00403580,
    (void *)_purecall,
    (void *)_purecall,
    0,
    (void *)DeleteStatusIndicatorPointSinkThunk00401834,
    (void *)QueueHelperLocalRectUpdateThunk00401E6F,
    (void *)DispatchHelperLocalRectUpdateThunk004037A1,
    0,
};
static void *g_LEVEL_QueuedRenderPointSinkFinalizeVtable =
    g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
static void *DeleteFramedScreenRenderChildEntryB(void *pObject, BYTE fDelete);
static void *DeleteFramedScreenRenderChildEntryThunk(void *pObject, BYTE fDelete);
static void LEMBALL_FASTCALL QueueFramedScreenRenderEntry(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL QueueFramedScreenRenderEntryThunk(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL FinalizeFramedScreenRenderEntry(
    void *pObject, int nUnusedEdx, void *pManager);
static void LEMBALL_FASTCALL FinalizeFramedScreenRenderEntryThunk(
    void *pObject, int nUnusedEdx, void *pManager);
static void *DeleteBaseModeObjectAdjusted(void *pObject, BYTE fDelete);
void *DeleteBaseModeObjectAdjustedThunk(void *pObject, BYTE fDelete);
int LEMBALL_FASTCALL HandleBaseModeActionButtonEventThunk(
    void *pClient, int nUnused, void *pEvent);
static void *g_LEVEL_CompositePointRectSinkEntryInitVtableStorage[8] = {
    (void *)DeleteFramedScreenRenderChildEntryThunk,
    (void *)QueueFramedScreenRenderEntryThunk,
    (void *)FinalizeFramedScreenRenderEntryThunk,
    0,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)DeleteBaseModeObjectAdjustedThunk,
    (void *)HandleBaseModeActionButtonEventThunk,
    0,
};
static void *g_LEVEL_CompositePointRectSinkEntryInitVtable =
    g_LEVEL_CompositePointRectSinkEntryInitVtableStorage;
static void *DeleteFramedScreenRenderChildEntryA(void *pObject, BYTE fDelete);
static void *DeleteFramedScreenRenderChildEntryAThunk(void *pObject, BYTE fDelete);
static void LEMBALL_FASTCALL QueueBitmapRenderEntry(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL QueueBitmapRenderEntryThunk(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL RenderBitmapEntryViaContext(
    void *pObject, int nUnusedEdx, void *pManager);
static void LEMBALL_FASTCALL RenderBitmapEntryViaContextThunk(
    void *pObject, int nUnusedEdx, void *pManager);
static void *g_LEVEL_FramedScreenRenderChildEntryVtableStorage[8] = {
    (void *)DeleteFramedScreenRenderChildEntryAThunk,
    (void *)QueueBitmapRenderEntryThunk,
    (void *)RenderBitmapEntryViaContextThunk,
    0,
    (void *)DeleteFramedScreenRenderChildEntryThunk,
    (void *)QueueFramedScreenRenderEntryThunk,
    (void *)FinalizeFramedScreenRenderEntryThunk,
    0,
};
static void *g_LEVEL_FramedScreenRenderChildEntryVtable =
    g_LEVEL_FramedScreenRenderChildEntryVtableStorage;
void *LEMBALL_FASTCALL InitializeFramedScreenRenderChildEntryBody(
    void *pObject);
void *LEMBALL_FASTCALL InitializeFramedScreenRenderChildEntry(
    void *pObject);
void *LEMBALL_FASTCALL InitializeHelperLocalRectUpdateEntryBody(
    void *pObject);
void *LEMBALL_FASTCALL InitializeHelperLocalRectUpdateEntry(
    void *pObject);
static void *DestroyPackagedSpriteRenderEntryArray00469930(void *pObject, unsigned int fDelete);
static void LEMBALL_FASTCALL QueuePackagedSpriteRenderEntry00439800(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL QueuePackagedSpriteRenderEntryThunk00403846(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL DispatchPackagedSpriteRenderEntry00439810(
    void *pObject, int nUnusedEdx, void *pContext);
static void LEMBALL_FASTCALL DispatchPackagedSpriteRenderEntryThunk00402A8B(
    void *pObject, int nUnusedEdx, void *pContext);
static void *DeletePackagedSpriteRenderEntry00439400(void *pObject, BYTE fDelete);
static void *DeletePackagedSpriteRenderEntryThunk004016F9(void *pObject, BYTE fDelete);
void NoopPackagedSpriteRenderEntry00439370(void);
void LEMBALL_FASTCALL NoopPackagedSpriteRenderEntryThunk00402A22(
    void *pUnused);
void PASCAL NoopPackagedSpriteRenderEntry00439380(void *pUnused);
void LEMBALL_FASTCALL NoopPackagedSpriteRenderEntryThunk00401B77(
    void *pUnusedThis, int nUnusedEdx, void *pArgument);
static void *g_LEVEL_PackagedSpriteRenderEntryVtableStorage[8] = {
    (void *)DestroyPackagedSpriteRenderEntryArray00469930,
    (void *)QueuePackagedSpriteRenderEntryThunk00403846,
    (void *)DispatchPackagedSpriteRenderEntryThunk00402A8B,
    0,
    (void *)DeletePackagedSpriteRenderEntryThunk004016F9,
    (void *)NoopPackagedSpriteRenderEntryThunk00402A22,
    (void *)_purecall,
    (void *)NoopPackagedSpriteRenderEntryThunk00401B77,
};
static void *g_LEVEL_PackagedSpriteRenderEntryVtable =
    g_LEVEL_PackagedSpriteRenderEntryVtableStorage;
static void *DeleteRenderPointRectSinkEntry004396E0(void *pObject, BYTE fDelete);
void DeleteRenderPointRectSinkThunk00401898(void *pObject, BYTE fDelete);
static void LEMBALL_FASTCALL QueueRenderPointRectSinkEntry004398A0(
    void *pObject, int nUnusedEdx, void *pQueue);
void LEMBALL_FASTCALL QueueRenderPointRectSinkThunk00402DD3(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL DispatchRenderPointRectSinkEntry004398B0(
    void *pObject, int nUnusedEdx, void *pContext);
void LEMBALL_FASTCALL DispatchRenderPointRectSinkThunk00401C71(
    void *pObject, int nUnusedEdx, void *pContext);
extern void *LEMBALL_FASTCALL DeleteGeometryOwnerRect(
    void *pOwner, int nUnused, int nDeleteFlag);
extern void LEMBALL_FASTCALL GeometryOwnerButtonTransitionNoop(
    void *pOwner, int nUnused, const VS_XYPair *pPoint, int nButton);
extern void LEMBALL_FASTCALL GeometryOwnerButtonInsideNoop(
    void *pOwner, int nUnused, const VS_XYPair *pPoint, int nButton);
extern void LEMBALL_FASTCALL ClearGeometryOwnerButtonState(
    void *pOwner, int nUnused, const VS_XYPair *pPoint, int nButton);
void *LEMBALL_FASTCALL DeleteRenderPointRectContextThunk00401488(
    void *pObject, int nUnused, int fDelete);
void LEMBALL_FASTCALL ResetRenderPointRectEntryThunk00401703(
    void *pObject, int nUnused, const VS_XYPair *pPoint, int nButton);
void LEMBALL_FASTCALL ResetRenderPointRectEntryThunk0040355D(
    void *pObject, int nUnused, const VS_XYPair *pPoint, int nButton);
void LEMBALL_FASTCALL ClearRenderPointRectEntryFieldsThunk00403724(
    void *pObject, int nUnused, const VS_XYPair *pPoint, int nButton);
static void *g_LEVEL_RenderPointRectSinkEntryVtableStorage[8] = {
    (void *)DeleteRenderPointRectSinkThunk00401898,
    (void *)QueueRenderPointRectSinkThunk00402DD3,
    (void *)DispatchRenderPointRectSinkThunk00401C71,
    0,
    (void *)DeleteRenderPointRectContextThunk00401488,
    (void *)ResetRenderPointRectEntryThunk00401703,
    (void *)ResetRenderPointRectEntryThunk0040355D,
    (void *)ClearRenderPointRectEntryFieldsThunk00403724,
};
static void *g_LEVEL_RenderPointRectSinkEntryVtable =
    g_LEVEL_RenderPointRectSinkEntryVtableStorage;
static void *LEMBALL_FASTCALL DeleteLevelRenderRectSink(
    void *pObject, int, unsigned int fDelete)
{
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}
static void LEMBALL_FASTCALL QueueLevelRenderRectSink(
    void *pObject, int, void *pQueue)
{
    AppendPointerQueueEntry(pQueue, pObject);
}
static void LEMBALL_FASTCALL DispatchLevelRenderRectSinkA(
    void *pObject, int, void *pContext)
{
    typedef void (LEMBALL_FASTCALL *DispatchProc)(
        void *pObject, int nUnusedEdx, void *pEntry);
    void *pOwner;

    pOwner = *(void **)((char *)pContext + 0x0c);
    ((DispatchProc)(*(void ***)pOwner)[38])(pOwner, 0, pObject);
}
static void LEMBALL_FASTCALL DispatchLevelRenderRectSinkB(
    void *pObject, int, void *pContext)
{
    typedef void (LEMBALL_FASTCALL *DispatchProc)(
        void *pObject, int nUnusedEdx, void *pEntry);
    void *pTarget;

    pTarget = (char *)*(void **)((char *)pContext + 0x0c) + 0x44;
    ((DispatchProc)(*(void ***)pTarget)[0])(pTarget, 0, pObject);
}
static void *g_LEVEL_RenderRectSinkEntryAVtable[4] = {
    (void *)DeleteLevelRenderRectSink,
    (void *)QueueLevelRenderRectSink,
    (void *)DispatchLevelRenderRectSinkA,
    0
};
static void *g_LEVEL_RenderRectSinkEntryBVtable[4] = {
    (void *)DeleteLevelRenderRectSink,
    (void *)QueueLevelRenderRectSink,
    (void *)DispatchLevelRenderRectSinkB,
    0
};
static void LEMBALL_FASTCALL DispatchLevelSurfaceRectSink(
    void *pObject, int, void *pContext)
{
    typedef void (LEMBALL_FASTCALL *DispatchProc)(
        void *pObject, int nUnusedEdx, void *pEntry);
    char *pGeometryContext;
    void *pRenderObject;

    pGeometryContext = *(char **)((char *)pContext + 0x0c);
    pRenderObject = pGeometryContext +
        *(int *)(*(int *)(pGeometryContext + 0x40) + 4) + 0x40;
    ((DispatchProc)(*(void ***)pRenderObject)[0])(
        pRenderObject, 0, pObject);
}
static void LEMBALL_FASTCALL DispatchLevelNestedRectSink(
    void *pObject, int, void *pContext)
{
    typedef void (LEMBALL_FASTCALL *DispatchProc)(
        void *pObject, int nUnusedEdx, void *pEntry);
    void *pNested;

    pNested = (char *)*(void **)((char *)pContext + 0x0c) + 0x44;
    ((DispatchProc)(*(void ***)pNested)[1])(pNested, 0, pObject);
}
static void *g_LEVEL_SurfaceRectSinkVtable[4] = {
    (void *)DeleteLevelRenderRectSink,
    (void *)QueueLevelRenderRectSink,
    (void *)DispatchLevelSurfaceRectSink,
    0
};
static void *g_LEVEL_NestedRectSinkVtable[4] = {
    (void *)DeleteLevelRenderRectSink,
    (void *)QueueLevelRenderRectSink,
    (void *)DispatchLevelNestedRectSink,
    0
};
static void *g_LEVEL_StatusIndicatorPointSinkEntryVtableStorage[8] = {
    (void *)DestroyStatusIndicatorPointSinkArray00467BB0,
    (void *)QueueStatusIndicatorPointSinkThunk004018AC,
    (void *)DispatchStatusIndicatorPointSinkThunk004025AE,
    0,
    (void *)_purecall,
    (void *)UpdateStatusIndicatorLoadProgressThunk00401DB1,
    (void *)VariantResourceBundleNoopThunk,
    (void *)GetTimedVariantFrameStartTimeThunk,
};
static void *g_LEVEL_StatusIndicatorPointSinkEntryVtable =
    g_LEVEL_StatusIndicatorPointSinkEntryVtableStorage;
static void *g_LEVEL_ActionButtonPointSinkDeleteVtable[1] = {
    (void *)DestroyBaseModeActionButtonPointSinkArray,
};
static void LEMBALL_FASTCALL QueueVariantActionFramePointSink(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL DispatchVariantActionFramePointSink(
    void *pObject, int nUnusedEdx, void *pContext);
static void *g_LEVEL_ActionButtonPointSinkVtable[3] = {
    (void *)DestroyBaseModeActionButtonPointSinkArray,
    (void *)QueueVariantActionFramePointSink,
    (void *)DispatchVariantActionFramePointSink,
};
/* 00499620: the adjacent 00499628 packaged-entry table is now emitted by
 * LevelPackagedRectQueueEntry rather than represented as raw storage here. */
static void *g_LEVEL_VariantRenderManagerVtableStorage[2] = {
    (void *)VariantResourceBundleNoopThunk,
    0,
};
static void *g_LEVEL_VariantRenderManagerVtable =
    g_LEVEL_VariantRenderManagerVtableStorage;
static void *DeleteLevelScreenVariantRenderManager(void *pObject,
                                                    BYTE fDelete);
static void *g_LEVEL_LevelScreenVariantManagerVtable[2] = {
    (void *)DeleteLevelScreenVariantRenderManager,
    (void *)GetTimedVariantFrameStartTimeThunk,
};
static void *g_LEVEL_LevelScreenVariantProgressVtable[1] = {
    (void *)UpdateStatusIndicatorLoadProgressThunk00401DB1,
};
/* 00497C90: manager base table installed after construction. */
static void *g_LEVEL_StatusIndicatorManagerVtable[4] = {
    (void *)NoopVtableCallbackThunk,
    (void *)_purecall,
    (void *)_purecall,
    0,
};
/* 00497C8C: the +0x70 secondary base has one slot before the adjacent
 * primary table begins at 00497C90. */
static void *g_LEVEL_StatusIndicatorManagerSlot70Vtable[1] = {
    (void *)SetStatusIndicatorLoadProgressThunk00402BFD,
};
static void *g_LEVEL_StatusIndicatorManagerSlot74Vtable[8] = {
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)_purecall,
    (void *)_purecall,
    (void *)NoopVtableCallbackThunk,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)NoopVtableCallbackThunk,
};
static void *g_LEVEL_TimedVariantFrameIndexSelectorCtorVtable[8] = {
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)InitializeNonZrleVariantRenderEntry,
    (void *)_purecall,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)0,
};
static void *g_LEVEL_TimedVariantFrameIndexSelectorVtable[6] = {
    (void *)NoopVtableCallbackThunk,
    (void *)_purecall,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)0,
};
static void *g_LEVEL_TimedVariantFrameIndexSelectorCallbackVtable[6] = {
    (void *)NoopVtableCallbackThunk,
    (void *)SetTimedVariantFrameDuration,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)NoopVtableCallbackThunk,
    (void *)0,
};
static int LEMBALL_FASTCALL SelectLevelFirstVariantFrame(void *) {
    return 0;
}
static int LEMBALL_FASTCALL SelectLevelStoredVariantFrame(void *pObject) {
    return *(int *)((char *)pObject + 0x0c);
}
static int LEMBALL_FASTCALL SelectLevelTimedVariantFrame(void *pObject) {
    int *pSelector;
    unsigned int dwNow;
    unsigned int iFrame;

    pSelector = (int *)pObject;
    dwNow = pSelector[5] == -1 ? (unsigned int)timeGetTime() :
        (unsigned int)pSelector[5];
    if (pSelector[4] == 0) return 0;
    iFrame = (((dwNow - (unsigned int)pSelector[3]) %
               (unsigned int)pSelector[4]) *
              (unsigned int)pSelector[1]) /
             (unsigned int)pSelector[4];
    return pSelector[6] == 1 ? (int)iFrame :
        pSelector[1] - (int)iFrame - 1;
}
static int LEMBALL_FASTCALL SelectLevelProgressiveVariantFrame(
    void *pObject)
{
    int *pSelector;
    unsigned int dwNow;
    unsigned int dwElapsed;
    unsigned int iFrame;

    pSelector = (int *)pObject;
    dwNow = pSelector[5] == -1 ? (unsigned int)timeGetTime() :
        (unsigned int)pSelector[5];
    dwElapsed = dwNow - (unsigned int)pSelector[3];
    if (pSelector[4] != 0 && dwElapsed < (unsigned int)pSelector[4]) {
        iFrame = (unsigned int)pSelector[1] * dwElapsed /
            (unsigned int)pSelector[4];
    } else {
        iFrame = (unsigned int)pSelector[1] - 1;
    }
    return pSelector[6] == 1 ? (int)iFrame :
        pSelector[1] - (int)iFrame - 1;
}
static void *g_LEVEL_SelectFirstFrameVtable[1] = {
    (void *)SelectLevelFirstVariantFrame,
};
static void *g_LEVEL_SelectStoredFrameVtable[1] = {
    (void *)SelectLevelStoredVariantFrame,
};
static void *g_LEVEL_SelectTimedFrameVtable[1] = {
    (void *)SelectLevelTimedVariantFrame,
};
static void *g_LEVEL_SelectProgressiveFrameVtable[1] = {
    (void *)SelectLevelProgressiveVariantFrame,
};
static int g_LEVEL_StatusIndicatorModeResourceIds[4] = {0, 0xfd, 0xfc, 0};
// GLOBAL: LEMBALL 0x0049f9b0
static unsigned int g_LEVEL_CompactStatusIndicatorPointTable[5] = {
    0x004d0087, 0x0063008f, 0x005b0094, 0x0000000a, 0x00040004
};
// GLOBAL: LEMBALL 0x0049f9c8
static unsigned int g_LEVEL_StandardStatusIndicatorPointTable[5] = {
    0x009a010e, 0x00c6011a, 0x00b60128, 0x00000014, 0x00080008
};
int g_fLevelScreenStatusIndicatorConfigured = 0;























struct GameCursorPositionEvent {
    unsigned short m_nType;
    unsigned short m_nReserved02;
    DWORD m_dwTime;
    int m_nPackedPosition;
    int m_nReserved0C;
    int m_nReserved10;
};

struct GameCursorEventQueueInterface {
    virtual void Reserved00(void) = 0;
    virtual void Reserved04(void) = 0;
    virtual void Append(void *pEvent) = 0;
};

struct GameSharedGeometryHelperView {
    char m_abReserved00[0x14];
    unsigned int m_dwFlags14;
};













































int g_nLevelScreenCompositeSequencePrimaryResourceId = 0;
int g_nLevelScreenCompositeSequenceSecondaryResourceId = 0;
int g_nLevelScreenContextSelectedVariantResourceId = 0;
int g_nLevelScreenMappedVariantResourceId0x202 = 0;
int g_nLevelScreenMappedVariantResourceId0x206 = 0;
int g_nLevelScreenMappedVariantResourceId0x207 = 0;
int g_nLevelScreenMappedVariantResourceId0x208 = 0;
int g_nLevelScreenMappedVariantResourceId0x209 = 0;
int g_nLevelScreenMappedVariantResourceId0x20A = 0;
int g_nLevelScreenMappedVariantResourceId0x20B = 0;
int g_nLevelScreenMappedVariantResourceId0x20C = 0;
int g_nLevelScreenMappedVariantResourceId0x20D = 0;
int g_nLevelScreenMappedVariantResourceId0x20E = 0;
int g_nLevelScreenMappedVariantResourceId0x20F = 0;
int g_nLevelScreenMultiStageSequencePrimaryResourceId = 0;
int g_nLevelScreenMultiStageSequenceSecondaryResourceId = 0;
int g_nLevelScreenTimedVariantResourceId = 0;
int g_nLevelScreenMultiPhaseSequenceResourceId = 0;

extern void *g_pQueuedRenderPointSinkFinalizeThunk;
extern void *g_pMainResourceArchive;
extern int g_fRootHelperGeometryDispatchSuppressed;
extern void LEMBALL_FASTCALL InitializeHelperUploadStatePending(int nUploadState);
extern void *LoadBitmapResource(int nResourceId);
extern void *LoadPalResource(int nResourceId);
extern void *LoadZrleOnlyListResource(int nResourceId);
extern void *LoadZrleResource(int nResourceId);
extern void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(void *pResourceObject);
extern void *g_pSharedRenderDispatchQueue;
extern void *LEMBALL_FASTCALL ConstructAlignedZrleChildOverlayFromPoint(
    void *pObject, int nUnusedEdx, const VS_XYPair *pPoint,
    void *pRenderOwner, int nResourceId, int fAlignment);
extern void *LEMBALL_FASTCALL ConstructResourceGeometryChildBase(
    void *pObject, int nUnusedEdx, void *pRenderOwner);
extern void LEMBALL_FASTCALL AppendGeometryOwnerChild(
    void *pOwner, int nUnusedEdx, void *pChildOwner);

void LEMBALL_FASTCALL SetStatusIndicatorActive(
    void *pObject, int nUnused, int fActive);
















void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntry(void *pObject);
void *LEMBALL_FASTCALL InitializeRenderPointRectSinkEntry(void *pObject);
void *LEMBALL_FASTCALL InitializeRenderRectSinkEntryA(void *pObject);
void *LEMBALL_FASTCALL InitializeRenderRectSinkEntryB(void *pObject);
void *LEMBALL_FASTCALL InitializeLevelSurfaceRectSink(void *pObject);
void *LEMBALL_FASTCALL InitializeLevelNestedRectSink(void *pObject);
void *LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntry(void *pObject);



































static void ReleaseLevelScreenVariantStateWrapper(
    void *pObject, int nResourceId)
{
    ((LevelVariantRenderManagerView *)pObject)
        ->ReleaseLevelScreenVariantStateWrapper(nResourceId);
}

static void ReleaseLevelScreenVariantStateWrapperRange(
    void *pObject, int nFirstResourceId, int nLastResourceId)
{
    ((LevelVariantRenderManagerView *)pObject)
        ->ReleaseLevelScreenVariantStateWrapperRange(
            nFirstResourceId, nLastResourceId);
}

static void ReleaseVariantRenderResource(void *pObject, int nResourceId)
{
    ((LevelVariantRenderManagerView *)pObject)
        ->ReleaseVariantRenderResource(nResourceId);
}

int ReturnTrueVirtualHookBody(void);







#include "ENGINE/CORE/SAFEVT.H"


int LEMBALL_FASTCALL GetTimedVariantFrameStartTimeThunk(void *pObject);
void LEMBALL_FASTCALL RestoreStatusIndicatorPointSinkEntryVtable(void *pObject);
int LEMBALL_FASTCALL HandleBaseModeActionButtonEventThunk(void *pClient, int, void *pEvent);
void *LEMBALL_FASTCALL InitializeRenderPointRectSinkEntryThunk(void *pObject);
void *LEMBALL_FASTCALL DeleteRenderPointRectContextThunk00401488(
    void *pObject, int, int fDelete);
static void *DeletePackagedSpriteRenderEntryThunk004016F9(void *pObject, BYTE fDelete);
void LEMBALL_FASTCALL ResetRenderPointRectEntryThunk00401703(
    void *pObject, int, const VS_XYPair *pPoint, int nButton);
void LEMBALL_FASTCALL ReleaseStatusIndicatorPrimaryBitmapThunk(
    void *pObject);
static void *DeleteStatusIndicatorPointSinkThunk00401834(void *pObject, BYTE fDelete);
void LEMBALL_FASTCALL RestoreCompositePointRectSinkFinalizeVtables(void *pObject);
void DeleteRenderPointRectSinkThunk00401898(void *pObject, BYTE fDelete);
static void LEMBALL_FASTCALL QueueStatusIndicatorPointSinkThunk004018AC(
    void *pObject, int, void *pQueue);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableD(void *pObject);
void LEMBALL_FASTCALL RestoreFramedScreenRenderChildEntryVtable(void *pObject);
void *LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntryThunk(void *pObject);
void LEMBALL_FASTCALL NoopPackagedSpriteRenderEntryThunk00401B77(
    void *pUnusedThis, int nUnusedEdx, void *pArgument);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableE(void *pObject);
void LEMBALL_FASTCALL DispatchRenderPointRectSinkThunk00401C71(
    void *pObject, int, void *pContext);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableB(void *pObject);
static void LEMBALL_FASTCALL UpdateStatusIndicatorLoadProgressThunk00401DB1(
    void *pObject);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableA(void *pObject);
static void LEMBALL_FASTCALL QueueHelperLocalRectUpdateThunk00401E6F(
    void *pObject, int, void *pQueue);
static void LEMBALL_FASTCALL FinalizeFramedScreenRenderEntryThunk(
    void *pObject, int, void *pManager);
void LEMBALL_FASTCALL RestoreLevelSelectionPointSinkFinalizeVtable(void *pObject);
static void LEMBALL_FASTCALL DispatchStatusIndicatorPointSinkThunk004025AE(
    void *pObject, int, void *pContext);
void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntryThunk(void *pObject);
void *InitializeLevelSelectionPointRectSinkEntry(void *pObject);
static void LEMBALL_FASTCALL QueueFramedScreenRenderEntryThunk(
    void *pObject, int, void *pQueue);
void LEMBALL_FASTCALL NoopPackagedSpriteRenderEntryThunk00402A22(
    void *pUnused);
static void LEMBALL_FASTCALL DispatchPackagedSpriteRenderEntryThunk00402A8B(
    void *pObject, int, void *pContext);
static void LEMBALL_FASTCALL SetStatusIndicatorLoadProgressThunk00402BFD(
    void *pObject, int, int nProgress);
void *DeleteBaseModeObjectAdjustedThunk(void *pObject, BYTE fDelete);
void LEMBALL_FASTCALL QueueRenderPointRectSinkThunk00402DD3(
    void *pObject, int, void *pQueue);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableC(void *pObject);
int ReturnTrueVirtualHook(void);
void *LEMBALL_FASTCALL InitializeFramedScreenRenderChildEntry(
    void *pObject);
void *LEMBALL_FASTCALL InitializeHelperLocalRectUpdateEntry(
    void *pObject);
static void *DeleteFramedScreenRenderChildEntryThunk(void *pObject, BYTE fDelete);
static void LEMBALL_FASTCALL RenderBitmapEntryViaContextThunk(
    void *pObject, int, void *pManager);
void *InitializeBaseModeActionButtonPointSink(void *pObject);
static void *DeleteFramedScreenRenderChildEntryAThunk(void *pObject, BYTE fDelete);
void LEMBALL_FASTCALL RestoreLevelSelectionRenderChildEntryVtable(void *pObject);
void LEMBALL_FASTCALL ResetRenderPointRectEntryThunk0040355D(
    void *pObject, int, const VS_XYPair *pPoint, int nButton);
static void *DeleteQueuedRenderPointSinkThunk00403580(void *pObject, BYTE fDelete);
static void LEMBALL_FASTCALL QueueBitmapRenderEntryThunk(
    void *pObject, int, void *pQueue);
void LEMBALL_FASTCALL ClearRenderPointRectEntryFieldsThunk00403724(
    void *pObject, int, const VS_XYPair *pPoint, int nButton);
static void LEMBALL_FASTCALL DispatchHelperLocalRectUpdateThunk004037A1(
    void *pObject, int, void *pContext);
static void LEMBALL_FASTCALL QueuePackagedSpriteRenderEntryThunk00403846(
    void *pObject, int, void *pQueue);
int ReturnTrueVirtualHookBody(void);
static void *DeleteQueuedRenderPointSinkWrapper00432350(void *pObject, BYTE fDelete);
static void *DeleteStatusIndicatorPointSinkEntry00432A90(void *pObject, BYTE fDelete);
void LEMBALL_FASTCALL RestoreStatusIndicatorPointSinkEntryVtableBody(void *pObject);
static void LEMBALL_FASTCALL QueueStatusIndicatorPointSink00432AD0(
    void *pObject, int, void *pQueue);
static void LEMBALL_FASTCALL DispatchStatusIndicatorPointSink00432AE0(
    void *pObject, int, void *pContext);
static void LEMBALL_FASTCALL QueueHelperLocalRectUpdateEntry00432B10(
    void *pObject, int, void *pQueue);
static void LEMBALL_FASTCALL DispatchHelperLocalRectUpdateEntry00432B20(
    void *pObject, int, void *pContext);
void LEMBALL_FASTCALL ReleaseLevelSelectionModeRenderStateResource(void *pObject);
static void LEMBALL_FASTCALL UpdateStatusIndicatorLoadProgress00434D10(
    void *pObject);
int LEMBALL_FASTCALL GetTimedVariantFrameStartTime(void *pObject);
void NoopPackagedSpriteRenderEntry00439370(void);
void PASCAL NoopPackagedSpriteRenderEntry00439380(void *pUnused);
static void *DeletePackagedSpriteRenderEntry00439400(void *pObject, BYTE fDelete);
void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntry(void *pObject);
void *LEMBALL_FASTCALL InitializeHelperLocalRectUpdateEntryBody(
    void *pObject);
void *LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntry(void *pObject);
void *LEMBALL_FASTCALL InitializeRenderPointRectSinkEntry(void *pObject);
static void *DeleteRenderPointRectSinkEntry004396E0(void *pObject, BYTE fDelete);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableABody(void *pObject);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableBBody(void *pObject);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableCBody(void *pObject);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableDBody(void *pObject);
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableEBody(void *pObject);
static void LEMBALL_FASTCALL QueuePackagedSpriteRenderEntry00439800(
    void *pObject, int, void *pQueue);
static void LEMBALL_FASTCALL DispatchPackagedSpriteRenderEntry00439810(
    void *pObject, int, void *pContext);
static void LEMBALL_FASTCALL QueueRenderPointRectSinkEntry004398A0(
    void *pObject, int, void *pQueue);
static void LEMBALL_FASTCALL DispatchRenderPointRectSinkEntry004398B0(
    void *pObject, int, void *pContext);
void SetLevelScreenStatusIndicatorMode(int nMode, int nValue);
void LEMBALL_FASTCALL ResetBaseModeActionButtonPointSink(void *pObject);
void *LEMBALL_FASTCALL DestroyBaseModeActionButtonPointSinkArray(
    void *pObject, int, unsigned int fDelete);
void *LEMBALL_FASTCALL ConstructFramedScreenRenderEntryGroup(void *pObject);
void *LEMBALL_FASTCALL InitializeFramedScreenRenderChildEntryBody(
    void *pObject);
static void *DeleteFramedScreenRenderChildEntryA(void *pObject, BYTE fDelete);
static void *DeleteFramedScreenRenderChildEntryB(void *pObject, BYTE fDelete);
void LEMBALL_FASTCALL RestoreFramedScreenRenderChildEntryVtableBody(void *pObject);
static void *DeleteBaseModeObjectAdjusted(void *pObject, BYTE fDelete);
static void LEMBALL_FASTCALL QueueFramedScreenRenderEntry(
    void *pObject, int, void *pQueue);
static void LEMBALL_FASTCALL FinalizeFramedScreenRenderEntry(
    void *pObject, int, void *pManager);
static void LEMBALL_FASTCALL QueueBitmapRenderEntry(
    void *pObject, int, void *pQueue);
static void LEMBALL_FASTCALL RenderBitmapEntryViaContext(
    void *pObject, int, void *pManager);
void LEMBALL_FASTCALL RestoreLevelSelectionRenderChildEntryVtableBody(void *pObject);
void *ConstructLevelScreenStatusIndicatorManager(void *pObject, int nStatusMode, void *pPrimaryContext);
void DestroyLevelScreenStatusIndicatorManager(void *pObject);
void *ConstructLevelScreenVariantRenderManager(void *pObject,
    int nRenderOwner, void *pPrimaryContext, void *pLevelMode);
void DestroyLevelScreenVariantRenderManager(void *pObject);
void LEMBALL_FASTCALL ReleaseStatusIndicatorPrimaryBitmap(void *pObject);
static void LEMBALL_FASTCALL SetStatusIndicatorLoadProgress0044B340(
    void *pObject, int, int nProgress);
void LEMBALL_FASTCALL RestoreLevelSelectionPointSinkFinalizeVtableBody(void *pObject);
void LEMBALL_FASTCALL RestoreCompositePointRectSinkFinalizeVtablesBody(void *pObject);
void LEMBALL_FASTCALL SetTimedVariantFrameDuration(
    void *pObject, int, int nFrameDuration);
void DestroyVariantRenderManager(void *pObject);
static void *DestroyStatusIndicatorPointSinkArray00467BB0(void *pObject, unsigned int fDelete);
void LEMBALL_FASTCALL RestoreQueuedRenderPointSinkVtable(void *pObject);
static void *DestroyPackagedSpriteRenderEntryArray00469930(void *pObject, unsigned int fDelete);
void SetStatusIndicatorZrleResource(void *pObject, int nResourceId);
void LEMBALL_FASTCALL SetStatusIndicatorActive(
    void *pObject, int, int fActive);
void SetStatusIndicatorListResourceEntry(void *pObject, int nResourceId, int nEntryIndex);

// STUB: LEMBALL 0x004011b8
int LEMBALL_FASTCALL GetTimedVariantFrameStartTimeThunk(void *pObject) {
    return GetTimedVariantFrameStartTime(pObject);
}

// STUB: LEMBALL 0x00401307
void LEMBALL_FASTCALL RestoreStatusIndicatorPointSinkEntryVtable(void *pObject) {
    RestoreStatusIndicatorPointSinkEntryVtableBody(pObject);
}

// STUB: LEMBALL 0x0040134d
int LEMBALL_FASTCALL HandleBaseModeActionButtonEventThunk(void *pClient, int, void *pEvent) {
    return ((GameBaseModeEventClient *)pClient)
        ->GameBaseModeEventClient::HandleBaseModeActionButtonEvent(
            (VsNetEffDispatchEvent *)pEvent);
}

// STUB: LEMBALL 0x0040135c
void *LEMBALL_FASTCALL InitializeRenderPointRectSinkEntryThunk(void *pObject) {
    return InitializeRenderPointRectSinkEntry(pObject);
}

// STUB: LEMBALL 0x00401488
void *LEMBALL_FASTCALL DeleteRenderPointRectContextThunk00401488(
    void *pObject, int, int fDelete)
{
    return DeleteGeometryOwnerRect(pObject, 0, fDelete);
}

// STUB: LEMBALL 0x004016f9
static void *DeletePackagedSpriteRenderEntryThunk004016F9(void *pObject, BYTE fDelete) {
    return DeletePackagedSpriteRenderEntry00439400(pObject, fDelete);
}

// STUB: LEMBALL 0x00401703
void LEMBALL_FASTCALL ResetRenderPointRectEntryThunk00401703(
    void *pObject, int, const VS_XYPair *pPoint, int nButton)
{
    GeometryOwnerButtonTransitionNoop(pObject, 0, pPoint, nButton);
}

// STUB: LEMBALL 0x00401708
void LEMBALL_FASTCALL ReleaseStatusIndicatorPrimaryBitmapThunk(
    void *pObject)

{
    ReleaseStatusIndicatorPrimaryBitmap(pObject);
}

// STUB: LEMBALL 0x00401834
static void *DeleteStatusIndicatorPointSinkThunk00401834(void *pObject, BYTE fDelete) {
    return DeleteStatusIndicatorPointSinkEntry00432A90(pObject, fDelete);
}

// STUB: LEMBALL 0x00401848
void LEMBALL_FASTCALL RestoreCompositePointRectSinkFinalizeVtables(void *pObject) {
    RestoreCompositePointRectSinkFinalizeVtablesBody(pObject);
}

// STUB: LEMBALL 0x00401898
void DeleteRenderPointRectSinkThunk00401898(void *pObject, BYTE fDelete) {
    DeleteRenderPointRectSinkEntry004396E0(pObject, fDelete);
}

// STUB: LEMBALL 0x004018ac
static void LEMBALL_FASTCALL QueueStatusIndicatorPointSinkThunk004018AC(
    void *pObject, int, void *pQueue)
{
    QueueStatusIndicatorPointSink00432AD0(pObject, 0, pQueue);
}

// STUB: LEMBALL 0x004018b1
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableD(void *pObject) {
    RestorePackagedSpriteRenderEntryVtableDBody(pObject);
}

// STUB: LEMBALL 0x00401974
void LEMBALL_FASTCALL RestoreFramedScreenRenderChildEntryVtable(void *pObject) {
    RestoreFramedScreenRenderChildEntryVtableBody(pObject);
}

// STUB: LEMBALL 0x00401ac8
void *LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntryThunk(void *pObject) {
    return InitializeStatusIndicatorPointSinkEntry(pObject);
}

// STUB: LEMBALL 0x00401b77
void LEMBALL_FASTCALL NoopPackagedSpriteRenderEntryThunk00401B77(
    void *pUnusedThis, int nUnusedEdx, void *pArgument)

{
    (void)pUnusedThis;
    (void)nUnusedEdx;
    NoopPackagedSpriteRenderEntry00439380(pArgument);
}

// STUB: LEMBALL 0x00401b8b
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableE(void *pObject) {
    RestorePackagedSpriteRenderEntryVtableEBody(pObject);
}

// STUB: LEMBALL 0x00401c71
void LEMBALL_FASTCALL DispatchRenderPointRectSinkThunk00401C71(
    void *pObject, int, void *pContext)
{
    DispatchRenderPointRectSinkEntry004398B0(pObject, 0, pContext);
}

// STUB: LEMBALL 0x00401cd5
void LevelVariantRenderManagerView::ReleaseLevelScreenVariantStateWrapper(int nResourceId) {
    ReleaseLevelScreenVariantStateWrapperBody(nResourceId);
}

// STUB: LEMBALL 0x00401d11
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableB(void *pObject) {
    RestorePackagedSpriteRenderEntryVtableBBody(pObject);
}

// STUB: LEMBALL 0x00401db1
static void LEMBALL_FASTCALL UpdateStatusIndicatorLoadProgressThunk00401DB1(
    void *pObject)
{
    UpdateStatusIndicatorLoadProgress00434D10(pObject);
}

// STUB: LEMBALL 0x00401dde
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableA(void *pObject) {
    RestorePackagedSpriteRenderEntryVtableABody(pObject);
}

// STUB: LEMBALL 0x00401e6f
static void LEMBALL_FASTCALL QueueHelperLocalRectUpdateThunk00401E6F(
    void *pObject, int, void *pQueue)
{
    QueueHelperLocalRectUpdateEntry00432B10(pObject, 0, pQueue);
}

// STUB: LEMBALL 0x00401faa
void GameBaseModeScreen::QueueActiveBaseModeUserActionThunk(int nAction, int nPhase) {
    QueueActiveBaseModeUserAction(nAction, nPhase);
}

// STUB: LEMBALL 0x0040213a
static void LEMBALL_FASTCALL FinalizeFramedScreenRenderEntryThunk(
    void *pObject, int, void *pManager)
{
    FinalizeFramedScreenRenderEntry(pObject, 0, pManager);
}

// STUB: LEMBALL 0x004022a2
void LEMBALL_FASTCALL RestoreLevelSelectionPointSinkFinalizeVtable(void *pObject) {
    RestoreLevelSelectionPointSinkFinalizeVtableBody(pObject);
}

// STUB: LEMBALL 0x004025ae
static void LEMBALL_FASTCALL DispatchStatusIndicatorPointSinkThunk004025AE(
    void *pObject, int, void *pContext)
{
    DispatchStatusIndicatorPointSink00432AE0(pObject, 0, pContext);
}

// STUB: LEMBALL 0x0040272f
void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntryThunk(void *pObject) {
    return InitializePackagedSpriteRenderEntry(pObject);
}

// STUB: LEMBALL 0x00402784
void *InitializeLevelSelectionPointRectSinkEntry(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(unsigned short *)(pEntry + 1) = 0;
    *(void **)pEntry = g_LEVEL_CompositePointRectSinkEntryInitVtable;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    *(unsigned short *)((char *)pEntry + 0xe) = 0;
    *(unsigned short *)(pEntry + 3) = 0;
    return pObject;
}

// STUB: LEMBALL 0x004029be
void GameBaseModeScreen::DispatchBaseModeUserActionThunk(int nAction, int nPhase) {
    DispatchBaseModeUserAction(nAction, nPhase);
}

// STUB: LEMBALL 0x004029cd
static void LEMBALL_FASTCALL QueueFramedScreenRenderEntryThunk(
    void *pObject, int, void *pQueue)
{
    QueueFramedScreenRenderEntry(pObject, 0, pQueue);
}

// STUB: LEMBALL 0x00402a22
void LEMBALL_FASTCALL NoopPackagedSpriteRenderEntryThunk00402A22(
    void *pUnused)

{
    (void)pUnused;
    NoopPackagedSpriteRenderEntry00439370();
}

// STUB: LEMBALL 0x00402a8b
static void LEMBALL_FASTCALL DispatchPackagedSpriteRenderEntryThunk00402A8B(
    void *pObject, int, void *pContext)
{
    DispatchPackagedSpriteRenderEntry00439810(pObject, 0, pContext);
}

// STUB: LEMBALL 0x00402bfd
static void LEMBALL_FASTCALL SetStatusIndicatorLoadProgressThunk00402BFD(
    void *pObject, int, int nProgress)
{
    SetStatusIndicatorLoadProgress0044B340(pObject, 0, nProgress);
}

// STUB: LEMBALL 0x00402c20
void *DeleteBaseModeObjectAdjustedThunk(void *pObject, BYTE fDelete) {
    return DeleteBaseModeObjectAdjusted(pObject, fDelete);
}

// STUB: LEMBALL 0x00402d1a
void GameBaseModeObject::QueueBaseModeUserActionThunk(int nAction, int nPhase) {
    QueueBaseModeUserAction(nAction, nPhase);
}

// STUB: LEMBALL 0x00402dd3
void LEMBALL_FASTCALL QueueRenderPointRectSinkThunk00402DD3(
    void *pObject, int, void *pQueue)
{
    QueueRenderPointRectSinkEntry004398A0(pObject, 0, pQueue);
}

// STUB: LEMBALL 0x00402e19
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableC(void *pObject) {
    RestorePackagedSpriteRenderEntryVtableCBody(pObject);
}

// STUB: LEMBALL 0x00402f1d
void LevelVariantRenderManagerView::ReleaseLevelScreenVariantStateWrapperRange(int nFirstResourceId, int nLastResourceId) {
    ReleaseLevelScreenVariantStateWrapperRangeBody(nFirstResourceId, nLastResourceId);
}

// STUB: LEMBALL 0x00402fef
int ReturnTrueVirtualHook(void) {
    return ReturnTrueVirtualHookBody();
}

// STUB: LEMBALL 0x004030bc
void *LEMBALL_FASTCALL InitializeFramedScreenRenderChildEntry(
    void *pObject)

{
    return InitializeFramedScreenRenderChildEntryBody(pObject);
}

// STUB: LEMBALL 0x00403139
void *LEMBALL_FASTCALL InitializeHelperLocalRectUpdateEntry(
    void *pObject)

{
    return InitializeHelperLocalRectUpdateEntryBody(pObject);
}

// STUB: LEMBALL 0x00403166
static void *DeleteFramedScreenRenderChildEntryThunk(void *pObject, BYTE fDelete) {
    return DeleteFramedScreenRenderChildEntryB(pObject, fDelete);
}

// STUB: LEMBALL 0x004031ac
static void LEMBALL_FASTCALL RenderBitmapEntryViaContextThunk(
    void *pObject, int, void *pManager)
{
    RenderBitmapEntryViaContext(pObject, 0, pManager);
}

// STUB: LEMBALL 0x00403265
void *InitializeBaseModeActionButtonPointSink(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(void **)pEntry = g_LEVEL_ActionButtonPointSinkVtable;
    *(unsigned short *)(pEntry + 1) = 0;
    return pObject;
}

// STUB: LEMBALL 0x004033fa
static void *DeleteFramedScreenRenderChildEntryAThunk(void *pObject, BYTE fDelete) {
    return DeleteFramedScreenRenderChildEntryA(pObject, fDelete);
}

// STUB: LEMBALL 0x00403558
void LEMBALL_FASTCALL RestoreLevelSelectionRenderChildEntryVtable(void *pObject) {
    RestoreLevelSelectionRenderChildEntryVtableBody(pObject);
}

// STUB: LEMBALL 0x0040355d
void LEMBALL_FASTCALL ResetRenderPointRectEntryThunk0040355D(
    void *pObject, int, const VS_XYPair *pPoint, int nButton)
{
    GeometryOwnerButtonInsideNoop(pObject, 0, pPoint, nButton);
}

// STUB: LEMBALL 0x00403580
static void *DeleteQueuedRenderPointSinkThunk00403580(void *pObject, BYTE fDelete) {
    return DeleteQueuedRenderPointSinkWrapper00432350(pObject, fDelete);
}

// STUB: LEMBALL 0x0040362f
static void LEMBALL_FASTCALL QueueBitmapRenderEntryThunk(
    void *pObject, int, void *pQueue)
{
    QueueBitmapRenderEntry(pObject, 0, pQueue);
}

// STUB: LEMBALL 0x00403724
void LEMBALL_FASTCALL ClearRenderPointRectEntryFieldsThunk00403724(
    void *pObject, int, const VS_XYPair *pPoint, int nButton)
{
    ClearGeometryOwnerButtonState(pObject, 0, pPoint, nButton);
}

// STUB: LEMBALL 0x004037a1
static void LEMBALL_FASTCALL DispatchHelperLocalRectUpdateThunk004037A1(
    void *pObject, int, void *pContext)
{
    DispatchHelperLocalRectUpdateEntry00432B20(pObject, 0, pContext);
}

// STUB: LEMBALL 0x00403846
static void LEMBALL_FASTCALL QueuePackagedSpriteRenderEntryThunk00403846(
    void *pObject, int, void *pQueue)
{
    QueuePackagedSpriteRenderEntry00439800(pObject, 0, pQueue);
}

// FUNCTION: LEMBALL 0x00413090
int ReturnTrueVirtualHookBody(void) {
    return 1;
}

struct LevelViewRotationTransform {
    void *m_pVtable00;
    int m_nRotation04;

    void TransformScreenToTile(int nScreenX, int nScreenY,
                               int *pTileX, int *pTileY);
    void TransformTileToScreen(int nTileX, int nTileY,
                               int *pScreenX, int *pScreenY);
    void TransformTileInPlace(int *pX, int *pY);
};

static short SampleLevelTileHeightOffset(
    const char *pTile, int nLocalX, int nLocalY)
{
    short nHeight;

    nHeight = *(const short *)(pTile + 8);
    switch (*(const int *)pTile) {
    case 0x206: return (short)(nHeight + 0x10);
    case 0x207: return (short)(nHeight + 8);
    case 0x208: return (short)(nHeight - nLocalY + 0x10);
    case 0x20b: return (short)(nHeight + nLocalX);
    case 0x20c: return (short)(nHeight + nLocalY);
    case 0x20d: return (short)(nHeight - nLocalX + 0x10);
    case 0x20e: return (short)(nHeight - nLocalY / 2 + 7);
    case 0x20f: return (short)(nHeight - nLocalX / 2 + 7);
    case 0x202: case 0x209: case 0x20a: case 0x210:
    case 0x214: case 0x215: case 0x216: case 0x217:
    case 0x219: case 0x21a:
        return nHeight;
    }
    return 0;
}

static short GetLevelTileHeightSample(
    const char *pLevel, int x, int y, int nLocalX, int nLocalY)
{
    int nTileX;
    int nTileY;

    nTileX = x >> 4;
    nTileY = y >> 4;
    if (x < 0 || y < 0 ||
        nTileX >= *(const int *)(pLevel + 0x10) ||
        nTileY >= *(const int *)(pLevel + 0x14)) return 0;
    return SampleLevelTileHeightOffset(
        *(const char **)(pLevel + 0x0c) +
        (nTileY * *(const int *)(pLevel + 0x10) + nTileX) * 0x0c,
        nLocalX, nLocalY);
}

static int GetLevelTileSolidFlag(
    const char *pLevel, int nTileX, int nTileY)
{
    if (nTileX < 0 || nTileY < 0 ||
        nTileX >= *(const int *)(pLevel + 0x10) ||
        nTileY >= *(const int *)(pLevel + 0x14)) return 1;
    return (*(const unsigned short *)(
        *(const char **)(pLevel + 0x0c) +
        (nTileY * *(const int *)(pLevel + 0x10) + nTileX) * 0x0c + 6) & 1) != 0;
}

struct LevelScreenTerrainRenderView {
    void EmitRepeatedVariantColumn(short x, int y,
                                   int nHeight, int cEntries);
    void EmitVariantStripPassA(int nTileX, int nTileY, int cEntries);
    void EmitVariantStripPassB(int nTileX, int nTileY, int cEntries);
    void BuildStaticRegionEntries(const short *pRegion);
    void EmitObjectVariantRenderEntries(
        unsigned short *pEntry, unsigned int nEntityId, int fSecondary);
    void DispatchDynamicRenderEntry(int iEntry, unsigned short nDepth);
    void PrepareAndSortDynamicDepthEntries(void);
    short ComputeDynamicRenderEntrySortKey(int iEntry);
    void DrawStaticRenderEntry(int iEntry, unsigned short nDepth);
    void DrawStaticDepthEntries(void);
};

// FUNCTION: LEMBALL 0x00430a50
void LevelViewRotationTransform::TransformScreenToTile(
    int nScreenX, int nScreenY, int *pTileX, int *pTileY)
{
    switch (m_nRotation04) {
    case 0: *pTileX = nScreenX / 2 + nScreenY - 8;
            *pTileY = nScreenY - nScreenX / 2 + 8; break;
    case 1: *pTileX = nScreenY - nScreenX / 2 + 8;
            *pTileY = 0x18 - nScreenX / 2 - nScreenY; break;
    case 2: *pTileX = 0x18 - nScreenX / 2 - nScreenY;
            *pTileY = nScreenX / 2 - nScreenY + 8; break;
    case 3: *pTileX = nScreenX / 2 - nScreenY + 8;
            *pTileY = nScreenX / 2 + nScreenY - 8; break;
    }
}

// FUNCTION: LEMBALL 0x00430be0
void LevelViewRotationTransform::TransformTileToScreen(
    int nTileX, int nTileY, int *pScreenX, int *pScreenY)
{
    switch (m_nRotation04) {
    case 0: *pScreenX = nTileX - nTileY + 0x10;
            *pScreenY = nTileY / 2 + nTileX / 2; break;
    case 1: *pScreenX = 0x20 - nTileY - nTileX;
            *pScreenY = nTileX / 2 - nTileY / 2 + 8; break;
    case 2: *pScreenX = nTileY - nTileX + 0x10;
            *pScreenY = 0x10 - nTileY / 2 - nTileX / 2; break;
    case 3: *pScreenX = nTileX + nTileY;
            *pScreenY = nTileY / 2 - nTileX / 2 + 8; break;
    }
}

// FUNCTION: LEMBALL 0x00430ce0
void LevelViewRotationTransform::TransformTileInPlace(int *pX, int *pY)
{
    int nTileX;
    int nTileY;

    nTileX = *pX;
    nTileY = *pY;
    TransformTileToScreen(nTileX, nTileY, pX, pY);
}

// FUNCTION: LEMBALL 0x00432350
static void *DeleteQueuedRenderPointSinkWrapper00432350(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00432a90
static void *DeleteStatusIndicatorPointSinkEntry00432A90(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00432ac0
void LEMBALL_FASTCALL RestoreStatusIndicatorPointSinkEntryVtableBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00432ad0
static void LEMBALL_FASTCALL QueueStatusIndicatorPointSink00432AD0(
    void *pObject, int, void *pQueue)
{
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00432ae0
static void LEMBALL_FASTCALL DispatchStatusIndicatorPointSink00432AE0(
    void *pObject, int, void *pContext)
{
    int pOwner;
    int pOwnerOffsets;
    char *pDispatchObject;

    pOwner = *(int *)((char *)pContext + 0x0c);
    pOwnerOffsets = *(int *)(pOwner + 0x40);
    pDispatchObject = (char *)(*(int *)(pOwnerOffsets + 4) + pOwner + 0x40);
    ((LevelOneArgVirtualProc)(*(void ***)pDispatchObject)[4])(
        pDispatchObject, 0, pObject);
}

// FUNCTION: LEMBALL 0x00432b10
static void LEMBALL_FASTCALL QueueHelperLocalRectUpdateEntry00432B10(
    void *pObject, int, void *pQueue)
{
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00432b20
static void LEMBALL_FASTCALL DispatchHelperLocalRectUpdateEntry00432B20(
    void *pObject, int, void *pContext)
{
    int pOwner;
    int pOwnerOffsets;
    int pDispatchBase;
    void *pDispatchObject;

    pOwner = *(int *)((char *)pContext + 0x0c);
    pOwnerOffsets = *(int *)(pOwner + 0x40);
    pDispatchBase = *(int *)(pOwnerOffsets + 4);
    pDispatchObject = (void *)(unsigned long)(pDispatchBase + 0x40 + pOwner);
    ((LevelOneArgVirtualProc)(*(void ***)pDispatchObject)[1])(
        pDispatchObject, 0, pObject);
}

static void EmitLevelScreenFixedVariantEntry(
    void *pObject, short x, short y, int nResourceId,
    int iFrame, int nValue)
{
    int *pManager;
    short iSlot;
    int *pSelector;
    short Point[2];
    short Rect[4];

    if (nResourceId == 0) {
        return;
    }
    pManager = (int *)pObject;
    iSlot = *(short *)(unsigned long)(pManager[10] +
                                      nResourceId * 2);
    if ((unsigned short)iSlot >= (unsigned int)pManager[0xb]) {
        return;
    }
    pSelector = *(int **)(unsigned long)(pManager[0x1d] +
                                         iSlot * 4);
    if (pSelector == 0) {
        return;
    }
    pSelector[3] = iFrame;
    Point[0] = (short)(x + *(short *)((char *)pManager + 0xb0));
    Point[1] = (short)(y + *(short *)((char *)pManager + 0xb2));
    ((LevelVariantRenderManagerView *)pManager)
        ->EmitVariantRenderResourceEntry(
            Rect, Point, nResourceId,
            pManager[0x2b], pSelector, nValue);
}

static void EmitLevelScreenVariantEntry(
    void *pObject, short x, short y, int nResourceId,
    int nFlags, void *pFrameSelector, int nValue)
{
    int *pManager;
    short iSlot;
    short Point[2];
    short Rect[4];

    if (nResourceId == 0) return;
    pManager = (int *)pObject;
    iSlot = *(short *)(unsigned long)(pManager[10] + nResourceId * 2);
    if ((unsigned short)iSlot >= (unsigned int)pManager[0xb] ||
        *(void **)(unsigned long)(pManager[9] + iSlot * 4) == 0) return;
    Point[0] = (short)(x + *(short *)((char *)pManager + 0xb0));
    Point[1] = (short)(y + *(short *)((char *)pManager + 0xb2));
    ((LevelVariantRenderManagerView *)pManager)
        ->EmitVariantRenderResourceEntry(
            Rect, Point, nResourceId,
            nFlags, pFrameSelector, nValue);
}

void EmitMappedLevelScreenVariantRenderEntry(
    void *pScreenObject, unsigned short x, short y,
    int nTileResourceId, unsigned int nFrameValue)
{
    char *pScreen;
    void *pManager;
    int nResourceId;
    short nXOffset;
    short nYOffset;
    int iFrame;

    pScreen = (char *)pScreenObject;
    pManager = *(void **)(pScreen + 0xa40);
    if (pManager == 0) {
        return;
    }
    nResourceId = 0;
    nXOffset = -0x10;
    nYOffset = -0x10;
    iFrame = (int)(nFrameValue & 0xffff);
    switch (nTileResourceId) {
    case 0x202:
        nResourceId = g_nLevelScreenMappedVariantResourceId0x202;
        nXOffset = -0x20;
        nYOffset = -0x30;
        break;
    case 0x206: nResourceId = g_nLevelScreenMappedVariantResourceId0x206; break;
    case 0x207: nResourceId = g_nLevelScreenMappedVariantResourceId0x207; break;
    case 0x208: nResourceId = g_nLevelScreenMappedVariantResourceId0x208; break;
    case 0x209: nResourceId = g_nLevelScreenMappedVariantResourceId0x209; break;
    case 0x20a: nResourceId = g_nLevelScreenMappedVariantResourceId0x20A; break;
    case 0x20b: nResourceId = g_nLevelScreenMappedVariantResourceId0x20B; break;
    case 0x20c: nResourceId = g_nLevelScreenMappedVariantResourceId0x20C; break;
    case 0x20d: nResourceId = g_nLevelScreenMappedVariantResourceId0x20D; break;
    case 0x20e: nResourceId = g_nLevelScreenMappedVariantResourceId0x20E; break;
    case 0x20f: nResourceId = g_nLevelScreenMappedVariantResourceId0x20F; break;
    case 0x210: nResourceId = 0xde; break;
    case 0x215:
        nResourceId = 0xdf;
        nYOffset = -0x20;
        iFrame = ((x >> 4) +
                  *(unsigned short *)(pScreen + 0x166)) % 9;
        break;
    case 0x216:
        nResourceId = 0xe0;
        iFrame = *(unsigned short *)(pScreen + 0x166) & 3;
        break;
    case 0x217: nResourceId = 0xe1; break;
    case 0x219:
    case 0x21a:
        nResourceId = 0xdd;
        break;
    }
    EmitLevelScreenFixedVariantEntry(
        pManager, (short)(x + nXOffset),
        (short)(y + nYOffset), nResourceId, iFrame, 0);
}

static short GetLevelActionPanelListEntryDimension(
    void *pResource, int nOffset)
{
    char *pEntry;

    if (pResource == 0) {
        return 0;
    }
    pEntry = *(char **)((char *)pResource + 0x78);
    if (pEntry == 0) {
        return 0;
    }
    return *(short *)(pEntry + nOffset);
}

static void *ConstructLevelActionButtonGeometry(
    void *pObject, void *pModeObject, const VS_XYPair *pPanelOrigin,
    void *pPanel)
{
    typedef void (LEMBALL_FASTCALL *InitializeRectProc)(
        void *pObject, int nUnusedEdx, short *pRect,
        void *pOwner, const char *pszName);
    char *pButton;
    char *pChild;
    char *pPanelBytes;
    short aRect[4];
    short nSlot;
    void *pPrimaryContext;

    pButton = (char *)pObject;
    pPanelBytes = (char *)pPanel;
    memset(pButton, 0, 0x2c);
    *(void **)(pButton + 8) = pModeObject;
    *(void **)(pButton + 0x0c) = pPanel;
    nSlot = pModeObject == 0 ? 0 :
        *(unsigned short *)((char *)pModeObject + 0x168);
    *(unsigned short *)(pButton + 0x14) = (unsigned short)nSlot;
    *(int *)(pButton + 0x18) = -1;

    pChild = (char *)AllocateVSMemBlock(0x1b8);
    *(void **)(pButton + 4) = pChild;
    if (pChild == 0) {
        return pButton;
    }
    pPrimaryContext = *(void **)(pPanelBytes + 0x14);
    ConstructResourceGeometryChildBase(pChild, 0, pPrimaryContext);
    aRect[0] = (short)(*(short *)(pPanelBytes + 0x44) +
                             *(short *)(pPanelBytes + 0x48));
    aRect[1] = *(short *)(pPanelBytes + 0x4a);
    aRect[2] = (short)(pPanelOrigin->m_x +
        aRect[0] * nSlot);
    aRect[3] = pPanelOrigin->m_y;
    *(short *)(pChild + 0xdc) = aRect[2];
    *(short *)(pChild + 0xde) = aRect[3];
    ((InitializeRectProc)(*(void ***)pChild)[1])(
        pChild, 0, aRect, pPrimaryContext, "Button");
    *(short *)(pChild + 0xc4) += *(short *)(pChild + 0x18);
    *(short *)(pChild + 0xc6) += *(short *)(pChild + 0x1a);
    AppendGeometryOwnerChild(
        *(void **)((char *)pPrimaryContext + 0x1c), 0,
        pChild + 0x90);
    *(int *)(pChild + 0xb8) = 1;
    return pButton;
}

void *ConstructLevelScreenActionPanelResources(
    void *pObject, void *pLevelScreen)
{
    char *pPanel;
    char *pScreen;
    char *pPrimaryContext;
    int nScale;
    int nContextWidth;
    int nContextHeight;

    pPanel = (char *)pObject;
    pScreen = (char *)pLevelScreen;
    memset(pPanel, 0, 0x58);
    InitializeRenderQueueNodeBase(pPanel);
    *(void **)(pPanel + 0x10) = pScreen;
    *(void **)(pPanel + 0x14) = *(void **)(pScreen + 0x978);
    *(void **)(pPanel + 0x18) = *(void **)(pScreen + 0x96c);
    *(void **)(pPanel + 0x30) = LoadZrleOnlyListResource(0x2d);
    *(void **)(pPanel + 0x34) = LoadZrleOnlyListResource(0x2a);
    *(void **)(pPanel + 0x38) = LoadZrleOnlyListResource(0x2c);
    *(void **)(pPanel + 0x3c) = LoadZrleOnlyListResource(0x2b);
    *(short *)(pPanel + 0x44) = GetLevelActionPanelListEntryDimension(
        *(void **)(pPanel + 0x34), 0x4c);
    *(short *)(pPanel + 0x46) = GetLevelActionPanelListEntryDimension(
        *(void **)(pPanel + 0x34), 0x4e);
    *(short *)(pPanel + 0x48) = GetLevelActionPanelListEntryDimension(
        *(void **)(pPanel + 0x38), 0x4c);
    *(short *)(pPanel + 0x4a) = GetLevelActionPanelListEntryDimension(
        *(void **)(pPanel + 0x38), 0x4e);
    *(short *)(pPanel + 0x4c) = GetLevelActionPanelListEntryDimension(
        *(void **)(pPanel + 0x30), 0x4c);
    *(short *)(pPanel + 0x4e) = GetLevelActionPanelListEntryDimension(
        *(void **)(pPanel + 0x30), 0x4e);
    *(short *)(pPanel + 0x50) = (short)(
        *(short *)(pPanel + 0x4c) +
        (*(short *)(pPanel + 0x48) + *(short *)(pPanel + 0x44)) * 4);

    pPrimaryContext = *(char **)(pPanel + 0x14);
    nScale = *(int *)(pPrimaryContext + 0x38);
    if (nScale == 0) {
        nScale = 1;
    }
    if (*(short *)(pPrimaryContext + 0x10) *
            *(short *)(pPrimaryContext + 0x12) == 0) {
        nContextWidth = *(short *)(pPrimaryContext + 8);
        nContextHeight = *(short *)(pPrimaryContext + 10);
    } else {
        nContextWidth = *(short *)(pPrimaryContext + 0x10);
        nContextHeight = *(short *)(pPrimaryContext + 0x12);
    }
    *(short *)(pPanel + 0x54) = (short)(
        (nContextWidth / nScale - *(short *)(pPanel + 0x50)) / 2);
    *(short *)(pPanel + 0x56) = (short)(
        nContextHeight / nScale - *(short *)(pPanel + 0x4e));
    {
        VS_XYPair Point;
        void *pOverlay;

        Point.m_x = *(short *)(pPanel + 0x54);
        Point.m_y = *(short *)(pPanel + 0x56);
        pOverlay = AllocateVSMemBlock(0x13c);
        if (pOverlay != 0) {
            ConstructAlignedZrleChildOverlayFromPoint(
                pOverlay, 0, &Point, pPrimaryContext, 0x2d, 3);
            *(void **)((char *)pOverlay + 0x134) = pPanel;
            *(int *)((char *)pOverlay + 0x138) = 0;
            *(int *)((char *)pOverlay + 0xb8) = 1;
        }
        *(void **)(pPanel + 0x2c) = pOverlay;
    }
    {
        VS_XYPair ButtonOrigin;
        char *pLevelMode;
        void *pButton;
        void *pModeObject;
        int i;

        ButtonOrigin.m_x = (short)(*(short *)(pPanel + 0x54) +
                                     *(short *)(pPanel + 0x4c));
        ButtonOrigin.m_y = *(short *)(pPanel + 0x56);
        pLevelMode = *(char **)(pPanel + 0x18);
        for (i = 0; i < 4; ++i) {
            pButton = AllocateVSMemBlock(0x2c);
            pModeObject = pLevelMode == 0 ? 0 :
                *(void **)(pLevelMode + 0x1d0 + i * 4);
            if (pButton != 0) {
                ConstructLevelActionButtonGeometry(
                    pButton, pModeObject, &ButtonOrigin, pPanel);
            }
            *(void **)(pPanel + 0x1c + i * 4) = pButton;
        }
    }
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->RegisterOrderedRenderDispatchClient(pPanel, 0);
    }
    return pPanel;
}

void DestroyLevelScreenActionPanelResources(void *pObject) {
    char *pPanel;
    int i;

    pPanel = (char *)pObject;
    if (g_pSharedRenderDispatchQueue != 0) {
        ((GameRenderDispatchQueue *)g_pSharedRenderDispatchQueue)
            ->UnregisterOrderedRenderDispatchClient(pPanel, 0);
    }
    for (i = 0; i < 4; ++i) {
        char *pButton;
        void *pChild;

        pButton = *(char **)(pPanel + 0x1c + i * 4);
        if (pButton != 0) {
            pChild = *(void **)(pButton + 4);
            if (pChild != 0) {
                typedef void (LEMBALL_FASTCALL *DeleteProc)(
                    void *pObject, int nUnusedEdx,
                    unsigned int fDelete);
                ((DeleteProc)(*(void ***)((char *)pChild + 0x90))[0])(
                    (char *)pChild + 0x90, 0, 1);
            }
            FreeVSMemBlock(pButton);
            *(void **)(pPanel + 0x1c + i * 4) = 0;
        }
    }
    for (i = 0x3c; i >= 0x30; i -= 4) {
        if (*(void **)(pPanel + i) != 0) {
            ReleaseTypedResourceObjectReference(*(void **)(pPanel + i));
            *(void **)(pPanel + i) = 0;
        }
    }
    if (*(void **)(pPanel + 0x2c) != 0) {
        typedef void (LEMBALL_FASTCALL *DeleteProc)(
            void *pObject, int nUnusedEdx, unsigned int fDelete);
        void *pOverlay;

        pOverlay = *(void **)(pPanel + 0x2c);
        ((DeleteProc)(*(void ***)((char *)pOverlay + 0x90))[0])(
            (char *)pOverlay + 0x90, 0, 1);
        *(void **)(pPanel + 0x2c) = 0;
    }
}

// FUNCTION: LEMBALL 0x00432b50
void *ConstructLevelScreenVariantRenderManager(void *pObject,
    int nRenderOwner, void *pPrimaryContext, void *pLevelMode)
{
    int *pManager;
    int *pWrapperTable;
    void *pStatusManager;
    int i;

    pManager = (int *)pObject;
    ((LevelVariantRenderManagerView *)pObject)->Construct(
        nRenderOwner, 0x2b6, 200, 0x28, 0x14, 1);
    *(void ***)pManager = g_LEVEL_LevelScreenVariantManagerVtable;
    *(void ***)(pManager + 0x1c) =
        g_LEVEL_LevelScreenVariantProgressVtable;
    pManager[0x1f] = (int)(unsigned long)pPrimaryContext;
    pManager[0x20] = nRenderOwner;
    pManager[0x22] = (int)(unsigned long)
        g_pQueuedRenderPointSinkFinalizeThunk;
    pManager[0x26] = (int)(unsigned long)pLevelMode;

    pWrapperTable = (int *)AllocateVSMemBlock(0xad8);
    pManager[0x1d] = (int)(unsigned long)pWrapperTable;
    if (pWrapperTable != 0) {
        for (i = 0; i < 0xad8 / 4; ++i) {
            pWrapperTable[i] = 0;
        }
    }
    pManager[0x2b] = 0;
    pManager[0x21] = 0;
    pManager[0x27] = 0;
    pManager[0x28] = 0;
    pManager[0x29] = 0;

    pStatusManager = AllocateVSMemBlock(0x134);
    if (pStatusManager != 0) {
        pStatusManager = ConstructLevelScreenStatusIndicatorManager(
            pStatusManager, nRenderOwner, pPrimaryContext);
    }
    pManager[0x1e] = (int)(unsigned long)pStatusManager;
    *(unsigned short *)((char *)pManager + 0xb0) = 0;
    *(unsigned short *)((char *)pManager + 0xb2) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00432c20
void DestroyLevelScreenVariantRenderManager(void *pObject)
{
    int *pManager;
    void *pStatusManager;

    pManager = (int *)pObject;
    *(void ***)pManager = g_LEVEL_LevelScreenVariantManagerVtable;
    *(void ***)(pManager + 0x1c) =
        g_LEVEL_LevelScreenVariantProgressVtable;
    pStatusManager = (void *)(unsigned long)pManager[0x1e];
    if (pStatusManager != 0) {
        DestroyLevelScreenStatusIndicatorManager(pStatusManager);
        FreeVSMemBlock(pStatusManager);
        pManager[0x1e] = 0;
    }
    ReleaseLevelSelectionModeRenderStateResource(pObject);
    if (pManager[0x1d] != 0) {
        FreeVSMemBlock((void *)(unsigned long)pManager[0x1d]);
        pManager[0x1d] = 0;
    }
    pManager[0x22] = (int)(unsigned long)
        g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    DestroyVariantRenderManager(pObject);
}

static void *DeleteLevelScreenVariantRenderManager(void *pObject,
                                                    BYTE fDelete)
{
    DestroyLevelScreenVariantRenderManager(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

static void LEMBALL_FASTCALL QueueVariantActionFramePointSink(
    void *pObject, int, void *pQueue) {
    int *pResource;

    pResource = *(int **)((char *)pObject + 0x1c);
    if (pResource[4] == 0) {
        ((void (LEMBALL_FASTCALL *)(void *, int))
            (*(void ***)pResource)[7])(pResource, 0);
    } else {
        pResource[9] = 0;
    }
    ++pResource[2];
    AppendPointerQueueEntry(pQueue, pObject);
}

static void LEMBALL_FASTCALL DispatchVariantActionFramePointSink(
    void *pObject, int, void *pContext) {
    typedef void (LEMBALL_FASTCALL *RenderProc)(
        void *, int, void *, void *);
    int nContext;
    int nRenderObject;
    int *pResource;
    void *pFrame;

    pResource = *(int **)((char *)pObject + 0x1c);
    pFrame = (char *)pResource[0x1e] +
             *(int *)((char *)pObject + 0x18) * 0x54;
    nContext = *(int *)((char *)pContext + 0x0c);
    nRenderObject = *(int *)(*(int *)(nContext + 0x40) + 4) +
                    0x40 + nContext;
    ((RenderProc)(*(void ***)nRenderObject)[9])(
        (void *)nRenderObject, 0, pObject, pFrame);
    --pResource[2];
}

struct LevelVariantResourceLoadSpec {
    unsigned short m_nResourceId;
    unsigned short m_nSelectorMode;
};

static void LoadLevelScreenVariantState(void *pObject,
                                        int nResourceId,
                                        int nSelectorMode)
{
    int *pManager;
    int *pSelector;
    int iSlot;
    int cFrames;

    pManager = (int *)pObject;
    if (pManager[0x25] != 0) {
        ++pManager[0x24];
        return;
    }
    ((LevelVariantRenderManagerView *)pObject)
        ->LoadVariantRenderResource(nResourceId);
    cFrames = 1;
    if (nSelectorMode == 1 || nSelectorMode == 3) {
        cFrames = ((LevelVariantRenderManagerView *)pObject)
            ->GetVariantRenderFrameCount(nResourceId);
        pSelector = (int *)AllocateVSMemBlock(0x1c);
        if (pSelector != 0) {
            pSelector[0] = (int)(unsigned long)
                (nSelectorMode == 1 ? g_LEVEL_SelectTimedFrameVtable :
                                      g_LEVEL_SelectProgressiveFrameVtable);
            pSelector[1] = cFrames;
            pSelector[3] = (int)timeGetTime();
            pSelector[4] = cFrames * 0x42;
            pSelector[5] = -1;
            pSelector[6] = 1;
        }
    } else {
        pSelector = (int *)AllocateVSMemBlock(0x10);
        if (pSelector != 0) {
            pSelector[0] = (int)(unsigned long)
                (nSelectorMode == 0 ? g_LEVEL_SelectFirstFrameVtable :
                                      g_LEVEL_SelectStoredFrameVtable);
            pSelector[1] = 1;
            pSelector[2] = 0;
            pSelector[3] = 0;
        }
    }
    iSlot = *(short *)(unsigned long)(pManager[10] + nResourceId * 2);
    *(int *)(unsigned long)(pManager[0x1d] + iSlot * 4) =
        (int)(unsigned long)pSelector;
    UpdateStatusIndicatorLoadProgress00434D10((char *)pObject + 0x70);
}

static void SelectLevelScreenVariantResourceIds(int nMode) {
    static const int aanMappedIds[4][11] = {
        {0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xe4},
        {0xbf,0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0x105},
        {0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0x103},
        {0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0}
    };
    static const int aanSequenceIds[4][7] = {
        {0x20,0x21,0x8b,0x72,0x73,0x99,0xe7},
        {0x22,0x23,0x8b,0x79,0x7a,0x9b,0x106},
        {0x24,0x25,0x8a,0x77,0x78,0x9a,0x104},
        {0x26,0x27,0x8b,0x72,0x73,0x9b,0}
    };
    int i;

    if (nMode < 0 || nMode > 3) nMode = 0;
    g_nLevelScreenMappedVariantResourceId0x206 = aanMappedIds[nMode][0];
    g_nLevelScreenMappedVariantResourceId0x207 = aanMappedIds[nMode][1];
    g_nLevelScreenMappedVariantResourceId0x208 = aanMappedIds[nMode][2];
    g_nLevelScreenMappedVariantResourceId0x209 = aanMappedIds[nMode][3];
    g_nLevelScreenMappedVariantResourceId0x20A = aanMappedIds[nMode][4];
    g_nLevelScreenMappedVariantResourceId0x20B = aanMappedIds[nMode][5];
    g_nLevelScreenMappedVariantResourceId0x20C = aanMappedIds[nMode][6];
    g_nLevelScreenMappedVariantResourceId0x20D = aanMappedIds[nMode][7];
    g_nLevelScreenMappedVariantResourceId0x20E = aanMappedIds[nMode][8];
    g_nLevelScreenMappedVariantResourceId0x20F = aanMappedIds[nMode][9];
    g_nLevelScreenMappedVariantResourceId0x202 = aanMappedIds[nMode][10];
    g_nLevelScreenMultiStageSequencePrimaryResourceId = aanSequenceIds[nMode][0];
    g_nLevelScreenMultiStageSequenceSecondaryResourceId = aanSequenceIds[nMode][1];
    g_nLevelScreenTimedVariantResourceId = aanSequenceIds[nMode][2];
    g_nLevelScreenCompositeSequencePrimaryResourceId = aanSequenceIds[nMode][3];
    g_nLevelScreenCompositeSequenceSecondaryResourceId = aanSequenceIds[nMode][4];
    g_nLevelScreenMultiPhaseSequenceResourceId = aanSequenceIds[nMode][5];
    g_nLevelScreenContextSelectedVariantResourceId = aanSequenceIds[nMode][6];
    (void)i;
}

static void LoadLevelScreenVariantResourceBatch(void *pObject) {
    static const LevelVariantResourceLoadSpec aResources[] = {
        {0xdd,2},{0xfd,0},{0xad,0},{0xaf,0},{0x6f,0},{0x70,0},{0x71,0},
        {0x94,0},{0x95,0},{0x96,0},{0x44,2},{0x43,2},{0x46,2},{0x45,2},
        {0x3f,2},{0x7c,1},{0x82,3},{0xae,1},{0x75,2},{0x76,2},{0x80,1},
        {0x81,1},{0xb1,0},{0x86,0},{0xdf,2},{0xe0,2},{0xe1,2},{0xb2,0},
        {0xde,2},{0x84,0},{0x2a,2},{0x2c,2},{0x2b,2},{0x2d,2},{0xa0,2},
        {0x83,3},{0x87,3},{0xb0,3},{0x74,3},{0x88,2},{0x89,2},{0x8c,2},
        {0x8d,2},{0x8e,2},{0x8f,2},{0x97,2},{0x98,2},{0x9d,2},{0x9f,2},
        {0x9c,2},{0x9e,2},{0x28,2},{0x29,2},{0x7d,1},{0x7e,2},{0x85,1},
        {0x1f,2}
    };
    int i;
    int nResourceId;

    for (i = 0; i < (int)(sizeof(aResources) / sizeof(aResources[0])); ++i) {
        LoadLevelScreenVariantState(
            pObject, aResources[i].m_nResourceId,
            aResources[i].m_nSelectorMode);
    }
    for (nResourceId = 0x35; nResourceId <= 0x3e; ++nResourceId)
        LoadLevelScreenVariantState(pObject, nResourceId, 1);
    for (nResourceId = 0x47; nResourceId <= 0x56; ++nResourceId)
        LoadLevelScreenVariantState(pObject, nResourceId, 1);
    for (nResourceId = 0x5f; nResourceId <= 0x6e; ++nResourceId)
        LoadLevelScreenVariantState(pObject, nResourceId, 1);
    for (nResourceId = 0xa1; nResourceId <= 0xac; ++nResourceId)
        LoadLevelScreenVariantState(pObject, nResourceId, 1);

    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenCompositeSequencePrimaryResourceId, 0);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenCompositeSequenceSecondaryResourceId, 3);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMultiStageSequencePrimaryResourceId, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMultiStageSequenceSecondaryResourceId, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenTimedVariantResourceId, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMultiPhaseSequenceResourceId, 2);
    if (g_nLevelScreenContextSelectedVariantResourceId != 0)
        LoadLevelScreenVariantState(pObject,
            g_nLevelScreenContextSelectedVariantResourceId, 0);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMappedVariantResourceId0x206, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMappedVariantResourceId0x207, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMappedVariantResourceId0x208, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMappedVariantResourceId0x209, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMappedVariantResourceId0x20A, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMappedVariantResourceId0x20B, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMappedVariantResourceId0x20C, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMappedVariantResourceId0x20D, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMappedVariantResourceId0x20E, 2);
    LoadLevelScreenVariantState(pObject,
        g_nLevelScreenMappedVariantResourceId0x20F, 2);
    if (g_nLevelScreenMappedVariantResourceId0x202 != 0)
        LoadLevelScreenVariantState(pObject,
            g_nLevelScreenMappedVariantResourceId0x202, 2);
}

// FUNCTION: LEMBALL 0x004337f0
void RebuildLevelScreenVariantResourceSet(void *pObject, int nMode) {
    int *pManager;
    void *pStatusManager;

    pManager = (int *)pObject;
    pManager[0x2a] = nMode;
    SelectLevelScreenVariantResourceIds(nMode);
    pManager[0x23] = 0;
    pManager[0x24] = 0;
    pManager[0x25] = 1;
    LoadLevelScreenVariantResourceBatch(pObject);
    pManager[0x25] = 0;
    ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
        ->SwitchVariantResourceEntryMode(4, (char *)pObject + 0x70);
    LoadLevelScreenVariantResourceBatch(pObject);

    pStatusManager = (void *)(unsigned long)pManager[0x1e];
    if (pStatusManager != 0) {
        DestroyLevelScreenStatusIndicatorManager(pStatusManager);
        FreeVSMemBlock(pStatusManager);
        pManager[0x1e] = 0;
    }
    ((GamePrimaryContext *)(unsigned long)pManager[0x1f])
        ->ApplyWindowOwnerPaletteResource(0x2e);
    pManager[0x21] = 1;
}

// FUNCTION: LEMBALL 0x004338b0
void LEMBALL_FASTCALL ReleaseLevelSelectionModeRenderStateResource(void *pObject) {
    int *pManager;

    pManager = (int *)pObject;
    if (pManager[0x21] == 0) {
        return;
    }

    ReleaseVariantRenderResource(pObject, 0xf6);
    ReleaseVariantRenderResource(pObject, 0xf7);
    ReleaseVariantRenderResource(pObject, 0xf9);
    ReleaseVariantRenderResource(pObject, 0xfa);

    if (pManager[0x27] != 0) {
        ReleaseTypedResourceObjectReference((void *)(unsigned long)pManager[0x27]);
    }
    if (pManager[0x28] != 0) {
        ReleaseTypedResourceObjectReference((void *)(unsigned long)pManager[0x28]);
    }
    if (pManager[0x29] != 0) {
        ReleaseTypedResourceObjectReference((void *)(unsigned long)pManager[0x29]);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0xfd);
    ReleaseLevelScreenVariantStateWrapperRange(pObject, 0x35, 0x3c);
    ReleaseLevelScreenVariantStateWrapperRange(pObject, 0x47, 0x4e);
    ReleaseLevelScreenVariantStateWrapperRange(pObject, 0x4f, 0x56);
    ReleaseLevelScreenVariantStateWrapperRange(pObject, 0x5f, 0x66);

    if (ReturnTrueVirtualHook() != 0 || ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x90);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x91);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x92);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x93);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6f);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x70);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x71);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x3d);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x3e);

    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapperRange(pObject, 0xa1, 0xa8);
        ReleaseLevelScreenVariantStateWrapperRange(pObject, 0xa9, 0xac);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0x40);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x41);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x42);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xaf);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xad);

    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenCompositeSequencePrimaryResourceId);
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenCompositeSequenceSecondaryResourceId);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x74);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0x7c);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x82);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xae);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x75);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x76);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x43);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x44);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x46);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x45);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x3f);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x67);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x68);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x69);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6a);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6b);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6c);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6d);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x6e);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x80);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x81);

    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x28);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x29);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x7d);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x7e);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x85);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x87);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x86);
    }
    if (pManager[0x2a] != 3 && ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenContextSelectedVariantResourceId);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0xdf);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xe0);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xe1);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xdd);

    if (g_nLevelScreenMappedVariantResourceId0x206 != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x206);
    }
    if (g_nLevelScreenMappedVariantResourceId0x207 != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x207);
    }
    if (g_nLevelScreenMappedVariantResourceId0x208 != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x208);
    }
    if (g_nLevelScreenMappedVariantResourceId0x209 != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x209);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20A != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20A);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20B != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20B);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20C != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20C);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20D != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20D);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20E != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20E);
    }
    if (g_nLevelScreenMappedVariantResourceId0x20F != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x20F);
    }
    if (g_nLevelScreenMappedVariantResourceId0x202 != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMappedVariantResourceId0x202);
    }
    if (g_nLevelScreenMultiStageSequencePrimaryResourceId != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMultiStageSequencePrimaryResourceId);
    }
    if (g_nLevelScreenMultiStageSequenceSecondaryResourceId != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMultiStageSequenceSecondaryResourceId);
    }
    if (g_nLevelScreenTimedVariantResourceId != 0 && ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenTimedVariantResourceId);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0x1f);
    if (ReturnTrueVirtualHook() != 0 || ReturnTrueVirtualHook() != 0 || ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0xb2);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0xde);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x83);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x84);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0xa0);

    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x88);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x89);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x8c);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x8d);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x8e);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x8f);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x97);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, g_nLevelScreenMultiPhaseSequenceResourceId);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x98);
    }
    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x9d);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x9f);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x9c);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x9e);
    }
    if (ReturnTrueVirtualHook() != 0 || ReturnTrueVirtualHook() != 0 || ReturnTrueVirtualHook() != 0 ||
        ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x94);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0x95);
    }

    ReleaseLevelScreenVariantStateWrapper(pObject, 0x96);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x2a);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x2c);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x2b);
    ReleaseLevelScreenVariantStateWrapper(pObject, 0x2d);

    if (ReturnTrueVirtualHook() != 0) {
        ReleaseLevelScreenVariantStateWrapper(pObject, 0xb1);
        ReleaseLevelScreenVariantStateWrapper(pObject, 0xb0);
    }

    ((MogLoadResourceArchive *)g_pMainResourceArchive)
        ->PruneUnreferencedCachedResourceObjects();
}

// FUNCTION: LEMBALL 0x00434d10
static void LEMBALL_FASTCALL UpdateStatusIndicatorLoadProgress00434D10(
    void *pObject)
{
    int nLoaded;
    void *pOwner;

    nLoaded = ++*(int *)((char *)pObject + 0x1c);
    pOwner = *(void **)((char *)pObject + 0x08);
    if (pOwner != 0) {
        LevelStatusProgressClient *pProgressClient =
            (LevelStatusProgressClient *)((char *)pOwner + 0x70);
        pProgressClient->SetLoadProgress(
            (nLoaded * 100) / *(int *)((char *)pObject + 0x20));
    }
}

// FUNCTION: LEMBALL 0x00434ec0
void LevelVariantRenderManagerView::ReleaseLevelScreenVariantStateWrapperBody(int nResourceId) {
    int *pManager;
    unsigned int uStatePointer;

    pManager = (int *)this;
    uStatePointer = *(unsigned int *)(unsigned long)(pManager[0x1d] +
                                                     (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4);
    if (uStatePointer != 0) {
        FreeVSMemBlock((void *)(unsigned long)uStatePointer);
        *(int *)(unsigned long)(pManager[0x1d] +
                                (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4) = 0;
    }
    ReleaseVariantRenderResource(nResourceId);
}

// FUNCTION: LEMBALL 0x00434f00
void LevelVariantRenderManagerView::ReleaseLevelScreenVariantStateWrapperRangeBody(int nFirstResourceId, int nLastResourceId) {
    int nResourceId;
    int *pManager;
    unsigned int uStatePointer;

    nResourceId = nFirstResourceId;
    pManager = (int *)this;
    while (nResourceId <= nLastResourceId) {
        uStatePointer = (unsigned int)*(void **)((char *)pManager[0x1d] +
                                                 (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4);
        if (uStatePointer != 0) {
            FreeVSMemBlock((void *)(unsigned long)uStatePointer);
            *(void **)((char *)pManager[0x1d] +
                       (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4) = 0;
        }
        ReleaseVariantRenderResource(nResourceId);
        ++nResourceId;
    }
}

// FUNCTION: LEMBALL 0x00435870
int LEMBALL_FASTCALL GetTimedVariantFrameStartTime(void *pObject) {
    return *(int *)((char *)pObject + 0x0c);
}

// FUNCTION: LEMBALL 0x00439370
void NoopPackagedSpriteRenderEntry00439370(void) {
}

// FUNCTION: LEMBALL 0x00439380
void PASCAL NoopPackagedSpriteRenderEntry00439380(void *pUnused) {
    (void)pUnused;
}

// FUNCTION: LEMBALL 0x00439400
static void *DeletePackagedSpriteRenderEntry00439400(void *pObject, BYTE fDelete) {
    *(void **)pObject = &g_LEVEL_PackagedSpriteRenderEntryVtableStorage[4];
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004394c0
void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntry(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(void **)pEntry = g_LEVEL_PackagedSpriteRenderEntryVtable;
    *(unsigned short *)(pEntry + 1) = 0;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00439520
void *LEMBALL_FASTCALL InitializeHelperLocalRectUpdateEntryBody(
    void *pObject)

{
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage + 4;
    *(unsigned short *)(pEntry + 1) = 0;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00439550
void *LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntry(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(void **)pEntry = g_LEVEL_StatusIndicatorPointSinkEntryVtable;
    *(unsigned short *)(pEntry + 1) = 0;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00439580
void *LEMBALL_FASTCALL InitializeRenderPointRectSinkEntry(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(unsigned short *)(pEntry + 1) = 0;
    *(void **)pEntry = g_LEVEL_RenderPointRectSinkEntryVtable;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    *(unsigned short *)((char *)pEntry + 0xe) = 0;
    *(unsigned short *)(pEntry + 3) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x004396e0
static void *DeleteRenderPointRectSinkEntry004396E0(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00439710
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableABody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00439720
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableBBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00439730
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableCBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00439740
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableDBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00439750
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableEBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00439800
static void LEMBALL_FASTCALL QueuePackagedSpriteRenderEntry00439800(
    void *pObject, int, void *pQueue)
{
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00439810
static void LEMBALL_FASTCALL DispatchPackagedSpriteRenderEntry00439810(
    void *pObject, int, void *pContext)
{
    int pOwner;
    int pOwnerOffsets;
    int pDispatchBase;
    void *pDispatchObject;

    pOwner = *(int *)((char *)pContext + 0x0c);
    pOwnerOffsets = *(int *)(pOwner + 0x40);
    pDispatchBase = *(int *)(pOwnerOffsets + 4);
    pDispatchObject = (void *)(unsigned long)(pDispatchBase + 0x40 + pOwner);
    ((LevelOneArgVirtualProc)(*(void ***)pDispatchObject)[6])(
        pDispatchObject, 0, pObject);
}

// FUNCTION: LEMBALL 0x004398a0
static void LEMBALL_FASTCALL QueueRenderPointRectSinkEntry004398A0(
    void *pObject, int, void *pQueue)
{
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x004398b0
static void LEMBALL_FASTCALL DispatchRenderPointRectSinkEntry004398B0(
    void *pObject, int, void *pContext)
{
    int pOwner;
    void *pDispatchObject;

    pOwner = *(int *)((char *)pContext + 0x0c);
    pDispatchObject = (void *)(unsigned long)(pOwner + 0x98);
    ((LevelOneArgVirtualProc)(*(void ***)pDispatchObject)[1])(
        pDispatchObject, 0, pObject);
}

// FUNCTION: LEMBALL 0x0043a720
void SetLevelScreenStatusIndicatorMode(int nMode, int nValue) {
    int nResourceId;

    switch (nMode) {
    case 0:
        SetStatusIndicatorActive(
            g_pArrowCursorStatusIndicatorRenderClient, 0, 0);
        SetStatusIndicatorZrleResource(g_pArrowCursorStatusIndicatorRenderClient, g_LEVEL_StatusIndicatorModeResourceIds[0]);
        return;

    case 1:
        if (g_pLevelDemoPlaybackController != 0 && *(int *)((char *)g_pLevelDemoPlaybackController + 0x4c) != 0) {
            nValue = 0;
        }
        SetStatusIndicatorListResourceEntry(g_pArrowCursorStatusIndicatorRenderClient,
                                            g_LEVEL_StatusIndicatorModeResourceIds[1],
                                            nValue);
        if (g_fLevelScreenStatusIndicatorConfigured == 0) {
            *(int *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x2c) = 1;
            g_fLevelScreenStatusIndicatorConfigured = 1;
        }
        SetStatusIndicatorActive(
            g_pArrowCursorStatusIndicatorRenderClient, 0, 1);
        *(unsigned short *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x14) = 3;
        *(unsigned short *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x16) = 3;
        return;

    case 2:
    case 3:
        nResourceId = g_LEVEL_StatusIndicatorModeResourceIds[nMode];
        SetStatusIndicatorZrleResource(g_pArrowCursorStatusIndicatorRenderClient, nResourceId);
        if (g_fLevelScreenStatusIndicatorConfigured == 0) {
            *(int *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x2c) = 1;
            *(unsigned short *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x14) = 3;
            *(unsigned short *)((char *)g_pArrowCursorStatusIndicatorRenderClient + 0x16) = 3;
            g_fLevelScreenStatusIndicatorConfigured = 1;
        }
        SetStatusIndicatorActive(
            g_pArrowCursorStatusIndicatorRenderClient, 0, 1);
        break;
    }
}

// FUNCTION: LEMBALL 0x0043ace0
void LevelScreenTerrainRenderView::EmitRepeatedVariantColumn(
    short x, int y, int nHeight, int cEntries)
{
    char *pScreen;
    int nEntryY;

    pScreen = (char *)this;
    nEntryY = y + cEntries * 0x10 - nHeight;
    while (cEntries > 0) {
        EmitLevelScreenFixedVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(x - 0x10), (short)(nEntryY - 0x10),
            g_nLevelScreenMappedVariantResourceId0x206, 0, 0);
        --cEntries;
        nEntryY -= 0x10;
    }
}

// FUNCTION: LEMBALL 0x0043ad40
void LevelScreenTerrainRenderView::EmitVariantStripPassA(
    int nTileX, int nTileY, int cEntries)
{
    char *pScreen;
    char *pLevel;
    char *pTile;
    int nScreenX;
    int nScreenY;
    int i;
    int nResourceId;
    unsigned int nFrame;
    short nHeight;
    short cRepeated;
    short nDepth;
    short nDepthBias;
    int fDrawTile;

    pScreen = (char *)this;
    pLevel = *(char **)(pScreen + 0x914);
    nScreenX = *(int *)(pScreen + 0x184);
    nScreenY = *(int *)(pScreen + 0x188);
    nDepth = (short)((short)nTileX + (short)nTileY) * 0x40;
    if (nDepth < 0) nDepth = 0;
    for (i = 0; i < cEntries; ++i) {
        nResourceId = *(int *)(pLevel + 0x4c);
        nFrame = *(unsigned int *)(pLevel + 0x50);
        nHeight = 0;
        cRepeated = 0;
        nDepthBias = 0;
        fDrawTile = 1;
        if (nTileX >= 0 && nTileY >= 0 &&
            nTileX < (int)*(unsigned short *)(pScreen + 0x162) &&
            nTileY < (int)*(unsigned short *)(pScreen + 0x164)) {
            pTile = *(char **)(pLevel + 0x0c) +
                    (*(int *)(pLevel + 0x10) * nTileY + nTileX) * 0x0c;
            if ((pTile[6] & 0x20) == 0) {
                nResourceId = *(int *)pTile;
                nFrame = *(unsigned short *)(pTile + 4);
                nHeight = *(short *)(pTile + 8);
                cRepeated = *(short *)(pTile + 0x0a);
                if (nHeight < 0) {
                    nHeight = 0;
                    nResourceId = *(int *)(pLevel + 0x4c);
                    nFrame = *(unsigned short *)(pLevel + 0x50);
                }
                switch (nResourceId) {
                case 0x202: nDepthBias = 0x20; break;
                case 0x206: nDepthBias = 0x10; break;
                case 0x207:
                case 0x20a: nDepthBias = 8; break;
                case 0x210:
                case 0x215:
                case 0x216:
                case 0x219:
                case 0x21a: fDrawTile = 0; break;
                }
                *(short *)(*(char **)(pScreen + 0xa40) + 0x68) =
                    (short)(nDepth + nHeight + nDepthBias);
                if (nHeight > 0) {
                    EmitRepeatedVariantColumn(
                        (short)nScreenX, nScreenY, nHeight, cRepeated);
                }
                if (fDrawTile) {
                    EmitMappedLevelScreenVariantRenderEntry(
                        pScreen, (unsigned short)nScreenX,
                        (short)(nScreenY - nHeight), nResourceId,
                        ((unsigned int)(unsigned short)nDepth << 16) |
                        (nFrame & 0xffff));
                }
            }
        } else {
            *(short *)(*(char **)(pScreen + 0xa40) + 0x68) = nDepth;
            EmitMappedLevelScreenVariantRenderEntry(
                pScreen, (unsigned short)nScreenX, (short)nScreenY,
                nResourceId, nFrame);
        }
        nTileX += *(int *)(pScreen + 0x170);
        nTileY += *(int *)(pScreen + 0x174);
        nScreenX += 0x20;
    }
}

// FUNCTION: LEMBALL 0x0043b0e0
void LevelScreenTerrainRenderView::EmitVariantStripPassB(
    int nTileX, int nTileY, int cEntries)
{
    char *pScreen;
    char *pLevel;
    char *pTile;
    int nScreenX;
    int nScreenY;
    int i;
    int nResourceId;
    unsigned int nFrame;
    short nHeight;
    short cRepeated;
    short nDepth;
    short nDepthBias;
    int fDrawTile;
    int fDrawColumn;

    pScreen = (char *)this;
    pLevel = *(char **)(pScreen + 0x914);
    nScreenX = *(int *)(pScreen + 0x184);
    nScreenY = *(int *)(pScreen + 0x188);
    nDepth = (short)((short)nTileX + (short)nTileY) * 0x40;
    if (nDepth < 0) nDepth = 0;
    for (i = 0; i < cEntries; ++i) {
        nResourceId = *(int *)(pLevel + 0x4c);
        nFrame = *(unsigned int *)(pLevel + 0x50);
        if (nTileX >= 0 && nTileY >= 0 &&
            nTileX < (int)*(unsigned short *)(pScreen + 0x162) &&
            nTileY < (int)*(unsigned short *)(pScreen + 0x164)) {
            pTile = *(char **)(pLevel + 0x0c) +
                    (*(int *)(pLevel + 0x10) * nTileY + nTileX) * 0x0c;
            if ((pTile[6] & 0x20) == 0) {
                nResourceId = *(int *)pTile;
                nFrame = *(unsigned short *)(pTile + 4);
                nHeight = *(short *)(pTile + 8);
                cRepeated = *(short *)(pTile + 0x0a);
                if (nHeight < 0) {
                    nHeight = 0;
                    nResourceId = *(int *)(pLevel + 0x4c);
                    nFrame = *(unsigned short *)(pLevel + 0x50);
                }
                nDepthBias = 0;
                if (nResourceId == 0x202) nDepthBias = 0x20;
                else if (nResourceId == 0x206) nDepthBias = 0x10;
                else if (nResourceId == 0x207 || nResourceId == 0x20a)
                    nDepthBias = 8;
                *(short *)(*(char **)(pScreen + 0xa40) + 0x68) =
                    (short)(nDepth + nHeight + nDepthBias);
                fDrawTile = 0;
                fDrawColumn = 0;
                switch (nResourceId) {
                case 0x202: case 0x206: case 0x207: case 0x208:
                case 0x209: case 0x20a: case 0x20d: case 0x20e:
                case 0x20f: case 0x214: case 0x217:
                    fDrawTile = 1; fDrawColumn = nHeight > 0; break;
                case 0x20b: case 0x20c:
                    fDrawTile = 1; fDrawColumn = nHeight > 0x18; break;
                case 0x210: case 0x215: case 0x216:
                case 0x219: case 0x21a:
                    fDrawColumn = nHeight > 0; break;
                }
                if (fDrawColumn) {
                    EmitRepeatedVariantColumn(
                        (short)nScreenX, nScreenY, nHeight, cRepeated);
                }
                if (fDrawTile) {
                    EmitMappedLevelScreenVariantRenderEntry(
                        pScreen, (unsigned short)nScreenX,
                        (short)(nScreenY - nHeight), nResourceId,
                        ((unsigned int)(unsigned short)nDepth << 16) |
                        (nFrame & 0xffff));
                }
            }
        } else {
            *(short *)(*(char **)(pScreen + 0xa40) + 0x68) = nDepth;
            EmitMappedLevelScreenVariantRenderEntry(
                pScreen, (unsigned short)nScreenX, (short)nScreenY,
                nResourceId, nFrame);
        }
        nTileX += *(int *)(pScreen + 0x170);
        nTileY += *(int *)(pScreen + 0x174);
        nScreenX += 0x20;
    }
}

// FUNCTION: LEMBALL 0x0043b4d0
void LevelScreenTerrainRenderView::BuildStaticRegionEntries(
    const short *pRegion)
{
    static const int s_anTileStepX[4] = { 1, -1, -1, 1 };
    static const int s_anTileStepY[4] = { -1, -1, 1, 1 };
    static const int s_anHalfStepX[4] = { 0, 1, 0, -1 };
    static const int s_anHalfStepY[4] = { 1, 0, -1, 0 };
    static const int s_anNextRowX[4] = { 1, -1, -1, 1 };
    static const int s_anNextRowY[4] = { -1, -1, 1, 1 };
    typedef void (LEMBALL_FASTCALL *QueueProc)(void *, int, void *);
    char *pScreen;
    char *pEntry;
    LevelViewRotationTransform *pTransform;
    int nRotation;
    int nWidth;
    int nHeight;
    int nRegionX;
    int nRegionY;
    int nLeft;
    int nTop;
    int nRight;
    int nBottom;
    int nTileX;
    int nTileY;
    int nStripCount;
    int nRowCount;
    int i;

    pScreen = (char *)this;
    pTransform = (LevelViewRotationTransform *)*(void **)(pScreen + 0x914);
    nRotation = pTransform->m_nRotation04 & 3;
    *(int *)(pScreen + 0x170) = s_anTileStepX[nRotation];
    *(int *)(pScreen + 0x174) = s_anTileStepY[nRotation];
    *(int *)(pScreen + 0x1464) = 0;
    nWidth = pRegion[0];
    nHeight = pRegion[1];
    nRegionX = pRegion[2];
    nRegionY = pRegion[3];
    if (nRegionX + nWidth > *(short *)(pScreen + 0x958))
        nWidth = *(short *)(pScreen + 0x958) - nRegionX;
    if (nRegionY + nHeight > *(short *)(pScreen + 0x95a))
        nHeight = *(short *)(pScreen + 0x95a) - nRegionY;
    if (nWidth <= 0 || nHeight <= 0) return;
    i = *(int *)(pScreen + 0x2188);
    if (i < 210) {
        pEntry = pScreen + 0x1468 + i * 0x10;
        *(short *)(pEntry + 4) = (short)nWidth;
        *(short *)(pEntry + 6) = (short)nHeight;
        *(short *)(pEntry + 8) = (short)nRegionX;
        *(short *)(pEntry + 0x0a) = (short)nRegionY;
        *(int *)(pEntry + 0x0c) = 0;
        ((QueueProc)(*(void ***)pEntry)[1])(
            pEntry, 0, *(void **)(pScreen + 0x970));
        *(int *)(pScreen + 0x2188) = i + 1;
    }

    nLeft = nRegionX - 0x10;
    if (nLeft < -0x10) nLeft = -0x10;
    nTop = nRegionY - 0x18;
    if (nTop < -0x18) nTop = -0x18;
    nRight = nLeft + nWidth + 0x20;
    if (nRight > *(short *)(pScreen + 0x958))
        nRight = *(short *)(pScreen + 0x958);
    nBottom = nTop + nHeight + 0x30;
    if (nBottom > *(short *)(pScreen + 0x95a))
        nBottom = *(short *)(pScreen + 0x95a);
    nStripCount = (nRight - nLeft + 0x1f) / 0x20;

    pTransform->TransformScreenToTile(
        *(int *)(pScreen + 0x918) + nLeft,
        *(int *)(pScreen + 0x91c) + nTop, &nTileX, &nTileY);
    nTileX /= 0x10;
    nTileY /= 0x10;
    pTransform->TransformTileToScreen(
        nTileX << 4, nTileY << 4,
        (int *)(pScreen + 0x184), (int *)(pScreen + 0x188));
    *(int *)(pScreen + 0x184) -= *(int *)(pScreen + 0x918);
    *(int *)(pScreen + 0x188) -= *(int *)(pScreen + 0x91c);
    nRowCount = (nBottom - nTop + 0x0f) / 0x10;
    for (i = 0; i < nRowCount; ++i) {
        EmitVariantStripPassA(nTileX, nTileY, nStripCount + 3);
        *(int *)(pScreen + 0x184) -= 0x10;
        *(int *)(pScreen + 0x188) += 8;
        EmitVariantStripPassA(
            nTileX + s_anHalfStepX[nRotation],
            nTileY + s_anHalfStepY[nRotation], nStripCount + 4);
        nTileX += s_anNextRowX[nRotation];
        nTileY += s_anNextRowY[nRotation];
        *(int *)(pScreen + 0x184) += 0x10;
        *(int *)(pScreen + 0x188) += 8;
    }

    nBottom += *(unsigned short *)(pScreen + 0x8d0);
    pTransform->TransformScreenToTile(
        *(int *)(pScreen + 0x918) + nLeft,
        *(int *)(pScreen + 0x91c) + nTop + nRowCount * 0x10,
        &nTileX, &nTileY);
    nTileX /= 0x10;
    nTileY /= 0x10;
    pTransform->TransformTileToScreen(
        nTileX << 4, nTileY << 4,
        (int *)(pScreen + 0x184), (int *)(pScreen + 0x188));
    *(int *)(pScreen + 0x184) -= *(int *)(pScreen + 0x918);
    *(int *)(pScreen + 0x188) -= *(int *)(pScreen + 0x91c);
    nRowCount = (*(unsigned short *)(pScreen + 0x8d0) + 0x0f) / 0x10;
    for (i = 0; i < nRowCount; ++i) {
        EmitVariantStripPassB(nTileX, nTileY, nStripCount + 3);
        *(int *)(pScreen + 0x184) -= 0x10;
        *(int *)(pScreen + 0x188) += 8;
        EmitVariantStripPassB(
            nTileX + s_anHalfStepX[nRotation],
            nTileY + s_anHalfStepY[nRotation], nStripCount + 4);
        nTileX += s_anNextRowX[nRotation];
        nTileY += s_anNextRowY[nRotation];
        *(int *)(pScreen + 0x184) += 0x10;
        *(int *)(pScreen + 0x188) += 8;
    }

    i = *(int *)(pScreen + 0x21f0);
    if (i < 2) {
        pEntry = pScreen + 0x21f4 + i * 0x10;
        *(short *)(pEntry + 4) = (short)nWidth;
        *(short *)(pEntry + 6) = (short)nHeight;
        *(short *)(pEntry + 8) = (short)nRegionX;
        *(short *)(pEntry + 0x0a) = (short)nRegionY;
        *(short *)(pEntry + 0x0c) = (short)nWidth;
        *(short *)(pEntry + 0x0e) = (short)nHeight;
        ((QueueProc)(*(void ***)pEntry)[1])(
            pEntry, 0, *(void **)(pScreen + 0x970));
        *(int *)(pScreen + 0x21f0) = i + 1;
    }
}

// FUNCTION: LEMBALL 0x0043c200
void LevelScreenTerrainRenderView::EmitObjectVariantRenderEntries(
    unsigned short *pEntry, unsigned int, int fSecondary)
{
    static const int s_anDefaultResources[8] =
        { 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x47 };
    static const int s_anAlternateResources[8] =
        { 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x35 };
    static const int s_anOffsetResources[8] =
        { 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x4f };
    static const int s_anTallResources[8] =
        { 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x67 };
    static const short s_anOffsetX[8] =
        { 11, 8, 10, 16, 19, 26, 26, 14 };
    static const short s_anOffsetY[8] =
        { 9, 5, 5, 4, 4, 6, 9, 13 };
    static const short s_anTallOffsetX[8] =
        { 28, 24, 24, 19, 23, 25, 32, 25 };
    static const short s_anTallOffsetY[8] =
        { 25, 24, 31, 25, 24, 21, 36, 26 };
    static const short s_anModeOffsetX[4] = { 13, 10, 7, 7 };
    static const short s_anModeOffsetY[4] = { 25, 18, 14, 14 };
    static const int s_anModeResources[4] = { 0x42, 0x41, 0x40, 0x40 };
    char *pScreen;
    int nDirection;
    int nSubtype;
    int nResourceId;
    short nOffsetX;
    short nOffsetY;
    int nMode;
    int nPalette;

    pScreen = (char *)this;
    nDirection = (pEntry[0] + *(int *)(pScreen + 0x90c) * 2) & 7;
    nSubtype = *(int *)(pEntry + 0x0c);
    nResourceId = 0;
    nOffsetX = 0;
    nOffsetY = 0;
    switch (nSubtype) {
    case 0: case 1: case 9: case 0x0d: case 0x23:
        nResourceId = s_anDefaultResources[nDirection];
        nOffsetX = 8; nOffsetY = 0x12; break;
    case 2:
        nResourceId = s_anAlternateResources[nDirection];
        nOffsetX = 8; nOffsetY = 0x12; break;
    case 3:
        nResourceId = s_anOffsetResources[nDirection];
        nOffsetX = (short)(s_anOffsetX[nDirection] + 8);
        nOffsetY = (short)(s_anOffsetY[nDirection] + 0x12);
        break;
    case 6:
        nMode = fSecondary == 0 ? pEntry[0x0e] : 0;
        nMode &= 3;
        nResourceId = s_anModeResources[nMode];
        nOffsetX = s_anModeOffsetX[nMode];
        nOffsetY = s_anModeOffsetY[nMode];
        break;
    case 7:
        nResourceId = s_anTallResources[nDirection];
        nOffsetX = s_anTallOffsetX[nDirection];
        nOffsetY = s_anTallOffsetY[nDirection];
        break;
    case 0x0e:
        nResourceId = pEntry[0x0e] == 0 ? 0x3e : 0x3d;
        nOffsetX = 0x0e; nOffsetY = 0x1b; break;
    default:
        return;
    }
    nPalette = fSecondary != 0 ? *(int *)(pScreen + 0x968) : 0;
    EmitLevelScreenVariantEntry(
        *(void **)(pScreen + 0xa40),
        (short)(*(int *)(pEntry + 2) - nOffsetX),
        (short)(*(int *)(pEntry + 4) - nOffsetY),
        nResourceId, *(int *)(pEntry + 0x10),
        *(void **)(pEntry + 0x12), nPalette);
}

// FUNCTION: LEMBALL 0x0043dc70
void LevelScreenTerrainRenderView::DispatchDynamicRenderEntry(
    int iEntry, unsigned short nDepth)
{
    static const int s_anRotatedResources[8] =
        { 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x5f };
    char *pScreen;
    char *pRecord;
    unsigned short *pEntry;
    int nType;
    int nDirection;
    int nSubtype;
    int nFlags;
    int nPalette;
    int nResourceId;
    int nOffsetX;
    int nOffsetY;
    unsigned int nProgress;

    pScreen = (char *)this;
    pEntry = (unsigned short *)(*(char **)(pScreen + 0x95c) +
                                iEntry * 0x4c);
    pRecord = (char *)pEntry;
    *(unsigned short *)(*(char **)(pScreen + 0xa40) + 0x68) = nDepth;
    nType = *(int *)(pEntry + 0x14);
    switch (nType) {
    case 1:
        EmitObjectVariantRenderEntries(pEntry, pEntry[0x16], 1);
        break;
    case 2:
        EmitObjectVariantRenderEntries(pEntry, pEntry[0x16], 0);
        break;
    case 3:
        nDirection = (pEntry[0] + *(int *)(pScreen + 0x90c) * 2) & 7;
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pEntry + 2) - 4),
            (short)(*(int *)(pEntry + 4) - 4),
            s_anRotatedResources[nDirection], 0,
            *(void **)(pEntry + 0x12), 0);
        break;
    case 4:
        nSubtype = *(int *)(pRecord + 0x18);
        nFlags = *(int *)(pRecord + 0x20);
        nPalette = *(short *)(pRecord + 0x1c) != 0 ?
            *(int *)(pScreen + 0x968) : 0;
        if (nSubtype >= 0x18 && nSubtype <= 0x1b) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x28),
                (short)(*(int *)(pRecord + 8) - 0x3c),
                g_nLevelScreenCompositeSequencePrimaryResourceId,
                1, 0, 0);
            if (nSubtype < 0x1a) {
                EmitLevelScreenVariantEntry(
                    *(void **)(pScreen + 0xa40),
                    (short)(*(int *)(pRecord + 4) - 0x28),
                    (short)(*(int *)(pRecord + 8) - 0x3c),
                    g_nLevelScreenCompositeSequencePrimaryResourceId,
                    0, 0, 0);
            } else {
                EmitLevelScreenVariantEntry(
                    *(void **)(pScreen + 0xa40),
                    (short)(*(int *)(pRecord + 4) - 0x28),
                    (short)(*(int *)(pRecord + 8) - 0x3c),
                    g_nLevelScreenCompositeSequenceSecondaryResourceId,
                    nFlags + 0x640, *(void **)(pRecord + 0x24), 0);
            }
            if (nSubtype == 0x19 || nSubtype == 0x1a) {
                EmitLevelScreenVariantEntry(
                    *(void **)(pScreen + 0xa40),
                    (short)(*(int *)(pRecord + 4) - 0x30),
                    (short)(*(int *)(pRecord + 8) - 0x3c),
                    0x74, nFlags, *(void **)(pRecord + 0x24), nPalette);
            }
        }
        break;
    case 5:
        nSubtype = *(int *)(pRecord + 0x18);
        if (nSubtype == 0x18 || nSubtype == 0x19) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 8),
                (short)(*(int *)(pRecord + 8) - 0x10),
                0x7c, 0, *(void **)(pRecord + 0x24), 0);
        } else if (nSubtype == 0x1a) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x10),
                (short)(*(int *)(pRecord + 8) - 0x10),
                0x82, *(int *)(pRecord + 0x20),
                *(void **)(pRecord + 0x24), 0);
        }
        break;
    case 7:
        {
            static const int s_anMode2Resources[8] =
                { 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa1 };
            static const int s_anMode014Resources[8] =
                { 0xa9, 0xa3, 0xaa, 0xa5, 0xab, 0xa7, 0xac, 0xa1 };

            nDirection = (pEntry[0] +
                          *(int *)(pScreen + 0x90c) * 2) & 7;
            nSubtype = *(int *)(pRecord + 0x18);
            if (nSubtype == 2) {
                nResourceId = s_anMode2Resources[nDirection];
            } else if (nSubtype == 0 || nSubtype == 1 || nSubtype == 4) {
                nResourceId = s_anMode014Resources[nDirection];
            } else {
                break;
            }
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 9),
                (short)(*(int *)(pRecord + 8) - 8),
                nResourceId, *(int *)(pRecord + 0x20),
                *(void **)(pRecord + 0x24), 0);
        }
        break;
    case 9:
        nSubtype = *(int *)(pRecord + 0x18);
        if (nSubtype == 0x25) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 10),
                (short)(*(int *)(pRecord + 8) - 0x0f),
                0x7d, 0, *(void **)(pRecord + 0x24), 0);
        } else if (nSubtype == 0x26) {
            nFlags = (*(int *)(pRecord + 0x24) -
                      *(int *)(pRecord + 0x20)) / 0x40;
            if (nFlags > 8) nFlags = 8;
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x0f),
                (short)(*(int *)(pRecord + 8) - 0x11),
                0x7e, nFlags, 0, 0);
        }
        break;
    case 0x0b:
    case 0x0c:
        nPalette = nType == 0x0b ?
            *(int *)(pScreen + 0x70) : *(int *)(pScreen + 0x68);
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - 0x0f),
            (short)(*(int *)(pRecord + 8) - 0x1c),
            0x80, 0, *(void **)(pRecord + 0x24), nPalette);
        break;
    case 0x0d:
        {
            static const short s_anOriginX[3] = { 0x1f, 0x22, 0x22 };
            static const short s_anOriginY[3] = { 0x5a, 0x60, 0x60 };
            int nMode;

            nMode = *(int *)(*(char **)(pScreen + 0x96c) + 0x60);
            if (nMode < 0 || nMode >= 3) break;
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - s_anOriginX[nMode]),
                (short)(*(int *)(pRecord + 8) - s_anOriginY[nMode]),
                g_nLevelScreenContextSelectedVariantResourceId,
                0, 0, 0);
        }
        break;
    case 0x11:
        nSubtype = *(int *)(pRecord + 0x18);
        if (nSubtype == 0x18) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 8),
                (short)(*(int *)(pRecord + 8) - 0x18),
                0x86, *(int *)(pRecord + 0x20),
                *(void **)(pRecord + 0x24), 0);
        } else if (nSubtype == 0x19 || nSubtype == 0x1a) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x22),
                (short)(*(int *)(pRecord + 8) - 0x32),
                0x87, *(int *)(pRecord + 0x20),
                *(void **)(pRecord + 0x24), 0);
        }
        break;
    case 0x12:
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pEntry + 2) - 0x10),
            (short)(*(int *)(pEntry + 4) - 0x10),
            0x81, 0, *(void **)(pEntry + 0x12), 0);
        break;
    case 0x13:
        nSubtype = *(int *)(pRecord + 0x18);
        if (nSubtype >= 0x18 && nSubtype < 0x1b) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 2),
                (short)(*(int *)(pRecord + 8) - 2),
                0x84, 0, 0, 0);
        } else if (nSubtype == 0x1b) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x1e),
                (short)(*(int *)(pRecord + 8) - 0x23),
                0x83, *(int *)(pRecord + 0x20),
                *(void **)(pRecord + 0x24), 0);
        }
        break;
    case 0x14:
        nSubtype = *(int *)(pRecord + 0x18);
        if (nSubtype == 7 || nSubtype == 0x18) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 5),
                (short)(*(int *)(pRecord + 8) - 0x19),
                0xb1, 0, 0, 0);
        } else if (nSubtype == 0x1a) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 5),
                (short)(*(int *)(pRecord + 8) - 0x19),
                0xb0, *(int *)(pRecord + 0x20),
                *(void **)(pRecord + 0x24), 0);
        }
        break;
    case 0x15:
    case 0x16:
    case 0x17:
        if (nType == 0x15) nPalette = 3;
        else if (nType == 0x16) nPalette = 1;
        else nPalette = 4;
        nPalette = nPalette < 4 ? *(int *)(pScreen + 0x64 + nPalette * 4) : 0;
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - 8),
            (short)(*(int *)(pRecord + 8) - 0x20),
            0xb2, 0, 0, nPalette);
        break;
    case 0x18:
        nSubtype = *(int *)(pRecord + 0x18);
        nProgress = (unsigned int)(*(int *)(pRecord + 0x24) -
                                   *(int *)(pRecord + 0x20)) / 0x42;
        if (nSubtype == 0x1f) {
            if (nProgress > 0x28) nProgress = 0x28;
            nFlags = (int)nProgress;
            if (nProgress > 0x20) {
                EmitLevelScreenVariantEntry(
                    *(void **)(pScreen + 0xa40),
                    (short)(*(int *)(pRecord + 4) - 0x30),
                    (short)(*(int *)(pRecord + 8) - 0x28),
                    g_nLevelScreenMultiStageSequencePrimaryResourceId,
                    0x21, 0, 0);
                nFlags = (int)nProgress + 1;
            }
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x30),
                (short)(*(int *)(pRecord + 8) - 0x28),
                g_nLevelScreenMultiStageSequencePrimaryResourceId,
                nFlags, 0, 0);
            if (nProgress > 0x17) {
                if (nProgress > 0x21) nProgress = 0x21;
                EmitLevelScreenVariantEntry(
                    *(void **)(pScreen + 0xa40),
                    (short)(*(int *)(pRecord + 4) - 0x20),
                    (short)(*(int *)(pRecord + 8) + 0x26),
                    0x1f, (int)nProgress - 0x17, 0, 0);
            }
        } else if (nSubtype >= 0x20 && nSubtype <= 0x22) {
            if (nSubtype == 0x20) {
                if (nProgress > 0x0e) nProgress = 0x0e;
                nFlags = (int)nProgress + 1;
            } else if (nSubtype == 0x21) {
                nFlags = 0x0f;
            } else {
                if (nProgress > 7) nProgress = 7;
                nFlags = 8 - (int)nProgress;
            }
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x30),
                (short)(*(int *)(pRecord + 8) - 0x28),
                g_nLevelScreenMultiStageSequenceSecondaryResourceId,
                0, 0, 0);
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x30),
                (short)(*(int *)(pRecord + 8) - 0x28),
                g_nLevelScreenMultiStageSequenceSecondaryResourceId,
                nFlags, 0, 0);
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x20),
                (short)(*(int *)(pRecord + 8) + 0x26),
                0x1f, 10, 0, 0);
        } else if (nSubtype == 0x23) {
            if (nProgress > 0x28) nProgress = 0x28;
            if (0x28 - nProgress >= 0x21) {
                EmitLevelScreenVariantEntry(
                    *(void **)(pScreen + 0xa40),
                    (short)(*(int *)(pRecord + 4) - 0x30),
                    (short)(*(int *)(pRecord + 8) - 0x28),
                    g_nLevelScreenMultiStageSequencePrimaryResourceId,
                    0x21, 0, 0);
                nFlags = 0x29 - (int)nProgress;
            } else {
                nFlags = 0x28 - (int)nProgress;
            }
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x30),
                (short)(*(int *)(pRecord + 8) - 0x28),
                g_nLevelScreenMultiStageSequencePrimaryResourceId,
                nFlags, 0, 0);
            if (nProgress < 0x0d) {
                nFlags = 10;
            } else if (nProgress < 0x17) {
                nFlags = 0x17 - (int)nProgress;
            } else {
                break;
            }
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x20),
                (short)(*(int *)(pRecord + 8) + 0x26),
                0x1f, nFlags, 0, 0);
        }
        break;
    case 0x19:
    case 0x1a:
        nResourceId = nType == 0x19 ? 0x29 : 0x28;
        nSubtype = *(int *)(pRecord + 0x18);
        if (nSubtype == 0x1c) {
            switch (*(short *)(pRecord + 0x1c)) {
            case 0x14: nPalette = -1; break;
            case 0x15: nPalette = 3; break;
            case 0x16: nPalette = 1; break;
            case 0x17: nPalette = 4; break;
            default: nPalette = -1; break;
            }
            if (nPalette >= 0) {
                nPalette = nPalette < 4 ?
                    *(int *)(pScreen + 0x64 + nPalette * 4) : 0;
                EmitLevelScreenVariantEntry(
                    *(void **)(pScreen + 0xa40),
                    (short)(*(int *)(pRecord + 4) - 10),
                    (short)(*(int *)(pRecord + 8) - 0x2c),
                    0xb2, 0, 0, nPalette);
            }
            nFlags = 1;
        } else if (nSubtype == 0x1d || nSubtype == 0x1e) {
            nFlags = 1;
        } else if (nSubtype == 0x20 || nSubtype == 0x22) {
            nFlags = ((*(int *)(pRecord + 0x24) -
                       *(int *)(pRecord + 0x20)) * 0x0f) / 0x400;
            if (nFlags > 7) nFlags = 7;
            nFlags = nSubtype == 0x20 ? nFlags + 1 : 8 - nFlags;
        } else if (nSubtype == 0x21) {
            nFlags = 8;
        } else {
            break;
        }
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - 0x1a),
            (short)(*(int *)(pRecord + 8) - 0x18),
            nResourceId, 0, 0, 0);
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - 0x1a),
            (short)(*(int *)(pRecord + 8) - 0x18),
            nResourceId, nFlags, 0, 0);
        break;
    case 0x1b:
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - 0x10),
            (short)(*(int *)(pRecord + 8) - 0x12),
            0x85, *(int *)(pRecord + 0x20),
            *(void **)(pRecord + 0x24), 0);
        break;
    case 0x1c:
        nPalette = *(short *)(pRecord + 0x1c) != 0 ?
            *(int *)(pScreen + 0x968) : 0;
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - 0x1c),
            (short)(*(int *)(pRecord + 8) - 0x3f),
            0x89, 0, 0, nPalette);
        nSubtype = *(int *)(pRecord + 0x18);
        if (nSubtype == 0x18) {
            nFlags = 0x3f;
        } else if (nSubtype == 0x1a) {
            nProgress = (unsigned int)(
                ((*(int *)(pRecord + 0x24) -
                  *(int *)(pRecord + 0x20)) * 0x0f) / 1000);
            if (nProgress > 0x3e) nProgress = 0x3e;
            nFlags = (int)nProgress + 1;
        } else {
            break;
        }
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - 0x1c),
            (short)(*(int *)(pRecord + 8) - 0x3f),
            0x89, nFlags, 0, nPalette);
        break;
    case 0x1e:
    case 0x2f:
    case 0x30:
    case 0x31:
        if (nType == 0x1e || nType == 0x30) {
            nResourceId = 0x8c;
            nOffsetX = 0x14;
        } else {
            nResourceId = 0x8e;
            nOffsetX = 0x2e;
        }
        nSubtype = *(int *)(pRecord + 0x18);
        if (nSubtype == 0x17 || nSubtype == 0x18) {
            nFlags = 0;
        } else if (nSubtype == 0x19 || nSubtype == 0x1a) {
            nProgress = (unsigned int)(
                ((*(int *)(pRecord + 0x24) -
                  *(int *)(pRecord + 0x20)) * 0x0f) / 1000);
            if (nProgress > 0x11) nProgress = 0x11;
            nFlags = (int)nProgress;
        } else {
            break;
        }
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - nOffsetX),
            (short)(*(int *)(pRecord + 8) - 10),
            nResourceId, nFlags, 0, 0);
        break;
    case 0x1f:
        nSubtype = *(int *)(pRecord + 0x18);
        nPalette = *(short *)(pRecord + 0x1c) != 0 ?
            *(int *)(pScreen + 0x968) : 0;
        if (nSubtype == 0x17 || nSubtype == 0x18) {
            nFlags = 0;
            nPalette = 0;
        } else if (nSubtype == 0x19 || nSubtype == 0x1a) {
            nProgress = (unsigned int)(
                ((*(int *)(pRecord + 0x24) -
                  *(int *)(pRecord + 0x20)) * 0x0f) / 1000);
            if (nProgress > 0x0b) nProgress = 0x0b;
            nFlags = (int)nProgress;
        } else {
            break;
        }
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - 0x31),
            (short)(*(int *)(pRecord + 8) - 0x14),
            g_nLevelScreenTimedVariantResourceId,
            nFlags, 0, nPalette);
        break;
    case 0x20:
        nProgress = (unsigned int)(
            ((*(int *)(pRecord + 0x24) -
              *(int *)(pRecord + 0x20)) * 0x0f) / 1000);
        nFlags = -1;
        if (nProgress < 8) {
            nResourceId = nProgress > 3 ? 4 : (int)nProgress;
        } else if (nProgress < 0x0e) {
            nResourceId = nProgress < 0x0b ?
                (int)nProgress - 3 : 8;
        } else if (nProgress < 0x14) {
            nFlags = 9;
            nResourceId = (int)nProgress - 4;
        } else if (nProgress < 0x20) {
            nFlags = 0x10;
            nResourceId = ((int)nProgress - 0x14 & 1) + 0x11;
        } else if (nProgress <= 0x2a) {
            nResourceId = (int)nProgress - 0x0b;
            if (nResourceId > 0x19) {
                nFlags = 0x1a;
                nResourceId = (int)nProgress - 10;
            }
        } else {
            nFlags = 0x20;
            nResourceId = ((int)nProgress & 1) + 0x21;
        }
        if (nFlags != -1) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x0d),
                (short)(*(int *)(pRecord + 8) - 0x49),
                0x88, nFlags, 0, 0);
        }
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - 0x0d),
            (short)(*(int *)(pRecord + 8) - 0x49),
            0x88, nResourceId, 0, 0);
        break;
    case 0x21:
        nSubtype = *(int *)(pRecord + 0x18);
        if (nSubtype != 3 && nSubtype != 0x1b) break;
        nProgress = (unsigned int)(
            ((*(int *)(pRecord + 0x24) -
              *(int *)(pRecord + 0x20)) * 8) / 1000);
        if (nProgress > 0x39) nProgress = 0;
        if (nProgress < 0x0c) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x19),
                (short)(*(int *)(pRecord + 8) - 0x1b),
                g_nLevelScreenMultiPhaseSequenceResourceId,
                (int)nProgress, 0, 0);
        } else if (nProgress < 0x2f) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x19),
                (short)(*(int *)(pRecord + 8) - 0x1b),
                g_nLevelScreenMultiPhaseSequenceResourceId,
                0x0b, 0, 0);
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x19),
                (short)(*(int *)(pRecord + 8) - 0x1b),
                0x98, (int)nProgress - 0x0c, 0, 0);
        }
        if (nProgress > 0x2e) {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x19),
                (short)(*(int *)(pRecord + 8) - 0x1b +
                        (int)nProgress - 0x2f),
                g_nLevelScreenMultiPhaseSequenceResourceId,
                0x0b, 0, 0);
        }
        break;
    case 0x22:
        nSubtype = *(int *)(pRecord + 0x18);
        if (nSubtype == 0x18) {
            nFlags = 0;
        } else if (nSubtype == 0x1b) {
            nProgress = (unsigned int)(
                ((*(int *)(pRecord + 0x24) -
                  *(int *)(pRecord + 0x20)) * 0x0f) / 1000);
            if (nProgress > 0x0b) nProgress = 0x0b;
            nFlags = (int)nProgress;
        } else {
            break;
        }
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - 0x16),
            (short)(*(int *)(pRecord + 8) - 0x16),
            0x97, nFlags, 0, 0);
        break;
    case 0x26:
    case 0x32:
        if (nType == 0x26) {
            nResourceId = 0x8f;
            nOffsetX = 0x0d;
            nOffsetY = 9;
        } else {
            nResourceId = 0x8d;
            nOffsetX = 0x16;
            nOffsetY = 0x0f;
        }
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - nOffsetX),
            (short)(*(int *)(pRecord + 8) - nOffsetY),
            nResourceId, 0, 0, 0);
        break;
    case 0x27:
    case 0x28:
    case 0x29:
    case 0x2a:
    case 0x2b:
    case 0x2c:
    case 0x2d:
    case 0x2e:
        if (nType == 0x27 || nType == 0x28) nPalette = 3;
        else if (nType == 0x29 || nType == 0x2a) nPalette = 1;
        else if (nType == 0x2b || nType == 0x2c) nPalette = 4;
        else nPalette = 0;
        nPalette = nPalette < 4 ?
            *(int *)(pScreen + 0x64 + nPalette * 4) : 0;
        if ((nType & 1) != 0) {
            nProgress = (unsigned int)(*(int *)(pRecord + 0x24) -
                                       *(int *)(pRecord + 0x20));
            nOffsetX = (int)((nProgress & 0x780) >> 7) - 4;
            if (((nProgress & 0x780) >> 7) > 7)
                nOffsetX = 0x0c - (int)((nProgress & 0x780) >> 7);
            nOffsetY = nOffsetX / 4;
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) + nOffsetX - 0x10),
                (short)(*(int *)(pRecord + 8) + nOffsetY - 0x40),
                0x94, 0, 0, nPalette);
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) + nOffsetX - 9),
                (short)(*(int *)(pRecord + 8) + nOffsetY - 9),
                0x96, 0, 0, 0);
        } else {
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - 0x10),
                (short)(*(int *)(pRecord + 8) - 0x40),
                0x95, 0, 0, nPalette);
        }
        break;
    case 0x34:
        switch (*(int *)(*(char **)(pScreen + 0x96c) + 0x60)) {
        case 0: nFlags = 0x50; break;
        case 1: nFlags = 0x1a; break;
        case 2: nFlags = 0x52; break;
        case 3: nFlags = 0x38; break;
        default: break;
        }
        nSubtype = *(short *)(pRecord + 0x1c);
        if (nSubtype == 0) {
            nResourceId = g_nLevelScreenMappedVariantResourceId0x209;
            nOffsetX = 0x10;
            nOffsetY = 0x1c;
        } else if (nSubtype == 1) {
            nResourceId = 0xa0;
            nOffsetX = 0x11;
            nOffsetY = 0x26;
            nFlags = (g_nLevelFrameClockTimeMs / 0x46) & 7;
        } else {
            break;
        }
        EmitLevelScreenVariantEntry(
            *(void **)(pScreen + 0xa40),
            (short)(*(int *)(pRecord + 4) - nOffsetX),
            (short)(*(int *)(pRecord + 8) - nOffsetY),
            nResourceId, nFlags, 0, 0);
        break;
    case 0x35:
        {
            static const short s_anOffsetX[4] = { 13, 30, 29, 14 };
            static const short s_anOffsetY[4] = { 25, 32, 26, 34 };
            static const int s_anResources[4] =
                { 0x9c, 0x9e, 0x9d, 0x9f };
            unsigned int iVariant;

            iVariant = *(unsigned short *)(pRecord + 0x1c);
            if (iVariant >= 4) break;
            nSubtype = *(int *)(pRecord + 0x18);
            if (nSubtype == 0x18) {
                nFlags = 0;
            } else if (nSubtype == 0x1b) {
                nFlags = ((*(int *)(pRecord + 0x24) -
                           *(int *)(pRecord + 0x20)) * 0x0f) / 1000;
                if (nFlags > 0x0c) nFlags = 0x0c;
            } else {
                break;
            }
            EmitLevelScreenVariantEntry(
                *(void **)(pScreen + 0xa40),
                (short)(*(int *)(pRecord + 4) - s_anOffsetX[iVariant]),
                (short)(*(int *)(pRecord + 8) - s_anOffsetY[iVariant]),
                s_anResources[iVariant], nFlags, 0, 0);
        }
        break;
    }
}

// FUNCTION: LEMBALL 0x0043ff60
static int CompareLevelDynamicRenderEntryDepth(
    const void *pLeft, const void *pRight)
{
    return *(const int *)((const char *)pLeft + 0x48) -
           *(const int *)((const char *)pRight + 0x48);
}

// FUNCTION: LEMBALL 0x0043ff70
void LevelScreenTerrainRenderView::PrepareAndSortDynamicDepthEntries(void)
{
    char *pScreen;
    char *pEntry;
    int cEntries;
    int i;
    int nX;
    int nY;
    int nHeight;

    pScreen = (char *)this;
    cEntries = *(unsigned short *)(pScreen + 0x964);
    if (cEntries <= 0 || cEntries > 200 ||
        *(void **)(pScreen + 0x95c) == 0) return;
    pEntry = *(char **)(pScreen + 0x95c);
    for (i = 0; i < cEntries; ++i, pEntry += 0x4c) {
        *(short *)(pEntry + 0x10) = (short)*(int *)(pEntry + 4);
        *(short *)(pEntry + 0x12) = (short)*(int *)(pEntry + 8);
        nX = *(int *)(pEntry + 4);
        nY = *(int *)(pEntry + 8);
        nHeight = *(int *)(pEntry + 0x0c);
        ((LevelViewRotationTransform *)*(void **)(pScreen + 0x914))
            ->TransformTileInPlace(&nX, &nY);
        *(int *)(pEntry + 4) = nX - *(int *)(pScreen + 0x918);
        *(int *)(pEntry + 8) =
            nY - nHeight - *(int *)(pScreen + 0x91c);
        *(int *)(pEntry + 0x0c) = nHeight;
        if (*(int *)(pEntry + 0x34) != 0) {
            *(int *)(pScreen + 0x2214) = 1;
        }
        *(int *)(pEntry + 0x48) =
            (unsigned short)ComputeDynamicRenderEntrySortKey(i);
    }
    qsort(*(void **)(pScreen + 0x95c), cEntries, 0x4c,
          CompareLevelDynamicRenderEntryDepth);
}

// FUNCTION: LEMBALL 0x00440490
void LevelScreenTerrainRenderView::DrawStaticRenderEntry(
    int iEntry, unsigned short nDepth)
{
    char *pScreen;
    unsigned short *pEntry;
    int *pTile;
    unsigned short nHeight;
    short cRepeatedEntries;
    unsigned short nFlags;
    int nScreenX;
    int nScreenY;
    unsigned int nFrameValue;

    pScreen = (char *)this;
    pEntry = (unsigned short *)(unsigned long)(
        *(int *)(pScreen + 0x2220) + iEntry * 0x0c);
    pTile = *(int **)(pEntry + 4);
    nHeight = *(unsigned short *)(pTile + 2);
    cRepeatedEntries = *(short *)((char *)pTile + 0x0a);
    nFlags = *(unsigned short *)((char *)pTile + 6);
    ((LevelViewRotationTransform *)*(void **)(pScreen + 0x914))
        ->TransformTileToScreen(
            (int)pEntry[0] << 4, (int)pEntry[1] << 4,
            &nScreenX, &nScreenY);
    nScreenX -= *(int *)(pScreen + 0x918);
    nScreenY -= *(int *)(pScreen + 0x91c);
    *(unsigned short *)(*(char **)(pScreen + 0xa40) + 0x68) = nDepth;
    if ((nFlags & 0x20) != 0 && nHeight != 0) {
        EmitRepeatedVariantColumn(
            (short)nScreenX, nScreenY, nHeight, cRepeatedEntries);
    }
    nFrameValue = ((unsigned int)(unsigned short)nScreenY << 16) |
                  *(unsigned short *)(pTile + 1);
    EmitMappedLevelScreenVariantRenderEntry(
        pScreen, (unsigned short)nScreenX,
        (short)(nScreenY - nHeight), pTile[0], nFrameValue);
}

void LevelScreenTerrainRenderView::DrawStaticDepthEntries(void)
{
    char *pScreen;
    char *pLevelMode;
    int *pList;
    char *pEntries;
    int cEntries;
    int iStatic;
    int iDynamic;
    int cDynamic;
    char *pEntry;
    char *pTile;
    unsigned short nStaticDepth;
    unsigned short nDynamicDepth;

    pScreen = (char *)this;
    pLevelMode = *(char **)(pScreen + 0x96c);
    pList = pLevelMode != 0 ? *(int **)(pLevelMode + 0x8c) : 0;
    pEntries = pList != 0 ? (char *)(unsigned long)pList[0] : 0;
    cEntries = pList != 0 ? pList[1] : 0;
    if (pEntries == 0 || cEntries < 0 || cEntries > 0x10000)
        cEntries = 0;
    cDynamic = *(unsigned short *)(pScreen + 0x964);
    if (cDynamic > 200) cDynamic = 0;
    *(char **)(pScreen + 0x2220) = pEntries;
    *(int *)(pScreen + 0x2224) = cEntries;
    *(int *)(*(char **)(pScreen + 0xa40) + 0xac) = 0x80000;
    iStatic = 0;
    iDynamic = 0;
    while (iStatic < cEntries && iDynamic < cDynamic) {
        pEntry = pEntries + iStatic * 0x0c;
        pTile = *(char **)(pEntry + 8);
        if (pTile == 0) {
            ++iStatic;
            continue;
        }
        nStaticDepth = (unsigned short)(
            *(short *)(pEntry + 4) + *(short *)(pTile + 8));
        nDynamicDepth = (unsigned short)*(int *)(
            *(char **)(pScreen + 0x95c) + iDynamic * 0x4c + 0x48);
        if (nStaticDepth < nDynamicDepth) {
            DrawStaticRenderEntry(iStatic++, nStaticDepth);
        } else {
            DispatchDynamicRenderEntry(iDynamic++, nDynamicDepth);
        }
    }
    while (iDynamic < cDynamic) {
        nDynamicDepth = (unsigned short)*(int *)(
            *(char **)(pScreen + 0x95c) + iDynamic * 0x4c + 0x48);
        DispatchDynamicRenderEntry(iDynamic++, nDynamicDepth);
    }
    while (iStatic < cEntries) {
        pEntry = pEntries + iStatic * 0x0c;
        pTile = *(char **)(pEntry + 8);
        if (pTile != 0) {
            nStaticDepth = (unsigned short)(
                *(short *)(pEntry + 4) + *(short *)(pTile + 8));
            DrawStaticRenderEntry(iStatic, nStaticDepth);
        }
        ++iStatic;
    }
}

// FUNCTION: LEMBALL 0x004407e0
short LevelScreenTerrainRenderView::ComputeDynamicRenderEntrySortKey(
    int iEntry)
{
    char *pScreen;
    char *pEntry;
    int nType;
    int nTileX;
    int nTileY;
    int nHeight;
    int nDepth;
    char *pLevel;
    short nSampleA;
    short nSampleB;
    short nSampleC;
    int nThreshold;
    int fSolidA;
    int fSolidB;
    int fSolidC;
    int fIgnoreTerrain;

    pScreen = (char *)this;
    pEntry = *(char **)(pScreen + 0x95c) + iEntry * 0x4c;
    nType = *(int *)(pEntry + 0x28);
    if (nType == 0x18 || nType == 0x1f) return 0x7fff;
    nTileX = *(short *)(pEntry + 0x10);
    nTileY = *(short *)(pEntry + 0x12);
    fIgnoreTerrain = 0;
    if (nType == 0x19 || nType == 0x1a) {
        nTileX += 0x10;
        nTileY += 0x10;
        fIgnoreTerrain = 1;
    } else if (nType == 0x22) {
        fIgnoreTerrain = 1;
    } else if (nType == 0x34) {
        nTileX = (nTileX - 8) & ~0x0f;
        nTileY = (nTileY - 8) & ~0x0f;
    }
    nHeight = *(short *)(pEntry + 0x0c);
    nDepth = ((nTileX / 0x10) + (nTileY / 0x10)) * 0x40 + 4;
    if (fIgnoreTerrain) return (short)(nDepth + nHeight + 1);
    pLevel = *(char **)(pScreen + 0x914);
    if (pLevel == 0 || *(void **)(pLevel + 0x0c) == 0)
        return (short)(nDepth + nHeight + 1);
    nSampleA = GetLevelTileHeightSample(
        pLevel, nTileX, nTileY + 0x10,
        nTileX & 0x0f, nTileY & 0x0f);
    nSampleB = GetLevelTileHeightSample(
        pLevel, nTileX + 0x10, nTileY,
        nTileX & 0x0f, nTileY & 0x0f);
    nSampleC = GetLevelTileHeightSample(
        pLevel, nTileX + 0x10, nTileY + 0x10,
        nTileX & 0x0f, nTileY & 0x0f);
    nThreshold = nHeight - 0x16;
    nTileX /= 0x10;
    nTileY /= 0x10;
    fSolidA = nSampleA >= nThreshold &&
              GetLevelTileSolidFlag(pLevel, nTileX, nTileY + 1);
    fSolidB = nSampleB >= nThreshold &&
              GetLevelTileSolidFlag(pLevel, nTileX + 1, nTileY);
    fSolidC = nSampleC >= nThreshold &&
              GetLevelTileSolidFlag(pLevel, nTileX + 1, nTileY + 1);
    if (nHeight + 2 < nSampleA && nHeight + 2 < nSampleB)
        return (short)(nDepth + nHeight + 1);
    if (nHeight + 2 >= nSampleA &&
        nHeight + 2 >= nSampleB &&
        nHeight + 2 >= nSampleC &&
        !fSolidA && !fSolidB && !fSolidC)
        nDepth += 0x80;
    else
        nDepth += 0x40;
    return (short)(nDepth + nHeight + 1);
}

void RenderReconstructedLevelScreenTerrain(void *pObject)
{
    char *pScreen;
    char *pLevel;
    void *pManager;
    short Region[4];

    pScreen = (char *)pObject;
    pLevel = *(char **)(pScreen + 0x914);
    pManager = *(void **)(pScreen + 0xa40);
    if (pLevel == 0 || pManager == 0 ||
        *(void **)(pLevel + 0x0c) == 0 ||
        *(int *)(pLevel + 0x10) <= 0 ||
        *(int *)(pLevel + 0x14) <= 0 ||
        *(short *)(pScreen + 0x958) <= 0 ||
        *(short *)(pScreen + 0x95a) <= 0) {
        return;
    }
    ((LevelVariantRenderManagerView *)pManager)
        ->ResetOrFlipVariantRenderBuffers();
    if (*(MappedListResourceTable **)(pScreen + 0x78) != 0) {
        (*(MappedListResourceTable **)(pScreen + 0x78))->ResetDrawEntryPool();
    }
    *(unsigned short *)(pScreen + 0x8d0) = 0x40;
    *(int *)(pScreen + 0x2188) = 0;
    *(int *)(pScreen + 0x21f0) = 0;
    *(int *)((char *)pManager + 0xac) = 0x40000;
    Region[0] = *(short *)(pScreen + 0x958);
    Region[1] = *(short *)(pScreen + 0x95a);
    Region[2] = 0;
    Region[3] = 0;
    ((LevelScreenTerrainRenderView *)pScreen)->BuildStaticRegionEntries(Region);
    ((LevelScreenTerrainRenderView *)pScreen)
        ->PrepareAndSortDynamicDepthEntries();
    ((LevelScreenTerrainRenderView *)pScreen)->DrawStaticDepthEntries();
    *(int *)((char *)pManager + 0xac) = 0;
}

void DrawReconstructedLevelScreenStatusText(void *pObject)
{
    static DWORD s_dwDemoBlinkStart = 0;
    static int s_fDemoBlinkInitialized = 0;
    static int s_fDemoBlinkVisible = 0;
    static char s_szDemoStatus[] = "Demo";
    char *pScreen;
    char *pMode;
    MappedListResourceTable *pMappedList;
    short RelativePoint[2];
    char szTimer[5];
    char szCounter[8];
    int nBaseTime;
    int nTime;
    int nTargetCounter;
    int nCounter;
    int i;

    pScreen = (char *)pObject;
    pMode = *(char **)(pScreen + 0x96c);
    pMappedList = *(MappedListResourceTable **)(pScreen + 0x78);
    if (pMode == 0 || pMappedList == 0 ||
        *(void **)(pScreen + 0x970) == 0) return;

    RelativePoint[0] = -4;
    RelativePoint[1] = 0;
    if (g_pLevelDemoPlaybackController != 0 &&
        *(int *)((char *)g_pLevelDemoPlaybackController + 0x4c) != 0) {
        DWORD dwNow;

        if (s_fDemoBlinkInitialized == 0) {
            s_fDemoBlinkInitialized = 1;
            s_dwDemoBlinkStart = timeGetTime();
        }
        dwNow = timeGetTime();
        if (dwNow - s_dwDemoBlinkStart > 500) {
            s_fDemoBlinkVisible = s_fDemoBlinkVisible == 0;
            s_dwDemoBlinkStart = timeGetTime();
        }
        if (s_fDemoBlinkVisible != 0) {
            RelativePoint[0] = 0;
            pMappedList->SubmitRenderEntry(
                *(void **)(pScreen + 0x970),
                (short *)(pScreen + 0x8e6), RelativePoint,
                0xf8, s_szDemoStatus, 0x20,
                *(int *)(pScreen + 0x74));
        }
        return;
    }

    nBaseTime = *(unsigned short *)(pMode + 0xe4);
    nTime = nBaseTime + *(int *)(pMode + 0xe8);
    if (nTime < 0) nTime = 0;
    if (nTime <= 599 || nBaseTime <= 599) {
        if (nTime > 599) nTime = 599;
        szTimer[0] = (char)(nTime / 60) + '0';
        szTimer[1] = ':';
        szTimer[2] = (char)((nTime % 60) / 10) + '0';
        szTimer[3] = (char)(nTime % 10) + '0';
        szTimer[4] = '\0';
        pMappedList->SubmitRenderEntry(
            *(void **)(pScreen + 0x970),
            (short *)(pScreen + 0x8d6), RelativePoint,
            0x115, szTimer, 0x20, 0);
    }

    nTargetCounter = *(int *)(pMode + 0xf0);
    nCounter = *(int *)(pScreen + 0x8f0);
    if ((unsigned int)*(int *)(pScreen + 0x8f8) <=
        (unsigned int)g_nLevelFrameClockTick) {
        if (nCounter != nTargetCounter) {
            nCounter += *(int *)(pMode + 0x108) == 2 ? 10 : 100;
            if (nCounter > nTargetCounter) nCounter = nTargetCounter;
            *(int *)(pScreen + 0x8f0) = nCounter;
        }
        *(int *)(pScreen + 0x8f8) = g_nLevelFrameClockTick + 1;
    }
    if (nCounter > 9999999) nCounter = 9999999;
    szCounter[7] = '\0';
    for (i = 6; i >= 0; --i) {
        szCounter[i] = (char)(nCounter % 10) + '0';
        nCounter /= 10;
    }
    pMappedList->SubmitRenderEntry(
        *(void **)(pScreen + 0x970),
        (short *)(pScreen + 0x8de), RelativePoint,
        0x115, szCounter, 0x20, 0);
}

// FUNCTION: LEMBALL 0x004439e0
void LEMBALL_FASTCALL ResetBaseModeActionButtonPointSink(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x004452e0
void *LEMBALL_FASTCALL DestroyBaseModeActionButtonPointSinkArray(
    void *pObject, int, unsigned int fDelete)

{
    int cEntries;
    char *pEntry;

    if ((fDelete & 2) == 0) {
        ResetBaseModeActionButtonPointSink(pObject);
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pObject);
        }
        return pObject;
    }

    cEntries = *((int *)pObject - 1);
    pEntry = (char *)pObject + cEntries * 0x20;
    while (cEntries-- > 0) {
        pEntry -= 0x20;
        ResetBaseModeActionButtonPointSink(pEntry);
    }
    FreeVSMemBlock((char *)pObject - 4);
    return pObject;
}

// FUNCTION: LEMBALL 0x004465e0
void GameBaseModeScreen::QueueActiveBaseModeUserAction(int nAction, int nPhase) {
    m_fActionBusy90 = 1;
    ((GameBaseModeObject *)g_pActiveNetworkLobbyTransportController)
        ->QueueBaseModeUserActionThunk(nAction, nPhase);
}

// FUNCTION: LEMBALL 0x00446610
void GameBaseModeScreen::DispatchBaseModeUserAction(int nAction, int nPhase) {
    switch (nPhase) {
    case 0:
        if (m_fActionBusy90 != 0) {
            QueueActiveBaseModeUserActionThunk(nAction, 2);
            return;
        }

        m_fActionBusy90 = 1;
        QueueActiveBaseModeUserActionThunk(nAction, 1);
        ((GameVariantResourceEntryManager *)g_pVariantResourceEntryManager)
            ->PlayVariantResourceEffectThunk(0x25);
        if (HandleUserAction(nAction) == 0) {
            g_pErrorOutputStream->AppendCStringToStream(
                g_szUnknownUserActionSpecified);
        }
        m_fActionBusy90 = 0;
        return;
    case 1:
        if (HandleUserAction(nAction) == 0) {
            g_pErrorOutputStream->AppendCStringToStream(
                g_szUnknownUserActionReceived);
        }
        m_fActionBusy90 = 0;
        return;
    case 2:
        m_fActionBusy90 = 0;
        return;
    default:
        return;
    }
}

// FUNCTION: LEMBALL 0x00446860
void GameBaseModeObject::QueueBaseModeUserAction(int nAction, int nPhase) {
    DWORD dwStart;

    if (m_pActionStream20->m_fWritePending != 0) {
        dwStart = timeGetTime();
        while (m_pActionStream20->m_fWritePending != 0 &&
               timeGetTime() - dwStart < 2000) {
            ((GameRuntimeServiceInterface *)g_pActiveNetworkRuntimeWindow)
                ->ServiceRuntime();
        }
    }

    m_pActionStream20->m_nU32Payload = nAction;
    m_pActionStream20->m_nActionPhase30 = nPhase;
    m_pActionStream20->QueueEffStreamWriteEvent(
        g_nSelectedNetworkLobbyPeerId);
}

// FUNCTION: LEMBALL 0x004468d0
int GameBaseModeEventClient::HandleBaseModeActionButtonEvent(VsNetEffDispatchEvent *pEvent) {
    GameBaseModeObject *pMode;
    VsNetEffDispatchEventPayloadView *pPayload;
    int nSerializedStream;
    int nEventType;
    int nStreamEvent;
    void *pPeer;
    int nCode;

    nCode = pEvent->m_nCode;
    if (g_pActiveNetworkLobbyScreen == 0) {
        return 0;
    }

    pMode = (GameBaseModeObject *)((char *)this - 0x0c);
    if (pMode->HandleTransportEvent(pEvent) != 0) {
        goto handled;
    }
    nEventType = pEvent->m_nType;
    if (nEventType != 5) {
        return 0;
    }

    pPeer = pEvent->m_pPeer0C;
    pPayload = pEvent->m_pPayload10;
    if (nCode != 0) {
        return 1;
    }

    nSerializedStream = pPayload->m_pSerializedStream04;
    nStreamEvent =
        ((VsNetEffTransportPacketHeader *)(unsigned long)
             nSerializedStream)
            ->m_nStreamEvent08;
    if (nStreamEvent != 8) {
        return pMode->HandleTransportPayload(
            nStreamEvent, pPayload, pPeer);
    }

    (*(GameBaseModeActionStream **)((char *)this + 0x14))
        ->LoadEffStreamFromMemory(nSerializedStream + 0x10);
    pPayload->m_pOwnedPayload24 = 0;
    ((GameBaseModeScreen *)g_pActiveNetworkLobbyScreen)
        ->DispatchBaseModeUserActionThunk(
            (*(GameBaseModeActionStream **)((char *)this + 0x14))
                ->m_nU32Payload,
            (*(GameBaseModeActionStream **)((char *)this + 0x14))
                ->m_nActionPhase30);
handled:
    return 1;
}

// FUNCTION: LEMBALL 0x00447030
void *LEMBALL_FASTCALL ConstructFramedScreenRenderEntryGroup(void *pObject) {
    int *pGroup;
    int i;

    pGroup = (int *)pObject;
    *(void **)pGroup = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    *(unsigned short *)(pGroup + 1) = 0;
    *(unsigned short *)((char *)pGroup + 6) = 0;
    *(void **)pGroup = g_LEVEL_RenderPointRectSinkEntryVtable;
    *(unsigned short *)(pGroup + 2) = 0;
    *(unsigned short *)((char *)pGroup + 10) = 0;
    *(unsigned short *)(pGroup + 3) = 0;
    *(unsigned short *)((char *)pGroup + 14) = 0;

    *(void **)(pGroup + 4) = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    *(unsigned short *)(pGroup + 5) = 0;
    *(unsigned short *)((char *)pGroup + 22) = 0;
    *(void **)(pGroup + 4) = g_LEVEL_CompositePointRectSinkEntryInitVtable;
    *(unsigned short *)(pGroup + 6) = 0;
    *(unsigned short *)((char *)pGroup + 26) = 0;
    *(void **)(pGroup + 4) = g_LEVEL_FramedScreenRenderChildEntryVtable;
    *(unsigned short *)(pGroup + 7) = 0;
    *(unsigned short *)((char *)pGroup + 30) = 0;
    *(unsigned short *)(pGroup + 12) = 0;
    *(unsigned short *)((char *)pGroup + 50) = 0;

    for (i = 0; i < 10; ++i) {
        InitializeFramedScreenRenderChildEntry(
            (char *)pObject + 0x34 + i * 0x24);
    }
    for (i = 0; i < 2; ++i) {
        InitializeHelperLocalRectUpdateEntry(
            (char *)pObject + 0x19c + i * 0x10);
    }

    *(void **)((char *)pObject + 0x1bc) =
        g_pQueuedRenderPointSinkFinalizeThunk;
    for (i = 0; i < 10; ++i) {
        InitializePackagedSpriteRenderEntry(
            (char *)pObject + 0x1c0 + i * 0x10);
    }
    return pObject;
}

void *LEMBALL_FASTCALL InitializeRenderRectSinkEntryA(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(void **)pEntry = g_LEVEL_RenderRectSinkEntryAVtable;
    *(unsigned short *)(pEntry + 2) = 0;
    *(unsigned short *)((char *)pEntry + 14) = 0;
    *(unsigned short *)(pEntry + 3) = 0;
    return pObject;
}

void *LEMBALL_FASTCALL InitializeRenderRectSinkEntryB(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(void **)pEntry = g_LEVEL_RenderRectSinkEntryBVtable;
    *(unsigned short *)(pEntry + 2) = 0;
    *(unsigned short *)((char *)pEntry + 14) = 0;
    *(unsigned short *)(pEntry + 3) = 0;
    return pObject;
}

void *LEMBALL_FASTCALL InitializeLevelSurfaceRectSink(void *pObject) {
    memset(pObject, 0, 0x10);
    *(void ***)pObject = g_LEVEL_SurfaceRectSinkVtable;
    return pObject;
}

void *LEMBALL_FASTCALL InitializeLevelNestedRectSink(void *pObject) {
    memset(pObject, 0, 0x10);
    *(void ***)pObject = g_LEVEL_NestedRectSinkVtable;
    return pObject;
}

void LEMBALL_FASTCALL DestroyFramedScreenRenderEntryGroup(void *pObject) {
    int i;
    char *pEntry;

    pEntry = (char *)pObject + 0x1c0 + 10 * 0x10;
    for (i = 0; i < 10; ++i) {
        pEntry -= 0x10;
        RestorePackagedSpriteRenderEntryVtableA(pEntry);
    }
    *(void **)((char *)pObject + 0x1bc) =
        g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    pEntry = (char *)pObject + 0x19c + 2 * 0x10;
    for (i = 0; i < 2; ++i) {
        pEntry -= 0x10;
        RestorePackagedSpriteRenderEntryVtableC(pEntry);
    }
    pEntry = (char *)pObject + 0x34 + 10 * 0x24;
    for (i = 0; i < 10; ++i) {
        pEntry -= 0x24;
        RestoreFramedScreenRenderChildEntryVtableBody(pEntry);
    }
    *(void **)((char *)pObject + 0x10) =
        g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    *(void **)pObject =
        g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00447120
void *LEMBALL_FASTCALL InitializeFramedScreenRenderChildEntryBody(
    void *pObject)

{
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(unsigned short *)(pEntry + 1) = 0;
    *(void **)pEntry = g_LEVEL_CompositePointRectSinkEntryInitVtable;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    *(void **)pEntry = g_LEVEL_FramedScreenRenderChildEntryVtable;
    *(unsigned short *)((char *)pEntry + 0xe) = 0;
    *(unsigned short *)(pEntry + 3) = 0;
    *(unsigned short *)((char *)pEntry + 0x22) = 0;
    *(unsigned short *)(pEntry + 8) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00447170
static void *DeleteFramedScreenRenderChildEntryA(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004471a0
static void *DeleteFramedScreenRenderChildEntryB(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00447260
void LEMBALL_FASTCALL RestoreFramedScreenRenderChildEntryVtableBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x004472e0
static void *DeleteBaseModeObjectAdjusted(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x0c);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00447300
static void LEMBALL_FASTCALL QueueFramedScreenRenderEntry(
    void *pObject, int, void *pQueue)
{
    int *pOwner;

    pOwner = *(int **)((char *)pObject + 0x10);
    if (pOwner[4] == 0) {
        ((LevelNoArgVirtualProc)(*(void ***)pOwner)[7])(pOwner, 0);
    } else {
        pOwner[9] = 0;
    }
    ++pOwner[2];
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00447340
static void LEMBALL_FASTCALL FinalizeFramedScreenRenderEntry(
    void *pObject, int, void *pManager)
{
    int *pOwner;
    int pOwnerOffsets;
    int pDispatchBase;
    void *pDispatchObject;

    pOwner = *(int **)((char *)pObject + 0x10);
    pOwnerOffsets = *(int *)(*(int *)((char *)pManager + 0x0c) + 0x40);
    pDispatchBase = *(int *)(pOwnerOffsets + 4);
    pDispatchObject = (void *)(unsigned long)(
        pDispatchBase + 0x40 + *(int *)((char *)pManager + 0x0c));
    ((LevelTwoArgVirtualProc)(*(void ***)pDispatchObject)[8])(
        pDispatchObject, 0, pObject, pOwner);
    --pOwner[2];
}

// FUNCTION: LEMBALL 0x00447380
static void LEMBALL_FASTCALL QueueBitmapRenderEntry(
    void *pObject, int, void *pQueue)
{
    int *pOwner;

    pOwner = *(int **)((char *)pObject + 0x10);
    if (pOwner[4] == 0) {
        ((LevelNoArgVirtualProc)(*(void ***)pOwner)[7])(pOwner, 0);
    } else {
        pOwner[9] = 0;
    }
    ++pOwner[2];
    *(int *)((char *)pOwner + 0x50) = 1;
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x004473d0
static void LEMBALL_FASTCALL RenderBitmapEntryViaContext(
    void *pObject, int, void *pManager)
{
    int *pOwner;
    int pOwnerOffsets;
    int pDispatchBase;
    void *pDispatchObject;

    pOwner = *(int **)((char *)pObject + 0x10);
    pOwnerOffsets = *(int *)(*(int *)((char *)pManager + 0x0c) + 0x40);
    pDispatchBase = *(int *)(pOwnerOffsets + 4);
    pDispatchObject = (void *)(unsigned long)(
        pDispatchBase + 0x40 + *(int *)((char *)pManager + 0x0c));
    ((LevelTwoArgVirtualProc)(*(void ***)pDispatchObject)[7])(
        pDispatchObject, 0, pObject, pOwner);
    *(int *)((char *)pOwner + 0x50) = 0;
    --pOwner[2];
}

// FUNCTION: LEMBALL 0x0044a9c0
void LEMBALL_FASTCALL RestoreLevelSelectionRenderChildEntryVtableBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x0044aa80
void *ConstructLevelScreenStatusIndicatorManager(void *pObject, int nStatusMode, void *pPrimaryContext) {
    int *pManager;
    unsigned int *puPointTable;
    int i;
    int nFrameCount;
    int *pSelector;
    short *paPoint;

    pManager = (int *)pObject;

    ((LevelVariantRenderManagerView *)pObject)->Construct(
        nStatusMode, 0x2b6, 1, 1, 0, 0);
    *(void **)(pManager + 0x1c) = g_LEVEL_StatusIndicatorManagerSlot70Vtable;
    *(void **)(pManager + 0x1d) = g_LEVEL_StatusIndicatorManagerSlot74Vtable;
    *(unsigned short *)((char *)pManager + 0x8a) = 0;
    *(unsigned short *)((char *)pManager + 0x88) = 0;

    for (i = 0; i < 1; ++i) {
        InitializeFramedScreenRenderChildEntry((char *)pManager + 0x9c + i * 0x24);
    }
    for (i = 0; i < 1; ++i) {
        InitializeLevelSelectionPointRectSinkEntry((char *)pManager + 0xc0 + i * 0x1c);
    }
    for (i = 0; i < 1; ++i) {
        InitializePackagedSpriteRenderEntry((char *)pManager + 0xe0 + i * 0x10);
    }
    for (i = 0; i < 1; ++i) {
        InitializeStatusIndicatorPointSinkEntry((char *)pManager + 0xf0 + i * 0x10);
        InitializeStatusIndicatorPointSinkEntry((char *)pManager + 0x100 + i * 0x10);
        InitializeStatusIndicatorPointSinkEntry((char *)pManager + 0x110 + i * 0x10);
    }

    *(void **)((char *)pManager + 0x120) = g_pQueuedRenderPointSinkFinalizeThunk;
    for (i = 0; i < 1; ++i) {
        InitializeRenderPointRectSinkEntry((char *)pManager + 0x124 + i * 0x10);
    }

    *(void **)pManager = g_LEVEL_StatusIndicatorManagerVtable;
    *(void **)(pManager + 0x1f) = pPrimaryContext;
    *(void **)(pManager + 0x1c) = g_LEVEL_StatusIndicatorManagerSlot70Vtable;
    *(void **)(pManager + 0x1d) = g_LEVEL_StatusIndicatorManagerSlot74Vtable;
    pManager[0x20] = nStatusMode;

    paPoint = (short *)AllocateVSMemBlock(0x14);
    if (paPoint == 0) {
        pManager[0x1e] = 0;
    } else {
        for (i = 0; i < 5; ++i) {
            paPoint[i * 2] = 0;
            paPoint[i * 2 + 1] = 0;
        }
        pManager[0x1e] = (int)(unsigned long)paPoint;
    }

    SetStatusIndicatorActive(
        g_pArrowCursorStatusIndicatorRenderClient, 0, 0);
    if (g_fCompactPrimaryContextLayout == 0) {
        pManager[0x25] = (int)(unsigned long)LoadBitmapResource(0xf1);
        puPointTable = g_LEVEL_StandardStatusIndicatorPointTable;
        pManager[0x26] = (int)(unsigned long)LoadBitmapResource(0xf2);
        pManager[0x21] = 0xf3;
    } else {
        pManager[0x25] = (int)(unsigned long)LoadBitmapResource(0xee);
        puPointTable = g_LEVEL_CompactStatusIndicatorPointTable;
        pManager[0x26] = (int)(unsigned long)LoadBitmapResource(0xef);
        pManager[0x21] = 0xf0;
    }

    ((LevelVariantRenderManagerView *)pManager)
        ->LoadVariantRenderResource(pManager[0x21]);
    {
        void *pPaletteObject;

        pPaletteObject = LoadPalResource(0xed);
        if (*(int *)((char *)pPrimaryContext + 4) == 1) {
            ((GamePrimaryContext *)pPrimaryContext)
                ->SampleRootHelperGeometryAndDispatchRenderGroups(-1);
        }
        ((GamePrimaryContext *)pPrimaryContext)
            ->ApplyWindowOwnerPaletteResource(0xed);
        ReleaseTypedResourceObjectReference(pPaletteObject);
    }

    *(unsigned short *)((char *)pManager + 0x8a) =
        (short)((*(short *)((char *)pPrimaryContext + 10) - *(short *)(unsigned long)(pManager[0x25] + 0x4a)) / 2);
    *(unsigned short *)((char *)pManager + 0x88) =
        (short)((*(short *)((char *)pPrimaryContext + 8) - *(short *)(unsigned long)(pManager[0x25] + 0x48)) / 2);

    for (i = 0; i < 5; ++i) {
        paPoint = (short *)(unsigned long)pManager[0x1e];
        paPoint[i * 2] = (short)puPointTable[i];
        paPoint[i * 2 + 1] = (short)(puPointTable[i] >> 16);
    }

    *(unsigned short *)((char *)pManager + 0x90) = 0;
    pManager[0x23] = 1;
    nFrameCount =
        ((LevelVariantRenderManagerView *)pManager)
            ->GetVariantRenderFrameCount(pManager[0x21]);

    pSelector = (int *)AllocateVSMemBlock(0x1c);
    if (pSelector == 0) {
        pManager[0x37] = 0;
    } else {
        *(void **)pSelector = g_LEVEL_TimedVariantFrameIndexSelectorCtorVtable;
        pSelector[1] = nFrameCount;
        *(void **)pSelector = g_LEVEL_TimedVariantFrameIndexSelectorVtable;
        pSelector[6] = 1;
        *(void **)pSelector = g_LEVEL_TimedVariantFrameIndexSelectorCallbackVtable;
        pManager[0x37] = (int)(unsigned long)pSelector;
    }

    *(int *)(unsigned long)(pManager[0x37] + 0x14) = -1;
    SetTimedVariantFrameDuration(
        (void *)(unsigned long)pManager[0x37], 0,
        nFrameCount * 0x42);
    return pObject;
}

// FUNCTION: LEMBALL 0x0044ad60
void DestroyLevelScreenStatusIndicatorManager(void *pObject) {
    int *pManager;
    int i;

    pManager = (int *)pObject;
    *(void **)pManager = g_LEVEL_StatusIndicatorManagerVtable;
    *(void **)(pManager + 0x1c) = g_LEVEL_StatusIndicatorManagerSlot70Vtable;
    *(void **)(pManager + 0x1d) = g_LEVEL_StatusIndicatorManagerSlot74Vtable;
    FreeVSMemBlock((void *)(unsigned long)pManager[0x1e]);
    FreeVSMemBlock((void *)(unsigned long)pManager[0x37]);
    ReleaseVariantRenderResource(pManager, pManager[0x21]);
    if (pManager[0x25] != 0) {
        ReleaseTypedResourceObjectReference((void *)(unsigned long)pManager[0x25]);
    }
    ReleaseTypedResourceObjectReference((void *)(unsigned long)pManager[0x26]);
    if (*(int *)(pManager[0x1f] + 4) == 1) {
        ((GamePrimaryContext *)(unsigned long)pManager[0x1f])
            ->SampleRootHelperGeometryAndDispatchRenderGroups(-1);
    }

    for (i = 0; i < 1; ++i) {
        RestorePackagedSpriteRenderEntryVtableE((char *)pManager + 0xe0 - i * 0x10);
    }
    *(void **)(pManager + 0x48) = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    for (i = 0; i < 1; ++i) {
        RestoreStatusIndicatorPointSinkEntryVtable((char *)pManager + 0x120 - (i + 1) * 0x10);
        RestoreStatusIndicatorPointSinkEntryVtable((char *)pManager + 0x110 - i * 0x10);
        RestoreStatusIndicatorPointSinkEntryVtable((char *)pManager + 0x100 - i * 0x10);
    }
    for (i = 0; i < 1; ++i) {
        RestorePackagedSpriteRenderEntryVtableA((char *)pManager + 0xf0 - i * 0x10);
    }
    for (i = 0; i < 1; ++i) {
        RestoreLevelSelectionPointSinkFinalizeVtable((char *)pManager + 0xc0 - i * 0x1c);
    }
    for (i = 0; i < 1; ++i) {
        RestoreFramedScreenRenderChildEntryVtable((char *)pManager + 0x9c - i * 0x24);
    }
    DestroyVariantRenderManager(pObject);
}

// FUNCTION: LEMBALL 0x0044ae80
void LEMBALL_FASTCALL ReleaseStatusIndicatorPrimaryBitmap(void *pObject) {
    int *pManager;
    void *pPrimaryContext;

    pManager = (int *)pObject;
    pPrimaryContext = (void *)(unsigned long)pManager[0x1f];
    if (*(int *)((char *)pPrimaryContext + 4) == 1) {
        ((LevelOneArgVirtualProc)(*(void ***)pPrimaryContext)[28])(
            pPrimaryContext, 0, 0);
    }
    ReleaseTypedResourceObjectReference(
        (void *)(unsigned long)pManager[0x25]);
    pManager[0x25] = 0;
}

// FUNCTION: LEMBALL 0x0044b340
static void LEMBALL_FASTCALL SetStatusIndicatorLoadProgress0044B340(
    void *pObject, int, int nProgress)
{
    noop_callback_00456600();
    *(short *)((char *)pObject + 0x20) = (short)nProgress;
    PresentPrimaryContextScreen(*(void **)((char *)pObject + 0x0c));
}

// FUNCTION: LEMBALL 0x0044b630
void LEMBALL_FASTCALL RestoreLevelSelectionPointSinkFinalizeVtableBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

void *LEMBALL_FASTCALL InitializeCompositePointRectSinkEntries(
    void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(unsigned short *)(pEntry + 1) = 0;
    *(void **)pEntry = g_LEVEL_CompositePointRectSinkEntryInitVtable;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    *(void **)pEntry = g_LEVEL_FramedScreenRenderChildEntryVtable;
    *(unsigned short *)((char *)pEntry + 0xe) = 0;
    *(unsigned short *)(pEntry + 3) = 0;
    *(unsigned short *)((char *)pEntry + 0x22) = 0;
    *(unsigned short *)(pEntry + 8) = 0;

    pEntry += 9;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(unsigned short *)(pEntry + 1) = 0;
    *(void **)pEntry = g_LEVEL_CompositePointRectSinkEntryInitVtable;
    *(unsigned short *)((char *)pEntry + 10) = 0;
    *(unsigned short *)(pEntry + 2) = 0;
    *(void **)pEntry = g_LEVEL_FramedScreenRenderChildEntryVtable;
    *(unsigned short *)((char *)pEntry + 0xe) = 0;
    *(unsigned short *)(pEntry + 3) = 0;
    *(unsigned short *)((char *)pEntry + 0x22) = 0;
    *(unsigned short *)(pEntry + 8) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x004511a0
void LEMBALL_FASTCALL RestoreCompositePointRectSinkFinalizeVtablesBody(void *pObject) {
    *(void **)((char *)pObject + 0x24) =
        g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x004671b0
void LEMBALL_FASTCALL SetTimedVariantFrameDuration(
    void *pObject, int, int nFrameDuration)

{
    int *pSelector;

    pSelector = (int *)pObject;
    pSelector[4] = nFrameDuration;
    if (pSelector[5] == -1) {
        pSelector[3] = (int)timeGetTime();
    } else {
        pSelector[3] = pSelector[5];
    }
}

// FUNCTION: LEMBALL 0x00467260
void *LevelVariantRenderManagerView::Construct(
    int nOwner, int cResourceIds, int cPackagedRectEntries,
    int cActionButtonEntries, int nUnknown58, int fDoubleBuffered)

{
    int *pManager;
    int i;
    int cEntries;
    int *pEntry;

    pManager = (int *)this;
    *(void **)pManager = g_LEVEL_VariantRenderManagerVtable;
    pManager[0x10] = 0;
    pManager[0x17] = 0;
    pManager[7] = nOwner;
    pManager[0xb] = cPackagedRectEntries;
    pManager[0xc] = cResourceIds;
    pManager[0x15] = 0;
    pManager[0x19] = 0;
    pManager[8] = 0;
    pManager[0xe] = fDoubleBuffered;
    pManager[0xd] = 0;
    pManager[0x1b] = -1;
    pManager[9] = (int)(unsigned long)AllocateVSMemBlock((unsigned int)cPackagedRectEntries * 4U);
    pManager[10] = (int)(unsigned long)AllocateVSMemBlock((unsigned int)cResourceIds * 2U);

    for (i = 0; i < pManager[0xb]; ++i) {
        *(int *)(unsigned long)(pManager[9] + i * 4) = 0;
    }

    for (i = 0; i < pManager[0xc]; ++i) {
        *(short *)(unsigned long)(pManager[10] + i * 2) = (short)pManager[0xb];
    }

    if (pManager[0xe] == 0) {
        pManager[0x16] = nUnknown58;
        pManager[0xf] = cActionButtonEntries;
    } else {
        pManager[0x14] = 0;
        pManager[0x16] = nUnknown58 * 2;
        pManager[0x13] = 0;
        pManager[0x12] = 0;
        pManager[0xf] = cActionButtonEntries * 2;
    }

    cEntries = pManager[0x16];
    if (cEntries != 0) {
        pEntry = (int *)AllocateVSMemBlock((unsigned int)(cEntries * 0x18 + 4));
        if (pEntry == 0) {
            pManager[0x19] = 0;
        } else {
            *pEntry = cEntries;
            ++pEntry;
            while (--cEntries >= 0) {
                new (pEntry) LevelPackagedRectQueueEntry;
                pEntry += 6;
            }
            pManager[0x19] = (int)(unsigned long)pEntry - 0x18 * pManager[0x16];
        }
    }

    cEntries = pManager[0xf];
    if (cEntries != 0) {
        pEntry = (int *)AllocateVSMemBlock((unsigned int)(cEntries * 0x20 + 4));
        if (pEntry == 0) {
            pManager[0x15] = 0;
        } else {
            *pEntry = cEntries;
            ++pEntry;
            while (--cEntries >= 0) {
                InitializeBaseModeActionButtonPointSink(pEntry);
                pEntry += 8;
            }
            pManager[0x15] = (int)(unsigned long)pEntry - 0x20 * pManager[0xf];
        }
    }

    pManager[4] = 0;
    pManager[1] = 0;
    ((LevelVariantRenderManagerView *)pManager)
        ->ResetOrFlipVariantRenderBuffers();
    return this;
}

// FUNCTION: LEMBALL 0x004673d0
void DestroyVariantRenderManager(void *pObject) {
    int *pManager;
    int iLoaded;
    int iSlot;
    int *pnLoadedResource;

    pManager = (int *)pObject;
    *(void **)pManager = g_LEVEL_VariantRenderManagerVtable;
    iSlot = 0;
    if (pManager[0xd] != 0) {
        iLoaded = 0;
        while (iLoaded < pManager[0xd]) {
            pnLoadedResource = (int *)(unsigned long)(pManager[9] + iSlot * 4);
            while (*pnLoadedResource == 0) {
                ++pnLoadedResource;
                ++iSlot;
            }
            ReleaseTypedResourceObjectReference(*(void **)(unsigned long)(pManager[9] + iSlot * 4));
            ++iLoaded;
            ++iSlot;
        }
    }

    if (pManager[9] != 0) {
        FreeVSMemBlock((void *)(unsigned long)pManager[9]);
        pManager[9] = 0;
    }
    if (pManager[10] != 0) {
        FreeVSMemBlock((void *)(unsigned long)pManager[10]);
        pManager[10] = 0;
    }
    if (pManager[0x19] != 0) {
        void *pArray = (void *)(unsigned long)pManager[0x19];
        ((LevelDeleteArrayVirtualProc)(*(void ***)pArray)[0])(
            pArray, 0, 3);
        pManager[0x19] = 0;
    }
    if (pManager[0x15] != 0) {
        void *pArray = (void *)(unsigned long)pManager[0x15];
        ((LevelDeleteArrayVirtualProc)(*(void ***)pArray)[0])(
            pArray, 0, 3);
        pManager[0x15] = 0;
    }
    if (pManager[4] != 0 && pManager[1] != 0) {
        void *pArray = (void *)(unsigned long)pManager[1];
        ((LevelDeleteArrayVirtualProc)(*(void ***)pArray)[0])(
            pArray, 0, 3);
        pManager[1] = 0;
    }
}

// FUNCTION: LEMBALL 0x00467490
void LevelVariantRenderManagerView::LoadVariantRenderResource(int nResourceId) {
    int *pManager;
    short *pnResourceIndex;
    int iSlot;
    int *pnLoadedResource;

    pManager = (int *)this;
    iSlot = (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2);
    if (pManager[0xb] == iSlot) {
        pnLoadedResource = (int *)(unsigned long)pManager[9];
        iSlot = 0;
        while (*pnLoadedResource != 0) {
            ++pnLoadedResource;
            ++iSlot;
        }
    }

    *(void **)(unsigned long)(pManager[9] + iSlot * 4) = LoadZrleOnlyListResource(nResourceId);
    if (*(int *)(unsigned long)(pManager[9] + iSlot * 4) == 0) {
        *(void **)(unsigned long)(pManager[9] + iSlot * 4) = LoadZrleResource(nResourceId);
    }

    pnResourceIndex = (short *)(unsigned long)(pManager[10] + nResourceId * 2);
    if ((int)*pnResourceIndex == pManager[0xb]) {
        *pnResourceIndex = (short)iSlot;
        ++pManager[0xd];
    }
}

// FUNCTION: LEMBALL 0x00467500
void LevelVariantRenderManagerView::ReleaseVariantRenderResource(int nResourceId) {
    void *pResourceObject;
    int iResourceSlot;

    iResourceSlot = m_pResourceIds28[nResourceId];
    if (iResourceSlot == m_nFreeResourceId2C) {
        return;
    }
    pResourceObject = m_ppResourceObjects24[iResourceSlot];
    if (pResourceObject != 0) {
        ReleaseTypedResourceObjectReference(pResourceObject);
    }
    m_ppResourceObjects24[iResourceSlot] = 0;
    m_pResourceIds28[nResourceId] = (short)m_nFreeResourceId2C;
    --m_cLoadedResources34;
}

// FUNCTION: LEMBALL 0x00467540
int LevelVariantRenderManagerView::GetVariantRenderFrameCount(int nResourceId) {
    int *pResourceObject;

    pResourceObject =
        (int *)m_ppResourceObjects24[m_pResourceIds28[nResourceId]];
    if (pResourceObject[0x10] == 0x5a524c45) {
        return 1;
    }
    return pResourceObject[0x1b];
}

// FUNCTION: LEMBALL 0x00467730
void LevelVariantRenderManagerView::EmitVariantRenderResourceEntry(short *pRect, const short *pPoint, int nResourceId, int nFlags, void *pFrameSelector, int nValue) {
    typedef int (LEMBALL_FASTCALL *FrameProc)(void *, int);
    typedef void (LEMBALL_FASTCALL *QueueProc)(void *, int, void *);
    int *pManager;
    int *pResource;
    int *pEntry;
    int *pFrame;
    int iEntry;
    int iFrame;

    pManager = (int *)this;
    if (pManager[0x0e] != 0 && pManager[7] != pManager[8] &&
        pManager[8] != 0) {
        int nOwner = pManager[7];
        pManager[7] = pManager[8];
        ResetOrFlipVariantRenderBuffers();
        pManager[7] = nOwner;
    }
    pManager[8] = pManager[7];
    pResource = *(int **)(unsigned long)(pManager[9] +
        *(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4);

    if (pResource[0x10] == 0x5a524c45) {
        if (pManager[0x0e] == 0) {
            iEntry = pManager[0x17]++;
            pEntry = (int *)(unsigned long)(pManager[0x19] + iEntry * 0x18);
        } else {
            if (pManager[0x16] == pManager[0x13]) {
                ResetOrFlipVariantRenderBuffers();
            }
            iEntry = pManager[0x13];
            pManager[0x13] += 2;
            pEntry = (int *)(unsigned long)(pManager[0x19] +
                (pManager[0x14] + iEntry) * 0x18);
        }
        pEntry[5] = *(unsigned short *)((char *)pManager + 0x68);
        *(short *)(pEntry + 1) = pPoint[0];
        *(short *)((char *)pEntry + 6) = pPoint[1];
        pEntry[2] = (int)(unsigned long)pResource;
        pEntry[3] = nFlags;
        pEntry[4] = nValue;
        if (pResource[4] == 0) {
            ((void (LEMBALL_FASTCALL *)(void *, int))
                (*(void ***)pResource)[7])(pResource, 0);
        } else {
            pResource[9] = 0;
        }
        ++pResource[2];
        AppendPointerQueueEntry((void *)(unsigned long)pManager[7], pEntry);
        pFrame = pResource;
    } else {
        iFrame = 0;
        if (pFrameSelector != 0) {
            iFrame = ((FrameProc)(*(void ***)pFrameSelector)[0])(
                pFrameSelector, 0);
            ((int *)pFrameSelector)[2] = iFrame;
        }
        pFrame = (int *)(unsigned long)(pResource[0x1e] + iFrame * 0x54);
        if (pManager[0x0e] == 0) {
            iEntry = pManager[0x10]++;
            pEntry = (int *)(unsigned long)(pManager[0x15] + iEntry * 0x20);
        } else {
            if (pManager[0x0f] == pManager[0x12]) {
                ResetOrFlipVariantRenderBuffers();
            }
            iEntry = pManager[0x12];
            pManager[0x12] += 2;
            pEntry = (int *)(unsigned long)(pManager[0x15] +
                (pManager[0x14] + iEntry) * 0x20);
        }
        pEntry[5] = *(unsigned short *)((char *)pManager + 0x68);
        *(short *)(pEntry + 1) = pPoint[0];
        *(short *)((char *)pEntry + 6) = pPoint[1];
        pEntry[7] = (int)(unsigned long)pResource;
        pEntry[6] = iFrame;
        pEntry[3] = nFlags;
        pEntry[4] = nValue;
        if (pResource[4] == 0) {
            ((void (LEMBALL_FASTCALL *)(void *, int))
                (*(void ***)pResource)[7])(pResource, 0);
        } else {
            pResource[9] = 0;
        }
        ++pResource[2];
        AppendPointerQueueEntry((void *)(unsigned long)pManager[7], pEntry);
    }
    pRect[0] = *(short *)((char *)pFrame + 0x4c);
    pRect[1] = *(short *)((char *)pFrame + 0x4e);
    pRect[2] = *(short *)((char *)pFrame + 0x48);
    pRect[3] = *(short *)((char *)pFrame + 0x4a);
}

// FUNCTION: LEMBALL 0x004678c0
void LevelVariantRenderManagerView::ResetOrFlipVariantRenderBuffers(void) {
    struct VariantRenderBufferCounts {
        int m_cEntries;
        int m_nToken;
    };
    VariantRenderBufferCounts *pActiveCounts;
    VariantRenderBufferCounts *pAvailableCounts;
    int nTarget;
    unsigned char *pbToken;
    VsGdiCompactGeometryTokenDispatch *pCompactTarget;

    *(int *)((char *)this + 0x0c) = 0;
    if (*(int *)((char *)this + 0x38) == 0) {
        pAvailableCounts =
            (VariantRenderBufferCounts *)((char *)this + 0x5c);
        pActiveCounts =
            (VariantRenderBufferCounts *)((char *)this + 0x40);
        pAvailableCounts->m_cEntries = 0;
        *pActiveCounts = *pAvailableCounts;
        nTarget = *(int *)(*(int *)((char *)this + 0x1c) + 0x0c);
        pCompactTarget = (VsGdiCompactGeometryTokenDispatch *)(
            *(int *)(*(int *)(unsigned long)(nTarget + 0x40) + 4) +
            0x40 + nTarget);
        pbToken = pCompactTarget->GetToken();
        *(unsigned char *)((char *)this + 0x44) = *pbToken;
        *(unsigned char *)((char *)this + 0x60) = *pbToken;
        return;
    }

    DispatchAndClearPointerQueue(*(void **)((char *)this + 0x1c));
    *(int *)((char *)*(void **)((char *)this + 0x1c) + 4) = 0;
    *(int *)((char *)this + 0x4c) = 0;
    *(int *)((char *)this + 0x48) = 0;
    *(unsigned int *)((char *)this + 0x50) ^= 1;
}

// FUNCTION: LEMBALL 0x00467bb0
static void *DestroyStatusIndicatorPointSinkArray00467BB0(void *pObject, unsigned int fDelete) {
    int cEntries;
    char *pEntry;

    if ((fDelete & 2) == 0) {
        *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pObject);
        }
        return pObject;
    }

    cEntries = *((int *)pObject - 1);
    pEntry = (char *)pObject + cEntries * 0x10;
    while (cEntries-- > 0) {
        pEntry -= 0x10;
        RestoreStatusIndicatorPointSinkEntryVtable(pEntry);
    }
    FreeVSMemBlock((char *)pObject - 4);
    return pObject;
}

// FUNCTION: LEMBALL 0x004698f0
void LEMBALL_FASTCALL RestoreQueuedRenderPointSinkVtable(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00469930
static void *DestroyPackagedSpriteRenderEntryArray00469930(void *pObject, unsigned int fDelete) {
    int cEntries;
    char *pEntry;

    if ((fDelete & 2) == 0) {
        *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pObject);
        }
        return pObject;
    }

    cEntries = *((int *)pObject - 1);
    pEntry = (char *)pObject + cEntries * 0x10;
    while (cEntries-- > 0) {
        pEntry -= 0x10;
        RestorePackagedSpriteRenderEntryVtableA(pEntry);
    }
    FreeVSMemBlock((char *)pObject - 4);
    return pObject;
}

// FUNCTION: LEMBALL 0x0046b310
void SetStatusIndicatorZrleResource(void *pObject, int nResourceId) {
    int *pClient;
    int nOffset;

    pClient = (int *)pObject;
    if (nResourceId != pClient[0xa]) {
        if (pClient[0xa] != 0) {
            ReleaseTypedResourceObjectReference((void *)(unsigned long)pClient[8]);
        }
        pClient[0xa] = nResourceId;
        if (nResourceId != 0) {
            pClient[8] = (int)(unsigned long)LoadZrleResource(nResourceId);
            nOffset = 0;
            do {
                nOffset += 0x18;
                *(int *)(pClient[7] - 0x10 + nOffset) = pClient[8];
            } while (nOffset < 0x18);
            return;
        }
        pClient[8] = 0;
    }
}

// FUNCTION: LEMBALL 0x0046b370
void LEMBALL_FASTCALL SetStatusIndicatorActive(
    void *pObject, int, int fActive)

{
    int *pClient;
    DWORD dwNow;

    pClient = (int *)pObject;
    if (pClient[6] != fActive) {
        pClient[6] = fActive;
        if (fActive == 0) {
            ((void (LEMBALL_FASTCALL *)(void *))
                 (*(void ***)pClient)[5])(pClient);
            return;
        }
        pClient[0x17] = 0;
        pClient[0x18] = 0;
        pClient[0x19] = 0;
        pClient[0x1a] = 0;
        dwNow = timeGetTime();
        pClient[0x1b] = (int)dwNow;
        pClient[0x1c] = (int)dwNow;
    }
}

// FUNCTION: LEMBALL 0x0046b3b0
void SetStatusIndicatorListResourceEntry(void *pObject, int nResourceId, int nEntryIndex) {
    int *pClient;
    int nOffset;

    pClient = (int *)pObject;
    if (nResourceId != pClient[0xa]) {
        if (pClient[0xa] != 0) {
            ReleaseTypedResourceObjectReference((void *)(unsigned long)pClient[8]);
        }
        pClient[0xa] = nResourceId;
        pClient[9] = -1;
        if (nResourceId == 0) {
            pClient[8] = 0;
        } else {
            pClient[8] = (int)(unsigned long)LoadZrleOnlyListResource(nResourceId);
        }
    }

    if (pClient[9] != nEntryIndex && pClient[8] != 0) {
        pClient[9] = nEntryIndex;
        nOffset = 0;
        do {
            nOffset += 0x18;
            *(int *)(pClient[7] - 0x10 + nOffset) = *(int *)(pClient[8] + 0x78) + pClient[9] * 0x54;
        } while (nOffset < 0x18);
    }
}

// FUNCTION: LEMBALL 0x0046b810
void GameCursorRenderClient::TickCursorRenderClientMotion(void) {
    VsMathPoint2D PreviousPosition;
    DWORD dwNow;
    int nElapsed;
    int nMaximumVelocity;
    short nMaximumX;
    short nMaximumY;

    if (m_fMotionPending38 == 0 &&
        m_fIdleRefreshPending3C == 0) {
        ShowNativeCursor();
    }

    if (m_fPrimaryVisible2C == 0 ||
        ((((GameSharedGeometryHelperView *)g_pSharedGeometryHelper)
              ->m_dwFlags14 & 1) == 0)) {
        if (m_fSecondaryVisible30 == 0 ||
            ((((GameSharedGeometryHelperView *)g_pSharedGeometryHelper)
                  ->m_dwFlags14 & 6) == 0)) {
            return;
        }
    }
    if (m_fActive18 == 0) {
        return;
    }

    dwNow = timeGetTime();
    if (m_Acceleration64.m_nX != 0) {
        nElapsed = (int)(dwNow - m_dwLastAccelerationXTick6C);
        m_dwLastAccelerationXTick6C = dwNow;
        m_Velocity5C.m_nX +=
            (m_Acceleration64.m_nX * nElapsed) / 20;
        nMaximumVelocity = m_nMaximumVelocity50;
        if (nMaximumVelocity < m_Velocity5C.m_nX) {
            m_Velocity5C.m_nX = nMaximumVelocity;
        }
        if (m_Velocity5C.m_nX < -nMaximumVelocity) {
            m_Velocity5C.m_nX = -nMaximumVelocity;
        }
    }
    if (m_Acceleration64.m_nY != 0) {
        nElapsed = (int)(dwNow - m_dwLastAccelerationYTick70);
        m_dwLastAccelerationYTick70 = dwNow;
        m_Velocity5C.m_nY +=
            (m_Acceleration64.m_nY * nElapsed) / 20;
        nMaximumVelocity = m_nMaximumVelocity50;
        if (nMaximumVelocity < m_Velocity5C.m_nY) {
            m_Velocity5C.m_nY = nMaximumVelocity;
        }
        if (m_Velocity5C.m_nY < -nMaximumVelocity) {
            m_Velocity5C.m_nY = -nMaximumVelocity;
        }
    }

    PreviousPosition.m_nX = m_Position10.m_nX;
    PreviousPosition.m_nY = m_Position10.m_nY;
    m_FixedPosition54.m_nX += m_Velocity5C.m_nX;
    m_FixedPosition54.m_nY += m_Velocity5C.m_nY;
    m_Position10.m_nX =
        (short)(m_FixedPosition54.m_nX >> 12);
    m_Position10.m_nY =
        (short)(m_FixedPosition54.m_nY >> 12);

    if (m_fSecondaryVisible30 != 0 &&
        m_Position10.Equals(&PreviousPosition) == 0) {
        GameCursorPositionEvent Event;

        Event.m_nType = 10;
        Event.m_dwTime = timeGetTime();
        Event.m_nPackedPosition =
            PackEventXYWords((unsigned short)m_Position10.m_nX,
                             m_Position10.m_nY);
        Event.m_nReserved0C = 0;
        Event.m_nReserved10 = 0;
        ((GameCursorEventQueueInterface *)g_pSharedRenderDispatchQueue)
            ->Append(&Event);
    }

    if ((int)m_BoundsSize74.m_nX *
            (int)m_BoundsSize74.m_nY != 0) {
        if (!(m_BoundsOrigin78.m_nX <= m_Position10.m_nX &&
              m_Position10.m_nX <
                  (short)(m_BoundsOrigin78.m_nX +
                          m_BoundsSize74.m_nX) &&
              m_BoundsOrigin78.m_nY <= m_Position10.m_nY &&
              m_Position10.m_nY <
                  (short)(m_BoundsOrigin78.m_nY +
                          m_BoundsSize74.m_nY))) {
            VsMathFixed12Vector2D FixedPosition;

            if (m_Position10.m_nX < m_BoundsOrigin78.m_nX) {
                m_Position10.m_nX = m_BoundsOrigin78.m_nX;
            }
            if (m_Position10.m_nY < m_BoundsOrigin78.m_nY) {
                m_Position10.m_nY = m_BoundsOrigin78.m_nY;
            }

            nMaximumX = (short)(m_BoundsOrigin78.m_nX +
                                m_BoundsSize74.m_nX - 1);
            nMaximumY = (short)(m_BoundsOrigin78.m_nY +
                                m_BoundsSize74.m_nY - 1);
            if (nMaximumX < m_Position10.m_nX) {
                m_Position10.m_nX = nMaximumX;
            }
            if (nMaximumY < m_Position10.m_nY) {
                m_Position10.m_nY = nMaximumY;
            }

            FixedPosition.InitializeFromPixelsThunk(
                m_Position10.m_nX, m_Position10.m_nY);
            m_FixedPosition54 = FixedPosition;
        }
    }
    m_fMotionPending38 = 0;
}

// FUNCTION: LEMBALL 0x0046ba50
int VsMathPoint2D::Equals(const VsMathPoint2D *pOther) {
    if (pOther->m_nX == m_nX && pOther->m_nY == m_nY) {
        return 1;
    }
    return 0;
}
