#include "PaintGun.h"

// 68K 0x1061b270 __ct__9CPaintGunFv
// FUNCTION: LEMBALL 0x0042bad0
PaintGun::PaintGun() : GlobalGameObject(0x21, 0, 0)
{
}

// 68K 0x1061b2b4 Restart__9CPaintGunFv
// FUNCTION: LEMBALL 0x0042baf0
void PaintGun::Restart()
{
	GlobalGameObject::Restart();
	m_stateTimer = 0;
	m_enabled = 0;
	m_active = 0;
	m_direction = 0;
}

#include "../../Map/Base/Map.h"

// 68K 0x1061b352 Set__9CPaintGunFUsRC7AICOORDi
// FUNCTION: LEMBALL 0x0042bb30
void PaintGun::Set(unsigned short p_id, const AiCoord& p_position, int p_direction)
{
	SetId(p_id);
	int x = p_position.m_xFixed;
	m_position.m_xFixed = x;
	int y = p_position.m_yFixed;
	m_position.m_yFixed = y;
	m_position.m_zFixed = p_position.m_zFixed;
	m_direction = p_direction;
	m_active = 1;
	m_enabled = 1;
	m_action = (eAction) 0x18;
	int groundX = (x >> 12) / 16;
	int groundY = (y >> 12) / 16;
	if (groundX >= 0 && groundY >= 0) {
		Map* map = g_pMap;
		int width = map->m_ground.m_width;
		if (width > groundX && map->m_ground.m_height > groundY) {
			Ground* ground = &g_pMap->m_ground.m_ground[groundY * width + groundX];
			ground->m_collision |= 1;
		}
	}
}

// 68K 0x1061b40c Process__9CPaintGunFv
// STUB: LEMBALL 0x0042bbe0
bool PaintGun::Process()
{
	return 0;
}

// 68K 0x1011ae1e DoActivate__9CPaintGunFv
// FUNCTION: LEMBALL 0x0042c9a0
void PaintGun::DoActivate()
{
}

// 68K 0x1061b2f6 __dt__9CPaintGunFv
PaintGun::~PaintGun()
{
}
