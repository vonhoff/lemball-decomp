#include "MogloadArena.h"

#include "../Foundation/Arena.h"
#include "../Foundation/SmallMemory.h"

// GLOBAL: LEMBALL 0x004a1d54
Arena* g_pMogloadArena = 0;

// 68K 0x10201652 __nw__13CMogloadArenaFUl
// FUNCTION: LEMBALL 0x0045baf0
void* MogloadArena::operator new(unsigned int p_size)
{
	if (g_pMogloadArena == 0) {
		return ::operator new(p_size);
	}
	unsigned char* result;
	if (g_nSmallMemoryEnabled != 0) {
		if (p_size < g_maxSmallMemorySize) {
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

// 68K 0x102016fe __dl__13CMogloadArenaFPv
// FUNCTION: LEMBALL 0x0045bb70
void MogloadArena::operator delete(void* p_memory)
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
