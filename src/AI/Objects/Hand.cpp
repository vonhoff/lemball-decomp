#include "Hand.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/Map.h"

// 68K 0x10610d62 __ct__5CHandFv
// FUNCTION: LEMBALL 0x00427ad0
Hand::Hand() : GlobalGameObject(0x1f, 0, 0)
{
}

// 68K 0x10610da2 Initialise__5CHandFv
// FUNCTION: LEMBALL 0x00427af0
void Hand::Initialise()
{
	m_stateTimer = 0;
	m_activated = 0;
	m_enabled = 0;
}

// 68K 0x10610dd2 Restart__5CHandFv
// FUNCTION: LEMBALL 0x00427b10
void Hand::Restart()
{
	GlobalGameObject::Restart();
	Initialise();
	m_position.m_xFixed = m_spawnPosition.m_xFixed;
	m_position.m_yFixed = m_spawnPosition.m_yFixed;
	m_position.m_zFixed = m_spawnPosition.m_zFixed;
}

// 68K 0x10610e74 Set__5CHandFUsRC7AICOORD
// FUNCTION: LEMBALL 0x00427b50
void Hand::Set(unsigned short p_id, const AiCoord& p_position)
{
	SetId(p_id);
	m_spawnPosition.m_xFixed = p_position.m_xFixed;
	m_spawnPosition.m_yFixed = p_position.m_yFixed;
	m_spawnPosition.m_zFixed = p_position.m_zFixed;
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_enabled = 1;
	m_action = (eAction) 0x18;
	m_actionArgument = 0;
	m_activated = 0;

	int blockX = (p_position.m_xFixed >> 12) / 16;
	int blockY = (p_position.m_yFixed >> 12) / 16;

	if (blockX >= 0) {
		int collisionY = blockY + 1;
		if (collisionY >= 0 && g_pMap->m_ground.m_width > blockX && g_pMap->m_ground.m_height > collisionY) {
			g_pMap->m_ground.m_ground[g_pMap->m_ground.m_width * collisionY + blockX].m_collision |= 0x8000;
		}

		if (blockX >= 0) {
			collisionY = blockY + 2;
			if (collisionY >= 0 && g_pMap->m_ground.m_width > blockX && g_pMap->m_ground.m_height > collisionY) {
				g_pMap->m_ground.m_ground[g_pMap->m_ground.m_width * collisionY + blockX].m_collision |= 0x8000;
			}
		}
	}
}

// 68K 0x10610f5a Process__5CHandFv
// STUB: LEMBALL 0x00427c40
bool Hand::Process()
{
	return 0;
}

// 68K 0x106110b6 StepOn__5CHandFRC7AICOORDP11CGameObject
// FUNCTION: LEMBALL 0x00427d70
bool Hand::StepOn(const AiCoord& p_position, GameObject* p_object)
{
	if (p_object->m_objectType == (eObjectType) 2) {
		int distanceY = (p_position.m_yFixed >> 12) - (m_position.m_yFixed >> 12);
		int distanceX = (p_position.m_xFixed >> 12) - (m_position.m_xFixed >> 12);
		if ((distanceX < 0 ? -distanceX : distanceX) < 16 && distanceY >= 0 && distanceY < 48) {
			m_unk0xd0 = 6;
			m_actionDeadline = 16;
			m_activator = p_object;
			p_object->ResetInstructions();
			m_activator->Action((eAction) 0);
			m_activator->m_actionDeadline = g_dwGameTick + 1000;
			RequestAction((eAction) 25);
			return 1;
		}
	}
	return 0;
}

// 68K 0x106111a0 DoActivate__5CHandFv
// FUNCTION: LEMBALL 0x00427e10
void Hand::DoActivate()
{
	m_activated = 1;
	m_target = m_activator;
	m_lastMovementTick = g_dwGameTick;
	m_unk0xd0 += g_dwGameTick;
	m_actionDeadline += g_dwGameTick;
	m_stateTimer = g_dwSimulationTimestamp;
}

// 68K 0x10610e1c __dt__5CHandFv
Hand::~Hand()
{
}
