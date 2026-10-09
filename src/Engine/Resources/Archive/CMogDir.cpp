#include "CMogDir.h"

#include "CMogloadArena.h"
#include "CRawRead.h"
#include "tagCHUNK.h"
#include "tagChunkInfo.h"
#include "Engine/Diagnostics/VsDebug.h"
#include "Engine/Files/VsFile.h"

#include <stddef.h>

// GLOBAL: LEMBALL 0x004a1d78
int g_emptyChunkIndex = CHUNK_INDEX_BEFORE_FIRST_ENTRY;

// GLOBAL: LEMBALL 0x004a1d7c
tagChunkInfo* g_pEmptyChunkInfo = NULL;

#define MOG_SEEK_FROM_START 0
#define MOG_DIRECTORY_ENTRY_STRIDE 36

// FUNCTION: LEMBALL 0x0045bda0
CMogDir::CMogDir(unsigned long p_fileOffset)
{
	tagCHUNK chunk;
	unsigned int directoryDataSize;
	int* firstIndex;
	int* iteratorIndex;
	int* currentDirIndex;

	int chunkIndex = g_emptyChunkIndex;
	tagChunkInfo* chunkInfo = g_pEmptyChunkInfo;
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
		InputByte();
		vsSeek(g_pMogFile, p_fileOffset, MOG_SEEK_FROM_START);
	}
	InputDword();
	InputDword();
	m_chunkCount = InputDword();
	if (InputDword() != MOG_FORMAT_VERSION) {
		_VSRELassert("IsValidResourceFile", "MOGLOAD.CPP", 0x1a2);
	}
	m_directoryEndOffset = InputDword();
	m_payloadStartOffset = vsTell(g_pMogFile);
	directoryDataSize = m_directoryEndOffset - m_payloadStartOffset;
	m_directoryData = (unsigned char*) CMogloadArena::operator new(directoryDataSize);
	vsRead(g_pMogFile, m_directoryData, directoryDataSize);
	if (m_chunkCount != 0) {
		tagChunkInfo* info = (tagChunkInfo*) CMogloadArena::operator new(sizeof(tagChunkInfo));
		m_first.m_info = info;
		*firstIndex = 0;
		GetChunkInfo(info);
		m_loadedChunkCount++;
	}
	*iteratorIndex = *firstIndex;
	m_iterator.m_info = m_first.m_info;
	do {
		FindNext(chunk, RESOURCE_CHUNK_ANY_TYPE);
		if (chunk.m_info == NULL) {
			break;
		}
		if (chunk.m_info->m_type == RESOURCE_CHUNK_DIRECTORY) {
			GetNextDir();
		}
	} while (chunk.m_info != NULL);
	*iteratorIndex = *firstIndex;
	m_iterator.m_info = m_first.m_info;
	m_currentDir.m_index = m_root.m_index;
	m_currentDir.m_info = m_root.m_info;
}

// FUNCTION: LEMBALL 0x0045bf10
CMogDir::~CMogDir()
{
	tagCHUNK* first;
	tagCHUNK* iterator;
	tagChunkInfo* chunk;
	tagCHUNK* next;

	iterator = &m_iterator;
	first = &m_first;
	m_iterator = m_first;
	chunk = m_first.m_info;
	while (chunk != NULL) {
		*first = *iterator;
		chunk = m_first.m_info;
		next = &chunk->m_next;
		*iterator = *next;
		if (chunk->m_type == RESOURCE_CHUNK_DIRECTORY && chunk->m_directory != NULL) {
			CMogloadArena::operator delete(chunk->m_directory);
			m_first.m_info->m_directory = NULL;
		}
		CMogloadArena::operator delete(m_first.m_info);
		m_first.m_info = NULL;
		chunk = m_iterator.m_info;
	}
	if (m_directoryData != NULL) {
		CMogloadArena::operator delete(m_directoryData);
		m_directoryData = NULL;
	}
}

// FUNCTION: LEMBALL 0x0045bfa0
void CMogDir::GetChunkInfo(tagChunkInfo* p_info)
{
	vsSeek(g_pMogFile,
		   (m_iterator.m_index + 1) * MOG_DIRECTORY_ENTRY_STRIDE + m_directoryEndOffset,
		   MOG_SEEK_FROM_START);
	p_info->m_next.m_info = NULL;
	p_info->m_child.m_info = NULL;
	p_info->m_directory = NULL;
	p_info->m_data = m_directoryData + (InputDword() - m_payloadStartOffset);
	p_info->m_id = InputDword();
	p_info->m_type = InputDword();
	p_info->m_fileOffset = InputDword();
	p_info->m_size = InputDword();
	vsRead(g_pMogFile, p_info->m_name, sizeof(p_info->m_name));
}

// FUNCTION: LEMBALL 0x0045c030
tagChunkInfo* CMogDir::NewChunkInfo()
{
	tagChunkInfo* info = (tagChunkInfo*) CMogloadArena::operator new(sizeof(tagChunkInfo));
	m_iterator.m_info->m_next.m_info = info;
	m_iterator.m_info->m_next.m_index = m_loadedChunkCount;
	m_loadedChunkCount++;
	GetChunkInfo(info);
	return info;
}

// FUNCTION: LEMBALL 0x0045c060
CMogDir* CMogDir::GetNextDir()
{
	tagCHUNK chunk;
	tagCHUNK* current;
	CMogDir* dir;

	chunk.m_info = NULL;
	if (m_root.m_info == NULL) {
		FindFirst(chunk, RESOURCE_CHUNK_DIRECTORY);
		if (chunk.m_info == NULL) {
			m_currentDir = m_root;
			return NULL;
		}
		if (chunk.m_info->m_type == RESOURCE_CHUNK_DIRECTORY) {
			dir = (CMogDir*) CMogloadArena::operator new(sizeof(CMogDir));
			if (dir == NULL) {
				chunk.m_info->m_directory = NULL;
			}
			else {
				chunk.m_info->m_directory = new (dir) CMogDir(chunk.m_info->m_fileOffset);
			}
			m_root = chunk;
		}
		else {
			m_root.m_info = NULL;
		}
		m_currentDir = m_root;
		return m_currentDir.m_info->m_directory;
	}

	current = &m_currentDir;
	if (current->m_index != CHUNK_INDEX_BEFORE_FIRST_ENTRY) {

		if (m_currentDir.m_info->m_child.m_info != NULL) {
			*current = m_currentDir.m_info->m_child;
			return m_currentDir.m_info->m_directory;
		}

		m_iterator = *current;
		FindNext(chunk, RESOURCE_CHUNK_DIRECTORY);
		if (chunk.m_info == NULL) {
			return NULL;
		}
		if (chunk.m_info->m_type == RESOURCE_CHUNK_DIRECTORY) {
			chunk.m_info->m_directory = new CMogDir(chunk.m_info->m_fileOffset);
			m_currentDir.m_info->m_child = chunk;
			*current = chunk;
			return m_currentDir.m_info->m_directory;
		}
		m_currentDir.m_info->m_child.m_info = NULL;
		return m_currentDir.m_info->m_directory;
	}
	*current = m_root;
	return m_currentDir.m_info->m_directory;
}

// FUNCTION: LEMBALL 0x0045c200
void CMogDir::FindNext(tagCHUNK& p_chunk, unsigned int p_type)
{
	int exhausted = 0;
	unsigned int type = p_type;
	tagCHUNK* iterator = &m_iterator;
	tagCHUNK* next;

	do {
		if (iterator->m_index != CHUNK_INDEX_BEFORE_FIRST_ENTRY) {
			if (m_chunkCount - iterator->m_index == 1) {
				exhausted = 1;
				break;
			}
			if (m_iterator.m_info->m_next.m_info == NULL) {
				NewChunkInfo();
			}
			next = &m_iterator.m_info->m_next;
		}
		else {
			next = &m_first;
		}
		*iterator = *next;
		if (type == RESOURCE_CHUNK_ANY_TYPE) {
			break;
		}
	} while (m_iterator.m_info->m_type != type);

	if (type == RESOURCE_CHUNK_ANY_TYPE || m_iterator.m_info->m_type == type) {
		if (exhausted == 0) {
			p_chunk = *iterator;
			return;
		}
	}
	p_chunk.m_info = NULL;
}

// FUNCTION: LEMBALL 0x0045c2a0
void CMogDir::FindFirst(tagCHUNK& p_chunk, unsigned int p_type)
{
	tagCHUNK* iterator = &m_iterator;
	tagCHUNK* first = &m_first;

	*iterator = *first;
	m_iterator.m_index = CHUNK_INDEX_BEFORE_FIRST_ENTRY;
	FindNext(p_chunk, p_type);
}

// FUNCTION: LEMBALL 0x0045c2d0
void CMogDir::Find(tagCHUNK& p_chunk, unsigned int p_id, RECURSE p_recurse)
{
	tagCHUNK saved;
	CMogDir* dir;
	tagCHUNK* current;
	tagCHUNK* root;

	FindFirst(p_chunk, RESOURCE_CHUNK_ANY_TYPE);
	while (p_chunk.m_info != NULL && p_chunk.m_info->m_id != p_id) {
		FindNext(p_chunk, RESOURCE_CHUNK_ANY_TYPE);
	}
	if (p_chunk.m_info == NULL) {
		current = &m_currentDir;
		saved = *current;
		root = &m_root;
		current->m_index = root->m_index;
		current->m_info = root->m_info;
		current->m_index = CHUNK_INDEX_BEFORE_FIRST_ENTRY;
		while (p_chunk.m_info == NULL) {
			dir = GetNextDir();
			if (dir == NULL) {
				break;
			}
			dir->Find(p_chunk, p_id, p_recurse);
		}
		*current = saved;
	}
}
