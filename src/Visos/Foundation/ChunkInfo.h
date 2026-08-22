#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_CHUNKINFO_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_CHUNKINFO_H

#include "../../Common.h"

// SIZE 0x38
struct ChunkInfo {
public:
	// No functions mapped yet.

	unsigned char* m_data;     // 0x00
	unsigned int m_type;       // 0x04
	unsigned int m_id;         // 0x08
	unsigned int m_fileOffset; // 0x0c
	unsigned int m_size;       // 0x10
	int m_nextIndex;           // 0x14
	ChunkInfo* m_next;         // 0x18
	int m_childIndex;          // 0x1c
	ChunkInfo* m_child;        // 0x20
	MogDir* m_directory;       // 0x24
	char m_name[16];           // 0x28
};

#endif
