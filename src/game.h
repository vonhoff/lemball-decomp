#ifndef LEMBALL_GAME_H
#define LEMBALL_GAME_H

#include "win32.h"

struct GAME_DynamicCString {
    char *m_pszText;
    int m_cchCapacity;
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
    int m_nReserved20;
    int m_nReserved24;
};

class GAME_MainContext {
public:
    GAME_MainContext(void);

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

GAME_MainContext *InitializeMainGameContext(GAME_MainContext *pMainContext, const char *pszCmdLine);
void ShutdownMainGameContext(GAME_MainContext *pMainContext);
int RunMainGameSession(int cArgs, const char *const *ppszArgs);
char *FindCdromFilePathBySuffix(const char *pszSuffix);
void DestroyNamedStatusEntry(void *pEntry);
void UpdateNamedStatusEntry(void *pEntry, unsigned int nValue);
VSINIT_FormattedOutputStream *WriteNamedStatusEntry(void *pEntry, VSINIT_FormattedOutputStream *pStream);
void ReleaseTypedResourceObjectReference(void *pResourceObject);
void InitializeRenderQueueNodeBase(void *pRenderQueueNode);
void RegisterOrderedRenderDispatchClient(void *pDispatchQueue, void *pClient, int nOrder);
void UnregisterOrderedRenderDispatchClient(void *pDispatchQueue, void *pClient, int nOrder);

extern void *g_pSharedRenderDispatchQueue;

#endif
