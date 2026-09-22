#include "PackParam.h"

// FUNCTION: LEMBALL 0x00463000
int PackParam(short p_low, short p_high)
{
	return ((unsigned int) (unsigned short) p_high) * 0x10000 + (unsigned int) (unsigned short) p_low;
}
