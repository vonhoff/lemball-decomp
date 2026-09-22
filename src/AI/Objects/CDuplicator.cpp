#include "CDuplicator.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../Groups/CPlayerLemmingGroup.h"
#include "../Navigation/CAI.h"
#include "../Objects/CPlayerLemming.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x004275b0
CDuplicator::CDuplicator(const AiCoord& p_position) : CGlobalGameObject(OBJECT_DUPLICATOR, 0, 0)
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
	m_actionArgument = 0;
	m_stateTimer = 0;
	m_terrainCell1Set = 0;
	m_terrainCell0Set = 0;
	m_action = ACTION_0x18;
	Set(m_spawnPosition);
}

// FUNCTION: LEMBALL 0x00427630
void CDuplicator::Set(const AiCoord& p_position)
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

// FUNCTION: LEMBALL 0x004276f0
void CDuplicator::Delete()
{
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;
	if (blockX >= 0) {
		if (blockY >= 0) {
			CMap* map = g_pMap;
			int width = map->m_ground.m_width;
			if (blockX < width && map->m_ground.m_height > blockY) {
				map->m_ground.m_ground[blockY * width + blockX].m_collision &= ~1;
			}
		}
		if (blockX >= 0 && --blockY >= 0) {
			CMap* map = g_pMap;
			int width = map->m_ground.m_width;
			if (blockX < width && map->m_ground.m_height > blockY) {
				map->m_ground.m_ground[blockY * width + blockX].m_collision &= ~1;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x00427780
bool CDuplicator::Process()
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
		CGameObject* duplicatedObject = m_duplicatedObject;
		duplicatedObject->m_unk0xc0 = 0;
		duplicatedObject->m_action = ACTION_NONE;
		m_duplicatedObject->Action(ACTION_NONE);
		m_duplicatedObject->ResetInstructions();
		CPlayerLemming* dead = g_pAI->GetDead();
		if (dead != 0) {
			AiCoord pos(m_position.m_xFixed, m_position.m_yFixed - 0x34000, m_position.m_zFixed);
			dead->Resurrect(pos);
			CPlayerLemmingGroup* group = ((CPlayerLemming*) m_duplicatedObject)->GetGroup();
			group->AddLemmingToGroup(dead);
		}
		Action(ACTION_0x18);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x00427890
AiCoord CDuplicator::ActivatePosition()
{
	int y = m_position.m_yFixed + 0x8000;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed;
	return AiCoord(x, y, z);
}

// FUNCTION: LEMBALL 0x004278c0
bool CDuplicator::Activate(CGameObject* p_object)
{
	if (!g_pAI->nDead()) {
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

// FUNCTION: LEMBALL 0x00427910
void CDuplicator::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	int y = m_position.m_yFixed - 0x3c000;
	int z = m_position.m_zFixed;
	CGameObject* activator = m_activator;
	m_actionDeadline += g_dwGameTick;
	m_duplicatedObject = activator;
	int x = m_position.m_xFixed;
	activator->m_unk0xc0 = 1;
	activator->m_action = ACTION_5;
	CGameObject* dup = m_duplicatedObject;
	dup->m_position.m_xFixed = x;
	dup->m_position.m_yFixed = y;
	dup->m_position.m_zFixed = z;
	SetSndEffect(SFX_DUPLICTR);
	g_pAI->Score(100);
}

// FUNCTION: LEMBALL 0x00427a90
int CDuplicator::Usage()
{
	return 2;
}
