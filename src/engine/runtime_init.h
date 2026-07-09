#ifndef LEMBALL_RUNTIME_INIT_H
#define LEMBALL_RUNTIME_INIT_H

#include "engine/common.h"

struct VSINIT_CommandLineOption {
    const char *m_pszName;
    int *m_pnValue;
};

struct VSINIT_FixedBufferStream;
struct VSINIT_FormattedOutputStream;
struct VSINIT_StreamFormatTargetState;

void InitializeCoreSubsystems(void);
void ShutdownCoreSubsystems(void);
int RunGameStartupSequence(char *pszCmdLine);
int ShutdownDebugMessageThreadFromStartup(int fForceTerminate);
int ParseCommandLineOptionToken(char *pszToken);
void TokenizeAndFilterCommandLineArgs(char *pszCmdLine);
unsigned int GetParsedCommandLineArgumentCount(void);
const char *const *GetParsedCommandLineArgs(void);
const VSINIT_CommandLineOption *GetCommandLineOptions(size_t *pcOptions);
void AppendStartupCString(const char *pszText);
void AppendStartupUInt(unsigned int uValue);
void AppendStatusCString(const char *pszText);
void AppendErrorCString(const char *pszText);
void AppendErrorUInt(unsigned int uValue);
void FormatSignedIntToRadixString(int nValue, char *pszBuffer, unsigned int nRadix);
VSINIT_FormattedOutputStream *AppendCStringToStream(VSINIT_FormattedOutputStream *pStream, const char *pszText);
void AppendIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
VSINIT_FormattedOutputStream *AppendCharToStreamVariant(VSINIT_FormattedOutputStream *pStream, char ch);
void AppendSignedIntToStreamVariant(VSINIT_FormattedOutputStream *pStream, int nValue);
void AppendPointerToStreamVariant(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
void AppendCharToFormattedStream(VSINIT_FormattedOutputStream *pStream, char ch);
void AppendUIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
void AppendUnsignedIntToStreamVariant(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
VSINIT_FormattedOutputStream *AppendUIntHexToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
VSINIT_FormattedOutputStream *AppendHexUIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
VSINIT_FormattedOutputStream *WriteStatusEntryPointerArray(void *pRegistry, VSINIT_FormattedOutputStream *pStream);
void AppendStatusEntryToRegistry(void *pRegistry, void *pEntry);
void *ConstructRenderDispatchQueue(void *pQueue, int cEntries);
void *ConstructRenderDispatchQueueVariant(void *pQueue, int cEntries);
void DestroyRenderDispatchQueue(void *pQueue);
void *DeleteRenderDispatchQueue(void *pQueue, unsigned char fFreeMemory);
VSINIT_FormattedOutputStream *ConstructFormattedOutputStream(VSINIT_FormattedOutputStream *pStream,
                                                             VSINIT_FixedBufferStream *pDownstream,
                                                             int fConstructTargetState);
VSINIT_StreamFormatTargetState *ConstructStreamFormatTargetState(VSINIT_StreamFormatTargetState *pState,
                                                                 VSINIT_FixedBufferStream *pDownstream);
void ConstructStreamFormatState(VSINIT_StreamFormatTargetState *pState);
void RestoreStreamFormatSubobjectVtable(VSINIT_StreamFormatTargetState *pState);
VSINIT_FixedBufferStream *ConstructFixedBufferStream(VSINIT_FixedBufferStream *pStream,
                                                     char *pszBuffer,
                                                     unsigned int cbBuffer,
                                                     void (*pfnFlush)(char *));
void DestroyFixedBufferStream(VSINIT_FixedBufferStream *pStream);
void *DeleteFixedBufferStreamReturnThis(VSINIT_FixedBufferStream *pStream, unsigned char fFreeMemory);
void ResetFixedBufferStream(VSINIT_FixedBufferStream *pStream);
void AppendCharToFixedBufferStream(VSINIT_FixedBufferStream *pStream, char ch);
void AppendCStringToFixedBufferStream(VSINIT_FixedBufferStream *pStream, const char *pszText);
void *ReturnStreamArgument(void *pStream, void *pArgument);
void EnterObjectCriticalSection(void *pObject);
void LeaveObjectCriticalSection(void *pObject);

extern int g_fStartupNoWait;
extern VSINIT_FormattedOutputStream *g_pStatusOutputStream;
extern VSINIT_FormattedOutputStream *g_pErrorOutputStream;
extern char g_abProcessCurrentDirectoryBuffer[0x100];
extern void *g_StreamBaseVtable[1];
extern void *g_FormattedOutputStreamVtable[2];
extern void *g_StreamFormatTargetStateVtable[1];
extern void *g_StreamFormatSubobjectVtable[1];

struct VSINIT_FixedBufferStream {
    void *m_pVtable;
    char *m_pszBuffer;
    unsigned int m_cbBuffer;
    char *m_pszCursor;
    unsigned int m_cchWritten;
    int m_nTabWidth;
    void (*m_pfnFlush)(char *);
};

struct VSINIT_StreamFormatTargetState {
    void *m_pVtable;
    int m_nReserved04;
    unsigned int m_dwFlags;
    int m_nReserved0c;
    char m_chFill;
    char m_abReserved11[3];
    int m_nWidth;
    int m_nRadix;
    VSINIT_FixedBufferStream *m_pDownstream;
};

struct VSINIT_FormattedOutputStream {
    void *m_pVtable;
    char m_szFormatBuffer[0x124];
    char *m_pszFormattedText;
    VSINIT_StreamFormatTargetState m_TargetState;
};

#endif
