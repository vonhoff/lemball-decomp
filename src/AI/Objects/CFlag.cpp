#include "CFlag.h"

#include "../Navigation/CAI.h"

// FUNCTION: LEMBALL 0x00422b30
void CFlag::SetSFX()
{
	SetSndEffect(SFX_YIPPEE);
}

#include "../../Map/Base/CMap.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Objects/CCollectable.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x00422b40
bool CFlag::Process()
{
	if (m_isRemoteObject == 0 && m_objectType == OBJECT_FLAG_1) {
		if (m_action == ACTION_0x18 && m_onMover == 0) {
			int y = m_position.m_yFixed >> 12;
			int x = m_position.m_xFixed >> 12;
			unsigned short z;
			int blockX;
			int blockY;
			CMap* map = g_pMap;
			blockX = x >> 4;
			blockY = y >> 4;

			if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
				int cellX = x & 0xf;
				int cellY = y & 0xf;
				z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
			}
			else {
				z = 0;
			}
			m_position.m_zFixed = (int) z << 12;
		}
		return 1;
	}
	return CCollectable::Process();
}

// FUNCTION: LEMBALL 0x00422c00
int CFlag::Collected()
{
	m_activator->AddObject(m_objectType, this);
	g_pAI->Score(2500);
	g_pAI->m_flagCounts[0]--;
	return 1;
}
