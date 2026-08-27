#include "MBlock.h"

#include "VsOStream.h"

// 68K 0x10215938 __ct__7CMBlockFP6CArenaP7CMBlockPcUl
// FUNCTION: LEMBALL 0x0045a540
MBlock::MBlock(Arena* p_arena, MBlock* p_previous, char* p_description, unsigned long p_size)
{
	m_description = (char*) p_arena;
	m_nextBlock = p_previous;
	m_previousBlock = 0;
	m_previousFree = 0;
	m_nextFree = 0;
	m_flags = 0;
}

// 68K 0x1021599a __dt__7CMBlockFv
// FUNCTION: LEMBALL 0x0045a570
MBlock::~MBlock()
{
}

// 68K 0x102159c8 StreamOut__7CMBlockFR10CVSOStream
// FUNCTION: LEMBALL 0x0045a5b0
VsOStream& MBlock::StreamOut(VsOStream& p_stream)
{
	p_stream << (const void*) m_data << "\t0x" << Hex8(m_size);
	if ((m_flags & 1) != 0) {
		p_stream << "\tFree\n";
		return p_stream;
	}
	p_stream << "\tNew\n";
	return p_stream;
}

// 68K 0x10215a70 SetDesc__7CMBlockFPc
// FUNCTION: LEMBALL 0x0045a610
void MBlock::SetDesc(char* p_description)
{
}

// 68K 0x10215a90 __nw__7CMBlockFUlPv
// FUNCTION: LEMBALL 0x0045a630
void* MBlock::operator new(size_t p_size, void* p_memory)
{
	return p_memory;
}

void MBlock::operator delete(void* p_memory)
{
}
