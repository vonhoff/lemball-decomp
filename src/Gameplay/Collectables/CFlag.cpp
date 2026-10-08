#include "CFlag.h"

#include "Engine/Math/FixedPoint.h"
#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Simulation/CAI.h"

// FUNCTION: LEMBALL 0x00422b30
void CFlag::SetSFX()
{
	SetSndEffect(SFX_YIPPEE);
}

#include "Application/SoundEffects.h"
#include "CCollectable.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Map/CMap.h"

// FUNCTION: LEMBALL 0x00422b40
bool CFlag::Process()
{
	if (m_isRemoteObject == 0 && m_objectType == OBJECT_FLAG_1) {
		if (m_action == ACTION_READY && m_onMover == 0) {
			int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
			int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
			unsigned short z;
			int blockX;
			int blockY;
			CMap* map = g_pMap;
			blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
			blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;

			if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
				int cellX = x & GROUND_BLOCK_PIXEL_MASK;
				int cellY = y & GROUND_BLOCK_PIXEL_MASK;
				z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
			}
			else {
				z = 0;
			}
			const int height = (int) z << FIXED_POINT_FRACTION_BITS;
			m_position.m_zFixed = height;
		}
		return true;
	}
	return CCollectable::Process();
}

// FUNCTION: LEMBALL 0x00422c00
int CFlag::Collected()
{
	m_activator->AddObject(m_objectType, this);
	g_pAI->Score(AI_SCORE_FLAG_PICKUP_POINTS);
	g_pAI->m_flagCounts[0]--;
	return 1;
}
