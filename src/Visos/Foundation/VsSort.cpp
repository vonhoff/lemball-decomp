#include "VsSort.h"

#include "../../AI/Objects/AnimSpecialEntry.h"
#include "../../AI/Objects/ViewData.h"

// 68K 0x10602c1c AnimSpCmp__FPCvPCv
// FUNCTION: LEMBALL 0x00409910
int AnimSpCmp(const void* p_left, const void* p_right)
{
	const AnimSpecialEntry* left;
	const AnimSpecialEntry* right;

	left = (const AnimSpecialEntry*) p_left;
	right = (const AnimSpecialEntry*) p_right;
	return left->m_sortKey - right->m_sortKey;
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
// FUNCTION: LEMBALL 0x00463ac0
void ShortSort(unsigned char* p_low,
			   unsigned char* p_high,
			   unsigned int p_width,
			   int (*p_compare)(const void*, const void*))
{
	while (p_high > p_low) {
		unsigned char* current;
		unsigned char* selected;

		selected = p_low;
		current = p_low + p_width;
		if (p_high >= current) {
			do {
				if (p_compare(current, selected) > 0) {
					selected = current;
				}
				current += p_width;
			} while (p_high >= current);
		}
		Swap(selected, p_high, p_width);
		p_high -= p_width;
	}
}
