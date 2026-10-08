#include "CRAMArena.h"

#include "CArena.h"
#include "CMBlock.h"
#include "CMRAMBlock.h"

#include <stddef.h>

namespace
{
enum {
	RAM_ARENA_SIGNATURE = 0x5241524e
};
}

// FUNCTION: LEMBALL 0x0045a3f0
CRAMArena::CRAMArena(unsigned long p_arenaSize, char* p_description, CArena* p_parentArena, CArena* p_arenaLink)
	: CArena(p_arenaSize, p_description, p_parentArena, p_arenaLink)
{
	m_signature = RAM_ARENA_SIGNATURE;
	m_arenaSize = p_arenaSize - GetSizeOf();
	m_freeSize = m_arenaSize - GetSizeOfBlock();
	m_arenaBase = (unsigned char*) this + GetSizeOf();
	CMBlock* block = CreateNewBlock(m_arenaBase, this, NULL, "Free", m_arenaSize);
	block->m_flags |= 1;
	AddToBlockList(block, NULL);
	AddToFreeList(block);
}

// FUNCTION: LEMBALL 0x0045a480
CRAMArena::~CRAMArena()
{
	DeleteLists();
}

// FUNCTION: LEMBALL 0x0045a4a0
int CRAMArena::GetSizeOf()
{
	return sizeof(CRAMArena);
}

// FUNCTION: LEMBALL 0x0045a4b0
int CRAMArena::GetSizeOfBlock()
{
	return sizeof(CMBlock);
}

// FUNCTION: LEMBALL 0x0045a4c0
CArena* CRAMArena::CreateNew(unsigned char* p_memory,
							 unsigned long p_arenaSize,
							 char* p_description,
							 CArena* p_parentArena,
							 CArena* p_arenaLink)
{
	return new (p_memory) CRAMArena(p_arenaSize, p_description, p_parentArena, p_arenaLink);
}

// FUNCTION: LEMBALL 0x0045a500
CMBlock* CRAMArena::CreateNewBlock(unsigned char* p_memory,
								   CArena* p_arena,
								   CMBlock* p_previousBlock,
								   char* p_description,
								   unsigned long p_totalSize)
{
	return new (p_memory) CMRAMBlock(p_arena, p_previousBlock, p_description, p_totalSize);
}
