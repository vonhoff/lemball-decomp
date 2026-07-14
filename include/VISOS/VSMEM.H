#ifndef LEMBALL_MEMORY_ARENA_H
#define LEMBALL_MEMORY_ARENA_H

#include "VISOS/WIN32.H"

struct VsInitFormattedOutputStream;
struct SmallMemoryBucket;

// SIZE 0x28
struct CMemoryBlock {
    void **m_pVtable;                     // 0x00
    int m_nMagic;                         // 0x04
    void *m_pPayload;                     // 0x08
    unsigned int m_cbPayload;             // 0x0c
    void *m_pArena;                       // 0x10
    CMemoryBlock *m_pNextFree;            // 0x14
    CMemoryBlock *m_pPreviousFree;        // 0x18
    CMemoryBlock *m_pNextAddress;         // 0x1c
    CMemoryBlock *m_pPreviousAddress;     // 0x20
    unsigned int m_dwFlags;               // 0x24

    CMemoryBlock *ConstructMemoryBlockBase(void *pArena,
                                           CMemoryBlock *pPreviousBlock,
                                           const char *pszName,
                                           unsigned int cbBlock);
    CMemoryBlock *ConstructMemoryArenaBlock(void *pArena,
                                             CMemoryBlock *pPreviousBlock,
                                             const char *pszName,
                                             unsigned int cbBlock);
};

// SIZE 0x50
struct CArena {
    void **m_pVtable;                         // 0x00
    int m_nMagic;                             // 0x04
    void **m_pLockVtable;                     // 0x08
    unsigned char m_abCriticalSection[0x18]; // 0x0c
    void *m_pStorage;                         // 0x24
    unsigned int m_cbStorage;                 // 0x28
    unsigned int m_cbFree;                    // 0x2c
    void *m_pStatusEntry;                     // 0x30
    CMemoryBlock *m_pFirstFreeBlock;          // 0x34
    CMemoryBlock *m_pLastFreeBlock;           // 0x38
    CMemoryBlock *m_pFirstAddressBlock;       // 0x3c
    CMemoryBlock *m_pLastAddressBlock;        // 0x40
    CArena *m_pParentArena;                   // 0x44
    const char *m_pszName;                    // 0x48
    CArena *m_pFirstChildArena;               // 0x4c

    CArena *ConstructMemoryArena(unsigned int cbStorage,
                                  const char *pszName,
                                  void *pParentArena,
                                  void *pReserved);
    CArena *ConstructMemoryArenaBaseState(unsigned int cbStorage,
                                           const char *pszName,
                                           void *pParentArena,
                                           void *pReserved);
    int Allocate(void **ppvBlock,
                                 unsigned int cbBlock,
                                 const char *pszDescription);
    int FreeMemoryArenaBlock(void *pvBlock);
    int AllocateChildMemoryArena(void **ppChildArena,
                                 unsigned int cbChildArena,
                                 const char *pszName);
    int ReleaseChildMemoryArena(void *pChildArena);
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

// SIZE 0x3c
struct SmallMemoryBucketTable {
    SmallMemoryBucket *m_apBuckets[7]; // 0x00
    unsigned int m_acbBucketSizes[7];    // 0x1c
    int m_cBuckets;                      // 0x38

    int AllocateFromSmallMemoryBucketTable(unsigned int cbBlock);
    int FreeToSmallMemoryBucketTable(void *pBlock);
};

int InitializeMasterMainRamArena(void);
void ShutdownMasterMainRamArena(void);
void *AllocateVSMemBlockImpl(unsigned int cbBlock);
void FreeVSMemBlockImpl(void *pvBlock);
void *AllocateVSMemBlock(unsigned int cbBlock);
void FreeVSMemBlock(void *pvBlock);
long LEMBALL_FASTCALL CalculateMemoryArenaAvailableBytes(void *pArena);
void *WriteMemoryArenaReport(void *pArena, VsInitFormattedOutputStream *pOutputStream);
unsigned int GetMemoryArenaPayloadByteCounter(void *pArena);
void *ReturnSuppliedPlacementStorage(unsigned int cbStorage, void *pvStorage);
void FillMemoryByte(void *pvTarget, unsigned char chValue, unsigned int cbTarget);
void __stdcall CopyMemoryBytes(void *pvTarget, const void *pvSource, unsigned int cbCopy);
void *ConstructMemoryArenaBaseState(void *pArena,
                                   unsigned int cbStorage,
                                   const char *pszName,
                                   void *pParentArena,
                                   void *pReserved);
void ReleaseMemoryArenaBlockLists(void *pArena);
void DestroyMemoryArenaBaseState(void *pArena);
int PASCAL HasMemoryBlockMagic(void *pBlock);
int PASCAL IsUsedMemoryBlock(void *pBlock);
int PASCAL IsFreeMemoryBlock(void *pBlock);
void *ConstructMemoryArena(void *pArena,
                           unsigned int cbStorage,
                           const char *pszName,
                           void *pParentArena,
                           void *pReserved);
void DestroyMemoryArena(void *pArena);
void *PlacementConstructMemoryArenaWithStorage(void *pvStorage,
                                               unsigned int cbStorage,
                                               const char *pszName,
                                               void *pParentArena,
                                               void *pReserved);
void *PlacementConstructMemoryArenaBlock(void *pvStorage,
                                         void *pArena,
                                         void *pPreviousBlock,
                                         const char *pszName,
                                         unsigned int cbBlock);
void RestoreMemoryBlockBaseVtable(void *pBlock);
int IsPointerInsideMemoryBlock(void *pBlock, void *pvPointer);
void NoOpCallback0045A610(void);
void *ReturnPlacementBlockStorage(unsigned int cbStorage, void *pvStorage);
void *ConstructMemoryArenaBlock(void *pBlock,
                                void *pArena,
                                void *pPreviousBlock,
                                const char *pszName,
                                unsigned int cbBlock);
void *LEMBALL_FASTCALL DestroyMemoryArenaBaseStateReturnThis(
    void *pArena, int nUnused, int fDelete);
void *LEMBALL_FASTCALL DestroyMemoryArenaReturnThis(
    void *pArena, int nUnused, int fDelete);
void *RestoreMemoryBlockBaseVtableReturnThis(void *pBlock);
int IsPointerInsideManagedMemoryRegions(void *pvPointer);
unsigned int GetMemoryArenaHeaderSize(void);
unsigned int GetMemoryBlockHeaderSize(void);

extern void *g_pMainMemoryArena;

#endif
