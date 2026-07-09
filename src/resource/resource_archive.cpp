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

struct MOGLOAD_TypedResourceObjectVtable {
    void *(*m_pDelete)(void *pObject, int fDelete);
    void (*m_pAssignLoadedBuffer)(unsigned int cbBuffer, int *pBufferField, int nResourceOffset);
    void (*m_pConstructFromArchiveEntry)(void);
    int (*m_pGetFrameAge14)(void *pObject);
    int (*m_pGetArchiveEntryField1C)(void *pObject);
    int (*m_pGetFrameAgeMirror14)(void *pObject);
    int (*m_pReturnZero)(void);
    void (*m_pEnsureLoaded)(void *pObject);
    void (*m_pReleaseIfLoaded)(void *pObject, int fReleaseMode);
    void (*m_pOnRelease)(void);
    void *(*m_pGetResourceDataPointer)(void *pObject);
    void (*m_pOnLoad)(void);
    void (*m_pAfterLoad)(void);
    void (*m_pClearTypeTag)(void *pObject);
    int (*m_pGetMemorySize)(void *pObject);
};

void *DeleteCachedResourceObjectBase(void *pObject, int fDelete);
void InitializeTypedResourceObjectBaseVtable(void *pObject);
void EnsureTypedResourceObjectLoaded(void *pObject);
void ReleaseTypedResourceObjectIfLoaded(void *pObject, int fReleaseMode);
int GetField14NeedsAgeIncrement(void *pObject);
int GetField1CVfunc04(void *pObject);
int GetField14Vfunc05(void *pObject);
int ReturnZeroVfunc06(void);
void NoopVfunc09(void);
void *GetEffResourceDataPointer(void *pObject);
void NoopOnLoadVfunc11(void);
void NoopVfunc12(void);
void ClearResourceTypeTag(void *pObject);
int GetField28GetMemorySize(void *pObject);
void *DeleteTypedResourceObject(void *pObject, int fDelete);
void *DestroyEffResource(void *pObject, int fDelete);
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
    0,
    0,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)ReleaseTypedResourceObjectIfLoaded,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)ClearResourceTypeTag,
    (void *)GetField28GetMemorySize,
};
static void *g_MOGLOAD_TypedResourceObjectVtable[15] = {
    (void *)DeleteTypedResourceObject,
    0,
    0,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)ReleaseTypedResourceObjectIfLoaded,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)ClearResourceTypeTag,
    (void *)GetField28GetMemorySize,
};
static void *g_MOGLOAD_EffResourceObjectVtable[15] = {
    (void *)DestroyEffResource,
    0,
    0,
    (void *)GetField14NeedsAgeIncrement,
    (void *)GetField1CVfunc04,
    (void *)GetField14Vfunc05,
    (void *)ReturnZeroVfunc06,
    (void *)EnsureTypedResourceObjectLoaded,
    (void *)ReleaseTypedResourceObjectIfLoaded,
    (void *)NoopVfunc09,
    (void *)GetEffResourceDataPointer,
    (void *)NoopOnLoadVfunc11,
    (void *)NoopVfunc12,
    (void *)ClearResourceTypeTag,
    (void *)GetField28GetMemorySize,
};

extern void *g_pMainResourceArchive;
extern void *FinalizeLoadedResourceObjectResult(void *pObject);
extern void ResetTypedResourceObjectState(void *pObject);
extern int LoadResourceObjectById(void *pArchive, int nResourceId, int pObject, int fCacheObject);
extern void AdvanceCachedResourceObjectFrameCounters(void *pArchive);
extern int AllocateFromSmallMemoryBucketTable(void *pBucketTable, unsigned int cbBlock);
extern int FreeToSmallMemoryBucketTable(void *pBucketTable, void *pBlock);
extern void *g_pSmallMemoryBucketTable;
extern unsigned int g_cbSmallMemoryBucketUpperBound;
extern const char *g_pszSmallMemoryBucketAllocTag;
extern void *g_pStatusEntryRegistry;
extern void EnterObjectCriticalSection(void *pObject);
extern void LeaveObjectCriticalSection(void *pObject);
extern void CloseCrtFilePointer(FILE *pFile);
extern FILE *OpenFileWithMode(const char *pszPath, const char *pszMode);
extern void SeekFile(FILE *pFile, long lOffset, int nOrigin);
extern unsigned int ReadFileBytes(FILE *pFile, void *pBuffer, size_t cbBuffer);
extern "C" DWORD timeGetTime(void);
extern int LoadResourceArchiveEntryDataIntoBuffer(void *pArchive, int *plFileOffset, unsigned int *pcbBuffer);
extern void FreeResourceObjectDataBuffer(unsigned int pBuffer);
extern void *DestroyMemoryArenaBaseStateReturnThis(void *pArena);

void *g_pCachedResourceObjectBaseDeleteVtable = g_MOGLOAD_CachedResourceObjectBaseVtable;
void *g_pDestroyGRTSResourceAndFreeThunk = g_MOGLOAD_TypedResourceObjectVtable;
void *g_pDestroyEffResourceAndFreeThunk = g_MOGLOAD_EffResourceObjectVtable;

void DestroyResourceArchiveDirectoryTree(int pDirectoryNode);

struct MOGLOAD_EntrySearchState {
    long m_iIndex;
    MOGLOAD_EntryRecord *m_pEntry;
};

void FindNextResourceArchiveEntry(MOGLOAD_DirectoryNode *pDirectory,
                                  MOGLOAD_EntrySearchState *pState,
                                  unsigned int uTagFilter);
void ResetResourceArchiveEntryCursor(MOGLOAD_DirectoryNode *pDirectory,
                                     MOGLOAD_EntrySearchState *pState,
                                     unsigned int uTagFilter);
void FindResourceArchiveEntryByIdRecursive(MOGLOAD_DirectoryNode *pDirectory,
                                           MOGLOAD_EntrySearchState *pState,
                                           int nResourceId,
                                           int fAllowRecursion);
int FindResourceCacheEvictionCandidateIndex(void *pArchive, unsigned int cbMinimumEvict);

// FUNCTION: LEMBALL 0x0045E660
void *DeleteCachedResourceObjectBase(void *pObject, int fDelete) {
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0 && pObject != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045D040
void InitializeTypedResourceObjectBaseVtable(void *pObject) {
    *(void **)pObject = &g_pCachedResourceObjectBaseDeleteVtable;
}

// FUNCTION: LEMBALL 0x0045E5B0
int GetField14NeedsAgeIncrement(void *pObject) {
    return ((MOGLOAD_StringResourceObject *)pObject)->m_nReserved14;
}

// FUNCTION: LEMBALL 0x0045E5C0
int GetField1CVfunc04(void *pObject) {
    return ((MOGLOAD_StringResourceObject *)pObject)->m_nReserved1C;
}

// FUNCTION: LEMBALL 0x0045E5D0
int GetField14Vfunc05(void *pObject) {
    return ((MOGLOAD_StringResourceObject *)pObject)->m_nReserved14;
}

// FUNCTION: LEMBALL 0x0045E5E0
int ReturnZeroVfunc06(void) {
    return 0;
}

// FUNCTION: LEMBALL 0x0045E5F0
void NoopVfunc09(void) {
}

// FUNCTION: LEMBALL 0x0045D100
void EnsureTypedResourceObjectLoaded(void *pObject) {
    MOGLOAD_StringResourceObject *pResourceObject;
    MOGLOAD_TypedResourceObjectVtable *pVtable;
    int fArchiveEntryAlreadyLoaded;
    unsigned long nArchiveEntryCallback;
    unsigned long uLoadedBufferValue;

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    if (pResourceObject->m_nLoadState10 == 0) {
        pVtable = (MOGLOAD_TypedResourceObjectVtable *)pResourceObject->m_pVtable;
        fArchiveEntryAlreadyLoaded = pVtable->m_pGetArchiveEntryField1C(pResourceObject);
        if (fArchiveEntryAlreadyLoaded == 0) {
            if (pResourceObject->m_nReserved04 == 0) {
                if (pResourceObject->m_cbResourceData28 == 0) {
                    pResourceObject->m_pszText38 = 0;
                    pVtable->m_pAssignLoadedBuffer(0, (int *)&pResourceObject->m_pszText38, 0);
                } else {
                    uLoadedBufferValue = (unsigned long)pResourceObject->m_pszText38;
                    if (LoadResourceArchiveEntryDataIntoBuffer(g_pMainResourceArchive,
                                                               &pResourceObject->m_lResourceOffset2C,
                                                               (unsigned int *)&uLoadedBufferValue) != 0) {
                        pResourceObject->m_pszText38 = (char *)uLoadedBufferValue;
                        pVtable->m_pAssignLoadedBuffer((unsigned int)uLoadedBufferValue,
                                                       (int *)&pResourceObject->m_pszText38,
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
void ReleaseTypedResourceObjectIfLoaded(void *pObject, int fReleaseMode) {
    MOGLOAD_StringResourceObject *pResourceObject;
    MOGLOAD_TypedResourceObjectVtable *pVtable;

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    pVtable = (MOGLOAD_TypedResourceObjectVtable *)pResourceObject->m_pVtable;
    pVtable->m_pOnRelease();
    if (pResourceObject->m_nLoadState10 != 0) {
        pResourceObject->m_nLoadState10 = 0;
        pResourceObject->m_pszText38 = 0;
        --pResourceObject->m_nLockCount08;
    }
}

// FUNCTION: LEMBALL 0x0045E600
void *GetEffResourceDataPointer(void *pObject) {
    return ((MOGLOAD_StringResourceObject *)pObject)->m_pszText38;
}

// FUNCTION: LEMBALL 0x0045E610
void NoopOnLoadVfunc11(void) {
}

// FUNCTION: LEMBALL 0x0045E620
void NoopVfunc12(void) {
}

// FUNCTION: LEMBALL 0x0045E640
void ClearResourceTypeTag(void *pObject) {
    ((MOGLOAD_StringResourceObject *)pObject)->m_uTypeTag = 0;
}

// FUNCTION: LEMBALL 0x0045E650
int GetField28GetMemorySize(void *pObject) {
    return ((MOGLOAD_StringResourceObject *)pObject)->m_cbResourceData28;
}

// FUNCTION: LEMBALL 0x0045E820
void *DeleteTypedResourceObject(void *pObject, int fDelete) {
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045EBB0
void *DestroyEffResource(void *pObject, int fDelete) {
    InitializeTypedResourceObjectBaseVtable(pObject);
    if ((fDelete & 1) != 0) {
        FreeVSMemBlock(pObject);
    }
    return pObject;
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

    if (m_cLoadedEntries >= m_cEntries) {
        return 0;
    }

    if (m_pCursorEntry == 0) {
        return 0;
    }

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
    long iIndex;
    MOGLOAD_EntryRecord *pEntry;

    m_iSavedIndex = -1;
    m_pSavedEntry = 0;
    m_iCursorIndex = -1;
    m_pCursorEntry = 0;
    m_lRecordTableOffset = 0;
    m_lNameDataOffset = 0;
    m_cEntries = 0;
    m_cLoadedEntries = 0;
    m_pNameData = 0;

    /* The CRID directory header carries five u32 fields before the name blob and entry table. */
    if (g_pResourceArchiveFile == 0) {
        return this;
    }

    fseek(g_pResourceArchiveFile, lFileOffset, SEEK_SET);
    if (lFileOffset == 0) {
        (void)MOGLOAD_ReadU8();
        fseek(g_pResourceArchiveFile, 0L, SEEK_SET);
    }

    (void)MOGLOAD_ReadU32();
    (void)MOGLOAD_ReadU32();
    m_cEntries = MOGLOAD_ReadU32();
    uFormatVersion = MOGLOAD_ReadU32();
    if (uFormatVersion != 3) {
        return this;
    }

    m_lRecordTableOffset = (long)MOGLOAD_ReadU32();
    lCurrentFileOffset = ftell(g_pResourceArchiveFile);
    m_lNameDataOffset = lCurrentFileOffset;
    lNameDataSize = m_lRecordTableOffset - lCurrentFileOffset;

    if (lNameDataSize > 0) {
        m_pNameData = (char *)AllocateResourceArchiveMemory((unsigned int)lNameDataSize);
        if (m_pNameData != 0) {
            fread(m_pNameData, 1, (size_t)lNameDataSize, g_pResourceArchiveFile);
        }
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
        FindNextResourceArchiveEntry(this, &state, (unsigned int)-1);
        iIndex = state.m_iIndex;
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
    return this;
}

// FUNCTION: LEMBALL 0x0045BFA0
void MOGLOAD_DirectoryNode::ReadEntryRecord(MOGLOAD_EntryRecord *pEntry) {
    long lNameOffset;

    /* On-disk records are fixed 36-byte entries whose name pointer is rebased into the name blob. */
    if (pEntry == 0 || g_pResourceArchiveFile == 0) {
        return;
    }

    fseek(g_pResourceArchiveFile, ((m_iCursorIndex * 4L) + 4L) * 9L + m_lRecordTableOffset, SEEK_SET);

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
    pEntry->m_pNextEntry = 0;
    pEntry->m_pSavedEntry = 0;
    pEntry->m_pChildDirectory = 0;
    fread(pEntry->m_adwDescriptor, 1, sizeof(pEntry->m_adwDescriptor), g_pResourceArchiveFile);
}

// FUNCTION: LEMBALL 0x0045C200
void FindNextResourceArchiveEntry(MOGLOAD_DirectoryNode *pDirectory,
                                  MOGLOAD_EntrySearchState *pState,
                                  unsigned int uTagFilter) {
    int fAtEnd;
    MOGLOAD_EntryRecord *pStateSource;

    fAtEnd = 0;
    do {
        if (pDirectory->m_iCursorIndex == -1) {
            pStateSource = pDirectory->m_pSavedEntry;
            pDirectory->m_iCursorIndex = pDirectory->m_iSavedIndex;
            pDirectory->m_pCursorEntry = pStateSource;
        } else {
            if ((long)pDirectory->m_cEntries - pDirectory->m_iCursorIndex == 1) {
                fAtEnd = 1;
                break;
            }
            if (pDirectory->m_pCursorEntry->m_pNextEntry == 0) {
                pDirectory->AppendEntryAfterCursor();
            }
            pStateSource = pDirectory->m_pCursorEntry->m_pNextEntry;
            pDirectory->m_iCursorIndex += 1;
            pDirectory->m_pCursorEntry = pStateSource;
        }
    } while (uTagFilter != (unsigned int)-1 &&
             pDirectory->m_pCursorEntry->m_uTag != uTagFilter);

    if (!fAtEnd) {
        pState->m_iIndex = pDirectory->m_iCursorIndex;
        pState->m_pEntry = pDirectory->m_pCursorEntry;
        return;
    }

    pState->m_pEntry = 0;
}

// FUNCTION: LEMBALL 0x0045C2A0
void ResetResourceArchiveEntryCursor(MOGLOAD_DirectoryNode *pDirectory,
                                     MOGLOAD_EntrySearchState *pState,
                                     unsigned int uTagFilter) {
    pDirectory->m_iCursorIndex = pDirectory->m_iSavedIndex;
    pDirectory->m_pCursorEntry = pDirectory->m_pSavedEntry;
    pDirectory->m_iCursorIndex = -1;
    FindNextResourceArchiveEntry(pDirectory, pState, uTagFilter);
}

// FUNCTION: LEMBALL 0x0045C060
MOGLOAD_DirectoryNode *MOGLOAD_DirectoryNode::AdvanceSubdirectory(void) {
    long iIndex;
    MOGLOAD_EntryRecord *pEntry;

    pEntry = 0;
    iIndex = -1;

    if (m_pCursorEntry == 0) {
        m_iCursorIndex = m_iSavedIndex;
        m_pCursorEntry = m_pSavedEntry;
        {
            MOGLOAD_EntrySearchState state;

            state.m_iIndex = 0;
            state.m_pEntry = 0;
            FindNextResourceArchiveEntry(this, &state, MOGLOAD_TAG_CRID);
            iIndex = state.m_iIndex;
            pEntry = state.m_pEntry;
        }
        if (pEntry == 0) {
            m_iSavedIndex = m_iCursorIndex;
            m_pSavedEntry = m_pCursorEntry;
            return 0;
        }
        m_iSavedIndex = m_iCursorIndex;
        m_pSavedEntry = m_pCursorEntry;
        if (pEntry == 0) {
            return 0;
        }
    } else {
        m_iSavedIndex = m_iCursorIndex;
        m_pSavedEntry = m_pCursorEntry;
        {
            MOGLOAD_EntrySearchState state;

            state.m_iIndex = 0;
            state.m_pEntry = 0;
            FindNextResourceArchiveEntry(this, &state, MOGLOAD_TAG_CRID);
            iIndex = state.m_iIndex;
            pEntry = state.m_pEntry;
        }
        if (pEntry == 0) {
            return 0;
        }
    }

    if (pEntry == 0 || pEntry->m_uTag != MOGLOAD_TAG_CRID) {
        return 0;
    }

    if (pEntry->m_pChildDirectory == 0) {
        pEntry->m_pChildDirectory =
            (MOGLOAD_DirectoryNode *)AllocateResourceArchiveMemory(sizeof(MOGLOAD_DirectoryNode));
        if (pEntry->m_pChildDirectory != 0) {
            pEntry->m_pChildDirectory->m_iSavedIndex = -1;
            pEntry->m_pChildDirectory->m_pSavedEntry = 0;
            pEntry->m_pChildDirectory->m_iCursorIndex = -1;
            pEntry->m_pChildDirectory->m_pCursorEntry = 0;
            pEntry->m_pChildDirectory->m_lRecordTableOffset = 0;
            pEntry->m_pChildDirectory->m_lNameDataOffset = 0;
            pEntry->m_pChildDirectory->m_cEntries = 0;
            pEntry->m_pChildDirectory->m_cLoadedEntries = 0;
            pEntry->m_pChildDirectory->m_pNameData = 0;
            pEntry->m_pChildDirectory->Construct(pEntry->m_lFileOffset);
        }
    }

    return pEntry->m_pChildDirectory;
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
        pDirectory->m_iSavedIndex = pDirectory->m_iCursorIndex;
        pDirectory->m_pSavedEntry = pDirectory->m_pCursorEntry;
        pEntry = pDirectory->m_pSavedEntry;
        pNextEntry = pDirectory->m_pCursorEntry;
        pDirectory->m_iCursorIndex = pEntry->m_iNextIndex;
        pDirectory->m_pCursorEntry = pEntry->m_pNextEntry;
        if (pEntry->m_uTag == MOGLOAD_TAG_CRID && pEntry->m_pChildDirectory != 0) {
            FreeResourceArchiveMemory(pEntry->m_pChildDirectory);
            pEntry->m_pChildDirectory = 0;
        }
        FreeResourceArchiveMemory(pEntry);
        pDirectory->m_pSavedEntry = 0;
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
        pvBlock = (unsigned int)AllocateFromSmallMemoryBucketTable(g_pSmallMemoryBucketTable, cbBytes);
        if (pvBlock != 0) {
            return (void *)(unsigned long)pvBlock;
        }
    }

    pvBlock = 0;
    if (((int (*)(unsigned int *, unsigned int, const char *))(*(void ***)g_pResourceArchiveMemoryArena)[2])(
            &pvBlock, cbBytes, g_pszSmallMemoryBucketAllocTag) == 0) {
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
        FreeToSmallMemoryBucketTable(g_pSmallMemoryBucketTable, pMemoryBlock) != 0) {
        return;
    }

    ((void (*)(void *))(*(void ***)g_pResourceArchiveMemoryArena)[3])(pMemoryBlock);
}

// FUNCTION: LEMBALL 0x0045BBC0
int OpenResourceArchiveFileHandle(const char *pszPath, const char *pszMode) {
    g_pResourceArchiveFile = OpenFileWithMode(pszPath, pszMode);
    return g_pResourceArchiveFile != 0;
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
            pVtable->m_pReleaseIfLoaded(pResourceArchive->m_ppCachedResourceObjects[iSlot], 1);
        }
    }
    return pBuffer;
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
int FindReusableResourceCacheSlotIndex(MOGLOAD_ResourceArchive *pArchive) {
    MOGLOAD_StringResourceObject **ppCachedObjects;
    int iSlot;
    int nResult;

    nResult = -1;
    iSlot = 0;
    if (pArchive->m_cCachedResourceObjects < 1) {
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
int AttachResourceEntryToObject(MOGLOAD_StringResourceObject *pObject, int iEntryIndex, MOGLOAD_EntryRecord *pEntry) {
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
void FindResourceArchiveEntryByIdRecursive(MOGLOAD_DirectoryNode *pDirectory,
                                          MOGLOAD_EntrySearchState *pState,
                                          int nResourceId,
                                          int fAllowRecursion) {
    MOGLOAD_DirectoryNode *pChildDirectory;
    long iSavedCursorIndex;
    MOGLOAD_EntryRecord *pSavedCursorEntry;

    (void)fAllowRecursion;
    if (pDirectory == 0) {
        pState->m_pEntry = 0;
        return;
    }

    ResetResourceArchiveEntryCursor(pDirectory, pState, (unsigned int)-1);
    while (pState->m_pEntry != 0 && (int)pState->m_pEntry->m_uResourceId != nResourceId) {
        FindNextResourceArchiveEntry(pDirectory, pState, (unsigned int)-1);
    }

    if (pState->m_pEntry == 0) {
        iSavedCursorIndex = pDirectory->m_iCursorIndex;
        pSavedCursorEntry = pDirectory->m_pCursorEntry;
        pDirectory->m_iCursorIndex = pDirectory->m_iSavedIndex;
        pDirectory->m_pCursorEntry = pDirectory->m_pSavedEntry;
        pDirectory->m_iCursorIndex = -1;
        while (pState->m_pEntry == 0) {
            pChildDirectory = pDirectory->AdvanceSubdirectory();
            if (pChildDirectory == 0) {
                break;
            }
            FindResourceArchiveEntryByIdRecursive(pChildDirectory, pState, nResourceId, fAllowRecursion);
        }
        pDirectory->m_iCursorIndex = iSavedCursorIndex;
        pDirectory->m_pCursorEntry = pSavedCursorEntry;
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
    pCurrentDirectory->m_iCursorIndex = pCurrentDirectory->m_iSavedIndex;
    pCurrentDirectory->m_pCursorEntry = pCurrentDirectory->m_pSavedEntry;
    pCurrentDirectory->m_iCursorIndex = -1;

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
            } while (!ResourceArchiveComponentEquals(pResourceArchive->m_pCurrentDirectory->m_pCursorEntry->m_pszName,
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
    }
    pResourceArchive->m_pszSelectedPath = pszNewPath;
    return 1;

bad_path:
    pszOldPath = pResourceArchive->m_pszSelectedPath;
    if (pszOldPath != 0) {
        SelectResourceArchiveDirectoryPath(pResourceArchive, pszOldPath);
    }
    if (pszNewPath != 0) {
        FreeResourceArchiveMemory(pszNewPath);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045C630
void *ConstructResourceArchive(void *pArchive, const char *pszArchiveName, unsigned int cbArenaSize) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    void *pChildArena;
    GAME_StatusEntry *pStatusEntry;
    int nOffset;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    *(char *)g_MOGLOAD_RootPath = '/';
    g_pMainResourceArchive = pArchive;
    pResourceArchive->m_nOpenFailed = 0;
    pResourceArchive->m_ppCachedResourceObjects = 0;
    pResourceArchive->m_pszSelectedPath = 0;
    pResourceArchive->m_pRootDirectory = 0;
    pResourceArchive->m_pCurrentDirectory = 0;
    pResourceArchive->m_cCachedResourceObjects = 0;
    pResourceArchive->m_fSkipPruneOnDestroy = 0;
    pResourceArchive->m_cbArena = cbArenaSize;

    if (AllocateChildMemoryArena(g_pMainMemoryArena, &pChildArena, cbArenaSize) == 1) {
        pResourceArchive->m_fExternalArena = 0;
    }
    g_pResourceArchiveMemoryArena = pChildArena;

    if (!OpenResourceArchiveFileHandle(pszArchiveName, g_MOGLOAD_OpenMode)) {
        pResourceArchive->m_nOpenFailed = 1;
        return pArchive;
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
    AppendStatusEntryToRegistry(g_pStatusEntryRegistry, pStatusEntry);
    ((MOGLOAD_ResourceArenaStatusOwner *)g_pResourceArchiveMemoryArena)->m_pStatusEntry = pStatusEntry;

    return pArchive;
}

// FUNCTION: LEMBALL 0x0045C770
void DestroyResourceArchive(void *pArchive) {
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
        ReleaseChildMemoryArena(g_pMainMemoryArena, g_pResourceArchiveMemoryArena);
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
        ((void (*)())pResourceArchive->m_ppCachedResourceObjects[iSlot]->m_pVtable[7])();
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
            ((int (*)())pResourceObject->m_pVtable[3])() != 0) {
            ++pResourceObject->m_nReserved24;
        }

        ++iCount;
        ++iSlot;
    }
}

// FUNCTION: LEMBALL 0x0045CEB0
void RemoveCachedResourceObject(void *pArchive, void *pResourceObject) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    int iFound;
    int iSlot;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    iFound = 0;
    iSlot = 0;
    if (pResourceArchive->m_cCachedResourceObjects > 0) {
        do {
            if (pResourceArchive->m_ppCachedResourceObjects[iSlot] == 0) {
                do {
                    ++iSlot;
                } while (pResourceArchive->m_ppCachedResourceObjects[iSlot] == 0);
            }
            if (pResourceArchive->m_ppCachedResourceObjects[iSlot] == pResourceObject) {
                pResourceArchive->m_ppCachedResourceObjects[iSlot] = 0;
                break;
            }
            ++iFound;
            ++iSlot;
        } while (iFound < pResourceArchive->m_cCachedResourceObjects);
    }

    if (iFound != pResourceArchive->m_cCachedResourceObjects) {
        --pResourceArchive->m_cCachedResourceObjects;
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
                    ((void (*)(int))*(void **)pResourceObject)(1);
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
    void **ppVtable;

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    ResetTypedResourceObjectState(pResourceObject);
    if (LoadResourceObjectById(g_pMainResourceArchive, nResourceId, (int)(unsigned long)pResourceObject, 1) != 0) {
        pResourceObject->m_nResourceId30 = nResourceId;
        ppVtable = pResourceObject->m_pVtable;
        ((void (*)())ppVtable[2])();
        ((void (*)())ppVtable[7])();
        ++pResourceObject->m_cReferences;
        return;
    }
    pResourceObject->m_nResultCode44 = 1;
}

// FUNCTION: LEMBALL 0x0045D050
void ResetTypedResourceObjectState(void *pObject) {
    MOGLOAD_StringResourceObject *pResourceObject;
    void (**ppVtable)(void *);

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    ppVtable = (void (**)(void *))pResourceObject->m_pVtable;
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
    ppVtable[13](pResourceObject);
    AdvanceCachedResourceObjectFrameCounters(g_pMainResourceArchive);
    pResourceObject->m_nReserved24 = 0;
}

// FUNCTION: LEMBALL 0x0045D250
void *FinalizeLoadedResourceObjectResult(void *pObject) {
    MOGLOAD_StringResourceObject *pResourceObject;

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    if (pResourceObject->m_nResultCode44 == 1) {
        RemoveCachedResourceObject(g_pMainResourceArchive, pResourceObject);
        if (pResourceObject != 0) {
            ((void (*)(void *, int))pResourceObject->m_pVtable[0])(pResourceObject, 1);
        }
        return 0;
    }
    if (pResourceObject->m_nResultCode44 == 2) {
        return 0;
    }
    return pObject;
}

// FUNCTION: LEMBALL 0x0045CB80
int LoadResourceObjectById(void *pArchive, int nResourceId, int pObject, int fCacheObject) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    int iSlot;
    MOGLOAD_StringResourceObject *pCachedObject;
    MOGLOAD_EntrySearchState state;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    state.m_iIndex = 0;
    state.m_pEntry = 0;
    FindResourceArchiveEntryByIdRecursive(pResourceArchive->m_pCurrentDirectory, &state, nResourceId, fCacheObject);
    if (state.m_pEntry == 0) {
        return 0;
    }

    iSlot = FindReusableResourceCacheSlotIndex(pResourceArchive);
    pCachedObject = pResourceArchive->m_ppCachedResourceObjects[iSlot];
    if (pCachedObject != 0) {
        ((void (*)(void *, int))pCachedObject->m_pVtable[0])(pCachedObject, 1);
        pResourceArchive->m_ppCachedResourceObjects[iSlot] = 0;
        --pResourceArchive->m_cCachedResourceObjects;
    }

    pResourceArchive->m_ppCachedResourceObjects[iSlot] = (MOGLOAD_StringResourceObject *)(unsigned long)pObject;
    ++pResourceArchive->m_cCachedResourceObjects;
    return AttachResourceEntryToObject((MOGLOAD_StringResourceObject *)(unsigned long)pObject,
                                       (int)state.m_iIndex,
                                       state.m_pEntry);
}

// FUNCTION: LEMBALL 0x0045CE00
int LoadResourceArchiveEntryDataIntoBuffer(void *pArchive, int *plFileOffset, unsigned int *pcbBuffer) {
    unsigned int pBuffer;

    pBuffer = AllocateResourceDataBufferWithEviction(pArchive, (unsigned int)plFileOffset[1]);
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
        pResourceObject->m_pVtable = (void **)g_pDestroyGRTSResourceAndFreeThunk;
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

// FUNCTION: LEMBALL 0x0045D180
void ReleaseTypedResourceObjectReference(void *pResourceObject) {
    MOGLOAD_StringResourceObject *pObject;

    pObject = (MOGLOAD_StringResourceObject *)pResourceObject;
    if (pObject == 0) {
        return;
    }

    --pObject->m_cReferences;
    if (pObject->m_cReferences == 0) {
        ((void (*)(void *, int))pObject->m_pVtable[8])(pObject, 1);
        if (((MOGLOAD_ResourceArchive *)g_pMainResourceArchive)->m_fSkipPruneOnDestroy != 0) {
            if (pObject->m_nResourceId30 != 0) {
                RemoveCachedResourceObject(g_pMainResourceArchive, pObject);
            }
            ((void (*)(void *, int))pObject->m_pVtable[0])(pObject, 1);
        }
    }
}
