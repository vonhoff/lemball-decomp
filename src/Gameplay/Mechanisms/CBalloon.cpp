#include "CBalloon.h"

#include "Application/GameMain.h"
#include "Application/SoundEffects.h"
#include "Engine/Math/FixedPoint.h"
#include "Engine/Math/RandomConstants.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CBaseGlobalObject.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectInteractionStates.h"
#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Simulation/GameTime.h"
#include "Map/CMap.h"

enum {
	BALLOON_ANIMATION_PHASE_RANDOMIZATION_RANGE_MS = 4096
};

#include "Map/CGround.h"
#include "Map/CGroundArray.h"

// FUNCTION: LEMBALL 0x0041c630
int CBalloon::Usage()
{
	return GROUP_OBJECT_USAGE_SINGLE;
}

// FUNCTION: LEMBALL 0x0041d600
void CBalloon::Restart()
{
	CBaseGlobalObject::Restart();
	int randVal = (*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
	*g_pRandomSeed = randVal;
	m_stateTimer = g_dwSimulationTimestamp - (randVal % BALLOON_ANIMATION_PHASE_RANDOMIZATION_RANGE_MS);
}

// FUNCTION: LEMBALL 0x0041d650
bool CBalloon::Process()
{
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	CMap* map = g_pMap;
	int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	if (x >= 0 && y >= 0 && map->m_ground.m_width > blockX && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & GROUND_BLOCK_PIXEL_MASK;
		int cellY = y & GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = z << FIXED_POINT_FRACTION_BITS;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_ACTIVATED) {
				SetSndEffect(SFX_COLLECT_BALLOON);
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	if (m_action == ACTION_ACTIVATED) {
		Action(ACTION_READY);
		m_objectActive = 0;
		return true;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0041d740
bool CBalloon::Activate(CGameObject* p_object)
{
	m_activator = p_object;
	if (m_activator->HasObject(m_objectType) == 0) {
		RequestAction(ACTION_ACTIVATED);
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041d780
void CBalloon::DoActivate()
{
	m_activator->AddObject(m_objectType, this);
	SetSndEffect(SFX_COLLECT_BALLOON);
	g_pAI->Score(AI_SCORE_BALLOON_PICKUP_POINTS);
}

// FUNCTION: LEMBALL 0x0041d7b0
AICOORD CBalloon::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	return AICOORD(m_position.m_xFixed, y, z);
}
