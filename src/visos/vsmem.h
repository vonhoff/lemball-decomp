#ifndef LEMBALL_VSMEM_H
#define LEMBALL_VSMEM_H

struct VSINIT_FormattedOutputStream;

int InitializeMasterMainRamArena(void);
void ShutdownMasterMainRamArena(void);
void *AllocateVSMemBlockImpl(unsigned int cbBlock);
void FreeVSMemBlockImpl(void *pvBlock);
void *AllocateVSMemBlock(unsigned int cbBlock);
void FreeVSMemBlock(void *pvBlock);
long CalculateMemoryArenaAvailableBytes(void *pArena);
void *WriteMemoryArenaReport(void *pArena, VSINIT_FormattedOutputStream *pOutputStream);
unsigned int GetMemoryArenaPayloadByteCounter(void *pArena);
void *ReturnSuppliedPlacementStorage(unsigned int cbStorage, void *pvStorage);
void FillMemoryByte(void *pvTarget, unsigned char chValue, unsigned int cbTarget);
void CopyMemoryBytes(void *pvTarget, const void *pvSource, unsigned int cbCopy);
void *ConstructMemoryArenaBaseState(void *pArena,
                                   unsigned int cbStorage,
                                   const char *pszName,
                                   void *pParentArena,
                                   void *pReserved);
void ReleaseMemoryArenaBlockLists(void *pArena);
void DestroyMemoryArenaBaseState(void *pArena);
int ConsumeTrailingMemoryBlockIfAdjacent(void *pArena, void *pBlock, void *pNextBlock);
int AppendTailMemoryBlockAddressList(void *pArena, void *pBlock);
int InsertMemoryBlockAfterFreeListNode(void *pArena, void *pBlock, void *pPreviousBlock);
int InsertMemoryBlockSortedByAddress(void *pArena, void *pBlock);
int UnlinkMemoryBlockFromAddressListLinks(void *pArena, void *pBlock);
int UnlinkMemoryBlockFromFreeList(void *pArena, void *pBlock);
int UnlinkMemoryBlockFromAddressList(void *pArena, void *pBlock);
void *FindSmallestFreeMemoryBlockAtLeast(void *pArena, unsigned int cbPayload);
int AllocateMemoryArenaBlock(void *pArena, void **ppvBlock, unsigned int cbBlock, const char *pszDescription);
int FreeMemoryArenaBlock(void *pArena, void *pvBlock);
int AllocateChildMemoryArena(void *pArena, void **ppChildArena, unsigned int cbChildArena);
int ReleaseChildMemoryArena(void *pArena, void *pChildArena);
int IsPointerInsideMemoryArenaStorage(void *pArena, void *pvPointer);
int HasMemoryBlockMagic(void *pBlock);
int IsUsedMemoryBlock(void *pBlock);
int IsFreeMemoryBlock(void *pBlock);
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
void *ConstructMemoryBlockBase(void *pBlock, void *pArena, void *pPreviousBlock);
void RestoreMemoryBlockBaseVtable(void *pBlock);
int IsPointerInsideMemoryBlock(void *pBlock, void *pvPointer);
void NoOpCallback0045A610(void);
void *ReturnPlacementBlockStorage(unsigned int cbStorage, void *pvStorage);
void *ConstructMemoryArenaBlock(void *pBlock,
                                void *pArena,
                                void *pPreviousBlock,
                                const char *pszName,
                                unsigned int cbBlock);
void *DestroyMemoryArenaBaseStateReturnThis(void *pArena);
void *DestroyMemoryArenaReturnThis(void *pArena);
void *RestoreMemoryBlockBaseVtableReturnThis(void *pBlock);
int IsPointerInsideManagedMemoryRegions(void *pvPointer);
unsigned int GetMemoryArenaHeaderSize(void);
unsigned int GetMemoryBlockHeaderSize(void);

extern void *g_pMainMemoryArena;

#endif
