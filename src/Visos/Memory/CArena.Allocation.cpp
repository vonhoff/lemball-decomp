#include "CArena.h"

#include "Visos/Statistics/CBaseStat.h"
#include "CMBlock.h"
#include "Visos/Streams/CVSOStream.h"

enum {
	ARENA_ALLOCATION_ALIGNMENT_BYTES = 4
};

// FUNCTION: LEMBALL 0x00459d70
bool CArena::Allocate(unsigned char** p_data, unsigned long p_size, char* p_description)
{
	unsigned long aligned;
	unsigned long needed;
	int headerSize;
	CMBlock* block;
	unsigned char* data;

	EnterCritical();
	*p_data = NULL;
	aligned = (p_size + ARENA_ALLOCATION_ALIGNMENT_BYTES - 1) & ~(ARENA_ALLOCATION_ALIGNMENT_BYTES - 1);
	headerSize = GetSizeOfBlock();
	needed = aligned + headerSize;
	if (aligned > m_freeSize) {
		Hex hexSize;
		hexSize.m_value = aligned;
		*g_pErrorOutput << "CArena::Allocate : Not enough free memory : " << p_description << " : 0x" << hexSize
						<< " : Free Size is " << (unsigned long) m_freeSize << "\n";
		LeaveCritical();
		return false;
	}
	block = FindSmallestBlock(needed, p_description);
	if (block == NULL) {
		unsigned long freeSize = m_freeSize;
		Hex hexSize;
		hexSize.m_value = aligned;
		*g_pErrorOutput << "CArena::Allocate : memory blocks are too fragmented to satisfy : " << p_description
						<< " : 0x" << hexSize << " : Free Size is " << freeSize << "\n";
		{
			CArena* master = g_pMasterArena;
			master->StreamOut(*g_pErrorOutput);
		}
		LeaveCritical();
		return false;
	}
	if (GetSizeOfBlock() + needed + 4 <= block->m_size) {
		unsigned long oldSize = block->m_size;
		CMBlock* leftover;
		block->m_size = aligned;
		block->SetDesc(p_description);
		block->m_flags &= ~MEMORY_BLOCK_FLAG_FREE;
		RemoveFromFreeList(block);
		unsigned char* nextData = block->m_data + aligned;
		leftover = CreateNewBlock(nextData, block->m_ownerArena, NULL, "Free", oldSize - aligned);
		leftover->m_flags |= MEMORY_BLOCK_FLAG_FREE;
		AddToFreeList(leftover);
		AddToBlockList(leftover, block);
		m_freeSize -= block->m_size + GetSizeOfBlock();
	}
	else {
		block->m_flags &= ~MEMORY_BLOCK_FLAG_FREE;
		RemoveFromFreeList(block);
		block->SetDesc(p_description);
		m_freeSize -= block->m_size;
	}
	{
		unsigned char* blockData = block->m_data;
		*p_data = blockData;
	}
	if (m_usageStat != NULL) {
		data = *p_data + aligned;
		if (g_pArenaHighWater < data) {
			g_pArenaHighWater = data;
		}
		((CBaseStat*) m_usageStat)->Update(data - m_arenaBase);
	}
	LeaveCritical();
	return true;
}
