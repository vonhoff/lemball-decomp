#include "GAME.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/VSINIT.H"
#include "ENGINE/NET/VSNET.H"
#include "ENGINE/CORE/SAFEVT.H"
#include "ENGINE/MEDIA/VSSTRM.H"
#include <string.h>
#include <stdio.h>

extern void *g_GAME_EffStreamConstructionVtable[6];

struct VsSoundWaveOutEffectBackend;

extern int PASCAL ReturnTrueVtableCallback(void *pObject);
extern int ReturnTrueVtableCallbackSecondary(void);
extern void NoopVtableCallback(void);
struct VsNetChannelOwnerObject {
    void ServiceEffTransportConnectRequest(void);
};
extern void *LEMBALL_FASTCALL DeleteWaveOutEffectBackend(
    VsSoundWaveOutEffectBackend *pObject, int nUnused, int fDelete);
struct VsNetCompleteEffTransportPendingWriteView {
    void CompleteEffTransportPendingWrite(int fQueueEvent);
};
struct VsNetEffTransportPeer {
    void PrepareEffTransportBroadcastStatusPayload(char *pszHostName);
};
extern void *g_pEffTransportDispatchQueue;
extern char *GetWaveOutEffectBackendDescription(VsSoundWaveOutEffectBackend *pObject);
extern int GetWaveOutEffectBackendStatus0047CC10(void *pObject);
extern int InitializeWaveOutEffectDevice0047CB00(void *pObject,
                                                  int fActivateDevice,
                                                  int fStartStreaming);
extern int IsWaveOutEffectBackendAvailable(void);
extern int LEMBALL_FASTCALL ResetAndCloseWaveOutEffectDevice0047CC20(void *pObject);
extern int IsWaveOutEffectInstanceAvailable0047CDD0(void *pObject);
extern int ReturnOneWaveOutBackendValue0047CDF0(void *pObject);
// FUNCTION: LEMBALL 0x00462F50
size_t WriteAndFlushFileBuffer(FILE *pFile, void *pBuffer, size_t cbBuffer) {
    size_t cbWritten;

    cbWritten = fwrite(pBuffer, 1, cbBuffer, pFile);
    fflush(pFile);
    return cbWritten;
}

#if !defined(_MSC_VER)
extern "C" int _purecall(void) {
    ExitProcess(0x19);
    return 0;
}
#endif

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
int WINAPI WSAGetLastError(void);
}

// FUNCTION: LEMBALL 0x0047B8D0
int WINAPI WSAGetLastErrorThunk0047B8D0(void) {
    return WSAGetLastError();
}

enum {
    CREATE_NEW = 1,
    OPEN_EXISTING = 3,
    FILE_BEGIN = 0,
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
BYTE __fastcall MapNestedEffCallbackFailureToStatus6(void *pObject);
void *DeleteEffChannelStateSideBufferWrapper(void *pObject, BYTE fFreeMemory);
void LEMBALL_FASTCALL ClearNestedEffChannelAsyncStatus(void *pObject);
void AdjustAndClearEmbeddedFileBackedEffChannelAsyncErrorStatus(void *pObject);
void LEMBALL_FASTCALL AdjustAndClearGlobalStateTimedEffTransportPendingWriteState(
    void *pObject, void *pUnused);
void *AdjustAndDeleteGlobalStateCompositeAt30(void *pObject, BYTE fFreeMemory);
void AdjustAndWriteGlobalStateTimedEffStreamWithGlobalSession(void *pObject);
void AdjustAndClaimGlobalStateTimedEffTransportRecordPayload(void *pObject);
void *AdjustAndDeleteGlobalStateCompositeAtA8(void *pObject, BYTE fFreeMemory);
void AdjustAndWriteGlobalStateDualEffStreamWithGlobalSession(void *pObject);
void AdjustAndClaimGlobalStateDualEffTransportRecordPayload(void *pObject);
BYTE AdjustAndMapGlobalStateFileBackedEffCallbackFailure(void *pObject);
void LEMBALL_FASTCALL AdjustAndClearGlobalStateTimedEffTransportPendingWriteState(
    void *pObject, void *pUnused);
struct VsNetAdjustedEffTransportPendingWriteState {
    void ClearPendingWrite(void *pUnused);
};
void AdjustAndWriteDualSlotTableFileBackedEffHeader(void *pObject);
int PrepareGlobalStateBroadcastPath0047AB20(void *pObject, char *pszHostName);
void InitializeDualSlotCompositeStorage0047B580(void *pObject);
void ConfigureDualSlotCompositeFilePath0047B440(void *pObject);
void AdjustAndCompleteDualSlotEffTransportPendingWrite0047BFB0(void *pObject, int fQueueEvent);
char *FindCharInCString0047FE00(char *pszText, char ch);
void *AdjustAndDeleteDualSlotTableTimedFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory);
void *AdjustAndDeleteDualSlotTableDualFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory);
void *AdjustAndDeleteDualSlotTableFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory);
struct VsNetAckedEffTransportRecordOwner {
    char m_abUnknown00[0x4c];
    void *m_pRecordTable4c;
    void *ClaimAckedEffTransportRecordPayload(void);
};
extern int PASCAL ReturnTrueVtableCallbackThunk(void *pObject);
extern int ReturnTrueVtableCallbackSecondaryThunk(void);
extern void ReverseEffTransportPayload(void *pObject);
extern void NoopVtableCallbackThunk(void);
extern void WriteEffTransportTaggedHeader(void *pObject);
extern void ReturnVoidVtableCallback(void);
struct VsNetEffTransportConnectCallback {
    void QueueEffTransportConnectEvent(void);
};
struct VsNetDispatchAckedEffTransportPayloadView {
    void *DispatchAckedEffTransportPayloadEvent(void);
};
struct VsNetPrefixedEffTransportControlView {
    void DispatchPrefixedEffTransportControlStream(int nDispatchType, void *pEvent);
};
struct VsNetQueueEffTransportPayloadEventView {
    void QueueEffTransportPayloadEvent(unsigned short nType, void *pPayload);
};
struct VsNetAdjustedEffTransportPeerView {
    void CloseAdjustedEffTransportPeerByKey(void *pUnused);
};
extern void EffStreamChannelStateRet4Thunk(BYTE fDelete);
BYTE AdjustAndMapNestedEffCallbackFailureFromB8(void *pObject);
void *AdjustAndDeleteDualFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory);
void *AdjustAndDeleteDualFileBackedEffChannelWrapper2c(void *pObject, BYTE fFreeMemory);
void *AdjustAndDeleteTimedFileBackedEffChannelWrapper30(void *pObject, BYTE fFreeMemory);
void *AdjustAndDeleteTimedFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory);
void AdjustAndClearTimedFileBackedEffChannelAsyncStatus(void *pObject);
void AdjustAndClearPendingWriteStateThunk(void *pObject, void *pUnused);
void InvokeAdjustedTimedEffStreamServiceCallbackThunk(void *pObject, void *pArgument);
int AppendAdjustedFileBackedEffRecordSlotThunk(void *pObject, const void *pvPayload, DWORD cbPayload);
int AppendAdjustedGlobalStateFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload);
int AppendAdjustedGlobalStateTimedFileBackedEffRecordSlot(void *pObject,
                                                           const void *pvPayload,
                                                           DWORD cbPayload);
void SetTimedFileBackedEffChannelWord(void *pObject, unsigned short nValue);
void AdjustAndClearNestedEffChannelAsyncStatusFromB8(void *pObject);
void *DeleteDualFileBackedEffChannelNoopThunk(void *pObject, BYTE fFreeMemory);
BYTE AdjustAndMapEmbeddedFileBackedEffCallbackFailure(void *pObject);
void *AdjustAndDeleteEmbeddedFileBackedEffChannelStackWrapper40(void *pObject, BYTE fFreeMemory);
void AdjustAndClearEmbeddedFileBackedEffChannelAsyncErrorStatus(void *pObject);
void *AdjustAndDeleteEmbeddedTimedFileBackedEffChannelStackWrapper(void *pObject, BYTE fFreeMemory);
void AdjustAndWriteEmbeddedTimedEffStreamWithGlobalSession(void *pObject);
void AdjustAndClaimEmbeddedTimedEffTransportRecordPayload(void *pObject);
void AdjustAndInvokeGlobalStateTimedEffStreamServiceCallback(void *pObject, void *pArgument);
int AppendAdjustedEmbeddedFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload);
void *AdjustAndDeleteEmbeddedDualFileBackedEffChannelStackWrapper(void *pObject, BYTE fFreeMemory);
void AdjustAndWriteEmbeddedDualEffStreamWithGlobalSession(void *pObject);
void AdjustAndClaimEmbeddedDualEffTransportRecordPayload(void *pObject);
BYTE AdjustAndReleaseGlobalStateFileBackedEffMarker(void *pObject);
void *AdjustAndDeleteGlobalStateFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory);
void *AdjustAndDeleteGlobalStateTimedFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory);
void *AdjustAndDeleteGlobalStateDualFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory);
void AdjustAndCloseEffTransportPeerByKey(void *pObject, void *pArgument);
void AdjustAndClearEmbeddedEffChannelAsyncStatus13c(void *pObject);
void AdjustAndWriteGlobalStateTimedEffStreamFrom158(void *pObject);
void AdjustAndClaimGlobalStateTimedEffStreamFrom158(void *pObject);
void AdjustAndInvokeTimedEffStreamServiceFrom154(void *pObject, void *pArgument);
void AdjustAndWriteDualSlotTimedEffStreamWithGlobalSession(void *pObject);
void AdjustAndClaimDualSlotTimedEffTransportRecordPayload(void *pObject);
void DispatchAdjustedPrefixedEffTransportControlStream(void *pObject, int nDispatchType, void *pEvent);
void DispatchAdjustedAckedEffTransportPayloadEventFromEmbeddedThunk(void *pObject);
void QueueAdjustedEffTransportConnectEvent(void *pObject);
void DispatchAdjustedAckedEffTransportPayloadEventFromDualThunk(void *pObject);
void QueueAdjustedEffTransportPayloadEvent(void *pObject, unsigned short nType, void *pPayload);
BYTE AdjustAndReleaseCompositeFileBackedEffMarker(void *pObject);
void *AdjustAndDeleteCompositeFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory);
extern void *g_NETWORK_SocketWindowEffChannelVtable;
extern void *g_NETWORK_TcpipSocketStackDualRet8;
static void NoopGlobalStateCompositeCallback0047AB10(void *pObject);
static void SetGlobalStateCompositeOpenFlag0047AED0(void *pObject);
static void ClearGlobalStateCompositeOpenFlag0047AEE0(void *pObject);
static int AllocateGlobalStateEffRecordSlot0047AD70(void *pObject);
static void ReleaseGlobalStateEffRecordSlot0047AE00(void *pObject, unsigned short nSlot);
static void ServiceGlobalStateEffComposite0047AEF0(void *pObject);
static int SetDualSlotCompositePathPair0047B3B0(void *pObject,
                                                 const char *pszPath08,
                                                 const char *pszPath0c);
static void PrepareDualSlotComposite0047B4D0(void *pObject, int nArgument);
static void ActivateDualSlotComposite0047B540(void *pObject);
static void ServiceDualSlotCompositeRecords0047B990(void *pObject);

struct VsNetDualSlotCompositeMethodView {
    virtual void Slot00(void) = 0;
    virtual void Slot01(void) = 0;
    virtual void Prepare(void) = 0;
    virtual void Open(void *pArgument) = 0;
    virtual void Activate(void) = 0;
    virtual void Poll(void) = 0;
};

// FUNCTION: LEMBALL 0x0047AB10
static void NoopGlobalStateCompositeCallback0047AB10(void *pObject) {
    (void)pObject;
}

// FUNCTION: LEMBALL 0x0047AED0
static void SetGlobalStateCompositeOpenFlag0047AED0(void *pObject) {
    *(int *)((char *)pObject + 0x14) = 1;
}

// FUNCTION: LEMBALL 0x0047AEE0
static void ClearGlobalStateCompositeOpenFlag0047AEE0(void *pObject) {
    *(int *)((char *)pObject + 0x14) = 0;
}


struct VsNetConstructionAdjustorVtable {
    int m_nSelfDelta00;
    int m_nPrimaryOffset;
    int m_nSecondaryOffset;
    int m_nTertiaryOffset;
    int m_nQuaternaryOffset;
    int m_nFatalOffset;
    int m_nRecordTableOffsetA;
    int m_nRecordTableOffsetB;
};

struct VsNetFileBackedStreamAdjustorOffsets {
    int m_nSelfDelta00;
    int m_nPrimaryOffset04;
    int m_nSecondaryOffset08;
    int m_nTertiaryOffset0c;
    int m_nQuaternaryOffset10;
    int m_nFatalOffset14;
    int m_nRecordTableOffset18;
    void (*m_pFatalCallback1c)(void);
};

// These tables are not ordinary virtual tables. They are compiler-emitted
// adjustor metadata describing where embedded subobjects and thunk fronts live
// inside one larger aggregate object.

struct VsNetCompositeViewOffsets {
    int m_nSelfDelta00;
    int m_nTransportViewOffset04;
    int m_nTimedStreamViewOffset08;
    int m_nDualStreamViewOffset0c;
    int m_nDualThunkViewOffset10;
    int m_nMarkerViewOffset14;
    int m_nRecordTableAOffset18;
    int m_nRecordTableBOffset1c;
    /* Dual-slot composites carry two further embedded stream fronts. */
    int m_nRecordTableCOffset20;
    int m_nRecordTableDOffset24;
};

struct VsNetFileBackedChannelViewOffsets {
    int m_nSelfDelta00;
    int m_nChannelStateViewOffset04;
    int m_nStreamViewOffset08;
    int m_nFileWrapperViewOffset0c;
    int m_nFatalThunkViewOffset10;
    void (*m_pFatalCallback14)(void);
    void (*m_pOpenCallback18)(void);
    void (*m_pCloseCallback1c)(void);
};

struct VsNetFileWrapperThunkOffsets {
    int m_nSelfDelta00;
    int m_nOpenThunkViewOffset04;
    int m_nNestedCallbackThunkOffset08;
    int m_nReserved0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1c;
};

struct VsNetEmbeddedStackViewOffsets {
    int m_nSelfDelta00;
    int m_nChannelStateViewOffset04;
    int m_nTimedStreamViewOffset08;
    int m_nDualStreamViewOffset0c;
    int m_nFinalFileThunkViewOffset10;
    int m_nFinalFatalThunkViewOffset14;
    int m_nFinalPrimaryThunkViewOffset18;
    int m_nFinalTimedThunkViewOffset1c;
};

struct VsNetEmbeddedStackFileViewOffsets {
    int m_nSelfDelta00;
    int m_nFileWrapperViewOffset04;
    int m_nChannelStateViewOffset08;
    int m_nTimedStreamViewOffset0c;
    int m_nDualStreamViewOffset10;
    int m_nReserved14;
    int m_nPrimaryThunkViewOffset18;
    int m_nFileViewThunkOffset1c;
};

static const VsNetFileBackedStreamAdjustorOffsets g_NETWORK_FileBackedDualStreamOffsets = {
    -0x44,
    -0x70,
    -0x0c,
    0x0c,
    0x38,
    0x90,
    0xa4,
    (void (*)(void))_purecall,
};
/* 0049A5D8: dual file-backed channel-state thunk table.  First four entries
 * are callable adjustors, followed by fifteen fatal stubs and a null tail. */
static void *g_NETWORK_FileBackedDualPrimaryThunkVtable[] = {
    (void *)AdjustAndMapNestedEffCallbackFailureFromB8,
    (void *)AdjustAndDeleteDualFileBackedEffChannelWrapper,
    (void *)AdjustAndClearNestedEffChannelAsyncStatusFromB8,
    (void *)DeleteDualFileBackedEffChannelNoopThunk,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    0,
};
struct VsNetFileBackedDualSecondaryThunkVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pb;
        unsigned char b;

        pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        b = pb[0]; pb[0] = pb[3]; pb[3] = b;
        b = pb[1]; pb[1] = pb[2]; pb[2] = b;
        pStream->m_nReserved20 += 4;
        pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        b = pb[0]; pb[0] = pb[3]; pb[3] = b;
        b = pb[1]; pb[1] = pb[2]; pb[2] = b;
        pStream->m_nReserved20 += 4;
        for (int i = 0; i < 3; ++i) {
            pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
            b = pb[0]; pb[0] = pb[1]; pb[1] = b;
            pStream->m_nReserved20 += 2;
        }
        ++pStream->m_nReserved20;
    }
    virtual void Noop(void) {}
    virtual void WriteTaggedHeader(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pbTag = (unsigned char *)pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
        pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
        *(unsigned char *)pStream->m_nReserved1c = pbTag[0xe];
        ++pStream->m_nReserved1c;
    }
    virtual void *Delete(BYTE fDelete) {
        return AdjustAndDeleteDualFileBackedEffChannelWrapper2c(this, fDelete);
    }
    // FUNCTION: LEMBALL 0x00462920
    virtual void ReservedRet(void)
    {
    }
    virtual void Fatal0(void) { _purecall(); }
    virtual void Fatal1(void) { _purecall(); }
    virtual BYTE MapNestedFailure(void) {
        return MapNestedEffCallbackFailureToStatus6((char *)this + 0xb8);
    }
};

static VsNetFileBackedDualSecondaryThunkVtableModel g_NETWORK_FileBackedDualSecondaryThunkVtableModel;
static void *g_NETWORK_FileBackedDualSecondaryThunkVtable =
    *(void ***)&g_NETWORK_FileBackedDualSecondaryThunkVtableModel;
static void *g_NETWORK_FileBackedFatalFileWrapperVtable[] = {
    (void *)_purecall,
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
    /* 0047B8D0 -> 00486C34; call sites compare this result with WSAEWOULDBLOCK. */
    (void *)WSAGetLastErrorThunk0047B8D0,
    (void *)GetWin32FileWrapperOwnedPath,
    0,
};

static const VsNetFileBackedStreamAdjustorOffsets g_NETWORK_FileBackedTimedStreamOffsets = {
    -0x44,
    -0x74,
    -0x0c,
    0x0c,
    0x3c,
    0xb0,
    0xc4,
    (void (*)(void))_purecall,
};
static void *g_NETWORK_FileBackedTimedPrimaryThunkVtable[] = {
    (void *)AdjustAndMapGlobalStateFileBackedEffCallbackFailure,
    (void *)AdjustAndDeleteTimedFileBackedEffChannelWrapper,
    (void *)AdjustAndClearTimedFileBackedEffChannelAsyncStatus,
    (void *)AdjustAndClearPendingWriteStateThunk,
};
struct VsNetFileBackedTimedSecondaryThunkVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void ReversePayload(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pb;
        unsigned char b;

        pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        b = pb[0]; pb[0] = pb[3]; pb[3] = b;
        b = pb[1]; pb[1] = pb[2]; pb[2] = b;
        pStream->m_nReserved20 += 4;
        pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        b = pb[0]; pb[0] = pb[3]; pb[3] = b;
        b = pb[1]; pb[1] = pb[2]; pb[2] = b;
        pStream->m_nReserved20 += 4;
        for (int i = 0; i < 3; ++i) {
            pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
            b = pb[0]; pb[0] = pb[1]; pb[1] = b;
            pStream->m_nReserved20 += 2;
        }
        ++pStream->m_nReserved20;
    }
    virtual void Noop(void) {}
    virtual void WriteTaggedHeader(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pbTag = (unsigned char *)pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
        pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
        *(unsigned char *)pStream->m_nReserved1c = pbTag[0xe];
        ++pStream->m_nReserved1c;
    }
    virtual void *Delete(BYTE fDelete) {
        return AdjustAndDeleteTimedFileBackedEffChannelWrapper30(this, fDelete);
    }
    virtual void WriteGlobalSession(void) {
        ((VsNetPeerPayloadSender *)this)->WriteEffStreamWithGlobalSession();
    }
    virtual void *ClaimTimedRecord(void) {
        return ((VsNetAckedEffTransportRecordOwner *)this)
            ->ClaimAckedEffTransportRecordPayload();
    }
    virtual void InvokeService(void *pArgument) {
        InvokeAdjustedTimedEffStreamServiceCallbackThunk(this, pArgument);
    }
    virtual int AppendRecord(const void *pvPayload, DWORD cbPayload) {
        return AppendAdjustedFileBackedEffRecordSlotThunk(this, pvPayload, cbPayload);
    }
};
static VsNetFileBackedTimedSecondaryThunkVtableModel g_NETWORK_FileBackedTimedSecondaryThunkVtableModel;
static void *g_NETWORK_FileBackedTimedSecondaryThunkVtable =
    *(void ***)&g_NETWORK_FileBackedTimedSecondaryThunkVtableModel;

static const VsNetEmbeddedStackViewOffsets g_NETWORK_EmbeddedFileBackedTimedStreamOffsets = {
    -0x44,
    -0x74,
    0,
    0x08,
    0x38,
    0xb0,
    0x108,
    0x10c,
};
static const VsNetEmbeddedStackViewOffsets g_NETWORK_EmbeddedFileBackedDualStreamOffsets = {
    -0x44,
    -0xec,
    -0x44,
    -0x74,
    0,
    0x08,
    0x38,
    0xb0,
};
static const VsNetEmbeddedStackFileViewOffsets g_NETWORK_EmbeddedFileBackedPrimaryThunkOffsets = {
    -0x0c,
    -0x130,
    0x78,
    -0x2c,
    -0x18,
    0,
    -0x04,
    -0x18,
};
static const VsNetEmbeddedStackFileViewOffsets g_NETWORK_EmbeddedFileBackedTimedFileViewOffsets = {
    -0x0c,
    -0x144,
    -0x114,
    -0x40,
    -0x2c,
    0,
    -0x0c,
    -0x130,
};
struct VsNetEmbeddedFileBackedFinalPrimaryThunkVtableModel {
    virtual BYTE MapFailure(void) {
        return AdjustAndMapEmbeddedFileBackedEffCallbackFailure(this);
    }
    virtual void *Delete(BYTE fDelete) {
        return AdjustAndDeleteEmbeddedFileBackedEffChannelStackWrapper40(this, fDelete);
    }
    virtual void ClearAsyncError(void) {
        AdjustAndClearEmbeddedFileBackedEffChannelAsyncErrorStatus(this);
    }
    virtual void ClearPendingWrite(void *pUnused) {
        ((VsNetAdjustedEffTransportPendingWriteState *)
             ((char *)this - *(int *)((char *)this - 4)))
            ->ClearPendingWrite(pUnused);
    }
};
static VsNetEmbeddedFileBackedFinalPrimaryThunkVtableModel
    g_NETWORK_EmbeddedFileBackedFinalPrimaryThunkVtableModel;
static void *g_NETWORK_EmbeddedFileBackedFinalPrimaryThunkVtable =
    *(void ***)&g_NETWORK_EmbeddedFileBackedFinalPrimaryThunkVtableModel;

struct VsNetEmbeddedFileBackedFinalCommonThunkVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    void ReversePayloadBody(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        unsigned char b;
        b = pb[0]; pb[0] = pb[3]; pb[3] = b;
        b = pb[1]; pb[1] = pb[2]; pb[2] = b;
        pStream->m_nReserved20 += 4;
        pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
        b = pb[0]; pb[0] = pb[3]; pb[3] = b;
        b = pb[1]; pb[1] = pb[2]; pb[2] = b;
        pStream->m_nReserved20 += 4;
        for (int i = 0; i < 3; ++i) {
            pb = (unsigned char *)(unsigned long)pStream->m_nReserved20;
            b = pb[0]; pb[0] = pb[1]; pb[1] = b;
            pStream->m_nReserved20 += 2;
        }
        ++pStream->m_nReserved20;
    }
    virtual void WriteTaggedHeader(void) { WriteTaggedHeaderBody(); }
    virtual void Noop(void) {}
    virtual void ReversePayload(void) { ReversePayloadBody(); }
    void WriteTaggedHeaderBody(void) {
        VsNetEffStreamBase *pStream = (VsNetEffStreamBase *)this;
        unsigned char *pbTag = (unsigned char *)(unsigned long)pStream->m_pTagBuffer2c;
        pStream->WriteEffStreamU32BE(*(unsigned int *)pbTag);
        pStream->WriteEffStreamU32BE(*(unsigned int *)(pbTag + 4));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 8));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xa));
        pStream->WriteEffStreamU16BE(*(unsigned short *)(pbTag + 0xc));
        *(unsigned char *)pStream->m_nReserved1c = pbTag[0xe];
        ++pStream->m_nReserved1c;
    }
};

struct VsNetEmbeddedFileBackedFinalTimedThunkVtableModel
    : VsNetEmbeddedFileBackedFinalCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        return AdjustAndDeleteEmbeddedTimedFileBackedEffChannelStackWrapper(this, fDelete);
    }
    virtual void WriteGlobalSession(void) {
        AdjustAndWriteEmbeddedTimedEffStreamWithGlobalSession(this);
    }
    virtual void *ClaimRecord(void) {
        AdjustAndClaimEmbeddedTimedEffTransportRecordPayload(this);
        return 0;
    }
    virtual void InvokeService(void *pArgument) {
        AdjustAndInvokeGlobalStateTimedEffStreamServiceCallback(this, pArgument);
    }
    virtual int AppendRecord(const void *pvPayload, DWORD cbPayload) {
        return AppendAdjustedEmbeddedFileBackedEffRecordSlot(this, pvPayload, cbPayload);
    }
};
static VsNetEmbeddedFileBackedFinalTimedThunkVtableModel
    g_NETWORK_EmbeddedFileBackedFinalTimedThunkVtableModel;
static void *g_NETWORK_EmbeddedFileBackedFinalTimedThunkVtable =
    *(void ***)&g_NETWORK_EmbeddedFileBackedFinalTimedThunkVtableModel;

struct VsNetEmbeddedFileBackedFinalDualThunkVtableModel
    : VsNetEmbeddedFileBackedFinalCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        return AdjustAndDeleteEmbeddedDualFileBackedEffChannelStackWrapper(this, fDelete);
    }
    virtual void Ret(void) {}
    virtual void WriteGlobalSession(void) {
        AdjustAndWriteEmbeddedDualEffStreamWithGlobalSession(this);
    }
    virtual void *ClaimRecord(void) {
        AdjustAndClaimEmbeddedDualEffTransportRecordPayload(this);
        return 0;
    }
    virtual void Ret8(BYTE fDelete, BYTE fReserved) {
        (void)fDelete;
        (void)fReserved;
    }
};
static VsNetEmbeddedFileBackedFinalDualThunkVtableModel
    g_NETWORK_EmbeddedFileBackedFinalDualThunkVtableModel;
static void *g_NETWORK_EmbeddedFileBackedFinalDualThunkVtable =
    *(void ***)&g_NETWORK_EmbeddedFileBackedFinalDualThunkVtableModel;

/* Final global-state subobject tables.  These are separate compiler views;
 * they are not interchangeable with the embedded-stack tables above. */
struct VsNetGlobalStateFinalTimedThunkVtableModel
    : VsNetEmbeddedFileBackedFinalCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        return AdjustAndDeleteGlobalStateTimedFileBackedEffCompositeWrapper(this, fDelete);
    }
    virtual void WriteGlobalSession(void) {
        AdjustAndWriteGlobalStateTimedEffStreamFrom158(this);
    }
    virtual void ClaimRecord(void) {
        AdjustAndClaimGlobalStateTimedEffStreamFrom158(this);
    }
    virtual void InvokeService(void *pArgument) {
        AdjustAndInvokeTimedEffStreamServiceFrom154(this, pArgument);
    }
    virtual int AppendRecord(const void *pvPayload, DWORD cbPayload) {
        return AppendAdjustedGlobalStateFileBackedEffRecordSlot(this, pvPayload, cbPayload);
    }
};
static VsNetGlobalStateFinalTimedThunkVtableModel
    g_NETWORK_GlobalStateFinalTimedThunkVtableModel;

struct VsNetGlobalStateFinalDualThunkVtableModel
    : VsNetEmbeddedFileBackedFinalCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        return AdjustAndDeleteGlobalStateDualFileBackedEffCompositeWrapper(this, fDelete);
    }
    virtual void Ret(void) {}
    virtual void WriteGlobalSession(void) {
        AdjustAndWriteDualSlotTimedEffStreamWithGlobalSession(this);
    }
    virtual void ClaimRecord(void) {
        AdjustAndClaimDualSlotTimedEffTransportRecordPayload(this);
    }
    virtual void DispatchControl(int nDispatchType, void *pEvent) {
        DispatchAdjustedPrefixedEffTransportControlStream(this, nDispatchType, pEvent);
    }
};
static VsNetGlobalStateFinalDualThunkVtableModel
    g_NETWORK_GlobalStateFinalDualThunkVtableModel;

static void *g_NETWORK_GlobalStateFinalPrimaryThunkVtable[] = {
    (void *)AdjustAndReleaseGlobalStateFileBackedEffMarker,
    (void *)AdjustAndDeleteGlobalStateFileBackedEffCompositeWrapper,
    (void *)AdjustAndClearEmbeddedEffChannelAsyncStatus13c,
    (void *)AdjustAndCloseEffTransportPeerByKey,
    (void *)AllocateGlobalStateEffRecordSlot0047AD70,
    (void *)ReleaseGlobalStateEffRecordSlot0047AE00,
    (void *)NoopGlobalStateCompositeCallback0047AB10,
    (void *)PrepareGlobalStateBroadcastPath0047AB20,
    (void *)ServiceGlobalStateEffComposite0047AEF0,
    (void *)SetGlobalStateCompositeOpenFlag0047AED0,
    (void *)ClearGlobalStateCompositeOpenFlag0047AEE0,
    (void *)_purecall,
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
    (void *)WSAGetLastErrorThunk0047B8D0,
    (void *)GetWin32FileWrapperOwnedPath,
    0,
};

static void *g_NETWORK_GlobalStateFinalFatalVtable[] = {
    (void *)InitializeGlobalStateFileBackedEffStorageAdjustor,
    0,
};

// Global-state composite layout:
// 00 self-delta / owning view
// 04 transport-service view
// 08 timed-stream view
// 0C dual-stream view
// 10 dual-thunk-or-file view
// 14 marker / fatal / embedded-stack anchor
// 18 slot-count/storage front
// 1C first record-table front
// 20 second record-table front
// 24 auxiliary storage front
static const VsNetCompositeViewOffsets g_NETWORK_GlobalStateCompositeTimedStreamOffsets = {
    0,      // owning timed-stream view
    0x40,   // transport-service view
    0x70,   // timed-stream body
    0xe8,   // dual-stream body
    0x140,  // dual-thunk front
    0x144,  // marker/fatal anchor
    0x15c,  // slot-count/storage front
    0x168,  // first record-table front
    0x17c,  // second record-table front
    0x190,  // auxiliary storage front
};
static const VsNetCompositeViewOffsets g_NETWORK_GlobalStateCompositeTransportOffsets = {
    -0x04,  // transport view back to owning header
    0x5c,   // timed-stream view
    0x8c,   // dual-stream view
    0x104,  // dual-thunk view
    0x15c,  // marker/fatal anchor
    0,      // transport-local payload slot
    0,      // transport-local payload slot
    0x40,   // initial-stream/timed-stream anchor
};
static const VsNetCompositeViewOffsets g_NETWORK_GlobalStateCompositeDualStreamOffsets = {
    -0x44,  // dual-stream view back to owner
    -0x74,  // transport view
    -0x04,  // timed-stream view
    0x5c,   // dual-stream body
    0x8c,   // dual-thunk front
    0x104,  // marker/fatal anchor
    0x15c,  // record-table-A slot
    0,      // no secondary record-table slot here
};
static const VsNetCompositeViewOffsets g_NETWORK_GlobalStateCompositeDualThunkOffsets = {
    -0x44,  // dual-thunk view back to owner
    -0xec,  // transport view
    -0x44,  // timed-stream view
    -0x74,  // dual-stream view
    -0x04,  // file-wrapper / thunk front itself
    0x5c,   // initial-stream anchor
    0x8c,   // peer-state payload slot
    0x104,  // record-table payload slot
};
static const VsNetConstructionAdjustorVtable g_NETWORK_GlobalStateCompositeEmbeddedStackOffsets = {
    -0x0c,
    -0x134,
    -0x8c,
    -0x30,
    -0x18,
    0,
    -0x04,
    -0x1c,
};
static const VsNetConstructionAdjustorVtable g_NETWORK_GlobalStateCompositeEmbeddedStackDataOffsets = {
    -0x0c,
    -0x148,
    -0x118,
    -0x44,
    -0x2c,
    0,
    -0x0c,
    -0x134,
};
/* Raw 0049A848: initial global-state stream table.  This is a six-slot
 * stream view; it is not the one-slot return-true table used during the
 * temporary construction phase. */
struct VsNetGlobalStateCompositeInitialVtableModel {
    virtual int ReturnTrue(void) { return 1; }
    virtual int ReturnTrueSecondary(void) { return 1; }
    virtual void Slot02(void) {}
    virtual void Noop(void) {}
    virtual void Slot04(void) {}
    virtual void *Delete(BYTE fDelete) {
        return ((VsNetEffStreamBase *)this)->DeleteEffStreamBaseWrapper(fDelete);
    }
};
static VsNetGlobalStateCompositeInitialVtableModel
    g_NETWORK_GlobalStateCompositeInitialVtableModel;
static void *g_NETWORK_GlobalStateCompositeInitialVtable =
    *(void ***)&g_NETWORK_GlobalStateCompositeInitialVtableModel;
/* Raw 0049AA90, 0049AA60, 0049AA38. */
static void *g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable[] = {
    (void *)AdjustAndReleaseCompositeFileBackedEffMarker,
    (void *)AdjustAndDeleteCompositeFileBackedEffChannelWrapper,
    (void *)AdjustAndClearEmbeddedFileBackedEffChannelAsyncErrorStatus,
    (void *)AdjustAndClearGlobalStateTimedEffTransportPendingWriteState,
};
static void *g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable[11] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReverseEffTransportPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteEffTransportTaggedHeader,
    (void *)AdjustAndDeleteGlobalStateCompositeAt30,
    (void *)AdjustAndWriteGlobalStateTimedEffStreamWithGlobalSession,
    (void *)AdjustAndClaimGlobalStateTimedEffTransportRecordPayload,
    (void *)AdjustAndInvokeGlobalStateTimedEffStreamServiceCallback,
    (void *)AppendAdjustedGlobalStateTimedFileBackedEffRecordSlot,
};
static void *g_NETWORK_GlobalStateCompositeInitialDualThunkVtable[10] = {
    (void *)ReturnTrueVtableCallbackThunk,
    (void *)ReturnTrueVtableCallbackSecondaryThunk,
    (void *)ReverseEffTransportPayload,
    (void *)NoopVtableCallbackThunk,
    (void *)WriteEffTransportTaggedHeader,
    (void *)AdjustAndDeleteGlobalStateCompositeAtA8,
    (void *)ReturnVoidVtableCallback,
    (void *)AdjustAndWriteGlobalStateDualEffStreamWithGlobalSession,
    (void *)AdjustAndClaimGlobalStateDualEffTransportRecordPayload,
    ((void **)g_NETWORK_TcpipSocketStackDualRet8)[0],
};
/* Raw table 0049A9D8: seven composite operations, fatal slot, then the
 * fifteen-slot Win32/file-status tail.  First seven bodies remain shared
 * code fragments; preserve their slots until their owning views are proven. */
static void *g_NETWORK_GlobalStateCompositeFinalVtableModel[] = {
    (void *)AllocateGlobalStateEffRecordSlot0047AD70,
    (void *)ReleaseGlobalStateEffRecordSlot0047AE00,
    (void *)NoopGlobalStateCompositeCallback0047AB10,
    (void *)PrepareGlobalStateBroadcastPath0047AB20,
    (void *)ServiceGlobalStateEffComposite0047AEF0,
    (void *)SetGlobalStateCompositeOpenFlag0047AED0,
    (void *)ClearGlobalStateCompositeOpenFlag0047AEE0,
    (void *)_purecall,
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
    (void *)WSAGetLastErrorThunk0047B8D0,
    (void *)GetWin32FileWrapperOwnedPath,
    0,
};
static void *g_NETWORK_GlobalStateCompositeFinalVtable =
    g_NETWORK_GlobalStateCompositeFinalVtableModel;
static void *g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable =
    g_NETWORK_GlobalStateFinalPrimaryThunkVtable;
static void *g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable =
    *(void ***)&g_NETWORK_GlobalStateFinalTimedThunkVtableModel;
static void *g_NETWORK_GlobalStateCompositeFinalDualThunkVtable =
    *(void ***)&g_NETWORK_GlobalStateFinalDualThunkVtableModel;
static void *g_NETWORK_GlobalStateCompositeFinalFatalFileWrapperVtable =
    g_NETWORK_GlobalStateFinalFatalVtable;

// Dual-slot composite layout uses the same view ordering as the global-state
// composite, but its initial stream starts 8 bytes later and it carries two
// separate record tables.
static const VsNetCompositeViewOffsets g_NETWORK_DualSlotCompositeTimedStreamOffsets = {
    0,      // owning timed-stream view
    0x38,   // transport-service view
    0x68,   // timed-stream body
    0xe0,   // dual-stream body
    0x138,  // dual-thunk front
    0x13c,  // marker/fatal anchor
    0x154,  // slot-count/storage front
    0x160,  // first record-table front
    0x174,  // second record-table front
    0x188,  // auxiliary storage front
};
static const VsNetCompositeViewOffsets g_NETWORK_DualSlotCompositeTransportOffsets = {
    -0x04,  // transport view back to owning header
    0x60,   // timed-stream view
    0x90,   // dual-stream view
    0x108,  // dual-thunk view
    0x160,  // marker/fatal anchor
    0,      // transport-local payload slot
    0,      // transport-local payload slot
    0x38,   // initial-stream/timed-stream anchor
};
static const VsNetCompositeViewOffsets g_NETWORK_DualSlotCompositeDualStreamOffsets = {
    -0x44,  // dual-stream view back to owner
    -0x74,  // transport view
    -0x04,  // timed-stream view
    0x60,   // dual-stream body
    0x90,   // dual-thunk front
    0x108,  // marker/fatal anchor
    0x160,  // record-table-A slot
    0,      // no secondary record-table slot here
};
static const VsNetCompositeViewOffsets g_NETWORK_DualSlotCompositeDualThunkOffsets = {
    -0x44,  // dual-thunk view back to owner
    -0xec,  // transport view
    -0x44,  // timed-stream view
    -0x74,  // dual-stream view
    -0x04,  // file-wrapper / thunk front itself
    0x60,   // initial-stream anchor
    0x90,   // record-table-A payload slot
    0x108,  // record-table-B payload slot
};
static const VsNetConstructionAdjustorVtable g_NETWORK_DualSlotCompositeEmbeddedStackOffsets = {
    -0x0c,
    -0x134,
    -0x8c,
    -0x30,
    0,
    -0x18,
    -0x04,
    -0x1c,
};
static const VsNetConstructionAdjustorVtable g_NETWORK_DualSlotCompositeEmbeddedStackDataOffsets = {
    -0x0c,
    -0x148,
    -0x118,
    -0x44,
    -0x2c,
    0,
    -0x0c,
    -0x134,
};
struct VsNetDualSlotFinalTimedThunkVtableModel
    : VsNetEmbeddedFileBackedFinalCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        return AdjustAndDeleteDualSlotTableTimedFileBackedEffCompositeWrapper(this, fDelete);
    }
    virtual void WriteGlobalSession(void) {
        AdjustAndWriteGlobalStateTimedEffStreamFrom158(this);
    }
    virtual void DispatchAck(void) {
        DispatchAdjustedAckedEffTransportPayloadEventFromEmbeddedThunk(this);
    }
    virtual void InvokeService(void *pArgument) {
        AdjustAndInvokeTimedEffStreamServiceFrom154(this, pArgument);
    }
    virtual int AppendRecord(const void *pvPayload, DWORD cbPayload) {
        return AppendAdjustedGlobalStateFileBackedEffRecordSlot(this, pvPayload, cbPayload);
    }
};
static VsNetDualSlotFinalTimedThunkVtableModel
    g_NETWORK_DualSlotFinalTimedThunkVtableModel;

struct VsNetDualSlotFinalDualThunkVtableModel
    : VsNetEmbeddedFileBackedFinalCommonThunkVtableModel {
    virtual void *Delete(BYTE fDelete) {
        return AdjustAndDeleteDualSlotTableDualFileBackedEffCompositeWrapper(this, fDelete);
    }
    virtual void QueueConnect(void) {
        QueueAdjustedEffTransportConnectEvent(this);
    }
    virtual void WriteGlobalSession(void) {
        AdjustAndWriteDualSlotTimedEffStreamWithGlobalSession(this);
    }
    virtual void DispatchAck(void) {
        DispatchAdjustedAckedEffTransportPayloadEventFromDualThunk(this);
    }
    virtual void QueuePayload(unsigned short nType, void *pPayload) {
        QueueAdjustedEffTransportPayloadEvent(this, nType, pPayload);
    }
};
static VsNetDualSlotFinalDualThunkVtableModel
    g_NETWORK_DualSlotFinalDualThunkVtableModel;

struct VsNetDualSlotWaveOutStopVtableModel {
    // FUNCTION: LEMBALL 0x0047CE00
    virtual int StopWaveOutPlayback(void) {
        HWAVEOUT *pWaveOut;
        MMRESULT nResult;
        unsigned int i;
        char szErrorText[0x100];

        pWaveOut = (HWAVEOUT *)((char *)this + 0x8c);
        if (*pWaveOut == 0) {
            return 1;
        }

        nResult = (MMRESULT)-1;
        i = 0;
        do {
            if (i >= 500) {
                break;
            }
            ++i;
            nResult = waveOutReset(*pWaveOut);
        } while (nResult != 0);

        if (i != 500) {
            return 1;
        }

        AppendCStringToStream(g_pErrorOutputStream,
                              "Error stopping playback in device : ");
        AppendCStringToStream(g_pErrorOutputStream,
            GetWaveOutEffectBackendDescription((VsSoundWaveOutEffectBackend *)this));
        AppendCStringToStream(g_pErrorOutputStream,
                              ".\nSystem may be unstable!\n");
        waveOutGetErrorTextA(nResult, szErrorText, sizeof(szErrorText));
        AppendCStringToStream(g_pErrorOutputStream, szErrorText);
        AppendCStringToStream(g_pErrorOutputStream, "\n");
        return 0;
    }
};
static VsNetDualSlotWaveOutStopVtableModel
    g_NETWORK_DualSlotWaveOutStopVtableModel;
static void *g_NETWORK_DualSlotWaveOutStopVtable =
    *(void ***)&g_NETWORK_DualSlotWaveOutStopVtableModel;

/* 49AC08 is the primary compiler vtable.  49AC18 is its +4-view: the
 * composite's public methods begin after the four owner/thunk entries. */
static void *g_NETWORK_DualSlotCompositeFullVtableModel[] = {
    (void *)AdjustAndReleaseCompositeFileBackedEffMarker,
    (void *)AdjustAndDeleteDualSlotTableFileBackedEffCompositeWrapper,
    (void *)AdjustAndClearEmbeddedEffChannelAsyncStatus13c,
    (void *)AdjustAndCompleteDualSlotEffTransportPendingWrite0047BFB0,
    (void *)SetDualSlotCompositePathPair0047B3B0,
    (void *)ServiceDualSlotCompositeRecords0047B990,
    (void *)ConfigureDualSlotCompositeFilePath0047B440,
    (void *)PrepareDualSlotComposite0047B4D0,
    (void *)ActivateDualSlotComposite0047B540,
    (void *)InitializeDualSlotCompositeStorage0047B580,
    0,
    0,
    (void *)DeleteWaveOutEffectBackend,
    (void *)GetWaveOutEffectBackendDescription,
    (void *)GetWaveOutEffectBackendStatus0047CC10,
    (void *)InitializeWaveOutEffectDevice0047CB00,
    (void *)IsWaveOutEffectBackendAvailable,
    (void *)ResetAndCloseWaveOutEffectDevice0047CC20,
    (void *)IsWaveOutEffectInstanceAvailable0047CDD0,
    (void *)ReturnOneWaveOutBackendValue0047CDF0,
    ((void **)g_NETWORK_DualSlotWaveOutStopVtable)[0],
};
static void *g_NETWORK_DualSlotCompositeFinalVtable =
    g_NETWORK_DualSlotCompositeFullVtableModel + 4;
static void *g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable =
    g_NETWORK_DualSlotCompositeFullVtableModel;
static void *g_NETWORK_DualSlotCompositeFinalTimedThunkVtable =
    *(void ***)&g_NETWORK_DualSlotFinalTimedThunkVtableModel;
static void *g_NETWORK_DualSlotCompositeFinalDualThunkVtable =
    *(void ***)&g_NETWORK_DualSlotFinalDualThunkVtableModel;
static void *g_NETWORK_DualSlotCompositeFinalFatalFileWrapperVtable[] = {
    (void *)AdjustAndWriteDualSlotTableFileBackedEffHeader,
    0,
};

static void *g_NETWORK_CrtFatalRuntimeErrorThunkSlots[14] = {
    (void *)_purecall, (void *)_purecall,
    (void *)_purecall, (void *)_purecall,
    (void *)_purecall, (void *)_purecall,
    (void *)_purecall, (void *)_purecall,
    (void *)_purecall, (void *)_purecall,
    (void *)_purecall, (void *)_purecall,
    (void *)_purecall,
    0,
};
static void *g_NETWORK_CrtFatalRuntimeErrorThunk = g_NETWORK_CrtFatalRuntimeErrorThunkSlots;
static void *g_NETWORK_RuntimeChannelStackFatalThunkVtable = g_NETWORK_CrtFatalRuntimeErrorThunkSlots;
static void *g_NETWORK_MapNestedEffCallbackFailureThunkVtable[] = {
    0,
    (void *)DeleteEffChannelStateSideBufferWrapper,
    (void *)ClearNestedEffChannelAsyncStatus,
    (void *)EffStreamChannelStateRet4Thunk,
};

struct VsNetCrtFatalRuntimeErrorAdjustor {
    void (*m_pFatal)(void);
    int m_nViewOffset04;
    int m_nViewOffset08;
    int m_nViewOffset0c;
    int m_nViewOffset10;
    int m_nViewOffset14;
    int m_nViewOffset18;
    int m_nViewOffset1c;
    int m_nViewOffset20;
    int m_nViewOffset24;
};

static const VsNetCrtFatalRuntimeErrorAdjustor g_NETWORK_CrtFatalRuntimeErrorDeleteAdjustor = {
    (void (*)(void))_purecall,
    -0x04,
    -0x18,
    -0x9c,
    0,
    -0x44,
    -0x70,
    -0x0c,
    -0x0c,
    0x0c,
};
static void *g_NETWORK_CrtFatalRuntimeErrorDeleteThunk =
    (void *)&g_NETWORK_CrtFatalRuntimeErrorDeleteAdjustor;

/* Uses the common stream core.  Marker data begins at +0x2c, so this is not
 * a GameEffStream-derived object. */
class VsNetEffStreamRecordSlot : public VsNetEffStreamCore {
public:
    unsigned short m_wCommittedMarker;
    unsigned short m_wObservedMarker;
    DWORD m_cbPayload;
    char m_szSourceName[0x15];
    char m_szTargetName[0x17];

    VsNetEffStreamRecordSlot *Initialize(void);
    void Load(void);
    void Save(void);
    void *Delete(BYTE fDeleteFlags);
};

struct VsNetConstructionAdjustorVtable;
struct VsNetFileBackedDispatchOffsets;

class VsNetEffStreamRecordSlotTable {
public:
    void *m_pVtable;
    int m_nRecordHeaderOffset04;
    int m_nPayloadBaseOffset08;
    VsNetFileBackedDispatchOffsets *m_pChannelThunk0c;
    int m_nPendingSlot10;
    int m_nReserved14;
    int m_nAccumulatedStreamLength18;
    int m_nSaveRangeCursor1c;
    int m_nLoadSourceCursor20;
    int m_nReserved24;
    int m_nReserved28;
    VsNetEffStreamRecordSlot *m_pSlots;
    unsigned short *m_pwSlotFlags;
    int m_cSlots;
    int m_nCurrentChangedSlot38;

    VsNetEffStreamRecordSlotTable *Construct(int cSlots);
    void Destroy();
    void Load(void);
    void Save(void);
    VsNetEffStreamRecordSlot *FindNextChangedSlot(void);
    void *Delete(BYTE fDeleteFlags);
};

static const VsNetFileBackedChannelViewOffsets g_NETWORK_FileBackedDualChannelStateOffsets = {
    -0x0c,
    0x0c,
    0x38,
    0x90,
    0xa4,
    (void (*)(void))_purecall,
    (void (*)(void))OpenWin32FileWrapperWithCreateFlag,
    (void (*)(void))OpenWin32FileWrapperCreateNew,
};

static const VsNetFileWrapperThunkOffsets g_NETWORK_FileBackedDualFileWrapperThunkOffsets = {
    -0x04,
    -0x18,
    -0x9c,
    0,
    -0x44,
    -0x70,
    -0x0c,
    0x0c,
};

static const VsNetFileBackedChannelViewOffsets g_NETWORK_FileBackedTimedChannelStateOffsets = {
    -0x0c,
    0x0c,
    0x3c,
    0xb0,
    0xc4,
    (void (*)(void))_purecall,
    (void (*)(void))OpenWin32FileWrapperWithCreateFlag,
    (void (*)(void))OpenWin32FileWrapperCreateNew,
};

static const VsNetFileWrapperThunkOffsets g_NETWORK_FileBackedTimedFileWrapperThunkOffsets = {
    -0x04,
    -0x18,
    -0xbc,
    0,
    -0x44,
    -0x74,
    -0x0c,
    0x0c,
};

static const VsNetEmbeddedStackFileViewOffsets g_NETWORK_EmbeddedFileBackedFileWrapperThunkOffsets = {
    -0x04,
    -0x18,
    -0x11c,
    0,
    0,
    -0x100,
    -0xd0,
    -0x58,
};

static const VsNetEmbeddedStackViewOffsets g_NETWORK_EmbeddedFileBackedDualThunkOffsets = {
    0,
    -0x100,
    -0xd0,
    -0x58,
    -0x44,
    -0xec,
    -0x44,
    -0x74,
};

static const VsNetEmbeddedStackViewOffsets g_NETWORK_EmbeddedFileBackedFinalThunkOffsets = {
    0,
    0x08,
    0x38,
    0xb0,
    0x108,
    0x10c,
    0x120,
    0x12c,
};

static const VsNetEmbeddedStackViewOffsets g_NETWORK_GlobalStateCompositeFileWrapperOffsets = {
    0,
    -0x100,
    -0xd0,
    -0x58,
    -0x44,
    -0xec,
    -0x44,
    -0x74,
};

static const VsNetEmbeddedStackFileViewOffsets g_NETWORK_GlobalStateCompositeTimedFileThunkOffsets = {
    -0x04,
    -0x1c,
    -0x120,
    0,
    0,
    -0x100,
    -0xd0,
    -0x58,
};

static const VsNetEmbeddedStackViewOffsets g_NETWORK_GlobalStateCompositeEmbeddedStackFinalOffsets = {
    0,
    -0x150,
    -0x120,
    -0xa8,
    -0x50,
    -0x4c,
    -0x34,
    -0x28,
};

static const VsNetEmbeddedStackViewOffsets g_NETWORK_DualSlotCompositeFileWrapperOffsets = {
    0,
    -0x100,
    -0xd0,
    -0x58,
    -0x44,
    -0xec,
    -0x44,
    -0x74,
};

static const VsNetEmbeddedStackFileViewOffsets g_NETWORK_DualSlotCompositeTimedFileThunkOffsets = {
    -0x04,
    -0x1c,
    -0x120,
    0,
    0,
    -0x100,
    -0xd0,
    -0x58,
};

static const VsNetEmbeddedStackViewOffsets g_NETWORK_DualSlotCompositeEmbeddedStackFinalOffsets = {
    0,
    -0x150,
    -0x120,
    -0xa8,
    -0x50,
    -0x4c,
    -0x34,
    -0x28,
};

struct VsNetFileBackedDispatchOffsets {
    void *m_pReserved00;
    int m_nChannelStateViewOffset04;
    int m_nTimedStreamViewOffset08;
    int m_nFileWrapperViewOffset0c;
    int m_nSlotCursorStateOffset10;
    int m_nReadThunkOffset14;
    int m_nSlotCountViewOffset18;
    int m_nWriteThunkOffset20;
};

struct VsNetRecordWriterAdjustorOffsets {
    int m_nSelfDelta00;
    int m_nReserved04;
    int m_nReserved08;
    int m_nReserved0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nRecordTableViewOffset18;
    int m_nRecordWriterViewOffset1c;
};

struct VsNetGlobalSessionAdjustorOffsets {
    int m_nSelfDelta00;
    int m_nReserved04;
    int m_nEmbeddedStreamViewOffset08;
    int m_nReserved0c;
    int m_nTransportViewOffset10;
};

struct VsNetGlobalSessionAdjustorFront {
    VsNetGlobalSessionAdjustorOffsets *m_pOffsets;
    char m_abReserved04[0x34];
    void WriteAdjustedEffStreamWithGlobalSession(void);
};

struct VsNetAdjustorThunkHeader {
    int m_nThisDelta;
};

struct VsNetFileBackedMarkerOffsets {
    void *m_pReserved00;
    int m_nReserved04;
    int m_nReserved08;
    int m_nReserved0c;
    int m_nReadThunkOffset14;
    int m_nNestedThunkOffset18;
    int m_nStreamOffset1c;
    int m_nWriteThunkOffset20;
};

struct VsNetFileBackedAppendOffsets {
    void *m_pReserved00;
    int m_nOutputStateOffset04;
    int m_nNameProviderOffset08;
    int m_nFileWrapperOffset0c;
};

struct VsNetAdjustorSubobject {
    int m_nThisDelta;
    void *m_pVtable;
};

struct VsNetAdjustedTimedEffStreamServiceView {
    void InvokeAdjustedTimedEffStreamServiceCallback(void *pArgument);
};

struct VsNetThunkAdjustorGroup {
    VsNetConstructionAdjustorVtable *m_pOffsets;
    VsNetAdjustorSubobject *m_pPrimary;
    VsNetAdjustorSubobject *m_pSecondary;
    VsNetAdjustorSubobject *m_pTertiary;
};

struct VsNetQuadThunkAdjustorGroup {
    VsNetConstructionAdjustorVtable *m_pOffsets;
    VsNetAdjustorSubobject *m_pPrimary;
    VsNetAdjustorSubobject *m_pSecondary;
    VsNetAdjustorSubobject *m_pTertiary;
    VsNetAdjustorSubobject *m_pQuaternary;
};

struct VsNetQuintThunkAdjustorGroup {
    VsNetConstructionAdjustorVtable *m_pOffsets;
    VsNetAdjustorSubobject *m_pPrimary;
    VsNetAdjustorSubobject *m_pSecondary;
    VsNetAdjustorSubobject *m_pTertiary;
    VsNetAdjustorSubobject *m_pQuaternary;
    VsNetAdjustorSubobject *m_pFatal;
};

struct VsNetEffTransportRuntimeServiceVtable {
    char *(*m_pGetLocalHostName)(void *pThis);
    void *m_pReserved04;
    void (*m_pSelectLocalHostName)(void *pThis, char *pszHostName);
};

struct VsNetEffTransportRuntimeService {
    VsNetEffTransportRuntimeServiceVtable *m_pVtable;
};

struct VsNetFileWrapperVtable {
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

struct VsNetFileWrapperObject {
    VsNetFileWrapperVtable *m_pVtable;
};

class VsNetWin32FileWrapper {
public:
    int OpenWithCreateFlag(LPCSTR pszPath, int nAccessMode, int fCreateNew);
    int OpenCreateNew(LPCSTR pszPath, int nAccessMode);
    int ProbeAccess(LPCSTR pszPath);
    int CloseIfOpen(void);
    void FreeOwnedPath(void);
    void *GetOwnedPath(void);
    int Seek(DWORD dwOffset);
    DWORD GetLength(void);
    DWORD GetOffset(void);
    int LockRange(DWORD dwOffset, DWORD cbRange);
    int UnlockRange(DWORD dwOffset, DWORD cbRange);
    int Write(const void *pvSource, DWORD cbWrite);
    int Read(LPVOID pvTarget, DWORD cbRead);
};

struct VsNetFileWrapperOpenObject {
    VsNetFileWrapperVtable *m_pVtable;
};

struct VsNetInlineOpenWin32FileWrapper {
    void *m_pVtable;
    int m_nReserved04;
    int m_fOpen08;
    int m_nReserved0c;
    int m_nReserved10;
    void *m_pDeleteThunk14;
};

struct VsNetWin32FileWrapperPathOwner {
    void *m_pVtable;
    char *m_pszOwnedPath04;
};

struct VsNetCallbackThunkVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void *m_pReserved08;
    int (*m_pInvoke)(void *pThis);
    void (*m_pRelease)(void *pThis);
};

struct VsNetCallbackThunkObject {
    VsNetCallbackThunkVtable *m_pVtable;
};

struct VsNetDeleteVtable {
    void *m_apReserved00[5];
    void (*m_pDelete)(void *pThis, int nDeleteMode);
};

struct VsNetDeleteObject {
    VsNetDeleteVtable *m_pVtable;
};

struct VsNetChannelStateHeader {
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

struct VsNetPendingRecordChannelStateHeader {
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
    int m_nReserved24;
    int m_fChannelOpen28;
};

struct VsNetPeerAddressServiceVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void (*m_pSelectPeerName)(void *pThis, char *pszName);
};

struct VsNetPeerAddressService {
    VsNetPeerAddressServiceVtable *m_pVtable;
};

/* Partial timed-stream view used inside file-backed composites.  The real
 * +0x44 construction-offset pointer is stored by the enclosing object; the
 * full channel-state tail overlaps adjacent composite storage. */
struct VsNetTimedStreamHeader : VsNetEffStreamCore {
    int *m_pTagBuffer2c;
    unsigned short m_nWord30;
    unsigned short m_nWord32;
    unsigned short m_nWord34;
    unsigned short m_nWord36;
    int m_nReserved38;
    DWORD m_dwLastTick3c;
};

struct VsNetFileBackedTimedChannelWrapperView {
    VsNetEffStreamChannelState m_ChannelState00;
    char m_abChannelStateTail2c[4];
    VsNetTimedStreamHeader m_TimedStream30;
    char m_abReserved70[0x34];
    void *m_pResetThunkA4;
    void *m_pSideBufferA8;
    void *DeleteTimedFileBackedEffChannelWrapper(BYTE fFreeMemory);
};

struct VsNetFileBackedCompositeWrapperView {
    VsNetEffStreamChannelState m_ChannelState00;
    char m_abChannelStateTail2c[4];
    VsNetTimedStreamHeader m_TimedStream30;
    char m_abReserved70[0x38];
    char m_abDualStreamA8[0x58];
    char m_abReserved100[4];
    void *m_pResetThunk104;
    void *m_pSideBuffer108;
    void *DeleteEmbeddedFileBackedEffChannelStackWrapper(BYTE fFreeMemory);
    void *DeleteCompositeFileBackedEffChannelWrapper(BYTE fFreeMemory);
    void *DeleteDualSlotTableFileBackedEffCompositeWrapper(BYTE fFreeMemory);
};

struct VsNetEmbeddedFileBackedEffChannelStackWrapper {
    char m_abOwnerPrefix00[0x08];
    VsNetFileBackedCompositeWrapperView m_Runtime08;
};

struct VsNetFileBackedCompositeChannelWrapper {
    char m_abOwnerPrefix00[0x38];
    VsNetFileBackedCompositeWrapperView m_Runtime38;
};

struct VsNetGlobalStateFileBackedEffCompositeWrapper {
    char m_abOwnerPrefix00[0x60];
    VsNetFileBackedCompositeWrapperView m_Runtime60;
};

struct VsNetDualSlotTableFileBackedEffCompositeWrapper {
    char m_abOwnerPrefix00[100];
    VsNetFileBackedCompositeWrapperView m_Runtime64;
};

struct VsNetDualFileBackedEffChannelConstructorView {
    char m_abReserved00[0x18];
    VsNetEffStreamChannelState m_ChannelState18;
    char m_abDualHandleStream44[0x58];
    VsNetInlineOpenWin32FileWrapper m_FileWrapper9c;
    void *ConstructDualFileBackedEffChannel(int fConstructEmbeddedObjects);
};

struct VsNetTimedFileBackedEffChannelConstructorView {
    char m_abReserved00[0x18];
    VsNetEffStreamChannelState m_ChannelState18;
    char m_abChannelStateTail44[4];
    VsNetTimedStreamHeader m_TimedStream48;
    char m_abReserved88[0x34];
    VsNetInlineOpenWin32FileWrapper m_FileWrapperbc;
    void *ConstructTimedFileBackedEffChannel(int fConstructEmbeddedObjects);
};

struct VsNetEmbeddedFileBackedEffChannelStackConstructorView {
    char m_abReserved00[0x08];
    VsNetEffStreamChannelState m_ChannelState08;
    char m_abChannelStateTail34[4];
    VsNetTimedStreamHeader m_TimedStream38;
    char m_abReserved78[0x38];
    char m_abDualHandleStreamB0[0x58];
    VsNetInlineOpenWin32FileWrapper m_FileWrapper10c;
    char m_abReserved124[0x08];
    char m_abDualFileChannel12c[0xA4];
    char m_abTimedFileChannel1d0[0xD8];
    void *ConstructEmbeddedFileBackedEffChannelStack(int fConstructEmbeddedObjects);
};

struct VsNetFileBackedCompositeInitialStream {
    void *m_pVtable;
    int m_nReserved04;
    char m_abState08[0x10];
    int m_nSerializedLength18;
    char m_abReserved1c[0x0e];
    unsigned short m_wLatch2a;
    unsigned short m_wFlags2c;
    unsigned short m_wReserved2e;
};

class VsNetEff512ByteStateStream : public VsNetEffStreamCore {
public:
    unsigned char *m_pbStateBytes2c;

    VsNetEff512ByteStateStream *Construct();
    void Load(void);
    void Save(void);
    void *Delete(BYTE fDeleteFlags);
};

// FUNCTION: LEMBALL 0x00479860
int LEMBALL_FASTCALL HasNonzeroEff512ByteState(int nObject) {
    VsNetEff512ByteStateStream *pStream;
    int i;

    pStream = (VsNetEff512ByteStateStream *)(unsigned long)nObject;
    i = 0;
    do {
        if (pStream->m_pbStateBytes2c[i] != 0) {
            return 1;
        }
        ++i;
    } while (i < 0x200);
    return 0;
}

struct VsNetGlobalStateCompositeLayout {
    VsNetCompositeViewOffsets *m_pTransportOffsets00;
    char m_abReserved04[0x0c];
    unsigned char *m_pbGlobalStateSlotFlags10;
    char m_abReserved14[4];
    DWORD m_dwGlobalStateMarkerTick18;
    char m_abReserved1c[4];
    VsNetCompositeViewOffsets *m_pTimedStreamOffsets20;
    VsNetFileBackedCompositeInitialStream m_InitialStream24;
    int m_fGlobalStateStreamLoaded54;
    DWORD m_dwGlobalStateServiceTick58;
    char m_abReserved5c[4];
    VsNetEffStreamChannelState m_ChannelState60;
    char m_abChannelStateTail8c[4];
    char m_abTimedStream90[0x78];
    char m_abDualStream108[0x58];
    VsNetCompositeViewOffsets *m_pDualThunkOffsets14c;
    char m_abReserved150[0x10];
    const VsNetEmbeddedStackViewOffsets *m_pEmbeddedStackTransportOffsets160;
    char m_abReserved164[0x1c];
    const VsNetEmbeddedStackFileViewOffsets *m_pEmbeddedStackFileViewOffsets180;
    char m_abReserved184[0x10];
    const VsNetEmbeddedStackViewOffsets *m_pEmbeddedStackTimedStreamOffsets194;
    char m_abReserved198[0x10];
    const VsNetEmbeddedStackFileViewOffsets *m_pEmbeddedStackDualStreamOffsets1a8;

    void *ConstructGlobalStateFileBackedEffComposite(int fConstructEmbeddedObjects);
};

struct VsNetDualSlotCompositeLayout {
    VsNetCompositeViewOffsets *m_pTransportOffsets00;
    char m_abReserved04[0x28];
    VsNetCompositeViewOffsets *m_pTimedStreamOffsets2c;
    VsNetFileBackedCompositeInitialStream m_InitialStream30;
    char m_abReserved60[4];
    VsNetEffStreamChannelState m_ChannelState64;
    char m_abChannelStateTail90[4];
    char m_abTimedStream94[0x78];
    char m_abDualStream10c[0x44];
    VsNetCompositeViewOffsets *m_pDualThunkOffsets150;
    char m_abReserved154[0x10];
    const VsNetEmbeddedStackViewOffsets *m_pEmbeddedStackTransportOffsets164;
    char m_abReserved168[0x1c];
    const VsNetEmbeddedStackFileViewOffsets *m_pEmbeddedStackFileViewOffsets184;
    char m_abReserved188[0x10];
    const VsNetEmbeddedStackViewOffsets *m_pEmbeddedStackTimedStreamOffsets198;
    char m_abReserved19c[0x10];
    const VsNetEmbeddedStackFileViewOffsets *m_pEmbeddedStackDualStreamOffsets1ac;

    void *ConstructDualSlotTableFileBackedEffComposite(int fConstructEmbeddedObjects);
};

struct VsNetGlobalStateCompositeDestroyView {
    void DestroyGlobalStateFileBackedEffComposite(void);
};

struct VsNetDualSlotCompositeDestroyView {
    void DestroyDualSlotTableFileBackedEffComposite(void);
};

struct VsNetGlobalStateCompositeTransportFront {
    VsNetCompositeViewOffsets *m_pViewOffsets00;
    VsNetFileBackedCompositeInitialStream m_InitialStream04;
};

struct VsNetDualSlotCompositeTransportFront {
    VsNetCompositeViewOffsets *m_pViewOffsets00;
    VsNetFileBackedCompositeInitialStream m_InitialStream04;
};

struct VsNetEmbeddedFileBackedStackOpenWrapperFront {
    char m_abReserved00[0x10c];
    VsNetInlineOpenWin32FileWrapper m_FileWrapper10c;
    const VsNetEmbeddedStackFileViewOffsets *m_pFileWrapperThunk124;
};

struct VsNetGlobalStateCompositeOpenWrapperFront {
    char m_abReserved00[0x164];
    VsNetInlineOpenWin32FileWrapper m_FileWrapper164;
    const VsNetEmbeddedStackFileViewOffsets *m_pFileWrapperThunk180;
};

struct VsNetDualSlotCompositeOpenWrapperFront {
    char m_abReserved00[0x168];
    VsNetInlineOpenWin32FileWrapper m_FileWrapper168;
    const VsNetEmbeddedStackFileViewOffsets *m_pFileWrapperThunk184;
};

struct VsNetEmbeddedFileBackedStackFront {
    const VsNetEmbeddedStackViewOffsets *m_pFinalThunkOffsets;
    char m_abReserved04[0x78];
    const VsNetEmbeddedStackViewOffsets *m_pTimedStreamOffsets7c;
    char m_abReserved80[0x74];
    const VsNetEmbeddedStackViewOffsets *m_pDualStreamOffsetsf4;
    char m_abReservedf8[0x10];
    const VsNetEmbeddedStackViewOffsets *m_pDualThunkOffsets108;
    VsNetInlineOpenWin32FileWrapper m_FileWrapper10c;
    char m_abReserved124To138[0x14];
    const VsNetEmbeddedStackFileViewOffsets *m_pPrimaryThunkOffsets138;
    char m_abReserved13c[0x10];
    const VsNetEmbeddedStackFileViewOffsets *m_pTimedFileViewOffsets14c;
};

struct VsNetDualFileBackedEffChannelFront {
    char m_abReserved00[0x0c];
    VsNetFileBackedChannelViewOffsets *m_pThunk0c;
    int m_nMode10;
    char m_abReserved14[0x74];
    const VsNetFileBackedStreamAdjustorOffsets *m_pDualStreamOffsets88;
    char m_abReserved8c[0x28];
    VsNetFileWrapperThunkOffsets *m_pFileThunkb4;
    char m_abReservedb8[0x04];
    VsNetInlineOpenWin32FileWrapper m_FileWrapper9c;
};

struct VsNetTimedFileBackedEffChannelFront {
    char m_abReserved00[0x0c];
    VsNetFileBackedChannelViewOffsets *m_pThunk0c;
    int m_nMode10;
    char m_abReserved14[0x78];
    const VsNetFileBackedStreamAdjustorOffsets *m_pTimedStreamOffsets8c;
    char m_abReserved90[0x44];
    VsNetFileWrapperThunkOffsets *m_pFileThunkd4;
    char m_abReservedd8[0x04];
    VsNetInlineOpenWin32FileWrapper m_FileWrapperbc;
};

struct VsNetTimedFileBackedEffChannelVtableRestoreView {
    void RestoreTimedFileBackedEffChannelVtables(void);
};

struct VsNetChannelStateRuntimeVtable {
    void *m_pReserved00;
    void *m_pReserved04;
    void (*m_pHandleReadFailure)(void *pThis);
};

struct VsNetTimedStreamRuntimeVtable {
    void *m_apReserved00[6];
    void (*m_pServiceLoadedPacket)(void *pThis);
};

struct VsNetFileBackedEffRecordCursor {
    void **m_pVtable;
    int m_nRecordHeaderOffset04;
    int m_nPayloadBaseOffset08;
    VsNetAdjustorSubobject m_ChannelStateThunk0c;
    VsNetAdjustorSubobject m_TimedStreamThunk1c;
    VsNetAdjustorSubobject m_FileWrapperThunk2c;
    int m_nCurrentSlot3c;
};

struct VsNetRecordWriterAdjustorFront {
    const VsNetRecordWriterAdjustorOffsets *m_pOffsets;
    char m_abReserved04[0x34];
};

struct VsNetFileBackedMarkerStream {
    int m_anReserved00[11];
    unsigned short m_wReferenceCount2c;
    unsigned short m_wReserved2e;
};

struct VsNetFileBackedMarkerState {
    void **m_pVtable;
    VsNetFileBackedMarkerStream m_Stream04;
};

struct VsNetFileBackedMarkerOwner {
    VsNetFileBackedMarkerOffsets *m_pOffsets;
    VsNetFileBackedMarkerState m_Marker04;

    DWORD IncrementFileBackedEffStreamMarker(void);
    DWORD DecrementFileBackedEffStreamMarker(void);
};

struct VsNetFileBackedMarkerReleaseFront {
    char m_abReserved00[0x38];
};

struct VsNetFileBackedCallbackStatusFront {
    char m_abReserved00[0x1c];
};

struct VsNetFileBackedAppendOwner {
    VsNetFileBackedAppendOffsets *m_pOffsets;
    char m_abReserved04[0x18];
};

struct VsNetFileBackedOpenRetryVtable {
    void (*m_pResetOrClose)(void *pThis);
};

struct VsNetFileBackedOpenRetryOwner {
    VsNetFileBackedOpenRetryVtable *m_pVtable;
    VsNetFileBackedAppendOffsets *m_pOffsets;
};

struct VsNetGlobalStateFileBackedStorageOwner {
    VsNetFileBackedDispatchOffsets *m_pOffsets;
    VsNetFileBackedCompositeInitialStream m_InitialStream04;
};

struct VsNetFileBackedRecordServiceView {
    VsNetEffStreamRecordSlotTable *m_pRecordTable;
    VsNetPendingRecordChannelStateHeader *m_pChannelState;
    VsNetWin32FileWrapper *m_pFileWrapper;
};

struct VsNetFileBackedRecordPayloadView {
    VsNetEffStreamRecordSlotTable *m_pRecordTable;
    VsNetChannelStateHeader *m_pChannelState;
    VsNetTimedStreamHeader *m_pTimedStream;
    VsNetFileWrapperObject *m_pFileWrapper;
};

struct VsNetFileBackedRangeLoadView {
    VsNetEffStreamRecordSlotTable *m_pRecordTable;
    VsNetFileWrapperObject *m_pFileWrapper;
};

struct VsNetEffStreamSerializedLengthView {
    char m_abReserved00[0x18];
    DWORD m_cbSerialized18;
};

struct VsNetSlotCountHolder {
    char m_abReserved00[8];
    int m_cSlots;
};

struct VsNetStreamNameProviderVtable {
    char *(*m_pGetName)(void *pThis);
};

struct VsNetStreamNameProvider {
    VsNetStreamNameProviderVtable *m_pVtable;
};

struct VsNetFileBackedAppendNameProviderState {
    DWORD m_dwLastWriteTime00;
    char m_abReserved04[0x30];
    VsNetStreamNameProvider m_NameProvider34;
};

struct VsNetSideBufferOwnerA4 {
    char m_abReserved00[0xa4];
    void *m_pResetThunk;
    void *m_pSideBuffer;
};

struct VsNetSideBufferOwner104 {
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

extern void *ConstructDualHandleEffStream(void *pStream, int fConstructChannelState);
struct VsNetCompositeEffTransportStackLayout {
    void *ConstructCompositeEffTransportStack(int fConstructEmbeddedObjects);
};
extern void RestoreCompositeEffTransportVtables(int nObjectBasePlus0x30);
struct VsNetFileBackedLockedWriterView {
    DWORD WriteEffStreamToLockedFile(int *pStream, int fKeepLock, int fFlush);
};

/* Compiler-overlapped view used by composite timed/embedded channel storage.
 * First word is stream pointer for locked-writer calls; +0x0c is shared
 * dispatch-offset metadata.  Do not treat this as a standalone allocation. */
struct VsNetFileBackedLockedWriterStorageView {
    int *m_pStream00;
    char m_abReserved04[8];
    VsNetFileBackedDispatchOffsets *m_pDispatchOffsets0c;
};
struct VsNetRuntimeWindowBase {
    void ScheduleNetworkRuntimeTimerEvent(unsigned int nMilliseconds);
};
struct GameEffStream {
    void ResetStateFields(void);
    int LoadEffStreamFromMemory(int nSourceBuffer);
    void BeginEffStreamWriteSession(void);
    void EndEffStreamWriteSession(void);
};
struct VsNetEffTransportPacketProcessor {
    int ProcessEffTransportPacketHeader(void);
};

extern "C" DWORD WINAPI timeGetTime(void);

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
void LEMBALL_FASTCALL InitializeGlobalStateFileBackedEffStorage(int nObjectBasePlus0x15c);
VsNetEffStreamRecordSlot *FindNextChangedEffStreamRecordSlot(void *pObject);
void LoadEffStreamRecordSlot(void *pObject);
void SaveEffStreamRecordSlot(void *pObject);
void *DeleteEffStreamRecordSlot(void *pObject, BYTE fDeleteFlags);
void LoadEffStreamRecordSlotTable(void *pObject);
void SaveEffStreamRecordSlotTable(void *pObject);
void *DeleteEffStreamRecordSlotTable(void *pObject, BYTE fDeleteFlags);
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

/* Raw 0049A930: global-composite file-wrapper view.  It extends ordinary
 * file-wrapper dispatch with Winsock status and path access slots. */
static void *g_NETWORK_FileBackedFinalFileWrapperVtable[] = {
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
    (void *)WSAGetLastErrorThunk0047B8D0,
    (void *)GetWin32FileWrapperOwnedPath,
    0,
};

// GLOBAL: LEMBALL 0x0049A4A8
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

// GLOBAL: LEMBALL 0x0049A4D8
static void *g_NETWORK_EffStateStreamVtable[] = {
    (void *)ReturnTrueVtableCallback,
    (void *)ReturnTrueVtableCallbackSecondary,
    (void *)LoadEff512ByteStatePayload,
    (void *)NoopVtableCallback,
    (void *)WriteEff512ByteStatePayload,
    (void *)DeleteEff512ByteStateStream,
};

// FUNCTION: LEMBALL 0x0047F5B0
int VsNetWin32FileWrapper::OpenWithCreateFlag(
    LPCSTR pszPath, int nAccessMode, int fCreateNew) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;
    HANDLE hFile;
    DWORD dwCreationDisposition;
    size_t cchPath;
    char *pszCopy;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
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

int OpenWin32FileWrapperWithCreateFlag(void *pObject, LPCSTR pszPath,
                                       int nAccessMode, int fCreateNew) {
    return ((VsNetWin32FileWrapper *)pObject)
        ->OpenWithCreateFlag(pszPath, nAccessMode, fCreateNew);
}

// FUNCTION: LEMBALL 0x0047F640
int VsNetWin32FileWrapper::OpenCreateNew(LPCSTR pszPath, int nAccessMode) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    pWrapper->m_nReserved10 = 0;
    return OpenWin32FileWrapperWithCreateFlag(this, pszPath, nAccessMode, 1);
}

int OpenWin32FileWrapperCreateNew(void *pObject, LPCSTR pszPath, int nAccessMode) {
    return ((VsNetWin32FileWrapper *)pObject)
        ->OpenCreateNew(pszPath, nAccessMode);
}

// FUNCTION: LEMBALL 0x0047F680
int VsNetWin32FileWrapper::ProbeAccess(LPCSTR pszPath) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;
    HANDLE hFile;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    hFile = CreateFileA(pszPath, 0xc0000000, 3, (LPSECURITY_ATTRIBUTES)0, OPEN_EXISTING, 0x80, (HANDLE)0);
    pWrapper->m_nReserved0c = (int)(unsigned long)hFile;
    if (hFile == (HANDLE)-1) {
        return 0;
    }

    CloseHandle(hFile);
    return 1;
}

int ProbeWin32FileWrapperAccess(void *pObject, LPCSTR pszPath) {
    return ((VsNetWin32FileWrapper *)pObject)->ProbeAccess(pszPath);
}

// FUNCTION: LEMBALL 0x0047F8C0
int VsNetWin32FileWrapper::CloseIfOpen(void) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    if (pWrapper->m_fOpen08 == 0 && CloseHandle((HANDLE)(unsigned long)pWrapper->m_nReserved0c) != 0) {
        pWrapper->m_fOpen08 = 1;
    }
    return 1;
}

int CloseWin32FileWrapperIfOpen(void *pObject) {
    return ((VsNetWin32FileWrapper *)pObject)->CloseIfOpen();
}

// FUNCTION: LEMBALL 0x0047F660
void VsNetWin32FileWrapper::FreeOwnedPath(void) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    FreeVSMemBlock((void *)(unsigned long)pWrapper->m_nReserved04);
}

void FreeWin32FileWrapperOwnedPath(void *pObject) {
    ((VsNetWin32FileWrapper *)pObject)->FreeOwnedPath();
}

// FUNCTION: LEMBALL 0x0047B8E0
void *VsNetWin32FileWrapper::GetOwnedPath(void) {
    return *(void **)((char *)this + 4);
}

void *GetWin32FileWrapperOwnedPath(void *pObject) {
    return ((VsNetWin32FileWrapper *)pObject)->GetOwnedPath();
}

// FUNCTION: LEMBALL 0x0047F670
void FreeWin32FileWrapperPath(char *pszPath) {
    FreeVSMemBlock(pszPath);
}

// FUNCTION: LEMBALL 0x0047F6C0
int VsNetWin32FileWrapper::Write(const void *pvSource, DWORD cbWrite) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;
    BOOL fSuccess;
    DWORD cbWritten;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    fSuccess = WriteFile((HANDLE)(unsigned long)pWrapper->m_nReserved0c, pvSource, cbWrite, &cbWritten, 0);
    if (fSuccess == 0) {
        AppendCStringToStream(g_pErrorOutputStream, "Write error: ");
        g_pErrorOutputStream->AppendIntToStream(GetLastError());
        AppendCStringToStream(g_pErrorOutputStream, "\n");
        return 0;
    }

    pWrapper->m_nReserved10 += (int)cbWritten;
    if (cbWrite != cbWritten) {
        AppendCStringToStream(g_pErrorOutputStream, "Write error: ");
        AppendUIntToStream(g_pErrorOutputStream, cbWritten);
        AppendCStringToStream(g_pErrorOutputStream, " bytes written instead of ");
        g_pErrorOutputStream->AppendIntToStream(cbWrite);
        AppendCStringToStream(g_pErrorOutputStream, "\n");
        return 0;
    }
    return 1;
}

int WriteWin32FileWrapper(void *pFileWrapper, const void *pvSource, DWORD cbWrite) {
    return ((VsNetWin32FileWrapper *)pFileWrapper)->Write(pvSource, cbWrite);
}

// FUNCTION: LEMBALL 0x0047F780
int VsNetWin32FileWrapper::Read(LPVOID pvTarget, DWORD cbRead) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;
    BOOL fSuccess;
    DWORD cbReadNow;
    DWORD dwError;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    fSuccess = ReadFile((HANDLE)(unsigned long)pWrapper->m_nReserved0c, pvTarget, cbRead, &cbReadNow, 0);
    if (fSuccess == 0) {
        dwError = GetLastError();
        if (dwError != 0x21) {
            AppendCStringToStream(g_pErrorOutputStream, "Read error: ");
        g_pErrorOutputStream->AppendIntToStream(dwError);
            AppendCStringToStream(g_pErrorOutputStream, "\n");
        }
        return 0;
    }

    pWrapper->m_nReserved10 += (int)cbReadNow;
    if (cbReadNow != cbRead) {
        AppendCStringToStream(g_pErrorOutputStream, "Read error: ");
        AppendUIntToStream(g_pErrorOutputStream, cbReadNow);
        AppendCStringToStream(g_pErrorOutputStream, " bytes read instead of ");
        g_pErrorOutputStream->AppendIntToStream(cbRead);
        AppendCStringToStream(g_pErrorOutputStream, "in file ");
        AppendCStringToStream(g_pErrorOutputStream, (const char *)(unsigned long)pWrapper->m_nReserved04);
        AppendCStringToStream(g_pErrorOutputStream, " which is ");
        AppendUIntToStream(g_pErrorOutputStream, GetWin32FileWrapperLength(pWrapper));
        AppendCStringToStream(g_pErrorOutputStream, " bytes long.\n");
        return 0;
    }
    return 1;
}

int ReadWin32FileWrapper(void *pFileWrapper, LPVOID pvTarget, DWORD cbRead) {
    return ((VsNetWin32FileWrapper *)pFileWrapper)->Read(pvTarget, cbRead);
}

// FUNCTION: LEMBALL 0x0047F890
int VsNetWin32FileWrapper::Seek(DWORD dwOffset) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    pWrapper->m_nReserved10 = (int)dwOffset;
    return SetFilePointer((HANDLE)(unsigned long)pWrapper->m_nReserved0c, dwOffset, 0, FILE_BEGIN) != 0xffffffff;
}

int SeekWin32FileWrapper(void *pFileWrapper, DWORD dwOffset) {
    return ((VsNetWin32FileWrapper *)pFileWrapper)->Seek(dwOffset);
}

// FUNCTION: LEMBALL 0x0047F8F0
DWORD VsNetWin32FileWrapper::GetLength(void) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    return GetFileSize((HANDLE)(unsigned long)pWrapper->m_nReserved0c, 0);
}

DWORD GetWin32FileWrapperLength(void *pFileWrapper) {
    return ((VsNetWin32FileWrapper *)pFileWrapper)->GetLength();
}

// FUNCTION: LEMBALL 0x0047F8B0
DWORD VsNetWin32FileWrapper::GetOffset(void) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    return (DWORD)pWrapper->m_nReserved10;
}

DWORD GetWin32FileWrapperOffset(void *pFileWrapper) {
    return ((VsNetWin32FileWrapper *)pFileWrapper)->GetOffset();
}

// FUNCTION: LEMBALL 0x0047F900
int VsNetWin32FileWrapper::LockRange(DWORD dwOffset, DWORD cbRange) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    return LockFile((HANDLE)(unsigned long)pWrapper->m_nReserved0c, dwOffset, 0, cbRange, 0);
}

int LockWin32FileRange(void *pFileWrapper, DWORD dwOffset, DWORD cbRange) {
    return ((VsNetWin32FileWrapper *)pFileWrapper)->LockRange(dwOffset, cbRange);
}

// FUNCTION: LEMBALL 0x0047F920
int VsNetWin32FileWrapper::UnlockRange(DWORD dwOffset, DWORD cbRange) {
    VsNetInlineOpenWin32FileWrapper *pWrapper;

    pWrapper = (VsNetInlineOpenWin32FileWrapper *)this;
    return UnlockFile((HANDLE)(unsigned long)pWrapper->m_nReserved0c, dwOffset, 0, cbRange, 0);
}

int UnlockWin32FileRange(void *pFileWrapper, DWORD dwOffset, DWORD cbRange) {
    return ((VsNetWin32FileWrapper *)pFileWrapper)->UnlockRange(dwOffset, cbRange);
}

// FUNCTION: LEMBALL 0x00479920
void *LEMBALL_FASTCALL ClearFileBackedEffChannelStateWord(void *pObject) {
    *(int *)pObject = 0;
    return pObject;
}

// FUNCTION: LEMBALL 0x00479540
VsNetEffStreamRecordSlot *VsNetEffStreamRecordSlot::Initialize(void) {
    m_pVtable = g_GAME_EffStreamConstructionVtable;
    m_nReserved04 = 0;
    ((GameEffStream *)this)->ResetStateFields();
    m_wObservedMarker = 1;
    m_nBufferEnd18 += 0x30;
    m_wCommittedMarker = 0;
    m_pVtable = g_NETWORK_EffRecordSlotVtable;
    m_cbPayload = 0;
    m_szTargetName[0] = '\0';
    m_szSourceName[0] = '\0';
    return this;
}

// FUNCTION: LEMBALL 0x00479580
void VsNetEffStreamRecordSlot::Save(void) {
    VsNetEffStreamRecordSlot *pRecordSlot;
    unsigned short wCommittedMarker;
    int cbPayload;

    pRecordSlot = (VsNetEffStreamRecordSlot *)this;
    ++pRecordSlot->m_wCommittedMarker;
    ((VsNetEffStreamBase *)pRecordSlot)->WriteEffStreamU16BE(pRecordSlot->m_wCommittedMarker);
    wCommittedMarker = pRecordSlot->m_wCommittedMarker;
    cbPayload = pRecordSlot->m_cbPayload;
    pRecordSlot->m_wObservedMarker = wCommittedMarker;
    ((VsNetEffStreamBase *)pRecordSlot)->WriteEffStreamU32BE(cbPayload);
    ((VsNetEffStreamBase *)pRecordSlot)->WriteEffStreamBytes(pRecordSlot->m_szTargetName, 0x15);
    ((VsNetEffStreamBase *)pRecordSlot)->WriteEffStreamBytes(pRecordSlot->m_szSourceName, 0x15);
}

// FUNCTION: LEMBALL 0x004795D0
void VsNetEffStreamRecordSlot::Load(void) {
    VsNetEffStreamRecordSlot *pRecordSlot;

    pRecordSlot = (VsNetEffStreamRecordSlot *)this;
    ((VsNetEffStreamBase *)pRecordSlot)->ReadEffStreamU16BE((unsigned char *)&pRecordSlot->m_wCommittedMarker);
    if (pRecordSlot->m_wObservedMarker != pRecordSlot->m_wCommittedMarker) {
        ((VsNetEffStreamBase *)pRecordSlot)->ReadEffStreamU32BE((unsigned char *)&pRecordSlot->m_cbPayload);
        ((VsNetEffStreamBase *)pRecordSlot)->ReadEffStreamBytes(pRecordSlot->m_szTargetName, 0x15);
        ((VsNetEffStreamBase *)pRecordSlot)->ReadEffStreamBytes(pRecordSlot->m_szSourceName, 0x15);
        return;
    }
    pRecordSlot->m_nReserved20 += 0x2e;
}

// FUNCTION: LEMBALL 0x00479620
VsNetEffStreamRecordSlotTable *VsNetEffStreamRecordSlotTable::Construct(int cSlots) {
    int *pSlotStorage;
    VsNetEffStreamRecordSlot *pSlot;
    int i;

    m_pVtable = g_GAME_EffStreamConstructionVtable;
    m_nRecordHeaderOffset04 = 0;
    ((GameEffStream *)this)->ResetStateFields();
    m_cSlots = cSlots;
    m_pVtable = g_NETWORK_EffRecordSlotTableVtable;

    pSlotStorage = (int *)AllocateVSMemBlock(cSlots * 0x60 + 4);
    if (pSlotStorage == 0) {
        m_pSlots = 0;
    } else {
        *pSlotStorage = cSlots;
        pSlot = (VsNetEffStreamRecordSlot *)(pSlotStorage + 1);
        i = cSlots;
        while (--i >= 0) {
            pSlot->Initialize();
            ++pSlot;
        }
        m_pSlots = (VsNetEffStreamRecordSlot *)(pSlotStorage + 1);
    }

    m_pwSlotFlags = (unsigned short *)AllocateVSMemBlock((unsigned int)(m_cSlots * 2));
    i = 0;
    if (0 < m_cSlots) {
        do {
            m_pwSlotFlags[i] = 0;
            ++i;
        } while (i < m_cSlots);
    }

    m_nAccumulatedStreamLength18 += m_pSlots->m_nBufferEnd18 * cSlots;
    return this;
}

// FUNCTION: LEMBALL 0x004796E0
void VsNetEffStreamRecordSlotTable::Destroy() {
    m_pVtable = g_NETWORK_EffRecordSlotTableVtable;
    FreeVSMemBlock(m_pwSlotFlags);
    if (m_pSlots != 0) {
        ((VsNetDeleteObject *)m_pSlots)->m_pVtable->m_pDelete(m_pSlots, 3);
    }
    ((VsNetEffStreamBase *)this)->DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x00479720
VsNetEffStreamRecordSlot *VsNetEffStreamRecordSlotTable::FindNextChangedSlot(void) {
    VsNetEffStreamRecordSlot *pSlot;
    int nSlotIndex;

    nSlotIndex = m_nCurrentChangedSlot38;
    if (nSlotIndex < m_cSlots) {
        pSlot = m_pSlots + nSlotIndex;
        while (pSlot->m_wCommittedMarker <= m_pwSlotFlags[nSlotIndex]) {
            ++nSlotIndex;
            ++pSlot;
            if (m_cSlots <= nSlotIndex) {
                break;
            }
        }
    }

    if (nSlotIndex == m_cSlots) {
        m_nCurrentChangedSlot38 = -1;
        return 0;
    }

    m_nCurrentChangedSlot38 = nSlotIndex;
    m_pwSlotFlags[nSlotIndex] = m_pSlots[nSlotIndex].m_wCommittedMarker;
    return &m_pSlots[nSlotIndex];
}

// FUNCTION: LEMBALL 0x00479790
void VsNetEffStreamRecordSlotTable::Save(void) {
    VsNetEffStreamRecordSlot *pSlot;
    int nSlotOffset;
    int i;

    nSlotOffset = 0;
    i = 0;
    if (0 < m_cSlots) {
        pSlot = m_pSlots;
        do {
            pSlot = (VsNetEffStreamRecordSlot *)((char *)m_pSlots + nSlotOffset);
            ((VsNetEffStreamBase *)pSlot)->SaveEffStreamToMemoryRange(m_nSaveRangeCursor1c, 0);
            m_nSaveRangeCursor1c +=
                (int)(unsigned long)pSlot->m_nReserved1c - pSlot->m_pPayloadBuffer08;
            nSlotOffset += 0x60;
            ++i;
        } while (i < m_cSlots);
    }
}

// FUNCTION: LEMBALL 0x004797D0
void VsNetEffStreamRecordSlotTable::Load(void) {
    VsNetEffStreamRecordSlotTable *pTable;
    VsNetEffStreamRecordSlot *pSlot;
    int i;

    pTable = (VsNetEffStreamRecordSlotTable *)this;
    i = 0;
    if (pTable->m_cSlots <= 0) {
        pTable->m_nCurrentChangedSlot38 = 0;
        return;
    }

    pSlot = pTable->m_pSlots;
    do {
        if (((VsNetEffStreamBase *)pSlot)
                ->SaveEffStreamToMemoryRange(pTable->m_nLoadSourceCursor20, 0) != 0) {
            pTable->m_nLoadSourceCursor20 = pSlot->m_nBufferEnd18;
        }
        ++i;
        ++pSlot;
    } while (i < pTable->m_cSlots);
    pTable->m_nCurrentChangedSlot38 = 0;
}

// FUNCTION: LEMBALL 0x00479810
VsNetEff512ByteStateStream *VsNetEff512ByteStateStream::Construct() {
    int i;

    m_pVtable = g_GAME_EffStreamConstructionVtable;
    i = 0;
    m_nReserved04 = 0;
    ((GameEffStream *)this)->ResetStateFields();
    m_pVtable = g_NETWORK_EffStateStreamVtable;
    m_pbStateBytes2c = (unsigned char *)AllocateVSMemBlock(0x200);
    m_nBufferEnd18 += 0x200;
    do {
        ++i;
        m_pbStateBytes2c[i - 1] = 0;
    } while (i < 0x200);
    return this;
}

// FUNCTION: LEMBALL 0x0047B7D0
void *VsNetEffStreamRecordSlotTable::Delete(BYTE fDeleteFlags) {
    VsNetEffStreamRecordSlotTable *pTable;

    pTable = (VsNetEffStreamRecordSlotTable *)this;
    pTable->Destroy();
    if ((fDeleteFlags & 1) != 0) {
        FreeVSMemBlock(pTable);
    }
    return pTable;
}

// FUNCTION: LEMBALL 0x0047B7F0
void *VsNetEffStreamRecordSlot::Delete(BYTE fDeleteFlags) {
    VsNetEffStreamRecordSlot *pSlot;
    VsNetEffStreamRecordSlot *pCurrent;
    int cSlots;

    pSlot = (VsNetEffStreamRecordSlot *)this;
    if ((fDeleteFlags & 2) != 0) {
        cSlots = *((int *)pSlot - 1);
        pCurrent = pSlot + cSlots;
        while (--cSlots >= 0) {
            --pCurrent;
    ((VsNetEffStreamBase *)pCurrent)->DestroyEffStreamBase();
        }
        FreeVSMemBlock((int *)pSlot - 1);
        return this;
    }

    ((VsNetEffStreamBase *)pSlot)->DestroyEffStreamBase();
    if ((fDeleteFlags & 1) != 0) {
        FreeVSMemBlock(pSlot);
    }
    return this;
}

// FUNCTION: LEMBALL 0x0047B970
void *VsNetEffStreamBase::DeleteEffStreamBaseWrapper(BYTE fDeleteFlags) {
    DestroyEffStreamBase();
    if ((fDeleteFlags & 1) != 0) {
        FreeVSMemBlock(this);
    }
    return this;
}

// FUNCTION: LEMBALL 0x0047B870
void VsNetEff512ByteStateStream::Save(void) {
    VsNetEff512ByteStateStream *pStateStream;

    pStateStream = (VsNetEff512ByteStateStream *)this;
    ((VsNetEffStreamBase *)pStateStream)->WriteEffStreamBytes(pStateStream->m_pbStateBytes2c, 0x200);
}

// FUNCTION: LEMBALL 0x0047B880
void VsNetEff512ByteStateStream::Load(void) {
    VsNetEff512ByteStateStream *pStateStream;

    pStateStream = (VsNetEff512ByteStateStream *)this;
    ((VsNetEffStreamBase *)pStateStream)->ReadEffStreamBytes(pStateStream->m_pbStateBytes2c, 0x200);
}

// FUNCTION: LEMBALL 0x0047B890
void *VsNetEff512ByteStateStream::Delete(BYTE fDeleteFlags) {
    VsNetEff512ByteStateStream *pStateStream;
    unsigned char *pbStateBytes;

    pStateStream = (VsNetEff512ByteStateStream *)this;
    pbStateBytes = pStateStream->m_pbStateBytes2c;
    pStateStream->m_pVtable = g_NETWORK_EffStateStreamVtable;
    FreeVSMemBlock((void *)(unsigned long)pbStateBytes);
    ((VsNetEffStreamBase *)pStateStream)->DestroyEffStreamBase();
    if ((fDeleteFlags & 1) != 0) {
        FreeVSMemBlock(pStateStream);
    }
    return pStateStream;
}

void LoadEffStreamRecordSlot(void *pObject) {
    ((VsNetEffStreamRecordSlot *)pObject)->Load();
}

void SaveEffStreamRecordSlot(void *pObject) {
    ((VsNetEffStreamRecordSlot *)pObject)->Save();
}

void *DeleteEffStreamRecordSlot(void *pObject, BYTE fDeleteFlags) {
    return ((VsNetEffStreamRecordSlot *)pObject)->Delete(fDeleteFlags);
}

void LoadEffStreamRecordSlotTable(void *pObject) {
    ((VsNetEffStreamRecordSlotTable *)pObject)->Load();
}

void SaveEffStreamRecordSlotTable(void *pObject) {
    ((VsNetEffStreamRecordSlotTable *)pObject)->Save();
}

void *DeleteEffStreamRecordSlotTable(void *pObject, BYTE fDeleteFlags) {
    return ((VsNetEffStreamRecordSlotTable *)pObject)->Delete(fDeleteFlags);
}

void WriteEff512ByteStatePayload(void *pObject) {
    ((VsNetEff512ByteStateStream *)pObject)->Save();
}

void LoadEff512ByteStatePayload(void *pObject) {
    ((VsNetEff512ByteStateStream *)pObject)->Load();
}

void *DeleteEff512ByteStateStream(void *pObject, BYTE fDeleteFlags) {
    return ((VsNetEff512ByteStateStream *)pObject)->Delete(fDeleteFlags);
}

// FUNCTION: LEMBALL 0x0047B910
void *DeleteEffChannelStateSideBufferWrapper(void *pObject, BYTE fFreeMemory) {
    ((VsNetEffStreamChannelState *)pObject)->DestroyEffStreamChannelState();
    *(void **)((char *)pObject - 0x14) = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (*(void **)((char *)pObject - 0x10) != 0) {
        FreeVSMemBlock(*(void **)((char *)pObject - 0x10));
    }
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x20);
    }
    return (char *)pObject - 0x20;
}

// FUNCTION: LEMBALL 0x0047B8F0
void LEMBALL_FASTCALL ClearNestedEffChannelAsyncStatus(void *pObject) {
    ((VsNetEffStreamChannelState *)(
         (char *)pObject +
         (*(VsNetConstructionAdjustorVtable **)((char *)pObject - 0x1c))
             ->m_nSecondaryOffset -
         0x1c))
        ->SetEffStreamChannelAsyncErrorStatus(0);
}

// FUNCTION: LEMBALL 0x0047BA40
void AdjustAndClearEmbeddedFileBackedEffChannelAsyncErrorStatus(void *pObject) {
    ClearNestedEffChannelAsyncStatus((char *)pObject + 0x138);
}

// FUNCTION: LEMBALL 0x0047BCA0
BYTE AdjustAndMapGlobalStateFileBackedEffCallbackFailure(void *pObject) {
    return MapNestedEffCallbackFailureToStatus6((char *)pObject + 0xd8);
}

// FUNCTION: LEMBALL 0x0047BD10
void AdjustAndClearTimedFileBackedEffChannelAsyncStatus(void *pObject) {
    ClearNestedEffChannelAsyncStatus((char *)pObject + 0xd8);
}

// FUNCTION: LEMBALL 0x00479930
void *VsNetDualFileBackedEffChannelConstructorView::ConstructDualFileBackedEffChannel(
    int fConstructEmbeddedObjects) {
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    VsNetAdjustorSubobject *pFatalThunk;
    VsNetDualFileBackedEffChannelFront *pFront;
    VsNetDualFileBackedEffChannelConstructorView *pConstructed;
    VsNetFileBackedChannelViewOffsets *pChannelOffsets;
    VsNetFileWrapperThunkOffsets *pWrapperThunkOffsets;
    char *pbObjectBase;
    void *pObject;

    pObject = (void *)this;
    pFront = (VsNetDualFileBackedEffChannelFront *)pObject;
    pConstructed = (VsNetDualFileBackedEffChannelConstructorView *)pObject;
    pbObjectBase = (char *)pObject;
    if (fConstructEmbeddedObjects != 0) {
        pFront->m_pThunk0c = (VsNetFileBackedChannelViewOffsets *)&g_NETWORK_FileBackedDualChannelStateOffsets;
        pFront->m_pDualStreamOffsets88 = &g_NETWORK_FileBackedDualStreamOffsets;
        pFront->m_pFileThunkb4 = (VsNetFileWrapperThunkOffsets *)&g_NETWORK_FileBackedDualFileWrapperThunkOffsets;
        pConstructed->m_ChannelState18.ConstructEffStreamChannelState();
        ConstructDualHandleEffStream(pConstructed->m_abDualHandleStream44, 0);
        pConstructed->m_FileWrapper9c.m_pVtable = g_NETWORK_CrtFatalRuntimeErrorThunk;
        pConstructed->m_FileWrapper9c.m_nReserved04 = 0;
        pConstructed->m_FileWrapper9c.m_fOpen08 = 1;
        pConstructed->m_FileWrapper9c.m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pConstructed->m_FileWrapper9c.m_pDeleteThunk14 = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        pWrapperThunkOffsets = pFront->m_pFileThunkb4;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0xb4 + pWrapperThunkOffsets->m_nOpenThunkViewOffset04);
        pSecondaryThunk =
            (VsNetAdjustorSubobject *)(pbObjectBase + 0xb4 + pWrapperThunkOffsets->m_nNestedCallbackThunkOffset08);
        pPrimaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunkVtable;
    }

    ClearFileBackedEffChannelStateWord(pObject);
    pChannelOffsets = pFront->m_pThunk0c;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nChannelStateViewOffset04 - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nStreamViewOffset08 - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nFileWrapperViewOffset0c - 4);
    pFatalThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nFatalThunkViewOffset10 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_FileBackedDualPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_FileBackedDualSecondaryThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_FileBackedFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pChannelOffsets->m_nChannelStateViewOffset04 - 0xc;
    pFront->m_nMode10 = -1;
    return this;
}

// FUNCTION: LEMBALL 0x00479E20
void *VsNetTimedFileBackedEffChannelConstructorView::ConstructTimedFileBackedEffChannel(
    int fConstructEmbeddedObjects) {
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    VsNetAdjustorSubobject *pFatalThunk;
    VsNetTimedFileBackedEffChannelFront *pFront;
    VsNetTimedFileBackedEffChannelConstructorView *pConstructed;
    VsNetFileBackedChannelViewOffsets *pChannelOffsets;
    VsNetFileWrapperThunkOffsets *pWrapperThunkOffsets;
    char *pbObjectBase;
    void *pObject;

    pObject = (void *)this;
    pFront = (VsNetTimedFileBackedEffChannelFront *)pObject;
    pConstructed = (VsNetTimedFileBackedEffChannelConstructorView *)pObject;
    pbObjectBase = (char *)pObject;
    if (fConstructEmbeddedObjects != 0) {
        pFront->m_pThunk0c = (VsNetFileBackedChannelViewOffsets *)&g_NETWORK_FileBackedTimedChannelStateOffsets;
        pFront->m_pTimedStreamOffsets8c = &g_NETWORK_FileBackedTimedStreamOffsets;
        pFront->m_pFileThunkd4 = (VsNetFileWrapperThunkOffsets *)&g_NETWORK_FileBackedTimedFileWrapperThunkOffsets;
        pConstructed->m_ChannelState18.ConstructEffStreamChannelState();
        ((VsNetTimedEffStream *)&pConstructed->m_TimedStream48)->ConstructTimedEffStream(0);
        pConstructed->m_FileWrapperbc.m_pVtable = g_NETWORK_CrtFatalRuntimeErrorThunk;
        pConstructed->m_FileWrapperbc.m_nReserved04 = 0;
        pConstructed->m_FileWrapperbc.m_fOpen08 = 1;
        pConstructed->m_FileWrapperbc.m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pConstructed->m_FileWrapperbc.m_pDeleteThunk14 = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        pWrapperThunkOffsets = pFront->m_pFileThunkd4;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0xd4 + pWrapperThunkOffsets->m_nOpenThunkViewOffset04);
        pSecondaryThunk =
            (VsNetAdjustorSubobject *)(pbObjectBase + 0xd4 + pWrapperThunkOffsets->m_nNestedCallbackThunkOffset08);
        pPrimaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunkVtable;
    }

    ClearFileBackedEffChannelStateWord(pObject);
    pChannelOffsets = pFront->m_pThunk0c;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nChannelStateViewOffset04 - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nStreamViewOffset08 - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nFileWrapperViewOffset0c - 4);
    pFatalThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pChannelOffsets->m_nFatalThunkViewOffset10 - 4);
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
void VsNetTimedFileBackedEffChannelVtableRestoreView::RestoreTimedFileBackedEffChannelVtables(void) {
    *(void **)((char *)this + (*(int **)((char *)this - 0xc))[1] - 0xc) =
        g_NETWORK_FileBackedTimedPrimaryThunkVtable;
    *(void **)((char *)this + (*(int **)((char *)this - 0xc))[2] - 0xc) =
        g_NETWORK_FileBackedTimedSecondaryThunkVtable;
    *(void **)((char *)this + (*(int **)((char *)this - 0xc))[3] - 0xc) =
        g_NETWORK_OpenWin32FileWrapperVtable;
    *(void **)((char *)this + (*(int **)((char *)this - 0xc))[4] - 0xc) =
        g_NETWORK_FileBackedFatalFileWrapperVtable;
    *(int *)((char *)this + (*(int **)((char *)this - 0xc))[1] - 0x10) =
        (*(int **)((char *)this - 0xc))[1] - 0xc;
    *(int *)((char *)this + (*(int **)((char *)this - 0xc))[2] - 0x10) =
        (*(int **)((char *)this - 0xc))[2] - 0x3c;
}

// FUNCTION: LEMBALL 0x0047A220
void *VsNetEmbeddedFileBackedEffChannelStackConstructorView::ConstructEmbeddedFileBackedEffChannelStack(
    int fConstructEmbeddedObjects) {
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    VsNetAdjustorSubobject *pQuaternaryThunk;
    VsNetAdjustorSubobject *pFatalThunk;
    const VsNetEmbeddedStackViewOffsets *pEmbeddedOffsets;
    const VsNetEmbeddedStackFileViewOffsets *pFileViewOffsets;
    VsNetEmbeddedFileBackedEffChannelStackConstructorView *pConstructed;
    VsNetEmbeddedFileBackedStackFront *pStackFront;
    VsNetEmbeddedFileBackedStackOpenWrapperFront *pFileWrapperFront;
    char *pbObjectBase;
    void *pObject;

    pObject = (void *)this;
    pbObjectBase = (char *)pObject;
    pConstructed = (VsNetEmbeddedFileBackedEffChannelStackConstructorView *)pObject;
    pStackFront = (VsNetEmbeddedFileBackedStackFront *)pObject;
    pFileWrapperFront = (VsNetEmbeddedFileBackedStackOpenWrapperFront *)pObject;
    if (fConstructEmbeddedObjects != 0) {
        pStackFront->m_pTimedStreamOffsets7c = &g_NETWORK_EmbeddedFileBackedTimedStreamOffsets;
        pStackFront->m_pDualStreamOffsetsf4 = &g_NETWORK_EmbeddedFileBackedDualStreamOffsets;
        pStackFront->m_pDualThunkOffsets108 = (VsNetEmbeddedStackViewOffsets *)&g_NETWORK_EmbeddedFileBackedDualThunkOffsets;
        *(VsNetEmbeddedStackFileViewOffsets **)(pbObjectBase + 0x124) =
            (VsNetEmbeddedStackFileViewOffsets *)&g_NETWORK_EmbeddedFileBackedFileWrapperThunkOffsets;
        pStackFront->m_pFinalThunkOffsets = (VsNetEmbeddedStackViewOffsets *)&g_NETWORK_EmbeddedFileBackedFinalThunkOffsets;
        pStackFront->m_pPrimaryThunkOffsets138 = &g_NETWORK_EmbeddedFileBackedPrimaryThunkOffsets;
        pStackFront->m_pTimedFileViewOffsets14c = &g_NETWORK_EmbeddedFileBackedTimedFileViewOffsets;

        pConstructed->m_ChannelState08.ConstructEffStreamChannelState();
        ((VsNetTimedEffStream *)&pConstructed->m_TimedStream38)->ConstructTimedEffStream(0);
        ConstructDualHandleEffStream(pConstructed->m_abDualHandleStreamB0, 0);

        pEmbeddedOffsets = (VsNetEmbeddedStackViewOffsets *)pStackFront->m_pDualThunkOffsets108;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x108 + pEmbeddedOffsets->m_nChannelStateViewOffset04 - 4);
        pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x108 + pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 4);
        pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x108 + pEmbeddedOffsets->m_nDualStreamViewOffset0c - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunkVtable;
        pSecondaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackTimedStreamVtable;
        pTertiaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackDualStreamVtable;
        pPrimaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nChannelStateViewOffset04 - 8;
        pSecondaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 0x38;
        pTertiaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nDualStreamViewOffset0c - 0xb0;

        pConstructed->m_FileWrapper10c.m_pVtable = g_NETWORK_CrtFatalRuntimeErrorThunk;
        pConstructed->m_FileWrapper10c.m_nReserved04 = 0;
        pConstructed->m_FileWrapper10c.m_fOpen08 = 1;
        pConstructed->m_FileWrapper10c.m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pConstructed->m_FileWrapper10c.m_pDeleteThunk14 = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        pFileViewOffsets = (VsNetEmbeddedStackFileViewOffsets *)pFileWrapperFront->m_pFileWrapperThunk124;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x124 + pFileViewOffsets->m_nFileWrapperViewOffset04 - 4);
        pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x124 + pFileViewOffsets->m_nChannelStateViewOffset08 - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunkVtable;

        ((VsNetDualFileBackedEffChannelConstructorView *)pConstructed->m_abDualFileChannel12c)
            ->ConstructDualFileBackedEffChannel(0);
        ((VsNetTimedFileBackedEffChannelConstructorView *)pConstructed->m_abTimedFileChannel1d0)
            ->ConstructTimedFileBackedEffChannel(0);
    }

    pEmbeddedOffsets = (VsNetEmbeddedStackViewOffsets *)pStackFront->m_pFinalThunkOffsets;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pEmbeddedOffsets->m_nChannelStateViewOffset04 - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pEmbeddedOffsets->m_nDualStreamViewOffset0c - 4);
    pQuaternaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pEmbeddedOffsets->m_nFinalFileThunkViewOffset10 - 4);
    pFatalThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pEmbeddedOffsets->m_nFinalFatalThunkViewOffset14 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_EmbeddedFileBackedFinalDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_FileBackedFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nChannelStateViewOffset04 - 8;
    pSecondaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 0x38;
    pTertiaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nDualStreamViewOffset0c - 0xb0;
    return (void *)(char *)this;
}

// FUNCTION: LEMBALL 0x0047A570
void *VsNetGlobalStateCompositeLayout::ConstructGlobalStateFileBackedEffComposite(
    int fConstructEmbeddedObjects) {
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    VsNetAdjustorSubobject *pQuaternaryThunk;
    VsNetAdjustorSubobject *pFatalThunk;
    VsNetCompositeViewOffsets *pCompositeOffsets;
    const VsNetEmbeddedStackViewOffsets *pEmbeddedOffsets;
    const VsNetEmbeddedStackFileViewOffsets *pFileViewOffsets;
    VsNetEffTransportRuntimeService *pRuntimeService;
    VsNetGlobalStateCompositeLayout *pFront;
    VsNetGlobalStateCompositeOpenWrapperFront *pOpenWrapperFront;
    VsNetEmbeddedFileBackedStackFront *pEmbeddedStackFront;
    VsNetFileBackedCompositeInitialStream *pInitialStream;
    VsNetEff512ByteStateStream *pStateStream;
    VsNetEffStreamRecordSlotTable *pRecordSlotTable;
    char *pbObjectBase;
    char *pbEmbeddedStackBase;
    void *pStorage;

    pbObjectBase = (char *)this;
    pFront = this;
    pCompositeOffsets = pFront->m_pTimedStreamOffsets20;
    pOpenWrapperFront = (VsNetGlobalStateCompositeOpenWrapperFront *)this;
    pEmbeddedStackFront = (VsNetEmbeddedFileBackedStackFront *)(pbObjectBase + 0x1b0);
    if (fConstructEmbeddedObjects != 0) {
        pFront->m_pTimedStreamOffsets20 = (VsNetCompositeViewOffsets *)&g_NETWORK_GlobalStateCompositeTimedStreamOffsets;
        pFront->m_pTransportOffsets00 = (VsNetCompositeViewOffsets *)&g_NETWORK_GlobalStateCompositeTransportOffsets;
        *(void **)(pbObjectBase + 0xd4) = (void *)&g_NETWORK_GlobalStateCompositeDualStreamOffsets;
        pFront->m_pDualThunkOffsets14c = (VsNetCompositeViewOffsets *)&g_NETWORK_GlobalStateCompositeDualThunkOffsets;
        pFront->m_pEmbeddedStackTransportOffsets160 = (VsNetEmbeddedStackViewOffsets *)&g_NETWORK_GlobalStateCompositeFileWrapperOffsets;
        pFront->m_pEmbeddedStackFileViewOffsets180 = &g_NETWORK_GlobalStateCompositeTimedFileThunkOffsets;
        pFront->m_pEmbeddedStackTimedStreamOffsets194 = (VsNetEmbeddedStackViewOffsets *)&g_NETWORK_GlobalStateCompositeEmbeddedStackOffsets;
        pFront->m_pEmbeddedStackDualStreamOffsets1a8 = (VsNetEmbeddedStackFileViewOffsets *)&g_NETWORK_GlobalStateCompositeEmbeddedStackDataOffsets;
        pEmbeddedStackFront->m_pFinalThunkOffsets = &g_NETWORK_GlobalStateCompositeEmbeddedStackFinalOffsets;

        pFront->m_ChannelState60.ConstructEffStreamChannelState();
        ((VsNetTimedEffStream *)pFront->m_abTimedStream90)->ConstructTimedEffStream(0);
        ConstructDualHandleEffStream(pFront->m_abDualStream108, 0);

        pbEmbeddedStackBase = pbObjectBase + 0x160;
        pEmbeddedOffsets = *(VsNetEmbeddedStackViewOffsets **)pbEmbeddedStackBase;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nChannelStateViewOffset04 - 4);
        pSecondaryThunk = (VsNetAdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 4);
        pTertiaryThunk = (VsNetAdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nDualStreamViewOffset0c - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunkVtable;
        pSecondaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackTimedStreamVtable;
        pTertiaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackDualStreamVtable;
        pPrimaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nChannelStateViewOffset04 - 8;
        pSecondaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 0x38;
        pTertiaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nDualStreamViewOffset0c - 0xb0;

        pOpenWrapperFront->m_FileWrapper164.m_nReserved04 = 0;
        pOpenWrapperFront->m_FileWrapper164.m_pVtable = g_NETWORK_CrtFatalRuntimeErrorThunk;
        pOpenWrapperFront->m_FileWrapper164.m_fOpen08 = 1;
        pOpenWrapperFront->m_FileWrapper164.m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pOpenWrapperFront->m_FileWrapper164.m_pDeleteThunk14 = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        pFileViewOffsets = pOpenWrapperFront->m_pFileWrapperThunk180;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x180 + pFileViewOffsets->m_nFileWrapperViewOffset04 - 4);
        pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x180 + pFileViewOffsets->m_nChannelStateViewOffset08 - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunkVtable;

        ((VsNetDualFileBackedEffChannelConstructorView *)(pbObjectBase + 0x188))
            ->ConstructDualFileBackedEffChannel(0);
        ((VsNetTimedFileBackedEffChannelConstructorView *)(pbObjectBase + 0x19c))
            ->ConstructTimedFileBackedEffChannel(0);
        ((VsNetEmbeddedFileBackedEffChannelStackConstructorView *)(pbObjectBase + 0x1b0))
            ->ConstructEmbeddedFileBackedEffChannelStack(0);
    }

    pInitialStream = &pFront->m_InitialStream24;
    pInitialStream->m_nReserved04 = 0;
    pInitialStream->m_pVtable = g_GAME_EffStreamConstructionVtable;
    ((GameEffStream *)pInitialStream)->ResetStateFields();
    pInitialStream->m_nSerializedLength18 += 2;
    pInitialStream->m_wFlags2c = 0;
    pInitialStream->m_pVtable = g_NETWORK_GlobalStateCompositeInitialVtable;

    pCompositeOffsets = pFront->m_pTimedStreamOffsets20;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nTransportViewOffset04 - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nTimedStreamViewOffset08 - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nDualStreamViewOffset0c - 4);
    pQuaternaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nDualThunkViewOffset10 - 4);
    pFatalThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nMarkerViewOffset14 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_FileBackedFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = 0;
    pSecondaryThunk->m_nThisDelta = 0;
    pTertiaryThunk->m_nThisDelta = 0;

    ((VsNetRuntimeChannelStack *)this)->ConstructEffTransportRuntimeChannelStack(0);

    *(void **)this = g_NETWORK_GlobalStateCompositeFinalVtable;
    pPrimaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_FileBackedFinalFileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_GlobalStateCompositeFinalFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pCompositeOffsets->m_nTransportViewOffset04 - 0x40;
    pSecondaryThunk->m_nThisDelta = pCompositeOffsets->m_nTimedStreamViewOffset08 - 0x70;
    pTertiaryThunk->m_nThisDelta = pCompositeOffsets->m_nDualStreamViewOffset0c - 0xe8;
    pFatalThunk->m_nThisDelta = pCompositeOffsets->m_nMarkerViewOffset14 - 0x15c;

    pRuntimeService = (VsNetEffTransportRuntimeService *)g_pEffTransportRuntimeService;
    pRuntimeService->m_pVtable->m_pSelectLocalHostName(pRuntimeService, g_pszFileBasedNetworkLocalHostName);

    if (g_pGlobalStateEff512ByteStream == 0) {
        pStorage = AllocateVSMemBlock(0x30);
        if (pStorage == 0) {
            g_pGlobalStateEff512ByteStream = 0;
        } else {
            g_pGlobalStateEff512ByteStream = ((VsNetEff512ByteStateStream *)pStorage)->Construct();
        }
    }
    pStateStream = (VsNetEff512ByteStateStream *)g_pGlobalStateEff512ByteStream;

    *(int *)(pbObjectBase + pCompositeOffsets->m_nRecordTableAOffset18 + 0x28) = 0x14;
    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableCOffset20) = 0;
    } else {
        pRecordSlotTable =
            ((VsNetEffStreamRecordSlotTable *)pStorage)
                ->Construct(*(int *)(pbObjectBase + pCompositeOffsets->m_nRecordTableAOffset18 + 0x28));
        *(VsNetEffStreamRecordSlotTable **)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableCOffset20) =
            pRecordSlotTable;
    }

    *(VsNetEffStreamRecordSlotTable **)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableBOffset1c) =
        *(VsNetEffStreamRecordSlotTable **)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableCOffset20);
    *(int *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableCOffset20 + 0x24) =
        pStateStream->m_nBufferEnd18 + pInitialStream->m_nSerializedLength18;
    *(int *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableBOffset1c + 0x24) =
        *(int *)(pbObjectBase + 0x20 + pCompositeOffsets->m_nRecordTableCOffset20 + 0x24);
    pFront->m_fGlobalStateStreamLoaded54 = 0;
    return (void *)(char *)this;
}

// FUNCTION: LEMBALL 0x0047A910
void VsNetGlobalStateCompositeDestroyView::DestroyGlobalStateFileBackedEffComposite(void) {
    int *pOffsets;
    int nOffset;
    char *pbSubobject;
    VsNetDeleteObject *pRecordSlotTable;

    pOffsets = *(int **)((char *)this - 0x40);
    *(void **)((char *)this - 0x60) = g_NETWORK_GlobalStateCompositeFinalVtable;
    nOffset = pOffsets[1];
    pbSubobject = (char *)this + nOffset - 0x40;
    *(void **)pbSubobject = g_NETWORK_GlobalStateCompositeFinalPrimaryThunkVtable;
    nOffset = pOffsets[2];
    pbSubobject = (char *)this + nOffset - 0x40;
    *(void **)pbSubobject = g_NETWORK_GlobalStateCompositeFinalTimedThunkVtable;
    nOffset = pOffsets[3];
    pbSubobject = (char *)this + nOffset - 0x40;
    *(void **)pbSubobject = g_NETWORK_GlobalStateCompositeFinalDualThunkVtable;
    nOffset = pOffsets[5];
    pbSubobject = (char *)this + nOffset - 0x40;
    *(void **)pbSubobject = g_NETWORK_FileBackedFinalFileWrapperVtable;
    nOffset = pOffsets[6];
    pbSubobject = (char *)this + nOffset - 0x40;
    *(void **)pbSubobject = g_NETWORK_GlobalStateCompositeFinalFatalFileWrapperVtable;
    nOffset = pOffsets[1];
    *(int *)((char *)this + nOffset - 0x44) = nOffset - 0x40;
    nOffset = pOffsets[2];
    *(int *)((char *)this + nOffset - 0x44) = nOffset - 0x70;
    nOffset = pOffsets[3];
    *(int *)((char *)this + nOffset - 0x44) = nOffset - 0xe8;
    nOffset = pOffsets[6];
    *(int *)((char *)this + nOffset - 0x44) = nOffset - 0x15c;

    pRecordSlotTable = *(VsNetDeleteObject **)((char *)this + pOffsets[7] - 0x40);
    if (pRecordSlotTable != 0) {
        pRecordSlotTable->m_pVtable->m_pDelete(pRecordSlotTable, 1);
    }

    if (g_pszFileBasedNetworkConfiguredPath != 0) {
        FreeVSMemBlock(g_pszFileBasedNetworkConfiguredPath);
        g_pszFileBasedNetworkConfiguredPath = 0;
    }

    if (g_pGlobalStateEff512ByteStream != 0) {
        ((VsNetDeleteObject *)g_pGlobalStateEff512ByteStream)
            ->m_pVtable->m_pDelete((VsNetDeleteObject *)g_pGlobalStateEff512ByteStream, 1);
        g_pGlobalStateEff512ByteStream = 0;
    }

    ((VsNetRuntimeChannelStackReleaseFront *)((char *)this - 0x3c))->Release();
    ((VsNetEffStreamBase *)((char *)this - 0x3c))->DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x0047BB60
void *DeleteGlobalStateFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    VsNetGlobalStateFileBackedEffCompositeWrapper *pWrapper;
    VsNetFileBackedCompositeWrapperView *pView;
    char *pbAllocationBase;

    pWrapper = (VsNetGlobalStateFileBackedEffCompositeWrapper *)((char *)pObject - 0x60);
    pView = &pWrapper->m_Runtime60;
    pbAllocationBase = (char *)pWrapper;
    ((VsNetGlobalStateCompositeDestroyView *)pObject)
        ->DestroyGlobalStateFileBackedEffComposite();
    ((VsNetTimedFileBackedEffChannelVtableRestoreView *)((char *)pObject + 0x154))
        ->RestoreTimedFileBackedEffChannelVtables();
    pView->m_pResetThunk104 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBuffer108 != 0) {
        FreeVSMemBlock(pView->m_pSideBuffer108);
    }
    ((VsNetDualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((VsNetTimedEffStream *)&pView->m_TimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x0047BC50
void *AdjustAndDeleteGlobalStateDualFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteGlobalStateFileBackedEffCompositeWrapper(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0xa8,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047AF30
void *VsNetDualSlotCompositeLayout::ConstructDualSlotTableFileBackedEffComposite(
    int fConstructEmbeddedObjects) {
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    VsNetAdjustorSubobject *pQuaternaryThunk;
    VsNetAdjustorSubobject *pFatalThunk;
    VsNetCompositeViewOffsets *pCompositeOffsets;
    const VsNetEmbeddedStackViewOffsets *pEmbeddedOffsets;
    const VsNetEmbeddedStackFileViewOffsets *pFileViewOffsets;
    VsNetDualSlotCompositeLayout *pFront;
    VsNetDualSlotCompositeOpenWrapperFront *pOpenWrapperFront;
    VsNetEmbeddedFileBackedStackFront *pEmbeddedStackFront;
    VsNetFileBackedCompositeInitialStream *pInitialStream;
    VsNetEffStreamRecordSlotTable *pRecordSlotTable;
    char *pbObjectBase;
    char *pbEmbeddedStackBase;
    void *pStorage;

    pbObjectBase = (char *)this;
    pFront = this;
    pCompositeOffsets = pFront->m_pTimedStreamOffsets2c;
    pOpenWrapperFront = (VsNetDualSlotCompositeOpenWrapperFront *)this;
    pEmbeddedStackFront = (VsNetEmbeddedFileBackedStackFront *)(pbObjectBase + 0x1b4);

    if (fConstructEmbeddedObjects != 0) {
        pFront->m_pTimedStreamOffsets2c = (VsNetCompositeViewOffsets *)&g_NETWORK_DualSlotCompositeTimedStreamOffsets;
        pFront->m_pTransportOffsets00 = (VsNetCompositeViewOffsets *)&g_NETWORK_DualSlotCompositeTransportOffsets;
        *(void **)(pbObjectBase + 0xd8) = (void *)&g_NETWORK_DualSlotCompositeDualStreamOffsets;
        pFront->m_pDualThunkOffsets150 = (VsNetCompositeViewOffsets *)&g_NETWORK_DualSlotCompositeDualThunkOffsets;
        pFront->m_pEmbeddedStackTransportOffsets164 = (VsNetEmbeddedStackViewOffsets *)&g_NETWORK_DualSlotCompositeFileWrapperOffsets;
        pFront->m_pEmbeddedStackFileViewOffsets184 = &g_NETWORK_DualSlotCompositeTimedFileThunkOffsets;
        pFront->m_pEmbeddedStackTimedStreamOffsets198 = (VsNetEmbeddedStackViewOffsets *)&g_NETWORK_DualSlotCompositeEmbeddedStackOffsets;
        pFront->m_pEmbeddedStackDualStreamOffsets1ac = (VsNetEmbeddedStackFileViewOffsets *)&g_NETWORK_DualSlotCompositeEmbeddedStackDataOffsets;
        pEmbeddedStackFront->m_pFinalThunkOffsets = &g_NETWORK_DualSlotCompositeEmbeddedStackFinalOffsets;

        pFront->m_ChannelState64.ConstructEffStreamChannelState();
        ((VsNetTimedEffStream *)pFront->m_abTimedStream94)->ConstructTimedEffStream(0);
        ConstructDualHandleEffStream(pFront->m_abDualStream10c, 0);

        pbEmbeddedStackBase = pbObjectBase + 0x164;
        pEmbeddedOffsets = *(VsNetEmbeddedStackViewOffsets **)pbEmbeddedStackBase;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nChannelStateViewOffset04 - 4);
        pSecondaryThunk = (VsNetAdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 4);
        pTertiaryThunk = (VsNetAdjustorSubobject *)(pbEmbeddedStackBase + pEmbeddedOffsets->m_nDualStreamViewOffset0c - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_RuntimeChannelStackFatalThunkVtable;
        pSecondaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackTimedStreamVtable;
        pTertiaryThunk->m_pVtable =
            g_NETWORK_RuntimeChannelStackDualStreamVtable;
        pPrimaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nChannelStateViewOffset04 - 8;
        pSecondaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nTimedStreamViewOffset08 - 0x38;
        pTertiaryThunk->m_nThisDelta = pEmbeddedOffsets->m_nDualStreamViewOffset0c - 0xb0;

        pOpenWrapperFront->m_FileWrapper168.m_nReserved04 = 0;
        pOpenWrapperFront->m_FileWrapper168.m_pVtable = g_NETWORK_CrtFatalRuntimeErrorThunk;
        pOpenWrapperFront->m_FileWrapper168.m_fOpen08 = 1;
        pOpenWrapperFront->m_FileWrapper168.m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pOpenWrapperFront->m_FileWrapper168.m_pDeleteThunk14 = g_NETWORK_CrtFatalRuntimeErrorDeleteThunk;
        pFileViewOffsets = pOpenWrapperFront->m_pFileWrapperThunk184;
        pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x184 + pFileViewOffsets->m_nFileWrapperViewOffset04 - 4);
        pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x184 + pFileViewOffsets->m_nChannelStateViewOffset08 - 4);
        pPrimaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
        pSecondaryThunk->m_pVtable = g_NETWORK_MapNestedEffCallbackFailureThunkVtable;

        ((VsNetDualFileBackedEffChannelConstructorView *)(pbObjectBase + 0x18c))
            ->ConstructDualFileBackedEffChannel(0);
        ((VsNetTimedFileBackedEffChannelConstructorView *)(pbObjectBase + 0x1a0))
            ->ConstructTimedFileBackedEffChannel(0);
        ((VsNetEmbeddedFileBackedEffChannelStackConstructorView *)(pbObjectBase + 0x1b4))
            ->ConstructEmbeddedFileBackedEffChannelStack(0);
    }

    pInitialStream = &pFront->m_InitialStream30;
    pInitialStream->m_nReserved04 = 0;
    pInitialStream->m_pVtable = g_GAME_EffStreamConstructionVtable;
    ((GameEffStream *)pInitialStream)->ResetStateFields();
    pInitialStream->m_nSerializedLength18 += 2;
    pInitialStream->m_wFlags2c = 0;
    pInitialStream->m_pVtable = g_NETWORK_GlobalStateCompositeInitialVtable;

    pCompositeOffsets = pFront->m_pTimedStreamOffsets2c;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nTransportViewOffset04 - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nTimedStreamViewOffset08 - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nDualStreamViewOffset0c - 4);
    pQuaternaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nDualThunkViewOffset10 - 4);
    pFatalThunk = (VsNetAdjustorSubobject *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nMarkerViewOffset14 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_GlobalStateCompositeInitialDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_OpenWin32FileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_FileBackedFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = 0;
    pSecondaryThunk->m_nThisDelta = 0;
    pTertiaryThunk->m_nThisDelta = 0;

    ((VsNetCompositeEffTransportStackLayout *)this)->ConstructCompositeEffTransportStack(0);

    /* The socket vtable is owned by another translation unit.  Resolve its
     * shared slot only after CRT/global initialization has completed. */
    if (g_NETWORK_DualSlotCompositeFullVtableModel[10] == 0) {
        g_NETWORK_DualSlotCompositeFullVtableModel[10] =
            ((void **)g_NETWORK_SocketWindowEffChannelVtable)[0];
    }

    *(void **)this = g_NETWORK_DualSlotCompositeFinalVtable;
    pPrimaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_FileBackedFinalFileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pCompositeOffsets->m_nTransportViewOffset04 - 0x38;
    pSecondaryThunk->m_nThisDelta = pCompositeOffsets->m_nTimedStreamViewOffset08 - 0x68;
    pTertiaryThunk->m_nThisDelta = pCompositeOffsets->m_nDualStreamViewOffset0c - 0xe0;
    pFatalThunk->m_nThisDelta = pCompositeOffsets->m_nMarkerViewOffset14 - 0x154;

    *(int *)(pbObjectBase + pCompositeOffsets->m_nRecordTableAOffset18 + 0x34) = 3;

    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nRecordTableBOffset1c) = 0;
    } else {
        pRecordSlotTable =
            ((VsNetEffStreamRecordSlotTable *)pStorage)
                ->Construct(*(int *)(pbObjectBase + pCompositeOffsets->m_nRecordTableAOffset18 + 0x34));
        *(VsNetEffStreamRecordSlotTable **)(pbObjectBase + 0x2c + pCompositeOffsets->m_nRecordTableBOffset1c) =
            pRecordSlotTable;
    }

    pStorage = AllocateVSMemBlock(0x3c);
    if (pStorage == 0) {
        *(int *)(pbObjectBase + 0x2c + pCompositeOffsets->m_nRecordTableCOffset20) = 0;
    } else {
        pRecordSlotTable =
            ((VsNetEffStreamRecordSlotTable *)pStorage)
                ->Construct(*(int *)(pbObjectBase + pCompositeOffsets->m_nRecordTableAOffset18 + 0x34));
        *(VsNetEffStreamRecordSlotTable **)(pbObjectBase + 0x2c + pCompositeOffsets->m_nRecordTableCOffset20) =
            pRecordSlotTable;
    }

    *(int *)((char *)&pFront->m_InitialStream30 + pCompositeOffsets->m_nRecordTableBOffset1c) =
        pInitialStream->m_nSerializedLength18;
    *(int *)((char *)&pFront->m_InitialStream30 + pCompositeOffsets->m_nRecordTableCOffset20) =
        pInitialStream->m_nSerializedLength18;
    return (void *)(char *)this;
}

// FUNCTION: LEMBALL 0x0047B2A0
void VsNetDualSlotCompositeDestroyView::DestroyDualSlotTableFileBackedEffComposite(void) {
    VsNetAdjustorSubobject *pPrimaryThunk;
    VsNetAdjustorSubobject *pSecondaryThunk;
    VsNetAdjustorSubobject *pTertiaryThunk;
    VsNetAdjustorSubobject *pQuaternaryThunk;
    VsNetAdjustorSubobject *pFatalThunk;
    VsNetCompositeViewOffsets *pCompositeOffsets;
    VsNetDualSlotCompositeLayout *pFront;
    char *pbObjectBase;
    VsNetDeleteObject *pRecordTableA;
    VsNetDeleteObject *pRecordTableB;
    pbObjectBase = (char *)this - 0x38;
    pFront = (VsNetDualSlotCompositeLayout *)pbObjectBase;
    pCompositeOffsets = pFront->m_pTimedStreamOffsets2c;
    *((void **)pbObjectBase - 0xb) = g_NETWORK_DualSlotCompositeFinalVtable;
    pPrimaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pCompositeOffsets->m_nTransportViewOffset04 - 4);
    pSecondaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pCompositeOffsets->m_nTimedStreamViewOffset08 - 4);
    pTertiaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pCompositeOffsets->m_nDualStreamViewOffset0c - 4);
    pQuaternaryThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pCompositeOffsets->m_nDualThunkViewOffset10 - 4);
    pFatalThunk = (VsNetAdjustorSubobject *)(pbObjectBase + pCompositeOffsets->m_nMarkerViewOffset14 - 4);
    pPrimaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalPrimaryThunkVtable;
    pSecondaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalTimedThunkVtable;
    pTertiaryThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalDualThunkVtable;
    pQuaternaryThunk->m_pVtable = g_NETWORK_FileBackedFinalFileWrapperVtable;
    pFatalThunk->m_pVtable = g_NETWORK_DualSlotCompositeFinalFatalFileWrapperVtable;
    pPrimaryThunk->m_nThisDelta = pCompositeOffsets->m_nTransportViewOffset04 - 0x38;
    pSecondaryThunk->m_nThisDelta = pCompositeOffsets->m_nTimedStreamViewOffset08 - 0x68;
    pTertiaryThunk->m_nThisDelta = pCompositeOffsets->m_nDualStreamViewOffset0c - 0xe0;
    pFatalThunk->m_nThisDelta = pCompositeOffsets->m_nMarkerViewOffset14 - 0x154;

    pRecordTableA = *(VsNetDeleteObject **)(pbObjectBase + pCompositeOffsets->m_nRecordTableBOffset1c);
    if (pRecordTableA != 0) {
        pRecordTableA->m_pVtable->m_pDelete(pRecordTableA, 1);
    }

    pRecordTableB = *(VsNetDeleteObject **)(pbObjectBase + pCompositeOffsets->m_nRecordTableCOffset20);
    if (pRecordTableB != 0) {
        pRecordTableB->m_pVtable->m_pDelete(pRecordTableB, 1);
    }

    RestoreCompositeEffTransportVtables((int)(unsigned long)this - 0x34);
    ((VsNetEffStreamBase *)((char *)this - 0x34))->DestroyEffStreamBase();
}

// FUNCTION: LEMBALL 0x0047B370
void __fastcall WriteDualSlotTableFileBackedEffHeader(void *pObjectBasePlus0x154) {
    VsNetFileBackedMarkerOwner *pOwner;
    VsNetFileBackedDispatchOffsets *pOffsets;
    VsNetFileWrapperObject *pReadThunk;
    VsNetFileWrapperObject *pWriteThunk;
    unsigned long ulObjectBase;

    pOwner = (VsNetFileBackedMarkerOwner *)((char *)pObjectBasePlus0x154 - 0x154);
    ulObjectBase = (unsigned long)pOwner;
    pOffsets = (VsNetFileBackedDispatchOffsets *)pOwner->m_pOffsets;
    pReadThunk = (VsNetFileWrapperObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (VsNetFileWrapperObject *)(unsigned long)(pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    ((VsNetFileBackedLockedWriterView *)pWriteThunk)
        ->WriteEffStreamToLockedFile((int *)&pOwner->m_Marker04, 0, 0);
}

// FUNCTION: LEMBALL 0x0047C040
void AdjustAndWriteDualSlotTableFileBackedEffHeader(void *pObject) {
    WriteDualSlotTableFileBackedEffHeader(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta);
}

// FUNCTION: LEMBALL 0x0047B9D0
void *VsNetFileBackedCompositeWrapperView::DeleteCompositeFileBackedEffChannelWrapper(
    BYTE fFreeMemory) {
    VsNetFileBackedCompositeChannelWrapper *pWrapper;
    VsNetFileBackedCompositeWrapperView *pView;
    char *pbAllocationBase;
    void *pObject;

    pObject = (void *)this;
    pWrapper = (VsNetFileBackedCompositeChannelWrapper *)((char *)this - 0x38);
    pView = &pWrapper->m_Runtime38;
    pbAllocationBase = (char *)pWrapper;
    ((VsNetEffStreamBase *)((char *)this - 0x34))->DestroyEffStreamBase();
    ((VsNetTimedFileBackedEffChannelVtableRestoreView *)((char *)this + 0x150))
        ->RestoreTimedFileBackedEffChannelVtables();
    pView->m_pResetThunk104 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBuffer108 != 0) {
        FreeVSMemBlock(pView->m_pSideBuffer108);
    }
    ((VsNetDualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((VsNetTimedEffStream *)&pView->m_TimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x0047B9C0
void *AdjustAndDeleteCompositeFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory) {
    return ((VsNetFileBackedCompositeWrapperView *)
                ((char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta))
        ->DeleteCompositeFileBackedEffChannelWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BCC0
void *VsNetFileBackedTimedChannelWrapperView::DeleteTimedFileBackedEffChannelWrapper(
    BYTE fFreeMemory) {
    VsNetFileBackedTimedChannelWrapperView *pView;
    char *pbAllocationBase;
    void *pObject;

    pObject = (void *)this;
    pView = this;
    pbAllocationBase = (char *)this - 0x18;
    ((VsNetTimedFileBackedEffChannelVtableRestoreView *)this)
        ->RestoreTimedFileBackedEffChannelVtables();
    pView->m_pResetThunkA4 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBufferA8 != 0) {
        FreeVSMemBlock(pView->m_pSideBufferA8);
    }
    ((VsNetTimedEffStream *)&pView->m_TimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x0047BCB0
void *AdjustAndDeleteTimedFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetFileBackedTimedChannelWrapperView *)pObject)
        ->DeleteTimedFileBackedEffChannelWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BD50
void *AdjustAndDeleteTimedFileBackedEffChannelWrapper30(void *pObject, BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x30;
    return ((VsNetFileBackedTimedChannelWrapperView *)pObject)
        ->DeleteTimedFileBackedEffChannelWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x00462970
void SetTimedFileBackedEffChannelWord(void *pObject, unsigned short nValue) {
    VsNetTimedEffStream *pTimedStream;
    VsNetConstructionAdjustorVtable *pOffsets;
    int nTimedStreamDelta;

    pTimedStream = (VsNetTimedEffStream *)pObject;
    pOffsets = (VsNetConstructionAdjustorVtable *)
        pTimedStream->m_pChannelStateConstructionOffsets44;
    nTimedStreamDelta = pOffsets->m_nPrimaryOffset;
    *(unsigned short *)((char *)pObject + nTimedStreamDelta + 0x64) = nValue;
}

struct VsNetDualFileBackedEffChannelDeleteView {
    unsigned char m_abReserved00[0x84];
    void *m_pFatalVtable84;
    void *m_pSideBuffer88;
    void *DeleteDualFileBackedEffChannelWrapper(BYTE fFreeMemory);
};

// FUNCTION: LEMBALL 0x0047BDA0
void *VsNetDualFileBackedEffChannelDeleteView::DeleteDualFileBackedEffChannelWrapper(
    BYTE fFreeMemory) {
    VsNetDualFileBackedEffChannelDeleteView *pView;
    void *pObject;

    pObject = (void *)this;
    pView = this;
    pView->m_pFatalVtable84 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBuffer88 != 0) {
        FreeVSMemBlock(pView->m_pSideBuffer88);
    }
    ((VsNetDualHandleEffStream *)((char *)pObject + 0x2c))->DestroyDualHandleEffStream();
    ((VsNetEffStreamChannelState *)pObject)->DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock((char *)pObject - 0x18);
    }
    return (char *)pObject - 0x18;
}

// FUNCTION: LEMBALL 0x0047BD90
void *AdjustAndDeleteDualFileBackedEffChannelWrapper(void *pObject, BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetDualFileBackedEffChannelDeleteView *)pObject)
        ->DeleteDualFileBackedEffChannelWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BE00
void *DeleteDualFileBackedEffChannelNoopThunk(void *pObject, BYTE fFreeMemory) {
    (void)fFreeMemory;
    return pObject;
}

// FUNCTION: LEMBALL 0x0047BE20
void *AdjustAndDeleteDualFileBackedEffChannelWrapper2c(void *pObject, BYTE fFreeMemory) {
    return ((VsNetDualFileBackedEffChannelDeleteView *)((char *)pObject - 0x2c))
        ->DeleteDualFileBackedEffChannelWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BB50
void *AdjustAndDeleteGlobalStateFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteGlobalStateFileBackedEffCompositeWrapper(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BC00
void *AdjustAndDeleteGlobalStateTimedFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return DeleteGlobalStateFileBackedEffCompositeWrapper(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0x30,
        fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BC90
void InitializeGlobalStateFileBackedEffStorageAdjustor(void *pObject) {
    InitializeGlobalStateFileBackedEffStorage((int)(unsigned long)((char *)pObject -
                                                                   ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))
                                                                       ->m_nThisDelta));
}

// FUNCTION: LEMBALL 0x0047BA50
void LEMBALL_FASTCALL AdjustAndClearGlobalStateTimedEffTransportPendingWriteState(
    void *pObject, void *pUnused) {
    ((VsNetAdjustedEffTransportPendingWriteState *)
         ((char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta + 0x154))
        ->ClearPendingWrite(pUnused);
}

// FUNCTION: LEMBALL 0x0047BA60
void VsNetAdjustedEffTransportPendingWriteState::ClearPendingWrite(void *pUnused) {
    char *pbTransportView;
    char *pPendingWrite;

    pbTransportView = (char *)this +
                      (*(VsNetGlobalSessionAdjustorOffsets **)((char *)this - 8))->m_nTransportViewOffset10;
    pPendingWrite = pbTransportView +
                    (*(VsNetGlobalSessionAdjustorOffsets **)(pbTransportView - 8))
                        ->m_nEmbeddedStreamViewOffset08 + 0x70;
    ((VsNetEffTransportPendingWriteState *)pPendingWrite)->Clear(pUnused);
}

// FUNCTION: LEMBALL 0x0047BAA0
void VsNetGlobalSessionAdjustorFront::WriteAdjustedEffStreamWithGlobalSession(void) {
    char *pbTransportView;

    pbTransportView = (char *)this +
                      ((VsNetGlobalSessionAdjustorFront *)((char *)this - 0x38))->m_pOffsets->m_nTransportViewOffset10;
    ((VsNetPeerPayloadSender *)(pbTransportView +
                                   ((VsNetGlobalSessionAdjustorFront *)(pbTransportView - 0x38))->m_pOffsets
                                       ->m_nEmbeddedStreamViewOffset08 -
                                   0x38))
        ->WriteEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BAD0
void LEMBALL_FASTCALL ClaimAdjustedEffTransportRecordPayload(void *pObject) {
    char *pbTransportView;

    pbTransportView = (char *)pObject +
                      ((VsNetGlobalSessionAdjustorFront *)((char *)pObject - 0x38))->m_pOffsets
                          ->m_nTransportViewOffset10;
    ((VsNetAckedEffTransportRecordOwner *)
         (pbTransportView +
          ((VsNetGlobalSessionAdjustorFront *)(pbTransportView - 0x38))->m_pOffsets
              ->m_nEmbeddedStreamViewOffset08 - 0x38))
        ->ClaimAckedEffTransportRecordPayload();
}

// FUNCTION: LEMBALL 0x0047BA90
void AdjustAndWriteGlobalStateTimedEffStreamWithGlobalSession(void *pObject) {
    ((VsNetGlobalSessionAdjustorFront *)((char *)pObject -
                                           ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta +
                                           0x154))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BAC0
void AdjustAndClaimGlobalStateTimedEffTransportRecordPayload(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta + 0x154);
}

// FUNCTION: LEMBALL 0x0047BB20
void AdjustAndWriteGlobalStateDualEffStreamWithGlobalSession(void *pObject) {
    ((VsNetGlobalSessionAdjustorFront *)((char *)pObject -
                                           ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta +
                                           0xdc))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BB30
void AdjustAndClaimGlobalStateDualEffTransportRecordPayload(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta + 0xdc);
}

// FUNCTION: LEMBALL 0x0047BAF0
void AdjustAndInvokeGlobalStateTimedEffStreamServiceCallback(void *pObject, void *pArgument) {
    ((VsNetAdjustedTimedEffStreamServiceView *)((char *)pObject + 0x150))
        ->InvokeAdjustedTimedEffStreamServiceCallback(pArgument);
}

// FUNCTION: LEMBALL 0x0047BBF0
void CloseAdjustedEffTransportPeerByKeyWithArgument(void *pObject, void *pUnused) {
    ((VsNetAdjustedEffTransportPeerView *)((char *)pObject - 0x3c))
        ->CloseAdjustedEffTransportPeerByKey(pUnused);
}

// FUNCTION: LEMBALL 0x0047BBD0
void AdjustAndClearEmbeddedEffChannelAsyncStatus13c(void *pObject) {
    ClearNestedEffChannelAsyncStatus((char *)pObject + 0x13c);
}

// FUNCTION: LEMBALL 0x0047BBE0
void AdjustAndCloseEffTransportPeerByKey(void *pObject, void *pArgument) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    CloseAdjustedEffTransportPeerByKeyWithArgument(pObject, pArgument);
}

// FUNCTION: LEMBALL 0x0047BB10
void *AdjustAndDeleteGlobalStateCompositeAtA8(void *pObject, BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0xa8;
    return ((VsNetFileBackedCompositeWrapperView *)pObject)
        ->DeleteCompositeFileBackedEffChannelWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BA80
void *AdjustAndDeleteGlobalStateCompositeAt30(void *pObject, BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4) - 0x30;
    return ((VsNetFileBackedCompositeWrapperView *)pObject)
        ->DeleteCompositeFileBackedEffChannelWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BC10
void AdjustAndWriteGlobalStateTimedEffStreamFrom158(void *pObject) {
    ((VsNetGlobalSessionAdjustorFront *)((char *)pObject - *(int *)((char *)pObject - 4) + 0x158))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BC20
void AdjustAndClaimGlobalStateTimedEffStreamFrom158(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - *(int *)((char *)pObject - 4) + 0x158);
}

// FUNCTION: LEMBALL 0x0047BC30
void AdjustAndInvokeTimedEffStreamServiceFrom154(void *pObject, void *pArgument) {
    ((VsNetAdjustedTimedEffStreamServiceView *)((char *)pObject + 0x154))
        ->InvokeAdjustedTimedEffStreamServiceCallback(pArgument);
}

// FUNCTION: LEMBALL 0x0047BC60
void AdjustAndWriteDualSlotTimedEffStreamWithGlobalSession(void *pObject) {
    ((VsNetGlobalSessionAdjustorFront *)((char *)pObject -
                                           ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta +
                                           0xe0))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BC70
void AdjustAndClaimDualSlotTimedEffTransportRecordPayload(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta + 0xe0);
}

// FUNCTION: LEMBALL 0x0047BC80
void DispatchAdjustedPrefixedEffTransportControlStream(void *pObject, int nDispatchType, void *pEvent) {
    ((VsNetPrefixedEffTransportControlView *)((char *)pObject - 0x3c))
        ->DispatchPrefixedEffTransportControlStream(nDispatchType, pEvent);
}

// FUNCTION: LEMBALL 0x0047BE50
void *VsNetFileBackedCompositeWrapperView::DeleteEmbeddedFileBackedEffChannelStackWrapper(
    BYTE fFreeMemory) {
    VsNetEmbeddedFileBackedEffChannelStackWrapper *pWrapper;
    VsNetFileBackedCompositeWrapperView *pView;
    char *pbAllocationBase;
    void *pObject;

    pObject = (void *)this;
    pWrapper = (VsNetEmbeddedFileBackedEffChannelStackWrapper *)((char *)this - 8);
    pView = &pWrapper->m_Runtime08;
    pbAllocationBase = (char *)pWrapper;
    ((VsNetTimedFileBackedEffChannelVtableRestoreView *)((char *)this + 0x150))
        ->RestoreTimedFileBackedEffChannelVtables();
    pView->m_pResetThunk104 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBuffer108 != 0) {
        FreeVSMemBlock(pView->m_pSideBuffer108);
    }
    ((VsNetDualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((VsNetTimedEffStream *)&pView->m_TimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x0047BEC0
void *AdjustAndDeleteEmbeddedTimedFileBackedEffChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    return ((VsNetFileBackedCompositeWrapperView *)
                ((char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0x30))
        ->DeleteEmbeddedFileBackedEffChannelStackWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BED0
void AdjustAndWriteEmbeddedTimedEffStreamWithGlobalSession(void *pObject) {
    ((VsNetGlobalSessionAdjustorFront *)((char *)pObject -
                                           ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BEE0
void AdjustAndClaimEmbeddedTimedEffTransportRecordPayload(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta);
}

// FUNCTION: LEMBALL 0x0047BF00
void *AdjustAndDeleteEmbeddedDualFileBackedEffChannelStackWrapper(void *pObject, BYTE fFreeMemory) {
    return ((VsNetFileBackedCompositeWrapperView *)
                ((char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0xa8))
        ->DeleteEmbeddedFileBackedEffChannelStackWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BF10
void AdjustAndWriteEmbeddedDualEffStreamWithGlobalSession(void *pObject) {
    ((VsNetGlobalSessionAdjustorFront *)((char *)pObject -
                                           ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta -
                                           0x78))
        ->WriteAdjustedEffStreamWithGlobalSession();
}

// FUNCTION: LEMBALL 0x0047BF20
void AdjustAndClaimEmbeddedDualEffTransportRecordPayload(void *pObject) {
    ClaimAdjustedEffTransportRecordPayload(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0x78);
}

// FUNCTION: LEMBALL 0x0047BF40
void *VsNetFileBackedCompositeWrapperView::DeleteDualSlotTableFileBackedEffCompositeWrapper(
    BYTE fFreeMemory) {
    VsNetDualSlotTableFileBackedEffCompositeWrapper *pWrapper;
    VsNetFileBackedCompositeWrapperView *pView;
    char *pbAllocationBase;
    void *pObject;

    pObject = (void *)this;
    pWrapper = (VsNetDualSlotTableFileBackedEffCompositeWrapper *)((char *)this - 100);
    pView = &pWrapper->m_Runtime64;
    pbAllocationBase = (char *)pWrapper;
    ((VsNetDualSlotCompositeDestroyView *)this)
        ->DestroyDualSlotTableFileBackedEffComposite();
    ((VsNetTimedFileBackedEffChannelVtableRestoreView *)((char *)this + 0x154))
        ->RestoreTimedFileBackedEffChannelVtables();
    pView->m_pResetThunk104 = g_NETWORK_CrtFatalRuntimeErrorThunk;
    if (pView->m_pSideBuffer108 != 0) {
        FreeVSMemBlock(pView->m_pSideBuffer108);
    }
    ((VsNetDualHandleEffStream *)pView->m_abDualStreamA8)->DestroyDualHandleEffStream();
    ((VsNetTimedEffStream *)&pView->m_TimedStream30)->DestroyTimedEffStream();
    pView->m_ChannelState00.DestroyEffStreamChannelState();
    if ((fFreeMemory & 1) != 0) {
        FreeVSMemBlock(pbAllocationBase);
    }
    return pbAllocationBase;
}

// FUNCTION: LEMBALL 0x0047BFD0
void *AdjustAndDeleteDualSlotTableTimedFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return ((VsNetFileBackedCompositeWrapperView *)
                ((char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0x30))
        ->DeleteDualSlotTableFileBackedEffCompositeWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BF30
void *AdjustAndDeleteDualSlotTableFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return ((VsNetFileBackedCompositeWrapperView *)
                ((char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta))
        ->DeleteDualSlotTableFileBackedEffCompositeWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047C000
void *AdjustAndDeleteDualSlotTableDualFileBackedEffCompositeWrapper(void *pObject, BYTE fFreeMemory) {
    return ((VsNetFileBackedCompositeWrapperView *)
                ((char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0xa8))
        ->DeleteDualSlotTableFileBackedEffCompositeWrapper(fFreeMemory);
}

// FUNCTION: LEMBALL 0x0047BFF0
void DispatchAdjustedAckedEffTransportPayloadEvent(void *pObject) {
    ((VsNetDispatchAckedEffTransportPayloadView *)((char *)pObject - 0x34))
        ->DispatchAckedEffTransportPayloadEvent();
}

// FUNCTION: LEMBALL 0x0047BFE0
void DispatchAdjustedAckedEffTransportPayloadEventFromEmbeddedThunk(void *pObject) {
    DispatchAdjustedAckedEffTransportPayloadEvent(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta);
}

// FUNCTION: LEMBALL 0x0047C010
void QueueAdjustedEffTransportConnectEvent(void *pObject) {
    ((VsNetEffTransportConnectCallback *)((char *)pObject - 0x34))
        ->QueueEffTransportConnectEvent();
}

// FUNCTION: LEMBALL 0x0047C020
void DispatchAdjustedAckedEffTransportPayloadEventFromDualThunk(void *pObject) {
    DispatchAdjustedAckedEffTransportPayloadEvent(
        (char *)pObject - ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 0x78);
}

// FUNCTION: LEMBALL 0x0047C030
void QueueAdjustedEffTransportPayloadEvent(void *pObject, unsigned short nType, void *pPayload) {
    ((VsNetQueueEffTransportPayloadEventView *)((char *)pObject - 0x34))
        ->QueueEffTransportPayloadEvent(nType, pPayload);
}

// FUNCTION: LEMBALL 0x00479880
int OpenFileBackedEffHandleWithRetry(void *pObject, int nHandle) {
    VsNetFileBackedOpenRetryOwner *pOwner;
    VsNetFileWrapperOpenObject *pOpenThunk;
    DWORD dwStartTime;
    DWORD dwNow;

    pOwner = (VsNetFileBackedOpenRetryOwner *)pObject;
    pOpenThunk = (VsNetFileWrapperOpenObject *)((char *)pOwner + pOwner->m_pOffsets->m_nOutputStateOffset04 + 4);

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
BYTE __fastcall MapNestedEffCallbackFailureToStatus6(void *pObject) {
    VsNetFileBackedCallbackStatusFront *pFront;
    VsNetFileBackedAppendOwner *pOwner;
    VsNetCallbackThunkObject *pNestedThunk;
    int nResult;

    pFront = (VsNetFileBackedCallbackStatusFront *)pObject;
    pOwner = (VsNetFileBackedAppendOwner *)((char *)pFront - 0x1c);
    pNestedThunk = (VsNetCallbackThunkObject *)((char *)pOwner + pOwner->m_pOffsets->m_nOutputStateOffset04);
    nResult = pNestedThunk->m_pVtable->m_pInvoke(pNestedThunk);
    return (BYTE)(-(nResult == 0) & 6);
}

// FUNCTION: LEMBALL 0x0047A090
void VsNetAdjustedTimedEffStreamServiceView::InvokeAdjustedTimedEffStreamServiceCallback(void *pArgument) {
    int nTimedStreamDelta;

    nTimedStreamDelta = *(int *)(unsigned long)(*(int *)((char *)this - 0x3c) + 8);
    ((VsNetTimedEffStream *)((char *)this + nTimedStreamDelta - 0x3c))
        ->InvokeTimedEffStreamServiceCallback(pArgument);
}

// FUNCTION: LEMBALL 0x0047BD60
void InvokeAdjustedTimedEffStreamServiceCallbackThunk(void *pObject, void *pArgument) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    ((VsNetAdjustedTimedEffStreamServiceView *)pObject)
        ->InvokeAdjustedTimedEffStreamServiceCallback(pArgument);
}

class VsNetLockedEffStreamFileRangeView {
    char m_abReserved00[0x20];
    VsNetCompositeViewOffsets *m_pViewOffsets20;
    char m_abReserved24[0x18];
    DWORD m_dwFileOffset3c;
    char m_abReserved40[0x14];
    int m_fLoaded54;

public:
    int LoadLockedEffStreamFromFileRange(void);
    int WriteLockedEffStreamToFileRange(void);
};

// FUNCTION: LEMBALL 0x0047AC50
int VsNetLockedEffStreamFileRangeView::LoadLockedEffStreamFromFileRange(void) {
    VsNetFileWrapperObject *pFileWrapper;
    DWORD cbStream;
    DWORD dwOffset;
    int nResult;

    pFileWrapper = (VsNetFileWrapperObject *)((char *)this +
                                                 m_pViewOffsets20->m_nMarkerViewOffset14 + 0x20);
    dwOffset = m_dwFileOffset3c;
    pFileWrapper->m_pVtable->m_pSeek(pFileWrapper, dwOffset);
    cbStream = *(DWORD *)((char *)g_pGlobalStateEff512ByteStream + 0x18);
    nResult = LockWin32FileRange(pFileWrapper, dwOffset, cbStream);
    if (nResult == 0) {
        return 0;
    }
    nResult = ReadWin32FileWrapper(pFileWrapper, (LPVOID)g_pEffTransportPacketBuffer, cbStream);
    if (nResult != 0) {
    ((GameEffStream *)g_pGlobalStateEff512ByteStream)
        ->LoadEffStreamFromMemory((int)(unsigned long)g_pEffTransportPacketBuffer);
        m_fLoaded54 = 1;
        return 1;
    }
    UnlockWin32FileRange(pFileWrapper, dwOffset, cbStream);
    return 0;
}

// FUNCTION: LEMBALL 0x0047ACE0
int VsNetLockedEffStreamFileRangeView::WriteLockedEffStreamToFileRange(void) {
    VsNetFileWrapperObject *pFileWrapper;
    DWORD cbStream;
    DWORD dwOffset;
    int nResult;

    pFileWrapper = (VsNetFileWrapperObject *)((char *)this +
                                                 m_pViewOffsets20->m_nMarkerViewOffset14 + 0x20);
    ((GameEffStream *)g_pGlobalStateEff512ByteStream)->BeginEffStreamWriteSession();
    cbStream = *(DWORD *)((char *)g_pGlobalStateEff512ByteStream + 0x18);
    dwOffset = m_dwFileOffset3c;
    pFileWrapper->m_pVtable->m_pSeek(pFileWrapper, dwOffset);
    m_fLoaded54 = 0;
    nResult = WriteWin32FileWrapper(
        pFileWrapper,
        (const void *)(unsigned long)(*(int *)((char *)g_pGlobalStateEff512ByteStream + 8) + 0x10),
        cbStream);
    ((GameEffStream *)g_pGlobalStateEff512ByteStream)->EndEffStreamWriteSession();
    if (nResult != 0) {
        return UnlockWin32FileRange(pFileWrapper, dwOffset, cbStream) != 0;
    }
    UnlockWin32FileRange(pFileWrapper, dwOffset, cbStream);
    return 0;
}

// FUNCTION: LEMBALL 0x0047AD70
static int AllocateGlobalStateEffRecordSlot0047AD70(void *pObject) {
    unsigned char *pbSlotState;
    VsNetEff512ByteStateStream *pStateStream;
    VsNetCompositeViewOffsets *pOffsets;
    VsNetFileWrapperObject *pFileWrapper;
    DWORD cbStream;
    DWORD dwOffset;
    int nSlot;

    if (((VsNetLockedEffStreamFileRangeView *)pObject)->LoadLockedEffStreamFromFileRange() == 0) {
        return -1;
    }

    pStateStream = (VsNetEff512ByteStateStream *)g_pGlobalStateEff512ByteStream;
    pbSlotState = pStateStream->m_pbStateBytes2c;
    nSlot = 0;
    while (nSlot < 0x200 && pbSlotState[nSlot] == 0) {
        ++nSlot;
    }
    if (nSlot == 0x200) {
        pOffsets = *(VsNetCompositeViewOffsets **)((char *)pObject + 0x20);
        pFileWrapper = (VsNetFileWrapperObject *)((char *)pObject + 0x20 +
                                                     pOffsets->m_nMarkerViewOffset14);
        dwOffset = *(DWORD *)((char *)pObject + 0x3c);
        cbStream = *(DWORD *)((char *)pStateStream + 0x18);
        UnlockWin32FileRange(pFileWrapper, dwOffset, cbStream);
        return -1;
    }

    ++pbSlotState[nSlot];
    if (((VsNetLockedEffStreamFileRangeView *)pObject)->WriteLockedEffStreamToFileRange() == 0) {
        return -1;
    }
    return nSlot;
}

// FUNCTION: LEMBALL 0x0047AE00
static void ReleaseGlobalStateEffRecordSlot0047AE00(void *pObject, unsigned short nSlot) {
    VsNetGlobalStateCompositeLayout *pComposite;
    VsNetEff512ByteStateStream *pStateStream;
    unsigned char *pbSlotState;
    VsNetCompositeViewOffsets *pOffsets;
    VsNetFileWrapperObject *pFileWrapper;

    pComposite = (VsNetGlobalStateCompositeLayout *)pObject;
    pbSlotState = pComposite->m_pbGlobalStateSlotFlags10;
    pStateStream = (VsNetEff512ByteStateStream *)g_pGlobalStateEff512ByteStream;
    if (pbSlotState == 0 && pStateStream != 0) {
        /* Assumption: constructor-owned slot map aliases shared state bytes. */
        pbSlotState = pStateStream->m_pbStateBytes2c;
    }
    if (pbSlotState == 0) {
        return;
    }

    pbSlotState[nSlot] = 0;
    if (pComposite->m_fGlobalStateStreamLoaded54 == 0) {
        ((VsNetLockedEffStreamFileRangeView *)pObject)->LoadLockedEffStreamFromFileRange();
    }
    --pbSlotState[nSlot];
    ((VsNetLockedEffStreamFileRangeView *)pObject)->WriteLockedEffStreamToFileRange();
    if (pStateStream != 0 && HasNonzeroEff512ByteState((int)(unsigned long)pStateStream) == 0) {
        pOffsets = pComposite->m_pTimedStreamOffsets20;
        pFileWrapper = (VsNetFileWrapperObject *)((char *)pObject + 0x20 +
                                                     pOffsets->m_nMarkerViewOffset14);
        if (g_pszFileBasedNetworkConfiguredPath != 0) {
            pFileWrapper->m_pVtable->m_pFreePath(g_pszFileBasedNetworkConfiguredPath);
            g_pszFileBasedNetworkConfiguredPath = 0;
        }
    }
}

struct VsNetFileBackedPendingWriteAdjustedView {
    void ClearAdjustedPendingWriteState(void *pUnused);
};

// FUNCTION: LEMBALL 0x0047BD20
void AdjustAndClearPendingWriteStateThunk(void *pObject, void *pUnused) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    ((VsNetFileBackedPendingWriteAdjustedView *)pObject)
        ->ClearAdjustedPendingWriteState(pUnused);
}

// FUNCTION: LEMBALL 0x0047BD30
void VsNetFileBackedPendingWriteAdjustedView::ClearAdjustedPendingWriteState(void *pUnused) {
    VsNetGlobalSessionAdjustorOffsets *pOffsets;
    char *pPendingWrite;

    pOffsets = *(VsNetGlobalSessionAdjustorOffsets **)((char *)this - 0x0c);
    pPendingWrite = (char *)this + pOffsets->m_nEmbeddedStreamViewOffset08 + 0x6c;
    ((VsNetEffTransportPendingWriteState *)pPendingWrite)->Clear(pUnused);
}

struct VsNetFileBackedRecordTableView {
    VsNetEffStreamRecordSlotTable *m_pRecordTable00;
    int m_nRecordHeaderOffset04;
    int m_nPayloadBaseOffset08;
    VsNetFileBackedDispatchOffsets *m_pChannelThunk0c;

    int LoadEffStreamFromFileBackedRange(void *pStream, int fKeepLocked, int fAlreadyLocked);
    int LoadFileBackedEffRecordPayload(int nSlotIndex);
};

// FUNCTION: LEMBALL 0x00479A40
int VsNetFileBackedRecordTableView::LoadEffStreamFromFileBackedRange(void *pStream,
                                                                        int fKeepLocked,
                                                                        int fAlreadyLocked) {
    VsNetFileBackedRangeLoadView LoadView;
    VsNetFileBackedDispatchOffsets *pOffsets;
    VsNetFileWrapperObject *pFileWrapper;
    VsNetEffStreamRecordSlotTable *pRecordTable;
    VsNetEffStreamSerializedLengthView *pSerializedView;
    char *pbRecordTableBase;
    DWORD dwOffset;
    DWORD cbRead;
    DWORD dwStartTime;
    DWORD dwNow;

    pRecordTable = m_pRecordTable00;
    pbRecordTableBase = (char *)this;
    pOffsets = m_pChannelThunk0c;
    LoadView.m_pRecordTable = pRecordTable;
    LoadView.m_pFileWrapper =
        (VsNetFileWrapperObject *)(pbRecordTableBase + 0xc + pOffsets->m_nFileWrapperViewOffset0c);
    pSerializedView = (VsNetEffStreamSerializedLengthView *)pStream;
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

    ((GameEffStream *)pStream)->LoadEffStreamFromMemory((int)(unsigned long)g_pEffTransportPacketBuffer);
    if (fKeepLocked == 0 && pFileWrapper->m_pVtable->m_pUnlockRange(pFileWrapper, dwOffset, cbRead) == 0) {
        return 0;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x00479B30
int VsNetFileBackedRecordTableView::LoadFileBackedEffRecordPayload(int nSlotIndex) {
    VsNetFileBackedRecordPayloadView PayloadView;
    VsNetFileBackedDispatchOffsets *pChannelOffsets;
    VsNetEffStreamRecordSlotTable *pRecordTable;
    VsNetEffStreamRecordSlot *pSlot;
    VsNetFileWrapperObject *pFileWrapper;
    VsNetChannelStateHeader *pChannelState;
    VsNetTimedStreamHeader *pTimedStream;
    VsNetChannelStateRuntimeVtable *pChannelStateVtable;
    VsNetTimedStreamRuntimeVtable *pTimedStreamVtable;
    VsNetPeerAddressService *pPeerAddressService;
    char *pbRecordTableBase;
    int nResult;
    DWORD dwNow;

    pRecordTable = m_pRecordTable00;
    pbRecordTableBase = (char *)this;
    pChannelOffsets = m_pChannelThunk0c;
    PayloadView.m_pRecordTable = pRecordTable;
    PayloadView.m_pChannelState =
        (VsNetChannelStateHeader *)(pbRecordTableBase +
                                       pChannelOffsets->m_nChannelStateViewOffset04);
    PayloadView.m_pTimedStream =
        (VsNetTimedStreamHeader *)(pbRecordTableBase + 0xc + pChannelOffsets->m_nTimedStreamViewOffset08);
    PayloadView.m_pFileWrapper =
        (VsNetFileWrapperObject *)(pbRecordTableBase + 0xc + pChannelOffsets->m_nFileWrapperViewOffset0c);
    pFileWrapper = PayloadView.m_pFileWrapper;
    pChannelState = PayloadView.m_pChannelState;
    pTimedStream = PayloadView.m_pTimedStream;
    pChannelStateVtable = (VsNetChannelStateRuntimeVtable *)pChannelState->m_pVtable;
    pTimedStreamVtable = (VsNetTimedStreamRuntimeVtable *)pTimedStream->m_pVtable;
    pSlot = &pRecordTable->m_pSlots[nSlotIndex];
    nResult = pFileWrapper->m_pVtable->m_pSeek(pFileWrapper,
                                               g_cbEffTransportMaxPacketBytes * nSlotIndex +
                                                   m_nPayloadBaseOffset08);
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
    pPeerAddressService = (VsNetPeerAddressService *)g_pEffTransportPeerAddressState;
    pPeerAddressService->m_pVtable->m_pSelectPeerName(pPeerAddressService, pSlot->m_szSourceName);
    if (pChannelState->m_fReceiving1c == 0) {
        pTimedStreamVtable->m_pServiceLoadedPacket(pTimedStream);
    }
    ((VsNetEffTransportPacketProcessor *)pTimedStream)
        ->ProcessEffTransportPacketHeader();
    return 1;
}

// FUNCTION: LEMBALL 0x0047A470
DWORD VsNetFileBackedMarkerOwner::IncrementFileBackedEffStreamMarker(void) {
    VsNetFileBackedMarkerOwner *pOwner;
    VsNetFileBackedMarkerOffsets *pOffsets;
    VsNetFileWrapperObject *pReadThunk;
    VsNetFileWrapperObject *pWriteThunk;
    unsigned long ulObjectBase;

    pOwner = this;
    ulObjectBase = (unsigned long)pOwner;
    pOffsets = pOwner->m_pOffsets;
    pReadThunk = (VsNetFileWrapperObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (VsNetFileWrapperObject *)(unsigned long)(pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    if (((VsNetFileBackedRecordTableView *)(unsigned long)(pOffsets->m_nStreamOffset1c + ulObjectBase))
            ->LoadEffStreamFromFileBackedRange(&pOwner->m_Marker04, 1, 0) == 0) {
        return 0;
    }
    pOwner->m_Marker04.m_Stream04.m_wReferenceCount2c =
        (unsigned short)(pOwner->m_Marker04.m_Stream04.m_wReferenceCount2c + 1);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    return ((VsNetFileBackedLockedWriterView *)pWriteThunk)
        ->WriteEffStreamToLockedFile((int *)&pOwner->m_Marker04, 0, 1);
}

// FUNCTION: LEMBALL 0x0047A4D0
DWORD VsNetFileBackedMarkerOwner::DecrementFileBackedEffStreamMarker(void) {
    VsNetFileBackedMarkerOwner *pOwner;
    VsNetFileBackedMarkerOffsets *pOffsets;
    VsNetFileWrapperObject *pReadThunk;
    VsNetFileWrapperObject *pWriteThunk;
    unsigned long ulObjectBase;

    pOwner = this;
    ulObjectBase = (unsigned long)pOwner;
    pOffsets = pOwner->m_pOffsets;
    pReadThunk = (VsNetFileWrapperObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (VsNetFileWrapperObject *)(unsigned long)(pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    if (((VsNetFileBackedRecordTableView *)(unsigned long)(pOffsets->m_nStreamOffset1c + ulObjectBase))
            ->LoadEffStreamFromFileBackedRange(&pOwner->m_Marker04, 1, 0) == 0) {
        return 0;
    }
    pOwner->m_Marker04.m_Stream04.m_wReferenceCount2c =
        (unsigned short)(pOwner->m_Marker04.m_Stream04.m_wReferenceCount2c - 1);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    return ((VsNetFileBackedLockedWriterView *)pWriteThunk)
        ->WriteEffStreamToLockedFile((int *)&pOwner->m_Marker04, 0, 1);
}

struct VsNetFileBackedMarkerReleaseView {
    BYTE DecrementFileBackedEffMarkerAndReleaseIfZero(void);
};

// FUNCTION: LEMBALL 0x0047A530
BYTE VsNetFileBackedMarkerReleaseView::DecrementFileBackedEffMarkerAndReleaseIfZero(void) {
    VsNetFileBackedCallbackStatusFront *pCallbackFront;
    VsNetFileBackedMarkerReleaseFront *pReleaseFront;
    VsNetFileBackedMarkerOwner *pOwner;
    VsNetFileBackedMarkerOffsets *pOffsets;
    VsNetCallbackThunkObject *pReleaseThunk;
    BYTE bStatus;

    pReleaseFront = (VsNetFileBackedMarkerReleaseFront *)this;
    pOwner = (VsNetFileBackedMarkerOwner *)((char *)this - 0x38);
    pOffsets = pOwner->m_pOffsets;
    pOwner->DecrementFileBackedEffStreamMarker();
    pCallbackFront = (VsNetFileBackedCallbackStatusFront *)((char *)pReleaseFront +
                                                                pOffsets->m_nNestedThunkOffset18 - 0x18);
    bStatus = MapNestedEffCallbackFailureToStatus6(pCallbackFront);
    if (pOwner->m_Marker04.m_Stream04.m_wReferenceCount2c == 0) {
        pReleaseThunk = (VsNetCallbackThunkObject *)(unsigned long)(pOffsets->m_nReadThunkOffset14 +
                                                                       (int)(unsigned long)pOwner);
        pReleaseThunk->m_pVtable->m_pRelease(pReleaseThunk);
    }
    return bStatus;
}

// FUNCTION: LEMBALL 0x0047B9B0
BYTE AdjustAndReleaseCompositeFileBackedEffMarker(void *pObject) {
    return ((VsNetFileBackedMarkerReleaseView *)((char *)pObject -
                                                    ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta))
        ->DecrementFileBackedEffMarkerAndReleaseIfZero();
}

// FUNCTION: LEMBALL 0x0047BB40
BYTE AdjustAndReleaseGlobalStateFileBackedEffMarker(void *pObject) {
    return ((VsNetFileBackedMarkerReleaseView *)((char *)pObject -
                                                    ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta - 8))
        ->DecrementFileBackedEffMarkerAndReleaseIfZero();
}

// FUNCTION: LEMBALL 0x0047BE30
BYTE AdjustAndMapEmbeddedFileBackedEffCallbackFailure(void *pObject) {
    return MapNestedEffCallbackFailureToStatus6((char *)pObject + 0x138);
}

// FUNCTION: LEMBALL 0x0047BE40
void *AdjustAndDeleteEmbeddedFileBackedEffChannelStackWrapper40(void *pObject, BYTE fFreeMemory) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetFileBackedCompositeWrapperView *)pObject)
        ->DeleteEmbeddedFileBackedEffChannelStackWrapper(fFreeMemory);
}

struct VsNetFileBackedPendingRecordServiceView {
    VsNetEffStreamRecordSlotTable *m_pRecordTable00;
    int m_nRecordHeaderOffset04;
    int m_nPayloadBaseOffset08;
    VsNetFileBackedDispatchOffsets *m_pChannelThunk0c;
    int m_nPendingSlot10;
    int m_nReserved14;

    void ServicePendingFileBackedEffRecords(void);
};

// FUNCTION: LEMBALL 0x00479C10
void VsNetFileBackedPendingRecordServiceView::ServicePendingFileBackedEffRecords(void) {
    VsNetFileBackedRecordServiceView ServiceView;
    VsNetEffStreamRecordSlotTable *pRecordTable;
    VsNetFileBackedDispatchOffsets *pChannelOffsets;
    VsNetWin32FileWrapper *pFileWrapper;
    VsNetPendingRecordChannelStateHeader *pChannelState;
    VsNetEffStreamRecordSlot *pSlot;
    char *pbRecordTableBase;
    int nLockResult;
    int cbRecordSlot;
    void *pRuntimeWindow;
    int i;
    int nSlotCount;
    int fLoadedOne;

    pRecordTable = m_pRecordTable00;
    pbRecordTableBase = (char *)this;
    pChannelOffsets = m_pChannelThunk0c;
    ServiceView.m_pRecordTable = pRecordTable;
    ServiceView.m_pChannelState = (VsNetPendingRecordChannelStateHeader *)(pbRecordTableBase +
        pChannelOffsets->m_nChannelStateViewOffset04);
    ServiceView.m_pFileWrapper =
        (VsNetWin32FileWrapper *)(pbRecordTableBase + 0xc + pChannelOffsets->m_nFileWrapperViewOffset0c);
    pChannelState = ServiceView.m_pChannelState;
    if ((pChannelState->m_fReceiving1c == 0 && pChannelState->m_fChannelOpen28 == 0) || pChannelState->m_fPending18 == 0) {
        return;
    }

    pFileWrapper = ServiceView.m_pFileWrapper;
    nLockResult = pFileWrapper->LockRange(
        m_nRecordHeaderOffset04,
        pRecordTable->m_nAccumulatedStreamLength18);
    if (nLockResult == 0) {
        pRuntimeWindow = 0;
        if (g_pActiveNetworkRuntimeWindow != 0) {
            pRuntimeWindow = (char *)g_pActiveNetworkRuntimeWindow - 0x10;
        }
        if (pRuntimeWindow != 0) {
            ((VsNetRuntimeWindowBase *)pRuntimeWindow)
                ->ScheduleNetworkRuntimeTimerEvent(0x32);
        }
        return;
    }

    if (m_nPendingSlot10 == -1) {
        pFileWrapper->Seek(m_nRecordHeaderOffset04);
        pFileWrapper->Read((LPVOID)g_pEffTransportPacketBuffer,
                           pRecordTable->m_nAccumulatedStreamLength18);
        nLockResult = pFileWrapper->UnlockRange(
            m_nRecordHeaderOffset04,
            pRecordTable->m_nAccumulatedStreamLength18);
        if (nLockResult != 0) {
            nSlotCount = 0;
            ((GameEffStream *)pRecordTable)
                ->LoadEffStreamFromMemory((int)(unsigned long)g_pEffTransportPacketBuffer);
            fLoadedOne = 0;
            if (0 < pChannelState->m_cSlots14) {
                do {
                    pSlot = &pRecordTable->m_pSlots[nSlotCount];
                    if (pSlot->m_wObservedMarker < pSlot->m_wCommittedMarker) {
                        if (fLoadedOne) {
                            m_nPendingSlot10 = nSlotCount;
                            return;
                        }
            ((VsNetFileBackedRecordTableView *)this)->LoadFileBackedEffRecordPayload(nSlotCount);
                        pSlot->m_wObservedMarker = pSlot->m_wCommittedMarker;
                        fLoadedOne = 1;
                    }
                    ++nSlotCount;
                } while (nSlotCount < pChannelState->m_cSlots14);
            }
        }
    } else {
        cbRecordSlot = pRecordTable->m_pSlots->m_nBufferEnd18;
        pFileWrapper->Seek(
            cbRecordSlot * m_nPendingSlot10 + m_nRecordHeaderOffset04);
        pFileWrapper->Read((LPVOID)g_pEffTransportPacketBuffer,
                           pRecordTable->m_nAccumulatedStreamLength18);
        nLockResult = pFileWrapper->UnlockRange(
            m_nRecordHeaderOffset04,
            pRecordTable->m_nAccumulatedStreamLength18);
        if (nLockResult != 0) {
            pSlot = &pRecordTable->m_pSlots[m_nPendingSlot10];
            ((GameEffStream *)pSlot)->LoadEffStreamFromMemory((int)(unsigned long)g_pEffTransportPacketBuffer);
            ((VsNetFileBackedRecordTableView *)this)
                ->LoadFileBackedEffRecordPayload(m_nPendingSlot10);
            pSlot->m_wObservedMarker = pSlot->m_wCommittedMarker;
            i = m_nPendingSlot10;
            nSlotCount = pChannelState->m_cSlots14;
            if (i < nSlotCount) {
                do {
                    pSlot = &pRecordTable->m_pSlots[i];
                    if (pSlot->m_wObservedMarker < pSlot->m_wCommittedMarker) {
                        m_nPendingSlot10 = i;
                        break;
                    }
                    ++i;
                } while (i < nSlotCount);
            }
            if (pChannelState->m_cSlots14 == i) {
                m_nPendingSlot10 = -1;
            }
        }
    }
}

// FUNCTION: LEMBALL 0x0047AEF0
static void ServiceGlobalStateEffComposite0047AEF0(void *pObject) {
    VsNetGlobalStateCompositeLayout *pComposite;
    VsNetCompositeViewOffsets *pOffsets;
    DWORD dwNow;

    pComposite = (VsNetGlobalStateCompositeLayout *)pObject;
    dwNow = timeGetTime();
    if (dwNow - pComposite->m_dwGlobalStateServiceTick58 > 100) {
        if (pComposite->m_fGlobalStateStreamLoaded54 != 0) {
            pOffsets = pComposite->m_pTimedStreamOffsets20;
            ((VsNetFileBackedPendingRecordServiceView *)
                ((char *)pComposite + 0x20 + pOffsets->m_nRecordTableBOffset1c))
                ->ServicePendingFileBackedEffRecords();
        } else {
            dwNow = timeGetTime();
        }
        pComposite->m_dwGlobalStateServiceTick58 = dwNow;
    }
    ((VsNetChannelOwnerObject *)pObject)->ServiceEffTransportConnectRequest();
}

// FUNCTION: LEMBALL 0x0047B440
void ConfigureDualSlotCompositeFilePath0047B440(void *pObject) {
    VsNetDualSlotCompositeTransportFront *pTransport;
    VsNetCompositeViewOffsets *pOffsets;
    VsNetFileBackedMarkerOwner *pMarkerOwner;
    char *pszPath;
    char *pszDot;
    char *pbRecordFront;

    pTransport = (VsNetDualSlotCompositeTransportFront *)((char *)pObject + 0x2c);
    pOffsets = pTransport->m_pViewOffsets00;
    pszPath = g_pszFileBasedNetworkConfiguredPath;
    pszDot = FindCharInCString0047FE00(pszPath, '.');
    if (pszDot == 0) {
        pszDot = pszPath + strlen(pszPath);
    }
    memcpy(pszDot, ".con", 5);

    OpenFileBackedEffHandleWithRetry(
        (char *)pObject + pOffsets->m_nRecordTableAOffset18 + 0x2c,
        (int)(unsigned long)pszPath);
    pMarkerOwner = (VsNetFileBackedMarkerOwner *)((char *)pObject + 0x2c);
    pMarkerOwner->IncrementFileBackedEffStreamMarker();

    pbRecordFront = (char *)pObject + pOffsets->m_nRecordTableBOffset1c;
    *(int *)(pbRecordFront + 0x34) =
    *(int *)(*(char **)(pbRecordFront + 0x2c) + 0x18) +
        *(int *)(pbRecordFront + 0x30);
}

struct VsNetGlobalBroadcastDispatchEvent {
    unsigned short m_nType;
    unsigned short m_anReserved[3];
    int m_nCode;
    int *m_pStream;
    void *m_pPeer;
};

// FUNCTION: LEMBALL 0x0047AB20
int PrepareGlobalStateBroadcastPath0047AB20(void *pObject, char *pszHostName) {
    VsNetGlobalStateCompositeTransportFront *pTransport;
    VsNetCompositeViewOffsets *pOffsets;
    char *pbBase;
    char *pszPath;
    char *pszDot;
    char *pbFirstFront;
    char *pbSecondFront;
    int fOpened;
    VsNetGlobalBroadcastDispatchEvent kEvent;

    pbBase = (char *)pObject;
    pTransport = (VsNetGlobalStateCompositeTransportFront *)(pbBase + 0x20);
    pOffsets = pTransport->m_pViewOffsets00;
    ((VsNetEffTransportPeer *)pObject)->PrepareEffTransportBroadcastStatusPayload(pszHostName);

    pszPath = g_pszFileBasedNetworkConfiguredPath;
    pszDot = FindCharInCString0047FE00(pszPath, '.');
    if (pszDot == 0) {
        pszDot = pszPath + strlen(pszPath);
    }
    memcpy(pszDot, ".bct", 5);

    fOpened = OpenFileBackedEffHandleWithRetry(
        pbBase + pOffsets->m_nRecordTableAOffset18 + 0x20,
        (int)(unsigned long)pszPath);
    ((VsNetFileBackedMarkerOwner *)pTransport)
        ->IncrementFileBackedEffStreamMarker();

    pbFirstFront = pbBase + pOffsets->m_nRecordTableBOffset1c;
    pbSecondFront = pbBase + pOffsets->m_nRecordTableCOffset20;
    *(int *)(pbSecondFront + 0x28) =
        *(int *)(*(char **)(pbFirstFront + 0x20) + 0x18) +
        *(int *)(pbFirstFront + 0x24);
    *(int *)(pbSecondFront + 0x28) = *(int *)(pbFirstFront + 0x28);
    if (fOpened != 0) {
        *(int *)(pbBase + pOffsets->m_nTransportViewOffset04 + 0x2c) = 1;
    }

    *(DWORD *)(pbBase + 0x18) = timeGetTime() - 1000;
    kEvent.m_nType = 2;
    kEvent.m_anReserved[0] = 0;
    kEvent.m_anReserved[1] = 0;
    kEvent.m_anReserved[2] = 0;
    kEvent.m_nCode = 0;
    kEvent.m_pStream = (int *)pObject;
    kEvent.m_pPeer = 0;
    if (g_pEffTransportDispatchQueue != 0) {
        ((void (*)(VsNetGlobalBroadcastDispatchEvent *))
            (*(void ***)g_pEffTransportDispatchQueue)[2])(&kEvent);
    }
    *(DWORD *)(pbBase + 0x58) = timeGetTime();
    return fOpened;
}

static void CompleteDualSlotEffTransportPendingWriteBody(void *pObject,
                                                           int fQueueEvent);

// FUNCTION: LEMBALL 0x0047BFB0
void AdjustAndCompleteDualSlotEffTransportPendingWrite0047BFB0(void *pObject,
                                                                 int fQueueEvent) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    CompleteDualSlotEffTransportPendingWriteBody(pObject, fQueueEvent);
}

// FUNCTION: LEMBALL 0x0047BFC0
static void CompleteDualSlotEffTransportPendingWriteBody(void *pObject,
                                                           int fQueueEvent) {
    pObject = (char *)pObject - 0x34;
    ((VsNetCompleteEffTransportPendingWriteView *)pObject)
        ->CompleteEffTransportPendingWrite(fQueueEvent);
}

// FUNCTION: LEMBALL 0x0047FE00
char *FindCharInCString0047FE00(char *pszText, char ch) {
    for (;;) {
        if (*pszText == ch) {
            return pszText;
        }
        if (*pszText == '\0') {
            return 0;
        }
        ++pszText;
    }
}

// FUNCTION: LEMBALL 0x0047B3B0
static int SetDualSlotCompositePathPair0047B3B0(void *pObject,
                                                 const char *pszPath08,
                                                 const char *pszPath0c) {
    unsigned int cbPath;
    char *pszCopy;

    cbPath = (unsigned int)strlen(pszPath0c) + 1;
    pszCopy = (char *)AllocateVSMemBlock(cbPath);
    memcpy(pszCopy, pszPath0c, cbPath);
    *(char **)((char *)pObject + 0x0c) = pszCopy;

    cbPath = (unsigned int)strlen(pszPath08) + 1;
    pszCopy = (char *)AllocateVSMemBlock(cbPath);
    memcpy(pszCopy, pszPath08, cbPath);
    *(char **)((char *)pObject + 0x08) = pszCopy;
    return 1;
}

// FUNCTION: LEMBALL 0x0047B4D0
static void PrepareDualSlotComposite0047B4D0(void *pObject, int nArgument) {
    VsNetDualSlotCompositeMethodView *pMethods;
    VsNetDualSlotCompositeLayout *pComposite;
    VsNetCompositeViewOffsets *pOffsets;
    VsNetDualSlotFinalTimedThunkVtableModel *pTimedThunk;
    DWORD dwNow;

    pMethods = (VsNetDualSlotCompositeMethodView *)pObject;
    pComposite = (VsNetDualSlotCompositeLayout *)pObject;
    pMethods->Prepare();
    pOffsets = pComposite->m_pTimedStreamOffsets2c;
    pTimedThunk = (VsNetDualSlotFinalTimedThunkVtableModel *)
        ((char *)pObject + 0x2c + pOffsets->m_nTimedStreamViewOffset08);
    pTimedThunk->InvokeService((void *)(unsigned long)nArgument);
    *(int *)((char *)pObject + pOffsets->m_nTransportViewOffset04 + 0x44) = 1;
    *(int *)((char *)pObject + pOffsets->m_nTransportViewOffset04 + 0x48) = 1;
    *(int *)((char *)pObject + 0x1c) = 0;
    dwNow = timeGetTime() - 100;
    *(DWORD *)((char *)pObject + pOffsets->m_nTimedStreamViewOffset08 + 0x68) = dwNow;
    dwNow = timeGetTime();
    *(DWORD *)((char *)pObject + pOffsets->m_nDualStreamViewOffset0c + 0x68) = dwNow;
    pMethods->Poll();
}

// FUNCTION: LEMBALL 0x0047B540
static void ActivateDualSlotComposite0047B540(void *pObject) {
    VsNetDualSlotCompositeMethodView *pMethods;
    VsNetDualSlotCompositeLayout *pComposite;
    VsNetCompositeViewOffsets *pOffsets;

    pMethods = (VsNetDualSlotCompositeMethodView *)pObject;
    pComposite = (VsNetDualSlotCompositeLayout *)pObject;
    pMethods->Prepare();
    pOffsets = pComposite->m_pTimedStreamOffsets2c;
    *(int *)((char *)pObject + pOffsets->m_nTransportViewOffset04 + 0x48) = 1;
    *(int *)((char *)pObject + 0x1c) = 1;
    *(DWORD *)((char *)pObject + pOffsets->m_nDualStreamViewOffset0c + 0x68) = timeGetTime();
    pMethods->Poll();
}

// FUNCTION: LEMBALL 0x0047B580
// The compiler lays two record fronts over the same composite. The writer
// view at the third front supplies dispatch metadata; its first word is the
// stream passed to the first write, while the second front supplies the
// stream passed to the second write. Keep these as views: not allocations.
void InitializeDualSlotCompositeStorage0047B580(void *pObject) {
    VsNetDualSlotCompositeLayout *pComposite;
    VsNetCompositeViewOffsets *pOffsets;
    char *pbBase;
    char *pbFrontA;
    char *pbFrontB;
    char *pbFrontC;
    VsNetFileBackedLockedWriterStorageView *pWriterView;
    VsNetFileBackedLockedWriterStorageView *pStreamView;
    VsNetInlineOpenWin32FileWrapper *pMarkerFile;
    VsNetFileWrapperVtable *pFileVtable;
    DWORD cbPacket;
    DWORD dwRequiredLength;
    DWORD dwFileLength;
    DWORD dwStartTime;
    DWORD dwNow;
    void *pZeroBuffer = 0;
    int fLocked;
    int nBlocks;
    int nBlocksB;
    int nBlocksC;

    pComposite = (VsNetDualSlotCompositeLayout *)pObject;
    pOffsets = pComposite->m_pTimedStreamOffsets2c;
    pbBase = (char *)pObject;
    pbFrontA = pbBase + pOffsets->m_nRecordTableAOffset18;
    pbFrontB = pbBase + pOffsets->m_nRecordTableBOffset1c;
    pbFrontC = pbBase + pOffsets->m_nRecordTableCOffset20;

    cbPacket = (DWORD)g_cbEffTransportMaxPacketBytes;
    nBlocks = 2 * (int)*(short *)(pbBase + pOffsets->m_nTransportViewOffset04 + 0x4c);
    if (*(int *)(pbBase + 0x1c) == 0) {
        nBlocksB = nBlocks + 1;
        nBlocksC = nBlocks;
    } else {
        nBlocksB = nBlocks;
        nBlocksC = nBlocks + 1;
    }

    *(int *)(pbFrontB + 0x30) +=
        (*(int *)(pbFrontA + 0x34) * (int)cbPacket +
         *(int *)(*(char **)(pbFrontB + 0x2c) + 0x18)) * nBlocksB;
    *(int *)(pbFrontB + 0x34) =
        *(int *)(*(char **)(pbFrontB + 0x2c) + 0x18) + *(int *)(pbFrontB + 0x30);

    *(int *)(pbFrontC + 0x30) +=
        (*(int *)(pbFrontA + 0x34) * (int)cbPacket +
         *(int *)(*(char **)(pbFrontC + 0x2c) + 0x18)) * nBlocksC;
    *(int *)(pbFrontC + 0x34) =
        *(int *)(*(char **)(pbFrontC + 0x2c) + 0x18) + *(int *)(pbFrontC + 0x30);

    dwRequiredLength = *(DWORD *)(pbFrontB + 0x34) +
                       *(DWORD *)(pbFrontA + 0x34) * cbPacket;
    dwFileLength = *(DWORD *)(pbFrontC + 0x34) +
                   *(DWORD *)(pbFrontA + 0x34) * cbPacket;
    if (dwFileLength > dwRequiredLength) {
        dwRequiredLength = dwFileLength;
    }

    pMarkerFile = (VsNetInlineOpenWin32FileWrapper *)
        (pbBase + pOffsets->m_nMarkerViewOffset14 + 0x2c);
    pFileVtable = (VsNetFileWrapperVtable *)pMarkerFile->m_pVtable;
    dwStartTime = timeGetTime();
    do {
        fLocked = LockWin32FileRange(pMarkerFile, 0, *(DWORD *)(pbBase + 0x48));
        if (fLocked != 0) {
            break;
        }
        dwNow = timeGetTime();
    } while (dwNow - dwStartTime < 100);
    if (fLocked == 0) {
        *(int *)(pbBase + pOffsets->m_nTransportViewOffset04 + 0x38) = 1;
        *(int *)(pbBase + pOffsets->m_nTransportViewOffset04 + 0x50) = 1;
        return;
    }

    if (pFileVtable->m_pGetLength(pMarkerFile) < dwRequiredLength) {
        pZeroBuffer = AllocateVSMemBlock((int)cbPacket);
        if (pZeroBuffer == 0) {
            *(int *)(pbBase + pOffsets->m_nTransportViewOffset04 + 0x38) = 1;
            *(int *)(pbBase + pOffsets->m_nTransportViewOffset04 + 0x50) = 1;
            return;
        }
        memset(pZeroBuffer, 0, cbPacket);
        pFileVtable->m_pSeek(pMarkerFile, pFileVtable->m_pGetLength(pMarkerFile));

        pWriterView = (VsNetFileBackedLockedWriterStorageView *)(pbFrontC + 0x2c);
        ((VsNetFileBackedLockedWriterView *)pWriterView)
            ->WriteEffStreamToLockedFile(pWriterView->m_pStream00, 0, 0);

        nBlocks = 0;
        while (*(DWORD *)(pbFrontA + 0x34) > (DWORD)nBlocks) {
            pFileVtable->m_pWrite(pMarkerFile, pZeroBuffer, cbPacket);
            ++nBlocks;
        }

        pStreamView = (VsNetFileBackedLockedWriterStorageView *)(pbFrontB + 0x2c);
        ((VsNetFileBackedLockedWriterView *)pWriterView)
            ->WriteEffStreamToLockedFile(pStreamView->m_pStream00, 0, 0);

        nBlocks = 0;
        while (*(DWORD *)(pbFrontB + 0x34) > (DWORD)nBlocks) {
            pFileVtable->m_pWrite(pMarkerFile, pZeroBuffer, cbPacket);
            ++nBlocks;
        }
        FreeVSMemBlock(pZeroBuffer);
    }
    if (UnlockWin32FileRange(pMarkerFile, 0, *(DWORD *)(pbBase + 0x48)) == 0) {
        *(int *)(pbBase + pOffsets->m_nTransportViewOffset04 + 0x38) = 1;
        *(int *)(pbBase + pOffsets->m_nTransportViewOffset04 + 0x50) = 1;
    }
}

// FUNCTION: LEMBALL 0x0047B990
static void ServiceDualSlotCompositeRecords0047B990(void *pObject) {
    VsNetDualSlotCompositeLayout *pComposite;
    VsNetCompositeViewOffsets *pOffsets;

    pComposite = (VsNetDualSlotCompositeLayout *)pObject;
    pOffsets = pComposite->m_pTimedStreamOffsets2c;
    ((VsNetFileBackedPendingRecordServiceView *)
        ((char *)pComposite + 0x2c + pOffsets->m_nRecordTableBOffset1c))
        ->ServicePendingFileBackedEffRecords();
    ((VsNetCompositeEffTransportVtableModel *)pObject)
        ->ServiceEffTransportPeerEntry();
}

// FUNCTION: LEMBALL 0x00479FA0
DWORD VsNetFileBackedLockedWriterView::WriteEffStreamToLockedFile(int *pStream,
                                                                     int fKeepLock, int fFlush) {
    VsNetFileBackedDispatchOffsets *pOffsets;
    VsNetFileWrapperObject *pFileWrapper;
    DWORD dwFileOffset;
    DWORD cbPayload;
    DWORD dwStartTime;
    DWORD dwNow;
    int fResult;

    pOffsets = *(VsNetFileBackedDispatchOffsets **)((char *)this + 0x0c);
    pFileWrapper = (VsNetFileWrapperObject *)((char *)this +
                                                 0x0c + pOffsets->m_nFileWrapperViewOffset0c);
    ((GameEffStream *)pStream)->BeginEffStreamWriteSession();
    dwFileOffset = pFileWrapper->m_pVtable->m_pGetOffset(pFileWrapper);
    cbPayload = (DWORD)(pStream[7] - pStream[2]) - 0x10;
    pFileWrapper->m_pVtable->m_pGetLength(pFileWrapper);

    if (fFlush == 0) {
        dwStartTime = timeGetTime();
        do {
            fResult = LockWin32FileRange(pFileWrapper, dwFileOffset, cbPayload);
            if (fResult != 0) {
                break;
            }
            dwNow = timeGetTime();
        } while (dwNow - dwStartTime < 100);
        if (fResult == 0) {
            cbPayload = 0;
        }
    }

    if (cbPayload != 0) {
        fResult = WriteWin32FileWrapper(pFileWrapper,
                                        (const void *)(unsigned long)(pStream[2] + 0x10), cbPayload);
        if (fResult == 0) {
            UnlockWin32FileRange(pFileWrapper, dwFileOffset, cbPayload);
        } else if (fKeepLock == 0 && UnlockWin32FileRange(pFileWrapper, dwFileOffset, cbPayload) == 0) {
            fResult = 0;
        }
        if (fResult == 0) {
            cbPayload = 0;
        }
    }

    ((GameEffStream *)pStream)->EndEffStreamWriteSession();
    return cbPayload;
}

struct VsNetFileBackedRecordWriterView {
    int AppendFileBackedEffRecordSlot(const void *pvPayload, DWORD cbPayload);
    int AppendAdjustedFileBackedEffRecordSlot(const void *pvPayload, DWORD cbPayload);
};

// FUNCTION: LEMBALL 0x0047A0B0
int VsNetFileBackedRecordWriterView::AppendFileBackedEffRecordSlot(const void *pvPayload,
                                                                       DWORD cbPayload) {
    VsNetFileBackedAppendOffsets *pOffsets;
    VsNetEffTransportRuntimeService *pRuntimeService;
    VsNetEffStreamRecordSlot *pSlot;
    VsNetEffStreamRecordSlotTable *pRecordTable;
    VsNetChannelStateHeader *pOutputState;
    VsNetFileWrapperObject *pFileWrapper;
    VsNetFileBackedAppendNameProviderState *pNameProviderState;
    VsNetStreamNameProvider *pNameProvider;
    char *pszSource;
    char *pszTarget;
    unsigned int cch;
    unsigned int count;
    int nCurrentSlot;
    const int cbRecordSlot = sizeof(VsNetEffStreamRecordSlot);
    DWORD dwRecordSlotOffset;
    DWORD dwPayloadSlotOffset;
    DWORD cbLocked;
    int fFailed;
    char *pbRecordWriterBase;
    char *pbThunkBase;
    DWORD dwNow;

    pbRecordWriterBase = (char *)this;
    pbThunkBase = pbRecordWriterBase - 0x3c;
    pRecordTable = (VsNetEffStreamRecordSlotTable *)((char *)this - 0x48);
    pOffsets = (VsNetFileBackedAppendOffsets *)pRecordTable->m_pChannelThunk0c;
    pOutputState = (VsNetChannelStateHeader *)(pbThunkBase + pOffsets->m_nOutputStateOffset04);
    pFileWrapper = (VsNetFileWrapperObject *)(pbThunkBase + pOffsets->m_nFileWrapperOffset0c);
    pNameProviderState =
        (VsNetFileBackedAppendNameProviderState *)(pbRecordWriterBase + pOffsets->m_nNameProviderOffset08);
    pNameProvider = &pNameProviderState->m_NameProvider34;
    if (pOutputState->m_fChannelOpen24 == 0) {
        return 0;
    }

    pRuntimeService = (VsNetEffTransportRuntimeService *)g_pEffTransportRuntimeService;
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
    cbLocked = ((VsNetFileBackedLockedWriterView *)pRecordTable)
                   ->WriteEffStreamToLockedFile((int *)pSlot, 1, 0);
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
int VsNetFileBackedRecordWriterView::AppendAdjustedFileBackedEffRecordSlot(const void *pvPayload,
                                                                                DWORD cbPayload) {
    int nRecordWriterDelta;
    int nRecordTableDelta;
    int nSlotCountDelta;
    int *pnCurrentSlot;
    int fAppended;

    nRecordWriterDelta = *(int *)(unsigned long)(*(int *)((char *)this - 0x38) + 0x20);
    fAppended = ((VsNetFileBackedRecordWriterView *)((char *)this + nRecordWriterDelta + 0x10))
                    ->AppendFileBackedEffRecordSlot(pvPayload, cbPayload);
    if (fAppended == 0) {
        return 0;
    }

    pnCurrentSlot = (int *)((char *)this + nRecordWriterDelta - 0x28);
    nSlotCountDelta = *(int *)(unsigned long)(*(int *)((char *)this - 0x38) + 0x18);
    nRecordTableDelta = *(int *)((char *)this + nSlotCountDelta - 0x30);
    *pnCurrentSlot = *pnCurrentSlot % nRecordTableDelta;
    return fAppended;
}

// FUNCTION: LEMBALL 0x0047BD70
int AppendAdjustedFileBackedEffRecordSlotThunk(void *pObject, const void *pvPayload, DWORD cbPayload) {
    pObject = (char *)pObject - *(int *)((char *)pObject - 4);
    return ((VsNetFileBackedRecordWriterView *)pObject)
        ->AppendFileBackedEffRecordSlot(pvPayload, cbPayload);
}

// FUNCTION: LEMBALL 0x0047BD80
BYTE AdjustAndMapNestedEffCallbackFailureFromB8(void *pObject) {
    return MapNestedEffCallbackFailureToStatus6((char *)pObject + 0xb8);
}

// FUNCTION: LEMBALL 0x0047BDF0
void AdjustAndClearNestedEffChannelAsyncStatusFromB8(void *pObject) {
    ClearNestedEffChannelAsyncStatus((char *)pObject + 0xb8);
}

// FUNCTION: LEMBALL 0x0047BC40
int AppendAdjustedGlobalStateFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    return ((VsNetFileBackedRecordWriterView *)((char *)pObject -
                                                   ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta +
                                                   0x158))
        ->AppendAdjustedFileBackedEffRecordSlot(pvPayload, cbPayload);
}

// FUNCTION: LEMBALL 0x0047BEF0
int AppendAdjustedEmbeddedFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    return ((VsNetFileBackedRecordWriterView *)((char *)pObject -
                                                   ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta))
        ->AppendAdjustedFileBackedEffRecordSlot(pvPayload, cbPayload);
}

// FUNCTION: LEMBALL 0x0047BB00
int AppendAdjustedGlobalStateTimedFileBackedEffRecordSlot(void *pObject, const void *pvPayload, DWORD cbPayload) {
    return ((VsNetFileBackedRecordWriterView *)((char *)pObject -
                                                   ((VsNetAdjustorThunkHeader *)((char *)pObject - 4))->m_nThisDelta +
                                                   0x154))
        ->AppendAdjustedFileBackedEffRecordSlot(pvPayload, cbPayload);
}

// FUNCTION: LEMBALL 0x0047AA10
void LEMBALL_FASTCALL InitializeGlobalStateFileBackedEffStorage(int nObjectBasePlus0x15c) {
    VsNetGlobalStateFileBackedStorageOwner *pOwner;
    VsNetFileWrapperObject *pReadThunk;
    VsNetFileWrapperObject *pWriteThunk;
    VsNetSlotCountHolder *pSlotCount;
    unsigned long ulObjectBase;
    DWORD cbPacket;
    unsigned int count;
    unsigned char *pByte;
    unsigned int i;
    void *pZeroBuffer;

    pOwner = (VsNetGlobalStateFileBackedStorageOwner *)(unsigned long)(nObjectBasePlus0x15c - 0x15c);
    ulObjectBase = (unsigned long)pOwner;
    pReadThunk = (VsNetFileWrapperObject *)(unsigned long)(pOwner->m_pOffsets->m_nReadThunkOffset14 + ulObjectBase);
    pWriteThunk = (VsNetFileWrapperObject *)(unsigned long)(pOwner->m_pOffsets->m_nWriteThunkOffset20 + ulObjectBase);
    pSlotCount =
        (VsNetSlotCountHolder *)(unsigned long)(pOwner->m_pOffsets->m_nSlotCountViewOffset18 + ulObjectBase);
    pReadThunk->m_pVtable->m_pSeek(pReadThunk, 0);
    ((VsNetFileBackedLockedWriterView *)pWriteThunk)
        ->WriteEffStreamToLockedFile((int *)&pOwner->m_InitialStream04, 0, 0);
    ((VsNetFileBackedLockedWriterView *)pWriteThunk)
        ->WriteEffStreamToLockedFile((int *)(unsigned long)g_pGlobalStateEff512ByteStream, 0, 0);
    ((VsNetFileBackedLockedWriterView *)pWriteThunk)
        ->WriteEffStreamToLockedFile((int *)(unsigned long)*(int *)pWriteThunk, 0, 0);

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
