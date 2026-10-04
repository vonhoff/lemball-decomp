#include "CArena.h"

#include "CMBlock.h"
#include "Visos/Streams/CVSOStream.h"
#include "Platform/Windows/CCritical.h"

#include <string.h>

// FUNCTION: LEMBALL 0x004599f0
CArena::CArena(unsigned long p_arenaSize, char* p_description, class CArena* p_parentArena, class CArena* p_arenaLink)
	: CCritical()
{
	m_parentArena = p_parentArena;
	m_arenaLinkB = p_arenaLink;
	m_firstBlock = NULL;
	m_lastBlock = NULL;
	m_lastFreeBlock = NULL;
	m_firstFreeBlock = NULL;
	m_nextArena = NULL;
	m_usageStat = NULL;
}

// FUNCTION: LEMBALL 0x00459a40
void CArena::DeleteLists()
{
	EnterCritical();
	CMBlock* block = m_firstBlock;
	m_firstBlock = NULL;
	for (;;) {
		if (block == NULL) {
			break;
		}
		CMBlock* next = block->m_nextBlock;
		delete block;
		block = next;
	}
	CArena* arena = m_nextArena;
	m_nextArena = NULL;
	while (arena != NULL) {
		CArena* next = arena->m_nextArena;
		delete arena;
		arena = next;
	}
	LeaveCritical();
}

// FUNCTION: LEMBALL 0x00459aa0
CArena::~CArena()
{
}

// FUNCTION: LEMBALL 0x00459b10
bool CArena::CheckAndAmalgamate(CMBlock* p_block1, CMBlock* p_block2)
{
	if (p_block1->m_data + p_block1->m_size != (unsigned char*) p_block2) {
		return false;
	}
	unsigned long size2 = p_block2->m_size;
	int blockSize = GetSizeOfBlock();
	p_block1->m_size += size2 + blockSize;
	RemoveFromBlockList(p_block2);
	RemoveFromFreeList(p_block2);
	delete p_block2;
	m_freeSize += GetSizeOfBlock();
	return true;
}

// FUNCTION: LEMBALL 0x00459bd0
bool CArena::AddToFreeList(CMBlock* p_block)
{
	if (m_firstFreeBlock == NULL) {
		m_lastFreeBlock = p_block;
	}
	else {
		m_firstFreeBlock->m_previousFree = p_block;
	}
	p_block->m_previousFree = NULL;
	p_block->m_nextFree = m_firstFreeBlock;
	m_firstFreeBlock = p_block;
	return true;
}

// FUNCTION: LEMBALL 0x00459c00
bool CArena::AddToBlockList(CMBlock* p_block, CMBlock* p_previous)
{
	CMBlock* next;
	if (p_previous == NULL) {
		m_firstBlock = p_block;
		next = NULL;
	}
	else {
		next = p_previous->m_nextBlock;
		p_previous->m_nextBlock = p_block;
	}
	p_block->m_previousBlock = p_previous;
	p_block->m_nextBlock = next;
	if (next == NULL) {
		m_lastBlock = p_block;
	}
	else {
		next->m_previousBlock = p_block;
	}
	return true;
}

// FUNCTION: LEMBALL 0x00459c40
bool CArena::AddToArenaList(class CArena* p_arena)
{
	CArena* current = m_nextArena;
	CArena* previous = NULL;
	while (current != NULL && current <= p_arena) {
		previous = current;
		current = current->m_nextArena;
	}
	if (previous == NULL) {
		m_nextArena = p_arena;
	}
	else {
		previous->m_nextArena = p_arena;
	}
	p_arena->m_nextArena = current;
	return true;
}

// FUNCTION: LEMBALL 0x00459c80
bool CArena::RemoveFromFreeList(CMBlock* p_block)
{
	CMBlock* next = p_block->m_nextFree;
	CMBlock* prev = p_block->m_previousFree;
	if (prev != NULL) {
		prev->m_nextFree = next;
	}
	else {
		m_firstFreeBlock = next;
	}
	if (next != NULL) {
		next->m_previousFree = prev;
	}
	else {
		m_lastFreeBlock = prev;
	}
	return true;
}

// FUNCTION: LEMBALL 0x00459cb0
bool CArena::RemoveFromBlockList(CMBlock* p_block)
{
	CMBlock* prev = p_block->m_previousBlock;
	CMBlock* next = p_block->m_nextBlock;
	if (next != NULL) {
		next->m_previousBlock = prev;
	}
	else {
		m_lastBlock = prev;
	}
	if (prev != NULL) {
		prev->m_nextBlock = next;
	}
	else {
		m_firstBlock = next;
	}
	return true;
}

// FUNCTION: LEMBALL 0x00459ce0
bool CArena::RemoveFromArenaList(class CArena* p_arena)
{
	CArena* current = m_nextArena;
	CArena* previous = NULL;
	while (current != NULL) {
		if (current == p_arena) {
			if (previous != NULL) {
				previous->m_nextArena = current->m_nextArena;
			}
			else {
				m_nextArena = current->m_nextArena;
			}
			return true;
		}
		previous = current;
		current = current->m_nextArena;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00459d20
CMBlock* CArena::FindSmallestBlock(unsigned long p_size, char* p_description)
{
	CMBlock* current = m_firstFreeBlock;
	CMBlock* best = NULL;
	for (; current != NULL; current = current->m_nextFree) {
		if (CheckFreeMemoryBlock(current) && p_size <= current->m_size) {
			if (best == NULL || best->m_size > current->m_size) {
				best = current;
			}
		}
	}
	return best;
}

// FUNCTION: LEMBALL 0x00459f70
bool CArena::Free(unsigned char* p_memory)
{
	if (!CheckValidPointer(p_memory)) {
		return false;
	}
	EnterCritical();
	CMBlock* block = (CMBlock*) (p_memory - GetSizeOfBlock());
	block->m_flags |= MEMORY_BLOCK_FLAG_FREE;
	AddToFreeList(block);
	m_freeSize += block->m_size;
	CMBlock* prev = block->m_previousBlock;
	CMBlock* next = block->m_nextBlock;
	if (prev != NULL && CheckFreeMemoryBlock(prev)) {
		if (CheckAndAmalgamate(prev, block)) {
			block = prev;
		}
	}
	if (next != NULL && CheckFreeMemoryBlock(next)) {
		CheckAndAmalgamate(block, next);
	}
	LeaveCritical();
	return true;
}

// FUNCTION: LEMBALL 0x0045a010
bool CArena::AllocateArena(CArena** p_arena, unsigned long p_size, char* p_description)
{
	unsigned char* memory;
	CMBlock* block;
	CArena* arena;

	if (!Allocate(&memory, (*p_arena = NULL, p_size), "Arena container")) {
		return false;
	}
	EnterCritical();
	block = (CMBlock*) (memory - GetSizeOfBlock());
	arena = CreateNew(block->m_data, block->m_size, p_description, this, NULL);
	AddToArenaList(arena);
	*p_arena = arena;
	LeaveCritical();
	return true;
}

// FUNCTION: LEMBALL 0x0045a0a0
bool CArena::FreeArena(class CArena* p_arena)
{
	EnterCritical();
	if (!RemoveFromArenaList(p_arena)) {
		return false;
	}
	LeaveCritical();
	return Free((unsigned char*) p_arena);
}

// FUNCTION: LEMBALL 0x0045a0e0
bool CArena::CheckValidPointer(void* p_pointer)
{
	if (p_pointer == NULL) {
		return false;
	}
	unsigned char* start = (unsigned char*) this + GetSizeOf();
	unsigned char* end = (unsigned char*) this + GetSizeOf() + m_arenaSize;
	if (start <= p_pointer && p_pointer < end) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0045a1e0
bool CArena::CheckMemoryBlock(CMBlock* p_block)
{
	return p_block->m_signature == MEMORY_BLOCK_SIGNATURE;
}

// FUNCTION: LEMBALL 0x0045a230
bool CArena::CheckFreeMemoryBlock(CMBlock* p_block)
{
	if (CheckMemoryBlock(p_block) && (p_block->m_flags & MEMORY_BLOCK_FLAG_FREE) != 0) {
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0045a260
CVSOStream& CArena::StreamOut(CVSOStream& p_stream)
{
	EnterCritical();
	p_stream << "************************************************\n";
	p_stream << "Memory Block list free size = " << GetFreeSize() << "\n";
	p_stream << "--------------------\n";
	p_stream << "Addr\t\tSize\t\tDesc\n";
	p_stream << "------------------------------------------------\n";
	for (CMBlock* block = m_firstBlock; block != NULL; block = block->m_nextBlock) {
		block->StreamOut(p_stream);
	}
	if (m_nextArena != NULL) {
		p_stream << "Child Arena list\n";
		p_stream << "--------------------\n";
		for (CArena* arena = m_nextArena; arena != NULL; arena = arena->m_nextArena) {
			arena->StreamOut(p_stream);
		}
	}
	p_stream << "************************************************\n";
	LeaveCritical();
	return p_stream;
}

// FUNCTION: LEMBALL 0x0045a330
bool CArena::CheckIntegrity()
{
	return true;
}

// FUNCTION: LEMBALL 0x0045a340
unsigned long CArena::GetFreeSize()
{
	return m_freeSize;
}

// FUNCTION: LEMBALL 0x0045a350
unsigned long CArena::GetAllocSize()
{
	int blockCount = 0;
	CMBlock* current = m_firstBlock;
	if (current != NULL) {
		do {
			blockCount++;
			current = current->m_nextBlock;
		} while (current != NULL);
	}
	unsigned long arenaSize = m_arenaSize;
	return arenaSize - GetSizeOfBlock() * blockCount;
}

// FUNCTION: LEMBALL 0x0045a390
void* CArena::operator new(size_t p_size, void* p_memory)
{
	return p_memory;
}

// FUNCTION: LEMBALL 0x0045a3a0
void CArena::MemSet(unsigned char* p_destination, unsigned char p_value, unsigned long p_size)
{
	memset(p_destination, p_value, p_size);
}

// FUNCTION: LEMBALL 0x0045a3d0
void CArena::MemCopy(unsigned char* p_destination, unsigned char* p_source, unsigned long p_size)
{
	memmove(p_destination, p_source, p_size);
}

// GLOBAL: LEMBALL 0x004a117c
unsigned char* g_pArenaHighWater = NULL;

// GLOBAL: LEMBALL 0x004aa100
CArena* g_pMasterArena;
