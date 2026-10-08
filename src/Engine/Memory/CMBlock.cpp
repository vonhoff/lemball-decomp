#include "CMBlock.h"

#include "Engine/Streams/CVSOStream.h"

#include <stddef.h>

// FUNCTION: LEMBALL 0x0045a540
CMBlock::CMBlock(CArena* p_arena, CMBlock* p_previous, char* p_description, unsigned long p_size)
{
	m_ownerArena = p_arena;
	m_nextBlock = p_previous;
	m_previousBlock = NULL;
	m_previousFree = NULL;
	m_nextFree = NULL;
	m_flags = 0;
}

// FUNCTION: LEMBALL 0x0045a570
CMBlock::~CMBlock()
{
}

// FUNCTION: LEMBALL 0x0045a580
bool CMBlock::ContainsAddress(void* p_address)
{
	return (unsigned int) p_address >= (unsigned int) m_data &&
		   (unsigned int) p_address < (unsigned int) m_data + m_size;
}

// FUNCTION: LEMBALL 0x0045a5b0
CVSOStream& CMBlock::StreamOut(CVSOStream& p_stream)
{
	p_stream << (const void*) m_data << "\t0x" << Hex8(m_size);
	if ((m_flags & MEMORY_BLOCK_FLAG_FREE) != 0) {
		p_stream << "\tFree\n";
		return p_stream;
	}
	p_stream << "\tNew\n";
	return p_stream;
}

// FUNCTION: LEMBALL 0x0045a610
void CMBlock::SetDesc(char* p_description)
{
}

// FUNCTION: LEMBALL 0x0045a630
void* CMBlock::operator new(size_t p_size, void* p_memory)
{
	return p_memory;
}
