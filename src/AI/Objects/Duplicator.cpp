#include "Duplicator.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../Groups/PlayerLemmingGroup.h"
#include "../Navigation/Ai.h"
#include "../Objects/PlayerLemming.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/GameObject.h"
#include "AI/Base/GlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/Ground.h"
#include "Map/Ground/GroundArray.h"
#include "Views/Sound/SoundEffects.h"

// 68K 0x106072fe __ct__11CDuplicatorFRC7AICOORD
// FUNCTION: LEMBALL 0x004275b0
Duplicator::Duplicator(const AiCoord& p_position) : GlobalGameObject(OBJECT_DUPLICATOR, 0, 0)
{
	m_spawnPosition.m_xFixed = p_position.m_xFixed;
	m_spawnPosition.m_yFixed = p_position.m_yFixed;
	m_spawnPosition.m_zFixed = p_position.m_zFixed;
}

// 68K 0x10607368 __dt__11CDuplicatorFv
// FUNCTION: LEMBALL 0x004275e0
Duplicator::~Duplicator()
{
}

// 68K 0x106073c6 Restart__11CDuplicatorFv
// FUNCTION: LEMBALL 0x004275f0
void Duplicator::Restart()
{
	GlobalGameObject::Restart();
	m_actionArgument = 0;
	m_stateTimer = 0;
	m_terrainCell1Set = 0;
	m_terrainCell0Set = 0;
	m_action = ACTION_0x18;
	Set(m_spawnPosition);
}

// 68K 0x10607420 Set__11CDuplicatorFRC7AICOORD
// FUNCTION: LEMBALL 0x00427630
void Duplicator::Set(const AiCoord& p_position)
{
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_terrainCell0Set = 1;
	m_terrainCell1Set = 1;
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;
	if (blockX >= 0) {
		if (blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
			g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |= 1;
		}
		if (blockX >= 0 && --blockY >= 0) {
			int width = g_pMap->m_ground.m_width;
			if (blockX < width && g_pMap->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= 1;
			}
		}
	}
}

// 68K 0x106074de Delete__11CDuplicatorFv
// FUNCTION: LEMBALL 0x004276f0
void Duplicator::Delete()
{
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;
	if (blockX >= 0) {
		if (blockY >= 0) {
			Map* map = g_pMap;
			int width = map->m_ground.m_width;
			if (blockX < width && map->m_ground.m_height > blockY) {
				map->m_ground.m_ground[blockY * width + blockX].m_collision &= ~1;
			}
		}
		if (blockX >= 0 && --blockY >= 0) {
			Map* map = g_pMap;
			int width = map->m_ground.m_width;
			if (blockX < width && map->m_ground.m_height > blockY) {
				map->m_ground.m_ground[blockY * width + blockX].m_collision &= ~1;
			}
		}
	}
}

// 68K 0x10607574 Process__11CDuplicatorFv
// FUNCTION: LEMBALL 0x00427780
bool Duplicator::Process()
{
	if (m_isRemoteObject != 0) {
		m_actionArgument = 1;
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_0x1a) {
				SetSndEffect(SFX_DUPLICTR);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	m_actionArgument = 0;
	if (m_action == ACTION_0x1a && m_actionDeadline < g_dwGameTick) {
		GameObject* duplicatedObject = m_duplicatedObject;
		duplicatedObject->m_unk0xc0 = 0;
		duplicatedObject->m_action = ACTION_NONE;
		m_duplicatedObject->Action(ACTION_NONE);
		m_duplicatedObject->ResetInstructions();
		PlayerLemming* dead = g_pAI->GetDead();
		if (dead != 0) {
			AiCoord pos(m_position.m_xFixed, m_position.m_yFixed - 0x34000, m_position.m_zFixed);
			dead->Resurrect(pos);
			PlayerLemmingGroup* group = ((PlayerLemming*) m_duplicatedObject)->GetGroup();
			group->AddLemmingToGroup(dead);
		}
		Action(ACTION_0x18);
	}
	return 1;
}

// 68K 0x106076b8 ActivatePosition__11CDuplicatorFv
// FUNCTION: LEMBALL 0x00427890
AiCoord Duplicator::ActivatePosition()
{
	int y = m_position.m_yFixed + 0x8000;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed;
	return AiCoord(x, y, z);
}

// 68K 0x10607736 Activate__11CDuplicatorFP11CGameObject
// FUNCTION: LEMBALL 0x004278c0
bool Duplicator::Activate(GameObject* p_object)
{
	if (!g_pAI->NDead()) {
		return 0;
	}
	if (p_object->m_objectType == OBJECT_PLAYER_2 && m_action == ACTION_0x18) {
		m_actionDeadline = 82;
		m_activator = p_object;
		RequestAction(ACTION_0x1a);
		return 1;
	}
	return 0;
}

// 68K 0x106077b8 DoActivate__11CDuplicatorFv
// FUNCTION: LEMBALL 0x00427910
void Duplicator::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	int y = m_position.m_yFixed - 0x3c000;
	int z = m_position.m_zFixed;
	GameObject* activator = m_activator;
	m_actionDeadline += g_dwGameTick;
	m_duplicatedObject = activator;
	int x = m_position.m_xFixed;
	activator->m_unk0xc0 = 1;
	activator->m_action = ACTION_5;
	GameObject* dup = m_duplicatedObject;
	dup->m_position.m_xFixed = x;
	dup->m_position.m_yFixed = y;
	dup->m_position.m_zFixed = z;
	SetSndEffect(SFX_DUPLICTR);
	g_pAI->Score(100);
}

// 68K 0x10119bcc Usage__11CDuplicatorFv
// FUNCTION: LEMBALL 0x00427a90
int Duplicator::Usage()
{
	return 2;
}
