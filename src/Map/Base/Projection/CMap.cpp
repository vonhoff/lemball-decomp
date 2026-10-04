#include "../CMap.h"

// FUNCTION: LEMBALL 0x00430ce0
void CMap::GameToScreen(int& p_x, int& p_y)
{
	int* outputY = &p_y;
	int y = *outputY;
	int x = p_x;
	switch (m_orientation) {
	case MAP_ORIENTATION_ROTATION_0_DEGREES:
		p_x = x - y + MAP_PROJECTION_BLOCK_PIXEL_SIZE;
		*outputY = y / 2 + x / 2;
		break;
	case MAP_ORIENTATION_ROTATION_90_DEGREES:
		p_x = MAP_PROJECTION_DOUBLE_BLOCK_PIXEL_SIZE - y - x;
		*outputY = x / 2 - y / 2 + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
		break;
	case MAP_ORIENTATION_ROTATION_180_DEGREES:
		p_x = y - x + MAP_PROJECTION_BLOCK_PIXEL_SIZE;
		*outputY = MAP_PROJECTION_BLOCK_PIXEL_SIZE - y / 2 - x / 2;
		break;
	case MAP_ORIENTATION_ROTATION_270_DEGREES:
		p_x = x + y;
		*outputY = y / 2 - x / 2 + MAP_PROJECTION_HALF_BLOCK_PIXEL_SIZE;
	}
}
