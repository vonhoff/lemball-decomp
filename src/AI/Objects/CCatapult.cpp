#include "CCatapult.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../Navigation/CAI.h"

// FUNCTION: LEMBALL 0x0041c3f0
int CCatapult::Usage()
{
	return 1;
}

// FUNCTION: LEMBALL 0x0041c700
void CCatapult::Restart()
{
	CBaseGlobalObject::Restart();
	m_actionArgument = 0;
}

#include "../../Map/Base/CMap.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/C3DVector.h"
#include "AI/Base/CBaseGlobalObject.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x0041c720
bool CCatapult::Process()
{
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	CMap* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && map->m_ground.m_height > blockY) {
		int cellX = x & 0xf;
		int cellY = y & 0xf;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = (int) z * 0x1000;
	if (m_isRemoteObject != 0) {
		m_actionArgument = 1;
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_RUNNING) {
				SetSndEffect(SFX_CATAPULT);
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	m_actionArgument = 0;
	switch (m_action) {
	case ACTION_ACTIVATING:
		if (g_dwGameTick > m_actionPhase1Deadline) {
			Action(ACTION_ACTIVATED);
		}
		break;
	case ACTION_ACTIVATED: {
		if (g_dwGameTick > m_actionPhase2Deadline) {
			C3DVector pos;
			pos.m_xFixed = m_position.m_xFixed - 0xc000;
			pos.m_yFixed = m_position.m_yFixed - 0xc000;
			pos.m_zFixed = m_position.m_zFixed + 0x20000;

			C3DVector vel;
			int randX = (*g_pRandomSeed * 0x29 + 0x1f) & 0x7fffff;
			*g_pRandomSeed = randX;
			vel.m_xFixed = ((randX % 32768) * 4096 / 32768) + 0x9000;
			int randY = (*g_pRandomSeed * 0x29 + 0x1f) & 0x7fffff;
			*g_pRandomSeed = randY;
			vel.m_yFixed = ((randY % 32768) * 4096 / 32768);
			int randZ = (*g_pRandomSeed * 0x29 + 0x1f) & 0x7fffff;
			*g_pRandomSeed = randZ;
			vel.m_zFixed = ((randZ % 32768) * 4096 / 32768) + 0xc000;

			CGameObject* activator = m_activator;
			activator->m_hidden = 0;
			activator->m_action = ACTION_NONE;
			m_activator->StartFly(vel, &pos);
			m_activator = 0;
			Action(ACTION_RUNNING);
			SetSndEffect(SFX_CATAPULT);
		}
		break;
	}
	case ACTION_RUNNING:
		if (g_dwGameTick > m_actionDeadline) {
			Action(ACTION_READY);
		}
		break;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0041c9b0
bool CCatapult::Activate(CGameObject* p_object)
{
	if (m_action == ACTION_READY) {
		m_activator = p_object;
		m_stateTimer = g_dwSimulationTimestamp;
		m_actionPhase1Deadline = 32;
		m_actionPhase2Deadline = 46;
		m_actionDeadline = 94;
		RequestAction(ACTION_ACTIVATING);
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041ca00
void CCatapult::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionPhase1Deadline += g_dwGameTick;
	m_actionPhase2Deadline += g_dwGameTick;
	m_actionDeadline += g_dwGameTick;
	CGameObject* activator = m_activator;
	m_activatorObjectType = activator->m_objectType;
	activator->m_hidden = 1;
	activator->m_action = ACTION_HIDDEN;
	g_pAI->Score(20);
}

// FUNCTION: LEMBALL 0x0041ca60
AICOORD CCatapult::ActivatePosition()
{
	int y = m_position.m_yFixed - 0xc000;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed - 0x3c000;
	return AICOORD(x, y, z);
}
