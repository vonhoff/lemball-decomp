#include "../CGameObject.h"

#include "../../../Control/Game/CGame.h"
#include "../../../Control/Game/GameTime.h"
#include "../../../Map/Base/CMap.h"
#include "../../../Visos/Foundation/CVSMath.h"
#include "../../Navigation/CAI.h"
#include "../../Navigation/CAiDestinationEntry.h"
#include "../../Navigation/CAiDestinationList.h"
#include "../../Navigation/CMaze.h"
#include "../../Navigation/CMover.h"
#include "../CPt3.h"
#include "../Solution.h"

#include <string.h>

#pragma intrinsic(memcpy, memset)

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
