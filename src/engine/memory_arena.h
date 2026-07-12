#ifndef LEMBALL_MEMORY_ARENA_H
#define LEMBALL_MEMORY_ARENA_H

#include "platform/win32.h"

struct VSINIT_FormattedOutputStream;
struct SMALL_MEMORY_BUCKET;

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

    CMemoryBlock *ConstructMemoryBlockBase(void *pArena,
                                           CMemoryBlock *pPreviousBlock,
                                           const char *pszName,
                                           unsigned int cbBlock);
    CMemoryBlock *ConstructMemoryArenaBlock(void *pArena,
                                             CMemoryBlock *pPreviousBlock,
                                             const char *pszName,
                                             unsigned int cbBlock);
};

struct CArena {
    void **m_pVtable;
    int m_nMagic;
    void **m_pLockVtable;
    unsigned char m_abCriticalSection[0x18];
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

    CArena *ConstructMemoryArena(unsigned int cbStorage,
                                  const char *pszName,
                                  void *pParentArena,
                                  void *pReserved);
    CArena *ConstructMemoryArenaBaseState(unsigned int cbStorage,
                                           const char *pszName,
                                           void *pParentArena,
                                           void *pReserved);
    int AllocateMemoryArenaBlock(void **ppvBlock,
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

struct SMALL_MEMORY_BUCKET_TABLE {
    SMALL_MEMORY_BUCKET *m_apBuckets[7];
    unsigned int m_acbBucketSizes[7];
    int m_cBuckets;

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
void *WriteMemoryArenaReport(void *pArena, VSINIT_FormattedOutputStream *pOutputStream);
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
