#include "Visos/Generic/Memory.h"

#include "Platform/Windows/Mixed/Engine/CORE/VSINIT.H"
#include "Control/Options.h"

#include <stdlib.h>
#include <string.h>

DECOMP_SIZE_ASSERT(CMBlock, 0x28);
DECOMP_SIZE_ASSERT(CArena, 0x50);
DECOMP_SIZE_ASSERT(CSmallMemory, 0x3c);

#pragma function(memcpy)

extern void TriggerReleaseAssertFailure(const char* pszExpression, const char* pszFile, int nLine);

typedef void(LEMBALL_FASTCALL* VSMEM_LockProc)(void* pLockVtable);
typedef unsigned int (*VSMEM_SizeProc)(void);
typedef void*(LEMBALL_FASTCALL* VSMEM_ReportProc)(void* pObject,
												  int nUnused,
												  VsInitFormattedOutputStream* pOutputStream);
typedef void*(LEMBALL_FASTCALL* VSMEM_DeleteProc)(void* pObject, int nUnused, int fDelete);
typedef void (*VSMEM_StatusUpdateProc)(void* pStatusEntry, unsigned int cbValue);

struct CBucket {
	void** m_pVtable;
	unsigned char m_abCriticalSection[0x18];
	CBucket* m_pChildBucket;
	CBucket* m_pParentBucket;
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
	void* m_pBitmap;
	void* m_pStorage;
	unsigned short m_nNextFreeWord;
	unsigned short m_nNextFreeBit;

	CBucket* ConstructCBucket(unsigned int cbSlot, unsigned int cSlots, void* pStorage, void* pBitmap);
	int AllocateSlot(void** ppvSlot);
	int FreeSlot(void* pvSlot);
	int ContainsPointer(unsigned int nPointer);
};

// GLOBAL: LEMBALL 0x004aa100
void* g_pMainMemoryArena;
static void* g_pMainMemoryArenaStorage = 0;
static HGLOBAL g_hMainMemoryArenaStorage = 0;
static int g_cbMainArenaCapacity = 0;
static int g_cbMainArenaInUse = 0;
static int g_fMainArenaReady = 0;
// GLOBAL: LEMBALL 0x004aa0fc
void* g_pCSmallMemory;
// GLOBAL: LEMBALL 0x004a1178
unsigned int g_cbCBucketUpperBound = 0;
// GLOBAL: LEMBALL 0x004a1174
const char* g_pszCBucketAllocTag = "new";
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
void* LEMBALL_FASTCALL VSMEM_ReportBlockStub(void* pBlock, int nUnused, VsInitFormattedOutputStream* pOutputStream);
void* LEMBALL_FASTCALL VSMEM_DeleteBlockStub(void* pBlock, int nUnused, int fDelete);
void LEMBALL_FASTCALL RestoreMemoryBlockBaseVtable(void* pBlock);
void* LEMBALL_FASTCALL RestoreMemoryBlockBaseVtableReturnThis(void* pBlock, int nUnused, int fDelete);
static void UpdateMainMemoryArenaFreeCounter(void);
CBucket* LEMBALL_FASTCALL AllocateSmallMemoryChildBucket(CBucket* pParentBucket);

struct CArenaVtable {
	void* (CArena::*m_pWriteReport)(VsInitFormattedOutputStream*);
	void* m_pDelete;
	int (CArena::*m_pAllocate)(void**, unsigned int, const char*);
	int (CArena::*m_pFree)(void*);
	void* m_apRemaining[7];
};

static CArenaVtable g_MemoryArenaBaseStateVtable = {
	&CArena::StreamOut,
	(void*) DestroyCArenaReturnThis,
	&CArena::Allocate,
	&CArena::Free,
	{
		(void*) MemSetCArena,
		(void*) MemCopyCArena,
		(void*) _purecall,
		(void*) _purecall,
		(void*) _purecall,
		(void*) _purecall,
		(void*) _purecall,
	},
};
static CArenaVtable g_MainMemoryArenaVtable = {
	&CArena::StreamOut,
	(void*) DestroyMemoryArenaReturnThis,
	&CArena::Allocate,
	&CArena::Free,
	{
		(void*) MemSetCArena,
		(void*) MemCopyCArena,
		(void*) GetMemoryArenaHeaderSize,
		(void*) GetMemoryBlockHeaderSize,
		(void*) PlacementConstructMemoryArenaWithStorage,
		(void*) PlacementConstructMemoryArenaBlock,
		(void*) _purecall,
	},
};
static void* g_aMemoryArenaInitialVtable[1] = {
	(void*) _purecall,
};
static void* g_aMemoryArenaInitialLockVtable[4] = {
	(void*) EnterObjectCriticalSection,
	(void*) LeaveObjectCriticalSection,
	(void*) EnterObjectCriticalSection,
	(void*) LeaveObjectCriticalSection,
};
static void* g_aMainMemoryArenaLockVtable[2] = {
	(void*) EnterObjectCriticalSection,
	(void*) LeaveObjectCriticalSection,
};
// GLOBAL: LEMBALL 0x00498940
static void* g_aMemoryBlockVtables[2][2] = {
	{(void*) VSMEM_ReportBlockStub, (void*) RestoreMemoryBlockBaseVtableReturnThis},
	{(void*) VSMEM_ReportBlockStub, (void*) VSMEM_DeleteBlockStub},
};
static void* g_aCBucketVtable[2] = {
	(void*) EnterObjectCriticalSection,
	(void*) LeaveObjectCriticalSection,
};

CSmallMemory* LEMBALL_FASTCALL ConstructCSmallMemory(CSmallMemory* pTable);
void LEMBALL_FASTCALL DestroyCSmallMemory(CSmallMemory* pTable);

// FUNCTION: LEMBALL 0x004599f0
CArena* CArena::Construct(unsigned int cbStorage, const char* pszName, void* pParentArena, void* pReserved)

{
	(void) cbStorage;
	(void) pReserved;
	m_pVtable = g_aMemoryArenaInitialVtable;
	m_pLockVtable = g_aMemoryArenaInitialLockVtable;
	InitializeCriticalSection(m_abCriticalSection);
	m_pVtable = (void**) &g_MemoryArenaBaseStateVtable;
	m_pLockVtable = g_aMainMemoryArenaLockVtable;
	m_pszName = pszName;
	m_pParentArena = (CArena*) pParentArena;
	m_pFirstBlock = 0;
	m_pLastBlock = 0;
	m_pFirstFreeBlock = 0;
	m_pLastFreeBlock = 0;
	m_pFirstChildArena = 0;
	m_pStatusEntry = 0;
	return this;
}
// FUNCTION: LEMBALL 0x00459a40
void CArena::DeleteLists(void)
{
	void** pLockVtable;
	void* pBlock;
	void* pNextBlock;

	pLockVtable = m_pLockVtable;
	((VSMEM_LockProc) pLockVtable[0])((char*) this + 8);
	pBlock = m_pFirstBlock;
	m_pFirstBlock = 0;
	while (pBlock != 0) {
		pNextBlock = ((CMBlock*) pBlock)->m_pNextFree;
		((VSMEM_DeleteProc) (*(void***) pBlock)[1])(pBlock, 0, 1);
		pBlock = pNextBlock;
	}
	pBlock = m_pFirstChildArena;
	m_pFirstChildArena = 0;
	while (pBlock != 0) {
		pNextBlock = ((CArena*) pBlock)->m_pFirstChildArena;
		((VSMEM_DeleteProc) (*(void***) pBlock)[1])(pBlock, 0, 1);
		pBlock = pNextBlock;
	}
	((VSMEM_LockProc) pLockVtable[1])((char*) this + 8);
}

// FUNCTION: LEMBALL 0x00459aa0
void LEMBALL_FASTCALL DestroyCArena(void* pArena)
{
	((CArena*) pArena)->m_pVtable = (void**) &g_MemoryArenaBaseStateVtable;
	((CArena*) pArena)->m_pLockVtable = g_aMainMemoryArenaLockVtable;
	DeleteCriticalSection(((CArena*) pArena)->m_abCriticalSection);
}

// FUNCTION: LEMBALL 0x00459b10
int CArena::CheckAndAmalgamate(void* pBlock, void* pNextBlock)
{
	VSMEM_SizeProc pfnGetBlockHeaderSize;
	CMBlock* pMemoryBlock;
	CMBlock* pNextMemoryBlock;
	unsigned int cbNextPayload;

	pMemoryBlock = (CMBlock*) pBlock;
	pNextMemoryBlock = (CMBlock*) pNextBlock;
	if ((char*) pMemoryBlock->m_pPayload + pMemoryBlock->m_cbPayload != (char*) pNextMemoryBlock) {
		return 0;
	}
	cbNextPayload = pNextMemoryBlock->m_cbPayload;
	pfnGetBlockHeaderSize = (VSMEM_SizeProc) m_pVtable[7];
	pMemoryBlock->m_cbPayload += cbNextPayload + pfnGetBlockHeaderSize();
	RemoveFromBlockList(pNextMemoryBlock);
	RemoveFromFreeList(pNextMemoryBlock);
	if (pNextMemoryBlock != 0) {
		((VSMEM_DeleteProc) pNextMemoryBlock->m_pVtable[1])(pNextMemoryBlock, 0, 1);
	}
	m_cbFree += pfnGetBlockHeaderSize();
	return 1;
}

// FUNCTION: LEMBALL 0x00459bd0
int CArena::AddToFreeList(void* pBlock)
{
	CMBlock* pMemoryBlock;
	CMBlock* pTail;

	pMemoryBlock = (CMBlock*) pBlock;
	if (m_pLastFreeBlock == 0) {
		m_pFirstFreeBlock = pMemoryBlock;
	}
	else {
		m_pLastFreeBlock->m_pNextAddress = pMemoryBlock;
	}
	pTail = m_pLastFreeBlock;
	pMemoryBlock->m_pNextAddress = 0;
	pMemoryBlock->m_pPreviousAddress = pTail;
	m_pLastFreeBlock = pMemoryBlock;
	return 1;
}

// FUNCTION: LEMBALL 0x00459c00
int CArena::AddToBlockList(void* pBlock, void* pPreviousBlock)
{
	CMBlock* pMemoryBlock;
	CMBlock* pNextBlock;
	CMBlock* pPreviousMemoryBlock;

	pMemoryBlock = (CMBlock*) pBlock;
	pPreviousMemoryBlock = (CMBlock*) pPreviousBlock;
	pNextBlock = 0;
	if (pPreviousMemoryBlock == 0) {
		m_pFirstBlock = pMemoryBlock;
	}
	else {
		pNextBlock = pPreviousMemoryBlock->m_pNextFree;
		pPreviousMemoryBlock->m_pNextFree = pMemoryBlock;
	}
	pMemoryBlock->m_pPreviousFree = pPreviousMemoryBlock;
	pMemoryBlock->m_pNextFree = pNextBlock;
	if (pNextBlock == 0) {
		m_pLastBlock = pMemoryBlock;
		return 1;
	}
	pNextBlock->m_pPreviousFree = pMemoryBlock;
	return 1;
}

// FUNCTION: LEMBALL 0x00459c40
int CArena::AddToArenaList(void* pBlock)
{
	CMBlock* pCurrentBlock;
	CMBlock* pMemoryBlock;
	CMBlock* pPreviousBlock;

	pMemoryBlock = (CMBlock*) pBlock;
	pCurrentBlock = (CMBlock*) m_pFirstChildArena;
	pPreviousBlock = 0;
	while (pCurrentBlock != 0 && pCurrentBlock <= pMemoryBlock) {
		pPreviousBlock = pCurrentBlock;
		pCurrentBlock = (CMBlock*) ((CArena*) pCurrentBlock)->m_pFirstChildArena;
	}
	if (pPreviousBlock == 0) {
		m_pFirstChildArena = (CArena*) pMemoryBlock;
	}
	else {
		((CArena*) pPreviousBlock)->m_pFirstChildArena = (CArena*) pMemoryBlock;
	}
	((CArena*) pMemoryBlock)->m_pFirstChildArena = (CArena*) pCurrentBlock;
	return 1;
}

// FUNCTION: LEMBALL 0x00459c80
int CArena::RemoveFromFreeList(void* pBlock)
{
	CMBlock* pMemoryBlock;
	CMBlock* pPreviousBlock;
	CMBlock* pNextBlock;

	pMemoryBlock = (CMBlock*) pBlock;
	pPreviousBlock = pMemoryBlock->m_pPreviousAddress;
	pNextBlock = pMemoryBlock->m_pNextAddress;
	if (pNextBlock != 0) {
		pNextBlock->m_pPreviousAddress = pPreviousBlock;
	}
	else {
		m_pLastFreeBlock = pPreviousBlock;
	}
	if (pPreviousBlock != 0) {
		pPreviousBlock->m_pNextAddress = pNextBlock;
		return 1;
	}
	m_pFirstFreeBlock = pNextBlock;
	return 1;
}

// FUNCTION: LEMBALL 0x00459cb0
int CArena::RemoveFromBlockList(void* pBlock)
{
	CMBlock* pMemoryBlock;
	CMBlock* pPreviousBlock;
	CMBlock* pNextBlock;

	pMemoryBlock = (CMBlock*) pBlock;
	pPreviousBlock = pMemoryBlock->m_pPreviousFree;
	pNextBlock = pMemoryBlock->m_pNextFree;
	if (pNextBlock != 0) {
		pNextBlock->m_pPreviousFree = pPreviousBlock;
	}
	else {
		m_pLastBlock = pPreviousBlock;
	}
	if (pPreviousBlock != 0) {
		pPreviousBlock->m_pNextFree = pNextBlock;
		return 1;
	}
	m_pFirstBlock = pNextBlock;
	return 1;
}

// FUNCTION: LEMBALL 0x00459ce0
int CArena::RemoveFromArenaList(void* pBlock)
{
	CArena* pChildArena;
	CArena* pCurrentBlock;
	CArena* pPreviousBlock;

	pChildArena = (CArena*) pBlock;
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
void* CArena::FindSmallestBlock(unsigned int cbPayload)
{
	CMBlock* pBestBlock;
	CMBlock* pBlock;

	pBestBlock = 0;
	for (pBlock = m_pLastFreeBlock; pBlock != 0; pBlock = pBlock->m_pPreviousAddress) {
		if (IsFreeMemoryBlock(pBlock) != 0 && cbPayload <= pBlock->m_cbPayload &&
			(pBestBlock == 0 || pBlock->m_cbPayload < pBestBlock->m_cbPayload)) {
			pBestBlock = pBlock;
		}
	}
	return pBestBlock;
}

// FUNCTION: LEMBALL 0x00459d70
int CArena::Allocate(void** ppvBlock, unsigned int cbBlock, const char* pszDescription)

{
	CArena* pMemoryArena;
	CMBlock* pMemoryBlock;
	CMBlock* pSplitMemoryBlock;
	VSMEM_SizeProc pfnGetBlockHeaderSize;
	VSMEM_StatusUpdateProc pfnUpdateStatusEntry;
	unsigned int cbAligned;
	unsigned int cbBlockHeader;
	unsigned int cbFree;
	unsigned int cbOldBlockPayload;
	unsigned int cbRemainingBlock;
	void* pStatusEntry;
	void** pLockVtable;

	pMemoryArena = this;
	pLockVtable = pMemoryArena->m_pLockVtable;
	((VSMEM_LockProc) pLockVtable[0])((char*) pMemoryArena + 8);
	cbAligned = (cbBlock + 3) & 0xfffffffc;
	*ppvBlock = 0;
	pfnGetBlockHeaderSize = (VSMEM_SizeProc) pMemoryArena->m_pVtable[7];
	cbBlockHeader = pfnGetBlockHeaderSize();
	cbFree = pMemoryArena->m_cbFree;
	if (cbFree < cbAligned) {
		g_pErrorOutputStream->AppendCStringToStream(g_VSMEM_AllocateNoMemoryPrefix);
		g_pErrorOutputStream->AppendCStringToStream(pszDescription);
		g_pErrorOutputStream->AppendCStringToStream(g_VSMEM_HexPrefix);
		AppendHexUIntToStream(g_pErrorOutputStream, cbAligned);
		g_pErrorOutputStream->AppendCStringToStream(g_VSMEM_FreeSizePrefix);
		g_pErrorOutputStream->AppendUIntToStream(cbFree);
		g_pErrorOutputStream->AppendCStringToStream(g_VSMEM_LineBreak);
		((VSMEM_LockProc) pLockVtable[1])((char*) pMemoryArena + 8);
		return 0;
	}

	pMemoryBlock = (CMBlock*) pMemoryArena->FindSmallestBlock(cbAligned + cbBlockHeader);
	if (pMemoryBlock == 0) {
		g_pErrorOutputStream->AppendCStringToStream(g_VSMEM_AllocateFragmentedPrefix);
		g_pErrorOutputStream->AppendCStringToStream(pszDescription);
		g_pErrorOutputStream->AppendCStringToStream(g_VSMEM_HexPrefix);
		AppendHexUIntToStream(g_pErrorOutputStream, cbAligned);
		g_pErrorOutputStream->AppendCStringToStream(g_VSMEM_FreeSizePrefix);
		g_pErrorOutputStream->AppendUIntToStream(pMemoryArena->m_cbFree);
		g_pErrorOutputStream->AppendCStringToStream(g_VSMEM_LineBreak);
		((CArena*) g_pMainMemoryArena)->StreamOut(g_pErrorOutputStream);
		((VSMEM_LockProc) pLockVtable[1])((char*) pMemoryArena + 8);
		return 0;
	}

	if (pMemoryBlock->m_cbPayload < pfnGetBlockHeaderSize() + cbAligned + cbBlockHeader + 4) {
		pMemoryBlock->m_dwFlags &= 0xfffffffe;
		pMemoryArena->RemoveFromFreeList(pMemoryBlock);
		pMemoryBlock->SetMemoryBlockDescription(pszDescription);
		pMemoryArena->m_cbFree -= pMemoryBlock->m_cbPayload;
	}
	else {
		cbOldBlockPayload = pMemoryBlock->m_cbPayload;
		pMemoryBlock->m_cbPayload = cbAligned;
		pMemoryBlock->SetMemoryBlockDescription(pszDescription);
		pMemoryBlock->m_dwFlags &= 0xfffffffe;
		cbRemainingBlock = cbOldBlockPayload - cbAligned;
		pMemoryArena->RemoveFromFreeList(pMemoryBlock);
		pSplitMemoryBlock = (CMBlock*) PlacementConstructMemoryArenaBlock((char*) pMemoryBlock->m_pPayload + cbAligned,
																		  pMemoryBlock->m_pArena,
																		  0,
																		  g_VSMEM_FreeBlockName,
																		  cbRemainingBlock);
		pSplitMemoryBlock->m_dwFlags |= 1;
		pMemoryArena->AddToFreeList(pSplitMemoryBlock);
		pMemoryArena->AddToBlockList(pSplitMemoryBlock, pMemoryBlock);
		pMemoryArena->m_cbFree -= pMemoryBlock->m_cbPayload + pfnGetBlockHeaderSize();
	}

	*ppvBlock = pMemoryBlock->m_pPayload;
	pStatusEntry = pMemoryArena->m_pStatusEntry;
	if (pStatusEntry != 0) {
		cbFree = (unsigned int) ((char*) *ppvBlock + cbAligned - (char*) pMemoryArena->m_pStorage);
		pfnUpdateStatusEntry = (VSMEM_StatusUpdateProc) (*(void***) pStatusEntry)[1];
		pfnUpdateStatusEntry(pStatusEntry, cbFree);
	}
	((VSMEM_LockProc) pLockVtable[1])((char*) pMemoryArena + 8);
	return 1;
}

// FUNCTION: LEMBALL 0x00459f70
int CArena::Free(void* pvBlock)
{
	CArena* pMemoryArena;
	CMBlock* pMemoryBlock;
	CMBlock* pPreviousBlock;
	CMBlock* pNextBlock;
	VSMEM_SizeProc pfnGetBlockHeaderSize;
	void** pLockVtable;

	pMemoryArena = this;
	if (pMemoryArena->CheckValidPointer(pvBlock) == 0) {
		return 0;
	}
	pLockVtable = pMemoryArena->m_pLockVtable;
	((VSMEM_LockProc) pLockVtable[0])((char*) pMemoryArena + 8);
	pfnGetBlockHeaderSize = (VSMEM_SizeProc) pMemoryArena->m_pVtable[7];
	pMemoryBlock = (CMBlock*) ((char*) pvBlock - pfnGetBlockHeaderSize());
	pMemoryBlock->m_dwFlags |= 1;
	pMemoryArena->AddToFreeList(pMemoryBlock);
	pMemoryArena->m_cbFree += pMemoryBlock->m_cbPayload;
	pPreviousBlock = pMemoryBlock->m_pPreviousFree;
	pNextBlock = pMemoryBlock->m_pNextFree;
	if (pPreviousBlock != 0) {
		if (IsFreeMemoryBlock(pPreviousBlock) != 0) {
			if (pMemoryArena->CheckAndAmalgamate(pPreviousBlock, pMemoryBlock) != 0) {
				pMemoryBlock = pPreviousBlock;
			}
		}
	}
	if (pNextBlock != 0) {
		if (IsFreeMemoryBlock(pNextBlock) != 0) {
			pMemoryArena->CheckAndAmalgamate(pMemoryBlock, pNextBlock);
		}
	}
	((VSMEM_LockProc) pLockVtable[1])((char*) pMemoryArena + 8);
	return 1;
}

// FUNCTION: LEMBALL 0x0045a010
int CArena::AllocateArena(void** ppChildArena, unsigned int cbChildArena, const char* pszName)
{
	void** pLockVtable;
	void* pvStorage;
	CMBlock* pMemoryBlock;
	void* pChildArena;

	*ppChildArena = 0;
	if (Allocate(&pvStorage, cbChildArena, g_VSMEM_ArenaContainerName) == 0) {
		return 0;
	}
	pLockVtable = m_pLockVtable;
	((VSMEM_LockProc) pLockVtable[0])((char*) this + 8);
	/*
	 * Allocate returns the payload address.  The original
	 * code walks back to the CMBlock header before constructing the
	 * child arena, so that the constructor receives the block's recorded
	 * payload and size rather than the request values.
	 */
	pMemoryBlock = (CMBlock*) ((char*) pvStorage - GetMemoryBlockHeaderSize());
	pChildArena =
		PlacementConstructMemoryArenaWithStorage(pMemoryBlock->m_pPayload, pMemoryBlock->m_cbPayload, pszName, this, 0);
	AddToArenaList(pChildArena);
	*ppChildArena = pChildArena;
	((VSMEM_LockProc) pLockVtable[1])((char*) this + 8);
	return 1;
}

// FUNCTION: LEMBALL 0x0045a0a0
int CArena::FreeArena(void* pChildArena)
{
	void** pLockVtable;

	pLockVtable = m_pLockVtable;
	((VSMEM_LockProc) pLockVtable[0])((char*) this + 8);
	if (RemoveFromArenaList(pChildArena) == 0) {
		return 0;
	}
	((VSMEM_LockProc) pLockVtable[1])((char*) this + 8);
	return Free(pChildArena);
}

// FUNCTION: LEMBALL 0x0045a0e0
int CArena::CheckValidPointer(void* pvPointer)
{
	VSMEM_SizeProc pfnGetArenaHeaderSize;
	unsigned int cbArenaHeader;
	unsigned int cbArenaHeaderSecondRead;

	if (pvPointer == 0) {
		return 0;
	}
	pfnGetArenaHeaderSize = (VSMEM_SizeProc) m_pVtable[6];
	cbArenaHeader = pfnGetArenaHeaderSize();
	cbArenaHeaderSecondRead = pfnGetArenaHeaderSize();
	if ((char*) this + cbArenaHeader <= pvPointer && pvPointer < (char*) this + cbArenaHeaderSecondRead + m_cbStorage) {
		return 1;
	}
	return 0;
}
// FUNCTION: LEMBALL 0x0045a1e0
int PASCAL HasMemoryBlockMagic(void* pBlock)
{
	return *(int*) ((char*) pBlock + 4) == 0x524d424c;
}

// FUNCTION: LEMBALL 0x0045a200
int PASCAL IsUsedMemoryBlock(void* pBlock)
{
	if (HasMemoryBlockMagic(pBlock) != 0 && (*(unsigned char*) ((char*) pBlock + 0x24) & 1) == 0) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045a230
int PASCAL IsFreeMemoryBlock(void* pBlock)
{
	if (HasMemoryBlockMagic(pBlock) != 0 && (*(unsigned char*) ((char*) pBlock + 0x24) & 1) != 0) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045a260
void* CArena::StreamOut(VsInitFormattedOutputStream* pOutputStream)
{
	CArena* pMemoryArena;
	CMBlock* pMemoryBlock;
	CArena* pChildArena;
	void** pLockVtable;

	pMemoryArena = this;
	pLockVtable = pMemoryArena->m_pLockVtable;
	((VSMEM_LockProc) pLockVtable[0])((char*) pMemoryArena + 8);
	pOutputStream->AppendCStringToStream(g_VSMEM_ReportSeparator);
	pOutputStream->AppendCStringToStream(g_VSMEM_ReportFreeSizePrefix);
	pOutputStream->AppendUIntToStream(this->GetAllocSize());
	pOutputStream->AppendCStringToStream(g_VSMEM_ReportLineBreak);
	pOutputStream->AppendCStringToStream(g_VSMEM_ReportHeaderRule);
	pOutputStream->AppendCStringToStream(g_VSMEM_ReportColumns);
	pOutputStream->AppendCStringToStream(g_VSMEM_ReportEntryRule);
	for (pMemoryBlock = pMemoryArena->m_pFirstBlock; pMemoryBlock != 0; pMemoryBlock = pMemoryBlock->m_pNextFree) {
		((VSMEM_ReportProc) pMemoryBlock->m_pVtable[0])(pMemoryBlock, 0, pOutputStream);
	}
	if (pMemoryArena->m_pFirstChildArena != 0) {
		pOutputStream->AppendCStringToStream(g_VSMEM_ReportChildArenaList);
		pOutputStream->AppendCStringToStream(g_VSMEM_ReportChildArenaRule);
		for (pChildArena = pMemoryArena->m_pFirstChildArena; pChildArena != 0;
			 pChildArena = pChildArena->m_pFirstChildArena) {
			((VSMEM_ReportProc) pChildArena->m_pVtable[0])(pChildArena, 0, pOutputStream);
		}
	}
	pOutputStream->AppendCStringToStream(g_VSMEM_ReportSeparator);
	((VSMEM_LockProc) pLockVtable[1])((char*) pMemoryArena + 8);
	return pOutputStream;
}

// FUNCTION: LEMBALL 0x0045a340
unsigned int CArena::GetAllocSize(void)
{
	return m_cbFree;
}

// FUNCTION: LEMBALL 0x0045a350
long CArena::GetFreeSize(void)
{
	CMBlock* pBlock;
	unsigned int cFreeBlocks;

	cFreeBlocks = 0;
	pBlock = m_pFirstBlock;
	while (pBlock != 0) {
		++cFreeBlocks;
		pBlock = pBlock->m_pNextFree;
	}
	return m_cbStorage - (int) (((VSMEM_SizeProc) m_pVtable[7])() * cFreeBlocks);
}

// FUNCTION: LEMBALL 0x0045a390
void* NewCArenaPlacement(unsigned int cbStorage, void* pvStorage)
{
	(void) cbStorage;
	return pvStorage;
}

// FUNCTION: LEMBALL 0x0045a3a0
void __stdcall MemSetCArena(void* pvTarget, unsigned char chValue, unsigned int cbTarget)
{
	memset(pvTarget, chValue, cbTarget);
}

// FUNCTION: LEMBALL 0x0045a3d0
void __stdcall MemCopyCArena(void* pvTarget, const void* pvSource, unsigned int cbCopy)
{
	memcpy(pvTarget, pvSource, cbCopy);
}

// FUNCTION: LEMBALL 0x0045a3f0
CArena* CArena::ConstructMemoryArena(unsigned int cbStorage, const char* pszName, void* pParentArena, void* pReserved)

{
	CArena* pMemoryArena;
	CMBlock* pMemoryBlock;
	unsigned int cbArenaHeader;
	unsigned int cbFreeBlock;

	pMemoryArena = this;
	pMemoryArena->Construct(cbStorage, pszName, pParentArena, pReserved);
	pMemoryArena->m_pVtable = (void**) &g_MainMemoryArenaVtable;
	pMemoryArena->m_pLockVtable = g_aMainMemoryArenaLockVtable;
	pMemoryArena->m_nMagic = 0x5241524e;
	cbArenaHeader = GetMemoryArenaHeaderSize();
	cbFreeBlock = cbStorage - cbArenaHeader;
	pMemoryArena->m_cbStorage = cbFreeBlock;
	cbFreeBlock -= GetMemoryBlockHeaderSize();
	pMemoryArena->m_cbFree = cbFreeBlock;
	cbArenaHeader = GetMemoryArenaHeaderSize();
	pMemoryArena->m_pStorage = (char*) pMemoryArena + cbArenaHeader;
	pMemoryBlock = (CMBlock*) PlacementConstructMemoryArenaBlock((char*) pMemoryArena + cbArenaHeader,
																 pMemoryArena,
																 0,
																 g_VSMEM_FreeBlockName,
																 pMemoryArena->m_cbStorage);
	pMemoryBlock->m_dwFlags |= 1;
	pMemoryArena->AddToBlockList(pMemoryBlock, 0);
	pMemoryArena->AddToFreeList(pMemoryBlock);
	return pMemoryArena;
}

// FUNCTION: LEMBALL 0x0045a480
void LEMBALL_FASTCALL DestroyMemoryArena(void* pArena)
{
	((CArena*) pArena)->m_pVtable = (void**) &g_MainMemoryArenaVtable;
	((CArena*) pArena)->m_pLockVtable = g_aMainMemoryArenaLockVtable;
	((CArena*) pArena)->DeleteLists();
	DestroyCArena(pArena);
}

// FUNCTION: LEMBALL 0x0045a4a0
unsigned int GetMemoryArenaHeaderSize(void)
{
	return 0x50;
}

// FUNCTION: LEMBALL 0x0045a4b0
unsigned int GetMemoryBlockHeaderSize(void)
{
	return 0x28;
}

// FUNCTION: LEMBALL 0x0045a4c0
void* LEMBALL_STDCALL PlacementConstructMemoryArenaWithStorage(void* pvStorage,
															   unsigned int cbStorage,
															   const char* pszName,
															   void* pParentArena,
															   void* pReserved)

{
	void* pArena;

	pArena = NewCArenaPlacement(0x50, pvStorage);
	if (pArena != 0) {
		return ((CArena*) pArena)->ConstructMemoryArena(cbStorage, pszName, pParentArena, pReserved);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045a500
void* LEMBALL_STDCALL PlacementConstructMemoryArenaBlock(void* pvStorage,
														 void* pArena,
														 void* pPreviousBlock,
														 const char* pszName,
														 unsigned int cbBlock)

{
	void* pBlock;

	pBlock = ReturnPlacementBlockStorage(0x28, pvStorage);
	if (pBlock != 0) {
		return ((CMBlock*) pBlock)->ConstructMemoryArenaBlock(pArena, (CMBlock*) pPreviousBlock, pszName, cbBlock);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045a540
CMBlock* CMBlock::ConstructMemoryBlockBase(void* pArena,
										   CMBlock* pPreviousBlock,
										   const char* pszName,
										   unsigned int cbBlock)
{
	(void) pszName;
	(void) cbBlock;
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
void LEMBALL_FASTCALL RestoreMemoryBlockBaseVtable(void* pBlock)
{
	*(void***) pBlock = g_aMemoryBlockVtables[0];
}

// FUNCTION: LEMBALL 0x0045a580
int CMBlock::IsPointerInside(void* pvPointer)
{
	if (m_pPayload <= pvPointer &&
		pvPointer < (char*) m_pPayload + m_cbPayload) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0045a610
void CMBlock::SetMemoryBlockDescription(const char* pszDescription)
{
	(void) pszDescription;
}

// FUNCTION: LEMBALL 0x0045a630
void* ReturnPlacementBlockStorage(unsigned int cbStorage, void* pvStorage)
{
	(void) cbStorage;
	return pvStorage;
}

// FUNCTION: LEMBALL 0x0045a640
CMBlock* CMBlock::ConstructMemoryArenaBlock(void* pArena,
											CMBlock* pPreviousBlock,
											const char* pszName,
											unsigned int cbBlock)
{
	(void) pszName;
	ConstructMemoryBlockBase(pArena, pPreviousBlock, pszName, cbBlock);
	m_pVtable = g_aMemoryBlockVtables[1];
	m_cbPayload = cbBlock - 0x28;
	m_pPayload = (char*) this + 0x28;
	m_nMagic = 0x524d424c;
	return this;
}

// FUNCTION: LEMBALL 0x0045a6b0
void* AllocateVSMemBlockImpl(unsigned int cbBlock)
{
	void* pvBlock;
	int nSmallBlock;
	int fAllocated;

	pvBlock = 0;
	if (g_fCSmallMemoryEnabled != 0 && cbBlock < g_cbCBucketUpperBound) {
		nSmallBlock = ((CSmallMemory*) g_pCSmallMemory)->AllocateFromCSmallMemory(cbBlock, g_pszCBucketAllocTag);
		pvBlock = (void*) (unsigned long) nSmallBlock;
		if (nSmallBlock != 0) {
			g_pszCBucketAllocTag = "new";
			return pvBlock;
		}
	}

	fAllocated = ((CArena*) g_pMainMemoryArena)->Allocate(&pvBlock, cbBlock, g_pszCBucketAllocTag);
	if (fAllocated == 0) {
		TriggerReleaseAssertFailure("EnoughMemory", "VSMEM.CPP", 0x68d);
	}
	return pvBlock;
}

// FUNCTION: LEMBALL 0x0045a730
void FreeVSMemBlockImpl(void* pvBlock)
{
	int fFreed;

	if (g_fCSmallMemoryEnabled != 0) {
		fFreed = ((CSmallMemory*) g_pCSmallMemory)->FreeToCSmallMemory(pvBlock);
		if (fFreed != 0) {
			return;
		}
	}

	fFreed = ((CArena*) g_pMainMemoryArena)->Free(pvBlock);
	if (fFreed == 0) {
		TriggerReleaseAssertFailure("EnoughMemory", "VSMEM.CPP", 0x6ca);
	}
}

// FUNCTION: LEMBALL 0x0045a780
void* AllocateVSMemBlock(unsigned int cbBlock)
{
	return AllocateVSMemBlockImpl(cbBlock);
}

// FUNCTION: LEMBALL 0x0045a790
void FreeVSMemBlock(void* pvBlock)
{
	FreeVSMemBlockImpl(pvBlock);
}

static void UpdateMainMemoryArenaFreeCounter(void)
{
	if (g_pMainMemoryArena == 0) {
		return;
	}

	((CArena*) g_pMainMemoryArena)->m_cbStorage = g_cbMainArenaCapacity - g_cbMainArenaInUse;
}

// FUNCTION: LEMBALL 0x0045a800
int CheckValidPointer(void* pvPointer)
{
	if (g_pMainMemoryArena != 0 && ((CArena*) g_pMainMemoryArena)->CheckValidPointer(pvPointer) != 0) {
		return 1;
	}
	return 0;
}

static void VSMEM_NoOpLock(void)
{
}

void* LEMBALL_FASTCALL VSMEM_ReportBlockStub(void* pBlock, int nUnused, VsInitFormattedOutputStream* pOutputStream)
{
	CMBlock* pMemoryBlock;
	unsigned int cbPayload;
	const char* pszState;

	VsInitFormattedOutputStream* pOut;
	(void) nUnused;
	pMemoryBlock = (CMBlock*) pBlock;
	cbPayload = pMemoryBlock->m_cbPayload;
	pOut = pOutputStream;
	pOut->AppendPointerToStreamVariant((unsigned int) pMemoryBlock->m_pPayload);
	pOut->AppendCStringToStream("	0x");
	AppendHexUIntToStream(pOutputStream, cbPayload);
	if ((pMemoryBlock->m_dwFlags & 1) != 0) {
		pszState = "	Free\n";
	} else {
		pszState = "	New\n";
	}
	pOut->AppendCStringToStream(pszState);
	return pOutputStream;
}

// FUNCTION: LEMBALL 0x0045a8d0
void* LEMBALL_FASTCALL DestroyCArenaReturnThis(void* pArena, int nUnused, int fDelete)
{
	(void) nUnused;
	(void) fDelete;
	DestroyCArena(pArena);
	return pArena;
}

// FUNCTION: LEMBALL 0x0045a8e0
void* LEMBALL_FASTCALL DestroyMemoryArenaReturnThis(void* pArena, int nUnused, int fDelete)
{
	(void) nUnused;
	(void) fDelete;
	DestroyMemoryArena(pArena);
	return pArena;
}

// FUNCTION: LEMBALL 0x0045a8f0
void* LEMBALL_FASTCALL RestoreMemoryBlockBaseVtableReturnThis(void* pBlock, int nUnused, int fDelete)
{
	(void) nUnused;
	(void) fDelete;
	RestoreMemoryBlockBaseVtable(pBlock);
	return pBlock;
}

// FUNCTION: LEMBALL 0x0045a900
void* LEMBALL_FASTCALL VSMEM_DeleteBlockStub(void* pBlock, int nUnused, int fDelete)
{
	(void) nUnused;
	(void) fDelete;
	*(void***) pBlock = g_aMemoryBlockVtables[1];
	RestoreMemoryBlockBaseVtable(pBlock);
	return pBlock;
}
// FUNCTION: LEMBALL 0x0046f060
int InitializeMasterMainRamArena(void)
{
	HGLOBAL hStorage;
	void* pStorage;
	void* pArena;
	int fSmallBuckets;
	CSmallMemory* pTable;

	hStorage = GlobalAlloc(2, (unsigned int) g_StartupGraphicsDriverConfig.m_dwStyle);
	g_hMainMemoryArenaStorage = hStorage;
	if (hStorage == 0) {
		return 0;
	}

	pStorage = GlobalLock(hStorage);
	if (pStorage == 0) {
		return 0;
	}

	pArena = NewCArenaPlacement(0x50, pStorage);
	if (pArena == 0) {
		g_pMainMemoryArena = 0;
	}
	else {
		g_pMainMemoryArena = ((CArena*) pArena)
								 ->ConstructMemoryArena((unsigned int) g_StartupGraphicsDriverConfig.m_dwStyle,
														"Master Main Ram Arena",
														0,
														0);
	}

	fSmallBuckets = g_fCSmallMemoryEnabled;
	g_pCSmallMemory = 0;
	pTable = 0;
	if (fSmallBuckets != 0) {
		g_fCSmallMemoryEnabled = 0;
		pTable = (CSmallMemory*) AllocateVSMemBlock(sizeof(CSmallMemory));
		if (pTable != 0) {
			pTable = ConstructCSmallMemory(pTable);
		}
	}
	g_pCSmallMemory = pTable;
	g_fCSmallMemoryEnabled = fSmallBuckets;
	if (g_pMainMemoryArena != 0 && (fSmallBuckets == 0 || g_pCSmallMemory != 0)) {
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0046f120
void ShutdownMasterMainRamArena(void)
{
	if (g_pCSmallMemory != 0) {
		DestroyCSmallMemory((CSmallMemory*) g_pCSmallMemory);
		FreeVSMemBlock(g_pCSmallMemory);
		g_pCSmallMemory = 0;
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
CBucket* CBucket::ConstructCBucket(unsigned int cbSlot, unsigned int cSlots, void* pStorage, void* pBitmap)

{
	unsigned int cbStorage;
	unsigned int cBitmapWords;
	int fSmallBuckets;

	m_pVtable = g_aMemoryArenaInitialLockVtable;
	InitializeCriticalSection(m_abCriticalSection);
	m_pVtable = g_aCBucketVtable;
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

	fSmallBuckets = g_fCSmallMemoryEnabled;
	if (pStorage == 0) {
		g_fCSmallMemoryEnabled = 0;
		pStorage = AllocateVSMemBlock(cbStorage);
		g_fCSmallMemoryEnabled = fSmallBuckets;
		m_wFlags |= 2;
	}
	if (pBitmap == 0) {
		g_fCSmallMemoryEnabled = 0;
		pBitmap = AllocateVSMemBlock(cBitmapWords * sizeof(unsigned int));
		g_fCSmallMemoryEnabled = fSmallBuckets;
		m_wFlags |= 1;
	}

	m_pStorage = pStorage;
	m_pBitmap = pBitmap;
	if (pBitmap != 0) {
		MemSetCArena(pBitmap, 0, cBitmapWords * sizeof(unsigned int));
	}
	m_nNextFreeWord = 0;
	m_nNextFreeBit = 0;
	return this;
}

void LEMBALL_FASTCALL ReleaseSmallMemoryChildBucket(CBucket* pBucket);
extern void* g_RenderDispatchQueueCriticalSectionHelperVtable[2];
extern void* g_VSMEM_CBucketDestructionVtable[2];

// FUNCTION: LEMBALL 0x00472dc0
void LEMBALL_FASTCALL DestroyCBucket(CBucket* pBucket)
{
	int fSmallBuckets;

	pBucket->m_pVtable = g_VSMEM_CBucketDestructionVtable;
	fSmallBuckets = g_fCSmallMemoryEnabled;
	g_fCSmallMemoryEnabled = 0;
	if (pBucket->m_pChildBucket != 0) {
		ReleaseSmallMemoryChildBucket(pBucket);
	}
	if ((pBucket->m_wFlags & 2) != 0 && pBucket->m_pStorage != 0) {
		FreeVSMemBlock(pBucket->m_pStorage);
		pBucket->m_pStorage = 0;
	}
	if ((pBucket->m_wFlags & 1) != 0 && pBucket->m_pBitmap != 0) {
		FreeVSMemBlock(pBucket->m_pBitmap);
		pBucket->m_pBitmap = 0;
	}
	g_fCSmallMemoryEnabled = fSmallBuckets;
	pBucket->m_pVtable = g_RenderDispatchQueueCriticalSectionHelperVtable;
	DeleteCriticalSection(pBucket->m_abCriticalSection);
}

// GLOBAL: LEMBALL 0x004a2a68
static const unsigned int g_adwCBucketSlotMasks[32] = {
	0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000,
};
// GLOBAL: LEMBALL 0x004a2ae8
static const char g_VSMEM_AllocatingSmallMemoryChildPrefix[] = "Allocating Small Memory Child: ";
static const char g_VSMEM_NewLine[] = "\n";

struct CBucketSlotIndex {
	unsigned short m_iWord;
	unsigned short m_iBit;
};

struct CBucketSlotFinder {
	unsigned char m_abReserved00[0x40];
	unsigned short m_wFlags;
	unsigned short m_nReserved42;
	int m_cBitmapWords;
	unsigned int* m_pBitmap;

	CBucketSlotIndex* FindNextFreeSlot(unsigned int* pNextSlot, CBucketSlotIndex CurrentSlot);
};

class CBucketLockInterface {
public:
	virtual void Enter(void) = 0;
	virtual void Leave(void) = 0;
};

// FUNCTION: LEMBALL 0x00472e40
int CBucket::AllocateSlot(void** ppvSlot)
{
	CBucket* pBucket;
	CBucketSlotIndex CurrentSlot;
	unsigned short iWord;
	unsigned short iBit;
	unsigned int nSlot;

	pBucket = this;
	for (;;) {
		((CBucketLockInterface*) pBucket)->Enter();
		if ((pBucket->m_wFlags & 4) == 0) {
			break;
		}
		if (pBucket->m_pChildBucket == 0) {
			AllocateSmallMemoryChildBucket(pBucket);
		}
		((CBucketLockInterface*) pBucket)->Leave();
		pBucket = pBucket->m_pChildBucket;
	}
	*ppvSlot = 0;
	CurrentSlot = *(CBucketSlotIndex*) &pBucket->m_nNextFreeWord;
	iWord = CurrentSlot.m_iWord;
	iBit = CurrentSlot.m_iBit;
	((unsigned int*) pBucket->m_pBitmap)[iWord] |= g_adwCBucketSlotMasks[iBit];
	pBucket->m_cbStorageAvailable -= pBucket->m_cbSlot;
	if ((int) pBucket->m_cbStorageAvailable <= 0) {
		pBucket->m_wFlags |= 4;
	}
	*(CBucketSlotIndex*) &pBucket->m_nNextFreeWord =
		*((CBucketSlotFinder*) pBucket)->FindNextFreeSlot((unsigned int*) &CurrentSlot, CurrentSlot);
	nSlot = iWord * 32 + iBit;
	*ppvSlot = (char*) pBucket->m_pStorage + nSlot * pBucket->m_cbSlot;
	++pBucket->m_cAllocated;
	++pBucket->m_cAllocations;
	if (pBucket->m_cMaximumAllocations < pBucket->m_cAllocations) {
		pBucket->m_cMaximumAllocations = pBucket->m_cAllocations;
	}
	((CBucketLockInterface*) pBucket)->Leave();
	return 1;
}

void LEMBALL_FASTCALL ReleaseParentSmallMemoryChildBucket(CBucket* pBucket);

// FUNCTION: LEMBALL 0x00472f00
int CBucket::FreeSlot(void* pvSlot)
{
	struct SlotIndex {
		unsigned short m_iWord;
		unsigned short m_iBit;
	};

	CBucket* pBucket;
	int nSlot;
	unsigned short i;
	unsigned short j;
	unsigned int dwMask;
	unsigned short wFlags;
	SlotIndex NextFreeSlot;

	pBucket = this;
	for (;;) {
		((CBucketLockInterface*) pBucket)->Enter();
		if (pvSlot < pBucket->m_pStorage || (char*) pBucket->m_pStorage + pBucket->m_cbStorage <= (char*) pvSlot) {
			((CBucketLockInterface*) pBucket)->Leave();
			pBucket = pBucket->m_pChildBucket;
			continue;
		}
		break;
	}
	nSlot = ((char*) pvSlot - (char*) pBucket->m_pStorage) / (int) pBucket->m_cbSlot;
	i = (unsigned short) (nSlot / 32);
	j = (unsigned short) (nSlot % 32);
	wFlags = pBucket->m_wFlags;
	dwMask = g_adwCBucketSlotMasks[j];
	((unsigned int*) pBucket->m_pBitmap)[i] &= ~dwMask;
	pBucket->m_cbStorageAvailable += pBucket->m_cbSlot;
	pBucket->m_wFlags &= (unsigned short) ~4;
	--pBucket->m_cAllocations;
	if ((wFlags & 4) != 0 || (pBucket->m_nNextFreeWord > i && pBucket->m_nNextFreeBit > j)) {
		NextFreeSlot.m_iWord = i;
		NextFreeSlot.m_iBit = j;
		*(unsigned int*) &pBucket->m_nNextFreeWord = *(unsigned int*) &NextFreeSlot;
	}
	if (pBucket->m_cbStorageAvailable == pBucket->m_cbStorage && pBucket->m_pParentBucket != 0) {
		ReleaseParentSmallMemoryChildBucket(pBucket);
	}
	((CBucketLockInterface*) pBucket)->Leave();
	return 1;
}

// FUNCTION: LEMBALL 0x00472fd0
CBucketSlotIndex* CBucketSlotFinder::FindNextFreeSlot(unsigned int* pNextSlot, CBucketSlotIndex CurrentSlot)
{
	const unsigned int* pMask;
	unsigned int dwBitmap;

	if ((m_wFlags & 4) != 0) {
		*(CBucketSlotIndex*) pNextSlot = CurrentSlot;
		return (CBucketSlotIndex*) pNextSlot;
	}
	if (m_pBitmap[CurrentSlot.m_iWord] == 0xffffffffU) {
		do {
			++CurrentSlot.m_iWord;
			if (CurrentSlot.m_iWord >= m_cBitmapWords) {
				CurrentSlot.m_iWord = 0;
			}
		} while (m_pBitmap[CurrentSlot.m_iWord] == 0xffffffffU);
	}
	CurrentSlot.m_iBit = 0;
	dwBitmap = m_pBitmap[CurrentSlot.m_iWord];
	pMask = g_adwCBucketSlotMasks;
	do {
		if ((*pMask & dwBitmap) == 0) {
			break;
		}
		++CurrentSlot.m_iBit;
		++pMask;
	} while (pMask < g_adwCBucketSlotMasks + 32);
	*(CBucketSlotIndex*) pNextSlot = CurrentSlot;
	return (CBucketSlotIndex*) pNextSlot;
}
// FUNCTION: LEMBALL 0x00473050
CBucket* LEMBALL_FASTCALL AllocateSmallMemoryChildBucket(CBucket* pParentBucket)

{
	int fSmallBuckets;
	CBucket* pChildBucket;
	void* pStorage;

	g_pStartupOutputStream->AppendCStringToStream(g_VSMEM_AllocatingSmallMemoryChildPrefix)
		->AppendIntToStream(pParentBucket->m_cbSlot)
		->AppendCStringToStream(g_VSMEM_NewLine);
	fSmallBuckets = g_fCSmallMemoryEnabled;
	g_fCSmallMemoryEnabled = 0;
	pChildBucket = 0;
	((CArena*) g_pMainMemoryArena)->GetFreeSize();
	pStorage = AllocateVSMemBlock(0x54);
	if (pStorage != 0) {
		pChildBucket = ((CBucket*) pStorage)->ConstructCBucket(pParentBucket->m_cbSlot, pParentBucket->m_cSlots, 0, 0);
	}
	((CArena*) g_pMainMemoryArena)->GetFreeSize();
	g_fCSmallMemoryEnabled = fSmallBuckets;
	pParentBucket->m_pChildBucket = pChildBucket;
	pChildBucket->m_pParentBucket = pParentBucket;
	return pChildBucket;
}

// FUNCTION: LEMBALL 0x004730e0
void LEMBALL_FASTCALL ReleaseSmallMemoryChildBucket(CBucket* pBucket)
{
	CBucket* pChildBucket;

	((CArena*) g_pMainMemoryArena)->GetFreeSize();
	pChildBucket = pBucket->m_pChildBucket;
	if (pChildBucket != 0) {
		DestroyCBucket(pChildBucket);
		FreeVSMemBlock(pChildBucket);
	}
	((CArena*) g_pMainMemoryArena)->GetFreeSize();
	pBucket->m_pChildBucket = 0;
}

// FUNCTION: LEMBALL 0x00473120
void LEMBALL_FASTCALL ReleaseParentSmallMemoryChildBucket(CBucket* pBucket)
{
	ReleaseSmallMemoryChildBucket(pBucket->m_pParentBucket);
}

// FUNCTION: LEMBALL 0x00473140
int CBucket::ContainsPointer(unsigned int nPointer)
{
	CBucket* pBucket;
	unsigned int nStorage;

	pBucket = this;
	for (;;) {
		nStorage = (unsigned int) pBucket->m_pStorage;
		if (nStorage <= nPointer && nStorage + pBucket->m_cbStorage > nPointer) {
			return 1;
		}
		pBucket = pBucket->m_pChildBucket;
		if (pBucket == 0) {
			return 0;
		}
	}
}

// FUNCTION: LEMBALL 0x00473180
CSmallMemory* LEMBALL_FASTCALL ConstructCSmallMemory(CSmallMemory* pTable)

{
	int i;
	int iFirstBucket;
	int nBucketOffset;
	int nSlotShift;
	unsigned int* pcbBucketSize;

	iFirstBucket = (int) g_StartupGraphicsDriverConfig.m_dwReserved0;
	nSlotShift = 1 << (iFirstBucket + 1);
	pTable->m_cBuckets = iFirstBucket + (int) g_StartupGraphicsDriverConfig.m_cItems;
	if (pTable->m_cBuckets > 7) {
		pTable->m_cBuckets = 7;
	}
	memset(pTable->m_apBuckets, 0, sizeof(pTable->m_apBuckets));

	if (iFirstBucket < pTable->m_cBuckets) {
		pcbBucketSize = &pTable->m_acbBucketSizes[iFirstBucket];
		nBucketOffset = iFirstBucket * sizeof(int);
		i = iFirstBucket;
		do {
			int fSmallBuckets;

			*pcbBucketSize = 1U << nSlotShift;
			fSmallBuckets = g_fCSmallMemoryEnabled;
			g_fCSmallMemoryEnabled = 0;
			if (*(int*) ((char*) g_StartupGraphicsDriverConfig.m_pItemDataEnd + nBucketOffset) != 0) {
				CBucket* pBucket;

				pBucket = (CBucket*) AllocateVSMemBlock(sizeof(CBucket));
				if (pBucket != 0) {
					pBucket = pBucket->ConstructCBucket(
						*pcbBucketSize,
						*(unsigned int*) ((char*) g_StartupGraphicsDriverConfig.m_pItemDataEnd + nBucketOffset),
						0,
						0);
				}
				pTable->m_apBuckets[i] = pBucket;
				if (g_cbCBucketUpperBound < *pcbBucketSize) {
					g_cbCBucketUpperBound = *pcbBucketSize;
				}
			}
			++pcbBucketSize;
			nBucketOffset += sizeof(int);
			++i;
			++nSlotShift;
			g_fCSmallMemoryEnabled = fSmallBuckets;
		} while (i < pTable->m_cBuckets);
	}
	return pTable;
}

// FUNCTION: LEMBALL 0x00473280
void LEMBALL_FASTCALL DestroyCSmallMemory(CSmallMemory* pTable)
{
	int i;
	int iFirstBucket;
	CBucket** ppBucket;

	g_fCSmallMemoryEnabled = 0;
	iFirstBucket = (int) g_StartupGraphicsDriverConfig.m_dwReserved0;
	ppBucket = &pTable->m_apBuckets[iFirstBucket];
	for (i = iFirstBucket; i < pTable->m_cBuckets; ++i, ++ppBucket) {
		CBucket* pBucket;

		pBucket = *ppBucket;
		if (pBucket != 0) {
			DestroyCBucket(pBucket);
			FreeVSMemBlock(pBucket);
			*ppBucket = 0;
		}
	}
}

// FUNCTION: LEMBALL 0x004732d0
int CSmallMemory::AllocateFromCSmallMemory(int cbBlock, const char* pszName)
{
	unsigned int* pcbBucketSize;
	int iFirstBucket;
	int i;
	int cBuckets;
	int nPreviousSize;

	iFirstBucket = (int) g_StartupGraphicsDriverConfig.m_dwReserved0;
	cBuckets = m_cBuckets;
	nPreviousSize = 0;
	if (iFirstBucket < cBuckets) {
		pcbBucketSize = &m_acbBucketSizes[iFirstBucket];
		i = iFirstBucket;
		do {
			if (nPreviousSize < cbBlock && cbBlock <= (int) *pcbBucketSize && m_apBuckets[i] != 0) {
				void* pvSlot;
				if (m_apBuckets[i]->AllocateSlot(&pvSlot) != 0) {
					return (int) (unsigned long) pvSlot;
				}
				return 0;
			}
			nPreviousSize = (int) *pcbBucketSize++;
			++i;
		} while (i < cBuckets);
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00473340
int CSmallMemory::FreeToCSmallMemory(void* pBlock)
{
	int iFirstBucket;
	int i;

	iFirstBucket = (int) g_StartupGraphicsDriverConfig.m_dwReserved0;
	for (i = iFirstBucket; i < m_cBuckets; ++i) {
		if (m_apBuckets[i] != 0 && m_apBuckets[i]->ContainsPointer((unsigned int) pBlock) != 0) {
			m_apBuckets[i]->FreeSlot(pBlock);
			return 1;
		}
	}
	return 0;
}
