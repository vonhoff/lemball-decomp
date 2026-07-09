#include "../game/game_app.h"
#include "../engine/memory_arena.h"
#include "../engine/runtime_init.h"
#include "network/nruntime.h"
#include "network/nstream.h"
#include <string.h>

extern int ReturnTrueVtableCallback(void);
extern int ReturnTrueVtableCallbackSecondary(void);
extern void NoopVtableCallback(void);

extern "C" {
HANDLE WINAPI CreateFileA(LPCSTR lpFileName,
                          DWORD dwDesiredAccess,
                          DWORD dwShareMode,
                          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                          DWORD dwCreationDisposition,
                          DWORD dwFlagsAndAttributes,
                          HANDLE hTemplateFile);
BOOL WINAPI CloseHandle(HANDLE hObject);
BOOL WINAPI WriteFile(HANDLE hFile,
                      const void *lpBuffer,
                      DWORD nNumberOfBytesToWrite,
                      DWORD *lpNumberOfBytesWritten,
                      void *lpOverlapped);
BOOL WINAPI ReadFile(HANDLE hFile,
                     void *lpBuffer,
                     DWORD nNumberOfBytesToRead,
                     DWORD *lpNumberOfBytesRead,
                     void *lpOverlapped);
DWORD WINAPI SetFilePointer(HANDLE hFile, LONG lDistanceToMove, LONG *lpDistanceToMoveHigh, DWORD dwMoveMethod);
DWORD WINAPI GetFileSize(HANDLE hFile, DWORD *lpFileSizeHigh);
BOOL WINAPI LockFile(HANDLE hFile,
                     DWORD dwFileOffsetLow,
                     DWORD dwFileOffsetHigh,
                     DWORD nNumberOfBytesToLockLow,
                     DWORD nNumberOfBytesToLockHigh);
BOOL WINAPI UnlockFile(HANDLE hFile,
                       DWORD dwFileOffsetLow,
                       DWORD dwFileOffsetHigh,
                       DWORD nNumberOfBytesToUnlockLow,
                       DWORD nNumberOfBytesToUnlockHigh);
}

enum {
    CREATE_NEW = 1,
    OPEN_EXISTING = 3,
    FILE_BEGIN = 0,
};

static void *g_NETWORK_ReturnTrueVtable[1] = {
    (void *)ReturnTrueVtableCallback,
};

int OpenWin32FileWrapperWithCreateFlag(void *pObject, LPCSTR pszPath, int nAccessMode, int fCreateNew);
int OpenWin32FileWrapperCreateNew(void *pObject, LPCSTR pszPath, int nAccessMode);
int ProbeWin32FileWrapperAccess(void *pObject, LPCSTR pszPath);
int CloseWin32FileWrapperIfOpen(void *pObject);
void FreeWin32FileWrapperOwnedPath(void *pObject);
void FreeWin32FileWrapperPath(char *pszPath);
int WriteWin32FileWrapper(void *pFileWrapper, const void *pvSource, DWORD cbWrite);
int ReadWin32FileWrapper(void *pFileWrapper, LPVOID pvTarget, DWORD cbRead);
int SeekWin32FileWrapper(void *pFileWrapper, DWORD dwOffset);
DWORD GetWin32FileWrapperLength(void *pFileWrapper);
DWORD GetWin32FileWrapperOffset(void *pFileWrapper);
int LockWin32FileRange(void *pFileWrapper, DWORD dwOffset, DWORD cbRange);
int UnlockWin32FileRange(void *pFileWrapper, DWORD dwOffset, DWORD cbRange);
void InitializeGlobalStateFileBackedEffStorageAdjustor(void *pObject);
void *GetWin32FileWrapperOwnedPath(void *pObject);
BYTE MapNestedEffCallbackFailureToStatus6(void *pObject);
void ClearFileBackedEffChannelAsyncErrorStatus(void *pObject);
void ClearNestedEffChannelAsyncStatus(void *pObject);
void AdjustAndClearEmbeddedFileBackedEffChannelAsyncErrorStatus(void *pObject);
BYTE AdjustAndMapGlobalStateFileBackedEffCallbackFailure(void *pObject);
void AdjustAndClearGlobalStateTimedEffTransportPendingWriteState(void *pObject);
void ClearAdjustedEffTransportPendingWriteState(void *pObject);
void AdjustAndWriteDualSlotTableFileBackedEffHeader(void *pObject);
void *AdjustAndDeleteDualSlotTableTimedFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory);
void *AdjustAndDeleteDualSlotTableDualFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory);
void *ClaimAckedEffTransportRecordPayload(void *pObject);
void QueueEffTransportConnectEvent(void *pObject);
void *DispatchAckedEffTransportPayloadEvent(void *pObject);
void DispatchPrefixedEffTransportControlStream(void *pObject, int nDispatchType, void *pEvent);
void QueueEffTransportPayloadEvent(void *pObject, unsigned short nType, void *pPayload);


struct NETWORK_ConstructionAdjustorVtable {
    int m_nSelfDelta00;
    int m_nPrimaryOffset;
    int m_nSecondaryOffset;
    int m_nTertiaryOffset;
    int m_nQuaternaryOffset;
    int m_nFatalOffset;
    int m_nRecordTableOffsetA;
    int m_nRecordTableOffsetB;
};

// These tables are not ordinary virtual tables. They are compiler-emitted
// adjustor metadata describing where embedded subobjects and thunk fronts live
// inside one larger aggregate object.

struct NETWORK_CompositeViewOffsets {
    int m_nSelfDelta00;
    int m_nTransportViewOffset04;
    int m_nTimedStreamViewOffset08;
    int m_nDualStreamViewOffset0c;
    int m_nDualThunkViewOffset10;
    int m_nMarkerViewOffset14;
    int m_nRecordTableAOffset18;
    int m_nRecordTableBOffset1c;
};

struct NETWORK_FileBackedChannelViewOffsets {
    int m_nSelfDelta00;
    int m_nChannelStateViewOffset04;
    int m_nStreamViewOffset08;
    int m_nFileWrapperViewOffset0c;
    int m_nFatalThunkViewOffset10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1c;
};

struct NETWORK_FileWrapperThunkOffsets {
    int m_nSelfDelta00;
    int m_nOpenThunkViewOffset04;
    int m_nNestedCallbackThunkOffset08;
    int m_nReserved0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1c;
};

struct NETWORK_EmbeddedStackViewOffsets {
    int m_nSelfDelta00;
    int m_nChannelStateViewOffset04;
    int m_nTimedStreamViewOffset08;
    int m_nDualStreamViewOffset0c;
    int m_nFinalFileThunkViewOffset10;
    int m_nFinalFatalThunkViewOffset14;
    int m_nFinalPrimaryThunkViewOffset18;
    int m_nFinalTimedThunkViewOffset1c;
};

struct NETWORK_EmbeddedStackFileViewOffsets {
    int m_nSelfDelta00;
    int m_nFileWrapperViewOffset04;
    int m_nChannelStateViewOffset08;
    int m_nTimedStreamViewOffset0c;
    int m_nDualStreamViewOffset10;
    int m_nReserved14;
    int m_nPrimaryThunkViewOffset18;
    int m_nFileViewThunkOffset1c;
};

static const NETWORK_ConstructionAdjustorVtable g_NETWORK_FileBackedDualStreamOffsets = {
    -0x44,
    -0x70,
    -0x0c,
    0x0c,
    0x38,
    0x90,
    0xa4,
    0x0047FCA0,
};
static void *g_NETWORK_FileBackedDualPrimaryThunkVtable = (void *)0x0049A5D8;
static void *g_NETWORK_FileBackedDualSecondaryThunkVtable = (void *)0x0049A5B0;
static void *g_NETWORK_FileBackedFatalFileWrapperVtable[] = {
    (void *)0x0047FCA0,
    (void *)OpenWin32FileWrapperWithCreateFlag,
    (void *)OpenWin32FileWrapperCreateNew,
    (void *)ProbeWin32FileWrapperAccess,
    (void *)CloseWin32FileWrapperIfOpen,
    (void *)FreeWin32FileWrapperPath,
    (void *)FreeWin32FileWrapperOwnedPath,
    (void *)WriteWin32FileWrapper,
    (void *)ReadWin32FileWrapper,
    (void *)SeekWin32FileWrapper,
    (void *)GetWin32FileWrapperLength,
    (void *)GetWin32FileWrapperOffset,
    (void *)LockWin32FileRange,
    (void *)UnlockWin32FileRange,
    (void *)ClearFileBackedEffChannelAsyncErrorStatus,
    (void *)GetWin32FileWrapperOwnedPath,
};

static const NETWORK_ConstructionAdjustorVtable g_NETWORK_FileBackedTimedStreamOffsets = {
    -0x44,
    -0x74,
    -0x0c,
    0x0c,
    0x3c,
    0xb0,
    0xc4,
    0x0047FCA0,
};
static void *g_NETWORK_FileBackedTimedPrimaryThunkVtable = (void *)0x0049A708;
static void *g_NETWORK_FileBackedTimedSecondaryThunkVtable = (void *)0x0049A6D8;

static const NETWORK_EmbeddedStackViewOffsets g_NETWORK_EmbeddedFileBackedTimedStreamOffsets = {
    -0x44,
    -0x74,
    0,
    0x08,
    0x38,
    0xb0,
    0x108,
    0x10c,
};
static const NETWORK_EmbeddedStackViewOffsets g_NETWORK_EmbeddedFileBackedDualStreamOffsets = {
    -0x44,
    -0xec,
    -0x44,
    -0x74,
    0,
    0x08,
    0x38,
    0xb0,
};
static const NETWORK_EmbeddedStackFileViewOffsets g_NETWORK_EmbeddedFileBackedPrimaryThunkOffsets = {
    -0x0c,
    -0x130,
    0x78,
    -0x2c,
    -0x18,
    0,
    -0x04,
    -0x18,
};
static const NETWORK_EmbeddedStackFileViewOffsets g_NETWORK_EmbeddedFileBackedTimedFileViewOffsets = {
    -0x0c,
    -0x144,
    -0x114,
    -0x40,
    -0x2c,
    0,
    -0x0c,
    -0x130,
};
static void *g_NETWORK_EmbeddedFileBackedFinalPrimaryThunkVtable = (void *)0x0049A838;
static void *g_NETWORK_EmbeddedFileBackedFinalTimedThunkVtable = (void *)0x0049A808;
static void *g_NETWORK_EmbeddedFileBackedFinalDualThunkVtable = (void *)0x0049A7E0;

// Global-state composite layout:
// 00 self-delta / owning view
// 04 transport-service view
// 08 timed-stream view
// 0C dual-stream view
// 10 dual-thunk-or-file view
// 14 marker / fatal / embedded-stack anchor
// 18 record-table-A or initial-stream-relative payload slot
// 1C record-table-B or peer-state payload slot
static const NETWORK_CompositeViewOffsets g_NETWORK_GlobalStateCompositeTimedStreamOffsets = {
    0,      // owning timed-stream view
    0x40,   // transport-service view
    0x70,   // timed-stream body
    0xe8,   // dual-stream body
    0x140,  // dual-thunk front
    0x144,  // marker/fatal anchor
    0x15c,  // record-table-A slot
    0x168,  // record-table-B slot
};
static const NETWORK_CompositeViewOffsets g_NETWORK_GlobalStateCompositeTransportOffsets = {
    -0x04,  // transport view back to owning header
    0x5c,   // timed-stream view
    0x8c,   // dual-stream view
    0x104,  // dual-thunk view
    0x15c,  // marker/fatal anchor
    0,      // transport-local payload slot
    0,      // transport-local payload slot
    0x40,   // initial-stream/timed-stream anchor
};
static const NETWORK_CompositeViewOffsets g_NETWORK_GlobalStateCompositeDualStreamOffsets = {
    -0x44,  // dual-stream view back to owner
    -0x74,  // transport view
    -0x04,  // timed-stream view
    0x5c,   // dual-stream body
    0x8c,   // dual-thunk front
    0x104,  // marker/fatal anchor
    0x15c,  // record-table-A slot
    0,      // no secondary record-table slot here
};
static const NETWORK_CompositeViewOffsets g_NETWORK_GlobalStateCompositeDualThunkOffsets = {
    -0x44,  // dual-thunk view back to owner
    -0xec,  // transport view
    -0x44,  // timed-stream view
    -0x74,  // dual-stream view
    -0x04,  // file-wrapper / thunk front itself
    0x5c,   // initial-stream anchor
    0x8c,   // peer-state payload slot
    0x104,  // record-table payload slot
};
static const NETWORK_ConstructionAdjustorVtable g_NETWORK_GlobalStateCompositeEmbeddedStackOffsets = {
    -0x0c,
    -0x134,
    -0x8c,
    -0x30,
    -0x18,
    0,
    -0x04,
    -0x1c,
};
static const NETWORK_ConstructionAdjustorVtable g_NETWORK_GlobalStateCompositeEmbeddedStackDataOffsets = {
    -0x0c,
    -0x148,
    -0x118,
    -0x44,
    -0x2c,
    0,
    -0x0c,
    -0x134,
};
static void *g_NETWORK_GlobalStateCompositeInitialVtable = (void *)0x0049A848;
static void *g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable = (void *)0x0049AA90;
static void *g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable = (void *)0x0049AA60;
static void *g_NETWORK_GlobalStateCompositeInitialDualThunkVtable = (void *)0x0049AA38;
static void *g_NETWORK_GlobalStateCompositeFinalVtable = (void *)0x0049A9D8;
static void *g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable = (void *)0x0049A9C8;
static void *g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable = (void *)0x0049A998;
static void *g_NETWORK_GlobalStateCompositeFinalDualThunkVtable = (void *)0x0049A970;
static void *g_NETWORK_GlobalStateCompositeFinalFatalFileWrapperVtable[] = {
    (void *)InitializeGlobalStateFileBackedEffStorageAdjustor,
    (void *)0,
    (void *)OpenWin32FileWrapperWithCreateFlag,
    (void *)OpenWin32FileWrapperCreateNew,
    (void *)ProbeWin32FileWrapperAccess,
    (void *)CloseWin32FileWrapperIfOpen,
    (void *)FreeWin32FileWrapperPath,
    (void *)FreeWin32FileWrapperOwnedPath,
    (void *)WriteWin32FileWrapper,
    (void *)ReadWin32FileWrapper,
    (void *)SeekWin32FileWrapper,
    (void *)GetWin32FileWrapperLength,
    (void *)GetWin32FileWrapperOffset,
    (void *)LockWin32FileRange,
    (void *)UnlockWin32FileRange,
    (void *)ClearFileBackedEffChannelAsyncErrorStatus,
    (void *)GetWin32FileWrapperOwnedPath,
};

// Dual-slot composite layout uses the same view ordering as the global-state
// composite, but its initial stream starts 8 bytes later and it carries two
// separate record tables.
static const NETWORK_CompositeViewOffsets g_NETWORK_DualSlotCompositeTimedStreamOffsets = {
    0,      // owning timed-stream view
    0x38,   // transport-service view
    0x68,   // timed-stream body
    0xe0,   // dual-stream body
    0x138,  // dual-thunk front
    0x13c,  // marker/fatal anchor
    0x154,  // record-table-A slot
    0x160,  // record-table-B slot
};
static const NETWORK_CompositeViewOffsets g_NETWORK_DualSlotCompositeTransportOffsets = {
    -0x04,  // transport view back to owning header
    0x60,   // timed-stream view
    0x90,   // dual-stream view
    0x108,  // dual-thunk view
    0x160,  // marker/fatal anchor
    0,      // transport-local payload slot
    0,      // transport-local payload slot
    0x38,   // initial-stream/timed-stream anchor
};
static const NETWORK_CompositeViewOffsets g_NETWORK_DualSlotCompositeDualStreamOffsets = {
    -0x44,  // dual-stream view back to owner
    -0x74,  // transport view
    -0x04,  // timed-stream view
    0x60,   // dual-stream body
    0x90,   // dual-thunk front
    0x108,  // marker/fatal anchor
    0x160,  // record-table-A slot
    0,      // no secondary record-table slot here
};
static const NETWORK_CompositeViewOffsets g_NETWORK_DualSlotCompositeDualThunkOffsets = {
    -0x44,  // dual-thunk view back to owner
    -0xec,  // transport view
    -0x44,  // timed-stream view
    -0x74,  // dual-stream view
    -0x04,  // file-wrapper / thunk front itself
    0x60,   // initial-stream anchor
    0x90,   // record-table-A payload slot
    0x108,  // record-table-B payload slot
};
static const NETWORK_ConstructionAdjustorVtable g_NETWORK_DualSlotCompositeEmbeddedStackOffsets = {
    -0x0c,
    -0x134,
    -0x8c,
    -0x30,
    -0x18,
    0,
    -0x04,
    -0x1c,
};
static const NETWORK_ConstructionAdjustorVtable g_NETWORK_DualSlotCompositeEmbeddedStackDataOffsets = {
    -0x0c,
    -0x148,
    -0x118,
    -0x44,
    -0x2c,
    0,
    -0x0c,
    -0x134,
};
static void *g_NETWORK_DualSlotCompositeFinalVtable = (void *)0x0049AC18;
static void *g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable = (void *)0x0049AC08;
static void *g_NETWORK_DualSlotCompositeFinalTimedThunkVtable = (void *)0x0049ABD8;
static void *g_NETWORK_DualSlotCompositeFinalDualThunkVtable = (void *)0x0049ABB0;
static void *g_NETWORK_DualSlotCompositeFinalFatalFileWrapperVtable[] = {
    (void *)AdjustAndWriteDualSlotTableFileBackedEffHeader,
    (void *)0,
    (void *)OpenWin32FileWrapperWithCreateFlag,
    (void *)OpenWin32FileWrapperCreateNew,
    (void *)ProbeWin32FileWrapperAccess,
    (void *)CloseWin32FileWrapperIfOpen,
    (void *)FreeWin32FileWrapperPath,
    (void *)FreeWin32FileWrapperOwnedPath,
    (void *)WriteWin32FileWrapper,
    (void *)ReadWin32FileWrapper,
    (void *)SeekWin32FileWrapper,
    (void *)GetWin32FileWrapperLength,
    (void *)GetWin32FileWrapperOffset,
    (void *)LockWin32FileRange,
    (void *)UnlockWin32FileRange,
    (void *)ClearFileBackedEffChannelAsyncErrorStatus,
    (void *)GetWin32FileWrapperOwnedPath,
};

static void *g_NETWORK_RuntimeChannelStackFatalThunkVtable = (void *)0x004990D8;
static void *g_NETWORK_CrtFatalRuntimeErrorThunk = (void *)0x00498070;
static void *g_NETWORK_CrtFatalRuntimeErrorDeleteThunk = (void *)0x0049A53C;
static void *g_NETWORK_MapNestedEffCallbackFailureThunk = (void *)0x0049A4F0;

struct NETWORK_EffStreamRecordSlot {
    void *m_pVtable;
    int m_nReserved04;
    char m_abStreamState08[0x24];
    unsigned short m_wCommittedMarker;
    unsigned short m_wObservedMarker;
    DWORD m_cbPayload;
    char m_szSourceName[0x15];
    char m_szTargetName[0x17];
};

struct NETWORK_ConstructionAdjustorVtable;
struct NETWORK_FileBackedDispatchOffsets;

struct NETWORK_EffStreamRecordSlotTable {
    void *m_pVtable;
    int m_nRecordHeaderOffset04;
    int m_nPayloadBaseOffset08;
    NETWORK_FileBackedDispatchOffsets *m_pChannelThunk0c;
    int m_nPendingSlot10;
    int m_nReserved14;
    int m_nAccumulatedStreamLength18;
    int m_nSaveRangeCursor1c;
    int m_nLoadSourceCursor20;
    int m_nReserved24;
    int m_nReserved28;
    NETWORK_EffStreamRecordSlot *m_pSlots;
    unsigned short *m_pwSlotFlags;
    int m_cSlots;
    int m_nCurrentChangedSlot38;

    NETWORK_EffStreamRecordSlotTable *Construct(int cSlots);
    void Destroy();
};

static const NETWORK_FileBackedChannelViewOffsets g_NETWORK_FileBackedDualChannelStateOffsets = {
    -0x0c,
    0x0c,
    0x38,
    0x90,
    0xa4,
    0x0047FCA0,
    0x0047F5B0,
    0x0047F640,
};

static const NETWORK_FileWrapperThunkOffsets g_NETWORK_FileBackedDualFileWrapperThunkOffsets = {
    -0x04,
    -0x18,
    -0x9c,
    0,
    -0x44,
    -0x70,
    -0x0c,
    0x0c,
};

static const NETWORK_FileBackedChannelViewOffsets g_NETWORK_FileBackedTimedChannelStateOffsets = {
    -0x0c,
    0x0c,
    0x3c,
    0xb0,
    0xc4,
    0x0047FCA0,
    0x0047F5B0,
    0x0047F640,
};

static const NETWORK_FileWrapperThunkOffsets g_NETWORK_FileBackedTimedFileWrapperThunkOffsets = {
    -0x04,
    -0x18,
    -0xbc,
    0,
    -0x44,
    -0x74,
    -0x0c,
    0x0c,
};

static const NETWORK_EmbeddedStackFileViewOffsets g_NETWORK_EmbeddedFileBackedFileWrapperThunkOffsets = {
    -0x04,
    -0x18,
    -0x11c,
    0,
    0,
    -0x100,
    -0xd0,
    -0x58,
};

static const NETWORK_EmbeddedStackViewOffsets g_NETWORK_EmbeddedFileBackedDualThunkOffsets = {
    0,
    -0x100,
    -0xd0,
    -0x58,
    -0x44,
    -0xec,
    -0x44,
    -0x74,
};

static const NETWORK_EmbeddedStackViewOffsets g_NETWORK_EmbeddedFileBackedFinalThunkOffsets = {
    0,
    0x08,
    0x38,
    0xb0,
    0x108,
    0x10c,
    0x120,
    0x12c,
};

static const NETWORK_EmbeddedStackViewOffsets g_NETWORK_GlobalStateCompositeFileWrapperOffsets = {
    0,
    -0x100,
    -0xd0,
    -0x58,
    -0x44,
    -0xec,
    -0x44,
    -0x74,
};

static const NETWORK_EmbeddedStackFileViewOffsets g_NETWORK_GlobalStateCompositeTimedFileThunkOffsets = {
    -0x04,
    -0x1c,
    -0x120,
    0,
    0,
    -0x100,
    -0xd0,
    -0x58,
};

static const NETWORK_EmbeddedStackViewOffsets g_NETWORK_GlobalStateCompositeEmbeddedStackFinalOffsets = {
    0,
    -0x150,
    -0x120,
    -0xa8,
    -0x50,
    -0x4c,
    -0x34,
    -0x28,
};

static const NETWORK_EmbeddedStackViewOffsets g_NETWORK_DualSlotCompositeFileWrapperOffsets = {
    0,
    -0x100,
    -0xd0,
    -0x58,
    -0x44,
    -0xec,
    -0x44,
    -0x74,
};

static const NETWORK_EmbeddedStackFileViewOffsets g_NETWORK_DualSlotCompositeTimedFileThunkOffsets = {
    -0x04,
    -0x1c,
    -0x120,
    0,
    0,
    -0x100,
    -0xd0,
    -0x58,
};

static const NETWORK_EmbeddedStackViewOffsets g_NETWORK_DualSlotCompositeEmbeddedStackFinalOffsets = {
    0,
    -0x150,
    -0x120,
    -0xa8,
    -0x50,
    -0x4c,
    -0x34,
    -0x28,
};

struct NETWORK_FileBackedDispatchOffsets {
    void *m_pReserved00;
    int m_nChannelStateViewOffset04;
    int m_nTimedStreamViewOffset08;
    int m_nFileWrapperViewOffset0c;
    int m_nSlotCursorStateOffset10;
    int m_nReadThunkOffset14;
    int m_nSlotCountViewOffset18;
    int m_nWriteThunkOffset20;
};

struct NETWORK_RecordWriterAdjustorOffsets {
    int m_nSelfDelta00;
    int m_nReserved04;
    int m_nReserved08;
    int m_nReserved0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nRecordTableViewOffset18;
    int m_nRecordWriterViewOffset1c;
};

struct NETWORK_GlobalSessionAdjustorOffsets {
    int m_nSelfDelta00;
    int m_nReserved04;
    int m_nEmbeddedStreamViewOffset08;
    int m_nReserved0c;
    int m_nTransportViewOffset10;
};

struct NETWORK_GlobalSessionAdjustorFront {
    NETWORK_GlobalSessionAdjustorOffsets *m_pOffsets;
    char m_abReserved04[0x34];
    void WriteAdjustedEffStreamWithGlobalSession(void);
};

struct NETWORK_AdjustorThunkHeader {
    int m_nThisDelta;
};

struct NETWORK_FileBackedMarkerOffsets {
    void *m_pReserved00;
    int m_nReserved04;
    int m_nReserved08;
    int m_nReserved0c;
    int m_nReadThunkOffset14;
    int m_nNestedThunkOffset18;
    int m_nStreamOffset1c;
    int m_nWriteThunkOffset20;
};

struct NETWORK_FileBackedAppendOffsets {
    void *m_pReserved00;
    int m_nOutputStateOffset04;
    int m_nNameProviderOffset08;
    int m_nFileWrapperOffset0c;
};

struct NETWORK_AdjustorSubobject {
    int m_nThisDelta;
    void *m_pVtable;
};

struct NETWORK_ThunkAdjustorGroup {
    NETWORK_ConstructionAdjustorVtable *m_pOffsets;
    NETWORK_AdjustorSubobject *m_pPrimary;
    NETWORK_AdjustorSubobject *m_pSecondary;
    NETWORK_AdjustorSubobject *m_pTertiary;
};

struct NETWORK_QuadThunkAdjustorGroup {
    NETWORK_ConstructionAdjustorVtable *m_pOffsets;
    NETWORK_AdjustorSubobject *m_pPrimary;
    NETWORK_AdjustorSubobject *m_pSecondary;
    NETWORK_AdjustorSubobject *m_pTertiary;
    NETWORK_AdjustorSubobject *m_pQuaternary;
};

struct NETWORK_QuintThunkAdjustorGroup {
    NETWORK_ConstructionAdjustorVtable *m_pOffsets;
    NETWORK_AdjustorSubobject *m_pPrimary;
    NETWORK_AdjustorSubobject *m_pSecondary;
    NETWORK_AdjustorSubobject *m_pTertiary;
    NETWORK_AdjustorSubobject *m_pQuaternary;
    NETWORK_AdjustorSubobject *m_pFatal;
};

struct NETWORK_EffTransportRuntimeServiceVtable {
    char *(*m_pGetLocalHostName)(void *pThis);
    void *m_pReserved04;
    void (*m_pSelectLocalHostName)(void *pThis, char *pszHostName);
};

struct NETWORK_EffTransportRuntimeService {
    NETWORK_EffTransportRuntimeServiceVtable *m_pVtable;
};

struct NETWORK_FileWrapperVtable {
    int (*m_pOpenWithCreateFlag)(void *pThis, LPCSTR pszPath, int nAccessMode, int fCreateNew);
    int (*m_pOpenCreateNew)(void *pThis, LPCSTR pszPath, int nAccessMode);
    int (*m_pProbeAccess)(void *pThis, LPCSTR pszPath);
    int (*m_pCloseIfOpen)(void *pThis);
    void (*m_pFreePath)(char *pszPath);
    void (*m_pFreeOwnedPath)(void *pThis);
    int (*m_pWrite)(void *pThis, const void *pvSource, DWORD cbWrite);
    int (*m_pRead)(void *pThis, LPVOID pvTarget, DWORD cbRead);
    int (*m_pSeek)(void *pThis, DWORD dwOffset);
    DWORD (*m_pGetLength)(void *pThis);
    DWORD (*m_pGetOffset)(void *pThis);
    int (*m_pLockRange)(void *pThis, DWORD dwOffset, DWORD cbRange);
    int (*m_pUnlockRange)(void *pThis, DWORD dwOffset, DWORD cbRange);
};

struct NETWORK_FileWrapperObject {
    NETWORK_FileWrapperVtable *m_pVtable;
};

struct NETWORK_FileWrapperOpenObject {
    NETWORK_FileWrapperVtable *m_pVtable;
};

struct NETWORK_InlineOpenWin32FileWrapper {
    void *m_pVtable;
    int m_nReserved04;
    int m_fOpen08;
    int m_nReserved0c;
    int m_nReserved10;
    void *m_pDeleteThunk14;
};

struct NETWORK_Win32FileWrapperPathOwner {
    void *m_pVtable;
    char *m_pszOwnedPath04;
};

struct NETWORK_CallbackThunkVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void *m_pReserved08;
    int (*m_pInvoke)(void *pThis);
    void (*m_pRelease)(void *pThis);
};

struct NETWORK_CallbackThunkObject {
    NETWORK_CallbackThunkVtable *m_pVtable;
};

struct NETWORK_DeleteVtable {
    void *m_apReserved00[5];
    void (*m_pDelete)(void *pThis, int nDeleteMode);
};

struct NETWORK_DeleteObject {
    NETWORK_DeleteVtable *m_pVtable;
};

struct NETWORK_ChannelStateHeader {
    void **m_pVtable;
    int m_nReserved04;
    int m_nSelectedPeer08;
    int m_nReserved0c;
    int m_fBusy10;
    int m_cSlots14;
    int m_fPending18;
    int m_fReceiving1c;
    unsigned short m_nSelectedPort20;
    unsigned short m_nReserved22;
    int m_fChannelOpen24;
};

struct NETWORK_PeerAddressServiceVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void (*m_pSelectPeerName)(void *pThis, char *pszName);
};

struct NETWORK_PeerAddressService {
    NETWORK_PeerAddressServiceVtable *m_pVtable;
};

struct NETWORK_TimedStreamHeader {
    void **m_pVtable;
    int m_nReserved04;
    int m_pPayloadBuffer08;
    int m_fOwnsPayload0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nBufferEnd18;
    int m_nReserved1c;
    int m_nReserved20;
    int m_nReserved24;
    int m_fBusy28;
    int *m_pTagBuffer2c;
    unsigned short m_nWord30;
    unsigned short m_nWord32;
    unsigned short m_nWord34;
    unsigned short m_nWord36;
    int m_nReserved38;
    DWORD m_dwLastTick3c;
};

struct NETWORK_FileBackedTimedChannelWrapperView {
    char m_abChannelState00[0x30];
    NETWORK_TimedStreamHeader m_TimedStream30;
    char m_abReserved70[0x34];
    void *m_pResetThunkA4;
    void *m_pSideBufferA8;
};

struct NETWORK_FileBackedCompositeWrapperView {
    char m_abChannelState00[0x30];
    NETWORK_TimedStreamHeader m_TimedStream30;
    char m_abReserved70[0x38];
    char m_abDualStreamA8[0x58];
    char m_abReserved100[4];
    void *m_pResetThunk104;
    void *m_pSideBuffer108;
};

struct NETWORK_EmbeddedFileBackedEffChannelStackWrapper {
    char m_abOwnerPrefix00[0x08];
    NETWORK_FileBackedCompositeWrapperView m_Runtime08;
};

struct NETWORK_FileBackedCompositeChannelWrapper {
    char m_abOwnerPrefix00[0x38];
    NETWORK_FileBackedCompositeWrapperView m_Runtime38;
};

struct NETWORK_GlobalStateFileBackedEffCompositeWrapper {
    char m_abOwnerPrefix00[0x60];
    NETWORK_FileBackedCompositeWrapperView m_Runtime60;
};

struct NETWORK_DualSlotTableFileBackedEffCompositeWrapper {
    char m_abOwnerPrefix00[100];
    NETWORK_FileBackedCompositeWrapperView m_Runtime64;
};

struct NETWORK_DualFileBackedEffChannelConstructorView {
    char m_abReserved00[0x18];
    char m_abChannelState18[0x2c];
    char m_abDualHandleStream44[0x58];
    NETWORK_InlineOpenWin32FileWrapper m_FileWrapper9c;
};

struct NETWORK_TimedFileBackedEffChannelConstructorView {
    char m_abReserved00[0x18];
    char m_abChannelState18[0x30];
    NETWORK_TimedStreamHeader m_TimedStream48;
    char m_abReserved88[0x34];
    NETWORK_InlineOpenWin32FileWrapper m_FileWrapperbc;
};

struct NETWORK_EmbeddedFileBackedEffChannelStackConstructorView {
    char m_abReserved00[0x08];
    char m_abChannelState08[0x30];
    NETWORK_TimedStreamHeader m_TimedStream38;
    char m_abReserved78[0x38];
    char m_abDualHandleStreamB0[0x58];
    NETWORK_InlineOpenWin32FileWrapper m_FileWrapper10c;
    char m_abReserved124[0x08];
    char m_abDualFileChannel12c[0xA4];
    char m_abTimedFileChannel1d0[0xD8];
};

struct NETWORK_FileBackedCompositeInitialStream {
    void *m_pVtable;
    int m_nReserved04;
    char m_abState08[0x10];
    int m_nSerializedLength18;
    char m_abReserved1c[0x0e];
    unsigned short m_wLatch2a;
    unsigned short m_wFlags2c;
    unsigned short m_wReserved2e;
};

struct NETWORK_Eff512ByteStateStream {
    void *m_pVtable;
    int m_nReserved04;
    int m_pPayloadBuffer08;
    int m_fOwnsPayload0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nSerializedLength18;
    int m_nReserved1c;
    int m_nReserved20;
    int m_nReserved24;
    int m_fBusy28;
    unsigned char *m_pbStateBytes2c;

    NETWORK_Eff512ByteStateStream *Construct();
};

struct NETWORK_GlobalStateCompositeLayout {
    NETWORK_CompositeViewOffsets *m_pTransportOffsets00;
    char m_abReserved04[0x1c];
    NETWORK_CompositeViewOffsets *m_pTimedStreamOffsets20;
    NETWORK_FileBackedCompositeInitialStream m_InitialStream24;
    int m_nReserved54;
    char m_abReserved58[8];
    char m_abChannelState60[0x30];
    char m_abTimedStream90[0x78];
    char m_abDualStream108[0x58];
    NETWORK_CompositeViewOffsets *m_pDualThunkOffsets14c;
    char m_abReserved150[0x10];
    const NETWORK_EmbeddedStackViewOffsets *m_pEmbeddedStackTransportOffsets160;
    char m_abReserved164[0x1c];
    const NETWORK_EmbeddedStackFileViewOffsets *m_pEmbeddedStackFileViewOffsets180;
    char m_abReserved184[0x10];
    const NETWORK_EmbeddedStackViewOffsets *m_pEmbeddedStackTimedStreamOffsets194;
    char m_abReserved198[0x10];
    const NETWORK_EmbeddedStackFileViewOffsets *m_pEmbeddedStackDualStreamOffsets1a8;
};

struct NETWORK_DualSlotCompositeLayout {
    NETWORK_CompositeViewOffsets *m_pTransportOffsets00;
    char m_abReserved04[0x28];
    NETWORK_CompositeViewOffsets *m_pTimedStreamOffsets2c;
    NETWORK_FileBackedCompositeInitialStream m_InitialStream30;
    char m_abReserved60[4];
    char m_abChannelState64[0x30];
    char m_abTimedStream94[0x78];
    char m_abDualStream10c[0x44];
    NETWORK_CompositeViewOffsets *m_pDualThunkOffsets150;
    char m_abReserved154[0x10];
    const NETWORK_EmbeddedStackViewOffsets *m_pEmbeddedStackTransportOffsets164;
    char m_abReserved168[0x1c];
    const NETWORK_EmbeddedStackFileViewOffsets *m_pEmbeddedStackFileViewOffsets184;
    char m_abReserved188[0x10];
    const NETWORK_EmbeddedStackViewOffsets *m_pEmbeddedStackTimedStreamOffsets198;
    char m_abReserved19c[0x10];
    const NETWORK_EmbeddedStackFileViewOffsets *m_pEmbeddedStackDualStreamOffsets1ac;
};

struct NETWORK_GlobalStateCompositeTransportFront {
    NETWORK_CompositeViewOffsets *m_pViewOffsets00;
    NETWORK_FileBackedCompositeInitialStream m_InitialStream04;
};

struct NETWORK_DualSlotCompositeTransportFront {
    NETWORK_CompositeViewOffsets *m_pViewOffsets00;
    NETWORK_FileBackedCompositeInitialStream m_InitialStream04;
};

struct NETWORK_EmbeddedFileBackedStackOpenWrapperFront {
    char m_abReserved00[0x10c];
    NETWORK_InlineOpenWin32FileWrapper m_FileWrapper10c;
    const NETWORK_EmbeddedStackFileViewOffsets *m_pFileWrapperThunk124;
};

struct NETWORK_GlobalStateCompositeOpenWrapperFront {
    char m_abReserved00[0x164];
    NETWORK_InlineOpenWin32FileWrapper m_FileWrapper164;
    const NETWORK_EmbeddedStackFileViewOffsets *m_pFileWrapperThunk180;
};

struct NETWORK_DualSlotCompositeOpenWrapperFront {
    char m_abReserved00[0x168];
    NETWORK_InlineOpenWin32FileWrapper m_FileWrapper168;
    const NETWORK_EmbeddedStackFileViewOffsets *m_pFileWrapperThunk184;
};

struct NETWORK_EmbeddedFileBackedStackFront {
    const NETWORK_EmbeddedStackViewOffsets *m_pFinalThunkOffsets;
    char m_abReserved04[0x78];
    const NETWORK_EmbeddedStackViewOffsets *m_pTimedStreamOffsets7c;
    char m_abReserved80[0x74];
    const NETWORK_EmbeddedStackViewOffsets *m_pDualStreamOffsetsf4;
    char m_abReservedf8[0x10];
    const NETWORK_EmbeddedStackViewOffsets *m_pDualThunkOffsets108;
    NETWORK_InlineOpenWin32FileWrapper m_FileWrapper10c;
    char m_abReserved124To138[0x14];
    const NETWORK_EmbeddedStackFileViewOffsets *m_pPrimaryThunkOffsets138;
    char m_abReserved13c[0x10];
    const NETWORK_EmbeddedStackFileViewOffsets *m_pTimedFileViewOffsets14c;
};

struct NETWORK_DualFileBackedEffChannelFront {
    char m_abReserved00[0x0c];
    NETWORK_FileBackedChannelViewOffsets *m_pThunk0c;
    int m_nMode10;
    char m_abReserved14[0x74];
    const NETWORK_ConstructionAdjustorVtable *m_pDualStreamOffsets88;
    char m_abReserved8c[0x28];
    NETWORK_FileWrapperThunkOffsets *m_pFileThunkb4;
    char m_abReservedb8[0x04];
    NETWORK_InlineOpenWin32FileWrapper m_FileWrapper9c;
};

struct NETWORK_TimedFileBackedEffChannelFront {
    char m_abReserved00[0x0c];
    NETWORK_FileBackedChannelViewOffsets *m_pThunk0c;
    int m_nMode10;
    char m_abReserved14[0x78];
    const NETWORK_ConstructionAdjustorVtable *m_pTimedStreamOffsets8c;
    char m_abReserved90[0x44];
    NETWORK_FileWrapperThunkOffsets *m_pFileThunkd4;
    char m_abReservedd8[0x04];
    NETWORK_InlineOpenWin32FileWrapper m_FileWrapperbc;
};

struct NETWORK_ChannelStateRuntimeVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void (*m_pHandleReadFailure)(void *pThis);
};

struct NETWORK_TimedStreamRuntimeVtable {
    void *m_apReserved00[6];
    void (*m_pServiceLoadedPacket)(void *pThis);
};

struct NETWORK_FileBackedEffRecordCursor {
    void **m_pVtable;
    int m_nRecordHeaderOffset04;
    int m_nPayloadBaseOffset08;
    NETWORK_AdjustorSubobject m_ChannelStateThunk0c;
    NETWORK_AdjustorSubobject m_TimedStreamThunk1c;
    NETWORK_AdjustorSubobject m_FileWrapperThunk2c;
    int m_nCurrentSlot3c;
};

struct NETWORK_RecordWriterAdjustorFront {
    const NETWORK_RecordWriterAdjustorOffsets *m_pOffsets;
    char m_abReserved04[0x34];
};

struct NETWORK_FileBackedMarkerStream {
    int m_anReserved00[11];
    unsigned short m_wReferenceCount2c;
    unsigned short m_wReserved2e;
};

struct NETWORK_FileBackedMarkerState {
    void **m_pVtable;
    NETWORK_FileBackedMarkerStream m_Stream04;
};

struct NETWORK_FileBackedMarkerOwner {
    NETWORK_FileBackedMarkerOffsets *m_pOffsets;
    NETWORK_FileBackedMarkerState m_Marker04;
};

struct NETWORK_FileBackedMarkerReleaseFront {
    char m_abReserved00[0x38];
};

struct NETWORK_FileBackedCallbackStatusFront {
    char m_abReserved00[0x1c];
};

struct NETWORK_FileBackedAppendOwner {
    NETWORK_FileBackedAppendOffsets *m_pOffsets;
    char m_abReserved04[0x18];
};

struct NETWORK_FileBackedOpenRetryVtable {
    void (*m_pResetOrClose)(void *pThis);
};

struct NETWORK_FileBackedOpenRetryOwner {
    NETWORK_FileBackedOpenRetryVtable *m_pVtable;
    NETWORK_FileBackedAppendOffsets *m_pOffsets;
};

struct NETWORK_GlobalStateFileBackedStorageOwner {
    NETWORK_FileBackedDispatchOffsets *m_pOffsets;
    NETWORK_FileBackedCompositeInitialStream m_InitialStream04;
};

struct NETWORK_FileBackedRecordServiceView {
    NETWORK_EffStreamRecordSlotTable *m_pRecordTable;
    NETWORK_ChannelStateHeader *m_pChannelState;
    NETWORK_FileWrapperObject *m_pFileWrapper;
};

struct NETWORK_FileBackedRecordPayloadView {
    NETWORK_EffStreamRecordSlotTable *m_pRecordTable;
    NETWORK_ChannelStateHeader *m_pChannelState;
    NETWORK_TimedStreamHeader *m_pTimedStream;
    NETWORK_FileWrapperObject *m_pFileWrapper;
};

struct NETWORK_FileBackedRangeLoadView {
    NETWORK_EffStreamRecordSlotTable *m_pRecordTable;
    NETWORK_FileWrapperObject *m_pFileWrapper;
};

struct NETWORK_EffStreamSerializedLengthView {
    char m_abReserved00[0x18];
    DWORD m_cbSerialized18;
};

struct NETWORK_SlotCountHolder {
    char m_abReserved00[8];
    int m_cSlots;
};

struct NETWORK_StreamNameProviderVtable {
    char *(*m_pGetName)(void *pThis);
};

struct NETWORK_StreamNameProvider {
    NETWORK_StreamNameProviderVtable *m_pVtable;
};

struct NETWORK_FileBackedAppendNameProviderState {
    DWORD m_dwLastWriteTime00;
    char m_abReserved04[0x30];
    NETWORK_StreamNameProvider m_NameProvider34;
};

struct NETWORK_SideBufferOwnerA4 {
    char m_abReserved00[0xa4];
    void *m_pResetThunk;
    void *m_pSideBuffer;
};

struct NETWORK_SideBufferOwner104 {
    char m_abReserved00[0x104];
    void *m_pResetThunk;
    void *m_pSideBuffer;
};

void *g_pGlobalStateEff512ByteStream = 0;
void *g_pEffTransportPacketBuffer = 0;
int g_cbEffTransportCurrentPacketBytes = 0;

extern void *g_pEffTransportRuntimeService;
extern void *g_pEffTransportPeerAddressState;
extern void *g_pActiveNetworkRuntimeWindow;
extern char *g_pszFileBasedNetworkLocalHostName;
extern char *g_pszFileBasedNetworkConfiguredPath;
extern int g_cbEffTransportMaxPacketBytes;

extern void ResetEffStreamStateFields(void *pEffStreamSubobject);
extern void *ConstructEffStreamChannelState(void *pChannelState);
extern void DestroyEffStreamChannelState(void *pChannelState);
extern void *ConstructTimedEffStream(void *pStream, int fConstructChannelState);
extern void DestroyTimedEffStream(void *pStream);
extern void *ConstructDualHandleEffStream(void *pStream, int fConstructChannelState);
extern void DestroyDualHandleEffStream(void *pStream);
extern void *ConstructEffTransportRuntimeChannelStack(void *pChannelStack, int fConstructEmbeddedObjects);
extern void ReleaseEffTransportRuntimeBuffers(int nChannelStateBase);
extern void *ConstructCompositeEffTransportStack(void *pObject, int fConstructEmbeddedObjects);
extern void RestoreCompositeEffTransportVtables(int nObjectBasePlus0x30);
extern DWORD WriteEffStreamToLockedFile(void *pFileWrapper, int *pStream, int fWritePayload, int fFlush);
extern void ScheduleNetworkRuntimeTimerEvent(void *pRuntimeWindow, int nTicks);
extern int LoadEffStreamFromMemory(void *pStream, int nSourceBuffer);
extern void ProcessEffTransportPacketHeader(int *pStream);
extern void SetEffStreamChannelAsyncErrorStatus(void *pChannelState, int nStatus);
extern void ClearEffTransportPendingWriteState(int nAdjustedObject);
extern void WriteEffStreamU16BE(void *pStream, unsigned short nValue);
extern void WriteEffStreamU32BE(void *pStream, unsigned int nValue);
extern void WriteEffStreamBytes(void *pStream, const void *pvSource, unsigned int cbWrite);
extern void ReadEffStreamU16BE(void *pStream, unsigned char *pbTarget);
extern void ReadEffStreamU32BE(void *pStream, unsigned char *pbTarget);
extern void ReadEffStreamBytes(void *pStream, void *pvTarget, unsigned int cbRead);
extern void SaveEffStreamToMemoryRange(void *pStream, int nTargetBuffer, int cbRange);

extern "C" DWORD timeGetTime(void);

int OpenWin32FileWrapperWithCreateFlag(void *pObject, LPCSTR pszPath, int nAccessMode, int fCreateNew);
int OpenWin32FileWrapperCreateNew(void *pObject, LPCSTR pszPath, int nAccessMode);
int ProbeWin32FileWrapperAccess(void *pObject, LPCSTR pszPath);
int CloseWin32FileWrapperIfOpen(void *pObject);
void FreeWin32FileWrapperOwnedPath(void *pObject);
void FreeWin32FileWrapperPath(char *pszPath);
int WriteWin32FileWrapper(void *pFileWrapper, const void *pvSource, DWORD cbWrite);
int ReadWin32FileWrapper(void *pFileWrapper, LPVOID pvTarget, DWORD cbRead);
int SeekWin32FileWrapper(void *pFileWrapper, DWORD dwOffset);
DWORD GetWin32FileWrapperLength(void *pFileWrapper);
DWORD GetWin32FileWrapperOffset(void *pFileWrapper);
int LockWin32FileRange(void *pFileWrapper, DWORD dwOffset, DWORD cbRange);
int UnlockWin32FileRange(void *pFileWrapper, DWORD dwOffset, DWORD cbRange);
void InitializeGlobalStateFileBackedEffStorage(int nObjectBasePlus0x15c);
void SaveEffStreamRecordSlot(void *pSlot);
void LoadEffStreamRecordSlot(void *pSlot);
NETWORK_EffStreamRecordSlot *FindNextChangedEffStreamRecordSlot(void *pObject);
void SaveEffStreamRecordSlotTable(void *pObject);
void LoadEffStreamRecordSlotTable(void *pObject);
void *DeleteEffStreamRecordSlotTable(void *pObject, BYTE fDeleteFlags);
void *DeleteEffStreamRecordSlot(void *pObject, BYTE fDeleteFlags);
void WriteEff512ByteStatePayload(void *pObject);
void LoadEff512ByteStatePayload(void *pObject);
void *DeleteEff512ByteStateStream(void *pObject, BYTE fDeleteFlags);

static void *g_NETWORK_OpenWin32FileWrapperVtable[] = {
    (void *)OpenWin32FileWrapperWithCreateFlag,
    (void *)OpenWin32FileWrapperCreateNew,
    (void *)ProbeWin32FileWrapperAccess,
    (void *)CloseWin32FileWrapperIfOpen,
    (void *)FreeWin32FileWrapperPath,
    (void *)FreeWin32FileWrapperOwnedPath,
    (void *)WriteWin32FileWrapper,
    (void *)ReadWin32FileWrapper,
    (void *)SeekWin32FileWrapper,
    (void *)GetWin32FileWrapperLength,
    (void *)GetWin32FileWrapperOffset,
    (void *)LockWin32FileRange,
    (void *)UnlockWin32FileRange,
};

static void *g_NETWORK_EffRecordSlotVtable[] = {
    (void *)ReturnTrueVtableCallback,
    (void *)ReturnTrueVtableCallbackSecondary,
    (void *)LoadEffStreamRecordSlot,
    (void *)NoopVtableCallback,
    (void *)SaveEffStreamRecordSlot,
    (void *)DeleteEffStreamRecordSlot,
};

static void *g_NETWORK_EffRecordSlotTableVtable[] = {
    (void *)ReturnTrueVtableCallback,
    (void *)ReturnTrueVtableCallbackSecondary,
    (void *)LoadEffStreamRecordSlotTable,
    (void *)NoopVtableCallback,
    (void *)SaveEffStreamRecordSlotTable,
    (void *)DeleteEffStreamRecordSlotTable,
};

static void *g_NETWORK_EffStateStreamVtable[] = {
    (void *)ReturnTrueVtableCallback,
    (void *)ReturnTrueVtableCallbackSecondary,
    (void *)LoadEff512ByteStatePayload,
    (void *)NoopVtableCallback,
    (void *)WriteEff512ByteStatePayload,
    (void *)DeleteEff512ByteStateStream,
};

// FUNCTION: LEMBALL 0x0047F5B0
int OpenWin32FileWrapperWithCreateFlag(void *pObject, LPCSTR pszPath, int nAccessMode, int fCreateNew) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;
    HANDLE hFile;
    DWORD dwCreationDisposition;
    size_t cchPath;
    char *pszCopy;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pObject;
    (void)nAccessMode;
    cchPath = strlen(pszPath) + 1;
    pszCopy = (char *)AllocateVSMemBlock((int)cchPath);
    pWrapper->m_nReserved04 = (int)(unsigned long)pszCopy;
    memcpy(pszCopy, pszPath, cchPath);
    dwCreationDisposition = (fCreateNew == 0) ? OPEN_EXISTING : CREATE_NEW;
    hFile = CreateFileA(pszPath, 0xc0000000, 3, (LPSECURITY_ATTRIBUTES)0, dwCreationDisposition, 0x80, (HANDLE)0);
    pWrapper->m_nReserved0c = (int)(unsigned long)hFile;
    if (hFile == (HANDLE)-1) {
        return 0;
    }

    pWrapper->m_fOpen08 = 0;
    pWrapper->m_nReserved10 = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x0047F640
int OpenWin32FileWrapperCreateNew(void *pObject, LPCSTR pszPath, int nAccessMode) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pObject;
    pWrapper->m_nReserved10 = 0;
    return OpenWin32FileWrapperWithCreateFlag(pWrapper, pszPath, nAccessMode, 1);
}

// FUNCTION: LEMBALL 0x0047F680
int ProbeWin32FileWrapperAccess(void *pObject, LPCSTR pszPath) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;
    HANDLE hFile;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pObject;
    hFile = CreateFileA(pszPath, 0xc0000000, 3, (LPSECURITY_ATTRIBUTES)0, OPEN_EXISTING, 0x80, (HANDLE)0);
    pWrapper->m_nReserved0c = (int)(unsigned long)hFile;
    if (hFile == (HANDLE)-1) {
        return 0;
    }

    CloseHandle(hFile);
    return 1;
}

// FUNCTION: LEMBALL 0x0047F8C0
int CloseWin32FileWrapperIfOpen(void *pObject) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pObject;
    if (pWrapper->m_fOpen08 == 0 && CloseHandle((HANDLE)(unsigned long)pWrapper->m_nReserved0c) != 0) {
        pWrapper->m_fOpen08 = 1;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0047F660
void FreeWin32FileWrapperOwnedPath(void *pObject) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pObject;
    FreeVSMemBlock((void *)(unsigned long)pWrapper->m_nReserved04);
}

// FUNCTION: LEMBALL 0x0047F670
void FreeWin32FileWrapperPath(char *pszPath) {
    FreeVSMemBlock(pszPath);
}

// FUNCTION: LEMBALL 0x0047F6C0
int WriteWin32FileWrapper(void *pFileWrapper, const void *pvSource, DWORD cbWrite) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;
    BOOL fSuccess;
    DWORD cbWritten;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pFileWrapper;
    fSuccess = WriteFile((HANDLE)(unsigned long)pWrapper->m_nReserved0c, pvSource, cbWrite, &cbWritten, 0);
    if (fSuccess == 0) {
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A41F8);
        AppendIntToStream(g_pErrorOutputStream, GetLastError());
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A4208);
        return 0;
    }

    pWrapper->m_nReserved10 += (int)cbWritten;
    if (cbWrite != cbWritten) {
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A420C);
        AppendUIntToStream(g_pErrorOutputStream, cbWritten);
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A421C);
        AppendIntToStream(g_pErrorOutputStream, cbWrite);
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A4238);
        return 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0047F780
int ReadWin32FileWrapper(void *pFileWrapper, LPVOID pvTarget, DWORD cbRead) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;
    BOOL fSuccess;
    DWORD cbReadNow;
    DWORD dwError;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pFileWrapper;
    fSuccess = ReadFile((HANDLE)(unsigned long)pWrapper->m_nReserved0c, pvTarget, cbRead, &cbReadNow, 0);
    if (fSuccess == 0) {
        dwError = GetLastError();
        if (dwError != 0x21) {
            AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A423C);
            AppendIntToStream(g_pErrorOutputStream, dwError);
            AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A424C);
        }
        return 0;
    }

    pWrapper->m_nReserved10 += (int)cbReadNow;
    if (cbReadNow != cbRead) {
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A4250);
        AppendUIntToStream(g_pErrorOutputStream, cbReadNow);
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A4260);
        AppendIntToStream(g_pErrorOutputStream, cbRead);
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A4278);
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A427C);
        AppendCStringToStream(g_pErrorOutputStream, (const char *)(unsigned long)pWrapper->m_nReserved04);
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A4288);
        AppendUIntToStream(g_pErrorOutputStream, GetWin32FileWrapperLength(pWrapper));
        AppendCStringToStream(g_pErrorOutputStream, (const char *)0x004A4294);
        return 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0047F890
int SeekWin32FileWrapper(void *pFileWrapper, DWORD dwOffset) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pFileWrapper;
    pWrapper->m_nReserved10 = (int)dwOffset;
    return SetFilePointer((HANDLE)(unsigned long)pWrapper->m_nReserved0c, dwOffset, 0, FILE_BEGIN) != 0xffffffff;
}

// FUNCTION: LEMBALL 0x0047F8F0
DWORD GetWin32FileWrapperLength(void *pFileWrapper) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pFileWrapper;
    return GetFileSize((HANDLE)(unsigned long)pWrapper->m_nReserved0c, 0);
}

// FUNCTION: LEMBALL 0x0047F8B0
DWORD GetWin32FileWrapperOffset(void *pFileWrapper) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pFileWrapper;
    return (DWORD)pWrapper->m_nReserved10;
}

// FUNCTION: LEMBALL 0x0047F900
int LockWin32FileRange(void *pFileWrapper, DWORD dwOffset, DWORD cbRange) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pFileWrapper;
    return LockFile((HANDLE)(unsigned long)pWrapper->m_nReserved0c, dwOffset, 0, cbRange, 0);
}

// FUNCTION: LEMBALL 0x0047F920
int UnlockWin32FileRange(void *pFileWrapper, DWORD dwOffset, DWORD cbRange) {
    NETWORK_InlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (NETWORK_InlineOpenWin32FileWrapper *)pFileWrapper;
    return UnlockFile((HANDLE)(unsigned long)pWrapper->m_nReserved0c, dwOffset, 0, cbRange, 0);
}

// FUNCTION: LEMBALL 0x00479920
void *ClearFileBackedEffChannelStateWord(void *pObject) {
    *(int *)pObject = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479540
void *InitializeEffStreamRecordSlot(void *pSlot) {
    NETWORK_EffStreamRecordSlot *pRecordSlot;

    pRecordSlot = (NETWORK_EffStreamRecordSlot *)pSlot;
    pRecordSlot->m_pVtable = g_NETWORK_ReturnTrueVtable;
    pRecordSlot->m_nReserved04 = 0;
    ResetEffStreamStateFields(pSlot);
    pRecordSlot->m_wObservedMarker = 1;
    *(int *)((char *)pSlot + 0x18) = *(int *)((char *)pSlot + 0x18) + 0x30;
    pRecordSlot->m_wCommittedMarker = 0;
    pRecordSlot->m_pVtable = g_NETWORK_EffRecordSlotVtable;
    pRecordSlot->m_cbPayload = 0;
    pRecordSlot->m_szTargetName[0] = '\0';
    pRecordSlot->m_szSourceName[0] = '\0';
    return pSlot;
}

// FUNCTION: LEMBALL 0x00479580
void SaveEffStreamRecordSlot(void *pSlot) {
    NETWORK_EffStreamRecordSlot *pRecordSlot;

    pRecordSlot = (NETWORK_EffStreamRecordSlot *)pSlot;
    ++pRecordSlot->m_wCommittedMarker;
    WriteEffStreamU16BE(pRecordSlot, pRecordSlot->m_wCommittedMarker);
    pRecordSlot->m_wObservedMarker = pRecordSlot->m_wCommittedMarker;
    WriteEffStreamU32BE(pRecordSlot, pRecordSlot->m_cbPayload);
    WriteEffStreamBytes(pRecordSlot, pRecordSlot->m_szTargetName, 0x15);
    WriteEffStreamBytes(pRecordSlot, pRecordSlot->m_szSourceName, 0x15);
}

// FUNCTION: LEMBALL 0x004795D0
void LoadEffStreamRecordSlot(void *pSlot) {
    NETWORK_EffStreamRecordSlot *pRecordSlot;

    pRecordSlot = (NETWORK_EffStreamRecordSlot *)pSlot;
    ReadEffStreamU16BE(pRecordSlot, (unsigned char *)&pRecordSlot->m_wCommittedMarker);
    if (pRecordSlot->m_wObservedMarker != pRecordSlot->m_wCommittedMarker) {
        ReadEffStreamU32BE(pRecordSlot, (unsigned char *)&pRecordSlot->m_cbPayload);
        ReadEffStreamBytes(pRecordSlot, pRecordSlot->m_szTargetName, 0x15);
        ReadEffStreamBytes(pRecordSlot, pRecordSlot->m_szSourceName, 0x15);
        return;
    }
    *(int *)((char *)pSlot + 0x20) += 0x2e;
}

// FUNCTION: LEMBALL 0x00479620
NETWORK_EffStreamRecordSlotTable *NETWORK_EffStreamRecordSlotTable::Construct(int cSlots) {
    int *pSlotStorage;
    NETWORK_EffStreamRecordSlot *pSlot;
    int i;

    m_pVtable = g_NETWORK_ReturnTrueVtable;
    m_nRecordHeaderOffset04 = 0;
    ResetEffStreamStateFields(this);
    m_cSlots = cSlots;
    m_pVtable = g_NETWORK_EffRecordSlotTableVtable;

    pSlotStorage = (int *)AllocateVSMemBlock(cSlots * 0x60 + 4);
    if (pSlotStorage == 0) {
        m_pSlots = 0;
    } else {
        *pSlotStorage = cSlots;
        pSlot = (NETWORK_EffStreamRecordSlot *)(pSlotStorage + 1);
        i = cSlots;
        while (--i >= 0) {
            InitializeEffStreamRecordSlot(pSlot);
            ++pSlot;
        }
        m_pSlots = (NETWORK_EffStreamRecordSlot *)(pSlotStorage + 1);
    }

    m_pwSlotFlags = (unsigned short *)AllocateVSMemBlock((unsigned int)(m_cSlots * 2));
    i = 0;
    if (0 < m_cSlots) {
        do {
            m_pwSlotFlags[i] = 0;
            ++i;
        } while (i < m_cSlots);
    }

    m_nAccumulatedStreamLength18 += *(int *)((char *)&m_pSlots[0] + 0x18) * cSlots;
    return this;
}

// FUNCTION: LEMBALL 0x004796E0
void NETWORK_EffStreamRecordSlotTable::Destroy() {
    m_pVtable = g_NETWORK_EffRecordSlotTableVtable;
    FreeVSMemBlock(m_pwSlotFlags);
    if (m_pSlots != 0) {
        ((NETWORK_DeleteObject *)m_pSlots)->m_pVtable->m_pDelete(m_pSlots, 3);
    }
    ((NETWORK_EffStreamBase *)this)->DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x00479720
NETWORK_EffStreamRecordSlot *FindNextChangedEffStreamRecordSlot(void *pObject) {
    NETWORK_EffStreamRecordSlotTable *pTable;
    NETWORK_EffStreamRecordSlot *pSlot;
    int nSlotIndex;

    pTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    nSlotIndex = pTable->m_nCurrentChangedSlot38;
    if (nSlotIndex < pTable->m_cSlots) {
        pSlot = pTable->m_pSlots + nSlotIndex;
        while (pSlot->m_wCommittedMarker <= pTable->m_pwSlotFlags[nSlotIndex]) {
            ++nSlotIndex;
            ++pSlot;
            if (pTable->m_cSlots <= nSlotIndex) {
                break;
            }
        }
    }

    if (nSlotIndex == pTable->m_cSlots) {
        pTable->m_nCurrentChangedSlot38 = -1;
        return 0;
    }

    pTable->m_nCurrentChangedSlot38 = nSlotIndex;
    pTable->m_pwSlotFlags[nSlotIndex] = pTable->m_pSlots[nSlotIndex].m_wCommittedMarker;
    return &pTable->m_pSlots[nSlotIndex];
}

// FUNCTION: LEMBALL 0x00479790
void SaveEffStreamRecordSlotTable(void *pObject) {
    NETWORK_EffStreamRecordSlotTable *pTable;
    NETWORK_EffStreamRecordSlot *pSlot;
    int i;

    pTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    i = 0;
    if (pTable->m_cSlots <= 0) {
        return;
    }

    pSlot = pTable->m_pSlots;
    do {
        SaveEffStreamToMemoryRange(pSlot, pTable->m_nSaveRangeCursor1c, 0);
        pTable->m_nSaveRangeCursor1c += *(int *)((char *)pSlot + 0x1c) - *(int *)((char *)pSlot + 8);
        ++i;
        pSlot = (NETWORK_EffStreamRecordSlot *)((char *)pSlot + 0x60);
    } while (i < pTable->m_cSlots);
}

// FUNCTION: LEMBALL 0x004797D0
void LoadEffStreamRecordSlotTable(void *pObject) {
    NETWORK_EffStreamRecordSlotTable *pTable;
    NETWORK_EffStreamRecordSlot *pSlot;
    int nSource;
    int i;

    pTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    i = 0;
    if (pTable->m_cSlots <= 0) {
        pTable->m_nCurrentChangedSlot38 = 0;
        return;
    }

    pSlot = pTable->m_pSlots;
    do {
        nSource = pTable->m_nLoadSourceCursor20;
        LoadEffStreamFromMemory(pSlot, nSource);
        if (*(int *)((char *)pSlot + 0x20) != 0) {
            pTable->m_nLoadSourceCursor20 = *(int *)((char *)pSlot + 0x20);
        }
        ++i;
        pSlot = (NETWORK_EffStreamRecordSlot *)((char *)pSlot + 0x60);
    } while (i < pTable->m_cSlots);
    pTable->m_nCurrentChangedSlot38 = 0;
}

// FUNCTION: LEMBALL 0x00479810
NETWORK_Eff512ByteStateStream *NETWORK_Eff512ByteStateStream::Construct() {
    int i;
    unsigned char *pbPayload;

    m_pVtable = g_NETWORK_ReturnTrueVtable;
    i = 0;
    m_nReserved04 = 0;
    ResetEffStreamStateFields(this);
    m_pVtable = g_NETWORK_EffStateStreamVtable;
    pbPayload = (unsigned char *)AllocateVSMemBlock(0x200);
    m_pbStateBytes2c = pbPayload;
    m_nSerializedLength18 += 0x200;
    do {
        ++i;
        pbPayload[i - 1] = 0;
    } while (i < 0x200);
    return this;
}

// FUNCTION: LEMBALL 0x0047B7D0
void *DeleteEffStreamRecordSlotTable(void *pObject, BYTE fDeleteFlags) {
    NETWORK_EffStreamRecordSlotTable *pTable;

    pTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    pTable->Destroy();
    if ((fDeleteFlags & 1) != 0) {
        FreeVSMemBlock(pTable);
    }
    return pTable;
}

// FUNCTION: LEMBALL 0x0047B7F0
void *DeleteEffStreamRecordSlot(void *pObject, BYTE fDeleteFlags) {
    NETWORK_EffStreamRecordSlot *pSlot;
    NETWORK_EffStreamRecordSlot *pCurrent;
    int cSlots;

    pSlot = (NETWORK_EffStreamRecordSlot *)pObject;
    if ((fDeleteFlags & 2) != 0) {
        cSlots = *((int *)pSlot - 1);
        pCurrent = pSlot + cSlots;
        while (--cSlots >= 0) {
            --pCurrent;
    ((NETWORK_EffStreamBase *)pCurrent)->DestroyEffStreamBase();
        }
        FreeVSMemBlock((int *)pSlot - 1);
        return pObject;
    }

    ((NETWORK_EffStreamBase *)pSlot)->DestroyEffStreamBase();
    if ((fDeleteFlags & 1) != 0) {
        FreeVSMemBlock(pSlot);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0047B870
void WriteEff512ByteStatePayload(void *pObject) {
    NETWORK_Eff512ByteStateStream *pStateStream;

    pStateStream = (NETWORK_Eff512ByteStateStream *)pObject;
    WriteEffStreamBytes(pStateStream, pStateStream->m_pbStateBytes2c, 0x200);
}

// FUNCTION: LEMBALL 0x0047B880
void LoadEff512ByteStatePayload(void *pObject) {
    NETWORK_Eff512ByteStateStream *pStateStream;

    pStateStream = (NETWORK_Eff512ByteStateStream *)pObject;
    ReadEffStreamBytes(pStateStream, pStateStream->m_pbStateBytes2c, 0x200);
}

// FUNCTION: LEMBALL 0x0047B890
void *DeleteEff512ByteStateStream(void *pObject, BYTE fDeleteFlags) {
    NETWORK_Eff512ByteStateStream *pStateStream;

    pStateStream = (NETWORK_Eff512ByteStateStream *)pObject;
    pStateStream->m_pVtable = g_NETWORK_EffStateStreamVtable;
    FreeVSMemBlock((void *)(unsigned long)pStateStream->m_pbStateBytes2c);
    ((NETWORK_EffStreamBase *)pStateStream)->DestroyEffStreamBase();
    if ((fDeleteFlags & 1) != 0) {
        FreeVSMemBlock(pStateStream);
    }
    return pStateStream;
}

// FUNCTION: LEMBALL 0x0047B8D0
void ClearFileBackedEffChannelAsyncErrorStatus(void *pObject) {
    ClearNestedEffChannelAsyncStatus(pObject);
}

// FUNCTION: LEMBALL 0x0047B8F0
void ClearNestedEffChannelAsyncStatus(void *pObject) {
    NETWORK_ConstructionAdjustorVtable *pOffsets;
    char *pbChannelState;

    pOffsets = *(NETWORK_ConstructionAdjustorVtable **)((char *)pObject - 0x1c);
    pbChannelState = (char *)pObject + pOffsets->m_nSecondaryOffset - 0x1c;
    SetEffStreamChannelAsyncErrorStatus(pbChannelState, 0);
}

// FUNCTION: LEMBALL 0x0047BA40
void AdjustAndClearEmbeddedFileBackedEffChannelAsyncErrorStatus(void *pObject) {
    ClearNestedEffChannelAsyncStatus((char *)pObject + 0x138);
}

// FUNCTION: LEMBALL 0x0047BCA0
BYTE AdjustAndMapGlobalStateFileBackedEffCallbackFailure(void *pObject) {
    return MapNestedEffCallbackFailureToStatus6((char *)pObject + 0xd8);
}

// FUNCTION: LEMBALL 0x0047B8E0
void *GetWin32FileWrapperOwnedPath(void *pObject) {
    return ((NETWORK_Win32FileWrapperPathOwner *)pObject)->m_pszOwnedPath04;
}

// FUNCTION: LEMBALL 0x00479930
void *ConstructDualFileBackedEffChannel(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pFatalThunk;
    NETWORK_DualFileBackedEffChannelFront *pFront;
    NETWORK_DualFileBackedEffChannelConstructorView *pConstructed;
    NETWORK_FileBackedChannelViewOffsets *pChannelOffsets;
    NETWORK_FileWrapperThunkOffsets *pWrapperThunkOffsets;
    char *pbObjectBase;

    pFront = (NETWORK_DualFileBackedEffChannelFront *)pObject;
    pConstructed = (NETWORK_DualFileBackedEffChannelConstructorView *)pObject;
    pbObjectBase = (char *)pObject;
    if (fConstructEmbeddedObjects != 0) {
        pFront->m_pThunk0c = (NETWORK_FileBackedChannelViewOffsets *)&g_NETWORK_FileBackedDualChannelStateOffsets;
        pFront->m_pDualStreamOffsets88 = &g_NETWORK_FileBackedDualStreamOffsets;
        pFront->m_pFileThunkb4 = (NETWORK_FileWrapperThunkOffsets *)&g_NETWORK_FileBackedDualFileWrapperThunkOffsets;
        ConstructEffStreamChannelState(pConstructed->m_abChannelState18);
        ConstructDualHandleEffStream(pConstructed->m_abDualHandleStream44, 0);
        pConstructed->m_FileWrapper9c.m_pVtable = g_NETWORK_CrtFatalRuntimeErrorThunk;
        pConstructed->m_FileWrapper9c.m_nReserved04 = 0;
        pConstructed->m_FileWrapper9c.m_fOpen08 = 1;
        pConstructed->m_FileWrapper9c.m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pConstructed->m_FileWrapper9c.m_pDeleteThunk14 = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        pWrapperThunkOffsets = pFront->m_pFileThunkb4;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0xb4 + pWrapperThunkOffsets->m_nOpenThunkViewOffset04);
        pSecondaryThunk =
            (NETWORK_AdjustorSubobject *)(pbObjectBase + 0xb4 + pWrapperThunkOffsets->m_nNestedCallbackThunkOffset08);
        pPrimaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunk;
    }

    ClearFileBackedEffChannelStateWord(pObject);
    pChannelOffsets = pFront->m_pThunk0c;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nChannelStateViewOffset04 - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nStreamViewOffset08 - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nFileWrapperViewOffset0c - 4);
    pFatalThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nFatalThunkViewOffset10 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_FileBackedDualPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_FileBackedDualSecondaryThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_FileBackedFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pChannelOffsets->m_nChannelStateViewOffset04 - 0xc;
    pFront->m_nMode10 = -1;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479E20
void *ConstructTimedFileBackedEffChannel(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pFatalThunk;
    NETWORK_TimedFileBackedEffChannelFront *pFront;
    NETWORK_TimedFileBackedEffChannelConstructorView *pConstructed;
    NETWORK_FileBackedChannelViewOffsets *pChannelOffsets;
    NETWORK_FileWrapperThunkOffsets *pWrapperThunkOffsets;
    char *pbObjectBase;

    pFront = (NETWORK_TimedFileBackedEffChannelFront *)pObject;
    pConstructed = (NETWORK_TimedFileBackedEffChannelConstructorView *)pObject;
    pbObjectBase = (char *)pObject;
    if (fConstructEmbeddedObjects != 0) {
        pFront->m_pThunk0c = (NETWORK_FileBackedChannelViewOffsets *)&g_NETWORK_FileBackedTimedChannelStateOffsets;
        pFront->m_pTimedStreamOffsets8c = &g_NETWORK_FileBackedTimedStreamOffsets;
        pFront->m_pFileThunkd4 = (NETWORK_FileWrapperThunkOffsets *)&g_NETWORK_FileBackedTimedFileWrapperThunkOffsets;
        ConstructEffStreamChannelState(pConstructed->m_abChannelState18);
        ConstructTimedEffStream(&pConstructed->m_TimedStream48, 0);
        pConstructed->m_FileWrapperbc.m_pVtable = g_NETWORK_CrtFatalRuntimeErrorThunk;
        pConstructed->m_FileWrapperbc.m_nReserved04 = 0;
        pConstructed->m_FileWrapperbc.m_fOpen08 = 1;
        pConstructed->m_FileWrapperbc.m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pConstructed->m_FileWrapperbc.m_pDeleteThunk14 = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        pWrapperThunkOffsets = pFront->m_pFileThunkd4;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0xd4 + pWrapperThunkOffsets->m_nOpenThunkViewOffset04);
        pSecondaryThunk =
            (NETWORK_AdjustorSubobject *)(pbObjectBase + 0xd4 + pWrapperThunkOffsets->m_nNestedCallbackThunkOffset08);
        pPrimaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunk;
    }

    ClearFileBackedEffChannelStateWord(pObject);
    pChannelOffsets = pFront->m_pThunk0c;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nChannelStateViewOffset04 - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nStreamViewOffset08 - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nFileWrapperViewOffset0c - 4);
    pFatalThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nFatalThunkViewOffset10 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_FileBackedTimedPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_FileBackedTimedSecondaryThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_FileBackedFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pChannelOffsets->m_nChannelStateViewOffset04 - 0xc;
    pSecondaryThunk->m_nThisDelta = pChannelOffsets->m_nStreamViewOffset08 - 0x3c;
    pFront->m_nMode10 = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479F40
void RestoreTimedFileBackedEffChannelVtables(int nTimedFileBasePlus0x0c) {
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pFatalThunk;
    NETWORK_TimedFileBackedEffChannelFront *pFront;
    NETWORK_FileBackedChannelViewOffsets *pChannelOffsets;
    char *pbChannelBase;

    pbChannelBase = (char *)(unsigned long)(nTimedFileBasePlus0x0c - 0xc);
    pFront = (NETWORK_TimedFileBackedEffChannelFront *)pbChannelBase;
    pChannelOffsets = pFront->m_pThunk0c;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbChannelBase + pChannelOffsets->m_nChannelStateViewOffset04 - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbChannelBase + pChannelOffsets->m_nStreamViewOffset08 - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbChannelBase + pChannelOffsets->m_nFileWrapperViewOffset0c - 4);
    pFatalThunk = (NETWORK_AdjustorSubobject *)(pbChannelBase + pChannelOffsets->m_nFatalThunkViewOffset10 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_FileBackedTimedPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_FileBackedTimedSecondaryThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_FileBackedFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pChannelOffsets->m_nChannelStateViewOffset04 - 0xc;
    pSecondaryThunk->m_nThisDelta = pChannelOffsets->m_nStreamViewOffset08 - 0x3c;
}

// FUNCTION: LEMBALL 0x0047A220
void *ConstructEmbeddedFileBackedEffChannelStack(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;
    NETWORK_AdjustorSubobject *pFatalThunk;
    const NETWORK_EmbeddedStackViewOffsets *pEmbeddedOffsets;
    const NETWORK_EmbeddedStackFileViewOffsets *pFileViewOffsets;
    NETWORK_EmbeddedFileBackedEffChannelStackConstructorView *pConstructed;
    NETWORK_EmbeddedFileBackedStackFront *pStackFront;
    NETWORK_EmbeddedFileBackedStackOpenWrapperFront *pFileWrapperFront;
    char *pbObjectBase;

    pbObjectBase = (char *)pObject;
    pConstructed = (NETWORK_EmbeddedFileBackedEffChannelStackConstructorView *)pObject;
    pStackFront = (NETWORK_EmbeddedFileBackedStackFront *)pObject;
    pFileWrapperFront = (NETWORK_EmbeddedFileBackedStackOpenWrapperFront *)pObject;
    if (fConstructEmbeddedObjects != 0) {
        pStackFront->m_pTimedStreamOffsets7c = &g_NETWORK_EmbeddedFileBackedTimedStreamOffsets;
        pStackFront->m_pDualStreamOffsetsf4 = &g_NETWORK_EmbeddedFileBackedDualStreamOffsets;
        pStackFront->m_pDualThunkOffsets108 = (NETWORK_EmbeddedStackViewOffsets *)&g_NETWORK_EmbeddedFileBackedDualThunkOffsets;
        *(NETWORK_EmbeddedStackFileViewOffsets **)(pbObjectBase + 0x124) =
            (NETWORK_EmbeddedStackFileViewOffsets *)&g_NETWORK_EmbeddedFileBackedFileWrapperThunkOffsets;
        pStackFront->m_pFinalThunkOffsets = (NETWORK_EmbeddedStackViewOffsets *)&g_NETWORK_EmbeddedFileBackedFinalThunkOffsets;
        pStackFront->m_pPrimaryThunkOffsets138 = &g_NETWORK_EmbeddedFileBackedPrimaryThunkOffsets;
        pStackFront->m_pTimedFileViewOffsets14c = &g_NETWORK_EmbeddedFileBackedTimedFileViewOffsets;

        ConstructEffStreamChannelState(pConstructed->m_abChannelState08);
        ConstructTimedEffStream(&pConstructed->m_TimedStream38, 0);
        ConstructDualHandleEffStream(pConstructed->m_abDualHandleStreamB0, 0);

        pEmbeddedOffsets = (NETWORK_EmbeddedStackViewOffsets *)pStackFront->m_pDualThunkOffsets108;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x108 + pEmbeddedOffsets->m_nChannelStateViewOffset04 - 4);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x108 + pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 4);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x108 + pEmbeddedOffsets->m_nDualStreamViewOffset0c - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunkVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pPrimaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nChannelStateViewOffset04 - 8;
        pSecondaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 0x38;
        pTertiaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nDualStreamViewOffset0c - 0xb0;

        pConstructed->m_FileWrapper10c.m_pVtable = g_NETWORK_CrtFatalRuntimeErrorThunk;
        pConstructed->m_FileWrapper10c.m_nReserved04 = 0;
        pConstructed->m_FileWrapper10c.m_fOpen08 = 1;
        pConstructed->m_FileWrapper10c.m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pConstructed->m_FileWrapper10c.m_pDeleteThunk14 = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        pFileViewOffsets = (NETWORK_EmbeddedStackFileViewOffsets *)pFileWrapperFront->m_pFileWrapperThunk124;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x124 + pFileViewOffsets->m_nFileWrapperViewOffset04);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x124 + pFileViewOffsets->m_nChannelStateViewOffset08);
        pPrimaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunk;

        ConstructDualFileBackedEffChannel(pConstructed->m_abDualFileChannel12c, 0);
        ConstructTimedFileBackedEffChannel(pConstructed->m_abTimedFileChannel1d0, 0);
    }

    pEmbeddedOffsets = (NETWORK_EmbeddedStackViewOffsets *)pStackFront->m_pFinalThunkOffsets;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pEmbeddedOffsets->m_nChannelStateViewOffset04 - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pEmbeddedOffsets->m_nDualStreamViewOffset0c - 4);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pEmbeddedOffsets->m_nFinalFileThunkViewOffset10 - 4);
    pFatalThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + pEmbeddedOffsets->m_nFinalFatalThunkViewOffset14 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_FileBackedFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nChannelStateViewOffset04 - 8;
    pSecondaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 0x38;
    pTertiaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nDualStreamViewOffset0c - 0xb0;
    return pObject;
}

// FUNCTION: LEMBALL 0x0047A570
void *ConstructGlobalStateFileBackedEffComposite(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;
    NETWORK_AdjustorSubobject *pFatalThunk;
    NETWORK_CompositeViewOffsets *pCompositeOffsets;
    const NETWORK_EmbeddedStackViewOffsets *pEmbeddedOffsets;
    const NETWORK_EmbeddedStackFileViewOffsets *pFileViewOffsets;
    NETWORK_EffTransportRuntimeService *pRuntimeService;
    NETWORK_GlobalStateCompositeLayout *pFront;
    NETWORK_GlobalStateCompositeOpenWrapperFront *pOpenWrapperFront;
    NETWORK_EmbeddedFileBackedStackFront *pEmbeddedStackFront;
    NETWORK_FileBackedCompositeInitialStream *pInitialStream;
    NETWORK_Eff512ByteStateStream *pStateStream;
    NETWORK_EffStreamRecordSlotTable *pRecordSlotTable;
    char *pbObjectBase;
    char *pbEmbeddedStackBase;
    void *pStorage;

    pbObjectBase = (char *)pObject;
    pFront = (NETWORK_GlobalStateCompositeLayout *)pObject;
    pCompositeOffsets = pFront->m_pTimedStreamOffsets20;
    pOpenWrapperFront = (NETWORK_GlobalStateCompositeOpenWrapperFront *)pObject;
    pEmbeddedStackFront = (NETWORK_EmbeddedFileBackedStackFront *)(pbObjectBase + 0x1b0);
    if (fConstructEmbeddedObjects != 0) {
        pFront->m_pTimedStreamOffsets20 = (NETWORK_CompositeViewOffsets *)&g_NETWORK_GlobalStateCompositeTimedStreamOffsets;
        pFront->m_pTransportOffsets00 = (NETWORK_CompositeViewOffsets *)&g_NETWORK_GlobalStateCompositeTransportOffsets;
        *(void **)(pbObjectBase + 0xd4) = (void *)&g_NETWORK_GlobalStateCompositeDualStreamOffsets;
        pFront->m_pDualThunkOffsets14c = (NETWORK_CompositeViewOffsets *)&g_NETWORK_GlobalStateCompositeDualThunkOffsets;
        pFront->m_pEmbeddedStackTransportOffsets160 = (NETWORK_EmbeddedStackViewOffsets *)&g_NETWORK_GlobalStateCompositeFileWrapperOffsets;
        pFront->m_pEmbeddedStackFileViewOffsets180 = &g_NETWORK_GlobalStateCompositeTimedFileThunkOffsets;
        pFront->m_pEmbeddedStackTimedStreamOffsets194 = (NETWORK_EmbeddedStackViewOffsets *)&g_NETWORK_GlobalStateCompositeEmbeddedStackOffsets;
        pFront->m_pEmbeddedStackDualStreamOffsets1a8 = (NETWORK_EmbeddedStackFileViewOffsets *)&g_NETWORK_GlobalStateCompositeEmbeddedStackDataOffsets;
        pEmbeddedStackFront->m_pFinalThunkOffsets = &g_NETWORK_GlobalStateCompositeEmbeddedStackFinalOffsets;

        ConstructEffStreamChannelState(pFront->m_abChannelState60);
        ConstructTimedEffStream(pFront->m_abTimedStream90, 0);
        ConstructDualHandleEffStream(pFront->m_abDualStream108, 0);

        pbEmbeddedStackBase = pbObjectBase + 0x160;
        pEmbeddedOffsets = *(NETWORK_EmbeddedStackViewOffsets **)pbEmbeddedStackBase;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nChannelStateViewOffset04 - 4);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 4);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nDualStreamViewOffset0c - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunkVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pPrimaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nChannelStateViewOffset04 - 8;
        pSecondaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 0x38;
        pTertiaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nDualStreamViewOffset0c - 0xb0;

        pOpenWrapperFront->m_FileWrapper164.m_nReserved04 = 0;
        pOpenWrapperFront->m_FileWrapper164.m_pVtable = g_NETWORK_CrtFatalRuntimeErrorThunk;
        pOpenWrapperFront->m_FileWrapper164.m_fOpen08 = 1;
        pOpenWrapperFront->m_FileWrapper164.m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pOpenWrapperFront->m_FileWrapper164.m_pDeleteThunk14 = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        pFileViewOffsets = pOpenWrapperFront->m_pFileWrapperThunk180;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x180 + pFileViewOffsets->m_nFileWrapperViewOffset04);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x180 + pFileViewOffsets->m_nChannelStateViewOffset08);
        pPrimaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunk;

        ConstructDualFileBackedEffChannel(pbObjectBase + 0x188, 0);
        ConstructTimedFileBackedEffChannel(pbObjectBase + 0x19c, 0);
        ConstructEmbeddedFileBackedEffChannelStack(pbObjectBase + 0x1b0, 0);
    }

    pInitialStream = &pFront->m_InitialStream24;
    pInitialStream->m_nReserved04 = 0;
    pInitialStream->m_pVtable = g_NETWORK_ReturnTrueVtable;
    ResetEffStreamStateFields(pInitialStream);
    pInitialStream->m_nSerializedLength18 += 2;
    pInitialStream->m_wFlags2c = 0;
    pInitialStream->m_pVtable = g_NETWORK_GlobalStateCompositeInitialVtable;

    pCompositeOffsets = pFront->m_pTimedStreamOffsets20;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nTransportViewOffset04 - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nTimedStreamViewOffset08 - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nDualStreamViewOffset0c - 4);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nDualThunkViewOffset10 - 4);
    pFatalThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nMarkerViewOffset14 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_FileBackedFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = 0;
    pSecondaryThunk->m_nThisDelta = 0;
    pTertiaryThunk->m_nThisDelta = 0;

    ConstructEffTransportRuntimeChannelStack(pObject, 0);

    *(void **)pObject = g_NETWORK_GlobalStateCompositeFinalVtable;
    pPrimaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pCompositeOffsets->m_nTransportViewOffset04 - 0x40;
    pSecondaryThunk->m_nThisDelta = pCompositeOffsets->m_nTimedStreamViewOffset08 - 0x70;
    pTertiaryThunk->m_nThisDelta = pCompositeOffsets->m_nDualStreamViewOffset0c - 0xe8;
    pFatalThunk->m_nThisDelta = pCompositeOffsets->m_nMarkerViewOffset14 - 0x15c;

    pRuntimeService = (NETWORK_EffTransportRuntimeService *)g_pEffTransportRuntimeService;
    pRuntimeService->m_pVtable->m_pSelectLocalHostName(pRuntimeService, g_pszFileBasedNetworkLocalHostName);

    if (g_pGlobalStateEff512ByteStream == 0) {
        pStorage = AllocateVSMemBlock(0x30);
        if (pStorage == 0) {
            g_pGlobalStateEff512ByteStream = 0;
        } else {
            g_pGlobalStateEff512ByteStream = ((NETWORK_Eff512ByteStateStream *)pStorage)->Construct();
        }
    }
    pStateStream = (NETWORK_Eff512ByteStateStream *)g_pGlobalStateEff512ByteStream;

    *(int *)(pbObjectBase + pCompositeOffsets->m_nMarkerViewOffset14 + 8) = 0x14;
    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableBOffset1c) = 0;
    } else {
        pRecordSlotTable =
            ((NETWORK_EffStreamRecordSlotTable *)pStorage)
                ->Construct(*(int *)(pbObjectBase + pCompositeOffsets->m_nMarkerViewOffset14 + 8));
        *(NETWORK_EffStreamRecordSlotTable **)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableBOffset1c) =
            pRecordSlotTable;
    }

    *(NETWORK_EffStreamRecordSlotTable **)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableAOffset18) =
        *(NETWORK_EffStreamRecordSlotTable **)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableBOffset1c);
    *(int *)((char *)&pFront->m_InitialStream24 + pCompositeOffsets->m_nRecordTableBOffset1c) =
        pStateStream->m_nSerializedLength18 + pInitialStream->m_nSerializedLength18;
    *(int *)((char *)&pFront->m_InitialStream24 + pCompositeOffsets->m_nRecordTableAOffset18) =
        *(int *)((char *)&pFront->m_InitialStream24 + pCompositeOffsets->m_nRecordTableBOffset1c);
    pFront->m_nReserved54 = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x0047A910
void DestroyGlobalStateFileBackedEffComposite(int nObjectBasePlus0x60) {
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;
    NETWORK_AdjustorSubobject *pFatalThunk;
    NETWORK_CompositeViewOffsets *pCompositeOffsets;
    NETWORK_GlobalStateCompositeLayout *pFront;
    char *pbObjectBase;
    NETWORK_DeleteObject *pRecordSlotTable;
    NETWORK_DeleteObject *pGlobalStateStream;
    char *pszConfiguredPath;

    pbObjectBase = (char *)(unsigned long)(nObjectBasePlus0x60 - 0x60);
    pFront = (NETWORK_GlobalStateCompositeLayout *)pbObjectBase;
    pCompositeOffsets = pFront->m_pTimedStreamOffsets20;
    *(void **)pbObjectBase = g_NETWORK_GlobalStateCompositeFinalVtable;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nTransportViewOffset04 - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nTimedStreamViewOffset08 - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nDualStreamViewOffset0c - 4);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nDualThunkViewOffset10 - 4);
    pFatalThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nMarkerViewOffset14 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pCompositeOffsets->m_nTransportViewOffset04 - 0x40;
    pSecondaryThunk->m_nThisDelta = pCompositeOffsets->m_nTimedStreamViewOffset08 - 0x70;
    pTertiaryThunk->m_nThisDelta = pCompositeOffsets->m_nDualStreamViewOffset0c - 0xe8;
    pFatalThunk->m_nThisDelta = pCompositeOffsets->m_nMarkerViewOffset14 - 0x15c;

    pRecordSlotTable = *(NETWORK_DeleteObject **)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableAOffset18);
    if (pRecordSlotTable != 0) {
        pRecordSlotTable->m_pVtable->m_pDelete(pRecordSlotTable, 1);
    }

    pszConfiguredPath = g_pszFileBasedNetworkConfiguredPath;
    if (pszConfiguredPath != 0) {
        FreeVSMemBlock(pszConfiguredPath);
        g_pszFileBasedNetworkConfiguredPath = 0;
    }

    pGlobalStateStream = (NETWORK_DeleteObject *)g_pGlobalStateEff512ByteStream;
    if (pGlobalStateStream != 0) {
        pGlobalStateStream->m_pVtable->m_pDelete(pGlobalStateStream, 1);
        g_pGlobalStateEff512ByteStream = 0;
    }

    ReleaseEffTransportRuntimeBuffers(nObjectBasePlus0x60 - 0x3c);
    ((NETWORK_EffStreamBase *)&pFront->m_InitialStream24)->DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x0047BB60
void *DeleteGlobalStateFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_GlobalStateFileBackedEffCompositeWrapper *pWrapper;
    NETWORK_FileBackedCompositeWrapperView *pView;
    char *pbAllocationBase;

    pWrapper = (NETWORK_GlobalStateFileBackedEffCompositeWrapper *)((char *)pObject - 0x60);
    pView = &pWrapper->m_Runtime60;
    pbAllocationBase = (char *)pWrapper;
    DestroyGlobalStateFileBackedEffComposite((int)(unsigned long)pObject);
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x154);
    pView->m_pResetThunk104 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBuffer108 != 0) {
        FreeVSMemBlock(pView->m_pSideBuffer108);
    }
    DestroyDualHandleEffStream(pView->m_abDualStreamA8);
    DestroyTimedEffStream(&pView->m_TimedStream30);
    DestroyEffStreamChannelState(&pView->m_abChannelState00);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x0047BC50
void *AdjustAndDeleteGlobalStateDualFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteGlobalStateFileBackedEffCompositeWrapper(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0xa8,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047AF30
void *ConstructDualSlotTableFileBackedEffComposite(void *pObject, int fConstructEmbeddedObjects) {
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;
    NETWORK_AdjustorSubobject *pFatalThunk;
    NETWORK_CompositeViewOffsets *pCompositeOffsets;
    const NETWORK_EmbeddedStackViewOffsets *pEmbeddedOffsets;
    const NETWORK_EmbeddedStackFileViewOffsets *pFileViewOffsets;
    NETWORK_DualSlotCompositeLayout *pFront;
    NETWORK_DualSlotCompositeOpenWrapperFront *pOpenWrapperFront;
    NETWORK_EmbeddedFileBackedStackFront *pEmbeddedStackFront;
    NETWORK_FileBackedCompositeInitialStream *pInitialStream;
    NETWORK_EffStreamRecordSlotTable *pRecordSlotTable;
    char *pbObjectBase;
    char *pbEmbeddedStackBase;
    void *pStorage;

    pbObjectBase = (char *)pObject;
    pFront = (NETWORK_DualSlotCompositeLayout *)pObject;
    pCompositeOffsets = pFront->m_pTimedStreamOffsets2c;
    pOpenWrapperFront = (NETWORK_DualSlotCompositeOpenWrapperFront *)pObject;
    pEmbeddedStackFront = (NETWORK_EmbeddedFileBackedStackFront *)(pbObjectBase + 0x1b4);

    if (fConstructEmbeddedObjects != 0) {
        pFront->m_pTimedStreamOffsets2c = (NETWORK_CompositeViewOffsets *)&g_NETWORK_DualSlotCompositeTimedStreamOffsets;
        pFront->m_pTransportOffsets00 = (NETWORK_CompositeViewOffsets *)&g_NETWORK_DualSlotCompositeTransportOffsets;
        *(void **)(pbObjectBase + 0xd8) = (void *)&g_NETWORK_DualSlotCompositeDualStreamOffsets;
        pFront->m_pDualThunkOffsets150 = (NETWORK_CompositeViewOffsets *)&g_NETWORK_DualSlotCompositeDualThunkOffsets;
        pFront->m_pEmbeddedStackTransportOffsets164 = (NETWORK_EmbeddedStackViewOffsets *)&g_NETWORK_DualSlotCompositeFileWrapperOffsets;
        pFront->m_pEmbeddedStackFileViewOffsets184 = &g_NETWORK_DualSlotCompositeTimedFileThunkOffsets;
        pFront->m_pEmbeddedStackTimedStreamOffsets198 = (NETWORK_EmbeddedStackViewOffsets *)&g_NETWORK_DualSlotCompositeEmbeddedStackOffsets;
        pFront->m_pEmbeddedStackDualStreamOffsets1ac = (NETWORK_EmbeddedStackFileViewOffsets *)&g_NETWORK_DualSlotCompositeEmbeddedStackDataOffsets;
        pEmbeddedStackFront->m_pFinalThunkOffsets = &g_NETWORK_DualSlotCompositeEmbeddedStackFinalOffsets;

        ConstructEffStreamChannelState(pFront->m_abChannelState64);
        ConstructTimedEffStream(pFront->m_abTimedStream94, 0);
        ConstructDualHandleEffStream(pFront->m_abDualStream10c, 0);

        pbEmbeddedStackBase = pbObjectBase + 0x164;
        pEmbeddedOffsets = *(NETWORK_EmbeddedStackViewOffsets **)pbEmbeddedStackBase;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nChannelStateViewOffset04 - 4);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 4);
        pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nDualStreamViewOffset0c - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunkVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pTertiaryThunk->m_pVtable = g_NETWORK_ReturnTrueVtable;
        pPrimaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nChannelStateViewOffset04 - 8;
        pSecondaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 0x38;
        pTertiaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nDualStreamViewOffset0c - 0xb0;

        pOpenWrapperFront->m_FileWrapper168.m_nReserved04 = 0;
        pOpenWrapperFront->m_FileWrapper168.m_pVtable = g_NETWORK_CrtFatalRuntimeErrorThunk;
        pOpenWrapperFront->m_FileWrapper168.m_fOpen08 = 1;
        pOpenWrapperFront->m_FileWrapper168.m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pOpenWrapperFront->m_FileWrapper168.m_pDeleteThunk14 = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        pFileViewOffsets = pOpenWrapperFront->m_pFileWrapperThunk184;
        pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x184 + pFileViewOffsets->m_nFileWrapperViewOffset04);
        pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x184 + pFileViewOffsets->m_nChannelStateViewOffset08);
        pPrimaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunk;

        ConstructDualFileBackedEffChannel(pbObjectBase + 0x18c, 0);
        ConstructTimedFileBackedEffChannel(pbObjectBase + 0x1a0, 0);
        ConstructEmbeddedFileBackedEffChannelStack(pbObjectBase + 0x1b4, 0);
    }

    pInitialStream = &pFront->m_InitialStream30;
    pInitialStream->m_nReserved04 = 0;
    pInitialStream->m_pVtable = g_NETWORK_ReturnTrueVtable;
    ResetEffStreamStateFields(pInitialStream);
    pInitialStream->m_nSerializedLength18 += 2;
    pInitialStream->m_wFlags2c = 0;
    pInitialStream->m_pVtable = g_NETWORK_GlobalStateCompositeInitialVtable;

    pCompositeOffsets = pFront->m_pTimedStreamOffsets2c;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nTransportViewOffset04 - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nTimedStreamViewOffset08 - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nDualStreamViewOffset0c - 4);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nDualThunkViewOffset10 - 4);
    pFatalThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nMarkerViewOffset14 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_FileBackedFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = 0;
    pSecondaryThunk->m_nThisDelta = 0;
    pTertiaryThunk->m_nThisDelta = 0;

    ConstructCompositeEffTransportStack(pObject, 0);

    *(void **)pObject = g_NETWORK_DualSlotCompositeFinalVtable;
    pPrimaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pCompositeOffsets->m_nTransportViewOffset04 - 0x38;
    pSecondaryThunk->m_nThisDelta = pCompositeOffsets->m_nTimedStreamViewOffset08 - 0x68;
    pTertiaryThunk->m_nThisDelta = pCompositeOffsets->m_nDualStreamViewOffset0c - 0xe0;
    pFatalThunk->m_nThisDelta = pCompositeOffsets->m_nMarkerViewOffset14 - 0x154;

    *(int *)(pbObjectBase + pCompositeOffsets->m_nMarkerViewOffset14 + 0xc) = 3;

    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nRecordTableAOffset18) = 0;
    } else {
        pRecordSlotTable =
            ((NETWORK_EffStreamRecordSlotTable *)pStorage)
                ->Construct(*(int *)(pbObjectBase + pCompositeOffsets->m_nMarkerViewOffset14 + 0xc));
        *(NETWORK_EffStreamRecordSlotTable **)(pbObjectBase + 0x2c + pCompositeOffsets->m_nRecordTableAOffset18) =
            pRecordSlotTable;
    }

    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nRecordTableBOffset1c) = 0;
    } else {
        pRecordSlotTable =
            ((NETWORK_EffStreamRecordSlotTable *)pStorage)
                ->Construct(*(int *)(pbObjectBase + pCompositeOffsets->m_nMarkerViewOffset14 + 0xc));
        *(NETWORK_EffStreamRecordSlotTable **)(pbObjectBase + 0x2c + pCompositeOffsets->m_nRecordTableBOffset1c) =
            pRecordSlotTable;
    }

    *(int *)((char *)&pFront->m_InitialStream30 + pCompositeOffsets->m_nRecordTableAOffset18) =
        pInitialStream->m_nSerializedLength18;
    *(int *)((char *)&pFront->m_InitialStream30 + pCompositeOffsets->m_nRecordTableBOffset1c) =
        pInitialStream->m_nSerializedLength18;
    return pObject;
}

// FUNCTION: LEMBALL 0x0047B2A0
void DestroyDualSlotTableFileBackedEffComposite(int nObjectBasePlus0x64) {
    NETWORK_AdjustorSubobject *pPrimaryThunk;
    NETWORK_AdjustorSubobject *pSecondaryThunk;
    NETWORK_AdjustorSubobject *pTertiaryThunk;
    NETWORK_AdjustorSubobject *pQuaternaryThunk;
    NETWORK_AdjustorSubobject *pFatalThunk;
    NETWORK_CompositeViewOffsets *pCompositeOffsets;
    NETWORK_DualSlotCompositeLayout *pFront;
    char *pbObjectBase;
    NETWORK_DeleteObject *pRecordTableA;
    NETWORK_DeleteObject *pRecordTableB;

    pbObjectBase = (char *)(unsigned long)(nObjectBasePlus0x64 - 100);
    pFront = (NETWORK_DualSlotCompositeLayout *)pbObjectBase;
    pCompositeOffsets = pFront->m_pTimedStreamOffsets2c;
    *(void **)pbObjectBase = g_NETWORK_DualSlotCompositeFinalVtable;
    pPrimaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nTransportViewOffset04 - 4);
    pSecondaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nTimedStreamViewOffset08 - 4);
    pTertiaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nDualStreamViewOffset0c - 4);
    pQuaternaryThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nDualThunkViewOffset10 - 4);
    pFatalThunk = (NETWORK_AdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nMarkerViewOffset14 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pCompositeOffsets->m_nTransportViewOffset04 - 0x38;
    pSecondaryThunk->m_nThisDelta = pCompositeOffsets->m_nTimedStreamViewOffset08 - 0x68;
    pTertiaryThunk->m_nThisDelta = pCompositeOffsets->m_nDualStreamViewOffset0c - 0xe0;
    pFatalThunk->m_nThisDelta = pCompositeOffsets->m_nMarkerViewOffset14 - 0x154;

    pRecordTableA = *(NETWORK_DeleteObject **)(pbObjectBase + 0x2c + pCompositeOffsets->m_nRecordTableAOffset18);
    if (pRecordTableA != 0) {
        pRecordTableA->m_pVtable->m_pDelete(pRecordTableA, 1);
    }

    pRecordTableB = *(NETWORK_DeleteObject **)(pbObjectBase + 0x2c + pCompositeOffsets->m_nRecordTableBOffset1c);
    if (pRecordTableB != 0) {
        pRecordTableB->m_pVtable->m_pDelete(pRecordTableB, 1);
    }

    RestoreCompositeEffTransportVtables(nObjectBasePlus0x64 - 0x34);
    ((NETWORK_EffStreamBase *)&pFront->m_InitialStream30)->DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x0047B370
void WriteDualSlotTableFileBackedEffHeader(int nObjectBasePlus0x154) {
    NETWORK_FileBackedMarkerOwner *pOwner;
    NETWORK_FileBackedDispatchOffsets *pOffsets;
    NETWORK_FileWrapperObject *pReadThunk;
    NETWORK_FileWrapperObject *pWriteThunk;
    unsigned long ulObjectBase;

    pOwner = (NETWORK_FileBackedMarkerOwner *)(unsigned long)(nObjectBasePlus0x154 - 0x154);
    ulObjectBase = (unsigned long)pOwner;
    pOffsets = (NETWORK_FileBackedDispatchOffsets *)pOwner->m_pOffsets;
    pReadThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    WriteEffStreamToLockedFile(pWriteThunk, (int *)&pOwner->m_Marker04, 0, 0);
}

// FUNCTION: LEMBALL 0x0047C040
void AdjustAndWriteDualSlotTableFileBackedEffHeader(void *pObject) {
    WriteDualSlotTableFileBackedEffHeader(
        (int)(unsigned long)((char *)pObject -
                             ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta));
}

// FUNCTION: LEMBALL 0x0047B9D0
void *DeleteCompositeFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_FileBackedCompositeChannelWrapper *pWrapper;
    NETWORK_FileBackedCompositeWrapperView *pView;
    char *pbAllocationBase;

    pWrapper = (NETWORK_FileBackedCompositeChannelWrapper *)((char *)pObject - 0x38);
    pView = &pWrapper->m_Runtime38;
    pbAllocationBase = (char *)pWrapper;
    ((NETWORK_EffStreamBase *)((char *)pObject - 0x34))->DestroyEffStreamBase();
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x150);
    pView->m_pResetThunk104 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBuffer108 != 0) {
        FreeVSMemBlock(pView->m_pSideBuffer108);
    }
    DestroyDualHandleEffStream(pView->m_abDualStreamA8);
    DestroyTimedEffStream(&pView->m_TimedStream30);
    DestroyEffStreamChannelState(&pView->m_abChannelState00);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x0047B9C0
void *AdjustAndDeleteCompositeFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteCompositeFileBackedEffChannelWrapper(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BCC0
void *DeleteTimedFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_FileBackedTimedChannelWrapperView *pView;
    char *pbAllocationBase;

    pView = (NETWORK_FileBackedTimedChannelWrapperView *)pObject;
    pbAllocationBase = (char *)pObject - 0x18;
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject);
    pView->m_pResetThunkA4 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBufferA8 != 0) {
        FreeVSMemBlock(pView->m_pSideBufferA8);
    }
    DestroyTimedEffStream(&pView->m_TimedStream30);
    DestroyEffStreamChannelState(&pView->m_abChannelState00);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x0047BB50
void *AdjustAndDeleteGlobalStateFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteGlobalStateFileBackedEffCompositeWrapper(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BC00
void *AdjustAndDeleteGlobalStateTimedFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteGlobalStateFileBackedEffCompositeWrapper(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0x30,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BC90
void InitializeGlobalStateFileBackedEffStorageAdjustor(void *pObject) {
    InitializeGlobalStateFileBackedEffStorage((int)(unsigned long)((char *)pObject -
                                                                   ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))
                                                                       ->m_nThisDelta));
}

// FUNCTION: LEMBALL 0x0047BA50
void AdjustAndClearGlobalStateTimedEffTransportPendingWriteState(void *pObject) {
    ClearAdjustedEffTransportPendingWriteState(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta + 0x154);
}

// FUNCTION: LEMBALL 0x0047BA60
void ClearAdjustedEffTransportPendingWriteState(void *pObject) {
    char *pbTransportView;

    pbTransportView = (char *)pObject +
                      (*(NETWORK_GlobalSessionAdjustorOffsets **)((char *)pObject - 8))->m_nTransportViewOffset10;
    ClearEffTransportPendingWriteState(
        (int)(unsigned long)(pbTransportView +
                             (*(NETWORK_GlobalSessionAdjustorOffsets **)(pbTransportView - 8))->m_nEmbeddedStreamViewOffset08 +
                             0x70));
}

// FUNCTION: LEMBALL 0x0047BAA0
void NETWORK_GlobalSessionAdjustorFront::WriteAdjustedEffStreamWithGlobalSession(void) {
    char *pbTransportView;

    pbTransportView = (char *)this +
                      ((NETWORK_GlobalSessionAdjustorFront *)((char *)this - 0x38))->m_pOffsets->m_nTransportViewOffset10;
    ((NETWORK_PeerPayloadSender *)(pbTransportView +
                                   ((NETWORK_GlobalSessionAdjustorFront *)(pbTransportView - 0x38))->m_pOffsets
                                       ->m_nEmbeddedStreamViewOffset08 -
                                   0x38))
        ->WriteEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BAD0
void ClaimAdjustedEffTransportRecordPayload(void *pObject) {
    char *pbTransportView;

    pbTransportView = (char *)pObject +
                      ((NETWORK_GlobalSessionAdjustorFront *)((char *)pObject - 0x38))->m_pOffsets
                          ->m_nTransportViewOffset10;
    ClaimAckedEffTransportRecordPayload(pbTransportView +
                                        ((NETWORK_GlobalSessionAdjustorFront *)(pbTransportView - 0x38))->m_pOffsets
                                            ->m_nEmbeddedStreamViewOffset08 -
                                        0x38);
}

// FUNCTION: LEMBALL 0x0047BA90
void AdjustAndWriteGlobalStateTimedEffStreamWithGlobalSession(void *pObject) {
    ((NETWORK_GlobalSessionAdjustorFront *)((char *)pObject -
                                           ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta +
                                           0x154))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BAC0
void AdjustAndClaimGlobalStateTimedEffTransportRecordPayload(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta + 0x154);
}

// FUNCTION: LEMBALL 0x0047BB20
void AdjustAndWriteGlobalStateDualEffStreamWithGlobalSession(void *pObject) {
    ((NETWORK_GlobalSessionAdjustorFront *)((char *)pObject -
                                           ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta +
                                           0xdc))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BB30
void AdjustAndClaimGlobalStateDualEffTransportRecordPayload(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta + 0xdc);
}

// FUNCTION: LEMBALL 0x0047BC60
void AdjustAndWriteDualSlotTimedEffStreamWithGlobalSession(void *pObject) {
    ((NETWORK_GlobalSessionAdjustorFront *)((char *)pObject -
                                           ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta +
                                           0xe0))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BC70
void AdjustAndClaimDualSlotTimedEffTransportRecordPayload(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta + 0xe0);
}

// FUNCTION: LEMBALL 0x0047BC80
void DispatchAdjustedPrefixedEffTransportControlStream(void *pObject, int nDispatchType, void *pEvent) {
    DispatchPrefixedEffTransportControlStream((char *)pObject - 0x3c, nDispatchType, pEvent);
}

// FUNCTION: LEMBALL 0x0047BE50
void *DeleteEmbeddedFileBackedEffChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_EmbeddedFileBackedEffChannelStackWrapper *pWrapper;
    NETWORK_FileBackedCompositeWrapperView *pView;
    char *pbAllocationBase;

    pWrapper = (NETWORK_EmbeddedFileBackedEffChannelStackWrapper *)((char *)pObject - 8);
    pView = &pWrapper->m_Runtime08;
    pbAllocationBase = (char *)pWrapper;
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x150);
    pView->m_pResetThunk104 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBuffer108 != 0) {
        FreeVSMemBlock(pView->m_pSideBuffer108);
    }
    DestroyDualHandleEffStream(pView->m_abDualStreamA8);
    DestroyTimedEffStream(&pView->m_TimedStream30);
    DestroyEffStreamChannelState(&pView->m_abChannelState00);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x0047BE40
void *AdjustAndDeleteEmbeddedFileBackedEffChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteEmbeddedFileBackedEffChannelStackWrapper(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BEC0
void *AdjustAndDeleteEmbeddedTimedFileBackedEffChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteEmbeddedFileBackedEffChannelStackWrapper(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0x30,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BED0
void AdjustAndWriteEmbeddedTimedEffStreamWithGlobalSession(void *pObject) {
    ((NETWORK_GlobalSessionAdjustorFront *)((char *)pObject -
                                           ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BEE0
void AdjustAndClaimEmbeddedTimedEffTransportRecordPayload(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta);
}

// FUNCTION: LEMBALL 0x0047BF00
void *AdjustAndDeleteEmbeddedDualFileBackedEffChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteEmbeddedFileBackedEffChannelStackWrapper(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0xa8,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BF10
void AdjustAndWriteEmbeddedDualEffStreamWithGlobalSession(void *pObject) {
    ((NETWORK_GlobalSessionAdjustorFront *)((char *)pObject -
                                           ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta -
                                           0x78))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BF20
void AdjustAndClaimEmbeddedDualEffTransportRecordPayload(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0x78);
}

// FUNCTION: LEMBALL 0x0047BF40
void *DeleteDualSlotTableFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    NETWORK_DualSlotTableFileBackedEffCompositeWrapper *pWrapper;
    NETWORK_FileBackedCompositeWrapperView *pView;
    char *pbAllocationBase;

    pWrapper = (NETWORK_DualSlotTableFileBackedEffCompositeWrapper *)((char *)pObject - 100);
    pView = &pWrapper->m_Runtime64;
    pbAllocationBase = (char *)pWrapper;
    DestroyDualSlotTableFileBackedEffComposite((int)(unsigned long)pObject);
    RestoreTimedFileBackedEffChannelVtables((int)(unsigned long)pObject + 0x154);
    pView->m_pResetThunk104 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBuffer108 != 0) {
        FreeVSMemBlock(pView->m_pSideBuffer108);
    }
    DestroyDualHandleEffStream(pView->m_abDualStreamA8);
    DestroyTimedEffStream(&pView->m_TimedStream30);
    DestroyEffStreamChannelState(&pView->m_abChannelState00);
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x0047BFD0
void *AdjustAndDeleteDualSlotTableTimedFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteDualSlotTableFileBackedEffCompositeWrapper(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0x30,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BF30
void *AdjustAndDeleteDualSlotTableFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteDualSlotTableFileBackedEffCompositeWrapper(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047C000
void *AdjustAndDeleteDualSlotTableDualFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteDualSlotTableFileBackedEffCompositeWrapper(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0xa8,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BFF0
void DispatchAdjustedAckedEffTransportPayloadEvent(void *pObject) {
    DispatchAckedEffTransportPayloadEvent((char *)pObject - 0x34);
}

// FUNCTION: LEMBALL 0x0047BFE0
void DispatchAdjustedAckedEffTransportPayloadEventFromEmbeddedThunk(void *pObject) {
    DispatchAdjustedAckedEffTransportPayloadEvent(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta);
}

// FUNCTION: LEMBALL 0x0047C010
void QueueAdjustedEffTransportConnectEvent(void *pObject) {
    QueueEffTransportConnectEvent((char *)pObject - 0x34);
}

// FUNCTION: LEMBALL 0x0047C020
void DispatchAdjustedAckedEffTransportPayloadEventFromDualThunk(void *pObject) {
    DispatchAdjustedAckedEffTransportPayloadEvent(
        (char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0x78);
}

// FUNCTION: LEMBALL 0x0047C030
void QueueAdjustedEffTransportPayloadEvent(void *pObject, unsigned short nType, void *pPayload) {
    QueueEffTransportPayloadEvent((char *)pObject - 0x34, nType, pPayload);
}

// FUNCTION: LEMBALL 0x00479880
int OpenFileBackedEffHandleWithRetry(void *pObject, int nHandle) {
    NETWORK_FileBackedOpenRetryOwner *pOwner;
    NETWORK_FileWrapperOpenObject *pOpenThunk;
    DWORD dwStartTime;
    DWORD dwNow;

    pOwner = (NETWORK_FileBackedOpenRetryOwner *)pObject;
    pOpenThunk = (NETWORK_FileWrapperOpenObject *)((char *)pOwner + pOwner->m_pOffsets->m_nOutputStateOffset04 + 4);

    if (pOpenThunk->m_pVtable->m_pOpenWithCreateFlag(pOpenThunk, (LPCSTR)(unsigned long)nHandle, 1, 0) == 0) {
        if (pOpenThunk->m_pVtable->m_pOpenCreateNew(pOpenThunk, (LPCSTR)(unsigned long)nHandle, 1) == 0) {
            dwStartTime = timeGetTime();
            do {
                dwNow = timeGetTime();
            } while (dwNow - dwStartTime < 100);

            if (pOpenThunk->m_pVtable->m_pOpenWithCreateFlag(pOpenThunk, (LPCSTR)(unsigned long)nHandle, 1, 0) == 0) {
                return 0;
            }
        } else {
            pOwner->m_pVtable->m_pResetOrClose(pOwner);
        }
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00479900
BYTE MapNestedEffCallbackFailureToStatus6(void *pObject) {
    NETWORK_FileBackedCallbackStatusFront *pFront;
    NETWORK_FileBackedAppendOwner *pOwner;
    NETWORK_CallbackThunkObject *pNestedThunk;
    int nResult;

    pFront = (NETWORK_FileBackedCallbackStatusFront *)pObject;
    pOwner = (NETWORK_FileBackedAppendOwner *)((char *)pFront - 0x1c);
    pNestedThunk = (NETWORK_CallbackThunkObject *)((char *)pOwner + pOwner->m_pOffsets->m_nOutputStateOffset04);
    nResult = pNestedThunk->m_pVtable->m_pInvoke(pNestedThunk);
    return (BYTE)(-(nResult == 0) & 6);
}

// FUNCTION: LEMBALL 0x00479A50
int LoadEffStreamFromFileBackedRange(void *pObject, void *pStream, int fKeepLocked, int fAlreadyLocked) {
    NETWORK_FileBackedRangeLoadView LoadView;
    NETWORK_FileBackedDispatchOffsets *pOffsets;
    NETWORK_FileWrapperObject *pFileWrapper;
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    NETWORK_EffStreamSerializedLengthView *pSerializedView;
    char *pbRecordTableBase;
    DWORD dwOffset;
    DWORD cbRead;
    DWORD dwStartTime;
    DWORD dwNow;

    pRecordTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    pbRecordTableBase = (char *)pObject;
    pOffsets = pRecordTable->m_pChannelThunk0c;
    LoadView.m_pRecordTable = pRecordTable;
    LoadView.m_pFileWrapper =
        (NETWORK_FileWrapperObject *)(pbRecordTableBase + 0xc + pOffsets->m_nFileWrapperViewOffset0c);
    pSerializedView = (NETWORK_EffStreamSerializedLengthView *)pStream;
    pFileWrapper = LoadView.m_pFileWrapper;
    dwOffset = pFileWrapper->m_pVtable->m_pGetOffset(pFileWrapper);
    cbRead = pSerializedView->m_cbSerialized18;

    if (fAlreadyLocked == 0) {
        dwStartTime = timeGetTime();
        while (pFileWrapper->m_pVtable->m_pLockRange(pFileWrapper, dwOffset, cbRead) == 0) {
            dwNow = timeGetTime();
            if (99 < dwNow - dwStartTime) {
                return 0;
            }
        }
    }

    if (pFileWrapper->m_pVtable->m_pRead(pFileWrapper, (LPVOID)g_pEffTransportPacketBuffer, cbRead) == 0) {
        pFileWrapper->m_pVtable->m_pUnlockRange(pFileWrapper, dwOffset, cbRead);
        return 0;
    }

    LoadEffStreamFromMemory(pStream, (int)(unsigned long)g_pEffTransportPacketBuffer);
    if (fKeepLocked == 0 && pFileWrapper->m_pVtable->m_pUnlockRange(pFileWrapper, dwOffset, cbRead) == 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00479B30
int LoadFileBackedEffRecordPayload(void *pObject, int nSlotIndex) {
    NETWORK_FileBackedRecordPayloadView PayloadView;
    NETWORK_FileBackedDispatchOffsets *pChannelOffsets;
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    NETWORK_EffStreamRecordSlot *pSlot;
    NETWORK_FileWrapperObject *pFileWrapper;
    NETWORK_ChannelStateHeader *pChannelState;
    NETWORK_TimedStreamHeader *pTimedStream;
    NETWORK_ChannelStateRuntimeVtable *pChannelStateVtable;
    NETWORK_TimedStreamRuntimeVtable *pTimedStreamVtable;
    NETWORK_PeerAddressService *pPeerAddressService;
    char *pbRecordTableBase;
    int nResult;
    DWORD dwNow;

    pRecordTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    pbRecordTableBase = (char *)pObject;
    pChannelOffsets = pRecordTable->m_pChannelThunk0c;
    PayloadView.m_pRecordTable = pRecordTable;
    PayloadView.m_pChannelState =
        (NETWORK_ChannelStateHeader *)(pbRecordTableBase + 0xc + pChannelOffsets->m_nChannelStateViewOffset04);
    PayloadView.m_pTimedStream =
        (NETWORK_TimedStreamHeader *)(pbRecordTableBase + 0xc + pChannelOffsets->m_nTimedStreamViewOffset08);
    PayloadView.m_pFileWrapper =
        (NETWORK_FileWrapperObject *)(pbRecordTableBase + 0xc + pChannelOffsets->m_nFileWrapperViewOffset0c);
    pFileWrapper = PayloadView.m_pFileWrapper;
    pChannelState = PayloadView.m_pChannelState;
    pTimedStream = PayloadView.m_pTimedStream;
    pChannelStateVtable = (NETWORK_ChannelStateRuntimeVtable *)pChannelState->m_pVtable;
    pTimedStreamVtable = (NETWORK_TimedStreamRuntimeVtable *)pTimedStream->m_pVtable;
    pSlot = &pRecordTable->m_pSlots[nSlotIndex];
    nResult = pFileWrapper->m_pVtable->m_pSeek(pFileWrapper,
                                               g_cbEffTransportMaxPacketBytes * nSlotIndex +
                                                   pRecordTable->m_nPayloadBaseOffset08);
    if (nResult == 0) {
        return 0;
    }

    nResult = pFileWrapper->m_pVtable->m_pRead(pFileWrapper, (LPVOID)g_pEffTransportPacketBuffer, pSlot->m_cbPayload);
    if (nResult == 0) {
        pChannelStateVtable->m_pHandleReadFailure(pChannelState);
        return 0;
    }

    dwNow = timeGetTime();
    pTimedStream->m_dwLastTick3c = dwNow;
    g_cbEffTransportCurrentPacketBytes = (int)pSlot->m_cbPayload;
    pPeerAddressService = (NETWORK_PeerAddressService *)g_pEffTransportPeerAddressState;
    pPeerAddressService->m_pVtable->m_pSelectPeerName(pPeerAddressService, pSlot->m_szSourceName);
    if (pChannelState->m_fBusy10 == 0) {
        pTimedStreamVtable->m_pServiceLoadedPacket(pTimedStream);
    }
    ProcessEffTransportPacketHeader((int *)((char *)pTimedStream + 0xc));
    return 1;
}

// FUNCTION: LEMBALL 0x0047A470
DWORD IncrementFileBackedEffStreamMarker(int *pObject) {
    NETWORK_FileBackedMarkerOwner *pOwner;
    NETWORK_FileBackedMarkerOffsets *pOffsets;
    NETWORK_FileWrapperObject *pReadThunk;
    NETWORK_FileWrapperObject *pWriteThunk;
    unsigned long ulObjectBase;

    pOwner = (NETWORK_FileBackedMarkerOwner *)pObject;
    ulObjectBase = (unsigned long)pOwner;
    pOffsets = pOwner->m_pOffsets;
    pReadThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    if (LoadEffStreamFromFileBackedRange((void *)(unsigned long)(pOffsets->m_nStreamOffset1c + ulObjectBase),
                                         &pOwner->m_Marker04,
                                         1,
                                         0) == 0) {
        return 0;
    }
    pOwner->m_Marker04.m_Stream04.m_wReferenceCount2c =
        (unsigned short)(pOwner->m_Marker04.m_Stream04.m_wReferenceCount2c + 1);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    return WriteEffStreamToLockedFile(pWriteThunk, (int *)&pOwner->m_Marker04, 0, 1);
}

// FUNCTION: LEMBALL 0x0047A4D0
DWORD DecrementFileBackedEffStreamMarker(int *pObject) {
    NETWORK_FileBackedMarkerOwner *pOwner;
    NETWORK_FileBackedMarkerOffsets *pOffsets;
    NETWORK_FileWrapperObject *pReadThunk;
    NETWORK_FileWrapperObject *pWriteThunk;
    unsigned long ulObjectBase;

    pOwner = (NETWORK_FileBackedMarkerOwner *)pObject;
    ulObjectBase = (unsigned long)pOwner;
    pOffsets = pOwner->m_pOffsets;
    pReadThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    if (LoadEffStreamFromFileBackedRange((void *)(unsigned long)(pOffsets->m_nStreamOffset1c + ulObjectBase),
                                         &pOwner->m_Marker04,
                                         1,
                                         0) == 0) {
        return 0;
    }
    pOwner->m_Marker04.m_Stream04.m_wReferenceCount2c =
        (unsigned short)(pOwner->m_Marker04.m_Stream04.m_wReferenceCount2c - 1);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    return WriteEffStreamToLockedFile(pWriteThunk, (int *)&pOwner->m_Marker04, 0, 1);
}

// FUNCTION: LEMBALL 0x0047A530
BYTE DecrementFileBackedEffMarkerAndReleaseIfZero(int nObjectBasePlus0x38) {
    NETWORK_FileBackedCallbackStatusFront *pCallbackFront;
    NETWORK_FileBackedMarkerReleaseFront *pReleaseFront;
    NETWORK_FileBackedMarkerOwner *pOwner;
    NETWORK_FileBackedMarkerOffsets *pOffsets;
    NETWORK_CallbackThunkObject *pReleaseThunk;
    BYTE bStatus;

    pReleaseFront = (NETWORK_FileBackedMarkerReleaseFront *)(unsigned long)nObjectBasePlus0x38;
    pOwner = (NETWORK_FileBackedMarkerOwner *)(unsigned long)(nObjectBasePlus0x38 - 0x38);
    pOffsets = pOwner->m_pOffsets;
    DecrementFileBackedEffStreamMarker((int *)pOwner);
    pCallbackFront = (NETWORK_FileBackedCallbackStatusFront *)((char *)pReleaseFront + pOffsets->m_nNestedThunkOffset18);
    bStatus = MapNestedEffCallbackFailureToStatus6(pCallbackFront);
    if (pOwner->m_Marker04.m_Stream04.m_wReferenceCount2c == 0) {
        pReleaseThunk = (NETWORK_CallbackThunkObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 +
                                                                       (int)(unsigned long)pOwner);
        pReleaseThunk->m_pVtable->m_pRelease(pReleaseThunk);
    }
    return bStatus;
}

// FUNCTION: LEMBALL 0x0047B9B0
BYTE AdjustAndReleaseCompositeFileBackedEffMarker(void *pObject) {
    return DecrementFileBackedEffMarkerAndReleaseIfZero(
        (int)(unsigned long)((char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta));
}

// FUNCTION: LEMBALL 0x0047BB40
BYTE AdjustAndReleaseGlobalStateFileBackedEffMarker(void *pObject) {
    return DecrementFileBackedEffMarkerAndReleaseIfZero(
        (int)(unsigned long)((char *)pObject - ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 8));
}

// FUNCTION: LEMBALL 0x0047BE30
BYTE AdjustAndMapEmbeddedFileBackedEffCallbackFailure(void *pObject) {
    return MapNestedEffCallbackFailureToStatus6((char *)pObject + 0x138);
}

// FUNCTION: LEMBALL 0x00479C10
void ServicePendingFileBackedEffRecords(int *pObject) {
    NETWORK_FileBackedRecordServiceView ServiceView;
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    NETWORK_FileBackedDispatchOffsets *pChannelOffsets;
    NETWORK_FileWrapperObject *pFileWrapper;
    NETWORK_ChannelStateHeader *pChannelState;
    NETWORK_EffStreamRecordSlot *pSlot;
    char *pbRecordTableBase;
    int nLockResult;
    const int cbRecordSlot = sizeof(NETWORK_EffStreamRecordSlot);
    void *pRuntimeWindow;
    int i;
    int nSlotCount;
    int fLoadedOne;

    pRecordTable = (NETWORK_EffStreamRecordSlotTable *)pObject;
    pbRecordTableBase = (char *)pObject;
    pChannelOffsets = pRecordTable->m_pChannelThunk0c;
    ServiceView.m_pRecordTable = pRecordTable;
    ServiceView.m_pChannelState = (NETWORK_ChannelStateHeader *)(pbRecordTableBase + pChannelOffsets->m_nChannelStateViewOffset04);
    ServiceView.m_pFileWrapper =
        (NETWORK_FileWrapperObject *)(pbRecordTableBase + 0xc + pChannelOffsets->m_nFileWrapperViewOffset0c);
    pChannelState = ServiceView.m_pChannelState;
    if ((pChannelState->m_fReceiving1c == 0 && pChannelState->m_fChannelOpen24 == 0) || pChannelState->m_fPending18 == 0) {
        return;
    }

    pFileWrapper = ServiceView.m_pFileWrapper;
    nLockResult = pFileWrapper->m_pVtable->m_pLockRange(pFileWrapper,
                                                        pRecordTable->m_nRecordHeaderOffset04,
                                                        pRecordTable->m_nAccumulatedStreamLength18);
    if (nLockResult == 0) {
        pRuntimeWindow = 0;
        if (g_pActiveNetworkRuntimeWindow != 0) {
            pRuntimeWindow = (char *)g_pActiveNetworkRuntimeWindow - 0x10;
        }
        ScheduleNetworkRuntimeTimerEvent(pRuntimeWindow, 0x32);
        return;
    }

    if (pRecordTable->m_nPendingSlot10 == -1) {
        pFileWrapper->m_pVtable->m_pSeek(pFileWrapper, pRecordTable->m_nRecordHeaderOffset04);
        pFileWrapper->m_pVtable->m_pRead(pFileWrapper,
                                         (LPVOID)g_pEffTransportPacketBuffer,
                                         pRecordTable->m_nAccumulatedStreamLength18);
        nLockResult = pFileWrapper->m_pVtable->m_pUnlockRange(pFileWrapper,
                                                              pRecordTable->m_nRecordHeaderOffset04,
                                                              pRecordTable->m_nAccumulatedStreamLength18);
        if (nLockResult != 0) {
            nSlotCount = 0;
            LoadEffStreamFromMemory((void *)(unsigned long)pRecordTable->m_pVtable, (int)(unsigned long)g_pEffTransportPacketBuffer);
            fLoadedOne = 0;
            if (0 < pChannelState->m_cSlots14) {
                do {
                    pSlot = &pRecordTable->m_pSlots[nSlotCount];
                    if (pSlot->m_wObservedMarker < pSlot->m_wCommittedMarker) {
                        if (fLoadedOne) {
                            pRecordTable->m_nPendingSlot10 = nSlotCount;
                            return;
                        }
                        LoadFileBackedEffRecordPayload(pObject, nSlotCount);
                        pSlot->m_wObservedMarker = pSlot->m_wCommittedMarker;
                        fLoadedOne = 1;
                    }
                    ++nSlotCount;
                } while (nSlotCount < pChannelState->m_cSlots14);
            }
        }
    } else {
        pFileWrapper->m_pVtable->m_pSeek(pFileWrapper,
                                         cbRecordSlot * pRecordTable->m_nPendingSlot10 +
                                             pRecordTable->m_nRecordHeaderOffset04);
        pFileWrapper->m_pVtable->m_pRead(pFileWrapper,
                                         (LPVOID)g_pEffTransportPacketBuffer,
                                         pRecordTable->m_nAccumulatedStreamLength18);
        nLockResult = pFileWrapper->m_pVtable->m_pUnlockRange(pFileWrapper,
                                                              pRecordTable->m_nRecordHeaderOffset04,
                                                              pRecordTable->m_nAccumulatedStreamLength18);
        if (nLockResult != 0) {
            pSlot = &pRecordTable->m_pSlots[pRecordTable->m_nPendingSlot10];
            LoadEffStreamFromMemory(pSlot, (int)(unsigned long)g_pEffTransportPacketBuffer);
            LoadFileBackedEffRecordPayload(pObject, pRecordTable->m_nPendingSlot10);
            pSlot->m_wObservedMarker = pSlot->m_wCommittedMarker;
            i = pRecordTable->m_nPendingSlot10;
            nSlotCount = pChannelState->m_cSlots14;
            if (i < nSlotCount) {
                do {
                    pSlot = &pRecordTable->m_pSlots[i];
                    if (pSlot->m_wObservedMarker < pSlot->m_wCommittedMarker) {
                        pRecordTable->m_nPendingSlot10 = i;
                        break;
                    }
                    ++i;
                } while (i < nSlotCount);
            }
            if (pChannelState->m_cSlots14 == i) {
                pRecordTable->m_nPendingSlot10 = -1;
            }
        }
    }
}

// FUNCTION: LEMBALL 0x0047A0B0
int AppendFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    NETWORK_FileBackedAppendOffsets *pOffsets;
    NETWORK_EffTransportRuntimeService *pRuntimeService;
    NETWORK_EffStreamRecordSlot *pSlot;
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    NETWORK_ChannelStateHeader *pOutputState;
    NETWORK_FileWrapperObject *pFileWrapper;
    NETWORK_FileBackedAppendNameProviderState *pNameProviderState;
    NETWORK_StreamNameProvider *pNameProvider;
    char *pszSource;
    char *pszTarget;
    unsigned int cch;
    unsigned int count;
    int nCurrentSlot;
    const int cbRecordSlot = sizeof(NETWORK_EffStreamRecordSlot);
    DWORD dwRecordSlotOffset;
    DWORD dwPayloadSlotOffset;
    DWORD cbLocked;
    int fFailed;
    char *pbRecordWriterBase;
    char *pbThunkBase;
    DWORD dwNow;

    pbRecordWriterBase = (char *)pObject;
    pbThunkBase = pbRecordWriterBase - 0x3c;
    pRecordTable = (NETWORK_EffStreamRecordSlotTable *)((char *)pObject - 0x48);
    pOffsets = (NETWORK_FileBackedAppendOffsets *)pRecordTable->m_pChannelThunk0c;
    pOutputState = (NETWORK_ChannelStateHeader *)(pbThunkBase + pOffsets->m_nOutputStateOffset04);
    pFileWrapper = (NETWORK_FileWrapperObject *)(pbThunkBase + pOffsets->m_nFileWrapperOffset0c);
    pNameProviderState =
        (NETWORK_FileBackedAppendNameProviderState *)(pbRecordWriterBase + pOffsets->m_nNameProviderOffset08);
    pNameProvider = &pNameProviderState->m_NameProvider34;
    if (pOutputState->m_fChannelOpen24 == 0) {
        return 0;
    }

    pRuntimeService = (NETWORK_EffTransportRuntimeService *)g_pEffTransportRuntimeService;
    nCurrentSlot = pRecordTable->m_nPendingSlot10;
    pSlot = &pRecordTable->m_pSlots[nCurrentSlot];

    dwRecordSlotOffset = cbRecordSlot * nCurrentSlot + pRecordTable->m_nPayloadBaseOffset08;
    pFileWrapper->m_pVtable->m_pSeek(pFileWrapper, dwRecordSlotOffset);

    pszSource = pRuntimeService->m_pVtable->m_pGetLocalHostName(pRuntimeService);
    cch = 0xffffffff;
    do {
        pszTarget = pszSource;
        if (cch == 0) {
            break;
        }
        --cch;
        pszTarget = pszSource + 1;
        if (*pszSource == '\0') {
            break;
        }
        pszSource = pszTarget;
    } while (1);
    cch = ~cch;
    pszSource = pszTarget - cch;
    pszTarget = pSlot->m_szSourceName;
    for (count = cch >> 2; count != 0; --count) {
        *(int *)pszTarget = *(int *)pszSource;
        pszSource += 4;
        pszTarget += 4;
    }
    for (count = cch & 3; count != 0; --count) {
        *pszTarget++ = *pszSource++;
    }

    pszSource = pNameProvider->m_pVtable->m_pGetName(pNameProvider);
    cch = 0xffffffff;
    do {
        pszTarget = pszSource;
        if (cch == 0) {
            break;
        }
        --cch;
        pszTarget = pszSource + 1;
        if (*pszSource == '\0') {
            break;
        }
        pszSource = pszTarget;
    } while (1);
    cch = ~cch;
    pszSource = pszTarget - cch;
    pszTarget = pSlot->m_szTargetName;
    for (count = cch >> 2; count != 0; --count) {
        *(int *)pszTarget = *(int *)pszSource;
        pszSource += 4;
        pszTarget += 4;
    }
    for (count = cch & 3; count != 0; --count) {
        *pszTarget++ = *pszSource++;
    }

    pSlot->m_cbPayload = cbPayload;
    cbLocked = WriteEffStreamToLockedFile(pRecordTable,
                                          (int *)pSlot,
                                          1,
                                          0);
    fFailed = 1;
    if (cbLocked != 0) {
        dwPayloadSlotOffset = nCurrentSlot * g_cbEffTransportMaxPacketBytes + pRecordTable->m_nPayloadBaseOffset08;
        pFileWrapper->m_pVtable->m_pSeek(pFileWrapper, dwPayloadSlotOffset);
        if (pFileWrapper->m_pVtable->m_pWrite(pFileWrapper, pvPayload, cbPayload) != 0) {
            fFailed = 0;
        }
    }

    if (fFailed == 0) {
        pRecordTable->m_nPendingSlot10 = nCurrentSlot + 1;
        dwNow = timeGetTime();
        pNameProviderState->m_dwLastWriteTime00 = dwNow;
    }
    if (cbLocked != 0) {
        pFileWrapper->m_pVtable->m_pUnlockRange(pFileWrapper, dwRecordSlotOffset, cbLocked);
    }
    return fFailed == 0;
}

// FUNCTION: LEMBALL 0x0047A420
int AppendAdjustedFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    const NETWORK_RecordWriterAdjustorFront *pAdjustorFront;
    const NETWORK_RecordWriterAdjustorOffsets *pAdjustorOffsets;
    NETWORK_EffStreamRecordSlotTable *pRecordTable;
    int *pnCurrentSlot;
    int fAppended;
    char *pbRecordTableView;
    char *pbRecordWriterView;

    pAdjustorFront = (const NETWORK_RecordWriterAdjustorFront *)((char *)pObject - 0x38);
    pAdjustorOffsets = pAdjustorFront->m_pOffsets;
    pbRecordWriterView = (char *)pObject + pAdjustorOffsets->m_nRecordWriterViewOffset1c;
    fAppended = AppendFileBackedEffRecordSlot(pbRecordWriterView + 0x10, pvPayload, cbPayload);
    if (fAppended == 0) {
        return 0;
    }

    pnCurrentSlot = (int *)(pbRecordWriterView - 0x28);
    pbRecordTableView = (char *)pObject + pAdjustorOffsets->m_nRecordTableViewOffset18;
    pRecordTable = (NETWORK_EffStreamRecordSlotTable *)(pbRecordTableView - 0x30);
    *pnCurrentSlot = *pnCurrentSlot % pRecordTable->m_cSlots;
    return fAppended;
}

// FUNCTION: LEMBALL 0x0047BC40
int AppendAdjustedGlobalStateFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    return AppendAdjustedFileBackedEffRecordSlot(
                                                 (char *)pObject -
                                                     ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta +
                                                     0x158,
                                                 pvPayload,
                                                 cbPayload);
}

// FUNCTION: LEMBALL 0x0047BEF0
int AppendAdjustedEmbeddedFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    return AppendAdjustedFileBackedEffRecordSlot(
                                                 (char *)pObject -
                                                     ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta,
                                                 pvPayload,
                                                 cbPayload);
}

// FUNCTION: LEMBALL 0x0047BB00
int AppendAdjustedGlobalStateTimedFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    return AppendAdjustedFileBackedEffRecordSlot(
                                                 (char *)pObject -
                                                     ((NETWORK_AdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta +
                                                     0x154,
                                                 pvPayload,
                                                 cbPayload);
}

// FUNCTION: LEMBALL 0x0047AA10
void InitializeGlobalStateFileBackedEffStorage(int nObjectBasePlus0x15c) {
    NETWORK_GlobalStateFileBackedStorageOwner *pOwner;
    NETWORK_FileWrapperObject *pReadThunk;
    NETWORK_FileWrapperObject *pWriteThunk;
    NETWORK_SlotCountHolder *pSlotCount;
    unsigned long ulObjectBase;
    DWORD cbPacket;
    unsigned int count;
    unsigned char *pByte;
    unsigned int i;
    void *pZeroBuffer;

    pOwner = (NETWORK_GlobalStateFileBackedStorageOwner *)(unsigned long)(nObjectBasePlus0x15c - 0x15c);
    ulObjectBase = (unsigned long)pOwner;
    pReadThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOwner->m_pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (NETWORK_FileWrapperObject *)(unsigned long)(pOwner->m_pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pSlotCount =
        (NETWORK_SlotCountHolder *)(unsigned long)(pOwner->m_pOffsets->m_nSlotCountViewOffset18 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    WriteEffStreamToLockedFile(pWriteThunk, (int *)&pOwner->m_InitialStream04, 0, 0);
    WriteEffStreamToLockedFile(pWriteThunk, (int *)(unsigned long)g_pGlobalStateEff512ByteStream, 0, 0);
    WriteEffStreamToLockedFile(pWriteThunk, (int *)(unsigned long)*(int *)pWriteThunk, 0, 0);

    pZeroBuffer = AllocateVSMemBlock(g_cbEffTransportMaxPacketBytes);
    cbPacket = g_cbEffTransportMaxPacketBytes;
    pByte = (unsigned char *)pZeroBuffer;
    for (count = g_cbEffTransportMaxPacketBytes >> 2; count != 0; --count) {
        *(int *)pByte = 0;
        pByte += 4;
    }
    for (count = cbPacket & 3; count != 0; --count) {
        *pByte++ = 0;
    }

    i = 0;
    if (0 < pSlotCount->m_cSlots) {
        do {
            ++i;
            pReadThunk->m_pVtable->m_pWrite(pReadThunk, pZeroBuffer, g_cbEffTransportMaxPacketBytes);
        } while (i < pSlotCount->m_cSlots);
    }
    FreeVSMemBlock(pZeroBuffer);
}
