#include "SmallMemory.h"

// 68K 0x10217b0c __ct__12CSmallMemoryFv
// STUB: LEMBALL 0x00473180
SmallMemory::SmallMemory()
{
}

// 68K 0x10217bf4 __dt__12CSmallMemoryFv
// STUB: LEMBALL 0x00473280
SmallMemory::~SmallMemory()
{
}

// 68K 0x10217c66 Allocate__12CSmallMemoryFiPc
// STUB: LEMBALL 0x004732d0
unsigned char* SmallMemory::Allocate(int p_size, char* p_description)
{
	return 0;
}

// 68K 0x10217ce4 Free__12CSmallMemoryFPUc
// STUB: LEMBALL 0x00473340
bool SmallMemory::Free(unsigned char* p_memory)
{
	return 0;
}

// GLOBAL: LEMBALL 0x004aa0fc
SmallMemory* g_pSmallMemory;

// GLOBAL: LEMBALL 0x004a217c
int g_nSmallMemoryEnabled = 1;

// GLOBAL: LEMBALL 0x004a2180
void* g_pMasterArenaMemory = 0;
