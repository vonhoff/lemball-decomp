#include "ENGINE/CORE/VSMEM.H"

#include "SHELL/OPTIONS.H"
#include "ENGINE/CORE/VSINIT.H"

#include <string.h>
#include <stdlib.h>

DECOMP_SIZE_ASSERT(CMemoryBlock, 0x28);
DECOMP_SIZE_ASSERT(CArena, 0x50);
DECOMP_SIZE_ASSERT(SmallMemoryBucketTable, 0x3c);

#pragma function(memcpy)

extern void TriggerReleaseAssertFailure(const char *pszExpression,
                                        const char *pszFile,
                                        int nLine);

typedef void (LEMBALL_FASTCALL *VSMEM_LockProc)(void *pLockVtable);
typedef unsigned int (*VSMEM_SizeProc)(void);
typedef void *(LEMBALL_FASTCALL *VSMEM_ReportProc)(
    void *pObject, int nUnused, VsInitFormattedOutputStream *pOutputStream);
typedef void *(LEMBALL_FASTCALL *VSMEM_DeleteProc)(
    void *pObject, int nUnused, int fDelete);
typedef void (*VSMEM_StatusUpdateProc)(void *pStatusEntry, unsigned int cbValue);

struct SmallMemoryBucket {
    void **m_pVtable;
    unsigned char m_abCriticalSection[0x18];
    SmallMemoryBucket *m_pChildBucket;
    SmallMemoryBucket *m_pParentBucket;
    unsigned int m_cbSlot;
    unsigned int m_cSlots;
    unsigned int m_cbStorage;
    unsigned int m_cbStorageAvailable;
    int m_cAllocated;
    int m_cAllocations;
    int m_cMaximumAllocations;
    unsigned short m_wFlags;
    unsigned short m_nReserved42;
    unsigned int m_cBitmapWords;
    void *m_pBitmap;
    void *m_pStorage;
    unsigned short m_nReserved50;
    unsigned short m_nReserved52;

    SmallMemoryBucket *ConstructSmallMemoryBucket(unsigned int cbSlot,
                                                      unsigned int cSlots,
                                                      void *pStorage,
                                                      void *pBitmap);
    int AllocateSlot(void **ppvSlot);
    int FreeSlot(void *pvSlot);
};


// GLOBAL: LEMBALL 0x004aa100
void *g_pMainMemoryArena;
static void *g_pMainMemoryArenaStorage = 0;
static HGLOBAL g_hMainMemoryArenaStorage = 0;
static int g_cbMainArenaCapacity = 0;
static int g_cbMainArenaInUse = 0;
static int g_fMainArenaReady = 0;
// GLOBAL: LEMBALL 0x004aa0fc
void *g_pSmallMemoryBucketTable;
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

static void LEMBALL_FASTCALL EnterMemoryArenaCriticalSection(void *pLockVtable);
static void LEMBALL_FASTCALL LeaveMemoryArenaCriticalSection(void *pLockVtable);
static void VSMEM_NoOpLock(void);
void *LEMBALL_FASTCALL VSMEM_ReportBlockStub(
    void *pBlock, int nUnused, VsInitFormattedOutputStream *pOutputStream);
static void *LEMBALL_FASTCALL VSMEM_DeleteBlockStub(
    void *pBlock, int nUnused, int fDelete);
void LEMBALL_FASTCALL RestoreMemoryBlockBaseVtable(void *pBlock);
void *RestoreMemoryBlockBaseVtableReturnThis(void *pBlock);
static void UpdateMainMemoryArenaFreeCounter(void);
static SmallMemoryBucket *LEMBALL_FASTCALL AllocateSmallMemoryChildBucket(
    SmallMemoryBucket *pParentBucket);

union CArenaAllocateMethodAddress {
    int (CArena::*m_pMethod)(void **, unsigned int, const char *);
    void *m_pAddress;
};

union CArenaFreeMethodAddress {
    int (CArena::*m_pMethod)(void *);
    void *m_pAddress;
};

union CArenaReportMethodAddress {
    void *(CArena::*m_pMethod)(VsInitFormattedOutputStream *);
    void *m_pAddress;
};

static CArenaAllocateMethodAddress g_CArenaAllocateMethodAddress = {
    &CArena::Allocate
};
static CArenaFreeMethodAddress g_CArenaFreeMethodAddress = {
    &CArena::FreeMemoryArenaBlock
};
static CArenaReportMethodAddress g_CArenaReportMethodAddress = {
    &CArena::WriteMemoryArenaReport
};

static void *g_aMemoryArenaBaseStateVtable[11] = {
    g_CArenaReportMethodAddress.m_pAddress,
    (void *)DestroyMemoryArenaBaseStateReturnThis,
    (void *)g_CArenaAllocateMethodAddress.m_pAddress,
    g_CArenaFreeMethodAddress.m_pAddress,
    (void *)FillMemoryByte,
    (void *)CopyMemoryBytes,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
    (void *)_purecall,
};
static void *g_aMainMemoryArenaVtable[11] = {
    g_CArenaReportMethodAddress.m_pAddress,
    (void *)DestroyMemoryArenaReturnThis,
    (void *)g_CArenaAllocateMethodAddress.m_pAddress,
    g_CArenaFreeMethodAddress.m_pAddress,
    (void *)FillMemoryByte,
    (void *)CopyMemoryBytes,
    (void *)GetMemoryArenaHeaderSize,
    (void *)GetMemoryBlockHeaderSize,
    (void *)PlacementConstructMemoryArenaWithStorage,
    (void *)PlacementConstructMemoryArenaBlock,
    (void *)_purecall,
};
static void *g_aMemoryArenaInitialVtable[1] = {
    (void *)_purecall,
};
static void *g_aMemoryArenaInitialLockVtable[4] = {
    (void *)EnterMemoryArenaCriticalSection,
    (void *)LeaveMemoryArenaCriticalSection,
    (void *)EnterMemoryArenaCriticalSection,
    (void *)LeaveMemoryArenaCriticalSection,
};
static void *g_aMainMemoryArenaLockVtable[2] = {
    (void *)EnterMemoryArenaCriticalSection,
    (void *)LeaveMemoryArenaCriticalSection,
};
// GLOBAL: LEMBALL 0x00498940
static void *g_aMemoryBlockVtables[2][2] = {
    { (void *)VSMEM_ReportBlockStub,
      (void *)RestoreMemoryBlockBaseVtableReturnThis },
    { (void *)VSMEM_ReportBlockStub,
      (void *)VSMEM_DeleteBlockStub },
};
static void *g_aSmallMemoryBucketVtable[2] = {
    (void *)EnterMemoryArenaCriticalSection,
    (void *)LeaveMemoryArenaCriticalSection,
};

SmallMemoryBucketTable *LEMBALL_FASTCALL ConstructSmallMemoryBucketTable(
    SmallMemoryBucketTable *pTable);
void LEMBALL_FASTCALL DestroySmallMemoryBucketTable(
    SmallMemoryBucketTable *pTable);

// FUNCTION: LEMBALL 0x004599f0
CArena *CArena::ConstructMemoryArenaBaseState(unsigned int cbStorage,
                                               const char *pszName,
                                               void *pParentArena,
                                               void *pReserved)

{
    (void)cbStorage;
    (void)pReserved;
    m_pVtable = g_aMemoryArenaInitialVtable;
    m_pLockVtable = g_aMemoryArenaInitialLockVtable;
    InitializeCriticalSection(m_abCriticalSection);
    m_pVtable = g_aMemoryArenaBaseStateVtable;
    m_pLockVtable = g_aMainMemoryArenaLockVtable;
    m_pszName = pszName;
    m_pParentArena = (CArena *)pParentArena;
    m_pFirstFreeBlock = 0;
    m_pLastFreeBlock = 0;
    m_pFirstAddressBlock = 0;
    m_pLastAddressBlock = 0;
    m_pFirstChildArena = 0;
    m_pStatusEntry = 0;
    return this;
}
// FUNCTION: LEMBALL 0x00459a40
void LEMBALL_FASTCALL ReleaseMemoryArenaBlockLists(void *pArena) {
    CArena *pMemoryArena;
    void **pLockVtable;
    void *pBlock;
    void *pNextBlock;

    pMemoryArena = (CArena *)pArena;
    pLockVtable = pMemoryArena->m_pLockVtable;
    ((VSMEM_LockProc)pLockVtable[0])((char *)pMemoryArena + 8);
    pBlock = pMemoryArena->m_pFirstFreeBlock;
    pMemoryArena->m_pFirstFreeBlock = 0;
    while (pBlock != 0) {
        pNextBlock = ((CMemoryBlock *)pBlock)->m_pNextFree;
        ((VSMEM_DeleteProc)(*(void ***)pBlock)[1])(pBlock, 0, 1);
        pBlock = pNextBlock;
    }
    pBlock = pMemoryArena->m_pFirstChildArena;
    pMemoryArena->m_pFirstChildArena = 0;
    while (pBlock != 0) {
        pNextBlock = ((CArena *)pBlock)->m_pFirstChildArena;
        ((VSMEM_DeleteProc)(*(void ***)pBlock)[1])(pBlock, 0, 1);
        pBlock = pNextBlock;
    }
    ((VSMEM_LockProc)pLockVtable[1])((char *)pMemoryArena + 8);
}

// FUNCTION: LEMBALL 0x00459aa0
void LEMBALL_FASTCALL DestroyMemoryArenaBaseState(void *pArena) {
    ((CArena *)pArena)->m_pVtable = g_aMemoryArenaBaseStateVtable;
    ((CArena *)pArena)->m_pLockVtable = g_aMainMemoryArenaLockVtable;
    DeleteCriticalSection(((CArena *)pArena)->m_abCriticalSection);
}

// FUNCTION: LEMBALL 0x00459b10
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
        ((VSMEM_DeleteProc)pNextMemoryBlock->m_pVtable[1])(
            pNextMemoryBlock, 0, 1);
    }
    m_cbFree += pfnGetBlockHeaderSize();
    return 1;
}

// FUNCTION: LEMBALL 0x00459bd0
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

// FUNCTION: LEMBALL 0x00459c00
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

// FUNCTION: LEMBALL 0x00459c40
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

// FUNCTION: LEMBALL 0x00459c80
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

// FUNCTION: LEMBALL 0x00459cb0
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

// FUNCTION: LEMBALL 0x00459ce0
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
// FUNCTION: LEMBALL 0x00459d20
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

// FUNCTION: LEMBALL 0x00459d70
int CArena::Allocate(void **ppvBlock,
                                     unsigned int cbBlock,
                                     const char *pszDescription)

{
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

    pMemoryArena = this;
    pLockVtable = pMemoryArena->m_pLockVtable;
    ((VSMEM_LockProc)pLockVtable[0])((char *)pMemoryArena + 8);
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
        ((VSMEM_LockProc)pLockVtable[1])((char *)pMemoryArena + 8);
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
        ((CArena *)g_pMainMemoryArena)->WriteMemoryArenaReport(
            g_pErrorOutputStream);
        ((VSMEM_LockProc)pLockVtable[1])((char *)pMemoryArena + 8);
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
    ((VSMEM_LockProc)pLockVtable[1])((char *)pMemoryArena + 8);
    return 1;
}

// FUNCTION: LEMBALL 0x00459f70
int CArena::FreeMemoryArenaBlock(void *pvBlock) {
    CArena *pMemoryArena;
    CMemoryBlock *pMemoryBlock;
    CMemoryBlock *pPreviousBlock;
    CMemoryBlock *pNextBlock;
    VSMEM_SizeProc pfnGetBlockHeaderSize;
    void **pLockVtable;

    pMemoryArena = this;
    if (pMemoryArena->IsPointerInsideMemoryArenaStorage(pvBlock) == 0) {
        return 0;
    }
    pLockVtable = pMemoryArena->m_pLockVtable;
    ((VSMEM_LockProc)pLockVtable[0])((char *)pMemoryArena + 8);
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
    ((VSMEM_LockProc)pLockVtable[1])((char *)pMemoryArena + 8);
    return 1;
}

// FUNCTION: LEMBALL 0x0045a010
int CArena::AllocateChildMemoryArena(void **ppChildArena,
                                     unsigned int cbChildArena,
                                     const char *pszName)
{
      void **pLockVtable;
      void *pvStorage;
      CMemoryBlock *pMemoryBlock;
      void *pChildArena;

    *ppChildArena = 0;
    if (Allocate(
            &pvStorage, cbChildArena, g_VSMEM_ArenaContainerName) == 0) {
        return 0;
    }
      pLockVtable = m_pLockVtable;
      ((VSMEM_LockProc)pLockVtable[0])((char *)this + 8);
      /*
       * Allocate returns the payload address.  The original
       * code walks back to the CMemoryBlock header before constructing the
       * child arena, so that the constructor receives the block's recorded
       * payload and size rather than the request values.
       */
      pMemoryBlock = (CMemoryBlock *)((char *)pvStorage - GetMemoryBlockHeaderSize());
      pChildArena = PlacementConstructMemoryArenaWithStorage(
          pMemoryBlock->m_pPayload,
          pMemoryBlock->m_cbPayload,
          pszName,
          this,
          0);
    InsertMemoryBlockSortedByAddress(pChildArena);
    *ppChildArena = pChildArena;
    ((VSMEM_LockProc)pLockVtable[1])((char *)this + 8);
    return 1;
}

// FUNCTION: LEMBALL 0x0045a0a0
int CArena::ReleaseChildMemoryArena(void *pChildArena) {
    void **pLockVtable;

    pLockVtable = m_pLockVtable;
    ((VSMEM_LockProc)pLockVtable[0])((char *)this + 8);
    if (UnlinkMemoryBlockFromAddressList(pChildArena) == 0) {
        return 0;
    }
    ((VSMEM_LockProc)pLockVtable[1])((char *)this + 8);
    return FreeMemoryArenaBlock(pChildArena);
}

// FUNCTION: LEMBALL 0x0045a0e0
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
// FUNCTION: LEMBALL 0x0045a1e0
int PASCAL HasMemoryBlockMagic(void *pBlock) {
    return *(int *)((char *)pBlock + 4) == 0x524d424c;
}

// FUNCTION: LEMBALL 0x0045a200
int PASCAL IsUsedMemoryBlock(void *pBlock) {
    if (HasMemoryBlockMagic(pBlock) != 0 && (*(unsigned char *)((char *)pBlock + 0x24) & 1) == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045a230
int PASCAL IsFreeMemoryBlock(void *pBlock) {
    if (HasMemoryBlockMagic(pBlock) != 0 && (*(unsigned char *)((char *)pBlock + 0x24) & 1) != 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045a260
void *CArena::WriteMemoryArenaReport(VsInitFormattedOutputStream *pOutputStream) {
    CArena *pMemoryArena;
    CMemoryBlock *pMemoryBlock;
    CArena *pChildArena;
    void **pLockVtable;

    pMemoryArena = this;
    pLockVtable = pMemoryArena->m_pLockVtable;
    ((VSMEM_LockProc)pLockVtable[0])((char *)pMemoryArena + 8);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportSeparator);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportFreeSizePrefix);
    AppendUIntToStream(pOutputStream, GetMemoryArenaPayloadByteCounter(this));
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportLineBreak);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportHeaderRule);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportColumns);
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportEntryRule);
    for (pMemoryBlock = pMemoryArena->m_pFirstFreeBlock; pMemoryBlock != 0; pMemoryBlock = pMemoryBlock->m_pNextFree) {
        ((VSMEM_ReportProc)pMemoryBlock->m_pVtable[0])(
            pMemoryBlock, 0, pOutputStream);
    }
    if (pMemoryArena->m_pFirstChildArena != 0) {
        AppendCStringToStream(pOutputStream, g_VSMEM_ReportChildArenaList);
        AppendCStringToStream(pOutputStream, g_VSMEM_ReportChildArenaRule);
        for (pChildArena = pMemoryArena->m_pFirstChildArena; pChildArena != 0;
             pChildArena = pChildArena->m_pFirstChildArena) {
            ((VSMEM_ReportProc)pChildArena->m_pVtable[0])(
                pChildArena, 0, pOutputStream);
        }
    }
    AppendCStringToStream(pOutputStream, g_VSMEM_ReportSeparator);
    ((VSMEM_LockProc)pLockVtable[1])((char *)pMemoryArena + 8);
    return pOutputStream;
}

// FUNCTION: LEMBALL 0x0045a340
unsigned int LEMBALL_FASTCALL GetMemoryArenaPayloadByteCounter(void *pArena) {
    return ((CArena *)pArena)->m_cbFree;
}

// FUNCTION: LEMBALL 0x0045a350
long LEMBALL_FASTCALL CalculateMemoryArenaAvailableBytes(void *pArena) {
    CArena *pMemoryArena;
    CMemoryBlock *pBlock;
    unsigned int cFreeBlocks;

    pMemoryArena = (CArena *)pArena;
    cFreeBlocks = 0;
    pBlock = pMemoryArena->m_pFirstFreeBlock;
    while (pBlock != 0) {
        ++cFreeBlocks;
        pBlock = pBlock->m_pNextFree;
    }
    return pMemoryArena->m_cbStorage -
           (int)(((VSMEM_SizeProc)pMemoryArena->m_pVtable[7])() * cFreeBlocks);
}

// FUNCTION: LEMBALL 0x0045a390
void *ReturnSuppliedPlacementStorage(unsigned int cbStorage, void *pvStorage) {
    (void)cbStorage;
    return pvStorage;
}

// FUNCTION: LEMBALL 0x0045a3a0
void __stdcall FillMemoryByte(void *pvTarget, unsigned char chValue, unsigned int cbTarget) {
    memset(pvTarget, chValue, cbTarget);
}

// FUNCTION: LEMBALL 0x0045a3d0
void __stdcall CopyMemoryBytes(void *pvTarget, const void *pvSource, unsigned int cbCopy) {
    memcpy(pvTarget, pvSource, cbCopy);
}

// FUNCTION: LEMBALL 0x0045a3f0
CArena *CArena::ConstructMemoryArena(unsigned int cbStorage,
                                      const char *pszName,
                                      void *pParentArena,
                                      void *pReserved)

{
    CArena *pMemoryArena;
    CMemoryBlock *pMemoryBlock;
    unsigned int cbArenaHeader;
    unsigned int cbFreeBlock;

    pMemoryArena = this;
    pMemoryArena->ConstructMemoryArenaBaseState(cbStorage, pszName, pParentArena, pReserved);
    pMemoryArena->m_pVtable = g_aMainMemoryArenaVtable;
    pMemoryArena->m_pLockVtable = g_aMainMemoryArenaLockVtable;
    pMemoryArena->m_nMagic = 0x5241524e;
    cbArenaHeader = GetMemoryArenaHeaderSize();
    cbFreeBlock = cbStorage - cbArenaHeader;
    pMemoryArena->m_cbStorage = cbFreeBlock;
    cbFreeBlock -= GetMemoryBlockHeaderSize();
    pMemoryArena->m_cbFree = cbFreeBlock;
    cbArenaHeader = GetMemoryArenaHeaderSize();
    pMemoryArena->m_pStorage = (char *)pMemoryArena + cbArenaHeader;
    pMemoryBlock = (CMemoryBlock *)PlacementConstructMemoryArenaBlock((char *)pMemoryArena + cbArenaHeader,
                                                                      pMemoryArena,
                                                                      0,
                                                                      g_VSMEM_FreeBlockName,
                                                                      pMemoryArena->m_cbStorage);
    pMemoryBlock->m_dwFlags |= 1;
    pMemoryArena->InsertMemoryBlockAfterFreeListNode(pMemoryBlock, 0);
    pMemoryArena->AppendTailMemoryBlockAddressList(pMemoryBlock);
    return pMemoryArena;
}

// FUNCTION: LEMBALL 0x0045a480
void LEMBALL_FASTCALL DestroyMemoryArena(void *pArena) {
    ((CArena *)pArena)->m_pVtable = g_aMainMemoryArenaVtable;
    ((CArena *)pArena)->m_pLockVtable = g_aMainMemoryArenaLockVtable;
    ReleaseMemoryArenaBlockLists(pArena);
    DestroyMemoryArenaBaseState(pArena);
}

// FUNCTION: LEMBALL 0x0045a4a0
unsigned int GetMemoryArenaHeaderSize(void) {
    return 0x50;
}

// FUNCTION: LEMBALL 0x0045a4b0
unsigned int GetMemoryBlockHeaderSize(void) {
    return 0x28;
}

// FUNCTION: LEMBALL 0x0045a4c0
void *LEMBALL_STDCALL PlacementConstructMemoryArenaWithStorage(void *pvStorage,
                                               unsigned int cbStorage,
                                               const char *pszName,
                                               void *pParentArena,
                                               void *pReserved)

{
    void *pArena;

    pArena = ReturnSuppliedPlacementStorage(0x50, pvStorage);
    if (pArena != 0) {
        return ((CArena *)pArena)->ConstructMemoryArena(cbStorage, pszName, pParentArena, pReserved);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045a500
void *LEMBALL_STDCALL PlacementConstructMemoryArenaBlock(void *pvStorage,
                                         void *pArena,
                                         void *pPreviousBlock,
                                         const char *pszName,
                                         unsigned int cbBlock)

{
    void *pBlock;

    pBlock = ReturnPlacementBlockStorage(0x28, pvStorage);
    if (pBlock != 0) {
        return ((CMemoryBlock *)pBlock)->ConstructMemoryArenaBlock(
            pArena, (CMemoryBlock *)pPreviousBlock, pszName, cbBlock);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045a540
CMemoryBlock *CMemoryBlock::ConstructMemoryBlockBase(void *pArena, CMemoryBlock *pPreviousBlock, const char *pszName, unsigned int cbBlock) {
    (void)pszName;
    (void)cbBlock;
    m_pVtable = g_aMemoryBlockVtables[0];
    m_pArena = pArena;
    m_pNextFree = pPreviousBlock;
    m_pPreviousFree = 0;
    m_pNextAddress = 0;
    m_pPreviousAddress = 0;
    m_dwFlags = 0;
    return this;
}

// FUNCTION: LEMBALL 0x0045a570
void LEMBALL_FASTCALL RestoreMemoryBlockBaseVtable(void *pBlock) {
    *(void ***)pBlock = g_aMemoryBlockVtables[0];
}

// FUNCTION: LEMBALL 0x0045a580
int IsPointerInsideMemoryBlock(void *pBlock, void *pvPointer) {
    CMemoryBlock *pMemoryBlock;

    pMemoryBlock = (CMemoryBlock *)pBlock;
    if (pMemoryBlock->m_pPayload <= pvPointer &&
        pvPointer < (char *)pMemoryBlock->m_pPayload + pMemoryBlock->m_cbPayload) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0045a610
void NoOpCallback0045A610(void) {
}

// FUNCTION: LEMBALL 0x0045a630
void *ReturnPlacementBlockStorage(unsigned int cbStorage, void *pvStorage) {
    (void)cbStorage;
    return pvStorage;
}

// FUNCTION: LEMBALL 0x0045a640
CMemoryBlock *CMemoryBlock::ConstructMemoryArenaBlock(void *pArena, CMemoryBlock *pPreviousBlock, const char *pszName, unsigned int cbBlock) {
    (void)pszName;
    ConstructMemoryBlockBase(pArena, pPreviousBlock, pszName, cbBlock);
    m_pVtable = g_aMemoryBlockVtables[1];
    m_cbPayload = cbBlock - 0x28;
    m_pPayload = (char *)this + 0x28;
    m_nMagic = 0x524d424c;
    return this;
}

// FUNCTION: LEMBALL 0x0045a6b0
void *AllocateVSMemBlockImpl(unsigned int cbBlock) {
    void *pvBlock;
    int nSmallBlock;
    int fAllocated;

    pvBlock = 0;
    if (g_fSmallMemoryBucketTableEnabled != 0 &&
        cbBlock < g_cbSmallMemoryBucketUpperBound) {
        nSmallBlock = ((SmallMemoryBucketTable *)g_pSmallMemoryBucketTable)
            ->AllocateFromSmallMemoryBucketTable(
                cbBlock, g_pszSmallMemoryBucketAllocTag);
        pvBlock = (void *)(unsigned long)nSmallBlock;
        if (nSmallBlock != 0) {
            g_pszSmallMemoryBucketAllocTag = "Small memory bucket";
            return pvBlock;
        }
    }

    fAllocated = ((CArena *)g_pMainMemoryArena)->Allocate(
        &pvBlock, cbBlock, g_pszSmallMemoryBucketAllocTag);
    if (fAllocated == 0) {
        TriggerReleaseAssertFailure("EnoughMemory", "VSMEM.CPP", 0x68d);
    }
    return pvBlock;
}

// FUNCTION: LEMBALL 0x0045a730
void FreeVSMemBlockImpl(void *pvBlock) {
    int fFreed;

    if (pvBlock == 0) {
        return;
    }

    if (g_fSmallMemoryBucketTableEnabled != 0 &&
        g_pSmallMemoryBucketTable != 0) {
        fFreed = ((SmallMemoryBucketTable *)g_pSmallMemoryBucketTable)
            ->FreeToSmallMemoryBucketTable(pvBlock);
        if (fFreed != 0) {
            return;
        }
    }

    fFreed = ((CArena *)g_pMainMemoryArena)->FreeMemoryArenaBlock(pvBlock);
    if (fFreed == 0) {
        TriggerReleaseAssertFailure("EnoughMemory", "VSMEM.CPP", 0x6ca);
    }
}

// FUNCTION: LEMBALL 0x0045a780
void *AllocateVSMemBlock(unsigned int cbBlock) {
    return AllocateVSMemBlockImpl(cbBlock);
}

// FUNCTION: LEMBALL 0x0045a790
void FreeVSMemBlock(void *pvBlock) {
    FreeVSMemBlockImpl(pvBlock);
}

static void UpdateMainMemoryArenaFreeCounter(void) {
    if (g_pMainMemoryArena == 0) {
        return;
    }

    ((CArena *)g_pMainMemoryArena)->m_cbStorage = g_cbMainArenaCapacity - g_cbMainArenaInUse;
}

// FUNCTION: LEMBALL 0x0045a800
int IsPointerInsideManagedMemoryRegions(void *pvPointer) {
    if (g_pMainMemoryArena != 0 &&
        ((CArena *)g_pMainMemoryArena)->IsPointerInsideMemoryArenaStorage(pvPointer) != 0) {
        return 1;
    }
    return 0;
}

static void LEMBALL_FASTCALL EnterMemoryArenaCriticalSection(void *pLockVtable) {
    EnterCriticalSection((char *)pLockVtable + 4);
}

static void LEMBALL_FASTCALL LeaveMemoryArenaCriticalSection(void *pLockVtable) {
    LeaveCriticalSection((char *)pLockVtable + 4);
}

static void VSMEM_NoOpLock(void) {
}

void *LEMBALL_FASTCALL VSMEM_ReportBlockStub(
    void *pBlock, int nUnused, VsInitFormattedOutputStream *pOutputStream) {
    (void)pBlock;
    (void)nUnused;
    return pOutputStream;
}

// FUNCTION: LEMBALL 0x0045a8d0
void *LEMBALL_FASTCALL DestroyMemoryArenaBaseStateReturnThis(
    void *pArena, int nUnused, int fDelete)
{
    (void)nUnused;
    (void)fDelete;
    DestroyMemoryArenaBaseState(pArena);
    return pArena;
}

// FUNCTION: LEMBALL 0x0045a8e0
void *LEMBALL_FASTCALL DestroyMemoryArenaReturnThis(
    void *pArena, int nUnused, int fDelete)
{
    (void)nUnused;
    (void)fDelete;
    DestroyMemoryArena(pArena);
    return pArena;
}

// FUNCTION: LEMBALL 0x0045a8f0
void *RestoreMemoryBlockBaseVtableReturnThis(void *pBlock) {
    RestoreMemoryBlockBaseVtable(pBlock);
    return pBlock;
}

// FUNCTION: LEMBALL 0x0045a900
static void *LEMBALL_FASTCALL VSMEM_DeleteBlockStub(void *pBlock, int nUnused, int fDelete) {
    (void)nUnused;
    (void)fDelete;
    *(void ***)pBlock = g_aMemoryBlockVtables[1];
    RestoreMemoryBlockBaseVtable(pBlock);
    return pBlock;
}
// FUNCTION: LEMBALL 0x0046f060
int InitializeMasterMainRamArena(void) {
    HGLOBAL hStorage;
    void *pStorage;
    void *pArena;
    int fSmallBuckets;
    SmallMemoryBucketTable *pTable;

    hStorage = GlobalAlloc(2, (unsigned int)g_StartupGraphicsDriverConfig.m_dwStyle);
    g_hMainMemoryArenaStorage = hStorage;
    if (hStorage == 0) {
        return 0;
    }

    pStorage = GlobalLock(hStorage);
    if (pStorage == 0) {
        return 0;
    }

    pArena = ReturnSuppliedPlacementStorage(0x50, pStorage);
    if (pArena == 0) {
        g_pMainMemoryArena = 0;
    } else {
        g_pMainMemoryArena =
            ((CArena *)pArena)->ConstructMemoryArena(
                (unsigned int)g_StartupGraphicsDriverConfig.m_dwStyle,
                "Master Main Ram Arena", 0, 0);
    }

    fSmallBuckets = g_fSmallMemoryBucketTableEnabled;
    g_pSmallMemoryBucketTable = 0;
    pTable = 0;
    if (fSmallBuckets != 0) {
        g_fSmallMemoryBucketTableEnabled = 0;
        pTable = (SmallMemoryBucketTable *)AllocateVSMemBlock(sizeof(SmallMemoryBucketTable));
        if (pTable != 0) {
            pTable = ConstructSmallMemoryBucketTable(pTable);
        }
    }
    g_pSmallMemoryBucketTable = pTable;
    g_fSmallMemoryBucketTableEnabled = fSmallBuckets;
    if (g_pMainMemoryArena != 0 &&
        (fSmallBuckets == 0 || g_pSmallMemoryBucketTable != 0)) {
        return 1;
    }
    return 0;
}

// FUNCTION: LEMBALL 0x0046f120
void ShutdownMasterMainRamArena(void) {
    if (g_pSmallMemoryBucketTable != 0) {
        DestroySmallMemoryBucketTable(
            (SmallMemoryBucketTable *)g_pSmallMemoryBucketTable);
        FreeVSMemBlock(g_pSmallMemoryBucketTable);
        g_pSmallMemoryBucketTable = 0;
    }
    if (g_pMainMemoryArena != 0) {
        DestroyMemoryArena(g_pMainMemoryArena);
    }
    if (g_hMainMemoryArenaStorage != 0) {
        GlobalUnlock(g_hMainMemoryArenaStorage);
        GlobalFree(g_hMainMemoryArenaStorage);
        g_hMainMemoryArenaStorage = 0;
    }
    g_pMainMemoryArenaStorage = 0;
    g_pMainMemoryArena = 0;
    g_cbMainArenaCapacity = 0;
    g_cbMainArenaInUse = 0;
    g_fMainArenaReady = 0;
}

// FUNCTION: LEMBALL 0x00472ce0
SmallMemoryBucket *SmallMemoryBucket::ConstructSmallMemoryBucket(
    unsigned int cbSlot, unsigned int cSlots, void *pStorage, void *pBitmap)

{
    unsigned int cbStorage;
    unsigned int cBitmapWords;
    int fSmallBuckets;

    m_pVtable = g_aMemoryArenaInitialLockVtable;
    InitializeCriticalSection(m_abCriticalSection);
    m_pVtable = g_aSmallMemoryBucketVtable;
    m_pChildBucket = 0;
    m_pParentBucket = 0;
    m_cbSlot = cbSlot;
    m_cSlots = cSlots;
    m_cAllocated = 0;
    m_cAllocations = 0;
    m_cMaximumAllocations = 0;
    m_wFlags = 0;
    m_nReserved42 = 0;
    cbStorage = cbSlot * cSlots;
    m_cbStorage = cbStorage;
    m_cbStorageAvailable = cbStorage;
    cBitmapWords = (cSlots + 0x1f) >> 5;
    m_cBitmapWords = cBitmapWords;

    fSmallBuckets = g_fSmallMemoryBucketTableEnabled;
    if (pStorage == 0) {
        g_fSmallMemoryBucketTableEnabled = 0;
        pStorage = AllocateVSMemBlock(cbStorage);
        g_fSmallMemoryBucketTableEnabled = fSmallBuckets;
        m_wFlags |= 2;
    }
    if (pBitmap == 0) {
        g_fSmallMemoryBucketTableEnabled = 0;
        pBitmap = AllocateVSMemBlock(cBitmapWords * sizeof(unsigned int));
        g_fSmallMemoryBucketTableEnabled = fSmallBuckets;
        m_wFlags |= 1;
    }

    m_pStorage = pStorage;
    m_pBitmap = pBitmap;
    if (pBitmap != 0) {
        FillMemoryByte(pBitmap, 0, cBitmapWords * sizeof(unsigned int));
    }
    m_nReserved50 = 0;
    m_nReserved52 = 0;
    return this;
}

// FUNCTION: LEMBALL 0x00472dc0
static void LEMBALL_FASTCALL DestroySmallMemoryBucket(SmallMemoryBucket *pBucket) {
    int fSmallBuckets;

    pBucket->m_pVtable = g_aSmallMemoryBucketVtable;
    fSmallBuckets = g_fSmallMemoryBucketTableEnabled;
    g_fSmallMemoryBucketTableEnabled = 0;
    if ((pBucket->m_wFlags & 2) != 0 && pBucket->m_pStorage != 0) {
        FreeVSMemBlock(pBucket->m_pStorage);
        pBucket->m_pStorage = 0;
    }
    if ((pBucket->m_wFlags & 1) != 0 && pBucket->m_pBitmap != 0) {
        FreeVSMemBlock(pBucket->m_pBitmap);
        pBucket->m_pBitmap = 0;
    }
    g_fSmallMemoryBucketTableEnabled = fSmallBuckets;
    pBucket->m_pVtable = g_aMemoryArenaInitialLockVtable;
    DeleteCriticalSection(pBucket->m_abCriticalSection);
}

// FUNCTION: LEMBALL 0x00472e40
int SmallMemoryBucket::AllocateSlot(void **ppvSlot) {
    SmallMemoryBucket *pBucket;
    unsigned int i;

    pBucket = this;
    for (;;) {
        EnterMemoryArenaCriticalSection(pBucket);
        if ((pBucket->m_wFlags & 4) == 0)
            break;
        if (pBucket->m_pChildBucket == 0)
            AllocateSmallMemoryChildBucket(pBucket);
        LeaveMemoryArenaCriticalSection(pBucket);
        pBucket = pBucket->m_pChildBucket;
        if (pBucket == 0) {
            *ppvSlot = 0;
            return 0;
        }
    }

    for (i = 0; i < pBucket->m_cBitmapWords; ++i) {
        unsigned int dwWord = ((unsigned int *)pBucket->m_pBitmap)[i];
        if (dwWord != 0xffffffffU) {
            unsigned int j;
            for (j = 0; j < 32; ++j) {
                unsigned int dwMask = 1U << j;
                if ((dwWord & dwMask) == 0) {
                    unsigned int nSlot = i * 32 + j;
                    if (nSlot >= pBucket->m_cSlots)
                        break;
                    ((unsigned int *)pBucket->m_pBitmap)[i] = dwWord | dwMask;
                    pBucket->m_cbStorageAvailable -= pBucket->m_cbSlot;
                    pBucket->m_cAllocated += 1;
                    pBucket->m_cAllocations += 1;
                    if (pBucket->m_cMaximumAllocations < pBucket->m_cAllocations)
                        pBucket->m_cMaximumAllocations = pBucket->m_cAllocations;
                    if (pBucket->m_cbStorageAvailable == 0)
                        pBucket->m_wFlags |= 4;
                    *ppvSlot = (char *)pBucket->m_pStorage + nSlot * pBucket->m_cbSlot;
                    LeaveMemoryArenaCriticalSection(pBucket);
                    return 1;
                }
            }
        }
    }
    *ppvSlot = 0;
    pBucket->m_wFlags |= 4;
    LeaveMemoryArenaCriticalSection(pBucket);
    return 0;
}

// FUNCTION: LEMBALL 0x00472f00
int SmallMemoryBucket::FreeSlot(void *pvSlot) {
    SmallMemoryBucket *pBucket;
    unsigned int nSlot;
    unsigned int i;
    unsigned int j;
    unsigned int dwMask;

    pBucket = this;
    for (;;) {
        EnterMemoryArenaCriticalSection(pBucket);
        if (pvSlot >= pBucket->m_pStorage &&
            (char *)pvSlot < (char *)pBucket->m_pStorage + pBucket->m_cbStorage)
            break;
        LeaveMemoryArenaCriticalSection(pBucket);
        if (pBucket->m_pChildBucket == 0)
            return 0;
        pBucket = pBucket->m_pChildBucket;
    }
    if (pvSlot < pBucket->m_pStorage ||
        (char *)pvSlot >= (char *)pBucket->m_pStorage + pBucket->m_cbStorage)
    {
        LeaveMemoryArenaCriticalSection(pBucket);
        return 0;
    }
    nSlot = (unsigned int)(((char *)pvSlot - (char *)pBucket->m_pStorage) / pBucket->m_cbSlot);
    i = nSlot >> 5;
    j = nSlot & 31;
    dwMask = 1U << j;
    if ((((unsigned int *)pBucket->m_pBitmap)[i] & dwMask) == 0) {
        LeaveMemoryArenaCriticalSection(pBucket);
        return 0;
    }
    ((unsigned int *)pBucket->m_pBitmap)[i] &= ~dwMask;
    pBucket->m_cbStorageAvailable += pBucket->m_cbSlot;
    pBucket->m_cAllocations -= 1;
    pBucket->m_wFlags &= (unsigned short)~4;
    LeaveMemoryArenaCriticalSection(pBucket);
    return 1;
}

// FUNCTION: LEMBALL 0x00473050
static SmallMemoryBucket *LEMBALL_FASTCALL AllocateSmallMemoryChildBucket(
    SmallMemoryBucket *pParentBucket)

{
    int fSmallBuckets;
    SmallMemoryBucket *pChildBucket;
    void *pStorage;

    fSmallBuckets = g_fSmallMemoryBucketTableEnabled;
    g_fSmallMemoryBucketTableEnabled = 0;
    pStorage = AllocateVSMemBlock(0x54);
    pChildBucket = 0;
    if (pStorage != 0) {
        pChildBucket = ((SmallMemoryBucket *)pStorage)->ConstructSmallMemoryBucket(
            pParentBucket->m_cbSlot, pParentBucket->m_cSlots, 0, 0);
    }
    g_fSmallMemoryBucketTableEnabled = fSmallBuckets;
    pParentBucket->m_pChildBucket = pChildBucket;
    if (pChildBucket != 0)
        pChildBucket->m_pParentBucket = pParentBucket;
    return pChildBucket;
}

// FUNCTION: LEMBALL 0x00473180
SmallMemoryBucketTable *LEMBALL_FASTCALL ConstructSmallMemoryBucketTable(
    SmallMemoryBucketTable *pTable)

{
    int i;
    int iFirstBucket;
    int cBuckets;
    unsigned int cbSlot;

    iFirstBucket = (int)g_StartupGraphicsDriverConfig.m_dwReserved1;
    cBuckets = iFirstBucket + (int)g_StartupGraphicsDriverConfig.m_cItems;
    if (cBuckets > 7) {
        cBuckets = 7;
    }
    pTable->m_cBuckets = cBuckets;
    for (i = 0; i < 7; ++i) {
        pTable->m_apBuckets[i] = 0;
        pTable->m_acbBucketSizes[i] = 0;
    }

    cbSlot = 1U << (iFirstBucket + 1);
    for (i = iFirstBucket; i < cBuckets; ++i) {
        SmallMemoryBucket *pBucket;

        pTable->m_acbBucketSizes[i] = cbSlot;
        if (g_adwStartupGraphicsBucketSizeTable[i] != 0) {
            pBucket = (SmallMemoryBucket *)AllocateVSMemBlock(sizeof(SmallMemoryBucket));
            if (pBucket != 0) {
                pTable->m_apBuckets[i] =
                    pBucket->ConstructSmallMemoryBucket(
                        cbSlot, (unsigned int)g_adwStartupGraphicsBucketSizeTable[i], 0, 0);
            }
            if (g_cbSmallMemoryBucketUpperBound < cbSlot) {
                g_cbSmallMemoryBucketUpperBound = cbSlot;
            }
        }
        cbSlot <<= 1;
    }
    return pTable;
}

// FUNCTION: LEMBALL 0x00473280
void LEMBALL_FASTCALL DestroySmallMemoryBucketTable(SmallMemoryBucketTable *pTable) {
    int i;
    int iFirstBucket;

    g_fSmallMemoryBucketTableEnabled = 0;
    iFirstBucket = (int)g_StartupGraphicsDriverConfig.m_dwReserved1;
    for (i = iFirstBucket; i < pTable->m_cBuckets; ++i) {
        if (pTable->m_apBuckets[i] != 0) {
            DestroySmallMemoryBucket(pTable->m_apBuckets[i]);
            FreeVSMemBlock(pTable->m_apBuckets[i]);
            pTable->m_apBuckets[i] = 0;
        }
    }
}

// FUNCTION: LEMBALL 0x004732d0
int SmallMemoryBucketTable::AllocateFromSmallMemoryBucketTable(int cbBlock, const char *pszName) {
    unsigned int *pcbBucketSize;
    int iFirstBucket;
    int i;
    int cBuckets;
    int nPreviousSize;

    iFirstBucket = (int)g_StartupGraphicsDriverConfig.m_dwReserved1;
    cBuckets = m_cBuckets;
    nPreviousSize = 0;
    if (iFirstBucket < cBuckets) {
        pcbBucketSize = &m_acbBucketSizes[iFirstBucket];
        i = iFirstBucket;
        do {
            if (nPreviousSize < cbBlock &&
                cbBlock <= (int)*pcbBucketSize && m_apBuckets[i] != 0) {
                void *pvSlot;
                if (m_apBuckets[i]->AllocateSlot(&pvSlot) != 0) {
                    return (int)(unsigned long)pvSlot;
                }
                return 0;
            }
            nPreviousSize = (int)*pcbBucketSize++;
            ++i;
        } while (i < cBuckets);
    }
    return 0;
}

// FUNCTION: LEMBALL 0x00473340
int SmallMemoryBucketTable::FreeToSmallMemoryBucketTable(void *pBlock) {
    int iFirstBucket;
    int i;

    iFirstBucket = (int)g_StartupGraphicsDriverConfig.m_dwReserved1;
    for (i = iFirstBucket; i < m_cBuckets; ++i) {
        if (m_apBuckets[i] != 0 && m_apBuckets[i]->FreeSlot(pBlock) != 0) {
            return 1;
        }
    }
    return 0;
}
