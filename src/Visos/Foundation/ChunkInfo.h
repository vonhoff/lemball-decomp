#ifndef LEMBALL_VISOS_FOUNDATION_CHUNKINFO_H
#define LEMBALL_VISOS_FOUNDATION_CHUNKINFO_H

#include "Chunk.h"

class CMogDir;

// SIZE 0x38
struct ChunkInfo {
	unsigned char* m_data;     // 0x00
	unsigned int m_type;       // 0x04
	unsigned int m_id;         // 0x08
	unsigned int m_fileOffset; // 0x0c
	unsigned int m_size;       // 0x10
	Chunk m_next;              // 0x14
	Chunk m_child;             // 0x1c
	CMogDir* m_directory;      // 0x24
	char m_name[16];           // 0x28
};

#endif
