#include "CMogDir.h"

#include "../Foundation/Chunk.h"
#include "../Foundation/ChunkInfo.h"
#include "../Foundation/VsDebug.h"
#include "../Foundation/VsFile.h"
#include "CMogloadArena.h"
#include "CRawRead.h"

#include <new.h>
#include <string.h>

// GLOBAL: LEMBALL 0x004a1d78
int g_chunkIndex = -1;

// GLOBAL: LEMBALL 0x004a1d7c
ChunkInfo* g_pChunkInfo = 0;

#define kSeekSet 0
#define kDirectoryEntryStride 9

// FUNCTION: LEMBALL 0x0045bda0
CMogDir::CMogDir(unsigned long p_fileOffset)
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
	vsSeek(g_pMogFile, p_fileOffset, kSeekSet);
	if (p_fileOffset == 0) {
		((CRawRead*) this)->InputByte();
		vsSeek(g_pMogFile, 0, kSeekSet);
	}
	((CRawRead*) this)->InputDword();
	((CRawRead*) this)->InputDword();
	m_chunkCount = ((CRawRead*) this)->InputDword();
	if (((CRawRead*) this)->InputDword() != kMogFormatVersion) {
		_VSRELassert("IsValidResourceFile", "MOGLOAD.CPP", 0x1a2);
	}
	m_directoryEndOffset = ((CRawRead*) this)->InputDword();
	m_payloadStartOffset = vsTell(g_pMogFile);
	size = m_directoryEndOffset - m_payloadStartOffset;
	m_directoryData = (unsigned char*) CMogloadArena::operator new(size);
	vsRead(g_pMogFile, m_directoryData, size);
	if (m_chunkCount != 0) {
		m_firstChunk = (ChunkInfo*) CMogloadArena::operator new(kChunkInfoSize);
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

// FUNCTION: LEMBALL 0x0045bf10
CMogDir::~CMogDir()
{
	Chunk* first;
	Chunk* iterator;
	ChunkInfo* chunk;
	Chunk* next;

	iterator = (Chunk*) &m_iteratorIndex;
	first = (Chunk*) &m_firstIndex;
	*iterator = *first;
	chunk = m_firstChunk;
	while (chunk != 0) {
		*first = *iterator;
		chunk = m_firstChunk;
		next = (Chunk*) &chunk->m_nextIndex;
		*iterator = *next;
		if (chunk->m_type == kChunkDirc && chunk->m_directory != 0) {
			CMogloadArena::operator delete(chunk->m_directory);
			m_firstChunk->m_directory = 0;
		}
		CMogloadArena::operator delete(m_firstChunk);
		m_firstChunk = 0;
		chunk = m_iteratorChunk;
	}
	if (m_directoryData != 0) {
		CMogloadArena::operator delete(m_directoryData);
		m_directoryData = 0;
	}
}

// FUNCTION: LEMBALL 0x0045bfa0
void CMogDir::GetChunkInfo(ChunkInfo* p_info)
{
	vsSeek(g_pMogFile, (m_iteratorIndex * 4 + 4) * kDirectoryEntryStride + m_directoryEndOffset, 0);
	p_info->m_next = 0;
	p_info->m_child = 0;
	p_info->m_directory = 0;
	p_info->m_data = m_directoryData + (((CRawRead*) this)->InputDword() - m_payloadStartOffset);
	p_info->m_id = ((CRawRead*) this)->InputDword();
	p_info->m_type = ((CRawRead*) this)->InputDword();
	p_info->m_fileOffset = ((CRawRead*) this)->InputDword();
	p_info->m_size = ((CRawRead*) this)->InputDword();
	vsRead(g_pMogFile, p_info->m_name, sizeof(p_info->m_name));
}

// FUNCTION: LEMBALL 0x0045c030
ChunkInfo* CMogDir::NewChunkInfo()
{
	ChunkInfo* info = (ChunkInfo*) CMogloadArena::operator new(kChunkInfoSize);
	m_iteratorChunk->m_next = info;
	m_iteratorChunk->m_nextIndex = m_loadedChunkCount;
	m_loadedChunkCount++;
	GetChunkInfo(info);
	return info;
}

// FUNCTION: LEMBALL 0x0045c060
CMogDir* CMogDir::GetNextDir()
{
	Chunk chunk;
	int* current;
	CMogDir* dir;

	chunk.m_info = 0;
	if (m_rootChunk == 0) {
		FindFirst(chunk, kChunkDirc);
		if (chunk.m_info == 0) {
			m_currentDirIndex = m_rootIndex;
			m_currentDirChunk = m_rootChunk;
			return 0;
		}
		if (chunk.m_info->m_type == kChunkDirc) {
			dir = (CMogDir*) CMogloadArena::operator new(kMogDirAllocSize);
			if (dir == 0) {
				chunk.m_info->m_directory = 0;
			}
			else {
				chunk.m_info->m_directory = new (dir) CMogDir(chunk.m_info->m_fileOffset);
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
		dir = (CMogDir*) CMogloadArena::operator new(kMogDirAllocSize);
		if (dir == 0) {
			chunk.m_info->m_directory = 0;
		}
		else {
			chunk.m_info->m_directory = new (dir) CMogDir(chunk.m_info->m_fileOffset);
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

// FUNCTION: LEMBALL 0x0045c200
void CMogDir::FindNext(Chunk& p_chunk, unsigned int p_type)
{
	int exhausted = 0;
	unsigned int type = p_type;
	Chunk* iterator = (Chunk*) &m_iteratorIndex;
	Chunk* next;

	do {
		if (iterator->m_index != -1) {
			if (m_chunkCount - iterator->m_index == 1) {
				exhausted = 1;
				break;
			}
			if (m_iteratorChunk->m_next == 0) {
				NewChunkInfo();
			}
			next = (Chunk*) &m_iteratorChunk->m_nextIndex;
		}
		else {
			next = (Chunk*) &m_firstIndex;
		}
		*iterator = *next;
		if ((int) type == -1) {
			break;
		}
	} while (m_iteratorChunk->m_type != type);

	if ((int) type == -1 || m_iteratorChunk->m_type == type) {
		if (exhausted == 0) {
			p_chunk = *iterator;
			return;
		}
	}
	p_chunk.m_info = 0;
}

// FUNCTION: LEMBALL 0x0045c2a0
void CMogDir::FindFirst(Chunk& p_chunk, unsigned int p_type)
{
	Chunk* iterator = (Chunk*) &m_iteratorIndex;
	Chunk* first = (Chunk*) &m_firstIndex;

	*iterator = *first;
	m_iteratorIndex = -1;
	FindNext(p_chunk, p_type);
}

// FUNCTION: LEMBALL 0x0045c2d0
void CMogDir::Find(Chunk& p_chunk, unsigned int p_id, unsigned int p_recurse)
{
	Chunk saved;
	CMogDir* dir;
	Chunk* current;
	Chunk* root;

	FindFirst(p_chunk, kAnyChunkType);
	while (p_chunk.m_info != 0 && p_chunk.m_info->m_id != p_id) {
		FindNext(p_chunk, kAnyChunkType);
	}
	if (p_chunk.m_info == 0) {
		current = (Chunk*) &m_currentDirIndex;
		saved = *current;
		root = (Chunk*) &m_rootIndex;
		*current = *root;
		current->m_index = -1;
		while (p_chunk.m_info == 0) {
			dir = GetNextDir();
			if (dir == 0) {
				break;
			}
			dir->Find(p_chunk, p_id, p_recurse);
		}
		*current = saved;
	}
}
