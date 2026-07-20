#ifndef LEMBALL_RESOURCE_ARCHIVE_H
#define LEMBALL_RESOURCE_ARCHIVE_H

#include <stdio.h>
#include "ENGINE/CORE/COMMON.H"

enum {
    MogLoadTAG_CRID = 0x44495243u,
};

class MogLoadDirectoryNode;
struct MogLoadStringResourceObject;
class MogLoadResourceArchive;

struct MogLoadEntrySearchState {
    long m_iIndex;
    class MogLoadEntryRecord *m_pEntry;
};

class MogLoadEntryRecord {
public:
    char *m_pszName;
    unsigned int m_uTag;
    unsigned int m_uResourceId;
    long m_lFileOffset;
    long m_cbFileSize;
    int m_iNextIndex;
    MogLoadEntryRecord *m_pNextEntry;
    int m_iSavedIndex;
    MogLoadEntryRecord *m_pSavedEntry;
    MogLoadDirectoryNode *m_pChildDirectory;
    unsigned int m_adwDescriptor[4];
};

class MogLoadDirectoryNode {
public:
    MogLoadDirectoryNode *Construct(long lFileOffset);
    void ReadEntryRecord(MogLoadEntryRecord *pEntry);
    void FindNextResourceArchiveEntry(MogLoadEntrySearchState *pState,
                                      unsigned int uTagFilter);
    void ResetResourceArchiveEntryCursor(MogLoadEntrySearchState *pState,
                                         unsigned int uTagFilter);
    void FindResourceArchiveEntryByIdRecursive(MogLoadEntrySearchState *pState,
                                               int nResourceId,
                                               int fAllowRecursion);
    MogLoadDirectoryNode *AdvanceSubdirectory(void);
    int AppendEntryAfterCursor(void);
    void DestroyResourceArchiveDirectoryTree(void);

public:
    long m_iSubdirectorySavedIndex;
    MogLoadEntryRecord *m_pSubdirectorySavedEntry;
    long m_iSubdirectoryCursorIndex;
    MogLoadEntryRecord *m_pSubdirectoryCursorEntry;
    long m_lRecordTableOffset;
    long m_lNameDataOffset;
    long m_iSavedIndex;
    MogLoadEntryRecord *m_pSavedEntry;
    long m_iCursorIndex;
    MogLoadEntryRecord *m_pCursorEntry;
    unsigned int m_cEntries;
    unsigned int m_cLoadedEntries;
    char *m_pNameData;
    int m_nReserved30;
    int m_nReserved34;
};

class MogLoadResourceArchive {
public:
    void *ConstructResourceArchive(const char *pszArchiveName, unsigned int cbArenaSize);
    unsigned int AllocateResourceDataBufferWithEviction(unsigned int cbBuffer);
    void FreeResourceObjectDataBuffer(unsigned int pBuffer, int fReleaseMode);
    int LoadResourceObjectById(int nResourceId, void *pObject, int fCacheObject);
    int LoadResourceArchiveEntryDataIntoBuffer(int *plFileOffset,
                                               unsigned int *pcbBuffer,
                                               void *pUnused);
    void RemoveCachedResourceObject(void *pResourceObject);
    void AdvanceCachedResourceObjectFrameCounters(void);
    void PruneUnreferencedCachedResourceObjects(void);
    void *FindCachedResourceObjectById(int nResourceId);

    MogLoadDirectoryNode *m_pRootDirectory;
    MogLoadDirectoryNode *m_pCurrentDirectory;
    int m_nOpenFailed;
    char *m_pszSelectedPath;
    MogLoadStringResourceObject **m_ppCachedResourceObjects;
    int m_cCachedResourceObjects;
    unsigned int m_cbArena;
    int m_fSkipPruneOnDestroy;
    void *m_pReserved20;
    int m_fExternalArena;
};

extern void *g_pResourceArchive;

void *AllocateResourceArchiveMemory(unsigned int cbBytes);
void FreeResourceArchiveMemory(void *pMemoryBlock);
void LEMBALL_FASTCALL DestroyResourceArchive(void *pArchive);
int LEMBALL_FASTCALL AreAllCachedResourceObjectsUnreferenced(void *pArchive);
MogLoadStringResourceObject *LoadStringResource(int nResourceId);
void *LoadEffResource(int nResourceId);

struct MogLoadStringResourceObject {
    void **m_pVtable;
    int m_nReserved04;
    int m_nLockCount08;
    int m_cReferences;
    int m_nLoadState10;
    int m_nReserved14;
    int m_nReserved18;
    int m_nReserved1C;
    int m_nReserved20;
    int m_nReserved24;
    int m_cbResourceData28;
    int m_lResourceOffset2C;
    int m_nResourceId30;
    unsigned int *m_padwResourceDescriptor34;
    char *m_pszText38;
    int m_nReserved3C;
    unsigned int m_uTypeTag;
    int m_nResultCode44;

};

#endif
