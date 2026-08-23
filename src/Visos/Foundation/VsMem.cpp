#include "VsMem.h"

#include "Arena.h"
#include "Bucket.h"
#include "SmallMemory.h"

// 68K 0x10215b2e InternalNew__FUl
// ASSERT: _VSRELassert("EnoughMemory", "VSMEM.CPP", 1677)
// FUNCTION: LEMBALL 0x0045a6b0
void* InternalNew(unsigned long p_arg0)
{
	// STRING: LEMBALL 0x004a1388 "VSMEM.CPP"
	// STRING: LEMBALL 0x004a1394 "EnoughMemory"
	return 0;
}

// 68K 0x10215be0 InternalDelete__FPv
// ASSERT: _VSRELassert("EnoughMemory", "VSMEM.CPP", 1738)
// FUNCTION: LEMBALL 0x0045a730
void InternalDelete(void* p_arg0)
{
	// STRING: LEMBALL 0x004a13a4 "VSMEM.CPP"
	// STRING: LEMBALL 0x004a13b0 "EnoughMemory"
}

// 68K 0x10215c5e __nw__FUl
// FUNCTION: LEMBALL 0x0045a780
void* operator new(size_t p_arg0)
{
	return InternalNew(p_arg0);
}

// 68K 0x10215c7a __dl__FPv
// FUNCTION: LEMBALL 0x0045a790
void operator delete(void* p_arg0)
{
	InternalDelete(p_arg0);
}

// 68K 0x10215c96 CheckValidPointer__FPv
// FUNCTION: LEMBALL 0x0045a800
bool CheckValidPointer(void* p_arg0)
{
	if (g_nSmallMemoryEnabled != 0 && g_pSmallMemory != 0) {
		Bucket** bucket = g_pSmallMemory->m_buckets;
		for (int i = 0; i < 7; i++) {
			if (*bucket != 0 && (*bucket)->CheckValidPointer((unsigned char*) p_arg0)) {
				return true;
			}
			bucket++;
		}
	}
	return g_pMasterArena->CheckValidPointer(p_arg0);
}
