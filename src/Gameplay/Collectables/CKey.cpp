#include "CKey.h"

#include "Map/CMap.h"
#include "Gameplay/Simulation/AIScoreConstants.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Gameplay/Objects/ObjectInteractionStates.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Application/SoundEffects.h"

enum {
	KEY_ACTIVATION_POSITION_X_OFFSET_FIXED = -8 * FIXED_POINT_ONE
};

// FUNCTION: LEMBALL 0x0041c5f0
int CKey::Usage()
{
	return GROUP_OBJECT_USAGE_SINGLE;
}

// FUNCTION: LEMBALL 0x0041d480
bool CKey::Process()
{
	enum {
	};

	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	CMap* map = g_pMap;
	int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		int localX = x & GROUND_BLOCK_PIXEL_MASK;
		int localY = y & GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(localX, localY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = z << FIXED_POINT_FRACTION_BITS;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_ACTIVATED) {
				SetSndEffect(SFX_KEYS);
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	if (m_action == ACTION_ACTIVATED) {
		Action(ACTION_READY);
		m_objectActive = 0;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0041d560
bool CKey::Activate(CGameObject* p_object)
{
	m_activator = p_object;
	if (m_activator->HasObject(m_objectType) == 0) {
		RequestAction(ACTION_ACTIVATED);
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041d5a0
void CKey::DoActivate()
{
	m_activator->AddObject(m_objectType, this);
	SetSndEffect(SFX_KEYS);
	g_pAI->Score(AI_SCORE_KEY_PICKUP_POINTS);
}

// FUNCTION: LEMBALL 0x0041d5d0
AICOORD CKey::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed + KEY_ACTIVATION_POSITION_X_OFFSET_FIXED;
	return AICOORD(x, y, z);
}
