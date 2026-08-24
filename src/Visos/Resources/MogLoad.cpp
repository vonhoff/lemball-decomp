#include "MogLoad.h"

#include <new.h>
#include <string.h>

#include "../Foundation/Chunk.h"
#include "../Foundation/ChunkInfo.h"
#include "../Foundation/VsDebug.h"
#include "../Foundation/VsFile.h"
#include "MogloadArena.h"
#include "RawRead.h"

// GLOBAL: LEMBALL 0x004a1d78
int g_chunkIndex = -1;

// GLOBAL: LEMBALL 0x004a1d7c
ChunkInfo* g_pChunkInfo = 0;

#define kSeekSet 0
#define kDirectoryEntryStride 9

// 68K 0x1020189e __ct__7CMogDirFUl
// ASSERT: _VSRELassert("IsValidResourceFile", "MOGLOAD.CPP", 418)
// FUNCTION: LEMBALL 0x0045bda0
MogDir::MogDir(unsigned long p_fileOffset)
{
	Chunk chunk;
	unsigned int size;
	int* firstIndex;
	int* iteratorIndex;
	int* currentDir;

	firstIndex = &m_firstIndex;
	iteratorIndex = &m_iteratorIndex;
	m_loadedChunkCount = 0;
	*firstIndex = g_chunkIndex;
	*iteratorIndex = g_chunkIndex;
	m_firstChunk = g_pChunkInfo;
	m_iteratorChunk = g_pChunkInfo;
	m_rootIndex = g_chunkIndex;
	m_rootChunk = g_pChunkInfo;
	currentDir = &m_currentDirIndex;
	*currentDir = g_chunkIndex;
	m_currentDirChunk = g_pChunkInfo;
	VsSeek(g_pMogFile, p_fileOffset, kSeekSet);
	if (p_fileOffset == 0) {
		((RawRead*) this)->InputByte();
		VsSeek(g_pMogFile, 0, kSeekSet);
	}
	((RawRead*) this)->InputDword();
	((RawRead*) this)->InputDword();
	m_chunkCount = ((RawRead*) this)->InputDword();
	if (((RawRead*) this)->InputDword() != kMogFormatVersion) {
		VsRelAssert("IsValidResourceFile", "MOGLOAD.CPP", 0x1a2);
	}
	m_directoryEndOffset = ((RawRead*) this)->InputDword();
	m_payloadStartOffset = VsTell(g_pMogFile);
	size = m_directoryEndOffset - m_payloadStartOffset;
	m_directoryData = (unsigned char*) MogloadArena::operator new(size);
	VsRead(g_pMogFile, m_directoryData, size);
	if (m_chunkCount != 0) {
		m_firstChunk = (ChunkInfo*) MogloadArena::operator new(kChunkInfoSize);
		*firstIndex = 0;
		GetChunkInfo(m_firstChunk);
		m_loadedChunkCount++;
	}
	*iteratorIndex = *firstIndex;
	m_iteratorChunk = m_firstChunk;
	do {
		FindNext(chunk, kAnyChunkType);
		if (chunk.m_info == 0) {
			break;
		}
		if (chunk.m_info->m_type == kChunkDirc) {
			GetNextDir();
		}
	} while (chunk.m_info != 0);
	*iteratorIndex = *firstIndex;
	m_iteratorChunk = m_firstChunk;
	m_currentDirIndex = m_rootIndex;
	m_currentDirChunk = m_rootChunk;
}

// 68K 0x10201a68 __dt__7CMogDirFv
// FUNCTION: LEMBALL 0x0045bf10
MogDir::~MogDir()
{
	int* iterator;
	int* first;
	ChunkInfo* chunk;
	int* next;

	iterator = &m_iteratorIndex;
	first = &m_firstIndex;
	iterator[0] = first[0];
	iterator[1] = first[1];
	chunk = m_firstChunk;
	while (chunk != 0) {
		first[0] = iterator[0];
		first[1] = iterator[1];
		chunk = m_firstChunk;
		next = &chunk->m_nextIndex;
		iterator[0] = next[0];
		iterator[1] = next[1];
		if (chunk->m_type == kChunkDirc && chunk->m_directory != 0) {
			MogloadArena::operator delete(chunk->m_directory);
			m_firstChunk->m_directory = 0;
		}
		MogloadArena::operator delete(m_firstChunk);
		m_firstChunk = 0;
		chunk = m_iteratorChunk;
	}
	if (m_directoryData != 0) {
		MogloadArena::operator delete(m_directoryData);
		m_directoryData = 0;
	}
}

// 68K 0x10201b26 GetChunkInfo__7CMogDirFP12tagChunkInfo
// FUNCTION: LEMBALL 0x0045bfa0
void MogDir::GetChunkInfo(ChunkInfo* p_info)
{
	VsSeek(g_pMogFile, (m_iteratorIndex * 4 + 4) * kDirectoryEntryStride + m_directoryEndOffset, 0);
	p_info->m_next = 0;
	p_info->m_child = 0;
	p_info->m_directory = 0;
	p_info->m_data = m_directoryData + (((RawRead*) this)->InputDword() - m_payloadStartOffset);
	p_info->m_id = ((RawRead*) this)->InputDword();
	p_info->m_type = ((RawRead*) this)->InputDword();
	p_info->m_fileOffset = ((RawRead*) this)->InputDword();
	p_info->m_size = ((RawRead*) this)->InputDword();
	VsRead(g_pMogFile, p_info->m_name, sizeof(p_info->m_name));
}

// 68K 0x10201be8 NewChunkInfo__7CMogDirFv
// FUNCTION: LEMBALL 0x0045c030
ChunkInfo* MogDir::NewChunkInfo()
{
	ChunkInfo* info = (ChunkInfo*) MogloadArena::operator new(kChunkInfoSize);
	m_iteratorChunk->m_next = info;
	m_iteratorChunk->m_nextIndex = m_loadedChunkCount;
	m_loadedChunkCount++;
	GetChunkInfo(info);
	return info;
}

// 68K 0x10201c48 GetNextDir__7CMogDirFv
// FUNCTION: LEMBALL 0x0045c060
MogDir* MogDir::GetNextDir()
{
	Chunk chunk;
	int* current;
	MogDir* dir;

	chunk.m_info = 0;
	if (m_rootChunk == 0) {
		FindFirst(chunk, kChunkDirc);
		if (chunk.m_info == 0) {
			m_currentDirIndex = m_rootIndex;
			m_currentDirChunk = m_rootChunk;
			return 0;
		}
		if (chunk.m_info->m_type == kChunkDirc) {
			dir = (MogDir*) MogloadArena::operator new(kMogDirAllocSize);
			if (dir == 0) {
				chunk.m_info->m_directory = 0;
			}
			else {
				chunk.m_info->m_directory = new (dir) MogDir(chunk.m_info->m_fileOffset);
			}
			m_rootIndex = chunk.m_index;
			m_rootChunk = chunk.m_info;
		}
		else {
			m_rootChunk = 0;
		}
		m_currentDirIndex = m_rootIndex;
		m_currentDirChunk = m_rootChunk;
		return m_currentDirChunk->m_directory;
	}

	current = &m_currentDirIndex;
	if (*current == -1) {
		*current = m_rootIndex;
		m_currentDirChunk = m_rootChunk;
		return m_currentDirChunk->m_directory;
	}

	if (m_currentDirChunk->m_child != 0) {
		*current = m_currentDirChunk->m_childIndex;
		m_currentDirChunk = m_currentDirChunk->m_child;
		return m_currentDirChunk->m_directory;
	}

	m_iteratorIndex = *current;
	m_iteratorChunk = m_currentDirChunk;
	FindNext(chunk, kChunkDirc);
	if (chunk.m_info == 0) {
		return 0;
	}
	if (chunk.m_info->m_type == kChunkDirc) {
		dir = (MogDir*) MogloadArena::operator new(kMogDirAllocSize);
		if (dir == 0) {
			chunk.m_info->m_directory = 0;
		}
		else {
			chunk.m_info->m_directory = new (dir) MogDir(chunk.m_info->m_fileOffset);
		}
		m_currentDirChunk->m_childIndex = chunk.m_index;
		m_currentDirChunk->m_child = chunk.m_info;
		*current = chunk.m_index;
		m_currentDirChunk = chunk.m_info;
		return m_currentDirChunk->m_directory;
	}
	m_currentDirChunk->m_child = 0;
	return m_currentDirChunk->m_directory;
}

// 68K 0x10201dd6 FindNext__7CMogDirFR8tagCHUNK7CHUNKID
// FUNCTION: LEMBALL 0x0045c200
void MogDir::FindNext(Chunk& p_chunk, unsigned int p_type)
{
	int exhausted = 0;
	int* iterator = &m_iteratorIndex;
	int* nextIndex;

	do {
		if (*iterator == -1) {
			nextIndex = &m_firstIndex;
		}
		else {
			if (m_chunkCount - *iterator == 1) {
				exhausted = 1;
				break;
			}
			if (m_iteratorChunk->m_next == 0) {
				NewChunkInfo();
			}
			nextIndex = &m_iteratorChunk->m_nextIndex;
		}
		iterator[0] = nextIndex[0];
		iterator[1] = nextIndex[1];
		if ((int) p_type == -1) {
			break;
		}
	} while (m_iteratorChunk->m_type != p_type);

	if ((int) p_type == -1 || m_iteratorChunk->m_type == p_type) {
		if (exhausted == 0) {
			p_chunk.m_index = iterator[0];
			p_chunk.m_info = (ChunkInfo*) iterator[1];
			return;
		}
	}
	p_chunk.m_info = 0;
}

// 68K 0x10201e9a FindFirst__7CMogDirFR8tagCHUNK7CHUNKID
// FUNCTION: LEMBALL 0x0045c2a0
void MogDir::FindFirst(Chunk& p_chunk, unsigned int p_type)
{
	int* iteratorIndex = &m_iteratorIndex;
	int* firstIndex = &m_firstIndex;

	*iteratorIndex = *firstIndex;
	iteratorIndex[1] = firstIndex[1];
	*iteratorIndex = -1;
	FindNext(p_chunk, p_type);
}

// 68K 0x10201ef8 Find__7CMogDirFR8tagCHUNKUl7RECURSE
// FUNCTION: LEMBALL 0x0045c2d0
void MogDir::Find(Chunk& p_chunk, unsigned int p_id, unsigned int p_recurse)
{
	ChunkInfo* savedChunk;
	int savedIndex;
	MogDir* dir;
	int* current;

	FindFirst(p_chunk, kAnyChunkType);
	while (p_chunk.m_info != 0 && p_chunk.m_info->m_id != p_id) {
		FindNext(p_chunk, kAnyChunkType);
	}
	if (p_chunk.m_info == 0) {
		current = &m_currentDirIndex;
		savedIndex = *current;
		savedChunk = m_currentDirChunk;
		current[0] = m_rootIndex;
		current[1] = (int) m_rootChunk;
		*current = -1;
		while (p_chunk.m_info == 0) {
			dir = GetNextDir();
			if (dir == 0) {
				break;
			}
			dir->Find(p_chunk, p_id, p_recurse);
		}
		current[0] = savedIndex;
		current[1] = (int) savedChunk;
	}
}
