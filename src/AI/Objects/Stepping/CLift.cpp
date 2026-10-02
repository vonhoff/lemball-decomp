#include "AI/Objects/CLift.h"

#include "AI/Base/AICOORD.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "AI/Base/tCoord3d.h"
#include "Control/Game/CGame.h"
#include "Control/Game/GameTime.h"
#include "Map/Base/CMap.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Network/CConnect.h"

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
	x = position->m_xFixed >> 12;
	y = position->m_yFixed >> 12;
	if (x >= startX && x <= endX && y >= startY && y <= endY) {
		int z = position->m_zFixed >> 12;
		CMap* map = g_pActiveMap;
		int blockX = startX >> 4;
		int blockY = startY >> 4;
		unsigned short groundZ;
		if (startX < 0 || startY < 0 || blockX >= map->m_ground.m_width || blockY >= map->m_ground.m_height) {
			groundZ = 0;
		}
		else {
			startX &= 0xf;
			startY &= 0xf;
			groundZ = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(startX, startY);
		}
		int minZ = groundZ - 2;
		int maxZ = groundZ + 4;
		if (minZ <= z && z <= maxZ) {
			int i = 0;
			CGameObject** object = m_objects;
			do {
				if (*object == 0) {
					m_objects[i] = p_object;
					p_object->m_liftId = m_liftId;
					if (m_activateType == LIFT_ACTIVATE_STEP) {
						Activate();
						return 1;
					}
					if (m_activateType == LIFT_ACTIVATE_STEP_ONCE && m_activationLatched != 1) {
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
		p_object->m_liftId = 0xffff;
	}
	return 0;
}
