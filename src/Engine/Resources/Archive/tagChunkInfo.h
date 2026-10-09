#ifndef LEMBALL_VISOS_FOUNDATION_TAGCHUNKINFO_H
#define LEMBALL_VISOS_FOUNDATION_TAGCHUNKINFO_H

#include "tagCHUNK.h"

class CMogDir;

// SIZE 0x38
struct tagChunkInfo {
	unsigned char* m_data;     // 0x00
	unsigned int m_type;       // 0x04
	unsigned long m_id;        // 0x08
	unsigned int m_fileOffset; // 0x0c
	unsigned int m_size;       // 0x10
	tagCHUNK m_next;           // 0x14
	tagCHUNK m_child;          // 0x1c
	CMogDir* m_directory;      // 0x24
	char m_name[16];           // 0x28
};

#endif
