#include "vsmem.h"

#include "../startup.h"
#include "vsinit.h"

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

static unsigned char g_abMainMemoryArena[0x50];
void *g_pMainMemoryArena = 0;
static int g_cbMainArenaCapacity = 0;
static int g_cbMainArenaInUse = 0;
static int g_fMainArenaReady = 0;
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
    0,
    (void *)AllocateMemoryArenaBlock,
    (void *)FreeMemoryArenaBlock,
    0,
    0,
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
    g_cbMainArenaCapacity = (int)g_StartupGraphicsDriverConfig.m_dwStyle;
    g_cbMainArenaInUse = 0;
    g_fMainArenaReady = 1;
    g_pMainMemoryArena = g_abMainMemoryArena;
    *(void ***)(g_abMainMemoryArena + 0) = g_aMainMemoryArenaVtable;
    *(int *)(g_abMainMemoryArena + 4) = 0x5241524e;
    *(void ***)(g_abMainMemoryArena + 8) = g_aMainMemoryArenaLockVtable;
    *(int *)(g_abMainMemoryArena + 0x24) = (int)g_abMainMemoryArena + (int)GetMemoryArenaHeaderSize();
    *(int *)(g_abMainMemoryArena + 0x28) = g_cbMainArenaCapacity;
    *(int *)(g_abMainMemoryArena + 0x2c) = g_cbMainArenaCapacity;
    *(void **)(g_abMainMemoryArena + 0x30) = 0;
    *(int *)(g_abMainMemoryArena + 0x34) = 0;
    *(int *)(g_abMainMemoryArena + 0x38) = 0;
    *(int *)(g_abMainMemoryArena + 0x3c) = 0;
    *(int *)(g_abMainMemoryArena + 0x40) = 0;
    *(int *)(g_abMainMemoryArena + 0x4c) = 0;
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

    *(int *)((char *)g_pMainMemoryArena + 0x28) = g_cbMainArenaCapacity - g_cbMainArenaInUse;
}

// FUNCTION: LEMBALL 0x004599F0
void *ConstructMemoryArenaBaseState(void *pArena,
                                   unsigned int cbStorage,
                                   const char *pszName,
                                   void *pParentArena,
                                   void *pReserved) {
    (void)cbStorage;
    (void)pReserved;
    *(void ***)pArena = g_aMainMemoryArenaVtable;
    *(void ***)((char *)pArena + 8) = g_aMainMemoryArenaLockVtable;
    *(const char **)((char *)pArena + 0x48) = pszName;
    *(void **)((char *)pArena + 0x44) = pParentArena;
    *(void **)((char *)pArena + 0x34) = 0;
    *(void **)((char *)pArena + 0x38) = 0;
    *(void **)((char *)pArena + 0x3c) = 0;
    *(void **)((char *)pArena + 0x40) = 0;
    *(void **)((char *)pArena + 0x4c) = 0;
    *(void **)((char *)pArena + 0x30) = 0;
    return pArena;
}

// FUNCTION: LEMBALL 0x00459A40
void ReleaseMemoryArenaBlockLists(void *pArena) {
    void **pLockVtable;
    void *pBlock;
    void *pNextBlock;

    pLockVtable = *(void ***)((char *)pArena + 8);
    ((VSMEM_LockProc)pLockVtable[0])();
    pBlock = *(void **)((char *)pArena + 0x34);
    *(void **)((char *)pArena + 0x34) = 0;
    while (pBlock != 0) {
        pNextBlock = *(void **)((char *)pBlock + 0x14);
        ((VSMEM_DeleteProc)(*(void ***)pBlock)[1])(pBlock, 1);
        pBlock = pNextBlock;
    }
    pBlock = *(void **)((char *)pArena + 0x4c);
    *(void **)((char *)pArena + 0x4c) = 0;
    while (pBlock != 0) {
        pNextBlock = *(void **)((char *)pBlock + 0x4c);
        ((VSMEM_DeleteProc)(*(void ***)pBlock)[1])(pBlock, 1);
        pBlock = pNextBlock;
    }
    ((VSMEM_LockProc)pLockVtable[1])();
}

// FUNCTION: LEMBALL 0x00459AA0
void DestroyMemoryArenaBaseState(void *pArena) {
    *(void ***)pArena = g_aMainMemoryArenaVtable;
    *(void ***)((char *)pArena + 8) = g_aMainMemoryArenaLockVtable;
}

// FUNCTION: LEMBALL 0x00459B10
int ConsumeTrailingMemoryBlockIfAdjacent(void *pArena, void *pBlock, void *pNextBlock) {
    VSMEM_SizeProc pfnGetBlockHeaderSize;
    unsigned int cbNextPayload;

    if ((char *)*(void **)((char *)pBlock + 8) + *(unsigned int *)((char *)pBlock + 0xc) != pNextBlock) {
        return 0;
    }
    cbNextPayload = *(unsigned int *)((char *)pNextBlock + 0xc);
    pfnGetBlockHeaderSize = (VSMEM_SizeProc)(*(void ***)pArena)[7];
    *(unsigned int *)((char *)pBlock + 0xc) += cbNextPayload + pfnGetBlockHeaderSize();
    UnlinkMemoryBlockFromFreeList(pArena, pNextBlock);
    UnlinkMemoryBlockFromAddressListLinks(pArena, pNextBlock);
    if (pNextBlock != 0) {
        ((VSMEM_DeleteProc)(*(void ***)pNextBlock)[1])(pNextBlock, 1);
    }
    *(unsigned int *)((char *)pArena + 0x2c) += pfnGetBlockHeaderSize();
    return 1;
}

// FUNCTION: LEMBALL 0x00459BD0
int AppendTailMemoryBlockAddressList(void *pArena, void *pBlock) {
    void *pTail;

    if (*(void **)((char *)pArena + 0x40) == 0) {
        *(void **)((char *)pArena + 0x3c) = pBlock;
    } else {
        *(void **)((char *)*(void **)((char *)pArena + 0x40) + 0x1c) = pBlock;
    }
    pTail = *(void **)((char *)pArena + 0x40);
    *(void **)((char *)pBlock + 0x1c) = 0;
    *(void **)((char *)pBlock + 0x20) = pTail;
    *(void **)((char *)pArena + 0x40) = pBlock;
    return 1;
}

// FUNCTION: LEMBALL 0x00459C00
int InsertMemoryBlockAfterFreeListNode(void *pArena, void *pBlock, void *pPreviousBlock) {
    void *pNextBlock;

    pNextBlock = 0;
    if (pPreviousBlock == 0) {
        *(void **)((char *)pArena + 0x34) = pBlock;
    } else {
        pNextBlock = *(void **)((char *)pPreviousBlock + 0x14);
        *(void **)((char *)pPreviousBlock + 0x14) = pBlock;
    }
    *(void **)((char *)pBlock + 0x18) = pPreviousBlock;
    *(void **)((char *)pBlock + 0x14) = pNextBlock;
    if (pNextBlock == 0) {
        *(void **)((char *)pArena + 0x38) = pBlock;
        return 1;
    }
    *(void **)((char *)pNextBlock + 0x18) = pBlock;
    return 1;
}

// FUNCTION: LEMBALL 0x00459C40
int InsertMemoryBlockSortedByAddress(void *pArena, void *pBlock) {
    void *pCurrentBlock;
    void *pPreviousBlock;

    pCurrentBlock = *(void **)((char *)pArena + 0x4c);
    pPreviousBlock = 0;
    while (pCurrentBlock != 0 && pCurrentBlock <= pBlock) {
        pPreviousBlock = pCurrentBlock;
        pCurrentBlock = *(void **)((char *)pCurrentBlock + 0x4c);
    }
    if (pPreviousBlock == 0) {
        *(void **)((char *)pArena + 0x4c) = pBlock;
    } else {
        *(void **)((char *)pPreviousBlock + 0x4c) = pBlock;
    }
    *(void **)((char *)pBlock + 0x4c) = pCurrentBlock;
    return 1;
}

// FUNCTION: LEMBALL 0x00459C80
int UnlinkMemoryBlockFromAddressListLinks(void *pArena, void *pBlock) {
    void *pPreviousBlock;
    void *pNextBlock;

    pPreviousBlock = *(void **)((char *)pBlock + 0x20);
    pNextBlock = *(void **)((char *)pBlock + 0x1c);
    if (pNextBlock == 0) {
        *(void **)((char *)pArena + 0x40) = pPreviousBlock;
    } else {
        *(void **)((char *)pNextBlock + 0x20) = pPreviousBlock;
    }
    if (pPreviousBlock != 0) {
        *(void **)((char *)pPreviousBlock + 0x1c) = pNextBlock;
        return 1;
    }
    *(void **)((char *)pArena + 0x3c) = pNextBlock;
    return 1;
}

// FUNCTION: LEMBALL 0x00459CB0
int UnlinkMemoryBlockFromFreeList(void *pArena, void *pBlock) {
    void *pPreviousBlock;
    void *pNextBlock;

    pPreviousBlock = *(void **)((char *)pBlock + 0x18);
    pNextBlock = *(void **)((char *)pBlock + 0x14);
    if (pNextBlock == 0) {
        *(void **)((char *)pArena + 0x38) = pPreviousBlock;
    } else {
        *(void **)((char *)pNextBlock + 0x18) = pPreviousBlock;
    }
    if (pPreviousBlock != 0) {
        *(void **)((char *)pPreviousBlock + 0x14) = pNextBlock;
        return 1;
    }
    *(void **)((char *)pArena + 0x34) = pNextBlock;
    return 1;
}

// FUNCTION: LEMBALL 0x00459CE0
int UnlinkMemoryBlockFromAddressList(void *pArena, void *pBlock) {
    void *pCurrentBlock;
    void *pPreviousBlock;

    pCurrentBlock = *(void **)((char *)pArena + 0x4c);
    pPreviousBlock = 0;
    while (pCurrentBlock != 0) {
        if (pCurrentBlock == pBlock) {
            if (pPreviousBlock != 0) {
                *(void **)((char *)pPreviousBlock + 0x4c) = *(void **)((char *)pCurrentBlock + 0x4c);
                return 1;
            }
            *(void **)((char *)pArena + 0x4c) = *(void **)((char *)pCurrentBlock + 0x4c);
            return 1;
        }
        pPreviousBlock = pCurrentBlock;
        pCurrentBlock = *(void **)((char *)pCurrentBlock + 0x4c);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00459D20
void *FindSmallestFreeMemoryBlockAtLeast(void *pArena, unsigned int cbPayload) {
    void *pBestBlock;
    void *pBlock;

    pBestBlock = 0;
    for (pBlock = *(void **)((char *)pArena + 0x40); pBlock != 0; pBlock = *(void **)((char *)pBlock + 0x20)) {
        if (IsFreeMemoryBlock(pBlock) != 0 && cbPayload <= *(unsigned int *)((char *)pBlock + 0xc) &&
            (pBestBlock == 0 ||
             *(unsigned int *)((char *)pBlock + 0xc) < *(unsigned int *)((char *)pBestBlock + 0xc))) {
            pBestBlock = pBlock;
        }
    }
    return pBestBlock;
}

// FUNCTION: LEMBALL 0x00459D70
int AllocateMemoryArenaBlock(void *pArena, void **ppvBlock, unsigned int cbBlock, const char *pszDescription) {
    VSMEM_SizeProc pfnGetBlockHeaderSize;
    VSMEM_StatusUpdateProc pfnUpdateStatusEntry;
    unsigned int cbAligned;
    unsigned int cbBlockHeader;
    unsigned int cbFree;
    unsigned int cbOldBlockPayload;
    unsigned int cbRemainingBlock;
    void *pBlock;
    void *pSplitBlock;
    void *pStatusEntry;
    void **pLockVtable;

    pLockVtable = *(void ***)((char *)pArena + 8);
    ((VSMEM_LockProc)pLockVtable[0])();
    cbAligned = (cbBlock + 3) & 0xfffffffc;
    *ppvBlock = 0;
    pfnGetBlockHeaderSize = (VSMEM_SizeProc)(*(void ***)pArena)[7];
    cbBlockHeader = pfnGetBlockHeaderSize();
    cbFree = *(unsigned int *)((char *)pArena + 0x2c);
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

    pBlock = FindSmallestFreeMemoryBlockAtLeast(pArena, cbAligned + cbBlockHeader);
    if (pBlock == 0) {
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_AllocateFragmentedPrefix);
        AppendCStringToStream(g_pErrorOutputStream, pszDescription);
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_HexPrefix);
        AppendHexUIntToStream(g_pErrorOutputStream, cbAligned);
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_FreeSizePrefix);
        AppendUIntToStream(g_pErrorOutputStream, *(unsigned int *)((char *)pArena + 0x2c));
        AppendCStringToStream(g_pErrorOutputStream, g_VSMEM_LineBreak);
        WriteMemoryArenaReport(g_pMainMemoryArena, g_pErrorOutputStream);
        ((VSMEM_LockProc)pLockVtable[1])();
        return 0;
    }

    if (*(unsigned int *)((char *)pBlock + 0xc) < pfnGetBlockHeaderSize() + cbAligned + cbBlockHeader + 4) {
        *(unsigned int *)((char *)pBlock + 0x24) &= 0xfffffffe;
        UnlinkMemoryBlockFromAddressListLinks(pArena, pBlock);
        NoOpCallback0045A610();
        *(unsigned int *)((char *)pArena + 0x2c) -= *(unsigned int *)((char *)pBlock + 0xc);
    } else {
        cbOldBlockPayload = *(unsigned int *)((char *)pBlock + 0xc);
        *(unsigned int *)((char *)pBlock + 0xc) = cbAligned;
        NoOpCallback0045A610();
        *(unsigned int *)((char *)pBlock + 0x24) &= 0xfffffffe;
        cbRemainingBlock = cbOldBlockPayload - cbAligned;
        UnlinkMemoryBlockFromAddressListLinks(pArena, pBlock);
        pSplitBlock = PlacementConstructMemoryArenaBlock((char *)*(void **)((char *)pBlock + 8) + cbAligned,
                                                         *(void **)((char *)pBlock + 0x10),
                                                         0,
                                                         g_VSMEM_FreeBlockName,
                                                         cbRemainingBlock);
        *(unsigned int *)((char *)pSplitBlock + 0x24) |= 1;
        AppendTailMemoryBlockAddressList(pArena, pSplitBlock);
        InsertMemoryBlockAfterFreeListNode(pArena, pSplitBlock, pBlock);
        *(unsigned int *)((char *)pArena + 0x2c) -= *(unsigned int *)((char *)pBlock + 0xc) + pfnGetBlockHeaderSize();
    }

    *ppvBlock = *(void **)((char *)pBlock + 8);
    pStatusEntry = *(void **)((char *)pArena + 0x30);
    if (pStatusEntry != 0) {
        cbFree = (unsigned int)((char *)*ppvBlock + cbAligned - *(unsigned int *)((char *)pArena + 0x24));
        pfnUpdateStatusEntry = (VSMEM_StatusUpdateProc)(*(void ***)pStatusEntry)[1];
        pfnUpdateStatusEntry(pStatusEntry, cbFree);
    }
    ((VSMEM_LockProc)pLockVtable[1])();
    return 1;
}

// FUNCTION: LEMBALL 0x00459F70
int FreeMemoryArenaBlock(void *pArena, void *pvBlock) {
    VSMEM_SizeProc pfnGetBlockHeaderSize;
    void **pLockVtable;
    void *pBlock;
    void *pPreviousBlock;
    void *pNextBlock;

    if (IsPointerInsideMemoryArenaStorage(pArena, pvBlock) == 0) {
        return 0;
    }
    pLockVtable = *(void ***)((char *)pArena + 8);
    ((VSMEM_LockProc)pLockVtable[0])();
    pfnGetBlockHeaderSize = (VSMEM_SizeProc)(*(void ***)pArena)[7];
    pBlock = (char *)pvBlock - pfnGetBlockHeaderSize();
    *(unsigned int *)((char *)pBlock + 0x24) |= 1;
    AppendTailMemoryBlockAddressList(pArena, pBlock);
    *(unsigned int *)((char *)pArena + 0x2c) += *(unsigned int *)((char *)pBlock + 0xc);
    pPreviousBlock = *(void **)((char *)pBlock + 0x18);
    pNextBlock = *(void **)((char *)pBlock + 0x14);
    if (pPreviousBlock != 0) {
        if (IsFreeMemoryBlock(pPreviousBlock) != 0) {
            if (ConsumeTrailingMemoryBlockIfAdjacent(pArena, pPreviousBlock, pBlock) != 0) {
                pBlock = pPreviousBlock;
            }
        }
    }
    if (pNextBlock != 0) {
        if (IsFreeMemoryBlock(pNextBlock) != 0) {
            ConsumeTrailingMemoryBlockIfAdjacent(pArena, pBlock, pNextBlock);
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
    InsertMemoryBlockSortedByAddress(pArena, pChildArena);
    *ppChildArena = pChildArena;
    ((VSMEM_LockProc)pLockVtable[1])();
    return 1;
}

// FUNCTION: LEMBALL 0x0045A0A0
int ReleaseChildMemoryArena(void *pArena, void *pChildArena) {
    void **pLockVtable;

    pLockVtable = *(void ***)((char *)pArena + 8);
    ((VSMEM_LockProc)pLockVtable[0])();
    if (UnlinkMemoryBlockFromAddressList(pArena, pChildArena) == 0) {
        return 0;
    }
    ((VSMEM_LockProc)pLockVtable[1])();
    return FreeMemoryArenaBlock(pArena, pChildArena);
}

// FUNCTION: LEMBALL 0x0045A0E0
int IsPointerInsideMemoryArenaStorage(void *pArena, void *pvPointer) {
    VSMEM_SizeProc pfnGetArenaHeaderSize;
    unsigned int cbArenaHeader;
    unsigned int cbArenaHeaderSecondRead;

    if (pvPointer == 0) {
        return 0;
    }
    pfnGetArenaHeaderSize = (VSMEM_SizeProc)(*(void ***)pArena)[6];
    cbArenaHeader = pfnGetArenaHeaderSize();
    cbArenaHeaderSecondRead = pfnGetArenaHeaderSize();
    if ((char *)pArena + cbArenaHeader <= pvPointer &&
        pvPointer < (char *)pArena + cbArenaHeaderSecondRead + *(unsigned int *)((char *)pArena + 0x28)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045A1E0
int HasMemoryBlockMagic(void *pBlock) {
    return *(int *)((char *)pBlock + 4) == 0x524d424c;
}

// FUNCTION: LEMBALL 0x0045A200
int IsUsedMemoryBlock(void *pBlock) {
    if (HasMemoryBlockMagic(pBlock) != 0 && (*(unsigned char *)((char *)pBlock + 0x24) & 1) == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045A230
int IsFreeMemoryBlock(void *pBlock) {
    if (HasMemoryBlockMagic(pBlock) != 0 && (*(unsigned char *)((char *)pBlock + 0x24) & 1) != 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045A260
void *WriteMemoryArenaReport(void *pArena, VSINIT_FormattedOutputStream *pOutputStream) {
    void **pLockVtable;
    void **pBlock;

    pLockVtable = *(void ***)((char *)pArena + 8);
    ((VSMEM_LockProc)pLockVtable[0])();
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportSeparator);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportFreeSizePrefix);
    AppendUIntToStream(pOutputStream, GetMemoryArenaPayloadByteCounter(pArena));
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportLineBreak);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportHeaderRule);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportColumns);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportEntryRule);
    for (pBlock = *(void ***)((char *)pArena + 0x34); pBlock != 0; pBlock = (void **)pBlock[5]) {
        ((VSMEM_ReportProc)(*(void ***)pBlock)[0])(pBlock, pOutputStream);
    }
    if (*(void **)((char *)pArena + 0x4c) != 0) {
        AppendCStringToStream(pOutputStream, g_VSMEM_ReportChildArenaList);
        AppendCStringToStream(pOutputStream, g_VSMEM_ReportChildArenaRule);
        for (pBlock = *(void ***)((char *)pArena + 0x4c); pBlock != 0; pBlock = (void **)pBlock[0x13]) {
            ((VSMEM_ReportProc)(*(void ***)pBlock)[0])(pBlock, pOutputStream);
        }
    }
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportSeparator);
    ((VSMEM_LockProc)pLockVtable[1])();
    return pOutputStream;
}

// FUNCTION: LEMBALL 0x0045A340
unsigned int GetMemoryArenaPayloadByteCounter(void *pArena) {
    return *(unsigned int *)((char *)pArena + 0x2c);
}

// FUNCTION: LEMBALL 0x0045A350
long CalculateMemoryArenaAvailableBytes(void *pArena) {
    VSMEM_SizeProc pfnGetBlockHeaderSize;
    unsigned int cFreeBlocks;
    char *pBlock;

    cFreeBlocks = 0;
    pBlock = *(char **)((char *)pArena + 0x34);
    while (pBlock != 0) {
        ++cFreeBlocks;
        pBlock = *(char **)(pBlock + 0x14);
    }
    pfnGetBlockHeaderSize = (VSMEM_SizeProc)(*(void ***)pArena)[7];
    return *(int *)((char *)pArena + 0x28) - (int)(pfnGetBlockHeaderSize() * cFreeBlocks);
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
    unsigned int cbArenaHeader;
    unsigned int cbFreeBlock;
    void *pBlock;

    ConstructMemoryArenaBaseState(pArena, cbStorage, pszName, pParentArena, pReserved);
    *(void ***)pArena = g_aMainMemoryArenaVtable;
    *(void ***)((char *)pArena + 8) = g_aMainMemoryArenaLockVtable;
    *(int *)((char *)pArena + 4) = 0x5241524e;
    cbArenaHeader = GetMemoryArenaHeaderSize();
    cbFreeBlock = cbStorage - cbArenaHeader;
    *(unsigned int *)((char *)pArena + 0x28) = cbFreeBlock;
    cbFreeBlock -= GetMemoryBlockHeaderSize();
    *(unsigned int *)((char *)pArena + 0x2c) = cbFreeBlock;
    cbArenaHeader = GetMemoryArenaHeaderSize();
    *(char **)((char *)pArena + 0x24) = (char *)pArena + cbArenaHeader;
    pBlock = PlacementConstructMemoryArenaBlock((char *)pArena + cbArenaHeader,
                                                pArena,
                                                0,
                                                g_VSMEM_FreeBlockName,
                                                *(unsigned int *)((char *)pArena + 0x28));
    *(unsigned int *)((char *)pBlock + 0x24) |= 1;
    InsertMemoryBlockAfterFreeListNode(pArena, pBlock, 0);
    AppendTailMemoryBlockAddressList(pArena, pBlock);
    return pArena;
}

// FUNCTION: LEMBALL 0x0045A480
void DestroyMemoryArena(void *pArena) {
    *(void ***)pArena = g_aMainMemoryArenaVtable;
    *(void ***)((char *)pArena + 8) = g_aMainMemoryArenaLockVtable;
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
    *(void ***)pBlock = g_aMemoryArenaBlockVtable;
    *(void **)((char *)pBlock + 0x10) = pArena;
    *(void **)((char *)pBlock + 0x14) = pPreviousBlock;
    *(void **)((char *)pBlock + 0x18) = 0;
    *(void **)((char *)pBlock + 0x1c) = 0;
    *(void **)((char *)pBlock + 0x20) = 0;
    *(void **)((char *)pBlock + 0x24) = 0;
    return pBlock;
}

// FUNCTION: LEMBALL 0x0045A570
void RestoreMemoryBlockBaseVtable(void *pBlock) {
    *(void ***)pBlock = g_aMemoryArenaBlockVtable;
}

// FUNCTION: LEMBALL 0x0045A580
int IsPointerInsideMemoryBlock(void *pBlock, void *pvPointer) {
    if (*(void **)((char *)pBlock + 8) <= pvPointer &&
        pvPointer < (char *)*(void **)((char *)pBlock + 8) + *(unsigned int *)((char *)pBlock + 0xc)) {
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
    (void)pszName;
    ConstructMemoryBlockBase(pBlock, pArena, pPreviousBlock);
    *(void ***)pBlock = g_aMemoryArenaBlockVtable;
    *(unsigned int *)((char *)pBlock + 0xc) = cbBlock - 0x28;
    *(char **)((char *)pBlock + 8) = (char *)pBlock + 0x28;
    *(int *)((char *)pBlock + 4) = 0x524d424c;
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
    if (g_pMainMemoryArena != 0 && IsPointerInsideMemoryArenaStorage(g_pMainMemoryArena, pvPointer) != 0) {
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
