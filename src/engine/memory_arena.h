#ifndef LEMBALL_MEMORY_ARENA_H
#define LEMBALL_MEMORY_ARENA_H

#include "platform/win32.h"

struct VSINIT_FormattedOutputStream;
struct SMALL_MEMORY_BUCKET;

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
void CopyMemoryBytes(void *pvTarget, const void *pvSource, unsigned int cbCopy);
void *ConstructMemoryArenaBaseState(void *pArena,
                                   unsigned int cbStorage,
                                   const char *pszName,
                                   void *pParentArena,
                                   void *pReserved);
void ReleaseMemoryArenaBlockLists(void *pArena);
void DestroyMemoryArenaBaseState(void *pArena);
int FreeMemoryArenaBlock(void *pArena, void *pvBlock);
int AllocateChildMemoryArena(void *pArena, void **ppChildArena, unsigned int cbChildArena);
int ReleaseChildMemoryArena(void *pArena, void *pChildArena);
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
