#include "VsSort.h"

// 68K 0x1021746c swap
// FUNCTION: LEMBALL 0x00463b20
void Swap(unsigned char* p_first, unsigned char* p_second, unsigned int p_width)
{
	if (p_first != p_second) {
		while (p_width-- != 0) {
			unsigned char value = *p_first;
			*p_first = *p_second;
			p_first++;
			*p_second = value;
			p_second++;
		}
	}
}
