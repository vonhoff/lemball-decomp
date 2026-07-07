#ifndef LEMBALL_GAME_H
#define LEMBALL_GAME_H

#include "win32.h"

struct GAME_DynamicCString {
    char *m_pszText;
    int m_cchText;
    int m_cchCapacity;
    int m_fExternalStorage;
};

class GAME_StatusEntry {
public:
    GAME_StatusEntry(const char *pszName);

public:
    void *m_pVtable;
    int m_nCurrentValue;
    int m_nPeakValue;
    int m_nMinimumValue;
    int m_nMaximumValue;
    GAME_DynamicCString m_Name;
};

class GAME_MainContext {
public:
    GAME_MainContext(void);

public:
    char m_szCaption[0x50];
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
int RunGameStartupSequence(char *pszCmdLine);
char *FindCdromFilePathBySuffix(const char *pszSuffix);

#endif
