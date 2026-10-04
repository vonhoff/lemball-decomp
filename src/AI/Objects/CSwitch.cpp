#include "CSwitch.h"

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
#include "AI/Base/ObjectTypes.h"
#include "AI/Objects/SwitchEntry.h"
#include "CViewData.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// GLOBAL: LEMBALL 0x0049e1b8
unsigned short g_wNextSwitchIndex;

enum eLegacySwitchType {
	LEGACY_SWITCH_SINGLE_LIFT = 1,
	LEGACY_SWITCH_LIFT_RANGE = 2,
	LEGACY_SWITCH_DOOR = 3
};

enum {
	SWITCH_ACTIVATION_ANIMATION_DURATION_TICKS = 20,
	SWITCH_ACTIVATION_POSITION_X_OFFSET_FIXED = -8 * FIXED_POINT_ONE
};

// FUNCTION: LEMBALL 0x0041d040
CSwitch::CSwitch(AICOORD& p_position, swMessage p_legacyType, int p_legacyFirst, int p_legacyLast, int p_legacyAux)
	: CBaseGlobalObject(p_position, OBJECT_SWITCH)
{
	m_position.m_xFixed = p_position.m_xFixed;
	m_position.m_yFixed = p_position.m_yFixed;
	m_position.m_zFixed = p_position.m_zFixed;
	m_legacyType = p_legacyType;
	m_legacyFirst = p_legacyFirst;
	m_legacyLast = p_legacyLast;
	m_legacyAux = p_legacyAux;
	m_switchId = g_wNextSwitchIndex++;
}

// FUNCTION: LEMBALL 0x0041d100
void CSwitch::Restart()
{
	CBaseGlobalObject::Restart();
	m_entryCount = 0;
	m_scoreAwarded = 0;
	m_actionArgument = SWITCH_STATE_INACTIVE;
}

// FUNCTION: LEMBALL 0x0041d120
CSwitch::~CSwitch()
{
}

// FUNCTION: LEMBALL 0x0041d130
void CSwitch::Throw()
{
	SwitchEntry* entry;
	int i = 0;
	if (i < m_entryCount) {
		entry = m_entries;
		do {
			g_pAI->SwitchMessage(entry->m_message, entry->m_objectId, 0, 0);
			entry++;
			i++;
		} while (i < m_entryCount);
	}
	SetSndEffect(SFX_SWITCH);
}

// FUNCTION: LEMBALL 0x0041d180
bool CSwitch::Process()
{
	int y = m_position.m_yFixed >> FIXED_POINT_FRACTION_BITS;
	int x = m_position.m_xFixed >> FIXED_POINT_FRACTION_BITS;
	CMap* map = g_pMap;
	int blockX = x >> GROUND_BLOCK_PIXEL_SHIFT;
	int blockY = y >> GROUND_BLOCK_PIXEL_SHIFT;
	unsigned short z;
	if (x < 0 || y < 0 || blockX >= g_pMap->m_ground.m_width || blockY >= g_pMap->m_ground.m_height) {
		z = 0;
	}
	else {
		x &= 15;
		y &= 15;
		z = map->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].GetZ(x, y);
	}
	m_position.m_zFixed = ((int) z) << FIXED_POINT_FRACTION_BITS;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_HIT) {
				SetSndEffect(SFX_SWITCH);
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	switch (m_action) {
	case ACTION_HIT:
		Throw();
		Action(ACTION_READY);
		break;
	case ACTION_ACTIVATING:
		break;
	case ACTION_ACTIVATED:
		if (m_actionPhase2Deadline < g_dwGameTick) {
			Action(ACTION_HIT);
		}
		break;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0041d280
bool CSwitch::Activate(CGameObject* p_object)
{
	switch ((unsigned short) m_actionArgument) {
	case SWITCH_STATE_INACTIVE:
		m_actionPhase2Deadline = SWITCH_ACTIVATION_ANIMATION_DURATION_TICKS;
		m_actionArgument = SWITCH_STATE_ACTIVE;
		RequestAction(ACTION_ACTIVATED);
		return true;
	case SWITCH_STATE_ACTIVE:
		m_actionPhase2Deadline = SWITCH_ACTIVATION_ANIMATION_DURATION_TICKS;
		m_actionArgument = SWITCH_STATE_INACTIVE;
		RequestAction(ACTION_ACTIVATED);
		return true;
	default:
		return true;
	}
}

// FUNCTION: LEMBALL 0x0041d2e0
void CSwitch::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionPhase2Deadline += g_dwGameTick;
	if (m_scoreAwarded == 0) {
		g_pAI->Score(AI_SCORE_SWITCH_ACTIVATION_POINTS);
		m_scoreAwarded = 1;
	}
}

// FUNCTION: LEMBALL 0x0041d320
AICOORD CSwitch::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed + SWITCH_ACTIVATION_POSITION_X_OFFSET_FIXED;
	return AICOORD(x, y, z);
}

// FUNCTION: LEMBALL 0x0041d350
void CSwitch::AddEntry(swMessage p_message, unsigned short p_objectId)
{
	if (m_entryCount < SWITCH_ENTRY_CAPACITY) {
		m_entries[m_entryCount].m_message = p_message;
		m_entries[m_entryCount].m_objectId = p_objectId;
		m_entryCount++;
	}
}

// FUNCTION: LEMBALL 0x0041d390
void CSwitch::ConvertVer0ToVer1()
{
	switch (m_legacyType) {
	case LEGACY_SWITCH_SINGLE_LIFT: {
		unsigned int liftId = g_pAI->LiftId(m_legacyFirst);
		AddEntry(SW_LIFT, liftId);
		break;
	}
	case LEGACY_SWITCH_LIFT_RANGE: {
		for (int i = m_legacyFirst; i < m_legacyLast; i++) {
			unsigned int liftId = g_pAI->LiftId(i);
			AddEntry(SW_LIFT, liftId);
		}
		break;
	}
	case LEGACY_SWITCH_DOOR: {
		unsigned int doorId = g_pAI->DoorId(m_legacyFirst);
		if (doorId != INVALID_OBJECT_ID) {
			AddEntry(SW_DOOR, doorId);
		}
		break;
	}
	}
}

// FUNCTION: LEMBALL 0x0041d430
unsigned char* CSwitch::Load(unsigned char*& p_data)
{
	unsigned short* data = (unsigned short*) p_data;
	unsigned short count = *data;
	p_data += sizeof(count);
	if (count != 0) {
		unsigned int remaining = count;
		do {
			unsigned short objectId;
			unsigned short* cursor = (unsigned short*) p_data;
			swMessage message = (swMessage) *cursor++;
			p_data = (unsigned char*) cursor;
			objectId = *cursor++;
			p_data = (unsigned char*) cursor;
			AddEntry(message, objectId);
			remaining--;
		} while (remaining != 0);
	}
	return p_data;
}

// FUNCTION: LEMBALL 0x0041dc40
int CSwitch::Usage()
{
	return GROUP_OBJECT_USAGE_SINGLE;
}

// FUNCTION: LEMBALL 0x0041dc50
void CSwitch::GetViewData(CViewData& p_viewData)
{
	CGameObject::GetViewData(p_viewData);
	p_viewData.m_playerIndex = m_switchId;
}
