#include "VsMem.h"

#include "CArena.h"
#include "CBucket.h"
#include "CSmallMemory.h"
#include "VsDebug.h"

// FUNCTION: LEMBALL 0x0045a6b0
void* InternalNew(unsigned long allocationSize)
{
	unsigned char* result;
	if (g_nSmallMemoryEnabled != 0) {
		if (g_maxSmallMemorySize > allocationSize) {
			result = g_pSmallMemory->Allocate(allocationSize, g_pCurrentAllocDescription);
			if (result != 0) {
				g_pCurrentAllocDescription = "new";
				return result;
			}
		}
	}
	if (!g_pMasterArena->Allocate(&result, allocationSize, g_pCurrentAllocDescription)) {
		_VSRELassert("EnoughMemory", "VSMEM.CPP", 1677);
	}
	return result;
}

// FUNCTION: LEMBALL 0x0045a730
void InternalDelete(void* memory)
{
	if (g_nSmallMemoryEnabled != 0 && g_pSmallMemory->Free((unsigned char*) memory)) {
		return;
	}
	if (g_pMasterArena->Free((unsigned char*) memory)) {
		return;
	}
	_VSRELassert("EnoughMemory", "VSMEM.CPP", 1738);
}

// FUNCTION: LEMBALL 0x0045a780
void* operator new(size_t allocationSize)
{
	return InternalNew(allocationSize);
}

// FUNCTION: LEMBALL 0x0045a790
void operator delete(void* memory)
{
	InternalDelete(memory);
}

// FUNCTION: LEMBALL 0x0045a800
bool CheckValidPointer(void* pointer)
{
	if (g_nSmallMemoryEnabled != 0 && g_pSmallMemory != 0) {
		int i = 0;
		register unsigned char* ptr = (unsigned char*) pointer;
		register CBucket** buckets = (CBucket**) g_pSmallMemory;
		do {
			if (*buckets != 0 && (*buckets)->CheckValidPointer(ptr)) {
				return 1;
			}
			buckets++;
			i++;
		} while (i < 7);
	}
	return g_pMasterArena->CheckValidPointer(pointer) != 0;
}
