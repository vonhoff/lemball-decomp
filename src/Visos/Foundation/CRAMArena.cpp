#include "CRAMArena.h"

#include "CMRAMBlock.h"
#include "Visos/Foundation/CArena.h"
#include "Visos/Foundation/CMBlock.h"

// FUNCTION: LEMBALL 0x0045a3f0
CRAMArena::CRAMArena(unsigned long p_size, char* p_description, CArena* p_parent, CArena* p_owner)
	: CArena(p_size, p_description, p_parent, p_owner)
{
	m_signature = 0x5241524e;
	m_arenaSize = p_size - GetSizeOf();
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
CArena* CRAMArena::CreateNew(unsigned char* p_arg0, unsigned long p_arg1, char* p_arg2, CArena* p_arg3, CArena* p_arg4)
{
	return new (p_arg0) CRAMArena(p_arg1, p_arg2, p_arg3, p_arg4);
}

// FUNCTION: LEMBALL 0x0045a500
CMBlock* CRAMArena::CreateNewBlock(unsigned char* p_arg0,
								   CArena* p_arg1,
								   CMBlock* p_arg2,
								   char* p_arg3,
								   unsigned long p_arg4)
{
	return new (p_arg0) CMRAMBlock(p_arg1, p_arg2, p_arg3, p_arg4);
}
