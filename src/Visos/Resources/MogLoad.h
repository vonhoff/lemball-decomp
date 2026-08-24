#ifndef LEMBALL_VISOS_RESOURCES_MOGLOAD_H
#define LEMBALL_VISOS_RESOURCES_MOGLOAD_H

#include "../../Common.h"
#include "MogloadArena.h"
#include "RawRead.h"

#define kChunkDirc 0x44495243
#define kAnyChunkType 0xffffffff
#define kMogFormatVersion 3
#define kChunkInfoSize 0x38
#define kMogDirAllocSize 0x38

class MogDir {
public:
	void* operator new(size_t p_size) { return MogloadArena::operator new(p_size); }
	void* operator new(size_t, void* p_ptr) { return p_ptr; }
	void operator delete(void* p_data) { MogloadArena::operator delete(p_data); }

	ChunkInfo* NewChunkInfo();
	MogDir(unsigned long p_fileOffset);
	MogDir* GetNextDir();
	void Find(Chunk& p_chunk, unsigned int p_id, unsigned int p_recurse);
	void FindFirst(Chunk& p_chunk, unsigned int p_type);
	void FindNext(Chunk& p_chunk, unsigned int p_type);
	void GetChunkInfo(ChunkInfo* p_info);
	~MogDir();

	friend class MogRes;

private:
	int m_rootIndex;                   // 0x00
	ChunkInfo* m_rootChunk;            // 0x04
	int m_currentDirIndex;             // 0x08
	ChunkInfo* m_currentDirChunk;      // 0x0c
	unsigned int m_directoryEndOffset; // 0x10
	unsigned int m_payloadStartOffset; // 0x14
	int m_firstIndex;                  // 0x18
	ChunkInfo* m_firstChunk;           // 0x1c
	int m_iteratorIndex;               // 0x20
	ChunkInfo* m_iteratorChunk;        // 0x24
	int m_chunkCount;                  // 0x28
	int m_loadedChunkCount;            // 0x2c
	unsigned char* m_directoryData;    // 0x30
	undefined4 m_unk0x34;              // 0x34
};

extern int g_chunkIndex;
extern ChunkInfo* g_pChunkInfo;

#endif
