#include "CMramBlock.h"

#include "Visos/Foundation/CMBlock.h"

// FUNCTION: LEMBALL 0x0045a640
CMramBlock::CMramBlock(CArena* p_arena, CMBlock* p_previous, char* p_description, unsigned long p_totalSize)
	: CMBlock(p_arena, p_previous, p_description, p_totalSize)
{
	m_size = p_totalSize - sizeof(CMramBlock);
	m_data = (unsigned char*) (this + 1);
	m_signature = 0x524d424c;
}
