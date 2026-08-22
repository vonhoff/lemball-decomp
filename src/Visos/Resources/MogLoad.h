#ifndef LEMBALL_SCAFFOLD_VISOS_RESOURCES_MOGLOAD_H
#define LEMBALL_SCAFFOLD_VISOS_RESOURCES_MOGLOAD_H

#include "../../Common.h"

// SIZE 0x34
class MogDir {
public:
	ChunkInfo* NewChunkInfo();
	MogDir(unsigned long p_arg0);
	MogDir* GetNextDir();
	void Find(Chunk& p_chunk, unsigned int p_id, unsigned int p_recurse);
	void FindFirst(Chunk& p_chunk, unsigned int p_type);
	void FindNext(Chunk& p_chunk, unsigned int p_type);
	void GetChunkInfo(ChunkInfo* p_info);
	~MogDir();

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
};

#endif
