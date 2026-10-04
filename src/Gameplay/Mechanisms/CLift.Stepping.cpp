#include "CLift.h"

#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Geometry/tCoord3d.h"
#include "Map/CMap.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"

#include <stddef.h>

extern unsigned short g_wMovingLiftCount;

// FUNCTION: LEMBALL 0x004254a0
int CLift::StepOn(const AICOORD& p_position, CGameObject* p_object)
{
	if (m_liftId == p_object->m_liftId) {
		return 1;
	}
	int startX = m_start.m_x - 8;
	int endX = m_end.m_x + 7;
	int startY = m_start.m_y - 8;
	int endY = m_end.m_y + 7;
	const AICOORD* position = &p_position;
	int y;
	int x;
	x = position->m_xFixed >> FIXED_POINT_FRACTION_BITS;
	y = position->m_yFixed >> FIXED_POINT_FRACTION_BITS;
	if (x >= startX && x <= endX && y >= startY && y <= endY) {
		int z = position->m_zFixed >> FIXED_POINT_FRACTION_BITS;
		CMap* map = g_pActiveMap;
		int blockX = startX >> GROUND_BLOCK_PIXEL_SHIFT;
		int blockY = startY >> GROUND_BLOCK_PIXEL_SHIFT;
		unsigned short groundZ;
		if (startX < 0 || startY < 0 || blockX >= map->m_ground.m_width || blockY >= map->m_ground.m_height) {
			groundZ = 0;
		}
		else {
			startX &= GROUND_BLOCK_PIXEL_MASK;
			startY &= GROUND_BLOCK_PIXEL_MASK;
			groundZ = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(startX, startY);
		}
		int minZ = groundZ - 2;
		int maxZ = groundZ + 4;
		if (minZ <= z && z <= maxZ) {
			int i = 0;
			CGameObject** object = m_objects;
			do {
				if (*object == NULL) {
					m_objects[i] = p_object;
					p_object->m_liftId = m_liftId;
					if (m_activateType == LIFT_ACTIVATE_STEP) {
						Activate();
						return 1;
					}
					if (m_activateType == LIFT_ACTIVATE_STEP_ONCE && m_activationLatched != LIFT_ACTIVATION_LATCHED) {
						Activate();
					}
					return 1;
				}
				object++;
				i++;
			} while (i < 8);
		}
	}
	if (m_liftId == p_object->m_liftId) {
		p_object->m_liftId = INVALID_OBJECT_ID;
	}
	return 0;
}
