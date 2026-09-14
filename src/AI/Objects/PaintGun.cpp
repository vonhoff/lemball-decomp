#include "PaintGun.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../../Visos/Network/Connect.h"
#include "../Navigation/Ai.h"

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

// 68K 0x1061b2f6 __dt__9CPaintGunFv
// FUNCTION: LEMBALL 0x0042bb20
PaintGun::~PaintGun()
{
}

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
// FUNCTION: LEMBALL 0x0042bbe0
bool PaintGun::Process()
{
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == 3) {
				SetSndEffect((eSoundEffect) 7);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	if (g_pActiveConnection != 0 && g_pActiveConnection->m_isHost != 0) {
		return 1;
	}
	if (m_enabled == 0) {
		return 1;
	}
	switch (m_action) {
	case 3:
		Action((eAction) 0x1b);
		break;
	case 0x18: {
		int direction = m_direction;
		m_lastMovementTick = g_dwGameTick + ((28 - direction) * 1000) / 400;
		m_stateTimer = g_dwSimulationTimestamp + (-direction * 1000) / 8;
		m_actionDeadline = g_dwGameTick + ((58 - direction) * 1000) / 400;
		Action((eAction) 0x1b);
		break;
	}
	case 0x1b:
		if (m_actionDeadline < g_dwGameTick) {
			m_stateTimer = g_dwSimulationTimestamp;
			m_lastMovementTick = g_dwGameTick + 70;
			m_actionDeadline = g_dwGameTick + 145;
			Action((eAction) 0x1b);
		}
		else if (m_lastMovementTick < g_dwGameTick) {
			int x = m_position.m_xFixed;
			int y = m_position.m_yFixed;
			int z = m_position.m_zFixed + 0x8000;
			AiCoord start;
			AiCoord target;
			int coordinate;

			start.m_xFixed = x + 0x10000;
			start.m_yFixed = y;
			start.m_zFixed = z;
			target.m_yFixed = y;
			target.m_zFixed = z;
			coordinate = (x >> 12) + 200;
			// The original admits 1024, but clamps larger values to 1023.
			if (coordinate > 1024) {
				coordinate = 1023;
			}
			target.m_xFixed = coordinate << 12;
			g_pAI->FireBullet(m_linkedObjectId, (eBulletType) 0, (eOwner) 1, 2, start, target);

			start.m_xFixed = x - 0x10000;
			start.m_yFixed = y;
			start.m_zFixed = z;
			target.m_yFixed = y;
			target.m_zFixed = z;
			coordinate = (x >> 12) - 200;
			if (coordinate < 0) {
				coordinate = 0;
			}
			target.m_xFixed = coordinate << 12;
			g_pAI->FireBullet(m_linkedObjectId, (eBulletType) 0, (eOwner) 1, 6, start, target);

			start.m_xFixed = x;
			start.m_yFixed = y + 0x10000;
			start.m_zFixed = z;
			target.m_xFixed = x;
			target.m_zFixed = z;
			coordinate = (y >> 12) + 200;
			if (coordinate > 1024) {
				coordinate = 1023;
			}
			target.m_yFixed = coordinate << 12;
			g_pAI->FireBullet(m_linkedObjectId, (eBulletType) 0, (eOwner) 1, 8, start, target);

			start.m_xFixed = x;
			start.m_yFixed = y - 0x10000;
			start.m_zFixed = z;
			target.m_xFixed = x;
			target.m_zFixed = z;
			coordinate = (y >> 12) - 200;
			if (coordinate < 0) {
				coordinate = 0;
			}
			target.m_yFixed = coordinate << 12;
			g_pAI->FireBullet(m_linkedObjectId, (eBulletType) 0, (eOwner) 1, 4, start, target);
			SetSndEffect((eSoundEffect) 7);
			m_lastMovementTick = m_actionDeadline;
			Action((eAction) 3);
		}
		break;
	}
	return 1;
}

// 68K 0x1011ae1e DoActivate__9CPaintGunFv
// FUNCTION: LEMBALL 0x0042c9a0
void PaintGun::DoActivate()
{
}
