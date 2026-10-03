#include "CPaintGun.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Network/CConnect.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "AI/Objects/CBullet.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

inline static void ClampUpperTargetCoordinate(int& p_coordinate)
{
	if (p_coordinate > 1024) {
		p_coordinate = 1023;
	}
}

inline static void ClampLowerTargetCoordinate(int& p_coordinate)
{
	if (p_coordinate < 0) {
		p_coordinate = 0;
	}
}

// FUNCTION: LEMBALL 0x0042bad0
CPaintGun::CPaintGun() : CGlobalGameObject(OBJECT_PAINT_GUN, 0, 0)
{
}

// FUNCTION: LEMBALL 0x0042baf0
void CPaintGun::Restart()
{
	CGlobalGameObject::Restart();
	m_stateTimer = 0;
	m_enabled = 0;
	m_active = 0;
	m_direction = 0;
}

// FUNCTION: LEMBALL 0x0042bb20
CPaintGun::~CPaintGun()
{
}

// FUNCTION: LEMBALL 0x0042bb30
void CPaintGun::Set(unsigned short p_id, const AICOORD& p_position, int p_direction)
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
	m_action = ACTION_READY;
	int groundX = (x >> 12) / 16;
	int groundY = (y >> 12) / 16;
	if (groundX >= 0 && groundY >= 0) {
		CMap* map = g_pMap;
		int width = map->m_ground.m_width;
		if (width > groundX && map->m_ground.m_height > groundY) {
			CGround* ground = &g_pMap->m_ground.m_ground[groundY * width + groundX];
			ground->m_collision |= 1;
		}
	}
}

// FUNCTION: LEMBALL 0x0042bbe0
bool CPaintGun::Process()
{
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_FIRING) {
				SetSndEffect(SFX_BIGGUN);
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
	case ACTION_FIRING:
		Action(ACTION_RUNNING);
		break;
	case ACTION_READY: {
		int direction = m_direction;
		m_lastMovementTick = g_dwGameTick + ((28 - direction) * 1000) / 400;
		m_stateTimer = g_dwSimulationTimestamp + (-direction * 1000) / 8;
		m_actionDeadline = g_dwGameTick + ((58 - direction) * 1000) / 400;
		Action(ACTION_RUNNING);
		break;
	}
	case ACTION_RUNNING:
		if (m_actionDeadline < g_dwGameTick) {
			m_stateTimer = g_dwSimulationTimestamp;
			m_lastMovementTick = g_dwGameTick + 70;
			m_actionDeadline = g_dwGameTick + 145;
			Action(ACTION_RUNNING);
		}
		else if (m_lastMovementTick < g_dwGameTick) {
			int y = m_position.m_yFixed;
			int z = m_position.m_zFixed;
			AICOORD position(m_position.m_xFixed, y, z + 0x8000);
			AICOORD start(position);
			AICOORD target(position);
			int coordinate;

			start.m_xFixed = position.m_xFixed + 0x10000;
			coordinate = (position.m_xFixed >> 12) + 200;
			ClampUpperTargetCoordinate(coordinate);
			target.m_xFixed = coordinate << 12;
			g_pAI->FireBullet(m_linkedObjectId, BULLET_TYPE_DEFAULT, OWNER_ENEMY, 2, start, target);

			start.m_yFixed = position.m_yFixed;
			start.m_zFixed = position.m_zFixed;
			target.m_yFixed = position.m_yFixed;
			target.m_zFixed = position.m_zFixed;
			start.m_xFixed = position.m_xFixed - 0x10000;
			coordinate = (position.m_xFixed >> 12) - 200;
			ClampLowerTargetCoordinate(coordinate);
			target.m_xFixed = coordinate << 12;
			g_pAI->FireBullet(m_linkedObjectId, BULLET_TYPE_DEFAULT, OWNER_ENEMY, 6, start, target);

			start.m_xFixed = position.m_xFixed;
			start.m_zFixed = position.m_zFixed;
			target.m_xFixed = position.m_xFixed;
			target.m_zFixed = position.m_zFixed;
			start.m_yFixed = position.m_yFixed + 0x10000;
			coordinate = (position.m_yFixed >> 12) + 200;
			ClampUpperTargetCoordinate(coordinate);
			target.m_yFixed = coordinate << 12;
			g_pAI->FireBullet(m_linkedObjectId, BULLET_TYPE_DEFAULT, OWNER_ENEMY, 8, start, target);

			start.m_xFixed = position.m_xFixed;
			start.m_zFixed = position.m_zFixed;
			target.m_xFixed = position.m_xFixed;
			target.m_zFixed = position.m_zFixed;
			start.m_yFixed = position.m_yFixed - 0x10000;
			coordinate = (position.m_yFixed >> 12) - 200;
			ClampLowerTargetCoordinate(coordinate);
			target.m_yFixed = coordinate << 12;
			g_pAI->FireBullet(m_linkedObjectId, BULLET_TYPE_DEFAULT, OWNER_ENEMY, 4, start, target);
			SetSndEffect(SFX_BIGGUN);
			m_lastMovementTick = m_actionDeadline;
			Action(ACTION_FIRING);
		}
		break;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0042c9a0
void CPaintGun::DoActivate()
{
}
