#include "CAmmo.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "Visos/Time/VsTime.h"
#include "../Base/AIScoreConstants.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/CBaseGlobalObject.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectInteractionStates.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

enum {
	AMMO_PICKUP_ACTIVATION_DURATION_TICKS = 8,
	AMMO_PICKUP_AMOUNT = 25
};

// FUNCTION: LEMBALL 0x0041c430
int CAmmo::Usage()
{
	return GROUP_OBJECT_USAGE_SINGLE;
}

// FUNCTION: LEMBALL 0x0041ca90
void CAmmo::Restart()
{
	CBaseGlobalObject::Restart();
	m_ammo = 0;
}

// FUNCTION: LEMBALL 0x0041cab0
bool CAmmo::Process()
{
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	int blockX;
	int blockY;
	CMap* map = g_pMap;
	blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	if (x >= 0 && y >= 0 && map->m_ground.m_width > blockX && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & GROUND_BLOCK_PIXEL_MASK;
		int cellY = y & GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = z << FIXED_POINT_FRACTION_BITS;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_ACTIVATED) {
				SetSndEffect(SFX_RELOAD);
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	switch (m_action) {
	case ACTION_ACTIVATED:
		if (m_actionPhase2Deadline < g_dwGameTick) {
			if (m_ammo == 0) {
				m_objectActive = 0;
			}
			else {
				m_actionDeadline = g_dwGameTick + (m_ammo * MILLISECONDS_PER_SECOND) / GAME_TICK_MILLISECONDS;
				RequestAction(ACTION_RUNNING);
			}
		}
		break;
	case ACTION_RUNNING:
		if (m_actionDeadline < g_dwGameTick) {
			RequestAction(ACTION_READY);
		}
		break;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0041cbe0
bool CAmmo::Activate(CGameObject* p_object)
{
	if (m_action == ACTION_READY && p_object->HasObject(m_objectType) == 0) {
		m_actionPhase2Deadline = AMMO_PICKUP_ACTIVATION_DURATION_TICKS;
		m_activator = p_object;
		RequestAction(ACTION_ACTIVATED);
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041cc30
void CAmmo::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionPhase2Deadline += g_dwGameTick;
	SetSndEffect(SFX_RELOAD);
	m_activator->PickUpAmmo(AMMO_PICKUP_AMOUNT);
	g_pAI->Score(AI_SCORE_AMMO_PICKUP_POINTS);
}

// FUNCTION: LEMBALL 0x0041cc70
AICOORD CAmmo::ActivatePosition()
{
	int z = m_position.m_zFixed;
	int y = m_position.m_yFixed;
	int x = m_position.m_xFixed;
	return AICOORD(x, y, z);
}
