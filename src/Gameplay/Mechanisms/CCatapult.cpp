#include "CCatapult.h"

#include "Application/GameMain.h"

#include "Gameplay/Simulation/GameTime.h"
#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Objects/ObjectInteractionStates.h"
#include "Gameplay/Objects/CViewData.h"
#include "Engine/Math/RandomConstants.h"
// FUNCTION: LEMBALL 0x0041c3f0
int CCatapult::Usage()
{
	return GROUP_OBJECT_USAGE_GROUP;
}

// FUNCTION: LEMBALL 0x0041c700
void CCatapult::Restart()
{
	CBaseGlobalObject::Restart();
	m_actionArgument = REMOTE_PALETTE_REMAP_DISABLED;
}

#include "Map/CMap.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Geometry/C3DVector.h"
#include "Gameplay/Objects/CBaseGlobalObject.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Application/SoundEffects.h"

#include <stddef.h>

enum {
	CATAPULT_LAUNCH_HEIGHT_OFFSET_FIXED = 32 * FIXED_POINT_ONE,
	CATAPULT_RANDOM_VELOCITY_STEPS = 32768,
	CATAPULT_RANDOM_HORIZONTAL_LAUNCH_SPEED_FIXED = 9 * FIXED_POINT_ONE,
	CATAPULT_RANDOM_VERTICAL_LAUNCH_SPEED_FIXED = 12 * FIXED_POINT_ONE,
	CATAPULT_LAUNCH_ORIGIN_OFFSET_FIXED = 12 * FIXED_POINT_ONE,
	CATAPULT_ACTIVATION_POSITION_X_OFFSET_FIXED = 60 * FIXED_POINT_ONE,
	CATAPULT_ACTIVATION_POSITION_Y_OFFSET_FIXED = 12 * FIXED_POINT_ONE,
	CATAPULT_FIRST_ACTIVATION_PHASE_TICKS = 32,
	CATAPULT_SECOND_ACTIVATION_PHASE_TICKS = 46,
	CATAPULT_LAUNCH_DEADLINE_TICKS = 94
};

// FUNCTION: LEMBALL 0x0041c720
bool CCatapult::Process()
{
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	CMap* map = g_pMap;
	int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && map->m_ground.m_height > blockY) {
		int cellX = x & GROUND_BLOCK_PIXEL_MASK;
		int cellY = y & GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = (int) z * FIXED_POINT_ONE;
	if (m_isRemoteObject != 0) {
		m_actionArgument = REMOTE_PALETTE_REMAP_ENABLED;
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_RUNNING) {
				SetSndEffect(SFX_CATAPULT);
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	m_actionArgument = REMOTE_PALETTE_REMAP_DISABLED;
	switch (m_action) {
	case ACTION_ACTIVATING:
		if (g_dwGameTick > m_actionPhase1Deadline) {
			Action(ACTION_ACTIVATED);
		}
		break;
	case ACTION_ACTIVATED: {
		if (g_dwGameTick > m_actionPhase2Deadline) {
			C3DVector pos;
			pos.m_xFixed = m_position.m_xFixed - CATAPULT_LAUNCH_ORIGIN_OFFSET_FIXED;
			pos.m_yFixed = m_position.m_yFixed - CATAPULT_LAUNCH_ORIGIN_OFFSET_FIXED;
			pos.m_zFixed = m_position.m_zFixed + CATAPULT_LAUNCH_HEIGHT_OFFSET_FIXED;

			C3DVector vel;
			int randX = (*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
			*g_pRandomSeed = randX;
			vel.m_xFixed =
				((randX % CATAPULT_RANDOM_VELOCITY_STEPS) * FIXED_POINT_ONE / CATAPULT_RANDOM_VELOCITY_STEPS) +
				CATAPULT_RANDOM_HORIZONTAL_LAUNCH_SPEED_FIXED;
			int randY = (*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
			*g_pRandomSeed = randY;
			vel.m_yFixed = (randY % CATAPULT_RANDOM_VELOCITY_STEPS) * FIXED_POINT_ONE / CATAPULT_RANDOM_VELOCITY_STEPS;
			int randZ = (*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
			*g_pRandomSeed = randZ;
			vel.m_zFixed =
				((randZ % CATAPULT_RANDOM_VELOCITY_STEPS) * FIXED_POINT_ONE / CATAPULT_RANDOM_VELOCITY_STEPS) +
				CATAPULT_RANDOM_VERTICAL_LAUNCH_SPEED_FIXED;

			CGameObject* activator = m_activator;
			activator->m_hidden = 0;
			activator->m_action = ACTION_NONE;
			m_activator->StartFly(vel, &pos);
			m_activator = NULL;
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
		m_actionPhase1Deadline = CATAPULT_FIRST_ACTIVATION_PHASE_TICKS;
		m_actionPhase2Deadline = CATAPULT_SECOND_ACTIVATION_PHASE_TICKS;
		m_actionDeadline = CATAPULT_LAUNCH_DEADLINE_TICKS;
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
	g_pAI->Score(AI_SCORE_CATAPULT_USE_POINTS);
}

// FUNCTION: LEMBALL 0x0041ca60
AICOORD CCatapult::ActivatePosition()
{
	int y = m_position.m_yFixed - CATAPULT_ACTIVATION_POSITION_Y_OFFSET_FIXED;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed - CATAPULT_ACTIVATION_POSITION_X_OFFSET_FIXED;
	return AICOORD(x, y, z);
}
