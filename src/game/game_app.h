#ifndef LEMBALL_GAME_APP_H
#define LEMBALL_GAME_APP_H

#include "platform/win32.h"
#include "game/window_owner.h"

struct GAME_DynamicCString {
    char *m_pszText;
    int m_cchCapacity;

    GAME_DynamicCString *ConstructDynamicCString(void);
    GAME_DynamicCString *ConstructDynamicCStringFromCString(const char *pszText);
    GAME_DynamicCString *CopyConstructDynamicCString(const GAME_DynamicCString *pSource);
    GAME_DynamicCString *AssignDynamicCString(const char *pszText);
    GAME_DynamicCString *AssignDynamicCStringFromDynamic(const GAME_DynamicCString *pSource);
    GAME_DynamicCString *AppendDynamicCStringObjectAndCopyResult(
        GAME_DynamicCString *pResult, const GAME_DynamicCString *pSuffix);
    GAME_DynamicCString *AppendDynamicCStringAndCopyResult(
        GAME_DynamicCString *pResult, const char *pszSuffix);
};

struct VSINIT_FormattedOutputStream;

class GAME_StatusEntry {
public:
    GAME_StatusEntry(const char *pszName);

public:
    void *m_pVtable;
    int m_nReserved04;
    int m_nMaximumValue;
    int m_nMinimumValue;
    int m_nTotalValue;
    int m_cSamples;
    GAME_DynamicCString m_Name;
};

class GAME_MainContext {
public:
    GAME_MainContext(void);
    GAME_MainContext *InitializeMainGameContext(const char *pszCmdLine);
    GAME_MainContext *InitializeMainGameContextThunk(const char *pszCmdLine);
    void SwitchMainGameMode(int nMode);

public:
    char m_szLevelPath[0x50];
    GAME_StatusEntry *m_pProcessingStatus;
    GAME_StatusEntry *m_pRefreshingStatus;
    int m_nModeTicks;
    void *m_pActiveMode;
    int m_fQuitRequested;
    void *m_pPrimaryContext;
    int m_nActiveMode;
    void *m_pVariantMode;
};

struct GAME_PrimaryContext : public GAME_WindowOwnerBase {
    void *m_pWindowOwnerContext;
    int m_nFrameInterval;
    char m_Reserved54[0x3c];
    void *m_pRenderQueueNodeVtable;
    char m_Reserved94[0xc];
    int m_fA0;
    int m_nA4;
    void *m_pBackgroundZrle;
    void *m_pPalette2E;
    void *m_pPalette2F;
    int m_nLevelScreenStatusIndicatorMode;
    int m_nB8;
    int m_fScreenLayoutDirty;
    GAME_MainContext *m_pMainContext;
    void *m_pActiveLevelMode;
    char m_ReservedC8[4];
    int m_nActiveScreenMode;
    void *m_pFinalizeThunk;
    void *m_pActiveScreen;
    int m_fCompactLayout;
    short m_cxCompactLayout;
    short m_cyCompactLayout;
    short m_cxWideLayout;
    short m_cyWideLayout;

    void SampleRootHelperGeometryAndDispatchRenderGroups(int nToken);
    GAME_PrimaryContext *ConstructPrimaryContext(GAME_MainContext *pMainContext);
    GAME_PrimaryContext *ConstructPrimaryContextThunk(GAME_MainContext *pMainContext);
    void SwitchPrimaryContextScreen(int nMode);
    void DestroyPrimaryContextActiveScreen(int nUnused);
    void UpdateWindowOwnerRenderContext(void);
    void FlushWindowOwnerDirtyRect(int nUnused);
    void DispatchWindowOwnerRectInitialization(short *paRect,
                                                void *pWindowOwner,
                                                const char *pszTitle);
    void DispatchPrimaryContextActiveScreenFrame(void *pFrameArgument);
    void MarkNestedContextDirtyIfField20(void);
    void InitializeWindowOwnerFromRectAndActivate(short *paRect,
                                                   void *pWindowOwner,
                                                   const char *pszTitle,
                                                   int nActivate);
    void ApplyWindowOwnerPaletteResource(int nResourceId);
    short *ComputePrimaryContextCenteredScreenRect(short *paRect,
                                                   int nLeftOverride,
                                                   int nTopOverride);
    short *ComputePrimaryContextCenteredScreenRectThunk(short *paRect,
                                                        int nLeftOverride,
                                                        int nTopOverride);
};

void LEMBALL_FASTCALL ShutdownMainGameContext(GAME_MainContext *pMainContext);
int RunMainGameSession(int cArgs, const char *const *ppszArgs);
int RunMainGameSessionThunk(int cArgs, const char *const *ppszArgs);
char *FindCdromFilePathBySuffix(const char *pszSuffix);
void DestroyDynamicCString(GAME_DynamicCString *pString);
void *AppendDynamicCStringToStream(void *pStream, const GAME_DynamicCString *pString);
int LEMBALL_FASTCALL GetDynamicCStringLength(const GAME_DynamicCString *pString);
void DestroyNamedStatusEntry(void *pEntry);
void UpdateNamedStatusEntry(void *pEntry, unsigned int nValue);
VSINIT_FormattedOutputStream *WriteNamedStatusEntry(void *pEntry, VSINIT_FormattedOutputStream *pStream);
void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(void *pResourceObject);
void *LEMBALL_FASTCALL InitializeRenderQueueNodeBase(void *pRenderQueueNode);
struct GAME_RenderDispatchQueue {
    void *m_pVtable;
    int m_nReserved04;
    void *m_pLockVtable;
    char m_abCriticalSection[0x18];
    int m_cEntryCapacity;
    int m_cQueuedEntries;
    int m_cClients;
    int m_nReserved30;
    int m_nReserved34;
    int m_nReserved38;
    int m_nReserved3C;
    int m_cEntriesDropped;
    void *m_pEntryBuffer;
    void *m_pEntryBufferEnd;
    void *m_pHead;
    void *m_pTail;
    void *m_pClientList;

    void RegisterOrderedRenderDispatchClient(void *pClient, int nOrder);
    void UnregisterOrderedRenderDispatchClient(void *pClient, int nOrder);

    void LockDispatchQueue(void) {
        ((void (__fastcall *)(void *))(*(void ***)&m_pLockVtable)[0])(&m_pLockVtable);
    }
    void UnlockDispatchQueue(void) {
        ((void (__fastcall *)(void *))(*(void ***)&m_pLockVtable)[1])(&m_pLockVtable);
    }
};

extern void *g_pSharedRenderDispatchQueue;

#endif
