#include "CGameObject.h"

// FUNCTION: LEMBALL 0x004166d0
short CollectUnusedObjectIds(unsigned short* p_ids, int p_capacity)
{
	int count = 0;
	unsigned short* output;
	int i = 0;
	do {
		if (g_abObjectIdBitmap[i] != OBJECT_ID_BITMAP_BYTE_FULL_MASK) {
			int j = 0;
			output = p_ids + count;
			do {
				if ((g_abObjectIdBitmap[i] & g_abBitMasks[j]) == 0) {
					*output++ = j + i * OBJECT_ID_BITMAP_BITS_PER_BYTE;
					count++;
					if (count == p_capacity) {
						return p_capacity;
					}
				}
				j++;
			} while (j < OBJECT_ID_BITMAP_BITS_PER_BYTE);
		}
		i++;
	} while (i < OBJECT_ID_BITMAP_BYTE_CAPACITY);
	return count;
}
