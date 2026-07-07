#ifndef LEMBALL_VSINIT_H
#define LEMBALL_VSINIT_H

#include "COMMON.H"

struct VSINIT_CommandLineOption {
    const char *m_pszName;
    int m_nValue;
    int m_fSpecified;
};

int InitializeCoreSubsystems(void);
void ShutdownCoreSubsystems(void);
int ParseCommandLineOptionToken(const char *pszToken);
void TokenizeAndFilterCommandLineArgs(const char *pszCmdLine);
unsigned int GetParsedCommandLineArgumentCount(void);
const VSINIT_CommandLineOption *GetCommandLineOptions(size_t *pcOptions);
void LogParsedCommandLineOptions(void);
void AppendStartupCString(const char *pszText);
void AppendStartupUInt(unsigned int uValue);
void AppendStatusCString(const char *pszText);
void AppendErrorCString(const char *pszText);
void AppendErrorUInt(unsigned int uValue);

#endif
