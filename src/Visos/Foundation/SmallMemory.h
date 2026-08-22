#ifndef LEMBALL_VISOS_FOUNDATION_SMALLMEMORY_H
#define LEMBALL_VISOS_FOUNDATION_SMALLMEMORY_H

#include "../../Common.h"

// SIZE 0x3c
class SmallMemory {
public:
	SmallMemory();
	bool Free(unsigned char* p_memory);
	unsigned char* Allocate(int p_size, char* p_description);
	~SmallMemory();

private:
	Bucket* m_buckets[7];         // 0x00
	unsigned int m_sizeLimits[7]; // 0x1c
	unsigned int m_bucketLimit;   // 0x38
};

// Confirmed class-scoped globals.
extern SmallMemory* g_pSmallMemory;
extern int g_nSmallMemoryEnabled;
extern void* g_pMasterArenaMemory;
#endif
