#include "CDuplicator.h"

#include "Gameplay/Simulation/GameTime.h"
#include "Map/CMap.h"
#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Groups/CPlayerLemmingGroup.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Characters/CPlayerLemming.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectInteractionStates.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Gameplay/Objects/CViewData.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Application/SoundEffects.h"

#include <stddef.h>

enum {
	DUPLICATOR_ACTIVATION_DELAY_TICKS = 82,
	DUPLICATOR_DUPLICATE_SPAWN_Y_OFFSET_FIXED = -52 * FIXED_POINT_ONE,
	DUPLICATOR_ACTIVATION_POSITION_Y_OFFSET_FIXED = 8 * FIXED_POINT_ONE,
	DUPLICATOR_STAGING_POSITION_Y_OFFSET_FIXED = -60 * FIXED_POINT_ONE
};

// FUNCTION: LEMBALL 0x004275b0
CDuplicator::CDuplicator(const AICOORD& p_position) : CGlobalGameObject(OBJECT_DUPLICATOR, 0, 0)
{
	m_spawnPosition.m_xFixed = p_position.m_xFixed;
	m_spawnPosition.m_yFixed = p_position.m_yFixed;
	m_spawnPosition.m_zFixed = p_position.m_zFixed;
}

// FUNCTION: LEMBALL 0x004275e0
CDuplicator::~CDuplicator()
{
}

// FUNCTION: LEMBALL 0x004275f0
void CDuplicator::Restart()
{
	CGlobalGameObject::Restart();
	m_actionArgument = REMOTE_PALETTE_REMAP_DISABLED;
	m_stateTimer = 0;
	m_terrainCell1Set = 0;
	m_terrainCell0Set = 0;
	m_action = ACTION_READY;
	Set(m_spawnPosition);
}

// FUNCTION: LEMBALL 0x00427630
void CDuplicator::Set(const AICOORD& p_position)
{
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_terrainCell0Set = 1;
	m_terrainCell1Set = 1;
	int blockX = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	int blockY = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	if (blockX >= 0) {
		if (blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |=
				GROUND_COLLISION_BLOCKS_WALKING;
		}
		if (blockX >= 0 && --blockY >= 0) {
			int width = g_pMap->m_ground.m_width;
			if (blockX < width && g_pMap->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= GROUND_COLLISION_BLOCKS_WALKING;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x004276f0
void CDuplicator::Delete()
{
	int blockX = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	int blockY = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	if (blockX >= 0) {
		if (blockY >= 0) {
			CMap* map = g_pMap;
			int width = map->m_ground.m_width;
			if (blockX < width && map->m_ground.m_height > blockY) {
				map->m_ground.m_ground[blockY * width + blockX].m_collision &= ~GROUND_COLLISION_BLOCKS_WALKING;
			}
		}
		if (blockX >= 0 && --blockY >= 0) {
			CMap* map = g_pMap;
			int width = map->m_ground.m_width;
			if (blockX < width && map->m_ground.m_height > blockY) {
				map->m_ground.m_ground[blockY * width + blockX].m_collision &= ~GROUND_COLLISION_BLOCKS_WALKING;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00427780
bool CDuplicator::Process()
{
	if (m_isRemoteObject != 0) {
		m_actionArgument = REMOTE_PALETTE_REMAP_ENABLED;
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_ACTIVATED) {
				SetSndEffect(SFX_DUPLICTR);
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	m_actionArgument = REMOTE_PALETTE_REMAP_DISABLED;
	if (m_action == ACTION_ACTIVATED && m_actionDeadline < g_dwGameTick) {
		CGameObject* duplicatedObject = m_duplicatedObject;
		duplicatedObject->m_hidden = 0;
		duplicatedObject->m_action = ACTION_NONE;
		m_duplicatedObject->Action(ACTION_NONE);
		m_duplicatedObject->ResetInstructions();
		CPlayerLemming* dead = g_pAI->GetDead();
		if (dead != NULL) {
			AICOORD pos;
			pos.m_xFixed = m_position.m_xFixed;
			int z = m_position.m_zFixed;
			pos.m_yFixed = m_position.m_yFixed + DUPLICATOR_DUPLICATE_SPAWN_Y_OFFSET_FIXED;
			pos.m_zFixed = z;
			dead->Resurrect(pos);
			CPlayerLemmingGroup* group = ((CPlayerLemming*) m_duplicatedObject)->GetGroup();
			group->AddLemmingToGroup(dead);
		}
		Action(ACTION_READY);
	}
	return true;
}

// FUNCTION: LEMBALL 0x00427890
AICOORD CDuplicator::ActivatePosition()
{
	int y = m_position.m_yFixed + DUPLICATOR_ACTIVATION_POSITION_Y_OFFSET_FIXED;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed;
	return AICOORD(x, y, z);
}

// FUNCTION: LEMBALL 0x004278c0
bool CDuplicator::Activate(CGameObject* p_object)
{
	if (!g_pAI->nDead()) {
		return false;
	}
	if (p_object->m_objectType == OBJECT_PLAYER_2 && m_action == ACTION_READY) {
		m_actionDeadline = DUPLICATOR_ACTIVATION_DELAY_TICKS;
		m_activator = p_object;
		RequestAction(ACTION_ACTIVATED);
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00427910
void CDuplicator::DoActivate()
{
	unsigned long timestamp = g_dwSimulationTimestamp;
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	y += DUPLICATOR_STAGING_POSITION_Y_OFFSET_FIXED;
	m_stateTimer = timestamp;
	CGameObject* activator = m_activator;
	m_actionDeadline += g_dwGameTick;
	m_duplicatedObject = activator;
	int x = m_position.m_xFixed;
	activator->m_hidden = 1;
	activator->m_action = ACTION_HIDDEN;
	CGameObject* dup = m_duplicatedObject;
	dup->m_position.m_xFixed = x;
	dup->m_position.m_yFixed = y;
	dup->m_position.m_zFixed = z;
	SetSndEffect(SFX_DUPLICTR);
	g_pAI->Score(AI_SCORE_DUPLICATOR_ACTIVATION_POINTS);
}

// FUNCTION: LEMBALL 0x00427a90
int CDuplicator::Usage()
{
	return GROUP_OBJECT_USAGE_SINGLE;
}
