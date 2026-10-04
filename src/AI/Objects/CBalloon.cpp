#include "CBalloon.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../Base/AIScoreConstants.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/CBaseGlobalObject.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectInteractionStates.h"
#include "Views/Sound/SoundEffects.h"
#include "Visos/Foundation/RandomConstants.h"

enum {
	BALLOON_ANIMATION_PHASE_RANDOMIZATION_RANGE_MS = 4096
};
// FUNCTION: LEMBALL 0x0041c630
int CBalloon::Usage()
{
	return GROUP_OBJECT_USAGE_SINGLE;
}

// FUNCTION: LEMBALL 0x0041d600
void CBalloon::Restart()
{
	CBaseGlobalObject::Restart();
	int randVal = (*g_pRandomSeed * RANDOM_SEED_MULTIPLIER + RANDOM_SEED_INCREMENT) & RANDOM_SEED_MASK;
	*g_pRandomSeed = randVal;
	m_stateTimer = g_dwSimulationTimestamp - (randVal % BALLOON_ANIMATION_PHASE_RANDOMIZATION_RANGE_MS);
}

// FUNCTION: LEMBALL 0x0041d740
bool CBalloon::Activate(CGameObject* p_object)
{
	m_activator = p_object;
	if (m_activator->HasObject(m_objectType) == 0) {
		RequestAction(ACTION_ACTIVATED);
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041d780
void CBalloon::DoActivate()
{
	m_activator->AddObject(m_objectType, this);
	SetSndEffect(SFX_COLLECT_BALLOON);
	g_pAI->Score(AI_SCORE_BALLOON_PICKUP_POINTS);
}

// FUNCTION: LEMBALL 0x0041d7b0
AICOORD CBalloon::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	return AICOORD(m_position.m_xFixed, y, z);
}
