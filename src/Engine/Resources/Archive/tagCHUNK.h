#ifndef LEMBALL_VISOS_FOUNDATION_TAGCHUNK_H
#define LEMBALL_VISOS_FOUNDATION_TAGCHUNK_H

struct tagChunkInfo;

enum {
	CHUNK_INDEX_BEFORE_FIRST_ENTRY = -1
};

// SIZE 0x08
struct tagCHUNK {
	int m_index;          // 0x00
	tagChunkInfo* m_info; // 0x04
};

#endif
