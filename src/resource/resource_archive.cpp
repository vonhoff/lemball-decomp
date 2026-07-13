#include "resource_archive.h"

#include "../game/game_app.h"
#include "../platform/startup_options.h"
#include "../engine/runtime_init.h"
#include "../engine/memory_arena.h"

#include <ctype.h>
#if defined(_MSC_VER) && (_MSC_VER < 1100)
#include <new.h>
#else
#include <new>
#endif
#include <stdlib.h>
#include <string.h>

/* Overlay used only to express the target arena's virtual dispatch. */
struct MOGLOAD_ArenaDispatch {
    virtual void Report(void *pOutput, void *pStream) = 0;
    virtual void Destroy(int fDelete) = 0;
    virtual int Allocate(void **ppBlock,
                         unsigned int cbBlock,
                         const char *pszDescription) = 0;
    virtual int Free(void *pBlock) = 0;
};

static FILE *g_pResourceArchiveFile = 0;
void *g_pResourceArchiveMemoryArena = 0;
static const unsigned int g_MOGLOAD_StringResourceTypeTag = 0x53545247;
static const unsigned int g_MOGLOAD_EffResourceTypeTag = 0x45464620;
static const char g_MOGLOAD_OpenMode[] = "rb";
static const char g_MOGLOAD_StatusName[] = "Mogload memory";
static char g_MOGLOAD_RootPath[] = "/";
static const char g_MOGLOAD_SourceFileName[] = "MOGLOAD.CPP";
static const char g_MOGLOAD_IsValidResourceFileAssert[] = "IsValidResourceFile()";

struct MOGLOAD_ResourceArenaStatusOwner {
    char m_abReserved00[0x30];
    GAME_StatusEntry *m_pStatusEntry;
};

typedef void (LEMBALL_FASTCALL *MOGLOAD_ResourceObjectVoidProc)(void *pObject);
typedef int (LEMBALL_FASTCALL *MOGLOAD_ResourceObjectIntProc)(void *pObject);
typedef void *(LEMBALL_FASTCALL *MOGLOAD_ResourceObjectPointerProc)(void *pObject);
typedef void (LEMBALL_FASTCALL *MOGLOAD_ResourceObjectArgumentProc)(
    void *pObject, void *pUnusedEdx, int nArgument);

struct MOGLOAD_TypedResourceObjectVtable {
    void *m_pDelete;
    void *m_pAssignLoadedBuffer;
    MOGLOAD_ResourceObjectVoidProc m_pConstructFromArchiveEntry;
    MOGLOAD_ResourceObjectIntProc m_pGetFrameAge14;
    MOGLOAD_ResourceObjectIntProc m_pGetArchiveEntryField1C;
    MOGLOAD_ResourceObjectIntProc m_pGetFrameAgeMirror14;
    MOGLOAD_ResourceObjectIntProc m_pReturnZero;
    MOGLOAD_ResourceObjectVoidProc m_pEnsureLoaded;
    MOGLOAD_ResourceObjectArgumentProc m_pReleaseIfLoaded;
    MOGLOAD_ResourceObjectArgumentProc m_pOnRelease;
    MOGLOAD_ResourceObjectPointerProc m_pGetResourceDataPointer;
    MOGLOAD_ResourceObjectVoidProc m_pOnLoad;
    MOGLOAD_ResourceObjectVoidProc m_pAfterLoad;
    MOGLOAD_ResourceObjectVoidProc m_pClearTypeTag;
    MOGLOAD_ResourceObjectIntProc m_pGetMemorySize;
};

struct MOGLOAD_TypedResourceObjectCopyInterface {
    virtual void Reserved00(void) = 0;
    virtual void CopyBufferIntoTypedResourceObjectAndParse(unsigned int *pSource,
                                                           unsigned int nUnused,
                                                           unsigned int cbBuffer) = 0;
};

void LEMBALL_FASTCALL CopyBufferIntoTypedResourceObjectAndParse(
    void *pObject,
    void *pUnusedEdx,
    unsigned int *pSource,
    unsigned int nUnused,
    unsigned int cbBuffer);

typedef void *(LEMBALL_FASTCALL *MOGLOAD_DeleteResourceProc)(
    void *pObject, int nUnused, int fDelete);
void *LEMBALL_FASTCALL DestroyGRTSResourceAndFree(
    void *pObject, int nUnused, int fDelete);
void *LEMBALL_FASTCALL DeleteCachedResourceObjectBase(
    void *pObject, int nUnused, int fDelete);
void InitializeTypedResourceObjectBaseVtable(void *pObject);
void ParseIntResourceDescriptor(void);
void LEMBALL_FASTCALL UnloadTypedResourceObject(void *pObject,
                                                void *pUnusedEcx,
                                                int fReleaseMode);
void LEMBALL_FASTCALL EnsureTypedResourceObjectLoaded(void *pObject);
void LEMBALL_FASTCALL ReleaseTypedResourceObjectIfLoaded(
    void *pObject, void *pUnusedEdx, int fReleaseMode);
int LEMBALL_FASTCALL GetField14NeedsAgeIncrement(void *pObject);
int LEMBALL_FASTCALL GetField1CVfunc04(void *pObject);
int LEMBALL_FASTCALL GetField14Vfunc05(void *pObject);
int ReturnZeroVfunc06(void);
void LEMBALL_FASTCALL NoopVfunc09(
    void *pObject, void *pUnusedEdx, int nArgument);
void *LEMBALL_FASTCALL GetEffResourceDataPointer(void *pObject);
void NoopOnLoadVfunc11(void);
void NoopVfunc12(void);
void LEMBALL_FASTCALL ClearResourceTypeTag(void *pObject);
int LEMBALL_FASTCALL GetField28GetMemorySize(void *pObject);
void *LEMBALL_FASTCALL DeleteTypedResourceObject(
    void *pObject, int nUnused, int fDelete);
void LEMBALL_FASTCALL SetGRTSResourceTypeTag(void *pObject);
void *LEMBALL_FASTCALL DestroyBitmapResource(
    void *pObject, int nUnused, int fDelete);
void *LEMBALL_FASTCALL DestroyPaletteResource(
    void *pObject, int nUnused, int fDelete);
void LEMBALL_FASTCALL ParseBitmapResourceDescriptor(void *pObject);
void LEMBALL_FASTCALL ParsePaletteResourceDescriptor(void *pObject);
void *LEMBALL_FASTCALL DestroyEffResource(
    void *pObject, int nUnused, int fDelete);
extern void LEMBALL_FASTCALL SetPalResourceType(void *pObject);
extern void LEMBALL_FASTCALL SetBitmapResourceType(void *pObject);
extern void LEMBALL_FASTCALL SetEffResourceTypeTag(void *pObject);
static void *g_MOGLOAD_MemoryArenaStatusEntryVtable[8] = {
    (void *)WriteNamedStatusEntry,
    (void *)UpdateNamedStatusEntry,
    (void *)EnterObjectCriticalSection,
    (void *)LeaveObjectCriticalSection,
    (void *)EnterObjectCriticalSection,
    (void *)LeaveObjectCriticalSection,
    (void *)WriteMemoryArenaReport,
    (void *)DestroyMemoryArenaBaseStateReturnThis,
};
static void *g_MOGLOAD_CachedResourceObjectBaseVtable[15] = {
    (void *)DeleteCachedResourceObjectBase,
    (void *)CopyBufferIntoTypedResourceObjectAndParse,
    (void *)ParseIntResourceDescriptor,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)UnloadTypedResourceObject,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)ClearResourceTypeTag,
    (void *)GetField28GetMemorySize,
};
static void *g_MOGLOAD_TypedResourceObjectVtable[15] = {
    (void *)DeleteTypedResourceObject,
    (void *)CopyBufferIntoTypedResourceObjectAndParse,
    (void *)ParseIntResourceDescriptor,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)UnloadTypedResourceObject,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)ClearResourceTypeTag,
    (void *)GetField28GetMemorySize,
};
static void *g_MOGLOAD_StringResourceVtable[15] = {
    (void *)DestroyGRTSResourceAndFree,
    (void *)CopyBufferIntoTypedResourceObjectAndParse,
    (void *)ParseIntResourceDescriptor,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)UnloadTypedResourceObject,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)SetGRTSResourceTypeTag,
    (void *)GetField28GetMemorySize,
};
static void *g_MOGLOAD_EffResourceObjectVtable[15] = {
    (void *)DestroyEffResource,
    (void *)CopyBufferIntoTypedResourceObjectAndParse,
    (void *)ParseIntResourceDescriptor,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)UnloadTypedResourceObject,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)SetEffResourceTypeTag,
    (void *)GetField28GetMemorySize,
};
static void *g_MOGLOAD_BitmapResourceVtable[15] = {
    (void *)DestroyBitmapResource,
    (void *)CopyBufferIntoTypedResourceObjectAndParse,
    (void *)ParseBitmapResourceDescriptor,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)UnloadTypedResourceObject,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)SetBitmapResourceType,
    (void *)GetField28GetMemorySize,
};
static void *g_MOGLOAD_PaletteResourceVtable[15] = {
    (void *)DestroyPaletteResource,
    (void *)CopyBufferIntoTypedResourceObjectAndParse,
    (void *)ParsePaletteResourceDescriptor,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)UnloadTypedResourceObject,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)SetPalResourceType,
    (void *)GetField28GetMemorySize,
};

extern void *g_pMainResourceArchive;
extern void *FinalizeLoadedResourceObjectResult(void *pObject);
extern void ResetTypedResourceObjectState(void *pObject);
extern void AdvanceCachedResourceObjectFrameCounters(void *pArchive);
extern void *g_pSmallMemoryBucketTable;
extern unsigned int g_cbSmallMemoryBucketUpperBound;
extern const char *g_pszSmallMemoryBucketAllocTag;
extern void *g_pStatusEntryRegistry;
extern void EnterObjectCriticalSection(void *pObject);
extern void LeaveObjectCriticalSection(void *pObject);
extern void CloseCrtFilePointer(FILE *pFile);
extern FILE *OpenFileWithMode(const char *pszPath, const char *pszMode);
extern unsigned int TellFile(FILE *pFile);
extern unsigned int SeekFile(FILE *pFile, long lOffset, int nOrigin);
extern unsigned int ReadFileBytes(FILE *pFile, void *pBuffer, size_t cbBuffer);
extern "C" DWORD WINAPI timeGetTime(void);
extern void FreeResourceObjectDataBuffer(unsigned int pBuffer);
extern void *LEMBALL_FASTCALL DestroyMemoryArenaBaseStateReturnThis(
    void *pArena, int nUnused, int fDelete);

void *g_pCachedResourceObjectBaseDeleteVtable = g_MOGLOAD_CachedResourceObjectBaseVtable;
void *g_pDestroyGRTSResourceAndFreeThunk = g_MOGLOAD_TypedResourceObjectVtable;
void *g_pStringResourceVtable = g_MOGLOAD_StringResourceVtable;
void *g_pDestroyEffResourceAndFreeThunk = g_MOGLOAD_EffResourceObjectVtable;
void *g_pBitmapResourceVtable = g_MOGLOAD_BitmapResourceVtable;
void *g_pPaletteResourceVtable = g_MOGLOAD_PaletteResourceVtable;

extern void TriggerReleaseAssertFailure(const char *pszExpression, const char *pszFile, int nLine);
void DestroyResourceArchiveDirectoryTree(int pDirectoryNode);

int FindResourceCacheEvictionCandidateIndex(void *pArchive, unsigned int cbMinimumEvict);

// FUNCTION: LEMBALL 0x0045E660
void *LEMBALL_FASTCALL DeleteCachedResourceObjectBase(
    void *pObject, int nUnused, int fDelete) {
    (void)nUnused;
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0 && pObject != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045D040
void InitializeTypedResourceObjectBaseVtable(void *pObject) {
    *(void **)pObject = g_pCachedResourceObjectBaseDeleteVtable;
}

// FUNCTION: LEMBALL 0x0045E5B0
int LEMBALL_FASTCALL GetField14NeedsAgeIncrement(void *pObject) {
    return ((MOGLOAD_StringResourceObject *)pObject)->m_nReserved14;
}

// FUNCTION: LEMBALL 0x0045E5C0
int LEMBALL_FASTCALL GetField1CVfunc04(void *pObject) {
    return ((MOGLOAD_StringResourceObject *)pObject)->m_nReserved1C;
}

// FUNCTION: LEMBALL 0x0045E5D0
int LEMBALL_FASTCALL GetField14Vfunc05(void *pObject) {
    return ((MOGLOAD_StringResourceObject *)pObject)->m_nReserved14;
}

// FUNCTION: LEMBALL 0x0045E5E0
int ReturnZeroVfunc06(void) {
    return 0;
}

// FUNCTION: LEMBALL 0x0045E5F0
void LEMBALL_FASTCALL NoopVfunc09(
    void *pObject, void *pUnusedEdx, int nArgument) {
    (void)pObject;
    (void)pUnusedEdx;
    (void)nArgument;
}

// FUNCTION: LEMBALL 0x0045D100
void LEMBALL_FASTCALL EnsureTypedResourceObjectLoaded(void *pObject) {
    MOGLOAD_StringResourceObject *pResourceObject;
    MOGLOAD_TypedResourceObjectVtable *pVtable;
    int fArchiveEntryAlreadyLoaded;
    unsigned long nArchiveEntryCallback;
    int anFileRange[2];
    unsigned int *pBuffer;

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    if (pResourceObject->m_nLoadState10 == 0) {
        pVtable = (MOGLOAD_TypedResourceObjectVtable *)pResourceObject->m_pVtable;
        fArchiveEntryAlreadyLoaded = pVtable->m_pGetFrameAge14(pResourceObject);
        if (fArchiveEntryAlreadyLoaded == 0) {
            if (pResourceObject->m_nReserved04 == 0) {
                if (pResourceObject->m_cbResourceData28 == 0) {
                    pResourceObject->m_pszText38 = 0;
                    ((MOGLOAD_TypedResourceObjectCopyInterface *)pResourceObject)
                        ->CopyBufferIntoTypedResourceObjectAndParse(
                            0, (unsigned int)(unsigned long)&pResourceObject->m_pszText38, 0);
                } else {
                    anFileRange[0] = pResourceObject->m_lResourceOffset2C;
                    anFileRange[1] = pResourceObject->m_cbResourceData28;
                    pBuffer = (unsigned int *)&pResourceObject->m_pszText38;
                    if (((MOGLOAD_ResourceArchive *)g_pMainResourceArchive)
                            ->LoadResourceArchiveEntryDataIntoBuffer(
                                anFileRange,
                                pBuffer,
                                pResourceObject) != 0) {
                        ((MOGLOAD_TypedResourceObjectCopyInterface *)pResourceObject)
                            ->CopyBufferIntoTypedResourceObjectAndParse(
                                (unsigned int *)(unsigned long)*pBuffer,
                                (unsigned int)(unsigned long)pBuffer,
                                pResourceObject->m_cbResourceData28);
                    }
                }
            } else {
                nArchiveEntryCallback = (unsigned long)(*(int *)(unsigned long)pResourceObject->m_nReserved04 + 0x1c);
                ((void (*)())*(void **)nArchiveEntryCallback)();
            }
        }
    }
    pResourceObject->m_nReserved24 = 0;
}

// FUNCTION: LEMBALL 0x0045D220
void LEMBALL_FASTCALL ReleaseTypedResourceObjectIfLoaded(
    void *pObject, void *pUnusedEdx, int fReleaseMode) {
    MOGLOAD_StringResourceObject *pResourceObject;
    MOGLOAD_TypedResourceObjectVtable *pVtable;

    (void)pUnusedEdx;
    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    pVtable = (MOGLOAD_TypedResourceObjectVtable *)pResourceObject->m_pVtable;
    pVtable->m_pOnRelease(pResourceObject, 0, fReleaseMode);
    if (pResourceObject->m_nLoadState10 != 0) {
        pResourceObject->m_nLoadState10 = 0;
        pResourceObject->m_pszText38 = 0;
        --pResourceObject->m_nLockCount08;
    }
}

// FUNCTION: LEMBALL 0x0045E600
void *LEMBALL_FASTCALL GetEffResourceDataPointer(void *pObject) {
    return ((MOGLOAD_StringResourceObject *)pObject)->m_pszText38;
}

// FUNCTION: LEMBALL 0x0045E610
void NoopOnLoadVfunc11(void) {
}

// FUNCTION: LEMBALL 0x0045E620
void NoopVfunc12(void) {
}

// FUNCTION: LEMBALL 0x0045E640
void LEMBALL_FASTCALL ClearResourceTypeTag(void *pObject) {
    ((MOGLOAD_StringResourceObject *)pObject)->m_uTypeTag = 0;
}

// FUNCTION: LEMBALL 0x0045E650
int LEMBALL_FASTCALL GetField28GetMemorySize(void *pObject) {
    return ((MOGLOAD_StringResourceObject *)pObject)->m_cbResourceData28;
}

// FUNCTION: LEMBALL 0x0045E820
void *LEMBALL_FASTCALL DeleteTypedResourceObject(
    void *pObject, int nUnused, int fDelete) {
    (void)nUnused;
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045EBB0
void *LEMBALL_FASTCALL DestroyEffResource(
    void *pObject, int nUnused, int fDelete) {
    (void)nUnused;
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045EB80
void *LEMBALL_FASTCALL DestroyBitmapResource(
    void *pObject, int nUnused, int fDelete) {
    (void)nUnused;
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045EAC0
void *LEMBALL_FASTCALL DestroyGRTSResourceAndFree(
    void *pObject, int nUnused, int fDelete) {
    int cObjects;
    char *pObjectEnd;
    char *pCurrent;

    (void)nUnused;

    if ((fDelete & 2) == 0) {
        InitializeTypedResourceObjectBaseVtable(pObject);
        if ((fDelete & 1) != 0) {
            FreeVSMemBlock(pObject);
        }
        return pObject;
    }

    cObjects = *((int *)pObject - 1);
    pObjectEnd = (char *)pObject + cObjects * 0x48;
    pCurrent = pObjectEnd;
    while (cObjects-- > 0) {
        pCurrent -= 0x48;
        InitializeTypedResourceObjectBaseVtable(pCurrent);
    }
    FreeVSMemBlock((char *)pObject - 4);
    return pObject;
}

// FUNCTION: LEMBALL 0x0045EA70
void LEMBALL_FASTCALL SetGRTSResourceTypeTag(void *pObject) {
    *(unsigned int *)((char *)pObject + 0x40) = 0x53545247;
}

// FUNCTION: LEMBALL 0x0045EA50
void *LEMBALL_FASTCALL DestroyPaletteResource(
    void *pObject, int nUnused, int fDelete) {
    (void)nUnused;
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045E290
void LEMBALL_FASTCALL ParseBitmapResourceDescriptor(void *pObject) {
    unsigned char *pDescriptor;

    pDescriptor = *(unsigned char **)((char *)pObject + 0x34);
    *(unsigned short *)((char *)pObject + 0x48) =
        *(unsigned short *)(pDescriptor + 0);
    *(unsigned short *)((char *)pObject + 0x4a) =
        *(unsigned short *)(pDescriptor + 4);
    *(unsigned char *)((char *)pObject + 0x4c) = pDescriptor[8];
    *(unsigned char *)((char *)pObject + 0x4d) = pDescriptor[9];
}

// FUNCTION: LEMBALL 0x0045EA40
void LEMBALL_FASTCALL ParsePaletteResourceDescriptor(void *pObject) {
    *(unsigned int *)((char *)pObject + 0x48) =
        **(unsigned int **)((char *)pObject + 0x34);
}

// FUNCTION: LEMBALL 0x0045BBE0
unsigned char MOGLOAD_ReadU8(void) {
    unsigned char bValue;

    bValue = 0;
    if (g_pResourceArchiveFile != 0) {
        fread(&bValue, 1, 1, g_pResourceArchiveFile);
    }
    return bValue;
}

// FUNCTION: LEMBALL 0x0045BC40
unsigned int MOGLOAD_ReadU32(void) {
    unsigned int dwValue;

    dwValue = 0;
    if (g_pResourceArchiveFile != 0) {
        fread(&dwValue, 1, sizeof(dwValue), g_pResourceArchiveFile);
    }
    return dwValue;
}

// FUNCTION: LEMBALL 0x0045C030
int MOGLOAD_DirectoryNode::AppendEntryAfterCursor(void) {
    MOGLOAD_EntryRecord *pEntry;

    pEntry = (MOGLOAD_EntryRecord *)AllocateResourceArchiveMemory(sizeof(MOGLOAD_EntryRecord));
    if (pEntry == 0) {
        return 0;
    }

    m_pCursorEntry->m_pNextEntry = pEntry;
    m_pCursorEntry->m_iNextIndex = (int)m_cLoadedEntries;
    ++m_cLoadedEntries;
    ReadEntryRecord(pEntry);
    return 1;
}

// FUNCTION: LEMBALL 0x0045BDA0
MOGLOAD_DirectoryNode *MOGLOAD_DirectoryNode::Construct(long lFileOffset) {
    long lNameDataSize;
    long lCurrentFileOffset;
    unsigned int uFormatVersion;
    MOGLOAD_EntryRecord *pEntry;

    m_iSubdirectorySavedIndex = -1;
    m_pSubdirectorySavedEntry = 0;
    m_iSubdirectoryCursorIndex = -1;
    m_pSubdirectoryCursorEntry = 0;
    m_iSavedIndex = -1;
    m_pSavedEntry = 0;
    m_iCursorIndex = -1;
    m_pCursorEntry = 0;
    m_lRecordTableOffset = 0;
    m_lNameDataOffset = 0;
    m_cEntries = 0;
    m_cLoadedEntries = 0;
    m_pNameData = 0;
    m_nReserved30 = 0;
    m_nReserved34 = 0;

    /* The CRID directory header carries five u32 fields before the name blob and entry table. */
    if (g_pResourceArchiveFile == 0) {
        return this;
    }

    SeekFile(g_pResourceArchiveFile, lFileOffset, SEEK_SET);
    if (lFileOffset == 0) {
        (void)MOGLOAD_ReadU8();
        SeekFile(g_pResourceArchiveFile, 0L, SEEK_SET);
    }

    (void)MOGLOAD_ReadU32();
    (void)MOGLOAD_ReadU32();
    m_cEntries = MOGLOAD_ReadU32();
    uFormatVersion = MOGLOAD_ReadU32();
    if (uFormatVersion != 3) {
        TriggerReleaseAssertFailure(g_MOGLOAD_IsValidResourceFileAssert, g_MOGLOAD_SourceFileName, 0x1a2);
    }

    m_lRecordTableOffset = (long)MOGLOAD_ReadU32();
    lCurrentFileOffset = (long)TellFile(g_pResourceArchiveFile);
    m_lNameDataOffset = lCurrentFileOffset;
    lNameDataSize = m_lRecordTableOffset - lCurrentFileOffset;
    m_pNameData = (char *)AllocateResourceArchiveMemory((unsigned int)lNameDataSize);
    if (m_pNameData != 0) {
        ReadFileBytes(g_pResourceArchiveFile, m_pNameData, (size_t)lNameDataSize);
    }

    if (m_cEntries != 0) {
        pEntry = (MOGLOAD_EntryRecord *)AllocateResourceArchiveMemory(sizeof(MOGLOAD_EntryRecord));
        if (pEntry != 0) {
            pEntry->m_pszName = 0;
            pEntry->m_uTag = 0;
            pEntry->m_uResourceId = 0;
            pEntry->m_lFileOffset = 0;
            pEntry->m_cbFileSize = 0;
            pEntry->m_iNextIndex = 0;
            pEntry->m_pNextEntry = 0;
            pEntry->m_iSavedIndex = 0;
            pEntry->m_pSavedEntry = 0;
            pEntry->m_pChildDirectory = 0;
            memset(pEntry->m_adwDescriptor, 0, sizeof(pEntry->m_adwDescriptor));
            m_pSavedEntry = pEntry;
            m_iSavedIndex = 0;
            ReadEntryRecord(pEntry);
            m_cLoadedEntries = 1;
        }
    }

    m_iCursorIndex = m_iSavedIndex;
    m_pCursorEntry = m_pSavedEntry;

    for (;;) {
        MOGLOAD_EntrySearchState state;

        state.m_iIndex = 0;
        state.m_pEntry = 0;
        FindNextResourceArchiveEntry(&state, (unsigned int)-1);
        pEntry = state.m_pEntry;
        if (pEntry == 0) {
            break;
        }
        if (pEntry != 0 && pEntry->m_uTag == MOGLOAD_TAG_CRID) {
            (void)AdvanceSubdirectory();
        }
    }

    m_iCursorIndex = m_iSavedIndex;
    m_pCursorEntry = m_pSavedEntry;
    m_iSubdirectoryCursorIndex = m_iSubdirectorySavedIndex;
    m_pSubdirectoryCursorEntry = m_pSubdirectorySavedEntry;
    return this;
}

// FUNCTION: LEMBALL 0x0045BFA0
void MOGLOAD_DirectoryNode::ReadEntryRecord(MOGLOAD_EntryRecord *pEntry) {
    long lNameOffset;

    /* On-disk records are fixed 36-byte entries whose name pointer is rebased into the name blob. */
    if (pEntry == 0 || g_pResourceArchiveFile == 0) {
        return;
    }

    SeekFile(g_pResourceArchiveFile, ((m_iCursorIndex * 4L) + 4L) * 9L + m_lRecordTableOffset, SEEK_SET);

    lNameOffset = (long)MOGLOAD_ReadU32();
    if (m_pNameData != 0) {
        pEntry->m_pszName = m_pNameData + (lNameOffset - m_lNameDataOffset);
    } else {
        pEntry->m_pszName = 0;
    }

    pEntry->m_uResourceId = MOGLOAD_ReadU32();
    pEntry->m_uTag = MOGLOAD_ReadU32();
    pEntry->m_lFileOffset = (long)MOGLOAD_ReadU32();
    pEntry->m_cbFileSize = (long)MOGLOAD_ReadU32();
    pEntry->m_iNextIndex = 0;
    pEntry->m_pNextEntry = 0;
    pEntry->m_iSavedIndex = 0;
    pEntry->m_pSavedEntry = 0;
    pEntry->m_pChildDirectory = 0;
    ReadFileBytes(g_pResourceArchiveFile, pEntry->m_adwDescriptor, sizeof(pEntry->m_adwDescriptor));
}

// FUNCTION: LEMBALL 0x0045C200
void MOGLOAD_DirectoryNode::FindNextResourceArchiveEntry(
    MOGLOAD_EntrySearchState *pState, unsigned int uTagFilter) {
    int fAtEnd;
    MOGLOAD_EntryRecord *pCursorSource;

    fAtEnd = 0;
    do {
        if (m_iCursorIndex == -1) {
            pCursorSource = m_pSavedEntry;
            m_iCursorIndex = m_iSavedIndex;
            m_pCursorEntry = pCursorSource;
        } else {
            if ((long)m_cEntries - m_iCursorIndex == 1) {
                fAtEnd = 1;
                break;
            }
            if (m_pCursorEntry->m_pNextEntry == 0) {
                AppendEntryAfterCursor();
            }
            pCursorSource = m_pCursorEntry->m_pNextEntry;
            m_iCursorIndex += 1;
            m_pCursorEntry = pCursorSource;
        }
    } while (uTagFilter != (unsigned int)-1 &&
             m_pCursorEntry->m_uTag != uTagFilter);

    if (!fAtEnd) {
        pState->m_iIndex = m_iCursorIndex;
        pState->m_pEntry = m_pCursorEntry;
        return;
    }

    pState->m_pEntry = 0;
}

// FUNCTION: LEMBALL 0x0045C2A0
void MOGLOAD_DirectoryNode::ResetResourceArchiveEntryCursor(
    MOGLOAD_EntrySearchState *pState, unsigned int uTagFilter) {
    m_iCursorIndex = m_iSavedIndex;
    m_pCursorEntry = m_pSavedEntry;
    m_iCursorIndex = -1;
    FindNextResourceArchiveEntry(pState, uTagFilter);
}

// FUNCTION: LEMBALL 0x0045C060
MOGLOAD_DirectoryNode *MOGLOAD_DirectoryNode::AdvanceSubdirectory(void) {
    MOGLOAD_DirectoryNode *pChildDirectory;
    MOGLOAD_EntryRecord *pEntry;
    MOGLOAD_EntrySearchState state;

    state.m_iIndex = 0;
    state.m_pEntry = 0;
    if (m_pSubdirectorySavedEntry == 0) {
        ResetResourceArchiveEntryCursor(&state, MOGLOAD_TAG_CRID);
        pEntry = state.m_pEntry;
        if (pEntry == 0) {
            m_iSubdirectoryCursorIndex = m_iSubdirectorySavedIndex;
            m_pSubdirectoryCursorEntry = m_pSubdirectorySavedEntry;
            return 0;
        }
        if (pEntry->m_uTag == MOGLOAD_TAG_CRID) {
            pChildDirectory = (MOGLOAD_DirectoryNode *)AllocateResourceArchiveMemory(0x38);
            if (pChildDirectory == 0) {
                pEntry->m_pChildDirectory = 0;
            } else {
                pEntry->m_pChildDirectory = pChildDirectory->Construct(pEntry->m_lFileOffset);
            }
            m_iSubdirectorySavedIndex = state.m_iIndex;
            m_pSubdirectorySavedEntry = pEntry;
        } else {
            m_pSubdirectorySavedEntry = 0;
        }
        m_iSubdirectoryCursorIndex = m_iSubdirectorySavedIndex;
        m_pSubdirectoryCursorEntry = m_pSubdirectorySavedEntry;
        if (m_pSubdirectoryCursorEntry == 0) {
            return 0;
        }
        return m_pSubdirectoryCursorEntry->m_pChildDirectory;
    }

    if (m_iSubdirectoryCursorIndex == -1) {
        m_iSubdirectoryCursorIndex = m_iSubdirectorySavedIndex;
        m_pSubdirectoryCursorEntry = m_pSubdirectorySavedEntry;
        return m_pSubdirectoryCursorEntry->m_pChildDirectory;
    }

    pEntry = m_pSubdirectoryCursorEntry;
    if (pEntry->m_pSavedEntry != 0) {
        m_iSubdirectoryCursorIndex = pEntry->m_iSavedIndex;
        m_pSubdirectoryCursorEntry = pEntry->m_pSavedEntry;
        return m_pSubdirectoryCursorEntry->m_pChildDirectory;
    }

    m_iCursorIndex = m_iSubdirectoryCursorIndex;
    m_pCursorEntry = m_pSubdirectoryCursorEntry;
    FindNextResourceArchiveEntry(&state, MOGLOAD_TAG_CRID);
    pEntry = state.m_pEntry;
    if (pEntry == 0) {
        return 0;
    }
    if (pEntry->m_uTag == MOGLOAD_TAG_CRID) {
        pChildDirectory = (MOGLOAD_DirectoryNode *)AllocateResourceArchiveMemory(0x38);
        if (pChildDirectory == 0) {
            pEntry->m_pChildDirectory = 0;
        } else {
            pEntry->m_pChildDirectory = pChildDirectory->Construct(pEntry->m_lFileOffset);
        }
        m_pSubdirectoryCursorEntry->m_iSavedIndex = state.m_iIndex;
        m_pSubdirectoryCursorEntry->m_pSavedEntry = pEntry;
        m_iSubdirectoryCursorIndex = state.m_iIndex;
        m_pSubdirectoryCursorEntry = pEntry;
        return m_pSubdirectoryCursorEntry->m_pChildDirectory;
    }

    m_pSubdirectoryCursorEntry->m_pSavedEntry = 0;
    return m_pSubdirectoryCursorEntry->m_pChildDirectory;
}

// FUNCTION: LEMBALL 0x0045BF10
void DestroyResourceArchiveDirectoryTree(int pDirectoryNode) {
    MOGLOAD_DirectoryNode *pDirectory;
    MOGLOAD_EntryRecord *pEntry;
    MOGLOAD_EntryRecord *pNextEntry;

    pDirectory = (MOGLOAD_DirectoryNode *)(unsigned long)pDirectoryNode;
    pDirectory->m_iCursorIndex = pDirectory->m_iSavedIndex;
    pDirectory->m_pCursorEntry = pDirectory->m_pSavedEntry;
    pEntry = pDirectory->m_pSavedEntry;
    while (pEntry != 0) {
        pNextEntry = pEntry->m_pNextEntry;
        if (pEntry->m_uTag == MOGLOAD_TAG_CRID && pEntry->m_pChildDirectory != 0) {
            FreeResourceArchiveMemory(pEntry->m_pChildDirectory);
            pEntry->m_pChildDirectory = 0;
        }
        FreeResourceArchiveMemory(pEntry);
        pEntry = pNextEntry;
    }
    if (pDirectory->m_pNameData != 0) {
        FreeResourceArchiveMemory(pDirectory->m_pNameData);
        pDirectory->m_pNameData = 0;
    }
}

// FUNCTION: LEMBALL 0x0045BAF0
void *AllocateResourceArchiveMemory(unsigned int cbBytes) {
    unsigned int pvBlock;

    if (g_pResourceArchiveMemoryArena == 0) {
        return AllocateVSMemBlock(cbBytes);
    }

    if (g_fSmallMemoryBucketTableEnabled != 0 && cbBytes < g_cbSmallMemoryBucketUpperBound) {
        pvBlock = (unsigned int)((SMALL_MEMORY_BUCKET_TABLE *)g_pSmallMemoryBucketTable)
            ->AllocateFromSmallMemoryBucketTable(cbBytes);
        if (pvBlock != 0) {
            return (void *)(unsigned long)pvBlock;
        }
    }

    pvBlock = 0;
    if (((MOGLOAD_ArenaDispatch *)g_pResourceArchiveMemoryArena)->Allocate(
            (void **)&pvBlock,
            cbBytes,
            g_pszSmallMemoryBucketAllocTag) == 0) {
        return 0;
    }
    return (void *)(unsigned long)pvBlock;
}

// FUNCTION: LEMBALL 0x0045BB70
void FreeResourceArchiveMemory(void *pMemoryBlock) {
    if (g_pResourceArchiveMemoryArena == 0) {
        FreeVSMemBlock(pMemoryBlock);
        return;
    }

    if (g_fSmallMemoryBucketTableEnabled != 0 &&
        ((SMALL_MEMORY_BUCKET_TABLE *)g_pSmallMemoryBucketTable)
            ->FreeToSmallMemoryBucketTable(pMemoryBlock) != 0) {
        return;
    }

    ((MOGLOAD_ArenaDispatch *)g_pResourceArchiveMemoryArena)->Free(pMemoryBlock);
}

// FUNCTION: LEMBALL 0x0045CF10
void FreeResourceObjectDataBuffer(unsigned int pBuffer) {
    FreeResourceArchiveMemory((void *)(unsigned long)pBuffer);
}

// FUNCTION: LEMBALL 0x0045BBC0
int OpenResourceArchiveFileHandle(const char *pszPath, const char *pszMode) {
    g_pResourceArchiveFile = OpenFileWithMode(pszPath, pszMode);
    return 1 - (g_pResourceArchiveFile == 0);
}

// FUNCTION: LEMBALL 0x0045CA30
unsigned int AllocateResourceDataBufferWithEviction(void *pArchive, unsigned int cbBuffer) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    unsigned int pBuffer;
    int cbShortfall;
    unsigned int cbMinimumEvict;
    int iSlot;
    int cRemaining;
    MOGLOAD_StringResourceObject **ppCachedObjects;
    MOGLOAD_TypedResourceObjectVtable *pVtable;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    while ((pBuffer = (unsigned int)(unsigned long)AllocateResourceArchiveMemory(cbBuffer)) == 0) {
        cbShortfall = (int)cbBuffer - (int)GetMemoryArenaPayloadByteCounter(g_pResourceArchiveMemoryArena);
        cbMinimumEvict = cbShortfall < 0 ? cbBuffer : (unsigned int)cbShortfall;
        iSlot = FindResourceCacheEvictionCandidateIndex(pResourceArchive, cbMinimumEvict);
        if (iSlot == -1) {
            iSlot = 0;
            cRemaining = pResourceArchive->m_cCachedResourceObjects;
            if (cRemaining > 0) {
                ppCachedObjects = pResourceArchive->m_ppCachedResourceObjects;
                do {
                    if (ppCachedObjects[iSlot] == 0) {
                        ppCachedObjects += iSlot;
                        do {
                            ++ppCachedObjects;
                            ++iSlot;
                        } while (*ppCachedObjects == 0);
                        ppCachedObjects -= iSlot;
                    }
                    ++iSlot;
                    --cRemaining;
                } while (cRemaining != 0);
            }
        } else {
            pVtable = (MOGLOAD_TypedResourceObjectVtable *)pResourceArchive->m_ppCachedResourceObjects[iSlot]->m_pVtable;
            pVtable->m_pReleaseIfLoaded(
                pResourceArchive->m_ppCachedResourceObjects[iSlot], 0, 1);
        }
    }
    return pBuffer;
}

// FUNCTION: LEMBALL 0x0045D0A0
void LEMBALL_FASTCALL CopyBufferIntoTypedResourceObjectAndParse(
    void *pObject,
    void *pUnusedEdx,
    unsigned int *pSource,
    unsigned int nUnused,
    unsigned int cbBuffer) {
    MOGLOAD_StringResourceObject *pResourceObject =
        (MOGLOAD_StringResourceObject *)pObject;
    unsigned int *pTarget;
    unsigned int i;

    (void)pUnusedEdx;
    (void)nUnused;
    if (cbBuffer != 0 && pResourceObject->m_pszText38 == 0) {
        pTarget = (unsigned int *)AllocateResourceDataBufferWithEviction(
            g_pMainResourceArchive, cbBuffer);
        pResourceObject->m_pszText38 = (char *)pTarget;
        for (i = cbBuffer >> 2; i != 0; --i) {
            *pTarget++ = *pSource++;
        }
        for (i = cbBuffer & 3; i != 0; --i) {
            *(unsigned char *)pTarget++ = *(unsigned char *)pSource++;
        }
    }
    if (pResourceObject->m_padwResourceDescriptor34 != 0) {
        pResourceObject->m_nLoadState10 = 1;
        ((MOGLOAD_TypedResourceObjectVtable *)pResourceObject->m_pVtable)
            ->m_pOnLoad(pResourceObject);
    }
}

// FUNCTION: LEMBALL 0x0045E630
void ParseIntResourceDescriptor(void) {
}

// FUNCTION: LEMBALL 0x0045D1C0
void LEMBALL_FASTCALL UnloadTypedResourceObject(void *pObject,
                                                void *pUnusedEcx,
                                                int fReleaseMode) {
    MOGLOAD_StringResourceObject *pResourceObject;
    MOGLOAD_TypedResourceObjectVtable *pVtable;

    (void)pUnusedEcx;
    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    if (pResourceObject->m_nLoadState10 != 0 &&
        pResourceObject->m_nResourceId30 != 0 &&
        pResourceObject->m_cbResourceData28 != 0) {
        FreeResourceObjectDataBuffer((unsigned int)(unsigned long)pResourceObject->m_pszText38);
        pResourceObject->m_pszText38 = 0;
    }
    pVtable = (MOGLOAD_TypedResourceObjectVtable *)pResourceObject->m_pVtable;
    pVtable->m_pOnRelease(pResourceObject, 0, fReleaseMode);
    if (pResourceObject->m_nLoadState10 != 0) {
        pResourceObject->m_nLoadState10 = 0;
        pVtable->m_pAfterLoad(pResourceObject);
    }
}

// FUNCTION: LEMBALL 0x0045C940
int FindResourceCacheEvictionCandidateIndex(void *pArchive, unsigned int cbMinimumEvict) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    int iSlot;
    unsigned int nBestAge;
    int cRemaining;
    int nBestSlot;
    unsigned int cbBestSize;
    MOGLOAD_StringResourceObject **ppCachedObjects;
    MOGLOAD_StringResourceObject *pObject;
    MOGLOAD_TypedResourceObjectVtable *pVtable;
    unsigned int cbObjectSize;
    unsigned int nObjectAge;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    iSlot = 0;
    nBestAge = 0xffffffff;
    cRemaining = 0;
    nBestSlot = -1;
    cbBestSize = 0;
    if (0 < pResourceArchive->m_cCachedResourceObjects) {
        ppCachedObjects = pResourceArchive->m_ppCachedResourceObjects;
        do {
            pObject = ppCachedObjects[iSlot];
            while (pObject == 0) {
                ++iSlot;
                pObject = ppCachedObjects[iSlot];
            }
            if (pObject->m_nLoadState10 != 0 && pObject->m_nLockCount08 == 0) {
                pVtable = (MOGLOAD_TypedResourceObjectVtable *)pObject->m_pVtable;
                cbObjectSize = (unsigned int)pVtable->m_pGetMemorySize(pObject);
                nObjectAge = (unsigned int)pObject->m_cReferences;
                if (cbObjectSize < cbMinimumEvict) {
                    if (nBestAge <= nObjectAge) {
                        goto next_object;
                    }
                } else if (cbObjectSize <= cbBestSize && nBestAge <= nObjectAge) {
                    goto next_object;
                }

                    nBestAge = nObjectAge;
                    cbBestSize = cbObjectSize;
                    nBestSlot = iSlot;
            }
next_object:
            ++iSlot;
            ++cRemaining;
        } while (cRemaining < pResourceArchive->m_cCachedResourceObjects);
    }
    return nBestSlot;
}

// FUNCTION: LEMBALL 0x0045C9D0
int LEMBALL_FASTCALL FindReusableResourceCacheSlotIndex(MOGLOAD_ResourceArchive *pArchive) {
    int nResult;
    int iSlot;
    MOGLOAD_StringResourceObject **ppCachedObjects;

    nResult = -1;
    iSlot = 0;
    if (pArchive->m_cCachedResourceObjects <= 0) {
        nResult = 0;
    } else {
        ppCachedObjects = pArchive->m_ppCachedResourceObjects;
        if (pArchive->m_cCachedResourceObjects < 0x400) {
            do {
                if (*ppCachedObjects == 0) {
                    break;
                }
                ++ppCachedObjects;
                ++iSlot;
            } while (iSlot < 0x400);
        } else {
            do {
                if ((*ppCachedObjects)->m_cReferences == 0) {
                    break;
                }
                ++ppCachedObjects;
                ++iSlot;
            } while (iSlot < 0x400);
        }
        if (iSlot < 0x400) {
            return iSlot;
        }
    }
    return nResult;
}

// FUNCTION: LEMBALL 0x0045CB50
int LEMBALL_STDCALL AttachResourceEntryToObject(
    MOGLOAD_StringResourceObject *pObject, int iEntryIndex, MOGLOAD_EntryRecord *pEntry) {
    (void)iEntryIndex;
    if ((int)pEntry->m_uTag != (int)pObject->m_uTypeTag) {
        return 0;
    }

    pObject->m_cbResourceData28 = (int)pEntry->m_cbFileSize;
    pObject->m_lResourceOffset2C = (int)pEntry->m_lFileOffset;
    pObject->m_padwResourceDescriptor34 = pEntry->m_adwDescriptor;
    return 1;
}

// FUNCTION: LEMBALL 0x0045C2D0
void MOGLOAD_DirectoryNode::FindResourceArchiveEntryByIdRecursive(
    MOGLOAD_EntrySearchState *pState, int nResourceId, int fAllowRecursion) {
    MOGLOAD_DirectoryNode *pChildDirectory;
    long iSavedSubdirectoryCursorIndex;
    MOGLOAD_EntryRecord *pSavedSubdirectoryCursorEntry;

    (void)fAllowRecursion;
    ResetResourceArchiveEntryCursor(pState, (unsigned int)-1);
    while (pState->m_pEntry != 0 && (int)pState->m_pEntry->m_uResourceId != nResourceId) {
        FindNextResourceArchiveEntry(pState, (unsigned int)-1);
    }

    if (pState->m_pEntry == 0) {
        iSavedSubdirectoryCursorIndex = m_iSubdirectoryCursorIndex;
        pSavedSubdirectoryCursorEntry = m_pSubdirectoryCursorEntry;
        m_iSubdirectoryCursorIndex = m_iSubdirectorySavedIndex;
        m_pSubdirectoryCursorEntry = m_pSubdirectorySavedEntry;
        m_iSubdirectoryCursorIndex = -1;
        while (pState->m_pEntry == 0) {
            pChildDirectory = AdvanceSubdirectory();
            if (pChildDirectory == 0) {
                break;
            }
            pChildDirectory->FindResourceArchiveEntryByIdRecursive(pState, nResourceId, fAllowRecursion);
        }
        m_iSubdirectoryCursorIndex = iSavedSubdirectoryCursorIndex;
        m_pSubdirectoryCursorEntry = pSavedSubdirectoryCursorEntry;
        return;
    }
}

// FUNCTION: LEMBALL 0x0045BD50
int ResourceArchiveComponentEquals(char *pszEntryName, char *pszPathComponent) {
    int fEqual;
    char chEntry;

    fEqual = 0;
    do {
        chEntry = (char)toupper((unsigned char)*pszEntryName);
        if (chEntry == '/') {
            chEntry = '\0';
        }
        if ((char)toupper((unsigned char)*pszPathComponent) != chEntry) {
            return fEqual;
        }
        if (*pszEntryName == '\0') {
            fEqual = 1;
        }
        ++pszEntryName;
        ++pszPathComponent;
    } while (fEqual == 0);
    return fEqual;
}

// FUNCTION: LEMBALL 0x0045C810
int SelectResourceArchiveDirectoryPath(void *pArchive, const char *pszPath) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    char *pszNewPath;
    char *pszFoundSlash;
    char *pszOldPath;
    char *pszCopyDestination;
    unsigned int cchPath;
    MOGLOAD_DirectoryNode *pCurrentDirectory;
    MOGLOAD_DirectoryNode *pDirectory;
    MOGLOAD_DirectoryNode *pMatchedDirectory;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    if (*pszPath == '/') {
        pResourceArchive->m_pCurrentDirectory = pResourceArchive->m_pRootDirectory;
        cchPath = (unsigned int)strlen(pszPath) + 1;
        pszNewPath = (char *)AllocateResourceArchiveMemory(cchPath);
        pszCopyDestination = pszNewPath;
    } else {
        cchPath = (unsigned int)strlen(pszPath) + 2;
        pszNewPath = (char *)AllocateResourceArchiveMemory(cchPath);
        if (pszNewPath != 0) {
            *pszNewPath = '/';
        }
        pszCopyDestination = pszNewPath + 1;
    }

    if (pszNewPath == 0) {
        return 0;
    }

    if (*pszPath == '/') {
        memcpy(pszCopyDestination, pszPath, cchPath);
    } else {
        pszNewPath[0] = '/';
        memcpy(pszCopyDestination, pszPath, cchPath - 1);
    }

    pCurrentDirectory = pResourceArchive->m_pCurrentDirectory;
    pCurrentDirectory->m_iSubdirectoryCursorIndex = pCurrentDirectory->m_iSubdirectorySavedIndex;
    pCurrentDirectory->m_pSubdirectoryCursorEntry = pCurrentDirectory->m_pSubdirectorySavedEntry;
    pCurrentDirectory->m_iSubdirectoryCursorIndex = -1;

    pszFoundSlash = pszNewPath;
    for (;;) {
        pszFoundSlash = strchr(pszFoundSlash, '/');
        if (pszFoundSlash == 0) {
            break;
        }
        pszFoundSlash += 1;
        if (*pszFoundSlash != '\0') {
            do {
                pDirectory = pResourceArchive->m_pCurrentDirectory->AdvanceSubdirectory();
                if (pDirectory == 0) {
                    goto bad_path;
                }
            } while (!ResourceArchiveComponentEquals(
                pResourceArchive->m_pCurrentDirectory->m_pSubdirectoryCursorEntry->m_pszName,
                                                     pszFoundSlash));
            pMatchedDirectory = pDirectory;
            if (pMatchedDirectory == 0) {
                break;
            }
            pResourceArchive->m_pCurrentDirectory = pMatchedDirectory;
        }
    }

    pszOldPath = pResourceArchive->m_pszSelectedPath;
    if (pszOldPath != 0) {
        FreeResourceArchiveMemory(pszOldPath);
        pResourceArchive->m_pszSelectedPath = 0;
    }
    pResourceArchive->m_pszSelectedPath = pszNewPath;
    return 1;

bad_path:
    pszOldPath = pResourceArchive->m_pszSelectedPath;
    SelectResourceArchiveDirectoryPath(pResourceArchive, pszOldPath);
    if (pszNewPath != 0) {
        FreeResourceArchiveMemory(pszNewPath);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045C630
void *MOGLOAD_ResourceArchive::ConstructResourceArchive(
    const char *pszArchiveName, unsigned int cbArenaSize) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    void *pChildArena;
    GAME_StatusEntry *pStatusEntry;
    int nOffset;

    pResourceArchive = this;
    *(char *)g_MOGLOAD_RootPath = '/';
    g_pMainResourceArchive = this;
    pResourceArchive->m_nOpenFailed = 0;
    pResourceArchive->m_ppCachedResourceObjects = 0;
    pResourceArchive->m_pszSelectedPath = 0;
    pResourceArchive->m_pRootDirectory = 0;
    pResourceArchive->m_pCurrentDirectory = 0;
    pResourceArchive->m_cCachedResourceObjects = 0;
    pResourceArchive->m_fSkipPruneOnDestroy = 0;
    pResourceArchive->m_cbArena = cbArenaSize;

    if (((CArena *)g_pMainMemoryArena)->AllocateChildMemoryArena(
            &pChildArena, cbArenaSize, g_MOGLOAD_StatusName) == 1) {
        pResourceArchive->m_fExternalArena = 0;
    }
    g_pResourceArchiveMemoryArena = pChildArena;

    if (!OpenResourceArchiveFileHandle(pszArchiveName, g_MOGLOAD_OpenMode)) {
        pResourceArchive->m_nOpenFailed = 1;
        return this;
    }

    timeGetTime();
    pResourceArchive->m_pRootDirectory = (MOGLOAD_DirectoryNode *)AllocateResourceArchiveMemory(0x38);
    if (pResourceArchive->m_pRootDirectory == 0) {
        pResourceArchive->m_pRootDirectory = 0;
    } else {
        pResourceArchive->m_pRootDirectory =
            pResourceArchive->m_pRootDirectory->Construct(0);
    }
    timeGetTime();
    pResourceArchive->m_pCurrentDirectory = pResourceArchive->m_pRootDirectory;
    SelectResourceArchiveDirectoryPath(pResourceArchive, g_MOGLOAD_RootPath);

    pResourceArchive->m_ppCachedResourceObjects =
        (MOGLOAD_StringResourceObject **)AllocateResourceArchiveMemory(0x1000);
    nOffset = 0;
    do {
        nOffset += 4;
        *(void **)((char *)pResourceArchive->m_ppCachedResourceObjects - 4 + nOffset) = 0;
    } while (nOffset < 0x1000);

    pStatusEntry = (GAME_StatusEntry *)AllocateVSMemBlock(0x20);
    if (pStatusEntry == 0) {
        pStatusEntry = 0;
    } else {
        pStatusEntry = new (pStatusEntry) GAME_StatusEntry(g_MOGLOAD_StatusName);
        pStatusEntry->m_pVtable = g_MOGLOAD_MemoryArenaStatusEntryVtable;
    }
    ((VSINIT_StatusEntryPointerArray *)g_pStatusEntryRegistry)
        ->AppendStatusEntry(pStatusEntry);
    ((MOGLOAD_ResourceArenaStatusOwner *)g_pResourceArchiveMemoryArena)->m_pStatusEntry = pStatusEntry;

    return this;
}

// FUNCTION: LEMBALL 0x0045C770
void LEMBALL_FASTCALL DestroyResourceArchive(void *pArchive) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    unsigned int pRootDirectory;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    if (g_pResourceArchiveFile != 0) {
        CloseCrtFilePointer(g_pResourceArchiveFile);
    }
    AreAllCachedResourceObjectsUnreferenced(pResourceArchive);
    if (pResourceArchive->m_fSkipPruneOnDestroy == 0) {
        PruneUnreferencedCachedResourceObjects(pResourceArchive);
    }
    if (pResourceArchive->m_ppCachedResourceObjects != 0) {
        FreeResourceArchiveMemory(pResourceArchive->m_ppCachedResourceObjects);
        pResourceArchive->m_ppCachedResourceObjects = 0;
    }
    pRootDirectory = (unsigned int)(unsigned long)pResourceArchive->m_pRootDirectory;
    if (pRootDirectory != 0) {
        DestroyResourceArchiveDirectoryTree((int)pRootDirectory);
        FreeResourceArchiveMemory((void *)(unsigned long)pRootDirectory);
        pResourceArchive->m_pRootDirectory = 0;
    }
    if (pResourceArchive->m_pszSelectedPath != 0) {
        FreeResourceArchiveMemory(pResourceArchive->m_pszSelectedPath);
        pResourceArchive->m_pszSelectedPath = 0;
    }
    if (pResourceArchive->m_fExternalArena == 0) {
        ((CArena *)g_pMainMemoryArena)->ReleaseChildMemoryArena(g_pResourceArchiveMemoryArena);
    }
    g_pResourceArchiveMemoryArena = 0;
}

// FUNCTION: LEMBALL 0x0045CAB0
void *FindCachedResourceObjectById(void *pArchive, int nResourceId) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    int iSlot;
    int cRemaining;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    iSlot = 0;
    cRemaining = pResourceArchive->m_cCachedResourceObjects;
    if (cRemaining > 0) {
        do {
            while (pResourceArchive->m_ppCachedResourceObjects[iSlot] == 0) {
                ++iSlot;
            }
            if (pResourceArchive->m_ppCachedResourceObjects[iSlot]->m_nResourceId30 == nResourceId) {
                break;
            }
            --cRemaining;
            ++iSlot;
        } while (cRemaining > 0);
    }

    if (pResourceArchive->m_cCachedResourceObjects != 0 && cRemaining > 0) {
        AdvanceCachedResourceObjectFrameCounters(pResourceArchive);
        ((MOGLOAD_TypedResourceObjectVtable *)
             pResourceArchive->m_ppCachedResourceObjects[iSlot]->m_pVtable)
            ->m_pEnsureLoaded(pResourceArchive->m_ppCachedResourceObjects[iSlot]);
        ++pResourceArchive->m_ppCachedResourceObjects[iSlot]->m_cReferences;
        return pResourceArchive->m_ppCachedResourceObjects[iSlot];
    }

    return 0;
}

// FUNCTION: LEMBALL 0x0045CDB0
void AdvanceCachedResourceObjectFrameCounters(void *pArchive) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    MOGLOAD_StringResourceObject *pResourceObject;
    int iCount;
    int iSlot;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    iCount = 0;
    iSlot = 0;
    if (pResourceArchive == 0 || pResourceArchive->m_cCachedResourceObjects <= 0) {
        return;
    }

    while (iCount < pResourceArchive->m_cCachedResourceObjects) {
        while (pResourceArchive->m_ppCachedResourceObjects[iSlot] == 0) {
            ++iSlot;
        }

        pResourceObject = pResourceArchive->m_ppCachedResourceObjects[iSlot];
        if (pResourceObject->m_nLoadState10 != 0 ||
            ((MOGLOAD_TypedResourceObjectVtable *)pResourceObject->m_pVtable)
                    ->m_pGetFrameAge14(pResourceObject) != 0) {
            ++pResourceObject->m_nReserved24;
        }

        ++iCount;
        ++iSlot;
    }
}

// FUNCTION: LEMBALL 0x0045CEB0
void MOGLOAD_ResourceArchive::RemoveCachedResourceObject(void *pResourceObject) {
    int iFound;
    int iSlot;

    iFound = 0;
    iSlot = 0;
    if (m_cCachedResourceObjects > 0) {
        do {
            if (m_ppCachedResourceObjects[iSlot] == 0) {
                do {
                    ++iSlot;
                } while (m_ppCachedResourceObjects[iSlot] == 0);
            }
            if (m_ppCachedResourceObjects[iSlot] == pResourceObject) {
                m_ppCachedResourceObjects[iSlot] = 0;
                break;
            }
            ++iFound;
            ++iSlot;
        } while (iFound < m_cCachedResourceObjects);
    }

    if (iFound != m_cCachedResourceObjects) {
        --m_cCachedResourceObjects;
    }
}

// FUNCTION: LEMBALL 0x0045CD60
int AreAllCachedResourceObjectsUnreferenced(void *pArchive) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    int cRemaining;
    int iSlot;
    int fFoundReferencedObject;
    MOGLOAD_StringResourceObject **ppCachedObjects;
    MOGLOAD_StringResourceObject *pResourceObject;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    iSlot = 0;
    fFoundReferencedObject = 0;
    cRemaining = pResourceArchive->m_cCachedResourceObjects;
    if (cRemaining != 0) {
        ppCachedObjects = pResourceArchive->m_ppCachedResourceObjects;
        do {
            pResourceObject = ppCachedObjects[iSlot];
            while (pResourceObject == 0 && iSlot < 0x400) {
                ++iSlot;
                pResourceObject = ppCachedObjects[iSlot];
            }
            if (ppCachedObjects[iSlot]->m_cReferences != 0) {
                fFoundReferencedObject = 1;
            }
            ++iSlot;
            --cRemaining;
        } while (cRemaining != 0);
    }
    return fFoundReferencedObject == 0;
}

// FUNCTION: LEMBALL 0x0045CE50
void PruneUnreferencedCachedResourceObjects(void *pArchive) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    int cCachedObjects;
    int iProcessed;
    int iSlot;
    int *pnObjectSlot;
    MOGLOAD_StringResourceObject *pResourceObject;
    MOGLOAD_StringResourceObject **ppCachedObjects;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    cCachedObjects = pResourceArchive->m_cCachedResourceObjects;
    iSlot = 0;
    iProcessed = 0;
    if (cCachedObjects > 0) {
        do {
            ppCachedObjects = pResourceArchive->m_ppCachedResourceObjects;
            if (ppCachedObjects[iSlot] == 0) {
                pnObjectSlot = (int *)&ppCachedObjects[iSlot];
                do {
                    ++pnObjectSlot;
                    ++iSlot;
                } while (*pnObjectSlot == 0);
            }
            if (iSlot == 0x400) {
                return;
            }
            pResourceObject = ppCachedObjects[iSlot];
            if (pResourceObject->m_cReferences == 0) {
                if (pResourceObject != 0) {
                    ((MOGLOAD_DeleteResourceProc)pResourceObject->m_pVtable[0])(
                        pResourceObject, 0, 1);
                }
                pResourceArchive->m_ppCachedResourceObjects[iSlot] = 0;
                --pResourceArchive->m_cCachedResourceObjects;
            }
            ++iProcessed;
            ++iSlot;
        } while (iProcessed < cCachedObjects);
    }
}

// FUNCTION: LEMBALL 0x0045CF20
void InitializeResourceObjectFromId(void *pObject, int nResourceId) {
    MOGLOAD_StringResourceObject *pResourceObject;
    MOGLOAD_TypedResourceObjectVtable *pVtable;

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    ResetTypedResourceObjectState(pResourceObject);
    if (((MOGLOAD_ResourceArchive *)g_pMainResourceArchive)
            ->LoadResourceObjectById(nResourceId, pResourceObject, 1) != 0) {
        pResourceObject->m_nResourceId30 = nResourceId;
        pVtable = (MOGLOAD_TypedResourceObjectVtable *)pResourceObject->m_pVtable;
        pVtable->m_pConstructFromArchiveEntry(pResourceObject);
        pVtable->m_pEnsureLoaded(pResourceObject);
        ++pResourceObject->m_cReferences;
        return;
    }
    pResourceObject->m_nResultCode44 = 1;
}

// FUNCTION: LEMBALL 0x0045D050
void ResetTypedResourceObjectState(void *pObject) {
    MOGLOAD_StringResourceObject *pResourceObject;
    MOGLOAD_TypedResourceObjectVtable *pVtable;

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    pVtable = (MOGLOAD_TypedResourceObjectVtable *)pResourceObject->m_pVtable;
    pResourceObject->m_nReserved04 = 0;
    pResourceObject->m_nLockCount08 = 0;
    pResourceObject->m_cReferences = 0;
    pResourceObject->m_nLoadState10 = 0;
    pResourceObject->m_nReserved14 = 0;
    pResourceObject->m_cbResourceData28 = 0;
    pResourceObject->m_lResourceOffset2C = 0;
    pResourceObject->m_nResourceId30 = 0;
    pResourceObject->m_padwResourceDescriptor34 = 0;
    pResourceObject->m_pszText38 = 0;
    pResourceObject->m_nReserved3C = 0;
    pResourceObject->m_uTypeTag = 0;
    pResourceObject->m_nResultCode44 = 0;
    pVtable->m_pClearTypeTag(pResourceObject);
    AdvanceCachedResourceObjectFrameCounters(g_pMainResourceArchive);
    pResourceObject->m_nReserved24 = 0;
}

// FUNCTION: LEMBALL 0x0045D250
void *FinalizeLoadedResourceObjectResult(void *pObject) {
    MOGLOAD_StringResourceObject *pResourceObject;

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    if (pResourceObject->m_nResultCode44 == 1) {
        ((MOGLOAD_ResourceArchive *)g_pMainResourceArchive)
            ->RemoveCachedResourceObject(pResourceObject);
        if (pResourceObject != 0) {
            ((MOGLOAD_DeleteResourceProc)pResourceObject->m_pVtable[0])(
                pResourceObject, 0, 1);
        }
        return 0;
    }
    if (pResourceObject->m_nResultCode44 == 2) {
        return 0;
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045CB80
int MOGLOAD_ResourceArchive::LoadResourceObjectById(int nResourceId, void *pObject, int fCacheObject) {
    int iSlot;
    MOGLOAD_StringResourceObject *pCachedObject;
    MOGLOAD_EntrySearchState state;

    m_pCurrentDirectory->FindResourceArchiveEntryByIdRecursive(&state, nResourceId, fCacheObject);
    if (state.m_pEntry == 0) {
        return 0;
    }

    iSlot = FindReusableResourceCacheSlotIndex(this);
    pCachedObject = m_ppCachedResourceObjects[iSlot];
    if (pCachedObject != 0) {
        ((MOGLOAD_DeleteResourceProc)pCachedObject->m_pVtable[0])(
            pCachedObject, 0, 1);
        m_ppCachedResourceObjects[iSlot] = 0;
        --m_cCachedResourceObjects;
    }

    m_ppCachedResourceObjects[iSlot] = (MOGLOAD_StringResourceObject *)pObject;
    ++m_cCachedResourceObjects;
    return AttachResourceEntryToObject((MOGLOAD_StringResourceObject *)pObject,
                                       (int)state.m_iIndex,
                                       state.m_pEntry);
}

// FUNCTION: LEMBALL 0x0045CE00
int MOGLOAD_ResourceArchive::LoadResourceArchiveEntryDataIntoBuffer(
    int *plFileOffset,
    unsigned int *pcbBuffer,
    void *pUnused) {
    unsigned int pBuffer;

    (void)pUnused;
    pBuffer = AllocateResourceDataBufferWithEviction(this,
                                                     (unsigned int)plFileOffset[1]);
    *pcbBuffer = pBuffer;
    SeekFile(g_pResourceArchiveFile, (long)(plFileOffset[0] + 8), SEEK_SET);
    ReadFileBytes(g_pResourceArchiveFile, (void *)(unsigned long)*pcbBuffer, (unsigned int)plFileOffset[1]);
    return 1;
}

// FUNCTION: LEMBALL 0x0045DE00
MOGLOAD_StringResourceObject *LoadStringResource(int nResourceId) {
    MOGLOAD_StringResourceObject *pResourceObject;
    MOGLOAD_StringResourceObject *pLoadedObject;

    pResourceObject = (MOGLOAD_StringResourceObject *)FindCachedResourceObjectById(g_pMainResourceArchive, nResourceId);
    if (pResourceObject != 0) {
        if (pResourceObject->m_uTypeTag != g_MOGLOAD_StringResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (MOGLOAD_StringResourceObject *)AllocateVSMemBlock(0x48);
    if (pResourceObject != 0) {
        pResourceObject->m_pVtable = (void **)g_pCachedResourceObjectBaseDeleteVtable;
        pResourceObject->m_nReserved18 = 0;
        pResourceObject->m_pVtable = (void **)g_pStringResourceVtable;
        InitializeResourceObjectFromId(pResourceObject, nResourceId);
        pLoadedObject = (MOGLOAD_StringResourceObject *)FinalizeLoadedResourceObjectResult(pResourceObject);
        return pLoadedObject;
    }

    pLoadedObject = (MOGLOAD_StringResourceObject *)FinalizeLoadedResourceObjectResult(0);
    return pLoadedObject;
}

// FUNCTION: LEMBALL 0x0045E380
void *LoadEffResource(int nResourceId) {
    MOGLOAD_StringResourceObject *pResourceObject;
    MOGLOAD_StringResourceObject *pLoadedObject;

    pResourceObject = (MOGLOAD_StringResourceObject *)FindCachedResourceObjectById(g_pMainResourceArchive, nResourceId);
    if (pResourceObject != 0) {
        if (pResourceObject->m_uTypeTag != g_MOGLOAD_EffResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            pResourceObject = 0;
        }
        return pResourceObject;
    }

    pResourceObject = (MOGLOAD_StringResourceObject *)AllocateVSMemBlock(0x48);
    if (pResourceObject != 0) {
        pResourceObject->m_pVtable = (void **)g_pCachedResourceObjectBaseDeleteVtable;
        pResourceObject->m_nReserved18 = 0;
        pResourceObject->m_pVtable = (void **)g_pDestroyEffResourceAndFreeThunk;
        InitializeResourceObjectFromId(pResourceObject, nResourceId);
        pLoadedObject = (MOGLOAD_StringResourceObject *)FinalizeLoadedResourceObjectResult(pResourceObject);
        return pLoadedObject;
    }

    pLoadedObject = (MOGLOAD_StringResourceObject *)FinalizeLoadedResourceObjectResult(0);
    return pLoadedObject;
}

struct MOGLOAD_ResourceObjectDeleteInterface {
    virtual void Delete(int nDeleteFlag) = 0;
};

struct MOGLOAD_ResourceObjectReferenceInterface {
    virtual void Reserved0(int) = 0;
    virtual void Reserved1(int) = 0;
    virtual void Reserved2(int) = 0;
    virtual void Reserved3(int) = 0;
    virtual void Reserved4(int) = 0;
    virtual void Reserved5(int) = 0;
    virtual void Reserved6(int) = 0;
    virtual void Reserved7(int) = 0;
    virtual void ReleaseReference(int nReleaseFlag) = 0;
};

// FUNCTION: LEMBALL 0x0045D180
void LEMBALL_FASTCALL ReleaseTypedResourceObjectReference(void *pResourceObject) {
    MOGLOAD_StringResourceObject *pObject;

    pObject = (MOGLOAD_StringResourceObject *)pResourceObject;
    --pObject->m_cReferences;
    if (pObject->m_cReferences == 0) {
        ((MOGLOAD_ResourceObjectReferenceInterface *)pObject)
            ->ReleaseReference(1);
        if (((MOGLOAD_ResourceArchive *)g_pMainResourceArchive)->m_fSkipPruneOnDestroy != 0) {
            if (pObject->m_nResourceId30 != 0) {
                ((MOGLOAD_ResourceArchive *)g_pMainResourceArchive)
                    ->RemoveCachedResourceObject(pObject);
            }
            if (pObject != 0) {
                ((MOGLOAD_ResourceObjectDeleteInterface *)pObject)->Delete(1);
            }
        }
    }
}
