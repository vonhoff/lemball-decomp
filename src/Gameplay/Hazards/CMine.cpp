#include "CMine.h"

#include "Application/GameTime.h"
#include "Map/CMap.h"
#include "CMineManager.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Application/SoundEffects.h"

enum {
	MINE_DETONATION_DURATION_TICKS = 20,
	MINE_DEAD_STATE_DELAY_TICKS = 100
};

// GLOBAL: LEMBALL 0x004a7840
short g_mineTerrainOffsets[4];

// FUNCTION: LEMBALL 0x00423c10
CMine::CMine() : CGlobalGameObject(OBJECT_MINE, 0, 0)
{
}

// FUNCTION: LEMBALL 0x00423c30
void CMine::Restart()
{
	CGlobalGameObject::Restart();
	Initialise();
}

// FUNCTION: LEMBALL 0x00423c50
void CMine::Initialise()
{
	m_action = ACTION_READY;
	m_enabled = 0;
	m_activated = 0;
	m_terrainSet = 0;
	m_triggerPending = 0;
	m_actionDeadline = g_dwGameTick;
	g_mineTerrainOffsets[0] = 10;
	g_mineTerrainOffsets[1] = 17;
	g_mineTerrainOffsets[2] = 48;
	g_mineTerrainOffsets[3] = 8;
}

// FUNCTION: LEMBALL 0x00423cb0
void CMine::Set(AICOORD p_position)
{
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_activated = 0;
	m_enabled = 1;
	m_terrainSet = 0;
	int blockX = (p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	int blockY = (p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	if (blockX >= 0 && blockY >= 0) {
		int width = g_pMap->m_ground.m_width;
		if (blockX >= width) {
			return;
		}
		if (g_pMap->m_ground.m_height <= blockY) {
			return;
		}
		g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= GROUND_COLLISION_OBJECT_INTERACTION;
	}
}

// FUNCTION: LEMBALL 0x00423d40
void CMine::Trigger(int p_delay)
{
	if (m_triggerPending == 0 && m_action == ACTION_READY) {
		m_triggerPending = 1;
		m_triggerDelay = p_delay;
		RequestAction(ACTION_ACTIVATED);
	}
}

// FUNCTION: LEMBALL 0x00423d70
void CMine::DoActivate()
{
	m_activated = 1;
	if (m_triggerPending != 0) {
		m_terrainSet = 0;
		m_lastMovementTick = g_dwGameTick + m_triggerDelay;
		((CMineManager*) m_manager)->Triggered(this);
		return;
	}
	SetTerrain();
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionDeadline = g_dwGameTick + MINE_DETONATION_DURATION_TICKS;
}

// FUNCTION: LEMBALL 0x00423dd0
void CMine::SetTerrain()
{
	int blockX = (m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	int blockY = (m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	if (m_terrainSet == 0) {
		g_pMap->SetTerrain(blockX, blockY, TERRAIN_BLOX_5, (unsigned short) g_mineTerrainOffsets[g_pMap->m_mapType]);
		m_transientFlags = 1;
		if (blockX >= 0 && blockY >= 0) {
			CMap* map = g_pMap;
			int width = map->m_ground.m_width;
			if (blockX < width && blockY < map->m_ground.m_height) {
				map->m_ground.m_ground[width * blockY + blockX].m_collision |= GROUND_COLLISION_HAZARD;
			}
		}
	}
	SetSndEffect(SFX_MINEEXP);
}

// FUNCTION: LEMBALL 0x00423e70
void CMine::StepOn(CGameObject* p_object)
{
	RequestAction(ACTION_RUNNING);
	p_object->HitMine();
}

// FUNCTION: LEMBALL 0x00423e90
bool CMine::IsUsable(eAction p_action)
{
	return p_action == ACTION_DEAD || p_action == ACTION_READY;
}

// FUNCTION: LEMBALL 0x00423eb0
bool CMine::Process()
{
	eAction action = m_action;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != action) {
			switch (action) {
			case ACTION_DEAD:
				m_enabled = m_activated = 0;
				break;
			case ACTION_RUNNING:
				SetTerrain();
				break;
			}
		}
		m_pendingAction = m_action;
		return true;
	}

	switch (action) {
	case ACTION_ACTIVATING:
		m_terrainSet = 0;
		return false;
	case ACTION_ACTIVATED:
		if (m_lastMovementTick < g_dwGameTick) {
			SetTerrain();
			m_stateTimer = g_dwSimulationTimestamp;
			m_actionDeadline = g_dwGameTick + MINE_DETONATION_DURATION_TICKS;
			Action(ACTION_RUNNING);
			return false;
		}
		break;
	case ACTION_RUNNING:
		if (m_actionDeadline < g_dwGameTick) {
			m_activated = 0;
			m_enabled = 0;
			m_lastMovementTick = g_dwGameTick + MINE_DEAD_STATE_DELAY_TICKS;
			Action(ACTION_DEAD);
		}
		break;
	default:
		return false;
	}
	return false;
}

// FUNCTION: LEMBALL 0x00423fa0
void CMine::OnGround()
{
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	CMap* map = g_pMap;
	int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	if (x < 0 || y < 0 || map->m_ground.m_width <= blockX || g_pMap->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		int groundX = x & GROUND_BLOCK_PIXEL_MASK;
		int groundY = y & GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(groundX, groundY);
	}
	const unsigned int height = (unsigned int) z << FIXED_POINT_FRACTION_BITS;
	m_position.m_zFixed = height;
}
