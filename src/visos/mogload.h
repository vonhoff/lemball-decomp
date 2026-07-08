#ifndef LEMBALL_MOGLOAD_H
#define LEMBALL_MOGLOAD_H

#include <stdio.h>

enum {
    MOGLOAD_TAG_CRID = 0x44495243u,
};

class MOGLOAD_DirectoryNode;
struct MOGLOAD_StringResourceObject;
class MOGLOAD_ResourceArchive;

class MOGLOAD_EntryRecord {
public:
    char *m_pszName;
    unsigned int m_uTag;
    unsigned int m_uResourceId;
    long m_lFileOffset;
    long m_cbFileSize;
    unsigned char m_abTrailing[16];
    MOGLOAD_EntryRecord *m_pNext;
    MOGLOAD_EntryRecord *m_pPrevious;
    MOGLOAD_DirectoryNode *m_pChildDirectory;
    int m_iSavedIndex;
    MOGLOAD_EntryRecord *m_pSavedEntry;
};

class MOGLOAD_DirectoryNode {
public:
    MOGLOAD_DirectoryNode *Construct(long lFileOffset);
    void ReadEntryRecord(MOGLOAD_EntryRecord *pEntry);
    int FindNextEntry(long *piIndex, MOGLOAD_EntryRecord **ppEntry, unsigned int uTagFilter);
    MOGLOAD_DirectoryNode *AdvanceSubdirectory(void);
    void ResetCursor(void);
    void ResetNodeState(void);
    int AppendEntryAfterCursor(void);
    void SaveCursor(void);

public:
    long m_iSavedIndex;
    MOGLOAD_EntryRecord *m_pSavedEntry;
    long m_iCursorIndex;
    MOGLOAD_EntryRecord *m_pCursorEntry;
    long m_lRecordTableOffset;
    long m_lNameDataOffset;
    unsigned int m_cEntries;
    unsigned int m_cLoadedEntries;
    char *m_pNameData;
};

class MOGLOAD_ResourceArchive {
public:
    MOGLOAD_DirectoryNode *m_pRootDirectory;
    MOGLOAD_DirectoryNode *m_pCurrentDirectory;
    int m_nOpenFailed;
    char *m_pszSelectedPath;
    MOGLOAD_StringResourceObject **m_ppCachedResourceObjects;
    int m_cCachedResourceObjects;
    unsigned int m_cbArena;
    int m_fSkipPruneOnDestroy;
    void *m_pReserved20;
    int m_fExternalArena;
};

void mogload_set_resource_archive_file(FILE *pFile);
FILE *mogload_get_resource_archive_file(void);
void *AllocateResourceArchiveMemory(unsigned int cbBytes);
void FreeResourceArchiveMemory(void *pMemoryBlock);
void *ConstructResourceArchive(void *pArchive, const char *pszArchiveName, unsigned int cbArenaSize);
void DestroyResourceArchive(void *pArchive);
void *FindCachedResourceObjectById(void *pArchive, int nResourceId);
void RemoveCachedResourceObject(void *pArchive, void *pResourceObject);
int AreAllCachedResourceObjectsUnreferenced(void *pArchive);
void PruneUnreferencedCachedResourceObjects(void *pArchive);
MOGLOAD_StringResourceObject *LoadStringResource(int nResourceId);

struct MOGLOAD_StringResourceObject {
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
    int m_nReserved28;
    int m_nReserved2C;
    int m_nResourceId30;
    int m_nReserved30;
    int m_nReserved34;
    char *m_pszText38;
    int m_nReserved3C;
    unsigned int m_uTypeTag;
    int m_nResultCode44;
};

#endif
