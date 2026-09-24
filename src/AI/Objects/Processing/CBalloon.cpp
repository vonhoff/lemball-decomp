#include "../CBalloon.h"

#include "../../../Control/Game/CGame.h"
#include "../../../Control/Game/GameTime.h"
#include "../../../Map/Base/CMap.h"
#include "../../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CBaseGlobalObject.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

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
		m_objectActive = 0;
		return 1;
	}
	return 1;
}
