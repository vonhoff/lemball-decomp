#include "CCrate.h"

#include "../../Control/Game/CGame.h"
#include "../../Control/Game/GameTime.h"
#include "../Navigation/CAi.h"

// FUNCTION: LEMBALL 0x0041c470
CCrate::CCrate(const AiCoord& p_position, CGlobalGameObject* p_contents, unsigned short p_contentsId)
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
	m_pendingAction = ACTION_0x18;
}

#include "../../Map/Base/CMap.h"
#include "../Managers/CObjectManager.h"
#include "AI/Base/AiCoord.h"
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
			case 25:
				SetSndEffect(SFX_SNATCH);
				break;
			case 26:
				TriggerContents();
				SetSndEffect(SFX_CRATEEXP);
				break;
			}
			m_pendingAction = m_action;
		}
		return 1;
	}
	switch (m_action) {
	case ACTION_0x19:
		if (m_unk0xd0 < g_dwGameTick) {
			TriggerContents();
			SetSndEffect(SFX_CRATEEXP);
			Action(ACTION_0x1a);
		}
		break;
	case ACTION_0x1a:
		if (m_unk0xd4 < g_dwGameTick) {
			Action(ACTION_0x18);
			m_heading = 0;
		}
		break;
	}
	return 1;
}

// FUNCTION: LEMBALL 0x0041ce50
bool CCrate::Activate(CGameObject* p_object)
{
	if (m_action == ACTION_0x18) {
		m_unk0xd0 = 16;
		m_unk0xd4 = 30;
		RequestAction(ACTION_0x19);
		return 1;
	}
	return 0;
}

// FUNCTION: LEMBALL 0x0041ce90
void CCrate::DoActivate()
{
	m_stateTimer = g_dwSimulationTimestamp;
	m_unk0xd0 += g_dwGameTick;
	m_unk0xd4 += g_dwGameTick;
	SetSndEffect(SFX_SNATCH);
	int score;
	switch (m_contentsType) {
	case 4:
		score = 100;
		break;
	case 21:
	case 22:
	case 23:
		score = 50;
		break;
	case 0xffff:
		score = 25;
		break;
	}
	g_pAI->Score(score);
}

// FUNCTION: LEMBALL 0x0041cf10
AiCoord CCrate::ActivatePosition()
{
	int x = m_position.m_xFixed;
	int y = m_position.m_yFixed;
	int z = m_position.m_zFixed;
	if (m_contentsType < 0x15) {
		goto default_position;
	}
	if (m_contentsType <= 0x17 || m_contentsType == 0xffff) {
		goto contents_position;
	}

default_position:
	return AiCoord(x - 0x30000, y - 0x8000, z);

contents_position:
	return AiCoord(x - 0x8000, y, z);
}
