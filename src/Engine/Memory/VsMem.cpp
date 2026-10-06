#include "VsMem.h"

#include "CArena.h"
#include "CBucket.h"
#include "CSmallMemory.h"
#include "Engine/Diagnostics/VsDebug.h"

// FUNCTION: LEMBALL 0x0045a6b0
void* InternalNew(unsigned long p_size)
{
	unsigned char* result;
	if (g_nSmallMemoryEnabled != 0) {
		if (g_maxSmallMemorySize > p_size) {
			result = g_pSmallMemory->Allocate(p_size, g_pCurrentAllocDescription);
			if (result != NULL) {
				g_pCurrentAllocDescription = "new";
				return result;
			}
		}
	}
	if (!g_pMasterArena->Allocate(&result, p_size, g_pCurrentAllocDescription)) {
		_VSRELassert("EnoughMemory", "VSMEM.CPP", 1677);
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
	_VSRELassert("EnoughMemory", "VSMEM.CPP", 1738);
}

// FUNCTION: LEMBALL 0x0045a780
void* operator new(size_t p_allocationSize)
{
	return InternalNew(p_allocationSize);
}

// FUNCTION: LEMBALL 0x0045a790
void operator delete(void* p_memory)
{
	InternalDelete(p_memory);
}

// FUNCTION: LEMBALL 0x0045a800
bool CheckValidPointer(void* p_pointer)
{
	if (g_nSmallMemoryEnabled != 0 && g_pSmallMemory != NULL) {
		int i = 0;
		unsigned char* ptr = (unsigned char*) p_pointer;
		CBucket** buckets = g_pSmallMemory->m_buckets;
		do {
			if (*buckets != NULL && (*buckets)->CheckValidPointer(ptr)) {
				return true;
			}
			buckets++;
			i++;
		} while (i < 7);
	}
	return g_pMasterArena->CheckValidPointer(p_pointer) != 0;
}
