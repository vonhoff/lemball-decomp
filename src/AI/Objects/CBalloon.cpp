#include "CBalloon.h"

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

// FUNCTION: LEMBALL 0x0041c630
int CBalloon::Usage()
{
	return 2;
}

// FUNCTION: LEMBALL 0x0041d600
void CBalloon::Restart()
{
	CBaseGlobalObject::Restart();
	int randVal = (*g_pSentinel * 0x29 + 0x1f) & 0x7fffff;
	*g_pSentinel = randVal;
	m_stateTimer = g_dwSimulationTimestamp - (randVal % 4096);
}

// FUNCTION: LEMBALL 0x0041d650
bool CBalloon::Process()
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
			if (m_action == 26) {
				SetSndEffect(SFX_COLLECT_BALLOON);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	if (m_action == 26) {
		Action(ACTION_0x18);
		m_heading = 0;
		return 1;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0041d740
bool CBalloon::Activate(CGameObject* p_object)
{
	m_activator = p_object;
	if (m_activator->HasObject(m_objectType) == 0) {
		RequestAction(ACTION_0x1a);
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041d780
void CBalloon::DoActivate()
{
	m_activator->AddObject(m_objectType, this);
	SetSndEffect(SFX_COLLECT_BALLOON);
	g_pAI->Score(50);
}

// FUNCTION: LEMBALL 0x0041d7b0
AiCoord CBalloon::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	return AiCoord(m_position.m_xFixed, y, z);
}
