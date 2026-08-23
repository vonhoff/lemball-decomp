#ifndef LEMBALL_VISOS_FOUNDATION_ARENA_H
#define LEMBALL_VISOS_FOUNDATION_ARENA_H

#include "../../Common.h"
#include "Critical.h"
#include "MBlock.h"

// SIZE 0x08
// VTABLE: LEMBALL 0x00498908
class ArenaBase {
public:
	virtual VsOStream& StreamOut(VsOStream& p_stream) = 0; // vtable+0x00

protected:
	unsigned int m_signature; // 0x04
};

// SIZE 0x50
// VTABLE: LEMBALL 0x004988e0
class Arena : public ArenaBase, public Critical {
public:
	Arena(unsigned long p_arg0, char* p_arg1, Arena* p_arg2, Arena* p_arg3);
	MBlock* FindSmallestBlock(unsigned long p_arg0, char* p_arg1);
	bool AddToArenaList(Arena* p_arg0);
	bool AddToBlockList(MBlock* p_arg0, MBlock* p_arg1);
	bool AddToFreeList(MBlock* p_arg0);
	bool AllocateArena(Arena** p_arena, unsigned long p_size, char* p_description);
	bool CheckAndAmalgamate(MBlock* p_block1, MBlock* p_block2);
	bool CheckFreeMemoryBlock(MBlock* p_arg0);
	bool CheckIntegrity();
	bool CheckMemoryBlock(MBlock* p_arg0);
	bool CheckValidPointer(void* p_arg0);
	bool FreeArena(Arena* p_arg0);
	bool RemoveFromArenaList(Arena* p_arg0);
	bool RemoveFromBlockList(MBlock* p_arg0);
	bool RemoveFromFreeList(MBlock* p_arg0);
	unsigned long GetAllocSize();
	unsigned long GetFreeSize();
	virtual VsOStream& StreamOut(VsOStream& p_stream);                                                 // vtable+0x00
	virtual ~Arena();                                                                                  // vtable+0x04
	virtual bool Allocate(unsigned char** p_data, unsigned long p_size, char* p_description);         // vtable+0x08
	virtual bool Free(unsigned char* p_memory);                                                        // vtable+0x0c
	virtual void MemSet(unsigned char* p_destination, unsigned char p_value, unsigned long p_size);    // vtable+0x10
	virtual void MemCopy(unsigned char* p_destination, unsigned char* p_source, unsigned long p_size); // vtable+0x14
	virtual int GetSizeOf() = 0;                                                                       // vtable+0x18
	virtual int GetSizeOfBlock() = 0;                                                                  // vtable+0x1c
	virtual Arena* CreateNew(unsigned char* p_arg0, unsigned long p_arg1, char* p_arg2, Arena* p_arg3, Arena* p_arg4) = 0; // vtable+0x20
	virtual MBlock* CreateNewBlock(unsigned char* p_arg0, Arena* p_arg1, MBlock* p_arg2, char* p_arg3, unsigned long p_arg4) = 0; // vtable+0x24
	void DeleteLists();
	void* operator new(size_t p_arg0, void* p_arg1);
	Arena();

	friend class MogRes;

protected:
	unsigned char* m_arenaBase;            // 0x24
	unsigned int m_arenaSize;              // 0x28
	unsigned int m_freeSize;               // 0x2c
	Arena* m_parentArena;                  // 0x30
	MBlock* m_lastBlock;                   // 0x34
	MBlock* m_firstBlock;                  // 0x38
	MBlock* m_lastFreeBlock;               // 0x3c
	MBlock* m_firstFreeBlock;              // 0x40
	Arena* m_arenaLinkB;                   // 0x44
	Arena* m_arenaLinkA;                   // 0x48
	Arena* m_nextArena;                    // 0x4c
};

extern unsigned char* g_pArenaHighWater;
extern Arena* g_pMasterArena;
#endif
