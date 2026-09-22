#ifndef LEMBALL_VISOS_FOUNDATION_CARENA_H
#define LEMBALL_VISOS_FOUNDATION_CARENA_H

#include "ArenaBase.h"
#include "CCritical.h"

#include <stddef.h>
class CMBlock;

void InitSubSystems();

// SIZE 0x50
// VTABLE: LEMBALL 0x004988e0 ArenaBase
// VTABLE: LEMBALL 0x004988d8 CCritical
class CArena : public ArenaBase, public CCritical {
public:
	CArena(unsigned long p_arg0, char* p_arg1, CArena* p_arg2, CArena* p_arg3);
	CMBlock* FindSmallestBlock(unsigned long p_arg0, char* p_arg1);
	bool AddToArenaList(CArena* p_arg0);
	bool AddToBlockList(CMBlock* p_arg0, CMBlock* p_arg1);
	bool AddToFreeList(CMBlock* p_arg0);
	bool AllocateArena(CArena** p_arena, unsigned long p_size, char* p_description);
	bool CheckAndAmalgamate(CMBlock* p_block1, CMBlock* p_block2);
	bool CheckFreeMemoryBlock(CMBlock* p_arg0);
	bool CheckIntegrity();
	bool CheckMemoryBlock(CMBlock* p_arg0);
	bool CheckValidPointer(void* p_arg0);
	bool FreeArena(CArena* p_arg0);
	bool RemoveFromArenaList(CArena* p_arg0);
	bool RemoveFromBlockList(CMBlock* p_arg0);
	bool RemoveFromFreeList(CMBlock* p_arg0);
	unsigned long GetAllocSize();
	unsigned long GetFreeSize();
	virtual CVsOStream& StreamOut(CVsOStream& p_stream);                                               // vtable+0x00
	virtual ~CArena();                                                                                 // vtable+0x04
	virtual bool Allocate(unsigned char** p_data, unsigned long p_size, char* p_description);          // vtable+0x08
	virtual bool Free(unsigned char* p_memory);                                                        // vtable+0x0c
	virtual void MemSet(unsigned char* p_destination, unsigned char p_value, unsigned long p_size);    // vtable+0x10
	virtual void MemCopy(unsigned char* p_destination, unsigned char* p_source, unsigned long p_size); // vtable+0x14
	virtual int GetSizeOf() = 0;                                                                       // vtable+0x18
	virtual int GetSizeOfBlock() = 0;                                                                  // vtable+0x1c
	virtual CArena* CreateNew(unsigned char* p_arg0,
							  unsigned long p_arg1,
							  char* p_arg2,
							  CArena* p_arg3,
							  CArena* p_arg4) = 0; // vtable+0x20
	virtual CMBlock* CreateNewBlock(unsigned char* p_arg0,
									CArena* p_arg1,
									CMBlock* p_arg2,
									char* p_arg3,
									unsigned long p_arg4) = 0; // vtable+0x24
	void DeleteLists();
	void* operator new(size_t p_arg0, void* p_arg1);
	void operator delete(void*) {}
	CArena();

	friend class CMogRes;
	friend void InitSubSystems();

protected:
	unsigned char* m_arenaBase; // 0x24
	unsigned int m_arenaSize;   // 0x28
	unsigned int m_freeSize;    // 0x2c
	CArena* m_parentArena;      // 0x30
	CMBlock* m_lastBlock;       // 0x34
	CMBlock* m_firstBlock;      // 0x38
	CMBlock* m_lastFreeBlock;   // 0x3c
	CMBlock* m_firstFreeBlock;  // 0x40
	CArena* m_arenaLinkB;       // 0x44
	CArena* m_arenaLinkA;       // 0x48
	CArena* m_nextArena;        // 0x4c
};

extern unsigned char* g_pArenaHighWater;
extern CArena* g_pMasterArena;
// SYNTHETIC: LEMBALL 0x0045a8d0
// CArena::`scalar deleting destructor'

#endif
