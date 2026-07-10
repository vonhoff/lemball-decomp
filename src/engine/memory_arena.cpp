#include "memory_arena.h"

#include "../platform/startup_options.h"
#include "../engine/runtime_init.h"

#include <string.h>
#include <stdlib.h>

struct VSMEM_BlockHeader {
    unsigned int m_cbPayload;
};

typedef void (*VSMEM_LockProc)(void);
typedef unsigned int (*VSMEM_SizeProc)(void);
typedef void *(*VSMEM_ReportProc)(void *pObject, VSINIT_FormattedOutputStream *pOutputStream);
typedef void *(*VSMEM_DeleteProc)(void *pObject, int fDelete);
typedef void (*VSMEM_StatusUpdateProc)(void *pStatusEntry, unsigned int cbValue);

struct CMemoryBlock {
    void **m_pVtable;
    int m_nMagic;
    void *m_pPayload;
    unsigned int m_cbPayload;
    void *m_pArena;
    CMemoryBlock *m_pNextFree;
    CMemoryBlock *m_pPreviousFree;
    CMemoryBlock *m_pNextAddress;
    CMemoryBlock *m_pPreviousAddress;
    unsigned int m_dwFlags;
};

struct CArena {
    void **m_pVtable;
    int m_nMagic;
    void **m_pLockVtable;
    unsigned char m_abReserved[0x18];
    void *m_pStorage;
    unsigned int m_cbStorage;
    unsigned int m_cbFree;
    void *m_pStatusEntry;
    CMemoryBlock *m_pFirstFreeBlock;
    CMemoryBlock *m_pLastFreeBlock;
    CMemoryBlock *m_pFirstAddressBlock;
    CMemoryBlock *m_pLastAddressBlock;
    CArena *m_pParentArena;
    const char *m_pszName;
    CArena *m_pFirstChildArena;

    int ConsumeTrailingMemoryBlockIfAdjacent(void *pBlock, void *pNextBlock);
    int AppendTailMemoryBlockAddressList(void *pBlock);
    int InsertMemoryBlockAfterFreeListNode(void *pBlock, void *pPreviousBlock);
    int InsertMemoryBlockSortedByAddress(void *pBlock);
    int UnlinkMemoryBlockFromAddressListLinks(void *pBlock);
    int UnlinkMemoryBlockFromFreeList(void *pBlock);
    int UnlinkMemoryBlockFromAddressList(void *pBlock);
    void *FindSmallestFreeMemoryBlockAtLeast(unsigned int cbPayload);
    int IsPointerInsideMemoryArenaStorage(void *pvPointer);
};

static unsigned char g_abMainMemoryArena[0x50];
void *g_pMainMemoryArena = 0;
static int g_cbMainArenaCapacity = 0;
static int g_cbMainArenaInUse = 0;
static int g_fMainArenaReady = 0;
void *g_pSmallMemoryBucketTable = 0;
unsigned int g_cbSmallMemoryBucketUpperBound = 0;
const char *g_pszSmallMemoryBucketAllocTag = "Small memory bucket";
static const char g_VSMEM_ReportSeparator[] = "************************************************\n";
static const char g_VSMEM_ReportFreeSizePrefix[] = "Memory Block list free size = ";
static const char g_VSMEM_ReportLineBreak[] = "\n";
static const char g_VSMEM_ReportHeaderRule[] = "--------------------\n";
static const char g_VSMEM_ReportColumns[] = "Addr\t\tSize\t\tDesc\n";
static const char g_VSMEM_ReportEntryRule[] = "------------------------------------------------\n";
static const char g_VSMEM_ReportChildArenaList[] = "Child Arena list\n";
static const char g_VSMEM_ReportChildArenaRule[] = "----------------\n";
static const char g_VSMEM_FreeBlockName[] = "Free";
static const char g_VSMEM_ArenaContainerName[] = "Arena container";
static const char g_VSMEM_AllocateNoMemoryPrefix[] = "CArena::Allocate : Not enough free memory : ";
static const char g_VSMEM_AllocateFragmentedPrefix[] =
    "CArena::Allocate : memory blocks are too fragmented to satisfy : ";
static const char g_VSMEM_HexPrefix[] = " : 0x";
static const char g_VSMEM_FreeSizePrefix[] = " : Free Size is ";
static const char g_VSMEM_LineBreak[] = "\n";

static void VSMEM_NoOpLock(void);
static void *VSMEM_ReportBlockStub(void *pBlock, VSINIT_FormattedOutputStream *pOutputStream);
static void *VSMEM_DeleteBlockStub(void *pBlock, int fDelete);
static void UpdateMainMemoryArenaFreeCounter(void);

static void *g_aMainMemoryArenaVtable[10] = {
    (void *)WriteMemoryArenaReport,
    (void *)VSMEM_NoOpLock,
    (void *)AllocateMemoryArenaBlock,
    (void *)FreeMemoryArenaBlock,
    (void *)VSMEM_NoOpLock,
    (void *)VSMEM_NoOpLock,
    (void *)GetMemoryArenaHeaderSize,
    (void *)GetMemoryBlockHeaderSize,
    (void *)PlacementConstructMemoryArenaWithStorage,
    (void *)PlacementConstructMemoryArenaBlock,
};
static void *g_aMainMemoryArenaLockVtable[2] = {
    (void *)VSMEM_NoOpLock,
    (void *)VSMEM_NoOpLock,
};
static void *g_aMemoryArenaBlockVtable[2] = {
    (void *)VSMEM_ReportBlockStub,
    (void *)VSMEM_DeleteBlockStub,
};

int AllocateFromSmallMemoryBucketTable(void *pBucketTable, unsigned int cbBlock) {
    (void)pBucketTable;
    (void)cbBlock;
    return 0;
}

int FreeToSmallMemoryBucketTable(void *pBucketTable, void *pBlock) {
    (void)pBucketTable;
    (void)pBlock;
    return 0;
}

// FUNCTION: LEMBALL 0x0045A6B0
void *AllocateVSMemBlockImpl(unsigned int cbBlock) {
    VSMEM_BlockHeader *pHeader;

    if (!g_fMainArenaReady || cbBlock == 0) {
        return 0;
    }

    UpdateMainMemoryArenaFreeCounter();
    if ((long)cbBlock > CalculateMemoryArenaAvailableBytes(g_pMainMemoryArena)) {
        return 0;
    }

    pHeader = (VSMEM_BlockHeader *)malloc(sizeof(VSMEM_BlockHeader) + cbBlock);
    if (pHeader == 0) {
        return 0;
    }

    pHeader->m_cbPayload = cbBlock;
    g_cbMainArenaInUse += (long)cbBlock;
    UpdateMainMemoryArenaFreeCounter();
    return pHeader + 1;
}

// FUNCTION: LEMBALL 0x0045A730
void FreeVSMemBlockImpl(void *pvBlock) {
    VSMEM_BlockHeader *pHeader;

    if (pvBlock == 0) {
        return;
    }

    pHeader = ((VSMEM_BlockHeader *)pvBlock) - 1;
    g_cbMainArenaInUse -= (long)pHeader->m_cbPayload;
    UpdateMainMemoryArenaFreeCounter();
    free(pHeader);
}

// FUNCTION: LEMBALL 0x0046F060
int InitializeMasterMainRamArena(void) {
    CArena *pMainArena;

    g_cbMainArenaCapacity = (int)g_StartupGraphicsDriverConfig.m_dwStyle;
    g_cbMainArenaInUse = 0;
    g_fMainArenaReady = 1;
    g_pMainMemoryArena = g_abMainMemoryArena;
    pMainArena = (CArena *)g_abMainMemoryArena;
    pMainArena->m_pVtable = g_aMainMemoryArenaVtable;
    pMainArena->m_nMagic = 0x5241524e;
    pMainArena->m_pLockVtable = g_aMainMemoryArenaLockVtable;
    pMainArena->m_pStorage = (char *)g_abMainMemoryArena + GetMemoryArenaHeaderSize();
    pMainArena->m_cbStorage = g_cbMainArenaCapacity;
    pMainArena->m_cbFree = g_cbMainArenaCapacity;
    pMainArena->m_pStatusEntry = 0;
    pMainArena->m_pFirstFreeBlock = 0;
    pMainArena->m_pLastFreeBlock = 0;
    pMainArena->m_pFirstAddressBlock = 0;
    pMainArena->m_pLastAddressBlock = 0;
    pMainArena->m_pFirstChildArena = 0;
    return 1;
}

// FUNCTION: LEMBALL 0x0046F120
void ShutdownMasterMainRamArena(void) {
    g_pMainMemoryArena = 0;
    g_cbMainArenaCapacity = 0;
    g_cbMainArenaInUse = 0;
    g_fMainArenaReady = 0;
}

// FUNCTION: LEMBALL 0x0045A780
void *AllocateVSMemBlock(unsigned int cbBlock) {
    return AllocateVSMemBlockImpl(cbBlock);
}

// FUNCTION: LEMBALL 0x0045A790
void FreeVSMemBlock(void *pvBlock) {
    FreeVSMemBlockImpl(pvBlock);
}

static void UpdateMainMemoryArenaFreeCounter(void) {
    if (g_pMainMemoryArena == 0) {
        return;
    }

    ((CArena *)g_pMainMemoryArena)->m_cbStorage = g_cbMainArenaCapacity - g_cbMainArenaInUse;
}

// FUNCTION: LEMBALL 0x004599F0
void *ConstructMemoryArenaBaseState(void *pArena,
                                   unsigned int cbStorage,
                                   const char *pszName,
                                   void *pParentArena,
                                   void *pReserved) {
    CArena *pMemoryArena;

    (void)cbStorage;
    (void)pReserved;
    pMemoryArena = (CArena *)pArena;
    pMemoryArena->m_pVtable = g_aMainMemoryArenaVtable;
    pMemoryArena->m_pLockVtable = g_aMainMemoryArenaLockVtable;
    pMemoryArena->m_pszName = pszName;
    pMemoryArena->m_pParentArena = (CArena *)pParentArena;
    pMemoryArena->m_pFirstFreeBlock = 0;
    pMemoryArena->m_pLastFreeBlock = 0;
    pMemoryArena->m_pFirstAddressBlock = 0;
    pMemoryArena->m_pLastAddressBlock = 0;
    pMemoryArena->m_pFirstChildArena = 0;
    pMemoryArena->m_pStatusEntry = 0;
    return pArena;
}

// FUNCTION: LEMBALL 0x00459A40
void ReleaseMemoryArenaBlockLists(void *pArena) {
    CArena *pMemoryArena;
    void **pLockVtable;
    void *pBlock;
    void *pNextBlock;

    pMemoryArena = (CArena *)pArena;
    pLockVtable = pMemoryArena->m_pLockVtable;
    ((VSMEM_LockProc)pLockVtable[0])();
    pBlock = pMemoryArena->m_pFirstFreeBlock;
    pMemoryArena->m_pFirstFreeBlock = 0;
    while (pBlock != 0) {
        pNextBlock = ((CMemoryBlock *)pBlock)->m_pNextFree;
        ((VSMEM_DeleteProc)(*(void ***)pBlock)[1])(pBlock, 1);
        pBlock = pNextBlock;
    }
    pBlock = pMemoryArena->m_pFirstChildArena;
    pMemoryArena->m_pFirstChildArena = 0;
    while (pBlock != 0) {
        pNextBlock = ((CArena *)pBlock)->m_pFirstChildArena;
        ((VSMEM_DeleteProc)(*(void ***)pBlock)[1])(pBlock, 1);
        pBlock = pNextBlock;
    }
    ((VSMEM_LockProc)pLockVtable[1])();
}

// FUNCTION: LEMBALL 0x00459AA0
void DestroyMemoryArenaBaseState(void *pArena) {
    ((CArena *)pArena)->m_pVtable = g_aMainMemoryArenaVtable;
    ((CArena *)pArena)->m_pLockVtable = g_aMainMemoryArenaLockVtable;
}

// FUNCTION: LEMBALL 0x00459B10
int CArena::ConsumeTrailingMemoryBlockIfAdjacent(void *pBlock, void *pNextBlock) {
    VSMEM_SizeProc pfnGetBlockHeaderSize;
    CMemoryBlock *pMemoryBlock;
    CMemoryBlock *pNextMemoryBlock;
    unsigned int cbNextPayload;

    pMemoryBlock = (CMemoryBlock *)pBlock;
    pNextMemoryBlock = (CMemoryBlock *)pNextBlock;
    if ((char *)pMemoryBlock->m_pPayload + pMemoryBlock->m_cbPayload != (char *)pNextMemoryBlock) {
        return 0;
    }
    cbNextPayload = pNextMemoryBlock->m_cbPayload;
    pfnGetBlockHeaderSize = (VSMEM_SizeProc)m_pVtable[7];
    pMemoryBlock->m_cbPayload += cbNextPayload + pfnGetBlockHeaderSize();
    UnlinkMemoryBlockFromFreeList(pNextMemoryBlock);
    UnlinkMemoryBlockFromAddressListLinks(pNextMemoryBlock);
    if (pNextMemoryBlock != 0) {
        ((VSMEM_DeleteProc)pNextMemoryBlock->m_pVtable[1])(pNextMemoryBlock, 1);
    }
    m_cbFree += pfnGetBlockHeaderSize();
    return 1;
}

// FUNCTION: LEMBALL 0x00459BD0
int CArena::AppendTailMemoryBlockAddressList(void *pBlock) {
    CMemoryBlock *pMemoryBlock;
    CMemoryBlock *pTail;

    pMemoryBlock = (CMemoryBlock *)pBlock;
    if (m_pLastAddressBlock == 0) {
        m_pFirstAddressBlock = pMemoryBlock;
    } else {
        m_pLastAddressBlock->m_pNextAddress = pMemoryBlock;
    }
    pTail = m_pLastAddressBlock;
    pMemoryBlock->m_pNextAddress = 0;
    pMemoryBlock->m_pPreviousAddress = pTail;
    m_pLastAddressBlock = pMemoryBlock;
    return 1;
}

// FUNCTION: LEMBALL 0x00459C00
int CArena::InsertMemoryBlockAfterFreeListNode(void *pBlock, void *pPreviousBlock) {
    CMemoryBlock *pMemoryBlock;
    CMemoryBlock *pNextBlock;
    CMemoryBlock *pPreviousMemoryBlock;

    pMemoryBlock = (CMemoryBlock *)pBlock;
    pPreviousMemoryBlock = (CMemoryBlock *)pPreviousBlock;
    pNextBlock = 0;
    if (pPreviousMemoryBlock == 0) {
        m_pFirstFreeBlock = pMemoryBlock;
    } else {
        pNextBlock = pPreviousMemoryBlock->m_pNextFree;
        pPreviousMemoryBlock->m_pNextFree = pMemoryBlock;
    }
    pMemoryBlock->m_pPreviousFree = pPreviousMemoryBlock;
    pMemoryBlock->m_pNextFree = pNextBlock;
    if (pNextBlock == 0) {
        m_pLastFreeBlock = pMemoryBlock;
        return 1;
    }
    pNextBlock->m_pPreviousFree = pMemoryBlock;
    return 1;
}

// FUNCTION: LEMBALL 0x00459C40
int CArena::InsertMemoryBlockSortedByAddress(void *pBlock) {
    CMemoryBlock *pCurrentBlock;
    CMemoryBlock *pMemoryBlock;
    CMemoryBlock *pPreviousBlock;

    pMemoryBlock = (CMemoryBlock *)pBlock;
    pCurrentBlock = (CMemoryBlock *)m_pFirstChildArena;
    pPreviousBlock = 0;
    while (pCurrentBlock != 0 && pCurrentBlock <= pMemoryBlock) {
        pPreviousBlock = pCurrentBlock;
        pCurrentBlock = (CMemoryBlock *)((CArena *)pCurrentBlock)->m_pFirstChildArena;
    }
    if (pPreviousBlock == 0) {
        m_pFirstChildArena = (CArena *)pMemoryBlock;
    } else {
        ((CArena *)pPreviousBlock)->m_pFirstChildArena = (CArena *)pMemoryBlock;
    }
    ((CArena *)pMemoryBlock)->m_pFirstChildArena = (CArena *)pCurrentBlock;
    return 1;
}

// FUNCTION: LEMBALL 0x00459C80
int CArena::UnlinkMemoryBlockFromAddressListLinks(void *pBlock) {
    CMemoryBlock *pMemoryBlock;
    CMemoryBlock *pPreviousBlock;
    CMemoryBlock *pNextBlock;

    pMemoryBlock = (CMemoryBlock *)pBlock;
    pPreviousBlock = pMemoryBlock->m_pPreviousAddress;
    pNextBlock = pMemoryBlock->m_pNextAddress;
    if (pNextBlock == 0) {
        m_pLastAddressBlock = pPreviousBlock;
    } else {
        pNextBlock->m_pPreviousAddress = pPreviousBlock;
    }
    if (pPreviousBlock != 0) {
        pPreviousBlock->m_pNextAddress = pNextBlock;
        return 1;
    }
    m_pFirstAddressBlock = pNextBlock;
    return 1;
}

// FUNCTION: LEMBALL 0x00459CB0
int CArena::UnlinkMemoryBlockFromFreeList(void *pBlock) {
    CMemoryBlock *pMemoryBlock;
    CMemoryBlock *pPreviousBlock;
    CMemoryBlock *pNextBlock;

    pMemoryBlock = (CMemoryBlock *)pBlock;
    pPreviousBlock = pMemoryBlock->m_pPreviousFree;
    pNextBlock = pMemoryBlock->m_pNextFree;
    if (pNextBlock == 0) {
        m_pLastFreeBlock = pPreviousBlock;
    } else {
        pNextBlock->m_pPreviousFree = pPreviousBlock;
    }
    if (pPreviousBlock != 0) {
        pPreviousBlock->m_pNextFree = pNextBlock;
        return 1;
    }
    m_pFirstFreeBlock = pNextBlock;
    return 1;
}

// FUNCTION: LEMBALL 0x00459CE0
int CArena::UnlinkMemoryBlockFromAddressList(void *pBlock) {
    CArena *pChildArena;
    CArena *pCurrentBlock;
    CArena *pPreviousBlock;

    pChildArena = (CArena *)pBlock;
    pCurrentBlock = m_pFirstChildArena;
    pPreviousBlock = 0;
    while (pCurrentBlock != 0) {
        if (pCurrentBlock == pChildArena) {
            if (pPreviousBlock != 0) {
                pPreviousBlock->m_pFirstChildArena = pCurrentBlock->m_pFirstChildArena;
                return 1;
            }
            m_pFirstChildArena = pCurrentBlock->m_pFirstChildArena;
            return 1;
        }
        pPreviousBlock = pCurrentBlock;
        pCurrentBlock = pCurrentBlock->m_pFirstChildArena;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00459D20
void *CArena::FindSmallestFreeMemoryBlockAtLeast(unsigned int cbPayload) {
    CMemoryBlock *pBestBlock;
    CMemoryBlock *pBlock;

    pBestBlock = 0;
    for (pBlock = m_pLastAddressBlock; pBlock != 0; pBlock = pBlock->m_pPreviousAddress) {
        if (IsFreeMemoryBlock(pBlock) != 0 && cbPayload <= pBlock->m_cbPayload &&
            (pBestBlock == 0 ||
             pBlock->m_cbPayload < pBestBlock->m_cbPayload)) {
            pBestBlock = pBlock;
        }
    }
    return pBestBlock;
}

// FUNCTION: LEMBALL 0x00459D70
int AllocateMemoryArenaBlock(void *pArena, void **ppvBlock, unsigned int cbBlock, const char *pszDescription) {
    CArena *pMemoryArena;
    CMemoryBlock *pMemoryBlock;
    CMemoryBlock *pSplitMemoryBlock;
    VSMEM_SizeProc pfnGetBlockHeaderSize;
    VSMEM_StatusUpdateProc pfnUpdateStatusEntry;
    unsigned int cbAligned;
    unsigned int cbBlockHeader;
    unsigned int cbFree;
    unsigned int cbOldBlockPayload;
    unsigned int cbRemainingBlock;
    void *pStatusEntry;
    void **pLockVtable;

    pMemoryArena = (CArena *)pArena;
    pLockVtable = pMemoryArena->m_pLockVtable;
    ((VSMEM_LockProc)pLockVtable[0])();
    cbAligned = (cbBlock + 3) & 0xfffffffc;
    *ppvBlock = 0;
    pfnGetBlockHeaderSize = (VSMEM_SizeProc)pMemoryArena->m_pVtable[7];
    cbBlockHeader = pfnGetBlockHeaderSize();
    cbFree = pMemoryArena->m_cbFree;
    if (cbFree < cbAligned) {
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_AllocateNoMemoryPrefix);
        AppendCStringToStream(g_pErrorOutputStream, pszDescription);
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_HexPrefix);
        AppendHexUIntToStream(g_pErrorOutputStream, cbAligned);
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_FreeSizePrefix);
        AppendUIntToStream(g_pErrorOutputStream, cbFree);
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_LineBreak);
        ((VSMEM_LockProc)pLockVtable[1])();
        return 0;
    }

    pMemoryBlock = (CMemoryBlock *)pMemoryArena->FindSmallestFreeMemoryBlockAtLeast(cbAligned + cbBlockHeader);
    if (pMemoryBlock == 0) {
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_AllocateFragmentedPrefix);
        AppendCStringToStream(g_pErrorOutputStream, pszDescription);
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_HexPrefix);
        AppendHexUIntToStream(g_pErrorOutputStream, cbAligned);
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_FreeSizePrefix);
        AppendUIntToStream(g_pErrorOutputStream, pMemoryArena->m_cbFree);
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_LineBreak);
        WriteMemoryArenaReport(g_pMainMemoryArena, g_pErrorOutputStream);
        ((VSMEM_LockProc)pLockVtable[1])();
        return 0;
    }

    if (pMemoryBlock->m_cbPayload < pfnGetBlockHeaderSize() + cbAligned + cbBlockHeader + 4) {
        pMemoryBlock->m_dwFlags &= 0xfffffffe;
        pMemoryArena->UnlinkMemoryBlockFromAddressListLinks(pMemoryBlock);
        NoOpCallback0045A610();
        pMemoryArena->m_cbFree -= pMemoryBlock->m_cbPayload;
    } else {
        cbOldBlockPayload = pMemoryBlock->m_cbPayload;
        pMemoryBlock->m_cbPayload = cbAligned;
        NoOpCallback0045A610();
        pMemoryBlock->m_dwFlags &= 0xfffffffe;
        cbRemainingBlock = cbOldBlockPayload - cbAligned;
        pMemoryArena->UnlinkMemoryBlockFromAddressListLinks(pMemoryBlock);
        pSplitMemoryBlock =
            (CMemoryBlock *)PlacementConstructMemoryArenaBlock((char *)pMemoryBlock->m_pPayload + cbAligned,
                                                               pMemoryBlock->m_pArena,
                                                               0,
                                                               g_VSMEM_FreeBlockName,
                                                               cbRemainingBlock);
        pSplitMemoryBlock->m_dwFlags |= 1;
        pMemoryArena->AppendTailMemoryBlockAddressList(pSplitMemoryBlock);
        pMemoryArena->InsertMemoryBlockAfterFreeListNode(pSplitMemoryBlock, pMemoryBlock);
        pMemoryArena->m_cbFree -= pMemoryBlock->m_cbPayload + pfnGetBlockHeaderSize();
    }

    *ppvBlock = pMemoryBlock->m_pPayload;
    pStatusEntry = pMemoryArena->m_pStatusEntry;
    if (pStatusEntry != 0) {
        cbFree = (unsigned int)((char *)*ppvBlock + cbAligned - (char *)pMemoryArena->m_pStorage);
        pfnUpdateStatusEntry = (VSMEM_StatusUpdateProc)(*(void ***)pStatusEntry)[1];
        pfnUpdateStatusEntry(pStatusEntry, cbFree);
    }
    ((VSMEM_LockProc)pLockVtable[1])();
    return 1;
}

// FUNCTION: LEMBALL 0x00459F70
int FreeMemoryArenaBlock(void *pArena, void *pvBlock) {
    CArena *pMemoryArena;
    CMemoryBlock *pMemoryBlock;
    CMemoryBlock *pPreviousBlock;
    CMemoryBlock *pNextBlock;
    VSMEM_SizeProc pfnGetBlockHeaderSize;
    void **pLockVtable;

    pMemoryArena = (CArena *)pArena;
    if (pMemoryArena->IsPointerInsideMemoryArenaStorage(pvBlock) == 0) {
        return 0;
    }
    pLockVtable = pMemoryArena->m_pLockVtable;
    ((VSMEM_LockProc)pLockVtable[0])();
    pfnGetBlockHeaderSize = (VSMEM_SizeProc)pMemoryArena->m_pVtable[7];
    pMemoryBlock = (CMemoryBlock *)((char *)pvBlock - pfnGetBlockHeaderSize());
    pMemoryBlock->m_dwFlags |= 1;
    pMemoryArena->AppendTailMemoryBlockAddressList(pMemoryBlock);
    pMemoryArena->m_cbFree += pMemoryBlock->m_cbPayload;
    pPreviousBlock = pMemoryBlock->m_pPreviousFree;
    pNextBlock = pMemoryBlock->m_pNextFree;
    if (pPreviousBlock != 0) {
        if (IsFreeMemoryBlock(pPreviousBlock) != 0) {
            if (pMemoryArena->ConsumeTrailingMemoryBlockIfAdjacent(pPreviousBlock, pMemoryBlock) != 0) {
                pMemoryBlock = pPreviousBlock;
            }
        }
    }
    if (pNextBlock != 0) {
        if (IsFreeMemoryBlock(pNextBlock) != 0) {
            pMemoryArena->ConsumeTrailingMemoryBlockIfAdjacent(pMemoryBlock, pNextBlock);
        }
    }
    ((VSMEM_LockProc)pLockVtable[1])();
    return 1;
}

// FUNCTION: LEMBALL 0x0045A010
int AllocateChildMemoryArena(void *pArena, void **ppChildArena, unsigned int cbChildArena) {
    void **pLockVtable;
    void *pvStorage;
    void *pChildArena;

    *ppChildArena = 0;
    if (AllocateMemoryArenaBlock(pArena, &pvStorage, cbChildArena, g_VSMEM_ArenaContainerName) == 0) {
        return 0;
    }
    pLockVtable = *(void ***)((char *)pArena + 8);
    ((VSMEM_LockProc)pLockVtable[0])();
    pChildArena = PlacementConstructMemoryArenaWithStorage(pvStorage, cbChildArena, g_VSMEM_ArenaContainerName, pArena, 0);
    ((CArena *)pArena)->InsertMemoryBlockSortedByAddress(pChildArena);
    *ppChildArena = pChildArena;
    ((VSMEM_LockProc)pLockVtable[1])();
    return 1;
}

// FUNCTION: LEMBALL 0x0045A0A0
int ReleaseChildMemoryArena(void *pArena, void *pChildArena) {
    void **pLockVtable;

    pLockVtable = *(void ***)((char *)pArena + 8);
    ((VSMEM_LockProc)pLockVtable[0])();
    if (((CArena *)pArena)->UnlinkMemoryBlockFromAddressList(pChildArena) == 0) {
        return 0;
    }
    ((VSMEM_LockProc)pLockVtable[1])();
    return FreeMemoryArenaBlock(pArena, pChildArena);
}

// FUNCTION: LEMBALL 0x0045A0E0
int CArena::IsPointerInsideMemoryArenaStorage(void *pvPointer) {
    VSMEM_SizeProc pfnGetArenaHeaderSize;
    unsigned int cbArenaHeader;
    unsigned int cbArenaHeaderSecondRead;

    if (pvPointer == 0) {
        return 0;
    }
    pfnGetArenaHeaderSize = (VSMEM_SizeProc)m_pVtable[6];
    cbArenaHeader = pfnGetArenaHeaderSize();
    cbArenaHeaderSecondRead = pfnGetArenaHeaderSize();
    if ((char *)this + cbArenaHeader <= pvPointer &&
        pvPointer < (char *)this + cbArenaHeaderSecondRead + m_cbStorage) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045A1E0
int PASCAL HasMemoryBlockMagic(void *pBlock) {
    return *(int *)((char *)pBlock + 4) == 0x524d424c;
}

// FUNCTION: LEMBALL 0x0045A200
int PASCAL IsUsedMemoryBlock(void *pBlock) {
    if (HasMemoryBlockMagic(pBlock) != 0 && (*(unsigned char *)((char *)pBlock + 0x24) & 1) == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045A230
int PASCAL IsFreeMemoryBlock(void *pBlock) {
    if (HasMemoryBlockMagic(pBlock) != 0 && (*(unsigned char *)((char *)pBlock + 0x24) & 1) != 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045A260
void *WriteMemoryArenaReport(void *pArena, VSINIT_FormattedOutputStream *pOutputStream) {
    CArena *pMemoryArena;
    CMemoryBlock *pMemoryBlock;
    CArena *pChildArena;
    void **pLockVtable;

    pMemoryArena = (CArena *)pArena;
    pLockVtable = pMemoryArena->m_pLockVtable;
    ((VSMEM_LockProc)pLockVtable[0])();
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportSeparator);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportFreeSizePrefix);
    AppendUIntToStream(pOutputStream, GetMemoryArenaPayloadByteCounter(pArena));
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportLineBreak);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportHeaderRule);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportColumns);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportEntryRule);
    for (pMemoryBlock = pMemoryArena->m_pFirstFreeBlock; pMemoryBlock != 0; pMemoryBlock = pMemoryBlock->m_pNextFree) {
        ((VSMEM_ReportProc)pMemoryBlock->m_pVtable[0])(pMemoryBlock, pOutputStream);
    }
    if (pMemoryArena->m_pFirstChildArena != 0) {
        AppendCStringToStream(pOutputStream, g_VSMEM_ReportChildArenaList);
        AppendCStringToStream(pOutputStream, g_VSMEM_ReportChildArenaRule);
        for (pChildArena = pMemoryArena->m_pFirstChildArena; pChildArena != 0;
             pChildArena = pChildArena->m_pFirstChildArena) {
            ((VSMEM_ReportProc)pChildArena->m_pVtable[0])(pChildArena, pOutputStream);
        }
    }
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportSeparator);
    ((VSMEM_LockProc)pLockVtable[1])();
    return pOutputStream;
}

// FUNCTION: LEMBALL 0x0045A340
unsigned int GetMemoryArenaPayloadByteCounter(void *pArena) {
    return ((CArena *)pArena)->m_cbFree;
}

// FUNCTION: LEMBALL 0x0045A350
long CalculateMemoryArenaAvailableBytes(void *pArena) {
    CArena *pMemoryArena;
    CMemoryBlock *pBlock;
    VSMEM_SizeProc pfnGetBlockHeaderSize;
    unsigned int cFreeBlocks;

    pMemoryArena = (CArena *)pArena;
    cFreeBlocks = 0;
    pBlock = pMemoryArena->m_pFirstFreeBlock;
    while (pBlock != 0) {
        ++cFreeBlocks;
        pBlock = pBlock->m_pNextFree;
    }
    pfnGetBlockHeaderSize = (VSMEM_SizeProc)pMemoryArena->m_pVtable[7];
    return pMemoryArena->m_cbStorage - (int)(pfnGetBlockHeaderSize() * cFreeBlocks);
}

// FUNCTION: LEMBALL 0x0045A390
void *ReturnSuppliedPlacementStorage(unsigned int cbStorage, void *pvStorage) {
    (void)cbStorage;
    return pvStorage;
}

// FUNCTION: LEMBALL 0x0045A3A0
void FillMemoryByte(void *pvTarget, unsigned char chValue, unsigned int cbTarget) {
    unsigned int dwFill;
    unsigned int i;
    unsigned int *pdwTarget;
    unsigned char *pbTarget;

    pdwTarget = (unsigned int *)pvTarget;
    dwFill = chValue | ((unsigned int)chValue << 8) | ((unsigned int)chValue << 16) | ((unsigned int)chValue << 24);
    for (i = cbTarget >> 2; i != 0; --i) {
        *pdwTarget = dwFill;
        ++pdwTarget;
    }

    pbTarget = (unsigned char *)pdwTarget;
    for (i = cbTarget & 3; i != 0; --i) {
        *pbTarget = chValue;
        ++pbTarget;
    }
}

// FUNCTION: LEMBALL 0x0045A3D0
void CopyMemoryBytes(void *pvTarget, const void *pvSource, unsigned int cbCopy) {
    memcpy(pvTarget, pvSource, cbCopy);
}

// FUNCTION: LEMBALL 0x0045A3F0
void *ConstructMemoryArena(void *pArena,
                           unsigned int cbStorage,
                           const char *pszName,
                           void *pParentArena,
                           void *pReserved) {
    CArena *pMemoryArena;
    CMemoryBlock *pMemoryBlock;
    unsigned int cbArenaHeader;
    unsigned int cbFreeBlock;

    ConstructMemoryArenaBaseState(pArena, cbStorage, pszName, pParentArena, pReserved);
    pMemoryArena = (CArena *)pArena;
    pMemoryArena->m_pVtable = g_aMainMemoryArenaVtable;
    pMemoryArena->m_pLockVtable = g_aMainMemoryArenaLockVtable;
    pMemoryArena->m_nMagic = 0x5241524e;
    cbArenaHeader = GetMemoryArenaHeaderSize();
    cbFreeBlock = cbStorage - cbArenaHeader;
    pMemoryArena->m_cbStorage = cbFreeBlock;
    cbFreeBlock -= GetMemoryBlockHeaderSize();
    pMemoryArena->m_cbFree = cbFreeBlock;
    cbArenaHeader = GetMemoryArenaHeaderSize();
    pMemoryArena->m_pStorage = (char *)pArena + cbArenaHeader;
    pMemoryBlock = (CMemoryBlock *)PlacementConstructMemoryArenaBlock((char *)pArena + cbArenaHeader,
                                                                      pArena,
                                                                      0,
                                                                      g_VSMEM_FreeBlockName,
                                                                      pMemoryArena->m_cbStorage);
    pMemoryBlock->m_dwFlags |= 1;
    pMemoryArena->InsertMemoryBlockAfterFreeListNode(pMemoryBlock, 0);
    pMemoryArena->AppendTailMemoryBlockAddressList(pMemoryBlock);
    return pArena;
}

// FUNCTION: LEMBALL 0x0045A480
void DestroyMemoryArena(void *pArena) {
    ((CArena *)pArena)->m_pVtable = g_aMainMemoryArenaVtable;
    ((CArena *)pArena)->m_pLockVtable = g_aMainMemoryArenaLockVtable;
    ReleaseMemoryArenaBlockLists(pArena);
    DestroyMemoryArenaBaseState(pArena);
}

// FUNCTION: LEMBALL 0x0045A4A0
unsigned int GetMemoryArenaHeaderSize(void) {
    return 0x50;
}

// FUNCTION: LEMBALL 0x0045A4B0
unsigned int GetMemoryBlockHeaderSize(void) {
    return 0x28;
}

// FUNCTION: LEMBALL 0x0045A4C0
void *PlacementConstructMemoryArenaWithStorage(void *pvStorage,
                                               unsigned int cbStorage,
                                               const char *pszName,
                                               void *pParentArena,
                                               void *pReserved) {
    void *pArena;

    pArena = ReturnSuppliedPlacementStorage(0x50, pvStorage);
    if (pArena != 0) {
        return ConstructMemoryArena(pArena, cbStorage, pszName, pParentArena, pReserved);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045A500
void *PlacementConstructMemoryArenaBlock(void *pvStorage,
                                         void *pArena,
                                         void *pPreviousBlock,
                                         const char *pszName,
                                         unsigned int cbBlock) {
    void *pBlock;

    pBlock = ReturnPlacementBlockStorage(0x28, pvStorage);
    if (pBlock != 0) {
        return ConstructMemoryArenaBlock(pBlock, pArena, pPreviousBlock, pszName, cbBlock);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045A540
void *ConstructMemoryBlockBase(void *pBlock, void *pArena, void *pPreviousBlock) {
    CMemoryBlock *pMemoryBlock;

    pMemoryBlock = (CMemoryBlock *)pBlock;
    pMemoryBlock->m_pVtable = g_aMemoryArenaBlockVtable;
    pMemoryBlock->m_pArena = pArena;
    pMemoryBlock->m_pNextFree = (CMemoryBlock *)pPreviousBlock;
    pMemoryBlock->m_pPreviousFree = 0;
    pMemoryBlock->m_pNextAddress = 0;
    pMemoryBlock->m_pPreviousAddress = 0;
    pMemoryBlock->m_dwFlags = 0;
    return pBlock;
}

// FUNCTION: LEMBALL 0x0045A570
void RestoreMemoryBlockBaseVtable(void *pBlock) {
    *(void ***)pBlock = g_aMemoryArenaBlockVtable;
}

// FUNCTION: LEMBALL 0x0045A580
int IsPointerInsideMemoryBlock(void *pBlock, void *pvPointer) {
    CMemoryBlock *pMemoryBlock;

    pMemoryBlock = (CMemoryBlock *)pBlock;
    if (pMemoryBlock->m_pPayload <= pvPointer &&
        pvPointer < (char *)pMemoryBlock->m_pPayload + pMemoryBlock->m_cbPayload) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045A610
void NoOpCallback0045A610(void) {
}

// FUNCTION: LEMBALL 0x0045A630
void *ReturnPlacementBlockStorage(unsigned int cbStorage, void *pvStorage) {
    (void)cbStorage;
    return pvStorage;
}

// FUNCTION: LEMBALL 0x0045A640
void *ConstructMemoryArenaBlock(void *pBlock,
                                void *pArena,
                                void *pPreviousBlock,
                                const char *pszName,
                                unsigned int cbBlock) {
    CMemoryBlock *pMemoryBlock;

    (void)pszName;
    ConstructMemoryBlockBase(pBlock, pArena, pPreviousBlock);
    pMemoryBlock = (CMemoryBlock *)pBlock;
    pMemoryBlock->m_pVtable = g_aMemoryArenaBlockVtable;
    pMemoryBlock->m_cbPayload = cbBlock - 0x28;
    pMemoryBlock->m_pPayload = (char *)pBlock + 0x28;
    pMemoryBlock->m_nMagic = 0x524d424c;
    return pBlock;
}

// FUNCTION: LEMBALL 0x0045A8E0
void *DestroyMemoryArenaReturnThis(void *pArena) {
    DestroyMemoryArena(pArena);
    return pArena;
}

// FUNCTION: LEMBALL 0x0045A8D0
void *DestroyMemoryArenaBaseStateReturnThis(void *pArena) {
    DestroyMemoryArenaBaseState(pArena);
    return pArena;
}

// FUNCTION: LEMBALL 0x0045A8F0
void *RestoreMemoryBlockBaseVtableReturnThis(void *pBlock) {
    RestoreMemoryBlockBaseVtable(pBlock);
    return pBlock;
}

// FUNCTION: LEMBALL 0x0045A800
int IsPointerInsideManagedMemoryRegions(void *pvPointer) {
    if (g_pMainMemoryArena != 0 &&
        ((CArena *)g_pMainMemoryArena)->IsPointerInsideMemoryArenaStorage(pvPointer) != 0) {
        return 1;
    }
    return 0;
}

static void VSMEM_NoOpLock(void) {
}

static void *VSMEM_ReportBlockStub(void *pBlock, VSINIT_FormattedOutputStream *pOutputStream) {
    (void)pBlock;
    return pOutputStream;
}

static void *VSMEM_DeleteBlockStub(void *pBlock, int fDelete) {
    (void)fDelete;
    return pBlock;
}
