#include "Mine.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../Managers/MineManager.h"

// GLOBAL: LEMBALL 0x004a7840
short g_mineTerrainOffsets[4];

// 68K 0x10616282 __ct__5CMineFv
// FUNCTION: LEMBALL 0x00423c10
Mine::Mine() : GlobalGameObject(0x13, 0, 0)
{
}

// 68K 0x106162c2 Restart__5CMineFv
// FUNCTION: LEMBALL 0x00423c30
void Mine::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
}

// 68K 0x106162f2 Initialise__5CMineFv
// FUNCTION: LEMBALL 0x00423c50
void Mine::Initialise()
{
	m_action = (eAction) 0x18;
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

// 68K 0x10616354 Set__5CMineF7AICOORD
// FUNCTION: LEMBALL 0x00423cb0
void Mine::Set(AiCoord p_position)
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

// 68K 0x106163f0 Trigger__5CMineFi
// FUNCTION: LEMBALL 0x00423d40
void Mine::Trigger(int p_delay)
{
	if (m_triggerPending == 0 && m_action == (eAction) 0x18) {
		m_triggerPending = 1;
		m_triggerDelay = p_delay;
		RequestAction((eAction) 0x1a);
	}
}

// 68K 0x1061643c DoActivate__5CMineFv
// FUNCTION: LEMBALL 0x00423d70
void Mine::DoActivate()
{
	m_activated = 1;
	if (m_triggerPending != 0) {
		m_terrainSet = 0;
		m_lastMovementTick = g_dwGameTick + m_triggerDelay;
		((MineManager*) m_manager)->Triggered(this);
		return;
	}
	SetTerrain();
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionDeadline = g_dwGameTick + 20;
}

// 68K 0x106164b0 SetTerrain__5CMineFv
// FUNCTION: LEMBALL 0x00423dd0
void Mine::SetTerrain()
{
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;
	if (m_terrainSet == 0) {
		g_pMap->SetTerrain(blockX,
						   blockY,
						   (eObjectType) 0x20a,
						   (unsigned short) g_mineTerrainOffsets[g_pMap->m_reserved]);
		m_transientFlags = 1;
		if (blockX >= 0 && blockY >= 0) {
			Map* map = g_pMap;
			int width = map->m_ground.m_width;
			if (blockX < width && blockY < map->m_ground.m_height) {
				map->m_ground.m_ground[width * blockY + blockX].m_collision |= 4;
			}
		}
	}
	SetSndEffect((eSoundEffect) 16);
}

// 68K 0x10616578 StepOn__5CMineFP11CGameObject
// FUNCTION: LEMBALL 0x00423e70
void Mine::StepOn(GameObject* p_object)
{
	RequestAction(0x1b);
	p_object->HitMine();
}

// 68K 0x106165be IsUsable__5CMineF7eAction
// FUNCTION: LEMBALL 0x00423e90
bool Mine::IsUsable(eAction p_action)
{
	return p_action == (eAction) 8 || p_action == (eAction) 0x18;
}

// 68K 0x106165fa Process__5CMineFv
// FUNCTION: LEMBALL 0x00423eb0
bool Mine::Process()
{
	eAction action = m_action;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != action) {
			switch (action) {
			case (eAction) 8:
				m_enabled = m_activated = 0;
				break;
			case (eAction) 27:
				SetTerrain();
				break;
			}
		}
		m_pendingAction = m_action;
		return 1;
	}

	switch (action) {
	case (eAction) 25:
		m_terrainSet = 0;
		return 0;
	case (eAction) 26:
		if (m_lastMovementTick < g_dwGameTick) {
			SetTerrain();
			m_stateTimer = g_dwSimulationTimestamp;
			m_actionDeadline = g_dwGameTick + 20;
			Action((eAction) 27);
			return 0;
		}
		break;
	case (eAction) 27:
		if (m_actionDeadline < g_dwGameTick) {
			m_activated = 0;
			m_enabled = 0;
			m_lastMovementTick = g_dwGameTick + 100;
			Action((eAction) 8);
		}
		break;
	default:
		return 0;
	}
	return 0;
}

// 68K 0x106166fc OnGround__5CMineFv
// FUNCTION: LEMBALL 0x00423fa0
void Mine::OnGround()
{
	int x = m_position.m_xFixed >> 12;
	int y = m_position.m_yFixed >> 12;
	Map* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x < 0 || y < 0 || map->m_ground.m_width <= blockX || g_pMap->m_ground.m_height <= blockY) {
		z = 0;
	}
	else {
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(x & 0xf, y & 0xf);
	}
	m_position.m_zFixed = (unsigned int) z << 12;
}

// 68K 0x1011a84c __dt__5CMineFv
Mine::~Mine()
{
}
