#include "GroundArray.h"

// FUNCTION: LEMBALL 0x0040eb70
Ground* GroundArray::GetGroundCell(int p_x, int p_y)
{
	return &m_ground[m_width * p_y + p_x];
}
