#ifndef LEMBALL_VISOS_FOUNDATION_ARENA_H
#define LEMBALL_VISOS_FOUNDATION_ARENA_H

#include "../../Common.h"

// SIZE 0x50
// VTABLE: LEMBALL 0x004988e0
class Arena {
public:
	Arena(unsigned long p_arg0, char* p_arg1, struct Arena* p_arg2, struct Arena* p_arg3);
	MBlock* FindSmallestBlock(unsigned long p_arg0, char* p_arg1);
	bool AddToArenaList(struct Arena* p_arg0);
	bool AddToBlockList(MBlock* p_arg0, MBlock* p_arg1);
	bool AddToFreeList(MBlock* p_arg0);
	bool AllocateArena(struct Arena** p_arg0, unsigned long p_arg1, char* p_arg2);
	bool CheckAndAmalgamate(MBlock* p_arg0, MBlock* p_arg1);
	bool CheckFreeMemoryBlock(MBlock* p_arg0);
	bool CheckIntegrity();
	bool CheckMemoryBlock(MBlock* p_arg0);
	bool CheckValidPointer(void* p_arg0);
	bool FreeArena(struct Arena* p_arg0);
	bool RemoveFromArenaList(struct Arena* p_arg0);
	bool RemoveFromBlockList(MBlock* p_arg0);
	bool RemoveFromFreeList(MBlock* p_arg0);
	unsigned long GetAllocSize();
	unsigned long GetFreeSize();
	virtual bool Allocate(unsigned char** p_arg0, unsigned long p_arg1, char* p_arg2);                 // vtable+0x08
	virtual bool Free(unsigned char* p_memory);                                                        // vtable+0x0c
	virtual void MemCopy(unsigned char* p_destination, unsigned char* p_source, unsigned long p_size); // vtable+0x14
	virtual void MemSet(unsigned char* p_destination, unsigned char p_value, unsigned long p_size);    // vtable+0x10
	virtual void StreamOut(VsOStream& p_stream);                                                       // vtable+0x00
	virtual ~Arena();                                                                                  // vtable+0x04
	void DeleteLists();
	void* operator new(size_t p_arg0, void* p_arg1);
	Arena();

private:
	unsigned int m_signature;              // 0x04
	void* m_synchronizationVtable;         // 0x08
	undefined m_synchronizationData[0x18]; // 0x0c
	unsigned char* m_arenaBase;            // 0x24
	unsigned int m_arenaSize;              // 0x28
	unsigned int m_freeSize;               // 0x2c
	class Arena* m_parentArena;            // 0x30
	MBlock* m_lastBlock;                   // 0x34
	MBlock* m_firstBlock;                  // 0x38
	MBlock* m_lastFreeBlock;               // 0x3c
	MBlock* m_firstFreeBlock;              // 0x40
	class Arena* m_arenaLinkB;             // 0x44
	class Arena* m_arenaLinkA;             // 0x48
	class Arena* m_nextArena;              // 0x4c
};

// Confirmed class-scoped globals.
extern Arena* g_pMasterArena;
#endif
