#include "mogload.h"

#include <stdlib.h>
#include <string.h>

static FILE *g_pResourceArchiveFile = 0;

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
static unsigned char MOGLOAD_ReadU8(void) {
    unsigned char bValue;

    bValue = 0;
    MOGLOAD_ReadBytes(&bValue, 1);
    return bValue;
}

// FUNCTION: LEMBALL 0x0045BC40
static unsigned int MOGLOAD_ReadU32(void) {
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

    pEntry = (MOGLOAD_EntryRecord *)allocate_resource_archive_memory(sizeof(MOGLOAD_EntryRecord));
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
        m_pNameData = (char *)allocate_resource_archive_memory((unsigned int)lNameDataSize);
        if (m_pNameData != 0) {
            fread(m_pNameData, 1, (size_t)lNameDataSize, g_pResourceArchiveFile);
        }
    }

    if (m_cEntries != 0) {
        pEntry = (MOGLOAD_EntryRecord *)allocate_resource_archive_memory(sizeof(MOGLOAD_EntryRecord));
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

    fseek(
        g_pResourceArchiveFile,
        ((m_iCursorIndex * 4L) + 4L) * 9L + m_lRecordTableOffset,
        SEEK_SET
    );

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
            (MOGLOAD_DirectoryNode *)allocate_resource_archive_memory(sizeof(MOGLOAD_DirectoryNode));
        if (pEntry->m_pChildDirectory != 0) {
            pEntry->m_pChildDirectory->ResetNodeState();
            pEntry->m_pChildDirectory->Construct(pEntry->m_lFileOffset);
        }
    }

    return pEntry->m_pChildDirectory;
}

void mogload_set_resource_archive_file(FILE *pFile) {
    g_pResourceArchiveFile = pFile;
}

FILE *mogload_get_resource_archive_file(void) {
    return g_pResourceArchiveFile;
}

void *allocate_resource_archive_memory(unsigned int cbBytes) {
    return malloc(cbBytes);
}
