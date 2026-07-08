#ifndef LEMBALL_VSINIT_H
#define LEMBALL_VSINIT_H

#include "common.h"

struct VSINIT_CommandLineOption {
    const char *m_pszName;
    int *m_pnValue;
};

void InitializeCoreSubsystems(void);
void ShutdownCoreSubsystems(void);
int ShutdownDebugMessageThreadFromStartup(int fForceTerminate);
int ParseCommandLineOptionToken(char *pszToken);
void TokenizeAndFilterCommandLineArgs(char *pszCmdLine);
unsigned int GetParsedCommandLineArgumentCount(void);
const char *const *GetParsedCommandLineArgs(void);
const VSINIT_CommandLineOption *GetCommandLineOptions(size_t *pcOptions);
void LogParsedCommandLineOptions(void);
void AppendStartupCString(const char *pszText);
void AppendStartupUInt(unsigned int uValue);
void AppendStatusCString(const char *pszText);
void AppendErrorCString(const char *pszText);
void AppendErrorUInt(unsigned int uValue);

#endif
