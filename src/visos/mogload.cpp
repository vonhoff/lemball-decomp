#include "mogload.h"

#include "../game.h"
#include "../startup.h"
#include "vsinit.h"
#include "vsmem.h"

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
static const char g_MOGLOAD_OpenMode[] = "rb";
static const char g_MOGLOAD_StatusName[] = "Mogload memory";
static const char g_MOGLOAD_RootPath[] = "/";
static const char g_MOGLOAD_SourceFileName[] = "MOGLOAD.CPP";
static const char g_MOGLOAD_IsValidResourceFileAssert[] = "IsValidResourceFile()";
static void *g_MOGLOAD_MemoryArenaStatusEntryVtable[8] = {
    (void *)WriteNamedStatusEntry,
    (void *)UpdateNamedStatusEntry,
    (void *)DeleteFixedBufferStreamReturnThis,
    (void *)ResetFixedBufferStream,
    (void *)AppendCharToFixedBufferStream,
    (void *)AppendCStringToFixedBufferStream,
    (void *)ReturnStreamArgument,
    0,
};
static void *g_MOGLOAD_CachedResourceObjectBaseVtable[14] = { 0 };
static void *g_MOGLOAD_TypedResourceObjectVtable[14] = { 0 };

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
extern "C" DWORD timeGetTime(void);

void *g_pCachedResourceObjectBaseDeleteVtable = g_MOGLOAD_CachedResourceObjectBaseVtable;
void *g_pDestroyGRTSResourceAndFreeThunk = g_MOGLOAD_TypedResourceObjectVtable;

void DestroyResourceArchiveDirectoryTree(int pDirectoryNode);

void *DeleteCachedResourceObjectBase(void *pObject, int fDelete) {
    if (fDelete != 0 && pObject != 0) {
        FreeVSMemBlock(pObject);
    }
    return 0;
}

int ReturnTypedResourceObjectNeedsFrameAdvance(void) {
    return 0;
}

void NoOpTypedResourceObjectCallback(void) {
}

void InitializeMogloadResourceObjectVtables(void) {
    static int g_fInitialized = 0;

    if (g_fInitialized != 0) {
        return;
    }

    g_MOGLOAD_CachedResourceObjectBaseVtable[0] = (void *)DeleteCachedResourceObjectBase;
    g_MOGLOAD_CachedResourceObjectBaseVtable[3] = (void *)ReturnTypedResourceObjectNeedsFrameAdvance;
    g_MOGLOAD_CachedResourceObjectBaseVtable[7] = (void *)NoOpTypedResourceObjectCallback;
    g_MOGLOAD_CachedResourceObjectBaseVtable[8] = (void *)DeleteCachedResourceObjectBase;
    g_MOGLOAD_CachedResourceObjectBaseVtable[13] = (void *)NoOpTypedResourceObjectCallback;

    memcpy(g_MOGLOAD_TypedResourceObjectVtable,
           g_MOGLOAD_CachedResourceObjectBaseVtable,
           sizeof(g_MOGLOAD_TypedResourceObjectVtable));
    g_fInitialized = 1;
}

static void MOGLOAD_ReadBytes(void *pvBuffer, size_t cbBuffer) {
    if (pvBuffer == 0 || cbBuffer == 0) {
        return;
    }

    if (g_pResourceArchiveFile == 0) {
        memset(pvBuffer, 0, cbBuffer);
        return;
    }

    fread(pvBuffer, 1, cbBuffer, g_pResourceArchiveFile);
}

static void MOGLOAD_InitialiseEntryRecord(MOGLOAD_EntryRecord *pEntry) {
    if (pEntry == 0) {
        return;
    }

    pEntry->m_pszName = 0;
    pEntry->m_uTag = 0;
    pEntry->m_uResourceId = 0;
    pEntry->m_lFileOffset = 0;
    pEntry->m_cbFileSize = 0;
    memset(pEntry->m_abTrailing, 0, sizeof(pEntry->m_abTrailing));
    pEntry->m_pNext = 0;
    pEntry->m_pPrevious = 0;
    pEntry->m_pChildDirectory = 0;
    pEntry->m_iSavedIndex = -1;
    pEntry->m_pSavedEntry = 0;
}

// FUNCTION: LEMBALL 0x0045BBE0
unsigned char MOGLOAD_ReadU8(void) {
    unsigned char bValue;

    bValue = 0;
    MOGLOAD_ReadBytes(&bValue, 1);
    return bValue;
}

// FUNCTION: LEMBALL 0x0045BC40
unsigned int MOGLOAD_ReadU32(void) {
    unsigned int dwValue;

    dwValue = 0;
    MOGLOAD_ReadBytes(&dwValue, sizeof(dwValue));
    return dwValue;
}

void MOGLOAD_DirectoryNode::ResetNodeState(void) {
    m_iSavedIndex = -1;
    m_pSavedEntry = 0;
    m_iCursorIndex = -1;
    m_pCursorEntry = 0;
    m_lRecordTableOffset = 0;
    m_lNameDataOffset = 0;
    m_cEntries = 0;
    m_cLoadedEntries = 0;
    m_pNameData = 0;
}

// FUNCTION: LEMBALL 0x0045C2A0
void MOGLOAD_DirectoryNode::ResetCursor(void) {
    m_iCursorIndex = m_iSavedIndex;
    m_pCursorEntry = m_pSavedEntry;
}

void MOGLOAD_DirectoryNode::SaveCursor(void) {
    m_iSavedIndex = m_iCursorIndex;
    m_pSavedEntry = m_pCursorEntry;
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

    MOGLOAD_InitialiseEntryRecord(pEntry);
    pEntry->m_pPrevious = m_pCursorEntry;
    m_pCursorEntry->m_pNext = pEntry;
    ReadEntryRecord(pEntry);
    ++m_cLoadedEntries;
    return 1;
}

// FUNCTION: LEMBALL 0x0045BDA0
MOGLOAD_DirectoryNode *MOGLOAD_DirectoryNode::Construct(long lFileOffset) {
    long lNameDataSize;
    long lCurrentFileOffset;
    unsigned int uFormatVersion;
    long iIndex;
    MOGLOAD_EntryRecord *pEntry;

    ResetNodeState();

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
            MOGLOAD_InitialiseEntryRecord(pEntry);
            m_pSavedEntry = pEntry;
            m_iSavedIndex = 0;
            ReadEntryRecord(pEntry);
            m_cLoadedEntries = 1;
        }
    }

    ResetCursor();

    for (;;) {
        if (!FindNextEntry(&iIndex, &pEntry, (unsigned int)-1)) {
            break;
        }
        if (pEntry != 0 && pEntry->m_uTag == MOGLOAD_TAG_CRID) {
            (void)AdvanceSubdirectory();
        }
    }

    ResetCursor();
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
    MOGLOAD_ReadBytes(pEntry->m_abTrailing, sizeof(pEntry->m_abTrailing));
}

// FUNCTION: LEMBALL 0x0045C200
int MOGLOAD_DirectoryNode::FindNextEntry(long *piIndex, MOGLOAD_EntryRecord **ppEntry, unsigned int uTagFilter) {
    int fAtEnd;

    fAtEnd = 0;

    for (;;) {
        if (m_iCursorIndex == -1) {
            m_iCursorIndex = m_iSavedIndex;
            m_pCursorEntry = m_pSavedEntry;
        } else {
            if ((long)m_cEntries - m_iCursorIndex == 1) {
                fAtEnd = 1;
                break;
            }

            if (m_pCursorEntry != 0 && m_pCursorEntry->m_pNext == 0) {
                if (!AppendEntryAfterCursor()) {
                    fAtEnd = 1;
                    break;
                }
            }

            if (m_pCursorEntry != 0) {
                m_iCursorIndex += 1;
                m_pCursorEntry = m_pCursorEntry->m_pNext;
            }
        }

        if (uTagFilter == (unsigned int)-1) {
            break;
        }

        if (m_pCursorEntry != 0 && m_pCursorEntry->m_uTag == uTagFilter) {
            break;
        }
    }

    if (!fAtEnd && m_pCursorEntry != 0) {
        if (piIndex != 0) {
            *piIndex = m_iCursorIndex;
        }
        if (ppEntry != 0) {
            *ppEntry = m_pCursorEntry;
        }
        return 1;
    }

    if (ppEntry != 0) {
        *ppEntry = 0;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045C060
MOGLOAD_DirectoryNode *MOGLOAD_DirectoryNode::AdvanceSubdirectory(void) {
    long iIndex;
    MOGLOAD_EntryRecord *pEntry;

    pEntry = 0;
    iIndex = -1;

    if (m_pCursorEntry == 0) {
        ResetCursor();
        if (!FindNextEntry(&iIndex, &pEntry, MOGLOAD_TAG_CRID)) {
            SaveCursor();
            return 0;
        }
        SaveCursor();
        if (pEntry == 0) {
            return 0;
        }
    } else {
        SaveCursor();
        if (!FindNextEntry(&iIndex, &pEntry, MOGLOAD_TAG_CRID)) {
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
            pEntry->m_pChildDirectory->ResetNodeState();
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
        pDirectory->m_iCursorIndex = pEntry->m_iSavedIndex;
        pDirectory->m_pCursorEntry = pEntry->m_pSavedEntry;
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

void mogload_set_resource_archive_file(FILE *pFile) {
    g_pResourceArchiveFile = pFile;
}

FILE *mogload_get_resource_archive_file(void) {
    return g_pResourceArchiveFile;
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
    g_pResourceArchiveFile = fopen(pszPath, pszMode);
    return g_pResourceArchiveFile != 0;
}

int FindReusableResourceCacheSlotIndex(MOGLOAD_ResourceArchive *pArchive) {
    int iSlot;

    if (pArchive->m_cCachedResourceObjects < 1) {
        return 0;
    }

    for (iSlot = 0; iSlot < 0x400; ++iSlot) {
        if (pArchive->m_cCachedResourceObjects < 0x400) {
            if (pArchive->m_ppCachedResourceObjects[iSlot] == 0) {
                return iSlot;
            }
        } else if (pArchive->m_ppCachedResourceObjects[iSlot] != 0 &&
                   pArchive->m_ppCachedResourceObjects[iSlot]->m_cReferences == 0) {
            return iSlot;
        }
    }

    return -1;
}

int AttachResourceEntryToObject(MOGLOAD_StringResourceObject *pObject, MOGLOAD_EntryRecord *pEntry) {
    if (pEntry == 0 || pObject == 0) {
        return 0;
    }

    if ((int)pEntry->m_uTag != (int)pObject->m_uTypeTag) {
        return 0;
    }

    pObject->m_nReserved28 = (int)pEntry->m_cbFileSize;
    pObject->m_nResourceId30 = pEntry->m_lFileOffset;
    pObject->m_nReserved34 = (int)(unsigned long)(pEntry->m_abTrailing + 8);
    return 1;
}

int FindResourceArchiveEntryByIdRecursive(MOGLOAD_DirectoryNode *pDirectory,
                                          MOGLOAD_EntryRecord **ppEntry,
                                          int nResourceId,
                                          int fAllowRecursion) {
    long iIndex;
    MOGLOAD_EntryRecord *pEntry;
    MOGLOAD_DirectoryNode *pChildDirectory;

    (void)fAllowRecursion;
    if (ppEntry != 0) {
        *ppEntry = 0;
    }
    if (pDirectory == 0) {
        return 0;
    }

    pDirectory->ResetCursor();
    pDirectory->m_iCursorIndex = -1;
    while (pDirectory->FindNextEntry(&iIndex, &pEntry, (unsigned int)-1)) {
        if ((int)pEntry->m_uResourceId == nResourceId) {
            if (ppEntry != 0) {
                *ppEntry = pEntry;
            }
            return 1;
        }
    }

    pDirectory->ResetCursor();
    pDirectory->m_iCursorIndex = -1;
    for (;;) {
        pChildDirectory = pDirectory->AdvanceSubdirectory();
        if (pChildDirectory == 0) {
            break;
        }
        if (FindResourceArchiveEntryByIdRecursive(pChildDirectory, ppEntry, nResourceId, fAllowRecursion)) {
            return 1;
        }
    }

    return 0;
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
    char *pszComponent;
    char *pszOldPath;
    char *pszSource;
    char *pszTarget;
    unsigned int cBytes;
    MOGLOAD_DirectoryNode *pDirectory;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    if (*pszPath == '/') {
        pResourceArchive->m_pCurrentDirectory = pResourceArchive->m_pRootDirectory;
        pszSource = (char *)pszPath;
        while (*pszSource != '\0') {
            ++pszSource;
        }
        cBytes = (unsigned int)(pszSource - pszPath) + 1;
        pszNewPath = (char *)AllocateResourceArchiveMemory(cBytes);
        pszSource = (char *)pszPath;
        pszTarget = pszNewPath;
    } else {
        pszSource = (char *)pszPath;
        while (*pszSource != '\0') {
            ++pszSource;
        }
        cBytes = (unsigned int)(pszSource - pszPath) + 2;
        pszNewPath = (char *)AllocateResourceArchiveMemory(cBytes);
        pszNewPath[0] = '/';
        pszSource = (char *)pszPath;
        pszTarget = pszNewPath + 1;
        --cBytes;
    }

    if (pszNewPath == 0) {
        return 0;
    }

    do {
        *pszTarget = *pszSource;
        ++pszTarget;
        ++pszSource;
        --cBytes;
    } while (cBytes != 0);

    pDirectory = pResourceArchive->m_pCurrentDirectory;
    if (pDirectory != 0) {
        pDirectory->m_iCursorIndex = pDirectory->m_iSavedIndex;
        pDirectory->m_pCursorEntry = pDirectory->m_pSavedEntry;
        pDirectory->m_iCursorIndex = -1;
    }

    pszComponent = pszNewPath;
    for (;;) {
        pszComponent = strchr(pszComponent, '/');
        if (pszComponent == 0) {
            break;
        }
        ++pszComponent;
        if (*pszComponent != '\0') {
            do {
                pDirectory = pResourceArchive->m_pCurrentDirectory->AdvanceSubdirectory();
                if (pDirectory == 0) {
                    goto bad_path;
                }
            } while (!ResourceArchiveComponentEquals(pResourceArchive->m_pCurrentDirectory->m_pCursorEntry->m_pszName,
                                                     pszComponent));
            if (pDirectory == 0) {
                break;
            }
            pResourceArchive->m_pCurrentDirectory = pDirectory;
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

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    *(char *)g_MOGLOAD_RootPath = '/';
    g_pMainResourceArchive = pArchive;
    pResourceArchive->m_pRootDirectory = 0;
    pResourceArchive->m_pCurrentDirectory = 0;
    pResourceArchive->m_nOpenFailed = 0;
    pResourceArchive->m_pszSelectedPath = 0;
    pResourceArchive->m_ppCachedResourceObjects = 0;
    pResourceArchive->m_cCachedResourceObjects = 0;
    pResourceArchive->m_cbArena = cbArenaSize;
    pResourceArchive->m_fSkipPruneOnDestroy = 0;

    pChildArena = 0;
    InitializeMogloadResourceObjectVtables();
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
    if (pResourceArchive->m_pRootDirectory != 0) {
        pResourceArchive->m_pRootDirectory->Construct(0);
    }
    timeGetTime();
    pResourceArchive->m_pCurrentDirectory = pResourceArchive->m_pRootDirectory;
    SelectResourceArchiveDirectoryPath(pResourceArchive, g_MOGLOAD_RootPath);

    pResourceArchive->m_ppCachedResourceObjects =
        (MOGLOAD_StringResourceObject **)AllocateResourceArchiveMemory(0x1000);
    if (pResourceArchive->m_ppCachedResourceObjects != 0) {
        memset(pResourceArchive->m_ppCachedResourceObjects, 0, 0x1000);
    }

    pStatusEntry = new (AllocateVSMemBlock(sizeof(GAME_StatusEntry))) GAME_StatusEntry(g_MOGLOAD_StatusName);
    if (pStatusEntry != 0) {
        pStatusEntry->m_pVtable = g_MOGLOAD_MemoryArenaStatusEntryVtable;
    }
    AppendStatusEntryToRegistry(g_pStatusEntryRegistry, pStatusEntry);
    if (g_pResourceArchiveMemoryArena != 0) {
        *(GAME_StatusEntry **)((char *)g_pResourceArchiveMemoryArena + 0x30) = pStatusEntry;
    }

    return pArchive;
}

// FUNCTION: LEMBALL 0x0045C770
void DestroyResourceArchive(void *pArchive) {
    MOGLOAD_ResourceArchive *pResourceArchive;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    if (g_pResourceArchiveFile != 0) {
        fclose(g_pResourceArchiveFile);
        g_pResourceArchiveFile = 0;
    }
    AreAllCachedResourceObjectsUnreferenced(pResourceArchive);
    if (pResourceArchive->m_fSkipPruneOnDestroy == 0) {
        PruneUnreferencedCachedResourceObjects(pResourceArchive);
    }
    if (pResourceArchive->m_ppCachedResourceObjects != 0) {
        FreeResourceArchiveMemory(pResourceArchive->m_ppCachedResourceObjects);
        pResourceArchive->m_ppCachedResourceObjects = 0;
    }
    if (pResourceArchive->m_pRootDirectory != 0) {
        DestroyResourceArchiveDirectoryTree((int)(unsigned long)pResourceArchive->m_pRootDirectory);
        FreeResourceArchiveMemory(pResourceArchive->m_pRootDirectory);
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
    int iSlot;
    int cRemaining;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    iSlot = 0;
    cRemaining = pResourceArchive->m_cCachedResourceObjects;
    while (cRemaining != 0) {
        while (pResourceArchive->m_ppCachedResourceObjects[iSlot] == 0 && iSlot < 0x400) {
            ++iSlot;
        }
        if (pResourceArchive->m_ppCachedResourceObjects[iSlot]->m_cReferences != 0) {
            return 0;
        }
        ++iSlot;
        --cRemaining;
    }
    return 1;
}

// FUNCTION: LEMBALL 0x0045CE50
void PruneUnreferencedCachedResourceObjects(void *pArchive) {
    MOGLOAD_ResourceArchive *pResourceArchive;
    MOGLOAD_StringResourceObject *pResourceObject;
    int cRemaining;
    int iCount;
    int iSlot;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    cRemaining = pResourceArchive->m_cCachedResourceObjects;
    iCount = 0;
    iSlot = 0;
    if (cRemaining > 0) {
        do {
            if (pResourceArchive->m_ppCachedResourceObjects[iSlot] == 0) {
                do {
                    ++iSlot;
                } while (pResourceArchive->m_ppCachedResourceObjects[iSlot] == 0);
            }
            if (iSlot == 0x400) {
                return;
            }
            pResourceObject = pResourceArchive->m_ppCachedResourceObjects[iSlot];
            if (pResourceObject->m_cReferences == 0) {
                ((void (*)(void *, int))pResourceObject->m_pVtable[0])(pResourceObject, 1);
                pResourceArchive->m_ppCachedResourceObjects[iSlot] = 0;
                --pResourceArchive->m_cCachedResourceObjects;
            }
            ++iCount;
            ++iSlot;
        } while (iCount < cRemaining);
    }
}

// FUNCTION: LEMBALL 0x0045CF20
void InitializeResourceObjectFromId(void *pObject, int nResourceId) {
    ResetTypedResourceObjectState(pObject);
    if (LoadResourceObjectById(g_pMainResourceArchive, nResourceId, (int)(unsigned long)pObject, 1) != 0) {
        ((MOGLOAD_StringResourceObject *)pObject)->m_nResourceId30 = nResourceId;
        ((void (*)())(*(void ***)pObject)[2])();
        ((void (*)())(*(void ***)pObject)[7])();
        ++((MOGLOAD_StringResourceObject *)pObject)->m_cReferences;
        return;
    }
    ((MOGLOAD_StringResourceObject *)pObject)->m_nResultCode44 = 1;
}

// FUNCTION: LEMBALL 0x0045D050
void ResetTypedResourceObjectState(void *pObject) {
    MOGLOAD_StringResourceObject *pResourceObject;

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    pResourceObject->m_nLockCount08 = 0;
    pResourceObject->m_cReferences = 0;
    pResourceObject->m_nReserved14 = 0;
    pResourceObject->m_nLoadState10 = 0;
    pResourceObject->m_nReserved28 = 0;
    pResourceObject->m_nReserved2C = 0;
    pResourceObject->m_nReserved34 = 0;
    pResourceObject->m_pszText38 = 0;
    pResourceObject->m_nReserved04 = 0;
    pResourceObject->m_nReserved3C = 0;
    pResourceObject->m_uTypeTag = 0;
    pResourceObject->m_nReserved30 = 0;
    pResourceObject->m_nResultCode44 = 0;
    if (pResourceObject->m_pVtable != 0 && pResourceObject->m_pVtable[13] != 0) {
        ((void (*)())pResourceObject->m_pVtable[13])();
    }
    AdvanceCachedResourceObjectFrameCounters(g_pMainResourceArchive);
    pResourceObject->m_nReserved24 = 0;
}

// FUNCTION: LEMBALL 0x0045D250
void *FinalizeLoadedResourceObjectResult(void *pObject) {
    MOGLOAD_StringResourceObject *pResourceObject;

    pResourceObject = (MOGLOAD_StringResourceObject *)pObject;
    if (pResourceObject == 0) {
        return 0;
    }
    if (pResourceObject->m_nResultCode44 == 1) {
        RemoveCachedResourceObject(g_pMainResourceArchive, pResourceObject);
        ((void (*)(void *, int))pResourceObject->m_pVtable[0])(pResourceObject, 1);
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
    MOGLOAD_EntryRecord *pEntry;
    int iSlot;
    MOGLOAD_StringResourceObject *pCachedObject;

    pResourceArchive = (MOGLOAD_ResourceArchive *)pArchive;
    pEntry = 0;
    if (!FindResourceArchiveEntryByIdRecursive(pResourceArchive->m_pCurrentDirectory, &pEntry, nResourceId, fCacheObject) ||
        pEntry == 0) {
        return 0;
    }

    iSlot = FindReusableResourceCacheSlotIndex(pResourceArchive);
    if (iSlot < 0) {
        return 0;
    }

    pCachedObject = pResourceArchive->m_ppCachedResourceObjects[iSlot];
    if (pCachedObject != 0) {
        ((void (*)(void *, int))pCachedObject->m_pVtable[0])(pCachedObject, 1);
        pResourceArchive->m_ppCachedResourceObjects[iSlot] = 0;
        --pResourceArchive->m_cCachedResourceObjects;
    }

    pResourceArchive->m_ppCachedResourceObjects[iSlot] = (MOGLOAD_StringResourceObject *)(unsigned long)pObject;
    ++pResourceArchive->m_cCachedResourceObjects;
    return AttachResourceEntryToObject((MOGLOAD_StringResourceObject *)(unsigned long)pObject, pEntry);
}

// FUNCTION: LEMBALL 0x0045DE00
MOGLOAD_StringResourceObject *LoadStringResource(int nResourceId) {
    MOGLOAD_StringResourceObject *pResourceObject;

    pResourceObject = (MOGLOAD_StringResourceObject *)FindCachedResourceObjectById(g_pMainResourceArchive, nResourceId);
    if (pResourceObject != 0) {
        if (pResourceObject->m_uTypeTag != g_MOGLOAD_StringResourceTypeTag) {
            ReleaseTypedResourceObjectReference(pResourceObject);
            return 0;
        }
        return pResourceObject;
    }

    pResourceObject = (MOGLOAD_StringResourceObject *)AllocateVSMemBlock(0x48);
    if (pResourceObject != 0) {
        pResourceObject->m_pVtable = (void **)g_pCachedResourceObjectBaseDeleteVtable;
        pResourceObject->m_nReserved18 = 0;
        pResourceObject->m_pVtable = (void **)g_pDestroyGRTSResourceAndFreeThunk;
        InitializeResourceObjectFromId(pResourceObject, nResourceId);
        return (MOGLOAD_StringResourceObject *)FinalizeLoadedResourceObjectResult(pResourceObject);
    }

    return (MOGLOAD_StringResourceObject *)FinalizeLoadedResourceObjectResult(0);
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
        if (*(int *)((char *)g_pMainResourceArchive + 0x1c) != 0) {
            if (pObject->m_nResourceId30 != 0) {
                RemoveCachedResourceObject(g_pMainResourceArchive, pObject);
            }
            ((void (*)(void *, int))pObject->m_pVtable[0])(pObject, 1);
        }
    }
}
