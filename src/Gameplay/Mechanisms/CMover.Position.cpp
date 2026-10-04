#include "CMover.h"

#include "Map/CMap.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"

// FUNCTION: LEMBALL 0x0042e650
void CMover::SetPos()
{
	int x = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) - 8;
	int maxX = x + 15;
	int y = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) - 8;
	int maxY = y + 15;
	x /= 16;
	y /= 16;
	maxX /= 16;
	maxY /= 16;
	for (int groundY = y; groundY <= maxY; groundY++) {
		for (int groundX = x; groundX <= maxX; groundX++) {
			if (groundX >= 0 && groundY >= 0 && groundX < g_pMap->m_ground.m_width &&
				groundY < g_pMap->m_ground.m_height) {
				CGround* ground = &g_pMap->m_ground.m_ground[groundY * g_pMap->m_ground.m_width + groundX];
				ground->m_collision |= GROUND_COLLISION_MOVER_PRESENT;
			}
		}
	}
}
