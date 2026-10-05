#include "CTower.h"

#include "Map/CMap.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/ObjectInteractionStates.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"

enum {
	TOWER_ACTIVATION_POSITION_X_OFFSET_FIXED = -48 * FIXED_POINT_ONE,
	TOWER_ACTIVATION_POSITION_Y_OFFSET_FIXED = -8 * FIXED_POINT_ONE
};

// FUNCTION: LEMBALL 0x0041c5a0
void CTower::DoActivate()
{
}

// FUNCTION: LEMBALL 0x0041c5b0
int CTower::Usage()
{
	return GROUP_OBJECT_USAGE_GROUP;
}

// FUNCTION: LEMBALL 0x0041cf70
bool CTower::Process()
{
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int blockX;
	int blockY;
	CMap* map = g_pMap;
	blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & GROUND_BLOCK_PIXEL_MASK;
		int cellY = y & GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = z << FIXED_POINT_FRACTION_BITS;
	return true;
}

// FUNCTION: LEMBALL 0x0041d000
bool CTower::Activate(CGameObject* p_object)
{
	return true;
}

// FUNCTION: LEMBALL 0x0041d010
AICOORD CTower::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	y += TOWER_ACTIVATION_POSITION_Y_OFFSET_FIXED;
	int x = m_position.m_xFixed + TOWER_ACTIVATION_POSITION_X_OFFSET_FIXED;
	return AICOORD(x, y, z);
}
