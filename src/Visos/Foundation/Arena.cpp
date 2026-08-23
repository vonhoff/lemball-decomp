#include "Arena.h"
#include "MBlock.h"
#include "VsOStream.h"
#include <string.h>

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)

Arena::Arena()
{
}

// 68K 0x102147c2 __ct__6CArenaFUlPcP6CArenaP6CArena
// FUNCTION: LEMBALL 0x004599f0
Arena::Arena(unsigned long p_arg0, char* p_arg1, class Arena* p_arg2, class Arena* p_arg3)
	: Critical()
{
	m_arenaLinkA = p_arg2;
	m_arenaLinkB = p_arg3;
	m_lastBlock = 0;
	m_firstBlock = 0;
	m_lastFreeBlock = 0;
	m_firstFreeBlock = 0;
	m_nextArena = 0;
	m_parentArena = 0;
}

// 68K 0x1021484c DeleteLists__6CArenaFv
// FUNCTION: LEMBALL 0x00459a40
void Arena::DeleteLists()
{
	EnterCritical();
	MBlock* block = m_lastBlock;
	m_lastBlock = 0;
	while (block != 0) {
		MBlock* next = block->m_nextBlock;
		delete block;
		block = next;
	}
	Arena* arena = m_nextArena;
	m_nextArena = 0;
	while (arena != 0) {
		Arena* next = arena->m_nextArena;
		delete arena;
		arena = next;
	}
	LeaveCritical();
}

// 68K 0x102148f6 __dt__6CArenaFv
// FUNCTION: LEMBALL 0x00459aa0
Arena::~Arena()
{
}

// 68K 0x1021492e CheckAndAmalgamate__6CArenaFP7CMBlockP7CMBlock
// FUNCTION: LEMBALL 0x00459b10
bool Arena::CheckAndAmalgamate(MBlock* p_block1, MBlock* p_block2)
{
	if (p_block1->m_data + p_block1->m_size != (unsigned char*) p_block2) {
		return 0;
	}
	unsigned long size2 = p_block2->m_size;
	int blockSize = GetSizeOfBlock();
	p_block1->m_size += size2 + blockSize;
	RemoveFromBlockList(p_block2);
	RemoveFromFreeList(p_block2);
	delete p_block2;
	m_freeSize += GetSizeOfBlock();
	return 1;
}

// 68K 0x102149fc AddToFreeList__6CArenaFP7CMBlock
// FUNCTION: LEMBALL 0x00459bd0
bool Arena::AddToFreeList(MBlock* p_block)
{
	if (m_firstFreeBlock == 0) {
		m_lastFreeBlock = p_block;
	} else {
		m_firstFreeBlock->m_previousFree = p_block;
	}
	p_block->m_previousFree = 0;
	p_block->m_nextFree = m_firstFreeBlock;
	m_firstFreeBlock = p_block;
	return 1;
}

// 68K 0x10214a5a AddToBlockList__6CArenaFP7CMBlockP7CMBlock
// FUNCTION: LEMBALL 0x00459c00
bool Arena::AddToBlockList(MBlock* p_block, MBlock* p_previous)
{
	MBlock* next;
	if (p_previous == 0) {
		m_lastBlock = p_block;
		next = 0;
	} else {
		next = p_previous->m_nextBlock;
		p_previous->m_nextBlock = p_block;
	}
	p_block->m_previousBlock = p_previous;
	p_block->m_nextBlock = next;
	if (next == 0) {
		m_firstBlock = p_block;
	} else {
		next->m_previousBlock = p_block;
	}
	return 1;
}

// 68K 0x10214ad4 AddToArenaList__6CArenaFP6CArena
// FUNCTION: LEMBALL 0x00459c40
bool Arena::AddToArenaList(class Arena* p_arena)
{
	Arena* current = m_nextArena;
	Arena* previous = 0;
	while (current != 0 && current <= p_arena) {
		previous = current;
		current = current->m_nextArena;
	}
	if (previous == 0) {
		m_nextArena = p_arena;
	} else {
		previous->m_nextArena = p_arena;
	}
	p_arena->m_nextArena = current;
	return 1;
}

// 68K 0x10214b40 RemoveFromFreeList__6CArenaFP7CMBlock
// FUNCTION: LEMBALL 0x00459c80
bool Arena::RemoveFromFreeList(MBlock* p_block)
{
	MBlock* next = p_block->m_nextFree;
	MBlock* prev = p_block->m_previousFree;
	if (prev != 0) {
		prev->m_nextFree = next;
	} else {
		m_firstFreeBlock = next;
	}
	if (next != 0) {
		next->m_previousFree = prev;
	} else {
		m_lastFreeBlock = prev;
	}
	return 1;
}

// 68K 0x10214bac RemoveFromBlockList__6CArenaFP7CMBlock
// FUNCTION: LEMBALL 0x00459cb0
bool Arena::RemoveFromBlockList(MBlock* p_block)
{
	MBlock* prev = p_block->m_previousBlock;
	MBlock* next = p_block->m_nextBlock;
	if (next != 0) {
		next->m_previousBlock = prev;
	} else {
		m_firstBlock = prev;
	}
	if (prev != 0) {
		prev->m_nextBlock = next;
	} else {
		m_lastBlock = next;
	}
	return 1;
}

// 68K 0x10214c18 RemoveFromArenaList__6CArenaFP6CArena
// FUNCTION: LEMBALL 0x00459ce0
bool Arena::RemoveFromArenaList(class Arena* p_arena)
{
	Arena* current = m_nextArena;
	Arena* previous = 0;
	while (current != 0) {
		if (current == p_arena) {
			if (previous != 0) {
				previous->m_nextArena = current->m_nextArena;
			} else {
				m_nextArena = current->m_nextArena;
			}
			return 1;
		}
		previous = current;
		current = current->m_nextArena;
	}
	return 0;
}

// 68K 0x10214c90 FindSmallestBlock__6CArenaFUlPc
// FUNCTION: LEMBALL 0x00459d20
MBlock* Arena::FindSmallestBlock(unsigned long p_size, char* p_description)
{
	MBlock* best = 0;
	for (MBlock* current = m_firstFreeBlock; current != 0; current = current->m_nextFree) {
		if (CheckFreeMemoryBlock(current) && p_size <= current->m_size) {
			if (best == 0 || best->m_size > current->m_size) {
				best = current;
			}
		}
	}
	return best;
}

// 68K 0x10214d12 Allocate__6CArenaFPPUcUlPc
// STUB: LEMBALL 0x00459d70
bool Arena::Allocate(unsigned char** p_data, unsigned long p_size, char* p_description)
{
	return 0;
}

// 68K 0x102150e4 Free__6CArenaFPUc
// FUNCTION: LEMBALL 0x00459f70
bool Arena::Free(unsigned char* p_memory)
{
	if (!CheckValidPointer(p_memory)) {
		return 0;
	}
	EnterCritical();
	MBlock* block = (MBlock*) (p_memory - GetSizeOfBlock());
	block->m_flags |= 1;
	AddToFreeList(block);
	m_freeSize += block->m_size;
	MBlock* prev = block->m_previousBlock;
	MBlock* next = block->m_nextBlock;
	if (prev != 0 && CheckFreeMemoryBlock(prev)) {
		if (CheckAndAmalgamate(prev, block)) {
			block = prev;
		}
	}
	if (next != 0 && CheckFreeMemoryBlock(next)) {
		CheckAndAmalgamate(block, next);
	}
	LeaveCritical();
	return 1;
}

// 68K 0x102151ce AllocateArena__6CArenaFPP6CArenaUlPc
// STUB: LEMBALL 0x0045a010
bool Arena::AllocateArena(Arena** p_arena, unsigned long p_size, char* p_description)
{
	return 0;
}

// 68K 0x102152a2 FreeArena__6CArenaFP6CArena
// FUNCTION: LEMBALL 0x0045a0a0
bool Arena::FreeArena(class Arena* p_arena)
{
	EnterCritical();
	if (!RemoveFromArenaList(p_arena)) {
		return 0;
	}
	LeaveCritical();
	return Free((unsigned char*) p_arena);
}

// 68K 0x1021531e CheckValidPointer__6CArenaFPv
// FUNCTION: LEMBALL 0x0045a0e0
bool Arena::CheckValidPointer(void* p_pointer)
{
	if (p_pointer == 0) {
		return 0;
	}
	int sizeOf = GetSizeOf();
	unsigned char* start = (unsigned char*) this + sizeOf;
	unsigned char* end = (unsigned char*) this + GetSizeOf() + m_arenaSize;
	if (start <= p_pointer && p_pointer < end) {
		return 1;
	}
	return 0;
}

// 68K 0x1021539c CheckMemoryBlock__6CArenaFP7CMBlock
// FUNCTION: LEMBALL 0x0045a1e0
bool Arena::CheckMemoryBlock(MBlock* p_block)
{
	return p_block->m_signature == 0x524d424c;
}

// 68K 0x102153dc CheckFreeMemoryBlock__6CArenaFP7CMBlock
// FUNCTION: LEMBALL 0x0045a230
bool Arena::CheckFreeMemoryBlock(MBlock* p_block)
{
	if (CheckMemoryBlock(p_block) && (p_block->m_flags & 1) != 0) {
		return 1;
	}
	return 0;
}

// 68K 0x10215438 StreamOut__6CArenaFR10CVSOStream
// FUNCTION: LEMBALL 0x0045a260
VsOStream& Arena::StreamOut(VsOStream& p_stream)
{
	EnterCritical();
	p_stream << "************************************************\n";
	p_stream << "Memory Block list free size = " << GetFreeSize() << "\n";
	p_stream << "--------------------\n";
	p_stream << "Addr\t\tSize\t\tDesc\n";
	p_stream << "------------------------------------------------\n";
	for (MBlock* block = m_lastBlock; block != 0; block = block->m_nextBlock) {
		block->StreamOut(p_stream);
	}
	if (m_nextArena != 0) {
		p_stream << "Child Arena list\n";
		p_stream << "--------------------\n";
		for (Arena* arena = m_nextArena; arena != 0; arena = arena->m_nextArena) {
			arena->StreamOut(p_stream);
		}
	}
	p_stream << "************************************************\n";
	LeaveCritical();
	return p_stream;
}

// 68K 0x1021557e CheckIntegrity__6CArenaFv
// FUNCTION: LEMBALL 0x0045a330
bool Arena::CheckIntegrity()
{
	return 1;
}

// 68K 0x102155a4 GetFreeSize__6CArenaFv
// FUNCTION: LEMBALL 0x0045a340
unsigned long Arena::GetFreeSize()
{
	return m_freeSize;
}

// 68K 0x102155ce GetAllocSize__6CArenaFv
// FUNCTION: LEMBALL 0x0045a350
unsigned long Arena::GetAllocSize()
{
	int blockCount = 0;
	for (MBlock* current = m_lastBlock; current != 0; current = current->m_nextBlock) {
		blockCount++;
	}
	return m_arenaSize - (GetSizeOfBlock() * blockCount);
}

// 68K 0x1021562c __nw__6CArenaFUlPv
// FUNCTION: LEMBALL 0x0045a390
void* Arena::operator new(size_t p_size, void* p_memory)
{
	return p_memory;
}

// 68K 0x1021564e MemSet__6CArenaFPUcUcUl
// FUNCTION: LEMBALL 0x0045a3a0
void Arena::MemSet(unsigned char* p_destination, unsigned char p_value, unsigned long p_size)
{
	memset(p_destination, p_value, p_size);
}

// 68K 0x10215686 MemCopy__6CArenaFPUcPUcUl
// FUNCTION: LEMBALL 0x0045a3d0
void Arena::MemCopy(unsigned char* p_destination, unsigned char* p_source, unsigned long p_size)
{
	memcpy(p_destination, p_source, p_size);
}

// GLOBAL: LEMBALL 0x004aa100
Arena* g_pMasterArena;
