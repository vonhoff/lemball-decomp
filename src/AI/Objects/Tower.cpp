#include "Tower.h"

#include "../../Map/Base/Map.h"

// 68K 0x1011ab2c DoActivate__6CTowerFv
// FUNCTION: LEMBALL 0x0041c5a0
void Tower::DoActivate()
{
}

// 68K 0x1011ab4c Usage__6CTowerFv
// FUNCTION: LEMBALL 0x0041c5b0
int Tower::Usage()
{
	return 1;
}

// 68K 0x1011ab6a __dt__6CTowerFv
Tower::~Tower()
{
}

// 68K 0x10619554 Process__6CTowerFv
// FUNCTION: LEMBALL 0x0041cf70
bool Tower::Process()
{
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	int blockX = x >> 4;
	int blockY = y >> 4;
	if (x >= 0 && y >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		m_position.m_zFixed =
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf) << 12;
	}
	else {
		m_position.m_zFixed = 0;
	}
	return 1;
}

// 68K 0x106195ba Activate__6CTowerFP11CGameObject
// FUNCTION: LEMBALL 0x0041d000
bool Tower::Activate(GameObject* p_object)
{
	return 1;
}

// 68K 0x106195e8 ActivatePosition__6CTowerFv
// FUNCTION: LEMBALL 0x0041d010
AiCoord Tower::ActivatePosition()
{
	int y = m_position.m_yFixed - 0x8000;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed - 0x30000;
	return AiCoord(x, y, z);
}
