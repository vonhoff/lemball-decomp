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
// FUNCTION: LEMBALL 0x00463960
void VsQSort(void* p_base, unsigned int p_count, unsigned int p_width, int (*p_compare)(const void*, const void*))
{
	unsigned char* low;
	unsigned char* high;
	unsigned char* first;
	unsigned char* second;
	unsigned int size;
	unsigned char* lowStack[30];
	unsigned char* highStack[30];
	int stackIndex;

	if (p_count < 2 || p_width == 0) {
		return;
	}
	stackIndex = 0;
	low = (unsigned char*) p_base;
	high = low + (p_count - 1) * p_width;
nextPartition:
	size = (unsigned int) (high - low) / p_width + 1;
	if (size <= 8) {
		ShortSort(low, high, p_width, p_compare);
	}
	else {
		Swap(low + (size / 2) * p_width, low, p_width);
		first = low;
		second = high + p_width;
		for (;;) {
			do {
				first += p_width;
			} while (first <= high && p_compare(first, low) <= 0);
			do {
				second -= p_width;
			} while (second > low && p_compare(second, low) >= 0);
			if (first > second) {
				break;
			}
			Swap(first, second, p_width);
		}
		Swap(low, second, p_width);
		if (second - low - 1 >= high - first) {
			if (low + p_width < second) {
				lowStack[stackIndex] = low;
				highStack[stackIndex] = second - p_width;
				++stackIndex;
			}
			if (first < high) {
				low = first;
				goto nextPartition;
			}
		}
		else {
			if (first < high) {
				lowStack[stackIndex] = first;
				highStack[stackIndex] = high;
				++stackIndex;
			}
			if (low + p_width < second) {
				high = second - p_width;
				goto nextPartition;
			}
		}
	}
	if (--stackIndex >= 0) {
		low = lowStack[stackIndex];
		high = highStack[stackIndex];
		goto nextPartition;
	}
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
