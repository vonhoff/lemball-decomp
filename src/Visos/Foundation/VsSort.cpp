#include "VsSort.h"

#include "../../AI/Objects/ViewData.h"

// 68K 0x10602c1c AnimSpCmp__FPCvPCv
// FUNCTION: LEMBALL 0x00409910
int AnimSpCmp(const void* p_left, const void* p_right)
{
	return 0;
}

// 68K 0x10b05a34 ViewDataCmp__FPCvPCv
// FUNCTION: LEMBALL 0x0043ff60
int ViewDataCmp(const void* p_left, const void* p_right)
{
	const ViewData* left;
	const ViewData* right;

	left = (const ViewData*) p_left;
	right = (const ViewData*) p_right;
	return (int) left->m_sortZKey - (int) right->m_sortZKey;
}

// 68K 0x102172a4 VSQSort
// STUB: LEMBALL 0x00463960
void VsQSort(void* p_base, unsigned int p_count, unsigned int p_width, int (*p_compare)(const void*, const void*))
{
}

// 68K 0x1021740a shortsort
// STUB: LEMBALL 0x00463ac0
void ShortSort(unsigned char* p_low,
			   unsigned char* p_high,
			   unsigned int p_width,
			   int (*p_compare)(const void*, const void*))
{
}

// 68K 0x1021746c swap
// STUB: LEMBALL 0x00463b20
void Swap(unsigned char* p_first, unsigned char* p_second, unsigned int p_width)
{
}
