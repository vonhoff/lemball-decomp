#include "CMogloadArena.h"

#include "../Foundation/CArena.h"
#include "../Foundation/CSmallMemory.h"

// GLOBAL: LEMBALL 0x004a1d54
CArena* g_pMogloadArena = 0;

// FUNCTION: LEMBALL 0x0045baf0
void* CMogloadArena::operator new(unsigned int p_size)
{
	if (g_pMogloadArena == 0) {
		return ::operator new(p_size);
	}
	unsigned char* result;
	if (g_nSmallMemoryEnabled != 0) {
		if (g_maxSmallMemorySize > p_size) {
			result = g_pSmallMemory->Allocate(p_size, g_pCurrentAllocDescription);
			if (result != 0) {
				return result;
			}
		}
	}
	if (!g_pMogloadArena->Allocate(&result, p_size, g_pCurrentAllocDescription)) {
		return 0;
	}
	return result;
}

// FUNCTION: LEMBALL 0x0045bb70
void CMogloadArena::operator delete(void* p_memory)
{
	if (g_pMogloadArena == 0) {
		::operator delete(p_memory);
		return;
	}
	if (g_nSmallMemoryEnabled != 0 && g_pSmallMemory->Free((unsigned char*) p_memory)) {
		return;
	}
	g_pMogloadArena->Free((unsigned char*) p_memory);
}
