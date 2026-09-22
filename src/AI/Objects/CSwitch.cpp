#include "CSwitch.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../../Map/Base/CMap.h"
#include "../Navigation/CAI.h"
#include "AI/Base/AiCoord.h"
#include "AI/Base/CBaseGlobalObject.h"
#include "AI/Base/CGameObject.h"
#include "AI/Base/ObjectActions.h"
#include "AI/Base/ObjectTypes.h"
#include "AI/Objects/SwitchEntry.h"
#include "CViewData.h"
#include "Map/Ground/CGround.h"
#include "Map/Ground/CGroundArray.h"
#include "Views/Sound/SoundEffects.h"

// GLOBAL: LEMBALL 0x0049e1b8
unsigned short g_wNextSwitchIndex;

// FUNCTION: LEMBALL 0x0041d040
CSwitch::CSwitch(AiCoord& p_arg0, swMessage p_arg1, int p_arg2, int p_arg3, int p_arg4)
	: CBaseGlobalObject(p_arg0, OBJECT_SWITCH)
{
	m_position.m_xFixed = p_arg0.m_xFixed;
	m_position.m_yFixed = p_arg0.m_yFixed;
	m_position.m_zFixed = p_arg0.m_zFixed;
	m_legacyType = p_arg1;
	m_legacyFirst = p_arg2;
	m_legacyLast = p_arg3;
	m_legacyAux = p_arg4;
	m_switchId = g_wNextSwitchIndex++;
}

// FUNCTION: LEMBALL 0x0041d100
void CSwitch::Restart()
{
	CBaseGlobalObject::Restart();
	m_entryCount = 0;
	m_scoreAwarded = 0;
	m_actionArgument = 0;
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
			g_pAI->SwitchMessage((swMessage) entry->m_message, entry->m_objectId, 0, 0);
			entry++;
			i++;
		} while (i < m_entryCount);
	}
	SetSndEffect(SFX_SWITCH);
}

// FUNCTION: LEMBALL 0x0041d180
bool CSwitch::Process()
{
	int y = m_position.m_yFixed >> 12;
	int x = m_position.m_xFixed >> 12;
	CMap* map = g_pMap;
	int blockX = x >> 4;
	int blockY = y >> 4;
	unsigned short z;
	if (x < 0 || y < 0 || blockX >= g_pMap->m_ground.m_width || blockY >= g_pMap->m_ground.m_height) {
		z = 0;
	}
	else {
		x &= 15;
		y &= 15;
		z = map->m_ground.m_ground[blockY * g_pMap->m_ground.m_width + blockX].GetZ(x, y);
	}
	m_position.m_zFixed = ((int) z) << 12;
	if (m_isRemoteObject != 0) {
		if (m_pendingAction != m_action) {
			if (m_action == ACTION_7) {
				SetSndEffect(SFX_SWITCH);
			}
			m_pendingAction = m_action;
		}
		return true;
	}
	switch (m_action) {
	case ACTION_7:
		Throw();
		Action(ACTION_0x18);
		break;
	case ACTION_0x19:
		break;
	case ACTION_0x1a:
		if (m_actionPhase2Deadline < g_dwGameTick) {
			Action(ACTION_7);
		}
		break;
	}
	return true;
}

// FUNCTION: LEMBALL 0x0041d280
bool CSwitch::Activate(CGameObject* p_object)
{
	switch ((unsigned short) m_actionArgument) {
	case 0:
		m_actionPhase2Deadline = 20;
		m_actionArgument = 1;
		RequestAction(ACTION_0x1a);
		return 1;
	case 1:
		m_actionPhase2Deadline = 20;
		m_actionArgument = 0;
		RequestAction(ACTION_0x1a);
		return 1;
	default:
		return 1;
	}
}

// FUNCTION: LEMBALL 0x0041d2e0
void CSwitch::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_actionPhase2Deadline += g_dwGameTick;
	if (m_scoreAwarded == 0) {
		g_pAI->Score(25);
		m_scoreAwarded = 1;
	}
}

// FUNCTION: LEMBALL 0x0041d320
AiCoord CSwitch::ActivatePosition()
{
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	int x = m_position.m_xFixed - 0x8000;
	return AiCoord(x, y, z);
}

// FUNCTION: LEMBALL 0x0041d350
void CSwitch::AddEntry(swMessage p_message, unsigned short p_objectId)
{
	if (m_entryCount < 0x20) {
		m_entries[m_entryCount].m_message = p_message;
		m_entries[m_entryCount].m_objectId = p_objectId;
		m_entryCount++;
	}
}

// FUNCTION: LEMBALL 0x0041d390
void CSwitch::ConvertVer0ToVer1()
{
	switch (m_legacyType) {
	case 1: {
		unsigned int liftId = g_pAI->LiftId(m_legacyFirst);
		AddEntry(SW_LIFT, liftId);
		break;
	}
	case 2: {
		for (int i = m_legacyFirst; i < m_legacyLast; i++) {
			unsigned int liftId = g_pAI->LiftId(i);
			AddEntry(SW_LIFT, liftId);
		}
		break;
	}
	case 3: {
		unsigned int doorId = g_pAI->DoorId(m_legacyFirst);
		if (doorId != 0xffff) {
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
	p_data += 2;
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
	return 2;
}

// FUNCTION: LEMBALL 0x0041dc50
void CSwitch::GetViewData(CViewData& p_viewData)
{
	CGameObject::GetViewData(p_viewData);
	p_viewData.m_playerIndex = m_switchId;
}
