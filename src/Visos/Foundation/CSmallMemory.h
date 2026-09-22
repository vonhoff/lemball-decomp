#ifndef LEMBALL_VISOS_FOUNDATION_CSMALLMEMORY_H
#define LEMBALL_VISOS_FOUNDATION_CSMALLMEMORY_H

class CBucket;

// SIZE 0x3c
class CSmallMemory {
public:
	CSmallMemory();
	bool Free(unsigned char* p_memory);
	unsigned char* Allocate(int p_size, char* p_description);
	~CSmallMemory();

	friend bool CheckValidPointer(void* p_arg0);

private:
	CBucket* m_buckets[7]; // 0x00
	int m_sizeLimits[7];   // 0x1c
	int m_bucketLimit;     // 0x38
};

extern CSmallMemory* g_pSmallMemory;
extern int g_nSmallMemoryEnabled;
extern void* g_pMasterArenaMemory;
extern unsigned int g_maxSmallMemorySize;
extern char* g_pCurrentAllocDescription;
#endif
