#ifndef LEMBALL_MEMORY_ARENA_H
#define LEMBALL_MEMORY_ARENA_H

#include "Platform/Windows/Mixed/Engine/CORE/WIN32.H"

struct VsInitFormattedOutputStream;
struct CBucket;

// SIZE 0x28
struct CMBlock {
	void** m_pVtable;            // 0x00
	int m_nMagic;                // 0x04
	void* m_pPayload;            // 0x08
	unsigned int m_cbPayload;    // 0x0c
	void* m_pArena;              // 0x10
	CMBlock* m_pNextFree;        // 0x14
	CMBlock* m_pPreviousFree;    // 0x18
	CMBlock* m_pNextAddress;     // 0x1c
	CMBlock* m_pPreviousAddress; // 0x20
	unsigned int m_dwFlags;      // 0x24

	CMBlock* ConstructMemoryBlockBase(void* pArena, CMBlock* pPreviousBlock, const char* pszName, unsigned int cbBlock);
	CMBlock* ConstructMemoryArenaBlock(void* pArena,
									   CMBlock* pPreviousBlock,
									   const char* pszName,
									   unsigned int cbBlock);
	void SetMemoryBlockDescription(const char* pszDescription);
};

// SIZE 0x50
struct CArena {
	void** m_pVtable;                        // 0x00
	int m_nMagic;                            // 0x04
	void** m_pLockVtable;                    // 0x08
	unsigned char m_abCriticalSection[0x18]; // 0x0c
	void* m_pStorage;                        // 0x24
	unsigned int m_cbStorage;                // 0x28
	unsigned int m_cbFree;                   // 0x2c
	void* m_pStatusEntry;                    // 0x30
	CMBlock* m_pFirstBlock;                  // 0x34
	CMBlock* m_pLastBlock;                   // 0x38
	CMBlock* m_pFirstFreeBlock;              // 0x3c
	CMBlock* m_pLastFreeBlock;               // 0x40
	CArena* m_pParentArena;                  // 0x44
	const char* m_pszName;                   // 0x48
	CArena* m_pFirstChildArena;              // 0x4c

	CArena* ConstructMemoryArena(unsigned int cbStorage, const char* pszName, void* pParentArena, void* pReserved);
	CArena* Construct(unsigned int cbStorage, const char* pszName, void* pParentArena, void* pReserved);
	int Allocate(void** ppvBlock, unsigned int cbBlock, const char* pszDescription);
	int Free(void* pvBlock);
	void DeleteLists(void);
	unsigned int GetAllocSize(void);
	long GetFreeSize(void);
	int AllocateArena(void** ppChildArena, unsigned int cbChildArena, const char* pszName);
	int FreeArena(void* pChildArena);
	int CheckAndAmalgamate(void* pBlock, void* pNextBlock);
	int AddToFreeList(void* pBlock);
	int AddToBlockList(void* pBlock, void* pPreviousBlock);
	int AddToArenaList(void* pBlock);
	int RemoveFromFreeList(void* pBlock);
	int RemoveFromBlockList(void* pBlock);
	int RemoveFromArenaList(void* pBlock);
	void* FindSmallestBlock(unsigned int cbPayload);
	int CheckValidPointer(void* pvPointer);
	void* StreamOut(VsInitFormattedOutputStream* pOutputStream);
};

// SIZE 0x3c
struct CSmallMemory {
	CBucket* m_apBuckets[7];          // 0x00
	unsigned int m_acbBucketSizes[7]; // 0x1c
	int m_cBuckets;                   // 0x38

	int AllocateFromCSmallMemory(int cbBlock, const char* pszName);
	int FreeToCSmallMemory(void* pBlock);
};

int InitializeMasterMainRamArena(void);
void ShutdownMasterMainRamArena(void);
void* AllocateVSMemBlockImpl(unsigned int cbBlock);
void FreeVSMemBlockImpl(void* pvBlock);
void* AllocateVSMemBlock(unsigned int cbBlock);
void FreeVSMemBlock(void* pvBlock);
void* NewCArenaPlacement(unsigned int cbStorage, void* pvStorage);
void __stdcall MemSetCArena(void* pvTarget, unsigned char chValue, unsigned int cbTarget);
void __stdcall MemCopyCArena(void* pvTarget, const void* pvSource, unsigned int cbCopy);
void* ConstructMemoryArenaBaseState(void* pArena,
									unsigned int cbStorage,
									const char* pszName,
									void* pParentArena,
									void* pReserved);
void LEMBALL_FASTCALL DestroyCArena(void* pArena);
int PASCAL HasMemoryBlockMagic(void* pBlock);
int PASCAL IsUsedMemoryBlock(void* pBlock);
int PASCAL IsFreeMemoryBlock(void* pBlock);
void* ConstructMemoryArena(void* pArena,
						   unsigned int cbStorage,
						   const char* pszName,
						   void* pParentArena,
						   void* pReserved);
void LEMBALL_FASTCALL DestroyMemoryArena(void* pArena);
void* LEMBALL_STDCALL PlacementConstructMemoryArenaWithStorage(void* pvStorage,
															   unsigned int cbStorage,
															   const char* pszName,
															   void* pParentArena,
															   void* pReserved);
void* LEMBALL_STDCALL PlacementConstructMemoryArenaBlock(void* pvStorage,
														 void* pArena,
														 void* pPreviousBlock,
														 const char* pszName,
														 unsigned int cbBlock);
void LEMBALL_FASTCALL RestoreMemoryBlockBaseVtable(void* pBlock);
int IsPointerInsideMemoryBlock(void* pBlock, void* pvPointer);
// FUNCTION: LEMBALL 0x0045a5b0
// VSMEM_ReportBlockStub
void* LEMBALL_FASTCALL VSMEM_ReportBlockStub(void* pBlock, int nUnused, VsInitFormattedOutputStream* pOutputStream);
void* ReturnPlacementBlockStorage(unsigned int cbStorage, void* pvStorage);
void* ConstructMemoryArenaBlock(void* pBlock,
								void* pArena,
								void* pPreviousBlock,
								const char* pszName,
								unsigned int cbBlock);
void* LEMBALL_FASTCALL DestroyCArenaReturnThis(void* pArena, int nUnused, int fDelete);
void* LEMBALL_FASTCALL DestroyMemoryArenaReturnThis(void* pArena, int nUnused, int fDelete);
void* LEMBALL_FASTCALL RestoreMemoryBlockBaseVtableReturnThis(void* pBlock, int nUnused, int fDelete);
int CheckValidPointer(void* pvPointer);
unsigned int GetMemoryArenaHeaderSize(void);
unsigned int GetMemoryBlockHeaderSize(void);

extern void* g_pMainMemoryArena;

#endif
