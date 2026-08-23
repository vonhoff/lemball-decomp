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

	friend bool CheckValidPointer(void* p_arg0);

private:
	Bucket* m_buckets[7];         // 0x00
	unsigned int m_sizeLimits[7]; // 0x1c
	unsigned int m_bucketLimit;   // 0x38
};

extern SmallMemory* g_pSmallMemory;
extern int g_nSmallMemoryEnabled;
extern void* g_pMasterArenaMemory;
extern unsigned int g_maxSmallMemorySize;
extern char* g_pCurrentAllocDescription;
#endif
