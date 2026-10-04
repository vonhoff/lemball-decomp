#include "CCrate.h"

#include "Application/GameTime.h"
#include "Gameplay/Simulation/CAI.h"
#include "Gameplay/Objects/ObjectInteractionStates.h"

#include <stddef.h>

enum {
	CRATE_ACTIVATION_FIRST_PHASE_TICKS = 16,
	CRATE_ACTIVATION_SECOND_PHASE_TICKS = 30
};

// FUNCTION: LEMBALL 0x0041c470
CCrate::CCrate(const AICOORD& p_position, CGlobalGameObject* p_contents, unsigned short p_contentsId)
	: CBaseGlobalObject(p_position, OBJECT_CRATE)
{
	m_contentsId = p_contentsId;
	m_contents = p_contents;
	if (p_contents == NULL) {
		m_contentsType = OBJECT_INVALID;
	}
	else {
		m_contentsType = p_contents->m_objectType;
	}
}

// FUNCTION: LEMBALL 0x0041c530
int CCrate::Usage()
{
	return GROUP_OBJECT_USAGE_SINGLE;
}

inline CCrate::~CCrate()
{
	if (m_contents != NULL && m_contentsType != OBJECT_INVALID) {
		delete m_contents;
	}
}

// FUNCTION: LEMBALL 0x0041cca0
void CCrate::Restart()
{
	CBaseGlobalObject::Restart();
	m_pendingAction = ACTION_READY;
}

#include "Map/CMap.h"
#include "Gameplay/Objects/CObjectManager.h"
#include "Gameplay/Geometry/AICOORD.h"
#include "Gameplay/Objects/CBaseGlobalObject.h"
#include "Gameplay/Objects/CGlobalGameObject.h"
#include "Gameplay/Objects/ObjectActions.h"
#include "Map/CGround.h"
#include "Map/CGroundArray.h"
#include "Application/SoundEffects.h"

// FUNCTION: LEMBALL 0x0041ccc0
void CCrate::TriggerContents()
{
	if (m_contentsType != OBJECT_INVALID) {
		CGlobalGameObject* contents = m_contents;
		m_position.m_xFixed = contents->m_position.m_xFixed;
		m_position.m_yFixed = contents->m_position.m_yFixed;
		m_position.m_zFixed = contents->m_position.m_zFixed;
		g_pObjectManager->AddObject(INVALID_OBJECT_ID, contents, 0);
		m_contentsType = OBJECT_INVALID;
	}
}

// FUNCTION: LEMBALL 0x0041cd20
bool CCrate::Process()
{
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	CMap* map = g_pMap;
	int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & GROUND_BLOCK_PIXEL_MASK;
		int cellY = y & GROUND_BLOCK_PIXEL_MASK;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = (unsigned int) z << FIXED_POINT_FRACTION_BITS;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			switch (m_action) {
			case ACTION_ACTIVATING:
				SetSndEffect(SFX_SNATCH);
				break;
			case ACTION_ACTIVATED:
				TriggerContents();
				SetSndEffect(SFX_CRATEEXP);
				break;
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	switch (m_action) {
	case ACTION_ACTIVATING:
		if (m_actionPhase1Deadline < g_dwGameTick) {
			TriggerContents();
			SetSndEffect(SFX_CRATEEXP);
			Action(ACTION_ACTIVATED);
		}
		break;
	case ACTION_ACTIVATED:
		if (m_actionPhase2Deadline < g_dwGameTick) {
			Action(ACTION_READY);
			m_objectActive = 0;
		}
		break;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0041ce50
bool CCrate::Activate(CGameObject* p_object)
{
	if (m_action == ACTION_READY) {
		m_actionPhase1Deadline = CRATE_ACTIVATION_FIRST_PHASE_TICKS;
		m_actionPhase2Deadline = CRATE_ACTIVATION_SECOND_PHASE_TICKS;
		RequestAction(ACTION_ACTIVATING);
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041ce90
void CCrate::DoActivate()
{
	enum {
		CRATE_CATAPULT_CONTENT_REWARD_POINTS = 100,
		CRATE_KEY_CONTENT_REWARD_POINTS = 50,
		CRATE_EMPTY_CONTENT_REWARD_POINTS = 25
	};
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionPhase1Deadline += g_dwGameTick;
	m_actionPhase2Deadline += g_dwGameTick;
	SetSndEffect(SFX_SNATCH);
	int score;
	switch (m_contentsType) {
	case OBJECT_CATAPULT:
		score = CRATE_CATAPULT_CONTENT_REWARD_POINTS;
		break;
	case OBJECT_KEY_1:
	case OBJECT_KEY_2:
	case OBJECT_KEY_3:
		score = CRATE_KEY_CONTENT_REWARD_POINTS;
		break;
	case OBJECT_INVALID:
		score = CRATE_EMPTY_CONTENT_REWARD_POINTS;
		break;
	}
	g_pAI->Score(score);
}

// FUNCTION: LEMBALL 0x0041cf10
AICOORD CCrate::ActivatePosition()
{
	enum {
		DEFAULT_X_OFFSET = 48 << FIXED_POINT_FRACTION_BITS,
		DEFAULT_Y_OFFSET = 8 << FIXED_POINT_FRACTION_BITS,
		CONTENTS_X_OFFSET = 8 << FIXED_POINT_FRACTION_BITS
	};
	int x = m_position.m_xFixed;
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	if (m_contentsType < OBJECT_KEY_1) {
		goto default_position;
	}
	if (m_contentsType <= OBJECT_KEY_3 || m_contentsType == OBJECT_INVALID) {
		goto contents_position;
	}

default_position:
	return AICOORD(x - DEFAULT_X_OFFSET, y - DEFAULT_Y_OFFSET, z);

contents_position:
	return AICOORD(x - CONTENTS_X_OFFSET, y, z);
}
