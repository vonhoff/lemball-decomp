#ifndef LEMBALL_RUNTIME_INIT_H
#define LEMBALL_RUNTIME_INIT_H

#include "ENGINE/CORE/COMMON.H"

struct VsInitCommandLineOption {
    const char *m_pszName;
    int *m_pnValue;
};

struct VsInitFixedBufferStream;
struct VsInitFormattedOutputStream;
struct VsInitStreamFormatTargetState;
struct VsInitStatusEntryPointerArray {
    void *m_pVtable;
    int m_nReserved;
    void **m_ppEntries;
    int m_cEntriesMax;
    int m_cEntries;

    VsInitStatusEntryPointerArray *ConstructStatusEntryPointerArray(int cEntriesMax);
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
const VsInitCommandLineOption *GetCommandLineOptions(size_t *pcOptions);
void AppendStartupCString(const char *pszText);
void AppendStartupUInt(unsigned int uValue);
void AppendStatusCString(const char *pszText);
void AppendErrorCString(const char *pszText);
void AppendErrorUInt(unsigned int uValue);
void AppendStringToDebugOutFile(const char *pszText);
void FormatSignedIntToRadixString(int nValue, char *pszBuffer, unsigned int nRadix);
VsInitFormattedOutputStream *AppendCStringToStream(VsInitFormattedOutputStream *pStream,
                                                     const char *pszText);
void AppendPointerToStreamVariant(VsInitFormattedOutputStream *pStream, unsigned int uValue);
void AppendCharToFormattedStream(VsInitFormattedOutputStream *pStream, char ch);
void AppendUIntToStream(VsInitFormattedOutputStream *pStream, unsigned int uValue);
void AppendUnsignedIntToStreamVariant(VsInitFormattedOutputStream *pStream, unsigned int uValue);
VsInitFormattedOutputStream *AppendUIntHexToStream(VsInitFormattedOutputStream *pStream, unsigned int uValue);
VsInitFormattedOutputStream *AppendHexUIntToStream(VsInitFormattedOutputStream *pStream, unsigned int uValue);
// FUNCTION: LEMBALL 0x0045abb0
// WriteStatusEntryPointerArray
VsInitFormattedOutputStream *LEMBALL_FASTCALL WriteStatusEntryPointerArray(
    void *pRegistry, int nUnused, VsInitFormattedOutputStream *pStream);
void *ConstructRenderDispatchQueue(void *pQueue, int cEntries);
struct RenderDispatchQueueEntry {
    unsigned int m_awords[5];
};

struct RenderDispatchClientNode {
    void *m_pClient;
    int m_nOrder;
    RenderDispatchClientNode *m_pNext;
};

struct RenderDispatchQueue {
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
    RenderDispatchQueueEntry *m_pEntryBuffer;
    RenderDispatchQueueEntry *m_pEntryBufferEnd;
    RenderDispatchQueueEntry *m_pHead;
    RenderDispatchQueueEntry *m_pTail;
    RenderDispatchClientNode *m_pClientList;

    RenderDispatchQueue *ConstructRenderDispatchQueueVariant(int cEntries,
                                                           const char *pszName);
};
void LEMBALL_FASTCALL DestroyRenderDispatchQueue(void *pQueue);
void *DeleteRenderDispatchQueue(void *pQueue, unsigned char fFreeMemory);
void *RegisterPaletteRemapVariant(int nPaletteResourceId,
                                  unsigned char *pRemapTable,
                                  int nBuildMode);
void ReleasePaletteRemapVariant(void *pVariant);
void LEMBALL_FASTCALL ConstructStreamFormatState(VsInitStreamFormatTargetState *pState);
void LEMBALL_FASTCALL RestoreStreamFormatSubobjectVtable(VsInitStreamFormatTargetState *pState);
void LEMBALL_FASTCALL DestroyFixedBufferStream(VsInitFixedBufferStream *pStream);
void *LEMBALL_FASTCALL DeleteFixedBufferStreamReturnThis(
    VsInitFixedBufferStream *pStream, int nUnused, int fFreeMemory);
void __fastcall ResetFixedBufferStream(VsInitFixedBufferStream *pStream);
// FUNCTION: LEMBALL 0x0045ae10
// AppendCharToFixedBufferStream
void LEMBALL_FASTCALL AppendCharToFixedBufferStream(
    VsInitFixedBufferStream *pStream, int nUnused, char ch);
// FUNCTION: LEMBALL 0x0045af20
// AppendCStringToFixedBufferStream
void LEMBALL_FASTCALL AppendCStringToFixedBufferStream(
    VsInitFixedBufferStream *pStream, int nUnused, const char *pszText);
void *LEMBALL_FASTCALL ReturnStreamArgument(
    void *pStream, int nUnused, void *pArgument);
int LEMBALL_FASTCALL AppendRenderDispatchQueueEntry(
    void *pQueue, int nUnused, RenderDispatchQueueEntry *pEntry);
int LEMBALL_FASTCALL DispatchRenderQueueEntryImmediately(
    void *pQueue, int nUnused, RenderDispatchQueueEntry *pEntry);
// FUNCTION: LEMBALL 0x00459990
// EnterObjectCriticalSection
void EnterObjectCriticalSection(void *pObject);
// FUNCTION: LEMBALL 0x004599a0
// LeaveObjectCriticalSection
void LeaveObjectCriticalSection(void *pObject);

extern int g_fStartupNoWait;
extern VsInitFormattedOutputStream *g_pStatusOutputStream;
extern VsInitFormattedOutputStream *g_pErrorOutputStream;
extern char g_abProcessCurrentDirectoryBuffer[0x100];
struct VsInitResourceTypeTable;
extern VsInitResourceTypeTable *g_pPrimaryResourceTypeTable;
extern VsInitResourceTypeTable *g_pSecondaryResourceTypeTable;
extern VsInitResourceTypeTable *g_pTertiaryResourceTypeTable;
extern void *g_StreamBaseVtable[4];
extern unsigned long g_FormattedOutputStreamVtable[2];
extern unsigned long g_EffFormattedOutputStreamVtable[3];
extern unsigned long g_EffStreamSubobjectVtable[2];
extern void *g_StreamFormatTargetStateVtable[1];
extern void *g_StreamFormatSubobjectVtable[1];

struct VsInitFixedBufferStream {
    void *m_pVtable;
    char *m_pszBuffer;
    unsigned int m_cbBuffer;
    char *m_pszCursor;
    unsigned int m_cchWritten;
    int m_nTabWidth;
    void (*m_pfnFlush)(char *);

    void *ConstructStreamBaseVtable(char *pszBuffer, unsigned int cbBuffer);
    VsInitFixedBufferStream *ConstructFixedBufferStream(char *pszBuffer,
                                                          unsigned int cbBuffer,
                                                          void (*pfnFlush)(char *));
};

struct VsInitStreamFormatTargetState {
    void *m_pVtable;
    int m_nReserved04;
    unsigned int m_dwFlags;
    int m_nReserved0c;
    char m_chFill;
    char m_abReserved11[3];
    int m_nWidth;
    int m_nRadix;
    VsInitFixedBufferStream *m_pDownstream;

    VsInitStreamFormatTargetState *ConstructStreamFormatTargetState(VsInitFixedBufferStream *pDownstream);
};

/* The short-lived EFF stream used by startup is a composite object.  Its
 * formatted subobject is only a vtable-bearing view; the actual format state
 * is the subobject 0x20 bytes before it. */
struct VsInitEffStreamSubobject {
    void *m_pVtable;
};

struct VsInitEffFormattedOutputStream {
    void *m_pVtable;
    VsInitFixedBufferStream m_FixedBufferStream;
    VsInitStreamFormatTargetState m_TargetState;
    VsInitEffStreamSubobject m_FormattedSubobject;
};

VsInitStreamFormatTargetState *ConstructStreamFormatTargetState(VsInitStreamFormatTargetState *pState,
                                                                 VsInitFixedBufferStream *pDownstream);

VsInitFixedBufferStream *ConstructFixedBufferStream(VsInitFixedBufferStream *pStream,
                                                     char *pszBuffer,
                                                     unsigned int cbBuffer,
                                                     void (*pfnFlush)(char *));

struct VsInitFormattedOutputStream {
    void *m_pVtable;
    char m_szFormatBuffer[0x124];
    char *m_pszFormattedText;
    VsInitStreamFormatTargetState m_TargetState;

    VsInitFormattedOutputStream *AppendCStringToStream(const char *pszText);
    VsInitFormattedOutputStream *AppendCharToStreamVariant(char ch);
    VsInitFormattedOutputStream *AppendIntToStream(unsigned int uValue);
    VsInitFormattedOutputStream *AppendSignedIntToStreamVariant(int nValue);
    VsInitFormattedOutputStream *ConstructFormattedOutputStream(VsInitFixedBufferStream *pDownstream,
                                                                  int fConstructTargetState);
};

VsInitFormattedOutputStream *ConstructFormattedOutputStream(VsInitFormattedOutputStream *pStream,
                                                             VsInitFixedBufferStream *pDownstream,
                                                             int fConstructTargetState);

#endif
