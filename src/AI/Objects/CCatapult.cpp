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
#include "AI/Base/AiCoord.h"
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
			if (m_action == ACTION_0x1b) {
				SetSndEffect(SFX_CATAPULT);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	m_actionArgument = 0;
	switch (m_action) {
	case ACTION_0x19:
		if (g_dwGameTick > m_actionPhase1Deadline) {
			Action(ACTION_0x1a);
		}
		break;
	case ACTION_0x1a: {
		if (g_dwGameTick > m_actionPhase2Deadline) {
			C3DVector pos;
			pos.m_xFixed = m_position.m_xFixed - 0xc000;
			pos.m_yFixed = m_position.m_yFixed - 0xc000;
			pos.m_zFixed = m_position.m_zFixed + 0x20000;

			C3DVector vel;
			int r1 = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
			*g_pSentinel = r1;
			vel.m_xFixed = ((r1 % 32768) * 4096 / 32768) + 0x9000;
			int r2 = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
			*g_pSentinel = r2;
			vel.m_yFixed = ((r2 % 32768) * 4096 / 32768);
			int r3 = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
			*g_pSentinel = r3;
			vel.m_zFixed = ((r3 % 32768) * 4096 / 32768) + 0xc000;

			m_activator->m_unk0xc0 = 0;
			m_activator->m_action = ACTION_NONE;
			m_activator->StartFly(vel, &pos);
			m_activator = 0;
			Action(ACTION_0x1b);
			SetSndEffect(SFX_CATAPULT);
		}
		break;
	}
	case ACTION_0x1b:
		if (g_dwGameTick > m_actionDeadline) {
			Action(ACTION_0x18);
		}
		break;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0041c9b0
bool CCatapult::Activate(CGameObject* p_object)
{
	if (m_action == ACTION_0x18) {
		m_activator = p_object;
		m_stateTimer = g_dwSimulationTimestamp;
		m_actionPhase1Deadline = 32;
		m_actionPhase2Deadline = 46;
		m_actionDeadline = 94;
		RequestAction(ACTION_0x19);
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041ca00
void CCatapult::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionPhase1Deadline += g_dwGameTick;
	m_actionPhase2Deadline += g_dwGameTick;
	m_actionDeadline += g_dwGameTick;
	CGameObject* activator = m_activator;
	m_unk0x90 = activator->m_objectType;
	activator->m_unk0xc0 = 1;
	activator->m_action = ACTION_5;
	g_pAI->Score(20);
}

// FUNCTION: LEMBALL 0x0041ca60
AiCoord CCatapult::ActivatePosition()
{
	int y = m_position.m_yFixed - 0xc000;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed - 0x3c000;
	return AiCoord(x, y, z);
}
