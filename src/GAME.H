#ifndef LEMBALL_GAME_APP_H
#define LEMBALL_GAME_APP_H

#include "ENGINE/CORE/WIN32.H"
#include "ENGINE/GDI/VSWINDOW.H"

struct GameDynamicCString {
    char *m_pszText;
    int m_cchCapacity;

    GameDynamicCString *ConstructDynamicCString(void);
    GameDynamicCString *ConstructDynamicCStringFromCString(const char *pszText);
    GameDynamicCString *CopyConstructDynamicCString(const GameDynamicCString *pSource);
    GameDynamicCString *AssignDynamicCString(const char *pszText);
    GameDynamicCString *AssignDynamicCStringFromDynamic(const GameDynamicCString *pSource);
    GameDynamicCString *AppendDynamicCStringObjectAndCopyResult(
        GameDynamicCString *pResult, const GameDynamicCString *pSuffix);
    GameDynamicCString *AppendDynamicCStringAndCopyResult(
        GameDynamicCString *pResult, const char *pszSuffix);
    void DestroyDynamicCString(void);
};

struct VsInitFormattedOutputStream;

class GameStatusEntry {
public:
    GameStatusEntry(const char *pszName);

public:
    void *m_pVtable;
    int m_nReserved04;
    int m_nMaximumValue;
    int m_nMinimumValue;
    int m_nTotalValue;
    int m_cSamples;
    GameDynamicCString m_Name;
};

class GameMainContext {
public:
    GameMainContext(void);
    GameMainContext *InitializeMainGameContext(const char *pszCmdLine);
    GameMainContext *InitializeMainGameContextThunk(const char *pszCmdLine);
    void SwitchMainGameMode(int nMode);

public:
    char m_szLevelPath[0x50];
    GameStatusEntry *m_pProcessingStatus;
    GameStatusEntry *m_pRefreshingStatus;
    int m_nModeTicks;
    void *m_pActiveMode;
    int m_fQuitRequested;
    void *m_pPrimaryContext;
    int m_nActiveMode;
    void *m_pVariantMode;
};

struct GamePrimaryContext : public VS_WindowOwnerBase {
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
    GameMainContext *m_pMainContext;
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
    GamePrimaryContext *ConstructPrimaryContext(GameMainContext *pMainContext);
    GamePrimaryContext *ConstructPrimaryContextThunk(GameMainContext *pMainContext);
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

void LEMBALL_FASTCALL ShutdownMainGameContext(GameMainContext *pMainContext);
int RunMainGameSession(int cArgs, const char *const *ppszArgs);
int RunMainGameSessionThunk(int cArgs, const char *const *ppszArgs);
char *FindCdromFilePathBySuffix(const char *pszSuffix);
void *AppendDynamicCStringToStream(void *pStream, const GameDynamicCString *pString);
int LEMBALL_FASTCALL GetDynamicCStringLength(const GameDynamicCString *pString);
void LEMBALL_FASTCALL DestroyNamedStatusEntry(void *pEntry);
void UpdateNamedStatusEntry(void *pEntry, unsigned int nValue);
VsInitFormattedOutputStream *LEMBALL_FASTCALL WriteNamedStatusEntry(
    void *pEntry, int nUnused, VsInitFormattedOutputStream *pStream);
void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(void *pResourceObject);
void *LEMBALL_FASTCALL InitializeRenderQueueNodeBase(void *pRenderQueueNode);
struct GameRenderDispatchQueue {
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

    int RegisterOrderedRenderDispatchClient(void *pClient, int nOrder);
    int UnregisterOrderedRenderDispatchClient(void *pClient, int nOrder);

    void LockDispatchQueue(void) {
        ((void (__fastcall *)(void *))(*(void ***)&m_pLockVtable)[0])(&m_pLockVtable);
    }
    void UnlockDispatchQueue(void) {
        ((void (__fastcall *)(void *))(*(void ***)&m_pLockVtable)[1])(&m_pLockVtable);
    }
};

extern void *g_pSharedRenderDispatchQueue;

char *FormatCurrentLevelPassword(void);
int ValidateCurrentLevelPassword(char *pszPassword);
void SnapshotCurrentLevelProgress(void);

#endif
