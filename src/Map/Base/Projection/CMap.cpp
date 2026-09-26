#include "../CMap.h"

// FUNCTION: LEMBALL 0x00430ce0
void CMap::GameToScreen(int& p_x, int& p_y)
{
	int* outputY = &p_y;
	int y = *outputY;
	int x = p_x;
	switch (m_orientation) {
	case 0:
		p_x = x - y + 0x10;
		*outputY = y / 2 + x / 2;
		break;
	case 1:
		p_x = 0x20 - y - x;
		*outputY = x / 2 - y / 2 + 8;
		break;
	case 2:
		p_x = y - x + 0x10;
		*outputY = 0x10 - y / 2 - x / 2;
		break;
	case 3:
		p_x = x + y;
		*outputY = y / 2 - x / 2 + 8;
	}
}
