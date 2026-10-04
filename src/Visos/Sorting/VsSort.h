#ifndef LEMBALL_VISOS_FOUNDATION_VSSORT_H
#define LEMBALL_VISOS_FOUNDATION_VSSORT_H

int AnimSpCmp(const void* p_left, const void* p_right);
int ViewDataCmp(const void* p_left, const void* p_right);
void VSQSort(void* p_base, unsigned int p_count, unsigned int p_width, int (*p_compare)(const void*, const void*));
void shortsort(unsigned char* p_low,
			   unsigned char* p_high,
			   unsigned int p_width,
			   int (*p_compare)(const void*, const void*));
void swap(unsigned char* p_first, unsigned char* p_second, unsigned int p_width);
#endif
