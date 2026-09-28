#include "CTrampoline.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../../Visos/Foundation/CFixed.h"
#include "../../Visos/Foundation/CVSMath.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/C3DVector.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x0042a990
CTrampoline::CTrampoline() : CGlobalGameObject(OBJECT_TRAMPOLINE, 0, 0)
{
}

// FUNCTION: LEMBALL 0x0042a9b0
void CTrampoline::Restart()
{
	CGlobalGameObject::Restart();
	m_stateTimer = 0;
	m_enabled = 0;
	m_active = 0;
}

// FUNCTION: LEMBALL 0x0042a9d0
CTrampoline::~CTrampoline()
{
}

// FUNCTION: LEMBALL 0x0042a9e0
void CTrampoline::Set(unsigned short p_id, const AiCoord& p_position)
{
	SetId(p_id);
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_action = ACTION_READY;
	m_active = 1;
	m_enabled = 1;

	int blockX = (p_position.m_xFixed >> 12) / 16;
	if (blockX >= 0) {
		int blockY = (p_position.m_yFixed >> 12) / 16;
		if (blockY >= 0) {
			CMap* map = g_pMap;
			int width = map->m_ground.m_width;
			if (width > blockX && map->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= 0x8000;
			}
		}
	}
}

// FUNCTION: LEMBALL 0x0042aa80
bool CTrampoline::Process()
{
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_RUNNING) {
				SetSndEffect(SFX_TRMPLINE);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	if (m_enabled == 0) {
		return 1;
	}
	if (m_action == ACTION_RUNNING && m_actionDeadline < g_dwGameTick) {
		Action(ACTION_READY);
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0042aaf0
int CTrampoline::TryEnableNearPosition(const AiCoord& p_position, CGameObject* p_object)
{
	if ((int) Distance(m_position.m_xFixed >> 12,
					   m_position.m_yFixed >> 12,
					   p_position.m_xFixed >> 12,
					   p_position.m_yFixed >> 12) < 0x20) {
		m_enabled = 1;
		m_lastMovementTick = g_dwGameTick;
		m_stateTimer = g_dwSimulationTimestamp;
		m_position.m_xFixed = p_position.m_xFixed + 0x4000;
		m_position.m_yFixed = p_position.m_yFixed + 0x4000;
		int z = p_position.m_zFixed;
		m_position.m_zFixed = z;
		m_relocationZ = z >> 12;
		p_object->m_deathRequested = 1;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0042b9b0
void CTrampoline::DoActivate()
{
}
