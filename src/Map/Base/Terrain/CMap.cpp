#include "../CMap.h"

// FUNCTION: LEMBALL 0x00431030
void CMap::CalculateCliff()
{
	int x;
	int y;
	CGround* ground;
	y = 0;
	if (m_walkHeight > 0) {
		do {
			x = 0;
			for (;;) {
				int width = m_walkWidth;
				if (x >= width) {
					break;
				}
				ground = m_ground.m_ground + m_ground.m_width * y + x;
				int height = ground->m_height;
				if (x < width - 1 && y < m_walkHeight - 1) {
					CGround* below = m_ground.m_ground + (y + 1) * m_ground.m_width + x;
					CGround* right = ground + 1;
					int lowerHeight;
					if (right->m_height > below->m_height) {
						lowerHeight = below->m_height;
					}
					else {
						lowerHeight = right->m_height;
					}
					height -= lowerHeight;
				}
				ground->m_cliff = (unsigned short) ((height + 15) / 16);
				x++;
			}
			y++;
		} while (y < m_walkHeight);
	}
}
