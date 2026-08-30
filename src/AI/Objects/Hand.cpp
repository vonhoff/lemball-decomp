#include "Hand.h"

#include "../../Control/Game/Game.h"
#include "../../Control/Game/GameTime.h"

// 68K 0x10610d62 __ct__5CHandFv
// STUB: LEMBALL 0x00427ad0
Hand::Hand()
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
// STUB: LEMBALL 0x00427b50
void Hand::Set(unsigned short p_id, const AiCoord& p_position)
{
}

// 68K 0x10610f5a Process__5CHandFv
// STUB: LEMBALL 0x00427c40
bool Hand::Process()
{
	return 0;
}

// 68K 0x106110b6 StepOn__5CHandFRC7AICOORDP11CGameObject
// STUB: LEMBALL 0x00427d70
bool Hand::StepOn(const AiCoord& p_position, GameObject* p_object)
{
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
