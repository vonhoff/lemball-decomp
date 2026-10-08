#include "CMSizedBlock.h"

#include "Engine/Memory/CMBlock.h"

class CArena;

// FUNCTION: LEMBALL 0x0045a680
CMSizedBlock::CMSizedBlock(CArena* p_arena, CMBlock* p_previous, char* p_description, unsigned long p_size)
	: CMBlock(p_arena, p_previous, p_description, p_size)
{
	m_size = p_size;
}
