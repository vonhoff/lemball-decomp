#include "RamArena.h"

#include "MramBlock.h"

// 68K 0x102156bc __ct__9CRAMArenaFUlPcP6CArenaP6CArena
// FUNCTION: LEMBALL 0x0045a3f0
RamArena::RamArena(unsigned long p_size, char* p_description, Arena* p_parent, Arena* p_owner)
	: Arena(p_size, p_description, p_parent, p_owner)
{
	m_signature = 0x5241524e;
	m_arenaSize = p_size - GetSizeOf();
	m_freeSize = m_arenaSize - GetSizeOfBlock();
	m_arenaBase = (unsigned char*) this + GetSizeOf();
	MBlock* block = CreateNewBlock(m_arenaBase, this, 0, "Free", m_arenaSize);
	block->m_flags |= 1;
	AddToBlockList(block, 0);
	AddToFreeList(block);
}

// 68K 0x102157b6 __dt__9CRAMArenaFv
// FUNCTION: LEMBALL 0x0045a480
RamArena::~RamArena()
{
	DeleteLists();
}

// 68K 0x10215808 GetSizeOf__9CRAMArenaFv
// FUNCTION: LEMBALL 0x0045a4a0
int RamArena::GetSizeOf()
{
	return sizeof(RamArena);
}

// 68K 0x1021582c GetSizeOfBlock__9CRAMArenaFv
// FUNCTION: LEMBALL 0x0045a4b0
int RamArena::GetSizeOfBlock()
{
	return sizeof(MBlock);
}

// 68K 0x10215856 CreateNew__9CRAMArenaFPUcUlPcP6CArenaP6CArena
// FUNCTION: LEMBALL 0x0045a4c0
Arena* RamArena::CreateNew(unsigned char* p_arg0, unsigned long p_arg1, char* p_arg2, Arena* p_arg3, Arena* p_arg4)
{
	return new (p_arg0) RamArena(p_arg1, p_arg2, p_arg3, p_arg4);
}

// 68K 0x102158c2 CreateNewBlock__9CRAMArenaFPUcP6CArenaP7CMBlockPcUl
// FUNCTION: LEMBALL 0x0045a500
MBlock* RamArena::CreateNewBlock(unsigned char* p_arg0,
								 Arena* p_arg1,
								 MBlock* p_arg2,
								 char* p_arg3,
								 unsigned long p_arg4)
{
	return new (p_arg0) MramBlock(p_arg1, p_arg2, p_arg3, p_arg4);
}
