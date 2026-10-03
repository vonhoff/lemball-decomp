#include "../CGameObject.h"

// FUNCTION: LEMBALL 0x004166d0
short CollectUnusedObjectIds(unsigned short* p_ids, int p_capacity)
{
	int count = 0;
	unsigned short* output;
	int i = 0;
	do {
		if (g_abObjectIdBitmap[i] != 0xff) {
			int j = 0;
			output = p_ids + count;
			do {
				if ((g_abObjectIdBitmap[i] & g_abBitMasks[j]) == 0) {
					*output++ = j + i * 8;
					count++;
					if (count == p_capacity) {
						return p_capacity;
					}
				}
				j++;
			} while (j < 8);
		}
		i++;
	} while (i < 0x100);
	return count;
}
