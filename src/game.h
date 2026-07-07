#ifndef LEMBALL_GAME_H
#define LEMBALL_GAME_H

#include "win32.h"

class GAME_StatusEntry {
public:
    GAME_StatusEntry(const char *pszName);

public:
    const char *m_pszName;
    int m_nReserved0;
    int m_nReserved1;
    int m_fActive;
};

class GAME_MainContext {
public:
    GAME_MainContext(void);

public:
    int m_fRegistryRunning;
    int m_fMusicEnabled;
    int m_fEffectsEnabled;
    int m_fSessionReady;
    int m_fInstalled;
    const char *m_pszRegistryKey;
    const char *m_pszWindowTitle;
    const char *m_pszResourceArchiveName;
    const char *m_pszStartupMusicName;
    char m_szSrcDisk[256];
    char m_szDisplayCaption[256];
    GAME_StatusEntry *m_pProcessingStatus;
    GAME_StatusEntry *m_pRefreshingStatus;
};

GAME_MainContext *InitializeMainGameContext(GAME_MainContext *pMainContext, const char *pszCmdLine);
void ShutdownMainGameContext(GAME_MainContext *pMainContext);
int RunMainGameSession(int cArgs, const char *const *ppszArgs);
int RunGameStartupSequence(char *pszCmdLine);
char *FindCdromFilePathBySuffix(const char *pszSuffix);

#endif
