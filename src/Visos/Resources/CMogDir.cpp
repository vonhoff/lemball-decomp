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
int g_emptyChunkIndex = -1;

// GLOBAL: LEMBALL 0x004a1d7c
ChunkInfo* g_pEmptyChunkInfo = 0;

#define MOG_SEEK_FROM_START 0
#define MOG_DIRECTORY_ENTRY_STRIDE 9

// FUNCTION: LEMBALL 0x0045bda0
CMogDir::CMogDir(unsigned long p_fileOffset)
{
	Chunk chunk;
	unsigned int directoryDataSize;
	int* firstIndex;
	int* iteratorIndex;
	int* currentDirIndex;

	int chunkIndex = g_emptyChunkIndex;
	ChunkInfo* chunkInfo = g_pEmptyChunkInfo;
	firstIndex = &m_first.m_index;
	iteratorIndex = &m_iterator.m_index;
	m_loadedChunkCount = 0;
	*firstIndex = chunkIndex;
	*iteratorIndex = chunkIndex;
	m_first.m_info = chunkInfo;
	m_iterator.m_info = chunkInfo;
	chunkIndex = g_emptyChunkIndex;
	chunkInfo = g_pEmptyChunkInfo;
	m_root.m_index = chunkIndex;
	m_root.m_info = chunkInfo;
	currentDirIndex = &m_currentDir.m_index;
	*currentDirIndex = chunkIndex;
	m_currentDir.m_info = chunkInfo;
	vsSeek(g_pMogFile, p_fileOffset, MOG_SEEK_FROM_START);
	if (p_fileOffset == 0) {
		((CRawRead*) this)->InputByte();
		vsSeek(g_pMogFile, 0, MOG_SEEK_FROM_START);
	}
	((CRawRead*) this)->InputDword();
	((CRawRead*) this)->InputDword();
	m_chunkCount = ((CRawRead*) this)->InputDword();
	if (((CRawRead*) this)->InputDword() != MOG_FORMAT_VERSION) {
		_VSRELassert("IsValidResourceFile", "MOGLOAD.CPP", 0x1a2);
	}
	m_directoryEndOffset = ((CRawRead*) this)->InputDword();
	m_payloadStartOffset = vsTell(g_pMogFile);
	directoryDataSize = m_directoryEndOffset - m_payloadStartOffset;
	m_directoryData = (unsigned char*) CMogloadArena::operator new(directoryDataSize);
	vsRead(g_pMogFile, m_directoryData, directoryDataSize);
	if (m_chunkCount != 0) {
		m_first.m_info = (ChunkInfo*) CMogloadArena::operator new(CHUNK_INFO_ALLOCATION_BYTES);
		*firstIndex = 0;
		GetChunkInfo(m_first.m_info);
		m_loadedChunkCount++;
	}
	*iteratorIndex = *firstIndex;
	m_iterator.m_info = m_first.m_info;
	do {
		FindNext(chunk, RESOURCE_CHUNK_ANY_TYPE);
		if (chunk.m_info == 0) {
			break;
		}
		if (chunk.m_info->m_type == RESOURCE_CHUNK_DIRECTORY) {
			GetNextDir();
		}
	} while (chunk.m_info != 0);
	*iteratorIndex = *firstIndex;
	m_iterator.m_info = m_first.m_info;
	m_currentDir.m_index = m_root.m_index;
	m_currentDir.m_info = m_root.m_info;
}

// FUNCTION: LEMBALL 0x0045bf10
CMogDir::~CMogDir()
{
	Chunk* first;
	Chunk* iterator;
	ChunkInfo* chunk;
	Chunk* next;

	iterator = &m_iterator;
	first = &m_first;
	m_iterator = m_first;
	chunk = m_first.m_info;
	while (chunk != 0) {
		*first = *iterator;
		chunk = m_first.m_info;
		next = (Chunk*) &chunk->m_nextIndex;
		*iterator = *next;
		if (chunk->m_type == RESOURCE_CHUNK_DIRECTORY && chunk->m_directory != 0) {
			CMogloadArena::operator delete(chunk->m_directory);
			m_first.m_info->m_directory = 0;
		}
		CMogloadArena::operator delete(m_first.m_info);
		m_first.m_info = 0;
		chunk = m_iterator.m_info;
	}
	if (m_directoryData != 0) {
		CMogloadArena::operator delete(m_directoryData);
		m_directoryData = 0;
	}
}

// FUNCTION: LEMBALL 0x0045bfa0
void CMogDir::GetChunkInfo(ChunkInfo* p_info)
{
	vsSeek(g_pMogFile, (m_iterator.m_index * 4 + 4) * MOG_DIRECTORY_ENTRY_STRIDE + m_directoryEndOffset, 0);
	p_info->m_next = 0;
	p_info->m_child.m_info = 0;
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
	ChunkInfo* info = (ChunkInfo*) CMogloadArena::operator new(CHUNK_INFO_ALLOCATION_BYTES);
	m_iterator.m_info->m_next = info;
	m_iterator.m_info->m_nextIndex = m_loadedChunkCount;
	m_loadedChunkCount++;
	GetChunkInfo(info);
	return info;
}

// FUNCTION: LEMBALL 0x0045c060
CMogDir* CMogDir::GetNextDir()
{
	Chunk chunk;
	Chunk* current;
	CMogDir* dir;

	chunk.m_info = 0;
	if (m_root.m_info == 0) {
		FindFirst(chunk, RESOURCE_CHUNK_DIRECTORY);
		if (chunk.m_info == 0) {
			m_currentDir = m_root;
			return 0;
		}
		if (chunk.m_info->m_type == RESOURCE_CHUNK_DIRECTORY) {
			dir = (CMogDir*) CMogloadArena::operator new(MOG_DIRECTORY_ALLOCATION_BYTES);
			if (dir == 0) {
				chunk.m_info->m_directory = 0;
			}
			else {
				chunk.m_info->m_directory = new (dir) CMogDir(chunk.m_info->m_fileOffset);
			}
			m_root = chunk;
		}
		else {
			m_root.m_info = 0;
		}
		m_currentDir = m_root;
		return m_currentDir.m_info->m_directory;
	}

	current = &m_currentDir;
	if (current->m_index != -1) {

		if (m_currentDir.m_info->m_child.m_info != 0) {
			*current = m_currentDir.m_info->m_child;
			return m_currentDir.m_info->m_directory;
		}

		m_iterator = *current;
		FindNext(chunk, RESOURCE_CHUNK_DIRECTORY);
		if (chunk.m_info == 0) {
			return 0;
		}
		if (chunk.m_info->m_type == RESOURCE_CHUNK_DIRECTORY) {
			chunk.m_info->m_directory = new CMogDir(chunk.m_info->m_fileOffset);
			m_currentDir.m_info->m_child = chunk;
			*current = chunk;
			return m_currentDir.m_info->m_directory;
		}
		m_currentDir.m_info->m_child.m_info = 0;
		return m_currentDir.m_info->m_directory;
	}
	*current = m_root;
	return m_currentDir.m_info->m_directory;
}

// FUNCTION: LEMBALL 0x0045c200
void CMogDir::FindNext(Chunk& p_chunk, unsigned int p_type)
{
	int exhausted = 0;
	unsigned int type = p_type;
	Chunk* iterator = &m_iterator;
	Chunk* next;

	do {
		if (iterator->m_index != -1) {
			if (m_chunkCount - iterator->m_index == 1) {
				exhausted = 1;
				break;
			}
			if (m_iterator.m_info->m_next == 0) {
				NewChunkInfo();
			}
			next = (Chunk*) &m_iterator.m_info->m_nextIndex;
		}
		else {
			next = &m_first;
		}
		*iterator = *next;
		if ((int) type == -1) {
			break;
		}
	} while (m_iterator.m_info->m_type != type);

	if ((int) type == -1 || m_iterator.m_info->m_type == type) {
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
	Chunk* iterator = &m_iterator;
	Chunk* first = &m_first;

	*iterator = *first;
	m_iterator.m_index = -1;
	FindNext(p_chunk, p_type);
}

// FUNCTION: LEMBALL 0x0045c2d0
void CMogDir::Find(Chunk& p_chunk, unsigned int p_id, unsigned int p_recurse)
{
	Chunk saved;
	CMogDir* dir;
	Chunk* current;
	Chunk* root;

	FindFirst(p_chunk, RESOURCE_CHUNK_ANY_TYPE);
	while (p_chunk.m_info != 0 && p_chunk.m_info->m_id != p_id) {
		FindNext(p_chunk, RESOURCE_CHUNK_ANY_TYPE);
	}
	if (p_chunk.m_info == 0) {
		current = (Chunk*) &m_currentDir.m_index;
		saved = *current;
		root = (Chunk*) &m_root.m_index;
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
