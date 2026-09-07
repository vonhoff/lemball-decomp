#include "Mover.h"

// 68K 0x106173a0 IsAt__6CMoverFiiRi
// FUNCTION: LEMBALL 0x0042e700
bool Mover::IsAt(int p_x, int p_y, int& p_height)
{
	int x = (m_position.m_xFixed >> 12) - 8;
	int xMax = x + 15;
	int y = (m_position.m_yFixed >> 12) - 8;
	int yMax = y + 15;
	if (p_x >= x && p_x <= xMax && p_y >= y && p_y <= yMax) {
		p_height = (m_position.m_zFixed >> 12) + 8;
		return true;
	}
	return false;
}
