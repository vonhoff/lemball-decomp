#ifndef LEMBALL_GAME_APP_H
#define LEMBALL_GAME_APP_H

#include "platform/win32.h"

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
};

class GAME_MainContext {
public:
    GAME_MainContext(void);
    GAME_MainContext *InitializeMainGameContext(const char *pszCmdLine);
    GAME_MainContext *InitializeMainGameContextThunk(const char *pszCmdLine);

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

void ShutdownMainGameContext(GAME_MainContext *pMainContext);
int RunMainGameSession(int cArgs, const char *const *ppszArgs);
char *FindCdromFilePathBySuffix(const char *pszSuffix);
GAME_DynamicCString *ConstructDynamicCString(GAME_DynamicCString *pString);
GAME_DynamicCString *ConstructDynamicCStringFromCString(GAME_DynamicCString *pString, const char *pszText);
GAME_DynamicCString *CopyConstructDynamicCString(GAME_DynamicCString *pString, const GAME_DynamicCString *pSource);
void DestroyDynamicCString(GAME_DynamicCString *pString);
GAME_DynamicCString *AssignDynamicCString(GAME_DynamicCString *pString, const char *pszText);
GAME_DynamicCString *AssignDynamicCStringFromDynamic(GAME_DynamicCString *pString,
                                                       const GAME_DynamicCString *pSource);
GAME_DynamicCString *AppendDynamicCStringObjectAndCopyResult(GAME_DynamicCString *pString,
                                                              GAME_DynamicCString *pResult,
                                                              const GAME_DynamicCString *pSuffix);
GAME_DynamicCString *AppendDynamicCStringAndCopyResult(GAME_DynamicCString *pString,
                                                        GAME_DynamicCString *pResult,
                                                        const char *pszSuffix);
void *AppendDynamicCStringToStream(void *pStream, const GAME_DynamicCString *pString);
int LEMBALL_FASTCALL GetDynamicCStringLength(const GAME_DynamicCString *pString);
void DestroyNamedStatusEntry(void *pEntry);
void UpdateNamedStatusEntry(void *pEntry, unsigned int nValue);
VSINIT_FormattedOutputStream *WriteNamedStatusEntry(void *pEntry, VSINIT_FormattedOutputStream *pStream);
void ReleaseTypedResourceObjectReference(void *pResourceObject);
void LEMBALL_FASTCALL InitializeRenderQueueNodeBase(void *pRenderQueueNode);
void RegisterOrderedRenderDispatchClient(void *pDispatchQueue, void *pClient, int nOrder);
void UnregisterOrderedRenderDispatchClient(void *pDispatchQueue, void *pClient, int nOrder);

extern void *g_pSharedRenderDispatchQueue;

#endif
