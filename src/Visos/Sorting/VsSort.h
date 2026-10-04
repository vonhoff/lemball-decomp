#ifndef LEMBALL_VISOS_FOUNDATION_VSSORT_H
#define LEMBALL_VISOS_FOUNDATION_VSSORT_H

void VSQSort(void* p_base, unsigned int p_count, unsigned int p_width, int (*p_compare)(const void*, const void*));
void shortsort(unsigned char* p_low,
			   unsigned char* p_high,
			   unsigned int p_width,
			   int (*p_compare)(const void*, const void*));
void swap(unsigned char* p_first, unsigned char* p_second, unsigned int p_width);
#endif
