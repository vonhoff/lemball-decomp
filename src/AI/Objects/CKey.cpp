#include "CKey.h"

#include "../../Map/Base/CMap.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x0041c5f0
int CKey::Usage()
{
	return 2;
}

// FUNCTION: LEMBALL 0x0041d480
bool CKey::Process()
{
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	CMap* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		int localX = x & 0xf;
		int localY = y & 0xf;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(localX, localY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = z << 12;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == 26) {
				SetSndEffect(SFX_KEYS);
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	if (m_action == 26) {
		Action(ACTION_0x18);
		m_heading = 0;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0041d560
bool CKey::Activate(CGameObject* p_object)
{
	m_activator = p_object;
	if (m_activator->HasObject(m_objectType) == 0) {
		RequestAction(ACTION_0x1a);
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041d5a0
void CKey::DoActivate()
{
	m_activator->AddObject(m_objectType, this);
	SetSndEffect(SFX_KEYS);
	g_pAI->Score(10);
}

// FUNCTION: LEMBALL 0x0041d5d0
AiCoord CKey::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed - 0x8000;
	return AiCoord(x, y, z);
}
