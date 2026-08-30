#include "Duplicator.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"
#include "../Groups/PlayerLemmingGroup.h"
#include "../Navigation/Ai.h"
#include "../Objects/PlayerLemming.h"

// 68K 0x106072fe __ct__11CDuplicatorFRC7AICOORD
// FUNCTION: LEMBALL 0x004275b0
Duplicator::Duplicator(const AiCoord& p_arg0) : GlobalGameObject((eObjectType) 0x1c, 0, 0)
{
	m_spawnPosition.m_xFixed = p_arg0.m_xFixed;
	m_spawnPosition.m_yFixed = p_arg0.m_yFixed;
	m_spawnPosition.m_zFixed = p_arg0.m_zFixed;
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
	m_action = (eAction) 24;
	Set(m_spawnPosition);
}

// 68K 0x10607420 Set__11CDuplicatorFRC7AICOORD
// FUNCTION: LEMBALL 0x00427630
void Duplicator::Set(const AiCoord& p_position)
{
	m_position = p_position;
	m_terrainCell0Set = 1;
	m_terrainCell1Set = 1;
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;
	if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |= 1;
	}
	blockY--;
	if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision |= 1;
	}
}

// 68K 0x106074de Delete__11CDuplicatorFv
// FUNCTION: LEMBALL 0x004276f0
void Duplicator::Delete()
{
	int blockX = (m_position.m_xFixed >> 12) / 16;
	int blockY = (m_position.m_yFixed >> 12) / 16;
	if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
	}
	blockY--;
	if (blockX >= 0 && blockY >= 0 && blockX < g_pMap->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		g_pMap->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].m_collision &= ~1;
	}
}

// 68K 0x10607574 Process__11CDuplicatorFv
// FUNCTION: LEMBALL 0x00427780
bool Duplicator::Process()
{
	if (m_isRemoteObject != 0) {
		m_actionArgument = 1;
		if (m_pendingAction != m_action) {
			if (m_action == 26) {
				SetSndEffect((eSoundEffect) 12);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	m_actionArgument = 0;
	if (m_action == 26 && m_actionDeadline < g_dwGameTick) {
		m_duplicatedObject->m_unk0xc0 = 0;
		m_duplicatedObject->m_action = (eAction) 0;
		m_duplicatedObject->Action((eAction) 0);
		m_duplicatedObject->ResetInstructions();
		PlayerLemming* dead = g_pAI->GetDead();
		if (dead != 0) {
			AiCoord pos(m_position.m_xFixed, m_position.m_yFixed - 0x34000, m_position.m_zFixed);
			dead->Resurrect(pos);
			PlayerLemmingGroup* group = ((PlayerLemming*) m_duplicatedObject)->GetGroup();
			group->AddLemmingToGroup(dead);
		}
		Action((eAction) 24);
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
	if (p_object->m_objectType == (eObjectType) 2 && m_action == 24) {
		m_actionDeadline = 82;
		m_activator = p_object;
		RequestAction((eAction) 26);
		return 1;
	}
	return 0;
}

// 68K 0x106077b8 DoActivate__11CDuplicatorFv
// FUNCTION: LEMBALL 0x00427910
void Duplicator::DoActivate()
{
	int y = m_position.m_yFixed - 0x3c000;
	int z = m_position.m_zFixed;
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionDeadline += g_dwGameTick;
	m_duplicatedObject = m_activator;
	int x = m_position.m_xFixed;
	m_duplicatedObject->m_unk0xc0 = 1;
	m_duplicatedObject->m_action = (eAction) 5;
	m_duplicatedObject->m_position.m_xFixed = x;
	m_duplicatedObject->m_position.m_yFixed = y;
	m_duplicatedObject->m_position.m_zFixed = z;
	SetSndEffect((eSoundEffect) 12);
	g_pAI->AddTime(100);
}

// 68K 0x10119bcc Usage__11CDuplicatorFv
// FUNCTION: LEMBALL 0x00427a90
int Duplicator::Usage()
{
	return 2;
}

// 68K 0x10607368 __dt__11CDuplicatorFv
Duplicator::~Duplicator()
{
}
