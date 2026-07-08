#ifndef LEMBALL_VSINIT_H
#define LEMBALL_VSINIT_H

#include "common.h"

struct VSINIT_CommandLineOption {
    const char *m_pszName;
    int *m_pnValue;
};

struct VSINIT_FormattedOutputStream;

void InitializeCoreSubsystems(void);
void ShutdownCoreSubsystems(void);
int RunGameStartupSequence(char *pszCmdLine);
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
VSINIT_FormattedOutputStream *AppendCStringToStream(VSINIT_FormattedOutputStream *pStream, const char *pszText);
VSINIT_FormattedOutputStream *AppendIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
VSINIT_FormattedOutputStream *AppendCharToStreamVariant(VSINIT_FormattedOutputStream *pStream, char ch);
void AppendSignedIntToStreamVariant(VSINIT_FormattedOutputStream *pStream, int nValue);
void AppendPointerToStreamVariant(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
void AppendCharToFormattedStream(VSINIT_FormattedOutputStream *pStream, char ch);
void AppendUIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
void AppendUnsignedIntToStreamVariant(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
VSINIT_FormattedOutputStream *AppendUIntHexToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
VSINIT_FormattedOutputStream *AppendHexUIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
void AppendStatusEntryToRegistry(void *pRegistry, void *pEntry);

extern int g_fStartupNoWait;

#endif
