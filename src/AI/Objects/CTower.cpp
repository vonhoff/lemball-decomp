#include "CTower.h"

#include "../../Map/Base/CMap.h"
#include "AI/Base/AiCoord.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"

// FUNCTION: LEMBALL 0x0041c5a0
void CTower::DoActivate()
{
}

// FUNCTION: LEMBALL 0x0041c5b0
int CTower::Usage()
{
	return 1;
}

// FUNCTION: LEMBALL 0x0041cf70
bool CTower::Process()
{
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	int blockX;
	int blockY;
	CMap* map = g_pMap;
	blockX = x >> 4;
	blockY = y >> 4;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & 0xf;
		int cellY = y & 0xf;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = z << 12;
	return 1;
}

// FUNCTION: LEMBALL 0x0041d000
bool CTower::Activate(CGameObject* p_object)
{
	return 1;
}

// FUNCTION: LEMBALL 0x0041d010
AiCoord CTower::ActivatePosition()
{
	int y = m_position.m_yFixed - 0x8000;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed - 0x30000;
	return AiCoord(x, y, z);
}
