#include "ENGINE/MEDIA/VSSOUND.H"
#include "ENGINE/CORE/SAFEVT.H"
#include "ENGINE/CORE/SAFEVT.H"

#include "GAME.H"
#include "SHELL/ENTRY.H"
#include "RESOURCE/MOGLOAD.H"
#include "ENGINE/CORE/VSMEM.H"
#include "ENGINE/CORE/VSINIT.H"

#include <string.h>

typedef int (LEMBALL_FASTCALL *VsSoundTestProc)(void *pBackend, int nUnused);
typedef int (LEMBALL_FASTCALL *VsSoundOpenProc)(void *pBackend, int nUnused,
                                                int nResourceBase, int cResources,
                                                void *pPrimaryContext);
typedef void (*VsSoundVoidProc)(void);
typedef void (*VsSoundIntProc)(int nValue);
typedef void (LEMBALL_FASTCALL *VsSoundControlIntProc)(
    void *pControl, int nUnused, int nValue);
typedef void (LEMBALL_FASTCALL *VsSoundControlPairProc)(
    void *pControl, int nUnused, int nValue1, int nValue2);
typedef int (LEMBALL_FASTCALL *VsSoundCreateEffectProc)(
    void *pBackend, int nUnused, int nValue, void *pOut);
typedef void (*VsSoundEffectPlayProc)(int nEffectResourceId, int nVolume);
typedef void (*VsSoundEffectVolumeProc)(int nEffectInstanceId, int nVolume, int nReserved);
typedef int (*VsSoundReturnIntProc)(void);
typedef int (*VsSoundReturnIntArgProc)(int nValue);
typedef const char *(*VsSoundNameProc)(void);
typedef void (LEMBALL_FASTCALL *VsSoundContextProc)(
    void *pBackend, int nUnused, void *pPrimaryContext);
/* Vtable slot 0 is a thiscall delete entry: ECX is the object and the
   delete flag remains on the stack.  The fastcall spelling supplies the
   required ECX object, consumes a dummy EDX register argument, and leaves
   the flag as the stack argument. */
typedef void *(LEMBALL_FASTCALL *VsSoundDeleteProc)(void *pObject,
                                                   int nUnused,
                                                   int fDelete);
typedef void *(*VsSoundConstructPatchProc)(void *, void *, int, int, int, int);
typedef void (*VsSoundPlayBufferProc)(void *, int, int);
typedef void (*VsSoundDestroyBufferProc)(int);

void *LEMBALL_FASTCALL ConstructAudioEffectBackendBase(void *pBackend);

/* Raw targets at 0047BF00/0047D700/0047C8A0/0047C940/0047C990
   overlap unlabeled/interior code in the current Ghidra ownership map. Keep
   symbolic until their true function ownership is resolved. */
static VsSoundConstructPatchProc g_AUDIO_ConstructPatchProc = 0;
static VsSoundDestroyBufferProc g_AUDIO_DestroyBufferProc = 0;
static VsSoundPlayBufferProc g_AUDIO_PlayBufferProc = 0;
static void (*g_AUDIO_PauseBufferProc)(void *, int) = 0;
static void (*g_AUDIO_PlayBufferSlotProc)(void *, int) = 0;

struct VsNetFileBackedTimedChannelWrapperView {
    void *DeleteTimedFileBackedEffChannelWrapper(BYTE fFreeMemory);
};

struct VsSoundResourceObject {
    void **m_pVtable;
    int m_nReserved04;
    int m_nLockCount08;
    int m_cReferences;
    int m_nLoadState10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1C;
    int m_nReserved20;
    int m_nReserved24;
    int m_cbResourceData28;
    int m_lResourceOffset2C;
    int m_nResourceId30;
    unsigned int *m_padwResourceDescriptor34;
    void *m_pResourceData38;
    int m_nReserved3C;
    unsigned int m_uTypeTag;
    int m_nResultCode44;
};

struct VsSoundBackendControl {
    void **m_pVtable;
    int m_nReserved04;
    int m_fStartupMusicNameSet;
    int m_fMusicEnabled;
    GameDynamicCString m_StartupMusicName;
};

struct VsSoundBackendControlDispatch {
    virtual void Reserved00(void) = 0;
    virtual void InvokeEmbeddedSlot04(int nValue1, int nValue2) = 0;
    virtual void RegisterMusicHandle(int hMusic, int nMusicResourceId) = 0;
};

struct VsSoundWaveOutEffectBackend {
    void **m_pVtable;
    int m_cEffectResources;
    int m_nReserved08;
    int m_fDeviceAvailable;
    char m_abReserved10[0x48];
    char m_szDeviceDescription[0x34];

    VsSoundWaveOutEffectBackend *ConstructWaveOutEffectBackend(int cEffectResources);
};

struct VsSoundDirectSoundEffectBackend {
    void **m_pVtable;
    HMODULE m_hDsoundModule;
    FARPROC m_pfnDirectSoundCreate;
    int m_nReserved0c;
    int m_nReserved10;
    int m_cEffectResources;
    int m_nCooperativeLevel;
    int m_nReserved1c;
    int m_fDeviceAvailable;

    VsSoundDirectSoundEffectBackend *ConstructDirectSoundEffectBackend(
        int cEffectResources, int nCooperativeLevel);
    void PlayDirectSoundEffectInstance(int nEffectInstanceId, int nBufferSlot);
};

struct VsSoundDynamicStringEntry {
    void **m_pVtable;
    int m_nReserved04;
    int m_fBasePathSet;
    int m_fSearchCdromPath;
    GameDynamicCString m_BasePath;
};

struct VsSoundMciMusicBackend : VsSoundDynamicStringEntry {
    int m_nPreparedTrackHandle;
    int m_fDeviceAvailable;
    MCIDEVICEID m_uDeviceId;
    int m_fTrackPlaying;
    int m_fTrackPaused;
    int m_nPausedTrackPosition;
    HWND m_hNotificationWindow;
    VsSoundMciMusicBackend *ConstructMciMusicBackend(void);
};

struct VsSoundWaveFormat {
    WORD wFormatTag;
    WORD nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD nBlockAlign;
    WORD wBitsPerSample;
    WORD cbSize;
};

struct VsSoundDirectSoundBufferDesc {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwBufferBytes;
    DWORD dwReserved;
    VsSoundWaveFormat *pWaveFormat;
};

struct VsSoundDirectSoundEffectBufferPool {
    int m_fReady;
    int m_nCurrentBuffer;
    void **m_ppBuffers;
    int m_nReserved0c;
    int m_cBuffers;
    int m_nReserved14;
    int m_nFlags;
};

typedef LONG (__stdcall *VsSoundDirectSoundCreateProc)(
    const void *pGuid, void **ppDirectSound, void *pOuterUnknown);
typedef LONG (__stdcall *VsSoundDirectSoundCreateBufferProc)(
    void *pDevice, VsSoundDirectSoundBufferDesc *pDesc,
    void **ppBuffer, void *pOuterUnknown);
typedef LONG (__stdcall *VsSoundDirectSoundDuplicateBufferProc)(
    void *pDevice, void *pSourceBuffer, void **ppDuplicateBuffer);
typedef LONG (__stdcall *VsSoundDirectSoundSetCooperativeLevelProc)(
    void *pDevice, HWND hWnd, DWORD dwLevel);
typedef LONG (__stdcall *VsSoundDirectSoundLockProc)(
    void *pBuffer, DWORD dwOffset, DWORD dwBytes,
    void **ppvAudioPtr1, DWORD *pdwAudioBytes1,
    void **ppvAudioPtr2, DWORD *pdwAudioBytes2, DWORD dwFlags);
typedef LONG (__stdcall *VsSoundDirectSoundPlayProc)(
    void *pBuffer, DWORD dwReserved1, DWORD dwPriority, DWORD dwFlags);
typedef LONG (__stdcall *VsSoundDirectSoundSetFormatProc)(
    void *pBuffer, VsSoundWaveFormat *pWaveFormat);
typedef LONG (__stdcall *VsSoundDirectSoundUnlockProc)(
    void *pBuffer, void *pvAudioPtr1, DWORD dwAudioBytes1,
    void *pvAudioPtr2, DWORD dwAudioBytes2);

// GLOBAL: LEMBALL 0x004a3318
static void *g_pDirectSoundPrimaryBuffer = 0;
// GLOBAL: LEMBALL 0x004a331c
static void *g_pDirectSoundDevice = 0;

#define VsSoundDIRECTSOUND_METHOD(pObject, nSlot, ProcType) \
    ((ProcType)(*(void ***)(pObject))[nSlot])

#define VsSoundRESERVED_VIRTUAL(n) virtual void Reserved##n(void) = 0

struct VsSoundBackendOpenInterface {
    VsSoundRESERVED_VIRTUAL(00);
    VsSoundRESERVED_VIRTUAL(01);
    VsSoundRESERVED_VIRTUAL(02);
    virtual int Open(int nResourceBase,
                     int cResources,
                     void *pPrimaryContext) = 0;
};

struct VsSoundEffectBackendSlots00Through24 {
    VsSoundRESERVED_VIRTUAL(00);
    VsSoundRESERVED_VIRTUAL(01);
    VsSoundRESERVED_VIRTUAL(02);
    VsSoundRESERVED_VIRTUAL(03);
    VsSoundRESERVED_VIRTUAL(04);
    VsSoundRESERVED_VIRTUAL(05);
    VsSoundRESERVED_VIRTUAL(06);
    VsSoundRESERVED_VIRTUAL(07);
    VsSoundRESERVED_VIRTUAL(08);
    VsSoundRESERVED_VIRTUAL(09);
    VsSoundRESERVED_VIRTUAL(10);
    VsSoundRESERVED_VIRTUAL(11);
    VsSoundRESERVED_VIRTUAL(12);
    VsSoundRESERVED_VIRTUAL(13);
    VsSoundRESERVED_VIRTUAL(14);
    VsSoundRESERVED_VIRTUAL(15);
    VsSoundRESERVED_VIRTUAL(16);
    VsSoundRESERVED_VIRTUAL(17);
    VsSoundRESERVED_VIRTUAL(18);
    VsSoundRESERVED_VIRTUAL(19);
    VsSoundRESERVED_VIRTUAL(20);
    VsSoundRESERVED_VIRTUAL(21);
    VsSoundRESERVED_VIRTUAL(22);
    VsSoundRESERVED_VIRTUAL(23);
    VsSoundRESERVED_VIRTUAL(24);
};

struct VsSoundEffectPlaybackInterface : public VsSoundEffectBackendSlots00Through24 {
    virtual int GetEffectVolume(void) = 0;
    VsSoundRESERVED_VIRTUAL(26);
    VsSoundRESERVED_VIRTUAL(27);
    VsSoundRESERVED_VIRTUAL(28);
    VsSoundRESERVED_VIRTUAL(29);
    VsSoundRESERVED_VIRTUAL(30);
    VsSoundRESERVED_VIRTUAL(31);
    VsSoundRESERVED_VIRTUAL(32);
    VsSoundRESERVED_VIRTUAL(33);
    VsSoundRESERVED_VIRTUAL(34);
    virtual void PlayEffect(int nEffectResourceId,
                            int nVolume,
                            int nReserved) = 0;
};

struct VsSoundEffectVolumeInterface : public VsSoundEffectBackendSlots00Through24 {
    VsSoundRESERVED_VIRTUAL(25);
    VsSoundRESERVED_VIRTUAL(26);
    VsSoundRESERVED_VIRTUAL(27);
    VsSoundRESERVED_VIRTUAL(28);
    VsSoundRESERVED_VIRTUAL(29);
    VsSoundRESERVED_VIRTUAL(30);
    VsSoundRESERVED_VIRTUAL(31);
    VsSoundRESERVED_VIRTUAL(32);
    VsSoundRESERVED_VIRTUAL(33);
    VsSoundRESERVED_VIRTUAL(34);
    virtual void SetEffectVolume(int nEffectInstanceId,
                                 int nVolume,
                                 int nReserved) = 0;
};

#undef VsSoundRESERVED_VIRTUAL

struct VsSoundStringResourceObjectView {
    virtual void Slot00(void) = 0;
    virtual void Slot04(void) = 0;
    virtual void Slot08(void) = 0;
    virtual void Slot0C(void) = 0;
    virtual void Slot10(void) = 0;
    virtual void Slot14(void) = 0;
    virtual void Slot18(void) = 0;
    virtual void PrepareStringResource(void) = 0;
    int m_nReserved04;
    int m_nLockCount08;
    int m_cReferences;
    int m_nLoadState10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1C;
    int m_nReserved20;
    int m_nReserved24;
    int m_cbResourceData28;
    int m_lResourceOffset2C;
    int m_nResourceId30;
    unsigned int *m_padwResourceDescriptor34;
    char *m_pszText38;
};

struct VsSoundMciOpenParms {
    DWORD dwCallback;
    MCIDEVICEID wDeviceID;
    LPCSTR lpstrDeviceType;
    LPCSTR lpstrElementName;
    LPCSTR lpstrAlias;
};

struct VsSoundMciGenericParms {
    DWORD dwCallback;
};

struct VsSoundMciSeekParms {
    DWORD dwCallback;
    DWORD dwTo;
};

struct VsSoundMciSetParms {
    DWORD dwCallback;
    DWORD dwTimeFormat;
};

struct VsSoundMciPlayParms {
    DWORD dwCallback;
    DWORD dwFrom;
    DWORD dwTo;
};

struct VsSoundMciStatusParms {
    DWORD dwCallback;
    DWORD dwReturn;
    DWORD dwItem;
    DWORD dwTrack;
};

const char *ReturnEmptyAudioBackendName(void);
int ReturnAudioBackendReady(void);
int ReturnAudioBackendUnavailable(void);
int LEMBALL_FASTCALL AcceptAudioBackendResources(void *pBackend, int nUnused,
                                                  int nResourceBase, int cResources,
                                                  void *pPrimaryContext);
int CreateFallbackEffectInstance(int nValue, void *pOut);
void NoOpAudioInt(int nValue);
void NoOpAudioPair(int nValue1, int nValue2);
void NoOpAudioTriple(int nValue1, int nValue2, int nValue3);
void DestroyAudioDynamicStringEntry(VsSoundDynamicStringEntry *pEntry);
void *LEMBALL_FASTCALL DeleteAudioDynamicStringEntry(
    VsSoundDynamicStringEntry *pEntry, int nUnused, int fDelete);
void LEMBALL_FASTCALL DestroyWaveOutEffectBackend(void *pBackend);
char *GetWaveOutEffectBackendDescription(VsSoundWaveOutEffectBackend *pBackend);
int IsWaveOutEffectBackendAvailable(void);
int CreateWaveOutEffectInstance(void *pBackend, void *pPatchResource, int *pnEffectInstanceId);
int FreeWaveOutEffectInstance(void *pBackend, int nEffectInstanceId);
unsigned int PlayWaveOutEffectInstance(void *pBackend, int nEffectInstanceId);
void *LEMBALL_FASTCALL DeleteWaveOutEffectBackend(
    VsSoundWaveOutEffectBackend *pBackend, int nUnused, int fDelete);
int GetWaveOutEffectBackendStatus0047CC10(void *pBackend);
int InitializeWaveOutEffectDevice0047CB00(void *pBackend,
                                           int fActivateDevice,
                                           int fStartStreaming);
int LEMBALL_FASTCALL ResetAndCloseWaveOutEffectDevice0047CC20(void *pBackend);
int IsWaveOutEffectInstanceAvailable0047CDD0(void *pBackend);
int ReturnOneWaveOutBackendValue0047CDF0(void *pBackend);
void LEMBALL_FASTCALL CaptureWaveOutBackendContext0047D260(
    void *pBackend, int nUnused, void *pPrimaryContext);
void DestroyDirectSoundEffectBackend(void *pBackend);
char *__fastcall GetDirectSoundEffectBackendDescription(VsSoundDirectSoundEffectBackend *pBackend);
int LEMBALL_FASTCALL InitializeDirectSoundEffectDevice(
    void *pBackend, int nUnused, int nResourceBase,
    int cResources, void *pPrimaryContext);
int LEMBALL_FASTCALL AcceptDirectSoundBackendResources(
    void *pBackend, int nUnused, int nResourceBase,
    int cResources, void *pPrimaryContext);
int LEMBALL_FASTCALL GetDirectSoundBackendReservedValue1c(
    void *pBackend, int nUnused);
int LEMBALL_FASTCALL GetDirectSoundBackendAvailability(
    void *pBackend, int nUnused);
int LEMBALL_FASTCALL ReturnDirectSoundBackendUnavailable(
    void *pBackend, int nUnused);
int LEMBALL_FASTCALL GetDirectSoundBackendBufferCount(
    void *pBackend, int nUnused);
void LEMBALL_FASTCALL CaptureDirectSoundBackendWindow(
    void *pBackend, int nUnused, void *pPrimaryContext);
VsSoundDirectSoundEffectBufferPool *LEMBALL_FASTCALL
ConstructDirectSoundEffectBufferPool(
    VsSoundDirectSoundEffectBufferPool *pPool, int nUnused,
    int cBuffers, void *pPatchResource, int nOutputRate,
    int fSourceIs16Bit, int nReserved, int nFlags);
int LEMBALL_FASTCALL CreateDirectSoundEffectInstance(
    void *pBackend, int nUnused, void *pPatchResource,
    int *pnEffectInstanceId, int nFlags);
int LEMBALL_FASTCALL CreateDirectSoundEffectInstanceWithDefaultFlags(
    void *pBackend, int nUnused, void *pPatchResource,
    int *pnEffectInstanceId);
int FreeDirectSoundEffectInstance(void *pBackend, int nEffectInstanceId);
int ReleaseAllDirectSoundEffectInstances(void *pBackend);
void PlayDirectSoundEffectInstanceWithMappedVolume(void *pBackend, int nEffectInstanceId, unsigned char nVolume,
                                                   int fLoopEnabled);
void PauseDirectSoundEffectInstance(void *pBackend, int nEffectInstanceId, int nBufferSlot);
void *LEMBALL_FASTCALL DeleteDirectSoundEffectBackend(
    VsSoundDirectSoundEffectBackend *pBackend, int nUnused, int fDelete);
void PrepareMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle, int nMusicResourceId);
void FreePreparedMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle);
void PlayPreparedMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle);
void StopMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle);
void PauseMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle);
void ResumeMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle);
static void SetAudioDynamicStringEntryFlag(void *pObject, int fValue);
static int GetAudioDynamicStringEntryFlag(void *pObject);

/* The binary's MCI table is a normal single-inheritance thiscall vtable.
   Keep dispatch compiler-shaped; raw cdecl entries corrupt the caller stack. */
struct VsSoundMciMusicBackendVtableModel {
    virtual void *Delete(unsigned char fDelete) {
        return DeleteAudioDynamicStringEntry(
            (VsSoundDynamicStringEntry *)this, 0, fDelete);
    }
    virtual void Initialize(int nValue1, int nValue2);
    virtual void Prepare(int nTrackHandle, int nMusicResourceId) {
        PrepareMciMusicTrack((VsSoundMciMusicBackend *)this, nTrackHandle, nMusicResourceId);
    }
    virtual void Free(int nTrackHandle) {
        FreePreparedMciMusicTrack((VsSoundMciMusicBackend *)this, nTrackHandle);
    }
    virtual void Play(int nTrackHandle) {
        PlayPreparedMciMusicTrack((VsSoundMciMusicBackend *)this, nTrackHandle);
    }
    virtual void Stop(int nTrackHandle) {
        StopMciMusicTrack((VsSoundMciMusicBackend *)this, nTrackHandle);
    }
    virtual void Pause(int nTrackHandle) {
        PauseMciMusicTrack((VsSoundMciMusicBackend *)this, nTrackHandle);
    }
    virtual void Resume(int nTrackHandle) {
        ResumeMciMusicTrack((VsSoundMciMusicBackend *)this, nTrackHandle);
    }
    virtual void SetFlag(int fValue) {
        SetAudioDynamicStringEntryFlag((VsSoundDynamicStringEntry *)this, fValue);
    }
    virtual int GetFlag(void) {
        return GetAudioDynamicStringEntryFlag((VsSoundDynamicStringEntry *)this);
    }
    virtual void Reserved10(void) {
    }
    virtual int IsAvailable(void) {
        return ((VsSoundMciMusicBackend *)this)->m_fDeviceAvailable;
    }
    virtual const char *GetReservedName(void) {
        return ReturnEmptyAudioBackendName();
    }
};

static VsSoundMciMusicBackendVtableModel g_AUDIO_MciMusicBackendVtableModel;

// GLOBAL: LEMBALL 0x004a9bc8
void *g_pAudioManager = 0;

static void *g_AudioManagerVtable[1] = { (void *)NetworkSafeVtableNoop };
static void *g_WaveOutEffectBackendVtable[39] = {
    (void *)DeleteWaveOutEffectBackend,
    (void *)GetWaveOutEffectBackendDescription,
    0,
    (void *)AcceptAudioBackendResources,
    (void *)AcceptAudioBackendResources,
    (void *)NoOpAudioInt,
    0,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioInt,
    (void *)ReturnAudioBackendUnavailable,
    (void *)IsWaveOutEffectBackendAvailable,
    (void *)ReturnAudioBackendReady,
    0,
    0,
    0,
    (void *)CreateWaveOutEffectInstance,
    0,
    0,
    (void *)FreeWaveOutEffectInstance,
    (void *)ReturnAudioBackendReady,
    (void *)PlayWaveOutEffectInstance,
    0,
    (void *)NoOpAudioInt,
    0,
    (void *)NoOpAudioInt,
    (void *)ReturnAudioBackendReady,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioTriple,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (void *)NoOpAudioTriple,
    0,
    0,
    (void *)CaptureWaveOutBackendContext0047D260,
};

// FUNCTION: LEMBALL 0x0047F510
void VsSoundMciMusicBackendVtableModel::Initialize(int, int) {
}
static void *g_DirectSoundEffectBackendVtable[39] = {
    (void *)DeleteDirectSoundEffectBackend,
    (void *)GetDirectSoundEffectBackendDescription,
    0,
    (void *)InitializeDirectSoundEffectDevice,
    (void *)AcceptDirectSoundBackendResources,
    (void *)NoOpAudioInt,
    0,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioInt,
    (void *)GetDirectSoundBackendReservedValue1c,
    (void *)GetDirectSoundBackendAvailability,
    (void *)ReturnDirectSoundBackendUnavailable,
    (void *)GetDirectSoundBackendBufferCount,
    0,
    0,
    (void *)CreateDirectSoundEffectInstanceWithDefaultFlags,
    0,
    0,
    (void *)FreeDirectSoundEffectInstance,
    (void *)ReturnAudioBackendReady,
    (void *)PlayDirectSoundEffectInstanceWithMappedVolume,
    0,
    (void *)NoOpAudioInt,
    0,
    (void *)NoOpAudioInt,
    (void *)ReturnAudioBackendReady,
    (void *)NoOpAudioInt,
    (void *)NoOpAudioTriple,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    (void *)NoOpAudioTriple,
    0,
    0,
    (void *)CaptureDirectSoundBackendWindow,
};
struct VsSoundBackendVtableInitializer {
    VsSoundBackendVtableInitializer(void) {
        int i;
        for (i = 0; i < 39; ++i) {
            if (g_WaveOutEffectBackendVtable[i] == 0)
                g_WaveOutEffectBackendVtable[i] = (void *)NoOpAudioInt;
            if (g_DirectSoundEffectBackendVtable[i] == 0)
                g_DirectSoundEffectBackendVtable[i] = (void *)NoOpAudioInt;
        }
    }
};
static VsSoundBackendVtableInitializer g_AUDIO_BackendVtableInitializer;
static char g_szAudioManagerDescription[0x400];
// FUNCTION: LEMBALL 0x0047F520
static void SetAudioDynamicStringEntryFlag(void *pObject, int fValue) {
    ((VsSoundDynamicStringEntry *)pObject)->m_fBasePathSet = fValue;
}

static void AudioDynamicStringEntryNoop(int nUnused0, int nUnused1) {
    (void)nUnused0;
    (void)nUnused1;
}

// FUNCTION: LEMBALL 0x0047F530
static int GetAudioDynamicStringEntryFlag(void *pObject) {
    return ((VsSoundDynamicStringEntry *)pObject)->m_fBasePathSet;
}

static void *g_AUDIO_DynamicStringEntryVtable[10] = {
    (void *)DeleteAudioDynamicStringEntry,
    (void *)AudioDynamicStringEntryNoop,
    (void *)PrepareMciMusicTrack,
    (void *)FreePreparedMciMusicTrack,
    (void *)PlayPreparedMciMusicTrack,
    (void *)StopMciMusicTrack,
    (void *)PauseMciMusicTrack,
    (void *)NoOpAudioInt,
    (void *)SetAudioDynamicStringEntryFlag,
    (void *)GetAudioDynamicStringEntryFlag,
};
static void *g_AudioDynamicStringEntryVtable = g_AUDIO_DynamicStringEntryVtable;
static VsSoundMciMusicBackend *g_pActiveMciMusicBackend = 0;
static int g_nPreparedMciMusicTrackHandle = 0;
static const char g_AUDIO_SequencerDeviceType[] = "sequencer";
static const char g_AUDIO_MciWindowClassName[] = "HLMusicWindow";
static const char g_AUDIO_DsoundDll[] = "DSOUND.DLL";
static const char g_AUDIO_DirectSoundCreate[] = "DirectSoundCreate";
static const char g_AUDIO_UnknownDirectSoundError[] = "UNKNOWN DIRECT SOUND ERROR: ";
static char g_AUDIO_UnknownDirectSoundErrorBuffer[0x80];
struct VsSoundDirectSoundErrorEntry {
    const char *pszName;
    unsigned int uCode;
};
static const VsSoundDirectSoundErrorEntry g_AUDIO_DirectSoundErrors[] = {
    { "DSERR_ALLOCATED", 0x0000000a },
    { "DSERR_CONTROLUNAVAIL", 0x0000001e },
    { "DSERR_INVALIDPARAM", 0x80070057 },
    { "DSERR_INVALIDCALL", 0x00000032 },
    { "DSERR_GENERIC", 0x80004005 },
    { "DSERR_PRIOLEVELNEEDED", 0x00000046 },
    { "DSERR_OUTOFMEMORY", 0x8007000e },
    { "DSERR_BADFORMAT", 0x00000064 },
    { "DSERR_UNSUPPORTED", 0x80004001 },
    { "DSERR_NODRIVER", 0x00000078 },
    { "DSERR_ALREADYINITIALIZED", 0x00000082 },
    { "DSERR_NOAGGREGATION", 0x80040110 },
    { "DSERR_BUFFERLOST", 0x00000096 },
    { "DSERR_OTHERAPPHASPRIO", 0x000000a0 },
    { 0, 0 }
};
static const char g_AUDIO_NoEffectsBackendName[] = "ERROR: No Effects Device for Windows";
static const char g_AUDIO_NoDirectSoundBackendName[] = "ERROR: No Effects Device for Windows";
static const char g_AUDIO_HlMidiNotFoundPrefix[] = "Error: HL Midi Device Not Found. ";
static const char g_AUDIO_MciErrorPrefix[] = "MCI Error: ";
static const char g_AUDIO_ErrorSuffix[] = "\n";
static const char g_AUDIO_MusicExtension[] = ".mid";
static const char g_AUDIO_PathSeparator[] = "\\";

// FUNCTION: LEMBALL 0x0047E900
LRESULT CALLBACK MciMusicWindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
    if (uMessage == 0x3b9 && wParam == 1) {
        VsSoundMciMusicBackend *pBackend = g_pActiveMciMusicBackend;
        if (pBackend != 0) {
            ((VsSoundMciMusicBackendVtableModel *)pBackend)->Resume(
                g_nPreparedMciMusicTrackHandle);
        }
    }
    return DefWindowProcA(hWnd, uMessage, wParam, lParam);
}

// FUNCTION: LEMBALL 0x0045AF80
void *VsSoundManager::ConstructAudioManager(int nMusicResourceBase,
                                           int cMusicResources,
                                           int fMusicBackendEnabled,
                                           int cEffectResources,
                                           void *pPrimaryContext) {
    void *pAudioManager = this;
    void **ppBackend;
    unsigned int i;
    int fMusicBackendReady;
    int nMusicBase;
    int cBackends;
    int fBackendReady;

    *(void ***)pAudioManager = g_AudioManagerVtable;
    nMusicBase = 0;
    if (fMusicBackendEnabled == 0) {
        nMusicBase = nMusicResourceBase;
    }
    *(int *)((char *)pAudioManager + 8) = 1;
    *(int *)((char *)pAudioManager + 0x14) = nMusicBase;
    *(int *)((char *)pAudioManager + 0x18) = cMusicResources;
    *(void **)((char *)pAudioManager + 0x70) = 0;
    *(int *)((char *)pAudioManager + 0x6c) = 1;
    ppBackend = (void **)((char *)pAudioManager + 0x24);
    fMusicBackendReady = 0;
    *(int *)((char *)pAudioManager + 0x1c) = fMusicBackendEnabled;
    *(void **)((char *)pAudioManager + 0x34) = 0;
    cBackends = SelectAudioManagerBackends(ppBackend,
                                           nMusicBase,
                                           cMusicResources,
                                           fMusicBackendEnabled,
                                           &fMusicBackendReady,
                                           (void **)((char *)pAudioManager + 0x34),
                                           cEffectResources);
    *(int *)((char *)pAudioManager + 0x20) = cBackends;
    *(void **)((char *)pAudioManager + 0x74) = 0;
    *(void **)((char *)pAudioManager + 0x78) = 0;
    for (i = 0; i < (unsigned int)cBackends; ++i) {
        if (ppBackend[i] != 0) {
            fBackendReady = ((VsSoundTestProc)(*(void ***)ppBackend[i])[9])(
                ppBackend[i], 0);
            if (fBackendReady == 1) {
                *(void **)((char *)pAudioManager + 0x74) = ppBackend[i];
            }
            fBackendReady = ((VsSoundTestProc)(*(void ***)ppBackend[i])[10])(
                ppBackend[i], 0);
            if (fBackendReady == 1) {
                *(void **)((char *)pAudioManager + 0x78) = ppBackend[i];
            }
        }
    }
    *(int *)((char *)pAudioManager + 0xc) = 0;
    *(int *)((char *)pAudioManager + 0x10) = 0;
    if (*(int *)((char *)pAudioManager + 0x1c) == 1) {
        if (fMusicBackendReady == 0) {
            *(int *)((char *)pAudioManager + 0xc) = 0;
            *(void **)((char *)pAudioManager + 0x74) = 0;
        } else {
            *(int *)((char *)pAudioManager + 0xc) = 1;
        }
    }
    if (*(int *)((char *)pAudioManager + 0x18) != 0) {
        SetAudioManagerEffectResourceContext(pAudioManager, pPrimaryContext);
    }
    if (*(int *)((char *)pAudioManager + 0x14) != 0) {
        SetAudioManagerMusicResourceContext(pAudioManager, pPrimaryContext);
    }
    RefreshAudioManagerBackendHandles(pAudioManager);

    if (*(int *)((char *)pAudioManager + 0xc) == 1) {
        *(unsigned char *)((char *)pAudioManager + 0xc7c) = 1;
        *(unsigned char *)((char *)pAudioManager + 0xc7e) = 1;
        if (*(void **)((char *)pAudioManager + 0x74) != 0) {
            *(unsigned char *)((char *)pAudioManager + 0xc7e) =
                ((VsSoundTestProc)(*(void ***)*(void **)((char *)pAudioManager + 0x74))[11])(
                    *(void **)((char *)pAudioManager + 0x74), 0);
        }
    } else {
        *(unsigned char *)((char *)pAudioManager + 0xc7c) = 0;
        *(unsigned char *)((char *)pAudioManager + 0xc7e) = 0;
    }

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        *(unsigned char *)((char *)pAudioManager + 0xc7d) = 1;
        *(unsigned char *)((char *)pAudioManager + 0xc7f) =
            ((VsSoundTestProc)(*(void ***)*(void **)((char *)pAudioManager + 0x78))[12])(
                *(void **)((char *)pAudioManager + 0x78), 0);
    } else {
        *(unsigned char *)((char *)pAudioManager + 0xc7d) = 0;
        *(unsigned char *)((char *)pAudioManager + 0xc7f) = 0;
    }
    *(int *)((char *)pAudioManager + 0xc80) = *(int *)((char *)pAudioManager + 0xc);
    *(int *)((char *)pAudioManager + 0xc84) = *(int *)((char *)pAudioManager + 0x10);
    *(int *)((char *)pAudioManager + 0xc88) = 1;
    if (*(unsigned char *)((char *)pAudioManager + 0xc7f) < 2) {
        *(int *)((char *)pAudioManager + 0xc88) = 0;
    }
    *(int *)((char *)pAudioManager + 0xc8c) = *(int *)((char *)pAudioManager + 0xc);
    return pAudioManager;
}

// FUNCTION: LEMBALL 0x0045B110
void DestroyAudioManager(void *pAudioManager) {
    void **ppBackend;
    unsigned int i;
    void *pBackend;

    *(void ***)pAudioManager = g_AudioManagerVtable;
    if (*(int *)((char *)pAudioManager + 0xc) != 0 && *(void **)((char *)pAudioManager + 0x74) != 0) {
        pBackend = *(void **)((char *)pAudioManager + 0x74);
        ((VsSoundVoidProc)(*(void ***)pBackend)[7])();
    }
    if (*(int *)((char *)pAudioManager + 0x10) != 0 && *(void **)((char *)pAudioManager + 0x78) != 0) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        ((VsSoundVoidProc)(*(void ***)pBackend)[8])();
    }
    *(void **)((char *)pAudioManager + 0x74) = 0;
    *(void **)((char *)pAudioManager + 0x78) = 0;
    ppBackend = (void **)((char *)pAudioManager + 0x24);
    for (i = 0; i < *(unsigned int *)((char *)pAudioManager + 0x20); ++i) {
        if (ppBackend[i] != 0) {
            ((VsSoundIntProc)(*(void ***)ppBackend[i])[22])(0);
            ((VsSoundVoidProc)(*(void ***)ppBackend[i])[5])();
            ((VsSoundDeleteProc)(*(void ***)ppBackend[i])[0])(ppBackend[i], 0, 1);
        }
    }
    if (*(void **)((char *)pAudioManager + 0x34) != 0) {
        ((VsSoundDeleteProc)(*(void ***)*(void **)((char *)pAudioManager + 0x34))[0])(
            *(void **)((char *)pAudioManager + 0x34), 0, 1);
    }
}

// FUNCTION: LEMBALL 0x0045B190
void SetAudioManagerPrimaryContext(void *pAudioManager, void *pPrimaryContext) {
    int fAccepted;
    void *pBackend;

    *(void **)((char *)pAudioManager + 0x70) = pPrimaryContext;
    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (*(int *)((char *)pAudioManager + 8) == 0 && pBackend != 0) {
        fAccepted = ((VsSoundOpenProc)(*(void ***)pBackend)[4])(
            pBackend, 0, 0, *(int *)((char *)pAudioManager + 0x18), pPrimaryContext);
        if (fAccepted == 0) {
            *(int *)((char *)pAudioManager + 0x18) = 0;
        }
    }
}

// FUNCTION: LEMBALL 0x0045B1D0
void LEMBALL_FASTCALL StopAllAudioManagerBackends(void *pAudioManager) {
    void **ppBackend;
    unsigned int i;

    if (*(int *)((char *)pAudioManager + 8) == 0) {
        *(int *)((char *)pAudioManager + 8) = 1;
        ppBackend = (void **)((char *)pAudioManager + 0x24);
        for (i = 0; i < *(unsigned int *)((char *)pAudioManager + 0x20); ++i) {
            if (ppBackend[i] != 0) {
                ((VsSoundVoidProc)(*(void ***)ppBackend[i])[5])();
            }
        }
    }
}

// FUNCTION: LEMBALL 0x0045B210
void __fastcall RefreshAudioManagerBackendHandles(void *pAudioManager) {
    VsSoundBackendOpenInterface *pMusicBackend;
    int fAccepted;
    int nMusicResourceBase;
    int cEffectResources;

    if (*(int *)((char *)pAudioManager + 8) != 0) {
        nMusicResourceBase = *(int *)((char *)pAudioManager + 0x14);
        cEffectResources = 0;
        *(int *)((char *)pAudioManager + 8) = 0;
        pMusicBackend = *(VsSoundBackendOpenInterface **)((char *)pAudioManager + 0x74);
        if (*(VsSoundBackendOpenInterface **)((char *)pAudioManager + 0x78) == pMusicBackend) {
            *(void **)((char *)pAudioManager + 0x78) = 0;
            cEffectResources = *(int *)((char *)pAudioManager + 0x18);
        }
        if (*(int *)((char *)pAudioManager + 0x1c) == 0 && pMusicBackend != 0) {
            fAccepted = pMusicBackend->Open(
                nMusicResourceBase, cEffectResources,
                *(void **)((char *)pAudioManager + 0x70));
            if (fAccepted == 0) {
                nMusicResourceBase = 0;
                cEffectResources = 0;
            }
            *(int *)((char *)pAudioManager + 0xc) = nMusicResourceBase;
            *(int *)((char *)pAudioManager + 0x10) = cEffectResources;
        }
        if (*(VsSoundBackendOpenInterface **)((char *)pAudioManager + 0x78) != 0) {
            pMusicBackend = *(VsSoundBackendOpenInterface **)((char *)pAudioManager + 0x78);
            fAccepted = pMusicBackend->Open(
                0, *(int *)((char *)pAudioManager + 0x18),
                *(void **)((char *)pAudioManager + 0x70));
            if (fAccepted == 0) {
                *(int *)((char *)pAudioManager + 0x18) = 0;
            }
            *(int *)((char *)pAudioManager + 0x10) = *(int *)((char *)pAudioManager + 0x18);
        } else if (*(int *)((char *)pAudioManager + 0x10) != 0) {
            *(void **)((char *)pAudioManager + 0x78) = *(void **)((char *)pAudioManager + 0x74);
        }
    }
}

// FUNCTION: LEMBALL 0x0045B2C0
void VsSoundManager::InvokeAudioManagerEmbeddedSlot04(int nValue1, int nValue2) {
    VsSoundBackendControlDispatch *pControl;

    pControl = *(VsSoundBackendControlDispatch **)((char *)this + 0x34);
    pControl->InvokeEmbeddedSlot04(nValue1, nValue2);
}

// FUNCTION: LEMBALL 0x0045B2E0
int LEMBALL_FASTCALL RegisterVariantResourceMusicHandle(
    void *pAudioManager, int, int nMusicResourceId) {
    int hMusic;
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        hMusic = *(int *)((char *)pAudioManager + 0x6c);
        *(int *)((char *)pAudioManager + 0x6c) = hMusic + 1;
        if (hMusic + 1 == 0) {
            *(int *)((char *)pAudioManager + 0x6c) = 1;
        }
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((VsSoundControlPairProc)(*(void ***)pControl)[2])(
            pControl, 0, hMusic, nMusicResourceId);
        return hMusic;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045B330
void LEMBALL_FASTCALL StartRegisteredVariantResourceMusic(
    void *pAudioManager, int, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && hMusic != 0 &&
        *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((VsSoundControlIntProc)(*(void ***)pControl)[4])(
            pControl, 0, hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B370
void LEMBALL_FASTCALL DispatchChildInterfaceSlot18IfReady(
    void *pAudioManager, int, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((VsSoundControlIntProc)(*(void ***)pControl)[6])(
            pControl, 0, hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B390
void LEMBALL_FASTCALL InvokeAudioManagerEmbeddedSlot1cIfMusicActive(
    void *pAudioManager, int, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((VsSoundControlIntProc)(*(void ***)pControl)[7])(
            pControl, 0, hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B3B0
void LEMBALL_FASTCALL StopVariantResourceMusicPlayback(
    void *pAudioManager, int, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((VsSoundControlIntProc)(*(void ***)pControl)[5])(
            pControl, 0, hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B3D0
void LEMBALL_FASTCALL UnregisterVariantResourceMusicHandle(
    void *pAudioManager, int, int hMusic) {
    void *pControl;

    if (*(int *)((char *)pAudioManager + 0xc) == 1 && hMusic != 0 &&
        *(int *)((char *)pAudioManager + 0x1c) == 1) {
        pControl = *(void **)((char *)pAudioManager + 0x34);
        ((VsSoundControlIntProc)(*(void ***)pControl)[3])(
            pControl, 0, hMusic);
    }
}

// FUNCTION: LEMBALL 0x0045B3F0
int CreateVariantResourceEffectInstance(void *pAudioManager, int nEffectResourceId) {
    VsSoundResourceObject *pEffectResource;
    int nEffectInstanceId;
    int nEffectData;

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        pEffectResource = (VsSoundResourceObject *)LoadEffResource(nEffectResourceId);
        if (pEffectResource->m_nLoadState10 == 0) {
            ((VsSoundVoidProc)pEffectResource->m_pVtable[7])();
        } else {
            pEffectResource->m_nReserved24 = 0;
        }

        ++pEffectResource->m_nLockCount08;
        nEffectData = ((VsSoundReturnIntProc)pEffectResource->m_pVtable[10])();
        {
            void *pEffectBackend =
                *(void **)((char *)pAudioManager + 0x78);
            ((VsSoundCreateEffectProc)(*(void ***)pEffectBackend)[15])(
                pEffectBackend, 0, nEffectData, &nEffectInstanceId);
        }
        --pEffectResource->m_nLockCount08;
        ReleaseTypedResourceObjectReference(pEffectResource);
        return nEffectInstanceId;
    }

    nEffectInstanceId = 0;
    return 0;
}

// FUNCTION: LEMBALL 0x0045B460
void VsSoundManager::PlayVariantResourceEffectId(int nEffectResourceId) {
    VsSoundEffectPlaybackInterface *pBackend;
    int nReserved;

    nReserved = 0;
    if (*(int *)((char *)this + 0x10) == 1) {
        pBackend = *(VsSoundEffectPlaybackInterface **)((char *)this + 0x78);
        pBackend->PlayEffect(nEffectResourceId,
                             pBackend->GetEffectVolume(),
                             nReserved);
    }
}

// FUNCTION: LEMBALL 0x0045B490
void VsSoundManager::SetVariantResourceEffectInstanceVolume(int nEffectInstanceId, int nVolume) {
    VsSoundEffectVolumeInterface *pBackend;

    if (*(int *)((char *)this + 0x10) == 1) {
        pBackend = *(VsSoundEffectVolumeInterface **)((char *)this + 0x78);
        pBackend->SetEffectVolume(nEffectInstanceId, nVolume, 0);
    }
}

// FUNCTION: LEMBALL 0x0045B4F0
void InvokeAudioManagerEffectSlot48IfActive(void *pAudioManager, int nEffect) {
    void *pBackend;

    if (*(int *)((char *)pAudioManager + 0x10) == 1) {
        pBackend = *(void **)((char *)pAudioManager + 0x78);
        ((VsSoundIntProc)(*(void ***)pBackend)[18])(nEffect);
    }
}

// FUNCTION: LEMBALL 0x0045B510
void SetAudioManagerMusicAndEffectVolume(void *pAudioManager, int nMusicVolume, int nEffectVolume) {
    void *pBackend;
    void *pControl;

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (nMusicVolume != -1 && pBackend != 0) {
        ((VsSoundIntProc)(*(void ***)pBackend)[26])(nMusicVolume);
    }
    if (nEffectVolume != -1) {
        if (*(int *)((char *)pAudioManager + 0x1c) != 0) {
            pControl = *(void **)((char *)pAudioManager + 0x34);
            ((VsSoundIntProc)(*(void ***)pControl)[8])(nEffectVolume);
        } else {
            pBackend = *(void **)((char *)pAudioManager + 0x74);
            if (pBackend != 0) {
                ((VsSoundIntProc)(*(void ***)pBackend)[24])(nEffectVolume);
            }
        }
    }
}

// FUNCTION: LEMBALL 0x0045B560
unsigned int GetVariantResourceEffectMasterVolume(void *pAudioManager) {
    void *pBackend;

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (pBackend != 0) {
        return ((VsSoundReturnIntProc)(*(void ***)pBackend)[25])();
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045B580
void SetAudioManagerEffectResourceContext(void *pAudioManager, void *pPrimaryContext) {
    void *pBackend;

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    ((VsSoundContextProc)(*(void ***)pBackend)[38])(
        pBackend, 0, pPrimaryContext);
}

// FUNCTION: LEMBALL 0x0045B5A0
void SetAudioManagerMusicResourceContext(void *pAudioManager, void *pPrimaryContext) {
    void *pBackend;

    pBackend = *(void **)((char *)pAudioManager + 0x74);
    if (pBackend != 0) {
        ((VsSoundContextProc)(*(void ***)pBackend)[38])(
            pBackend, 0, pPrimaryContext);
    }
}

// FUNCTION: LEMBALL 0x0045B5C0
void VsSoundManager::SetAudioManagerStartupMusicName(const char *pszMusicName) {
    VsSoundBackendControl *pControl;

    pControl = *(VsSoundBackendControl **)((char *)this + 0x34);
    pControl->m_StartupMusicName.AssignDynamicCString(pszMusicName);
    pControl->m_fStartupMusicNameSet = 1;
}

// FUNCTION: LEMBALL 0x0045B5F0
void VsSoundManager::SetAudioManagerMusicEnabledFlag(int fEnabled) {
    VsSoundBackendControl *pControl;

    pControl = *(VsSoundBackendControl **)((char *)this + 0x34);
    pControl->m_fMusicEnabled = fEnabled;
}

// FUNCTION: LEMBALL 0x0045B600
char *__fastcall BuildAudioManagerDescriptionString(void *pAudioManager) {
    VsInitEffFormattedOutputStream DescriptionStream;
    VsInitFormattedOutputStream *pFormattedStream;
    void *pBackend;

    DescriptionStream.m_pVtable = g_EffFormattedOutputStreamVtable;
    DescriptionStream.m_FormattedSubobject.m_pVtable = g_EffStreamSubobjectVtable;
    DescriptionStream.m_TargetState.m_pVtable = g_StreamFormatSubobjectVtable;
    DescriptionStream.m_TargetState.m_dwFlags = 0x14;
    DescriptionStream.m_TargetState.m_chFill = ' ';
    DescriptionStream.m_TargetState.m_nWidth = 0;
    DescriptionStream.m_TargetState.m_nRadix = 10;
    DescriptionStream.m_FixedBufferStream.ConstructFixedBufferStream(
        g_szAudioManagerDescription, sizeof(g_szAudioManagerDescription), 0);
    DescriptionStream.m_TargetState.m_pDownstream = &DescriptionStream.m_FixedBufferStream;
    pFormattedStream = (VsInitFormattedOutputStream *)&DescriptionStream.m_FormattedSubobject;
    g_szAudioManagerDescription[0] = '\0';

    pBackend = *(void **)((char *)pAudioManager + 0x78);
    if (pBackend != 0 && *(int *)((char *)pAudioManager + 0x18) != 0) {
        pFormattedStream->AppendCStringToStream("Effects : ")
            ->AppendCStringToStream(((VsSoundNameProc)(*(void ***)pBackend)[1])());
    }

    if (*(int *)((char *)pAudioManager + 0x1c) != 0 && *(int *)((char *)pAudioManager + 0x14) != 0) {
        pFormattedStream->AppendCStringToStream(
            ((VsSoundNameProc)(*(void ***)*(void **)((char *)pAudioManager + 0x34))[12])())
            ->AppendCStringToStream("\n");
    }

    pBackend = *(void **)((char *)pAudioManager + 0x74);
    if (pBackend != 0 && *(int *)((char *)pAudioManager + 0x14) != 0) {
        pFormattedStream->AppendCStringToStream("Music : ")
            ->AppendCStringToStream(((VsSoundNameProc)(*(void ***)pBackend)[1])());
    }

    DestroyFixedBufferStream(&DescriptionStream.m_FixedBufferStream);
    RestoreStreamFormatSubobjectVtable(&DescriptionStream.m_TargetState);
    ConstructStreamFormatState(&DescriptionStream.m_TargetState);
    return g_szAudioManagerDescription;
}

// FUNCTION: LEMBALL 0x0045B770
int InitializeGlobalAudioManager(int nMusicResourceBase,
                                 int cMusicResources,
                                 int cEffectResources,
                                 void *pPrimaryContext) {
    void *pAudioManager;

    pAudioManager = AllocateVSMemBlock(0xc90);
    if (pAudioManager != 0) {
        g_pAudioManager =
            ((VsSoundManager *)pAudioManager)->ConstructAudioManager(
                nMusicResourceBase, cMusicResources, 1, cEffectResources, pPrimaryContext);
        return 1;
    }
    g_pAudioManager = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x0045B7C0
void ShutdownGlobalAudioManager(void) {
    void *pAudioManager;

    pAudioManager = g_pAudioManager;
    if (g_pAudioManager != 0) {
        DestroyAudioManager(g_pAudioManager);
        FreeVSMemBlock(pAudioManager);
    }
    g_pAudioManager = 0;
}

// FUNCTION: LEMBALL 0x0047C880
VsSoundWaveOutEffectBackend *VsSoundWaveOutEffectBackend::ConstructWaveOutEffectBackend(
    int cEffectResources) {
    unsigned int cBytes;
    unsigned int i;
    unsigned int deviceId;
    unsigned int cDevices;
    unsigned int dwFormats;
    int fFound;
    int nFormat;
    unsigned short nChannels;
    unsigned short nBitsPerSample;
    int nBytesPerSecond;
    unsigned short nBlockAlign;
    MMRESULT nResult;
    unsigned char *pbBackend;

    void *pBackend = this;

    ConstructAudioEffectBackendBase(pBackend);
    pbBackend = (unsigned char *)pBackend;
    *(void ***)pbBackend = g_WaveOutEffectBackendVtable;
    *(int *)(pbBackend + 4) = cEffectResources;
    cBytes = (unsigned int)cEffectResources * 4;
    *(void **)(pbBackend + 0xa8) = AllocateVSMemBlock(cBytes);
    *(void **)(pbBackend + 0xb0) = AllocateVSMemBlock(cBytes);
    *(void **)(pbBackend + 0xac) = AllocateVSMemBlock(cBytes);
    *(int *)(pbBackend + 8) = 0;
    *(int *)(pbBackend + 0xc) = 0;
    *(int *)(pbBackend + 0x10) = 0;
    *(int *)(pbBackend + 0x14) = 0;
    *(int *)(pbBackend + 0x18) = 0;
    *(int *)(pbBackend + 0x88) = -1;
    *(int *)(pbBackend + 0x84) = 0;
    for (i = 0; i < 8; ++i) {
        *(int *)(pbBackend + 0x20 + i * 4) = -1;
        pbBackend[0x40 + i] = 0;
        pbBackend[0x48 + i] = 0;
    }
    *(int *)(pbBackend + 0xa4) = 1;
    for (i = 0; i < (unsigned int)cEffectResources; ++i) {
        *(int *)((char *)*(void **)(pbBackend + 0xa8) + i * 4) = 0;
        *(int *)((char *)*(void **)(pbBackend + 0xac) + i * 4) = 0;
        *(int *)((char *)*(void **)(pbBackend + 0xb0) + i * 4) = 0;
    }

    fFound = 0;
    cDevices = waveOutGetNumDevs();
    for (deviceId = 0; deviceId < cDevices; ++deviceId) {
        if (fFound != 0) {
            return this;
        }
        nResult = waveOutGetDevCapsA(deviceId, pbBackend + 0x50, 0x34);
        if (nResult == 0) {
            dwFormats = *(unsigned int *)(pbBackend + 0x78);
            if ((dwFormats & 1) != 0) {
                *(int *)(pbBackend + 0xc) = 1;
                *(unsigned int *)(pbBackend + 0x88) = deviceId;
                *(int *)(pbBackend + 0x14) = 0;
                *(int *)(pbBackend + 0x84) = 0x2b11;
            }
            if ((dwFormats & 4) != 0) {
                *(int *)(pbBackend + 0x14) = 1;
                *(int *)(pbBackend + 0xc) = 1;
                *(int *)(pbBackend + 0x84) = 0x2b11;
                *(unsigned int *)(pbBackend + 0x88) = deviceId;
            }
            if ((dwFormats & 0x10) != 0) {
                *(int *)(pbBackend + 0xc) = 1;
                *(unsigned int *)(pbBackend + 0x88) = deviceId;
                *(int *)(pbBackend + 0x14) = 0;
                *(int *)(pbBackend + 0x84) = 0x5622;
            }
            if ((dwFormats & 0x40) != 0) {
                *(int *)(pbBackend + 0x14) = 1;
                *(int *)(pbBackend + 0xc) = 1;
                *(int *)(pbBackend + 0x84) = 0x5622;
                *(unsigned int *)(pbBackend + 0x88) = deviceId;
            }
        }
        nFormat = *(int *)(pbBackend + 0x84);
        if (nFormat != 0) {
            *(unsigned short *)(pbBackend + 0x90) = 1;
            nChannels = *(int *)(pbBackend + 0x10) == 1 ? 2 : 1;
            nBitsPerSample = *(int *)(pbBackend + 0x14) == 1 ? 16 : 8;
            *(unsigned short *)(pbBackend + 0x92) = nChannels;
            *(int *)(pbBackend + 0x94) = nFormat;
            *(int *)(pbBackend + 0x98) = 1;
            *(unsigned short *)(pbBackend + 0x9e) = nBitsPerSample;
            nBytesPerSecond = (int)nChannels * nFormat;
            *(int *)(pbBackend + 0x98) = nBytesPerSecond;
            nBlockAlign = (unsigned short)((((int)nBitsPerSample * (int)nChannels) + 7) >> 3);
            *(unsigned short *)(pbBackend + 0x9c) = nBlockAlign;
            if (*(int *)(pbBackend + 0x14) == 1) {
                *(int *)(pbBackend + 0x98) = nBytesPerSecond * 2;
            }
            fFound = 1;
        }
    }
    return this;
}

// FUNCTION: LEMBALL 0x0047CAF0
char *GetWaveOutEffectBackendDescription(VsSoundWaveOutEffectBackend *pBackend) {
    if (pBackend->m_fDeviceAvailable == 1) {
        return pBackend->m_szDeviceDescription;
    }
    return (char *)g_AUDIO_NoEffectsBackendName;
}

// FUNCTION: LEMBALL 0x0047CC10
int GetWaveOutEffectBackendStatus0047CC10(void *pBackend) {
    return *(int *)((char *)pBackend + 0x0c);
}

// FUNCTION: LEMBALL 0x0047CB00
int InitializeWaveOutEffectDevice0047CB00(void *pBackend,
                                           int fActivateDevice,
                                           int fStartStreaming) {
    HWAVEOUT *pWaveOut;
    MMRESULT nResult;
    char szErrorText[0x100];

    if (fActivateDevice == 1) {
        return *(int *)((char *)pBackend + 0x08);
    }
    if (fStartStreaming == 1) {
        pWaveOut = (HWAVEOUT *)((char *)pBackend + 0x8c);
        nResult = waveOutOpen(pWaveOut,
                              *(UINT *)((char *)pBackend + 0x88),
                              (const void *)((char *)pBackend + 0x90),
                              0,
                              0,
                              0);
        if (nResult != 0) {
            AppendCStringToStream(g_pErrorOutputStream,
                                  "Error! Windows Effect device cannot be opened!\n");
            waveOutGetErrorTextA(nResult, szErrorText, sizeof(szErrorText));
            AppendCStringToStream(g_pErrorOutputStream, szErrorText);
            AppendCStringToStream(g_pErrorOutputStream, "\n");
        }
        if ((*(unsigned char *)((char *)pBackend + 0x80) & 4) != 0) {
            waveOutGetVolume(*pWaveOut, (DWORD *)((char *)pBackend + 0xb4));
            waveOutSetVolume(*pWaveOut, 0xffffffff);
        }
        if (nResult != 0) {
            *(int *)((char *)pBackend + 0x0c) = 0;
            return 0;
        }
        *(int *)((char *)pBackend + 0x0c) = 1;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0047CBE0
int IsWaveOutEffectBackendAvailable(void) {
    return 1;
}

// FUNCTION: LEMBALL 0x0047CC20
int LEMBALL_FASTCALL ResetAndCloseWaveOutEffectDevice0047CC20(void *pBackend) {
    MMRESULT nResult;
    unsigned int i;
    char szErrorText[0x100];
    char *(*pGetDescription)(void *pThis);

    pGetDescription = (char *(*)(void *))(*(void ***)pBackend + 1);

    if (*(HWAVEOUT *)((char *)pBackend + 0x8c) != 0) {
        nResult = (MMRESULT)-1;
        i = 0;
        do {
            if (i >= 500) {
                break;
            }
            ++i;
            nResult = waveOutReset(*(HWAVEOUT *)((char *)pBackend + 0x8c));
        } while (nResult != 0);
        if (i == 500) {
            AppendCStringToStream(g_pErrorOutputStream,
                                  "Error: Shutting Down Wave Device: ");
            AppendCStringToStream(g_pErrorOutputStream,
                                  pGetDescription(pBackend));
            AppendCStringToStream(g_pErrorOutputStream,
                                  "\nSystem may be unstable. ");
            waveOutGetErrorTextA(nResult, szErrorText, sizeof(szErrorText));
            AppendCStringToStream(g_pErrorOutputStream, szErrorText);
            AppendCStringToStream(g_pErrorOutputStream, "\n");
            return 0;
        }
        if ((*(unsigned char *)((char *)pBackend + 0x80) & 4) != 0) {
            waveOutSetVolume(*(HWAVEOUT *)((char *)pBackend + 0x8c),
                             *(DWORD *)((char *)pBackend + 0xb4));
        }
        if (*(HWAVEOUT *)((char *)pBackend + 0x8c) != 0) {
            nResult = (MMRESULT)-1;
            i = 0;
            do {
                if (i >= 500) {
                    break;
                }
                ++i;
                nResult = waveOutClose(*(HWAVEOUT *)((char *)pBackend + 0x8c));
            } while (nResult != 0);
            if (i == 500) {
                AppendCStringToStream(g_pErrorOutputStream,
                                      "Error: Closing Down Wave Device: ");
                AppendCStringToStream(g_pErrorOutputStream,
                                      pGetDescription(pBackend));
                AppendCStringToStream(g_pErrorOutputStream,
                                      "\nSystem may be unstable. ");
                waveOutGetErrorTextA(nResult, szErrorText, sizeof(szErrorText));
                AppendCStringToStream(g_pErrorOutputStream, szErrorText);
                AppendCStringToStream(g_pErrorOutputStream, "\n");
                return 0;
            }
        }
    }
    *(HWAVEOUT *)((char *)pBackend + 0x8c) = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x0047CDD0
int IsWaveOutEffectInstanceAvailable0047CDD0(void *pBackend) {
    int i;

    for (i = 0; i < 8; ++i) {
        if (*(unsigned char *)((char *)pBackend + 0x48 + i) == 0) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047CDF0
int ReturnOneWaveOutBackendValue0047CDF0(void *pBackend) {
    (void)pBackend;
    return 1;
}

// FUNCTION: LEMBALL 0x0047CF10
int CreateWaveOutEffectInstance(void *pBackend, void *pPatchResource, int *pnEffectInstanceId) {
    unsigned int i;
    int *pnSlotInUse;
    int nBackendVtable;
    void *pPatchBuffer;
    int nEffectInstanceId;
    int nNextEffectInstanceId;

    if (*(unsigned int *)((char *)pBackend + 4) != 0) {
        pnSlotInUse = *(int **)((char *)pBackend + 0xac);
        for (i = 0; i < *(unsigned int *)((char *)pBackend + 4); ++i) {
            if (*pnSlotInUse == 0) {
                nBackendVtable = *(int *)pBackend;
                ((VsSoundVoidProc)*(void **)(nBackendVtable + 0x14))();
                pPatchBuffer = AllocateVSMemBlock(0x18);
                if (pPatchBuffer == 0) {
                    *(void **)(*(int *)((char *)pBackend + 0xa8) + i * 4) = 0;
                } else {
                    pPatchBuffer = g_AUDIO_ConstructPatchProc(
                        pPatchBuffer, pPatchResource, *(int *)((char *)pBackend + 0x8c), *(int *)((char *)pBackend + 0x84),
                        *(int *)((char *)pBackend + 0x14), *(int *)((char *)pBackend + 0x10));
                    *(void **)(*(int *)((char *)pBackend + 0xa8) + i * 4) = pPatchBuffer;
                }
                ((void (*)(int, int, int))*(void **)(nBackendVtable + 0xc))(0, 1, 0);
                *(int *)(*(int *)((char *)pBackend + 0xac) + i * 4) = 1;
                *(int *)(*(int *)((char *)pBackend + 0xb0) + i * 4) = *(int *)((char *)pBackend + 0xa4);
                *pnEffectInstanceId = *(int *)((char *)pBackend + 0xa4);
                nEffectInstanceId = *(int *)((char *)pBackend + 0xa4);
                nNextEffectInstanceId = nEffectInstanceId + 1;
                *(int *)((char *)pBackend + 0xa4) = nNextEffectInstanceId;
                if (nNextEffectInstanceId == 0) {
                    *(int *)((char *)pBackend + 0xa4) = nEffectInstanceId + 2;
                }
                return 1;
            }
            ++pnSlotInUse;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047D010
int FreeWaveOutEffectInstance(void *pBackend, int nEffectInstanceId) {
    unsigned int i;
    int nOffset;
    void *pPatchBuffer;

    if (*(int *)((char *)pBackend + 4) != 0) {
        nOffset = 0;
        for (i = 0; i < *(unsigned int *)((char *)pBackend + 4); ++i) {
            if (*(int *)(*(int *)((char *)pBackend + 0xb0) + nOffset) == nEffectInstanceId) {
                pPatchBuffer = *(void **)(*(int *)((char *)pBackend + 0xa8) + nOffset);
                if (pPatchBuffer != 0) {
                    ((VsNetFileBackedTimedChannelWrapperView *)pPatchBuffer)
                        ->DeleteTimedFileBackedEffChannelWrapper(1);
                    FreeVSMemBlock(pPatchBuffer);
                }
                *(int *)(*(int *)((char *)pBackend + 0xac) + nOffset) = 0;
                *(int *)(*(int *)((char *)pBackend + 0xb0) + nOffset) = 0;
            }
            nOffset += 4;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047D160
unsigned int PlayWaveOutEffectInstance(void *pBackend, int nEffectInstanceId) {
    unsigned int i;
    int nOffset;
    MMRESULT nWaveResult;

    if (*(int *)((char *)pBackend + 4) != 0) {
        nOffset = 0;
        for (i = 0; i < *(unsigned int *)((char *)pBackend + 4); ++i) {
            if (*(int *)(*(int *)((char *)pBackend + 0xb0) + nOffset) == nEffectInstanceId) {
                nWaveResult = waveOutReset(*(HWAVEOUT *)((char *)pBackend + 0x8c));
                if (nWaveResult != 0) {
                    AppendCStringToStream(g_pErrorOutputStream, "waveOutReset errored: ");
                    AppendUnsignedIntToStreamVariant(g_pErrorOutputStream, nWaveResult);
                    AppendCStringToStream(g_pErrorOutputStream, "\n");
                }
                nWaveResult = waveOutWrite(*(HWAVEOUT *)((char *)pBackend + 0x8c),
                                           *(LPWAVEHDR *)(*(int *)(*(int *)((char *)pBackend + 0xa8) + nOffset) + 0x10),
                                           0x20);
                if (nWaveResult != 0) {
                    AppendCStringToStream(g_pErrorOutputStream, "waveOutWrite (play effect) errored: ");
                    AppendUnsignedIntToStreamVariant(g_pErrorOutputStream, nWaveResult);
                    return (unsigned int)(unsigned char)'\n';
                }
            }
            nOffset += 4;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047CAA0
void LEMBALL_FASTCALL DestroyWaveOutEffectBackend(void *pBackend) {
    *(void ***)pBackend = g_WaveOutEffectBackendVtable;
    ResetAndCloseWaveOutEffectDevice0047CC20(pBackend);
    FreeVSMemBlock(*(void **)((char *)pBackend + 0xa8));
    FreeVSMemBlock(*(void **)((char *)pBackend + 0xb0));
    FreeVSMemBlock(*(void **)((char *)pBackend + 0xac));
    RestoreAudioEffectBackendBaseVtable(pBackend);
}

// FUNCTION: LEMBALL 0x0047D270
void *LEMBALL_FASTCALL DeleteWaveOutEffectBackend(
    VsSoundWaveOutEffectBackend *pBackend, int nUnused, int fDelete) {
    (void)nUnused;
    DestroyWaveOutEffectBackend(pBackend);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pBackend);
    }
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047D260
void LEMBALL_FASTCALL CaptureWaveOutBackendContext0047D260(
    void *, int, void *) {
}

// FUNCTION: LEMBALL 0x0047C210
unsigned short ByteSwapAudioWord(unsigned short uValue) {
    return (unsigned short)((uValue >> 8) + (uValue << 8));
}

// FUNCTION: LEMBALL 0x0047C230
unsigned int ByteSwapAudioDword(unsigned int uValue) {
    return ((unsigned int)ByteSwapAudioWord((unsigned short)uValue) << 16) +
           ByteSwapAudioWord((unsigned short)(uValue >> 16));
}

// FUNCTION: LEMBALL 0x0047D290
const char *LookupDirectSoundErrorString(unsigned int uError) {
    int i;

    strcpy(g_AUDIO_UnknownDirectSoundErrorBuffer,
           g_AUDIO_UnknownDirectSoundError);
    for (i = 0; g_AUDIO_DirectSoundErrors[i].uCode != 0; ++i) {
        if (g_AUDIO_DirectSoundErrors[i].uCode == uError)
            return g_AUDIO_DirectSoundErrors[i].pszName;
    }
    FormatSignedIntToRadixString(
        (int)uError,
        g_AUDIO_UnknownDirectSoundErrorBuffer +
            strlen(g_AUDIO_UnknownDirectSoundErrorBuffer),
        10);
    return g_AUDIO_UnknownDirectSoundErrorBuffer;
}

// FUNCTION: LEMBALL 0x0047DD80
VsSoundDirectSoundEffectBackend *VsSoundDirectSoundEffectBackend::ConstructDirectSoundEffectBackend(
    int cEffectResources, int nCooperativeLevel) {
    void *pBackend = this;
    int i;

    ConstructAudioEffectBackendBase(pBackend);
    *(void ***)pBackend = g_DirectSoundEffectBackendVtable;
    *(int *)((char *)pBackend + 0x14) = cEffectResources;
    *(int *)((char *)pBackend + 0x18) = nCooperativeLevel;
    *(int *)((char *)pBackend + 0x10) = 0;
    *(HMODULE *)((char *)pBackend + 4) = 0;
    *(FARPROC *)((char *)pBackend + 8) = 0;
    *(void **)((char *)pBackend + 0x50) =
        AllocateVSMemBlock((unsigned int)cEffectResources * 4 + 4);
    *(int *)((char *)pBackend + 0x0c) = 0;
    *(int *)((char *)pBackend + 0x1c) = 0;
    *(int *)((char *)pBackend + 0x20) = 0;
    *(int *)((char *)pBackend + 0x24) = 0;
    *(int *)((char *)pBackend + 0x28) = 1;
    *(int *)((char *)pBackend + 0x2c) = 0;
    *(int *)((char *)pBackend + 0x30) = 0;
    *(int *)((char *)pBackend + 0x34) = 0;
    *(int *)((char *)pBackend + 0x38) = -1;
    *(int *)((char *)pBackend + 0x34) = 0x5622;
    *(int *)((char *)pBackend + 0x38) = 0;
    if (*(void **)((char *)pBackend + 0x50) != 0) {
        for (i = 1; i <= cEffectResources; ++i) {
            *(int *)((char *)*(void **)((char *)pBackend + 0x50) + i * 4) = 0;
        }
    }
    *(int *)((char *)pBackend + 0x34) = 0x5622;
    *(int *)((char *)pBackend + 0x38) = 0;
    *(int *)((char *)pBackend + 0x40) = 0x5622;
    *(unsigned short *)((char *)pBackend + 0x4c) = 0;
    *(unsigned short *)((char *)pBackend + 0x4a) = 0x10;
    *(unsigned short *)((char *)pBackend + 0x48) = 2;
    *(unsigned short *)((char *)pBackend + 0x3c) = 1;
    *(int *)((char *)pBackend + 0x28) = 1;
    *(unsigned short *)((char *)pBackend + 0x3e) = 1;
    *(int *)((char *)pBackend + 0x44) = 1;
    *(int *)((char *)pBackend + 0x44) = 0xac44;
    *(HMODULE *)((char *)pBackend + 4) = LoadLibraryA(g_AUDIO_DsoundDll);
    if (*(HMODULE *)((char *)pBackend + 4) != 0) {
        *(FARPROC *)((char *)pBackend + 8) =
            GetProcAddress(*(HMODULE *)((char *)pBackend + 4),
                           g_AUDIO_DirectSoundCreate);
        if (*(FARPROC *)((char *)pBackend + 8) != 0) {
            *(int *)((char *)pBackend + 0x20) = 1;
            *(int *)((char *)pBackend + 0x0c) = 0;
        }
    }
    return this;
}

// FUNCTION: LEMBALL 0x0047E000
char *__fastcall GetDirectSoundEffectBackendDescription(VsSoundDirectSoundEffectBackend *pBackend) {
    if (pBackend->m_fDeviceAvailable == 1) {
        return (char *)"Direct Sound Device";
    }
    return (char *)g_AUDIO_NoDirectSoundBackendName;
}

// FUNCTION: LEMBALL 0x0047E020
int LEMBALL_FASTCALL InitializeDirectSoundEffectDevice(
    void *pBackend, int, int, int, void *) {
    VsSoundDirectSoundBufferDesc BufferDesc;
    VsSoundWaveFormat *pWaveFormat;
    LONG hResult;
    const char *pszError;
    VsInitFormattedOutputStream *pStream;

    hResult = ((VsSoundDirectSoundCreateProc)
        *(FARPROC *)((char *)pBackend + 8))(
            0, &g_pDirectSoundDevice, 0);
    if (hResult != 0) {
        pszError = LookupDirectSoundErrorString((unsigned int)hResult & 0xfff);
        pStream = AppendCStringToStream(
            g_pErrorOutputStream, "Direct Sound Create failed: ");
        pStream = AppendCStringToStream(pStream, pszError);
        AppendCStringToStream(pStream, "\n");
        *(int *)((char *)pBackend + 0x20) = 0;
        return 0;
    }

    memset(&BufferDesc, 0, sizeof(BufferDesc));
    BufferDesc.dwSize = sizeof(BufferDesc);
    BufferDesc.dwFlags = 1;
    hResult = VsSoundDIRECTSOUND_METHOD(
        g_pDirectSoundDevice, 6,
        VsSoundDirectSoundSetCooperativeLevelProc)(
            g_pDirectSoundDevice,
            *(HWND *)((char *)pBackend + 0x10), 2);
    if (hResult != 0) {
        pszError = LookupDirectSoundErrorString((unsigned int)hResult & 0xfff);
        pStream = AppendCStringToStream(
            g_pErrorOutputStream,
            "Effect Buffer Set Cooperative Level failed: ");
        pStream = AppendCStringToStream(pStream, pszError);
        AppendCStringToStream(pStream, "\n");
        *(int *)((char *)pBackend + 0x20) = 0;
        return 0;
    }

    hResult = VsSoundDIRECTSOUND_METHOD(
        g_pDirectSoundDevice, 3,
        VsSoundDirectSoundCreateBufferProc)(
            g_pDirectSoundDevice, &BufferDesc,
            &g_pDirectSoundPrimaryBuffer, 0);
    if (hResult != 0) {
        pszError = LookupDirectSoundErrorString((unsigned int)hResult & 0xfff);
        pStream = AppendCStringToStream(
            g_pErrorOutputStream, "Primary Sound Buffer failed: ");
        pStream = AppendCStringToStream(pStream, pszError);
        AppendCStringToStream(pStream, "\n");
        *(int *)((char *)pBackend + 0x20) = 0;
        return 0;
    }

    pWaveFormat = (VsSoundWaveFormat *)((char *)pBackend + 0x3c);
    hResult = VsSoundDIRECTSOUND_METHOD(
        g_pDirectSoundPrimaryBuffer, 14,
        VsSoundDirectSoundSetFormatProc)(
            g_pDirectSoundPrimaryBuffer, pWaveFormat);
    if (hResult != 0) {
        pszError = LookupDirectSoundErrorString((unsigned int)hResult & 0xfff);
        AppendStartupCString("Primary Buffer Set Format failed: ");
        AppendStartupCString(pszError);
        AppendStartupCString("\n");
        AppendStartupCString("Trying 22Khz 8 bit...\n");
        pWaveFormat->wBitsPerSample = 8;
        hResult = VsSoundDIRECTSOUND_METHOD(
            g_pDirectSoundPrimaryBuffer, 14,
            VsSoundDirectSoundSetFormatProc)(
                g_pDirectSoundPrimaryBuffer, pWaveFormat);
        if (hResult != 0) {
            pszError = LookupDirectSoundErrorString(
                (unsigned int)hResult & 0xfff);
            AppendStartupCString("Primary Buffer Set Format failed: ");
            AppendStartupCString(pszError);
            AppendStartupCString("\n");
            AppendStartupCString("Trying 11khz 16 bit...\n");
            pWaveFormat->nSamplesPerSec = 11025;
            pWaveFormat->wBitsPerSample = 16;
            hResult = VsSoundDIRECTSOUND_METHOD(
                g_pDirectSoundPrimaryBuffer, 14,
                VsSoundDirectSoundSetFormatProc)(
                    g_pDirectSoundPrimaryBuffer, pWaveFormat);
            if (hResult != 0) {
                pszError = LookupDirectSoundErrorString(
                    (unsigned int)hResult & 0xfff);
                AppendStartupCString("Primary Buffer Set Format failed: ");
                AppendStartupCString(pszError);
                AppendStartupCString("\n");
                AppendStartupCString("Trying 11khz 8 bit...\n");
                pWaveFormat->wBitsPerSample = 8;
                hResult = VsSoundDIRECTSOUND_METHOD(
                    g_pDirectSoundPrimaryBuffer, 14,
                    VsSoundDirectSoundSetFormatProc)(
                        g_pDirectSoundPrimaryBuffer, pWaveFormat);
                if (hResult != 0) {
                    pszError = LookupDirectSoundErrorString(
                        (unsigned int)hResult & 0xfff);
                    pStream = AppendCStringToStream(
                        g_pErrorOutputStream,
                        "Primary Buffer Set Format failed: ");
                    pStream = AppendCStringToStream(pStream, pszError);
                    AppendCStringToStream(pStream, "\n");
                    AppendCStringToStream(
                        g_pErrorOutputStream,
                        "Exausted iterations - cannot play effects.\n");
                    return 0;
                }
            }
        }
    }

    hResult = VsSoundDIRECTSOUND_METHOD(
        g_pDirectSoundPrimaryBuffer, 12,
        VsSoundDirectSoundPlayProc)(
            g_pDirectSoundPrimaryBuffer, 0, 0, 1);
    if (hResult != 0) {
        pszError = LookupDirectSoundErrorString((unsigned int)hResult & 0xfff);
        pStream = AppendCStringToStream(
            g_pErrorOutputStream, "Primary Sound Buffer play failed: ");
        pStream = AppendCStringToStream(pStream, pszError);
        AppendCStringToStream(pStream, "\n");
        *(int *)((char *)pBackend + 0x20) = 0;
        return 0;
    }

    *(int *)((char *)pBackend + 0x20) = 1;
    *(int *)((char *)pBackend + 0x0c) = 1;
    return 1;
}

// FUNCTION: LEMBALL 0x0047E350
int LEMBALL_FASTCALL AcceptDirectSoundBackendResources(
    void *, int, int, int, void *) {
    return 1;
}

// FUNCTION: LEMBALL 0x0047E360
int LEMBALL_FASTCALL ReturnDirectSoundBackendUnavailable(void *, int) {
    return 0;
}

// FUNCTION: LEMBALL 0x0047E370
int LEMBALL_FASTCALL GetDirectSoundBackendBufferCount(void *pBackend, int) {
    return *(int *)((char *)pBackend + 0x18);
}

// FUNCTION: LEMBALL 0x0047E4E0
int LEMBALL_FASTCALL GetDirectSoundBackendReservedValue1c(void *pBackend, int) {
    return *(int *)((char *)pBackend + 0x1c);
}

// FUNCTION: LEMBALL 0x0047E4F0
int LEMBALL_FASTCALL GetDirectSoundBackendAvailability(void *pBackend, int) {
    return *(int *)((char *)pBackend + 0x20);
}

// FUNCTION: LEMBALL 0x0047E8B0
void LEMBALL_FASTCALL CaptureDirectSoundBackendWindow(
    void *pBackend, int, void *pPrimaryContext) {
    *(HWND *)((char *)pBackend + 0x10) =
        *(HWND *)((char *)pPrimaryContext + 0x44);
}

// FUNCTION: LEMBALL 0x0047DFC0
void DestroyDirectSoundEffectBackend(void *pBackend) {
    *(void ***)pBackend = g_DirectSoundEffectBackendVtable;
    FreeVSMemBlock(*(void **)((char *)pBackend + 0x50));
    if (*(HMODULE *)((char *)pBackend + 4) != 0) {
        FreeLibrary(*(HMODULE *)((char *)pBackend + 4));
        *(HMODULE *)((char *)pBackend + 4) = 0;
    }
    RestoreAudioEffectBackendBaseVtable(pBackend);
}

// FUNCTION: LEMBALL 0x0047E8E0
void *LEMBALL_FASTCALL DeleteDirectSoundEffectBackend(
    VsSoundDirectSoundEffectBackend *pBackend, int nUnused, int fDelete) {
    (void)nUnused;
    DestroyDirectSoundEffectBackend(pBackend);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pBackend);
    }
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047D310
VsSoundDirectSoundEffectBufferPool *LEMBALL_FASTCALL
ConstructDirectSoundEffectBufferPool(
    VsSoundDirectSoundEffectBufferPool *pPool, int,
    int cBuffers, void *pPatchResource, int nOutputRate,
    int fSourceIs16Bit, int, int nFlags) {
    unsigned char PatchHeader[0x1c];
    unsigned char WaveHeader[0x38];
    VsSoundWaveFormat WaveFormat;
    VsSoundDirectSoundBufferDesc BufferDesc;
    unsigned int cbBuffer;
    int fDownsample;
    unsigned char *pSampleData;
    void *apvAudio[2];
    DWORD acbAudio[2];
    LONG hResult;
    int i;
    int j;
    unsigned char *pSource;
    unsigned char *pDestination;
    const char *pszError;
    VsInitFormattedOutputStream *pStream;

    pPool->m_nCurrentBuffer = 0;
    pPool->m_nFlags = nFlags;
    pPool->m_cBuffers = cBuffers;
    pPool->m_ppBuffers =
        (void **)AllocateVSMemBlock((unsigned int)cBuffers * sizeof(void *));
    for (i = 0; i < cBuffers; ++i)
        pPool->m_ppBuffers[i] = 0;

    memcpy(PatchHeader, pPatchResource, sizeof(PatchHeader));
    *(unsigned short *)(PatchHeader + 4) =
        ByteSwapAudioWord(*(unsigned short *)(PatchHeader + 4));
    *(unsigned short *)(PatchHeader + 0x18) =
        ByteSwapAudioWord(*(unsigned short *)(PatchHeader + 0x18));
    pPool->m_fReady = 0;
    pPool->m_nCurrentBuffer = 0;
    if (*(unsigned short *)(PatchHeader + 0x18) != 1) {
        pStream = AppendCStringToStream(
            g_pErrorOutputStream, "Warning: Effect Patch ");
        pStream = AppendCStringToStream(
            pStream, (const char *)pPatchResource + 6);
        pStream = AppendCStringToStream(
            pStream, " has more than ");
        AppendCStringToStream(
            g_pErrorOutputStream,
            "one Wave. Only one is supported.\n");
    }

    memcpy(WaveHeader, (char *)pPatchResource + 0x1c,
           sizeof(WaveHeader));
    *(unsigned short *)(WaveHeader + 4) =
        ByteSwapAudioWord(*(unsigned short *)(WaveHeader + 4));
    *(unsigned int *)(WaveHeader + 0x08) =
        ByteSwapAudioDword(*(unsigned int *)(WaveHeader + 0x08));
    *(unsigned int *)(WaveHeader + 0x14) =
        ByteSwapAudioDword(*(unsigned int *)(WaveHeader + 0x14));

    cbBuffer = *(unsigned int *)(WaveHeader + 8);
    if (fSourceIs16Bit == 0)
        cbBuffer >>= 1;
    fDownsample = 0;
    if (*(unsigned int *)(WaveHeader + 0x14) !=
        (unsigned int)nOutputRate) {
        cbBuffer >>= 1;
        fDownsample = 1;
    }

    WaveFormat.wFormatTag = 1;
    WaveFormat.nChannels = 1;
    WaveFormat.nSamplesPerSec = nOutputRate;
    WaveFormat.nAvgBytesPerSec = nOutputRate * 2;
    WaveFormat.nBlockAlign = 2;
    WaveFormat.wBitsPerSample = 16;
    WaveFormat.cbSize = 0;
    memset(&BufferDesc, 0, sizeof(BufferDesc));
    BufferDesc.dwSize = sizeof(BufferDesc);
    BufferDesc.dwFlags = 2;
    BufferDesc.dwBufferBytes = cbBuffer;
    BufferDesc.pWaveFormat = &WaveFormat;
    if ((nFlags & 1) != 0)
        BufferDesc.dwFlags = 0x82;
    if ((nFlags & 2) != 0)
        BufferDesc.dwFlags |= 0x40;
    if ((nFlags & 4) != 0)
        BufferDesc.dwFlags |= 0x20;

    hResult = VsSoundDIRECTSOUND_METHOD(
        g_pDirectSoundDevice, 3,
        VsSoundDirectSoundCreateBufferProc)(
            g_pDirectSoundDevice, &BufferDesc,
            &pPool->m_ppBuffers[0], 0);
    if (hResult != 0) {
        pszError = LookupDirectSoundErrorString((unsigned int)hResult & 0xfff);
        pStream = AppendCStringToStream(
            g_pErrorOutputStream, "Effect Buffer Creation failed: ");
        pStream = AppendCStringToStream(pStream, pszError);
        AppendCStringToStream(pStream, "\n");
        return pPool;
    }

    hResult = VsSoundDIRECTSOUND_METHOD(
        pPool->m_ppBuffers[0], 11,
        VsSoundDirectSoundLockProc)(
            pPool->m_ppBuffers[0], 0, cbBuffer,
            &apvAudio[0], &acbAudio[0],
            &apvAudio[1], &acbAudio[1], 0);
    if (hResult != 0) {
        pszError = LookupDirectSoundErrorString((unsigned int)hResult & 0xfff);
        pStream = AppendCStringToStream(
            g_pErrorOutputStream, "Effect Buffer Lock failed: ");
        pStream = AppendCStringToStream(pStream, pszError);
        AppendCStringToStream(pStream, "\n");
        return pPool;
    }

    pSampleData = (unsigned char *)pPatchResource + 0x54;
    pSource = pSampleData;
    for (i = 0; i < 2; ++i) {
        if (acbAudio[i] != 0) {
            pDestination = (unsigned char *)apvAudio[i];
            if (fDownsample == 0) {
                for (j = (int)(acbAudio[i] >> 1); j != 0; --j) {
                    *pDestination++ = pSource[1];
                    *pDestination++ = (unsigned char)(pSource[0] ^ 0x80);
                    pSource += 2;
                }
            } else {
                for (j = (int)(acbAudio[i] >> 2); j != 0; --j) {
                    *pDestination++ = pSource[1];
                    *pDestination++ = (unsigned char)(pSource[0] ^ 0x80);
                    pSource += 4;
                }
            }
        }
    }

    hResult = VsSoundDIRECTSOUND_METHOD(
        pPool->m_ppBuffers[0], 19,
        VsSoundDirectSoundUnlockProc)(
            pPool->m_ppBuffers[0], apvAudio[0], acbAudio[0],
            apvAudio[1], acbAudio[1]);
    if (hResult != 0) {
        pszError = LookupDirectSoundErrorString((unsigned int)hResult & 0xfff);
        pStream = AppendCStringToStream(
            g_pErrorOutputStream, "Effect Buffer Unlock failed: ");
        pStream = AppendCStringToStream(pStream, pszError);
        AppendCStringToStream(pStream, "\n");
        return pPool;
    }

    for (i = 1; i < cBuffers; ++i) {
        hResult = VsSoundDIRECTSOUND_METHOD(
            g_pDirectSoundDevice, 5,
            VsSoundDirectSoundDuplicateBufferProc)(
                g_pDirectSoundDevice, pPool->m_ppBuffers[0],
                &pPool->m_ppBuffers[i]);
        if (hResult != 0) {
            pszError = LookupDirectSoundErrorString(
                (unsigned int)hResult & 0xfff);
            pStream = AppendCStringToStream(
                g_pErrorOutputStream,
                "Effect Buffer Duplication failed: ");
            pStream = AppendCStringToStream(pStream, pszError);
            AppendCStringToStream(pStream, "\n");
            return pPool;
        }
    }
    pPool->m_fReady = 1;
    return pPool;
}

// FUNCTION: LEMBALL 0x0047E520
int LEMBALL_FASTCALL CreateDirectSoundEffectInstance(
    void *pBackend, int, void *pPatchResource, int *pnEffectInstanceId,
    int nFlags) {
    int nEffectInstanceId;
    void *pBufferPool;

    nEffectInstanceId = 1;
    if (0 < *(int *)((char *)pBackend + 0x14)) {
        do {
            if (*(int *)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4) == 0) {
                pBufferPool = AllocateVSMemBlock(0x1c);
                if (pBufferPool == 0) {
                    *(void **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4) = 0;
                } else {
                    pBufferPool = ConstructDirectSoundEffectBufferPool(
                        (VsSoundDirectSoundEffectBufferPool *)pBufferPool, 0,
                        *(int *)((char *)pBackend + 0x18), pPatchResource,
                        *(int *)((char *)pBackend + 0x34),
                        *(int *)((char *)pBackend + 0x28),
                        *(int *)((char *)pBackend + 0x24), nFlags);
                    *(void **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4) = pBufferPool;
                }
                if (**(int **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4) != 0) {
                    *pnEffectInstanceId = nEffectInstanceId;
                }
                return 1;
            }
            ++nEffectInstanceId;
        } while (nEffectInstanceId <= *(int *)((char *)pBackend + 0x14));
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047E5E0
int FreeDirectSoundEffectInstance(void *pBackend, int nEffectInstanceId) {
    unsigned int pBufferPool;

    pBufferPool = *(unsigned int *)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4);
    if (pBufferPool != 0) {
        g_AUDIO_DestroyBufferProc((int)pBufferPool);
        FreeVSMemBlock((void *)(unsigned long)pBufferPool);
        *(int *)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4) = 0;
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047E620
int ReleaseAllDirectSoundEffectInstances(void *pBackend) {
    int nEffectInstanceId;
    int nOffset;
    unsigned int pBufferPool;

    nEffectInstanceId = 1;
    if (0 < *(int *)((char *)pBackend + 0x14)) {
        nOffset = 4;
        do {
            pBufferPool = *(unsigned int *)(*(int *)((char *)pBackend + 0x50) + nOffset);
            if (pBufferPool != 0) {
                g_AUDIO_DestroyBufferProc((int)pBufferPool);
                FreeVSMemBlock((void *)(unsigned long)pBufferPool);
                *(int *)(*(int *)((char *)pBackend + 0x50) + nOffset) = 0;
            }
            nOffset += 4;
            ++nEffectInstanceId;
        } while (nEffectInstanceId <= *(int *)((char *)pBackend + 0x14));
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0047E7E0
void PlayDirectSoundEffectInstanceWithMappedVolume(void *pBackend, int nEffectInstanceId, unsigned char nVolume,
                                                   int fLoopEnabled) {
    g_AUDIO_PlayBufferProc(
        *(void **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4),
        ((unsigned int)nVolume * 10000) / 0xff - 10000, fLoopEnabled);
}

// FUNCTION: LEMBALL 0x0047E820
void PauseDirectSoundEffectInstance(void *pBackend, int nEffectInstanceId, int nBufferSlot) {
    g_AUDIO_PauseBufferProc(*(void **)(*(int *)((char *)pBackend + 0x50) + nEffectInstanceId * 4),
                                        nBufferSlot);
}

// FUNCTION: LEMBALL 0x0047E840
void VsSoundDirectSoundEffectBackend::PlayDirectSoundEffectInstance(int nEffectInstanceId,
                                                                   int nBufferSlot) {
    g_AUDIO_PlayBufferSlotProc(*(void **)(*(int *)((char *)this + 0x50) + nEffectInstanceId * 4),
                                        nBufferSlot);
}

// FUNCTION: LEMBALL 0x0047E8C0
int LEMBALL_FASTCALL CreateDirectSoundEffectInstanceWithDefaultFlags(
    void *pBackend, int, void *pPatchResource, int *pnEffectInstanceId) {
    return CreateDirectSoundEffectInstance(
        pBackend, 0, pPatchResource, pnEffectInstanceId, 1);
}

// FUNCTION: LEMBALL 0x0047E940
VsSoundMciMusicBackend *VsSoundMciMusicBackend::ConstructMciMusicBackend(void) {
    VsSoundMciMusicBackend *pMusicBackend;
    VsSoundMciOpenParms OpenParms;
    WNDCLASSA WindowClass;
    MCIERROR nMciError;
    char szMciErrorText[0x80];

    pMusicBackend = this;
    pMusicBackend->m_BasePath.ConstructDynamicCString();
    pMusicBackend->m_pVtable = (void **)g_AudioDynamicStringEntryVtable;
    pMusicBackend->m_fSearchCdromPath = 0;
    pMusicBackend->m_fBasePathSet = 0;
    pMusicBackend->m_pVtable = *(void ***)&g_AUDIO_MciMusicBackendVtableModel;
    pMusicBackend->m_nPreparedTrackHandle = 0;
    pMusicBackend->m_fDeviceAvailable = 0;
    pMusicBackend->m_uDeviceId = 0;
    pMusicBackend->m_fTrackPlaying = 0;
    pMusicBackend->m_fTrackPaused = 0;
    pMusicBackend->m_nPausedTrackPosition = 0;
    pMusicBackend->m_hNotificationWindow = 0;
    g_nPreparedMciMusicTrackHandle = 0;
    g_pActiveMciMusicBackend = pMusicBackend;

    OpenParms.dwCallback = 0;
    OpenParms.wDeviceID = 0;
    OpenParms.lpstrDeviceType = g_AUDIO_SequencerDeviceType;
    OpenParms.lpstrElementName = 0;
    OpenParms.lpstrAlias = 0;
    nMciError = mciSendCommandA(0, 0x803, 0x2000, (DWORD_PTR)&OpenParms);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        g_pErrorOutputStream->AppendCStringToStream(
            "Error!     HL Midi Device Not Found.\n");
        g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t")
            ->AppendCStringToStream(szMciErrorText)
            ->AppendCStringToStream("\n");
        pMusicBackend->m_fDeviceAvailable = 0;
    } else {
        pMusicBackend->m_uDeviceId = OpenParms.wDeviceID;
        pMusicBackend->m_fDeviceAvailable = 1;
        mciSendCommandA(OpenParms.wDeviceID, 0x804, 0, 0);

        memset(&WindowClass, 0, sizeof(WindowClass));
        WindowClass.style = 3;
        WindowClass.lpfnWndProc = MciMusicWindowProc;
        WindowClass.hInstance = g_hApplicationInstance;
        WindowClass.lpszClassName = g_AUDIO_MciWindowClassName;
        RegisterClassA(&WindowClass);
        pMusicBackend->m_hNotificationWindow =
            CreateWindowExA(0,
                            g_AUDIO_MciWindowClassName,
                            g_AUDIO_MciWindowClassName,
                            0,
                            (INT)CW_USEDEFAULT,
                            (INT)CW_USEDEFAULT,
                            (INT)CW_USEDEFAULT,
                            (INT)CW_USEDEFAULT,
                            0,
                            0,
                            g_hApplicationInstance,
                            0);
        if (pMusicBackend->m_hNotificationWindow == 0) {
            g_pErrorOutputStream->AppendCStringToStream(
                "Error! Unable to Create Window for HL Music.\n");
        }
    }
    return this;
}

// FUNCTION: LEMBALL 0x0047EAD0
void PrepareMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle, int nMusicResourceId) {
    VsSoundStringResourceObjectView *pStringResource;
    VsSoundMciOpenParms OpenParms;
    VsSoundMciGenericParms SeekParms;
    VsSoundMciSetParms SetParms;
    GameDynamicCString MusicPath;
    GameDynamicCString BasePath;
    GameDynamicCString TemporaryPath;
    MCIERROR nMciError;
    char szMciErrorText[0x80];

    if (nTrackHandle == 0) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error Call to Prepare Music (HL) with Invalid Handle!\n");
    }
    if (pBackend->m_nPreparedTrackHandle != 0) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error! Call to Prepare Music when already prepared!\n");
    }
    if (pBackend->m_fTrackPlaying == 1) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error! Cannot Prepare Music while playing.\n");
    }
    pBackend->m_nPreparedTrackHandle = nTrackHandle;
    g_nPreparedMciMusicTrackHandle = nTrackHandle;

    pStringResource =
        (VsSoundStringResourceObjectView *)LoadStringResource(nMusicResourceId);
    if (pStringResource->m_nLoadState10 != 0) {
        pStringResource->m_nReserved24 = 0;
    } else {
        pStringResource->PrepareStringResource();
    }
    ++pStringResource->m_nLockCount08;

    MusicPath.ConstructDynamicCString();
    if (pBackend->m_fBasePathSet != 0) {
        MusicPath.AssignDynamicCStringFromDynamic(&pBackend->m_BasePath);
        if (MusicPath.m_pszText[GetDynamicCStringLength(&MusicPath) - 1] != '\\') {
            MusicPath.AppendDynamicCStringAndCopyResult(&TemporaryPath, g_AUDIO_PathSeparator);
            DestroyDynamicCString(&TemporaryPath);
        }
    }
    MusicPath.AppendDynamicCStringAndCopyResult(&TemporaryPath, pStringResource->m_pszText38);
    DestroyDynamicCString(&TemporaryPath);
    MusicPath.AppendDynamicCStringAndCopyResult(&TemporaryPath, g_AUDIO_MusicExtension);
    DestroyDynamicCString(&TemporaryPath);

    BasePath.ConstructDynamicCString();
    if (pBackend->m_fSearchCdromPath == 0) {
        BasePath.AssignDynamicCString(g_abProcessCurrentDirectoryBuffer);
        if (BasePath.m_pszText[GetDynamicCStringLength(&BasePath) - 1] != '\\') {
            BasePath.AppendDynamicCStringAndCopyResult(&TemporaryPath, g_AUDIO_PathSeparator);
            DestroyDynamicCString(&TemporaryPath);
        }
    } else {
        const char *pszCdromPath;

        pszCdromPath = FindCdromFilePathBySuffix(MusicPath.m_pszText);
        if (pszCdromPath == 0) {
            pszCdromPath = g_abProcessCurrentDirectoryBuffer;
        }
        BasePath.AssignDynamicCString(pszCdromPath);
        if (BasePath.m_pszText[GetDynamicCStringLength(&BasePath) - 1] != '\\') {
            BasePath.AppendDynamicCStringAndCopyResult(&TemporaryPath, g_AUDIO_PathSeparator);
            DestroyDynamicCString(&TemporaryPath);
        }
    }
    BasePath.AppendDynamicCStringObjectAndCopyResult(&TemporaryPath, &MusicPath);
    DestroyDynamicCString(&TemporaryPath);

    OpenParms.dwCallback = 0;
    OpenParms.wDeviceID = 0;
    OpenParms.lpstrDeviceType = (LPCSTR)0x20b;
    OpenParms.lpstrElementName = BasePath.m_pszText;
    OpenParms.lpstrAlias = 0;
    nMciError = mciSendCommandA(0, 0x803, 0x3200, (DWORD_PTR)&OpenParms);
    --pStringResource->m_nLockCount08;
    ReleaseTypedResourceObjectReference(pStringResource);
    if (nMciError == 0) {
        pBackend->m_uDeviceId = OpenParms.wDeviceID;
        SeekParms.dwCallback = 0;
        nMciError = mciSendCommandA(OpenParms.wDeviceID, 0x807, 0x100, (DWORD_PTR)&SeekParms);
        if (nMciError == 0) {
            SetParms.dwCallback = 0;
            SetParms.dwTimeFormat = 0;
            nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x80d, 0x400, (DWORD_PTR)&SetParms);
            if (nMciError == 0) {
                pBackend->m_fTrackPlaying = 0;
                pBackend->m_fTrackPaused = 0;
            } else {
                mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
                g_pErrorOutputStream->AppendCStringToStream(
                    "Error!     Unable to Prepare Music! (Time)\n");
                g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t");
                g_pErrorOutputStream->AppendCStringToStream(szMciErrorText);
                g_pErrorOutputStream->AppendCStringToStream("\n");
                pBackend->m_nPreparedTrackHandle = 0;
                g_nPreparedMciMusicTrackHandle = 0;
            }
        } else {
            mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
            g_pErrorOutputStream->AppendCStringToStream(
                "Error!     Unable to Prepare Music (Seek)!\n");
            g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t");
            g_pErrorOutputStream->AppendCStringToStream(szMciErrorText);
            g_pErrorOutputStream->AppendCStringToStream("\n");
            pBackend->m_nPreparedTrackHandle = 0;
            g_nPreparedMciMusicTrackHandle = 0;
        }
    } else {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        g_pErrorOutputStream->AppendCStringToStream(
            "Error!     Unable to Prepare Music (Open) ");
        AppendDynamicCStringToStream(g_pErrorOutputStream, &BasePath);
        g_pErrorOutputStream->AppendCStringToStream("!\n");
        g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t");
        g_pErrorOutputStream->AppendCStringToStream(szMciErrorText);
        g_pErrorOutputStream->AppendCStringToStream("\n");
        pBackend->m_nPreparedTrackHandle = 0;
        g_nPreparedMciMusicTrackHandle = 0;
    }

    DestroyDynamicCString(&BasePath);
    DestroyDynamicCString(&MusicPath);
}

// FUNCTION: LEMBALL 0x0047EE70
void FreePreparedMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle) {
    if (nTrackHandle == 0) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error Call to Free Music (HL) with Invalid Handle!\n");
    }
    if (pBackend->m_nPreparedTrackHandle != nTrackHandle) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error Call to Free Music (HL) with unknown Handle!\n");
    }
    pBackend->m_nPreparedTrackHandle = 0;
    g_nPreparedMciMusicTrackHandle = 0;
    if (pBackend->m_fTrackPlaying == 1) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error! Must stop music before closing...\n");
    }
    mciSendCommandA(pBackend->m_uDeviceId, 0x804, 0, 0);
}

// FUNCTION: LEMBALL 0x0047EEE0
void PlayPreparedMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle) {
    VsSoundMciSeekParms SeekParms;
    VsSoundMciPlayParms PlayParms;
    MCIERROR nMciError;
    char szMciErrorText[0x80];

    if (nTrackHandle == 0) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error Call to Play Music (HL) with Invalid Handle!\n");
    }
    if (pBackend->m_nPreparedTrackHandle != nTrackHandle) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error Call to Play (HL) with unknown Handle!\n");
    }
    if (pBackend->m_fTrackPlaying == 1) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error! Play Command (HL) While already playing!\n");
    }

    SeekParms.dwTo = 0;
    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x807, 0x100, (DWORD_PTR)&SeekParms);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        g_pErrorOutputStream->AppendCStringToStream(
            "Error!     Unable to Play Music (Seek)! (HL)\n");
        g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t")
            ->AppendCStringToStream(szMciErrorText)
            ->AppendCStringToStream("\n");
        return;
    }

    PlayParms.dwCallback = (DWORD)(DWORD_PTR)pBackend->m_hNotificationWindow;
    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x806, 1, (DWORD_PTR)&PlayParms);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        g_pErrorOutputStream->AppendCStringToStream(
            "Error!     Unable to Play Music (Play)! (HL)\n");
        g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t")
            ->AppendCStringToStream(szMciErrorText)
            ->AppendCStringToStream("\n");
        return;
    }

    pBackend->m_fTrackPaused = 0;
    pBackend->m_nPausedTrackPosition = 0;
    pBackend->m_fTrackPlaying = 1;
}

// FUNCTION: LEMBALL 0x0047F040
void StopMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle) {
    MCIERROR nMciError;
    char szMciErrorText[0x80];

    if (nTrackHandle == 0) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error Call to Stop Music (HL) with Invalid Handle!\n");
    }
    if (pBackend->m_nPreparedTrackHandle != nTrackHandle) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error Call to Stop (HL) with unknown Handle!\n");
    }
    if (pBackend->m_fTrackPlaying == 0) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error! Stop Command (HL) when not playing!\n");
        return;
    }

    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x808, 0, 0);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        g_pErrorOutputStream->AppendCStringToStream(
            "Error!     Unable to Stop Music! (HL)\n");
        g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t")
            ->AppendCStringToStream(szMciErrorText)
            ->AppendCStringToStream("\n");
        return;
    }

    pBackend->m_fTrackPlaying = 0;
    (void)nTrackHandle;
}

// FUNCTION: LEMBALL 0x0047F120
void PauseMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle) {
    VsSoundMciStatusParms StatusParms;
    MCIERROR nMciError;
    char szMciErrorText[0x80];

    if (nTrackHandle == 0) {
        AppendCStringToStream(g_pErrorOutputStream,
                              "Error Call to Pause Music (HL) with Invalid Handle!\n");
    }
    if (pBackend->m_nPreparedTrackHandle != nTrackHandle) {
        AppendCStringToStream(g_pErrorOutputStream,
                              "Error Call to Pause (HL) with unknown Handle!\n");
    }

    StatusParms.dwItem = 2;
    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x814, 0x100, (DWORD_PTR)&StatusParms);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        g_pErrorOutputStream->AppendCStringToStream(
            "Error!     Unable to Get Position for Pause! (HL)\n");
        g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t")
            ->AppendCStringToStream(szMciErrorText)
            ->AppendCStringToStream("\n");
    }
    pBackend->m_nPausedTrackPosition = (int)StatusParms.dwReturn;

    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x808, 0, 0);
    if (nMciError == 0) {
        pBackend->m_fTrackPlaying = 0;
        pBackend->m_fTrackPaused = 1;
    } else {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        g_pErrorOutputStream->AppendCStringToStream(
            "Error!     Unable to Stop Music! (HL)\n");
        g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t")
            ->AppendCStringToStream(szMciErrorText)
            ->AppendCStringToStream("\n");
    }
}

// FUNCTION: LEMBALL 0x0047F250
void ResumeMciMusicTrack(VsSoundMciMusicBackend *pBackend, int nTrackHandle) {
    VsSoundMciSeekParms SeekParms;
    VsSoundMciPlayParms PlayParms;
    MCIERROR nMciError;
    char szMciErrorText[0x80];

    if (nTrackHandle == 0) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error Call to Resume Music (HL) with Invalid Handle!\n");
    }
    if (pBackend->m_nPreparedTrackHandle != nTrackHandle) {
        g_pErrorOutputStream->AppendCStringToStream(
            "Error Call to Resume (HL) with unknown Handle!\n");
    }

    SeekParms.dwTo = 0;
    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x807, 0x100,
                                (DWORD_PTR)&SeekParms);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        g_pErrorOutputStream->AppendCStringToStream(
            "Error!     Unable to Restart Music (Seek)! (HL)\n");
        g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t")
            ->AppendCStringToStream(szMciErrorText)
            ->AppendCStringToStream("\n");
        return;
    }

    PlayParms.dwCallback = (DWORD)(DWORD_PTR)pBackend->m_hNotificationWindow;
    nMciError = mciSendCommandA(pBackend->m_uDeviceId, 0x806, 1,
                                (DWORD_PTR)&PlayParms);
    if (nMciError != 0) {
        mciGetErrorStringA(nMciError, szMciErrorText, sizeof(szMciErrorText));
        g_pErrorOutputStream->AppendCStringToStream(
            "Error!     Unable to Restart Music (Play)! (HL)\n");
        g_pErrorOutputStream->AppendCStringToStream("MCI Error:\t")
            ->AppendCStringToStream(szMciErrorText)
            ->AppendCStringToStream("\n");
        return;
    }

    pBackend->m_fTrackPlaying = 1;
    pBackend->m_fTrackPaused = 0;
}

// FUNCTION: LEMBALL 0x0047F940
void *LEMBALL_FASTCALL ConstructAudioEffectBackendBase(void *pBackend) {
    *(void ***)pBackend = g_WaveOutEffectBackendVtable;
    return pBackend;
}

// FUNCTION: LEMBALL 0x0047F950
void LEMBALL_FASTCALL RestoreAudioEffectBackendBaseVtable(void *pBackend) {
    *(void ***)pBackend = g_WaveOutEffectBackendVtable;
}

// FUNCTION: LEMBALL 0x00473390
int SelectAudioManagerBackends(void **ppEffectBackends,
                               int nMusicResourceBase,
                               int cMusicResources,
                               int fMusicBackendEnabled,
                               int *pfMusicBackendReady,
                               void **ppMusicBackend,
                               int cEffectResources) {
    void *pBackendStorage;
    void *pBackend;
    void **pBackendVtable;
    int fAvailable;

    (void)nMusicResourceBase;
    *pfMusicBackendReady = 0;
    *ppMusicBackend = 0;
    if (fMusicBackendEnabled != 1) {
        if (cMusicResources == 1) {
            pBackend = 0;
            pBackendStorage = AllocateVSMemBlock(0xb8);
            if (pBackendStorage != 0) {
                pBackend =
                    ((VsSoundWaveOutEffectBackend *)pBackendStorage)
                        ->ConstructWaveOutEffectBackend(cEffectResources);
            }
            if (pBackend != 0) {
                pBackendVtable = *(void ***)pBackend;
                fAvailable = ((VsSoundTestProc)pBackendVtable[10])(
                    pBackend, 0);
                if (fAvailable == 1) {
                    *ppEffectBackends = pBackend;
                    return 1;
                }
                ((VsSoundDeleteProc)pBackendVtable[0])(pBackend, 0, 1);
            }
        }
        return 0;
    }

    pBackend = 0;
    pBackendStorage = AllocateVSMemBlock(0x34);
    if (pBackendStorage != 0) {
        pBackend =
            ((VsSoundMciMusicBackend *)pBackendStorage)->ConstructMciMusicBackend();
    }
    if (pBackend != 0) {
        pBackendVtable = *(void ***)pBackend;
        fAvailable = ((VsSoundMciMusicBackendVtableModel *)pBackend)->IsAvailable();
        if (fAvailable == 1) {
            *pfMusicBackendReady = 1;
            *ppMusicBackend = pBackend;
        } else {
            ((VsSoundDeleteProc)pBackendVtable[0])(pBackend, 0, 1);
        }
    }

    if (cMusicResources != 1) {
        return 0;
    }

    pBackend = 0;
    pBackendStorage = AllocateVSMemBlock(0x54);
    if (pBackendStorage != 0) {
        pBackend =
            ((VsSoundDirectSoundEffectBackend *)pBackendStorage)
                ->ConstructDirectSoundEffectBackend(cEffectResources, 5);
    }
    if (pBackend != 0) {
        pBackendVtable = *(void ***)pBackend;
        fAvailable = ((VsSoundTestProc)pBackendVtable[10])(
            pBackend, 0);
        if (fAvailable == 1) {
            *ppEffectBackends = pBackend;
            return 1;
        }
        ((VsSoundDeleteProc)pBackendVtable[0])(pBackend, 0, 1);
    }

    pBackend = 0;
    pBackendStorage = AllocateVSMemBlock(0xb8);
    if (pBackendStorage != 0) {
        pBackend =
            ((VsSoundWaveOutEffectBackend *)pBackendStorage)
                ->ConstructWaveOutEffectBackend(cEffectResources);
    }
    if (pBackend != 0) {
        pBackendVtable = *(void ***)pBackend;
        fAvailable = ((VsSoundTestProc)pBackendVtable[10])(
            pBackend, 0);
        if (fAvailable == 1) {
            *ppEffectBackends = pBackend;
            return 1;
        }
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0047EAC0
void DestroyAudioDynamicStringEntry(VsSoundDynamicStringEntry *pEntry) {
    pEntry->m_pVtable = (void **)g_AudioDynamicStringEntryVtable;
    DestroyDynamicCString(&pEntry->m_BasePath);
}

// FUNCTION: LEMBALL 0x0047F590
void *LEMBALL_FASTCALL DeleteAudioDynamicStringEntry(
    VsSoundDynamicStringEntry *pEntry, int nUnused, int fDelete) {
    (void)nUnused;
    DestroyAudioDynamicStringEntry(pEntry);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pEntry);
    }
    return pEntry;
}

const char *ReturnEmptyAudioBackendName(void) {
    return "";
}

int ReturnAudioBackendReady(void) {
    return 1;
}

int ReturnAudioBackendUnavailable(void) {
    return 0;
}

int LEMBALL_FASTCALL AcceptAudioBackendResources(void *pBackend, int nUnused,
                                                  int nResourceBase, int cResources,
                                                  void *pPrimaryContext) {
    (void)pBackend;
    (void)nUnused;
    (void)nResourceBase;
    (void)cResources;
    (void)pPrimaryContext;
    return 1;
}

int CreateFallbackEffectInstance(int nValue, void *pOut) {
    if (pOut != 0) {
        *(int *)pOut = nValue;
    }
    return nValue;
}

void NoOpAudioInt(int nValue) {
    (void)nValue;
}

void NoOpAudioPair(int nValue1, int nValue2) {
    (void)nValue1;
    (void)nValue2;
}

void NoOpAudioTriple(int nValue1, int nValue2, int nValue3) {
    (void)nValue1;
    (void)nValue2;
    (void)nValue3;
}
