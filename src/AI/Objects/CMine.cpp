#include "CMine.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../Managers/CMineManager.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

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
	m_action = ACTION_0x18;
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
void CMine::Set(AiCoord p_position)
{
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_activated = 0;
	m_enabled = 1;
	m_terrainSet = 0;
	int blockX = (p_position.m_xFixed >> 12) / 16;
	int blockY = (p_position.m_yFixed >> 12) / 16;
	if (blockX >= 0 && blockY >= 0) {
		int width = g_pMap->m_ground.m_width;
		if (blockX >= width) {
			return;
		}
		if (g_pMap->m_ground.m_height <= blockY) {
			return;
		}
		g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= 0x8000;
	}
}

// FUNCTION: LEMBALL 0x00423d40
void CMine::Trigger(int p_delay)
{
	if (m_triggerPending == 0 && m_action == ACTION_0x18) {
		m_triggerPending = 1;
		m_triggerDelay = p_delay;
		RequestAction(ACTION_0x1a);
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
	m_actionDeadline = g_dwGameTick + 20;
}

// FUNCTION: LEMBALL 0x00423dd0
void CMine::SetTerrain()
{
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;
	if (m_terrainSet == 0) {
		g_pMap->SetTerrain(blockX, blockY, TERRAIN_BLOX_5, (unsigned short) g_mineTerrainOffsets[g_pMap->m_reserved]);
		m_transientFlags = 1;
		if (blockX >= 0 && blockY >= 0) {
			CMap* map = g_pMap;
			int width = map->m_ground.m_width;
			if (blockX < width && blockY < map->m_ground.m_height) {
				map->m_ground.m_ground[width * blockY + blockX].m_collision |= 4;
			}
		}
	}
	SetSndEffect(SFX_MINEEXP);
}

// FUNCTION: LEMBALL 0x00423e70
void CMine::StepOn(CGameObject* p_object)
{
	RequestAction(ACTION_0x1b);
	p_object->HitMine();
}

// FUNCTION: LEMBALL 0x00423e90
bool CMine::IsUsable(eAction p_action)
{
	return p_action == ACTION_8 || p_action == ACTION_0x18;
}

// FUNCTION: LEMBALL 0x00423eb0
bool CMine::Process()
{
	eAction action = m_action;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != action) {
			switch (action) {
			case ACTION_8:
				m_enabled = m_activated = 0;
				break;
			case ACTION_0x1b:
				SetTerrain();
				break;
			}
		}
		m_pendingAction = m_action;
		return 1;
	}

	switch (action) {
	case ACTION_0x19:
		m_terrainSet = 0;
		return 0;
	case ACTION_0x1a:
		if (m_lastMovementTick < g_dwGameTick) {
			SetTerrain();
			m_stateTimer = g_dwSimulationTimestamp;
			m_actionDeadline = g_dwGameTick + 20;
			Action(ACTION_0x1b);
			return 0;
		}
		break;
	case ACTION_0x1b:
		if (m_actionDeadline < g_dwGameTick) {
			m_activated = 0;
			m_enabled = 0;
			m_lastMovementTick = g_dwGameTick + 100;
			Action(ACTION_8);
		}
		break;
	default:
		return 0;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x00423fa0
void CMine::OnGround()
{
	int x = m_position.m_xFixed >> 12;
	int y = m_position.m_yFixed >> 12;
	CMap* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x < 0 || y < 0 || map->m_ground.m_width <= blockX || g_pMap->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		int groundX = x & 0xf;
		int groundY = y & 0xf;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(groundX, groundY);
	}
	m_position.m_zFixed = (unsigned int) z << 12;
}
