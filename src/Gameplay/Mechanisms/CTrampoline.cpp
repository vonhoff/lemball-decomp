#include "CTrampoline.h"

#include "Game/CGame.h"
#include "Game/GameTime.h"
#include "Map/CMap.h"
#include "Gameplay/Geometry/CVSMath.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectTypes.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

enum {
	TRAMPOLINE_ACTIVATION_RADIUS_PIXELS = 32,
	TRAMPOLINE_OBJECT_RELOCATION_OFFSET_FIXED = 4 * FIXED_POINT_ONE
};

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
void CTrampoline::Set(unsigned short p_id, const AICOORD& p_position)
{
	SetId(p_id);
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_action = ACTION_READY;
	m_active = 1;
	m_enabled = 1;

	int blockX = (p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
	if (blockX >= 0) {
		int blockY = (p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) / GROUND_BLOCK_PIXEL_SIZE;
		if (blockY >= 0) {
			CMap* map = g_pMap;
			int width = map->m_ground.m_width;
			if (width > blockX && map->m_ground.m_height > blockY) {
				g_pMap->m_ground.m_ground[width * blockY + blockX].m_collision |= GROUND_COLLISION_OBJECT_INTERACTION;
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
		return true;
	}
	if (m_enabled == 0) {
		return true;
	}
	if (m_action == ACTION_RUNNING && m_actionDeadline < g_dwGameTick) {
		Action(ACTION_READY);
	}
	return true;
}

// FUNCTION: LEMBALL 0x0042aaf0
int CTrampoline::TryEnableNearPosition(const AICOORD& p_position, CGameObject* p_object)
{
	if ((int) Distance(m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
					   m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS,
					   p_position.m_xFixed >> FIXED_POINT_FRACTION_BITS,
					   p_position.m_yFixed >> FIXED_POINT_FRACTION_BITS) < TRAMPOLINE_ACTIVATION_RADIUS_PIXELS) {
		m_enabled = 1;
		m_lastMovementTick = g_dwGameTick;
		m_stateTimer = g_dwSimulationTimestamp;
		m_position.m_xFixed = p_position.m_xFixed + TRAMPOLINE_OBJECT_RELOCATION_OFFSET_FIXED;
		m_position.m_yFixed = p_position.m_yFixed + TRAMPOLINE_OBJECT_RELOCATION_OFFSET_FIXED;
		int z = p_position.m_zFixed;
		m_position.m_zFixed = z;
		m_relocationZ = z >> FIXED_POINT_FRACTION_BITS;
		p_object->m_deathRequested = 1;
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0042b9b0
void CTrampoline::DoActivate()
{
}
