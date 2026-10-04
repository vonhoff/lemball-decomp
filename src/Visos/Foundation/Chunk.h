#ifndef LEMBALL_VISOS_FOUNDATION_CHUNK_H
#define LEMBALL_VISOS_FOUNDATION_CHUNK_H

struct ChunkInfo;

enum {
	CHUNK_INDEX_BEFORE_FIRST_ENTRY = -1
};

// SIZE 0x08
struct Chunk {
	int m_index;       // 0x00
	ChunkInfo* m_info; // 0x04
};

#endif
