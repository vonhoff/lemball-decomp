#include "MramBlock.h"

// 68K 0x10215ab2 __ct__10CMRAMBlockFP6CArenaP7CMBlockPcUl
// FUNCTION: LEMBALL 0x0045a640
MramBlock::MramBlock(Arena* p_arena, MBlock* p_previous, char* p_description, unsigned long p_totalSize)
	: MBlock(p_arena, p_previous, p_description, p_totalSize)
{
	m_size = p_totalSize - sizeof(MramBlock);
	m_data = (unsigned char*) (this + 1);
	m_signature = 0x524d424c;
}

// 68K 0x10100de0 __dt__10CMRAMBlockFv
// FUNCTION: LEMBALL 0x0045a900
MramBlock::~MramBlock()
{
}
