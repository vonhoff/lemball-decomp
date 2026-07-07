#ifndef LEMBALL_MOGLOAD_H
#define LEMBALL_MOGLOAD_H

#include <stdio.h>

enum {
    MOGLOAD_TAG_CRID = 0x44495243u,
};

class MOGLOAD_DirectoryNode;

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

private:
    void ResetNodeState(void);
    int AppendEntryAfterCursor(void);
    void SaveCursor(void);

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

void mogload_set_resource_archive_file(FILE *pFile);
FILE *mogload_get_resource_archive_file(void);
void *allocate_resource_archive_memory(unsigned int cbBytes);

#endif
