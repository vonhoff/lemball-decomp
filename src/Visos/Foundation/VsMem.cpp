#include "VsMem.h"

#include "CArena.h"
#include "CBucket.h"
#include "CSmallMemory.h"
#include "VsDebug.h"

// FUNCTION: LEMBALL 0x0045a6b0
void* InternalNew(unsigned long p_size)
{
	unsigned char* result;
	if (g_nSmallMemoryEnabled != 0) {
		if (g_maxSmallMemorySize > p_size) {
			result = g_pSmallMemory->Allocate(p_size, g_pCurrentAllocDescription);
			if (result != 0) {
				g_pCurrentAllocDescription = "new";
				return result;
			}
		}
	}
	if (!g_pMasterArena->Allocate(&result, p_size, g_pCurrentAllocDescription)) {
		InternalVsRelAssert("EnoughMemory", "VSMEM.CPP", 1677);
	}
	return result;
}

// FUNCTION: LEMBALL 0x0045a730
void InternalDelete(void* p_ptr)
{
	if (g_nSmallMemoryEnabled != 0 && g_pSmallMemory->Free((unsigned char*) p_ptr)) {
		return;
	}
	if (g_pMasterArena->Free((unsigned char*) p_ptr)) {
		return;
	}
	InternalVsRelAssert("EnoughMemory", "VSMEM.CPP", 1738);
}

// FUNCTION: LEMBALL 0x0045a780
void* operator new(size_t p_arg0)
{
	return InternalNew(p_arg0);
}

// FUNCTION: LEMBALL 0x0045a790
void operator delete(void* p_arg0)
{
	InternalDelete(p_arg0);
}

// FUNCTION: LEMBALL 0x0045a800
bool CheckValidPointer(void* p_arg0)
{
	if (g_nSmallMemoryEnabled != 0 && g_pSmallMemory != 0) {
		int i = 0;
		register unsigned char* ptr = (unsigned char*) p_arg0;
		register CBucket** buckets = (CBucket**) g_pSmallMemory;
		do {
			if (*buckets != 0 && (*buckets)->CheckValidPointer(ptr)) {
				return 1;
			}
			buckets++;
			i++;
		} while (i < 7);
	}
	return g_pMasterArena->CheckValidPointer(p_arg0) != 0;
}
