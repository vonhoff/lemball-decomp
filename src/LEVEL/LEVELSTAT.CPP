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

extern "C" DWORD WINAPI timeGetTime(void);
extern int WINAPI InitializeNonZrleVariantRenderEntry(int nValue);
extern void NoopVtableCallbackThunk(void);
extern void VariantResourceBundleNoopThunk(void);
void LEMBALL_FASTCALL SetTimedVariantFrameDuration(
    void *pObject, int nUnusedEdx, int nFrameDuration);
extern void LEMBALL_FASTCALL DispatchAndClearPointerQueue(void *pQueue);
extern void LEMBALL_FASTCALL PresentPrimaryContextScreen(void *pPrimaryContext);
extern void noop_callback_00456600(void);

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

void ResetBaseModeActionButtonPointSink(void *pObject);
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
static void DeleteRenderPointRectSinkThunk00401898(void *pObject, BYTE fDelete);
static void LEMBALL_FASTCALL QueueRenderPointRectSinkEntry004398A0(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL QueueRenderPointRectSinkThunk00402DD3(
    void *pObject, int nUnusedEdx, void *pQueue);
static void LEMBALL_FASTCALL DispatchRenderPointRectSinkEntry004398B0(
    void *pObject, int nUnusedEdx, void *pContext);
static void LEMBALL_FASTCALL DispatchRenderPointRectSinkThunk00401C71(
    void *pObject, int nUnusedEdx, void *pContext);
extern void *LEMBALL_FASTCALL DeleteGeometryOwnerRect(
    void *pOwner, int nUnused, int nDeleteFlag);
extern void LEMBALL_FASTCALL GeometryOwnerButtonTransitionNoop(
    void *pOwner, int nUnused, const VS_XYPair *pPoint, int nButton);
extern void LEMBALL_FASTCALL GeometryOwnerButtonInsideNoop(
    void *pOwner, int nUnused, const VS_XYPair *pPoint, int nButton);
extern void LEMBALL_FASTCALL ClearGeometryOwnerButtonState(
    void *pOwner, int nUnused, const VS_XYPair *pPoint, int nButton);
static void *LEMBALL_FASTCALL DeleteRenderPointRectContextThunk00401488(
    void *pObject, int nUnused, int fDelete);
static void LEMBALL_FASTCALL ResetRenderPointRectEntryThunk00401703(
    void *pObject, int nUnused, const VS_XYPair *pPoint, int nButton);
static void LEMBALL_FASTCALL ResetRenderPointRectEntryThunk0040355D(
    void *pObject, int nUnused, const VS_XYPair *pPoint, int nButton);
static void LEMBALL_FASTCALL ClearRenderPointRectEntryFieldsThunk00403724(
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
/* 00499620: the adjacent 00499628 packaged-entry table is now emitted by
 * LevelPackagedRectQueueEntry rather than represented as raw storage here. */
static void *g_LEVEL_VariantRenderManagerVtableStorage[2] = {
    (void *)VariantResourceBundleNoopThunk,
    0,
};
static void *g_LEVEL_VariantRenderManagerVtable =
    g_LEVEL_VariantRenderManagerVtableStorage;
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
static int g_LEVEL_StatusIndicatorModeResourceIds[4] = {0, 0xfd, 0xfc, 0};
// GLOBAL: LEMBALL 0x0049F9B0
static unsigned int g_LEVEL_CompactStatusIndicatorPointTable[5] = {
    0x004d0087, 0x0063008f, 0x005b0094, 0x0000000a, 0x00040004
};
// GLOBAL: LEMBALL 0x0049F9C8
static unsigned int g_LEVEL_StandardStatusIndicatorPointTable[5] = {
    0x009a010e, 0x00c6011a, 0x00b60128, 0x00000014, 0x00080008
};
int g_fLevelScreenStatusIndicatorConfigured = 0;

// FUNCTION: LEMBALL 0x00432A90
static void *DeleteStatusIndicatorPointSinkEntry00432A90(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00401834
static void *DeleteStatusIndicatorPointSinkThunk00401834(void *pObject, BYTE fDelete) {
    return DeleteStatusIndicatorPointSinkEntry00432A90(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00432B10
static void LEMBALL_FASTCALL QueueHelperLocalRectUpdateEntry00432B10(
    void *pObject, int, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00401E6F
static void LEMBALL_FASTCALL QueueHelperLocalRectUpdateThunk00401E6F(
    void *pObject, int, void *pQueue) {
    QueueHelperLocalRectUpdateEntry00432B10(pObject, 0, pQueue);
}

// FUNCTION: LEMBALL 0x00432B20
static void LEMBALL_FASTCALL DispatchHelperLocalRectUpdateEntry00432B20(
    void *pObject, int, void *pContext) {
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

// FUNCTION: LEMBALL 0x004037A1
static void LEMBALL_FASTCALL DispatchHelperLocalRectUpdateThunk004037A1(
    void *pObject, int, void *pContext) {
    DispatchHelperLocalRectUpdateEntry00432B20(pObject, 0, pContext);
}

// FUNCTION: LEMBALL 0x004471A0
static void *DeleteFramedScreenRenderChildEntryB(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00403166
static void *DeleteFramedScreenRenderChildEntryThunk(void *pObject, BYTE fDelete) {
    return DeleteFramedScreenRenderChildEntryB(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00447300
static void LEMBALL_FASTCALL QueueFramedScreenRenderEntry(
    void *pObject, int, void *pQueue) {
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

// FUNCTION: LEMBALL 0x004029CD
static void LEMBALL_FASTCALL QueueFramedScreenRenderEntryThunk(
    void *pObject, int, void *pQueue) {
    QueueFramedScreenRenderEntry(pObject, 0, pQueue);
}

// FUNCTION: LEMBALL 0x00447340
static void LEMBALL_FASTCALL FinalizeFramedScreenRenderEntry(
    void *pObject, int, void *pManager) {
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

// FUNCTION: LEMBALL 0x0040213A
static void LEMBALL_FASTCALL FinalizeFramedScreenRenderEntryThunk(
    void *pObject, int, void *pManager) {
    FinalizeFramedScreenRenderEntry(pObject, 0, pManager);
}

// FUNCTION: LEMBALL 0x004472E0
static void *DeleteBaseModeObjectAdjusted(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x0c);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00402C20
void *DeleteBaseModeObjectAdjustedThunk(void *pObject, BYTE fDelete) {
    return DeleteBaseModeObjectAdjusted(pObject, fDelete);
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

// FUNCTION: LEMBALL 0x00402D1A
void GameBaseModeObject::QueueBaseModeUserActionThunk(int nAction, int nPhase) {
    QueueBaseModeUserAction(nAction, nPhase);
}

// FUNCTION: LEMBALL 0x004465E0
void GameBaseModeScreen::QueueActiveBaseModeUserAction(int nAction, int nPhase) {
    m_fActionBusy90 = 1;
    ((GameBaseModeObject *)g_pActiveNetworkLobbyTransportController)
        ->QueueBaseModeUserActionThunk(nAction, nPhase);
}

// FUNCTION: LEMBALL 0x00401FAA
void GameBaseModeScreen::QueueActiveBaseModeUserActionThunk(int nAction, int nPhase) {
    QueueActiveBaseModeUserAction(nAction, nPhase);
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

// FUNCTION: LEMBALL 0x004029BE
void GameBaseModeScreen::DispatchBaseModeUserActionThunk(int nAction, int nPhase) {
    DispatchBaseModeUserAction(nAction, nPhase);
}

// FUNCTION: LEMBALL 0x004468D0
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

// FUNCTION: LEMBALL 0x0040134D
int LEMBALL_FASTCALL HandleBaseModeActionButtonEventThunk(void *pClient, int, void *pEvent) {
    return ((GameBaseModeEventClient *)pClient)
        ->GameBaseModeEventClient::HandleBaseModeActionButtonEvent(
            (VsNetEffDispatchEvent *)pEvent);
}

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

// FUNCTION: LEMBALL 0x0046BA50
int VsMathPoint2D::Equals(const VsMathPoint2D *pOther) {
    if (pOther->m_nX == m_nX && pOther->m_nY == m_nY) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0046B810
void GameCursorRenderClient::TickCursorRenderClientMotion(void) {
    VsMathPoint2D PreviousPosition;
    DWORD dwNow;
    int nElapsed;
    int nMaximumVelocity;
    short nMaximumX;
    short nMaximumY;

    if (m_fMotionPending38 == 0 &&
        m_fIdleRefreshPending3C == 0) {
        RefreshIdleState();
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
    if (m_pRenderResource18 == 0) {
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

// FUNCTION: LEMBALL 0x00447170
static void *DeleteFramedScreenRenderChildEntryA(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004033FA
static void *DeleteFramedScreenRenderChildEntryAThunk(void *pObject, BYTE fDelete) {
    return DeleteFramedScreenRenderChildEntryA(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00447380
static void LEMBALL_FASTCALL QueueBitmapRenderEntry(
    void *pObject, int, void *pQueue) {
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

// FUNCTION: LEMBALL 0x0040362F
static void LEMBALL_FASTCALL QueueBitmapRenderEntryThunk(
    void *pObject, int, void *pQueue) {
    QueueBitmapRenderEntry(pObject, 0, pQueue);
}

// FUNCTION: LEMBALL 0x004473D0
static void LEMBALL_FASTCALL RenderBitmapEntryViaContext(
    void *pObject, int, void *pManager) {
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

// FUNCTION: LEMBALL 0x004031AC
static void LEMBALL_FASTCALL RenderBitmapEntryViaContextThunk(
    void *pObject, int, void *pManager) {
    RenderBitmapEntryViaContext(pObject, 0, pManager);
}

// FUNCTION: LEMBALL 0x00432350
static void *DeleteQueuedRenderPointSinkWrapper00432350(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00403580
static void *DeleteQueuedRenderPointSinkThunk00403580(void *pObject, BYTE fDelete) {
    return DeleteQueuedRenderPointSinkWrapper00432350(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00467BB0
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

// FUNCTION: LEMBALL 0x00439800
static void LEMBALL_FASTCALL QueuePackagedSpriteRenderEntry00439800(
    void *pObject, int, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00403846
static void LEMBALL_FASTCALL QueuePackagedSpriteRenderEntryThunk00403846(
    void *pObject, int, void *pQueue) {
    QueuePackagedSpriteRenderEntry00439800(pObject, 0, pQueue);
}

// FUNCTION: LEMBALL 0x00439810
static void LEMBALL_FASTCALL DispatchPackagedSpriteRenderEntry00439810(
    void *pObject, int, void *pContext) {
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

// FUNCTION: LEMBALL 0x00402A8B
static void LEMBALL_FASTCALL DispatchPackagedSpriteRenderEntryThunk00402A8B(
    void *pObject, int, void *pContext) {
    DispatchPackagedSpriteRenderEntry00439810(pObject, 0, pContext);
}

// FUNCTION: LEMBALL 0x00439400
static void *DeletePackagedSpriteRenderEntry00439400(void *pObject, BYTE fDelete) {
    *(void **)pObject = &g_LEVEL_PackagedSpriteRenderEntryVtableStorage[4];
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x004016F9
static void *DeletePackagedSpriteRenderEntryThunk004016F9(void *pObject, BYTE fDelete) {
    return DeletePackagedSpriteRenderEntry00439400(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x00439370
void NoopPackagedSpriteRenderEntry00439370(void) {
}

// FUNCTION: LEMBALL 0x00402A22
void LEMBALL_FASTCALL NoopPackagedSpriteRenderEntryThunk00402A22(
    void *pUnused)
{
    (void)pUnused;
    NoopPackagedSpriteRenderEntry00439370();
}

// FUNCTION: LEMBALL 0x00439380
void PASCAL NoopPackagedSpriteRenderEntry00439380(void *pUnused) {
    (void)pUnused;
}

// FUNCTION: LEMBALL 0x00401B77
void LEMBALL_FASTCALL NoopPackagedSpriteRenderEntryThunk00401B77(
    void *pUnusedThis, int nUnusedEdx, void *pArgument)
{
    (void)pUnusedThis;
    (void)nUnusedEdx;
    NoopPackagedSpriteRenderEntry00439380(pArgument);
}

// FUNCTION: LEMBALL 0x004396E0
static void *DeleteRenderPointRectSinkEntry004396E0(void *pObject, BYTE fDelete) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x00401898
static void DeleteRenderPointRectSinkThunk00401898(void *pObject, BYTE fDelete) {
    DeleteRenderPointRectSinkEntry004396E0(pObject, fDelete);
}

// FUNCTION: LEMBALL 0x004398A0
static void LEMBALL_FASTCALL QueueRenderPointRectSinkEntry004398A0(
    void *pObject, int, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x00402DD3
static void LEMBALL_FASTCALL QueueRenderPointRectSinkThunk00402DD3(
    void *pObject, int, void *pQueue) {
    QueueRenderPointRectSinkEntry004398A0(pObject, 0, pQueue);
}

// FUNCTION: LEMBALL 0x004398B0
static void LEMBALL_FASTCALL DispatchRenderPointRectSinkEntry004398B0(
    void *pObject, int, void *pContext) {
    int pOwner;
    void *pDispatchObject;

    pOwner = *(int *)((char *)pContext + 0x0c);
    pDispatchObject = (void *)(unsigned long)(pOwner + 0x98);
    ((LevelOneArgVirtualProc)(*(void ***)pDispatchObject)[1])(
        pDispatchObject, 0, pObject);
}

// FUNCTION: LEMBALL 0x00401C71
static void LEMBALL_FASTCALL DispatchRenderPointRectSinkThunk00401C71(
    void *pObject, int, void *pContext) {
    DispatchRenderPointRectSinkEntry004398B0(pObject, 0, pContext);
}

// FUNCTION: LEMBALL 0x00401488
static void *LEMBALL_FASTCALL DeleteRenderPointRectContextThunk00401488(
    void *pObject, int, int fDelete) {
    return DeleteGeometryOwnerRect(pObject, 0, fDelete);
}

// FUNCTION: LEMBALL 0x00401703
static void LEMBALL_FASTCALL ResetRenderPointRectEntryThunk00401703(
    void *pObject, int, const VS_XYPair *pPoint, int nButton) {
    GeometryOwnerButtonTransitionNoop(pObject, 0, pPoint, nButton);
}

// FUNCTION: LEMBALL 0x0040355D
static void LEMBALL_FASTCALL ResetRenderPointRectEntryThunk0040355D(
    void *pObject, int, const VS_XYPair *pPoint, int nButton) {
    GeometryOwnerButtonInsideNoop(pObject, 0, pPoint, nButton);
}

// FUNCTION: LEMBALL 0x00403724
static void LEMBALL_FASTCALL ClearRenderPointRectEntryFieldsThunk00403724(
    void *pObject, int, const VS_XYPair *pPoint, int nButton) {
    ClearGeometryOwnerButtonState(pObject, 0, pPoint, nButton);
}

// FUNCTION: LEMBALL 0x00432AD0
static void LEMBALL_FASTCALL QueueStatusIndicatorPointSink00432AD0(
    void *pObject, int, void *pQueue) {
    AppendPointerQueueEntry(pQueue, pObject);
}

// FUNCTION: LEMBALL 0x004018AC
static void LEMBALL_FASTCALL QueueStatusIndicatorPointSinkThunk004018AC(
    void *pObject, int, void *pQueue) {
    QueueStatusIndicatorPointSink00432AD0(pObject, 0, pQueue);
}

// FUNCTION: LEMBALL 0x00432AE0
static void LEMBALL_FASTCALL DispatchStatusIndicatorPointSink00432AE0(
    void *pObject, int, void *pContext) {
    int pOwner;
    int pOwnerOffsets;
    char *pDispatchObject;

    pOwner = *(int *)((char *)pContext + 0x0c);
    pOwnerOffsets = *(int *)(pOwner + 0x40);
    pDispatchObject = (char *)(*(int *)(pOwnerOffsets + 4) + pOwner + 0x40);
    ((LevelOneArgVirtualProc)(*(void ***)pDispatchObject)[4])(
        pDispatchObject, 0, pObject);
}

// FUNCTION: LEMBALL 0x004025AE
static void LEMBALL_FASTCALL DispatchStatusIndicatorPointSinkThunk004025AE(
    void *pObject, int, void *pContext) {
    DispatchStatusIndicatorPointSink00432AE0(pObject, 0, pContext);
}

// FUNCTION: LEMBALL 0x00434D10
static void LEMBALL_FASTCALL UpdateStatusIndicatorLoadProgress00434D10(
    void *pObject) {
    int nLoaded;
    void *pOwner;

    nLoaded = ++*(int *)((char *)pObject + 0x1c);
    pOwner = *(void **)((char *)pObject + 0x08);
    if (pOwner != 0) {
        void *pProgressClient = (char *)pOwner + 0x70;
        ((void (LEMBALL_FASTCALL *)(void *, int, int))
             (*(void ***)pProgressClient)[0])(
            pProgressClient, 0,
            (nLoaded * 100) / *(int *)((char *)pObject + 0x20));
    }
}

// FUNCTION: LEMBALL 0x00401DB1
static void LEMBALL_FASTCALL UpdateStatusIndicatorLoadProgressThunk00401DB1(
    void *pObject) {
    UpdateStatusIndicatorLoadProgress00434D10(pObject);
}

// FUNCTION: LEMBALL 0x0044B340
static void LEMBALL_FASTCALL SetStatusIndicatorLoadProgress0044B340(
    void *pObject, int, int nProgress) {
    noop_callback_00456600();
    *(short *)((char *)pObject + 0x20) = (short)nProgress;
    PresentPrimaryContextScreen(*(void **)((char *)pObject + 0x0c));
}

// FUNCTION: LEMBALL 0x00402BFD
static void LEMBALL_FASTCALL SetStatusIndicatorLoadProgressThunk00402BFD(
    void *pObject, int, int nProgress) {
    SetStatusIndicatorLoadProgress0044B340(pObject, 0, nProgress);
}

// FUNCTION: LEMBALL 0x00435870
int LEMBALL_FASTCALL GetTimedVariantFrameStartTime(void *pObject) {
    return *(int *)((char *)pObject + 0x0c);
}

// FUNCTION: LEMBALL 0x004011B8
int LEMBALL_FASTCALL GetTimedVariantFrameStartTimeThunk(void *pObject) {
    return GetTimedVariantFrameStartTime(pObject);
}

// FUNCTION: LEMBALL 0x004439E0
void ResetBaseModeActionButtonPointSink(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
}

// FUNCTION: LEMBALL 0x004452E0
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
extern void PruneUnreferencedCachedResourceObjects(void *pArchive);

void LEMBALL_FASTCALL SetStatusIndicatorActive(
    void *pObject, int nUnused, int fActive);

// FUNCTION: LEMBALL 0x0046B310
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

// FUNCTION: LEMBALL 0x0046B3B0
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

// FUNCTION: LEMBALL 0x0043A720
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

// FUNCTION: LEMBALL 0x00403265
void *InitializeBaseModeActionButtonPointSink(void *pObject) {
    int *pEntry;

    pEntry = (int *)pObject;
    *(void **)pEntry = g_LEVEL_QueuedRenderPointSinkFinalizeVtable;
    *(unsigned short *)((char *)pEntry + 6) = 0;
    *(void **)pEntry = g_LEVEL_ActionButtonPointSinkDeleteVtable;
    *(unsigned short *)(pEntry + 1) = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x004678C0
void ResetOrFlipVariantRenderBuffers(int nManager) {
    int nTarget;
    unsigned char *pbToken;
    VsGdiCompactGeometryTokenDispatch *pCompactTarget;

    *(int *)(unsigned long)(nManager + 0xc) = 0;
    if (*(int *)(unsigned long)(nManager + 0x38) == 0) {
        *(int *)(unsigned long)(nManager + 0x5c) = 0;
        *(int *)(unsigned long)(nManager + 0x40) = *(int *)(unsigned long)(nManager + 0x5c);
        *(int *)(unsigned long)(nManager + 0x44) = *(int *)(unsigned long)(nManager + 0x60);
        nTarget = *(int *)(*(int *)(unsigned long)(nManager + 0x1c) + 0xc);
        pCompactTarget = (VsGdiCompactGeometryTokenDispatch *)(
            *(int *)(*(int *)(unsigned long)(nTarget + 0x40) + 4) +
            0x40 + nTarget);
        pbToken = pCompactTarget->GetToken();
        *(unsigned char *)(unsigned long)(nManager + 0x44) = *pbToken;
        *(unsigned char *)(unsigned long)(nManager + 0x60) = *pbToken;
        return;
    }

    DispatchAndClearPointerQueue(
        (void *)(unsigned long)*(int *)(unsigned long)(nManager + 0x1c));
    *(int *)(unsigned long)(*(int *)(unsigned long)(nManager + 0x1c) + 4) = 0;
    *(int *)(unsigned long)(nManager + 0x4c) = 0;
    *(int *)(unsigned long)(nManager + 0x48) = 0;
    *(unsigned int *)(unsigned long)(nManager + 0x50) ^= 1;
}

// FUNCTION: LEMBALL 0x00467260
void *ConstructVariantRenderManager(void *pObject,
                                    int nOwner,
                                    int cResourceIds,
                                    int cPackagedRectEntries,
                                    int cActionButtonEntries,
                                    int nUnknown58,
                                    int fDoubleBuffered)
{
    int *pManager;
    int i;
    int cEntries;
    int *pEntry;

    pManager = (int *)pObject;
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
    ResetOrFlipVariantRenderBuffers((int)(unsigned long)pManager);
    return pObject;
}

// FUNCTION: LEMBALL 0x00467490
void LoadVariantRenderResource(void *pObject, int nResourceId) {
    int *pManager;
    short *pnResourceIndex;
    int iSlot;
    int *pnLoadedResource;

    pManager = (int *)pObject;
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
    ReleaseTypedResourceObjectReference(
        m_ppResourceObjects24[m_pResourceIds28[nResourceId]]);
    m_ppResourceObjects24[m_pResourceIds28[nResourceId]] = 0;
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

// FUNCTION: LEMBALL 0x004671B0
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

// FUNCTION: LEMBALL 0x0046B370
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

// FUNCTION: LEMBALL 0x004030BC
void *LEMBALL_FASTCALL InitializeFramedScreenRenderChildEntry(
    void *pObject)
{
    return InitializeFramedScreenRenderChildEntryBody(pObject);
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

// FUNCTION: LEMBALL 0x00403139
void *LEMBALL_FASTCALL InitializeHelperLocalRectUpdateEntry(
    void *pObject)
{
    return InitializeHelperLocalRectUpdateEntryBody(pObject);
}

void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntry(void *pObject);
void *LEMBALL_FASTCALL InitializeRenderPointRectSinkEntry(void *pObject);
void *LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntry(void *pObject);

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

// FUNCTION: LEMBALL 0x0040272F
void *LEMBALL_FASTCALL InitializePackagedSpriteRenderEntryThunk(void *pObject) {
    return InitializePackagedSpriteRenderEntry(pObject);
}

// FUNCTION: LEMBALL 0x004394C0
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

// FUNCTION: LEMBALL 0x0040135C
void *LEMBALL_FASTCALL InitializeRenderPointRectSinkEntryThunk(void *pObject) {
    return InitializeRenderPointRectSinkEntry(pObject);
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

// FUNCTION: LEMBALL 0x00401AC8
void *LEMBALL_FASTCALL InitializeStatusIndicatorPointSinkEntryThunk(void *pObject) {
    return InitializeStatusIndicatorPointSinkEntry(pObject);
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

// FUNCTION: LEMBALL 0x00402784
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

// FUNCTION: LEMBALL 0x00447260
void LEMBALL_FASTCALL RestoreFramedScreenRenderChildEntryVtableBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00401974
void LEMBALL_FASTCALL RestoreFramedScreenRenderChildEntryVtable(void *pObject) {
    RestoreFramedScreenRenderChildEntryVtableBody(pObject);
}

// FUNCTION: LEMBALL 0x00439710
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableABody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00401DDE
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableA(void *pObject) {
    RestorePackagedSpriteRenderEntryVtableABody(pObject);
}

// FUNCTION: LEMBALL 0x00439720
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableBBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00401D11
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableB(void *pObject) {
    RestorePackagedSpriteRenderEntryVtableBBody(pObject);
}

// FUNCTION: LEMBALL 0x00439730
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableCBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00402E19
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableC(void *pObject) {
    RestorePackagedSpriteRenderEntryVtableCBody(pObject);
}

// FUNCTION: LEMBALL 0x00439740
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableDBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x004018B1
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableD(void *pObject) {
    RestorePackagedSpriteRenderEntryVtableDBody(pObject);
}

// FUNCTION: LEMBALL 0x00439750
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableEBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00401B8B
void LEMBALL_FASTCALL RestorePackagedSpriteRenderEntryVtableE(void *pObject) {
    RestorePackagedSpriteRenderEntryVtableEBody(pObject);
}

// FUNCTION: LEMBALL 0x00432AC0
void LEMBALL_FASTCALL RestoreStatusIndicatorPointSinkEntryVtableBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00401307
void LEMBALL_FASTCALL RestoreStatusIndicatorPointSinkEntryVtable(void *pObject) {
    RestoreStatusIndicatorPointSinkEntryVtableBody(pObject);
}

// FUNCTION: LEMBALL 0x0044A9C0
void LEMBALL_FASTCALL RestoreLevelSelectionRenderChildEntryVtableBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00403558
void LEMBALL_FASTCALL RestoreLevelSelectionRenderChildEntryVtable(void *pObject) {
    RestoreLevelSelectionRenderChildEntryVtableBody(pObject);
}

// FUNCTION: LEMBALL 0x0044B630
void LEMBALL_FASTCALL RestoreLevelSelectionPointSinkFinalizeVtableBody(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x004022A2
void LEMBALL_FASTCALL RestoreLevelSelectionPointSinkFinalizeVtable(void *pObject) {
    RestoreLevelSelectionPointSinkFinalizeVtableBody(pObject);
}

// FUNCTION: LEMBALL 0x004511A0
void LEMBALL_FASTCALL RestoreCompositePointRectSinkFinalizeVtablesBody(void *pObject) {
    *(void **)((char *)pObject + 0x24) =
        g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x00401848
void LEMBALL_FASTCALL RestoreCompositePointRectSinkFinalizeVtables(void *pObject) {
    RestoreCompositePointRectSinkFinalizeVtablesBody(pObject);
}

// FUNCTION: LEMBALL 0x004698F0
void LEMBALL_FASTCALL RestoreQueuedRenderPointSinkVtable(void *pObject) {
    *(void **)pObject = g_LEVEL_QueuedRenderPointSinkFinalizeVtableStorage;
}

// FUNCTION: LEMBALL 0x004673D0
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

// FUNCTION: LEMBALL 0x00434EC0
void LEMBALL_FASTCALL ReleaseLevelScreenVariantStateWrapperBody(void *pObject, int nUnusedFastcallArgument, int nResourceId) {
    int *pManager;
    unsigned int uStatePointer;

    (void)nUnusedFastcallArgument;
    pManager = (int *)pObject;
    uStatePointer = *(unsigned int *)(unsigned long)(pManager[0x1d] +
                                                     (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4);
    if (uStatePointer != 0) {
        FreeVSMemBlock((void *)(unsigned long)uStatePointer);
        *(int *)(unsigned long)(pManager[0x1d] +
                                (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4) = 0;
    }
    ((LevelVariantRenderManagerView *)pObject)->ReleaseVariantRenderResource(nResourceId);
}

// FUNCTION: LEMBALL 0x00401CD5
void LEMBALL_FASTCALL ReleaseLevelScreenVariantStateWrapper(void *pObject, int nResourceId) {
    ReleaseLevelScreenVariantStateWrapperBody(pObject, 0, nResourceId);
}

// FUNCTION: LEMBALL 0x00434F00
void LEMBALL_FASTCALL ReleaseLevelScreenVariantStateWrapperRangeBody(void *pObject, int nUnusedFastcallArgument, int nFirstResourceId, int nLastResourceId) {
    int nResourceId;
    int *pManager;
    unsigned int uStatePointer;

    (void)nUnusedFastcallArgument;

    nResourceId = nFirstResourceId;
    pManager = (int *)pObject;
    while (nResourceId <= nLastResourceId) {
        uStatePointer = (unsigned int)*(void **)((char *)pManager[0x1d] +
                                                 (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4);
        if (uStatePointer != 0) {
            FreeVSMemBlock((void *)(unsigned long)uStatePointer);
            *(void **)((char *)pManager[0x1d] +
                       (int)*(short *)(unsigned long)(pManager[10] + nResourceId * 2) * 4) = 0;
        }
        ((LevelVariantRenderManagerView *)pObject)->ReleaseVariantRenderResource(nResourceId);
        ++nResourceId;
    }
}

// FUNCTION: LEMBALL 0x00402F1D
void LEMBALL_FASTCALL ReleaseLevelScreenVariantStateWrapperRange(void *pObject, int nFirstResourceId, int nLastResourceId) {
    ReleaseLevelScreenVariantStateWrapperRangeBody(pObject, 0, nFirstResourceId, nLastResourceId);
}

#define ReleaseVariantRenderResource(pObject, nResourceId) \
    ((LevelVariantRenderManagerView *)(pObject))->ReleaseVariantRenderResource(nResourceId)

int ReturnTrueVirtualHookBody(void);

// FUNCTION: LEMBALL 0x00402FEF
int ReturnTrueVirtualHook(void) {
    return ReturnTrueVirtualHookBody();
}

// FUNCTION: LEMBALL 0x00413090
int ReturnTrueVirtualHookBody(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x0044AA80
void *ConstructLevelScreenStatusIndicatorManager(void *pObject, int nStatusMode, void *pPrimaryContext) {
    int *pManager;
    unsigned int *puPointTable;
    int i;
    int nFrameCount;
    int *pSelector;
    short *paPoint;

    pManager = (int *)pObject;

    ConstructVariantRenderManager(pObject, nStatusMode, 0x2b6, 1, 1, 0, 0);
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

    LoadVariantRenderResource(pManager, pManager[0x21]);
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

// FUNCTION: LEMBALL 0x0044AE80
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

// FUNCTION: LEMBALL 0x00401708
void LEMBALL_FASTCALL ReleaseStatusIndicatorPrimaryBitmapThunk(
    void *pObject)
{
    ReleaseStatusIndicatorPrimaryBitmap(pObject);
}

// FUNCTION: LEMBALL 0x004338B0
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

    PruneUnreferencedCachedResourceObjects(g_pMainResourceArchive);
}

// FUNCTION: LEMBALL 0x0044AD60
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
#include "ENGINE/CORE/SAFEVT.H"
