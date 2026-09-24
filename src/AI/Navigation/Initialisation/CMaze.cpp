#include "../CMaze.h"

#include "../../../Map/Base/CMap.h"

extern "C" unsigned long __stdcall timeGetTime(void);

// FUNCTION: LEMBALL 0x00423530
void CMaze::BInitialise(unsigned int p_resetStats, int p_startX, int p_startY, int p_endX, int p_endY)
{
	int width;
	CMap* map;
	if (p_resetStats != 0) {
		m_totalTime = 0;
		m_solutionCount = 0;
	}
	m_startTime = timeGetTime();
	m_radius = 0;
	m_startX = p_startX;
	m_startY = p_startY;
	m_endX = p_endX;
	m_endY = p_endY;

	int y = 0;
	if (m_height > 0) {
		do {
			int x = 0;
			if (m_width > 0) {
				do {
					unsigned short collision;
					if (x < 0 || y < 0) {
						collision = 3;
					}
					else {
						map = m_map;
						width = map->m_ground.m_width;
						if (width <= x || map->m_ground.m_height <= y) {
							collision = 3;
						}
						else {
							collision = map->m_ground.m_ground[y * width + x].m_collision;
						}
					}
					if ((collision & 1) != 0) {
						m_distances[y][x] = 0xffff;
					}
					else {
						m_distances[y][x] = 0xff00;
					}
					x++;
				} while (x < m_width);
			}
			y++;
		} while (y < m_height);
	}

	m_changeSelect = 0;
	Clear(m_changeA);
	Clear(m_changeB);
	UpdateChangeNext(m_startX, m_startY);
	m_distances[m_startY][m_startX] = 0;
}
