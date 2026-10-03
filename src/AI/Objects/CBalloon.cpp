#include "CBalloon.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/CBaseGlobalObject.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
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
	int randVal = (*g_pRandomSeed * 0x29 + 0x1f) & 0x7fffff;
	*g_pRandomSeed = randVal;
	m_stateTimer = g_dwSimulationTimestamp - (randVal % 4096);
}

// FUNCTION: LEMBALL 0x0041d740
bool CBalloon::Activate(CGameObject* p_object)
{
	m_activator = p_object;
	if (m_activator->HasObject(m_objectType) == 0) {
		RequestAction(ACTION_ACTIVATED);
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
AICOORD CBalloon::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	return AICOORD(m_position.m_xFixed, y, z);
}
