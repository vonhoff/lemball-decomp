#include "Bucket.h"

#include "Arena.h"
#include "SmallMemory.h"
#include "VsDebug.h"
#include "VsMem.h"
#include "VsOStream.h"

#include <string.h>

#pragma intrinsic(memset)

// GLOBAL: LEMBALL 0x004a2a68
static unsigned int s_bitMasks[32] = {
	0x00000001, 0x00000002, 0x00000004, 0x00000008,
	0x00000010, 0x00000020, 0x00000040, 0x00000080,
	0x00000100, 0x00000200, 0x00000400, 0x00000800,
	0x00001000, 0x00002000, 0x00004000, 0x00008000,
	0x00010000, 0x00020000, 0x00040000, 0x00080000,
	0x00100000, 0x00200000, 0x00400000, 0x00800000,
	0x01000000, 0x02000000, 0x04000000, 0x08000000,
	0x10000000, 0x20000000, 0x40000000, 0x80000000
};

// 68K 0x1021749c __ct__7CBucketFiiPUcPUl
// FUNCTION: LEMBALL 0x00472ce0
Bucket::Bucket(int p_blockSize, int p_blockCount, unsigned char* p_memory, unsigned long* p_map)
	: Critical()
{
	m_child = 0;
	m_flags = 0;
	m_parent = 0;
	m_blockSize = p_blockSize;
	m_blockCount = p_blockCount;
	m_activeAllocations = 0;
	m_totalAllocations = 0;
	m_totalBytes = p_blockSize * p_blockCount;
	m_freeBytes = m_totalBytes;
	m_peakAllocations = 0;
	m_mapWordCount = (p_blockCount + 31) / 32;

	if (p_memory == 0) {
		int smallMemEnabled = g_nSmallMemoryEnabled;
		g_nSmallMemoryEnabled = 0;
		p_memory = (unsigned char*) operator new(m_totalBytes);
		g_nSmallMemoryEnabled = smallMemEnabled;
		m_flags |= 2;
	}

	if (p_map == 0) {
		int smallMemEnabled = g_nSmallMemoryEnabled;
		g_nSmallMemoryEnabled = 0;
		p_map = (unsigned long*) operator new(m_mapWordCount * sizeof(unsigned long));
		g_nSmallMemoryEnabled = smallMemEnabled;
		m_flags |= 1;
	}

	m_map = (unsigned int*) p_map;
	m_memory = p_memory;
	memset(p_map, 0, m_mapWordCount * sizeof(unsigned long));
	m_freeOffset.wWord = 0;
	m_freeOffset.wBit = 0;
}

// 68K 0x102175bc __dt__7CBucketFv
// FUNCTION: LEMBALL 0x00472dc0
Bucket::~Bucket()
{
	int smallMemEnabled = g_nSmallMemoryEnabled;
	g_nSmallMemoryEnabled = 0;
	if (m_child != 0) {
		RemoveChild();
	}
	if ((m_flags & 2) != 0) {
		if (m_memory != 0) {
			operator delete(m_memory);
			m_memory = 0;
		}
	}
	if ((m_flags & 1) != 0) {
		if (m_map != 0) {
			operator delete(m_map);
			m_map = 0;
		}
	}
	g_nSmallMemoryEnabled = smallMemEnabled;
}

// 68K 0x10217666 Allocate__7CBucketFPPUc
// FUNCTION: LEMBALL 0x00472e40
bool Bucket::Allocate(unsigned char** p_result)
{
	Bucket* current = this;
	while (true) {
		current->EnterCritical();
		if ((current->m_flags & 4) == 0) {
			break;
		}
		if (current->m_child == 0) {
			current->NewChild();
		}
		current->LeaveCritical();
		current = current->m_child;
	}
	*p_result = 0;
	Boffset offset = current->m_freeOffset;
	current->m_map[offset.wWord] |= s_bitMasks[offset.wBit];
	current->m_freeBytes -= current->m_blockSize;
	if ((int) current->m_freeBytes <= 0) {
		current->m_flags |= 4;
	}
	current->m_freeOffset = current->FindFreeOffset(offset);
	*p_result = current->m_memory + (offset.wWord * 32 + offset.wBit) * current->m_blockSize;
	current->m_totalAllocations++;
	current->m_activeAllocations++;
	if ((int) current->m_peakAllocations < (int) current->m_activeAllocations) {
		current->m_peakAllocations = current->m_activeAllocations;
	}
	current->LeaveCritical();
	return true;
}

// 68K 0x1021778c Free__7CBucketFPUc
// FUNCTION: LEMBALL 0x00472f00
bool Bucket::Free(unsigned char* p_memory)
{
	Bucket* current = this;
	while (true) {
		current->EnterCritical();
		if (p_memory > current->m_memory && current->m_memory + current->m_totalBytes > p_memory) {
			break;
		}
		current->LeaveCritical();
		current = current->m_child;
	}
	int index = (p_memory - current->m_memory) / (int) current->m_blockSize;
	Boffset offset;
	offset.wWord = (short) (index / 32);
	offset.wBit = (short) (index % 32);
	unsigned short oldFlags = current->m_flags;
	current->m_map[offset.wWord] &= ~s_bitMasks[offset.wBit];
	current->m_freeBytes += current->m_blockSize;
	current->m_flags &= ~4;
	current->m_activeAllocations--;
	if ((oldFlags & 4) != 0 || (current->m_freeOffset.wWord > offset.wWord && current->m_freeOffset.wBit > offset.wBit)) {
		current->m_freeOffset = offset;
	}
	if (current->m_totalBytes == current->m_freeBytes && current->m_parent != 0) {
		current->RemoveThis();
	}
	current->LeaveCritical();
	return true;
}

// 68K 0x102178a0 FindFreeOffset__7CBucketF10tagBOFFSET
// FUNCTION: LEMBALL 0x00472fd0
Boffset Bucket::FindFreeOffset(Boffset p_offset)
{
	if ((m_flags & 4) != 0) {
		return p_offset;
	}
	unsigned short word = p_offset.wWord;
	if (m_map[word] == 0xFFFFFFFF) {
		do {
			word++;
			if ((int) word >= (int) m_mapWordCount) {
				word = 0;
			}
		} while (m_map[word] == 0xFFFFFFFF);
	}
	p_offset.wBit = 0;
	unsigned int mask = m_map[word];
	unsigned int* pMask = s_bitMasks;
	while ((*pMask & mask) != 0) {
		p_offset.wBit++;
		pMask++;
		if (pMask >= s_bitMasks + 32) {
			break;
		}
	}
	p_offset.wWord = word;
	return p_offset;
}

// 68K 0x10217948 NewChild__7CBucketFv
// FUNCTION: LEMBALL 0x00473050
void Bucket::NewChild()
{
	*g_pSysOutput << "Allocating Small Memory Child: " << (int) m_blockSize << "\n";
	int smallMemEnabled = g_nSmallMemoryEnabled;
	g_nSmallMemoryEnabled = 0;
	g_pMasterArena->GetAllocSize();
	Bucket* child = new Bucket(m_blockSize, m_blockCount, 0, 0);
	g_pMasterArena->GetAllocSize();
	g_nSmallMemoryEnabled = smallMemEnabled;
	m_child = child;
	child->m_parent = this;
}

// 68K 0x10217a0e RemoveChild__7CBucketFv
// FUNCTION: LEMBALL 0x004730e0
void Bucket::RemoveChild()
{
	g_pMasterArena->GetAllocSize();
	if (m_child != 0) {
		delete m_child;
	}
	g_pMasterArena->GetAllocSize();
	m_child = 0;
}

// 68K 0x10217a74 RemoveThis__7CBucketFv
// FUNCTION: LEMBALL 0x00473120
void Bucket::RemoveThis()
{
	m_parent->RemoveChild();
}

// 68K 0x10217aa4 CheckValidPointer__7CBucketFPUc
// FUNCTION: LEMBALL 0x00473140
bool Bucket::CheckValidPointer(unsigned char* p_memory)
{
	Bucket* current = this;
	do {
		if (p_memory >= current->m_memory && current->m_totalBytes + current->m_memory > p_memory) {
			return 1;
		}
		current = current->m_child;
	} while (current != 0);
	return 0;
}
