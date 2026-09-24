#include "../CDoor.h"

#include "../../../Map/Base/CMap.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

// FUNCTION: LEMBALL 0x0040d910
void CDoor::SetCollision()
{
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;

	switch (m_objectType) {
	case OBJECT_DOOR_1:
		if (blockX >= 0) {
			if (blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |= 1;
			}
			if (blockX >= 0 && --blockY >= 0) {
				CMap* map = g_pMap;
				int width = map->m_ground.m_width;
				if (blockX < width && map->m_ground.m_height > blockY) {
					g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= 1;
				}
			}
		}
		break;
	case OBJECT_DOOR_2:
		if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |= 1;
		}
		if (blockX - 1 >= 0 && blockY >= 0 && g_pMap->m_ground.m_width > blockX - 1 &&
			g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX - 1].m_collision |= 1;
		}
		break;
	}
}
