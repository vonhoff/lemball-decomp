#include "CRAMArena.h"

#include "CMRAMBlock.h"
#include "Visos/Foundation/CArena.h"
#include "Visos/Foundation/CMBlock.h"

// FUNCTION: LEMBALL 0x0045a3f0
CRAMArena::CRAMArena(unsigned long arenaSize, char* description, CArena* parentArena, CArena* arenaLink)
	: CArena(arenaSize, description, parentArena, arenaLink)
{
	m_signature = 0x5241524e;
	m_arenaSize = arenaSize - GetSizeOf();
	m_freeSize = m_arenaSize - GetSizeOfBlock();
	m_arenaBase = (unsigned char*) this + GetSizeOf();
	CMBlock* block = CreateNewBlock(m_arenaBase, this, 0, "Free", m_arenaSize);
	block->m_flags |= 1;
	AddToBlockList(block, 0);
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
CArena* CRAMArena::CreateNew(unsigned char* memory,
							 unsigned long arenaSize,
							 char* description,
							 CArena* parentArena,
							 CArena* arenaLink)
{
	return new (memory) CRAMArena(arenaSize, description, parentArena, arenaLink);
}

// FUNCTION: LEMBALL 0x0045a500
CMBlock* CRAMArena::CreateNewBlock(unsigned char* memory,
								   CArena* arena,
								   CMBlock* previousBlock,
								   char* description,
								   unsigned long totalSize)
{
	return new (memory) CMRAMBlock(arena, previousBlock, description, totalSize);
}
