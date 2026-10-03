#include "CCrate.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../Navigation/CAI.h"

// FUNCTION: LEMBALL 0x0041c470
CCrate::CCrate(const AICOORD& p_position, CGlobalGameObject* p_contents, unsigned short p_contentsId)
	: CBaseGlobalObject(p_position, OBJECT_CRATE)
{
	m_contentsId = p_contentsId;
	m_contents = p_contents;
	if (p_contents == 0) {
		m_contentsType = OBJECT_INVALID;
	}
	else {
		m_contentsType = p_contents->m_objectType;
	}
}

// FUNCTION: LEMBALL 0x0041c530
int CCrate::Usage()
{
	return 2;
}

inline CCrate::~CCrate()
{
	if (m_contents != 0 && m_contentsType != OBJECT_INVALID) {
		delete m_contents;
	}
}

// FUNCTION: LEMBALL 0x0041cca0
void CCrate::Restart()
{
	CBaseGlobalObject::Restart();
	m_pendingAction = ACTION_READY;
}

#include "../../Map/Base/CMap.h"
#include "../Managers/CObjectManager.h"
#include "AI/Base/AICOORD.h"
#include "AI/Base/CBaseGlobalObject.h"
#include "AI/Base/CGlobalGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// FUNCTION: LEMBALL 0x0041ccc0
void CCrate::TriggerContents()
{
	if (m_contentsType != OBJECT_INVALID) {
		CGlobalGameObject* contents = (CGlobalGameObject*) m_contents;
		m_position.m_xFixed = contents->m_position.m_xFixed;
		m_position.m_yFixed = contents->m_position.m_yFixed;
		m_position.m_zFixed = contents->m_position.m_zFixed;
		g_pObjectManager->AddObject(0xffff, contents, 0);
		m_contentsType = OBJECT_INVALID;
	}
}

// FUNCTION: LEMBALL 0x0041cd20
bool CCrate::Process()
{
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	CMap* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x >= 0 && y >= 0 && blockX < map->m_ground.m_width && g_pMap->m_ground.m_height > blockY) {
		int cellX = x & 0xf;
		int cellY = y & 0xf;
		z = map->m_ground.m_ground[blockY * map->m_ground.m_width + blockX].GetZ(cellX, cellY);
	}
	else {
		z = 0;
	}
	m_position.m_zFixed = (unsigned int) z << 12;
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
		m_actionPhase1Deadline = 16;
		m_actionPhase2Deadline = 30;
		RequestAction(ACTION_ACTIVATING);
		return true;
	}
	return false;
}

// FUNCTION: LEMBALL 0x0041ce90
void CCrate::DoActivate()
{
	enum {
		CATAPULT_SCORE = 100,
		KEY_SCORE = 50,
		EMPTY_SCORE = 25
	};
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionPhase1Deadline += g_dwGameTick;
	m_actionPhase2Deadline += g_dwGameTick;
	SetSndEffect(SFX_SNATCH);
	int score;
	switch (m_contentsType) {
	case OBJECT_CATAPULT:
		score = CATAPULT_SCORE;
		break;
	case OBJECT_KEY_1:
	case OBJECT_KEY_2:
	case OBJECT_KEY_3:
		score = KEY_SCORE;
		break;
	case OBJECT_INVALID:
		score = EMPTY_SCORE;
		break;
	}
	g_pAI->Score(score);
}

// FUNCTION: LEMBALL 0x0041cf10
AICOORD CCrate::ActivatePosition()
{
	enum {
		DEFAULT_X_OFFSET = 48 << 12,
		DEFAULT_Y_OFFSET = 8 << 12,
		CONTENTS_X_OFFSET = 8 << 12
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
