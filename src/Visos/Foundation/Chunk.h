#ifndef LEMBALL_SCAFFOLD_VISOS_FOUNDATION_CHUNK_H
#define LEMBALL_SCAFFOLD_VISOS_FOUNDATION_CHUNK_H

#include "../../Common.h"

// SIZE 0x08
struct Chunk {
public:
	// No functions mapped yet.

	int m_index;       // 0x00
	ChunkInfo* m_info; // 0x04
};

#endif
