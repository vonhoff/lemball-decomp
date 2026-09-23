#include "CAmmo.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CBaseGlobalObject.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x0041c430
int CAmmo::Usage()
{
	return 2;
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
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	CMap* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x >= 0 && y >= 0 && map->m_ground.m_width > blockX && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & 0xf;
		int cellY = y & 0xf;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = z << 12;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_0x1a) {
				SetSndEffect(SFX_RELOAD);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	switch (m_action) {
	case ACTION_0x1a:
		if (m_actionPhase2Deadline < g_dwGameTick) {
			if (m_ammo == 0) {
				m_objectActive = 0;
			}
			else {
				m_actionDeadline = g_dwGameTick + (m_ammo * 1000) / GAME_TICK_MILLISECONDS;
				RequestAction(ACTION_0x1b);
			}
		}
		break;
	case ACTION_0x1b:
		if (m_actionDeadline < g_dwGameTick) {
			RequestAction(ACTION_0x18);
		}
		break;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0041cbe0
bool CAmmo::Activate(CGameObject* p_object)
{
	if (m_action == ACTION_0x18 && p_object->HasObject(m_objectType) == 0) {
		m_actionPhase2Deadline = 8;
		m_activator = p_object;
		RequestAction(ACTION_0x1a);
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041cc30
void CAmmo::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionPhase2Deadline += g_dwGameTick;
	SetSndEffect(SFX_RELOAD);
	m_activator->PickUpAmmo(25);
	g_pAI->Score(50);
}

// FUNCTION: LEMBALL 0x0041cc70
AiCoord CAmmo::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed;
	return AiCoord(x, y, z);
}
