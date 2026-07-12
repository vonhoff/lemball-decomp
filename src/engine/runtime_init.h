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
struct VSINIT_StatusEntryPointerArray {
    void *m_pVtable;
    int m_nReserved;
    void **m_ppEntries;
    int m_cEntriesMax;
    int m_cEntries;

    VSINIT_StatusEntryPointerArray *ConstructStatusEntryPointerArray(int cEntriesMax);
    void AppendStatusEntry(void *pEntry);
};

void InitializeCoreSubsystems(void);
void ShutdownCoreSubsystems(void);
int RunGameStartupSequence(char *pszCmdLine);
void LongjmpStartupExceptionFrame(int nJumpResult);
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
VSINIT_FormattedOutputStream *AppendCStringToStream(VSINIT_FormattedOutputStream *pStream,
                                                     const char *pszText);
void AppendPointerToStreamVariant(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
void AppendCharToFormattedStream(VSINIT_FormattedOutputStream *pStream, char ch);
void AppendUIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
void AppendUnsignedIntToStreamVariant(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
VSINIT_FormattedOutputStream *AppendUIntHexToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
VSINIT_FormattedOutputStream *AppendHexUIntToStream(VSINIT_FormattedOutputStream *pStream, unsigned int uValue);
VSINIT_FormattedOutputStream *WriteStatusEntryPointerArray(void *pRegistry, VSINIT_FormattedOutputStream *pStream);
void *ConstructRenderDispatchQueue(void *pQueue, int cEntries);
struct RDISPATCH_QueueEntry {
    unsigned int m_awords[5];
};

struct RDISPATCH_ClientNode {
    void *m_pClient;
    int m_nOrder;
    RDISPATCH_ClientNode *m_pNext;
};

struct RDISPATCH_Queue {
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
    RDISPATCH_QueueEntry *m_pEntryBuffer;
    RDISPATCH_QueueEntry *m_pEntryBufferEnd;
    RDISPATCH_QueueEntry *m_pHead;
    RDISPATCH_QueueEntry *m_pTail;
    RDISPATCH_ClientNode *m_pClientList;

    RDISPATCH_Queue *ConstructRenderDispatchQueueVariant(int cEntries,
                                                           const char *pszName);
};
void DestroyRenderDispatchQueue(void *pQueue);
void *DeleteRenderDispatchQueue(void *pQueue, unsigned char fFreeMemory);
void LEMBALL_FASTCALL ConstructStreamFormatState(VSINIT_StreamFormatTargetState *pState);
void LEMBALL_FASTCALL RestoreStreamFormatSubobjectVtable(VSINIT_StreamFormatTargetState *pState);
void LEMBALL_FASTCALL DestroyFixedBufferStream(VSINIT_FixedBufferStream *pStream);
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
extern void *g_StreamBaseVtable[4];
extern unsigned long g_FormattedOutputStreamVtable[2];
extern unsigned long g_EffFormattedOutputStreamVtable[3];
extern unsigned long g_EffStreamSubobjectVtable[2];
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

    void *ConstructStreamBaseVtable(char *pszBuffer, unsigned int cbBuffer);
    VSINIT_FixedBufferStream *ConstructFixedBufferStream(char *pszBuffer,
                                                          unsigned int cbBuffer,
                                                          void (*pfnFlush)(char *));
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

    VSINIT_StreamFormatTargetState *ConstructStreamFormatTargetState(VSINIT_FixedBufferStream *pDownstream);
};

/* The short-lived EFF stream used by startup is a composite object.  Its
 * formatted subobject is only a vtable-bearing view; the actual format state
 * is the subobject 0x20 bytes before it. */
struct VSINIT_EffStreamSubobject {
    void *m_pVtable;
};

struct VSINIT_EffFormattedOutputStream {
    void *m_pVtable;
    VSINIT_FixedBufferStream m_FixedBufferStream;
    VSINIT_StreamFormatTargetState m_TargetState;
    VSINIT_EffStreamSubobject m_FormattedSubobject;
};

VSINIT_StreamFormatTargetState *ConstructStreamFormatTargetState(VSINIT_StreamFormatTargetState *pState,
                                                                 VSINIT_FixedBufferStream *pDownstream);

VSINIT_FixedBufferStream *ConstructFixedBufferStream(VSINIT_FixedBufferStream *pStream,
                                                     char *pszBuffer,
                                                     unsigned int cbBuffer,
                                                     void (*pfnFlush)(char *));

struct VSINIT_FormattedOutputStream {
    void *m_pVtable;
    char m_szFormatBuffer[0x124];
    char *m_pszFormattedText;
    VSINIT_StreamFormatTargetState m_TargetState;

    VSINIT_FormattedOutputStream *AppendCStringToStream(const char *pszText);
    VSINIT_FormattedOutputStream *AppendCharToStreamVariant(char ch);
    VSINIT_FormattedOutputStream *AppendIntToStream(unsigned int uValue);
    VSINIT_FormattedOutputStream *AppendSignedIntToStreamVariant(int nValue);
    VSINIT_FormattedOutputStream *ConstructFormattedOutputStream(VSINIT_FixedBufferStream *pDownstream,
                                                                  int fConstructTargetState);
};

VSINIT_FormattedOutputStream *ConstructFormattedOutputStream(VSINIT_FormattedOutputStream *pStream,
                                                             VSINIT_FixedBufferStream *pDownstream,
                                                             int fConstructTargetState);

#endif
