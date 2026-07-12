#ifndef LEMBALL_GAME_APP_H
#define LEMBALL_GAME_APP_H

#include "platform/win32.h"

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
