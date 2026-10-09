#ifndef LEMBALL_VISOS_RESOURCES_CMOGDIR_H
#define LEMBALL_VISOS_RESOURCES_CMOGDIR_H
#include "CMogloadArena.h"
#include "CRawRead.h"
#include "RECURSE.h"
#include "tagCHUNK.h"

#include <stddef.h>

struct tagChunkInfo;

#define RESOURCE_CHUNK_DIRECTORY 0x44495243
#define RESOURCE_CHUNK_ANY_TYPE 0xffffffff
#define MOG_FORMAT_VERSION 3

// SIZE 0x38
class CMogDir : public CRawRead {
public:
	void* operator new(size_t p_size) { return CMogloadArena::operator new(p_size); }
	void* operator new(size_t, void* p_ptr) { return p_ptr; }
	void operator delete(void* p_data) { CMogloadArena::operator delete(p_data); }

	tagChunkInfo* NewChunkInfo();
	CMogDir(unsigned long p_fileOffset);
	CMogDir* GetNextDir();
	void Find(tagCHUNK& p_chunk, unsigned long p_id, RECURSE p_recurse);
	void FindFirst(tagCHUNK& p_chunk, unsigned int p_type);
	void FindNext(tagCHUNK& p_chunk, unsigned int p_type);
	void GetChunkInfo(tagChunkInfo* p_info);
	~CMogDir();

	friend class CMogRes;

private:
	tagCHUNK m_root;                   // 0x00
	tagCHUNK m_currentDir;             // 0x08
	unsigned int m_directoryEndOffset; // 0x10
	unsigned int m_payloadStartOffset; // 0x14
	tagCHUNK m_first;                  // 0x18
	tagCHUNK m_iterator;               // 0x20
	int m_chunkCount;                  // 0x28
	int m_loadedChunkCount;            // 0x2c
	unsigned char* m_directoryData;    // 0x30
	unsigned int m_unk0x34;            // 0x34
};

extern int g_emptyChunkIndex;
extern tagChunkInfo* g_pEmptyChunkInfo;

#endif
